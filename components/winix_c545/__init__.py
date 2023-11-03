# from typing import Optional

# import esphome.codegen as cg
# import esphome.config_validation as cv
# from esphome.components import uart
# from esphome.const import CONF_ID

CODEOWNERS = ["@mill1000"]

# DEPENDENCIES = ["uart"]

# MULTI_CONF = True
# CONF_WINIX_C545_ID = "winix_c545_id"

# winix_c545_ns = cg.esphome_ns.namespace("winix_c545")
# WinixC545Component = winix_c545_ns.class_(
#     "WinixC545Component", uart.UARTDevice, cg.Component)

# CONFIG_SCHEMA = (
#     cv.Schema(
#         {
#             cv.GenerateID(): cv.declare_id(WinixC545Component),
#         }
#     )
#     .extend(uart.UART_DEVICE_SCHEMA)
#     .extend(cv.COMPONENT_SCHEMA)
# )


# async def to_code(config) -> None:
#     var = cg.new_Pvariable(config[CONF_ID])

#     await cg.register_component(var, config)
#     await uart.register_uart_device(var, config)
