#include <iostream>
#include <string.h>
#include <unistd.h>
#include <libserialport.h>
#include <csignal>

#include "serialUtils.h"
#include "clientCore.h"

struct sp_port *port;
gamestate_t gameState;

void signalHandler(int signum);

int main() {
    signal(SIGINT, signalHandler);

    std::cout << "Opening port '" << SERIAL_PORT_PATH << "'\n";

    sp_return error = sp_get_port_by_name(SERIAL_PORT_PATH, &port);
    if (error != SP_OK) {
        std::cerr << "Error finding serial device\n";
        return -1;
    }

    error = sp_open(port, SP_MODE_READ_WRITE);
    if (error != SP_OK) {
        std::cerr << "Error opening serial device\n";
        return -2;
    }

    error = sp_set_baudrate(port, BAUDRATE);
      if (error != SP_OK) {
        std::cerr << "Error setting baudrate!\n";
        return -3;
    }

    flushSerialBuffer(port);
    waitServerReady(port);

    setup(&gameState);

    sp_close(port);
    return 0;
}

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";

    if (userInput("Save the game")) {
        std::cout << "Saving game...";
        saveGameState(&gameState);
    }

    if (port != nullptr)
        sp_close(port);

    exit(signum);
}