#pragma once

#include <string>

#include "esphome/components/fan/fan.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace winix_c545 {

class WinixC545Component : public fan::Fan, public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  fan::FanTraits get_traits() override;

 protected:
  // Fan control
  void control(const fan::FanCall &call) override;
  void write_state_();

  sensor::Sensor *aqi_sensor_{nullptr};
  sensor::Sensor *aqi_stoplight_sensor_{nullptr};

  sensor::Sensor *light_sensor_{nullptr};

  sensor::Sensor *filter_age_sensor_{nullptr};

  switch_::Switch *plasmawave_switch_ = {nullptr};

  // TODO the following belong as presets, not switches
  switch_::Switch *auto_switch_{nullptr};
  switch_::Switch *sleep_switch_{nullptr};
};

}  // namespace winix_c545
}  // namespace esphome