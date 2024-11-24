#include "../include/serverCore.h"
#include "../src/serverCore.cpp"

int passedTests = 0;
int failedTests = 0;

void test_deserializeCMD_InformNewGame() {
    String message = "100_$";
    int result = (int)deserializeCMD(message);
    if (result == (int)REQUEST_CMD::InformNewGame) {
        passedTests++;
        Serial.println("[V]Test deserializeCMD_InformNewGame Passed");
    } else {
        failedTests++;
        Serial.print("[X]Test deserializeCMD_InformNewGame Failed. Result: ");
        Serial.println(result);
    }
}

void test_deserializeCMD_Unknown() {
    String message = "999_$";
    int result = (int)deserializeCMD(message);
    if (result == (int)REQUEST_CMD::Unknown) {
        passedTests++;
        Serial.println("[V]Test deserializeCMD_Unknown Passed");
    } else {
        failedTests++;
        Serial.print("[X]Test deserializeCMD_Unknown Failed. Result: ");
        Serial.println(result);
    }
}

void test_deserializePosition_ValidInput() {
    String message = "100_4$";
    int result = deserializePosition(message);
    if (result == 4) {
        passedTests++;
        Serial.println("[V]Test deserializePosition_ValidInput Passed");
    } else {
        failedTests++;
        Serial.print("[X]Test deserializePosition_ValidInput Failed. Result: ");
        Serial.println(result);
    }
}

void test_deserializePosition_InvalidInput() {
    String message = "100_$";
    int result = deserializePosition(message);
    if (result == 0) {  // Expect default (or invalid) position
        passedTests++;
        Serial.println("[V]Test deserializePosition_InvalidInput Passed");
    } else {
        failedTests++;
        Serial.print("[X]Test deserializePosition_InvalidInput Failed. Result: ");
        Serial.println(result);
    }
}

void test_deserializeState_CorrectParsing() {
    String message = "100_0_X_O_1_XXXXXXXXX$";
    gamestate_t state;
    deserializeState(message, state);

    if (state.mode == GameMode::ManVsAI &&
        state.player_1 == 'X' &&
        state.player_2 == 'O' &&
        state.current_player == 1 &&
        String(state.board) == String("XXXXXXXXX")) {
        passedTests++;
        Serial.println("[V]Test deserializeState_CorrectParsing Passed");
    } else {
        failedTests++;
        Serial.println("[X]Test deserializeState_CorrectParsing Failed");
    }
}

void test_checkWin_WinningBoard() {
    char board[10] = "XXX......";
    bool result = checkWin(board);
    if (result) {
        passedTests++;
        Serial.println("[V]Test checkWin_WinningBoard Passed");
    } else {
        failedTests++;
        Serial.println("[X]Test checkWin_WinningBoard Failed");
    }
}

void test_checkWin_NoWinningBoard() {
    char board[10] = "XXOOOXXOX";
    bool result = checkWin(board);
    if (!result) {
        passedTests++;
        Serial.println("[V]Test checkWin_NoWinningBoard Passed");
    } else {
        failedTests++;
        Serial.println("[X]Test checkWin_NoWinningBoard Failed");
    }
}

void test_checkDraw_CompleteBoard() {
    gamestate_t state = {GameMode::ManVsMan, 1, "XXOOOXXOX", 'X', 'O'};
    bool result = checkDraw(state);
    if (result) {
        passedTests++;
        Serial.println("[V]Test checkDraw_CompleteBoard Passed");
    } else {
        failedTests++;
        Serial.println("[X]Test checkDraw_CompleteBoard Failed");
    }
}

void test_checkDraw_IncompleteBoard() {
    gamestate_t state = {GameMode::ManVsMan, 1, "XOXOXOX..", 'X', 'O'};
    bool result = checkDraw(state);
    if (!result) {
        passedTests++;
        Serial.println("[V]Test checkDraw_IncompleteBoard Passed");
    } else {
        failedTests++;
        Serial.println("[X]Test checkDraw_IncompleteBoard Failed");
    }
}

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    Serial.println("Running tests...");
    delay(100);
    
    test_deserializeCMD_InformNewGame();
    test_deserializeCMD_Unknown();
    test_deserializePosition_ValidInput();
    test_deserializePosition_InvalidInput();
    test_deserializeState_CorrectParsing();
    test_checkWin_WinningBoard();
    test_checkWin_NoWinningBoard();
    test_checkDraw_CompleteBoard();
    test_checkDraw_IncompleteBoard();

    Serial.print("Tests Passed: ");
    Serial.println(passedTests);
    Serial.print("Tests Failed: ");
    Serial.println(failedTests);

    Serial.println("Test complete.");
}

void loop() {
    // Nothing needed in loop for now
}
