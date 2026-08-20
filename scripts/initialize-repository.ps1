[CmdletBinding()]
param(
    [string]$Remote = "https://github.com/zglstudylinux/LVGL-Codex.git",
    [string]$Branch = "main"
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path

if (Test-Path (Join-Path $repoRoot ".git")) {
    throw "此目录已经是 Git 仓库；为避免改写现有历史，脚本已停止。"
}

$userName = ([string](git config --get user.name)).Trim()
$userEmail = ([string](git config --get user.email)).Trim()
if ([string]::IsNullOrWhiteSpace($userName) -or [string]::IsNullOrWhiteSpace($userEmail)) {
    throw "请先设置 Git 身份：git config --global user.name / user.email。"
}

# Commit the local scaffold on a temporary branch first. It makes every file
# recoverable while the checked-out branch is switched to the remote baseline.
git -C $repoRoot init -b scaffold-local
if ($LASTEXITCODE -ne 0) { throw "git init 失败。" }
git -C $repoRoot add --all
if ($LASTEXITCODE -ne 0) { throw "暂存脚手架失败。" }
git -C $repoRoot commit -m "chore: scaffold LVGL-Codex"
if ($LASTEXITCODE -ne 0) { throw "创建脚手架提交失败。" }

git -C $repoRoot remote add origin $Remote
git -C $repoRoot fetch origin $Branch
if ($LASTEXITCODE -ne 0) { throw "无法读取 origin/$Branch；本地脚手架提交仍保留在 scaffold-local。" }

git -C $repoRoot checkout -B $Branch "origin/$Branch"
if ($LASTEXITCODE -ne 0) { throw "无法切换到 origin/$Branch。" }
git -C $repoRoot merge scaffold-local --allow-unrelated-histories --no-edit
if ($LASTEXITCODE -ne 0) {
    throw "远端与脚手架存在同名文件冲突。请手动解决冲突；没有任何历史被丢弃。"
}

git -C $repoRoot branch -d scaffold-local
if ($LASTEXITCODE -ne 0) { throw "已合并脚手架，但无法删除临时分支 scaffold-local。" }

Write-Host "已基于 origin/$Branch 合并脚手架。请确认 CI 通过后执行 git -C '$repoRoot' push origin $Branch。"
