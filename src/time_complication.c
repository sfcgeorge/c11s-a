#include <pebble.h>
#include "c11s-a.h"
#include "time_complication.h"

void time_complication_update(void) {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  snprintf(time_complication_year, 5, "%d", 1900 + tm->tm_year);
  snprintf(time_complication_month, 3, "%d", tm->tm_mon + 1);
  strncpy(time_complication_month_name, MONTH_NAMES[tm->tm_mon], 4);
  snprintf(time_complication_week, 3, "%d", (int)(tm->tm_yday / 7 + 1));
  snprintf(time_complication_year_day, 4, "%d", tm->tm_yday + 1);
  snprintf(time_complication_month_day, 3, "%d", tm->tm_mday);
  snprintf(time_complication_week_day, 2, "%d", tm->tm_wday + 1);
  strncpy(time_complication_day_name, DAY_NAMES[tm->tm_wday], 3);
  snprintf(time_complication_hour, 3, "%d", tm->hour);
  snprintf(time_complication_minute, 3, "%d", tm->min);
  snprintf(time_complication_second, 3, "%d", tm->sec);
  strncpy(time_complication_ampm, (tm->tm_hour < 12 ? "AM" : "PM"), 3);
  strncpy(time_complication_dst, (tm->tm_isdst == 1 ? "DST" : ""), 4);

  time_complication_hour_angle = (TRIG_MAX_ANGLE * (((tm->tm_hour % 12) * 6) + (tm->tm_min / 10))) / (12 * 6);
  time_complication_minute_angle = TRIG_MAX_ANGLE * tm->tm_min / 60;
  time_complication_second_angle = TRIG_MAX_ANGLE * tm->tm_sec / 60;
}

void time_complication_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_complication_update();
  window_render();
}
