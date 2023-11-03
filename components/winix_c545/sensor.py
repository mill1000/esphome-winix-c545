import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import DEVICE_CLASS_DURATION, UNIT_HOUR


from . import CONF_WINIX_C545_ID, WinixC545Component

DEPENDENCIES = ["winix_c545"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
        cv.Optional("filter_age"): sensor.sensor_schema(
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_DURATION,
        ),
    }
)


async def to_code(config) -> None:
    component = await cg.get_variable(config[CONF_WINIX_C545_ID])

    if config := config.get("filter_age"):
        sens = await sensor.new_sensor(config)
        cg.add(component.set_filter_age_sensor(sens))
    
    
