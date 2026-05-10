#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace minikv {

enum class ClientInputAction {
    send,
    local
};

struct ClientInputResult {
    ClientInputAction action = ClientInputAction::local;
    std::string command;
    std::vector<std::string> messages;
};

class ClientHistory {
public:
    explicit ClientHistory(std::size_t max_entries = 100);

    void add(std::string command);
    bool empty() const;
    std::size_t size() const;
    const std::vector<std::string>& entries() const;
    std::optional<std::string> last() const;
    std::optional<std::string> get(std::size_t one_based_index) const;
    std::size_t load_from_file(const std::filesystem::path& path);
    void save_to_file(const std::filesystem::path& path) const;

private:
    std::size_t max_entries_;
    std::vector<std::string> entries_;
};

ClientInputResult resolve_client_input(ClientHistory& history, std::string_view input);

} // namespace minikv
