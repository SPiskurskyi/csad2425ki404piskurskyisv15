#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <fstream>
#include "cfgFile.h"

class ConfigFileTest : public ::testing::Test {
protected:
    std::string tempFileName;

    void SetUp() override {
        tempFileName = "test_config.cfg";
    }

    void TearDown() override {
        remove(tempFileName.c_str());
    }

    void writeTempFile(const std::string &content) {
        std::ofstream outFile(tempFileName);
        ASSERT_TRUE(outFile.is_open()) << "Failed to create temp file";
        outFile << content;
        outFile.close();
    }
};

TEST_F(ConfigFileTest, ParseValidConfigFile) {
    const std::string fileContent = R"(
        [GameConfig]
        mode=0
        player_1=X
        player_2=O

        [GameState]
        board=1O34X6789
        current_turn=player_1
        mode=0
    )";

    writeTempFile(fileContent);

    std::unordered_map<std::string, std::string> configMap;
    ASSERT_TRUE(parseConfigFile(tempFileName, configMap));

    EXPECT_EQ(configMap.size(), 6);
    EXPECT_EQ(configMap["GameConfig.mode"], "0");
    EXPECT_EQ(configMap["GameConfig.player_1"], "X");
    EXPECT_EQ(configMap["GameState.board"], "1O34X6789");
    EXPECT_EQ(configMap["GameState.current_turn"], "player_1");
}

TEST_F(ConfigFileTest, WriteConfigFileUpdatesSection) {
    const std::string initialContent = R"(
        [GameConfig]
        mode=0

        [GameState]
        board=1O34X6789
        current_turn=player_1
    )";

    writeTempFile(initialContent);

    std::unordered_map<std::string, std::string> configMap = {
        {"GameState.board", "XOXXOOOXX"},
        {"GameState.current_turn", "player_2"}
    };

    ASSERT_TRUE(writeConfigFile(tempFileName, configMap));

    std::ifstream inFile(tempFileName);
    ASSERT_TRUE(inFile.is_open()) << "Failed to open modified file";

    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    EXPECT_NE(fileContent.find("[GameState]"), std::string::npos);
    EXPECT_NE(fileContent.find("board=XOXXOOOXX"), std::string::npos);
    EXPECT_NE(fileContent.find("current_turn=player_2"), std::string::npos);
}
