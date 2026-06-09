#include "minikv/shard_preview_operator_value_supply_approval_profile_section_stages.hpp"

#include <array>

namespace minikv::shard_preview_operator_value_supply_approval_profile_section_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_release_number_value = 1111;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v1111", "route-preview-operator-value-supply-approval-profile-section-source-freeze", "v1110",
     "fixtures/release/shard-readiness-v1110.json",
     "freezes the v1110 current fixture before approval profile section evidence starts"},
    {2, "v1112", "route-preview-operator-value-supply-approval-profile-section-no-new-approval-chain", "v1111",
     "fixtures/release/shard-readiness-v1111.json",
     "records Node v1556 as a renderer split without creating a new approval chain"},
    {3, "v1113", "route-preview-operator-value-supply-approval-profile-section-group-boundary", "v1112",
     "fixtures/release/shard-readiness-v1112.json",
     "declares the five approval and capture profile sections as a read-only group"},
    {4, "v1114", "route-preview-operator-value-supply-approval-profile-section-main-renderer-delegation", "v1113",
     "fixtures/release/shard-readiness-v1113.json",
     "records main renderer delegation without executing Node rendering"},
    {5, "v1115", "route-preview-operator-value-supply-approval-profile-section-aggregator-boundary", "v1114",
     "fixtures/release/shard-readiness-v1114.json",
     "records the approval profile section aggregator as metadata only"},
    {6, "v1116", "route-preview-operator-value-supply-approval-profile-section-approval-packet-renderer-split",
     "v1115", "fixtures/release/shard-readiness-v1115.json",
     "records approval packet and template renderer ownership as metadata only"},
    {7, "v1117", "route-preview-operator-value-supply-approval-profile-section-capture-renderer-split", "v1116",
     "fixtures/release/shard-readiness-v1116.json",
     "records signed approval capture renderer ownership as metadata only"},
    {8, "v1118", "route-preview-operator-value-supply-approval-profile-section-approval-packet-draft-lock",
     "v1117", "fixtures/release/shard-readiness-v1117.json",
     "carries the Approval Packet Draft section and Node v986 marker as metadata only"},
    {9, "v1119", "route-preview-operator-value-supply-approval-profile-section-approval-packet-review-lock",
     "v1118", "fixtures/release/shard-readiness-v1118.json",
     "carries the Approval Packet Review section and Node v1011 marker as metadata only"},
    {10, "v1120", "route-preview-operator-value-supply-approval-profile-section-signed-approval-template-lock",
     "v1119", "fixtures/release/shard-readiness-v1119.json",
     "carries the Signed Approval Template section and Node v1036 marker as metadata only"},
    {11, "v1121", "route-preview-operator-value-supply-approval-profile-section-capture-preflight-lock", "v1120",
     "fixtures/release/shard-readiness-v1120.json",
     "carries the Signed Approval Capture Preflight section and Node v1061 marker as metadata only"},
    {12, "v1122", "route-preview-operator-value-supply-approval-profile-section-capture-artifact-preflight-lock",
     "v1121", "fixtures/release/shard-readiness-v1121.json",
     "carries the Signed Approval Capture Artifact Preflight section and Node v1086 marker as metadata only"},
    {13, "v1123", "route-preview-operator-value-supply-approval-profile-section-fail-closed-approval-state",
     "v1122", "fixtures/release/shard-readiness-v1122.json",
     "preserves fail-closed approval and signed approval states"},
    {14, "v1124", "route-preview-operator-value-supply-approval-profile-section-runtime-secret-false-lock",
     "v1123", "fixtures/release/shard-readiness-v1123.json",
     "keeps runtime payload and secret-value fields false"},
    {15, "v1125", "route-preview-operator-value-supply-approval-profile-section-type-catalog-aggregator-order",
     "v1124", "fixtures/release/shard-readiness-v1124.json",
     "records the approval aggregator type catalog order 214 without importing the catalog"},
    {16, "v1126", "route-preview-operator-value-supply-approval-profile-section-type-catalog-packet-order",
     "v1125", "fixtures/release/shard-readiness-v1125.json",
     "records the approval packet renderer type catalog order 215 without importing the catalog"},
    {17, "v1127", "route-preview-operator-value-supply-approval-profile-section-type-catalog-capture-order",
     "v1126", "fixtures/release/shard-readiness-v1126.json",
     "records the signed approval capture renderer type catalog order 216 without importing the catalog"},
    {18, "v1128", "route-preview-operator-value-supply-approval-profile-section-profile-entry-order-lock", "v1127",
     "fixtures/release/shard-readiness-v1127.json",
     "records profile-entry-types as type catalog order 217"},
    {19, "v1129", "route-preview-operator-value-supply-approval-profile-section-catalog-validation-version",
     "v1128", "fixtures/release/shard-readiness-v1128.json",
     "pins catalog and validation context to Node v1556"},
    {20, "v1130", "route-preview-operator-value-supply-approval-profile-section-route-markdown-parity", "v1129",
     "fixtures/release/shard-readiness-v1129.json",
     "keeps route Markdown output stable as a read-only statement"},
    {21, "v1131", "route-preview-operator-value-supply-approval-profile-section-main-renderer-size-control",
     "v1130", "fixtures/release/shard-readiness-v1130.json",
     "records main renderer line-count reduction as Node maintenance context"},
    {22, "v1132", "route-preview-operator-value-supply-approval-profile-section-subrenderer-size-control", "v1131",
     "fixtures/release/shard-readiness-v1131.json",
     "records subrenderer size control without importing renderer code"},
    {23, "v1133", "route-preview-operator-value-supply-approval-profile-section-parallel-status", "v1132",
     "fixtures/release/shard-readiness-v1132.json",
     "records that Java and mini-kv may proceed in parallel without fresh Node evidence dependency"},
    {24, "v1134", "route-preview-operator-value-supply-approval-profile-section-no-live-integration-startup",
     "v1133", "fixtures/release/shard-readiness-v1133.json",
     "keeps live integration startup outside approval profile-section evidence"},
    {25, "v1135", "route-preview-operator-value-supply-approval-profile-section-closeout-summary", "v1134",
     "fixtures/release/shard-readiness-v1134.json",
     "closes approval profile-section evidence without opening material, router, write, or execution authority"},
}};

} // namespace

std::span<const StageRecord> operator_value_supply_approval_profile_section_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_operator_value_supply_approval_profile_section_release_number() {
    return first_release_number_value;
}

int planned_operator_value_supply_approval_profile_section_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_operator_value_supply_approval_profile_section_stages
