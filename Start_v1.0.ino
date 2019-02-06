#define DATA_PIN 9 //Пин подключен к DS входу 74HC595
#define LATCH_PIN 10 //Пин подключен к ST_CP входу 74HC595
#define CLOCK_PIN 11 //Пин подключен к SH_CP входу 74HC595

unsigned long prevMillis = 0;
unsigned int interval = 500;
unsigned int programCounter = 0;

void setup()
{
  Serial.begin(9600);
  
  //устанавливаем режим OUTPUT
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
}

void loop()
{
  switch (5) {
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
    }
}

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

void onDisplayTwoProgram()
{
  setDataInRegisters(dataStepThree);
  //delay(500);
  setDataInRegisters(dataStepTwo);
  //delay(500);
  setDataInRegisters(dataStepOne);
  //delay(500);
}

void onDisplayThreeProgram()
{
  unsigned long currentMillis = millis();
 
  if (currentMillis - prevMillis > interval) {
    prevMillis = currentMillis;
    onThreeProgram();
  }

  return true;
}

void onThreeProgram()
{
  byte resultData[3];
  if (programCounter == 1) {
    setDataInRegisters(dataStepThree);
  }

  if (programCounter == 2) {
    for (int i=0; i<3; i++) {
       resultData[i] = dataStepThree[i] | dataStepTwo[i];
    }
    
    setDataInRegisters(dataStepTwo);
  }
  
  if (programCounter == 3) {
    for (int i=0; i<3; i++) {
       resultData[i] = dataStepOne[i] | dataStepTwo[i] | dataStepThree[i];
    }
    setDataInRegisters(dataStepOne);
  }
  
  if (programCounter == 4) {
    setRegistersNull();
    programCounter = 0;
  }
  
  programCounter++;

  return true;
}



void onDisplayFourProgram()
{
  unsigned long currentMillis = millis();
 
  if (currentMillis - prevMillis > interval) {
    prevMillis = currentMillis;
    doFourProgram();
  }

  return true;
}

void doFourProgram()
{
  byte resultData[3];
  if (programCounter == 1) {
    setDataInRegisters(dataStepOne);
  }
  if (programCounter == 2) {
     for (int i=0; i<3; i++) {
       resultData[i] = dataStepOne[i] | dataStepTwo[i];
    }
    setDataInRegisters(resultData);
  }

  if (programCounter == 3) {
    for (int i=0; i<3; i++) {
      resultData[i] = dataStepOne[i] | dataStepTwo[i] | dataStepThree[i];
    }
    
    setDataInRegisters(resultData);
  }

  if (programCounter == 4) {
    setRegistersNull();
    programCounter = 0;
  }
  
  programCounter++;

  return true;
}

void onDisplayFiveProgram()
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
}

void setDataInRegisters(byte *data)
{
  digitalWrite(LATCH_PIN, LOW);
  for (int i=0; i<3; i++) {
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data[i]);
  }
  digitalWrite(LATCH_PIN, HIGH);
  
  return true;
}

void setRegistersNull()
{
  digitalWrite(LATCH_PIN, LOW);  
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0b00000000);
  digitalWrite(LATCH_PIN, HIGH);
  return true;
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
int previousMillis = 0;

void onDisplayOneProgram()
{
  setRegistersUnits();
}

void setRegistersUnits()
{
  digitalWrite(LATCH_PIN, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b11111111);   // передаем последовательно на dataPin
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b11111111);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b11111111);
  digitalWrite(LATCH_PIN, HIGH);
}
