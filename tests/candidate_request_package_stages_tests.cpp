#include "minikv/shard_preview_candidate_request_package_stages.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cassert>
#include <string>

int main() {
    namespace stages = minikv::shard_preview_candidate_request_package_stages;
    const auto records = stages::candidate_document_request_package_stages();

    assert(stages::first_candidate_document_request_package_release_number() == 881);
    assert(stages::planned_candidate_document_request_package_stage_count() == 15);
    assert(records.size() == 15);
    assert(records.front().release_version == std::string{"v881"});
    assert(records.front().source_frozen_release_version == std::string{"v880"});
    assert(records.back().release_version == std::string{"v895"});
    assert(records.back().source_frozen_release_version == std::string{"v894"});
    assert(records.back().stage ==
           std::string{"route-preview-candidate-document-request-package-archive-closeout-final-summary"});

    const auto report = minikv::shard_route_preview_stage_chain::inspect_stage_chain(
        records, 15, stages::planned_candidate_document_request_package_stage_count(),
        stages::first_candidate_document_request_package_release_number());
    assert(report.published_stage_count == 15);
    assert(report.sequences_contiguous);
    assert(report.release_versions_contiguous);
    assert(report.source_frozen_release_versions_contiguous);
    assert(report.source_frozen_fixture_paths_contiguous);
    assert(report.chain_complete);
    return 0;
}
