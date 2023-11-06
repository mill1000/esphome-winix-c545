#pragma once

#include <map>
#include <string>

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#ifdef USE_FAN
#include "esphome/components/fan/fan.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

namespace esphome {
namespace winix_c545 {

class WinixC545Component;

// Define an alias for map of device states
using WinixStateMap = std::map<const std::string, uint16_t>;

class WinixC545Fan : public fan::Fan, public Parented<WinixC545Component> {
  fan::FanTraits get_traits() override {
    // Only support speed control with 4 levels: Low, Med, High, Turbo
    return fan::FanTraits(false, true, false, 4);
  }

 protected:
  void control(const fan::FanCall &call) override;
  void write_aws_sentence_(const WinixStateMap &);
};

class WinixC545Component : public uart::UARTDevice, public Component {
#ifdef USE_SENSOR
  SUB_SENSOR(filter_age)
  SUB_SENSOR(aqi)
  SUB_SENSOR(aqi_stoplight)
  SUB_SENSOR(light)
#endif

 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

#ifdef USE_FAN
  void set_fan(fan::Fan *fan) { this->fan_ = fan; };
#endif

#ifdef USE_SWITCH
  void set_plasmawave_switch(switch_::Switch *switch);
  void set_auto_switch(switch_::Switch *switch);
  void set_sleep_switch(switch_::Switch *switch);
#endif

  void write_sentence(const std::string &);

 protected:
  const std::string RX_PREFIX{"AT*ICT*"};
  const std::string TX_PREFIX{"*ICT*"};

  static constexpr uint32_t MAX_LINE_LENGTH = 255;
  bool readline_(char, char *, int);
  void parse_sentence_(const char *);
  void parse_aws_sentence_(const char *);
  void update_state_(const WinixStateMap &);

#ifdef USE_FAN
  fan::Fan *fan_{nullptr};
#endif

#ifdef USE_SWITCH
  switch_::Switch *plasmawave_switch_{nullptr};

  // TODO the following belong as presets, not switches
  switch_::Switch *auto_switch_{nullptr};
  switch_::Switch *sleep_switch_{nullptr};
#endif
};

}  // namespace winix_c545
}  // namespace esphome