#include <string>

class PseudoSerial {
public:
    PseudoSerial(const std::string& port, int baudrate) {}
    bool available() { return true; }
    std::string readStringUntil(char delimiter) { return "Mock message"; }
    void println(const std::string& message) {
        // Mock behavior
        std::cout << "Sent: " << message << std::endl;
    }
};

