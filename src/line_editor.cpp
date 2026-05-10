#include "minikv/line_editor.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace minikv {

namespace {

std::string to_upper_copy(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

} // namespace

void LineEditorBuffer::clear() {
    text_.clear();
    cursor_ = 0;
}

void LineEditorBuffer::set_text(std::string_view text) {
    text_ = std::string{text};
    cursor_ = text_.size();
}

const std::string& LineEditorBuffer::text() const {
    return text_;
}

std::size_t LineEditorBuffer::cursor() const {
    return cursor_;
}

bool LineEditorBuffer::empty() const {
    return text_.empty();
}

void LineEditorBuffer::insert(char ch) {
    text_.insert(text_.begin() + static_cast<std::string::difference_type>(cursor_), ch);
    ++cursor_;
}

bool LineEditorBuffer::erase_before_cursor() {
    if (cursor_ == 0) {
        return false;
    }

    text_.erase(text_.begin() + static_cast<std::string::difference_type>(cursor_ - 1));
    --cursor_;
    return true;
}

bool LineEditorBuffer::erase_at_cursor() {
    if (cursor_ >= text_.size()) {
        return false;
    }

    text_.erase(text_.begin() + static_cast<std::string::difference_type>(cursor_));
    return true;
}

bool LineEditorBuffer::move_left() {
    if (cursor_ == 0) {
        return false;
    }

    --cursor_;
    return true;
}

bool LineEditorBuffer::move_right() {
    if (cursor_ >= text_.size()) {
        return false;
    }

    ++cursor_;
    return true;
}

void LineEditorBuffer::move_home() {
    cursor_ = 0;
}

void LineEditorBuffer::move_end() {
    cursor_ = text_.size();
}

LineEditorHistoryNavigator::LineEditorHistoryNavigator(const std::vector<std::string>& entries)
    : entries_(entries),
      index_(entries.size()) {}

std::optional<std::string> LineEditorHistoryNavigator::previous(std::string_view current_text) {
    if (entries_.empty()) {
        return std::nullopt;
    }

    if (!draft_saved_) {
        draft_ = std::string{current_text};
        draft_saved_ = true;
        index_ = entries_.size();
    }

    if (index_ > 0) {
        --index_;
    }

    return entries_[index_];
}

std::optional<std::string> LineEditorHistoryNavigator::next() {
    if (entries_.empty() || !draft_saved_) {
        return std::nullopt;
    }

    if (index_ < entries_.size()) {
        ++index_;
    }

    if (index_ >= entries_.size()) {
        return draft_;
    }

    return entries_[index_];
}

LineEditorCompletion::LineEditorCompletion(std::vector<std::string> candidates)
    : candidates_(std::move(candidates)) {}

std::optional<std::string> LineEditorCompletion::complete(std::string_view text, std::size_t cursor) const {
    if (cursor > text.size() || candidates_.empty()) {
        return std::nullopt;
    }

    std::size_t token_start = 0;
    while (token_start < text.size() && std::isspace(static_cast<unsigned char>(text[token_start])) != 0) {
        ++token_start;
    }

    if (cursor < token_start) {
        return std::nullopt;
    }

    std::size_t token_end = token_start;
    while (token_end < text.size() && std::isspace(static_cast<unsigned char>(text[token_end])) == 0) {
        ++token_end;
    }

    if (cursor != token_end) {
        return std::nullopt;
    }

    const std::string prefix = to_upper_copy(text.substr(token_start, cursor - token_start));
    std::vector<std::string_view> matches;
    for (const auto& candidate : candidates_) {
        const std::string upper_candidate = to_upper_copy(candidate);
        if (upper_candidate.rfind(prefix, 0) == 0) {
            matches.push_back(candidate);
        }
    }

    if (matches.empty()) {
        return std::nullopt;
    }

    std::string replacement = std::string{matches.front()};
    if (matches.size() > 1) {
        std::string common_prefix = to_upper_copy(matches.front());
        for (std::size_t index = 1; index < matches.size(); ++index) {
            const std::string next = to_upper_copy(matches[index]);
            std::size_t length = 0;
            while (length < common_prefix.size() && length < next.size() && common_prefix[length] == next[length]) {
                ++length;
            }
            common_prefix.resize(length);
        }

        if (common_prefix.size() <= prefix.size()) {
            return std::nullopt;
        }
        replacement = common_prefix;
    }

    std::string completed{text.substr(0, token_start)};
    completed += replacement;
    const bool has_trailing_text = cursor < text.size();
    completed += text.substr(cursor);
    if (matches.size() == 1 && !has_trailing_text && (completed.empty() || completed.back() != ' ')) {
        completed.push_back(' ');
    }

    if (completed == text) {
        return std::nullopt;
    }
    return completed;
}

namespace {

std::vector<std::string> default_completion_candidates() {
    return {"PING",    "SET",   "GET",    "DEL",   "EXPIRE", "TTL",  "SIZE", "SAVE", "LOAD",
            "COMPACT", "WALINFO", "STATS", "STATSJSON", "RESETSTATS", "HEALTH", "HELP", "EXIT",
            "QUIT",    ":history"};
}

enum class KeyKind {
    character,
    tab,
    enter,
    backspace,
    delete_forward,
    left,
    right,
    up,
    down,
    home,
    end,
    eof,
    unknown
};

struct KeyEvent {
    KeyKind kind = KeyKind::unknown;
    char character = '\0';
};

bool terminal_input_available() {
#ifdef _WIN32
    const HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    DWORD output_mode = 0;
    return input != INVALID_HANDLE_VALUE && output != INVALID_HANDLE_VALUE &&
           GetConsoleMode(input, &mode) != 0 && GetConsoleMode(output, &output_mode) != 0;
#else
    return isatty(STDIN_FILENO) != 0 && isatty(STDOUT_FILENO) != 0;
#endif
}

#ifdef _WIN32
void enable_virtual_terminal_output() {
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (output != INVALID_HANDLE_VALUE && GetConsoleMode(output, &mode) != 0) {
        SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}
#endif

#ifndef _WIN32
class RawTerminalMode {
public:
    RawTerminalMode() {
        if (tcgetattr(STDIN_FILENO, &original_) != 0) {
            return;
        }

        termios raw = original_;
        raw.c_lflag &= ~(static_cast<tcflag_t>(ICANON) | static_cast<tcflag_t>(ECHO) |
                         static_cast<tcflag_t>(IEXTEN));
        raw.c_iflag &= ~(static_cast<tcflag_t>(IXON) | static_cast<tcflag_t>(ICRNL));
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == 0) {
            enabled_ = true;
        }
    }

    ~RawTerminalMode() {
        if (enabled_) {
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_);
        }
    }

