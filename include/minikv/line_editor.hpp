#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace minikv {

class LineEditorBuffer {
public:
    void clear();
    void set_text(std::string_view text);
    const std::string& text() const;
    std::size_t cursor() const;
    bool empty() const;

    void insert(char ch);
    bool erase_before_cursor();
    bool erase_at_cursor();
    bool move_left();
    bool move_right();
    void move_home();
    void move_end();

private:
    std::string text_;
    std::size_t cursor_ = 0;
};

class LineEditorHistoryNavigator {
public:
    explicit LineEditorHistoryNavigator(const std::vector<std::string>& entries);

    std::optional<std::string> previous(std::string_view current_text);
    std::optional<std::string> next();

private:
    const std::vector<std::string>& entries_;
    std::size_t index_;
    std::string draft_;
    bool draft_saved_ = false;
};

bool read_client_line(std::string_view prompt,
                      const std::vector<std::string>& history_entries,
                      std::string& line);

} // namespace minikv
