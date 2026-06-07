#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages.hpp"

#include <array>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_artifact_preflight_release_number = 711;

constexpr std::array<StageRecord, 25> artifact_preflight_stages = {{
    {1, "v711", "route-preview-value-supply-signed-approval-capture-artifact-preflight-request-id", "v710",
     "fixtures/release/shard-readiness-v710.json",
     "starts artifact preflight request id review without minting an artifact"},
    {2, "v712", "route-preview-value-supply-signed-approval-capture-artifact-preflight-capture-digest", "v711",
     "fixtures/release/shard-readiness-v711.json",
     "binds artifact preflight to the frozen v710 capture preflight digest"},
    {3, "v713", "route-preview-value-supply-signed-approval-capture-artifact-preflight-template-digest", "v712",
     "fixtures/release/shard-readiness-v712.json",
     "binds artifact preflight to the frozen signed approval template digest"},
    {4, "v714", "route-preview-value-supply-signed-approval-capture-artifact-preflight-review-digest", "v713",
     "fixtures/release/shard-readiness-v713.json",
     "keeps approval packet review digest as a required absent fragment"},
    {5, "v715", "route-preview-value-supply-signed-approval-capture-artifact-preflight-operator-identity", "v714",
     "fixtures/release/shard-readiness-v714.json",
     "records operator identity as a required artifact fragment without storing the value"},
    {6, "v716", "route-preview-value-supply-signed-approval-capture-artifact-preflight-operator-role", "v715",
     "fixtures/release/shard-readiness-v715.json",
     "records operator role as a required artifact fragment without storing the value"},
    {7, "v717", "route-preview-value-supply-signed-approval-capture-artifact-preflight-window-id", "v716",
     "fixtures/release/shard-readiness-v716.json",
     "keeps capture window id as review metadata without opening a window"},
    {8, "v718", "route-preview-value-supply-signed-approval-capture-artifact-preflight-channel-policy", "v717",
     "fixtures/release/shard-readiness-v717.json",
     "records channel policy while rejecting raw channel and endpoint values"},
    {9, "v719", "route-preview-value-supply-signed-approval-capture-artifact-preflight-signature-algorithm", "v718",
     "fixtures/release/shard-readiness-v718.json",
     "records signature algorithm metadata without parsing signature material"},
    {10, "v720", "route-preview-value-supply-signed-approval-capture-artifact-preflight-detached-signature",
     "v719", "fixtures/release/shard-readiness-v719.json",
     "keeps detached signature as a placeholder and does not capture bytes"},
    {11, "v721", "route-preview-value-supply-signed-approval-capture-artifact-preflight-signature-redaction",
     "v720", "fixtures/release/shard-readiness-v720.json",
     "locks raw signature material behind redaction policy"},
    {12, "v722", "route-preview-value-supply-signed-approval-capture-artifact-preflight-statement-digest",
     "v721", "fixtures/release/shard-readiness-v721.json",
     "keeps approval statement digest as a placeholder and does not fabricate text"},
    {13, "v723", "route-preview-value-supply-signed-approval-capture-artifact-preflight-source-version", "v722",
     "fixtures/release/shard-readiness-v722.json",
     "pins source evidence version without consuming rolling current evidence"},
    {14, "v724", "route-preview-value-supply-signed-approval-capture-artifact-preflight-source-file", "v723",
     "fixtures/release/shard-readiness-v723.json",
     "records source file id without runtime file lookup"},
    {15, "v725", "route-preview-value-supply-signed-approval-capture-artifact-preflight-source-snippet", "v724",
     "fixtures/release/shard-readiness-v724.json",
     "records source snippet id without runtime snippet lookup"},
    {16, "v726", "route-preview-value-supply-signed-approval-capture-artifact-preflight-redacted-value-digest",
     "v725", "fixtures/release/shard-readiness-v725.json",
     "requires redacted value digest while rejecting raw value material"},
    {17, "v727", "route-preview-value-supply-signed-approval-capture-artifact-preflight-value-shape", "v726",
     "fixtures/release/shard-readiness-v726.json",
     "records value shape while keeping payload absent"},
    {18, "v728", "route-preview-value-supply-signed-approval-capture-artifact-preflight-redaction-policy", "v727",
     "fixtures/release/shard-readiness-v727.json",
     "requires redaction policy and stores no unredacted evidence"},
    {19, "v729", "route-preview-value-supply-signed-approval-capture-artifact-preflight-provenance-policy", "v728",
     "fixtures/release/shard-readiness-v728.json",
     "requires provenance policy and performs no evidence import"},
    {20, "v730", "route-preview-value-supply-signed-approval-capture-artifact-preflight-no-raw-secret", "v729",
     "fixtures/release/shard-readiness-v729.json",
     "locks raw secret credential endpoint and signature material out of the artifact"},
    {21, "v731", "route-preview-value-supply-signed-approval-capture-artifact-preflight-no-grant", "v730",
     "fixtures/release/shard-readiness-v730.json",
     "proves artifact preflight emits no approval grant"},
    {22, "v732", "route-preview-value-supply-signed-approval-capture-artifact-preflight-zero-value-import", "v731",
     "fixtures/release/shard-readiness-v731.json",
     "proves artifact preflight imports zero operator values"},
    {23, "v733", "route-preview-value-supply-signed-approval-capture-artifact-preflight-no-write-route", "v732",
     "fixtures/release/shard-readiness-v732.json",
     "proves artifact preflight installs no active write route"},
    {24, "v734", "route-preview-value-supply-signed-approval-capture-artifact-preflight-sibling-non-mutation",
     "v733", "fixtures/release/shard-readiness-v733.json",
     "proves artifact preflight mutates no sibling project or evidence"},
    {25, "v735", "route-preview-value-supply-signed-approval-capture-artifact-preflight-closeout", "v734",
     "fixtures/release/shard-readiness-v734.json",
     "closes artifact preflight as read-only evidence before any signed artifact draft"},
}};

} // namespace

std::span<const StageRecord> signed_approval_capture_artifact_preflight_stages() {
    return {artifact_preflight_stages.data(), artifact_preflight_stages.size()};
}

int first_signed_approval_capture_artifact_preflight_release_number() {
    return first_artifact_preflight_release_number;
}

int planned_signed_approval_capture_artifact_preflight_stage_count() {
    return static_cast<int>(artifact_preflight_stages.size());
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages
