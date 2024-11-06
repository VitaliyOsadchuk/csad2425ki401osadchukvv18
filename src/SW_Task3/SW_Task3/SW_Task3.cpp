#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <windows.h>
#include <sstream> // for std::stringstream
#include <fstream> // for file operations
#include <map>

enum Move { ROCK = 1, PAPER, SCISSORS };

// Convert int to string
std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// Convert move int to string representation
std::string moveToString(int move) {
    switch (move) {
    case ROCK: return "Rock";
    case PAPER: return "Paper";
    case SCISSORS: return "Scissors";
    default: return "Unknown";
    }
}

// Load scores from ini file into a map
std::map<std::string, std::pair<std::string, std::string>> readScoresFromFile() {
    std::ifstream resultsFile("results.ini");
    std::map<std::string, std::pair<std::string, std::string>> scores;
    std::string line, mode;
    std::string highScore, previousScore;

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

// Write scores from map to ini file
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

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed for random number generation
    HANDLE hSerial = CreateFile(L"COM3",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }

    int pl1Score = 0;
    int pl2Score = 0;
    int mode;
    std::map<std::string, std::pair<std::string, std::string>> scores = readScoresFromFile();

    // Choose game mode
    std::cout << "Choose game mode:\n1 - Player vs PC\n2 - Player vs Player\n3 - PC vs PC\n\nEnter your choice: ";
    std::cin >> mode;

    // Determine mode string and read high scores
    std::string modeString;
    switch (mode) {
    case 1: modeString = "Player vs PC"; break;
    case 2: modeString = "Player vs Player"; break;
    case 3: modeString = "PC vs PC"; break;
    default: std::cerr << "Invalid mode selected." << std::endl; return 1;
    }

    std::string highScore = scores[modeString].first;
    std::string previousScore = scores[modeString].second;

    // Game loop
    while (true) {
        int pl1Move, pl2Move;

        if (mode == 1) {
            // Player vs PC
            std::cout << "Choose your move: 1 - Rock, 2 - Paper, 3 - Scissors (0 to exit): ";
            std::cin >> pl1Move;

            if (pl1Move == 0) break; // Exit the game
            if (pl1Move < 1 || pl1Move > 3) {
                std::cout << "\nInvalid input. Try again." << std::endl;
                continue;
            }

            pl2Move = rand() % 3 + 1; // Generate PC move
            std::cout << "PC chose: " << moveToString(pl2Move) << std::endl;

        }
        else if (mode == 2) {
            // Player vs Player
            std::cout << "Player1, choose your move: 1 - Rock, 2 - Paper, 3 - Scissors (0 to exit): ";
            std::cin >> pl1Move;

            if (pl1Move == 0) break; // Exit the game
            if (pl1Move < 1 || pl1Move > 3) {
                std::cout << "\nInvalid input. Try again." << std::endl;
                continue;
            }

            std::cout << "Player2, choose your move: 1 - Rock, 2 - Paper, 3 - Scissors (0 to exit): ";
            std::cin >> pl2Move;

            if (pl2Move == 0) break; // Exit the game
            if (pl2Move < 1 || pl2Move > 3) {
                std::cout << "\nInvalid input. Try again." << std::endl;
                continue;
            }

        }
        else if (mode == 3) {
            int pcGame;
            std::cout << "\nEnter 1 to play a round (press 0 to exit): ";
            std::cin >> pcGame;

            if (pcGame == 0) break; // Exit the game
            if (pcGame == 1) {
                pl1Move = rand() % 3 + 1; // PC1 move
                pl2Move = rand() % 3 + 1; // PC2 move
                std::cout << "PC1 chose: " << moveToString(pl1Move) << std::endl;
                std::cout << "PC2 chose: " << moveToString(pl2Move) << std::endl;
            }
            else {
                std::cout << "\nInvalid input. Try again." << std::endl;
                continue;
            }
        }

        // Prepare the message to send (same for all modes)
        std::string message = intToString(pl1Move) + "," + intToString(pl2Move) + "\n";
        DWORD bytesWritten;
        WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, NULL);

        // Wait for the result from Arduino
        char buffer[256];
        DWORD bytesRead;
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                std::cout << "Received " << buffer << std::endl;

                // Update scores based on result
                if (std::string(buffer).find("Player1 wins") != std::string::npos) {
                    pl1Score++;
                }
                else if (std::string(buffer).find("Player2 wins") != std::string::npos) {
                    pl2Score++;
                }

                std::cout << "-------------------------------------------------------------------------------------" << std::endl;
                std::cout << "Score: " << pl1Score << " : " << pl2Score << std::endl;
            }
        }
        else {
            std::cerr << "Failed to read from serial port." << std::endl;
        }

        // Update the high score if the player has a higher score
        int currentHighScore = highScore.empty() ? 0 : std::stoi(highScore.substr(0, highScore.find(":")));
        if (pl1Score > currentHighScore) {
            highScore = intToString(pl1Score) + ":" + intToString(pl2Score);
            std::cerr << "New High Score!\n" << std::endl;
        }
    }

    // Update the previous score and save the updated scores
    scores[modeString].second = intToString(pl1Score) + ":" + intToString(pl2Score);
    scores[modeString].first = highScore;
    writeScoresToFile(scores);

    // Cleanup
    CloseHandle(hSerial);
    return 0;
}
