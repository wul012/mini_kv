#pragma once

#include <string>
#include <string_view>

namespace minikv::release_test {

void assert_contains(const std::string& text, std::string_view expected);
void verify_release_inputs();

} // namespace minikv::release_test
