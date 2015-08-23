#include <pebble.h>
#include "c11s-a.h"
#include "bluetooth_complication.h"

void bluetooth_complication_update(bool bluetooth) {
  bluetooth_complication_bluetooth = bluetooth;
}

void bluetooth_complication_handler(bool bluetooth) {
  bluetooth_complication_update(bluetooth);
  window_render();
}

