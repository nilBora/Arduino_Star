int dataPin = 9; //Пин подключен к DS входу 74HC595
int latchPin = 10; //Пин подключен к ST_CP входу 74HC595
int clockPin = 11; //Пин подключен к SH_CP входу 74HC595

void setup()
{
//устанавливаем режим OUTPUT
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin, OUTPUT);

digitalWrite(latchPin, LOW); // устанавливаем синхронизацию "защелки" на LOW
shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000); // передаем последовательно на dataPin
shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
digitalWrite(latchPin, HIGH); //"защелкиваем" регистр, тем самым устанавливая значения на выходах
}

void loop()
{
  switch (4) {
  case 1:
    onDisplayFirstProgram();
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
  }
}

void onDisplayTwoProgram()
{
  byte registerData1 = B00100010;
  byte registerData2 = B01000100;
  byte registerData3 = B00000100;
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, registerData3);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData2);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData1);
  digitalWrite(latchPin, HIGH);
  
  delay(500);
  
  registerData1 = B01010101;
  registerData2 = B10101010;
  registerData3 = B00001010;
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, registerData3);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData2);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData1);
  digitalWrite(latchPin, HIGH);
  
  delay(500);
  
  registerData1 = B10001000;
  registerData2 = B00010001;
  registerData3 = B00010000;
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, registerData3);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData2);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData1);
  digitalWrite(latchPin, HIGH);

  delay(500);
}

void onDisplayThreeProgram()
{
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);
  delay(500);
  
  byte registerData1 = B00100010;
  byte registerData2 = B01000100;
  byte registerData3 = B00000100;
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, registerData3);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData2);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData1);
  digitalWrite(latchPin, HIGH);
  
  delay(500);
  
  registerData1 = registerData1 | B01010101;
  registerData2 = registerData2 | B10101010;
  registerData3 = registerData3 | B00001010;
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, registerData3);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData2);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData1);
  digitalWrite(latchPin, HIGH);
  
  delay(500);
  
  registerData1 = registerData1 | B10001000;
  registerData2 = registerData2 | B00010001;
  registerData3 = registerData3 | B00010000;
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, registerData3);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData2);
  shiftOut(dataPin, clockPin, MSBFIRST, registerData1);
  digitalWrite(latchPin, HIGH);

  delay(500);
}

void onDisplayFourProgram()
{
  digitalWrite(latchPin, LOW);  
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);
  delay(500);

  byte dataStepOne[3]= {
    B00010000,
    B00010001,
    B10001000
  };
  
  digitalWrite(latchPin, LOW);
  for (int i=0; i<3; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dataStepOne[i]);
  }
  digitalWrite(latchPin, HIGH);

  delay(500);

  byte dataStepTwo[3] = {
    B00001010,
    B10101010,
    B01010101
  };
  digitalWrite(latchPin, LOW);
  
  for (int i=0; i<3; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dataStepOne[i] | dataStepTwo[i]);
  }
  digitalWrite(latchPin, HIGH);
  
  delay(500);

  byte dataStepThree[3] = {
    B00000100,
    B01000100,
    B00100010
  };
  
  digitalWrite(latchPin, LOW);
  for (int i=0; i<3; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, dataStepOne[i] | dataStepTwo[i] | dataStepThree[i]);
  }
  digitalWrite(latchPin, HIGH);
  
  delay(500);
}

/* Эта функция сдвигает биты влево на одну позицию, перемещая старший бит
на место младшего. Другими словами, она "вращает" биты по кругу.
Например, 11110000 превращается в 11100001.
*/
void rotateLeft(uint8_t &bits)
{
  uint8_t high_bit = bits & (1 << 7) ? 1 : 0;
  bits = (bits << 1) | high_bit;
}
void rotateRight(uint8_t &bits)
{
  uint8_t low_bit = bits & 1 ? (1 << 7) : 0;
  bits = (bits >> 1) | low_bit;
}

void onDisplayFirstProgram()
{
  byte startByte = 0b00000001;

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);   // передаем последовательно на dataPin
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, LSBFIRST, startByte<<1);
  digitalWrite(latchPin, HIGH); 
}
