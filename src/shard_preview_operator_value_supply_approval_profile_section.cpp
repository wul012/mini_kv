#include "minikv/shard_preview_operator_value_supply_approval_profile_section.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_operator_value_supply_approval_profile_section_checks.hpp"
#include "minikv/shard_preview_operator_value_supply_approval_profile_section_stages.hpp"
#include "minikv/shard_preview_operator_value_supply_approval_profile_section_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_approval_template.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_operator_value_supply_approval_profile_section {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace approval_template = shard_route_preview_operator_value_supply_approval_template;
namespace artifact_preflight =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight;
namespace capture_preflight = shard_route_preview_operator_value_supply_signed_approval_capture_preflight;
namespace checks = shard_preview_operator_value_supply_approval_profile_section_checks;
namespace signed_template = shard_route_preview_operator_value_supply_signed_approval_template;
namespace stages = shard_preview_operator_value_supply_approval_profile_section_stages;
namespace validation = shard_preview_operator_value_supply_approval_profile_section_validation;

constexpr std::string_view profile_section_command = "SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1556-controlled-read-only-shard-preview-operator-evidence-value-supply-approval-profile-section-renderer-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_approval_template_planned_stage_count = 25;
constexpr int source_signed_approval_template_planned_stage_count = 25;
constexpr int source_capture_preflight_planned_stage_count = 25;
constexpr int source_artifact_preflight_planned_stage_count = 25;
constexpr int approval_profile_section_count = 5;
constexpr int node_migrated_approval_profile_section_count = 5;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool route_markdown_changed = false;
constexpr bool mini_kv_consumes_approval_material = false;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

const StageRecord& current_stage() {
    return stages::operator_value_supply_approval_profile_section_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::operator_value_supply_approval_profile_section_stages(),
        current_stage_count,
        stages::planned_operator_value_supply_approval_profile_section_stage_count(),
        stages::first_operator_value_supply_approval_profile_section_release_number());
}

int completed_profile_section_check_count() {
    return current_stage_count;
}

bool source_approval_template_complete() {
    return approval_template::published_stage_count() == source_approval_template_planned_stage_count;
}

bool source_signed_approval_template_complete() {
    return signed_template::published_stage_count() == source_signed_approval_template_planned_stage_count;
}

bool source_capture_preflight_complete() {
    return capture_preflight::published_stage_count() == source_capture_preflight_planned_stage_count;
}

bool source_artifact_preflight_complete() {
    return artifact_preflight::published_stage_count() == source_artifact_preflight_planned_stage_count;
}

} // namespace

std::string format_operator_value_supply_approval_profile_section_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::operator_value_supply_approval_profile_section_stages(),
        current_stage_count);
}