    bool enabled() const {
        return enabled_;
    }

private:
    termios original_{};
    bool enabled_ = false;
};
#endif

void redraw_line(std::string_view prompt, const LineEditorBuffer& buffer) {
    std::cout << '\r' << "\x1b[2K" << prompt << buffer.text();
    const std::size_t trailing = buffer.text().size() - buffer.cursor();
    if (trailing > 0) {
        std::cout << "\x1b[" << trailing << 'D';
    }
    std::cout.flush();
}

bool fallback_read_line(std::string_view prompt, std::string& line) {
    std::cout << prompt;
    std::cout.flush();
    return static_cast<bool>(std::getline(std::cin, line));
}

#ifdef _WIN32
KeyEvent read_key() {
    const int ch = _getch();
    if (ch == 3 || ch == 26) {
        return {KeyKind::eof, '\0'};
    }
    if (ch == '\r' || ch == '\n') {
        return {KeyKind::enter, '\0'};
    }
    if (ch == '\b') {
        return {KeyKind::backspace, '\0'};
    }
    if (ch == '\t') {
        return {KeyKind::tab, '\0'};
    }

    if (ch == 0 || ch == 224) {
        const int code = _getch();
        switch (code) {
        case 71:
            return {KeyKind::home, '\0'};
        case 72:
            return {KeyKind::up, '\0'};
        case 75:
            return {KeyKind::left, '\0'};
        case 77:
            return {KeyKind::right, '\0'};
        case 79:
            return {KeyKind::end, '\0'};
        case 80:
            return {KeyKind::down, '\0'};
        case 83:
            return {KeyKind::delete_forward, '\0'};
        default:
            return {KeyKind::unknown, '\0'};
        }
    }

    if (ch >= 32 && ch <= 126) {
        return {KeyKind::character, static_cast<char>(ch)};
    }
    return {KeyKind::unknown, '\0'};
}
#else
bool read_byte(char& ch) {
    return read(STDIN_FILENO, &ch, 1) == 1;
}

KeyEvent read_escape_sequence() {
    char first = '\0';
    if (!read_byte(first) || first != '[') {
        return {KeyKind::unknown, '\0'};
    }

    char second = '\0';
    if (!read_byte(second)) {
        return {KeyKind::unknown, '\0'};
    }

    switch (second) {
    case 'A':
        return {KeyKind::up, '\0'};
    case 'B':
        return {KeyKind::down, '\0'};
    case 'C':
        return {KeyKind::right, '\0'};
    case 'D':
        return {KeyKind::left, '\0'};
    case 'F':
        return {KeyKind::end, '\0'};
    case 'H':
        return {KeyKind::home, '\0'};
    case '1':
    case '7': {
        char tilde = '\0';
        if (read_byte(tilde) && tilde == '~') {
            return {KeyKind::home, '\0'};
        }
        break;
    }
    case '3': {
        char tilde = '\0';
        if (read_byte(tilde) && tilde == '~') {
            return {KeyKind::delete_forward, '\0'};
        }
        break;
    }
    case '4':
    case '8': {
        char tilde = '\0';
        if (read_byte(tilde) && tilde == '~') {
            return {KeyKind::end, '\0'};
        }
        break;
    }
    default:
        break;
    }

    return {KeyKind::unknown, '\0'};
}

KeyEvent read_key() {
    char ch = '\0';
    if (!read_byte(ch)) {
        return {KeyKind::eof, '\0'};
    }

    if (ch == '\n' || ch == '\r') {
        return {KeyKind::enter, '\0'};
    }
    if (ch == 4) {
        return {KeyKind::eof, '\0'};
    }
    if (ch == 8 || ch == 127) {
        return {KeyKind::backspace, '\0'};
    }
    if (ch == '\t') {
        return {KeyKind::tab, '\0'};
    }
    if (ch == '\x1b') {
        return read_escape_sequence();
    }
    if (ch >= 32 && ch <= 126) {
        return {KeyKind::character, ch};
    }
    return {KeyKind::unknown, '\0'};
}
#endif

} // namespace

