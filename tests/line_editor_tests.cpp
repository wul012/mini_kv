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
        {"PING", "SET", "GET", "SAVE", "SIZE", "STATS", "HELP", "HEALTH", "EXIT", "EXPIRE", ":history"}};
    assert(completion.complete("", 0) == std::nullopt);
    assert(completion.complete("PI", 2) == std::optional<std::string>{"PING "});
    assert(completion.complete("  pi", 4) == std::optional<std::string>{"  PING "});
    assert(completion.complete(":h", 2) == std::optional<std::string>{":history "});
    assert(completion.complete("HEA", 3) == std::optional<std::string>{"HEALTH "});
    assert(completion.complete("E", 1) == std::optional<std::string>{"EX"});
    assert(!completion.complete("S", 1).has_value());
    assert(!completion.complete("BAD", 3).has_value());
    assert(!completion.complete("PING name", 6).has_value());
    assert(!completion.complete("PING", 2).has_value());

    return 0;
}
