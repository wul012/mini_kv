#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_stages.hpp"

#include <array>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_signed_approval_artifact_draft_profile_section_release_number_value = 1036;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1036", "route-preview-signed-approval-artifact-draft-profile-section-source-freeze", "v1035",
     "fixtures/release/shard-readiness-v1035.json",
     "freezes the v1035 candidate profile section fixture before signed approval draft profile section evidence starts"},
    {2, "v1037", "route-preview-signed-approval-artifact-draft-profile-section-node-renderer-split-plan-pin", "v1036",
     "fixtures/release/shard-readiness-v1036.json",
     "pins Node v1506 as signed approval artifact draft renderer ownership context"},
    {3, "v1038", "route-preview-signed-approval-artifact-draft-profile-section-group-boundary", "v1037",
     "fixtures/release/shard-readiness-v1037.json",
     "declares the five signed approval artifact draft profile sections as a read-only group"},
    {4, "v1039", "route-preview-signed-approval-artifact-draft-profile-section-heading-catalog", "v1038",
     "fixtures/release/shard-readiness-v1038.json",
     "catalogs signed approval artifact draft headings without changing route Markdown"},
    {5, "v1040", "route-preview-signed-approval-artifact-draft-profile-section-preflight-carry-forward", "v1039",
     "fixtures/release/shard-readiness-v1039.json",
     "carries the signed approval artifact draft preflight section as metadata only"},
    {6, "v1041", "route-preview-signed-approval-artifact-draft-profile-section-readiness-carry-forward", "v1040",
     "fixtures/release/shard-readiness-v1040.json",
     "carries the signed approval artifact draft readiness section as metadata only"},
    {7, "v1042", "route-preview-signed-approval-artifact-draft-profile-section-review-package-carry-forward", "v1041",
     "fixtures/release/shard-readiness-v1041.json",
     "carries the signed approval artifact draft review package preflight section as metadata only"},
    {8, "v1043", "route-preview-signed-approval-artifact-draft-profile-section-authoring-readiness-carry-forward", "v1042",
     "fixtures/release/shard-readiness-v1042.json",
     "carries the signed approval artifact draft authoring readiness section as metadata only"},
    {9, "v1044", "route-preview-signed-approval-artifact-draft-profile-section-instruction-preflight-carry-forward", "v1043",
     "fixtures/release/shard-readiness-v1043.json",
     "carries the signed approval artifact draft instruction preflight section as metadata only"},
    {10, "v1045", "route-preview-signed-approval-artifact-draft-profile-section-version-marker-lock", "v1044",
     "fixtures/release/shard-readiness-v1044.json",
     "locks Node v1111 v1136 v1161 v1186 and v1211 marker expectations as metadata"},
    {11, "v1046", "route-preview-signed-approval-artifact-draft-profile-section-renderer-output-stability", "v1045",
     "fixtures/release/shard-readiness-v1045.json",
     "records that renderer ownership changed while route-facing output stays stable"},
    {12, "v1047", "route-preview-signed-approval-artifact-draft-profile-section-route-markdown-parity", "v1046",
     "fixtures/release/shard-readiness-v1046.json",
     "keeps route Markdown parity as a read-only contract statement"},
    {13, "v1048", "route-preview-signed-approval-artifact-draft-profile-section-no-fresh-evidence-requirement", "v1047",
     "fixtures/release/shard-readiness-v1047.json",
     "records that Node v1506 does not require fresh Java or mini-kv evidence"},
    {14, "v1049", "route-preview-signed-approval-artifact-draft-profile-section-false-flag-lock", "v1048",
     "fixtures/release/shard-readiness-v1048.json",
     "locks false draft artifact signed approval and runtime payload flags"},
    {15, "v1050", "route-preview-signed-approval-artifact-draft-profile-section-type-catalog-compatibility", "v1049",
     "fixtures/release/shard-readiness-v1049.json",
     "records Node type catalog compatibility without importing the catalog"},
    {16, "v1051", "route-preview-signed-approval-artifact-draft-profile-section-live-startup-boundary", "v1050",
     "fixtures/release/shard-readiness-v1050.json",
     "keeps Node Java and mini-kv service startup outside the profile section evidence"},
    {17, "v1052", "route-preview-signed-approval-artifact-draft-profile-section-runtime-probe-boundary", "v1051",
     "fixtures/release/shard-readiness-v1051.json",
     "keeps runtime probes and live reads disabled"},
    {18, "v1053", "route-preview-signed-approval-artifact-draft-profile-section-router-boundary", "v1052",
     "fixtures/release/shard-readiness-v1052.json",
     "keeps active signed approval draft routing disabled"},
    {19, "v1054", "route-preview-signed-approval-artifact-draft-profile-section-write-wal-boundary", "v1053",
     "fixtures/release/shard-readiness-v1053.json",
     "keeps write routing store mutation WAL touch load restore and compact disabled"},
    {20, "v1055", "route-preview-signed-approval-artifact-draft-profile-section-execution-boundary", "v1054",
     "fixtures/release/shard-readiness-v1054.json",
     "keeps signed approval draft profile section evidence non-executable"},
    {21, "v1056", "route-preview-signed-approval-artifact-draft-profile-section-command-catalog-exposure", "v1055",
     "fixtures/release/shard-readiness-v1055.json",
     "exposes the signed approval draft profile section command as read-only metadata"},
    {22, "v1057", "route-preview-signed-approval-artifact-draft-profile-section-shardjson-embedding", "v1056",
     "fixtures/release/shard-readiness-v1056.json",
     "embeds one signed approval draft profile section evidence block in SHARDJSON"},
    {23, "v1058", "route-preview-signed-approval-artifact-draft-profile-section-validation-split", "v1057",
     "fixtures/release/shard-readiness-v1057.json",
     "keeps validation checks in a separate module from the evidence body"},
    {24, "v1059", "route-preview-signed-approval-artifact-draft-profile-section-cli-tcp-smoke", "v1058",
     "fixtures/release/shard-readiness-v1058.json",
     "keeps CLI and TCP smoke scoped to the read-only signed approval draft profile section command"},
    {25, "v1060", "route-preview-signed-approval-artifact-draft-profile-section-closeout-summary", "v1059",
     "fixtures/release/shard-readiness-v1059.json",
     "closes the signed approval draft profile section evidence without opening approval or execution"},
}};

} // namespace

std::span<const StageRecord> signed_approval_artifact_draft_profile_section_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_signed_approval_artifact_draft_profile_section_release_number() {
    return first_signed_approval_artifact_draft_profile_section_release_number_value;
}

int planned_signed_approval_artifact_draft_profile_section_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_stages
