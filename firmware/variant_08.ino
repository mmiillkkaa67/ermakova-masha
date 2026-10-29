// Вариант 8. Комната отдыха. Система контроля температуры
// Плата: Arduino Uno (ATmega328P), симулятор Wokwi
// Подключение:
//   Датчик 1 (NTC, температура 0-40 °C)      -> A0
//   Датчик 2 (LDR, освещённость 0-100 %)     -> A1
//   Датчик 3 (PIR, присутствие)               -> D2
//   Реле K1 (обогреватель)                    -> D7
//   Светодиод HL1 (зелёный) через R1 = 200 Ом -> D8

const int PIN_S1 = A0;     // датчик 1: NTC (температура)
const int PIN_S2 = A1;     // датчик 2: LDR (освещённость), выход AO
const int PIN_S3 = 2;      // датчик 3: PIR (присутствие)
const int PIN_RELAY = 7;   // обогреватель
const int PIN_LED = 8;     // светодиод-индикатор

const unsigned long T_MS = 5000;  // период опроса T = 5 с
const float S1_MIN = 0.0;         // диапазон датчика 1: 0...40 °C
const float S1_MAX = 40.0;
const float S2_MIN = 0.0;         // диапазон датчика 2: 0...100 %
const float S2_MAX = 100.0;
const float THRESHOLD = 20.0;     // порог: температура < 20 °C

unsigned long n = 0;   // номер измерения
bool alarm = false;    // порог сейчас выполняется?

// Датчик 1: NTC (температура). Учебное допущение: шкала линейная
float readS1() {
  return S1_MIN + (S1_MAX - S1_MIN) * analogRead(PIN_S1) / 1023.0;
}

// Датчик 2: LDR (освещённость), линейная шкала 0-100 %
float readS2() {
  return S2_MIN + (S2_MAX - S2_MIN) * analogRead(PIN_S2) / 1023.0;
}

// Датчик 3: PIR (присутствие человека)
int readS3() {
  return digitalRead(PIN_S3);
}

void actuatorOn() {
  digitalWrite(PIN_RELAY, HIGH);
}

void actuatorOff() {
  digitalWrite(PIN_RELAY, LOW);
}

bool conditionMet(float s1, float s2, int s3) {
  return s1 < THRESHOLD;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_S3, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  actuatorOff();
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  n++;
  float s1 = readS1();
  float s2 = readS2();
  int s3 = readS3();

  Serial.print("DATA n=");  Serial.print(n);
  Serial.print(" s1=");     Serial.print(s1, 2);
  Serial.print(" s2=");     Serial.print(s2, 1);
  Serial.print(" s3=");     Serial.println(s3);

  bool cond = conditionMet(s1, s2, s3);

  if (cond && !alarm) {
    alarm = true;
    actuatorOn();
    digitalWrite(PIN_LED, HIGH);
    Serial.print("EVENT n="); Serial.print(n);
    Serial.println(" type=THRESHOLD");
  } else if (!cond && alarm) {
    alarm = false;
    actuatorOff();
    digitalWrite(PIN_LED, LOW);
    Serial.print("EVENT n="); Serial.print(n);
    Serial.println(" type=NORMAL");
  }

  delay(T_MS);
}
