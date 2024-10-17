void setup() {
    Serial.begin(115200);  // baut speed 115200
}

void loop() {
    if (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');  // read until '\n'
        
        if (message.length() > 0) {  // message not empty
            message += " - modified by HW*";  // modify
            Serial.println(message);  // send
        }
    }
}
