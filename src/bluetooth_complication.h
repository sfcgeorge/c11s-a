#pragma once

#include <pebble.h>

bool bluetooth_complication_bluetooth;

void bluetooth_complication_handler(bool bluetooth);
void bluetooth_complication_update(bool bluetooth);
