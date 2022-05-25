#include "I2CController.h"

static I2CController *m_ptr1 = NULL;
static I2CController *m_ptr2 = NULL;

void hiddenISR1()
{
#ifdef DBG
  //Serial.println(m_ptr1->pos);
#endif
  if (m_ptr1 == NULL) {
    return;
  }
  m_ptr1->pos += (digitalRead(m_ptr1->pA) == digitalRead(m_ptr1->pB) ? 1 : -1);
}

void hiddenISR2()
{
#ifdef DBG
  Serial.println(m_ptr2->pos);
#endif
  if (m_ptr2 == NULL) {
    return;
  }
  m_ptr2->pos += (digitalRead(m_ptr2->pA) == digitalRead(m_ptr2->pB) ? 1 : -1);
}

I2CController::I2CController(int c, int a, int b, int addr, int mid)
	: a0(c), pA(a), pB(b), address(addr), motID(mid)
{
  pinMode(a0, INPUT);
  pinMode(pA, INPUT);
  pinMode(pB, INPUT);
  if (mid == MOTOR1) {
    Serial.println(a0);
    attachInterrupt(a0, hiddenISR1, CHANGE);
    m_ptr1 = this;
  } else {
    Serial.println(a0);
    attachInterrupt(a0, hiddenISR2, CHANGE);
    m_ptr2 = this;
  }

#ifdef DBG
  Serial.println("Codeur incremental");
#endif

  //Ensure it is off!
  reset();
  Motor.speed(motID, 0);
}

I2CController::~I2CController()
{
   if (motID == 1) {
      m_ptr1 = NULL;
   } else {
      m_ptr2 = NULL;
   }
  detachInterrupt(a0);
}

void I2CController::reset()
{
#ifdef DBG
  Serial.println("Position reset");
#endif
	pos = 0;
}

int I2CController::convert(int v)
{
	return v + 511;
}

void I2CController::precise(int nv, int s)
{
  Serial.println("precise");
    while (nv != pos) {
        Motor.speed(motID, (pos < nv ? -s : s));
    }
    stop();

  //Make it more precise with a correction
  unsigned long source = millis();
  while ((millis() - source) < 10) {
  }
  
   if (pos != nv) {
      precise(nv, s);
   }
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
  stop();

  precise(nv, 10);
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

void I2CController::moveUntil(bool uw, int v, bool (*func)())
{
#ifdef DBG
  Serial.println("MoveUntil called.");
#endif
  while (!func()) {
    move(uw, v);
  }
#ifdef DBG
  Serial.println("Ended.");
#endif
  stop();
}
