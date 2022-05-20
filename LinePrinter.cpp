#include "LinePrinter.h"

LinePrinter::LinePrinter(int p_p, int p_c)
: pinServoPoincons(p_p), pinServoCremaillere(p_c)
{
  ServoPoincons.attach(pinServoPoincons);
  ServoCremaillere.attach(pinServoCremaillere);
  ServoPoincons.write(angle_init);
  ServoCremaillere.write(90);
}

LinePrinter::~LinePrinter()
{
  ServoPoincons.deatach();
  ServoCremaillere.deatach();
}

void LinePrinter::printLine(const char *buff, size_t s)
{
	if (deplacementX == NULL) {
		return;
	}
	int pos = 0;
	while (pos < s) {
		Serial.println(buff[pos]);
		convert(buff[pos]);
		imprimer(colone1, colone2);
		if (pos != (s-1) {
			deplacementX(SPACE_BETWEEN);
		}
		pos++;
	}
}

void LinePrinter::convert(const char lettre) {
  switch (lettre) {
    case 'a':
      colone1 = 7;
      colone2 = 4;
      break;
    case 'b':
      colone1 = 5;
      colone2 = 4;
      break;
    case 'c':
      colone1 = 7;
      colone2 = 7;
      break;
    case 'd':
      colone1 = 7;
      colone2 = 5;
      break;
    case 'e':
      colone1 = 7;
      colone2 = 2;
      break;
    case 'f':
      colone1 = 5;
      colone2 = 7;
      break;
    case 'g':
      colone1 = 5;
      colone2 = 5;
      break;
    case 'h':
      colone1 = 5;
      colone2 = 2;
      break;
    case 'i':
      colone1 = 2;
      colone2 = 7;
      break;
    case 'j':
      colone1 = 2;
      colone2 = 5;
      break;
    case 'k':
      colone1 = 8;
      colone2 = 4;
      break;
    case 'l':
      colone1 = 6;
      colone2 = 4;
      break;
    case 'm':
      colone1 = 8;
      colone2 = 7;
      break;
    case 'n':
      colone1 = 8;
      colone2 = 5;
      break;
    case 'o':
      colone1 = 8;
      colone2 = 2;
      break;
    case 'p':
      colone1 = 6;
      colone2 = 7;
      break;
    case 'q':
      colone1 = 6;
      colone2 = 5;
      break;
    case 'r':
      colone1 = 6;
      colone2 = 2;
      break;
    case 's':
      colone1 = 1;
      colone2 = 7;
      break;
    case 't':
      colone1 = 1;
      colone2 = 5;
      break;
    case 'u':
      colone1 = 8;
      colone2 = 3;
      break;
    case 'v':
      colone1 = 6;
      colone2 = 3;
      break;
    case 'w':
      colone1 = 2;
      colone2 = 6;
      break;
    case 'x':
      colone1 = 8;
      colone2 = 8;
      break;
    case 'y':
      colone1 = 8;
      colone2 = 6;
      break;
    case 'z':
      colone1 = 8;
      colone2 = 1;
      break;
    case ' ':
      colone1 = 4;
      colone2 = 4;
      break;
  }
}

void LinePrinter::rotation_servo(int colone) {
  double angle = 0;
  switch (colone) {
    case 0:
      angle = 4.95;
      break;

    case 1:
      angle = 14.85;
      break;

    case 2:
      angle = 24.75;
      break;

    case 3:
      angle = 34.65;
      break;

    case 4:
      angle = 44.55;
      break;

    case 5:
      angle = 54.45;
      break;

    case 6:
      angle = 64.35;
      break;

    case 7:
      angle = 74.25;
      break;
  }
  ServoPoincons.write(angle);
  delay(10);
}

void LinePrinter::appui_poincon(){
  ServoCremaillere.write(45);
  delay(80);
  ServoCremaillere.write(90);
  delay(25);
}

void LinePrinter::imprimer(int C1, int C2)
{
	if (deplacementX == NULL) {
		return;
	}
	rotation_servo(C1);
	appui_poincon();
	deplacementX(SPACE_INBETWEEN);
	rotation_servo(C2);
	appui_poincon();
}

void LinePrinter::setMoveFunc((*func)(int))
{
	deplacementX = func;
}