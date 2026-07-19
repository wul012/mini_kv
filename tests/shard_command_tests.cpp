#include "shard_test_suites.hpp"

namespace minikv::shard_test {

void run_shard_command_tests(ShardFixture& fixture) {
    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDJSON extra");
    assert(result.response == "ERR usage: SHARDJSON");

    result = processor.execute("SHARDJSON");
    assert(result.response == fixture.current);
    assert_shard_readiness_contract(fixture, result.response);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    assert_contains(result.response, "SHARDJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTE(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVERIFYREPORTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVERIFYREPORTARCHIVEJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        result.response,
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        result.response,
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON(category=read,mutates_store="
                                     "no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON(category=read,"
                                     "mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON("
                                     "category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEARCHIVEMAINTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEARCHIVEMAINTVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEWORKSHEETVERIFYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEIMPORTPREFLIGHTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUEDRAFTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUESUPPLYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUESUPPLYPRECHECKJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON(category=read,mutates_store=no,touches_wal=no,"
                    "stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON(category=read,mutates_store=no,touches_wal=no,"
                    "stable=yes)");
    assert_contains(result.response, "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON("
                                     "category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHT"
                                     "JSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTA"
                                     "UDITJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response, "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSE"
                                     "OUTAUDITJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUD"
                    "ITJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGEC"
                    "LOSEOUTJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        result.response,
        "SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        result.response,
        "SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(result.response,
                    "SHARDROUTECANDIDATEINTAKEPACKETJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    assert_contains(result.response, "\"name\":\"SHARDJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTE\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVERIFYJSON\",\"category\":\"read\",\"mutates_store\":false,"
                                     "\"touches_wal\":false");
    assert_contains(
        result.response,
        "\"name\":"
        "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\","
        "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":"
                                     "\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCU"
                                     "MENTREQUESTPACKAGECLOSEOUTJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON\",\"category\":\"read\","
                    "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\",\"category\":\"read\","
                                     "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTECANDIDATEINTAKEPACKETJSON\",\"category\":\"read\","
                                     "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTJSON\",\"category\":\"read\",\"mutates_store\":false,"
                    "\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\",\"category\":\"read\",\"mutates_store\":false,"
                    "\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\",\"category\":\"read\","
                                     "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\",\"category\":\"read\","
                    "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\",\"category\":\"read\","
                    "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\",\"category\":\"read\","
                    "\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\","
                    "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEARCHIVEMAINTJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEARCHIVEMAINTVERIFYJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEWORKSHEETVERIFYJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEIMPORTPREFLIGHTJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVALUEDRAFTJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVALUESUPPLYJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response, "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\","
                                     "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(result.response,
                    "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\","
                    "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(
        result.response,
        "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\","
        "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");
    assert_contains(
        result.response,
        "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\","
        "\"category\":\"read\",\"mutates_store\":false,\"touches_wal\":false");

    result = processor.execute("EXPLAINJSON SHARDJSON");
    assert_contains(result.response, "\"command\":\"SHARDJSON\"");
    assert_contains(result.response, "\"category\":\"read\"");
    assert_contains(result.response, "\"mutates_store\":false");
    assert_contains(result.response, "\"touches_wal\":false");
    assert_contains(result.response, "\"side_effects\":[\"metadata_read\"]");

    result = processor.execute("CHECKJSON SHARDJSON");
    assert_contains(result.response, "\"command\":\"SHARDJSON\"");
    assert_contains(result.response, "\"write_command\":false");
    assert_contains(result.response, "\"execution_allowed\":false");
    assert_contains(result.response, "\"warnings\":[\"not a write command\"]");

    result = processor.execute("CHECKJSON SHARDJSON extra");
    assert_contains(result.response, "\"allowed_by_parser\":false");
    assert_contains(result.response, "\"usage: SHARDJSON\"");

    result = processor.execute("SHARDROUTE orderops:alpha");
    assert(result.response == "slot=5 shard=shard-0 route_mode=preview-only read_only=yes "
                              "router_installed=no write_routing_allowed=no execution_allowed=no");

    result = processor.execute("SHARDROUTEJSON orderops:alpha");
    assert_contains(result.response, "\"contract\":\"shard-route-preview.v1\"");
    assert_contains(result.response, "\"slot\":5");
    assert_contains(result.response, "\"pinnedFixtureSample\":true");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYJSON orderops:alpha");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(result.response, "\"checkedCommand\":\"SHARDROUTEJSON\"");
    assert_contains(result.response, "\"passed\":true");
    assert_contains(result.response, "\"executesRoute\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYREPORTJSON orderops:alpha");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTJSON\"");
    assert_contains(result.response, "\"reportedCommand\":\"SHARDROUTEVERIFYJSON\"");
    assert_contains(result.response, "\"verificationPassed\":true");
    assert_contains(result.response, "\"executesRoute\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVEJSON");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report-archive.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVEJSON\"");
    assert_contains(result.response, "\"archiveStage\":\"route-preview-verification-report-archive-closeout-audit\"");
    assert_contains(result.response, "\"executionAllowed\":false");

    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report-archive-closeout.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON\"");
    assert_contains(result.response,
                    "\"closeoutStage\":\"route-preview-verification-report-archive-closeout-release-package\"");
    assert_contains(result.response, "\"sourceArchiveChainComplete\":true");
    assert_contains(result.response, "\"archiveCloseoutChainComplete\":true");
    assert_contains(result.response, "\"executionAllowed\":false");
    // Archive closeout verification command smoke updated per mini-kv release.
    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON");
    assert_contains(result.response,
                    "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON\"");
    assert_contains(
        result.response,
        "\"verificationStage\":\"route-preview-verification-report-archive-closeout-verification-release-package\"");
    assert_contains(result.response, "\"verificationReleaseVersion\":\"v400\"");
    assert_contains(result.response, "\"sourceFrozenReleaseVersion\":\"v399\"");
    assert_contains(result.response, "\"archiveCloseoutVerificationChainComplete\":true");
    assert_contains(result.response, "\"filesystemReadPerformed\":false");
    assert_contains(result.response, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");
    // End archive closeout verification command smoke.
    // Archive closeout verification audit command smoke updated per mini-kv release.
    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON");
    assert_contains(result.response,
                    "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(
        result.response,
        "\"auditStage\":\"route-preview-verification-report-archive-closeout-verification-audit-release-package\"");
    assert_contains(result.response, "\"auditReleaseVersion\":\"v420\"");
    assert_contains(result.response, "\"sourceFrozenReleaseVersion\":\"v419\"");
    assert_contains(result.response, "\"archiveCloseoutVerificationAuditChainComplete\":true");
    assert_contains(result.response, "\"filesystemReadPerformed\":false");
    assert_contains(result.response, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");
    // End archive closeout verification audit command smoke.
    // Archive closeout verification audit closeout command smoke.
    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON");
    assert_contains(
        result.response,
        "\"contract\":\"shard-route-preview-verification-report-archive-closeout-verification-audit-closeout.v1\"");
    assert_contains(result.response, "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(result.response, "\"sourceAuditCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON\"");
    assert_contains(result.response, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(result.response, "\"filesystemReadPerformed\":false");
    assert_contains(result.response, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");
    // End archive closeout verification audit closeout command smoke.
    // Archive closeout verification audit closeout archive verification command smoke.
    result = processor.execute("SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON");
    assert_contains(result.response, "\"contract\":\"shard-route-preview-verification-report-archive-closeout-"
                                     "verification-audit-closeout-archive-verification.v1\"");
    assert_contains(result.response,
                    "\"command\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON\"");
    assert_contains(result.response,
                    "\"sourceAuditCloseoutCommand\":\"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON\"");
    assert_contains(result.response, "\"sharedStageCatalogFormatterApplied\":true");
    assert_contains(result.response, "\"declaredArchiveVerificationOnly\":true");
    assert_contains(result.response, "\"filesystemReadPerformed\":false");
    assert_contains(result.response, "\"runtimeArchiveWalkAllowed\":false");
    assert_contains(result.response, "\"activeRouterInstalled\":false");
    assert_contains(result.response, "\"writeRoutingAllowed\":false");
    assert_contains(result.response, "\"executionAllowed\":false");
    // End archive closeout verification audit closeout archive verification command smoke.
}

} // namespace minikv::shard_test
