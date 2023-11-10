import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import DEVICE_CLASS_SWITCH

from . import CONF_WINIX_C545_ID, WinixC545Component, winix_c545_ns

DEPENDENCIES = ["winix_c545"]

WinixC545PlasmawaveSwitch = winix_c545_ns.class_(
    "WinixC545PlasmawaveSwitch", switch.Switch)

CONF_PLASMAWAVE = "plasmawave"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
        cv.Optional(CONF_PLASMAWAVE): switch.switch_schema(
            WinixC545PlasmawaveSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon="mdi:lightning-bolt-outline",
        )
    }
)


async def to_code(config) -> None:
    component = await cg.get_variable(config[CONF_WINIX_C545_ID])

    if switch_config := config.get(CONF_PLASMAWAVE):
        sw = await switch.new_switch(switch_config)
        await cg.register_parented(sw, config[CONF_WINIX_C545_ID])
        cg.add(component.set_plasmawave_switch(sw))
