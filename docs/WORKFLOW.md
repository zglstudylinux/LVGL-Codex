# Git 与发布工作流

首次上传本仓库时，先设置 Git 用户名和邮箱，然后在当前目录运行：

```powershell
.\scripts\initialize-repository.ps1
```

它会先把本地脚手架提交到临时分支，再读取 `origin/main`、以其为当前分支并合并脚手架。这样远端已有内容和本地文件都会保留；遇到同名文件冲突会停止并要求人工处理。确认 CI 通过后使用普通 `git push origin main`，不要强推。首次推送成功后创建并推送不可变的框架标签：`git tag -a v0.1.0 -m "LVGL-Codex 0.1.0"` 与 `git push origin v0.1.0`。

在 `feat/<app-id>/<screen>` 分支完成一个页面，先在 PC 模拟器手动验证，再提交并通过 CI 后合入 `main`。提交使用 Conventional Commits，例如 `feat(device-dashboard): add settings screen`。

`main` 必须始终可配置、可编译且 CTest 通过。框架公共 API、平台端口和 LVGL 版本升级都使用独立提交，并以语义化标签发布。

要将一个应用交付到独立项目仓库，先确认待导出的提交已通过 GitHub Actions，再执行 `scripts/build-simulator.ps1 -App <id>`，最后在干净工作区运行：

```powershell
.\scripts\export-app.ps1 -App <id> -Remote <project-repo-url>
```

脚本使用 Git subtree 导出 `apps/<id>` 的完整历史，会再次运行本地 CTest，只允许快进推送且不会强推。先以 `-DryRun` 检查导出分支。

`templates/app` 可通过 `scripts/export-template.ps1` 导出到单独仓库并在 GitHub 设置为 Template repository；模板仓库只包含可创建新应用的骨架，不承载产品代码。
