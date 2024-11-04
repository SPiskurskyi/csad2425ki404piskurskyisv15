#include <iostream>
#include <libserialport.h>
#include <string.h>
#include <sstream>

#include "serialUtils.h"

const int TIMEOUT_MS = 1000;
const char *READY_MESSAGE = "Arduino ready!";

std::string serializeGameState(REQUEST_CMD req_cmd, gamestate_t *gameState) {
    std::ostringstream oss;

    oss << static_cast<int>(req_cmd) << "_";
    oss << static_cast<int>(gameState->mode) << "_"
        << gameState->player_1 << "_"
        << gameState->player_2 << "_"
        << gameState->current_player << "_"
        << std::string(gameState->board, gameState->board + 9) << "$";

    return oss.str();
}

std::string serializeMove(REQUEST_CMD req_cmd, int *move) {
    std::ostringstream oss;
    oss << static_cast<int>(req_cmd) << '_';

    if (move == nullptr) {
        oss << "-1" << "$";
    } else {
        oss << *move << "$";
    }

    return oss.str();
}

void write_to_serial(sp_port *port, std::string message) {
    int result = sp_blocking_write(port, message.c_str(), message.length(), TIMEOUT_MS);

    if (result < 0) {
        const char* error_message = sp_last_error_message();
        std::cerr << "Error writing to serial port: " << error_message << std::endl;
        sp_free_error_message(const_cast<char*>(error_message)); // Cast away const for cleanup
        return;
    }
#ifdef DEBUG
    std::cout << "Message sent: " << message << std::endl;
#endif
}

std::string read_from_serial(sp_port *port) {
    char byte_buff[BUFF_SIZE];
    int byte_num;
    std::string received_data; 

    while (1) {
        int bytes_waiting = sp_input_waiting(port);
        if (bytes_waiting > 0) {
            byte_num = sp_blocking_read(port, byte_buff, sizeof(byte_buff), TIMEOUT_MS);
            byte_buff[byte_num] = '\0';
            
            received_data.append(byte_buff);
#ifdef DEBUG
            std::cout << "Response from Arduino: " << received_data << std::endl;
#endif            
            break;
        }
    }
    
    return received_data;
}

void wait_for_arduino_ready(sp_port *port) {
    char byte_buff[BUFF_SIZE];
    std::cout << "Waiting for Arduino to be ready...\n";

    while (1) {
        int bytes_waiting = sp_input_waiting(port);
        if (bytes_waiting > 0) {
            int byte_num = sp_blocking_read(port, byte_buff, sizeof(byte_buff), TIMEOUT_MS);
            byte_buff[byte_num] = '\0';  // Null-terminate the buffer

            if (strstr(byte_buff, READY_MESSAGE) != NULL) {
#ifdef DEBUG
                std::cout << "Received from Arduino: " << byte_buff << std::endl;
#endif
                break;
            }
        }
    }
}

void flushSerialBuffer(sp_port *port) {
    if (port == nullptr) {
        std::cerr << "Port is not initialized!" << std::endl;
        return;
    }

    while (sp_input_waiting(port) > 0) {
        char buffer[BUFF_SIZE];
        sp_nonblocking_read(port, buffer, sizeof(buffer));
    }
}