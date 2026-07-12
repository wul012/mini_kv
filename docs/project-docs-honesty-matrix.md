# v1661 Project Documentation Honesty Matrix

This matrix is the Stage 2 requirement-to-evidence ledger for `治理计划/v1637-production-excellence-completion-brief.md`. It records what the project documents claim, where the implementation lives, which mechanical evidence protects the claim, and what remains deliberately limited.

| Requirement | Current implementation evidence | Mechanical or runnable evidence | Status |
|---|---|---|---|
| Current delivery pointer | `README.md`, `START_HERE.md`, `docs/CHANGELOG.md`, `docs/production-excellence-progress.md` | `project_docs_honesty_contract` requires the v1661 review pointer and 351-test count | v1658 Stage-1 PASS retained; elegance verdict pending |
| Receipt consolidation reality | 28 receipt sources, 27 formatter owners, 27 builder-backed owners, zero pending, one named no-formatter waiver | `receipt_builder_census_contract`, `runtime_receipt_remaining_owner_baseline_tests`, `f/1656/证据/summary.txt` | complete |
| MFD/UFD two-level directory | `src/osfs_filesystem.cpp`, `src/osfs_directory.cpp`, `src/osfs_disk_layout.*` | `tests/osfs_tests.cpp::test_disk_users_and_two_level_directories`, `osfs_cli_smoke` | complete |
| Persistent user table and authentication | `src/osfs_filesystem.cpp`, `src/osfs_user_admin.cpp` | wrong-password, reopen, USERADD/PASSWD and permission assertions in `tests/osfs_tests.cpp` | complete; password hash remains teaching-grade, not a production KDF |
| Per-open descriptor positions | `include/minikv/osfs/command_processor.hpp`, `src/osfs_command_processor.cpp`, `src/osfs_file_io.cpp` | `test_descriptor_offsets_and_range_io` asserts READ/WRITE/SEEK/TELL offsets and persistence | complete |
| Direct plus single-indirect file blocks | `src/osfs_disk_layout.*`, `src/osfs_file_io.cpp` | `test_indirect_blocks_persist_and_release` plus boundary/range tests | complete for `8 + block_size / 4` blocks; no double/triple indirect blocks |
| Read-only consistency checker | `src/osfs_fsck.cpp`, `FileSystem::check_consistency()` | clean-image and deliberate bitmap-corruption assertions in `test_fsck_reports_ok_and_detects_corruption` | complete as detection; no automatic repair |
| Root user creation and password changes | `src/osfs_user_admin.cpp`, `src/osfs_command_processor.cpp` | `test_useradd_passwd_and_fsck_command` proves root-only USERADD, root reset, self-change, denial and reopen | complete |
| OSFS final package | `课程设计交付/v1636-osfs-final/` | requirement matrix, UTF-8 transcript, demo script, DOCX/PDF and Linux run evidence retained in the repository | complete; older packages are superseded history |
| Archive growth accounting | `scripts/archive_inventory.py`, `docs/archive-retention-index.md` | current script output plus explicit `f/1629-1636` and `课程设计交付/` measurements | complete; measurement never authorizes path movement |
| OSFS brief closure | `治理计划/v1631-osfs-coursework-completion-brief.md`, `治理计划/v1634-osfs-capacity-extension-brief.md`, `治理计划/README.md` | `project_docs_honesty_contract` requires both status markers | complete |

## Reconciliation Deviation

The Stage 2 prose says “three OSFS briefs”. **Only two OSFS execution briefs exist** in the live `治理计划/` directory: the v1631 completion brief and the v1634 capacity-extension brief. `v1637-production-excellence-completion-brief.md` is the Production Excellence closeout brief, not a third OSFS execution brief. v1657 marked the two real OSFS briefs executed; later maintenance versions retain this count correction instead of fabricating a third historical document.

## Boundaries

The OSFS layer is an independent coursework executable. It does not route KV commands, replace WAL or Snapshot persistence, participate in Node/Java managed-audit execution, or grant order authority. Its deliberate teaching boundaries remain one filesystem image, one MFD plus per-user UFDs, direct plus one single-indirect level, read-only FSCK without repair, and a deterministic teaching password hash. These limits are stated rather than hidden because “current capability” and “production-grade operating-system filesystem” are not equivalent claims.
