#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<SignedApprovalDraftTextPackageProfileSectionCheck, 25> check_catalog = {{
    {1, "source-current-fixture-frozen", "fixtures/release/shard-readiness-v1085.json",
     "v1085 remains the immutable source current fixture"},
    {2, "node-renderer-split-plan-pinned", "Node v1531 text package profile section renderer split roadmap",
     "mini-kv treats the split as read-only ownership context"},
    {3, "text-package-section-group-boundary", "Node v1531 nine-section group",
     "only the nine signed approval artifact draft text package profile sections are represented"},
    {4, "text-package-aggregator-boundary", "controlledReadOnlyShardPreviewSignedApprovalArtifactDraftTextPackageProfileSectionRenderer.ts",
     "aggregator ownership is recorded without executing Node code"},
    {5, "submission-renderer-boundary", "text package submission profile section renderer",
     "submission-side rendering ownership is metadata only"},
    {6, "compared-evidence-renderer-boundary", "text package compared-evidence profile section renderer",
     "compared-evidence rendering ownership is metadata only"},
    {7, "text-package-intake-section-carried", "Node v1236 text package intake marker",
     "text package intake section identity remains metadata only"},
    {8, "text-package-review-preflight-section-carried", "Node v1261 text package review preflight marker",
     "review preflight section identity remains metadata only"},
    {9, "text-package-submission-preflight-section-carried", "Node v1286 text package submission preflight marker",
     "submission preflight section identity remains metadata only"},
    {10, "text-package-comparison-preflight-section-carried", "Node v1311 text package comparison preflight marker",
     "comparison preflight section identity remains metadata only"},
    {11, "text-package-comparison-acceptance-precheck-section-carried", "Node v1321 comparison acceptance precheck marker",
     "comparison acceptance precheck section identity remains metadata only"},
    {12, "compared-package-evidence-intake-section-carried", "Node v1331 compared package evidence intake marker",
     "compared package evidence intake section identity remains metadata only"},
    {13, "compared-evidence-evaluation-preflight-section-carried", "Node v1351 compared evidence evaluation preflight marker",
     "compared evidence evaluation preflight section identity remains metadata only"},
    {14, "compared-evidence-candidate-section-carried", "Node v1361 compared evidence candidate marker",
     "compared evidence candidate section identity remains metadata only"},
    {15, "compared-evidence-candidate-intake-section-carried", "Node v1371 compared evidence candidate intake marker",
     "compared evidence candidate intake section identity remains metadata only"},
    {16, "node-version-marker-lock", "Node v1531 marker list",
     "all nine text package section markers remain metadata only"},
    {17, "type-catalog-order-lock", "Node v1531 type module catalog",
     "module orders 211 through 214 are recorded without catalog import"},
    {18, "route-markdown-parity", "Node v1531 route Markdown closeout",
     "mini-kv records parity without rendering Node Markdown"},
    {19, "no-fresh-evidence-required", "Node v1531 cross-project parallel plan",
     "Node does not require fresh mini-kv evidence in this range"},
    {20, "false-flag-lock", "Node v1531 false text package signed approval runtime flags",
     "text package material signed approval runtime and secret-value flags remain false"},
    {21, "write-wal-execution-boundary", "write and execution boundary fields",
     "router write routing store mutation WAL touch load restore compact and execution remain disabled"},
    {22, "command-catalog-exposure", "COMMANDS and COMMANDSJSON",
     "the text package profile section command is parser visible and read only"},
    {23, "shardjson-single-embedding", "SHARDJSON",
     "current SHARDJSON embeds exactly one text package profile section evidence block"},
    {24, "validation-split-cli-tcp-smoke", "validation plus CLI/TCP smoke",
     "validation and smoke checks use the same read-only text package profile section command"},
    {25, "no-text-package-chain-created", "profile section closeout",
     "closeout does not open signed approval, material intake, governance, or execution"},
}};

std::string format_check_json(const SignedApprovalDraftTextPackageProfileSectionCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"textPackageProfileSectionOnly\":true"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"textPackageMaterialImported\":false"
           ",\"textPackageMaterialAccepted\":false"
           ",\"signedApprovalCreated\":false"
           ",\"draftArtifactCreated\":false"
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

std::span<const SignedApprovalDraftTextPackageProfileSectionCheck>
signed_approval_artifact_draft_text_package_profile_section_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_signed_approval_artifact_draft_text_package_profile_section_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_signed_approval_artifact_draft_text_package_profile_section_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published signed approval artifact draft text package profile section check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks
