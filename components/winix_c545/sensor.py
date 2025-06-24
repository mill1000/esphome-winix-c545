import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (DEVICE_CLASS_AQI, DEVICE_CLASS_DURATION,
                           ENTITY_CATEGORY_DIAGNOSTIC, STATE_CLASS_MEASUREMENT,
                           UNIT_EMPTY, UNIT_HOUR)

from . import CONF_WINIX_C545_ID, WinixC545Component

DEPENDENCIES = ["winix_c545"]

CONF_AQI = "aqi"
CONF_FILTER_AGE = "filter_age"
CONF_FILTER_LIFETIME = "filter_lifetime"
CONF_LIGHT = "light"
CONF_FAN_SPEED = "fan_speed"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
        cv.Optional(CONF_FILTER_AGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_FILTER_LIFETIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_AQI): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_AQI,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_LIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FAN_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        )
    }
)


async def to_code(config) -> None:
    component = await cg.get_variable(config[CONF_WINIX_C545_ID])

    if sensor_config := config.get(CONF_FILTER_AGE):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_filter_age_sensor(sens))

    if sensor_config := config.get(CONF_FILTER_LIFETIME):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_filter_lifetime_sensor(sens))

    if sensor_config := config.get(CONF_AQI):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_aqi_sensor(sens))

    if sensor_config := config.get(CONF_LIGHT):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_light_sensor(sens))

    if sensor_config := config.get(CONF_FAN_SPEED):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_fan_speed_sensor(sens))
