# mini-kv v684 route-preview-value-supply-signed-approval-template-mutation-block-audit

## Goal

v684 maps the Node v1036 signed approval template closeout direction into mini-kv as read-only evidence and publishes the $Field slice. It is not a signature capture entry point, not an approval grant, not an operator value import, not a sibling evidence import, not a service start, not a WAL write, and not Java order authority.

## Module Split

The command surface is SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON. The main aggregator is src/shard_route_preview_operator_value_supply_signed_approval_template.cpp; it binds source v660 approval-template evidence, selects the current v684 stage, formats the stage chain, and appends the shared no-execution boundary fields.

Fields live in src/shard_route_preview_operator_value_supply_signed_approval_template_fields.cpp. Every field has a value class and a missing-field blocker. This version focuses on $Focus, while the complete 25-field template remains visible from the first slice. publishedByCurrentStage is the rolling signal that tells consumers which slice is active.

Clauses live in src/shard_route_preview_operator_value_supply_signed_approval_template_clauses.cpp. Every clause carries a rejection code and a non-execution clause. Missing signature data, missing source bindings, raw values, nonzero value counts, writes, sibling mutation, service starts, and execution all remain blocked.

Review controls live in src/shard_route_preview_operator_value_supply_signed_approval_template_controls.cpp. They map the signed template to source review-package controls by name only; mini-kv does not read Node files, call Node services, or open external connections.

Validation lives in src/shard_route_preview_operator_value_supply_signed_approval_template_validation.cpp. It proves that field, clause, and review-control catalogs all have 25 entries and remain aligned, while signatureCaptured=false, signedApprovalPresent=false, operatorValueImported=false, writeRoutingAllowed=false, and executionAllowed=false.

## Tests

Focused tests cover fields, clauses, controls, validation, and the main command. The main command test also verifies COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON classify the command as read-only metadata: no store mutation, no WAL touch, and no execution authority.

Full CTest covers 102 tests. CLI smoke reads both SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON and SHARDJSON. TCP smoke starts real minikv_server.exe and minikv_client.exe, reads the same evidence through the wire protocol, and then stops the server.

## Summary

v684 thickens the signed approval template evidence for a future capture preflight, while keeping signatures, grants, values, imports, routing, writes, and execution outside mini-kv.
