#include "minikv/osfs/virtual_disk.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace minikv::osfs {

namespace {

void validate_geometry(std::uint32_t block_count, std::uint32_t block_size) {
    if (block_size < 512 || block_size > 4096) {
        throw std::invalid_argument{"osfs block size must be between 512 and 4096 bytes"};
    }
    if (block_count < 16) {
        throw std::invalid_argument{"osfs disk requires at least 16 blocks"};
    }
}

std::string display_path(const std::filesystem::path& path) {
    const auto bytes = path.u8string();
    return {reinterpret_cast<const char*>(bytes.data()), bytes.size()};
}

} // namespace

VirtualDisk VirtualDisk::create(const std::filesystem::path& path, std::uint32_t block_count,
                                std::uint32_t block_size) {
    validate_geometry(block_count, block_size);

    std::ofstream output{path, std::ios::binary | std::ios::trunc};
    if (!output) {
        throw std::runtime_error{"failed to create osfs disk image: " + display_path(path)};
    }

    const auto total_bytes = static_cast<std::uint64_t>(block_count) * block_size;
    output.seekp(static_cast<std::streamoff>(total_bytes - 1));
    output.put('\0');
    if (!output) {
        throw std::runtime_error{"failed to size osfs disk image: " + display_path(path)};
    }

    return VirtualDisk{path, block_count, block_size};
}

VirtualDisk VirtualDisk::open(const std::filesystem::path& path, std::uint32_t block_size) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error{"osfs disk image does not exist: " + display_path(path)};
    }
    if (block_size < 512 || block_size > 4096) {
        throw std::invalid_argument{"osfs block size must be between 512 and 4096 bytes"};
    }

    const auto bytes = std::filesystem::file_size(path);
    if (bytes == 0 || bytes % block_size != 0) {
        throw std::runtime_error{"osfs disk image size is not aligned to the block size: " + display_path(path)};
    }

    const auto block_count = bytes / block_size;
    if (block_count > UINT32_MAX) {
        throw std::runtime_error{"osfs disk image is too large"};
    }
    validate_geometry(static_cast<std::uint32_t>(block_count), block_size);

    return VirtualDisk{path, static_cast<std::uint32_t>(block_count), block_size};
}

VirtualDisk::VirtualDisk(std::filesystem::path path, std::uint32_t block_count, std::uint32_t block_size)
    : path_{std::move(path)}, block_count_{block_count}, block_size_{block_size} {}

const std::filesystem::path& VirtualDisk::path() const { return path_; }

std::uint32_t VirtualDisk::block_size() const { return block_size_; }

std::uint32_t VirtualDisk::block_count() const { return block_count_; }

std::vector<char> VirtualDisk::read_block(std::uint32_t block_index) const {
    if (block_index >= block_count_) {
        throw std::out_of_range{"osfs block read out of range"};
    }

    std::ifstream input{path_, std::ios::binary};
    if (!input) {
        throw std::runtime_error{"failed to open osfs disk image for reading: " + display_path(path_)};
    }

    std::vector<char> block(block_size_, '\0');
    input.seekg(static_cast<std::streamoff>(static_cast<std::uint64_t>(block_index) * block_size_));
    input.read(block.data(), static_cast<std::streamsize>(block.size()));
    if (input.gcount() != static_cast<std::streamsize>(block.size())) {
        throw std::runtime_error{"short osfs block read"};
    }
    return block;
}

void VirtualDisk::write_block(std::uint32_t block_index, const std::vector<char>& bytes) {
    if (block_index >= block_count_) {
        throw std::out_of_range{"osfs block write out of range"};
    }
    if (bytes.size() != block_size_) {
        throw std::invalid_argument{"osfs block writes must match the block size"};
    }

    std::fstream io{path_, std::ios::binary | std::ios::in | std::ios::out};
    if (!io) {
        throw std::runtime_error{"failed to open osfs disk image for writing: " + display_path(path_)};
    }

    io.seekp(static_cast<std::streamoff>(static_cast<std::uint64_t>(block_index) * block_size_));
    io.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    if (!io) {
        throw std::runtime_error{"failed to write osfs block"};
    }
}

} // namespace minikv::osfs
