#pragma once

#include <pebble.h>

uint8_t battery_complication_charge_percent;
bool battery_complication_is_charging;

void battery_complication_handler(BatteryChargeState battery);
void battery_complication_update(BatteryChargeState battery);
