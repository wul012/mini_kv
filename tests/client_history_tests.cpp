#include "minikv/client_history.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    minikv::ClientHistory history{3};

    auto result = minikv::resolve_client_input(history, ":history");
    assert(result.action == minikv::ClientInputAction::local);
    assert(result.command.empty());
    assert(result.messages == std::vector<std::string>{"history is empty"});
    assert(history.empty());

    result = minikv::resolve_client_input(history, "PING one");
    assert(result.action == minikv::ClientInputAction::send);
    assert(result.command == "PING one");
    assert(result.messages.empty());
    assert(history.size() == 1);

    result = minikv::resolve_client_input(history, "SET name mini-kv");
    assert(result.action == minikv::ClientInputAction::send);
    assert(result.command == "SET name mini-kv");
    assert(history.size() == 2);

    result = minikv::resolve_client_input(history, ":history");
    assert(result.action == minikv::ClientInputAction::local);
    assert((result.messages == std::vector<std::string>{"1  PING one", "2  SET name mini-kv"}));

    result = minikv::resolve_client_input(history, "!!");
    assert(result.action == minikv::ClientInputAction::send);
    assert(result.command == "SET name mini-kv");
    assert(result.messages == std::vector<std::string>{"repeating: SET name mini-kv"});
    assert(history.size() == 3);

    result = minikv::resolve_client_input(history, "!1");
    assert(result.action == minikv::ClientInputAction::send);
    assert(result.command == "PING one");
    assert(result.messages == std::vector<std::string>{"repeating: PING one"});
    assert(history.size() == 3);
    assert(history.entries()[0] == "SET name mini-kv");
    assert(history.entries()[1] == "SET name mini-kv");
    assert(history.entries()[2] == "PING one");

    result = minikv::resolve_client_input(history, "!99");
    assert(result.action == minikv::ClientInputAction::local);
    assert(result.messages == std::vector<std::string>{"history entry not found: 99"});

    result = minikv::resolve_client_input(history, "!abc");
    assert(result.action == minikv::ClientInputAction::send);
    assert(result.command == "!abc");

    bool rejected_zero_capacity = false;
    try {
        minikv::ClientHistory invalid{0};
    } catch (const std::invalid_argument&) {
        rejected_zero_capacity = true;
    }
    assert(rejected_zero_capacity);

    return 0;
}
