#include "minikv/shard_preview_candidate_material_submission_precheck_validators.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_material_submission_precheck_validators {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<MaterialSubmissionPrecheckValidator, 10> validator_catalog = {{
    {1, "source-material-request-integrity-frozen", "fixtures/release/shard-readiness-v975.json",
     "v975 remains the immutable source for material submission precheck"},
    {2, "node-v1456-plan-pinned",
     "docs/plans3/v1456-controlled-read-only-shard-preview-candidate-document-material-submission-precheck-roadmap.md",
     "mini-kv follows submission precheck scope without creating a material intake path"},
    {3, "request-item-count-carried", "frozen v965 material request metadata",
     "twenty-five material request items remain metadata only"},
    {4, "acceptance-check-count-carried", "frozen v965 material request metadata",
     "twenty-five acceptance checks remain preconditions not runtime acceptance"},
    {5, "required-field-shape-carried", "Node v1456 plan metadata",
     "twenty required material fields and twenty submission fields remain unparsed"},
    {6, "gate-count-carried", "Node v1456 plan metadata",
     "forty-one gates are carried as expectations only"},
    {7, "real-material-absence-guard", "material submission boundary fields",
     "reviewed real candidate document material remains absent and unread"},
    {8, "payload-intake-freeze-guard", "material submission boundary fields",
     "candidate material payload import staging and evaluation remain disabled"},
    {9, "router-write-wal-guard", "command catalog and shard readiness",
     "document router write routing store mutation and WAL touch remain disabled"},
    {10, "no-execution-closeout-guard", "Node v1456 stop condition",
     "the chain stops at submission precheck until externally supplied reviewed material exists"},
}};

std::string format_validator_json(const MaterialSubmissionPrecheckValidator& validator) {
    return "{\"sequence\":" + std::to_string(validator.sequence) +
           ",\"validatorCode\":" + json_string(validator.validator_code) +
           ",\"source\":" + json_string(validator.source) +
           ",\"protects\":" + json_string(validator.protects) +
           ",\"readOnly\":true"
           ",\"materialSubmissionPrecheckOnly\":true"
           ",\"reviewedMaterialPresent\":false"
           ",\"materialPayloadImported\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const MaterialSubmissionPrecheckValidator> candidate_material_submission_precheck_validators() {
    return {validator_catalog.data(), validator_catalog.size()};
}

int planned_candidate_material_submission_precheck_validator_count() {
    return static_cast<int>(validator_catalog.size());
}

std::string format_candidate_material_submission_precheck_validators_json(int published_validator_count) {
    if (published_validator_count < 0 || static_cast<std::size_t>(published_validator_count) > validator_catalog.size()) {
        throw std::out_of_range{"published material submission precheck validator count exceeds validator catalog size"};
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

} // namespace minikv::shard_preview_candidate_material_submission_precheck_validators
