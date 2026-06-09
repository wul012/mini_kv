#include "minikv/shard_preview_candidate_profile_section_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_profile_section_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_profile_section_release_number_value = 1011;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1011", "route-preview-candidate-profile-section-source-freeze", "v1010",
     "fixtures/release/shard-readiness-v1010.json",
     "freezes the v1010 material submission precheck integrity fixture before profile section evidence starts"},
    {2, "v1012", "route-preview-candidate-profile-section-node-renderer-split-plan-pin", "v1011",
     "fixtures/release/shard-readiness-v1011.json",
     "pins Node v1481 as renderer ownership context without requiring fresh mini-kv evidence"},
    {3, "v1013", "route-preview-candidate-profile-section-group-boundary", "v1012",
     "fixtures/release/shard-readiness-v1012.json",
     "declares the five candidate document profile sections as a read-only group"},
    {4, "v1014", "route-preview-candidate-profile-section-heading-catalog", "v1013",
     "fixtures/release/shard-readiness-v1013.json",
     "catalogs profile section headings without changing route-facing Markdown"},
    {5, "v1015", "route-preview-candidate-profile-section-request-package-carry-forward", "v1014",
     "fixtures/release/shard-readiness-v1014.json",
     "carries the candidate request package section as metadata only"},
    {6, "v1016", "route-preview-candidate-profile-section-submission-precheck-carry-forward", "v1015",
     "fixtures/release/shard-readiness-v1015.json",
     "carries the candidate submission precheck section as metadata only"},
    {7, "v1017", "route-preview-candidate-profile-section-intake-packet-carry-forward", "v1016",
     "fixtures/release/shard-readiness-v1016.json",
     "carries the candidate intake packet section as metadata only"},
    {8, "v1018", "route-preview-candidate-profile-section-material-request-carry-forward", "v1017",
     "fixtures/release/shard-readiness-v1017.json",
     "carries the candidate material request section as metadata only"},
    {9, "v1019", "route-preview-candidate-profile-section-material-submission-precheck-carry-forward", "v1018",
     "fixtures/release/shard-readiness-v1018.json",
     "carries the candidate material submission precheck section as metadata only"},
    {10, "v1020", "route-preview-candidate-profile-section-renderer-output-stability", "v1019",
     "fixtures/release/shard-readiness-v1019.json",
     "records that renderer ownership changed while output shape stays stable"},
    {11, "v1021", "route-preview-candidate-profile-section-route-markdown-parity", "v1020",
     "fixtures/release/shard-readiness-v1020.json",
     "keeps route Markdown parity as a read-only contract statement"},
    {12, "v1022", "route-preview-candidate-profile-section-no-fresh-evidence-requirement", "v1021",
     "fixtures/release/shard-readiness-v1021.json",
     "records that Node v1481 does not require fresh Java or mini-kv evidence"},
    {13, "v1023", "route-preview-candidate-profile-section-disabled-upstream-probe-boundary", "v1022",
     "fixtures/release/shard-readiness-v1022.json",
     "keeps disabled upstream probe behavior as metadata without starting sibling services"},
    {14, "v1024", "route-preview-candidate-profile-section-type-catalog-compatibility", "v1023",
     "fixtures/release/shard-readiness-v1023.json",
     "records Node type catalog compatibility without importing the catalog"},
    {15, "v1025", "route-preview-candidate-profile-section-live-startup-boundary", "v1024",
     "fixtures/release/shard-readiness-v1024.json",
     "keeps Node Java and mini-kv service startup outside the profile section evidence"},
    {16, "v1026", "route-preview-candidate-profile-section-archive-scan-boundary", "v1025",
     "fixtures/release/shard-readiness-v1025.json",
     "keeps archive review declarative without walking screenshot folders at runtime"},
    {17, "v1027", "route-preview-candidate-profile-section-runtime-probe-boundary", "v1026",
     "fixtures/release/shard-readiness-v1026.json",
     "keeps runtime probes and live reads disabled"},
    {18, "v1028", "route-preview-candidate-profile-section-router-boundary", "v1027",
     "fixtures/release/shard-readiness-v1027.json",
     "keeps active candidate document routing disabled"},
    {19, "v1029", "route-preview-candidate-profile-section-write-wal-boundary", "v1028",
     "fixtures/release/shard-readiness-v1028.json",
     "keeps write routing store mutation WAL touch load restore and compact disabled"},
    {20, "v1030", "route-preview-candidate-profile-section-execution-boundary", "v1029",
     "fixtures/release/shard-readiness-v1029.json",
     "keeps profile section evidence non-executable"},
    {21, "v1031", "route-preview-candidate-profile-section-command-catalog-exposure", "v1030",
     "fixtures/release/shard-readiness-v1030.json",
     "exposes the profile section command as read-only metadata"},
    {22, "v1032", "route-preview-candidate-profile-section-shardjson-embedding", "v1031",
     "fixtures/release/shard-readiness-v1031.json",
     "embeds one candidate profile section evidence block in SHARDJSON"},
    {23, "v1033", "route-preview-candidate-profile-section-validation-split", "v1032",
     "fixtures/release/shard-readiness-v1032.json",
     "keeps validation checks in a separate module from the renderer evidence body"},
    {24, "v1034", "route-preview-candidate-profile-section-cli-tcp-smoke", "v1033",
     "fixtures/release/shard-readiness-v1033.json",
     "keeps CLI and TCP smoke scoped to the read-only profile section command"},
    {25, "v1035", "route-preview-candidate-profile-section-closeout-summary", "v1034",
     "fixtures/release/shard-readiness-v1034.json",
     "closes the profile section renderer split evidence without opening document intake"},
}};

} // namespace

std::span<const StageRecord> candidate_profile_section_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_candidate_profile_section_release_number() {
    return first_candidate_profile_section_release_number_value;
}

int planned_candidate_profile_section_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_candidate_profile_section_stages
