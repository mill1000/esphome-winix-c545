#pragma once

#include <string>
#include <unordered_map>

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#ifdef USE_FAN
#include "esphome/components/fan/fan.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

namespace esphome {
namespace winix_c545 {

// Control keys
static constexpr const char *KEY_POWER = "A02";
static constexpr const char *KEY_AUTO = "A03";
static constexpr const char *KEY_SPEED = "A04";
static constexpr const char *KEY_PLASMAWAVE = "A07";

// Sensor keys
static constexpr const char *KEY_FILTER_AGE = "A21";
static constexpr const char *KEY_FILTER_LIFETIME = "P01";
static constexpr const char *KEY_AQI_INDICATOR = "S07";
static constexpr const char *KEY_AQI = "S08";
static constexpr const char *KEY_LIGHT = "S14";

enum class StateKey {
  // Control keys
  Power,
  Auto,
  Speed,
  Plasmawave,

  // Sensor keys
  FilterAge,
  FilterLifetime,
  AQIIndicator,
  AQI,
  Light
};

// Define an alias for map of device states
using WinixStateMap = std::unordered_map<StateKey, uint16_t>;

class WinixC545Fan;

class WinixC545Component : public uart::UARTDevice, public Component {
#ifdef USE_SENSOR
  SUB_SENSOR(filter_age)
  SUB_SENSOR(filter_lifetime)
  SUB_SENSOR(aqi)
  SUB_SENSOR(light)
#endif

#ifdef USE_TEXT_SENSOR
  SUB_TEXT_SENSOR(aqi_indicator)
#endif

#ifdef USE_SWITCH
  SUB_SWITCH(plasmawave)
#endif

 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void write_state(const WinixStateMap &);

#ifdef USE_FAN
  void set_fan(WinixC545Fan *fan) { this->fan_ = fan; };
#endif

 protected:
  const std::string RX_PREFIX{"AT*ICT*"};
  const std::string TX_PREFIX{"*ICT*"};

  static constexpr uint32_t MAX_LINE_LENGTH = 128;

  static const std::unordered_map<std::string, StateKey> ENUM_KEY_MAP;
  static const std::unordered_map<StateKey, std::string> STRING_KEY_MAP;

  enum class HandshakeState {
    Reset,
    DeviceReady,
    McuReady,
    MIB,
    Connected,
    ApReboot,
    ApDeviceReady,
    ApStart,
    ApStop,
  };

  HandshakeState handshake_state_{HandshakeState::Reset};
  uint32_t last_handshake_event_ = 0;

  WinixStateMap states_;
  uint32_t aqi_indicator_raw_value_ = 0;

  void update_handshake_state_();
  bool readline_(char, char *, int);
  void parse_sentence_(char *);
  void parse_aws_sentence_(char *);
  void publish_state_();
  void write_sentence_(const std::string &);

#ifdef USE_FAN
  WinixC545Fan *fan_{nullptr};
#endif
};

class WinixC545Fan : public fan::Fan, public Parented<WinixC545Component> {
 public:
  WinixC545Fan() {
    // Only support speed control with 4 levels: Low, Med, High, Turbo
    this->traits_ = fan::FanTraits(false, true, false, 4);
    // Add presets
    this->traits_.set_supported_preset_modes({PRESET_AUTO, PRESET_SLEEP});
  }

  fan::FanTraits get_traits() override { return this->traits_; }

  void dump_config();
  void update_state(const WinixStateMap &);

 protected:
  const std::string PRESET_NONE{""};
  const std::string PRESET_SLEEP{"Sleep"};
  const std::string PRESET_AUTO{"Auto"};

  void control(const fan::FanCall &call) override;
  fan::FanTraits traits_;
};

class WinixC545Switch : public switch_::Switch, public Parented<WinixC545Component> {
 public:
  WinixC545Switch(StateKey key, uint8_t on_value = 1, uint8_t off_value = 0) : key_(key), on_value_(on_value), off_value_(off_value) {}

 protected:
  void write_state(bool state) override;

  const StateKey key_;
  const uint8_t on_value_;
  const uint8_t off_value_;
};

class WinixC545PlasmawaveSwitch : public WinixC545Switch {
 public:
  WinixC545PlasmawaveSwitch() : WinixC545Switch(StateKey::Plasmawave) {}
};

}  // namespace winix_c545
}  // namespace esphome