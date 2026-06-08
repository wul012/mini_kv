#include "minikv/shard_preview_candidate_request_package_integrity_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_request_package_integrity_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<IntegrityCheck, 25> integrity_check_catalog = {{
    {1, "source-fixture-frozen", "fixtures/release/shard-readiness-v895.json",
     "v895 remains the immutable source for integrity review"},
    {2, "fixture-range-declared", "candidate request package integrity stage catalog",
     "v896-v920 source fixtures can be reviewed in order"},
    {3, "release-continuity", "stage chain helper", "release versions are contiguous"},
    {4, "source-lineage-pinned", "candidate request package digest marker",
     "integrity review does not choose a new source package"},
    {5, "status-parity", "candidate request package status",
     "v895 closeout state remains visible to consumers"},
    {6, "request-item-count-parity", "candidate request package items", "fifteen request items stay frozen"},
    {7, "acceptance-check-count-parity", "candidate request package checks",
     "fifteen acceptance checks stay frozen"},
    {8, "request-gate-count-parity", "candidate request package gates",
     "thirty-eight request gates stay frozen"},
    {9, "requested-field-count-parity", "candidate request package requested fields",
     "twenty requested candidate fields stay frozen"},
    {10, "missing-document-boundary", "candidate request package closeout",
     "real candidate document remains absent"},
    {11, "synthetic-document-boundary", "candidate request package closeout",
     "synthetic candidate documents stay rejected"},
    {12, "payload-import-boundary", "candidate request package closeout",
     "staged payload import remains disabled"},
    {13, "evaluation-boundary", "candidate request package closeout",
     "candidate evaluation acceptance and rejection remain disabled"},
    {14, "approval-boundary", "candidate request package closeout",
     "approval grant capture remains disabled"},
    {15, "signature-boundary", "candidate request package closeout",
     "signed approval emission remains disabled"},
    {16, "runtime-boundary", "candidate request package closeout",
     "runtime payload import remains disabled"},
    {17, "write-routing-boundary", "command catalog and shard readiness",
     "write routing and store mutation remain disabled"},
    {18, "wal-boundary", "command catalog and shard readiness", "WAL touch remains disabled"},
    {19, "document-router-boundary", "shard readiness boundaries", "document router remains absent"},
    {20, "sibling-mutation-boundary", "shard readiness boundaries",
     "Node Java and archive sibling mutation remain disabled"},
    {21, "archive-walk-boundary", "shard readiness boundaries", "runtime archive walks remain disabled"},
    {22, "current-fixture-parity", "fixtures/release/shard-readiness.json",
     "current fixture carries the integrity section"},
    {23, "command-catalog-parity", "COMMANDS and COMMANDSJSON",
     "the integrity command is parser visible and read only"},
    {24, "cli-tcp-smoke-parity", "CLI and TCP smoke", "smoke checks use the same read-only evidence command"},
    {25, "no-additional-request-echo", "Node v1386 stop condition",
     "integrity closeout does not create another request echo"},
}};

std::string format_check_json(const IntegrityCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"readOnly\":true"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"documentIntakeOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const IntegrityCheck> candidate_request_package_integrity_checks() {
    return {integrity_check_catalog.data(), integrity_check_catalog.size()};
}

int planned_candidate_request_package_integrity_check_count() {
    return static_cast<int>(integrity_check_catalog.size());
}

std::string format_candidate_request_package_integrity_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > integrity_check_catalog.size()) {
        throw std::out_of_range{"published integrity check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_candidate_request_package_integrity_checks
