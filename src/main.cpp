#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>

Encoder encoder1(11, 12);
Encoder encoder2(9, 10);
int encVal1 = 0;
int encVal2 = 0;

const int button1 = 2;
const int button2 = 3;

Bounce bounce1 = Bounce();
Bounce bounce2 = Bounce();

void setup()
{
  bounce1.attach(button1, INPUT_PULLUP);
  bounce2.attach(button2, INPUT_PULLUP);

  bounce1.interval(5);
  bounce2.interval(5);
}

void loop()
{
  int encVal1 = encoder1.readAndReset();
  int encVal2 = encoder2.readAndReset();
  // Mouse.move(encVal1 * 10, encVal2 * 10);

  if (encVal1 > 0)
  {
    Keyboard.press(KEY_A);
    Keyboard.release(KEY_A);
  }
  else if (encVal1 < 0)
  {
    Keyboard.press(KEY_D);
    Keyboard.release(KEY_D);
  }

  if (encVal2 > 0)
  {
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.release(KEY_LEFT_ARROW);
  }
  else if (encVal2 < 0)
  {
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_RIGHT_ARROW);
  }

  bounce1.update();

  switch (bounce1.read())
  {
  case LOW:
    Keyboard.press(KEY_C);
    break;
  case HIGH:
    Keyboard.release(KEY_C);
    break;
  }

  bounce2.update();

  switch (bounce2.read())
  {
  case LOW:
    Keyboard.press(KEY_V);
    break;
  case HIGH:
    Keyboard.release(KEY_V);
    break;
  }
}
