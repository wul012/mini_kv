#include "minikv/command.hpp"

#include "command_parse_helpers.hpp"
#include "command_wal_gate.hpp"
#include "minikv/command_catalog.hpp"
#include "minikv/command_response_formatters.hpp"
#include "minikv/snapshot.hpp"
#include "minikv/wal.hpp"

#include <sstream>
#include <string>

namespace minikv {
namespace {

using CommandDispatchVerb = command_catalog::CommandDispatchVerb;
using command_internal::has_extra_token;
using command_internal::snapshot_error;
using command_internal::usage;
using command_internal::wal_command_gate;
using command_internal::wal_compact_error;
using command_internal::wal_disabled_error;

} // namespace

CommandResult CommandProcessor::execute_persistence_command(CommandDispatchVerb verb, std::istringstream& input) {
    switch (verb) {
    case CommandDispatchVerb::Save: {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("SAVE path");
        }

        std::size_t saved = 0;
        if (!SnapshotFile::save(store_, path, &saved)) {
            return snapshot_error("save");
        }

        return {std::string{"OK saved "} + std::to_string(saved)};
    }

    case CommandDispatchVerb::Load: {
        std::string path;
        std::getline(input >> std::ws, path);

        if (path.empty()) {
            return usage("LOAD path");
        }

        std::size_t loaded = 0;
        if (!SnapshotFile::load(store_, path, &loaded)) {
            return snapshot_error("load");
        }

        return {std::string{"OK loaded "} + std::to_string(loaded)};
    }

    case CommandDispatchVerb::Compact: {
        if (has_extra_token(input)) {
            return usage("COMPACT");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        std::size_t compacted = 0;
        {
            auto scope = wal_command_gate().enter_compaction();
            if (!wal_->compact(store_, &compacted)) {
                return wal_compact_error();
            }
        }

        return {std::string{"OK compacted "} + std::to_string(compacted)};
    }

    case CommandDispatchVerb::WalInfo: {
        if (has_extra_token(input)) {
            return usage("WALINFO");
        }

        if (wal_ == nullptr) {
            return wal_disabled_error();
        }

        auto scope = wal_command_gate().enter_command();
        return {command_response_formatters::format_walinfo(wal_->maintenance_report(store_))};
    }

    default:
        return {"ERR unknown command"};
    }
}

} // namespace minikv
