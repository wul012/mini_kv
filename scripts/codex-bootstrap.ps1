# codex session bootstrap — dot-source at session start:  . .\scripts\codex-bootstrap.ps1
# Prints orientation in one command and sets up the CLion toolchain for this shell.
$ErrorActionPreference = 'SilentlyContinue'

$mingwBin = 'D:\CLION\CLion 2025.1.1\bin\mingw\bin'
$pathEntries = $env:PATH -split ';' | Where-Object { $_ }
if ($pathEntries -notcontains $mingwBin) {
    $env:PATH = $mingwBin + ';' + $env:PATH
}
$global:cmake = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\cmake.exe'
$global:ctest = 'D:\CLION\CLion 2025.1.1\bin\cmake\win\x64\bin\ctest.exe'
$global:clangformat = 'D:\CLION\CLion 2025.1.1\bin\clang\win\x64\bin\clang-format.exe'

Write-Output '=== git: last 3 commits / status ==='
git log --oneline -3
git status -sb | Select-Object -First 8
Write-Output '=== latest tag ==='
git tag --sort=-creatordate | Select-Object -First 1
Write-Output '=== CI: last 3 runs (do not block-watch intermediates) ==='
gh run list --limit 3
Write-Output '=== pointers ==='
Write-Output 'Active program : D:\C\四项目理解统筹\AGENTS.md -> Current Active Program'
Write-Output 'Local briefs   : 治理计划\ (README index)'
Write-Output 'Progress ledger: docs\production-excellence-progress.md'
Write-Output 'Toolchain ready: PATH prepended (MinGW); $cmake / $ctest / $clangformat set for this shell.'
