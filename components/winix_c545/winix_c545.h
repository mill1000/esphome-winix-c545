#pragma once

#include <string>

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

namespace esphome {
namespace winix_c545 {

// class WinixC545Fan : public fan::Fan {
//  public:
//   // void setup() override;
//   // void loop() override;
//   // void dump_config() override;

//   fan::FanTraits get_traits() override;

//  protected:
//   // Fan control
//   void control(const fan::FanCall &call) override;
//   void write_state_();
// };

class WinixC545Component : public fan::Fan, public uart::UARTDevice, public Component {
 #ifdef USE_SENSOR
  SUB_SENSOR(filter_age)
#endif

 
 
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  fan::FanTraits get_traits() override;

#ifdef USE_SENSOR
  // Functions for configuration
  void set_aqi_sensor(sensor::Sensor *sensor) { this->aqi_sensor_ = sensor; }
  void set_aqi_stoplight_sensor(sensor::Sensor *sensor);
  void set_light_sensor(sensor::Sensor *sensor);
#endif

#ifdef USE_SWITCH
  void set_plasmawave_switch(switch_::Switch *switch);
  void set_auto_switch(switch_::Switch *switch);
  void set_sleep_switch(switch_::Switch *switch);
#endif

 protected:
  static constexpr uint32_t MAX_LINE_LENGTH = 255;
  bool readline_(char, char *, int);

  // Fan control
  void control(const fan::FanCall &call) override;
  void write_state_();

#ifdef USE_SENSOR
  sensor::Sensor *aqi_sensor_{nullptr};
  sensor::Sensor *aqi_stoplight_sensor_{nullptr};

  sensor::Sensor *light_sensor_{nullptr};
#endif

#ifdef USE_SWITCH
  switch_::Switch *plasmawave_switch_ = {nullptr};

  // TODO the following belong as presets, not switches
  switch_::Switch *auto_switch_{nullptr};
  switch_::Switch *sleep_switch_{nullptr};
#endif
};

}  // namespace winix_c545
}  // namespace esphome