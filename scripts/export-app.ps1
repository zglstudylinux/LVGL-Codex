[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [ValidatePattern("^[a-z0-9]+(?:-[a-z0-9]+)*$")]
    [string]$App,
    [Parameter(Mandatory)]
    [string]$Remote,
    [string]$Branch = "main",
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$prefix = "apps/$App"

if (-not (Test-Path (Join-Path $repoRoot "$prefix/app_manifest.cmake"))) {
    throw "未知应用：$App"
}
if ((git -C $repoRoot status --porcelain).Count -ne 0) {
    throw "工作区不干净；导出前请提交或暂存所有改动。"
}
$buildDir = Join-Path $repoRoot "build\pc-sdl2-$App-debug"
if (-not (Test-Path $buildDir)) {
    throw "未找到已验证构建：$buildDir。请先运行 scripts/build-simulator.ps1 -App $App。"
}
ctest --test-dir $buildDir --output-on-failure
if ($LASTEXITCODE -ne 0) { throw "导出前 CTest 失败。" }

$exportBranch = "export/$App"
git -C $repoRoot subtree split --prefix=$prefix --branch $exportBranch
if ($LASTEXITCODE -ne 0) { throw "git subtree split 失败。" }

if ($DryRun) {
    Write-Host "已生成 $exportBranch；DryRun 未推送。"
    exit 0
}

# No --force: the project repository must accept a fast-forward update.
git -C $repoRoot push $Remote "$exportBranch`:$Branch"
if ($LASTEXITCODE -ne 0) { throw "推送失败；未使用强推，请先处理项目仓库历史。" }
