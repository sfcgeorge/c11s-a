#include <pebble.h>
#include "c11s-a.h"
#include "time_complication.h"

void time_complication_update(void) {
  time_t now = time(NULL);
  time_complication_tm = localtime(&now);

  time_complication_hour_angle = (TRIG_MAX_ANGLE * (((time_complication_tm->tm_hour % 12) * 6) + (time_complication_tm->tm_min / 10))) / (12 * 6);
  time_complication_minute_angle = TRIG_MAX_ANGLE * time_complication_tm->tm_min / 60;
  time_complication_second_angle = TRIG_MAX_ANGLE * time_complication_tm->tm_sec / 60;
}

void time_complication_handler(struct tm *tick_time, TimeUnits units_changed) {
  time_complication_update();
  window_render();
}
