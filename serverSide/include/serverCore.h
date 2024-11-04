#ifndef SERVERCORE_H
#define SERVERCORE_H

#include "Arduino.h"

enum class GameMode {
    ManVsAI = 0,
    ManVsMan,
    AIVsAI
};

struct GameState {
    GameMode mode;
    int current_player;
    char board[10];
    char player_1;
    char player_2;

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

REQUEST_CMD deserializeCMD(const String &receivedMessage);
int deserializePosition(const String &receivedMessage);
void deserializeState(const String &receivedMessage, gamestate_t &gameState);
void flushSerial();

bool checkWin(const char board[10]);
bool checkDraw(const gamestate_t &gameState);

#endif // SERVERCORE_H