#include <Arduino.h>

/** 
 * @enum Move
 * @brief Enumeration for the possible moves in the game.
 * 
 * This enum defines the moves that can be made by the players.
 */
enum Move { ROCK = 1, PAPER, SCISSORS };

/** 
 * @brief Setup function for the Arduino.
 * 
 * Initializes the serial communication with a baud rate of 115200.
 */
void setup() {
    Serial.begin(115200);  /**< Baud rate for serial communication */
}

/** 
 * @brief Main loop for handling the game logic.
 * 
 * Continuously reads player moves from the serial input, processes the game logic, 
 * and sends the result back to the serial monitor.
 */
void loop() {
    if (Serial.available() > 0) {  /**< Check if there is data available in the serial buffer */
        String message = Serial.readStringUntil('\n');  /**< Read the message from serial input until newline character and store in String message*/

        if (message.length() > 0) {  /**< Ensure the message is not empty */
            // Split the message to get player move and PC move
            int delimiterIndex = message.indexOf(','); /**< Find the comma delimiter separating moves and store in int delimiterIndex*/
            int pl1Move = message.substring(0, delimiterIndex).toInt(); /**< Convert player 1's move from string to integer and store in int pl1Move*/
            int pl2Move = message.substring(delimiterIndex + 1).toInt(); /**< Convert player 2's move from string to integer and store in int pl2Move*/
            String result;  /**< String result - Variable to store the result of the game */

            // Determine winner
            if (pl1Move == pl2Move) {
                result = "Draw"; /**< If moves are the same, it's a result = draw */
            } else if ((pl1Move == ROCK && pl2Move == SCISSORS) ||
                       (pl1Move == PAPER && pl2Move == ROCK) ||
                       (pl1Move == SCISSORS && pl2Move == PAPER)) {
                result = "Player1 wins"; /**< If player 1 wins, result = Player1 wins*/
            } else {
                result = "Player2 wins"; /**< If player 2 wins, result = Player2 wins*/
            }

            // Print the result to the serial monitor
            Serial.print("result: ");  /**< Print the result label */
            Serial.println(result);    /**< Print the actual result */
        }
    }
}
