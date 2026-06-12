#include "minikv/shard_preview_f_folder_explanation_quality_closeout_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_f_folder_explanation_quality_closeout_checks {
namespace {

constexpr std::array<FFolderExplanationQualityCloseoutCheck, 19> checks{{
    {1, "source-v1606-closeout-read", "fixtures/release/shard-readiness-v1606.json",
     "f-folder quality closeout starts from the latest mini-kv release fixture"},
    {2, "node-v2103-plan-observed", "Node v2103 f-folder explanation expansion closeout",
     "mini-kv records the Node quality direction without editing Node"},
    {3, "single-version-scope-locked", "user granularity decision",
     "one thick version replaces five fragmented micro versions"},
    {4, "chinese-writing-required", "code walkthrough rule",
     "future non-trivial mini-kv walkthroughs are written in Chinese"},
    {5, "minimum-3000-chinese-character-walkthrough", "code walkthrough rule",
     "each substantial version keeps one 3000-Chinese-character walkthrough floor"},
    {6, "no-filler-padding", "quality gate policy",
     "the explanation floor is met by larger work, not by padded prose"},
    {7, "workload-expands-if-explanation-short", "version scope",
     "a version that cannot naturally explain itself at length must be expanded before closeout"},
    {8, "short-explanation-rejected", "quality gate policy",
     "short placeholder explanations are treated as blockers"},
    {9, "walkthrough-shape-required", "code walkthrough rule",
     "walkthroughs explain goal role non-goals command evidence boundaries tests and integration value"},
    {10, "human-machine-evidence-split", "archive evidence layout",
     "human explanations and machine evidence stay separately understandable"},
    {11, "picture-dir-requires-screenshot", "archive evidence layout",
     "picture directories are not created unless a screenshot exists"},
    {12, "f-root-version-directory", "archive evidence layout",
     "new command evidence remains under a version-scoped f-root folder"},
    {13, "no-node-folder-scan", "cross-project boundary",
     "mini-kv does not walk or rewrite Node f-folder archives"},
    {14, "no-node-quality-route-import", "cross-project boundary",
     "mini-kv does not import or execute Node f-folder quality routes"},
    {15, "no-historical-walkthrough-rewrite", "documentation boundary",
     "historical walkthroughs are indexed and referenced rather than bulk rewritten"},
    {16, "no-service-start", "runtime boundary",
     "quality evidence does not start Node Java or mini-kv services"},
    {17, "no-router-or-write", "storage boundary",
     "quality evidence does not install routers or route writes"},
    {18, "no-wal-touch", "storage boundary",
     "quality evidence does not touch WAL or storage state"},
    {19, "no-execution-authority", "execution boundary",
     "quality evidence does not grant production or archive execution authority"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const FFolderExplanationQualityCloseoutCheck> f_folder_explanation_quality_closeout_checks() {
    return checks;
}

int planned_f_folder_explanation_quality_closeout_check_count() {
    return static_cast<int>(checks.size());
}

std::string format_f_folder_explanation_quality_closeout_checks_json(int published_check_count) {
    if (published_check_count < 0 || static_cast<std::size_t>(published_check_count) > checks.size()) {
        throw std::out_of_range{"published f-folder explanation quality closeout check count exceeds check catalog"};
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
                  ",\"explanationQualityEvidence\":true"
                  ",\"chineseWritingRequired\":true"
                  ",\"longChineseExplanationsRequired\":true"
                  ",\"minimumWalkthroughChineseCharacterCount\":3000"
                  ",\"insufficientWordCountExpandsVersionWorkload\":true"
                  ",\"fillerPaddingAllowed\":false"
                  ",\"shortExplanationsRejected\":true"
                  ",\"singleVersionLargeScope\":true"
                  ",\"multiVersionFragmentationAllowed\":false"
                  ",\"humanExplanationSeparateFromMachineEvidence\":true"
                  ",\"pictureDirsRequireRealScreenshots\":true"
                  ",\"scansNodeFolders\":false"
                  ",\"importsNodeQualityRoute\":false"
                  ",\"rewritesHistoricalWalkthroughs\":false"
                  ",\"createsPictureDirsWithoutEvidence\":false"
                  ",\"startsNodeServices\":false"
                  ",\"startsJavaService\":false"
                  ",\"startsMiniKvService\":false"
                  ",\"activeRouterInstalled\":false"
                  ",\"writeRoutingAllowed\":false"
                  ",\"mutatesStore\":false"
                  ",\"touchesWal\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_f_folder_explanation_quality_closeout_checks
