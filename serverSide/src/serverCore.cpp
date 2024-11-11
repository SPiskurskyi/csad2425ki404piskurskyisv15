#include "../include/serverCore.h"

REQUEST_CMD deserializeCMD(const String &receivedMessage) {
    int delimiterPos = receivedMessage.indexOf('_');
    if (delimiterPos == -1) {
        Serial.println("Delimiter '_' for command not found.");
        return REQUEST_CMD::Unknown;
    }

    String cmdString = receivedMessage.substring(0, delimiterPos);
    return static_cast<REQUEST_CMD>(cmdString.toInt());
}

int deserializePosition(const String &receivedMessage) {
    // Find the position of the delimiter '_'
    int startIdx = receivedMessage.indexOf('_') + 1;  // Start after the command part
    String moveStr = receivedMessage.substring(startIdx, receivedMessage.indexOf('$')); // Get the pos (before '$')

    return moveStr.toInt(); // Convert the pos string to an integer and return
}

void deserializeState(const String &receivedMessage, gamestate_t &gameState) {
    int pos = 0;
    int nextPos;
    int fieldIdx = 0;

    int startIdx = receivedMessage.indexOf('_') + 1;  // Start after the command delimiter
    String remainder = receivedMessage.substring(startIdx, receivedMessage.indexOf('$')); // Remove the $ at the end

    while ((nextPos = remainder.indexOf('_', pos)) != -1 && fieldIdx < 5) {
        String field = remainder.substring(pos, nextPos);

        switch (fieldIdx) {
            case 0: // Mode
                gameState.mode = static_cast<GameMode>(field.toInt());
                break;
            case 1: // Player 1 symbol
                gameState.player_1 = field[0];
                break;
            case 2: // Player 2 symbol
                gameState.player_2 = field[0];
                break;
            case 3: // Current player
                gameState.current_player = field.toInt();
                break;
        }

        pos = nextPos + 1;
        ++fieldIdx;
    }

    // Handle the board part after all fields are processed
    String boardField = remainder.substring(pos);
    if (boardField.length() == 9) {
        boardField.toCharArray(gameState.board, 10);
    }
}

void flushSerial() {
    while (Serial.available() > 0) {
        Serial.read();
    }
}

bool checkWin(const char board[10]) {
    // Define winning combinations
    int wins[8][3] = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};
    for (auto& combo : wins) {
        if (board[combo[0]] == board[combo[1]] && board[combo[1]] == board[combo[2]])
            return true;
    }
    return false;
}

bool checkDraw(const gamestate_t &gameState) {
    for (int i = 0; i < 9; i++) {
        if (gameState.board[i] != gameState.player_1 && gameState.board[i] != gameState.player_2) return false;
    }
    return true;
}