#include "minikv/shard_preview_candidate_request_package_integrity_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_request_package_integrity_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_request_package_integrity_release_number_value = 896;

constexpr std::array<StageRecord, 25> candidate_request_package_integrity_stage_catalog = {{
    {1, "v896", "route-preview-candidate-request-package-integrity-source-freeze", "v895",
     "fixtures/release/shard-readiness-v895.json",
     "freezes the completed request package before integrity hardening starts"},
    {2, "v897", "route-preview-candidate-request-package-integrity-fixture-range-catalog", "v896",
     "fixtures/release/shard-readiness-v896.json",
     "catalogs the versioned fixture range without reading runtime archives"},
    {3, "v898", "route-preview-candidate-request-package-integrity-release-continuity", "v897",
     "fixtures/release/shard-readiness-v897.json",
     "checks release labels and source fixture continuity for the integrity chain"},
    {4, "v899", "route-preview-candidate-request-package-integrity-source-lineage", "v898",
     "fixtures/release/shard-readiness-v898.json",
     "pins the v895 request package as the only source package for this audit"},
    {5, "v900", "route-preview-candidate-request-package-integrity-status-parity", "v899",
     "fixtures/release/shard-readiness-v899.json",
     "preserves request package closeout status while adding no intake"},
    {6, "v901", "route-preview-candidate-request-package-integrity-item-count-parity", "v900",
     "fixtures/release/shard-readiness-v900.json",
     "keeps the fifteen request item count frozen from v895"},
    {7, "v902", "route-preview-candidate-request-package-integrity-check-count-parity", "v901",
     "fixtures/release/shard-readiness-v901.json",
     "keeps the fifteen acceptance check count frozen from v895"},
    {8, "v903", "route-preview-candidate-request-package-integrity-gate-count-parity", "v902",
     "fixtures/release/shard-readiness-v902.json",
     "keeps the thirty-eight request gate count frozen from v895"},
    {9, "v904", "route-preview-candidate-request-package-integrity-field-count-parity", "v903",
     "fixtures/release/shard-readiness-v903.json",
     "keeps the twenty requested candidate field count frozen from v895"},
    {10, "v905", "route-preview-candidate-request-package-integrity-missing-document-boundary", "v904",
     "fixtures/release/shard-readiness-v904.json",
     "keeps the missing real document boundary explicit and blocked"},
    {11, "v906", "route-preview-candidate-request-package-integrity-synthetic-document-boundary", "v905",
     "fixtures/release/shard-readiness-v905.json",
     "keeps synthetic candidate document acceptance disabled"},
    {12, "v907", "route-preview-candidate-request-package-integrity-payload-import-boundary", "v906",
     "fixtures/release/shard-readiness-v906.json",
     "keeps staged payload import and payload acceptance disabled"},
    {13, "v908", "route-preview-candidate-request-package-integrity-evaluation-boundary", "v907",
     "fixtures/release/shard-readiness-v907.json",
     "keeps candidate evaluation acceptance and rejection disabled"},
    {14, "v909", "route-preview-candidate-request-package-integrity-approval-boundary", "v908",
     "fixtures/release/shard-readiness-v908.json",
     "keeps approval grant capture separated and disabled"},
    {15, "v910", "route-preview-candidate-request-package-integrity-signature-boundary", "v909",
     "fixtures/release/shard-readiness-v909.json",
     "keeps signed approval emission and signature parsing disabled"},
    {16, "v911", "route-preview-candidate-request-package-integrity-runtime-boundary", "v910",
     "fixtures/release/shard-readiness-v910.json",
     "keeps runtime payload import disabled"},
    {17, "v912", "route-preview-candidate-request-package-integrity-write-routing-boundary", "v911",
     "fixtures/release/shard-readiness-v911.json",
     "keeps write routing and store mutation disabled"},
    {18, "v913", "route-preview-candidate-request-package-integrity-wal-boundary", "v912",
     "fixtures/release/shard-readiness-v912.json",
     "keeps WAL touch and compaction paths outside the audit"},
    {19, "v914", "route-preview-candidate-request-package-integrity-document-router-boundary", "v913",
     "fixtures/release/shard-readiness-v913.json",
     "keeps document router installation disabled"},
    {20, "v915", "route-preview-candidate-request-package-integrity-sibling-mutation-boundary", "v914",
     "fixtures/release/shard-readiness-v914.json",
     "keeps Node Java and archive sibling mutation disabled"},
    {21, "v916", "route-preview-candidate-request-package-integrity-archive-walk-boundary", "v915",
     "fixtures/release/shard-readiness-v915.json",
     "keeps runtime archive walks disabled during integrity review"},
    {22, "v917", "route-preview-candidate-request-package-integrity-current-fixture-parity", "v916",
     "fixtures/release/shard-readiness-v916.json",
     "checks the current fixture carries the integrity section without replacing v895"},
    {23, "v918", "route-preview-candidate-request-package-integrity-command-catalog-parity", "v917",
     "fixtures/release/shard-readiness-v917.json",
     "checks the integrity command remains read only and parser stable"},
    {24, "v919", "route-preview-candidate-request-package-integrity-cli-tcp-smoke-parity", "v918",
     "fixtures/release/shard-readiness-v918.json",
     "keeps CLI and TCP smoke expectations aligned without starting sibling services"},
    {25, "v920", "route-preview-candidate-request-package-integrity-closeout-summary", "v919",
     "fixtures/release/shard-readiness-v919.json",
     "closes the integrity hardening chain without adding another request echo"},
}};

} // namespace

std::span<const StageRecord> candidate_request_package_integrity_stages() {
    return {candidate_request_package_integrity_stage_catalog.data(),
            candidate_request_package_integrity_stage_catalog.size()};
}

int first_candidate_request_package_integrity_release_number() {
    return first_candidate_request_package_integrity_release_number_value;
}

int planned_candidate_request_package_integrity_stage_count() {
    return static_cast<int>(candidate_request_package_integrity_stage_catalog.size());
}

} // namespace minikv::shard_preview_candidate_request_package_integrity_stages
