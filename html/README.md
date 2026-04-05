# HTML Application for [VmaxMotorTester project](../) 

## CAUTION !!! Be carefull when working with thrust stand. 
## Rotating propeller is extreemly dangerous !!!
## Connect power battery just before starting test and disconnect after test end.   

### Features
 - Automatically detect motor rotation direction and reverse it when necessary 
 - Measure sweeping throttle from defined min to defined max values
 - Display charts for thrust, RPM, current, voltage and power 
 - Save and load measured data to file
 
![screenshot_tester](/img/Screenshot_tester.png)

### Chrominum based browser required to run application
Modern WebSerialApi is essential for connectivity between motor tester stand and browser HTML application via USB.
This API is only supported in chromium based browsers like  
Google Chrome, Microsoft Edge, Opera, Brave browsers.
Please, choose right browser to run application.

### Load cell Calibration instructions.
SAFETY FIRST!!! DISCONNECT BATTERY POWER. 

Tie a thin nylon cord to the load cell exactly at the motor axis level 
(in the middle between 2 mount holes) and 
put free end of cord over pulley. 

Connect USB cable and start application.
Without any load on cord free end press 'Tare' button.
Wait 2-3 seconds... Measured weight must go to Zero.  
Next step - tie a known weight ( 1000 gramm for example ) to the free end of cord.
Enter this known weight value in 'Calibration' input field and press 'Set' button.
Now measured weight must be shown same as you entered (almost same) 
You are done.  
Remove calibration weight and cord to avoid getting hit by a spinning propeller during tests

Calibration coefficient is stored in ESP32 flash memory 
so you do not need to do this each time...



