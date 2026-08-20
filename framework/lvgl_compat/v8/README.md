# LVGL v8 compatibility boundary

This directory is reserved for the LVGL v8 adapter. Do not add `LVGL_VERSION_MAJOR`
checks to application or platform code. A v8 port is enabled only after it has a
pinned source release, target `lv_conf.h` profiles, adapter implementation, and CI job.
