#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<SignedApprovalDraftProfileSectionIntegrityCheck, 25> check_catalog = {{
    {1, "source-profile-section-fixture-frozen", "fixtures/release/shard-readiness-v1060.json",
     "v1060 remains the immutable source profile section fixture"},
    {2, "source-profile-section-digest-pinned", "v1060 profile section digest marker",
     "integrity evidence compares against the source profile section digest marker"},
    {3, "source-command-contract-read-only", "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON",
     "source command remains read only mutatesStore false touchesWal false"},
    {4, "five-section-count-lock", "signed approval draft profile section names",
     "the signed approval draft profile section group still contains five sections"},
    {5, "node-marker-lock", "Node v1506 marker set",
     "Node v1111 v1136 v1161 v1186 and v1211 markers remain metadata only"},
    {6, "route-markdown-stability-lock", "Node v1506 route Markdown parity",
     "renderer ownership split does not imply route-facing Markdown changes"},
    {7, "no-fresh-evidence-lock", "Node v1506 cross-project parallel plan",
     "Node does not require fresh mini-kv or Java evidence for this maintenance split"},
    {8, "draft-artifact-false-lock", "draft artifact boundary fields",
     "draft artifact creation and import stay false"},
    {9, "signed-approval-false-lock", "signed approval boundary fields",
     "signed approval creation emission and approval grant capture stay false"},
    {10, "material-intake-false-lock", "material intake boundary fields",
     "material intake and external reviewed material consumption stay disabled"},
    {11, "runtime-payload-false-lock", "runtime payload boundary fields",
     "runtime payload import and acceptance stay disabled"},
    {12, "node-renderer-execution-lock", "renderer execution boundary fields",
     "mini-kv does not execute the Node renderer"},
    {13, "node-catalog-import-lock", "Node type catalog boundary fields",
     "mini-kv does not import or execute the Node catalog"},
    {14, "service-startup-lock", "service startup boundary fields",
     "Node Java and mini-kv services are not started by integrity evidence"},
    {15, "runtime-probe-lock", "runtime probe boundary fields",
     "live reads runtime probes filesystem reads and archive walks remain disabled"},
    {16, "router-lock", "router boundary fields",
     "active signed approval draft routing remains disabled"},
    {17, "write-wal-lock", "write and WAL boundary fields",
     "write routing store mutation and WAL touch remain disabled"},
    {18, "load-restore-compact-lock", "admin command boundary fields",
     "load restore and compact remain disabled"},
    {19, "execution-lock", "execution boundary fields",
     "integrity evidence remains non-executable"},
    {20, "command-catalog-exposure", "COMMANDS and COMMANDSJSON",
     "the integrity command is parser visible and read only"},
    {21, "shardjson-single-embedding", "SHARDJSON",
     "current SHARDJSON embeds exactly one integrity evidence block"},
    {22, "validation-split", "integrity validation module",
     "validation remains separate from the evidence formatter"},
    {23, "current-fixture-rollout", "fixtures/release/shard-readiness.json",
     "rolling current points to v1085 while v1060 stays frozen"},
    {24, "cli-tcp-smoke-parity", "CLI and TCP smoke",
     "smoke checks use the same read-only integrity command"},
    {25, "closeout-no-chain-created", "integrity closeout",
     "closeout does not open approval material governance routing write WAL or execution"},
}};

std::string format_check_json(const SignedApprovalDraftProfileSectionIntegrityCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"integrityOnly\":true"
           ",\"sourceFixtureFrozen\":true"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"draftArtifactCreated\":false"
           ",\"signedApprovalCreated\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"runtimePayloadImported\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const SignedApprovalDraftProfileSectionIntegrityCheck>
signed_approval_artifact_draft_profile_section_integrity_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_signed_approval_artifact_draft_profile_section_integrity_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_signed_approval_artifact_draft_profile_section_integrity_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published signed approval artifact draft profile section integrity check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks
