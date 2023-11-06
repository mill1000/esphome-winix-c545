import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan
from esphome.const import CONF_ID

from . import CONF_WINIX_C545_ID, WinixC545Component, winix_c545_ns

DEPENDENCIES = ["winix_c545"]

WinixC545Fan = winix_c545_ns.class_("WinixC545Fan", fan.Fan, cg.Component)

CONFIG_SCHEMA = (
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
            cv.GenerateID(CONF_ID): cv.declare_id(WinixC545Fan),
        }
    )
)


async def to_code(config) -> None:
    var = cg.new_Pvariable(config[CONF_ID])
    await fan.register_fan(var, config)
    await cg.register_parented(var, config[CONF_WINIX_C545_ID])

    component = await cg.get_variable(config[CONF_WINIX_C545_ID])
    cg.add(component.set_fan(var))
