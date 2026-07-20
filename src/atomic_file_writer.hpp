#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>

namespace minikv::detail {

class AtomicFileWriter {
public:
    explicit AtomicFileWriter(std::filesystem::path target, std::string_view temp_tag = {});
    ~AtomicFileWriter();

    AtomicFileWriter(const AtomicFileWriter&) = delete;
    AtomicFileWriter& operator=(const AtomicFileWriter&) = delete;
    AtomicFileWriter(AtomicFileWriter&&) = delete;
    AtomicFileWriter& operator=(AtomicFileWriter&&) = delete;

    bool ready() const;
    std::ostream& stream();
    bool commit();

    const std::filesystem::path& temp_path() const;

private:
    std::filesystem::path target_path_;
    std::filesystem::path temp_path_;
    std::ofstream output_;
    bool committed_ = false;
};

} // namespace minikv::detail
