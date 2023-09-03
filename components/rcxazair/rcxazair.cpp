#include "rcxazair.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

#ifdef USE_ESP32

namespace esphome {
namespace rcxazair {

static const char *const TAG = "rcxazair";

#if 0
void Rcxazair::dump_config() {
  ESP_LOGCONFIG(TAG, "rcxazair");
  LOG_SENSOR(" ", "CO2", this->co2_sensor_);
}
#endif


void Rcxazair::gattc_event_handler(
        esp_gattc_cb_event_t event,
        esp_gatt_if_t gattc_if,
        esp_ble_gattc_cb_param_t *param)
{
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = espbt::ClientState::IDLE;
      if (this->co2_sensor_ != nullptr)
        this->co2_sensor_->publish_state(NAN);
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto chr = this->parent_->get_characteristic(0xc760, 0xc761);
      if (chr == nullptr) {
          ESP_LOGE(TAG, "[%s] No control service found at device, not an rcxazair?",
                   this->parent_->address_str().c_str());
        break;
      }
      ESP_LOGI(TAG, "[%s] got characteristic!",
              this->parent_->address_str().c_str());
      auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(), this->parent()->get_remote_bda(), chr->handle);
      if (status) {
        ESP_LOGE(TAG, "[%s] esp_ble_gattc_register_for_notify failed, status=%d",
                this->parent_->address_str().c_str(),
                status);
      }
      this->char_handle_ = chr->handle;
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::ESTABLISHED;
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      if (param->notify.handle != this->char_handle_)
        break;
#if 0
      ESP_LOGI(TAG, "[%s] Got notify, len %u",
              this->parent_->address_str().c_str(),
              param->notify.value_len);
#endif

      handle_message(param->notify.value, param->notify.value_len);

      break;
    }
    default:
      break;
  }
}

void Rcxazair::handle_message(uint8_t *msg, uint16_t len)
{
    if (len < 4)
        return;
    auto u16 = [msg](size_t offset) {
        return (msg[offset] << 8) + msg[offset+1];
    };
    uint32_t type = (msg[0] << 24)
                  + (msg[1] << 16)
                  + (msg[2] << 8)
                  + (msg[3]);
    switch (type) {
        case 0x23061004:
            if (len < 8)
                return;
            if (this->temp_sensor_) {
                uint16_t temp = u16(4); // TODO: is this unsigned?
                this->temp_sensor_->publish_state(temp * .1f);
            }
            if (this->relhum_sensor_) {
                uint16_t rel_hum = u16(6);
                this->relhum_sensor_->publish_state(rel_hum);
            }
            break;
        case 0x23081004:
            if (len < 10)
                return;
            if (this->co2_sensor_) {
                uint16_t co2_ppm = u16(4);
                this->co2_sensor_->publish_state(co2_ppm);
            }
            if (this->tvoc_sensor_) {
                uint16_t tvoc = u16(6);
                this->tvoc_sensor_->publish_state(tvoc);
            }
            if (this->hcho_sensor_) {
                uint16_t hcho = u16(8);
                this->hcho_sensor_->publish_state(hcho);
            }
            break;
        default:
        ESP_LOGI(TAG, "[%s] Got unknown message type %x",
                this->parent_->address_str().c_str(),
                type);
            break;
    }
}

}  // namespace rcxazair
}  // namespace esphome

#endif
