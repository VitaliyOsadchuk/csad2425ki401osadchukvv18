#ifndef HW_MOCK_H
#define HW_MOCK_H

#include <iostream>
#include <string>

class HwMock {
public:
    HwMock() {}

    void begin(int baudRate) {
        std::cout << "HW Mock initialized with baud rate: " << baudRate << std::endl;
    }

    std::string readMessage(const std::string& message) {
        std::cout << "HW Mock received message: " << message << std::endl;
        return message + " --- modified by HW";
    }

    void sendMessage(const std::string& message) {
        std::cout << "HW Mock sending message: " << message << std::endl;
    }
};

#endif // HW_MOCK_H
