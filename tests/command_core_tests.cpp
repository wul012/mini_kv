#include "command_test_suites.hpp"

#include "../src/command_evidence_dispatch.hpp"
#include "minikv/command_catalog.hpp"

#include <set>

namespace minikv::command_test {

void run_command_core_tests(CommandFixture& fixture) {
    using namespace std::chrono_literals;

    const auto evidence_commands = command_internal::evidence_commands();
    assert(evidence_commands.size() == 62);
    std::set<std::string_view> evidence_names;
    for (const auto& entry : evidence_commands) {
        assert(entry.format != nullptr);
        assert(command_catalog::is_known(entry.name));
        assert(evidence_names.emplace(entry.name).second);
        assert(command_internal::find_evidence_command(entry.name) == &entry);
    }
    assert(command_internal::find_evidence_command("UNKNOWN") == nullptr);

    auto& store = fixture.store;
    auto& processor = fixture.processor;
    auto& result = fixture.result;

    result = processor.execute("");
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
    assert(result.response == "{\"prefix\":null,\"key_count\":3,\"keys\":[\"alpha\",\"alpine\",\"name\"],\"truncated\":"
                              "false,\"limit\":1000}");

    result = processor.execute("KEYSJSON alp");
    assert(result.response ==
           "{\"prefix\":\"alp\",\"key_count\":2,\"keys\":[\"alpha\",\"alpine\"],\"truncated\":false,\"limit\":1000}");

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
    assert_response_contains(
        result, "\"ci_evidence\":{\"consumer\":\"Node v201 real-read window CI artifact manifest verification\"");
    assert_response_contains(result, "\"artifact_path_hint\":\"c/80/\"");
    assert_response_contains(result, "\"no_restore_proof\":true");
    assert_response_contains(result, "\"upload_allowed\":false");
    assert_response_contains(
        result, "\"artifact_retention\":{\"consumer\":\"Node v203 cross-project CI artifact retention gate\"");
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
    assert(result.response.find("command_count=94") != std::string::npos);
    assert(result.response.find("PING(category=meta,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SET(category=write,mutates_store=yes,touches_wal=yes,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SETNXEX(category=write,mutates_store=yes,touches_wal=yes,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("GET(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("KEYSJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("COMPACT(category=admin,mutates_store=no,touches_wal=yes,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("COMMANDSJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTE(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(
        result.response.find("SHARDROUTEVERIFYREPORTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
        std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVERIFYREPORTCLOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVERIFYREPORTARCHIVEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON(category="
                                "read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(
        result.response.find("SHARDROUTEARCHIVEMAINTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
        std::string::npos);
    assert(result.response.find(
               "SHARDROUTEARCHIVEMAINTVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTEWORKSHEETVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTEIMPORTPREFLIGHTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEVALUEDRAFTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(
        result.response.find("SHARDROUTEVALUESUPPLYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
        std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYPRECHECKJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(
        result.response.find("SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON(category=read,mutates_store=no,touches_wal=no,"
                             "stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON(category=read,mutates_store=no,touches_wal=no,"
               "stable=yes)") != std::string::npos);
    assert(
        result.response.find("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON(category=read,mutates_store=no,"
                             "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON(category=read,mutates_store=no,"
               "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON(category=read,"
               "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON(category=read,"
               "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON(category=read,"
               "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(
        result.response.find(
            "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON(category=read,"
            "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDE"
                                "NCEINTAKEAUDITJSON(category=read,"
                                "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQ"
                                "UESTPACKAGECLOSEOUTJSON(category=read,"
                                "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON(category=read,mutates_store=no,touches_"
                                "wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTECANDIDATEINTAKEPACKETJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find(
               "SHARDROUTECANDIDATEMATERIALREQUESTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTECANDIDATEPROFILESECTIONJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON(category="
                                "read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON("
                                "category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON("
                                "category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find(
               "SHARDROUTEVALUESUPPLYPROFILESECTIONJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON(category=read,"
                                "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON(category="
                                "read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON(category="
                                "read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON(category=read,"
                                "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON"
                                "(category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON(category=read,"
                                "mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON("
                                "category=read,mutates_store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON(category=read,mutates_store="
                                "no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON(category=read,mutates_"
                                "store=no,touches_wal=no,stable=yes)") != std::string::npos);
    assert(
        result.response.find(
            "SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)") !=
        std::string::npos);
    assert(result.response.find("SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON(category=read,mutates_store=no,touches_"
                                "wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON(category=read,mutates_store=no,"
                                "touches_wal=no,stable=yes)") != std::string::npos);
    assert(result.response.find("EXPLAINJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("CHECKJSON(category=meta,mutates_store=no,touches_wal=no,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("SMOKEJSON(category=meta,mutates_store=no,touches_wal=yes,stable=yes)") !=
           std::string::npos);
    assert(result.response.find("STORAGEJSON(category=read,mutates_store=no,touches_wal=yes,stable=yes)") !=
           std::string::npos);

    result = processor.execute("COMMANDSJSON extra");
    assert(result.response == "ERR usage: COMMANDSJSON");

    result = processor.execute("COMMANDSJSON");
    assert(result.response.find("\"commands\":[") != std::string::npos);
    assert(result.response.find("\"name\":\"PING\",\"category\":\"meta\",\"mutates_store\":false") !=
           std::string::npos);
    assert(
        result.response.find("\"name\":\"SET\",\"category\":\"write\",\"mutates_store\":true,\"touches_wal\":true") !=
        std::string::npos);
    assert(result.response.find("\"name\":\"SETNXEX\",\"category\":\"write\",\"mutates_store\":true,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"GET\",\"category\":\"read\",\"mutates_store\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"KEYSJSON\",\"category\":\"read\",\"mutates_store\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"LOAD\",\"category\":\"admin\",\"mutates_store\":true") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"COMMANDSJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTE\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVERIFYJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":"
                                "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVI"
                                "DENCEINTAKEAUDITJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":"
                                "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTR"
                                "EQUESTPACKAGECLOSEOUTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(
        result.response.find(
            "\"name\":\"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\",\"category\":\"read\",\"mutates_store\":false,"
            "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTECANDIDATEINTAKEPACKETJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTECANDIDATEMATERIALREQUESTJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON\",\"category\":\"read\","
                                "\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON\",\"category\":\"read\","
                                "\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON\",\"category\":\"read\","
               "\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTECANDIDATEPROFILESECTIONJSON\",\"category\":\"read\","
                                "\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYAPPROVALPROFILESECTIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDPRODUCTIONSHARDEXECUTIONEXTERNALARTIFACTDRYRUNCLOSEOUTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVERIFYREPORTJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTCLOSEOUTJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\",\"category\":\"read\",\"mutates_store\":false,"
               "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\",\"category\":\"read\","
                                "\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\",\"category\":"
                                "\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\",\"category\":\"read\","
               "\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEARCHIVEMAINTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEWORKSHEETVERIFYJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEIMPORTPREFLIGHTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUEDRAFTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find("\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\","
                                "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") !=
           std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find(
               "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\","
               "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false") != std::string::npos);
    assert(result.response.find("\"name\":\"EXPLAINJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"CHECKJSON\",\"category\":\"meta\"") != std::string::npos);
    assert(result.response.find("\"name\":\"SMOKEJSON\",\"category\":\"meta\",\"mutates_store\":false,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"name\":\"STORAGEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                "\"touches_wal\":true") != std::string::npos);
    assert(result.response.find("\"description\":\"Read command catalog as JSON\"") != std::string::npos);
}

} // namespace minikv::command_test
