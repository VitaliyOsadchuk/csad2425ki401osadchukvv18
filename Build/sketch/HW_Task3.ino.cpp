#line 1 "D:\\git2\\csad2425ki401osadchukvv18\\src\\HW_Task3\\HW_Task3.ino"
#include <Arduino.h>

enum Move { ROCK = 1, PAPER, SCISSORS };

#line 5 "D:\\git2\\csad2425ki401osadchukvv18\\src\\HW_Task3\\HW_Task3.ino"
void setup();
#line 9 "D:\\git2\\csad2425ki401osadchukvv18\\src\\HW_Task3\\HW_Task3.ino"
void loop();
#line 5 "D:\\git2\\csad2425ki401osadchukvv18\\src\\HW_Task3\\HW_Task3.ino"
void setup() {
    Serial.begin(115200);  // Baud rate 115200
}

void loop() {
    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');  // read until '\n'

        if (message.length() > 0) {  // message not empty
            // Split the message to get player move and PC move
            int delimiterIndex = message.indexOf(',');
            int pl1Move = message.substring(0, delimiterIndex).toInt();
            int pl2Move = message.substring(delimiterIndex + 1).toInt();
            String result;

            // Determine winner
            if (pl1Move == pl2Move) {
                result = "Draw";
            } else if ((pl1Move == ROCK && pl2Move == SCISSORS) ||
                       (pl1Move == PAPER && pl2Move == ROCK) ||
                       (pl1Move == SCISSORS && pl2Move == PAPER)) {
                result = "Player1 wins";
            } else {
                result = "Player2 wins";
            }

            // Print the result to the serial monitor
            Serial.print("result: ");
            Serial.println(result);
        }
    }
}

