#pragma once

#include "minikv/store.hpp"

#include <string>
#include <string_view>

namespace minikv {

struct CommandResult {
    std::string response;
    bool should_close = false;
};

class CommandProcessor {
public:
    explicit CommandProcessor(Store& store);

    CommandResult execute(std::string_view line);

    static std::string help_text();

private:
    Store& store_;
};

} // namespace minikv
