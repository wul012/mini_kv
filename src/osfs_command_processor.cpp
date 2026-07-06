#include "minikv/osfs/command_processor.hpp"

#include <cctype>
#include <iomanip>
#include <sstream>

namespace minikv::osfs {

namespace {

std::string trim(std::string_view input) {
    const auto begin = input.find_first_not_of(" \t\r\n");
    if (begin == std::string_view::npos) {
        return {};
    }
    const auto end = input.find_last_not_of(" \t\r\n");
    return std::string{input.substr(begin, end - begin + 1)};
}

std::string upper(std::string value) {
    for (auto& ch : value) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return value;
}

std::string require_error(const std::string& error) { return "ERR " + error; }

bool parse_fd(const std::string& text, int* fd) {
    try {
        std::size_t parsed = 0;
        const auto value = std::stoi(text, &parsed);
        if (parsed != text.size() || value < 0) {
            return false;
        }
        *fd = value;
        return true;
    } catch (...) {
        return false;
    }
}

bool parse_octal_mode(const std::string& text, std::uint16_t* mode) {
    if (text.empty() || text.size() > 4) {
        return false;
    }
    std::uint16_t value = 0;
    for (const auto ch : text) {
        if (ch < '0' || ch > '7') {
            return false;
        }
        value = static_cast<std::uint16_t>((value << 3) + (ch - '0'));
    }
    *mode = static_cast<std::uint16_t>(value & 0777);
    return true;
}

} // namespace

CommandProcessor::CommandProcessor(FileSystem& fs) : fs_{fs} {}

std::string CommandProcessor::help_text() {
    return "Commands: HELP, WHOAMI, LOGIN user password, DIR [user], CREATE name, DELETE name, STAT name, "
           "CHMOD name mode, OPEN name r|w|rw, READ fd, WRITE fd text, CLOSE fd, QUIT";
}

std::string CommandProcessor::execute(std::string_view line) {
    const auto trimmed = trim(line);
    if (trimmed.empty()) {
        return {};
    }

    std::istringstream input{trimmed};
    std::string command;
    input >> command;
    command = upper(command);

    if (command == "HELP") {
        return help_text();
    }
    if (command == "QUIT" || command == "EXIT") {
        return "BYE";
    }
    if (command == "WHOAMI") {
        if (!authenticated_) {
            return "not logged in";
        }
        return current_user_ + " uid=" + std::to_string(current_uid_);
    }
    if (command == "LOGIN") {
        std::string user;
        std::string password;
        input >> user >> password;
        if (user.empty() || password.empty()) {
            return "ERR usage: LOGIN user password";
        }
        return login(user, password);
    }
    if (!authenticated_) {
        return "ERR login required";
    }
    if (command == "DIR") {
        std::string username;
        input >> username;
        if (username.empty()) {
            username = current_user_;
        }
        std::string error;
        const auto entries_result = fs_.list_directory_for(username, current_uid_, &error);
        if (!entries_result.has_value()) {
            return require_error(error);
        }
        const auto& entries = *entries_result;
        if (entries.empty()) {
            return "(empty)";
        }
        std::ostringstream out;
        out << "name inode physical mode owner size\n";
        for (const auto& entry : entries) {
            out << entry.name << ' ' << entry.inode << ' ' << entry.first_block << ' ' << format_mode(entry.mode) << ' '
                << entry.owner_uid << ' ' << entry.size << '\n';
        }
        auto value = out.str();
        if (!value.empty() && value.back() == '\n') {
            value.pop_back();
        }
        return value;
    }
    if (command == "CREATE") {
        std::string name;
        input >> name;
        if (name.empty()) {
            return "ERR usage: CREATE name";
        }
        std::string error;
        if (!fs_.create_file(name, current_uid_, &error)) {
            return require_error(error);
        }
        return "OK created " + name;
    }
    if (command == "DELETE") {
        std::string name;
        input >> name;
        if (name.empty()) {
            return "ERR usage: DELETE name";
        }
        std::string error;
        if (!fs_.delete_file(name, current_uid_, &error)) {
            return require_error(error);
        }
        return "OK deleted " + name;
    }
    if (command == "STAT") {
        std::string name;
        input >> name;
        if (name.empty()) {
            return "ERR usage: STAT name";
        }
        const auto stat = fs_.stat(name, current_uid_);
        if (!stat.has_value()) {
            return "ERR file not found";
        }
        std::ostringstream out;
        out << "name=" << stat->name << " inode=" << stat->inode << " physical=" << stat->first_block
            << " mode=" << format_mode(stat->mode) << " owner=" << stat->owner_uid << " group=" << stat->owner_gid
            << " size=" << stat->size << " created_at=" << stat->created_at << " accessed_at=" << stat->accessed_at
            << " modified_at=" << stat->modified_at;
        return out.str();
    }
    if (command == "CHMOD") {
        std::string name;
        std::string mode_text;
        input >> name >> mode_text;
        std::uint16_t mode = 0;
        if (name.empty() || !parse_octal_mode(mode_text, &mode)) {
            return "ERR usage: CHMOD name octal_mode";
        }
        std::string error;
        if (!fs_.chmod(name, mode, current_uid_, &error)) {
            return require_error(error);
        }
        return "OK chmod " + name + " " + format_mode(mode);
    }
    if (command == "OPEN") {
        std::string name;
        std::string mode;
        input >> name >> mode;
        if (name.empty() || mode.empty()) {
            return "ERR usage: OPEN name r|w|rw";
        }
        return open_file(name, mode);
    }
    if (command == "CLOSE") {
        std::string fd_text;
        input >> fd_text;
        int fd = 0;
        if (!parse_fd(fd_text, &fd)) {
            return "ERR usage: CLOSE fd";
        }
        return close_file(fd);
    }
    if (command == "READ") {
        std::string fd_text;
        input >> fd_text;
        int fd = 0;
        if (!parse_fd(fd_text, &fd)) {
            return "ERR usage: READ fd";
        }
        return read_file(fd);
    }
    if (command == "WRITE") {
        std::string fd_text;
        input >> fd_text;
        int fd = 0;
        if (!parse_fd(fd_text, &fd)) {
            return "ERR usage: WRITE fd text";
        }
        std::string contents;
        std::getline(input, contents);
        contents = trim(contents);
        return write_file(fd, contents);
    }

    return "ERR unknown command";
}

std::string CommandProcessor::login(const std::string& user, const std::string& password) {
    const auto authenticated = fs_.authenticate(user, password);
    if (!authenticated.has_value()) {
        return "ERR authentication failed";
    }
    current_user_ = authenticated->username;
    current_uid_ = authenticated->uid;
    authenticated_ = true;
    handles_.clear();
    next_fd_ = 3;
    return "OK login " + current_user_ + " uid=" + std::to_string(current_uid_);
}

std::string CommandProcessor::open_file(const std::string& name, const std::string& mode) {
    const bool readable = mode == "r" || mode == "rw";
    const bool writable = mode == "w" || mode == "rw";
    if (!readable && !writable) {
        return "ERR usage: OPEN name r|w|rw";
    }
    if (!fs_.stat(name, current_uid_).has_value()) {
        return "ERR file not found";
    }
    if (readable && !fs_.can_read(name, current_uid_)) {
        return "ERR permission denied";
    }
    if (writable && !fs_.can_write(name, current_uid_)) {
        return "ERR permission denied";
    }
    const auto fd = next_fd_++;
    handles_.emplace(fd, Handle{name, readable, writable});
    return "OK fd=" + std::to_string(fd);
}

std::string CommandProcessor::close_file(int fd) {
    if (handles_.erase(fd) == 0) {
        return "ERR bad fd";
    }
    return "OK closed " + std::to_string(fd);
}

std::string CommandProcessor::read_file(int fd) {
    const auto handle = handles_.find(fd);
    if (handle == handles_.end()) {
        return "ERR bad fd";
    }
    if (!handle->second.readable) {
        return "ERR fd not readable";
    }
    std::string error;
    const auto contents = fs_.read_file(handle->second.name, current_uid_, &error);
    if (!contents.has_value()) {
        return require_error(error);
    }
    return *contents;
}

std::string CommandProcessor::write_file(int fd, const std::string& contents) {
    const auto handle = handles_.find(fd);
    if (handle == handles_.end()) {
        return "ERR bad fd";
    }
    if (!handle->second.writable) {
        return "ERR fd not writable";
    }
    std::string error;
    if (!fs_.write_file(handle->second.name, contents, current_uid_, &error)) {
        return require_error(error);
    }
    return "OK wrote " + std::to_string(contents.size()) + " bytes";
}

} // namespace minikv::osfs
