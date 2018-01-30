#include<iostream>
#include<wiringPi.h>
#include<wiringPiI2C.h>
#include<time.h>
#include<math.h>
#include"GY521.hpp"

using namespace std;
using namespace RPGY521;

GY521::GY521(){
  while(init(0x68, 2, 1000) == 0){};
}

bool GY521::init(int dev, int bit, int calibration){
  //I2C Setup
  devId = dev;
  if((I2cId = wiringPiI2CSetup(devId)) > 0){
    if(gyroRead(WHO_AM_I) == devId){
      if(gyroRead(PWR_MGMT_1) == 0x40){
        gyroWrite(PWR_MGMT_1, 0x00);
      }
      cout << "I2C Success" << endl;
    }
    else{
      cout << "NotFound I2C DEVAICE" << endl;
      return 0;
    }
  }
  else{
    cout << "I2C Failed" << endl;
    return 0;
  }

  //Skew Detection
  short accelXNow = 0, accelYNow = 0, accelZNow = 0;
  double accelXAver = 0, accelYAver = 0, accelZAver = 0;
  //AccelSensar, Max:2[g], LSB:16384[LSB/g]
  gyroWrite(AFS_SEL, 0x00);

  for(int i = 0; i < 100; i++){
    accelXNow = gyroRead2(ACCEL_XOUT_H, ACCEL_XOUT_L);
    accelYNow = gyroRead2(ACCEL_YOUT_H, ACCEL_YOUT_L);
    accelZNow = gyroRead2(ACCEL_ZOUT_H, ACCEL_ZOUT_L);
    accelXAver += accelXNow;
    accelYAver += accelYNow;
    accelZAver += accelZNow;
  }
  accelXAver = accelXAver / calibration;
  accelYAver = accelYAver / calibration;
  accelZAver = accelZAver / calibration;
  double gyroReg = hypot(hypot(accelXAver, accelYAver), accelZAver) / accelZAver;

  //Calibration gyroZAver(deg/s)
	short gyroZNow;
  for(int i = 0; i < calibration; i++){
    gyroZNow = gyroRead2(GYRO_ZOUT_H, GYRO_ZOUT_L);
    gyroZAver += gyroZNow;
  }
  gyroZAver = gyroZAver / calibration;
  cout << "Calibration Finish:" << gyroZAver << endl;

  //Gyro init
  gyroWrite(FS_SEL, bit << 3);
  gyroLSB = LSBMap[bit] / gyroReg;
  return 1;
}


double GY521::getYaw(){
  clock_gettime(CLOCK_REALTIME, &now);
  short gyroZNow = gyroRead2(GYRO_ZOUT_H, GYRO_ZOUT_L);

  yaw += ((double)gyroZNow - gyroZAver) / gyroLSB * (now.tv_sec - prev.tv_sec + (now.tv_nsec - prev.tv_nsec) / 1000000000);

  prev = now;
  if(yaw > 180){
    yaw -= 360;
  }
  else if(yaw <= -180){
    yaw += 360;
  }
  return yaw;
}

