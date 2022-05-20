#include "BluetoothInterface.h"

BluetoothInterface::BluetoothInterface(int r, int t)
	: rx(r), tx(t), mhc(rx, tx)
{
}

void BluetoothInterface::nullData(char *ptr, size_t s)
{
  int i = 0;
  while (i < (s+1)) {
    ptr[i] = '\0';
    i++;
  }
}

int BluetoothInterface::readInt()
{
  int out = 0;
  bool goOn = true;
  while (mhc.available() && goOn) {
    delay(LOOP_DELAY);
    char current = mhc.read();
    bool acceptValue = true;

    //Check input
    switch (current) {
      case '[': {
        acceptValue = false;
        mhc.print("RDSB"); //ReaD Size Begun
        break;
      }
      case ']': {
        acceptValue = false;
        goOn = false;
        mhc.print("RDSE"); //ReaD Size Ended
        break;
      }
      //In case of abort request.
      case '%': {
        acceptValue = false;
        goOn = false;
        out = 0;
        mhc.print("ABORTED");
        break;
      }
      default: {
        break;
      }
    }

    //Make conversion
    if (acceptValue) {
      //Transform to ASCII 0-9 value
      int v = (int)current;
      v -= 48;
      if (v >= 0 && 9 >= v) {
        out *= 10;
        out += v;
      }
    }
  }
  return out;
}

void BluetoothInterface::receiveData(int dataSize)
{
  if (dataSize < 1) {
    return;
  } else if (dataSize > DATA_SIZE_LIMIT) {
    mhc.print("DOS");
  }

  Serial.print("Document size: ");
  Serial.println(dataSize);

  bool goOn = true;
  int t_offset = 0;
  int inputDataOffset = 0;
  dataset = (char *)malloc(sizeof(char) *(dataSize+1));
  //Null it all or we'll get artefacts
  nullData(dataset, dataSize+1);

  //Notify that's ready
  mhc.print("RDY");

  //Loop over to read the data.
  while (goOn && inputDataOffset < (dataSize+1) && t_offset < TIMEOUT) {
    delay(READING_DELAY);
    if (mhc.available()) {
      char v = mhc.read();
      //If abort is requested, flush and stop.
      if (v == '%') {
        //Cleanup
        goOn = false;
        //Notify
        mhc.print("ABORTED");
      } else {
        dataset[offset] = v;
        offset++;
        inputDataOffset++;
      }
      t_offset = 0;
    }
    t_offset++;
  }

  //Error checking
  if (t_offset == TIMEOUT) {
      //Notify
      mhc.print("TIMEDOUT");
  } else if (inputDataOffset == (dataSize+1)) {
    mhc.print("WOS"); //Write Operation Succeed.
  } else if (goOn) {
    mhc.print("WOF"); //Write Operation Failed.
  }

  //Print the document.
  if (goOn && m_trigger != NULL) {
    m_trigger(dataset, offset+1);
  }

  //Cleanup
  offset = 0;
  if (dataset) {
    /* [TODO] Print content from here. */
    free(dataset);
    dataset = NULL;
  }
}

void BluetoothInterface::processData()
{
  receiveData(readInt());
}

void BluetoothInterface::setTriggers(void (*ptr)(const char *, size_t s))
{
  m_trigger = ptr;
}


