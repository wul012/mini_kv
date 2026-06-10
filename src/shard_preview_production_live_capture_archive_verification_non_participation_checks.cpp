#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ProductionLiveCaptureArchiveVerificationNonParticipationCheck, 35> check_catalog = {{
    {1, "source-archive-non-participation-fixture-frozen", "fixtures/release/shard-readiness-v1225.json",
     "v1225 remains the immutable source archive non-participation fixture"},
    {2, "node-verification-entrypoint-reexport-context", "Node v1658",
     "stable verification entrypoint reexports remain Node-owned metadata"},
    {3, "node-verification-build-parts-context", "Node v1659",
     "verification build parts are not imported into mini-kv"},
    {4, "node-expected-archive-digest-recompute-context", "Node v1660",
     "expected archive digest recomputation is not performed by mini-kv"},
    {5, "node-verification-digest-generation-context", "Node v1661",
     "verification digest generation remains Node-owned"},
    {6, "node-upstream-reference-constant-context", "Node v1662",
     "Java v50 and mini-kv v59 reference constants do not make mini-kv order authoritative"},
    {7, "node-verification-endpoint-constant-context", "Node v1663",
     "verification endpoint constants do not become raw endpoint reads"},
    {8, "node-verification-check-assembly-context", "Node v1664",
     "verification check assembly does not become a mini-kv command"},
    {9, "node-ready-for-verification-context", "Node v1665",
     "ready-for-read-only-capture-archive-verification evaluation does not start mini-kv services"},
    {10, "node-verification-state-resolution-context", "Node v1666",
     "verification-state resolution is not accepted as mini-kv runtime state"},
    {11, "node-archive-digest-match-context", "Node v1667",
     "archive digest match validation remains Node-owned"},
    {12, "node-archive-profile-readiness-context", "Node v1668",
     "archive profile version and readiness checks are not imported as mini-kv profiles"},
    {13, "node-upstream-reference-readiness-context", "Node v1669",
     "Java and mini-kv upstream reference readiness checks are sibling context only"},
    {14, "node-skipped-mixed-capture-policy-context", "Node v1670",
     "skipped and mixed capture non-pass policy checks do not alter mini-kv behavior"},
    {15, "node-upstream-action-auto-start-safety-context", "Node v1671",
     "upstream actions and automatic-start safety checks do not start mini-kv"},
    {16, "node-production-blocker-context", "Node v1672",
     "production blocker collection stays inside Node"},
    {17, "node-verification-warning-context", "Node v1673",
     "warning generation stays inside Node"},
    {18, "node-verification-recommendation-context", "Node v1674",
     "recommendation generation stays inside Node"},
    {19, "node-verification-summary-count-context", "Node v1675",
     "summary counts do not become mini-kv verification records"},
    {20, "node-verification-markdown-renderer-context", "Node v1676",
     "Markdown rendering is not executed from mini-kv"},
    {21, "node-verification-renderer-reexport-context", "Node v1677",
     "stable renderer reexports remain read-only sibling metadata"},
    {22, "node-verification-loader-orchestration-context", "Node v1678",
     "loader orchestration remains Node-owned and is not executed by mini-kv"},
    {23, "node-verification-renderer-regression-context", "Node v1679",
     "renderer reexport regression coverage remains Node-owned"},
    {24, "node-verification-skipped-behavior-context", "Node v1680",
     "skipped behavior coverage does not open mini-kv routes"},
    {25, "node-verification-blocked-behavior-context", "Node v1681",
     "blocked behavior coverage does not enable mini-kv writes or execution"},
    {26, "node-verification-route-coverage-context", "Node v1682",
     "JSON and Markdown route coverage does not install a mini-kv route"},
    {27, "node-release-evidence-review-consumer-context", "Node v1683",
     "release evidence review consumer validation remains read-only sibling context"},
    {28, "node-typecheck-focused-adjacent-build-context", "Node v1684",
     "typecheck focused adjacent and build validation require no fresh mini-kv evidence"},
    {29, "node-http-smoke-segmented-cleanup-context", "Node v1685",
     "HTTP smoke segmented full tests and cleanup validation remain Node-owned"},
    {30, "node-verification-closeout-context", "Node v1686",
     "closeout commits tags push and GitHub Actions are recorded without mini-kv participation"},
    {31, "mini-kv-command-catalog-parity", "mini-kv v1256",
     "command catalog declares read-only no-WAL stable evidence behavior for archive verification non-participation"},
    {32, "mini-kv-fixture-export-parity", "mini-kv v1257",
     "versioned fixtures export the archive verification section without changing source evidence"},
    {33, "mini-kv-cli-tcp-smoke-parity", "mini-kv v1258",
     "real CLI and TCP smoke read evidence without services writes WAL or execution"},
    {34, "mini-kv-archive-explanation-parity", "mini-kv v1259",
     "archive screenshots and explanations describe no router no write no execution behavior"},
    {35, "no-archive-verification-chain-created", "mini-kv v1260",
     "closeout opens no verification endpoint raw endpoint payload router write routing WAL or execution authority"},
}};

std::string format_check_json(const ProductionLiveCaptureArchiveVerificationNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"verificationNonParticipationOnly\":true"
           ",\"sourceArchiveNonParticipationFixtureFrozen\":true"
           ",\"nodeArchiveVerificationModuleSplitObserved\":true"
           ",\"nodeCaptureArchiveVerificationExecuted\":false"
           ",\"nodeArchiveVerificationLoaderExecuted\":false"
           ",\"nodeArchiveVerificationRendererExecuted\":false"
           ",\"nodeReleaseEvidenceReviewExecuted\":false"
           ",\"nodeVerificationEndpointReadByMiniKv\":false"
           ",\"nodeVerificationFixtureImportedByMiniKv\":false"
           ",\"nodeVerificationPayloadAcceptedByMiniKv\":false"
           ",\"archiveDigestRecomputedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvCaptureArchiveVerificationStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"verificationEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"verificationPayloadImported\":false"
           ",\"verificationPayloadAccepted\":false"
           ",\"verificationFixtureImported\":false"
           ",\"verificationStateAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"archiveVerificationRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const ProductionLiveCaptureArchiveVerificationNonParticipationCheck>
production_live_capture_archive_verification_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_production_live_capture_archive_verification_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_production_live_capture_archive_verification_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > check_catalog.size()) {
        throw std::out_of_range{
            "published production live capture archive verification check count exceeds check catalog size"};
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

} // namespace minikv::shard_preview_production_live_capture_archive_verification_non_participation_checks
