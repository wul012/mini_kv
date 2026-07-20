#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::command_internal {

using EvidenceFormatter = std::string (*)();

struct EvidenceCommand {
    std::string_view name;
    EvidenceFormatter format;
};

std::span<const EvidenceCommand> evidence_commands();
const EvidenceCommand* find_evidence_command(std::string_view name);

} // namespace minikv::command_internal
