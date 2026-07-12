# mini-kv Elegance Hotspot Closeout (v1661)

Status: READY FOR CLAUDE REVIEW. External verdict: PENDING.

## Bounded Result

| Metric | v1659 start | v1661 terminal | Change |
|---|---:|---:|---:|
| filename stems over 40 | 740 | 735 | -5 |
| public-header-visible identifiers over 40 | 883 | 883 | 0 |
| new waivers | 0 | 0 | 0 |

Terminal filename categories are exact: `include=218`, `src=241`, `tests=276`. v1660 records five 100%-identical internal translation-unit renames; v1661 then formats those newly touched legacy units after CI enforces the scout rule. Public headers, symbols, CLI/RESP/JSON keys, release fixtures, archives, Store/WAL behavior, router authority, write authority, and execution authority did not change.

## Requirement-To-Evidence Closure

| Requirement | Implementation | Failing or reproducible evidence | State |
|---|---|---|---|
| 40-character census | standalone C++ scanner over filenames and public headers | injected 741st filename fails | closed v1659 |
| shrink-only baseline | exact key sets plus compiled maxima | new, replacement, stale, or count growth fails | terminal 735/883 |
| top-five remediation | five concise `compared_package_*` `.cpp` paths | pre-refresh run lists five stale paths; v1660 records 100% renames | closed v1660 |
| cross-platform gate | terminal-CR normalization plus CRLF sample | v1659 Windows run `29176190797` failed before fix | closed locally v1660 |
| touched-file format | clang-format over five renamed units | v1660 changed-files job failed before repair | closed locally v1661 |
| byte/runtime stability | fixture manifest, full CTest, real CLI | 1514-file digest plus no-router/no-write/no-execution checks | local proof complete |
| hard stop | this terminal ledger and pending external verdict | plan forbids a fifth version or silent continuation | reached v1661 |

## Intentionally Retained Debt

- **218 include paths and 883 header-visible identifiers:** these are source-compatibility surfaces. The bounded plan did not authorize public API aliases, deprecation periods, or consumer migration. Their presence is debt, not a waiver or an assertion that every name is desirable.
- **276 test paths:** tests are internal, but renaming them safely requires separate CMake/CI batches and does not improve runtime design by itself. The hard stop takes priority over inflating this program.
- **241 source paths:** some are private and future candidates; others pair with public families or historical evidence. This program fixed only the mechanically selected top five and does not label the remaining 241 as contract-pinned.

No archive or fixture path is counted by this scanner, and none was moved. No wildcard exemption, directory exclusion, case folding, path trimming, or second baseline was introduced.

## CI Findings Were Not Hidden

v1659 run `29176190797` exposed CRLF baseline parsing on Windows; v1660 added a permanent in-memory CRLF sample. v1660 run `29176799196` then exposed inherited formatting debt in the five renamed files; v1661 formats them rather than bypassing the changed-files gate. Because formatting changes source bytes, final-tree stability is proved by focused/full CTest, fixture manifest, and real CLI output. The v1660 commit remains the auditable proof that the path migration itself began as five 100% renames.

## Strongest Reviewer Objection

“Five of 740 filenames is a small reduction.” Correct. The primary deliverable is a cross-platform gate that makes future growth fail and makes every repayment reviewable; the top-five batch proves the safe migration process. Claiming broad elegance completion would be false. The plan was deliberately capped at four versions, uses three, and stops with 735/883 visible rather than hiding the remainder.

## Reviewer Reproduction

```powershell
cmake -S . -B cmake-build-review -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-review --parallel 4
ctest --test-dir cmake-build-review -R 'elegance_name_census_contract|project_docs_honesty_contract|comp_pkg_intake_.*' --output-on-failure
ctest --test-dir cmake-build-review --output-on-failure --timeout 120 --progress
python scripts/archive_inventory.py --budget-mib 8 --strict
git show --summary --find-renames v1660
```

The executor does not award the final verdict. Claude should reproduce the terminal census, inspect both CI finding/repair chains, verify no public/fixture contract moved, and then record PASS or findings.
