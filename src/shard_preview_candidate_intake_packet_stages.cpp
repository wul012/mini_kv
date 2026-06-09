#include "minikv/shard_preview_candidate_intake_packet_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_intake_packet_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_intake_packet_release_number_value = 931;

constexpr std::array<StageRecord, 25> candidate_intake_packet_stage_catalog = {{
    {1, "v931", "route-preview-candidate-intake-packet-source-freeze", "v930",
     "fixtures/release/shard-readiness-v930.json",
     "freezes the v930 submission precheck before intake packet handoff starts"},
    {2, "v932", "route-preview-candidate-intake-packet-node-plan-pin", "v931",
     "fixtures/release/shard-readiness-v931.json",
     "pins Node v1421 as read-only planning context"},
    {3, "v933", "route-preview-candidate-intake-packet-precheck-lineage", "v932",
     "fixtures/release/shard-readiness-v932.json",
     "keeps v930 precheck lineage visible before intake packet preparation"},
    {4, "v934", "route-preview-candidate-intake-packet-slot-catalog", "v933",
     "fixtures/release/shard-readiness-v933.json",
     "declares ten intake slots without opening material intake"},
    {5, "v935", "route-preview-candidate-intake-packet-guard-catalog", "v934",
     "fixtures/release/shard-readiness-v934.json",
     "declares ten intake guards without accepting documents"},
    {6, "v936", "route-preview-candidate-intake-packet-covered-checkpoints", "v935",
     "fixtures/release/shard-readiness-v935.json",
     "carries twenty-five source submission checkpoints as metadata"},
    {7, "v937", "route-preview-candidate-intake-packet-covered-validators", "v936",
     "fixtures/release/shard-readiness-v936.json",
     "carries twenty-five source submission validators as metadata"},
    {8, "v938", "route-preview-candidate-intake-packet-field-carry-forward", "v937",
     "fixtures/release/shard-readiness-v937.json",
     "carries twenty requested candidate fields without parsing values"},
    {9, "v939", "route-preview-candidate-intake-packet-gate-alignment", "v938",
     "fixtures/release/shard-readiness-v938.json",
     "records Node v1421 thirty-five gate expectation as read-only metadata"},
    {10, "v940", "route-preview-candidate-intake-packet-envelope-placeholder", "v939",
     "fixtures/release/shard-readiness-v939.json",
     "declares document envelope placeholders without materializing a document"},
    {11, "v941", "route-preview-candidate-intake-packet-material-intake-closed", "v940",
     "fixtures/release/shard-readiness-v940.json",
     "keeps reviewed real candidate material absent and unread"},
    {12, "v942", "route-preview-candidate-intake-packet-synthetic-material-rejected", "v941",
     "fixtures/release/shard-readiness-v941.json",
     "keeps synthetic candidate material rejected"},
    {13, "v943", "route-preview-candidate-intake-packet-payload-import-boundary", "v942",
     "fixtures/release/shard-readiness-v942.json",
     "keeps payload import and staging disabled"},
    {14, "v944", "route-preview-candidate-intake-packet-candidate-staging-boundary", "v943",
     "fixtures/release/shard-readiness-v943.json",
     "keeps candidate staging and materialization disabled"},
    {15, "v945", "route-preview-candidate-intake-packet-evaluation-boundary", "v944",
     "fixtures/release/shard-readiness-v944.json",
     "keeps candidate evaluation acceptance and rejection disabled"},
    {16, "v946", "route-preview-candidate-intake-packet-approval-boundary", "v945",
     "fixtures/release/shard-readiness-v945.json",
     "keeps approval grant capture disabled"},
    {17, "v947", "route-preview-candidate-intake-packet-signature-boundary", "v946",
     "fixtures/release/shard-readiness-v946.json",
     "keeps signature emission parsing and verification disabled"},
    {18, "v948", "route-preview-candidate-intake-packet-runtime-boundary", "v947",
     "fixtures/release/shard-readiness-v947.json",
     "keeps runtime payload import and execution disabled"},
    {19, "v949", "route-preview-candidate-intake-packet-router-boundary", "v948",
     "fixtures/release/shard-readiness-v948.json",
     "keeps document router installation disabled"},
    {20, "v950", "route-preview-candidate-intake-packet-write-boundary", "v949",
     "fixtures/release/shard-readiness-v949.json",
     "keeps write routing and store mutation disabled"},
    {21, "v951", "route-preview-candidate-intake-packet-wal-boundary", "v950",
     "fixtures/release/shard-readiness-v950.json",
     "keeps WAL touch load restore and compaction outside the packet"},
    {22, "v952", "route-preview-candidate-intake-packet-sibling-mutation-boundary", "v951",
     "fixtures/release/shard-readiness-v951.json",
     "keeps Node Java and archive sibling mutation disabled"},
    {23, "v953", "route-preview-candidate-intake-packet-current-fixture-parity", "v952",
     "fixtures/release/shard-readiness-v952.json",
     "checks the current fixture carries the intake packet section"},
    {24, "v954", "route-preview-candidate-intake-packet-cli-tcp-smoke-boundary", "v953",
     "fixtures/release/shard-readiness-v953.json",
     "keeps CLI and TCP smoke scoped to the short read-only intake command"},
    {25, "v955", "route-preview-candidate-intake-packet-closeout-summary", "v954",
     "fixtures/release/shard-readiness-v954.json",
     "closes the intake packet handoff without consuming reviewed material"},
}};

} // namespace

std::span<const StageRecord> candidate_intake_packet_stages() {
    return {candidate_intake_packet_stage_catalog.data(), candidate_intake_packet_stage_catalog.size()};
}

int first_candidate_intake_packet_release_number() {
    return first_candidate_intake_packet_release_number_value;
}

int planned_candidate_intake_packet_stage_count() {
    return static_cast<int>(candidate_intake_packet_stage_catalog.size());
}

} // namespace minikv::shard_preview_candidate_intake_packet_stages
