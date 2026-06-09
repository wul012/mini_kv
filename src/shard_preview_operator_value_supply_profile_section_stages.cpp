#include "minikv/shard_preview_operator_value_supply_profile_section_stages.hpp"

#include <array>

namespace minikv::shard_preview_operator_value_supply_profile_section_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1136;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1136", "route-preview-operator-value-supply-profile-section-source-freeze", "v1135",
     "fixtures/release/shard-readiness-v1135.json",
     "freezes the v1135 current fixture before value supply profile section evidence starts"},
    {2, "v1137", "route-preview-operator-value-supply-profile-section-no-new-evidence-chain", "v1136",
     "fixtures/release/shard-readiness-v1136.json",
     "records Node v1581 as a renderer split without creating a new evidence chain"},
    {3, "v1138", "route-preview-operator-value-supply-profile-section-group-boundary", "v1137",
     "fixtures/release/shard-readiness-v1137.json",
     "declares the four import and value supply profile sections as a read-only group"},
    {4, "v1139", "route-preview-operator-value-supply-profile-section-main-renderer-delegation", "v1138",
     "fixtures/release/shard-readiness-v1138.json",
     "records main renderer delegation without executing Node rendering"},
    {5, "v1140", "route-preview-operator-value-supply-profile-section-aggregator-boundary", "v1139",
     "fixtures/release/shard-readiness-v1139.json",
     "records the value supply profile section aggregator as metadata only"},
    {6, "v1141", "route-preview-operator-value-supply-profile-section-import-draft-renderer-split", "v1140",
     "fixtures/release/shard-readiness-v1140.json",
     "records import preflight and value draft renderer ownership as metadata only"},
    {7, "v1142", "route-preview-operator-value-supply-profile-section-sibling-envelope-renderer-split", "v1141",
     "fixtures/release/shard-readiness-v1141.json",
     "records fresh sibling intake and value supply envelope renderer ownership as metadata only"},
    {8, "v1143", "route-preview-operator-value-supply-profile-section-import-preflight-lock", "v1142",
     "fixtures/release/shard-readiness-v1142.json",
     "carries the Operator Evidence Import Preflight section and Node v886 marker as metadata only"},
    {9, "v1144", "route-preview-operator-value-supply-profile-section-value-draft-lock", "v1143",
     "fixtures/release/shard-readiness-v1143.json",
     "carries the Operator Evidence Value Draft section and Node v911 marker as metadata only"},
    {10, "v1145", "route-preview-operator-value-supply-profile-section-fresh-sibling-intake-lock", "v1144",
     "fixtures/release/shard-readiness-v1144.json",
     "carries the Operator Evidence Fresh Sibling Intake section and Node v936 marker as plan metadata only"},
    {11, "v1146", "route-preview-operator-value-supply-profile-section-value-supply-envelope-lock", "v1145",
     "fixtures/release/shard-readiness-v1145.json",
     "carries the Operator Evidence Value Supply Envelope section and Node v961 marker as metadata only"},
    {12, "v1147", "route-preview-operator-value-supply-profile-section-runtime-payload-false-lock", "v1146",
     "fixtures/release/shard-readiness-v1146.json",
     "keeps runtime payload fields false for value supply profile section evidence"},
    {13, "v1148", "route-preview-operator-value-supply-profile-section-synthetic-evidence-false-lock", "v1147",
     "fixtures/release/shard-readiness-v1147.json",
     "keeps synthetic evidence acceptance false"},
    {14, "v1149", "route-preview-operator-value-supply-profile-section-secret-value-false-lock", "v1148",
     "fixtures/release/shard-readiness-v1148.json",
     "keeps credential secret and raw endpoint value fields false"},
    {15, "v1150", "route-preview-operator-value-supply-profile-section-type-catalog-aggregator-order", "v1149",
     "fixtures/release/shard-readiness-v1149.json",
     "records the value supply aggregator type catalog order 217 without importing the catalog"},
    {16, "v1151", "route-preview-operator-value-supply-profile-section-type-catalog-import-renderer-order",
     "v1150", "fixtures/release/shard-readiness-v1150.json",
     "records the value supply import renderer type catalog order 218 without importing the catalog"},
    {17, "v1152", "route-preview-operator-value-supply-profile-section-type-catalog-envelope-renderer-order",
     "v1151", "fixtures/release/shard-readiness-v1151.json",
     "records the value supply envelope renderer type catalog order 219 without importing the catalog"},
    {18, "v1153", "route-preview-operator-value-supply-profile-section-profile-entry-order-lock", "v1152",
     "fixtures/release/shard-readiness-v1152.json",
     "records profile-entry-types as type catalog order 220"},
    {19, "v1154", "route-preview-operator-value-supply-profile-section-catalog-validation-version", "v1153",
     "fixtures/release/shard-readiness-v1153.json",
     "pins catalog and validation context to Node v1581"},
    {20, "v1155", "route-preview-operator-value-supply-profile-section-route-markdown-parity", "v1154",
     "fixtures/release/shard-readiness-v1154.json",
     "keeps route Markdown output stable as a read-only statement"},
    {21, "v1156", "route-preview-operator-value-supply-profile-section-main-renderer-size-control", "v1155",
     "fixtures/release/shard-readiness-v1155.json",
     "records main renderer line-count reduction as Node maintenance context"},
    {22, "v1157", "route-preview-operator-value-supply-profile-section-subrenderer-size-control", "v1156",
     "fixtures/release/shard-readiness-v1156.json",
     "records subrenderer size control without importing renderer code"},
    {23, "v1158", "route-preview-operator-value-supply-profile-section-parallel-status", "v1157",
     "fixtures/release/shard-readiness-v1157.json",
     "records that Java and mini-kv may proceed in parallel without fresh Node evidence dependency"},
    {24, "v1159", "route-preview-operator-value-supply-profile-section-no-live-integration-startup", "v1158",
     "fixtures/release/shard-readiness-v1158.json",
     "keeps live integration startup outside value supply profile section evidence"},
    {25, "v1160", "route-preview-operator-value-supply-profile-section-closeout-summary", "v1159",
     "fixtures/release/shard-readiness-v1159.json",
     "closes value supply profile section evidence without opening import router write or execution authority"},
}};

} // namespace

std::span<const StageRecord> operator_value_supply_profile_section_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_operator_value_supply_profile_section_release_number() {
    return first_release_number_value;
}

int planned_operator_value_supply_profile_section_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section_stages
