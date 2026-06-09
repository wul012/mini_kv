#include "minikv/shard_preview_candidate_profile_section_checks.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_profile_section_checks {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<ProfileSectionCheck, 25> profile_section_check_catalog = {{
    {1, "source-integrity-fixture-frozen", "fixtures/release/shard-readiness-v1010.json",
     "v1010 remains the immutable source material submission precheck integrity fixture"},
    {2, "node-renderer-split-plan-pinned", "Node v1481 candidate document profile section renderer split roadmap",
     "mini-kv treats the split as read-only ownership context"},
    {3, "candidate-section-group-boundary", "Node v1481 five-section group",
     "only the five candidate document profile sections are represented"},
    {4, "heading-catalog-declared", "candidate profile section stage catalog",
     "section headings are explicit and versioned"},
    {5, "request-package-section-carried", "candidate request package evidence lineage",
     "request package section identity remains metadata only"},
    {6, "submission-precheck-section-carried", "candidate submission precheck evidence lineage",
     "submission precheck section identity remains metadata only"},
    {7, "intake-packet-section-carried", "candidate intake packet evidence lineage",
     "intake packet section identity remains metadata only"},
    {8, "material-request-section-carried", "candidate material request evidence lineage",
     "material request section identity remains metadata only"},
    {9, "material-submission-precheck-section-carried", "candidate material submission precheck evidence lineage",
     "material submission precheck section identity remains metadata only"},
    {10, "renderer-output-stability", "Node v1481 maintenance split",
     "renderer ownership changes do not imply route-facing output changes"},
    {11, "route-markdown-parity", "Node v1481 route Markdown closeout",
     "mini-kv records parity without rendering Node Markdown"},
    {12, "no-fresh-evidence-required", "Node v1481 cross-project parallel plan",
     "Node does not require fresh mini-kv evidence in this range"},
    {13, "disabled-upstream-probe-boundary", "candidate profile section disabled upstream probes",
     "upstream probe visibility remains fail-closed metadata"},
    {14, "type-catalog-compatibility", "Node v1481 type module catalog",
     "mini-kv does not import or execute the Node catalog"},
    {15, "live-startup-boundary", "service startup boundary fields",
     "Node Java and mini-kv services are not started by this evidence"},
    {16, "archive-scan-boundary", "archive boundary fields",
     "runtime archive walks remain disabled"},
    {17, "runtime-probe-boundary", "runtime boundary fields",
     "live reads and runtime probes remain disabled"},
    {18, "router-boundary", "router boundary fields", "active candidate document routing remains disabled"},
    {19, "write-wal-boundary", "write boundary fields",
     "write routing store mutation WAL touch load restore and compact remain disabled"},
    {20, "execution-boundary", "execution boundary fields", "profile section evidence remains non-executable"},
    {21, "command-catalog-exposure", "COMMANDS and COMMANDSJSON",
     "the profile section command is parser visible and read only"},
    {22, "shardjson-single-embedding", "SHARDJSON",
     "current SHARDJSON embeds exactly one candidate profile section evidence block"},
    {23, "validation-split", "candidate profile section validation module",
     "validation stays separate from the evidence formatter"},
    {24, "cli-tcp-smoke-parity", "CLI and TCP smoke",
     "smoke checks use the same read-only profile section command"},
    {25, "no-document-intake-opened", "profile section closeout",
     "closeout does not open document intake or material submission"},
}};

std::string format_check_json(const ProfileSectionCheck& check) {
    return "{\"sequence\":" + std::to_string(check.sequence) +
           ",\"checkCode\":" + json_string(check.check_code) +
           ",\"source\":" + json_string(check.source) +
           ",\"protects\":" + json_string(check.protects) +
           ",\"profileSectionOnly\":true"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"candidateDocumentRead\":false"
           ",\"materialSubmissionOpened\":false"
           ",\"materialPayloadImported\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"documentIntakeOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const ProfileSectionCheck> candidate_profile_section_checks() {
    return {profile_section_check_catalog.data(), profile_section_check_catalog.size()};
}

int planned_candidate_profile_section_check_count() {
    return static_cast<int>(profile_section_check_catalog.size());
}

std::string format_candidate_profile_section_checks_json(int published_check_count) {
    if (published_check_count < 0 ||
        static_cast<std::size_t>(published_check_count) > profile_section_check_catalog.size()) {
        throw std::out_of_range{"published candidate profile section check count exceeds check catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_check_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_check_json(profile_section_check_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_profile_section_checks
