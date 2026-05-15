#include <Constants.h>
#include <HX711.h>
#include <DShotRMT.h>
#include "Thrust_sensor.h"
#include <Arduino_JSON.h>
#include <SimpleWebSerial.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>



SimpleWebSerial WebSerial;
JSONVar readings;

ThrustSensor thrustSensor;

/*
ESP32 S3 Supermini 
Safe to use pins
IO1
IO2
IO4
IO5
IO6
IO7
IO8
IO15
IO16
IO17
IO18
IO21

Pins to use with cautions
IO3	GPIO3	Sampled at reset to select JTAG interface (USB Serial/JTAG controller vs. external pins). Improper use can disable external JTAG or alter debug interface.	🛠️ Strapping
IO9	FSPIHD	Connected to external flash (data/hold signal) on most modules. Not recommended for use as GPIO, since it must remain dedicated to flash communication.	⚡ Flash
IO10	FSPICS0	Used to select the external flash chip. It is required for flash access and cannot be repurposed without losing flash connectivity	⚡ Flash
IO11	FSPID	Used as a data line for flash (and in-package PSRAM). It should not be used as GPIO when the flash/PSRAM is in use.	⚡ Flash
IO12	FSPICLK	Drives the flash (and PSRAM) clock. This critical signal must be reserved for memory and not used as general GPIO.	⚡ Flash
*/



#define HX711_DATA_PIN 1
#define HX711_CLK_PIN 2
#define MOTOR01_PIN GPIO_NUM_3 // unsafe pin?
#define GY906_DATA GPIO_NUM_4
#define GY906_CLK GPIO_NUM_5
#define TELEMETRY_RX_PIN GPIO_NUM_6
#define TELEMETRY_TX_PIN GPIO_NUM_7 //dummy any free pin


//static constexpr gpio_num_t MOTOR01_PIN = GPIO_NUM_3;
static constexpr dshot_mode_t DSHOT_MODE = DSHOT300;
static constexpr auto IS_BIDIRECTIONAL = false;

#define THRUST_SCALE_DEFAULT  1931.0

#define MLX90614_I2C_ADRR 0x5a

static uint16_t throttle = DSHOT_CMD_MOTOR_STOP;

unsigned long lastTime = 0;
unsigned long timerDelay = 200;
static int16_t step = 0;

bool mlxFound = false;


char termbuff[64];
static uint8_t telemetryBuff[10];

volatile uint8_t measureInx=0;
volatile uint16_t temperature[4] ={0,0,0,0};
volatile uint16_t voltage[4] = {0,0,0,0};
volatile uint16_t current[4] = {0,0,0,0};
volatile uint16_t consumed[4] = {0,0,0,0}; // used mA/h
volatile uint16_t erpm[4] = {0,0,0,0};


DShotRMT motor01(MOTOR01_PIN, DSHOT_MODE, IS_BIDIRECTIONAL);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
int16_t motTemp = -255;
int16_t ambTemp = -255;

uint16_t avgMeasure(volatile uint16_t *measureData) {
    uint16_t ret = 0;
    for(uint8_t i=0;i<4;i++) {
        ret+=measureData[i];
    }
    return ret/4;
}


float prevThrust = 0;
JSONVar getSensorReadings(){
    float thrust = thrustSensor.measure();
    readings["thrust"]  =  (prevThrust+thrust)/2.0;
    prevThrust = thrust;
    readings["voltage"] = avgMeasure(voltage);
    readings["current"] = avgMeasure(current);
    readings["erpm"] = avgMeasure(erpm);
    readings["temp"] = avgMeasure(temperature);
    readings["consumed"] = avgMeasure(consumed);
    if(mlxFound) {
        readings["mT"] = motTemp;
        readings["aT"] = ambTemp;
    }
    return readings;
}


