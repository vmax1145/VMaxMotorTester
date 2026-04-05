# VMaxMotorTester DIY brushless motors thrust stand
Thrust stand is implemented with ESP32S3 supermini board to control Tekko32 F4 AM32 ESC 
and read extended KISS telemetry data as well as data from HX711 connected to load cell.
Collected data transmitted via USB serial to HTML page for visualization and control.
![screenshot_tester](/img/Screenshot_tester.png)

![img1](/img/assembly.png) ![img2](/img/assembled.png) ![img3](/img/opened.png) 

## Project include
- [Arduino sketch for ESP32S3 supermini board](arduino/README.md)
- HTML page application (Chromium based browser required)
- STL files for 3d printing stand parts

## Requirements
- Before wiring connect ESC to AM32 configurator to enable 30ms telemetry on ESC telemetry output
- Chromium based browser required to run HTML application.

## Wiring
![scheme](/img/connections.png)
