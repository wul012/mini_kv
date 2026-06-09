#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_stages.hpp"

#include <array>

namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1161;

constexpr std::array<StageRecord, 10> stage_catalog = {{
    {1, "v1161", "route-preview-operator-value-supply-profile-section-integrity-source-freeze", "v1160",
     "fixtures/release/shard-readiness-v1160.json",
     "freezes the v1160 value supply profile section fixture before integrity evidence starts"},
    {2, "v1162", "route-preview-operator-value-supply-profile-section-integrity-source-command-digest-pin", "v1161",
     "fixtures/release/shard-readiness-v1161.json",
     "pins the v1160 profile section command and digest marker as the only integrity source"},
    {3, "v1163", "route-preview-operator-value-supply-profile-section-integrity-section-count-lock", "v1162",
     "fixtures/release/shard-readiness-v1162.json",
     "checks the four value supply profile sections remain four"},
    {4, "v1164", "route-preview-operator-value-supply-profile-section-integrity-source-lineage-lock", "v1163",
     "fixtures/release/shard-readiness-v1163.json",
     "locks v560 v585 v610 and Node v936 source lineage as metadata only"},
    {5, "v1165", "route-preview-operator-value-supply-profile-section-integrity-operator-value-lock", "v1164",
     "fixtures/release/shard-readiness-v1164.json",
     "keeps operator value and fresh sibling evidence import disabled"},
    {6, "v1166", "route-preview-operator-value-supply-profile-section-integrity-runtime-secret-lock", "v1165",
     "fixtures/release/shard-readiness-v1165.json",
     "keeps runtime payload synthetic evidence secret value and raw endpoint fields false"},
    {7, "v1167", "route-preview-operator-value-supply-profile-section-integrity-node-v1606-parallel-lock",
     "v1166", "fixtures/release/shard-readiness-v1166.json",
     "records Node v1606 as parallel maintenance context without fresh mini-kv evidence"},
    {8, "v1168", "route-preview-operator-value-supply-profile-section-integrity-command-shardjson-exposure",
     "v1167", "fixtures/release/shard-readiness-v1167.json",
     "exposes the integrity command and embeds one integrity block in SHARDJSON"},
    {9, "v1169", "route-preview-operator-value-supply-profile-section-integrity-validation-split", "v1168",
     "fixtures/release/shard-readiness-v1168.json",
     "keeps integrity validation separate from the evidence formatter"},
    {10, "v1170", "route-preview-operator-value-supply-profile-section-integrity-closeout-summary", "v1169",
     "fixtures/release/shard-readiness-v1169.json",
     "closes profile section integrity without importing values routing writes WAL or execution"},
}};

} // namespace

std::span<const StageRecord> operator_value_supply_profile_section_integrity_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_operator_value_supply_profile_section_integrity_release_number() {
    return first_release_number_value;
}

int planned_operator_value_supply_profile_section_integrity_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_stages
