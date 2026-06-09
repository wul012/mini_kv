#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_material_submission_precheck_integrity_release_number_value = 986;

constexpr std::array<StageRecord, 25> stage_catalog = {{
    {1, "v986", "route-preview-candidate-material-submission-precheck-integrity-source-freeze", "v985",
     "fixtures/release/shard-readiness-v985.json",
     "freezes the completed v985 material submission precheck fixture before integrity review starts"},
    {2, "v987", "route-preview-candidate-material-submission-precheck-integrity-node-stop-condition-pin", "v986",
     "fixtures/release/shard-readiness-v986.json",
     "pins the Node v1456 stop condition so mini-kv does not create another submission echo"},
    {3, "v988", "route-preview-candidate-material-submission-precheck-integrity-fixture-range-catalog", "v987",
     "fixtures/release/shard-readiness-v987.json",
     "catalogs the v986-v1010 fixture range without reading runtime archives"},
    {4, "v989", "route-preview-candidate-material-submission-precheck-integrity-release-continuity", "v988",
     "fixtures/release/shard-readiness-v988.json",
     "checks release labels and source fixture continuity for the integrity chain"},
    {5, "v990", "route-preview-candidate-material-submission-precheck-integrity-source-lineage", "v989",
     "fixtures/release/shard-readiness-v989.json",
     "pins v985 as the only material submission precheck source for this audit"},
    {6, "v991", "route-preview-candidate-material-submission-precheck-integrity-status-parity", "v990",
     "fixtures/release/shard-readiness-v990.json",
     "preserves material submission precheck closeout status while adding no intake"},
    {7, "v992", "route-preview-candidate-material-submission-precheck-integrity-checkpoint-count-parity", "v991",
     "fixtures/release/shard-readiness-v991.json",
     "keeps the ten checkpoint count frozen from v985"},
    {8, "v993", "route-preview-candidate-material-submission-precheck-integrity-validator-count-parity", "v992",
     "fixtures/release/shard-readiness-v992.json",
     "keeps the ten validator count frozen from v985"},
    {9, "v994", "route-preview-candidate-material-submission-precheck-integrity-request-item-count-parity", "v993",
     "fixtures/release/shard-readiness-v993.json",
     "keeps the twenty-five material request item count frozen through v975 lineage"},
    {10, "v995", "route-preview-candidate-material-submission-precheck-integrity-acceptance-check-count-parity", "v994",
     "fixtures/release/shard-readiness-v994.json",
     "keeps the twenty-five material acceptance check count frozen through v975 lineage"},
    {11, "v996", "route-preview-candidate-material-submission-precheck-integrity-required-field-count-parity", "v995",
     "fixtures/release/shard-readiness-v995.json",
     "keeps the twenty required material field count frozen"},
    {12, "v997", "route-preview-candidate-material-submission-precheck-integrity-submission-field-count-parity", "v996",
     "fixtures/release/shard-readiness-v996.json",
     "keeps the twenty submission material field count frozen"},
    {13, "v998", "route-preview-candidate-material-submission-precheck-integrity-gate-count-parity", "v997",
     "fixtures/release/shard-readiness-v997.json",
     "keeps the forty-one material submission gate expectation frozen"},
    {14, "v999", "route-preview-candidate-material-submission-precheck-integrity-real-material-absence-boundary", "v998",
     "fixtures/release/shard-readiness-v998.json",
     "keeps reviewed real material absent unread and unconsumed"},
    {15, "v1000", "route-preview-candidate-material-submission-precheck-integrity-payload-import-boundary", "v999",
     "fixtures/release/shard-readiness-v999.json",
     "keeps material and candidate payload import disabled"},
    {16, "v1001", "route-preview-candidate-material-submission-precheck-integrity-evaluation-boundary", "v1000",
     "fixtures/release/shard-readiness-v1000.json",
     "keeps candidate material evaluation acceptance and rejection disabled"},
    {17, "v1002", "route-preview-candidate-material-submission-precheck-integrity-approval-boundary", "v1001",
     "fixtures/release/shard-readiness-v1001.json",
     "keeps approval grant capture disabled"},
    {18, "v1003", "route-preview-candidate-material-submission-precheck-integrity-signature-boundary", "v1002",
     "fixtures/release/shard-readiness-v1002.json",
     "keeps signature emission parsing and verification disabled"},
    {19, "v1004", "route-preview-candidate-material-submission-precheck-integrity-runtime-boundary", "v1003",
     "fixtures/release/shard-readiness-v1003.json",
     "keeps runtime payload import and live runtime probes disabled"},
    {20, "v1005", "route-preview-candidate-material-submission-precheck-integrity-router-boundary", "v1004",
     "fixtures/release/shard-readiness-v1004.json",
     "keeps active document router installation disabled"},
    {21, "v1006", "route-preview-candidate-material-submission-precheck-integrity-write-wal-boundary", "v1005",
     "fixtures/release/shard-readiness-v1005.json",
     "keeps write routing store mutation WAL touch load restore and compact outside the audit"},
    {22, "v1007", "route-preview-candidate-material-submission-precheck-integrity-sibling-mutation-boundary", "v1006",
     "fixtures/release/shard-readiness-v1006.json",
     "keeps Node Java and archive sibling mutation disabled"},
    {23, "v1008", "route-preview-candidate-material-submission-precheck-integrity-command-catalog-parity", "v1007",
     "fixtures/release/shard-readiness-v1007.json",
     "checks the integrity command remains parser visible read-only metadata"},
    {24, "v1009", "route-preview-candidate-material-submission-precheck-integrity-cli-tcp-smoke-parity", "v1008",
     "fixtures/release/shard-readiness-v1008.json",
     "keeps CLI and TCP smoke scoped to read-only integrity evidence"},
    {25, "v1010", "route-preview-candidate-material-submission-precheck-integrity-closeout-summary", "v1009",
     "fixtures/release/shard-readiness-v1009.json",
     "closes the integrity hardening chain without opening material submission"},
}};

} // namespace

std::span<const StageRecord> candidate_material_submission_precheck_integrity_stages() {
    return {stage_catalog.data(), stage_catalog.size()};
}

int first_candidate_material_submission_precheck_integrity_release_number() {
    return first_candidate_material_submission_precheck_integrity_release_number_value;
}

int planned_candidate_material_submission_precheck_integrity_stage_count() {
    return static_cast<int>(stage_catalog.size());
}

} // namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_stages
