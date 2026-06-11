#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

namespace {

std::string format_validation(int source_stage_count = 20,
                              bool source_complete = true,
                              bool fresh_minikv = false,
                              bool fresh_java = false,
                              bool imports_node = false,
                              bool executes_node_checks = false,
                              bool starts_services = false,
                              bool reads_endpoints = false,
                              bool reads_credentials = false,
                              bool parses_receipts = false,
                              bool scans_java = false,
                              bool executes_downstream = false,
                              bool mutates_inventory = false,
                              int planned_checks = 20,
                              int completed_checks = 20,
                              int published_stages = 20,
                              int planned_stages = 20) {
    namespace validation =
        minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation;
    return validation::format_precheck_upstream_receipt_verification_split_non_participation_validation_json(
        source_stage_count,
        source_complete,
        fresh_minikv,
        fresh_java,
        imports_node,
        executes_node_checks,
        starts_services,
        reads_endpoints,
        reads_credentials,
        parses_receipts,
        scans_java,
        executes_downstream,
        mutates_inventory,
        planned_checks,
        completed_checks,
        published_stages,
        planned_stages);
}

void assert_validation_fails(const std::string& json) {
    minikv::test_support::assert_contains(
        json,
        "\"precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed\":false");
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto valid = format_validation();
    assert_contains(valid, "\"precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed\":true");
    assert_contains(valid, "\"sourceLocked\":true");
    assert_contains(valid, "\"nodePlanObservedOnly\":true");
    assert_contains(valid, "\"miniKvBoundaryClosed\":true");
    assert_contains(valid, "\"countsAligned\":true");
    assert_contains(valid, "\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true");
    assert_contains(valid, "\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false");
    assert_contains(valid, "\"endpointHandleRead\":false");
    assert_contains(valid, "\"writeRoutingAllowed\":false");
    assert_contains(valid, "\"executionAllowed\":false");

    assert_validation_fails(format_validation(19));
    assert_validation_fails(format_validation(20, false));
    assert_validation_fails(format_validation(20, true, true));
    assert_validation_fails(format_validation(20, true, false, true));
    assert_validation_fails(format_validation(20, true, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(
        format_validation(20, true, false, false, false, false, false, false, false, false, false, false, true));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, 20, 19));
    assert_validation_fails(format_validation(20, true, false, false, false, false, false, false, false, false, false,
                                              false, false, 20, 20, 19));

    return 0;
}
