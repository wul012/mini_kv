#include "minikv/line_editor.hpp"

#include <cassert>
#include <optional>
#include <string>
#include <vector>

int main() {
    minikv::LineEditorBuffer buffer;
    assert(buffer.empty());
    assert(buffer.cursor() == 0);

    buffer.insert('S');
    buffer.insert('E');
    buffer.insert('T');
    assert(buffer.text() == "SET");
    assert(buffer.cursor() == 3);

    assert(buffer.move_left());
    buffer.insert('X');
    assert(buffer.text() == "SEXT");
    assert(buffer.cursor() == 3);

    assert(buffer.erase_before_cursor());
    assert(buffer.text() == "SET");
    assert(buffer.cursor() == 2);

    assert(buffer.erase_at_cursor());
    assert(buffer.text() == "SE");
    assert(buffer.cursor() == 2);

    buffer.move_home();
    assert(buffer.cursor() == 0);
    assert(!buffer.move_left());
    buffer.insert('G');
    assert(buffer.text() == "GSE");
    assert(buffer.cursor() == 1);

    buffer.move_end();
    assert(buffer.cursor() == buffer.text().size());
    assert(!buffer.move_right());
    buffer.clear();
    assert(buffer.empty());

    buffer.set_text("PING one");
    assert(buffer.text() == "PING one");
    assert(buffer.cursor() == buffer.text().size());

    const std::vector<std::string> entries{"PING one", "SET name mini-kv", "GET name"};
    minikv::LineEditorHistoryNavigator navigator{entries};

    auto value = navigator.previous("draft command");
    assert(value == std::optional<std::string>{"GET name"});
    value = navigator.previous("ignored");
    assert(value == std::optional<std::string>{"SET name mini-kv"});
    value = navigator.previous("ignored");
    assert(value == std::optional<std::string>{"PING one"});
    value = navigator.previous("ignored");
    assert(value == std::optional<std::string>{"PING one"});

    value = navigator.next();
    assert(value == std::optional<std::string>{"SET name mini-kv"});
    value = navigator.next();
    assert(value == std::optional<std::string>{"GET name"});
    value = navigator.next();
    assert(value == std::optional<std::string>{"draft command"});
    value = navigator.next();
    assert(value == std::optional<std::string>{"draft command"});

    const std::vector<std::string> empty_entries;
    minikv::LineEditorHistoryNavigator empty_navigator{empty_entries};
    assert(!empty_navigator.previous("draft").has_value());
    assert(!empty_navigator.next().has_value());

    minikv::LineEditorCompletion completion{
        {"PING", "SET", "SETNXEX", "GET", "SAVE", "SIZE", "KEYS", "KEYSJSON", "COMPACT", "STATS", "STATSJSON",
         "RESETSTATS", "SMOKEJSON", "STORAGEJSON", "HELP", "HEALTH", "INFO", "INFOJSON", "SHARDJSON",
          "SHARDROUTE", "SHARDROUTEJSON", "SHARDROUTEVERIFYJSON", "SHARDROUTEVERIFYREPORTJSON",
          "SHARDROUTEVERIFYREPORTCLOSEOUTJSON", "SHARDROUTEVERIFYREPORTARCHIVEJSON",
          "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON", "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON",
          "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON",
          "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON",
          "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON", "SHARDROUTEARCHIVEMAINTJSON",
          "SHARDROUTEARCHIVEMAINTVERIFYJSON",
          "SHARDROUTEWORKSHEETVERIFYJSON",
          "SHARDROUTEIMPORTPREFLIGHTJSON",
          "SHARDROUTEVALUEDRAFTJSON",
          "SHARDROUTEVALUESUPPLYJSON",
          "SHARDROUTEVALUESUPPLYPRECHECKJSON",
          "SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON",
          "SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON",
          "SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON",
          "SHARDROUTECANDIDATEINTAKEPACKETJSON",
          "SHARDROUTECANDIDATEMATERIALREQUESTJSON",
          "SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON",
          "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON",
          "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON",
          "SHARDROUTECANDIDATEPROFILESECTIONJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON",
          "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON",
          "COMMANDS", "COMMANDSJSON", "EXPLAINJSON", "CHECKJSON",
         "EXIT", "EXPIRE", ":history"}};
    assert(completion.complete("", 0) == std::nullopt);
    assert(completion.complete("PI", 2) == std::optional<std::string>{"PING "});
    assert(completion.complete("  pi", 4) == std::optional<std::string>{"  PING "});
    assert(completion.complete(":h", 2) == std::optional<std::string>{":history "});
    assert(completion.complete("HEA", 3) == std::optional<std::string>{"HEALTH "});
    assert(completion.complete("INFOJ", 5) == std::optional<std::string>{"INFOJSON "});
    assert(completion.complete("INF", 3) == std::optional<std::string>{"INFO"});
    assert(completion.complete("COMM", 4) == std::optional<std::string>{"COMMANDS"});
    assert(completion.complete("COMMANDSJ", 9) == std::optional<std::string>{"COMMANDSJSON "});
    assert(!completion.complete("COM", 3).has_value());
    assert(completion.complete("KEYSJ", 5) == std::optional<std::string>{"KEYSJSON "});
    assert(completion.complete("K", 1) == std::optional<std::string>{"KEYS"});
    assert(completion.complete("E", 1) == std::optional<std::string>{"EX"});
    assert(completion.complete("SETN", 4) == std::optional<std::string>{"SETNXEX "});
    assert(completion.complete("EXPLAINJ", 8) == std::optional<std::string>{"EXPLAINJSON "});
    assert(completion.complete("CHECKJ", 6) == std::optional<std::string>{"CHECKJSON "});
    assert(completion.complete("R", 1) == std::optional<std::string>{"RESETSTATS "});
    assert(completion.complete("STAT", 4) == std::optional<std::string>{"STATS"});
    assert(!completion.complete("STATS", 5).has_value());
    assert(completion.complete("STATSJ", 6) == std::optional<std::string>{"STATSJSON "});
    assert(completion.complete("SMOKEJ", 6) == std::optional<std::string>{"SMOKEJSON "});
    assert(completion.complete("STOR", 4) == std::optional<std::string>{"STORAGEJSON "});
    assert(completion.complete("SHARDR", 6) == std::optional<std::string>{"SHARDROUTE"});
    assert(completion.complete("SHARDROUTEJ", 11) == std::optional<std::string>{"SHARDROUTEJSON "});
    assert(!completion.complete("SHARDROUTEV", 11).has_value());
    assert(completion.complete("SHARDROUTEVE", 12) == std::optional<std::string>{"SHARDROUTEVERIFY"});
    assert(completion.complete("SHARDROUTEVERIFYJ", 17) == std::optional<std::string>{"SHARDROUTEVERIFYJSON "});
    assert(completion.complete("SHARDROUTEVERIFYR", 17) == std::optional<std::string>{"SHARDROUTEVERIFYREPORT"});
    assert(completion.complete("SHARDROUTEVERIFYREPORTJ", 23) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTJSON "});
    assert(completion.complete("SHARDROUTEVERIFYREPORTC", 23) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTCLOSEOUTJSON "});
    assert(completion.complete("SHARDROUTEVERIFYREPORTA", 23) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVE"});
    assert(completion.complete("SHARDROUTEVERIFYREPORTARCHIVEJ", 30) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVEJSON "});
    const std::string archive_closeout_prefix = "SHARDROUTEVERIFYREPORTARCHIVEC";
    assert(completion.complete(archive_closeout_prefix, archive_closeout_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUT"});
    const std::string archive_closeout_json_prefix = "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJ";
    assert(completion.complete(archive_closeout_json_prefix, archive_closeout_json_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON "});
    const std::string archive_closeout_verify_prefix = "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTV";
    assert(completion.complete(archive_closeout_verify_prefix, archive_closeout_verify_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFY"});
    const std::string archive_closeout_verify_json_prefix = "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJ";
    assert(completion.complete(archive_closeout_verify_json_prefix, archive_closeout_verify_json_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON "});
    const std::string archive_closeout_verify_audit_prefix = "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYA";
    assert(completion.complete(archive_closeout_verify_audit_prefix, archive_closeout_verify_audit_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDIT"});
    const std::string archive_closeout_verify_audit_json_prefix = "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJ";
    assert(completion.complete(archive_closeout_verify_audit_json_prefix,
                               archive_closeout_verify_audit_json_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITJSON "});
    const std::string archive_closeout_verify_audit_closeout_prefix =
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITC";
    assert(completion.complete(archive_closeout_verify_audit_closeout_prefix,
                               archive_closeout_verify_audit_closeout_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUT"});
    const std::string archive_closeout_verify_audit_closeout_json_prefix =
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJ";
    assert(completion.complete(archive_closeout_verify_audit_closeout_json_prefix,
                               archive_closeout_verify_audit_closeout_json_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTJSON "});
    const std::string archive_closeout_verify_audit_closeout_archive_prefix =
        "SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTA";
    assert(completion.complete(archive_closeout_verify_audit_closeout_archive_prefix,
                               archive_closeout_verify_audit_closeout_archive_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON "});
    const std::string archive_maintenance_prefix = "SHARDROUTEARCHIVEMAINTJ";
    assert(completion.complete(archive_maintenance_prefix, archive_maintenance_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEARCHIVEMAINTJSON "});
    const std::string archive_maintenance_verify_prefix = "SHARDROUTEARCHIVEMAINTV";
    assert(completion.complete(archive_maintenance_verify_prefix, archive_maintenance_verify_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEARCHIVEMAINTVERIFYJSON "});
    const std::string worksheet_verify_prefix = "SHARDROUTEWORKSHEETV";
    assert(completion.complete(worksheet_verify_prefix, worksheet_verify_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEWORKSHEETVERIFYJSON "});
    const std::string import_preflight_prefix = "SHARDROUTEIMPORTP";
    assert(completion.complete(import_preflight_prefix, import_preflight_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEIMPORTPREFLIGHTJSON "});
    const std::string value_draft_prefix = "SHARDROUTEVALUED";
    assert(completion.complete(value_draft_prefix, value_draft_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUEDRAFTJSON "});
    const std::string value_supply_prefix = "SHARDROUTEVALUESUPPLYJ";
    assert(completion.complete(value_supply_prefix, value_supply_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYJSON "});
    const std::string value_supply_precheck_prefix = "SHARDROUTEVALUESUPPLYP";
    assert(completion.complete(value_supply_precheck_prefix, value_supply_precheck_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYPRECHECKJSON "});
    const std::string value_supply_approval_template_prefix = "SHARDROUTEVALUESUPPLYA";
    assert(completion.complete(value_supply_approval_template_prefix,
                               value_supply_approval_template_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON "});
    const std::string value_supply_signed_approval_template_prefix = "SHARDROUTEVALUESUPPLYSI";
    assert(completion.complete(value_supply_signed_approval_template_prefix,
                               value_supply_signed_approval_template_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVAL"});
    const std::string value_supply_signed_approval_template_exact_prefix = "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALT";
    assert(completion.complete(value_supply_signed_approval_template_exact_prefix,
                               value_supply_signed_approval_template_exact_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON "});
    const std::string value_supply_signed_approval_capture_prefix = "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALC";
    assert(completion.complete(value_supply_signed_approval_capture_prefix,
                               value_supply_signed_approval_capture_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTURE"});
    const std::string value_supply_signed_approval_capture_preflight_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREP";
    assert(completion.complete(value_supply_signed_approval_capture_preflight_prefix,
                               value_supply_signed_approval_capture_preflight_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON "});
    const std::string value_supply_signed_approval_capture_artifact_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREA";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_prefix,
                               value_supply_signed_approval_capture_artifact_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACT"});
    const std::string value_supply_signed_approval_capture_artifact_preflight_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTP";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_preflight_prefix,
                               value_supply_signed_approval_capture_artifact_preflight_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON "});
    const std::string value_supply_signed_approval_capture_artifact_draft_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTD";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_draft_prefix,
                               value_supply_signed_approval_capture_artifact_draft_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFT"});
    const std::string value_supply_signed_approval_capture_artifact_authoring_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTA";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_authoring_prefix,
                               value_supply_signed_approval_capture_artifact_authoring_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON "});
    const std::string value_supply_signed_approval_capture_artifact_instruction_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTI";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_instruction_prefix,
                               value_supply_signed_approval_capture_artifact_instruction_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON "});
    const std::string value_supply_signed_approval_capture_artifact_text_package_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTT";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_text_package_prefix,
                               value_supply_signed_approval_capture_artifact_text_package_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGE"});
    const std::string value_supply_signed_approval_capture_artifact_text_package_closeout_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWC";
    assert(completion.complete(value_supply_signed_approval_capture_artifact_text_package_closeout_prefix,
                               value_supply_signed_approval_capture_artifact_text_package_closeout_prefix.size()) ==
           std::optional<std::string>{
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON "});
    const std::string candidate_request_package_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATE";
    assert(completion.complete(candidate_request_package_prefix,
                               candidate_request_package_prefix.size()) ==
           std::optional<std::string>{
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON "});
    const std::string candidate_request_package_integrity_prefix = "SHARDROUTECANDIDATER";
    assert(completion.complete(candidate_request_package_integrity_prefix,
                               candidate_request_package_integrity_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEREQUESTPACKAGEINTEGRITYJSON "});
    const std::string candidate_submission_precheck_prefix = "SHARDROUTECANDIDATES";
    assert(completion.complete(candidate_submission_precheck_prefix,
                               candidate_submission_precheck_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON "});
    const std::string candidate_intake_packet_prefix = "SHARDROUTECANDIDATEI";
    assert(completion.complete(candidate_intake_packet_prefix,
                               candidate_intake_packet_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEINTAKEPACKETJSON "});
    const std::string candidate_material_request_prefix = "SHARDROUTECANDIDATEM";
    assert(completion.complete(candidate_material_request_prefix,
                               candidate_material_request_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIAL"});
    const std::string candidate_material_request_json_prefix = "SHARDROUTECANDIDATEMATERIALR";
    assert(completion.complete(candidate_material_request_json_prefix,
                               candidate_material_request_json_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIALREQUEST"});
    const std::string candidate_material_request_plain_prefix = "SHARDROUTECANDIDATEMATERIALREQUESTJ";
    assert(completion.complete(candidate_material_request_plain_prefix,
                               candidate_material_request_plain_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIALREQUESTJSON "});
    const std::string candidate_material_request_integrity_prefix = "SHARDROUTECANDIDATEMATERIALREQUESTI";
    assert(completion.complete(candidate_material_request_integrity_prefix,
                               candidate_material_request_integrity_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON "});
    const std::string candidate_material_submission_precheck_prefix = "SHARDROUTECANDIDATEMATERIALS";
    assert(completion.complete(candidate_material_submission_precheck_prefix,
                               candidate_material_submission_precheck_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECK"});
    const std::string candidate_material_submission_precheck_plain_prefix =
        "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJ";
    assert(completion.complete(candidate_material_submission_precheck_plain_prefix,
                               candidate_material_submission_precheck_plain_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON "});
    const std::string candidate_material_submission_precheck_integrity_prefix =
        "SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKI";
    assert(completion.complete(candidate_material_submission_precheck_integrity_prefix,
                               candidate_material_submission_precheck_integrity_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON "});
    const std::string candidate_profile_section_prefix = "SHARDROUTECANDIDATEP";
    assert(completion.complete(candidate_profile_section_prefix,
                               candidate_profile_section_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTECANDIDATEPROFILESECTIONJSON "});
    const std::string signed_approval_draft_profile_section_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTP";
    assert(completion.complete(signed_approval_draft_profile_section_prefix,
                               signed_approval_draft_profile_section_prefix.size()) ==
           std::optional<std::string>{"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTION"});
    const std::string signed_approval_draft_profile_section_integrity_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONI";
    assert(completion.complete(signed_approval_draft_profile_section_integrity_prefix,
                               signed_approval_draft_profile_section_integrity_prefix.size()) ==
           std::optional<std::string>{
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON "});
    const std::string signed_approval_draft_text_package_profile_section_prefix =
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEP";
    assert(completion.complete(signed_approval_draft_text_package_profile_section_prefix,
                               signed_approval_draft_text_package_profile_section_prefix.size()) ==
           std::optional<std::string>{
               "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON "});
    assert(!completion.complete("S", 1).has_value());
    assert(!completion.complete("BAD", 3).has_value());
    assert(!completion.complete("PING name", 6).has_value());
    assert(!completion.complete("PING", 2).has_value());

    minikv::LineEditorCompletionOptions contextual_options;
    contextual_options.command_candidates =
        {"PING", "SET", "SETNXEX", "GET", "DEL", "EXPIRE", "TTL", "KEYS", "STATS", "STATSJSON", "RESETSTATS"};
    contextual_options.key_candidates = {"alpha", "alpine", "name", "user:1"};
    minikv::LineEditorCompletion contextual_completion{contextual_options};
    assert(contextual_completion.complete("GET na", 6) == std::optional<std::string>{"GET name "});
    assert(contextual_completion.complete("GET al", 6) == std::optional<std::string>{"GET alp"});
    assert(contextual_completion.complete("SET na", 6) == std::optional<std::string>{"SET name "});
    assert(contextual_completion.complete("SETNXEX na", 10) == std::optional<std::string>{"SETNXEX name "});
    assert(contextual_completion.complete("EXPIRE na", 9) == std::optional<std::string>{"EXPIRE name "});
    assert(contextual_completion.complete("TTL user:", 9) == std::optional<std::string>{"TTL user:1 "});
    assert(!contextual_completion.complete("GET AL", 6).has_value());
    assert(!contextual_completion.complete("PING na", 7).has_value());
    assert(!contextual_completion.complete("GET name now", 8).has_value());
    assert(!contextual_completion.complete("GET name", 5).has_value());

    return 0;
}
