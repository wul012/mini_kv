#include "minikv/client_history.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
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

    const auto suffix = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
    const auto temp_dir = std::filesystem::temp_directory_path() / ("minikv-client-history-tests-" + suffix);
    std::filesystem::remove_all(temp_dir);
    std::filesystem::create_directories(temp_dir);

    minikv::ClientHistory missing_history{3};
    assert(missing_history.load_from_file(temp_dir / "missing.history") == 0);
    assert(missing_history.empty());

    minikv::ClientHistory persistent_history{3};
    result = minikv::resolve_client_input(persistent_history, "PING one");
    assert(result.action == minikv::ClientInputAction::send);
    result = minikv::resolve_client_input(persistent_history, "SET name mini-kv");
    assert(result.action == minikv::ClientInputAction::send);
    result = minikv::resolve_client_input(persistent_history, "GET name");
    assert(result.action == minikv::ClientInputAction::send);
    result = minikv::resolve_client_input(persistent_history, "SIZE");
    assert(result.action == minikv::ClientInputAction::send);

    const auto saved_file = temp_dir / "nested" / "client.history";
    persistent_history.save_to_file(saved_file);
    assert(std::filesystem::exists(saved_file));

    minikv::ClientHistory restored_history{5};
    assert(restored_history.load_from_file(saved_file) == 3);
    assert((restored_history.entries() ==
            std::vector<std::string>{"SET name mini-kv", "GET name", "SIZE"}));

    const auto manual_file = temp_dir / "manual.history";
    {
        std::ofstream manual{manual_file};
        manual << "PING old\n";
        manual << "\n";
        manual << "SET loaded value\r\n";
        manual << "GET loaded\n";
        manual << "SIZE\n";
    }

    minikv::ClientHistory clipped_history{2};
    assert(clipped_history.load_from_file(manual_file) == 4);
    assert((clipped_history.entries() == std::vector<std::string>{"GET loaded", "SIZE"}));

    minikv::ClientKeyCache key_cache{3};
    assert(key_cache.empty());
    assert(key_cache.add("alpha"));
    assert(!key_cache.add("alpha"));
    assert(key_cache.add(" beta "));
    assert((key_cache.entries() == std::vector<std::string>{"alpha", "beta"}));
    assert(!key_cache.remove("missing"));
    assert(key_cache.remove("alpha"));
    assert((key_cache.entries() == std::vector<std::string>{"beta"}));
    assert(key_cache.clear());
    assert(!key_cache.clear());
    assert(key_cache.replace({"old", "alpha", "alpha", " beta "}));
    assert((key_cache.entries() == std::vector<std::string>{"old", "alpha", "beta"}));
    assert(key_cache.replace({"alpha", "beta", "gamma", "delta"}));
    assert((key_cache.entries() == std::vector<std::string>{"beta", "gamma", "delta"}));
    assert(!key_cache.replace({"beta", "gamma", "delta"}));
    assert(key_cache.replace({}));
    assert(key_cache.empty());

    bool rejected_zero_key_capacity = false;
    try {
        minikv::ClientKeyCache invalid{0};
    } catch (const std::invalid_argument&) {
        rejected_zero_key_capacity = true;
    }
    assert(rejected_zero_key_capacity);

    minikv::ClientKeyCache missing_key_cache{3};
    assert(missing_key_cache.load_from_file(temp_dir / "missing.keys") == 0);
    assert(missing_key_cache.empty());

    minikv::ClientKeyCache persistent_key_cache{3};
    assert(persistent_key_cache.add("alpha"));
    assert(persistent_key_cache.add("alpine"));
    assert(persistent_key_cache.add("name"));
    assert(persistent_key_cache.add("user:1"));

    const auto saved_keys_file = temp_dir / "nested" / "client.keys";
    persistent_key_cache.save_to_file(saved_keys_file);
    assert(std::filesystem::exists(saved_keys_file));

    minikv::ClientKeyCache restored_key_cache{5};
    assert(restored_key_cache.load_from_file(saved_keys_file) == 3);
    assert((restored_key_cache.entries() == std::vector<std::string>{"alpine", "name", "user:1"}));

    const auto manual_keys_file = temp_dir / "manual.keys";
    {
        std::ofstream manual{manual_keys_file};
        manual << "alpha\n";
        manual << "\n";
        manual << "alpha\r\n";
        manual << "name\n";
        manual << "user:1\n";
    }

    minikv::ClientKeyCache clipped_key_cache{2};
    assert(clipped_key_cache.load_from_file(manual_keys_file) == 3);
    assert((clipped_key_cache.entries() == std::vector<std::string>{"name", "user:1"}));

    auto parsed_keys = minikv::parse_key_list_response("key_count=2 keys=alpha beta");
    assert(parsed_keys.has_value());
    assert((*parsed_keys == std::vector<std::string>{"alpha", "beta"}));

    parsed_keys = minikv::parse_key_list_response("key_count=0 keys=");
    assert(parsed_keys.has_value());
    assert(parsed_keys->empty());

    assert(!minikv::parse_key_list_response("ERR usage: KEYS").has_value());
    assert(!minikv::parse_key_list_response("key_count=two keys=alpha beta").has_value());
    assert(!minikv::parse_key_list_response("key_count=1 keys=alpha beta").has_value());

    std::filesystem::remove_all(temp_dir);

    return 0;
}
