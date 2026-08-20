[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    [string]$App = "device-dashboard",
    [string]$Msys2Root = "D:\software_download\code\MSYS2"
)

$ErrorActionPreference = "Stop"
$toolBin = Join-Path $Msys2Root "ucrt64\bin"
$cmake = Join-Path $toolBin "cmake.exe"

if (-not (Test-Path $cmake)) {
    throw "未找到 UCRT64 CMake。请先运行 scripts/setup-windows-simulator.ps1。"
}
if (-not (Test-Path (Join-Path $PSScriptRoot "..\apps\$App\app_manifest.cmake"))) {
    throw "未知应用：$App"
}

$env:Path = "$toolBin;$env:Path"
$buildDir = Join-Path $PSScriptRoot "..\build\pc-sdl2-$App-$($Configuration.ToLowerInvariant())"

& $cmake -S (Join-Path $PSScriptRoot "..") -B $buildDir -G Ninja `
    "-DCMAKE_BUILD_TYPE=$Configuration" `
    "-DLVGL_APP=$App" `
    "-DLVGL_TARGET=pc-sdl2" `
    "-DLVGL_SERIES=9"
& $cmake --build $buildDir
& $cmake --build $buildDir --target test
