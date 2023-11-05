#include "winix_c545.h"

#include "esphome/core/log.h"

namespace esphome {
namespace winix_c545 {

static const char *const TAG = "winix_c545";

// void WinixC545Component::on_plasmawave_state_(bool state) {
//   // TODO Send uart command and update fan state if necessary
// }

void WinixC545Component::setup() {
  // Restore state
  // auto restore = this->restore_state_();
  // if (restore.has_value()) {
  //   restore->apply(*this);
  //   this->write_state_();
  // }

  // TODO need to create switches first?? Switches
  // this->plasmawave_switch_.add_on_state_callback(this->on_plasmawave_state_);

  // Indicate device is ready
  // TODO base on wifi state?
  this->write_sentence_("DEVICEREADY");
  // Some subset of these may be needed too
  // *ICT*ASSOCIATED:0
  // *ICT*IPALLOCATED:10.100.1.250 255.255.255.0 10.100.1.1 10.100.1.6
  // *ICT*AWS_IND:MQTT OK
  // *ICT*AWS_IND:SUBSCRIBE OK
  // *ICT*AWS_IND:CONNECT OK
}

void WinixC545Component::write_sentence_(const char *sentence) {
  // Add TX prefix
  char buffer[MAX_LINE_LENGTH] = {0};
  strncpy(buffer, TX_PREFIX, sizeof(TX_PREFIX));

  // Copy sentence
  strncpy(buffer, sentence, sizeof(buffer) - sizeof(TX_PREFIX));

  // Send over UART
  ESP_LOGD(TAG, "Sending sentence: %s", buffer);
  this->write_str(buffer);
}

bool WinixC545Component::readline_(char data, char *buffer, int len) {
  static int pos = 0;

  // Read failed
  if (data < 0) return false;

  switch (data) {
    case '\n':  // Ignore new-lines
      break;

    case '\r': {  // Return on CR
      int rpos = pos;
      pos = 0;  // Reset position index ready for next time
      return rpos;
    }

    default:
      if (pos < len - 1) {
        buffer[pos++] = data;
        buffer[pos] = 0;
      }
      break;
  }

  // No end of line has been found
  return false;
}


void WinixC545Component::parse_aws_sentence_(const char *sentence) {
  uint16_t api_code = 0;
  if (sscanf(sentence, "AWS_SEND=A%3d", &api_code) != 1)
  {
    ESP_LOGE(TAG, "Failed to extract API code from sentence: %s", sentence);
    return;
  }
}

void WinixC545Component::parse_sentence_(const char *sentence) {
  ESP_LOGD(TAG, "Received sentence: %s", sentence);

  // Example eentence formats
  // AT*ICT*MCU_READY=1.2.0
  // AT*ICT*MIB=32
  // AT*ICT*SETMIB=18 C545
  // AT*ICT*AWS_SEND=A210 {"A02":"1","A03":"02","A04":"02","A05":"01","A07":"1","A21":"3706","S07":"01","S08":"97","S14":"34"}
  // AT*ICT*AWS_SEND=A220 {"S07":"01","S08":"116","S14":"34"}

  // Ensure sentence starts as expected
  if (strncmp(sentence, RX_PREFIX, sizeof(RX_PREFIX)) != 0) {
    ESP_LOGW(TAG, "Received invalid sentence: %s", sentence);
    return;
  }

  // Advance past prefix
  sentence += sizeof(RX_PREFIX);

  // Handle MCU_READY message
  if (strncmp(sentence, "MCU_READY", sizeof("MCU_READY")) == 0) {
    ESP_LOGI(TAG, "MCU_READY");
    this->write_sentence_("MCU_READY:OK");
    return;
  }

  // Handle MIB=32 message
  if (strncmp(sentence, "MIB=32", sizeof("MIB=32")) == 0) {
    ESP_LOGI(TAG, "MIB:OK");
    // 7595 is version of OEM wifi module
    this->write_sentence_("MIB:OK 7595");
    return;
  }

  // Handle SETMIB messages
  if (strncmp(sentence, "SETMIB", sizeof("SETMIB")) == 0) {
    ESP_LOGI(TAG, "SETMIB:OK");
    this->write_sentence_("SETMIB:OK");
    return;
  }

  // Handle SMODE messages
  if (strncmp(sentence, "SMODE", sizeof("SMODE")) == 0) {
    ESP_LOGI(TAG, "SMODE:OK");
    this->write_sentence_("SMODE:OK");
    return;
  }

  // Parse AWS sentences from MCU
  if (strncmp(sentence, "AWS_SEND", sizeof("AWS_SEND")) == 0)
    this->parse_aws_sentence_(sentence);

  ESP_LOGW(TAG, "Unsupported sentence: %s", sentence);  
}

void WinixC545Component::loop() {
  static char buffer[MAX_LINE_LENGTH];
  // TODO Read UART data here and publish values

  // TODO check available() against a min size?
  if (!this->available()) return;

  while (this->available()) {
    char data = this->read();
    bool found = this->readline_(data, buffer, MAX_LINE_LENGTH);
    if (!found) continue;

    // Complete line read
    this->parse_sentence_(buffer);
  }
}

void WinixC545Component::dump_config() {
  // TODO
}

fan::FanTraits WinixC545Fan::get_traits() {
  // Only support speed control with 4 levels: Low, Med, High, Turbo
  return fan::FanTraits(false, true, false, 4);
}

void WinixC545Fan::control(const fan::FanCall &call) {
  if (call.get_state().has_value()) this->state = *call.get_state();

  if (call.get_speed().has_value()) this->speed = *call.get_speed();

  this->write_state_();
  this->publish_state();
}

void WinixC545Fan::write_state_() {
  // TODO write UART commands here
}

}  // namespace winix_c545
}  // namespace esphome