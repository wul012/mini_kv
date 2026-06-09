#include "minikv/shard_preview_candidate_material_request_integrity_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_material_request_integrity_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_material_request_integrity_release_number_value = 966;

constexpr std::array<StageRecord, 10> candidate_material_request_integrity_stage_catalog = {{
    {1, "v966", "route-preview-candidate-material-request-integrity-source-freeze", "v965",
     "fixtures/release/shard-readiness-v965.json",
     "freezes the completed v965 material request package before integrity review starts"},
    {2, "v967", "route-preview-candidate-material-request-integrity-node-stop-condition-pin", "v966",
     "fixtures/release/shard-readiness-v966.json",
     "pins the Node v1446 stop condition so mini-kv does not create another request echo"},
    {3, "v968", "route-preview-candidate-material-request-integrity-fixture-range", "v967",
     "fixtures/release/shard-readiness-v967.json",
     "declares the v966-v975 fixture range without reading reviewed material"},
    {4, "v969", "route-preview-candidate-material-request-integrity-command-catalog-parity", "v968",
     "fixtures/release/shard-readiness-v968.json",
     "keeps the integrity command visible as read-only parser metadata"},
    {5, "v970", "route-preview-candidate-material-request-integrity-fixture-encoding-bom-guard", "v969",
     "fixtures/release/shard-readiness-v969.json",
     "guards versioned fixtures against UTF-8 BOM and trailing-quality drift"},
    {6, "v971", "route-preview-candidate-material-request-integrity-archive-reference-boundary", "v970",
     "fixtures/release/shard-readiness-v970.json",
     "references archive evidence without walking runtime archive directories"},
    {7, "v972", "route-preview-candidate-material-request-integrity-no-material-consumption-boundary", "v971",
     "fixtures/release/shard-readiness-v971.json",
     "keeps reviewed material absent unread unimported and unaccepted"},
    {8, "v973", "route-preview-candidate-material-request-integrity-router-write-wal-execution-boundary", "v972",
     "fixtures/release/shard-readiness-v972.json",
     "keeps router activation write routing store mutation WAL touch and execution disabled"},
    {9, "v974", "route-preview-candidate-material-request-integrity-cli-tcp-smoke-boundary", "v973",
     "fixtures/release/shard-readiness-v973.json",
     "keeps CLI and TCP smoke scoped to read-only integrity evidence"},
    {10, "v975", "route-preview-candidate-material-request-integrity-closeout-summary", "v974",
     "fixtures/release/shard-readiness-v974.json",
     "closes the integrity hardening chain without reopening material intake"},
}};

} // namespace

std::span<const StageRecord> candidate_material_request_integrity_stages() {
    return {candidate_material_request_integrity_stage_catalog.data(),
            candidate_material_request_integrity_stage_catalog.size()};
}

int first_candidate_material_request_integrity_release_number() {
    return first_candidate_material_request_integrity_release_number_value;
}

int planned_candidate_material_request_integrity_stage_count() {
    return static_cast<int>(candidate_material_request_integrity_stage_catalog.size());
}

} // namespace minikv::shard_preview_candidate_material_request_integrity_stages
