#include "minikv/shard_preview_candidate_material_request_integrity_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_material_request_integrity_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<IntegrityCheck, 15> integrity_check_catalog = {{
    {1, "source-material-request-fixture-frozen", "fixtures/release/shard-readiness-v965.json",
     "v965 remains the immutable source material request fixture"},
    {2, "node-stop-condition-respected", "Node v1446 material request roadmap",
     "mini-kv does not create another material request echo"},
    {3, "fixture-range-declared", "candidate material request integrity stage catalog",
     "v966-v975 fixtures can be reviewed in order"},
    {4, "release-continuity", "stage chain helper", "release versions and source fixtures are contiguous"},
    {5, "source-lineage-pinned", "candidate material request digest marker",
     "integrity review keeps v965 as its only source package"},
    {6, "status-parity", "candidate material request status",
     "v965 closeout status remains visible to control-plane consumers"},
    {7, "request-item-count-parity", "candidate material request items",
     "twenty-five requested material items stay frozen"},
    {8, "acceptance-check-count-parity", "candidate material request checks",
     "twenty-five material acceptance checks stay frozen"},
    {9, "request-gate-count-parity", "candidate material request gates",
     "forty material request gates stay frozen"},
    {10, "requested-field-count-parity", "candidate material requested fields",
     "twenty requested material fields stay frozen"},
    {11, "command-catalog-parity", "COMMANDS and COMMANDSJSON",
     "the integrity command is parser visible and read only"},
    {12, "fixture-encoding-bom-guard", "versioned release fixtures",
     "fixture evidence stays UTF-8 without BOM drift"},
    {13, "archive-reference-boundary", "e/965 archive evidence",
     "archive references are recorded without runtime archive walking"},
    {14, "no-material-consumption-boundary", "material request closeout",
     "reviewed real material remains absent unread unimported and unaccepted"},
    {15, "router-write-wal-execution-boundary", "command catalog and shard readiness boundaries",
     "router write routing store mutation WAL touch and execution stay disabled"},
}};

std::string format_check_json(const IntegrityCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"integrityOnly\":true"
           ",\"materialRead\":false"
           ",\"materialAccepted\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"documentIntakeOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const IntegrityCheck> candidate_material_request_integrity_checks() {
    return {integrity_check_catalog.data(), integrity_check_catalog.size()};
}

int planned_candidate_material_request_integrity_check_count() {
    return static_cast<int>(integrity_check_catalog.size());
}

std::string format_candidate_material_request_integrity_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > integrity_check_catalog.size()) {
        throw std::out_of_range{"published material request integrity check count exceeds check catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_check_json(integrity_check_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_material_request_integrity_checks
