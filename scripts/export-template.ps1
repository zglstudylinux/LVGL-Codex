[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [string]$Remote,
    [string]$Branch = "main",
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
if ((git -C $repoRoot status --porcelain).Count -ne 0) {
    throw "工作区不干净；导出模板前请提交或暂存所有改动。"
}

$exportBranch = "export/app-template"
git -C $repoRoot subtree split --prefix=templates/app --branch $exportBranch
if ($LASTEXITCODE -ne 0) { throw "git subtree split 失败。" }

if ($DryRun) {
    Write-Host "已生成 $exportBranch；DryRun 未推送。"
    exit 0
}

git -C $repoRoot push $Remote "$exportBranch`:$Branch"
if ($LASTEXITCODE -ne 0) { throw "推送失败；未使用强推。" }
