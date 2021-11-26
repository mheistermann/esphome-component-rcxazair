# About

This implements an esphome component to read data from a cheap air quality sensor that measures temperature, humidity, as well as, more interestingly, CO2, HCHO and TVOC concentrations.
I bought mine here, hopefully the link stays alive: https://aliexpress.com/item/4001342074070.html

"Air quality detector With Bluetooth 5in1 CO2 Meter Digital Temperature Humidity Sensor Tester Air Quality Monitor Carbon Dioxide TVOC HCHO Detector"

There is no clear manufacturer or device name, so this component takes its name from the companion Android app available at http://www.pgyer.com/qoWB

The default name advertised via Bluetooth for my device is "XS-####", where #### are the last 4 hex digits of its Bluetooth mac address.

# Usage

Add this to your esphome `.yaml`:

```

external_components:
  - source: github://mheistermann/esphome-component-rcxazair

ble_client:
  - mac_address: B6:6B:04:.....
    id: air_quality

sensor:
  - platform: rcxazair
    ble_client_id: air_quality
    co2:
      name: "CO2"
    tvoc:
      name: "TVOC"
    formaldehyde:
      name: "HCHO"
    temperature:
      name: "Temp"
    humidity:
      name: "Relhum"
```