bool read_client_line(std::string_view prompt,
                      const std::vector<std::string>& history_entries,
                      std::string& line) {
    return read_client_line(prompt, history_entries, default_completion_candidates(), line);
}

bool read_client_line(std::string_view prompt,
                      const std::vector<std::string>& history_entries,
                      const std::vector<std::string>& completion_candidates,
                      std::string& line) {
    line.clear();

    if (!terminal_input_available()) {
        return fallback_read_line(prompt, line);
    }

#ifndef _WIN32
    RawTerminalMode raw_mode;
    if (!raw_mode.enabled()) {
        return fallback_read_line(prompt, line);
    }
#endif

#ifdef _WIN32
    enable_virtual_terminal_output();
#endif

    LineEditorBuffer buffer;
    LineEditorHistoryNavigator navigator{history_entries};
    LineEditorCompletion completion{completion_candidates};
    redraw_line(prompt, buffer);

    while (true) {
        const KeyEvent key = read_key();
        bool changed = false;

        switch (key.kind) {
        case KeyKind::character:
            buffer.insert(key.character);
            changed = true;
            break;
        case KeyKind::tab:
            if (const auto completed = completion.complete(buffer.text(), buffer.cursor())) {
                buffer.set_text(*completed);
                changed = true;
            }
            break;
        case KeyKind::enter:
            line = buffer.text();
            std::cout << '\n';
            return true;
        case KeyKind::backspace:
            changed = buffer.erase_before_cursor();
            break;
        case KeyKind::delete_forward:
            changed = buffer.erase_at_cursor();
            break;
        case KeyKind::left:
            changed = buffer.move_left();
            break;
        case KeyKind::right:
            changed = buffer.move_right();
            break;
        case KeyKind::home:
            buffer.move_home();
            changed = true;
            break;
        case KeyKind::end:
            buffer.move_end();
            changed = true;
            break;
        case KeyKind::up:
            if (const auto previous = navigator.previous(buffer.text())) {
                buffer.set_text(*previous);
                changed = true;
            }
            break;
        case KeyKind::down:
            if (const auto next = navigator.next()) {
                buffer.set_text(*next);
                changed = true;
            }
            break;
        case KeyKind::eof:
            if (buffer.empty()) {
                return false;
            }
            line = buffer.text();
            std::cout << '\n';
            return true;
        case KeyKind::unknown:
            break;
        }

        if (changed) {
            redraw_line(prompt, buffer);
        }
    }
}

} // namespace minikv
