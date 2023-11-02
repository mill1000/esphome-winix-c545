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
    // this->parse_sentence_(buffer);
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