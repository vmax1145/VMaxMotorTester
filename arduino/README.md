# Arduino HOWTO

### Installing ESP32 board manager
Open Arduino File->Preferences->Settings and add https://espressif.github.io/arduino-esp32/package_esp32_index.json
to Additional boards managerURLs  

### Installing required libraries 
Click on Library icon and add the following libraris   
 - Arduino_JSON by Arduino (version 0.2.0)
 - DShotRMT by Wastl Kraus  (version 0.2.0)
 - HX711 by Rob Tillaart  (version 0.6.3)
 - Preferences by Volodymyr Shymanskyy (version 2.2.2)
 - Simple Web Serial by Fabian Mohr  (version 1.0.0)

### Select board and port 
For ESP32S3 supermini open Select Board and Port dialog and search for
'esp32s3 dev module'. Connect USB cable and select COM port.   

### Tools settings
Just for info screenshot with settings in tools menu 
![img1](/arduino/img/tools.png)
 


