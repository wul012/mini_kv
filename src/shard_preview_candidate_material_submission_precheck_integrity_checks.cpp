#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<IntegrityCheck, 25> integrity_check_catalog = {{
    {1, "source-precheck-fixture-frozen", "fixtures/release/shard-readiness-v985.json",
     "v985 remains the immutable source material submission precheck fixture"},
    {2, "node-stop-condition-respected", "Node v1456 material submission precheck roadmap",
     "mini-kv does not create another material submission echo"},
    {3, "fixture-range-declared", "candidate material submission precheck integrity stage catalog",
     "v986-v1010 fixtures can be reviewed in order"},
    {4, "release-continuity", "stage chain helper", "release versions and source fixtures are contiguous"},
    {5, "source-lineage-pinned", "candidate material submission precheck digest marker",
     "integrity review keeps v985 as its only source precheck"},
    {6, "status-parity", "candidate material submission precheck status",
     "v985 closeout status remains visible to control-plane consumers"},
    {7, "checkpoint-count-parity", "candidate material submission precheck checkpoints",
     "ten submission checkpoints stay frozen"},
    {8, "validator-count-parity", "candidate material submission precheck validators",
     "ten submission validators stay frozen"},
    {9, "request-item-count-parity", "Node v1456 precheck metadata",
     "twenty-five material request items stay frozen"},
    {10, "acceptance-check-count-parity", "Node v1456 precheck metadata",
     "twenty-five acceptance checks stay frozen"},
    {11, "required-field-count-parity", "Node v1456 precheck metadata",
     "twenty required material fields stay frozen"},
    {12, "submission-field-count-parity", "Node v1456 precheck metadata",
     "twenty submission material fields stay frozen"},
    {13, "submission-gate-count-parity", "Node v1456 precheck metadata",
     "forty-one material submission gates stay frozen"},
    {14, "real-material-absence-boundary", "material submission boundary fields",
     "reviewed real material remains absent unread and unconsumed"},
    {15, "payload-import-boundary", "material submission boundary fields",
     "material payload candidate payload and staged payload imports remain disabled"},
    {16, "evaluation-boundary", "material submission boundary fields",
     "candidate evaluation acceptance and rejection remain disabled"},
    {17, "approval-boundary", "material submission boundary fields", "approval grant capture remains disabled"},
    {18, "signature-boundary", "material submission boundary fields",
     "signature emission parsing and verification remain disabled"},
    {19, "runtime-boundary", "material submission boundary fields",
     "runtime payload import and runtime probes remain disabled"},
    {20, "router-boundary", "command catalog and shard readiness", "document router remains absent"},
    {21, "write-wal-boundary", "command catalog and shard readiness",
     "write routing store mutation WAL touch load restore and compact remain disabled"},
    {22, "sibling-mutation-boundary", "shard readiness boundaries",
     "Node Java and archive sibling mutation remain disabled"},
    {23, "command-catalog-parity", "COMMANDS and COMMANDSJSON",
     "the integrity command is parser visible and read only"},
    {24, "cli-tcp-smoke-parity", "CLI and TCP smoke",
     "smoke checks use the same read-only integrity command"},
    {25, "no-additional-submission-echo", "Node v1456 stop condition",
     "integrity closeout does not open material submission or create another echo"},
}};

std::string format_check_json(const IntegrityCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"integrityOnly\":true"
           ",\"materialSubmissionOpened\":false"
           ",\"materialRead\":false"
           ",\"materialAccepted\":false"
           ",\"materialPayloadImported\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"documentIntakeOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const IntegrityCheck> candidate_material_submission_precheck_integrity_checks() {
    return {integrity_check_catalog.data(), integrity_check_catalog.size()};
}

int planned_candidate_material_submission_precheck_integrity_check_count() {
    return static_cast<int>(integrity_check_catalog.size());
}

std::string format_candidate_material_submission_precheck_integrity_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > integrity_check_catalog.size()) {
        throw std::out_of_range{"published material submission precheck integrity check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_checks
