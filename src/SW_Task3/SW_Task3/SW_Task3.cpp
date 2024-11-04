#include <iostream>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <windows.h>
#include <sstream> // for std::stringstream
#include <fstream> // for file operations

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

// Write results to the ini file
void writeResultsToFile(const std::string& mode, int playerScore, int pcScore, const std::string& highScore) {
    std::ofstream resultsFile("results.ini");
    if (resultsFile.is_open()) {
        resultsFile << "Mode : " << mode << ";\n";
        resultsFile << "Highest score: " << highScore << "\n"; // Changed format to X:Y
        resultsFile << "Previous score: " << playerScore << " : " << pcScore << "\n";
        resultsFile.close();
        std::cout << "Results written to results.ini." << std::endl;
    }
    else {
        std::cerr << "Unable to open results.ini for writing." << std::endl;
    }
}

// Read highest scores from the ini file
std::string readScoresFromFile() {
    std::ifstream resultsFile("results.ini");
    std::string line;
    std::string highScore = "0:0"; // Default high score

    if (resultsFile.is_open()) {
        while (std::getline(resultsFile, line)) {
            if (line.find("Highest score: ") != std::string::npos) {
                // Extract the highest score
                std::size_t pos = line.find(": ") + 2; // Find position after ": "
                highScore = line.substr(pos);
            }
        }
        resultsFile.close();
    }
    else {
        std::cerr << "Unable to open results.ini for reading." << std::endl;
    }
    return highScore;
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

    int playerScore = 0;
    int pcScore = 0;

    // Read the highest scores from the file
    std::string highScore = readScoresFromFile();

    // Send moves in a loop
    while (true) {
        std::cout << "Choose your move: 1 - Rock, 2 - Paper, 3 - Scissors (0 to exit): ";
        int playerMove;
        std::cin >> playerMove;

        // Check if input is valid
        if (playerMove == 0) {
            break; // Exit the game
        }
        else if (playerMove < 0 || playerMove > 3) {
            std::cout << "\nInvalid input. Choose your move: 1 - Rock, 2 - Paper, 3 - Scissors (0 to exit): " << std::endl;
            continue; // Skip to the next iteration of the loop
        }

        // Generate PC move
        int pcMove = rand() % 3 + 1; // Random move: 1 - 3
        std::cout << "PC chose: " << moveToString(pcMove) << std::endl;

        // Prepare the message to send
        std::string message = intToString(playerMove) + "," + intToString(pcMove) + "\n";
        DWORD bytesWritten;
        WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, NULL);

        // Wait for the result from Arduino
        char buffer[256];
        DWORD bytesRead;
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string
                std::cout << "Received: " << buffer << std::endl;

                // Update scores based on result
                if (std::string(buffer).find("Player wins") != std::string::npos) {
                    playerScore++;
                }
                else if (std::string(buffer).find("PC wins") != std::string::npos) {
                    pcScore++;
                }

                // Print the current score
                std::cout << "Current Score - Player: " << playerScore << " : PC: " << pcScore << std::endl;
                std::cout << "-----------------------------------------------------------------------------" << std::endl;
            }
        }
    }


    // Check if the player has a new highest score and update if necessary
    int currentHighPlayerScore = std::stoi(highScore.substr(0, highScore.find(':')));
    int currentHighPCScore = std::stoi(highScore.substr(highScore.find(':') + 1));

    // Determine if the current game score beats the previous high score
    if (playerScore > currentHighPlayerScore || (playerScore == currentHighPlayerScore && pcScore < currentHighPCScore)) {
        std::cout << "Congratulations! New High Score: " << playerScore << " : " << pcScore << std::endl;
        highScore = intToString(playerScore) + " : " + intToString(pcScore); // Update high score
    }

    // Write final results to ini file
    writeResultsToFile("Player vs PC", playerScore, pcScore, highScore);

    // Close the serial port
    CloseHandle(hSerial);
    std::cout << "Serial port closed." << std::endl;

    return 0;
}
