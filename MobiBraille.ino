#include "I2CController.h"
#include "BluetoothInterface.h"
#include "LinePrinter.h"
#include "DocumentPrinter.h"

#define LP_C 8
#define LP_P 9
#define BI_TX 10
#define BI_RX 11
#define FC_P 5

BluetoothInterface iface(0, 0);
LinePrinter lp(0, 0);
DocumentPrinter dp(&lp);
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
  ctrly->moveByTicks(v);
}

void printData(const char *d, size_t l)
{
  dp.printDocument(d, l);
}

void handleAbort()
{
  dp.cleanup();
  lp.cleanup();
}

void resetPos()
{
  Serial.println("b");
  ctrlx->moveUntil(false, 200, &readState);
  ctrlx->moveUntil(false, 150, &readState);
  ctrlx->reset();
}

void setup()
{
  Serial.begin(250000);
#ifdef DBG
  Serial.println("MobiBraille starting...");
#endif

  if (ctrlx != NULL) {
    delete ctrlx;
  }
  if (ctrly != NULL) {
    delete ctrly;
  }

  iface = BluetoothInterface(BI_RX, BI_TX);
  lp = LinePrinter(LP_P, LP_C);
  dp = DocumentPrinter(&lp);
  ctrlx = new I2CController(A0, 2, 3, 0x0f, 1);
  //ctrly = I2CController(A1, 4, 5, 0x0f, 2); //[TODO] Add the pins PLZ

  iface.setTriggers(&printData);
  iface.setAbortHandler(&handleAbort);
  lp.setMoveFunc(&deplacementX);
  dp.setMoveFunc(&deplacementY);

  resetPos();
#ifdef DBG
  Serial.println("MobiBraille started.");
#endif
}

void loop()
{
  //We use an event model, split tasks into sub ones to be able to suspend or anything.
  iface.processData();
  dp.processData();
  lp.processData();
}
