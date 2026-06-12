#include "test_support.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace {

std::string read_text(const std::filesystem::path& path) {
    std::ifstream input{path, std::ios::binary};
    assert(input.is_open());

    std::ostringstream output;
    output << input.rdbuf();
    return output.str();
}

bool is_cjk(std::uint32_t codepoint) {
    return (codepoint >= 0x4E00 && codepoint <= 0x9FFF) ||
           (codepoint >= 0x3400 && codepoint <= 0x4DBF) ||
           (codepoint >= 0x20000 && codepoint <= 0x2A6DF) ||
           (codepoint >= 0x2A700 && codepoint <= 0x2B73F) ||
           (codepoint >= 0x2B740 && codepoint <= 0x2B81F) ||
           (codepoint >= 0x2B820 && codepoint <= 0x2CEAF) ||
           (codepoint >= 0xF900 && codepoint <= 0xFAFF);
}

std::size_t count_chinese_characters(const std::string& text) {
    std::size_t count = 0;
    std::size_t index = 0;
    while (index < text.size()) {
        const auto byte = static_cast<unsigned char>(text[index]);
        std::uint32_t codepoint = 0;
        std::size_t length = 0;
        if ((byte & 0x80U) == 0U) {
            codepoint = byte;
            length = 1;
        } else if ((byte & 0xE0U) == 0xC0U && index + 1 < text.size()) {
            codepoint = static_cast<std::uint32_t>(byte & 0x1FU) << 6U;
            codepoint |= static_cast<unsigned char>(text[index + 1]) & 0x3FU;
            length = 2;
        } else if ((byte & 0xF0U) == 0xE0U && index + 2 < text.size()) {
            codepoint = static_cast<std::uint32_t>(byte & 0x0FU) << 12U;
            codepoint |= (static_cast<unsigned char>(text[index + 1]) & 0x3FU) << 6U;
            codepoint |= static_cast<unsigned char>(text[index + 2]) & 0x3FU;
            length = 3;
        } else if ((byte & 0xF8U) == 0xF0U && index + 3 < text.size()) {
            codepoint = static_cast<std::uint32_t>(byte & 0x07U) << 18U;
            codepoint |= (static_cast<unsigned char>(text[index + 1]) & 0x3FU) << 12U;
            codepoint |= (static_cast<unsigned char>(text[index + 2]) & 0x3FU) << 6U;
            codepoint |= static_cast<unsigned char>(text[index + 3]) & 0x3FU;
            length = 4;
        } else {
            length = 1;
        }

        if (is_cjk(codepoint)) {
            ++count;
        }
        index += length;
    }
    return count;
}

} // namespace

int main() {
    using minikv::test_support::assert_contains;

    const auto path =
        minikv::test_support::source_root() / "代码讲解记录_生产雏形阶段_第四册" /
        "978-version-1607-f-folder-explanation-quality-closeout.md";
    assert(std::filesystem::exists(path));

    const auto text = read_text(path);
    assert(count_chinese_characters(text) >= 3000);
    assert_contains(text, "# v1607 f-folder 解释质量收口");
    assert_contains(text, "不是新的执行入口");
    assert_contains(text, "SHARDFFOLDEREXPLANATIONQUALITYCLOSEOUTJSON");
    assert_contains(text, "minimumWalkthroughChineseCharacterCount");
    assert_contains(text, "fillerPaddingAllowed");
    assert_contains(text, "每个版本至少一篇三千字级别的中文讲解");
    assert_contains(text, "字数不够就加大版本工作量");
    assert_contains(text, "禁止硬凑");
    assert_contains(text, "readOnly");
    assert_contains(text, "executionAllowed");
    assert_contains(text, "touchesWal");

    return 0;
}
