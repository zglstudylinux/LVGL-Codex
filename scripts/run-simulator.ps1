[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    [string]$App = "device-dashboard",
    [string]$Msys2Root = "D:\software_download\code\MSYS2"
)

$ErrorActionPreference = "Stop"
& (Join-Path $PSScriptRoot "build-simulator.ps1") -Configuration $Configuration -App $App -Msys2Root $Msys2Root

$env:Path = "$(Join-Path $Msys2Root 'ucrt64\bin');$env:Path"
$executableName = $App -replace "-", "_"
$executable = Join-Path $PSScriptRoot "..\build\pc-sdl2-$App-$($Configuration.ToLowerInvariant())\apps\$App\$executableName.exe"
& $executable
