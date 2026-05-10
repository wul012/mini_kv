#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <cerrno>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace {

#ifdef _WIN32
using SocketHandle = SOCKET;
constexpr SocketHandle invalid_socket = INVALID_SOCKET;

void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        closesocket(socket);
    }
}

class NetworkRuntime {
public:
    NetworkRuntime() {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            throw std::runtime_error{"WSAStartup failed"};
        }
    }

    ~NetworkRuntime() {
        WSACleanup();
    }

    NetworkRuntime(const NetworkRuntime&) = delete;
    NetworkRuntime& operator=(const NetworkRuntime&) = delete;
};

std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": WSA error " + std::to_string(WSAGetLastError());
}

#else
using SocketHandle = int;
constexpr SocketHandle invalid_socket = -1;

void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        close(socket);
    }
}

class NetworkRuntime {
public:
    NetworkRuntime() = default;
};

std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": " + std::strerror(errno);
}
#endif

class SocketGuard {
public:
    explicit SocketGuard(SocketHandle socket = invalid_socket) : socket_(socket) {}

    ~SocketGuard() {
        close_socket(socket_);
    }

    SocketGuard(const SocketGuard&) = delete;
    SocketGuard& operator=(const SocketGuard&) = delete;

    SocketGuard(SocketGuard&& other) noexcept : socket_(other.release()) {}

    SocketGuard& operator=(SocketGuard&& other) noexcept {
        if (this != &other) {
            close_socket(socket_);
            socket_ = other.release();
        }
        return *this;
    }

    SocketHandle get() const {
        return socket_;
    }

    SocketHandle release() {
        const SocketHandle socket = socket_;
        socket_ = invalid_socket;
        return socket;
    }

private:
    SocketHandle socket_;
};

class AddrInfoGuard {
public:
    explicit AddrInfoGuard(addrinfo* info) : info_(info) {}

    ~AddrInfoGuard() {
        if (info_ != nullptr) {
            freeaddrinfo(info_);
        }
    }

    AddrInfoGuard(const AddrInfoGuard&) = delete;
    AddrInfoGuard& operator=(const AddrInfoGuard&) = delete;

    addrinfo* get() const {
        return info_;
    }

private:
    addrinfo* info_;
};

struct ClientOptions {
    std::string host = "127.0.0.1";
    std::uint16_t port = 6379;
    std::optional<std::chrono::milliseconds> timeout;
    int connect_retries = 0;
    std::chrono::milliseconds retry_delay{250};
};

class UsageError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

std::uint16_t parse_port(const char* text) {
    const int port = std::stoi(text);
    if (port < 1 || port > 65535) {
        throw std::out_of_range{"port must be between 1 and 65535"};
    }
    return static_cast<std::uint16_t>(port);
}

std::chrono::milliseconds parse_positive_milliseconds(const char* text, std::string_view name) {
    if (text[0] == '\0' || text[0] == '-') {
        throw std::out_of_range{std::string{name} + " must be a positive integer"};
    }

    std::size_t parsed_chars = 0;
    const long long value = std::stoll(text, &parsed_chars);
    if (text[parsed_chars] != '\0' || value <= 0) {
        throw std::out_of_range{std::string{name} + " must be a positive integer"};
    }

    return std::chrono::milliseconds{value};
}

int parse_nonnegative_int(const char* text, std::string_view name) {
    if (text[0] == '\0' || text[0] == '-') {
        throw std::out_of_range{std::string{name} + " must be a non-negative integer"};
    }

    std::size_t parsed_chars = 0;
    const long long value = std::stoll(text, &parsed_chars);
    if (text[parsed_chars] != '\0' || value < 0 || value > std::numeric_limits<int>::max()) {
        throw std::out_of_range{std::string{name} + " must be a non-negative integer"};
    }

    return static_cast<int>(value);
}

std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}

std::string first_token(std::string_view line) {
    while (!line.empty() && std::isspace(static_cast<unsigned char>(line.front())) != 0) {
        line.remove_prefix(1);
    }

    const std::size_t end = line.find_first_of(" \t\r\n");
    if (end == std::string_view::npos) {
        return to_upper(line);
    }
    return to_upper(line.substr(0, end));
}

bool is_blank(std::string_view line) {
    return std::ranges::all_of(line, [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
}

bool send_all(SocketHandle socket, std::string_view data) {
    while (!data.empty()) {
        const std::size_t chunk_size = std::min<std::size_t>(data.size(), 4096);
#ifdef _WIN32
        const int sent = send(socket, data.data(), static_cast<int>(chunk_size), 0);
#else
        const auto sent = send(socket, data.data(), chunk_size, 0);
#endif
        if (sent <= 0) {
            return false;
        }

        data.remove_prefix(static_cast<std::size_t>(sent));
    }

    return true;
}

void set_socket_timeout(SocketHandle socket, std::chrono::milliseconds timeout) {
#ifdef _WIN32
    const auto clamped = std::min<long long>(
        timeout.count(), static_cast<long long>(std::numeric_limits<DWORD>::max()));
    const DWORD timeout_ms = static_cast<DWORD>(clamped);
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout_ms), sizeof(timeout_ms)) !=
            0 ||
        setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&timeout_ms), sizeof(timeout_ms)) !=
            0) {
        throw std::runtime_error{socket_error_message("setsockopt timeout failed")};
    }
#else
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeout - seconds);
    timeval value{};
    value.tv_sec = static_cast<long>(seconds.count());
    value.tv_usec = static_cast<long>(microseconds.count());
    if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &value, sizeof(value)) != 0 ||
        setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, &value, sizeof(value)) != 0) {
        throw std::runtime_error{socket_error_message("setsockopt timeout failed")};
    }
#endif
}

bool read_line(SocketHandle socket, std::string& line) {
    line.clear();

    while (true) {
        char ch = '\0';
#ifdef _WIN32
        const int received = recv(socket, &ch, 1, 0);
#else
        const auto received = recv(socket, &ch, 1, 0);
#endif
        if (received <= 0) {
            return false;
        }

        if (ch == '\n') {
            return true;
        }

        if (ch != '\r') {
            line.push_back(ch);
        }

        if (line.size() > 64 * 1024) {
            throw std::runtime_error{"response line too long"};
        }
    }
}

SocketGuard connect_to_server(std::string_view host,
                              std::uint16_t port,
                              std::optional<std::chrono::milliseconds> timeout) {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* raw_info = nullptr;
    const std::string port_text = std::to_string(port);
    const int result = getaddrinfo(std::string{host}.c_str(), port_text.c_str(), &hints, &raw_info);
    if (result != 0) {
#ifdef _WIN32
        throw std::runtime_error{"getaddrinfo failed: " + std::to_string(result)};
#else
        throw std::runtime_error{std::string{"getaddrinfo failed: "} + gai_strerror(result)};
#endif
    }

    AddrInfoGuard addresses{raw_info};
    std::string last_connect_error;

    for (addrinfo* addr = addresses.get(); addr != nullptr; addr = addr->ai_next) {
        SocketGuard socket{::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)};
        if (socket.get() == invalid_socket) {
            continue;
        }

        if (timeout.has_value()) {
            set_socket_timeout(socket.get(), *timeout);
        }

        if (connect(socket.get(), addr->ai_addr, static_cast<int>(addr->ai_addrlen)) == 0) {
            return socket;
        }

        last_connect_error = socket_error_message("connect failed");
    }

    if (last_connect_error.empty()) {
        last_connect_error = socket_error_message("connect failed");
    }
    throw std::runtime_error{last_connect_error};
}

SocketGuard connect_with_retries(const ClientOptions& options) {
    const int total_attempts = options.connect_retries + 1;
    std::string last_error;

    for (int attempt = 1; attempt <= total_attempts; ++attempt) {
        try {
            return connect_to_server(options.host, options.port, options.timeout);
        } catch (const std::exception& error) {
            last_error = error.what();
            if (attempt == total_attempts) {
                break;
            }

            std::cerr << "connect attempt " << attempt << '/' << total_attempts << " failed: " << error.what()
                      << '\n';
            std::cerr << "retrying in " << options.retry_delay.count() << " ms\n";
            std::this_thread::sleep_for(options.retry_delay);
        }
    }

    throw std::runtime_error{"connect failed after " + std::to_string(total_attempts) +
                             " attempt(s): " + last_error};
}

void print_usage(const char* program) {
    std::cerr << "Usage: " << program
              << " [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]\n";
}

bool print_response(SocketHandle socket, std::string_view command) {
    std::string response;
    if (!read_line(socket, response)) {
        return false;
    }

    std::cout << response << '\n';

    if (first_token(command) == "HELP" && response == "Commands:") {
        while (response != "  EXIT") {
            if (!read_line(socket, response)) {
                return false;
            }
            std::cout << response << '\n';
        }
    }

    return true;
}

ClientOptions parse_options(int argc, char** argv) {
    ClientOptions options;
    int positional = 0;

    for (int index = 1; index < argc; ++index) {
        const std::string_view argument = argv[index];
        if (argument == "--connect-retries") {
            if (++index >= argc) {
                throw UsageError{"missing value for --connect-retries"};
            }
            options.connect_retries = parse_nonnegative_int(argv[index], "connect-retries");
            continue;
        }

        if (argument == "--retry-delay-ms") {
            if (++index >= argc) {
                throw UsageError{"missing value for --retry-delay-ms"};
            }
            options.retry_delay = parse_positive_milliseconds(argv[index], "retry-delay-ms");
            continue;
        }

        if (!argument.empty() && argument.front() == '-') {
            throw UsageError{"unknown option: " + std::string{argument}};
        }

        ++positional;
        if (positional == 1) {
            options.host = argv[index];
        } else if (positional == 2) {
            options.port = parse_port(argv[index]);
        } else if (positional == 3) {
            options.timeout = parse_positive_milliseconds(argv[index], "timeout_ms");
        } else {
            throw UsageError{"too many positional arguments"};
        }
    }

    return options;
}

} // namespace

int main(int argc, char** argv) {
    try {
        const ClientOptions options = parse_options(argc, argv);

        NetworkRuntime runtime;
        SocketGuard socket = connect_with_retries(options);
        std::cout << "connected to mini-kv at " << options.host << ':' << options.port << '\n';
        if (options.timeout.has_value()) {
            std::cout << "socket timeout: " << options.timeout->count() << " ms\n";
        }
        if (options.connect_retries > 0) {
            std::cout << "connect retries: " << options.connect_retries
                      << ", retry delay: " << options.retry_delay.count() << " ms\n";
        }

        std::string line;
        while (true) {
            std::cout << "mini-kv@" << options.host << ':' << options.port << "> ";
            if (!std::getline(std::cin, line)) {
                std::cout << '\n';
                break;
            }

            if (is_blank(line)) {
                continue;
            }

            if (!send_all(socket.get(), line + "\n")) {
                std::cerr << "failed to send command\n";
                return 1;
            }

            if (!print_response(socket.get(), line)) {
                std::cerr << "server closed connection\n";
                return 1;
            }

            const std::string command = first_token(line);
            if (command == "QUIT" || command == "EXIT") {
                break;
            }
        }
    } catch (const UsageError& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        print_usage(argv[0]);
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
