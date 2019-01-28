int dataPin  = 9;   //Пин подключен к DS входу 74HC595
int latchPin = 10;  //Пин подключен к ST_CP входу 74HC595
int clockPin = 11;  //Пин подключен к SH_CP входу 74HC595

void setup() {
  //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, LOW);                        // устанавливаем синхронизацию "защелки" на LOW
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);   // передаем последовательно на dataPin
  shiftOut(dataPin, clockPin, LSBFIRST, 0b00000000);
  shiftOut(dataPin, clockPin, LSBFIRST, 0b11111111);
  digitalWrite(latchPin, HIGH);                       //"защелкиваем" регистр, тем самым устанавливая значения на выходах
}
 
void loop() {

}