void parseCommand(JSONVar myObject) {    

    if(JSON.typeof(myObject) == "undefined") {
        WebSerial.send("term","undefined command");
    }
    else{
        if(myObject.hasOwnProperty("tare")) {
            thrustSensor.zero();
            WebSerial.send("term","Thrust zero set");
        }
        if(myObject.hasOwnProperty("thrust")) {
            double v= (double)(myObject["thrust"]);
            float_t val = float(v);
            thrustSensor.calibrateScale(val);            
            sprintf(termbuff, "Thrust set at %i", val);
            WebSerial.send("term",termbuff);
        }
        if(myObject.hasOwnProperty("throttle")) {                        
            double v= (double)(myObject["throttle"]);
            if(v==0) {
               throttle=0;
            }
            else {
               throttle = DSHOT_THROTTLE_MIN + v*(DSHOT_THROTTLE_MAX-DSHOT_THROTTLE_MIN)/100;
            }            
            sprintf(termbuff, "Throttle %f", v);
            WebSerial.send("term",termbuff);
        }
        if(myObject.hasOwnProperty("spin")) {            
            bool v = ((int)(myObject["spin"]))>0;            
            
            motor01.sendCustomCommand(DSHOT_CMD_MOTOR_STOP, 10, 1000);            
            motor01.sendCustomCommand(v ? DSHOT_CMD_SPIN_DIRECTION_NORMAL : DSHOT_CMD_SPIN_DIRECTION_REVERSED, 10, 1000);            
            motor01.sendCustomCommand(DSHOT_CMD_SAVE_SETTINGS, 10, 1000);            
            sprintf(termbuff, "Spin dir %s", v ? "forward":"reversed");
            WebSerial.send("term",termbuff);
        }
        if(myObject.hasOwnProperty("echo")) {
            String v = (String)(myObject["echo"]);
            WebSerial.send("term","ECHO:"+v);
        }
    }
}


uint8_t update_crc8(uint8_t crc, uint8_t crc_seed){
	uint8_t crc_u, i;
	crc_u = crc;
	crc_u ^= crc_seed;
	for ( i=0; i<8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
	return (crc_u);
}

uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen) {
	uint8_t crc = 0, i;
	for( i=0; i<BufLen; i++) crc = update_crc8(Buf[i], crc);
	return (crc);
}

void onKiss() {
    uint8_t receivedCnt = Serial1.available();
    if(receivedCnt != 10) {
        while(receivedCnt > 0) {
            Serial1.read();
            receivedCnt--;
        }
        return;
    }
    else {
        Serial1.read(telemetryBuff,10);
        uint8_t crc8 = get_crc8(telemetryBuff, 9); // get the 8 bit CRC
        if(crc8 != telemetryBuff[9]) {
            return;
        }
        temperature[measureInx] = telemetryBuff[0]; // temperature
        voltage[measureInx] = (telemetryBuff[1]<<8)|telemetryBuff[2]; // voltage        
        current[measureInx] = (telemetryBuff[3]<<8)|telemetryBuff[4]; // Current
        consumed[measureInx] = (telemetryBuff[5]<<8)|telemetryBuff[6]; // used mA/h
        erpm[measureInx] = (telemetryBuff[7]<<8)|telemetryBuff[8]; // eRpM *100
        measureInx = (measureInx+1)%4;
    }
}

void setup() {
   Serial.begin(115200);

   WebSerial.on("cmd", parseCommand);  
   thrustSensor.begin(HX711_DATA_PIN,HX711_CLK_PIN, THRUST_SCALE_DEFAULT);
   motor01.begin();

   Serial1.begin(115200,SERIAL_8N1,TELEMETRY_RX_PIN,TELEMETRY_TX_PIN);
   Serial1.setRxTimeout(5);
   Serial1.onReceive(onKiss,false);

   Wire.begin(GY906_DATA, GY906_CLK);
   if (mlx.begin(MLX90614_I2C_ADRR, &Wire) ) {
       mlxFound = true;
   };
}




void loop() {
    unsigned long delta = millis() - lastTime; 
        
    WebSerial.check();
    motor01.sendThrottle(throttle);

    if(mlxFound){        
        if(step==1) {
            motTemp = mlx.readObjectTempC()*10.0;
        }
        else if(step==2) {
            ambTemp = mlx.readAmbientTempC()*10.0;
        }
    }
    step++;
    if (delta > timerDelay) {              
       JSONVar sensorReadings = getSensorReadings();
       WebSerial.send("value", sensorReadings);       
       lastTime = millis();                      
       step = 0;
    }
}