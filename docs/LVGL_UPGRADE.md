# LVGL 升级规则

1. 在 `cmake/dependencies/lvgl_versions.cmake` 新增官方 URL、精确 SHA256 和版本号。
2. 为每个受支持 target 验证对应 `lv_conf.h`。
3. 只在 `framework/lvgl_compat/v<major>/` 处理大版本 API 差异；禁止在 app 目录散布版本宏。
4. 完成 PC、Linux 编译和逻辑测试后，以新的框架语义化版本标签发布。
5. 应用升级框架版本时，单独提交 `framework_lock.cmake` 的变更并重新验证。

v9.5.0 是当前基线。v8 尚未标记为受支持版本。
