#include <iostream>
#include <string.h>
#include <fstream>
#include <limits>

#include "clientCore.h"
#include "cfgFile.h"
#include "serialUtils.h"

const char* CFG_FILE_NAME = "configuration.ini";

static bool checkWin(char player, const char board[10]);

void sendRequest(REQUEST_CMD req_cmd, void *data) {
    std::string message = "";
    gamestate_t* gameSate = nullptr;
    int *move = nullptr;

    switch (req_cmd) {
        case REQUEST_CMD::InformNewGame:
        case REQUEST_CMD::InformLoadGame:
            gameSate = static_cast <gamestate_t*> (data);
            message = serializeGameState(req_cmd, gameSate);
            break;

        case REQUEST_CMD::RequestMove:
            move = static_cast <int*> (data);
            message = serializeMove(req_cmd, move);
            break;
    }

    writeToSerial(port, message);
}

FINISHED processResponse(gamestate_t *gameState) {
    std::string message = readFromSerial(port);
    RESPONSE_CMD res_cmd = static_cast<RESPONSE_CMD>(std::stoi(message));

    switch (res_cmd) {
        case RESPONSE_CMD::AcknowledgeGame:
            std::cout << "Acknowledgment of game start received from hardware." << std::endl;
            break;
        case RESPONSE_CMD::NextMove:
            break;
        case RESPONSE_CMD::Winner:
            return FINISHED::WINNER;
        case RESPONSE_CMD::Draw:
            return FINISHED::DRAW;
        default:
            std::cout << "unrecognized response type..." << std::endl;
    }
    return FINISHED::NOT_YET;
}

void setup(gamestate_t *gameState) {
    bool newGame;
    
    std::cout << "Starting Tic-Tac-Toe game..." << std::endl;

    do {
        newGame = userInput("Start a new game");

        if (newGame) {
            loadGameConfiguration(gameState);
            sendRequest(REQUEST_CMD::InformNewGame, gameState);
        } else {
            loadGameState(gameState);
            sendRequest(REQUEST_CMD::InformLoadGame, gameState);
        }
        
        processResponse(gameState); // receive acknowledgement

        gameLoop(gameState);  
        break;

    } while (true);
}

void gameLoop(gamestate_t *gameState) {
    int move = -1;
    FINISHED finished = FINISHED::NOT_YET;
    char curr_player = '-';

    displayBoard(gameState->board);
    
    while (true) {
        makeMove(gameState);
        finished = processResponse(gameState);
        displayBoard(gameState->board);
        if (finished == FINISHED::DRAW) {
            std::cout << "GAME DRAW!" << std::endl;
            break;
        } else if (finished == FINISHED::WINNER) {
            curr_player = (gameState->current_player == 1) ? gameState->player_1 : gameState->player_2;
            std::cout << "WE HAVE A WINNER - " << curr_player << " !!!" << std::endl;
            break;
        } else { // not finished yet
            gameState->current_player = (gameState->current_player == 1) ? 2 : 1;
        }
    }
}

void makeMove(GameState *gameState) {
    int move = -1;

    switch (gameState->mode) {
        case GameMode::ManVsAI:
            if (gameState->current_player == 1) {
                move = humanMove(gameState); // human move
            } else {
                move = aiMove(gameState); // AI move
            }
            break;

        case GameMode::ManVsMan:
            move = humanMove(gameState); // Both players are human
            break;

        case GameMode::AIVsAI:
            move = aiMove(gameState); // Both players are AI
            break;
    }
    sendRequest(REQUEST_CMD::RequestMove, &move);
}

