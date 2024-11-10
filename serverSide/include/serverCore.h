#ifndef SERVERCORE_H
#define SERVERCORE_H

#include "Arduino.h"

/**
 * @enum GameMode
 * @brief Represents the different modes of the game.
 */
enum class GameMode {
    ManVsAI = 0, /**< Mode for player versus AI. */
    ManVsMan,    /**< Mode for player versus player. */
    AIVsAI       /**< Mode for AI versus AI. */
};

/**
 * @struct GameState
 * @brief Holds the state of the game.
 */
struct GameState {
    GameMode mode; /**< The current game mode. */
    int current_player; /**< The current player’s turn. */
    char board[10]; /**< The game board (1-9). */
    char player_1; /**< Symbol for player 1. */
    char player_2; /**< Symbol for player 2. */
} typedef gamestate_t;

/**
 * @enum REQUEST_CMD
 * @brief Command types for requests sent by the client.
 */
enum class REQUEST_CMD {
    InformNewGame = 100, /**< Command to inform about new game. */
    InformLoadGame,      /**< Command to inform about saved game. */
    RequestMove,         /**< Command to request a move. */
    Unknown              /**< Unknown command. */
};

/**
 * @enum RESPONSE_CMD
 * @brief Command types for responses from the server.
 */
enum class RESPONSE_CMD {
    AcknowledgeGame = 200, /**< Server acknowledges game start. */
    NextMove,              /**< Server indicates the next move. */
    Winner,                /**< Server declares a winner. */
    Draw,                  /**< Server declares a draw. */
    Unknown                /**< Unknown response. */
};


/**
 * @brief Deserializes a command from a string.
 * 
 * @param receivedMessage The received message string.
 * @return The deserialized command.
 */
REQUEST_CMD deserializeCMD(const String &receivedMessage);

/**
 * @brief Deserializes a move position from a string.
 * 
 * @param receivedMessage The received message string.
 * @return The deserialized position.
 */
int deserializePosition(const String &receivedMessage);

/**
 * @brief Deserializes the game state from a string.
 * 
 * @param receivedMessage The received message string.
 * @param gameState The game state to deserialize into.
 */
void deserializeState(const String &receivedMessage, gamestate_t &gameState);

/**
 * @brief Flushes the serial buffer. (UNUSED)
 */
void flushSerial();

/**
 * @brief Checks if there is a winning combination on the board.
 * 
 * @param board The game board array.
 * @return True if there is a win, false otherwise.
 */
bool checkWin(const char board[10]);

/**
 * @brief Checks if the game is a draw.
 * 
 * @param gameState The current game state.
 * @return True if the game is a draw, false otherwise.
 */
bool checkDraw(const gamestate_t &gameState);

#endif // SERVERCORE_H