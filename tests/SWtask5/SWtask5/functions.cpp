#include "pch.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include <map>

enum Move { ROCK = 1, PAPER, SCISSORS };

std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string moveToString(int move) {
    switch (move) {
    case ROCK: return "Rock";
    case PAPER: return "Paper";
    case SCISSORS: return "Scissors";
    default: return "Unknown";
    }
}

std::map<std::string, std::pair<std::string, std::string>> readScoresFromFile() {
    std::ifstream resultsFile("results.ini");
    std::map<std::string, std::pair<std::string, std::string>> scores;
    std::string line, mode, highScore, previousScore;

    if (resultsFile.is_open()) {
        while (std::getline(resultsFile, line)) {
            if (line.find("Mode :") != std::string::npos) {
                mode = line.substr(7, line.find(';') - 7);
            }
            else if (line.find("Highest score:") != std::string::npos) {
                highScore = line.substr(line.find(": ") + 2);
            }
            else if (line.find("Previous score:") != std::string::npos) {
                previousScore = line.substr(line.find(": ") + 2);
                scores[mode] = { highScore, previousScore };
            }
        }
        resultsFile.close();
    }
    else {
        std::cerr << "Unable to open results.ini for reading." << std::endl;
    }
    return scores;
}

void writeScoresToFile(const std::map<std::string, std::pair<std::string, std::string>>& scores) {
    std::ofstream resultsFile("results.ini");
    if (resultsFile.is_open()) {
        for (const auto& entry : scores) {
            resultsFile << "Mode : " << entry.first << ";\n";
            resultsFile << "Highest score: " << entry.second.first << "\n";
            resultsFile << "Previous score: " << entry.second.second << "\n\n";
        }
        resultsFile.close();
    }
    else {
        std::cerr << "Unable to open results.ini for writing." << std::endl;
    }
}

HANDLE openSerialPort(const std::string& portName) {
    HANDLE hSerial = CreateFileA(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
    }
    return hSerial;
}

void sendMovesToSerialPort(HANDLE hSerial, int pl1Move, int pl2Move) {
    std::string message = intToString(pl1Move) + "," + intToString(pl2Move) + "\n";
    DWORD bytesWritten;
    WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, NULL);
}

std::string getSerialResponse(HANDLE hSerial) {
    char buffer[256];
    DWORD bytesRead;
    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            return std::string(buffer);
        }
    }
    std::cerr << "Failed to read from serial port." << std::endl;
    return "";
}

void updateScores(const std::string& result, int& pl1Score, int& pl2Score) {
    if (result.find("Player1 wins") != std::string::npos) {
        pl1Score++;
    }
    else if (result.find("Player2 wins") != std::string::npos) {
        pl2Score++;
    }
}