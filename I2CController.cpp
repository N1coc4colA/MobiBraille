#include "I2CController.h"

I2CController *m_ptr = NULL;

void hiddenISR()
{
  if (m_ptr == NULL) {
    return;
  }
  m_ptr->pos += (digitalRead(m_ptr->pA) == digitalRead(m_ptr->pB) ? 1 : -1);
#ifdef DBG
  Serial.println(m_ptr->pos);
#endif
}

I2CController::I2CController(int c, int a, int b, int addr, int mid)
	: a0(c), pA(a), pB(b), address(addr), motID(mid)
{
  Motor.begin(address);

  pinMode(a0, INPUT);
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  attachInterrupt(digitalPinToInterrupt(a0), hiddenISR, CHANGE);
  
#ifdef DBG
  Serial.println("Codeur incremental");
#endif

  //Ensure it is off!
  reset();
  Motor.speed(motID, 0);

  m_ptr = this;
}

I2CController::~I2CController()
{
  m_ptr = NULL;
  detachInterrupt(digitalPinToInterrupt(a0));
}

void I2CController::reset()
{
	pos = 0;
}

int I2CController::convert(int v)
{
	return v + 511;
}

void I2CController::moveByTicks(int ticks, int v)
{
  const int nv = pos + ticks;
  while (nv != pos) {
    const int s = convert(pos < nv ? -v : v);
    if ((0 <= s) && (s < 460)) {
      Motor.speed(motID, map(s, 0, 460, -100, 0));
    } else if ((573 < s) && (s <= 1023)) {
      Motor.speed(motID, map(s, 573, 1023, 0, 100));
    }
  }
#ifdef DBG
  Serial.println(pos);
#endif
  stop();
}

void I2CController::move(bool upward, int v)
{
    const int s = convert(upward ? -v : v);
    if ((0 <= s) && (s < 460)) {
      Motor.speed(motID, map(s, 0, 460, -100, 0));
    } else if ((573 < s) && (s <= 1023)) {
      Motor.speed(motID, map(s, 573, 1023, 0, 100));
    }
}

void I2CController::stop()
{
  Motor.speed(motID, 0);
}

void I2CController::moveUntil(bool uw, int v, volatile bool *s)
{
  while (!(*s)) {
    move(uw, v);
  }
  stop();
}


