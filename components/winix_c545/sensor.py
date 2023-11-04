import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import DEVICE_CLASS_DURATION, UNIT_HOUR, UNIT_EMPTY, DEVICE_CLASS_AQI


from . import CONF_WINIX_C545_ID, WinixC545Component

DEPENDENCIES = ["winix_c545"]

CONF_AQI = "aqi"
CONF_AQI_STOPLIGHT = "aqi_stoplight"
CONF_FILTER_AGE = "filter_age"
CONF_LIGHT = "light"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
        cv.Optional(CONF_FILTER_AGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
        ),
        cv.Optional(CONF_AQI): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_AQI,
        ),
        cv.Optional(CONF_AQI_STOPLIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_AQI, # TODO Use ENUM instead?
        ),
        cv.Optional(CONF_LIGHT): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0
        ),
    }
)


async def to_code(config) -> None:
    component = await cg.get_variable(config[CONF_WINIX_C545_ID])

    if sensor_config := config.get(CONF_FILTER_AGE):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_filter_age_sensor(sens))

    if sensor_config := config.get(CONF_AQI):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_aqi_sensor(sens))

    if sensor_config := config.get(CONF_AQI_STOPLIGHT):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_aqi_stoplight_sensor(sens))

    if sensor_config := config.get(CONF_LIGHT):
        sens = await sensor.new_sensor(sensor_config)
        cg.add(component.set_life_sensor(sens))
