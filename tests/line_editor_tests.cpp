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
         "SHARDROUTEVERIFYREPORTCLOSEOUTJSON", "SHARDROUTEVERIFYREPORTARCHIVEJSON", "COMMANDS",
         "COMMANDSJSON", "EXPLAINJSON", "CHECKJSON", "EXIT", "EXPIRE", ":history"}};
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
    assert(completion.complete("SHARDROUTEV", 11) == std::optional<std::string>{"SHARDROUTEVERIFY"});
    assert(completion.complete("SHARDROUTEVERIFYJ", 17) == std::optional<std::string>{"SHARDROUTEVERIFYJSON "});
    assert(completion.complete("SHARDROUTEVERIFYR", 17) == std::optional<std::string>{"SHARDROUTEVERIFYREPORT"});
    assert(completion.complete("SHARDROUTEVERIFYREPORTJ", 23) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTJSON "});
    assert(completion.complete("SHARDROUTEVERIFYREPORTC", 23) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTCLOSEOUTJSON "});
    assert(completion.complete("SHARDROUTEVERIFYREPORTA", 23) ==
           std::optional<std::string>{"SHARDROUTEVERIFYREPORTARCHIVEJSON "});
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
