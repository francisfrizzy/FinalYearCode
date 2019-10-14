/*
  * Author: Francis Eseko
  * Std Num: ESKFRA001
  * University of Cape Town (UCT)
  * Code written for EEE4022S Final Year Project for the Automation of Streetlights
  * This code is written to be compiled and run on an Arduino Uno R3
*/
#include <OneWire.h>
#include <DallasTemperature.h>     //MAX31850 DallasTemp Library
#include <Adafruit_INA219.h>

//Using timers to concurrently control code
unsigned long prevtime_pir = 0;
unsigned long prevtime_ldr = 0;
unsigned long prevtime_rain = 0;
unsigned long prevtime_temp = 0;
unsigned long prevtime_current = 0;
unsigned long prevtime_serial = 0;

//constant assigned measurement intervals in ms. How long till next readings taken.
const unsigned long event_ldr = 3000;       //3s
const unsigned long event_pir = 4000;       //4s
const unsigned long event_rain = 10000;     //10s
const unsigned long event_temp = 4000;     //4s
const unsigned long event_current = 4000;   //4s
const unsigned long event_serial = 4000;    //4s

//DB assigned values - initialisations
String currentread = "";
String tempread = "";
String pirread = "";
String led_status = "1";    //operational LED

//Current sensor initialisation
Adafruit_INA219 ina219;
int ina219_led_output = 11;
int test_brightness = 200;  //testing power consumption with this PWM

//LDR and LED initialisations
int LDRpin = A0;       //Pin A0 where res and LDR connected
int led_output = 9;   //Pin 9 for LED
int ldrvalue = 0;        //Analog pin LDR reading value
int brightness = 0;     //Set PWM brightness for LED to zero

//PIR initialisations
int pirPin = 2;                    //PIR input pin
int pirStat = LOW;                 //PIR status

//Rain sensor initialisation
int rainPin = A5;                  //rain sensor analog input pin
int rainStat = 0;                     //Rain stat

//Temperature sensor initialisation
int ONE_WIRE_BUS = 10;                    //Pin 4 for temperature sensor. Dont forget 5k resistor attachment
OneWire oneWire(ONE_WIRE_BUS);     //setup a OneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);  //pass oneWire to Dallas temperature sensor

////Variables
//  bool started= false;
//  bool ended   = false; 
//  char incomingByte; //Variable to store the incoming byte
//  char msg[3];  //Message - array from 0 to 2 (3 values - PWM - e.g. 240)
//  byte index;  //Index of array

void setup() {
  uint32_t currentFrequency;  
  Serial.begin(9600);            //set serial port for communication
  sensors.begin();
  pinMode(led_output, OUTPUT);   //set pin 9 as an output
  pinMode(LDRpin, INPUT);   //set pin A0 as LDR input
  pinMode(pirPin, INPUT);           //set PIR pin as input
  pinMode(rainPin, INPUT);         //set A5 as rain sensor input
  pinMode(ina219_led_output, OUTPUT);   // set pin 5 as ina219 led output
  ina219.begin();            //initialising ina219
}

void loop() {
  /*
  *PWM ranges from 0 - 255 ; PWM pins: 3 5 6 9 10 11
  *ADC converts analog V from 0-5V into value in range 0-1023
  */
  /*
  while (Serial.available()>0){
    //Read the incoming byte
    incomingByte = Serial.read();
    if(incomingByte == '<')
    {
       started = true;
       index = 0;
       msg[index] = '\0';
     }
     else if(incomingByte == '>')
     {
       ended = true;
       break; }
     //Read the message!
     else
     {
       if(index < 4) // Make sure there is room
       {
         msg[index] = incomingByte; // Add char to array
         index++;
         msg[index] = '\0'; // Add NULL to end
       }}}
   
   if(started && ended){
     int value = atoi(msg);
     Serial.println(value); //Only for debugging
     index = 0;
     msg[index] = '\0';
     started = false;
     ended = false; }*/
  //end of test code for receiving data
  //beginning of sensor code
  unsigned long currentTime = millis();     //updates frequently

  //LDR event
  if (currentTime - prevtime_ldr >= event_ldr){
    ldrvalue = analogRead(LDRpin);
//    Serial.print("LDR measured value: ");        //print value to serial port
//    Serial.println(ldrvalue);
    if (ldrvalue <= 100){           //darkness
      brightness = 250;
    }else if (ldrvalue <=200){
      brightness = 150;
    }else if (ldrvalue <= 300){
      brightness = 80;
    }else {
      brightness = 0;    //) on bright environments
    }
    if (led_status == 1){
      analogWrite(led_output,  brightness);
    }
    //analogWrite(led_output, brightness);    //PWM: 0 - 255 (0 to 100% duty cycle)
    prevtime_ldr = currentTime;            //update timing for next event
  }

  //PIR event
  if (currentTime - prevtime_pir >= event_pir){    //check if 1s has passed
    pirStat = digitalRead(pirPin);   //read value from PIR
    if (pirStat == HIGH){
      pirread = "1";
      //Serial.println("Motion Detected!");
      prevtime_pir = currentTime;}        //update timing for next motion sensing
    else if(pirStat == LOW){
      pirread = "0";
      //Serial.println("No Motion Detected!");
      prevtime_pir = currentTime; }
  }
//
  //Rain sensor event
  if (currentTime - prevtime_rain >= event_rain){
    rainStat = analogRead(rainPin);  //read value from Rain Sensor
    int range = map(rainStat, 0, 1023, 0, 3);
    switch (range){
      case 0:
        //Serial.println("RAINING");
        if (ldrvalue <= 300){
          analogWrite(led_output,200);
          }
        prevtime_rain = currentTime;
        break;
      case 1:
        //Serial.println("RAIN WARNING");
        prevtime_rain = currentTime;
        break;
      case 2:
        //Serial.println("NOT RAINING");
        prevtime_rain = currentTime;
        break;
      }
  }

  //Temperature sensor event
  if (currentTime - prevtime_temp >= event_temp){
    sensors.requestTemperatures();    //call function to issue global temperature
    tempread = String(sensors.getTempCByIndex(0));
    prevtime_temp = currentTime;
  }

  //Current Sensor event - testing current sensor explicitly
  if (currentTime - prevtime_current >= event_current){
    analogWrite(ina219_led_output, test_brightness);    //PWM: 0 - 255 (0 to 100% duty cycle)
    //switch on LED then measure the values
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power = 0;
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    loadvoltage = busvoltage + (shuntvoltage / 1000);  //actual voltage on LED
    power = current_mA * loadvoltage;  //power consumed by LED
    currentread = String(power);
//    Serial.print("LED Voltage: "); Serial.print(loadvoltage); Serial.println(" V");
//    Serial.print("Current through LED: "); Serial.print(current_mA); Serial.println(" mA");
//    Serial.print("Power Consumption: "); Serial.print(power); Serial.println(" mW");
//    Serial.println("");
    
    prevtime_current = currentTime;
  }
  
  //testing XBee
  if (currentTime - prevtime_serial >= event_serial){
    String value = "node1 "+tempread+" "+pirread+" "+currentread+" "+led_status;
    Serial.println(value);
    prevtime_serial = currentTime;
  }
}