std::string operator_value_supply_approval_profile_section_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_operator_value_supply_approval_profile_section_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int approval_template_stage_count = approval_template::published_stage_count();
    const bool approval_template_complete = source_approval_template_complete();
    const int signed_template_stage_count = signed_template::published_stage_count();
    const bool signed_template_complete = source_signed_approval_template_complete();
    const int capture_preflight_stage_count = capture_preflight::published_stage_count();
    const bool capture_preflight_complete = source_capture_preflight_complete();
    const int artifact_preflight_stage_count = artifact_preflight::published_stage_count();
    const bool artifact_preflight_complete = source_artifact_preflight_complete();
    const int planned_check_count = checks::planned_operator_value_supply_approval_profile_section_check_count();
    const int check_count = completed_profile_section_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_operator_value_supply_approval_profile_section_stage_count() &&
        approval_template_complete && signed_template_complete && capture_preflight_complete &&
        artifact_preflight_complete;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-approval-profile-section-renderer-split.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(profile_section_command) +
           ",\"operatorValueSupplyApprovalProfileSectionMode\":\"controlled-read-only-operator-value-supply-approval-profile-section-renderer-split-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeOperatorValueSupplyApprovalProfileSectionReleaseVersion\":\"Node v1556\""
           ",\"sourceNodeApprovalProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewOperatorEvidenceValueSupplyApprovalProfileSectionRenderer.ts\""
           ",\"sourceNodeApprovalPacketRendererGroup\":\"approval-packet-and-signed-approval-template-renderer\""
           ",\"sourceNodeSignedApprovalCaptureRendererGroup\":\"signed-approval-capture-profile-section-renderer\""
           ",\"sourceNodeApprovalProfileSectionGroupCount\":" + std::to_string(approval_profile_section_count) +
           ",\"sourceNodeMigratedApprovalProfileSectionCount\":" +
           std::to_string(node_migrated_approval_profile_section_count) +
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRouteMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeVersionMarkers\":" +
           json_string_array({"Node v986", "Node v1011", "Node v1036", "Node v1061", "Node v1086"}) +
           ",\"sourceNodeTypeCatalogOrderRange\":\"214-217\""
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1110\""
           ",\"sourceCurrentShardReadinessFixturePath\":\"fixtures/release/shard-readiness-v1110.json\""
           ",\"sourceApprovalPacketDraftCommand\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\""
           ",\"sourceApprovalPacketDraftReleaseVersion\":\"v660\""
           ",\"sourceApprovalPacketDraftFixturePath\":\"fixtures/release/shard-readiness-v660.json\""
           ",\"sourceApprovalPacketDraftPublishedStageCount\":" + std::to_string(approval_template_stage_count) +
           ",\"sourceApprovalPacketDraftComplete\":" + json_bool(approval_template_complete) +
           ",\"sourceApprovalPacketDraftDigestMarker\":" +
           json_string(approval_template::value_supply_approval_template_digest_marker()) +
           ",\"sourceApprovalPacketReviewAndSignedApprovalTemplateCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\""
           ",\"sourceApprovalPacketReviewReleaseVersion\":\"v685\""
           ",\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\""
           ",\"sourceSignedApprovalTemplateFixturePath\":\"fixtures/release/shard-readiness-v685.json\""
           ",\"sourceSignedApprovalTemplatePublishedStageCount\":" + std::to_string(signed_template_stage_count) +
           ",\"sourceSignedApprovalTemplateComplete\":" + json_bool(signed_template_complete) +
           ",\"sourceSignedApprovalTemplateDigestMarker\":" +
           json_string(signed_template::signed_approval_template_digest_marker()) +
           ",\"sourceSignedApprovalCapturePreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\""
           ",\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\""
           ",\"sourceSignedApprovalCapturePreflightFixturePath\":\"fixtures/release/shard-readiness-v710.json\""
           ",\"sourceSignedApprovalCapturePreflightPublishedStageCount\":" +
           std::to_string(capture_preflight_stage_count) +
           ",\"sourceSignedApprovalCapturePreflightComplete\":" + json_bool(capture_preflight_complete) +
           ",\"sourceSignedApprovalCapturePreflightDigestMarker\":" +
           json_string(capture_preflight::signed_approval_capture_preflight_digest_marker()) +
           ",\"sourceSignedApprovalCaptureArtifactPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\""
           ",\"sourceSignedApprovalCaptureArtifactPreflightReleaseVersion\":\"v735\""
           ",\"sourceSignedApprovalCaptureArtifactPreflightFixturePath\":\"fixtures/release/shard-readiness-v735.json\""
           ",\"sourceSignedApprovalCaptureArtifactPreflightPublishedStageCount\":" +
           std::to_string(artifact_preflight_stage_count) +
           ",\"sourceSignedApprovalCaptureArtifactPreflightComplete\":" + json_bool(artifact_preflight_complete) +
           ",\"sourceSignedApprovalCaptureArtifactPreflightDigestMarker\":" +
           json_string(artifact_preflight::signed_approval_capture_artifact_preflight_digest_marker()) +
           ",\"operatorValueSupplyApprovalProfileSectionStage\":" + json_string(stage.stage) +
           ",\"operatorValueSupplyApprovalProfileSectionStageSequence\":" + std::to_string(stage.sequence) +
           ",\"operatorValueSupplyApprovalProfileSectionReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"operatorValueSupplyApprovalProfileSectionReleaseRangeStart\":\"v1111\""
           ",\"operatorValueSupplyApprovalProfileSectionReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_operator_value_supply_approval_profile_section_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedOperatorValueSupplyApprovalProfileSectionCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedOperatorValueSupplyApprovalProfileSectionCheckCount\":" +
           std::to_string(check_count) +
           ",\"operatorValueSupplyApprovalProfileSectionRendererSplitDeclared\":true"
           ",\"operatorValueSupplyApprovalProfileSectionOnly\":true"
           ",\"operatorValueSupplyApprovalProfileSectionChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceCurrentFixtureFrozen\":true"
           ",\"sourceApprovalEvidenceFrozen\":true"
           ",\"nodeRendererSplitObservedAsReadOnlyPlan\":true"
           ",\"routeFacingMarkdownStable\":true"
           ",\"routeFacingOutputChanged\":false"
           ",\"nodeRuntimeBehaviorAdded\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"operatorValueSupplyApprovalProfileSectionNames\":" +
           json_string_array({"approval-packet-draft",
                              "approval-packet-review",
                              "signed-approval-template",
                              "signed-approval-capture-preflight",
                              "signed-approval-capture-artifact-preflight"}) +
           ",\"operatorValueSupplyApprovalProfileSectionCount\":" +
           std::to_string(approval_profile_section_count) +
           ",\"nodeMigratedOperatorValueSupplyApprovalProfileSectionCount\":" +
           std::to_string(node_migrated_approval_profile_section_count) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesApprovalMaterial\":" + json_bool(mini_kv_consumes_approval_material) +
           ",\"approvalMaterialImported\":false"
           ",\"approvalMaterialAccepted\":false"
           ",\"signedApprovalCreated\":false"
           ",\"signedApprovalCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"governanceEchoCreated\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"materialIntakeOpened\":false"
           ",\"materialPayloadImported\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"secretValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"typeCatalogImported\":false"
           ",\"moduleSplit\":[\"operator_value_supply_approval_profile_section_core\",\"operator_value_supply_approval_profile_section_stages\",\"operator_value_supply_approval_profile_section_checks\",\"operator_value_supply_approval_profile_section_validation\"]"
           ",\"stageCatalog\":" + format_operator_value_supply_approval_profile_section_stage_catalog_json() +
           ",\"profileSectionChecks\":" +
           checks::format_operator_value_supply_approval_profile_section_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_operator_value_supply_approval_profile_section_validation_json(
               approval_template_stage_count,
               approval_template_complete,
               signed_template_stage_count,
               signed_template_complete,
               capture_preflight_stage_count,
               capture_preflight_complete,
               artifact_preflight_stage_count,
               artifact_preflight_complete,
               approval_profile_section_count,
               node_migrated_approval_profile_section_count,
               node_requires_fresh_minikv_evidence,
               route_markdown_changed,
               mini_kv_consumes_approval_material,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_operator_value_supply_approval_profile_section_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1556 is treated as an approval profile-section renderer ownership split",
                              "v1110 remains the frozen current fixture before v1111-v1135 approval profile-section evidence",
                              "v660 v685 v710 and v735 remain the frozen approval source lineage",
                              "mini-kv does not create signed approvals or consume approval material",
                              "renderer execution catalog import router write routing WAL touch service startup runtime probes secret reads and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string operator_value_supply_approval_profile_section_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_operator_value_supply_approval_profile_section_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_operator_value_supply_approval_profile_section
