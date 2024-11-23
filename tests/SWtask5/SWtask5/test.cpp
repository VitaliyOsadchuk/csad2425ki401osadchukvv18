#include "pch.h"
#include "test.h"
#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <map>

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#define UNICODE


namespace {
    // Test for intToString function
    TEST(GameTest, IntToString) {
        EXPECT_EQ(intToString(0), "0");
        EXPECT_EQ(intToString(1), "1");
        EXPECT_EQ(intToString(-1), "-1");
        EXPECT_EQ(intToString(12345), "12345");
    }

    // Test for moveToString function
    TEST(GameTest, moveToString) {
        EXPECT_EQ(moveToString(1), "Rock");
        EXPECT_EQ(moveToString(2), "Paper");
        EXPECT_EQ(moveToString(3), "Scissors");
        EXPECT_EQ(moveToString(4), "Unknown");  // Invalid input
    }

    // Test for reading scores from file (mocked file)
    TEST(GameTest, ReadScoresFromFile) {
        std::map<std::string, std::pair<std::string, std::string>> expected = {
            {"PC vs PC", {"2:1", "2:3"}},
            {"Player vs PC", {"2:1", "2:1"}},
            {"Player vs Player", {"3:1", "3:2"}}
        };

        // Create a test file with scores
        std::ofstream testFile("results.ini");
        ASSERT_TRUE(testFile.is_open()) << "Failed to open results.ini for writing.";
        testFile << "Mode : PC vs PC;\nHighest score: 2:1\nPrevious score: 2:3\n\n";
        testFile << "Mode : Player vs PC;\nHighest score: 2:1\nPrevious score: 2:1\n\n";
        testFile << "Mode : Player vs Player;\nHighest score: 3:1\nPrevious score: 3:2\n";
        testFile.close();

        auto scores = readScoresFromFile();
        EXPECT_EQ(scores["PC vs PC"], std::make_pair("2:1", "2:3"));
        EXPECT_EQ(scores["Player vs PC"], std::make_pair("2:1", "2:1"));
        EXPECT_EQ(scores["Player vs Player"], std::make_pair("3:1", "3:2"));

        // Clean up the test file
        std::remove("results.ini");
    }

    // Test for writing scores to file
    TEST(GameTest, WriteScoresToFile) {
        std::map<std::string, std::pair<std::string, std::string>> scores = {
            {"PC vs PC", {"3:1", "2:3"}},
            {"Player vs PC", {"3:1", "2:1"}},
            {"Player vs Player", {"3:1", "3:2"}}
        };

        writeScoresToFile(scores);

        // Read the file to check if data was written correctly
        std::ifstream testFile("results.ini");
        std::string content((std::istreambuf_iterator<char>(testFile)), std::istreambuf_iterator<char>());

        EXPECT_NE(content.find("Mode : PC vs PC;"), std::string::npos);
        EXPECT_NE(content.find("Highest score: 3:1"), std::string::npos);
        EXPECT_NE(content.find("Previous score: 2:3"), std::string::npos);

        std::remove("results.ini");
    }

    // Test for updateScores function
    TEST(GameTest, UpdateScores) {
        int pl1Score = 0;
        int pl2Score = 0;

        updateScores("Player1 wins", pl1Score, pl2Score);
        EXPECT_EQ(pl1Score, 1);
        EXPECT_EQ(pl2Score, 0);

        updateScores("Player2 wins", pl1Score, pl2Score);
        EXPECT_EQ(pl1Score, 1);
        EXPECT_EQ(pl2Score, 1);

        updateScores("Draw", pl1Score, pl2Score);
        EXPECT_EQ(pl1Score, 1);
        EXPECT_EQ(pl2Score, 1);
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}