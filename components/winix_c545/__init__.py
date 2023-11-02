from typing import Optional

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, fan, sensor
from esphome.const import (
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_EMPTY,
)

CODEOWNERS = ["@mill1000"]
DEPENDENCIES = ["uart"]

winix_c545_ns = cg.esphome_ns.namespace("winix_c545")
WinixC545 = winix_c545_ns.class_("WinixC545Component", fan.Fan, uart.UARTDevice, cg.Component)

CONFIG_SCHEMA = (
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(WinixC545),
            # cv.Required("AQI"): sensor.sensor_schema(
            #     unit_of_measurement=UNIT_EMPTY,
            #     accuracy_decimals=0,
            #     state_class=STATE_CLASS_MEASUREMENT,
            # )
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    await uart.register_uart_device(var, config)

    # sensor = await cg.get_variable(config["AQI"])
    # cg.add(var.set_aqi_sensor(sensor))
