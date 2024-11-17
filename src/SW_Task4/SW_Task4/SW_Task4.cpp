#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <windows.h>
#include <sstream> // for std::stringstream
#include <fstream> // for file operations
#include <map>

/**
* SW_Task4_CPP
* Author : Osadchuk Vitaliy KI401
*/
#define UNICODE

/**
 * @enum Move
 * @brief Represents the possible moves in the Rock-Paper-Scissors game.
 *
 * Possible values:
 * - ROCK: Rock (value = 1)
 * - PAPER: Paper (value = 2)
 * - SCISSORS: Scissors (value = 3)
 */
enum Move { ROCK = 1, PAPER, SCISSORS };

/**
 * @brief Converts an integer to its string representation.
 * @param value The integer value to convert.
 * @return The string representation of the integer.
 */
std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

/**
 * @brief Converts a move (int) to its corresponding string representation.
 *
 * @param move The move to convert (ROCK, PAPER, or SCISSORS).
 * @return A string ("Rock", "Paper", "Scissors") corresponding to the move.
 */
std::string moveToString(int move) {
    switch (move) {
    case ROCK: return "Rock";
    case PAPER: return "Paper";
    case SCISSORS: return "Scissors";
    default: return "Unknown";
    }
}

/**
 * @brief Reads the high scores and previous scores from the "results.ini" file.
 *
 * The file format:
 * ```
 * Mode : Player vs PC;
 * Highest score: 5:3
 * Previous score: 3:4
 * ```
 *
 * @return A map where the key is the mode name (e.g., "Player vs PC") and the value
 *         is a pair of strings representing the highest score and the previous score.
 */
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

/**
 * @brief Writes the high scores and previous scores to the "results.ini" file.
 *
 * @param scores A map containing the scores for each mode. Each key is a mode name
 *               and each value is a pair of strings: highest score and previous score.
 */
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


/**
 * @brief Main function of the program. Implements the Rock-Paper-Scissors game.
 *
 * The program supports three modes:
 * - Player vs PC
 * - Player vs Player
 * - PC vs PC
 *
 * @return Exit status (0 for success, non-zero for failure).
 */
int main() {
    srand(static_cast<unsigned int>(time(nullptr))); 
    
    /**
     * @brief Handle hSerial for the serial port.
     * Used to send and receive data during gameplay.
     */
    HANDLE hSerial = CreateFileA("COM3",  
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

   
   /**
    * @brief int pl1Score and int pl2Score - Player scores.
    */
    int pl1Score = 0;
    int pl2Score = 0; 
    /**
    * @brief Game mode selected by the user.
    * Possible values:
    * - 1: Player vs PC
    * - 2: Player vs Player
    * - 3: PC vs PC
    */
    int mode;         

   
   /**
    * @brief A map containing scores for each game mode.
    * Key: Mode name (e.g., "Player vs PC").
    * Value: A pair of strings (highest score, previous score).
    */
    std::map<std::string, std::pair<std::string, std::string>> scores = readScoresFromFile();   

    // User input for game mode
    std::cout << "Choose game mode:\n1 - Player vs PC\n2 - Player vs Player\n3 - PC vs PC\n\nEnter your choice: ";
    std::cin >> mode;
    std::string modeString;         // Holds the game mode selected by the user.

    switch (mode) {
    case 1: modeString = "Player vs PC"; break;
    case 2: modeString = "Player vs Player"; break;
    case 3: modeString = "PC vs PC"; break;
    default: std::cerr << "Invalid mode selected." << std::endl; return 1;
    }

    /**
     * @brief string highScore - Highest score for the selected mode, read from the file.
     */
    std::string highScore = scores[modeString].first;      

    /**
     * @brief string previousScore - Previous score for the selected mode, read from the file.
     */
    std::string previousScore = scores[modeString].second;  

    /**
     * @brief Game loop. User can press 0 to exit game.
     */
    while (true) {
        /**
         * @brief int pl1Move, pl2Move - player1 and 2 choices(rock, paper, scissors or exit)
         */
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

            // Random move for the PC
            pl2Move = rand() % 3 + 1;
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

        /**
         * @brief DWORD bytesWritten - Number of bytes written to the serial port.
         */
        DWORD bytesWritten;   

        /**
         * @brief Send moves to the serial port
         */
        WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, NULL);

        /**
         * @brief char buffer[256] - Buffer for receiving data from the serial port
         */
        char buffer[256];     
        DWORD bytesRead;

        /**
         * @brief Read response from serial port
         */
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                std::cout << "Received " << buffer << std::endl;

                /**
                * @brief Update scores based on result
                */
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

        /**
         * @brief int currentHighScore - value of current high score
         */
        int currentHighScore = highScore.empty() ? 0 : std::stoi(highScore.substr(0, highScore.find(":")));   
        if (pl1Score > currentHighScore) {
            highScore = intToString(pl1Score) + ":" + intToString(pl2Score);
            std::cerr << "New High Score!\n" << std::endl;
        }
    }

        /**
         * @brief Update the previous score and save the updated scores
         */
    scores[modeString].second = intToString(pl1Score) + ":" + intToString(pl2Score);  
    scores[modeString].first = highScore;
    writeScoresToFile(scores);

    /**
    * @brief Close the serial port and exit
    */
    CloseHandle(hSerial);
    return 0;
}
