#pragma once

#include <Arduino.h>
#include "Grove_I2C_Motor_Driver.h"

//#define DBG

class I2CController
{
public:
	explicit I2CController(int c, int a, int b, int addr, int mid);
  ~I2CController();

	void moveByTicks(int ticks, int s = 200);
	void reset();

  int a0;
  int pA;
  int pB;
  int address;
  int motID;
  volatile int pos = 0;

  void moveUntil(bool uw, int v, bool (*state)());
  void move(bool upward, int v);
  void stop();
  
protected:
	virtual int convert(int v);
};
