# 架构边界

`domain/` 是纯 C 业务状态和用例，不能包含 LVGL、SDL、Linux 或板级头文件。

`application/` 是 Presenter/Controller：接收 UI 语义动作，调用领域用例，并把领域状态映射为纯 C 视图模型。它可以引用 UI 的动作和视图模型声明，但不能引用 UI 实现。

`ui/` 只处理 LVGL 对象、页面和组件；它只能发出语义动作，不能包含 `domain/` 头文件或调用设备服务。

`framework/contracts/` 提供不依赖 LVGL 的应用生命周期和运行结果码；各应用的语义动作与视图模型同样必须保持为纯 C 数据。`framework/runtime/` 只负责 LVGL 生命周期和主循环，`framework/lvgl_compat/` 是唯一可处理 LVGL 大版本 API 差异的位置。`ports/` 负责 SDL、Linux 和未来 MCU 的显示、输入和时基；`boards/` 保存具体硬件常量、驱动初始化和工具链。

每次新增页面都在目标应用的 `ui/` 添加页面，再通过 `application/` 添加动作处理和视图模型。不得让 UI 回调直接修改领域对象。
