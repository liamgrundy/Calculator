#ifndef CALCULATOR_LOG_H
#define CALCULATOR_LOG_H

#include <string>
#include <iostream>

namespace log {
    inline void logWarning(const std::string &message) {
        std::cout << "\e[33m" << "Warning: " << message << "\e[0m" << '\n';
    }

    inline void logError(const std::string &message) {
        std::cout << "\e[31m" << "Error: " << message << "\e[0m" << '\n';
    }
}

#endif //CALCULATOR_LOG_H
