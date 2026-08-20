[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [ValidatePattern("^[a-z0-9]+(?:-[a-z0-9]+)*$")]
    [string]$Name
)

$ErrorActionPreference = "Stop"
$repoRoot = Join-Path $PSScriptRoot ".."
$template = Join-Path $repoRoot "templates\app"
$destination = Join-Path $repoRoot "apps\$Name"

if (Test-Path $destination) {
    throw "应用已存在：$Name"
}

Copy-Item -LiteralPath $template -Destination $destination -Recurse
$identifier = $Name -replace "-", "_"
Get-ChildItem -LiteralPath $destination -File -Recurse | ForEach-Object {
    $content = Get-Content -LiteralPath $_.FullName -Raw
    $content = $content.Replace("__APP_NAME__", $Name).Replace("__APP_IDENTIFIER__", $identifier)
    Set-Content -LiteralPath $_.FullName -Value $content -NoNewline
}

Write-Host "已创建 apps/$Name。先实现 domain/application/ui，再执行 scripts/build-simulator.ps1 -App $Name。"
