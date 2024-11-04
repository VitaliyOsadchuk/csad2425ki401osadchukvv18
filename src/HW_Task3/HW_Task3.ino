#include <Arduino.h>

enum Move { ROCK = 1, PAPER, SCISSORS };

void setup() {
    Serial.begin(115200);  // Baud rate 115200
}

void loop() {
    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');  // read until '\n'

        if (message.length() > 0) {  // message not empty
            // Split the message to get player move and PC move
            int delimiterIndex = message.indexOf(',');
            int playerMove = message.substring(0, delimiterIndex).toInt();
            int pcMove = message.substring(delimiterIndex + 1).toInt();
            String result;

            // Determine winner
            if (playerMove == pcMove) {
                result = "Draw";
            } else if ((playerMove == ROCK && pcMove == SCISSORS) ||
                       (playerMove == PAPER && pcMove == ROCK) ||
                       (playerMove == SCISSORS && pcMove == PAPER)) {
                result = "Player wins";
            } else {
                result = "PC wins";
            }

            // Print the result to the serial monitor
            Serial.print("Result: ");
            Serial.println(result);
        }
    }
}
