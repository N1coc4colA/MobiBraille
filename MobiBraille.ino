#include "I2CController.h"
#include "BluetoothInterface.h"
#include "LinePrinter.h"
#include "DocumentPrinter.h"
#include "Grove_I2C_Motor_Driver.h"

#define LP_C 6
#define LP_P 7
#define BI_RX 10
#define BI_TX 11
#define FC_P 5
#define I2C_ADDRESS 0x0f

#define DBG

BluetoothInterface *iface = NULL;
LinePrinter *lp = NULL;
DocumentPrinter *dp = NULL;
I2CController *ctrlx = NULL;
I2CController *ctrly = NULL;

bool readState()
{
  return bool(digitalRead(FC_P));
}

void deplacementX(int v)
{
  ctrlx->moveByTicks(v);
}

void deplacementY(int v)
{
  Serial.print("Y");
  Serial.println(v);
  ctrly->moveByTicks(v);
}

void resetPos()
{
#ifdef DBG
  Serial.println("Reseting pos...");
#endif
  ctrlx->moveUntil(true, 150, &readState);

  //We need a minimum delay to ensure it is alright.
  unsigned long source = millis();
  while ((millis() - source) < 20) {
  }

  if (!readState()) {
    resetPos();
  } else {
    if (ctrlx->pos > 0) {
      ctrlx->moveUntil(true, 80, &readState);
    }
    ctrlx->reset();
  }
}

void moveToOrigin()
{
  resetPos();
}

void printData(char *d, size_t l)
{
 Serial.println(d);
  dp->printDocument(d, l);
}

void handleAbort()
{
  dp->cleanup();
  lp->cleanup();
}

bool documentState()
{
  return false; //dp->isBusy();
}

void setup()
{
  Serial.begin(250000);
#ifdef DBG
  Serial.println("MobiBraille starting...");
  Serial.println('{');
#endif

  if (ctrlx != NULL) {
    delete ctrlx;
  }
  if (ctrly != NULL) {
    delete ctrly;
  }
  if (iface != NULL) {
    delete iface;
  }
  if (dp != NULL) {
    delete dp;
  }
  if (lp != NULL) {
    delete lp;
  }

#ifdef DBG
  Serial.println("Setting up I2C Motor driver...");
#endif
  Motor.begin(I2C_ADDRESS);
#ifdef DBG
  Serial.println("I2C Motor driver setup.");
#endif

  iface = new BluetoothInterface(BI_RX, BI_TX);
  lp = new LinePrinter(LP_P, LP_C);
  dp = new DocumentPrinter(lp);
  ctrlx = new I2CController(0, 2, 3, I2C_ADDRESS, MOTOR1);
  ctrly = new I2CController(A1, 4, 5, I2C_ADDRESS, MOTOR2);

  iface->setTriggers(&printData);
  iface->setAbortHandler(&handleAbort);
  iface->setBusyCallback(&documentState);
  lp->setMoveFunc(&deplacementX);
  lp->setGotoOrigin(&moveToOrigin);
  dp->setMoveFunc(&deplacementY);

  resetPos();
#ifdef DBG
  Serial.println("MobiBraille started.");
#endif
}

void loop()
{
  //We use an event model, split tasks into sub ones to be able to suspend or anything.
  iface->processData();
  dp->processData();
  lp->processData();
}
