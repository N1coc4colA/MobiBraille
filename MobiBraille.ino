#include "I2CController.h"
#include "BluetoothInterface.h"
#include "LinePrinter.h"
#include "DocumentPrinter.h"

#define LP_C 8
#define LP_P 9
#define BI_TX 10
#define BI_RX 11

volatile bool n_state = false;

BluetoothInterface iface(0, 0);
LinePrinter lp(0, 0);
DocumentPrinter dp(&lp);
I2CController ctrlx(0, 0, 0, 0, 0);
//Add the Y Controller!
//I2CController ctrly(0, 0, 0, 0, 0;

void deplacementX(int v)
{
  ctrlx.moveByTicks(v);
}

/*
void deplacementY(int v)
{
  ctrly.moveByTicks(v);
}
*/

void printData(const char *d, size_t l)
{
  dp.printDocument(d, l);
}

void handleAbort()
{
  dp.cleanup();
  lp.cleanup();
}

void reset()
{
  n_state = 0;
  ctrlx.moveUntil(false, 200, &n_state);
  ctrlx.moveUntil(false, 150, &n_state);
  ctrlx.reset();
}

void setup()
{
  Serial.begin(250000);
  iface = BluetoothInterface(BI_RX, BI_TX);
  lp = LinePrinter(LP_P, LP_C);
  dp = DocumentPrinter(&lp);
  ctrlx = I2CController(A0, 2, 3, 0x0f, 1);
  //ctrly = I2CController(, , , , );

  iface.setTriggers(&printData);
  iface.setAbortHandler(&handleAbort);
  lp.setMoveFunc(&deplacementX);
  //dp.setMoveFunc(&deplacementY);

  reset();
}

void loop()
{
  //We use an event model, split tasks into sub ones to be able to suspend or anything.
  iface.processData();
  dp.processData();
  lp.processData();
}
