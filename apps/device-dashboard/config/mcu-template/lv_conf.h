#ifndef LV_CONF_H
#define LV_CONF_H

/* Copy this profile into a board package and enable only the selected display
 * and input drivers. Never reuse a desktop or Linux profile on an MCU. */
#define LV_COLOR_DEPTH 16
#define LV_USE_OS LV_OS_NONE
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
#define LV_FONT_MONTSERRAT_20 1

#endif /* LV_CONF_H */
