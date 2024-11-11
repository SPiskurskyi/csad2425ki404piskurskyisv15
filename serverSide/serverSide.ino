#include "include/serverCore.h"

const int BAUDRATE = 9600;
gamestate_t gameState;

void setup() {
    Serial.begin(BAUDRATE);
    while (!Serial) {
        ;
    }
    Serial.println("Arduino ready!");
}

void loop() {
    static String receivedMessage = "";
    
    if (Serial.available() > 0) {
        RESPONSE_CMD cmd_res;
        int position;
        
        String receivedMessage = Serial.readStringUntil('$');
        receivedMessage += '$';

        REQUEST_CMD cmd_req = deserializeCMD(receivedMessage);
        
        // Handle the request command
        switch (cmd_req) {
            case REQUEST_CMD::InformNewGame:
            case REQUEST_CMD::InformLoadGame:
                deserializeState(receivedMessage, gameState);
                cmd_res = RESPONSE_CMD::AcknowledgeGame;
                Serial.println(static_cast<int>(cmd_res));
                break;
            case REQUEST_CMD::RequestMove:
                position = deserializePosition(receivedMessage);
                gameState.board[position] = (gameState.current_player == 1) ? gameState.player_1 : gameState.player_2;
                
                if (checkWin(gameState.board)) {
                    cmd_res = RESPONSE_CMD::Winner;
                } else if (checkDraw(gameState)) {
                    cmd_res = RESPONSE_CMD::Draw;                   
                } else {
                    cmd_res = RESPONSE_CMD::NextMove;
                }
                
                gameState.current_player = (gameState.current_player == 1) ? 2 : 1;
                Serial.println(static_cast<int>(cmd_res));
                break;
        }
    }
}
