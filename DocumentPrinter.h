#pragma once

#include <Arduino.h>

#define DBG

class LinePrinter;

class DocumentPrinter
{
public:
  explicit DocumentPrinter(LinePrinter *lp = NULL);

  void setMoveFunc(void (*func)(int));
  void printDocument(char *buff, size_t s);
  void processData();

  void cleanup();

  bool isBusy();

private:
  LinePrinter *lp;

  char *c_buff = NULL;
  size_t c_l = 0;
  int c_spos = 0;
  int c_p = 0;
  bool set = false;

  void (*deplacement)(int) = NULL;
};
