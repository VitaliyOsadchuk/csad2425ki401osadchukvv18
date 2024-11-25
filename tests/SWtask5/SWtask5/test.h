#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <sstream>
#include <fstream>
#include <map>


std::string intToString(int value);

std::string moveToString(int move);

std::map<std::string, std::pair<std::string, std::string>> readScoresFromFile();

void writeScoresToFile(const std::map<std::string, std::pair<std::string, std::string>>& scores);

HANDLE openSerialPort(const std::string& portName);

void sendMovesToSerialPort(HANDLE hSerial, int pl1Move, int pl2Move);

std::string getSerialResponse(HANDLE hSerial);

void updateScores(const std::string& result, int& pl1Score, int& pl2Score);