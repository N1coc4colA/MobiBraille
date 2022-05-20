#include "I2CController.h"
#include "BluetoothInterface.h"
#include "LinePrinter.h"

#define LP_C 8
#define LP_P 9
#define BI_TX 10
#define BI_RX 11

BluetoothInterface iface;
LinePrinter lp;
I2CController ctrlx;
//Add the Y Controller!

void deplacement(int v)
{
  ctrlx.moveByTicks(v);
}
void printData(const char *d, size_t l)
{
  //[TODO] split by line and back to line!
  lp.printLin(d, l);
}

void setup()
{
  Serial.begin(250000);
  iface = BluetoothInterface(BI_RX, BI_TX);
  lp = LinePrinter(LP_P, LP_C);
  ctrlx = I2CController(A0, 2, 3, 0x0f, 1);

  iface.setTriggers(&printData);
  lp.setMoveFunc(&deplacement);
}

void loop()
{
  iface.processData();
}

