#define LED_PIN 7

unsigned long previousMillis = 0;
unsigned long period = 1000; // 1ms
int dutyCycle = 0;
int direction = 1; // 1: 증가, -1: 감소

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= period) {
    previousMillis = currentMillis;

    // 듀티 사이클 조절
    dutyCycle += direction;
    if (dutyCycle == 100) {
      direction = -1;
    } else if (dutyCycle == 0) {
      direction = 1;
    }

    // LED 상태 변경
    digitalWrite(LED_PIN, dutyCycle > 0);
  }
}

void set_period(int new_period) {
  period = new_period;
}

void set_duty(int new_duty) {
  // 최대 듀티 사이클은 100으로 제한
  dutyCycle = constrain(new_duty, 0, 100);
}
