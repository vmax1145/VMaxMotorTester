#pragma once

#include <Preferences.h>
#include <HX711.h>


class ThrustSensor {

  
   public:
        

      void begin(uint8_t dataPin, uint8_t clockPin, float scale) {
         _sensor.begin(dataPin, clockPin);
         _sensor.tare(20);
         _preferences.begin("thrust_settings",true);
         float scaleFactor = _preferences.getFloat("scale", scale);         
         _preferences.end();
         _sensor.set_scale(scaleFactor);
      }

      void zero() {
         _sensor.tare(20);
      }
      
      void calibrateScale(float_t sampleWeight) {
           _sensor.calibrate_scale(sampleWeight, 20);
           float newscale = _sensor.get_scale();
          _preferences.begin("thrust_settings",false);
          _preferences.putFloat("scale",newscale);
          _preferences.end();
      }

      float_t measure() {
         return _sensor.get_units(1);            
      } 


    private:
      Preferences _preferences;
      HX711 _sensor;

   
};


