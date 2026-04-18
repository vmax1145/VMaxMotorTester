# VMaxMotorTester DIY brushless motors thrust stand

## CAUTION !!! Be carefull when working with thrust stand. Rotating propeller is extreemly dangerous !!!  


Thrust stand is implemented with ESP32S3 supermini board to control Tekko32 F4 AM32 ESC 
and read extended KISS telemetry data as well as data from HX711 connected to load cell.
Collected data transmitted via USB serial to HTML page for visualization and control.
![screenshot_tester](/img/Screenshot_tester.png)

![img1](/img/assembly.png) ![img2](/img/assembled.png) ![img3](/img/opened.png) 

## Project include
- [Arduino sketch for ESP32S3 supermini board](arduino/README.md)
- [HTML application](html/README.md) (Chromium based browser required)
- STL files for 3d printed stand parts
- Autodesk Fusion360 3d model source file 

## Requirements
- Before wiring connect ESC to [AM32 configurator](https://am32.ca/configurator) and ensure 30ms telemetry on ESC telemetry output is enabled 
- Chromium based browser required to run HTML application.

## Wiring
![scheme](/img/connections.png)

## Bill of materials
- [ESP32S3 supermini board](/img/esp32s3supermini.png)
- [HX711 amplifier and Load cell](/img/load_cell_HX711.png) Choose 1/2/5/10 KG load cell depending on your requirements. 
- [Tekko32 F4 AM32 ESC](/img/tekko32_bom.png) Or other ESC supporting full extended telemetry (erpm/temperature/current/voltage/consumption) on separate wire with KISS protocol.
- [Amass male XT60 connector](/img/xt60.jpg)
- [GT2 idler pulley 20T smooth, bore 3mm](/img/pulley.png) 
- [WP3-19 spring load connector](/img/WP3-19.png) Optional, but recommended.
- [2020 alluminium profile](/img/2020.png) 250-330mm length sutable for most cases up to 8-10 inch propellers. But for large propellers you may need more.
- [Clamp alluminium](/img/clamp.png) Printed parts are designed to match this specific clamp sizes
- [M2.5 inserts ⌀4mm l=5mm](/img/m3_inserts.png) (8 pcs)
- [M3 inserts ⌀4mm l=5mm](/img/m3_inserts.png) (2 pcs)
- [M3 profile nuts for 2020 profile](/img/M3_2020_nuts.png) (6pcs)
- AWG16 and AWG28 silicone wires
- M3, M2.5 bolts and nuts 

## Known issues
- Due to poor design of current sensor in Tekko32 F4 AM32 
it has deadzone at low currens below 1-1.5A, this also lower down measured value aproximatly the same value.