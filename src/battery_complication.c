#include <pebble.h>
#include "c11s-a.h"
#include "battery_complication.h"
#include "time_complication.h"

void battery_complication_update(BatteryChargeState battery) {
  battery_complication_charge_percent = battery.charge_percent;
  battery_complication_is_charging = battery.is_charging;
  if (battery.is_charging) { strncpy(battery_complication_charger_connection, "Chg", 4); }
  else if (battery.is_plugged) { strncpy(battery_complication_charger_connection, "Plg", 4); }
  else { strncpy(battery_complication_charger_connection, "", 4); }

  if (battery_complication_charge_percent > 20) {
    tick_timer_service_subscribe(SECOND_UNIT, time_complication_handler);
  }
  else {
    tick_timer_service_subscribe(MINUTE_UNIT, time_complication_handler);
  }
}

void battery_complication_handler(BatteryChargeState battery) {
  battery_complication_update(battery);
  window_render();
}
