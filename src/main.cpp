#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>

Encoder encoder1(11, 12);
Encoder encoder2(9, 10);
int encVal1 = 0;
int encVal2 = 0;

// Set up more buttons than needed at moment, just in case things on the hardware side change. Physical buttons are connected to pins 0-6 on the Teensy 3.2
#define BUTTON_COUNT 6
Bounce *buttons[BUTTON_COUNT];

const int KEY_MAP[BUTTON_COUNT] = {
    KEY_C, // p1 b1
    KEY_V, // p1 b2
    KEY_N, // p2 b1
    KEY_M, // p2 b2
    KEY_S, // p1 start
    KEY_P, // p2 start
};

void setup()
{
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    buttons[i] = new Bounce();
    buttons[i]->attach(i, INPUT_PULLUP);
    buttons[i]->interval(5);
  }
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

  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    buttons[i]->update();

    switch (buttons[i]->read())
    {
    case LOW:
      Keyboard.press(KEY_MAP[i]);
      break;
    case HIGH:
      Keyboard.release(KEY_MAP[i]);
      break;
    }
  }
}
