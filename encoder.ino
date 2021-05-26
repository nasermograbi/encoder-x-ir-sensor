#include <Arduino.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include "ArduinoMotorShieldR3.h"


#define joyX A9

ArduinoMotorShieldR3 md;

ros::NodeHandle  nh;

const int ProxSensor=24;
const byte interruptPin = 18;

volatile byte state = LOW;
int sum;
int dir;
int deg;
const double y = 1.27;
int xInput = 0;
int flag = 0;

void vel_Subscriber(const std_msgs::Int16& msg) {
  xInput = msg.data;
}

ros::Subscriber<std_msgs::Int16> sub("chatter", vel_Subscriber);

void setup() {

  nh.initNode();
  nh.getHardware()-> setBaud(57600);
  nh.subscribe(sub);
  
  md.init();
  nulirai();
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), plusOne, CHANGE);
  
}

void loop() {
  
  void vel_Subscriber(const std_msgs::Int16& msg);
  

  deg = xInput;
  
  
  
  proceed(deg);
  Serial.print("flag -> ");
  Serial.println(flag);

  Serial.print("sum -> ");
  Serial.println(sum);

  Serial.print("deg -> ");
  Serial.println(deg);
}

void plusOne() {
  sum += dir;
}

void proceed(int deg) {
  int pulse = deg * y;
  if(deg > flag) {
    while(flag < deg){
      dir = 1;
      md.setM1Speed(375);
        if(sum > pulse) {
        md.setM1Speed(0);
       
       flag = deg;
       
      }
    }
  }
  if (deg < flag){
    while(deg < flag){
      dir = -1;
      md.setM1Speed(-375);
        if(sum < pulse) {
        md.setM1Speed(0);
        flag = deg;
        
       
      }
    }
  }
  else{
      md.setM1Speed(0);
    }
  nh.spinOnce();
   
}

void nulirai() {
  while(digitalRead(ProxSensor)!= HIGH) {
    md.setM1Speed(375);
  }
}
