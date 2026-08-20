# Board packages

Put hardware-specific display timing, touch calibration, storage setup and
cross-toolchain configuration in `boards/<vendor>-<board>/`. Generic LVGL
ports belong in `ports/`; application code must not contain board constants.
