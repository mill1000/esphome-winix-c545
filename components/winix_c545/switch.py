import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import DEVICE_CLASS_SWITCH

from . import CONF_WINIX_C545_ID, WinixC545Component, winix_c545_ns

DEPENDENCIES = ["winix_c545"]

WinixC545PlasmawaveSwitch = winix_c545_ns.class_(
    "WinixC545PlasmawaveSwitch", switch.Switch)
WinixC545AutoSwitch = winix_c545_ns.class_(
    "WinixC545AutoSwitch", switch.Switch)
WinixC545SleepSwitch = winix_c545_ns.class_(
    "WinixC545SleepSwitch", switch.Switch)

# TODO auto and sleep should be presets not switches but fan doesn't support presets currently
CONF_PLASMAWAVE = "plasmawave"
CONF_AUTO = "auto"
CONF_SLEEP = "sleep"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_WINIX_C545_ID): cv.use_id(WinixC545Component),
        cv.Optional(CONF_PLASMAWAVE): switch.switch_schema(
            WinixC545PlasmawaveSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon="mdi:lightning-bolt-outline",
        ),
        cv.Optional(CONF_AUTO): switch.switch_schema(
            WinixC545AutoSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon="mdi:auto-mode",
        ),
        cv.Optional(CONF_SLEEP): switch.switch_schema(
            WinixC545SleepSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon="mdi:sleep",
        )
    }
)


async def to_code(config) -> None:
    component = await cg.get_variable(config[CONF_WINIX_C545_ID])

    if switch_config := config.get(CONF_PLASMAWAVE):
        sw = await switch.new_switch(switch_config)
        await cg.register_parented(sw, config[CONF_WINIX_C545_ID])
        cg.add(component.set_plasmawave_switch(sw))

    if switch_config := config.get(CONF_AUTO):
        sw = await switch.new_switch(switch_config)
        await cg.register_parented(sw, config[CONF_WINIX_C545_ID])
        cg.add(component.set_auto_switch(sw))

    if switch_config := config.get(CONF_SLEEP):
        sw = await switch.new_switch(switch_config)
        await cg.register_parented(sw, config[CONF_WINIX_C545_ID])
        cg.add(component.set_sleep_switch(sw))
