#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages.hpp"

#include <array>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_signed_approval_artifact_draft_profile_section_integrity_release_number_value = 1061;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1061", "route-preview-signed-approval-artifact-draft-profile-section-integrity-source-freeze",
     "v1060", "fixtures/release/shard-readiness-v1060.json",
     "freezes the v1060 signed approval draft profile section fixture before integrity evidence starts"},
    {2, "v1062", "route-preview-signed-approval-artifact-draft-profile-section-integrity-source-digest-pin",
     "v1061", "fixtures/release/shard-readiness-v1061.json",
     "pins the v1060 profile section digest marker as the only integrity source"},
    {3, "v1063", "route-preview-signed-approval-artifact-draft-profile-section-integrity-command-contract-lock",
     "v1062", "fixtures/release/shard-readiness-v1062.json",
     "locks the source profile section command as read-only metadata"},
    {4, "v1064", "route-preview-signed-approval-artifact-draft-profile-section-integrity-section-count",
     "v1063", "fixtures/release/shard-readiness-v1063.json",
     "checks the five signed approval artifact draft profile sections remain five"},
    {5, "v1065", "route-preview-signed-approval-artifact-draft-profile-section-integrity-node-marker-lock",
     "v1064", "fixtures/release/shard-readiness-v1064.json",
     "keeps Node v1506 and its five historical draft markers as metadata only"},
    {6, "v1066", "route-preview-signed-approval-artifact-draft-profile-section-integrity-route-markdown-stability",
     "v1065", "fixtures/release/shard-readiness-v1065.json",
     "keeps route-facing Markdown stability as an integrity condition"},
    {7, "v1067", "route-preview-signed-approval-artifact-draft-profile-section-integrity-no-fresh-evidence",
     "v1066", "fixtures/release/shard-readiness-v1066.json",
     "rechecks that Node v1506 requires no fresh mini-kv or Java evidence"},
    {8, "v1068", "route-preview-signed-approval-artifact-draft-profile-section-integrity-draft-artifact-false-lock",
     "v1067", "fixtures/release/shard-readiness-v1067.json",
     "keeps draft artifact creation and import flags false"},
    {9, "v1069", "route-preview-signed-approval-artifact-draft-profile-section-integrity-signed-approval-false-lock",
     "v1068", "fixtures/release/shard-readiness-v1068.json",
     "keeps signed approval creation emission and approval grant capture false"},
    {10, "v1070", "route-preview-signed-approval-artifact-draft-profile-section-integrity-material-intake-lock",
     "v1069", "fixtures/release/shard-readiness-v1069.json",
     "keeps material intake and external reviewed material consumption disabled"},
    {11, "v1071", "route-preview-signed-approval-artifact-draft-profile-section-integrity-runtime-payload-lock",
     "v1070", "fixtures/release/shard-readiness-v1070.json",
     "keeps runtime payload import and acceptance disabled"},
    {12, "v1072", "route-preview-signed-approval-artifact-draft-profile-section-integrity-node-renderer-execution-lock",
     "v1071", "fixtures/release/shard-readiness-v1071.json",
     "keeps Node renderer execution outside mini-kv integrity evidence"},
    {13, "v1073", "route-preview-signed-approval-artifact-draft-profile-section-integrity-node-catalog-import-lock",
     "v1072", "fixtures/release/shard-readiness-v1072.json",
     "keeps Node catalog import and type catalog execution disabled"},
    {14, "v1074", "route-preview-signed-approval-artifact-draft-profile-section-integrity-service-startup-lock",
     "v1073", "fixtures/release/shard-readiness-v1073.json",
     "keeps Node Java and mini-kv service startup outside integrity evidence"},
    {15, "v1075", "route-preview-signed-approval-artifact-draft-profile-section-integrity-runtime-probe-lock",
     "v1074", "fixtures/release/shard-readiness-v1074.json",
     "keeps runtime probes live reads and filesystem archive walks disabled"},
    {16, "v1076", "route-preview-signed-approval-artifact-draft-profile-section-integrity-router-lock",
     "v1075", "fixtures/release/shard-readiness-v1075.json",
     "keeps active signed approval draft routing disabled"},
    {17, "v1077", "route-preview-signed-approval-artifact-draft-profile-section-integrity-write-wal-lock",
     "v1076", "fixtures/release/shard-readiness-v1076.json",
     "keeps write routing store mutation and WAL touch disabled"},
    {18, "v1078", "route-preview-signed-approval-artifact-draft-profile-section-integrity-load-restore-compact-lock",
     "v1077", "fixtures/release/shard-readiness-v1077.json",
     "keeps load restore and compact outside the integrity command"},
    {19, "v1079", "route-preview-signed-approval-artifact-draft-profile-section-integrity-execution-lock",
     "v1078", "fixtures/release/shard-readiness-v1078.json",
     "keeps signed approval draft profile section integrity non-executable"},
    {20, "v1080", "route-preview-signed-approval-artifact-draft-profile-section-integrity-command-catalog-exposure",
     "v1079", "fixtures/release/shard-readiness-v1079.json",
     "exposes the integrity command as read-only metadata"},
    {21, "v1081", "route-preview-signed-approval-artifact-draft-profile-section-integrity-shardjson-embedding",
     "v1080", "fixtures/release/shard-readiness-v1080.json",
     "embeds one signed approval draft profile section integrity block in SHARDJSON"},
    {22, "v1082", "route-preview-signed-approval-artifact-draft-profile-section-integrity-validation-split",
     "v1081", "fixtures/release/shard-readiness-v1081.json",
     "keeps integrity validation separate from the evidence formatter"},
    {23, "v1083", "route-preview-signed-approval-artifact-draft-profile-section-integrity-current-fixture-rollout",
     "v1082", "fixtures/release/shard-readiness-v1082.json",
     "rolls current shard readiness to the integrity evidence while preserving source fixture lineage"},
    {24, "v1084", "route-preview-signed-approval-artifact-draft-profile-section-integrity-cli-tcp-smoke",
     "v1083", "fixtures/release/shard-readiness-v1083.json",
     "verifies CLI and TCP smoke through the read-only integrity command"},
    {25, "v1085", "route-preview-signed-approval-artifact-draft-profile-section-integrity-closeout-summary",
     "v1084", "fixtures/release/shard-readiness-v1084.json",
     "closes profile section integrity without creating approval material or execution authority"},
}};

} // namespace

std::span<const StageRecord> signed_approval_artifact_draft_profile_section_integrity_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_signed_approval_artifact_draft_profile_section_integrity_release_number() {
    return first_signed_approval_artifact_draft_profile_section_integrity_release_number_value;
}

int planned_signed_approval_artifact_draft_profile_section_integrity_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages
