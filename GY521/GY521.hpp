#pragma once
#include<wiringPi.h>
#include<wiringPiI2C.h>
#include<time.h>

namespace RPGY521{
  enum RegisterMap{
    WHO_AM_I = 0x75,
    PWR_MGMT_1 = 0x6B,
    FS_SEL = 0x1B,
		GYRO_ZOUT_H = 0x47,
		GYRO_ZOUT_L = 0x48,
		AFS_SEL = 0x1C,
		ACCEL_XOUT_H = 0x3B,
		ACCEL_XOUT_L = 0x3C,
		ACCEL_YOUT_H = 0x3D,
		ACCEL_YOUT_L = 0x3E,
		ACCEL_ZOUT_H = 0x3F,
		ACCEL_ZOUT_L = 0x40,
	};
  constexpr double LSBMap[4] = {131, 65.5, 32.8, 16.4};
	class GY521{
    public:
      GY521();
      double getYaw();
      void resetYaw(){
        yaw = 0;
      }
      void start(){
        clock_gettime(CLOCK_REALTIME, &prev);
        resetYaw();
      }
    private:
      int devId;
      int I2cId;
      double yaw;
      double gyroZAver;
      double gyroLSB;
      struct timespec now, prev;

      bool init(int devId, int bit, int calibration);

      //マクロ的なやつ
      int gyroRead(enum RegisterMap Register){
        return wiringPiI2CReadReg8(I2cId, Register);
      }

      int gyroRead2(enum RegisterMap RegisterH, enum RegisterMap RegisterL){
        return (wiringPiI2CReadReg8(I2cId, RegisterH) << 8) + wiringPiI2CReadReg8(I2cId, RegisterL);
      }

      bool gyroWrite(enum RegisterMap Register, int data){
        return wiringPiI2CWriteReg8(I2cId, Register, data) == devId ? 1 : 0;
      }
  };
};
