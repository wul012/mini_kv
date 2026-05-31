#include "minikv/shard_readiness_release_catalog.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_readiness::release_catalog {
namespace {

struct ReleaseCatalogEntry {
    std::string_view release_version;
    std::string_view status;
    std::string_view fixture_path;
    std::string_view primary_section;
    std::string_view evidence_digest;
    std::string_view boundary_catalog_version;
    int field_count;
    int group_count;
};

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

std::string format_entry_json(const ReleaseCatalogEntry& entry) {
    return "{\"releaseVersion\":" + json_string(entry.release_version) +
           ",\"status\":" + json_string(entry.status) +
           ",\"fixturePath\":" + json_string(entry.fixture_path) +
           ",\"primarySection\":" + json_string(entry.primary_section) +
           ",\"evidenceDigest\":" + json_string(entry.evidence_digest) +
           ",\"boundaryCatalogVersion\":" + json_string(entry.boundary_catalog_version) +
           ",\"fieldCount\":" + std::to_string(entry.field_count) +
           ",\"groupCount\":" + std::to_string(entry.group_count) +
           ",\"readOnly\":true"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"executionAllowed\":false}";
}

std::string format_entries_json() {
    const std::vector<ReleaseCatalogEntry> entries = {
        {"v161",
         "boundary-field-catalog-index-read-only",
         "fixtures/release/shard-readiness-v161.json",
         "boundaryCatalogIndex",
         "fnv1a64:2193962195ba633a",
         "read-only-boundary-fields.v2",
         290,
         15},
        {"v162",
         "slot-table-preview-read-only",
         "fixtures/release/shard-readiness-v162.json",
         "slotTablePreview",
         "fnv1a64:8c01a3657f07cfd1",
         "read-only-boundary-fields.v3",
         303,
         16},
        {"v163",
         "slot-table-preview-audit-read-only",
         "fixtures/release/shard-readiness-v163.json",
         "slotTablePreviewAudit",
         "fnv1a64:f81c5b47cb396df6",
         "read-only-boundary-fields.v4",
         319,
         17},
        {"v164",
         "slot-table-preview-audit-maintenance-read-only",
         "fixtures/release/shard-readiness-v164.json",
         "slotTablePreviewAuditMaintenance",
         "fnv1a64:9935ccb959b5a3b8",
         "read-only-boundary-fields.v5",
         337,
         18},
    };

    std::string result = "[";
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (i != 0) {
            result += ",";
        }
        result += format_entry_json(entries[i]);
    }
    result += "]";
    return result;
}

} // namespace

std::string format_release_catalog_json() {
    return "{\"catalogMode\":\"versioned-shard-readiness-release-catalog-read-only\","
           "\"sourceNodePlan\":\"docs/plans3/v431-post-credential-resolver-credential-handle-approval-route-group-split-roadmap.md\","
           "\"sourceFrozenReleaseVersion\":\"v164\","
           "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\","
           "\"catalogedReleaseCount\":4,"
           "\"latestCatalogedReleaseVersion\":\"v164\","
           "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\","
           "\"catalogedSections\":" +
           json_string_array({
               "boundaryCatalogIndex",
               "slotTablePreview",
               "slotTablePreviewAudit",
               "slotTablePreviewAuditMaintenance",
           }) +
           ",\"catalogEntries\":" + format_entries_json() +
           ",\"versionedFixtureCatalogOnly\":true"
           ",\"rollingCurrentUsedForFrozenCatalog\":false"
           ",\"publicShardJsonContractChanged\":false"
           ",\"fixturePathChanged\":false"
           ",\"shardJsonCommandChanged\":false"
           ",\"nodeAddsEvidenceGate\":false"
           ",\"runtimeGateApprovalPresent\":false"
           ",\"runtimeExecutionPacketExecutable\":false"
           ",\"startsJavaService\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsServices\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"liveReadAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"executionAllowed\":false"
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_readiness::release_catalog
