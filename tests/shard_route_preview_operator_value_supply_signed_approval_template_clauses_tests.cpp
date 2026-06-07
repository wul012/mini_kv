#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_clauses.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_template_clauses::
               planned_signed_approval_template_clause_count() == 25);

    const std::string clauses =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template_clauses::
            format_signed_approval_template_clauses_json(2);
    minikv::test_support::assert_contains(clauses, "\"sequence\":1");
    minikv::test_support::assert_contains(
        clauses,
        "\"clauseCode\":\"SIGNED_APPROVAL_TEMPLATE_PACKET_ID_CLAUSE\"");
    minikv::test_support::assert_contains(
        clauses,
        "\"rejectionCode\":\"REJECT_MISSING_SIGNED_APPROVAL_PACKET_ID\"");
    minikv::test_support::assert_contains(clauses, "\"sourceReviewControl\":\"approval-packet-review-control-01\"");
    minikv::test_support::assert_contains(clauses, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(clauses, "\"sequence\":25");
    minikv::test_support::assert_contains(
        clauses,
        "\"clauseCode\":\"SIGNED_APPROVAL_TEMPLATE_CAPTURE_PREFLIGHT_HANDOFF_CLAUSE\"");
    minikv::test_support::assert_contains(clauses, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(clauses, "\"blocksSignedApprovalCapture\":true");
    minikv::test_support::assert_contains(clauses, "\"blocksOperatorValueImport\":true");
    minikv::test_support::assert_contains(clauses, "\"blocksAutomaticSiblingImport\":true");
    minikv::test_support::assert_contains(clauses, "\"blocksRuntimePayload\":true");
    minikv::test_support::assert_contains(clauses, "\"blocksRouterActivation\":true");
    minikv::test_support::assert_contains(clauses, "\"blocksWrites\":true");
    minikv::test_support::assert_contains(clauses, "\"blocksExecution\":true");
    minikv::test_support::assert_contains(clauses, "\"readOnly\":true");

    const std::string completed_clauses =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template_clauses::
            format_signed_approval_template_clauses_json(25);
    minikv::test_support::assert_contains(completed_clauses, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_clauses, "\"publishedByCurrentStage\":true");

    return 0;
}
