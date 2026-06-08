#include "minikv/shard_preview_candidate_request_package_stages.hpp"

#include <array>

namespace minikv::shard_preview_candidate_request_package_stages {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr int first_candidate_request_package_release_number = 881;

constexpr std::array<StageRecord, 15> candidate_request_package_stages = {{
    {1, "v881", "route-preview-candidate-document-request-package-source-intake-freeze", "v880",
     "fixtures/release/shard-readiness-v880.json",
     "freezes the compared package evidence intake audit before requesting real candidate material"},
    {2, "v882", "route-preview-candidate-document-request-package-request-field-catalog", "v881",
     "fixtures/release/shard-readiness-v881.json",
     "declares the twenty requested candidate fields without accepting a document"},
    {3, "v883", "route-preview-candidate-document-request-package-operator-instruction-map", "v882",
     "fixtures/release/shard-readiness-v882.json",
     "maps request instructions to operator-owned handles without creating payload content"},
    {4, "v884", "route-preview-candidate-document-request-package-identity-and-provenance-request", "v883",
     "fixtures/release/shard-readiness-v883.json",
     "requests identity and provenance handles while keeping identity secrets absent"},
    {5, "v885", "route-preview-candidate-document-request-package-digest-lineage-request", "v884",
     "fixtures/release/shard-readiness-v884.json",
     "requests digest lineage references without recalculating or importing payload bytes"},
    {6, "v886", "route-preview-candidate-document-request-package-signature-envelope-reference-request", "v885",
     "fixtures/release/shard-readiness-v885.json",
     "requests detached signature envelope references without parsing signatures"},
    {7, "v887", "route-preview-candidate-document-request-package-policy-lock-request", "v886",
     "fixtures/release/shard-readiness-v886.json",
     "requests policy lock handles while runtime execution stays disabled"},
    {8, "v888", "route-preview-candidate-document-request-package-approval-separation-request", "v887",
     "fixtures/release/shard-readiness-v887.json",
     "requests approval separation markers without capturing or emitting approval grants"},
    {9, "v889", "route-preview-candidate-document-request-package-missing-document-freeze-check", "v888",
     "fixtures/release/shard-readiness-v888.json",
     "pins the missing-document state as blocked until a reviewed real candidate exists"},
    {10, "v890", "route-preview-candidate-document-request-package-synthetic-document-freeze-check", "v889",
     "fixtures/release/shard-readiness-v889.json",
     "pins synthetic candidate material rejection before any acceptance path exists"},
    {11, "v891", "route-preview-candidate-document-request-package-payload-import-freeze-check", "v890",
     "fixtures/release/shard-readiness-v890.json",
     "keeps staged payload import disabled for the request package"},
    {12, "v892", "route-preview-candidate-document-request-package-evaluation-and-acceptance-freeze-check", "v891",
     "fixtures/release/shard-readiness-v891.json",
     "keeps candidate evaluation acceptance and rejection actions disabled"},
    {13, "v893", "route-preview-candidate-document-request-package-runtime-and-write-freeze-check", "v892",
     "fixtures/release/shard-readiness-v892.json",
     "keeps runtime payload import write routing WAL touch and execution disabled"},
    {14, "v894", "route-preview-candidate-document-request-package-sibling-mutation-freeze-check", "v893",
     "fixtures/release/shard-readiness-v893.json",
     "keeps Java Node and archive sibling mutation disabled"},
    {15, "v895", "route-preview-candidate-document-request-package-archive-closeout-final-summary", "v894",
     "fixtures/release/shard-readiness-v894.json",
     "closes the read-only request package with archive references and no document execution"},
}};

} // namespace

std::span<const StageRecord> candidate_document_request_package_stages() {
    return {candidate_request_package_stages.data(), candidate_request_package_stages.size()};
}

int first_candidate_document_request_package_release_number() {
    return first_candidate_request_package_release_number;
}

int planned_candidate_document_request_package_stage_count() {
    return static_cast<int>(candidate_request_package_stages.size());
}

} // namespace minikv::shard_preview_candidate_request_package_stages
