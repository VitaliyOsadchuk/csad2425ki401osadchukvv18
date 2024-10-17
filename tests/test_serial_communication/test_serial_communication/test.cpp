#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#include "pch.h"
#include "pseudo_serial.h"
#include <gtest/gtest.h>
#include <cstring>

// Тест для перевірки довжини повідомлення
TEST(SerialCommunicationTest, CheckMessageLength) {
    const char* message = "Message from SW side\n";
    ASSERT_TRUE(strlen(message) > 0);
}

// Тест для серійної комунікації
TEST(SerialCommunicationTest, TestSerialCommunication) {
    PseudoSerial Serial("COM3", 115200);

    std::string message = "Message from SW side\n";

    if (Serial.available()) {
        std::string received = Serial.readStringUntil('\n');
        received += " - modified by HW*";
        Serial.println(received);
        EXPECT_EQ(received, "Mock message - modified by HW*");
    }
    else {
        FAIL() << "No data available on mock serial port.";
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
