#include "DocumentPrinter.h"

#include "LinePrinter.h"

DocumentPrinter::DocumentPrinter(LinePrinter *l)
{
  lp = l;
}

void DocumentPrinter::cleanup()
{
  c_spos = 0;
  c_p = 0;
  c_l = 0;
  c_buff = NULL;
  set = false;
}

void DocumentPrinter::setMoveFunc(void (*func)(int))
{
  deplacement = func;
}

void DocumentPrinter::printDocument(const char *buff, size_t s)
{
  c_buff = buff;
  c_l = s;
  c_spos = 0;
  c_p = 0;
  set = false;
}

void DocumentPrinter::processData()
{
  if (c_buff == NULL || lp == NULL || deplacement == NULL) {
    return;
  }

  if (c_spos < ((unsigned int)c_l)) {
    if (!set) {
      Serial.println("Prepared data.");
      //We preprocess text, this way we can properly split by lines.
      c_p = c_spos;
      bool nobrk = true;
      while (c_p < c_l) {
        if (c_buff[c_p] == '\n') {
          nobrk = false;
        }
        c_p++;
      }
      set = true;
      c_spos += c_p;
    } else if (lp->isAvailable()) {
      Serial.print(c_spos);
      Serial.print(" - ");
      Serial.println(c_p);
      lp->printLine(c_buff, c_spos, c_p);
      set = false;
    } else {
      Serial.println("Line printer not available!");
    }
  } else {
    cleanup();
  }
}

bool DocumentPrinter::isBusy()
{
  return c_buff != NULL;
}

