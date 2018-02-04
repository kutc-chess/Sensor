#include<wiringPi.h>
#include"E6CP.hpp"

E6CP::E6CP(int pin[8]){
  //Decide PinNumber
  for(int i = 0; i < 8; i++){
    absolute[i] = pin[i];
  }

  //Create Macro
  for(unsigned char i = 0;; ++i){
    Change[i ^ (i >> 1)] = i;
    if(i == 255){
      break;
    }
  }

  //Pin Config
  wiringPiSetupGpio();
  for(int i = 0; i < 8; ++i){
    pinMode(absolute[i], INPUT);
  }
  wiringPiISR(absolute[7], INT_EDGE_BOTH, readSpecial());
}

long E6CP::get(){
  static unsigned char rotaryDummy;
  rotaryDummy = 0;
  for(unsigned char i = 0; i < 8; ++i){
    rotaryDummy += !digitalRead(absolute[i]) << i;
  }
  rotaryNow = Change[rotaryDummy];
  totalNow += rotaryNow - rotaryPrev;
  rotaryPrev = rotaryNow;
  return totalNow;
}

void E6CP::readSpecial(){
  static unsigned char rotaryDummy;
  //0 -> 255
  if(!digitalRead(absolute[7])){
    rotaryDummy = 0;
    for(unsigned char i = 0; i < 8; ++i){
      rotaryDummy += !digitalRead(absolute[i]) << i;
    }
    rotaryNow = Change[rotaryDummy];
    totalNow += rotaryNow - rotaryPrev;
    rotaryPrev = rotaryNow;
    if(rotaryPrev == 255){
      totalNow -= 256;
    }
  }
  //255 -> 0
  else{
    rotaryDummy = 0;
    for(unsigned char i = 0; i < 8; ++i){
      rotaryDummy += !digitalRead(absolute[i]) << i;
    }
    rotaryNow = Change[rotaryDummy];
    totalNow += rotaryNow - rotaryPrev;
    rotaryPrev = rotaryNow;
    if(rotaryPrev == 0){
      totalNow += 256;
    }
  }
}

