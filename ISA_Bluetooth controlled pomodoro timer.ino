#include <Wire.h>
#include <TM1650.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// assigning variable int for work time & break time
 int work_time_min;
 int break_time_min;

 //defined pushbutton for start, work and break time
const int PButton0 = 16;
const int PButton1 = 17;


short p_sec;
short p_min;
short type; 

// For 7 seg display function
TM1650 d;
char line1[1];
char line[3];


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32BT"); //Bluetooth device name
  Serial.println("The device has started");
  Wire.begin();
  d.init();
  SerialBT.begin();

//input from BT Serial
   while(SerialBT.available()) SerialBT.read();
    while(SerialBT.available() == 0){
      
    } 
  work_time_min = SerialBT.parseInt();
  SerialBT.println("work_time_min: ");
  SerialBT.println(work_time_min);


  while(SerialBT.available()) SerialBT.read();
    while(SerialBT.available() == 0){
      
    } 
  break_time_min = SerialBT.parseInt();
  SerialBT.println("break_time_min: ");
  SerialBT.println(break_time_min);


   
  pinMode(PButton0, INPUT);
  pinMode(PButton1, INPUT);
  while(digitalRead(PButton0) == 1);   // For starting the pomo timer 
}

void loop() { 
    p_sec++;
    delay(1000);
  if(p_sec == 60){
    p_sec = 0;
    p_min ++;
  }

  Serial.print("minute: ");
  Serial.println(p_min);
  Serial.print("sec: ");
  Serial.println(p_sec);

  if((p_min == work_time_min) && (type == 0)){
    p_min = 0;
    type = 1;
    d.displayOn();
    char pae[]="Brr";
    d.displayString(pae);
    d.setBrightnessGradually(TM1650_MAX_BRIGHT);
    delay(1000);
    Serial.println(p_min);
  Serial.println("Brr");
  while(digitalRead(PButton1) == 1);
  }
  if((p_min == break_time_min) && (type == 1)){
    p_min = 0;
    type = 0;
    d.displayOn();
    char paf[]="nrr";
    d.displayString(paf);
    d.setBrightnessGradually(TM1650_MAX_BRIGHT);
    delay(1000);
    Serial.println(p_min);
  Serial.println("---");
  while(digitalRead(PButton0) == 1);
  }

  d.displayOn();
   sprintf(line, "%3u", p_sec);
 d.displayString(line);
 sprintf(line1, "%1u", p_min);
  d.displayString(line1);
 d.setDot(0, true);
 d.setBrightnessGradually(TM1650_MAX_BRIGHT);
}
