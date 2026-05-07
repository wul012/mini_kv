#include "minikv/command.hpp"

#include <iostream>
#include <string>

int main() {
    minikv::Store store;
    minikv::CommandProcessor processor{store};

    std::cout << "mini-kv CLI\n";
    std::cout << minikv::CommandProcessor::help_text() << '\n';

    std::string line;
    while (true) {
        std::cout << "mini-kv> ";
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;
        }

        const auto result = processor.execute(line);
        if (!result.response.empty()) {
            std::cout << result.response << '\n';
        }

        if (result.should_close) {
            break;
        }
    }

    return 0;
}
