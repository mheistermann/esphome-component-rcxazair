#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/sensor/sensor.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace rcxazair {

namespace espbt = esphome::esp32_ble_tracker;

class Rcxazair : public esphome::ble_client::BLEClientNode
               , public Component
{
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event,
                           esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  //void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void set_temp_sensor(sensor::Sensor *s) { temp_sensor_ = s; }
  void set_relhum_sensor(sensor::Sensor *s) { relhum_sensor_ = s; }
  void set_co2_sensor(sensor::Sensor *s) { co2_sensor_ = s; }
  void set_tvoc_sensor(sensor::Sensor *s) { tvoc_sensor_ = s; }
  void set_hcho_sensor(sensor::Sensor *s) { hcho_sensor_ = s; }

 protected:
  void handle_message(uint8_t *msg, uint16_t len);
  uint16_t char_handle_;
  sensor::Sensor *temp_sensor_{nullptr};
  sensor::Sensor *relhum_sensor_{nullptr};
  sensor::Sensor *co2_sensor_{nullptr};
  sensor::Sensor *tvoc_sensor_{nullptr};
  sensor::Sensor *hcho_sensor_{nullptr};
};

}  // namespace rcxazair
}  // namespace esphome

#endif
