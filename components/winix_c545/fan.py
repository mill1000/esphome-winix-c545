import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan, uart, sensor
from esphome.const import CONF_ID, DEVICE_CLASS_DURATION, UNIT_HOUR, STATE_CLASS_MEASUREMENT

AUTO_LOAD = ["sensor"]
DEPENDENCIES = ["uart"]

winix_c545_ns = cg.esphome_ns.namespace("winix_c545")
WinixC545Component = winix_c545_ns.class_(
    "WinixC545Component", fan.Fan, uart.UARTDevice, cg.Component)

CONFIG_SCHEMA = (
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(WinixC545Component),
            cv.Optional("filter_age"): sensor.sensor_schema(
            unit_of_measurement=UNIT_HOUR,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config) -> None:
    var = cg.new_Pvariable(config[CONF_ID])
    await fan.register_fan(var, config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if "filter_age" in config:
        sens = await sensor.new_sensor(config["filter_age"])
        cg.add(var.set_filter_age_sensor(sens))