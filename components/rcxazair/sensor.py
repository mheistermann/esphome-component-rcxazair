import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, ble_client
from esphome.const import (
    CONF_ID,
    #CONF_BATTERY_LEVEL,
    #DEVICE_CLASS_BATTERY,
    #ENTITY_CATEGORY_DIAGNOSTIC,
    CONF_TEMPERATURE,
    DEVICE_CLASS_TEMPERATURE,
    UNIT_CELSIUS,
    ICON_THERMOMETER,

    CONF_HUMIDITY,
    DEVICE_CLASS_HUMIDITY,
    UNIT_PERCENT,
    ICON_WATER_PERCENT,

    CONF_CO2,
    UNIT_PARTS_PER_MILLION,
    ICON_MOLECULE_CO2,

    CONF_TVOC,
    CONF_FORMALDEHYDE,
    UNIT_MICROGRAMS_PER_CUBIC_METER,
    ICON_CHEMICAL_WEAPON,
)

CODEOWNERS = ["@mheistermann"]

rcxazair_ns = cg.esphome_ns.namespace("rcxazair")
Rcxazair = rcxazair_ns.class_("Rcxazair", ble_client.BLEClientNode, cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Rcxazair),
            #cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
            #    unit_of_measurement=UNIT_PERCENT,
            #    device_class=DEVICE_CLASS_BATTERY,
            #    accuracy_decimals=0,
            #    entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            #),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=1,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_WATER_PERCENT,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_CO2): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                icon=ICON_MOLECULE_CO2,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_TVOC): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=0,
            ),
            cv.Optional(CONF_FORMALDEHYDE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                icon=ICON_CHEMICAL_WEAPON,
                accuracy_decimals=0,
            ),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield ble_client.register_ble_node(var, config)

    #if CONF_BATTERY_LEVEL in config:
    #    sens = yield sensor.new_sensor(config[CONF_BATTERY_LEVEL])
    #    cg.add(var.set_battery(sens))

    if CONF_CO2 in config:
        sens = yield sensor.new_sensor(config[CONF_CO2])
        cg.add(var.set_co2_sensor(sens))

    if CONF_TVOC in config:
        sens = yield sensor.new_sensor(config[CONF_TVOC])
        cg.add(var.set_tvoc_sensor(sens))

    if CONF_FORMALDEHYDE in config:
        sens = yield sensor.new_sensor(config[CONF_FORMALDEHYDE])
        cg.add(var.set_hcho_sensor(sens))

    if CONF_TEMPERATURE in config:
        sens = yield sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temp_sensor(sens))

    if CONF_HUMIDITY in config:
        sens = yield sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_relhum_sensor(sens))

