#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheck, 35> check_catalog = {{
    {1, "source-archive-verification-non-participation-fixture-frozen", "fixtures/release/shard-readiness-v1260.json",
     "v1260 remains the immutable source archive verification non-participation fixture"},
    {2, "node-release-review-entrypoint-reexport-context", "Node v1688",
     "stable release review entrypoint reexports remain Node-owned metadata"},
    {3, "node-release-review-build-parts-context", "Node v1689",
     "release review build parts are not imported into mini-kv"},
    {4, "node-java-field-guide-reference-context", "Node v1690",
     "Java v51 field guide constants do not make mini-kv Java authoritative"},
    {5, "node-mini-kv-field-guide-reference-context", "Node v1691",
     "mini-kv v60 field guide constants remain historical references only"},
    {6, "node-release-review-endpoint-constant-context", "Node v1692",
     "release review endpoint constants do not become raw endpoint reads"},
    {7, "node-release-semantics-context", "Node v1693",
     "release semantics selection is not accepted as mini-kv runtime state"},
    {8, "node-release-review-digest-generation-context", "Node v1694",
     "release review digest generation remains Node-owned"},
    {9, "node-release-review-check-assembly-context", "Node v1695",
     "release review check assembly does not become a mini-kv command"},
    {10, "node-ready-for-release-review-context", "Node v1696",
     "ready-for-read-only-capture-release-evidence-review evaluation does not start mini-kv services"},
    {11, "node-review-state-resolution-context", "Node v1697",
     "review-state resolution is not accepted as mini-kv runtime state"},
    {12, "node-field-guide-readiness-context", "Node v1698",
     "field guide readiness validation remains Node-owned"},
    {13, "node-non-pass-field-guide-policy-context", "Node v1699",
     "non-pass-only field guide policy checks do not alter mini-kv behavior"},
    {14, "node-skipped-mixed-capture-policy-context", "Node v1700",
     "skipped or mixed capture non-pass policy checks do not open mini-kv routes"},
    {15, "node-upstream-action-auto-start-safety-context", "Node v1701",
     "upstream actions and automatic-start safety checks do not start mini-kv"},
    {16, "node-no-execution-authorization-context", "Node v1702",
     "no-execution-authorization review checks do not grant mini-kv execution authority"},
    {17, "node-production-blocker-context", "Node v1703",
     "production blocker collection stays inside Node"},
    {18, "node-release-review-warning-context", "Node v1704",
     "warning generation stays inside Node"},
    {19, "node-release-review-recommendation-context", "Node v1705",
     "recommendation generation stays inside Node"},
    {20, "node-release-review-summary-count-context", "Node v1706",
     "summary counts do not become mini-kv release review records"},
    {21, "node-release-review-markdown-renderer-context", "Node v1707",
     "Markdown rendering is not executed from mini-kv"},
    {22, "node-release-review-renderer-reexport-context", "Node v1708",
     "stable renderer reexports remain read-only sibling metadata"},
    {23, "node-release-review-loader-orchestration-context", "Node v1709",
     "loader orchestration remains Node-owned and is not executed by mini-kv"},
    {24, "node-release-review-renderer-regression-context", "Node v1710",
     "renderer reexport regression coverage remains Node-owned"},
    {25, "node-field-guide-behavior-coverage-context", "Node v1711",
     "Java v51 and mini-kv v60 field-guide behavior coverage remains sibling context only"},
    {26, "node-blocked-release-review-behavior-context", "Node v1712",
     "blocked release evidence review behavior does not enable mini-kv writes or execution"},
    {27, "node-release-review-route-coverage-context", "Node v1713",
     "JSON and Markdown route coverage does not install a mini-kv route"},
    {28, "node-archive-verification-upstream-consumer-context", "Node v1714",
     "archive verification upstream consumer validation remains read-only sibling context"},
    {29, "node-status-live-probe-route-wiring-context", "Node v1715",
     "status live probe route wiring does not open mini-kv services"},
    {30, "node-typecheck-focused-release-review-context", "Node v1716",
     "typecheck and focused tests require no fresh mini-kv evidence"},
    {31, "node-adjacent-build-context", "Node v1717",
     "adjacent tests and build validation remain Node-owned"},
    {32, "node-http-smoke-release-review-context", "Node v1718",
     "HTTP smoke for JSON and Markdown remains Node-only"},
    {33, "node-segmented-full-vitest-context", "Node v1719",
     "segmented full Vitest verification does not trigger mini-kv execution"},
    {34, "node-clean-output-commit-prep-context", "Node v1720",
     "generated validation cleanup and commit preparation do not create mini-kv cleanup authority"},
    {35, "no-release-evidence-review-chain-created", "Node v1721 / mini-kv v1295",
     "closeout opens no release review endpoint raw endpoint payload router write routing WAL or execution authority"},
}};

std::string format_check_json(const ProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"releaseEvidenceReviewNonParticipationOnly\":true"
           ",\"sourceArchiveVerificationNonParticipationFixtureFrozen\":true"
           ",\"nodeReleaseEvidenceReviewModuleSplitObserved\":true"
           ",\"nodeReleaseEvidenceReviewExecuted\":false"
           ",\"nodeReleaseReviewLoaderExecuted\":false"
           ",\"nodeReleaseReviewRendererExecuted\":false"
           ",\"nodeReleaseReviewEndpointReadByMiniKv\":false"
           ",\"nodeReleaseReviewFixtureImportedByMiniKv\":false"
           ",\"nodeReleaseReviewPayloadAcceptedByMiniKv\":false"
           ",\"nodeReleaseReviewDigestGeneratedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvReleaseEvidenceReviewStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"releaseReviewEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"releaseReviewPayloadImported\":false"
           ",\"releaseReviewPayloadAccepted\":false"
           ",\"releaseReviewFixtureImported\":false"
           ",\"releaseReviewStateAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"releaseEvidenceReviewRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const ProductionLiveCaptureReleaseEvidenceReviewNonParticipationCheck>
production_live_capture_release_evidence_review_non_participation_checks() {
    return {check_catalog.data(), check_catalog.size()};
}

int planned_production_live_capture_release_evidence_review_non_participation_check_count() {
    return static_cast<int>(check_catalog.size());
}

std::string format_production_live_capture_release_evidence_review_non_participation_checks_json(
    int published_check_count) {
    if (published_check_count < 0 || published_check_count > static_cast<int>(check_catalog.size())) {
        throw std::out_of_range("production live capture release evidence review check count out of range");
    }

    std::string json = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            json += ",";
        }
        json += format_check_json(check_catalog[static_cast<std::size_t>(index)]);
    }
    json += "]";
    return json;
}

} // namespace minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_checks
