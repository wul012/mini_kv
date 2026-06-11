#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_checks {
namespace {

constexpr std::array<CodeWalkthroughQualityGateNonParticipationCheck, 12> checks{{
    {1, "node-v2077-plan-observed-only", "Node v2077 plan",
     "mini-kv records the documentation-quality gate without executing Node routes or tests"},
    {2, "substantive-walkthrough-required", "mini-kv walkthrough governance",
     "future walkthroughs must explain real implementation value instead of merely listing files"},
    {3, "tiny-maintenance-may-omit-walkthrough", "mini-kv walkthrough governance",
     "small doc-only maintenance may skip walkthroughs rather than creating filler"},
    {4, "goal-role-non-goal-required", "walkthrough structure",
     "written walkthroughs must state goal, role, motivation, and what the version is not"},
    {5, "entry-and-module-responsibility-required", "walkthrough structure",
     "written walkthroughs must identify command entry points and module responsibilities"},
    {6, "response-and-evidence-structure-required", "walkthrough structure",
     "written walkthroughs must explain response shape, fixture lineage, and archive evidence"},
    {7, "boundary-fields-required", "walkthrough structure",
     "read_only, execution_allowed, order_authoritative, router, write, WAL, warnings, and blockers must be explained"},
    {8, "tests-explain-behavior-required", "walkthrough structure",
     "test sections must explain protected behavior rather than only naming commands"},
    {9, "archive-fixture-screenshot-alignment", "evidence alignment",
     "screenshots, fixtures, README notes, and walkthrough claims must agree"},
    {10, "placeholder-and-unsafe-claim-blocked", "quality floor",
     "placeholder walkthroughs and production-ready claims without evidence are rejected"},
    {11, "historical-docs-not-bulk-moved", "archive governance",
     "old walkthroughs stay in place unless a dedicated historical relocation pass is requested"},
    {12, "command-surface-read-only-clean-ci", "mini-kv closeout",
     "the quality gate is exposed only as read-only JSON and closes with cleanup and CI"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const CodeWalkthroughQualityGateNonParticipationCheck>
code_walkthrough_quality_gate_non_participation_checks() {
    return checks;
}

int planned_code_walkthrough_quality_gate_non_participation_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_code_walkthrough_quality_gate_non_participation_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published check count exceeds code walkthrough quality gate checks"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& check = checks[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(check.sequence) +
                  ",\"checkCode\":" + json_string(check.check_code) +
                  ",\"source\":" + json_string(check.source) +
                  ",\"protects\":" + json_string(check.protects) +
                  ",\"walkthroughQualityGateOnly\":true"
                  ",\"nodeWalkthroughFilesReadByMiniKv\":false"
                  ",\"nodeQualityRouteExecutedByMiniKv\":false"
                  ",\"miniKvScansNodeRepository\":false"
                  ",\"placeholderWalkthroughAllowed\":false"
                  ",\"unsafeProductionClaimAllowed\":false"
                  ",\"bulkHistoricalRelocationAllowed\":false"
                  ",\"startsNodeServices\":false"
                  ",\"startsJavaService\":false"
                  ",\"startsMiniKvService\":false"
                  ",\"mutatesStore\":false"
                  ",\"touchesWal\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_checks
