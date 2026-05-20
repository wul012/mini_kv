# mini-kv Code Walkthroughs - Production Prototype Stage, Volume 2

This sibling folder is for new mini-kv walkthroughs starting with v122.

## Entries

- `178-version-122-credential-resolver-approval-required-implementation-readiness-non-participation-receipt.md` - mini-kv v122 Node v281 approval-required implementation readiness non-participation receipt, fixture, tests, TCP smoke, and `d/122` archive.
- `179-version-123-smokejson-command-test-split.md` - mini-kv v123 test-only split that moves live `SMOKEJSON` receipt assertions from `command_tests.cpp` into `smokejson_command_receipt_tests.cpp`.
- `180-version-124-command-smoke-formatters-split.md` - mini-kv v124 format-only split that moves `SMOKEJSON` formatter logic into `command_smoke_formatters.cpp`.
- `181-version-125-credential-resolver-test-only-shell-receipt-split.md` - mini-kv v125 maintenance split that moves the v116 test-only resolver shell receipt into its own compilation unit and adds a private credential resolver receipt helper.

## Rules

- Continue file numbering from the previous walkthrough folder.
- Keep each version's walkthrough focused on purpose, boundaries, module roles, tests, smoke evidence, and downstream value.
- Do not move historical walkthroughs from `../代码讲解记录_生产雏形阶段/`.