int humanMove(GameState *gameState) {
    int position;
    bool validMove = false;
    char curr_player = (gameState->current_player == 1) ? gameState->player_1 : gameState->player_2;

    while (!validMove) {
        std::cout << "Player " << gameState->current_player << "(" << curr_player << ") enter your move (1-9): ";
        
        if (!(std::cin >> position)) {
            std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (position >= 1 && position <= 9 && 
            gameState->board[position - 1] != gameState->player_1 && 
            gameState->board[position - 1] != gameState->player_2) {
            gameState->board[position - 1] = (gameState->current_player == 1) ? gameState->player_1 : gameState->player_2;
            validMove = true; // Move is valid, exit the loop
        } else {
            std::cout << "Invalid move. Try again.\n";
        }
    }
    return position - 1;
}

int aiMove(GameState *gameState) {
    int position = -1;
    char ai_player = (gameState->current_player == 1) ? gameState->player_1 : gameState->player_2;
    char opponent = (ai_player == gameState->player_1) ? gameState->player_2 : gameState->player_1;

    // Step 1: Check for winning move
    for (int i = 0; i < 9; ++i) {
        if (gameState->board[i] != gameState->player_1 && gameState->board[i] != gameState->player_2) {
            gameState->board[i] = ai_player;
            if (checkWin(ai_player, gameState->board)) {
                position = i;
                break;
            }
            gameState->board[i] = '1' + i; // revert temporary move
        }
    }

    // Step 2: Check for blocking opponent’s winning move
    if (position == -1) {
        for (int i = 0; i < 9; ++i) {
            if (gameState->board[i] != gameState->player_1 && gameState->board[i] != gameState->player_2) {
                gameState->board[i] = opponent;
                if (checkWin(opponent, gameState->board)) {
                    position = i;
                    break;
                }
                gameState->board[i] = '1' + i; // revert temporary move
            }
        }
    }

    // Step 3: If no winning or blocking move, pick a random position
    if (position == -1) {
        do {
            position = rand() % 9;
        } while (gameState->board[position] == gameState->player_1 || gameState->board[position] == gameState->player_2);
    }

    gameState->board[position] = ai_player;
    std::cout << "AI (Player " << gameState->current_player << "(" << ai_player << ")) chose position " << position + 1 << "\n";

    return position;
}

bool loadGameConfiguration(gamestate_t *gameState) {
    std::unordered_map<std::string, std::string> config;
    if (!parseConfigFile(CFG_FILE_NAME, config)) {
        std::cerr << "Failed to load configuration" << std::endl;
        return false;
    }

    int mode = std::stoi(config["GameConfig.mode"]);
    gameState->mode = static_cast<GameMode>(mode);

    gameState->current_player = 1; // default

    strcpy(gameState->board, "123456789"); //default

    gameState->player_1 = config["GameConfig.player_1"][0];
    gameState->player_2 = config["GameConfig.player_2"][0];

    std::cout << "Game configuration loaded successfully.\n";
    return true;
}

bool loadGameState(gamestate_t *gameState) {
    std::unordered_map<std::string, std::string> config;
    if (!parseConfigFile(CFG_FILE_NAME, config)) {
        std::cerr << "Failed to load game state" << std::endl;
        return false;
    }

    int mode = std::stoi(config["GameState.mode"]);
    gameState->mode = static_cast<GameMode>(mode);

    gameState->current_player = (config["GameState.current_turn"] == "player_1") ? 1 : 2;

    std::string boardStr = config["GameState.board"];
    for (size_t i = 0; i < BOARD_SIZE; ++i) {
        gameState->board[i] = boardStr[i];
    }

    gameState->player_1 = config["GameState.player_1"][0];
    gameState->player_2 = config["GameState.player_2"][0];

    std::cout << "Game state loaded successfully.\n";
    return true;
}

void saveGameState(gamestate_t *gameState) {

    // Step 1: Create the updated configuration map for the game state
    std::unordered_map<std::string, std::string> config;
    config["GameState.mode"] = std::to_string(static_cast<int>(gameState->mode));
    config["GameState.current_turn"] = (gameState->current_player == 1) ? "player_1" : "player_2";

    // Save the board as a string
    std::string boardStr(gameState->board, gameState->board + BOARD_SIZE);
    config["GameState.board"] = boardStr;

    config["GameState.player_1"] = std::string(1, gameState->player_1);
    config["GameState.player_2"] = std::string(1, gameState->player_2);

    // Step 2: Write the updated configuration back to the file
    if (!writeConfigFile(CFG_FILE_NAME, config)) {
        std::cerr << "Failed to save game state.\n";
        return;
    }

    std::cout << "Game state saved successfully.\n";
    return;
}

void displayBoard(const char board[BOARD_SIZE]) {
    std::cout << "-------------\n";
    
    for (size_t i = 0; i < BOARD_SIZE; ++i) {
        std::cout << " " << board[i] << " ";
        
        if ((i + 1) % 3 != 0) {
            std::cout << "|";
        } else {
            std::cout << "\n";
            if (i < 8) {
                std::cout << "-------------\n";
            }
        }
    }
    
    std::cout << "-------------\n";
}

bool userInput(std::string message) {
    char choice;
    while (true) {
        std::cout << message << "? (y/n): ";
        std::cin >> choice;

        choice = std::tolower(choice);

        if (choice == 'y') {
            return true;
        } else if (choice == 'n') {
            return false;
        } else {
            std::cout << "Invalid input. Please enter 'y' or 'n'." << std::endl;
        }
    }
}

static bool checkWin(char player, const char board[10]) {
    return (
        (board[0] == player && board[1] == player && board[2] == player) ||
        (board[3] == player && board[4] == player && board[5] == player) ||
        (board[6] == player && board[7] == player && board[8] == player) ||
        (board[0] == player && board[3] == player && board[6] == player) ||
        (board[1] == player && board[4] == player && board[7] == player) ||
        (board[2] == player && board[5] == player && board[8] == player) ||
        (board[0] == player && board[4] == player && board[8] == player) ||
        (board[2] == player && board[4] == player && board[6] == player)
    );
}