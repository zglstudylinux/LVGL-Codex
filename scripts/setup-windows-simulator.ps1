[CmdletBinding()]
param(
    [string]$Msys2Root = "D:\software_download\code\MSYS2"
)

$ErrorActionPreference = "Stop"
$bash = Join-Path $Msys2Root "usr\bin\bash.exe"

if (-not (Test-Path $bash)) {
    throw "未找到 MSYS2：$Msys2Root。请安装 MSYS2 后通过 -Msys2Root 指定其目录。"
}

# Initialise the package keyring first. This also repairs older MSYS2 installs
# whose package signatures are rejected after a key rotation.
& $bash -lc "pacman-key --init && pacman-key --populate msys2"

# Use a project-local UCRT64 repository configuration. It avoids changing the
# user's MSYS2 mirror settings and is more reliable on mainland China networks.
$pacmanConfig = (& $bash -lc "cygpath -u '$PSScriptRoot/msys2-ucrt64-pacman.conf'").Trim()
& $bash -lc "pacman --config '$pacmanConfig' -Syy --needed --noconfirm mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-SDL2"
