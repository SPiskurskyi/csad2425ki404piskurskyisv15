#ifndef CLIENTCORE_H
#define CLIENTCORE_H

#define DEBUG

enum class GameMode {
    ManVsAI = 0,
    ManVsMan,
    AIVsAI
};

struct GameState {
    GameMode mode = GameMode::ManVsAI;
    int current_player = 1;
    char board[10] = "123456789";
    char player_1 = 'X';
    char player_2 = 'O';
    
} typedef gamestate_t;

enum class REQUEST_CMD {
    InformNewGame = 100,
    InformLoadGame,
    RequestMove,
    Unknown
};

enum class RESPONSE_CMD {
    AcknowledgeGame = 200,
    NextMove,
    Winner,
    Draw,
    Unknown
};

enum class FINISHED {
    NOT_YET = -1,
    DRAW,
    WINNER
};

const int BOARD_SIZE = 9;

void setup(gamestate_t *gameState);

void sendRequest(REQUEST_CMD req_cmd, void *data);
FINISHED processResponse(gamestate_t *gameState, int position);

bool loadGameConfiguration(gamestate_t *gameState);
bool loadGameState(gamestate_t *gameState);
void saveGameState(gamestate_t *gameState);

void gameLoop(gamestate_t *gameState);

int makeMove(GameState *gameState);
int humanMove(GameState *gameState);
int aiMove(GameState *gameState);

void displayBoard(const char board[BOARD_SIZE]);
bool userInput(std::string);

extern struct sp_port *port;

#endif // CLIENTCORE_H