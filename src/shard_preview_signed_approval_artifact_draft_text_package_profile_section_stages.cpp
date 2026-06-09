#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages.hpp"

#include <array>

namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1086;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1086", "route-preview-signed-approval-artifact-draft-text-package-profile-section-source-freeze", "v1085",
     "fixtures/release/shard-readiness-v1085.json",
     "freezes the v1085 signed approval profile-section integrity fixture before text package profile evidence starts"},
    {2, "v1087", "route-preview-signed-approval-artifact-draft-text-package-profile-section-node-renderer-split-plan-pin",
     "v1086", "fixtures/release/shard-readiness-v1086.json",
     "pins Node v1531 as signed approval artifact draft text package renderer ownership context"},
    {3, "v1088", "route-preview-signed-approval-artifact-draft-text-package-profile-section-group-boundary", "v1087",
     "fixtures/release/shard-readiness-v1087.json",
     "declares the nine signed approval artifact draft text package profile sections as a read-only group"},
    {4, "v1089", "route-preview-signed-approval-artifact-draft-text-package-profile-section-aggregator-boundary",
     "v1088", "fixtures/release/shard-readiness-v1088.json",
     "records the text package profile section aggregator without executing Node rendering"},
    {5, "v1090", "route-preview-signed-approval-artifact-draft-text-package-profile-section-submission-renderer-boundary",
     "v1089", "fixtures/release/shard-readiness-v1089.json",
     "records the submission-side text package profile renderer as metadata only"},
    {6, "v1091", "route-preview-signed-approval-artifact-draft-text-package-profile-section-compared-renderer-boundary",
     "v1090", "fixtures/release/shard-readiness-v1090.json",
     "records the compared-evidence text package profile renderer as metadata only"},
    {7, "v1092", "route-preview-signed-approval-artifact-draft-text-package-profile-section-intake-carry-forward",
     "v1091", "fixtures/release/shard-readiness-v1091.json",
     "carries the text package intake section and Node v1236 marker as metadata only"},
    {8, "v1093", "route-preview-signed-approval-artifact-draft-text-package-profile-section-review-preflight-carry-forward",
     "v1092", "fixtures/release/shard-readiness-v1092.json",
     "carries the text package review preflight section and Node v1261 marker as metadata only"},
    {9, "v1094", "route-preview-signed-approval-artifact-draft-text-package-profile-section-submission-preflight-carry-forward",
     "v1093", "fixtures/release/shard-readiness-v1093.json",
     "carries the text package submission preflight section and Node v1286 marker as metadata only"},
    {10, "v1095",
     "route-preview-signed-approval-artifact-draft-text-package-profile-section-comparison-preflight-carry-forward",
     "v1094", "fixtures/release/shard-readiness-v1094.json",
     "carries the text package comparison preflight section and Node v1311 marker as metadata only"},
    {11, "v1096",
     "route-preview-signed-approval-artifact-draft-text-package-profile-section-comparison-acceptance-precheck-carry-forward",
     "v1095", "fixtures/release/shard-readiness-v1095.json",
     "carries the text package comparison acceptance precheck section and Node v1321 marker as metadata only"},
    {12, "v1097",
     "route-preview-signed-approval-artifact-draft-text-package-profile-section-compared-package-evidence-intake-carry-forward",
     "v1096", "fixtures/release/shard-readiness-v1096.json",
     "carries the compared package evidence intake section and Node v1331 marker as metadata only"},
    {13, "v1098",
     "route-preview-signed-approval-artifact-draft-text-package-profile-section-compared-evidence-evaluation-preflight-carry-forward",
     "v1097", "fixtures/release/shard-readiness-v1097.json",
     "carries the compared evidence evaluation preflight section and Node v1351 marker as metadata only"},
    {14, "v1099",
     "route-preview-signed-approval-artifact-draft-text-package-profile-section-compared-evidence-candidate-carry-forward",
     "v1098", "fixtures/release/shard-readiness-v1098.json",
     "carries the compared evidence candidate section and Node v1361 marker as metadata only"},
    {15, "v1100",
     "route-preview-signed-approval-artifact-draft-text-package-profile-section-compared-evidence-candidate-intake-carry-forward",
     "v1099", "fixtures/release/shard-readiness-v1099.json",
     "carries the compared evidence candidate intake section and Node v1371 marker as metadata only"},
    {16, "v1101", "route-preview-signed-approval-artifact-draft-text-package-profile-section-version-marker-lock",
     "v1100", "fixtures/release/shard-readiness-v1100.json",
     "locks Node v1236 v1261 v1286 v1311 v1321 v1331 v1351 v1361 and v1371 markers as metadata"},
    {17, "v1102", "route-preview-signed-approval-artifact-draft-text-package-profile-section-type-catalog-order-lock",
     "v1101", "fixtures/release/shard-readiness-v1101.json",
     "records Node v1531 type catalog orders 211 through 214 without importing the catalog"},
    {18, "v1103", "route-preview-signed-approval-artifact-draft-text-package-profile-section-route-markdown-parity",
     "v1102", "fixtures/release/shard-readiness-v1102.json",
     "keeps route Markdown parity as a read-only contract statement"},
    {19, "v1104", "route-preview-signed-approval-artifact-draft-text-package-profile-section-no-fresh-evidence-requirement",
     "v1103", "fixtures/release/shard-readiness-v1103.json",
     "records that Node v1531 does not require fresh Java or mini-kv evidence"},
    {20, "v1105", "route-preview-signed-approval-artifact-draft-text-package-profile-section-false-flag-lock",
     "v1104", "fixtures/release/shard-readiness-v1104.json",
     "locks false text package material signed approval runtime and secret-value flags"},
    {21, "v1106", "route-preview-signed-approval-artifact-draft-text-package-profile-section-write-wal-execution-boundary",
     "v1105", "fixtures/release/shard-readiness-v1105.json",
     "keeps router write routing store mutation WAL touch load restore compact and execution disabled"},
    {22, "v1107", "route-preview-signed-approval-artifact-draft-text-package-profile-section-command-catalog-exposure",
     "v1106", "fixtures/release/shard-readiness-v1106.json",
     "exposes the signed approval draft text package profile section command as read-only metadata"},
    {23, "v1108", "route-preview-signed-approval-artifact-draft-text-package-profile-section-shardjson-embedding",
     "v1107", "fixtures/release/shard-readiness-v1107.json",
     "embeds one signed approval draft text package profile section evidence block in SHARDJSON"},
    {24, "v1109", "route-preview-signed-approval-artifact-draft-text-package-profile-section-validation-split-cli-tcp-smoke",
     "v1108", "fixtures/release/shard-readiness-v1108.json",
     "keeps validation split and CLI/TCP smoke scoped to the read-only text package profile section command"},
    {25, "v1110", "route-preview-signed-approval-artifact-draft-text-package-profile-section-closeout-summary",
     "v1109", "fixtures/release/shard-readiness-v1109.json",
     "closes the text package profile section evidence without opening approval material or execution"},
}};

} // namespace

std::span<const StageRecord> signed_approval_artifact_draft_text_package_profile_section_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_signed_approval_artifact_draft_text_package_profile_section_release_number() {
    return first_release_number_value;
}

int planned_signed_approval_artifact_draft_text_package_profile_section_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages
