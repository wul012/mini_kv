#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

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

std::uint16_t parse_port(const char* text) {
    const int port = std::stoi(text);
    if (port < 1 || port > 65535) {
        throw std::out_of_range{"port must be between 1 and 65535"};
    }
    return static_cast<std::uint16_t>(port);
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

SocketGuard connect_to_server(std::string_view host, std::uint16_t port) {
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

    for (addrinfo* addr = addresses.get(); addr != nullptr; addr = addr->ai_next) {
        SocketGuard socket{::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)};
        if (socket.get() == invalid_socket) {
            continue;
        }

        if (connect(socket.get(), addr->ai_addr, static_cast<int>(addr->ai_addrlen)) == 0) {
            return socket;
        }
    }

    throw std::runtime_error{socket_error_message("connect failed")};
}

void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [host] [port]\n";
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

} // namespace

int main(int argc, char** argv) {
    try {
        if (argc > 3) {
            print_usage(argv[0]);
            return 2;
        }

        std::string host = "127.0.0.1";
        std::uint16_t port = 6379;

        if (argc >= 2) {
            host = argv[1];
        }

        if (argc >= 3) {
            port = parse_port(argv[2]);
        }

        NetworkRuntime runtime;
        SocketGuard socket = connect_to_server(host, port);

        std::string greeting;
        if (!read_line(socket.get(), greeting)) {
            std::cerr << "server closed connection before greeting\n";
            return 1;
        }

        std::cout << greeting << '\n';

        std::string line;
        while (true) {
            std::cout << "mini-kv@" << host << ':' << port << "> ";
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
    } catch (const std::exception& error) {
        std::cerr << "fatal: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
