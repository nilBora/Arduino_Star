
#include "StarMode.h"

#define BUTTON_PIN 7

unsigned long prevMillis = 0;
unsigned long prevMillisCount = 0;
unsigned int interval = 500;
unsigned int count = 0;         // переменная для подсчета нажатий на кнопку
unsigned int button_old = 1;    // предидущее значение входа для отлавливания момента нажатия

const unsigned int maxProgramMod = 5;

StarMode mode = StarMode();

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  mode.setPins();
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillisCount > 20) {
    prevMillisCount = currentMillis;
    doButtonCount();
  }
  
  switch (count) {
    case 1:
      onDisplayOneProgram();
      break;
    case 2:
      onDisplayTwoProgram();
      break;
    case 3:
      onDisplayThreeProgram();
      break;
    case 4:
      onDisplayFourProgram();
      break;
    case 5:
      onDisplayFiveProgram();
      break;
    default:
      onDisplayOneProgram();
    }
}

void doButtonCount()
{
  if ((digitalRead(BUTTON_PIN) == LOW) && (button_old == 1))
  { 
    count++;
    if (count > maxProgramMod) {
      count = 1;
    }
    Serial.println(count);
  }
  button_old = digitalRead(BUTTON_PIN);

  return true;
}

void onDisplayOneProgram()
{
  mode.setRegistersUnits();
}

void onDisplayTwoProgram()
{
  unsigned long currentMillis = millis();
 
  if (currentMillis - prevMillis > interval) {
    prevMillis = currentMillis;
    mode.doTwoProgram();
  }
  return true;
}

void onDisplayThreeProgram()
{
  unsigned long currentMillis = millis();
 
  if (currentMillis - prevMillis > interval) {
    prevMillis = currentMillis;
    mode.doThreeProgram();
  }

  return true;
}

void onDisplayFourProgram()
{
  unsigned long currentMillis = millis();
  Serial.println(currentMillis - prevMillis);
  if (currentMillis - prevMillis > interval) {
    
    prevMillis = currentMillis;
    mode.doFourProgram();
  }

  return true;
}

void onDisplayFiveProgram()
{
  mode.doFiveProgram();
}
