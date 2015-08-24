#include <pebble.h>
#include "c11s-a.h"
#include "time_complication.h"

void time_complication_update(void) {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  snprintf(time_complication_year, 5, "%d", 1900 + tm->tm_year);
  snprintf(time_complication_month, 3, "%d", tm->tm_mon + 1);

  time_complication_hour_angle = (TRIG_MAX_ANGLE * (((tm->tm_hour % 12) * 6) + (tm->tm_min / 10))) / (12 * 6);
  time_complication_minute_angle = TRIG_MAX_ANGLE * tm->tm_min / 60;
  time_complication_second_angle = TRIG_MAX_ANGLE * tm->tm_sec / 60;
}

void time_complication_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_complication_update();
  window_render();
}
