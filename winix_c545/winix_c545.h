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

  // Functions for configuration
  void set_aqi_sensor(sensor::Sensor *sensor) { this->aqi_sensor_ = sensor; }
  void set_aqi_stoplight_sensor(sensor::Sensor *sensor);

  void set_light_sensor(sensor::Sensor *sensor);

  void set_filter_age_sensor(sensor::Sensor *sensor);

  void set_plasmawave_switch(switch_::Switch *switch);
  
  void set_auto_switch(switch_::Switch *switch);
  void set_sleep_switch(switch_::Switch *switch);

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