#pragma once

#include "minikv/store.hpp"

#include <string>
#include <string_view>

namespace minikv {

class WriteAheadLog;

struct CommandResult {
    std::string response;
    bool should_close = false;
};

class CommandProcessor {
public:
    explicit CommandProcessor(Store& store, WriteAheadLog* wal = nullptr);

    CommandResult execute(std::string_view line);

    static std::string help_text();

private:
    Store& store_;
    WriteAheadLog* wal_;
};

} // namespace minikv
