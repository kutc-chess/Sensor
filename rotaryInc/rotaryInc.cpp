#include"rotaryInc.hpp"
#include<pigpio.h>
#include<iostream>
#include<atomic>

using namespace std;

rotaryInc::rotaryInc(int userA, int userB, bool precision){
  pinA = userA;
  pinB = userB;

  gpioSetMode(pinA, PI_INPUT);
  gpioSetMode(pinB, PI_INPUT);
  
  nowA = gpioRead(pinA);
  nowB = gpioRead(pinB);

  if(precision){
    gpioSetAlertFuncEx(pinA, rotaryEx, this);
    gpioSetAlertFuncEx(pinB, rotaryEx, this);
  }
  else{
    gpioSetAlertFuncEx(pinA, rotary, this);
    gpioSetAlertFuncEx(pinB, rotary, this);
  }
}

int rotaryInc::get(){
  return pulse.load();
}

void rotaryInc::rotary(int gpio, int level, uint32_t tick, void *userdata){
  rotaryInc *regist = (rotaryInc *)userdata;

  if(gpio == regist->pinA){
    regist->nowA = level;
    if(level){
      regist->nowB ? ++regist->pulse : --regist->pulse;
    }
    else{
      regist->nowB ? --regist->pulse : ++regist->pulse;
    }
  }
  else{
    regist->nowB = level;
  }
}

void rotaryInc::rotaryEx(int gpio, int level, uint32_t tick, void *userdata){
}
