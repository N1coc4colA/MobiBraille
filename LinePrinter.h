#pragma once

#include <Arduino.h>
#include <Servo.h>

#define SPACE_INBETWEEN 11
#define SPACE_BETWEEN 21

class LinePrinter
{
public:
	explicit LinePrinter(int p_poincons, int p_cremaillere);
	~LinePrinter();

	void convert(const char lettre);
	void rotation_servo(int col);
	void appui_poincon();
	void imprimer(int c1, int c2);
	
	void printLine(const char *ptr, unsigned int beg, size_t len);
	void setMoveFunc(void (*func)(int));
  
  void processData();
  bool isAvailable();

  void cleanup();

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

  //Currently running task
  const char *c_buff = NULL;
  unsigned int c_pos = 0;
  size_t c_l = 0;
};
