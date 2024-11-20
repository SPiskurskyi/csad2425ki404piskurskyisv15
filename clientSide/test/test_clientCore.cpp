#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <cstring>
#include <fstream>

#include "clientCore.h"
#include "serialUtils.h"

struct sp_port *port; //mock
extern const char* CFG_FILE_NAME;

using namespace testing;

TEST(AIMoveTest, WinningMoveSelected) {
    GameState gameState;
    gameState.current_player = 1;
    gameState.player_1 = 'X';
    gameState.player_2 = 'O';

    // Winning move setup
    strcpy(gameState.board, "XX3456789");

    int move = aiMove(&gameState);

    ASSERT_EQ(move, 2); // Winning move at index 2
    ASSERT_EQ(gameState.board[2], 'X'); // 'X' completes the win
}

TEST(AIMoveTest, BlocksOpponentWinningMove) {
    GameState gameState;
    gameState.current_player = 2;
    gameState.player_1 = 'X';
    gameState.player_2 = 'O';

    // Opponent can win
    strcpy(gameState.board, "XX3456789");

    int move = aiMove(&gameState);

    ASSERT_EQ(move, 2); // AI blocks opponent at index 2
    ASSERT_EQ(gameState.board[2], 'O'); // AI ('O') blocks 'X'
}

TEST(HumanMoveTest, ValidMove) {
    GameState gameState;
    gameState.current_player = 1;
    gameState.player_1 = 'X';
    strcpy(gameState.board, "123456789");

    std::istringstream input("1\n"); // Simulate input for position 1
    std::cin.rdbuf(input.rdbuf());

    int move = humanMove(&gameState);

    ASSERT_EQ(move, 0); // First position (0-indexed)
    ASSERT_EQ(gameState.board[0], 'X'); // 'X' marks position
}

TEST(DisplayBoardTest, CorrectOutput) {
    char board[BOARD_SIZE] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

    testing::internal::CaptureStdout();
    displayBoard(board);
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_NE(output.find(" 1 | 2 | 3 "), std::string::npos);
    ASSERT_NE(output.find(" 4 | 5 | 6 "), std::string::npos);
    ASSERT_NE(output.find(" 7 | 8 | 9 "), std::string::npos);
}
