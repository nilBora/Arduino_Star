#include "StarMode.h"
#include <Arduino.h>

#define DATA_PIN 9 //Пин подключен к DS входу 74HC595
#define LATCH_PIN 10 //Пин подключен к ST_CP входу 74HC595
#define CLOCK_PIN 11 //Пин подключен к SH_CP входу 74HC595

unsigned int programCounter = 0;

StarMode::StarMode() {}

StarMode::~StarMode() {}

const byte dataStepOne[3]= {
    B00010000, //третий регистр
    B00010001, //второй регистр
    B10001000 //первый регистр
 };

 const byte dataStepTwo[3] = {
    B00001010, //третий регистр
    B10101010, //второй регистр
    B01010101 //первый регистр
 };

 const byte dataStepThree[3] = {
    B00000100, //третий регистр
    B01000100, //второй регистр
    B00100010 //первый регистр
 };

void StarMode::setPins()
{
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
}

void StarMode::_setDataInRegisters(byte *data)
{
  digitalWrite(LATCH_PIN, LOW);
  for (int i=0; i<3; i++) {
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data[i]);
  }
  digitalWrite(LATCH_PIN, HIGH);
  
  return true;
}

void StarMode::setRegistersNull()
{
  digitalWrite(LATCH_PIN, LOW);  
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000);
  digitalWrite(LATCH_PIN, HIGH);
  return true;
}

void StarMode::setRegistersUnits()
{
  digitalWrite(LATCH_PIN, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b11111111);   // передаем последовательно на dataPin
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b11111111);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b11111111);
  digitalWrite(LATCH_PIN, HIGH);
}

void StarMode::doFourProgram()
{
  byte resultData[3];
  if (programCounter == 1) {
    _setDataInRegisters(dataStepOne);
  }
  if (programCounter == 2) {
     for (int i=0; i<3; i++) {
       resultData[i] = dataStepOne[i] | dataStepTwo[i];
    }
    _setDataInRegisters(resultData);
  }

  if (programCounter == 3) {
    for (int i=0; i<3; i++) {
      resultData[i] = dataStepOne[i] | dataStepTwo[i] | dataStepThree[i];
    }
    
    _setDataInRegisters(resultData);
  }

  if (programCounter == 4) {
    setRegistersNull();
    programCounter = 0;
  }
  
  programCounter++;

  return true;
}

void StarMode::doFiveProgram()
{
    byte byteToSend = 0; //Создаем пустой байт B00000000
    for (int bitPos = 0; bitPos < 8; bitPos++) { // В переменной хранится позиция изменяемого бита
        byteToSend = 0; // Обнуляем байт при каждом проходе
        bitWrite(byteToSend, bitPos, HIGH); // При bitPos=0 получим B00000001, при bitPos=1 - B00000010, при bitPos=2 - B00000100 и т.д.

        digitalWrite(LATCH_PIN, LOW);
        shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, byteToSend); // Инвертируем сигнал при помощи MSBFIRST, грузим с первого бита
        digitalWrite(LATCH_PIN, HIGH);
        delay(50);
    }

    return true;
}

void StarMode::doThreeProgram()
{
  byte resultData[3];
  if (programCounter == 1) {
    _setDataInRegisters(dataStepThree);
  }

  if (programCounter == 2) {
    for (int i=0; i<3; i++) {
       resultData[i] = dataStepThree[i] | dataStepTwo[i];
    }
    
    _setDataInRegisters(resultData);
  }
  
  if (programCounter == 3) {
    for (int i=0; i<3; i++) {
       resultData[i] = dataStepOne[i] | dataStepTwo[i] | dataStepThree[i];
    }
    _setDataInRegisters(resultData);
  }
  
  if (programCounter == 4) {
    setRegistersNull();
    programCounter = 0;
  }
  
  programCounter++;

  if (programCounter>4) {
    programCounter = 0;
  }
  
  return true;
}

void StarMode::doTwoProgram()
{ 
  if (programCounter == 1) {
    _setDataInRegisters(dataStepThree);
  }

  if (programCounter == 2) {
    _setDataInRegisters(dataStepTwo);
  }

  if (programCounter == 3) {
    _setDataInRegisters(dataStepOne);
    programCounter = 0;
  }
  programCounter++;
  if (programCounter>3) {
    programCounter = 0;
  }

  return true;
}



