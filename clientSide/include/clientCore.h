#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#define DEBUG

const int BOARD_SIZE = 9;

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
    GameMode mode = GameMode::ManVsAI; /**< The current game mode. */
    int current_player = 1; /**< The current player’s turn. */
    char board[10] = "123456789"; /**< The game board (1-9). */
    char player_1 = 'X'; /**< Symbol for player 1. */
    char player_2 = 'O'; /**< Symbol for player 2. */
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
 * @enum FINISHED
 * @brief Represents the game completion status.
 */
enum class FINISHED {
    NOT_YET = -1, /**< Game is not finished. */
    DRAW,         /**< Game is a draw. */
    WINNER        /**< Game has a winner. */
};

/**
 * @brief Initializes the game state.
 * 
 * @param gameState The game state structure to initialize.
 */
void setup(gamestate_t *gameState);

/**
 * @brief Sends a request command to the server.
 * 
 * @param req_cmd The command to send.
 * @param data Optional data associated with the command.
 */
void sendRequest(REQUEST_CMD req_cmd, void *data);

/**
 * @brief Processes the server's response.
 * 
 * @param gameState The current game state.
 * @return The game's finished status.
 */
FINISHED processResponse(gamestate_t *gameState);

/**
 * @brief Loads the game configuration from a file.
 * 
 * @param gameState The game state to load into.
 * @return True if successful, false otherwise.
 */
bool loadGameConfiguration(gamestate_t *gameState);

/**
 * @brief Loads the game state from a file.
 * 
 * @param gameState The game state to load into.
 * @return True if successful, false otherwise.
 */
bool loadGameState(gamestate_t *gameState);

/**
 * @brief Saves the current game state to a file.
 * 
 * @param gameState The game state to save.
 */
void saveGameState(gamestate_t *gameState);

/**
 * @brief Runs the main game loop.
 * 
 * @param gameState The current game state.
 */
void gameLoop(gamestate_t *gameState);

/**
 * @brief Makes a move based on the current player and game mode.
 * 
 * @param gameState The current game state.
 */
void makeMove(GameState *gameState);

/**
 * @brief Allows a human player to make a move.
 * 
 * @param gameState The current game state.
 * @return The position chosen by the human player.
 */
int humanMove(GameState *gameState);

/**
 * @brief Allows an AI player to make a move.
 * 
 * @param gameState The current game state.
 * @return The position chosen by the AI player.
 */
int aiMove(GameState *gameState);

/**
 * @brief Displays the game board.
 * 
 * @param board The game board array.
 */
void displayBoard(const char board[BOARD_SIZE]);

/**
 * @brief Gets input from the user.
 * 
 * @param prompt The prompt message to display.
 * @return True if input is received, false otherwise.
 */
bool userInput(std::string prompt);

extern struct sp_port *port;

#endif // CLIENTCORE_H