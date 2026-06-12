#include "minikv/shard_preview_f_folder_explanation_quality_closeout_rules.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_f_folder_explanation_quality_closeout_rules {
namespace {

constexpr std::array<FFolderExplanationQualityRule, 13> rules{{
    {1, "source-v1606-closeout-frozen", "mini-kv current release fixture",
     "v1606 external artifact dry-run closeout remains the frozen source"},
    {2, "chinese-walkthrough-writing-required", "code walkthrough documents",
     "future non-trivial mini-kv walkthroughs are written in Chinese by default"},
    {3, "minimum-3000-chinese-character-walkthrough", "code walkthrough documents",
     "each substantial version keeps at least one walkthrough with 3000 or more Chinese characters"},
    {4, "insufficient-word-count-expands-workload", "version scope",
     "if a walkthrough cannot naturally reach the floor, the version workload must be expanded before writing"},
    {5, "filler-padding-forbidden", "quality gate policy",
     "padding text to hit the counter is rejected; the implementation and evidence must earn the explanation"},
    {6, "single-version-large-scope", "future mini-kv evidence batches",
     "one substantial version may contain multiple internal tracks instead of five tiny version numbers"},
    {7, "human-machine-evidence-separated", "archive layout",
     "human explanations stay under f/<version>/explanation while machine command evidence stays separate"},
    {8, "picture-dir-requires-real-screenshot", "archive layout",
     "picture directories are created only when actual screenshot evidence exists"},
    {9, "f-root-per-version-archive", "archive layout",
     "new mini-kv archive evidence remains under f/<version> without combining unrelated versions"},
    {10, "walkthrough-structure-retained", "code walkthroughs",
     "walkthroughs keep goal, role, non-goals, entry command, evidence structure, boundaries, tests, and integration value"},
    {11, "no-node-folder-scan", "cross-project boundary",
     "mini-kv records Node v2103 as planning context without scanning Node f-folders"},
    {12, "no-production-execution", "runtime boundary",
     "explanation quality evidence does not open production execution, routes, writes, WAL, or services"},
    {13, "future-short-explanation-fails", "quality gate policy",
     "future short or empty explanations should fail review instead of relying on manual memory"},
}};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

} // namespace

std::span<const FFolderExplanationQualityRule> f_folder_explanation_quality_rules() {
    return rules;
}

int planned_f_folder_explanation_quality_rule_count() {
    return static_cast<int>(rules.size());
}

std::string format_f_folder_explanation_quality_rules_json(int published_rule_count) {
    if (published_rule_count < 0 || static_cast<std::size_t>(published_rule_count) > rules.size()) {
        throw std::out_of_range{"published f-folder explanation quality rule count exceeds rule catalog"};
    }

    std::string result = "[";
    for (int index = 0; index < published_rule_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        const auto& rule = rules[static_cast<std::size_t>(index)];
        result += "{\"sequence\":" + std::to_string(rule.sequence) +
                  ",\"ruleCode\":" + json_string(rule.rule_code) +
                  ",\"appliesTo\":" + json_string(rule.applies_to) +
                  ",\"requiredEvidence\":" + json_string(rule.required_evidence) +
                  ",\"readOnly\":true"
                  ",\"mutatesArchive\":false"
                  ",\"scansNodeFolders\":false"
                  ",\"startsServices\":false"
                  ",\"executionAllowed\":false}";
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_f_folder_explanation_quality_closeout_rules
