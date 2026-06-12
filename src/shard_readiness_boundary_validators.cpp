#include "shard_readiness_internal.hpp"

#include "minikv/command_catalog.hpp"

namespace minikv::shard_readiness::internal {

std::string format_boundaries_json() {
    return "{\"orderAuthoritative\":false,\"auditAuthoritative\":false,"
           "\"writeCommandsAllowed\":false,\"adminCommandsAllowed\":false,"
           "\"loadRestoreCompactAllowed\":false,\"setnxexExecutionAllowed\":false,"
           "\"multiProcessStarted\":false,\"storageDirectoriesCreated\":false,"
           "\"activeRouterInstalled\":false,\"archivedNodeEvidenceMutated\":false}";
}

std::string format_command_catalog_json() {
    return "{\"command\":\"SHARDJSON\",\"category\":\"read\",\"mutatesStore\":false,"
           "\"touchesWal\":false,\"stable\":true,\"extraArgsAllowed\":false,"
           "\"sideEffects\":[\"metadata_read\"]}";
}

std::string format_command_catalog_quality_json() {
    return "{\"releaseRangeStart\":\"v1546\",\"releaseRangeEnd\":\"v1607\","
           "\"scope\":\"command-catalog-single-source-refactor-plus-code-walkthrough-owner-request-mini-kv-receipt-"
           "external-artifact-and-f-folder-explanation-quality-gates\","
           "\"commandCount\":" +
           std::to_string(command_catalog::count()) +
           ",\"dispatchCatalogSplit\":true"
           ",\"contractCatalogDeduplicated\":true"
           ",\"helpTextGeneratedFromCatalog\":true"
           ",\"completionCandidatesGeneratedFromCatalog\":true"
           ",\"keyCompletionPolicyGeneratedFromCatalog\":true"
           ",\"runtimeCommandBehaviorChanged\":false"
           ",\"storageBehaviorChanged\":false"
           ",\"walBehaviorChanged\":false"
           ",\"routerBehaviorChanged\":false"
           ",\"nodeOrJavaFilesModified\":false}";
}

} // namespace minikv::shard_readiness::internal
