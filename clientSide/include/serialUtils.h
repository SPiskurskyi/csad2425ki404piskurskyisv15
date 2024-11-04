#ifndef SERIALUTILS_H
#define SERIALUTILS_H

#include <cstdint>

#include "clientCore.h"

const uint32_t BAUDRATE = 9600;
const uint32_t BUFF_SIZE = 512;
extern const char *READY_MESSAGE;

void wait_for_arduino_ready(sp_port *port);

std::string serializeGameState(REQUEST_CMD req_cmd, gamestate_t *gameState);
std::string serializeMove(REQUEST_CMD req_cmd, int *move);

void write_to_serial(sp_port *port, std::string message);
std::string read_from_serial(sp_port *port);

void flushSerialBuffer(sp_port *port);

#endif // SERIALUTILS_H