#include <iostream>
#include <windows.h>
#include <cstring>


bool checkMessageLength(const char* message) {
    return message && strlen(message) > 0;
}

int main() {
    const char* message = "Message from SW side\n"; // Message to send

#ifdef CHECK_MESSAGE_ONLY
    // check message length
    if (!checkMessageLength(message)) {
        std::cerr << "Message length is 0!" << std::endl;
        return 1; // Помилка
    }
    std::cout << "Test passed: Message length is greater than 0!" << std::endl;
    return 0; 
#endif

    // Open the serial port
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

    // Set up the serial port parameters
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_115200; // Baut rate
    dcbSerialParams.ByteSize = 8;        // Byte size
    dcbSerialParams.StopBits = ONESTOPBIT; // Stop bits
    dcbSerialParams.Parity = NOPARITY;   // Parity

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial state." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Set timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting timeouts." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Send a message to the Arduino
    DWORD bytesWritten;
    if (!WriteFile(hSerial, message, strlen(message), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    std::cout << "Connected to HW successfully!\n" << std::endl;
    std::cout << "Message sent: " << message << std::endl;

    // Read the response from the Arduino
    char buffer[256];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port." << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    if (bytesRead == 0) {
        std::cout << "No response received from HW." << std::endl;
    }
    else {
        // Null-terminate the string
        buffer[bytesRead] = '\0';
        std::cout << "Received message: " << buffer << std::endl;
    }

    // Close the serial port
    CloseHandle(hSerial);
    std::cout << "Serial port closed." << std::endl;
    return 0;
}
