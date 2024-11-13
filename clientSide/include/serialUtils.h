#ifndef SERIALUTILS_H
#define SERIALUTILS_H

#include <cstdint>

#include "clientCore.h"

const uint32_t BAUDRATE = 9600;
const uint32_t BUFF_SIZE = 512;
extern const char *READY_MESSAGE;

/**
 * @brief Waits for the server to indicate readiness.
 * 
 * @param port The serial port to read from.
 */
void waitServerReady(sp_port *port);

/**
 * @brief Serializes the game state into a string for communication.
 * 
 * @param req_cmd The command to serialize with the state.
 * @param gameState The game state to serialize.
 * @return A serialized string representing the game state.
 */
std::string serializeGameState(REQUEST_CMD req_cmd, gamestate_t *gameState);

/**
 * @brief Serializes a move into a string for communication.
 * 
 * @param req_cmd The command to serialize with the move.
 * @param move The move to serialize.
 * @return A serialized string representing the move.
 */
std::string serializeMove(REQUEST_CMD req_cmd, int *move);

/**
 * @brief Writes a message to the serial port.
 * 
 * @param port The serial port to write to.
 * @param message The message to send.
 */
void writeToSerial(sp_port *port, std::string message);

/**
 * @brief Reads a message from the serial port.
 * 
 * @param port The serial port to read from.
 * @return The received message as a string.
 */
std::string readFromSerial(sp_port *port);

/**
 * @brief Flushes the serial buffer.
 * 
 * @param port The serial port to flush.
 */
void flushSerialBuffer(sp_port *port);

#endif // SERIALUTILS_H