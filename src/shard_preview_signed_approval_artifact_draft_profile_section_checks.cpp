#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<SignedApprovalDraftProfileSectionCheck, 25> check_catalog = {{
    {1, "source-current-fixture-frozen", "fixtures/release/shard-readiness-v1035.json",
     "v1035 remains the immutable source current fixture"},
    {2, "node-renderer-split-plan-pinned", "Node v1506 signed approval draft renderer split roadmap",
     "mini-kv treats the split as read-only ownership context"},
    {3, "signed-approval-draft-section-group-boundary", "Node v1506 five-section group",
     "only the five signed approval artifact draft profile sections are represented"},
    {4, "heading-catalog-declared", "signed approval draft profile section stage catalog",
     "section headings are explicit and versioned"},
    {5, "draft-preflight-section-carried", "signed approval artifact draft preflight lineage",
     "draft preflight section identity remains metadata only"},
    {6, "draft-readiness-section-carried", "signed approval artifact draft readiness lineage",
     "draft readiness section identity remains metadata only"},
    {7, "review-package-section-carried", "signed approval artifact draft review package lineage",
     "review package preflight section identity remains metadata only"},
    {8, "authoring-readiness-section-carried", "signed approval artifact draft authoring readiness lineage",
     "authoring readiness section identity remains metadata only"},
    {9, "instruction-preflight-section-carried", "signed approval artifact draft instruction preflight lineage",
     "instruction preflight section identity remains metadata only"},
    {10, "node-version-marker-lock", "Node v1506 marker list",
     "Node v1111 v1136 v1161 v1186 and v1211 markers remain metadata only"},
    {11, "renderer-output-stability", "Node v1506 maintenance split",
     "renderer ownership changes do not imply route-facing output changes"},
    {12, "route-markdown-parity", "Node v1506 route Markdown closeout",
     "mini-kv records parity without rendering Node Markdown"},
    {13, "no-fresh-evidence-required", "Node v1506 cross-project parallel plan",
     "Node does not require fresh mini-kv evidence in this range"},
    {14, "false-flag-lock", "Node v1506 false draft artifact signed approval runtime flags",
     "draft artifact signed approval and runtime payload flags remain false"},
    {15, "type-catalog-compatibility", "Node v1506 type module catalog",
     "mini-kv does not import or execute the Node catalog"},
    {16, "live-startup-boundary", "service startup boundary fields",
     "Node Java and mini-kv services are not started by this evidence"},
    {17, "runtime-probe-boundary", "runtime boundary fields",
     "live reads and runtime probes remain disabled"},
    {18, "router-boundary", "router boundary fields", "active signed approval draft routing remains disabled"},
    {19, "write-wal-boundary", "write boundary fields",
     "write routing store mutation WAL touch load restore and compact remain disabled"},
    {20, "execution-boundary", "execution boundary fields", "profile section evidence remains non-executable"},
    {21, "command-catalog-exposure", "COMMANDS and COMMANDSJSON",
     "the signed approval draft profile section command is parser visible and read only"},
    {22, "shardjson-single-embedding", "SHARDJSON",
     "current SHARDJSON embeds exactly one signed approval draft profile section evidence block"},
    {23, "validation-split", "signed approval draft profile section validation module",
     "validation stays separate from the evidence formatter"},
    {24, "cli-tcp-smoke-parity", "CLI and TCP smoke",
     "smoke checks use the same read-only signed approval draft profile section command"},
    {25, "no-approval-chain-created", "profile section closeout",
     "closeout does not open signed approval, material intake, governance, or execution"},
}};

std::string format_check_json(const SignedApprovalDraftProfileSectionCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"profileSectionOnly\":true"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"signedApprovalCreated\":false"
           ",\"draftArtifactCreated\":false"
           ",\"runtimePayloadImported\":false"
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

std::span<const SignedApprovalDraftProfileSectionCheck> signed_approval_artifact_draft_profile_section_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_signed_approval_artifact_draft_profile_section_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_signed_approval_artifact_draft_profile_section_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published signed approval artifact draft profile section check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_checks
