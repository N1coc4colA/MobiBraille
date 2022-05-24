#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

#define TIMEOUT 100
#define LOOP_DELAY 100
#define READING_DELAY 100
#define DATA_SIZE_LIMIT 2048

#define DBG

/*
 * PROTOCOL SYSTEM:
 * 
 * [SIZE]TEXT...
 * -> TEXT of length SIZE is printed.
 * 
 * IN WAITING OF TEXT; ABORT:
 * TEXT1... % ...TEXT2
 * -> TEXT2 is not printed.
 * 
 * MESSAGING SYSTEM ON:
 * | : 1/0   : The busy state of the printer, 1 is busy, 0 is free
 * ? : RDY   : ReaDY to print
 * < : DSRB  : Data Size Read Begun
 * > : DSRE  : Data Size Read Ended
 * % : ABORT : operation ABORTed
 * ? : WOS   : Write Operation Succeed
 * ? : WOF   : Write Operation Failed
 * ? : DOS   : Data OverSize
 * ? : BUSY  : System is printing
 * 
 * MESSAGING EXAMPLE:
 * [Connecting to MHC]
 * CLI <- CONN -- MHC
 * CLI -- "|" --> MHC
 * CLI <--- 0 --- MHC
 * CLI -- "<" --> MHC
 * CLI <- DSRB -- MHC
 * CLI -- ... --> MHC
 * CLI -- ">" --> MHC
 * CLI <- DSRE -- MHC
 * CLI <-- RDY -- MHC
 * CLI -- ... --> MHC
 * CLI <- BUSY -- MHC
 * CLI <-- WOS -- MHC
 *  [Printing ended]
 */
 
class BluetoothInterface
{
public:
	explicit BluetoothInterface(int rx = 11, int tx = 10);
  ~BluetoothInterface();
	void processData();

  void setTriggers(void (*ptr)(const char *arr, size_t s));
  void setAbortHandler(void (*f)());
  void setBusyCallback(bool (*ptr)());

protected:
	void nullData(char *ptr, size_t s);
	int readInt();
  //receiveData MUST be made as a continuous operation, no pause must be done.
	void receiveData(int dataSize);

private:
	int tx;
	int rx;
	int offset = 0;
	char *dataset = NULL;
  void (*m_trigger)(const char *, size_t) = NULL;
  void (*m_aborted)() = NULL;
  bool (*bstate)() = NULL;
	SoftwareSerial *mhc = NULL;
};
