#pragma once

#include <Arduino.h>
#include <Servo.h>

#define DBG
#define SPACE_INBETWEEN 11
#define SPACE_BETWEEN 21

class LinePrinter
{
public:
	explicit LinePrinter(int p_poincons, int p_cremaillere);
	~LinePrinter();
	
	void printLine(const char *ptr, unsigned int beg, size_t len);
	void setMoveFunc(void (*func)(int));
  
  void processData();
  bool isAvailable();

  void cleanup();

  static double columnToAngle(int column);

protected:
  void printPart(double angle);
  void prepareCharacter(char c);
  void rotate(double angle);
  void apply();
  void print(char c);

private:
	Servo ServoPoincons;
	Servo ServoCremaillere;

	int pinServoPoincons = 0;
	int pinServoCremaillere = 0;

	int angle_init = 45;
	int column1 = 0;
	int column2 = 0;
	int char_limite = 20;
	int x = 0;

	void (*deplacementX)(int v) = NULL;

  //Currently running task
  const char *c_buff = NULL;
  unsigned int c_pos = 0;
  size_t c_l = 0;
};
