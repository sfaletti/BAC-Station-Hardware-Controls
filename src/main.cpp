#include <Arduino.h>
#include <Bounce2.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
// Set up more buttons than needed at moment, just in case things on the hardware side change. Physical buttons are connected to pins 0-6 on the Teensy 3.2
#define BUTTON_COUNT 10
#define FIRST_BUTTON_PIN 0
Bounce *buttons[BUTTON_COUNT];

const int KEY_MAP[BUTTON_COUNT] = {
    KEY_C, // p1 b1
    KEY_V, // p1 b2
    KEY_N, // p2 b1
    KEY_M, // p2 b2
    KEY_B, // p3 b1
    KEY_G, // p3 b2
    KEY_H, // p4 b1
    KEY_J, // p4 b2
    KEY_O, // p1 start
    KEY_P, // p2 start
};

// Set up encoders. Physical encoders are connected to pins 14-21 on the Teensy 3.2
#define ENCODER_COUNT 2
#define FIRST_ENCODER_PIN 14
Encoder *encoders[ENCODER_COUNT];
int32_t encoderValues[ENCODER_COUNT];

const int ENCODER_MAP[ENCODER_COUNT * 2] = {
    KEY_A,           // p1 d1
    KEY_D,           // p1 d2
    KEY_LEFT_ARROW,  // p2 d1
    KEY_RIGHT_ARROW, // p2 d2
};

void setup()
// Initialize buttons
{
  for (int i = 0; i < BUTTON_COUNT; i++)
  {
    int index = FIRST_BUTTON_PIN + i;
    buttons[index] = new Bounce();
    buttons[index]->attach(index, INPUT_PULLUP);
    buttons[index]->interval(5);
  }

  // Intialize encoders.
  for (int i = 0; i < ENCODER_COUNT; i++)
  {
    int index = FIRST_ENCODER_PIN + (i * ENCODER_COUNT);
    encoders[i] = new Encoder(index, index + 1);
    encoderValues[i] = 0;
  }
  Keyboard.begin();
  Mouse.begin();

  // Turn on the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  // Read encoder values
  for (int i = 0; i < ENCODER_COUNT; i++)
  {
    // Get relative encoder motion since last read
    encoderValues[i] = encoders[i]->readAndReset();
  }

  // Send movements over mouse protocol
  Mouse.move(encoderValues[0], encoderValues[1]);

  // Send a keypress each time the encoder is moved, regardless of how far it is moved between reads. This is probably not very useful
  for (int i = 0; i < ENCODER_COUNT; i++)
  {
    int mapIndex = i * 2;
    if (encoderValues[i] < 0)
    {
      Keyboard.press(ENCODER_MAP[mapIndex]);
      Keyboard.release(ENCODER_MAP[mapIndex]);
    }
    else if (encoderValues[i] > 0)
    {
      Keyboard.press(ENCODER_MAP[mapIndex + 1]);
      Keyboard.release(ENCODER_MAP[mapIndex + 1]);
    }
  }

  // Read button values
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

  // Delay to prevent overloading the USB connection
  delay(5);
}
