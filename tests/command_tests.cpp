#include "minikv/command.hpp"
#include "minikv/store.hpp"
#include "minikv/version.hpp"
#include "minikv/wal.hpp"
#include "test_support.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <string>
#include <string_view>
#include <thread>

namespace {

const minikv::CommandBreakdownMetrics* find_command_metrics(const minikv::CommandProcessorMetrics& metrics,
                                                            std::string_view command) {
    for (const auto& command_metrics : metrics.command_breakdown) {
        if (command_metrics.command == command) {
            return &command_metrics;
        }
    }
    return nullptr;
}

using minikv::test_support::assert_response_contains;
using minikv::test_support::read_fixture_text;

std::string extract_json_string_field(const std::string& json, std::string_view field) {
    const std::string marker = "\"" + std::string{field} + "\":\"";
    const auto start = json.find(marker);
    assert(start != std::string::npos);
    const auto value_start = start + marker.size();
    const auto value_end = json.find('"', value_start);
    assert(value_end != std::string::npos);
    return json.substr(value_start, value_end - value_start);
}

} // namespace

int main() {
    using namespace std::chrono_literals;

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("");
    assert(result.response.empty());
    assert(!result.should_close);

    result = processor.execute("PING");
    assert(result.response == "PONG");
    assert(!result.should_close);

    result = processor.execute("PING hello from ping");
    assert(result.response == "hello from ping");
    assert(!result.should_close);

    result = processor.execute("SET name mini-kv");
    assert(result.response == "OK inserted");
    assert(!result.should_close);

    result = processor.execute("GET name");
    assert(result.response == "mini-kv");

    result = processor.execute("set name redis-ish");
    assert(result.response == "OK updated");

    result = processor.execute("GET name");
    assert(result.response == "redis-ish");

    result = processor.execute("SIZE");
    assert(result.response == "1");

    result = processor.execute("SET alpha first");
    assert(result.response == "OK inserted");

    result = processor.execute("KEYS");
    assert(result.response == "key_count=2 keys=alpha name");

    result = processor.execute("SET alpine mountain");
    assert(result.response == "OK inserted");

    result = processor.execute("KEYS alp");
    assert(result.response == "key_count=2 prefix=alp keys=alpha alpine");

    result = processor.execute("KEYS z");
    assert(result.response == "key_count=0 prefix=z keys=");

    result = processor.execute("KEYS alp extra");
    assert(result.response == "ERR usage: KEYS [prefix]");

    result = processor.execute("KEYSJSON extra more");
    assert(result.response == "ERR usage: KEYSJSON [prefix]");

    result = processor.execute("KEYSJSON");
    assert(result.response == "{\"prefix\":null,\"key_count\":3,\"keys\":[\"alpha\",\"alpine\",\"name\"],\"truncated\":false,\"limit\":1000}");

    result = processor.execute("KEYSJSON alp");
    assert(result.response == "{\"prefix\":\"alp\",\"key_count\":2,\"keys\":[\"alpha\",\"alpine\"],\"truncated\":false,\"limit\":1000}");

    result = processor.execute("KEYSJSON z");
    assert(result.response == "{\"prefix\":\"z\",\"key_count\":0,\"keys\":[],\"truncated\":false,\"limit\":1000}");

    result = processor.execute("DEL alpha");
    assert(result.response == "1");

    result = processor.execute("DEL alpine");
    assert(result.response == "1");

    result = processor.execute("DEL name");
    assert(result.response == "1");

    result = processor.execute("GET name");
    assert(result.response == "(nil)");

    result = processor.execute("SET temp keep");
    assert(result.response == "OK inserted");

    result = processor.execute("TTL temp");
    assert(result.response == "-1");

    result = processor.execute("EXPIRE temp 1");
    assert(result.response == "1");

    result = processor.execute("TTL temp");
    const long long remaining = std::stoll(result.response);
    assert(remaining >= 0);
    assert(remaining <= 1);

    std::this_thread::sleep_for(1100ms);

    result = processor.execute("GET temp");
    assert(result.response == "(nil)");

    result = processor.execute("TTL temp");
    assert(result.response == "-2");

    result = processor.execute("EXPIRE temp 1");
    assert(result.response == "0");

    result = processor.execute("EXPIRE temp 0");
    assert(result.response == "ERR usage: EXPIRE key seconds");

    result = processor.execute("TTL temp extra");
    assert(result.response == "ERR usage: TTL key");

    result = processor.execute("SETNXEX token 1 first");
    assert(result.response == "1");

    result = processor.execute("SETNXEX token 1 duplicate");
    assert(result.response == "0");

    result = processor.execute("GET token");
    assert(result.response == "first");

    result = processor.execute("TTL token");
    const long long token_remaining = std::stoll(result.response);
    assert(token_remaining >= 0);
    assert(token_remaining <= 1);

    std::this_thread::sleep_for(1100ms);

    result = processor.execute("GET token");
    assert(result.response == "(nil)");

    result = processor.execute("SETNXEX token 1 second");
    assert(result.response == "1");

    result = processor.execute("GET token");
    assert(result.response == "second");

    result = processor.execute("SETNXEX token 0 bad");
    assert(result.response == "ERR usage: SETNXEX key seconds value");

    result = processor.execute("SETNXEX token 1");
    assert(result.response == "ERR usage: SETNXEX key seconds value");

    const auto snapshot_path = std::filesystem::path{"minikv-command-snapshot-test.snap"};
    std::filesystem::remove(snapshot_path);

    store.clear();

    result = processor.execute("SET snap saved value");
    assert(result.response == "OK inserted");

    result = processor.execute(std::string{"SAVE "} + snapshot_path.string());
    assert(result.response == "OK saved 1");

    result = processor.execute("DEL snap");
    assert(result.response == "1");

    result = processor.execute(std::string{"LOAD "} + snapshot_path.string());
    assert(result.response == "OK loaded 1");

    result = processor.execute("GET snap");
    assert(result.response == "saved value");

    std::filesystem::remove(snapshot_path);

    result = processor.execute("COMPACT now");
    assert(result.response == "ERR usage: COMPACT");

    result = processor.execute("COMPACT");
    assert(result.response == "ERR WAL not enabled");

    result = processor.execute("WALINFO extra");
    assert(result.response == "ERR usage: WALINFO");

    result = processor.execute("WALINFO");
    assert(result.response == "ERR WAL not enabled");

    result = processor.execute("STATS extra");
    assert(result.response == "ERR usage: STATS");

    result = processor.execute("STATS");
    assert(result.response.find("live_keys=1") != std::string::npos);
    assert(result.response.find("wal_enabled=no") != std::string::npos);
    assert(result.response.find("connection_stats_available=no") != std::string::npos);

    result = processor.execute("HEALTH extra");
    assert(result.response == "ERR usage: HEALTH");

    result = processor.execute("HEALTH");
    assert(result.response.find("OK live_keys=1") != std::string::npos);
    assert(result.response.find("wal_enabled=no") != std::string::npos);
    assert(result.response.find("compact_recommended=na") != std::string::npos);
    assert(result.response.find("connection_stats_available=no") != std::string::npos);

    result = processor.execute("INFO extra");
    assert(result.response == "ERR usage: INFO");

    result = processor.execute("INFO");
    assert(result.response.find("version=" + std::string{minikv::version}) != std::string::npos);
    assert(result.response.find("protocol=inline") != std::string::npos);
    assert(result.response.find("uptime_seconds=") != std::string::npos);
    assert(result.response.find("live_keys=1") != std::string::npos);
    assert(result.response.find("wal_enabled=no") != std::string::npos);
    assert(result.response.find("metrics_enabled=no") != std::string::npos);
    assert(result.response.find("max_request_bytes=0") != std::string::npos);

    result = processor.execute("INFOJSON extra");
    assert(result.response == "ERR usage: INFOJSON");

    result = processor.execute("INFOJSON");
    assert(result.response.find("\"schema_version\":1") != std::string::npos);
    assert(result.response.find("\"read_only\":true") != std::string::npos);
    assert(result.response.find("\"execution_allowed\":false") != std::string::npos);
    assert(result.response.find("\"order_authoritative\":false") != std::string::npos);
    assert(result.response.find("\"evidence_type\":\"runtime_identity\"") != std::string::npos);
    assert(result.response.find("\"version\":\"" + std::string{minikv::version} + "\"") != std::string::npos);
    assert(result.response.find("\"server\":{\"protocol\":[\"inline\"]") != std::string::npos);
    assert(result.response.find("\"uptime_seconds\":") != std::string::npos);
    assert(result.response.find("\"max_request_bytes\":0") != std::string::npos);
    assert(result.response.find("\"store\":{\"live_keys\":1}") != std::string::npos);
    assert(result.response.find("\"wal\":{\"enabled\":false}") != std::string::npos);
    assert(result.response.find("\"metrics\":{\"enabled\":false}") != std::string::npos);
    assert_response_contains(result, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/80/\"");
    assert_response_contains(result, "\"no_restore_proof\":true");
    assert_response_contains(result, "\"upload_allowed\":false");
    assert_response_contains(result, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
    assert_response_contains(result, "\"artifact_root\":\"c/\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/81/\"");
    assert_response_contains(result, "\"retention_days\":30");
    assert_response_contains(result, "\"retention_mode\":\"dry-run-contract-only\"");
    assert_response_contains(result, "\"github_artifact_upload_attempted\":false");
    assert_response_contains(result, "\"production_window_allowed\":false");
    assert(result.response.find("\"diagnostics\":{\"write_commands_executed\":false") != std::string::npos);

    result = processor.execute("COMMANDS extra");
    assert(result.response == "ERR usage: COMMANDS");

    result = processor.execute("COMMANDS");
    assert(result.response.find("command_count=36") != std::string::npos);
    assert(result.response.find("PING(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SET(category=write,mutates_store=yes,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("SETNXEX(category=write,mutates_store=yes,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("GET(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("KEYSJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("COMPACT(category=admin,mutates_store=no,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("COMMANDSJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTE(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVERIFYREPORTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
                "SHARDROUTEVERIFYREPORTCLOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVERIFYREPORTARCHIVEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("EXPLAINJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("CHECKJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SMOKEJSON(category=meta,mutates_store=no,touches_wal=yes,stable=yes)") != std::string::npos);
    assert(result.response.find("STORAGEJSON(category=read,mutates_store=no,touches_wal=yes,stable=yes)") != std::string::npos);

    result = processor.execute("COMMANDSJSON extra");
    assert(result.response == "ERR usage: COMMANDSJSON");

    result = processor.execute("COMMANDSJSON");
    assert(result.response.find("\"commands\":[") != std::string::npos);
    assert(result.response.find("\"name\":\"PING\",\"category\":\"meta\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SET\",\"category\":\"write\",\"mutates_store\":true,\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"SETNXEX\",\"category\":\"write\",\"mutates_store\":true,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"GET\",\"category\":\"read\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"KEYSJSON\",\"category\":\"read\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"LOAD\",\"category\":\"admin\",\"mutates_store\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"COMMANDSJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTE\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVERIFYJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"EXPLAINJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"CHECKJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"SMOKEJSON\",\"category\":\"meta\",\"mutates_store\":false,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"STORAGEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"description\":\"Read command catalog as JSON\"") != std::string::npos);

    result = processor.execute("SHARDJSON");
    assert_response_contains(result, "\"contract\":\"shard-readiness.v1\"");
    assert_response_contains(result, "\"project\":\"mini-kv\"");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} +
                                        "\",\"releaseVersion\":\"v347\"");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardEnabled\":false");
    assert_response_contains(result, "\"shardCount\":1");
    assert_response_contains(result, "\"slotCount\":16");
    assert_response_contains(result, "\"routingMode\":\"single-shard-readiness-prototype\"");
    assert_response_contains(result, "\"evidencePath\":\"fixtures/release/shard-readiness.json\"");
    assert_response_contains(result, "\"status\":\"node-route-catalog-cleanup-post-closeout-continuity-read-only\"");
    assert_response_contains(result, "\"slotTablePreview\":{\"previewMode\":\"single-shard-slot-table-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v425-post-credential-resolver-disabled-runtime-shell-readiness-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v161\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v161.json\"");
    assert_response_contains(result, "\"slotTableMaterializedForEvidence\":true");
    assert_response_contains(result, "\"slotTablePreviewOnly\":true");
    assert_response_contains(result, "\"readOnlyShardMapPreview\":true");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"writeCommandsAllowed\":false");
    assert_response_contains(result, "\"adminCommandsAllowed\":false");
    assert_response_contains(result, "\"loadRestoreCompactAllowed\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"slotAssignments\":[{\"slot\":0,\"shardId\":\"shard-0\","
                                     "\"routeMode\":\"preview-only\",\"readOnly\":true,\"writesAllowed\":false,"
                                     "\"routerInstalled\":false}");
    assert_response_contains(result, "\"slotTablePreviewAudit\":{\"auditMode\":\"slot-table-preview-consistency-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v426-post-credential-resolver-runtime-shell-decision-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v162\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v162.json\"");
    assert_response_contains(result, "\"auditedPreviewSection\":\"slotTablePreview\"");
    assert_response_contains(result, "\"expectedSlotCount\":16");
    assert_response_contains(result, "\"observedSlotCount\":16");
    assert_response_contains(result, "\"contiguousSlotRange\":true");
    assert_response_contains(result, "\"duplicateSlotsDetected\":false");
    assert_response_contains(result, "\"unassignedSlotsDetected\":false");
    assert_response_contains(result, "\"allSlotsMapped\":true");
    assert_response_contains(result, "\"allAssignmentsReadOnly\":true");
    assert_response_contains(result, "\"allAssignmentsPreviewOnly\":true");
    assert_response_contains(result, "\"derivedFromSlotPreviewModule\":true");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance\":{\"maintenanceMode\":"
                                     "\"slot-table-preview-audit-formatter-split-contract-preserving\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v430-post-credential-resolver-signed-human-approval-artifact-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v163\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v163.json\"");
    assert_response_contains(result, "\"auditFormatterExtracted\":true");
    assert_response_contains(result, "\"slotPreviewModuleOwnsAssignments\":true");
    assert_response_contains(result, "\"slotPreviewAuditStillPublished\":true");
    assert_response_contains(result, "\"slotTablePreviewStillPublished\":true");
    assert_response_contains(result, "\"publicShardJsonContractChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreview\":{\"rolloutMode\":\"read-only-shard-route-preview-rollout\"");
    assert_response_contains(result, "\"rolloutStage\":\"route-preview-release-package-audit\"");
    assert_response_contains(result, "\"rolloutStageSequence\":20");
    assert_response_contains(result, "\"rolloutReleaseVersion\":\"v298\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"commands\":[\"SHARDROUTE\",\"SHARDROUTEJSON\"]");
    assert_response_contains(result, "\"sampleSlotsPinnedToExistingEvidence\":true");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerification\":{\"rolloutMode\":"
                                     "\"read-only-shard-route-preview-verification-rollout\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v318\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v318.json\"");
    assert_response_contains(result, "\"rolloutStage\":\"route-preview-verification-closeout-audit\"");
    assert_response_contains(result, "\"rolloutStageSequence\":21");
    assert_response_contains(result, "\"rolloutReleaseVersion\":\"v319\"");
    assert_response_contains(result, "\"publishedStageCount\":21");
    assert_response_contains(result, "\"verificationCommandAvailable\":true");
    assert_response_contains(result, "\"verifiedCommand\":\"SHARDROUTEJSON\"");
    assert_response_contains(result, "\"commands\":[\"SHARDROUTEVERIFYJSON\"]");
    assert_response_contains(result, "\"executesRoute\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReport\":{\"rolloutMode\":"
                                     "\"read-only-shard-route-preview-verification-report-rollout\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v338\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v338.json\"");
    assert_response_contains(result, "\"rolloutStage\":\"route-preview-verification-report-closeout-audit\"");
    assert_response_contains(result, "\"rolloutStageSequence\":20");
    assert_response_contains(result, "\"rolloutReleaseVersion\":\"v339\"");
    assert_response_contains(result, "\"publishedStageCount\":20");
    assert_response_contains(result, "\"reportCommandAvailable\":true");
    assert_response_contains(result, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_response_contains(result, "\"previewCommand\":\"SHARDROUTEJSON\"");
    assert_response_contains(result, "\"commands\":[\"SHARDROUTEVERIFYREPORTJSON\"]");
    assert_response_contains(result, "\"executesRoute\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportCloseout\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-closeout.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\"");
    assert_response_contains(result, "\"closeoutMode\":\"read-only-route-preview-verification-report-closeout-summary\"");
    assert_response_contains(result, "\"closedReleaseRangeStart\":\"v320\"");
    assert_response_contains(result, "\"closedReleaseRangeEnd\":\"v339\"");
    assert_response_contains(result, "\"closedVersionCount\":20");
    assert_response_contains(result, "\"reportStageCount\":20");
    assert_response_contains(result, "\"allReportStagesPublished\":true");
    assert_response_contains(result, "\"archiveManifestPlannedReleaseVersion\":\"v341\"");
    assert_response_contains(result, "\"archiveVerificationPlannedReleaseVersion\":\"v342\"");
    assert_response_contains(result, "\"executesRoute\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardRoutePreviewVerificationReportArchive\":{\"contract\":"
                                     "\"shard-route-preview-verification-report-archive.v1\"");
    assert_response_contains(result, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_response_contains(result, "\"archiveMode\":\"read-only-route-preview-verification-report-archive-chain\"");
    assert_response_contains(result, "\"sourceCloseoutReleaseVersion\":\"v340\"");
    assert_response_contains(result, "\"archiveStage\":\"route-preview-verification-report-archive-continuity-map\"");
    assert_response_contains(result, "\"archiveStageSequence\":7");
    assert_response_contains(result, "\"archiveReleaseVersion\":\"v347\"");
    assert_response_contains(result, "\"publishedStageCount\":7");
    assert_response_contains(result, "\"plannedStageCount\":20");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v346\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v346.json\"");
    assert_response_contains(result, "\"archiveManifestPublished\":true");
    assert_response_contains(result, "\"filesystemReadPerformed\":false");
    assert_response_contains(result, "\"runtimeArchiveWalkAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog\":{\"catalogMode\":"
                                     "\"versioned-shard-readiness-release-catalog-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v431-post-credential-resolver-credential-handle-approval-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v164\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v164.json\"");
    assert_response_contains(result, "\"catalogedReleaseCount\":4");
    assert_response_contains(result, "\"latestCatalogedReleaseVersion\":\"v164\"");
    assert_response_contains(result, "\"latestCatalogedDigest\":\"fnv1a64:9935ccb959b5a3b8\"");
    assert_response_contains(result, "\"catalogedSections\":[\"boundaryCatalogIndex\",\"slotTablePreview\","
                                     "\"slotTablePreviewAudit\",\"slotTablePreviewAuditMaintenance\"]");
    assert_response_contains(result, "\"primarySection\":\"slotTablePreviewAuditMaintenance\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v5\"");
    assert_response_contains(result, "\"versionedFixtureCatalogOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_response_contains(result, "\"publicShardJsonContractChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit\":{\"auditMode\":"
                                     "\"shard-readiness-release-catalog-consistency-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v432-post-credential-resolver-endpoint-handle-allowlist-approval-route-group-split-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v165\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v165.json\"");
    assert_response_contains(result, "\"auditedCatalogSection\":\"shardReadinessReleaseCatalog\"");
    assert_response_contains(result, "\"expectedReleaseCount\":4");
    assert_response_contains(result, "\"observedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeStart\":\"v161\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v164\"");
    assert_response_contains(result, "\"contiguousReleaseRange\":true");
    assert_response_contains(result, "\"duplicateReleasesDetected\":false");
    assert_response_contains(result, "\"missingCatalogEntriesDetected\":false");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestDigestMatchesFrozenSource\":true");
    assert_response_contains(result, "\"catalogOnlyAudit\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"activeRouterInstalled\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"archivedNodeEvidenceMutated\":false");
    assert_response_contains(result, "\"commandCatalog\":{\"command\":\"SHARDJSON\",\"category\":\"read\"");
    assert_response_contains(result, "\"fixtureParity\":{\"currentFixturePath\":\"fixtures/release/shard-readiness.json\"");
    assert_response_contains(result, "\"archiveCompatibility\":{\"preservesNodeArchivedEvidence\":true");
    assert_response_contains(result, "\"historicalFallback\":{\"previousConsumedReleaseVersion\":\"v346\"");
    assert_response_contains(result, "\"previousConsumedFixturePath\":\"fixtures/release/shard-readiness-v346.json\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance\":{\"maintenanceMode\":"
                                     "\"history-fixture-archive-formatter-split-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v179\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v179.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:d88350b581f2c458\"");
    assert_response_contains(result, "\"splitFrom\":\"src/shard_readiness.cpp\"");
    assert_response_contains(result, "\"splitInto\":\"src/shard_readiness_history.cpp\"");
    assert_response_contains(result, "\"preservesFixtureParity\":true");
    assert_response_contains(result, "\"preservesArchiveCompatibility\":true");
    assert_response_contains(result, "\"preservesHistoricalFallback\":true");
    assert_response_contains(result, "\"changesShardJsonCommand\":false");
    assert_response_contains(result, "\"changesFixturePath\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"rollingCurrentUsedForHistoricalBaseline\":false");
    assert_response_contains(result, "\"activePrototypePlan\":{\"planMode\":\"prerequisite-only\"");
    assert_response_contains(result, "\"activeShardPrototypeAllowed\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"activePrototypePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenRouterActivationAllowed\":false");
    assert_response_contains(result, "\"frozenWriteRoutingAllowed\":false");
    assert_response_contains(result, "\"consumerHandoff\":{\"handoffMode\":\"frozen-evidence-only\"");
    assert_response_contains(result, "\"readyForNodeConsumption\":true");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"liveReadGatePlan\":{\"planMode\":\"service-lifecycle-prerequisite-only\"");
    assert_response_contains(result, "\"liveReadGateAllowed\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"requiresCleanup\":true");
    assert_response_contains(result, "\"liveReadGatePlanFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenRuntimeProbeAllowed\":false");
    assert_response_contains(result, "\"frozenExecutionAllowed\":false");
    assert_response_contains(result, "\"operatorServiceLifecycleTemplate\":{\"evidenceMode\":\"template-only-no-runtime\"");
    assert_response_contains(result, "\"operatorOwnedServiceLifecycleRequired\":true");
    assert_response_contains(result, "\"serviceOwnerDeclared\":false");
    assert_response_contains(result, "\"getOnlySmokeTargetDeclared\":false");
    assert_response_contains(result, "\"operatorServiceLifecycleTemplateFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenServiceOwnerDeclared\":false");
    assert_response_contains(result, "\"operatorServiceLifecycleEvidence\":{\"evidenceMode\":\"declared-lifecycle-no-runtime\"");
    assert_response_contains(result, "\"operatorOwnedServiceLifecycleDeclared\":true");
    assert_response_contains(result, "\"serviceOwnerDeclared\":true");
    assert_response_contains(result, "\"startupCommandDeclared\":true");
    assert_response_contains(result, "\"portListDeclared\":true");
    assert_response_contains(result, "\"getOnlySmokeTargetDeclared\":true");
    assert_response_contains(result, "\"runtimeGateApproved\":false");
    assert_response_contains(result, "\"requiresSeparateRuntimeGate\":true");
    assert_response_contains(result, "\"operatorServiceLifecycleEvidenceFreeze\":{\"frozenReleaseVersion\":\"v153\"");
    assert_response_contains(result, "\"frozenRuntimeGateApproved\":false");
    assert_response_contains(result,
                             "\"runtimeExecutionArtifactIntakePreflight\":{\"preflightMode\":"
                             "\"blocked-missing-runtime-execution-artifacts\"");
    assert_response_contains(result, "\"readyForRuntimeExecutionPacket\":false");
    assert_response_contains(result, "\"runtimeExecutionArtifactsComplete\":false");
    assert_response_contains(result, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_response_contains(result, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"executionAttempted\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"failClosedOnMissingArtifacts\":true");
    assert_response_contains(result,
                             "\"runtimeExecutionArtifactIntakePreflightFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result,
                             "\"miniKvRuntimeExecutionArtifactCandidate\":{\"candidateMode\":"
                             "\"mini-kv-side-candidate-no-runtime\"");
    assert_response_contains(result, "\"candidateArtifactCount\":4");
    assert_response_contains(result, "\"acceptedRuntimeExecutionArtifactCount\":0");
    assert_response_contains(result, "\"operatorApprovalRecordPresent\":false");
    assert_response_contains(result, "\"nodeRuntimeWindowApproved\":false");
    assert_response_contains(result, "\"miniKvLoopbackPortCandidate\":6424");
    assert_response_contains(result, "\"miniKvLoopbackPortOperatorApproved\":false");
    assert_response_contains(result, "\"getOnlySmokeCommandOperatorApproved\":false");
    assert_response_contains(result, "\"serviceOwnerCandidateDeclared\":true");
    assert_response_contains(result, "\"cleanupProofPresent\":false");
    assert_response_contains(result, "\"requiresCrossProjectPacket\":true");
    assert_response_contains(result,
                             "\"miniKvRuntimeExecutionArtifactCandidateFreeze\":{\"frozenReleaseVersion\":\"v154\"");
    assert_response_contains(result, "\"preservesCandidateOnlyContribution\":true");
    assert_response_contains(result, "\"frozenAcceptedRuntimeExecutionArtifactCount\":0");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result,
                             "\"runtimeExecutionApprovalGateInputPrecheck\":{\"precheckMode\":"
                             "\"blocked-missing-approval-gate-inputs\"");
    assert_response_contains(result, "\"approvalGateInputCount\":0");
    assert_response_contains(result, "\"missingApprovalGateInputCount\":3");
    assert_response_contains(result, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_response_contains(result, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_response_contains(result, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_response_contains(result, "\"approvalGateInputsComplete\":false");
    assert_response_contains(result, "\"runtimeGateApprovalPresent\":false");
    assert_response_contains(result, "\"concreteLoopbackPortsAssigned\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"failClosedOnMissingApprovalInputs\":true");
    assert_response_contains(result,
                             "\"runtimeExecutionApprovalGateInputPrecheckFreeze\":{\"frozenReleaseVersion\":\"v155\"");
    assert_response_contains(result, "\"preservesApprovalGateInputPrecheck\":true");
    assert_response_contains(result, "\"frozenApprovalGateInputCount\":0");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result,
                             "\"miniKvFinalApprovalGateInput\":{\"inputMode\":\"mini-kv-final-approval-gate-input-no-runtime\"");
    assert_response_contains(result, "\"finalMiniKvApprovalGateInputPresent\":true");
    assert_response_contains(result, "\"miniKvApprovalGateInputComplete\":true");
    assert_response_contains(result, "\"miniKvApprovalGateInputItemCount\":4");
    assert_response_contains(result, "\"miniKvLoopbackPort\":6424");
    assert_response_contains(result, "\"serviceOwnerConfirmed\":true");
    assert_response_contains(result, "\"processCleanupRulesComplete\":true");
    assert_response_contains(result, "\"stopOnlyOwnedProcessCleanupRequired\":true");
    assert_response_contains(result, "\"cleanupProofPresent\":false");
    assert_response_contains(result, "\"cleanupProofRequiresApprovedRuntimeStart\":true");
    assert_response_contains(result, "\"nodeApprovedRuntimeWindowPresent\":false");
    assert_response_contains(result, "\"correlatedOperatorApprovalRecordPresent\":false");
    assert_response_contains(result, "\"completeCrossProjectRuntimeExecutionPacketPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"requiresNodeRuntimeWindow\":true");
    assert_response_contains(result, "\"requiresCorrelatedOperatorApproval\":true");
    assert_response_contains(result, "\"requiresCompleteCrossProjectPacket\":true");
    assert_response_contains(result, "\"miniKvFinalApprovalGateInputFreeze\":{\"frozenReleaseVersion\":\"v156\"");
    assert_response_contains(result, "\"preservesMiniKvFinalApprovalGateInput\":true");
    assert_response_contains(result, "\"frozenRuntimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"frozenRouterActivationAllowed\":false");
    assert_response_contains(result,
                             "\"runtimeExecutionApprovalInputTemplateValidatorEcho\":{\"echoMode\":"
                             "\"template-validator-echo-no-canonical-inputs\"");
    assert_response_contains(result,
                             "\"sourceNodeValidatorVersion\":\"Node v402 runtime execution approval input template validator\"");
    assert_response_contains(result, "\"templateArchiveRoot\":\"e/402/input-templates\"");
    assert_response_contains(result, "\"canonicalInputRoot\":\"e/398/input\"");
    assert_response_contains(result, "\"templateOnlyInputCount\":3");
    assert_response_contains(result, "\"canonicalRuntimeInputPresent\":false");
    assert_response_contains(result, "\"templateCopiedToCanonicalInput\":false");
    assert_response_contains(result, "\"nodeApprovedRuntimeWindowCanonicalPresent\":false");
    assert_response_contains(result, "\"correlatedOperatorApprovalRecordCanonicalPresent\":false");
    assert_response_contains(result, "\"completeCrossProjectRuntimeExecutionPacketCanonicalPresent\":false");
    assert_response_contains(result, "\"templatesAuthorizeRuntime\":false");
    assert_response_contains(result, "\"writesCanonicalApprovalInputFiles\":false");
    assert_response_contains(result, "\"changesNodeInputTemplateFiles\":false");
    assert_response_contains(result, "\"failClosedOnTemplateOnlyInputs\":true");
    assert_response_contains(result,
                             "\"runtimeExecutionApprovalInputTemplateValidatorEchoFreeze\":{\"frozenReleaseVersion\":\"v157\"");
    assert_response_contains(result, "\"preservesTemplateValidatorEcho\":true");
    assert_response_contains(result, "\"frozenCanonicalRuntimeInputPresent\":false");
    assert_response_contains(result, "\"frozenTemplateCopiedToCanonicalInput\":false");
    assert_response_contains(result, "\"frozenTemplatesAuthorizeRuntime\":false");
    assert_response_contains(result, "\"frozenExecutionAllowed\":false");
    assert_response_contains(result,
                             "\"runtimeExecutionCanonicalApprovalInputPrecheck\":{\"precheckMode\":"
                             "\"blocked-missing-canonical-approval-inputs\"");
    assert_response_contains(result, "\"sourceNodeCompatibilityVersion\":\"Node v403 template compatibility intake\"");
    assert_response_contains(result, "\"requiredCanonicalInputCount\":3");
    assert_response_contains(result, "\"presentCanonicalInputCount\":0");
    assert_response_contains(result, "\"missingCanonicalInputCount\":3");
    assert_response_contains(result, "\"canonicalApprovalInputsComplete\":false");
    assert_response_contains(result, "\"sharedApprovalCorrelationIdValidated\":false");
    assert_response_contains(result, "\"templatesAcceptedAsCanonicalInputs\":false");
    assert_response_contains(result, "\"templateCompatibilityEvidenceAcceptedAsApproval\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"requiresRealCanonicalInputs\":true");
    assert_response_contains(result, "\"failClosedOnMissingCanonicalInputs\":true");
    assert_response_contains(result, "\"nodeRouteGroupSplitCompatibility\":{\"compatibilityMode\":"
                             "\"node-route-group-split-contract-stable\"");
    assert_response_contains(result,
                             "\"sourceNodeVersion\":\"Node v418 sandbox endpoint credential resolver route group split\"");
    assert_response_contains(result, "\"nodeChangeType\":\"route-registration-refactor-only\"");
    assert_response_contains(result, "\"nodeApiPathChanged\":false");
    assert_response_contains(result, "\"nodeResponseShapeChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"nodeStartsMiniKvService\":false");
    assert_response_contains(result, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_response_contains(result, "\"miniKvFixturePathChanged\":false");
    assert_response_contains(result, "\"miniKvShardJsonCommandChanged\":false");
    assert_response_contains(result, "\"miniKvRequiresNodeRouteTableChange\":false");
    assert_response_contains(result, "\"archiveCompatibilityPreserved\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow\":{\"windowMode\":"
                                     "\"node-v433-v472-route-catalog-cleanup-closeout-window-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v472-post-route-catalog-cleanup-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_response_contains(result, "\"windowStartNodeVersion\":\"Node v433\"");
    assert_response_contains(result, "\"windowEndNodeVersion\":\"Node v472\"");
    assert_response_contains(result, "\"splitRouteGroupCount\":40");
    assert_response_contains(result, "\"coveredNodeVersions\":[\"Node v433\",\"Node v434\",\"Node v435\","
                                     "\"Node v436\",\"Node v437\",\"Node v438\",\"Node v439\",\"Node v440\","
                                     "\"Node v441\",\"Node v442\",\"Node v443\",\"Node v444\",\"Node v445\","
                                     "\"Node v446\",\"Node v447\",\"Node v448\",\"Node v449\",\"Node v450\","
                                     "\"Node v451\",\"Node v452\",\"Node v453\",\"Node v454\",\"Node v455\","
                                     "\"Node v456\",\"Node v457\",\"Node v458\",\"Node v459\",\"Node v460\","
                                     "\"Node v461\",\"Node v462\",\"Node v463\",\"Node v464\","
                                     "\"Node v465\",\"Node v466\",\"Node v467\","
                                     "\"Node v468\",\"Node v469\",\"Node v470\",\"Node v471\",\"Node v472\"]");
    assert_response_contains(result, "\"allChangesRouteRegistrationOnly\":true");
    assert_response_contains(result, "\"allChangesRouteCatalogOnly\":true");
    assert_response_contains(result, "\"nodeApiPathChanged\":false");
    assert_response_contains(result, "\"nodeResponseShapeChanged\":false");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"nodeStartsJavaService\":false");
    assert_response_contains(result, "\"nodeStartsMiniKvService\":false");
    assert_response_contains(result, "\"miniKvContractChangedForNodeRouteSplit\":false");
    assert_response_contains(result, "\"miniKvFixturePathChanged\":false");
    assert_response_contains(result, "\"miniKvShardJsonCommandChanged\":false");
    assert_response_contains(result, "\"miniKvRequiresNodeRouteTableChange\":false");
    assert_response_contains(result, "\"runtimeGateApprovalPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"activeShardPrototypeEnabled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit\":{\"auditMode\":"
                                     "\"node-route-split-compatibility-window-consistency-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_response_contains(result, "\"auditedWindowSection\":\"nodeRouteSplitCompatibilityWindow\"");
    assert_response_contains(result, "\"expectedWindowVersionCount\":40");
    assert_response_contains(result, "\"observedWindowVersionCount\":40");
    assert_response_contains(result, "\"windowRangeStart\":\"Node v433\"");
    assert_response_contains(result, "\"windowRangeEnd\":\"Node v472\"");
    assert_response_contains(result, "\"windowRangeStartNumber\":433");
    assert_response_contains(result, "\"windowRangeEndNumber\":472");
    assert_response_contains(result, "\"computedWindowVersionSpan\":40");
    assert_response_contains(result, "\"windowRangeNumbersParseable\":true");
    assert_response_contains(result, "\"windowCountMatchesRange\":true");
    assert_response_contains(result, "\"contiguousNodeVersionWindow\":true");
    assert_response_contains(result, "\"duplicateWindowVersionsDetected\":false");
    assert_response_contains(result, "\"allWindowVersionsRouteRegistrationOnly\":true");
    assert_response_contains(result, "\"sourceFrozenWindowDigest\":\"fnv1a64:d1e889711b5d8574\"");
    assert_response_contains(result, "\"latestWindowMatchesFrozenSource\":true");
    assert_response_contains(result, "\"windowAuditOnly\":true");
    assert_response_contains(result, "\"runtimeGateApprovalPresent\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"activeShardPrototypeEnabled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"writeCommandsAllowed\":false");
    assert_response_contains(result, "\"adminCommandsAllowed\":false");
    assert_response_contains(result, "\"loadRestoreCompactAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/v471-post-route-catalog-expected-integrity-snapshot-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v186\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v186.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:a5d69dff949088ee\"");
    assert_response_contains(result, "\"auditedNodeVersion\":\"Node v471\"");
    assert_response_contains(result, "\"anchorFieldsRemovedByNodeV468\":true");
    assert_response_contains(result, "\"summaryBuilderIntroducedByNodeV469\":true");
    assert_response_contains(result, "\"expectedSummaryCentralizedByNodeV470\":true");
    assert_response_contains(result, "\"expectedIntegritySnapshotFactoryMovedByNodeV471\":true");
    assert_response_contains(result, "\"routeQualityDefaultSnapshotReusesExpectedSummary\":true");
    assert_response_contains(result, "\"routeQualityServiceConsumesCatalogIntegrityFactory\":true");
    assert_response_contains(result, "\"serviceLocalIntegrityFallbackRemoved\":true");
    assert_response_contains(result, "\"catalogEvaluatorParityTested\":true");
    assert_response_contains(result, "\"miniKvComputesNodeRouteSummary\":false");
    assert_response_contains(result, "\"miniKvOwnsNodeRouteQualitySnapshot\":false");
    assert_response_contains(result, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_response_contains(result, "\"focusedExpectedSummaryTestFileCount\":3");
    assert_response_contains(result, "\"focusedExpectedSummaryTestCount\":7");
    assert_response_contains(result, "\"focusedExpectedIntegritySnapshotTestFileCount\":3");
    assert_response_contains(result, "\"focusedExpectedIntegritySnapshotTestCount\":7");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\":{\"freezeMode\":"
                                     "\"node-route-catalog-expected-integrity-snapshot-freeze-read-only\"");
    assert_response_contains(result, "\"frozenReleaseVersion\":\"v187\"");
    assert_response_contains(result, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v187.json\"");
    assert_response_contains(result, "\"frozenEvidenceDigest\":\"fnv1a64:f4459b7beacceedd\"");
    assert_response_contains(result, "\"frozenStatus\":\"node-route-catalog-expected-integrity-snapshot-audit-read-only\"");
    assert_response_contains(result, "\"frozenWindowEndNodeVersion\":\"Node v471\"");
    assert_response_contains(result, "\"frozenSplitRouteGroupCount\":39");
    assert_response_contains(result, "\"frozenBoundaryCatalogVersion\":\"read-only-boundary-fields.v15\"");
    assert_response_contains(result, "\"frozenBoundaryFieldCount\":502");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_response_contains(result, "\"preservesExpectedIntegritySnapshotAudit\":true");
    assert_response_contains(result, "\"preservesNodeV471CatalogOwnershipCleanup\":true");
    assert_response_contains(result, "\"preservesRouteCatalogWindow\":true");
    assert_response_contains(result, "\"preservesBoundaryCatalog\":true");
    assert_response_contains(result, "\"miniKvOwnsNodeCatalogIntegritySnapshot\":false");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChain\":{\"catalogMode\":"
                                     "\"node-route-catalog-evidence-chain-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v188\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v188.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_response_contains(result, "\"catalogedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeStart\":\"v185\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v188\"");
    assert_response_contains(result, "\"latestCatalogedReleaseVersion\":\"v188\"");
    assert_response_contains(result, "\"latestCatalogedDigest\":\"fnv1a64:565e5ce1fdb5f308\"");
    assert_response_contains(result, "\"catalogedSections\":[\"nodeRouteCatalogCloseoutSnapshot\","
                                     "\"nodeRouteCatalogAnchorRemovalAudit\","
                                     "\"nodeRouteCatalogExpectedIntegritySnapshotAudit\","
                                     "\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\"]");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogExpectedIntegritySnapshotFreeze\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v16\"");
    assert_response_contains(result, "\"fieldCount\":525");
    assert_response_contains(result, "\"groupCount\":27");
    assert_response_contains(result, "\"versionedFixtureCatalogOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChainAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v189\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v189.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:a595f2d9cdcbac8d\"");
    assert_response_contains(result, "\"auditedChainSection\":\"nodeRouteCatalogEvidenceChain\"");
    assert_response_contains(result, "\"nodeCleanupBatchStart\":\"Node v465\"");
    assert_response_contains(result, "\"nodeCleanupBatchEnd\":\"Node v472\"");
    assert_response_contains(result, "\"nodeCleanupCloseoutVersion\":\"Node v472\"");
    assert_response_contains(result, "\"expectedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"observedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeContiguous\":true");
    assert_response_contains(result, "\"duplicateReleasesDetected\":false");
    assert_response_contains(result, "\"latestDigestMatchesFrozenSource\":true");
    assert_response_contains(result, "\"nodeCleanupCloseoutValidated\":true");
    assert_response_contains(result, "\"nodeFullVitestShardsPassed\":true");
    assert_response_contains(result, "\"nodeFullVitestFileCount\":394");
    assert_response_contains(result, "\"nodeFullVitestTestCount\":1222");
    assert_response_contains(result, "\"nodeTypecheckPassed\":true");
    assert_response_contains(result, "\"nodeBuildPassed\":true");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff\":{\"handoffMode\":"
                                     "\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v190\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v190.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_response_contains(result, "\"upstreamNodeCloseoutVersion\":\"Node v472\"");
    assert_response_contains(result, "\"latestAuditedMiniKvRelease\":\"v190\"");
    assert_response_contains(result, "\"handoffBoundaryCatalogVersion\":\"read-only-boundary-fields.v18\"");
    assert_response_contains(result, "\"handoffBoundaryFieldCount\":573");
    assert_response_contains(result, "\"handoffBoundaryGroupCount\":29");
    assert_response_contains(result, "\"includedSections\":[\"nodeRouteCatalogEvidenceChain\","
                                     "\"nodeRouteCatalogEvidenceChainAudit\","
                                     "\"nodeRouteSplitCompatibilityWindow\","
                                     "\"boundaryCatalogIndex\"]");
    assert_response_contains(result, "\"readyForDownstreamConsumption\":true");
    assert_response_contains(result, "\"requiresVersionedFixture\":true");
    assert_response_contains(result, "\"rollingCurrentAllowedAsHistoricalBaseline\":false");
    assert_response_contains(result, "\"readOnlyFixtureRequired\":true");
    assert_response_contains(result, "\"preservesRouteCatalogEvidenceChainAudit\":true");
    assert_response_contains(result, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_response_contains(result, "\"handoffOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v191\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v191.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_response_contains(result, "\"auditedHandoffSection\":\"nodeRouteCatalogCleanupCloseoutHandoff\"");
    assert_response_contains(result, "\"auditedHandoffReleaseVersion\":\"v191\"");
    assert_response_contains(result, "\"handoffSourceReleaseVersion\":\"v190\"");
    assert_response_contains(result, "\"handoffSourceDigest\":\"fnv1a64:b24193bae62876e6\"");
    assert_response_contains(result, "\"expectedIncludedSectionCount\":4");
    assert_response_contains(result, "\"observedIncludedSectionCount\":4");
    assert_response_contains(result, "\"handoffIncludedSectionsComplete\":true");
    assert_response_contains(result, "\"handoffReadyForDownstreamConsumption\":true");
    assert_response_contains(result, "\"versionedFixtureRequired\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"consumerGuidanceCountMatches\":true");
    assert_response_contains(result, "\"preservesRouteCatalogEvidenceChainAudit\":true");
    assert_response_contains(result, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\":{\"freezeMode\":"
                                     "\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_response_contains(result, "\"frozenReleaseVersion\":\"v192\"");
    assert_response_contains(result, "\"frozenFixturePath\":\"fixtures/release/shard-readiness-v192.json\"");
    assert_response_contains(result, "\"frozenEvidenceDigest\":\"fnv1a64:89bf4177b09f1ec0\"");
    assert_response_contains(result, "\"frozenStatus\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_response_contains(result, "\"frozenHandoffSourceReleaseVersion\":\"v191\"");
    assert_response_contains(result, "\"frozenHandoffSourceDigest\":\"fnv1a64:e6a06c9207144d9d\"");
    assert_response_contains(result, "\"frozenBoundaryCatalogVersion\":\"read-only-boundary-fields.v20\"");
    assert_response_contains(result, "\"frozenBoundaryFieldCount\":625");
    assert_response_contains(result, "\"frozenBoundaryGroupCount\":31");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenBaseline\":false");
    assert_response_contains(result, "\"preservesHandoffAudit\":true");
    assert_response_contains(result, "\"preservesHandoffManifest\":true");
    assert_response_contains(result, "\"preservesNodeCleanupCloseoutValidation\":true");
    assert_response_contains(result, "\"frozenReadyForDownstreamConsumption\":true");
    assert_response_contains(result, "\"frozenRollingCurrentRejected\":true");
    assert_response_contains(result, "\"freezeOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\":{\"catalogMode\":"
                                     "\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v193\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v193.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_response_contains(result, "\"catalogedReleaseCount\":4");
    assert_response_contains(result, "\"releaseRangeStart\":\"v190\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v193\"");
    assert_response_contains(result, "\"latestCatalogedReleaseVersion\":\"v193\"");
    assert_response_contains(result, "\"latestCatalogedDigest\":\"fnv1a64:0aad0fd5d2732af5\"");
    assert_response_contains(result, "\"catalogedSections\":[\"nodeRouteCatalogEvidenceChainAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutHandoff\","
                                     "\"nodeRouteCatalogCleanupCloseoutHandoffAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\"]");
    assert_response_contains(result, "\"releaseVersion\":\"v190\",\"status\":\"node-route-catalog-evidence-chain-audit-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v191\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v192\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v193\",\"status\":\"node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only\"");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v21\"");
    assert_response_contains(result, "\"fieldCount\":648");
    assert_response_contains(result, "\"groupCount\":32");
    assert_response_contains(result, "\"versionedFixtureCatalogOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCatalog\":false");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-closeout-release-catalog-consistency-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v474-post-java-mini-kv-route-catalog-cleanup-handoff-evidence-report-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v194\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v194.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:626f5a2a96a980c2\"");
    assert_response_contains(result, "\"auditedCatalogSection\":\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\"");
    assert_response_contains(result, "\"auditedCatalogReleaseVersion\":\"v194\"");
    assert_response_contains(result, "\"catalogSourceReleaseVersion\":\"v193\"");
    assert_response_contains(result, "\"expectedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"observedCatalogedReleaseCount\":4");
    assert_response_contains(result, "\"expectedReleaseRangeSpan\":4");
    assert_response_contains(result, "\"observedReleaseRangeSpan\":4");
    assert_response_contains(result, "\"catalogEntryVersions\":[\"v190\",\"v191\",\"v192\",\"v193\"]");
    assert_response_contains(result, "\"catalogIncludesEvidenceChainAudit\":true");
    assert_response_contains(result, "\"catalogIncludesHandoff\":true");
    assert_response_contains(result, "\"catalogIncludesHandoffAudit\":true");
    assert_response_contains(result, "\"catalogIncludesHandoffAuditFreeze\":true");
    assert_response_contains(result, "\"releaseVersionsContiguous\":true");
    assert_response_contains(result, "\"duplicateReleaseVersionsDetected\":false");
    assert_response_contains(result, "\"allCatalogedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCatalogedFixturesVersioned\":true");
    assert_response_contains(result, "\"allCatalogedDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestDigestMatchesFrozenSource\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"catalogOnlyAudit\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\":{\"maintenanceMode\":"
                                     "\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v477-post-java-mini-kv-latest-route-catalog-cleanup-evidence-report-archive-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v195\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v195.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:36463a895469c4bc\"");
    assert_response_contains(result, "\"splitFrom\":\"src/shard_readiness_route_catalog_evidence_chain.cpp\"");
    assert_response_contains(result, "\"splitInto\":\"src/shard_readiness_route_catalog_cleanup_catalog.cpp\"");
    assert_response_contains(result, "\"extractedCatalogFormatter\":true");
    assert_response_contains(result, "\"extractedCatalogAuditFormatter\":true");
    assert_response_contains(result, "\"preservesReleaseCatalog\":true");
    assert_response_contains(result, "\"preservesReleaseCatalogAudit\":true");
    assert_response_contains(result, "\"addsMaintenanceSection\":true");
    assert_response_contains(result, "\"publicCatalogSectionsChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage\":{\"packageMode\":"
                                     "\"node-route-catalog-cleanup-latest-evidence-package-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v196\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v196.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_response_contains(result, "\"packageReleaseCount\":3");
    assert_response_contains(result, "\"releaseRangeStart\":\"v194\"");
    assert_response_contains(result, "\"releaseRangeEnd\":\"v196\"");
    assert_response_contains(result, "\"latestPackagedReleaseVersion\":\"v196\"");
    assert_response_contains(result, "\"latestPackagedDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_response_contains(result, "\"packagedSections\":[\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\","
                                     "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\"]");
    assert_response_contains(result, "\"releaseVersion\":\"v194\",\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v195\",\"status\":\"node-route-catalog-cleanup-closeout-release-catalog-audit-read-only\"");
    assert_response_contains(result, "\"releaseVersion\":\"v196\",\"status\":\"node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only\"");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v24\"");
    assert_response_contains(result, "\"fieldCount\":714");
    assert_response_contains(result, "\"groupCount\":35");
    assert_response_contains(result, "\"versionedFixturePackageOnly\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenPackage\":false");
    assert_response_contains(result, "\"allPackagedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allPackagedFixturesVersioned\":true");
    assert_response_contains(result, "\"allPackagedDigestsPresent\":true");
    assert_response_contains(result, "\"packageReadyForArchiveVerification\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-latest-evidence-package-consistency-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v197\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v197.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:836d9f0b3956fe30\"");
    assert_response_contains(result, "\"auditedPackageSection\":\"nodeRouteCatalogCleanupLatestEvidencePackage\"");
    assert_response_contains(result, "\"auditedPackageReleaseVersion\":\"v197\"");
    assert_response_contains(result, "\"packageSourceReleaseVersion\":\"v196\"");
    assert_response_contains(result, "\"packageSourceDigest\":\"fnv1a64:116ec6b912bb4026\"");
    assert_response_contains(result, "\"expectedPackageReleaseCount\":3");
    assert_response_contains(result, "\"observedPackageReleaseCount\":3");
    assert_response_contains(result, "\"packageRangeStart\":\"v194\"");
    assert_response_contains(result, "\"packageRangeEnd\":\"v196\"");
    assert_response_contains(result, "\"expectedPackageRangeSpan\":3");
    assert_response_contains(result, "\"observedPackageRangeSpan\":3");
    assert_response_contains(result, "\"packageEntryVersions\":[\"v194\",\"v195\",\"v196\"]");
    assert_response_contains(result, "\"packageIncludesReleaseCatalog\":true");
    assert_response_contains(result, "\"packageIncludesReleaseCatalogAudit\":true");
    assert_response_contains(result, "\"packageIncludesCatalogMaintenance\":true");
    assert_response_contains(result, "\"releaseVersionsContiguous\":true");
    assert_response_contains(result, "\"duplicatePackageVersionsDetected\":false");
    assert_response_contains(result, "\"allPackagedEntriesReadOnly\":true");
    assert_response_contains(result, "\"allPackagedFixturesVersioned\":true");
    assert_response_contains(result, "\"allPackagedDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestPackagedDigestMatchesPackageSource\":true");
    assert_response_contains(result, "\"packageReadyForArchiveVerification\":true");
    assert_response_contains(result, "\"auditUsesVersionedFixture\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"packageAuditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout\":{\"closeoutMode\":"
                                     "\"node-route-catalog-cleanup-evidence-batch-closeout-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v198\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_response_contains(result, "\"nodeBatchCloseoutVersion\":\"Node v480\"");
    assert_response_contains(result, "\"nodeBatchStartVersion\":\"Node v473\"");
    assert_response_contains(result, "\"nodeBatchEndVersion\":\"Node v479\"");
    assert_response_contains(result, "\"miniKvCloseoutReleaseCount\":5");
    assert_response_contains(result, "\"miniKvCloseoutRangeStart\":\"v194\"");
    assert_response_contains(result, "\"miniKvCloseoutRangeEnd\":\"v198\"");
    assert_response_contains(result, "\"latestCloseoutReleaseVersion\":\"v198\"");
    assert_response_contains(result, "\"latestCloseoutDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_response_contains(result, "\"closeoutSections\":[\"nodeRouteCatalogCleanupCloseoutReleaseCatalog\","
                                     "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit\","
                                     "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance\","
                                     "\"nodeRouteCatalogCleanupLatestEvidencePackage\","
                                     "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\"]");
    assert_response_contains(result, "\"releaseVersion\":\"v198\",\"fixturePath\":\"fixtures/release/shard-readiness-v198.json\"");
    assert_response_contains(result, "\"primarySection\":\"nodeRouteCatalogCleanupLatestEvidencePackageAudit\"");
    assert_response_contains(result, "\"boundaryCatalogVersion\":\"read-only-boundary-fields.v26\"");
    assert_response_contains(result, "\"fieldCount\":756");
    assert_response_contains(result, "\"groupCount\":37");
    assert_response_contains(result, "\"nodeBatchValidationRecorded\":true");
    assert_response_contains(result, "\"nodeFocusedCleanupTestsPassed\":true");
    assert_response_contains(result, "\"nodeFullVitestFileCount\":398");
    assert_response_contains(result, "\"nodeFullVitestTestCount\":1230");
    assert_response_contains(result, "\"nodeHttpStyleSmokePassed\":true");
    assert_response_contains(result, "\"allCloseoutEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCloseoutFixturesVersioned\":true");
    assert_response_contains(result, "\"allCloseoutDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"futureNodeBatchMayConsumeCleanTags\":true");
    assert_response_contains(result, "\"closeoutReadyForNextBatch\":true");
    assert_response_contains(result, "\"rollingCurrentUsedForFrozenCloseout\":false");
    assert_response_contains(result, "\"closeoutOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-cleanup-evidence-batch-closeout-consistency-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/"
                                     "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v199\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v199.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:3a5716f6f09c2b3b\"");
    assert_response_contains(result, "\"auditedCloseoutSection\":\"nodeRouteCatalogCleanupEvidenceBatchCloseout\"");
    assert_response_contains(result, "\"auditedCloseoutReleaseVersion\":\"v199\"");
    assert_response_contains(result, "\"closeoutSourceReleaseVersion\":\"v198\"");
    assert_response_contains(result, "\"closeoutSourceDigest\":\"fnv1a64:8fcbe53d8b1700b7\"");
    assert_response_contains(result, "\"expectedCloseoutReleaseCount\":5");
    assert_response_contains(result, "\"observedCloseoutReleaseCount\":5");
    assert_response_contains(result, "\"closeoutRangeStart\":\"v194\"");
    assert_response_contains(result, "\"closeoutRangeEnd\":\"v198\"");
    assert_response_contains(result, "\"expectedCloseoutRangeSpan\":5");
    assert_response_contains(result, "\"observedCloseoutRangeSpan\":5");
    assert_response_contains(result, "\"closeoutEntryVersions\":[\"v194\",\"v195\",\"v196\",\"v197\",\"v198\"]");
    assert_response_contains(result, "\"closeoutIncludesReleaseCatalog\":true");
    assert_response_contains(result, "\"closeoutIncludesReleaseCatalogAudit\":true");
    assert_response_contains(result, "\"closeoutIncludesCatalogMaintenance\":true");
    assert_response_contains(result, "\"closeoutIncludesLatestEvidencePackage\":true");
    assert_response_contains(result, "\"closeoutIncludesLatestEvidencePackageAudit\":true");
    assert_response_contains(result, "\"releaseVersionsContiguous\":true");
    assert_response_contains(result, "\"duplicateCloseoutVersionsDetected\":false");
    assert_response_contains(result, "\"allCloseoutEntriesReadOnly\":true");
    assert_response_contains(result, "\"allCloseoutFixturesVersioned\":true");
    assert_response_contains(result, "\"allCloseoutDigestsPresent\":true");
    assert_response_contains(result, "\"fieldCountsMonotonic\":true");
    assert_response_contains(result, "\"groupCountsMonotonic\":true");
    assert_response_contains(result, "\"latestCloseoutDigestMatchesCloseoutSource\":true");
    assert_response_contains(result, "\"nodeBatchValidationRecorded\":true");
    assert_response_contains(result, "\"nodeFullVitestCountMatchesPlan\":true");
    assert_response_contains(result, "\"nodeArchiveVerifierCheckCount\":16");
    assert_response_contains(result, "\"nodeArchiveVerifierChecksPassed\":16");
    assert_response_contains(result, "\"closeoutReadyForNextBatch\":true");
    assert_response_contains(result, "\"auditUsesVersionedFixture\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"closeoutAuditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity\":{\"continuityMode\":"
                                     "\"node-route-catalog-cleanup-post-closeout-continuity-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v346\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v346.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:5a2f46c60bb2a2e6\"");
    assert_response_contains(
        result,
        "\"continuityStage\":\"feature-read-only-shard-route-preview-verification-report-archive-continuity-map\"");
    assert_response_contains(result, "\"stageSequence\":147");
    assert_response_contains(result, "\"stageReleaseVersion\":\"v347\"");
    assert_response_contains(result, "\"nodeBatchCloseoutVersion\":\"Node v549\"");
    assert_response_contains(result, "\"trackedMiniKvCloseoutRangeEnd\":\"v200\"");
    assert_response_contains(result, "\"trackedPostCloseoutRangeStart\":\"v201\"");
    assert_response_contains(result, "\"trackedPostCloseoutRangeEnd\":\"v347\"");
    assert_response_contains(result, "\"trackedPostCloseoutReleaseCount\":147");
    assert_response_contains(result, "\"nodePlanStillLatestForMiniKv\":true");
    assert_response_contains(result, "\"sourceFixtureVersioned\":true");
    assert_response_contains(result, "\"rollingCurrentRejected\":true");
    assert_response_contains(result, "\"preservesBatchCloseoutAudit\":true");
    assert_response_contains(result, "\"preservesNodeV480Validation\":true");
    assert_response_contains(result, "\"archiveVerifierChecksPassed\":3");
    assert_response_contains(result, "\"postCloseoutStageOnly\":true");
    assert_response_contains(result, "\"readyForNextNodeBatch\":true");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"nodeRouteCatalogAnchorRemovalAudit\":{\"auditMode\":"
                                     "\"node-route-catalog-anchor-removal-audit-read-only\"");
    assert_response_contains(result, "\"sourceNodePlan\":\"docs/plans3/v467-post-route-catalog-anchor-removal-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v185\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v185.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:6a6b4cbdac4bdb46\"");
    assert_response_contains(result, "\"auditedNodeVersion\":\"Node v467\"");
    assert_response_contains(result, "\"sourceAnchorCompatibilityRemoved\":true");
    assert_response_contains(result, "\"centralRouteTablePureFlatMapConsumer\":true");
    assert_response_contains(result, "\"routeGroupTestsUseTypedCatalogIdentity\":true");
    assert_response_contains(result, "\"affectedRouteCatalogTestFileCount\":53");
    assert_response_contains(result, "\"affectedRouteCatalogTestCount\":59");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRouteAnchorDependencyIntroduced\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"auditOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCloseoutSnapshot\":{\"snapshotMode\":"
                                     "\"node-route-catalog-final-closeout-snapshot-read-only\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v184\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v184.json\"");
    assert_response_contains(result, "\"sourceFrozenDigest\":\"fnv1a64:5b7cd9ee9a9f2524\"");
    assert_response_contains(result, "\"nodeFinalCloseoutVersion\":\"Node v464\"");
    assert_response_contains(result, "\"routeCatalogBatchStart\":\"Node v433\"");
    assert_response_contains(result, "\"routeCatalogBatchEnd\":\"Node v464\"");
    assert_response_contains(result, "\"coveredNodeVersionCount\":32");
    assert_response_contains(result, "\"focusedCloseoutTestFileCount\":7");
    assert_response_contains(result, "\"focusedCloseoutTestCount\":17");
    assert_response_contains(result, "\"nodeTypecheckPassed\":true");
    assert_response_contains(result, "\"nodeBuildPassed\":true");
    assert_response_contains(result, "\"nodeFullVitestFileCount\":393");
    assert_response_contains(result, "\"nodeFullVitestTestCount\":1221");
    assert_response_contains(result, "\"nodeConsumesFreshMiniKvEvidence\":false");
    assert_response_contains(result, "\"miniKvRuntimeContractChanged\":false");
    assert_response_contains(result, "\"snapshotOnly\":true");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"readOnly\":true");
    assert_response_contains(result, "\"boundaryCatalogMaintenance\":{\"maintenanceMode\":"
                                     "\"boundary-field-catalog-split-contract-preserving\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v159\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v159.json\"");
    assert_response_contains(result, "\"boundaryFieldCatalogExtracted\":true");
    assert_response_contains(result, "\"readOnlyBoundaryFieldsStillPublished\":true");
    assert_response_contains(result, "\"publicShardJsonContractChanged\":false");
    assert_response_contains(result, "\"fixturePathChanged\":false");
    assert_response_contains(result, "\"shardJsonCommandChanged\":false");
    assert_response_contains(result, "\"nodeRouteGroupSplitCompatibilityPreserved\":true");
    assert_response_contains(result, "\"nodeAddsEvidenceGate\":false");
    assert_response_contains(result, "\"runtimeExecutionPacketExecutable\":false");
    assert_response_contains(result, "\"startsJavaService\":false");
    assert_response_contains(result, "\"startsMiniKvService\":false");
    assert_response_contains(result, "\"startsServices\":false");
    assert_response_contains(result, "\"runtimeProbeAllowed\":false");
    assert_response_contains(result, "\"liveReadAllowed\":false");
    assert_response_contains(result, "\"activeShardPrototypeEnabled\":false");
    assert_response_contains(result, "\"routerActivationAllowed\":false");
    assert_response_contains(result, "\"writeRoutingAllowed\":false");
    assert_response_contains(result, "\"writeCommandsAllowed\":false");
    assert_response_contains(result, "\"adminCommandsAllowed\":false");
    assert_response_contains(result, "\"loadRestoreCompactAllowed\":false");
    assert_response_contains(result, "\"executionAllowed\":false");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.routerActivationAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex\":{\"catalogVersion\":\"read-only-boundary-fields.v29\"");
    assert_response_contains(result, "\"sourceNodePlan\":"
                                     "\"docs/plans3/"
                                     "v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md\"");
    assert_response_contains(result, "\"sourceFrozenReleaseVersion\":\"v200\"");
    assert_response_contains(result, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v200.json\"");
    assert_response_contains(result, "\"fieldCount\":821");
    assert_response_contains(result, "\"groupCount\":40");
    assert_response_contains(result, "\"fieldListGeneratedFromSharedVector\":true");
    assert_response_contains(result, "\"catalogGroups\":[\"top-level-read-only\",\"slot-table-preview\","
                                     "\"slot-table-preview-audit\",\"slot-table-preview-audit-maintenance\","
                                     "\"shard-readiness-release-catalog\",\"shard-readiness-release-catalog-audit\","
                                     "\"core-boundaries\"");
    assert_response_contains(result, "\"slotTablePreviewCataloged\":true");
    assert_response_contains(result, "\"slotTablePreviewAuditCataloged\":true");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenanceCataloged\":true");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogCataloged\":true");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowCataloged\":true");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCloseoutSnapshotCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogAnchorRemovalAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogExpectedIntegritySnapshotFreezeCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChainCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogEvidenceChainAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreezeCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenanceCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAuditCataloged\":true");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuityCataloged\":true");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenanceCataloged\":true");
    assert_response_contains(result, "\"boundaryCatalogMaintenancePreserved\":true");
    assert_response_contains(result, "\"slotTablePreview.routerActivationAllowed\"");
    assert_response_contains(result, "\"slotTablePreview.writeRoutingAllowed\"");
    assert_response_contains(result, "\"slotTablePreview.executionAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.contiguousSlotRange\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAudit.executionAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.routerActivationAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"slotTablePreviewAuditMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.versionedFixtureCatalogOnly\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.rollingCurrentUsedForFrozenCatalog\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.routerActivationAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.writeRoutingAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalog.executionAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.contiguousReleaseRange\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.duplicateReleasesDetected\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.latestDigestMatchesFrozenSource\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"shardReadinessReleaseCatalogAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.nodeAddsEvidenceGate\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindow.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.windowRangeNumbersParseable\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.windowCountMatchesRange\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.contiguousNodeVersionWindow\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.duplicateWindowVersionsDetected\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.latestWindowMatchesFrozenSource\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteSplitCompatibilityWindowAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.readyForDownstreamConsumption\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.requiresVersionedFixture\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.rollingCurrentAllowedAsHistoricalBaseline\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.preservesNodeCleanupCloseoutValidation\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoff.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.handoffIncludedSectionsComplete\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.handoffReadyForDownstreamConsumption\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.rollingCurrentRejected\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.consumerGuidanceCountMatches\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.rollingCurrentUsedForFrozenBaseline\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.preservesHandoffAudit\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.frozenRollingCurrentRejected\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.catalogedReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.versionedFixtureCatalogOnly\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.allCatalogedEntriesReadOnly\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalog.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.expectedCatalogedReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.releaseVersionsContiguous\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.latestDigestMatchesFrozenSource\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.extractedCatalogFormatter\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.preservesReleaseCatalogAudit\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.publicCatalogSectionsChanged\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupCloseoutCatalogMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.packageReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.packageReadyForArchiveVerification\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.allPackagedEntriesReadOnly\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackage.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.expectedPackageReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.releaseVersionsContiguous\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.latestPackagedDigestMatchesPackageSource\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.packageReadyForArchiveVerification\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupLatestEvidencePackageAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.miniKvCloseoutReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.nodeBatchCloseoutVersion\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.futureNodeBatchMayConsumeCleanTags\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.closeoutReadyForNextBatch\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseout.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.expectedCloseoutReleaseCount\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.releaseVersionsContiguous\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.latestCloseoutDigestMatchesCloseoutSource\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.closeoutReadyForNextBatch\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit.executionAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.stageSequence\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.stageReleaseVersion\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.sourceFixtureVersioned\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.preservesBatchCloseoutAudit\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.readyForNextNodeBatch\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.routerActivationAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.writeRoutingAllowed\"");
    assert_response_contains(result, "\"nodeRouteCatalogCleanupPostCloseoutContinuity.executionAllowed\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.preservesFixtureParity\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.preservesArchiveCompatibility\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.preservesHistoricalFallback\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.changesShardJsonCommand\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.changesFixturePath\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.startsServices\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.runtimeProbeAllowed\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.writeRoutingAllowed\"");
    assert_response_contains(result, "\"shardReadinessHistoryMaintenance.executionAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.publicShardJsonContractChanged\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.routerActivationAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.writeRoutingAllowed\"");
    assert_response_contains(result, "\"boundaryCatalogIndex.executionAllowed\"");

    result = processor.execute("EXPLAINJSON");
    assert(result.response == "ERR usage: EXPLAINJSON command");

    result = processor.execute("EXPLAINJSON SET orderops:1 value");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"command_digest\":\"fnv1a64:");
    const auto set_digest = extract_json_string_field(result.response, "command_digest");
    assert(set_digest.size() == 24);
    assert_response_contains(result, "\"command\":\"SET\"");
    assert_response_contains(result, "\"category\":\"write\"");
    assert_response_contains(result, "\"mutates_store\":true");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"key\":\"orderops:1\"");
    assert_response_contains(result, "\"requires_value\":true");
    assert_response_contains(result, "\"ttl_sensitive\":false");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");
    assert_response_contains(result, "\"warnings\":[]");

    const auto repeated_set = processor.execute("EXPLAINJSON SET orderops:1 value");
    assert(extract_json_string_field(repeated_set.response, "command_digest") == set_digest);

    result = processor.execute("GET orderops:1");
    assert(result.response == "(nil)");

    result = processor.execute("EXPLAINJSON GET orderops:1");
    assert(extract_json_string_field(result.response, "command_digest") != set_digest);
    const auto get_digest = extract_json_string_field(result.response, "command_digest");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"command\":\"GET\"");
    assert_response_contains(result, "\"category\":\"read\"");
    assert_response_contains(result, "\"mutates_store\":false");
    assert_response_contains(result, "\"touches_wal\":false");
    assert_response_contains(result, "\"key\":\"orderops:1\"");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON EXPIRE orderops:1 60");
    assert_response_contains(result, "\"command\":\"EXPIRE\"");
    assert_response_contains(result, "\"ttl_sensitive\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_ttl_update\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");

    result = processor.execute("EXPLAINJSON SETNXEX orderops:token 30 value");
    assert_response_contains(result, "\"command\":\"SETNXEX\"");
    assert_response_contains(result, "\"category\":\"write\"");
    assert_response_contains(result, "\"mutates_store\":true");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"key\":\"orderops:token\"");
    assert_response_contains(result, "\"requires_value\":true");
    assert_response_contains(result, "\"ttl_sensitive\":true");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"store_ttl_update\","
                                     "\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");

    result = processor.execute("EXPLAINJSON GET orderops:1 extra");
    assert_response_contains(result, "\"command\":\"GET\"");
    assert_response_contains(result, "\"allowed_by_parser\":false");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");
    assert_response_contains(result, "\"warnings\":[\"usage: GET key\"]");

    result = processor.execute("EXPLAINJSON NOPE orderops:1");
    assert_response_contains(result, "\"command\":\"NOPE\"");
    assert_response_contains(result, "\"category\":\"unknown\"");
    assert_response_contains(result, "\"allowed_by_parser\":false");
    assert_response_contains(result, "\"side_effects\":[]");
    assert_response_contains(result, "\"side_effect_count\":0");
    assert_response_contains(result, "\"warnings\":[\"unknown command\"]");

    result = processor.execute("EXPLAINJSON TTL orderops:1");
    assert_response_contains(result, "\"command\":\"TTL\"");
    assert_response_contains(result, "\"ttl_sensitive\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON DEL orderops:1");
    assert_response_contains(result, "\"command\":\"DEL\"");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");

    result = processor.execute("EXPLAINJSON SAVE data/snap.txt");
    assert_response_contains(result, "\"command\":\"SAVE\"");
    assert_response_contains(result, "\"category\":\"admin\"");
    assert_response_contains(result, "\"side_effects\":[\"snapshot_file_write\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON LOAD data/snap.txt");
    assert_response_contains(result, "\"command\":\"LOAD\"");
    assert_response_contains(result, "\"mutates_store\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_replace_from_snapshot\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON COMPACT");
    assert_response_contains(result, "\"command\":\"COMPACT\"");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"side_effects\":[\"wal_rewrite_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON RESETSTATS");
    assert_response_contains(result, "\"command\":\"RESETSTATS\"");
    assert_response_contains(result, "\"side_effects\":[\"metrics_reset\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON INFO");
    assert_response_contains(result, "\"command\":\"INFO\"");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("EXPLAINJSON STORAGEJSON");
    assert_response_contains(result, "\"command\":\"STORAGEJSON\"");
    assert_response_contains(result, "\"category\":\"read\"");
    assert_response_contains(result, "\"mutates_store\":false");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\",\"store_read\","
                                     "\"wal_metadata_read_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");

    result = processor.execute("EXPLAINJSON SMOKEJSON");
    assert_response_contains(result, "\"command\":\"SMOKEJSON\"");
    assert_response_contains(result, "\"category\":\"meta\"");
    assert_response_contains(result, "\"mutates_store\":false");
    assert_response_contains(result, "\"touches_wal\":true");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\",\"store_read\","
                                     "\"wal_metadata_read_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");

    result = processor.execute("EXPLAINJSON QUIT");
    assert_response_contains(result, "\"command\":\"QUIT\"");
    assert_response_contains(result, "\"side_effects\":[\"connection_close\"]");
    assert_response_contains(result, "\"side_effect_count\":1");

    result = processor.execute("CHECKJSON");
    assert(result.response == "ERR usage: CHECKJSON command");

    result = processor.execute("CHECKJSON SET orderops:1 value");
    const auto checkjson_set_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "set-orderops-write-contract.json");
    assert(result.response == checkjson_set_fixture);
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"command_digest\":\"" + set_digest + "\"");
    assert_response_contains(result, "\"command\":\"SET\"");
    assert_response_contains(result, "\"write_command\":true");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":2");
    assert_response_contains(result, "\"checks\":{\"parser_allowed\":true,\"write_command\":true,"
                                     "\"wal_append_when_enabled\":true,\"wal_enabled\":false}");
    assert_response_contains(result, "\"wal\":{\"enabled\":false,\"touches_wal\":true,"
                                     "\"append_when_enabled\":true,\"durability\":\"memory_only\"}");
    assert_response_contains(result, "\"warnings\":[\"wal disabled; write would be in-memory only\"]");

    result = processor.execute("CHECKJSON SETNXEX orderops:token 30 value");
    assert_response_contains(result, "\"command\":\"SETNXEX\"");
    assert_response_contains(result, "\"write_command\":true");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_write\",\"store_ttl_update\","
                                     "\"wal_append_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");
    assert_response_contains(result, "\"checks\":{\"parser_allowed\":true,\"write_command\":true,"
                                     "\"wal_append_when_enabled\":true,\"wal_enabled\":false}");
    assert_response_contains(result, "\"durability\":\"memory_only\"");
    assert_response_contains(result, "\"warnings\":[\"wal disabled; write would be in-memory only\"]");

    result = processor.execute("GET orderops:1");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON GET orderops:1");
    const auto checkjson_get_fixture =
        read_fixture_text(std::filesystem::path{"fixtures"} / "checkjson" / "get-orderops-read-contract.json");
    const auto ttl_token_index =
        read_fixture_text(std::filesystem::path{"fixtures"} / "ttl-token" / "index.json");

    assert_response_contains({ttl_token_index}, "\"index_version\":\"mini-kv-ttl-token-fixtures.v1\"");
    assert_response_contains({ttl_token_index}, "\"command\":\"SETNXEX key seconds value\"");
    assert_response_contains({ttl_token_index}, "\"order_authoritative\":false");
    assert_response_contains({ttl_token_index}, "\"consumer_hint\":\"Node v160 idempotency vertical readiness review\"");
    assert_response_contains({ttl_token_index}, "\"atomic_absent_claim\":true");
    assert_response_contains({ttl_token_index}, "\"enabled_record\":\"SETEXAT key epoch_millis value\"");
    assert_response_contains({ttl_token_index}, "\"single_record_claim\":true");
    assert_response_contains({ttl_token_index}, "\"replay_drops_expired_token\":true");
    assert_response_contains({ttl_token_index}, "not connected to Java transaction chain");
    assert_response_contains({ttl_token_index}, "CHECKJSON SETNXEX can inspect the contract without executing the write");
    assert(result.response == checkjson_get_fixture);
    assert_response_contains(result, "\"command_digest\":\"" + get_digest + "\"");
    assert_response_contains(result, "\"command\":\"GET\"");
    assert_response_contains(result, "\"write_command\":false");
    assert_response_contains(result, "\"allowed_by_parser\":true");
    assert_response_contains(result, "\"side_effects\":[\"store_read\"]");
    assert_response_contains(result, "\"side_effect_count\":1");
    assert_response_contains(result, "\"checks\":{\"parser_allowed\":true,\"write_command\":false,"
                                     "\"wal_append_when_enabled\":false,\"wal_enabled\":false}");
    assert_response_contains(result, "\"durability\":\"not_applicable\"");
    assert_response_contains(result, "\"warnings\":[\"not a write command\"]");

    result = processor.execute("STORAGEJSON extra");
    assert(result.response == "ERR usage: STORAGEJSON");

    result = processor.execute("STORAGEJSON");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_response_contains(result, "\"store\":{\"live_keys\":1,\"order_authoritative\":false}");
    assert_response_contains(result, "\"wal\":{\"enabled\":false,\"status\":\"disabled\"}");
    assert_response_contains(result, "\"snapshot\":{\"supported\":true,\"mode\":\"manual\",\"atomic_save\":true,"
                                     "\"load_replaces_store\":true}");
    assert_response_contains(result, "\"side_effects\":[\"metadata_read\",\"store_read\","
                                     "\"wal_metadata_read_when_enabled\"]");
    assert_response_contains(result, "\"side_effect_count\":3");
    assert_response_contains(result, "\"diagnostics\":{\"read_only_command\":true,\"write_commands_executed\":false,"
                                     "\"order_authoritative\":false");
    assert_response_contains(result, "\"notes\":[\"read_only_storage_evidence\",\"not_order_authoritative\","
                                     "\"manual_snapshot_only\",\"wal_disabled\"]");

    result = processor.execute("SMOKEJSON extra");
    assert(result.response == "ERR usage: SMOKEJSON");

    result = processor.execute("SMOKEJSON");
    assert_response_contains(result, "\"schema_version\":1");
    assert_response_contains(result, "\"read_only\":true");
    assert_response_contains(result, "\"execution_allowed\":false");
    assert_response_contains(result, "\"restore_execution_allowed\":false");
    assert_response_contains(result, "\"order_authoritative\":false");
    assert_response_contains(result, "\"evidence_type\":\"runtime_smoke\"");
    assert_response_contains(result, "\"version\":\"" + std::string{minikv::version} + "\"");
    assert_response_contains(result, "\"store\":{\"live_keys\":1,\"order_authoritative\":false}");
    assert_response_contains(result, "\"write_commands_executed\":false");
    assert_response_contains(result, "\"runtime_write_observed\":false");

    result = processor.execute("GET restore:real-read-token");
    assert(result.response == "(nil)");

    result = processor.execute("CHECKJSON SET orderops:1");
    assert_response_contains(result, "\"command\":\"SET\"");
    assert_response_contains(result, "\"write_command\":true");
    assert_response_contains(result, "\"allowed_by_parser\":false");
    assert_response_contains(result, "\"warnings\":[\"usage: SET key value\","
                                     "\"wal disabled; write would be in-memory only\"]");

    const auto wal_contract_path = std::filesystem::path{"minikv-command-checkjson-test.wal"};
    std::filesystem::remove(wal_contract_path);
    minikv::Store wal_contract_store;
    minikv::WriteAheadLog wal_contract{wal_contract_path};
    minikv::CommandProcessor wal_contract_processor{wal_contract_store, &wal_contract};
    result = wal_contract_processor.execute("CHECKJSON SET orderops:1 value");
    assert_response_contains(result, "\"wal_enabled\":true");
    assert_response_contains(result, "\"wal\":{\"enabled\":true,\"touches_wal\":true,"
                                     "\"append_when_enabled\":true,\"durability\":\"wal_backed\"}");
    assert_response_contains(result, "\"warnings\":[]");

    result = wal_contract_processor.execute("STORAGEJSON");
    assert_response_contains(result, "\"wal\":{\"enabled\":true,\"status\":\"enabled\"");
    assert_response_contains(result, "\"records\":0");
    assert_response_contains(result, "\"live_keys\":0");
    assert_response_contains(result, "\"notes\":[\"read_only_storage_evidence\",\"not_order_authoritative\","
                                     "\"manual_snapshot_only\",\"wal_enabled\"]");
    assert(!std::filesystem::exists(wal_contract_path));
    std::filesystem::remove(wal_contract_path);

    minikv::Store inventory_store;
    minikv::CommandProcessor inventory_processor{inventory_store};
    for (int index = 0; index < 1002; ++index) {
        result = inventory_processor.execute("SET bulk:" + std::to_string(index) + " value");
        assert(result.response == "OK inserted");
    }
    result = inventory_processor.execute("KEYSJSON bulk:");
    assert(result.response.find("\"prefix\":\"bulk:\"") != std::string::npos);
    assert(result.response.find("\"key_count\":1000") != std::string::npos);
    assert(result.response.find("\"truncated\":true") != std::string::npos);
    assert(result.response.find("\"limit\":1000") != std::string::npos);

    minikv::CommandProcessorOptions stats_options;
    stats_options.connection_stats = [] {
        minikv::CommandConnectionStats stats;
        stats.available = true;
        stats.total_connections = 7;
        stats.active_connections = 2;
        stats.peak_connections = 3;
        return stats;
    };
    minikv::CommandProcessor stats_processor{store, nullptr, stats_options};

    result = stats_processor.execute("STATS");
    assert(result.response.find("connection_stats_available=yes") != std::string::npos);
    assert(result.response.find("active_connections=2") != std::string::npos);
    assert(result.response.find("total_connections=7") != std::string::npos);
    assert(result.response.find("peak_connections=3") != std::string::npos);

    result = stats_processor.execute("HEALTH");
    assert(result.response.find("connection_stats_available=yes") != std::string::npos);
    assert(result.response.find("active_connections=2") != std::string::npos);

    minikv::Store metrics_store;
    minikv::CommandProcessor metrics_processor{metrics_store};
    assert(metrics_processor.metrics().total_commands == 0);
    assert(metrics_processor.metrics().successful_commands == 0);
    assert(metrics_processor.metrics().error_commands == 0);
    assert(metrics_processor.metrics().total_latency_ns == 0);
    assert(metrics_processor.metrics().max_latency_ns == 0);
    assert(metrics_processor.metrics().command_breakdown.empty());

    result = metrics_processor.execute("");
    assert(result.response.empty());
    assert(metrics_processor.metrics().total_commands == 0);

    result = metrics_processor.execute("PING");
    assert(result.response == "PONG");
    assert(metrics_processor.metrics().total_commands == 1);
    assert(metrics_processor.metrics().successful_commands == 1);
    assert(metrics_processor.metrics().error_commands == 0);
    auto metrics = metrics_processor.metrics();
    assert(metrics.total_latency_ns > 0);
    assert(metrics.max_latency_ns > 0);
    const auto* ping_metrics = find_command_metrics(metrics, "PING");
    assert(ping_metrics != nullptr);
    assert(ping_metrics->total_commands == 1);
    assert(ping_metrics->successful_commands == 1);
    assert(ping_metrics->error_commands == 0);
    assert(ping_metrics->total_latency_ns > 0);
    assert(ping_metrics->max_latency_ns > 0);

    result = metrics_processor.execute("GET name extra");
    assert(result.response == "ERR usage: GET key");
    assert(metrics_processor.metrics().total_commands == 2);
    assert(metrics_processor.metrics().successful_commands == 1);
    assert(metrics_processor.metrics().error_commands == 1);
    metrics = metrics_processor.metrics();
    const auto* get_metrics = find_command_metrics(metrics, "GET");
    assert(get_metrics != nullptr);
    assert(get_metrics->total_commands == 1);
    assert(get_metrics->successful_commands == 0);
    assert(get_metrics->error_commands == 1);

    result = metrics_processor.execute("BADCOMMAND");
    assert(result.response == "ERR unknown command");
    assert(metrics_processor.metrics().total_commands == 3);
    assert(metrics_processor.metrics().successful_commands == 1);
    assert(metrics_processor.metrics().error_commands == 2);
    metrics = metrics_processor.metrics();
    const auto* unknown_metrics = find_command_metrics(metrics, "UNKNOWN");
    assert(unknown_metrics != nullptr);
    assert(unknown_metrics->total_commands == 1);
    assert(unknown_metrics->successful_commands == 0);
    assert(unknown_metrics->error_commands == 1);

    result = metrics_processor.execute("STATS");
    assert(result.response.find("total_commands=3") != std::string::npos);
    assert(result.response.find("successful_commands=1") != std::string::npos);
    assert(result.response.find("error_commands=2") != std::string::npos);
    assert(result.response.find("total_latency_ns=") != std::string::npos);
    assert(result.response.find("avg_latency_ns=") != std::string::npos);
    assert(result.response.find("max_latency_ns=") != std::string::npos);
    assert(result.response.find("command_breakdown=GET:1/0/1/") != std::string::npos);
    assert(result.response.find("PING:1/1/0/") != std::string::npos);
    assert(result.response.find("UNKNOWN:1/0/1/") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 4);
    assert(metrics_processor.metrics().successful_commands == 2);
    assert(metrics_processor.metrics().error_commands == 2);
    metrics = metrics_processor.metrics();
    const auto* stats_metrics = find_command_metrics(metrics, "STATS");
    assert(stats_metrics != nullptr);
    assert(stats_metrics->total_commands == 1);
    assert(stats_metrics->successful_commands == 1);
    assert(stats_metrics->error_commands == 0);

    result = metrics_processor.execute("HEALTH");
    assert(result.response.find("total_commands=4") != std::string::npos);
    assert(result.response.find("successful_commands=2") != std::string::npos);
    assert(result.response.find("error_commands=2") != std::string::npos);
    assert(result.response.find("STATS:1/1/0/") != std::string::npos);

    result = metrics_processor.execute("STATSJSON");
    assert(result.response.find("\"schema_version\":1") != std::string::npos);
    assert(result.response.find("\"read_only\":true") != std::string::npos);
    assert(result.response.find("\"execution_allowed\":false") != std::string::npos);
    assert(result.response.find("\"order_authoritative\":false") != std::string::npos);
    assert(result.response.find("\"evidence_type\":\"runtime_metrics\"") != std::string::npos);
    assert(result.response.find("\"live_keys\":0") != std::string::npos);
    assert(result.response.find("\"wal_enabled\":false") != std::string::npos);
    assert(result.response.find("\"wal\":null") != std::string::npos);
    assert(result.response.find("\"commands\":{\"total_commands\":5") != std::string::npos);
    assert(result.response.find("\"successful_commands\":3") != std::string::npos);
    assert(result.response.find("\"error_commands\":2") != std::string::npos);
    assert(result.response.find("\"command\":\"PING\"") != std::string::npos);
    assert(result.response.find("\"command\":\"UNKNOWN\"") != std::string::npos);
    assert(result.response.find("\"connection_stats\":{\"available\":false}") != std::string::npos);
    assert(result.response.find("\"diagnostics\":{\"write_commands_executed\":false") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 6);
    metrics = metrics_processor.metrics();
    const auto* statsjson_metrics = find_command_metrics(metrics, "STATSJSON");
    assert(statsjson_metrics != nullptr);
    assert(statsjson_metrics->total_commands == 1);
    assert(statsjson_metrics->successful_commands == 1);

    minikv::CommandProcessorOptions info_options;
    info_options.runtime_info.protocol = "inline,resp";
    info_options.runtime_info.started_at = std::chrono::steady_clock::now() - 3s;
    info_options.runtime_info.max_request_bytes = 4096;
    info_options.runtime_info.metrics_enabled = true;
    minikv::CommandProcessor info_processor{metrics_store, nullptr, info_options};
    result = info_processor.execute("INFO");
    assert(result.response.find("protocol=inline,resp") != std::string::npos);
    assert(result.response.find("metrics_enabled=yes") != std::string::npos);
    assert(result.response.find("max_request_bytes=4096") != std::string::npos);

    result = info_processor.execute("INFOJSON");
    assert(result.response.find("\"protocol\":[\"inline\",\"resp\"]") != std::string::npos);
    assert(result.response.find("\"metrics\":{\"enabled\":true}") != std::string::npos);
    assert(result.response.find("\"max_request_bytes\":4096") != std::string::npos);
    assert(result.response.find("\"evidence_type\":\"runtime_identity\"") != std::string::npos);

    result = metrics_processor.execute("RESETSTATS extra");
    assert(result.response == "ERR usage: RESETSTATS");
    assert(metrics_processor.metrics().total_commands == 7);
    assert(metrics_processor.metrics().error_commands == 3);

    result = metrics_processor.execute("RESETSTATS");
    assert(result.response == "OK stats reset");
    assert(metrics_processor.metrics().total_commands == 0);
    assert(metrics_processor.metrics().successful_commands == 0);
    assert(metrics_processor.metrics().error_commands == 0);
    assert(metrics_processor.metrics().command_breakdown.empty());

    result = metrics_processor.execute("STATS");
    assert(result.response.find("total_commands=0") != std::string::npos);
    assert(result.response.find("successful_commands=0") != std::string::npos);
    assert(result.response.find("error_commands=0") != std::string::npos);
    assert(result.response.find("command_breakdown=none") != std::string::npos);
    assert(metrics_processor.metrics().total_commands == 1);

    result = processor.execute("HELP");
    assert(result.response.find("COMPACT") != std::string::npos);
    assert(result.response.find("KEYS") != std::string::npos);
    assert(result.response.find("KEYSJSON") != std::string::npos);
    assert(result.response.find("WALINFO") != std::string::npos);
    assert(result.response.find("STATS") != std::string::npos);
    assert(result.response.find("STATSJSON") != std::string::npos);
    assert(result.response.find("RESETSTATS") != std::string::npos);
    assert(result.response.find("HEALTH") != std::string::npos);
    assert(result.response.find("INFO") != std::string::npos);
    assert(result.response.find("INFOJSON") != std::string::npos);
    assert(result.response.find("SHARDJSON") != std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTJSON") != std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTCLOSEOUTJSON") != std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTARCHIVEJSON") != std::string::npos);
    assert(result.response.find("COMMANDS") != std::string::npos);
    assert(result.response.find("COMMANDSJSON") != std::string::npos);
    assert(result.response.find("EXPLAINJSON") != std::string::npos);
    assert(result.response.find("CHECKJSON") != std::string::npos);
    assert(result.response.find("STORAGEJSON") != std::string::npos);
    assert(result.response.find("SETNXEX") != std::string::npos);

    result = processor.execute("GET name extra");
    assert(result.response == "ERR usage: GET key");

    result = processor.execute("UNKNOWN");
    assert(result.response == "ERR unknown command");

    result = processor.execute("QUIT");
    assert(result.response == "BYE");
    assert(result.should_close);

    return 0;
}
