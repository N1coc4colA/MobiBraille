#pragma once

#include <Servo.h>

#define SPACE_INBETWEEN 11
#define SPACE_BETWEEN 21

class LinePrinter
{
public:
	explicit LinePrinter(int p_poincons, int p_cremaillere);
	~LinePrinter();

	void convert(const char lettre);
	void rotationS_servo(int col);
	void appui_poincon();
	void imprimer(int c1, int c2);
	
	void printLine(const char *ptr, size_t len);

	void setMoveFunc((*func)(int));

private:
	Servo ServoPoincons;
	Servo ServoCremaillere;
	
	int pinServoPoincons;
	int pinServoCremaillere;

	int angle_init = 45;
	int colone1;
	int colone2;
	int char_limite = 20;
	int x = 0;
	
	void (*deplacementX)(int v) = NULL;
};
