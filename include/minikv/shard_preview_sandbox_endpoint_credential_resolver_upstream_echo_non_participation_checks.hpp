#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks {

struct SandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const SandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheck>
sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks();
int planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_check_count();
std::string format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_json(
    int published_check_count);

} // namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks
