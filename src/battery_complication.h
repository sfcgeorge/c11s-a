#pragma once

#include <pebble.h>

char battery_complication_charge_level[5];
char battery_complication_charger_connection[4];

uint8_t battery_complication_charge_percent;
bool battery_complication_is_charging;
bool battery_complication_is_plugged;

void battery_complication_handler(BatteryChargeState battery);
void battery_complication_update(BatteryChargeState battery);
