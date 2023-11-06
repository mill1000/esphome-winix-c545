import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor

from . import CONF_WINIX_C545_ID, WinixC545Component

DEPENDENCIES = ["winix_c545"]

CONF_AQI_INDICATOR = "aqi_indicator"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
        cv.Optional(CONF_AQI_INDICATOR): text_sensor.text_sensor_schema(
            icon="mdi:air-filter"
        )
    }
)


async def to_code(config) -> None:
    component = await cg.get_variable(config[CONF_WINIX_C545_ID])

    if sensor_config := config.get(CONF_AQI_INDICATOR):
        sens = await text_sensor.new_text_sensor(sensor_config)
        cg.add(component.set_aqi_indicator_text_sensor(sens))
