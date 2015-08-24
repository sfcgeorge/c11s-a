#pragma once

#include <pebble.h>

char bluetooth_complication_bluetooth_connection[4];

bool bluetooth_complication_bluetooth;

void bluetooth_complication_handler(bool bluetooth);
void bluetooth_complication_update(bool bluetooth);
