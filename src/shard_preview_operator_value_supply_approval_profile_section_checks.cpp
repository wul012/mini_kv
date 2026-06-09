#include "minikv/shard_preview_operator_value_supply_approval_profile_section_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_operator_value_supply_approval_profile_section_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<OperatorValueSupplyApprovalProfileSectionCheck, 25> check_catalog = {{
    {1, "source-current-fixture-frozen", "fixtures/release/shard-readiness-v1110.json",
     "v1110 remains the immutable source current fixture"},
    {2, "node-no-new-approval-chain-decision", "Node v1556 renderer split roadmap",
     "mini-kv treats the split as read-only ownership context"},
    {3, "approval-section-group-boundary", "Node v1556 five-section group",
     "only approval packet and signed approval capture profile sections are represented"},
    {4, "main-renderer-delegation-boundary", "live read-only window profile sections renderer",
     "main renderer delegation is recorded without executing Node code"},
    {5, "approval-aggregator-boundary", "operator evidence value supply approval profile section renderer",
     "aggregator ownership is metadata only"},
    {6, "approval-packet-renderer-boundary", "approval packet and template renderer split",
     "approval packet and template rendering ownership is metadata only"},
    {7, "capture-renderer-boundary", "signed approval capture profile section renderer split",
     "capture rendering ownership is metadata only"},
    {8, "approval-packet-draft-section-carried", "Node v986 approval packet draft marker",
     "approval packet draft identity remains metadata only"},
    {9, "approval-packet-review-section-carried", "Node v1011 approval packet review marker",
     "approval packet review identity remains metadata only"},
    {10, "signed-approval-template-section-carried", "Node v1036 signed approval template marker",
     "signed approval template identity remains metadata only"},
    {11, "signed-approval-capture-preflight-section-carried", "Node v1061 capture preflight marker",
     "signed approval capture preflight identity remains metadata only"},
    {12, "signed-approval-capture-artifact-preflight-section-carried", "Node v1086 capture artifact preflight marker",
     "signed approval capture artifact preflight identity remains metadata only"},
    {13, "fail-closed-approval-state-lock", "approval and signed approval false states",
     "approval material and signed approvals remain absent and fail closed"},
    {14, "runtime-secret-false-lock", "runtime payload and secret-value fields",
     "runtime payload and secret value reads remain disabled"},
    {15, "type-catalog-aggregator-order-lock", "Node v1556 catalog order 214",
     "approval aggregator catalog order is recorded without import"},
    {16, "type-catalog-packet-order-lock", "Node v1556 catalog order 215",
     "approval packet renderer catalog order is recorded without import"},
    {17, "type-catalog-capture-order-lock", "Node v1556 catalog order 216",
     "capture renderer catalog order is recorded without import"},
    {18, "profile-entry-order-lock", "Node v1556 catalog order 217",
     "profile-entry-types catalog order is recorded without import"},
    {19, "catalog-validation-version-lock", "Node v1556 validation version",
     "catalog and validation context is metadata only"},
    {20, "route-markdown-parity", "Node v1556 route Markdown closeout",
     "mini-kv records parity without rendering Node Markdown"},
    {21, "main-renderer-size-control", "Node v1556 main renderer size reduction",
     "line-count reduction is maintenance context, not a mini-kv execution dependency"},
    {22, "subrenderer-size-control", "Node v1556 subrenderer size control",
     "subrenderer sizing is metadata only"},
    {23, "parallel-status-recorded", "Node v1556 cross-project parallel plan",
     "Node does not require fresh mini-kv evidence in this range"},
    {24, "no-live-integration-startup", "live integration startup boundary",
     "Java, mini-kv, and sibling service startup remain out of scope"},
    {25, "no-approval-profile-chain-created", "approval profile section closeout",
     "closeout does not open approval material, router, write, or execution"},
}};

std::string format_check_json(const OperatorValueSupplyApprovalProfileSectionCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"approvalProfileSectionOnly\":true"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"approvalMaterialImported\":false"
           ",\"approvalMaterialAccepted\":false"
           ",\"signedApprovalCreated\":false"
           ",\"signedApprovalCaptured\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"runtimePayloadImported\":false"
           ",\"secretValueRead\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const OperatorValueSupplyApprovalProfileSectionCheck>
operator_value_supply_approval_profile_section_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_operator_value_supply_approval_profile_section_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_operator_value_supply_approval_profile_section_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published operator value supply approval profile section check count exceeds check catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_operator_value_supply_approval_profile_section_checks
