#include "minikv/command_catalog.hpp"
#include "minikv/command_contracts.hpp"
#include "minikv/line_editor.hpp"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

namespace {

bool contains(const std::vector<std::string>& values, const std::string& value) {
    return std::find(values.begin(), values.end(), value) != values.end();
}

void assert_contains(const std::string& haystack, const std::string& needle) {
    assert(haystack.find(needle) != std::string::npos);
}

} // namespace

int main() {
    using minikv::command_catalog::CommandDispatchVerb;

    const auto& entries = minikv::command_catalog::entries();
    assert(entries.size() == 91);
    assert(minikv::command_catalog::count() == 91);

    const auto* set = minikv::command_catalog::find("SET");
    assert(set != nullptr);
    assert(set->usage == "SET key value");
    assert(set->category == "write");
    assert(set->mutates_store);
    assert(set->touches_wal);
    assert(set->key_completion);

    const auto* precheck =
        minikv::command_catalog::find("SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON");
    assert(precheck != nullptr);
    assert(precheck->category == "read");
    assert(!precheck->mutates_store);
    assert(!precheck->touches_wal);
    assert(precheck->dispatch_verb == CommandDispatchVerb::RuntimeEvidence);
    assert(!precheck->key_completion);

    const auto* runtime_archive = minikv::command_catalog::find(
        "SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON");
    assert(runtime_archive != nullptr);
    assert(runtime_archive->category == "read");
    assert(!runtime_archive->mutates_store);
    assert(!runtime_archive->touches_wal);
    assert(runtime_archive->dispatch_verb == CommandDispatchVerb::RuntimeEvidence);
    assert(!runtime_archive->key_completion);

    const auto* code_walkthrough_gate =
        minikv::command_catalog::find("SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON");
    assert(code_walkthrough_gate != nullptr);
    assert(code_walkthrough_gate->category == "read");
    assert(!code_walkthrough_gate->mutates_store);
    assert(!code_walkthrough_gate->touches_wal);
    assert(code_walkthrough_gate->dispatch_verb == CommandDispatchVerb::RuntimeEvidence);
    assert(!code_walkthrough_gate->key_completion);

    const auto* owner_receipt_packet =
        minikv::command_catalog::find("SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON");
    assert(owner_receipt_packet != nullptr);
    assert(owner_receipt_packet->category == "read");
    assert(!owner_receipt_packet->mutates_store);
    assert(!owner_receipt_packet->touches_wal);
    assert(owner_receipt_packet->dispatch_verb == CommandDispatchVerb::RuntimeEvidence);
    assert(!owner_receipt_packet->key_completion);

    assert(minikv::command_catalog::lookup_dispatch_verb("SET") == CommandDispatchVerb::Set);
    assert(minikv::command_catalog::lookup_dispatch_verb("COMMANDSJSON") == CommandDispatchVerb::RuntimeEvidence);
    assert(minikv::command_catalog::lookup_dispatch_verb("EXPLAINJSON") == CommandDispatchVerb::ExplainJson);
    assert(minikv::command_catalog::lookup_dispatch_verb("CHECKJSON") == CommandDispatchVerb::CheckJson);
    assert(minikv::command_catalog::lookup_dispatch_verb("HELP") == CommandDispatchVerb::Help);
    assert(minikv::command_catalog::lookup_dispatch_verb("QUIT") == CommandDispatchVerb::Quit);
    assert(minikv::command_catalog::lookup_dispatch_verb("NOT_A_COMMAND") == CommandDispatchVerb::Unknown);

    assert(minikv::command_catalog::is_key_completion_command("SET"));
    assert(minikv::command_catalog::is_key_completion_command("SETNXEX"));
    assert(minikv::command_catalog::is_key_completion_command("GET"));
    assert(minikv::command_catalog::is_key_completion_command("DEL"));
    assert(minikv::command_catalog::is_key_completion_command("EXPIRE"));
    assert(minikv::command_catalog::is_key_completion_command("TTL"));
    assert(!minikv::command_catalog::is_key_completion_command("KEYS"));
    assert(!minikv::command_catalog::is_key_completion_command("SHARDROUTE"));

    const std::string help = minikv::command_catalog::help_text();
    assert(help.rfind("Commands:\n  PING [message]", 0) == 0);
    assert_contains(help, "\n  SETNXEX key seconds value\n");
    assert_contains(help, "\n  SHARDROUTEVERIFYREPORTJSON key\n");
    assert_contains(help, "\n  STORAGEJSON\n  HELP\n  EXIT\n  QUIT");

    const auto names = minikv::command_catalog::command_names();
    assert(names.size() == entries.size());
    for (const auto& entry : entries) {
        assert(contains(names, std::string{entry.name}));
    }

    const auto completion_options = minikv::default_client_completion_options();
    assert(completion_options.command_candidates.size() == entries.size() + 1);
    assert(completion_options.command_candidates.back() == ":history");
    for (const auto& entry : entries) {
        assert(contains(completion_options.command_candidates, std::string{entry.name}));
    }

    assert(minikv::command_contracts::find_command_catalog_entry("SET") == set);
    assert(minikv::command_contracts::is_known_command("STORAGEJSON"));
    assert(!minikv::command_contracts::is_known_command("NOT_A_COMMAND"));

    const std::string commands = minikv::command_contracts::format_commands();
    assert_contains(commands, "command_count=91");
    assert_contains(commands, "SET(category=write,mutates_store=yes,touches_wal=yes,stable=yes)");
    assert_contains(
        commands,
        "SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        commands,
        "SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        commands,
        "SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");
    assert_contains(
        commands,
        "SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON(category=read,mutates_store=no,touches_wal=no,stable=yes)");

    const std::string commands_json = minikv::command_contracts::format_commands_json();
    assert_contains(commands_json, "\"name\":\"STORAGEJSON\",\"category\":\"read\"");
    assert_contains(commands_json,
                    "\"name\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\",\"category\":\"read\"");
    assert_contains(
        commands_json,
        "\"name\":\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\",\"category\":\"read\"");
    assert_contains(commands_json,
                    "\"name\":\"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON\",\"category\":\"read\"");
    assert_contains(
        commands_json,
        "\"name\":\"SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON\",\"category\":\"read\"");

    return 0;
}
