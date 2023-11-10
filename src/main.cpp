#include <iostream>
#include "parser.h"

int main() {
    std::string input;
    while (std::getline(std::cin, input) && !input.empty()) {
        try {
            std::cout << parser::calculate(input) << "\n\n";
        }
        catch (std::exception &e) {
            log::logError(e.what());
            std::cout << '\n';
        }
    }

    return 0;
}
