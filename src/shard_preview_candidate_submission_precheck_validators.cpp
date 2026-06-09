#include "minikv/shard_preview_candidate_submission_precheck_validators.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_submission_precheck_validators {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<SubmissionPrecheckValidator, 10> validator_catalog = {{
    {1, "source-integrity-fixture-frozen", "fixtures/release/shard-readiness-v920.json",
     "v920 remains the immutable source for submission precheck handoff"},
    {2, "node-v1411-plan-pinned",
     "docs/plans3/v1411-controlled-read-only-shard-preview-candidate-document-submission-precheck-closeout-roadmap.md",
     "mini-kv follows submission precheck scope without creating a document intake path"},
    {3, "request-package-lineage-carried", "candidate request package integrity digest marker",
     "v895 request package and v920 integrity lineage remain visible"},
    {4, "requested-field-count-carried", "frozen request package metadata",
     "twenty requested candidate fields are metadata only"},
    {5, "submission-gate-count-carried", "Node v1411 plan metadata",
     "forty submission gates are carried as expectations only"},
    {6, "real-document-absence-guard", "submission precheck boundary fields",
     "reviewed real candidate document remains absent and unread"},
    {7, "payload-intake-guard", "submission precheck boundary fields",
     "candidate payload import and staging remain disabled"},
    {8, "approval-signature-guard", "submission precheck boundary fields",
     "approval capture signature parsing and runtime execution remain disabled"},
    {9, "router-write-wal-guard", "command catalog and shard readiness",
     "document router write routing store mutation and WAL touch remain disabled"},
    {10, "no-extra-precheck-echo-guard", "Node v1411 stop condition",
     "the chain stops at precheck until a reviewed real candidate document exists"},
}};

std::string format_validator_json(const SubmissionPrecheckValidator& validator) {
    return "{\"sequence\":" + std::to_string(validator.sequence) +
           ",\"validatorCode\":" + json_string(validator.validator_code) +
           ",\"source\":" + json_string(validator.source) +
           ",\"protects\":" + json_string(validator.protects) +
           ",\"readOnly\":true"
           ",\"documentIntakeOpened\":false"
           ",\"candidatePayloadImported\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const SubmissionPrecheckValidator> candidate_submission_precheck_validators() {
    return {validator_catalog.data(), validator_catalog.size()};
}

int planned_candidate_submission_precheck_validator_count() {
    return static_cast<int>(validator_catalog.size());
}

std::string format_candidate_submission_precheck_validators_json(int published_validator_count) {
    if (published_validator_count < 0 || static_cast<std::size_t>(published_validator_count) > validator_catalog.size()) {
        throw std::out_of_range{"published submission precheck validator count exceeds validator catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_validator_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_validator_json(validator_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_submission_precheck_validators
