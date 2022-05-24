#include "LinePrinter.h"

LinePrinter::LinePrinter(int p_p, int p_c)
: pinServoPoincons(p_p), pinServoCremaillere(p_c)
{
  ServoPoincons.attach(pinServoPoincons);
  ServoCremaillere.attach(pinServoCremaillere);
  ServoPoincons.write(angle_init);
  ServoCremaillere.write(90);

  cleanup();
}

LinePrinter::~LinePrinter()
{
  ServoPoincons.detach();
  ServoCremaillere.detach();
}

void LinePrinter::cleanup()
{
  c_buff = NULL;
  c_pos = 0;
  c_l = 0;
}

void LinePrinter::processData()
{
  if (c_pos < c_l) {
#ifdef DBG
    Serial.print("Printing: ");
    Serial.println(c_buff[c_pos]);
#endif
    prepareCharacter(c_buff[c_pos]);
    print(c_buff[c_pos]);
    if (c_pos != (c_l-1)) {
      deplacementX(SPACE_BETWEEN);
    }
    c_pos++;
  } else {
    if (c_buff != NULL && gotoOrigin != NULL) {
      gotoOrigin();
    }
    cleanup();
  }
}

bool LinePrinter::isAvailable()
{
  return c_buff == NULL;
}

void LinePrinter::printLine(const char *buff, unsigned int beg, size_t s)
{
	if (deplacementX == NULL) {
		return;
	}

  c_buff = buff;
  c_pos = beg;
  c_l = s;
}

void LinePrinter::prepareCharacter(const char lettre) {
  switch (lettre) {
    case 'a':
      column1 = 7;
      column2 = 4;
      break;
    case 'b':
      column1 = 5;
      column2 = 4;
      break;
    case 'c':
      column1 = 7;
      column2 = 7;
      break;
    case 'd':
      column1 = 7;
      column2 = 5;
      break;
    case 'e':
      column1 = 7;
      column2 = 2;
      break;
    case 'f':
      column1 = 5;
      column2 = 7;
      break;
    case 'g':
      column1 = 5;
      column2 = 5;
      break;
    case 'h':
      column1 = 5;
      column2 = 2;
      break;
    case 'i':
      column1 = 2;
      column2 = 7;
      break;
    case 'j':
      column1 = 2;
      column2 = 5;
      break;
    case 'k':
      column1 = 8;
      column2 = 4;
      break;
    case 'l':
      column1 = 6;
      column2 = 4;
      break;
    case 'm':
      column1 = 8;
      column2 = 7;
      break;
    case 'n':
      column1 = 8;
      column2 = 5;
      break;
    case 'o':
      column1 = 8;
      column2 = 2;
      break;
    case 'p':
      column1 = 6;
      column2 = 7;
      break;
    case 'q':
      column1 = 6;
      column2 = 5;
      break;
    case 'r':
      column1 = 6;
      column2 = 2;
      break;
    case 's':
      column1 = 1;
      column2 = 7;
      break;
    case 't':
      column1 = 1;
      column2 = 5;
      break;
    case 'u':
      column1 = 8;
      column2 = 3;
      break;
    case 'v':
      column1 = 6;
      column2 = 3;
      break;
    case 'w':
      column1 = 2;
      column2 = 6;
      break;
    case 'x':
      column1 = 8;
      column2 = 8;
      break;
    case 'y':
      column1 = 8;
      column2 = 6;
      break;
    case 'z':
      column1 = 8;
      column2 = 1;
      break;
    case ' ':
      column1 = 4;
      column2 = 4;
      break;
  }
}

double LinePrinter::columnToAngle(int colone)
{
  switch (colone) {
    case 0:
      return 7.05;
    case 1:
      return 21.15;
    case 2:
      return 35.25;
    case 3:
      return 49.35;
    case 4:
      return 63.45;
    case 5:
      return 77.55;
    case 6:
      return 91.65;
    case 7:
      return 105.75;
  }
  return 0;
}

void LinePrinter::apply(){
  ServoCremaillere.write(33);
  delay(80);
  ServoCremaillere.write(0);
  delay(25);
}

void LinePrinter::rotate(double angle)
{
  ServoPoincons.write(angle);
  delay(10);
}

void LinePrinter::printPart(double angle)
{
  rotate(angle);
  apply();
}

void LinePrinter::print(char c)
{
 if (deplacementX == NULL) {
    return;
  }
  printPart(columnToAngle(column1));
  deplacementX(SPACE_INBETWEEN);
  printPart(columnToAngle(column2));
}

void LinePrinter::setMoveFunc(void (*func)(int))
{
	deplacementX = func;
}

void LinePrinter::setGotoOrigin(void (*f)())
{
  gotoOrigin = f;
}

