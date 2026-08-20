# LVGL-Codex

可复用的嵌入式 LVGL 单仓多应用脚手架。先在 Windows SDL2 模拟器开发和验证 UI，再迁移到通用 Linux fbdev/evdev 或后续 MCU 板级包。

## 快速开始

Windows 首次安装 UCRT64 GCC、CMake、Ninja 和 SDL2：

```powershell
.\scripts\setup-windows-simulator.ps1
```

构建并启动默认仪表盘应用：

```powershell
.\scripts\run-simulator.ps1
```

启动 480×640 竖屏蓝牙音箱 Demo：

```powershell
.\scripts\run-simulator.ps1 -App bluetooth-speaker
```

首次上传到 GitHub 时，使用 `scripts/initialize-repository.ps1` 先合并远端已有的 `main` 历史；完整流程见 [Git 工作流](docs/WORKFLOW.md)。

选择应用、目标和 LVGL 大版本的统一接口：

```powershell
cmake -S . -B build/pc-dashboard -G Ninja `
  -DLVGL_APP=device-dashboard `
  -DLVGL_TARGET=pc-sdl2 `
  -DLVGL_SERIES=9
cmake --build build/pc-dashboard
ctest --test-dir build/pc-dashboard --output-on-failure
```

Linux fbdev/evdev 在 Linux 主机或交叉工具链环境执行：

```bash
./scripts/build-linux.sh device-dashboard
LVGL_CODEX_FBDEV=/dev/fb0 LVGL_CODEX_EVDEV=/dev/input/event0 ./build/linux-device-dashboard/apps/device-dashboard/device_dashboard
```

## 仓库结构

```text
framework/  运行时、纯 C 契约和 LVGL 大版本兼容层
ports/      PC SDL2、Linux fbdev/evdev、MCU 契约
boards/     硬件与工具链专属包
apps/       可独立构建、可 subtree 导出的产品 UI
templates/  新应用模板
cmake/      版本锁定与统一构建函数
docs/       架构、移植、升级和协作规范
```

详细约束见 [架构说明](docs/ARCHITECTURE.md)、[移植说明](docs/PORTING.md)、[LVGL 升级流程](docs/LVGL_UPGRADE.md) 与 [Git 工作流](docs/WORKFLOW.md)。
