#include <Servo.h>
#include <math.h> // 수학 라이브러리 추가

#define PIN_SERVO 10

Servo myServo;
unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
unsigned long moveStartTime;
int startAngle = 0;
int stopAngle  = 180;


void setup() {
  Serial.begin(57600);
  myServo.attach(PIN_SERVO);
  moveStartTime = millis(); // start moving

  myServo.write(startAngle); // Set initial position
  delay(500);
}

void loop() {
  unsigned long progress = millis() - moveStartTime;

  if (progress <= MOVING_TIME) {
    // -6에서 6 사이의 x 값을 계산
    float x = ((float)progress / (float)MOVING_TIME) * 12.0 - 6.0;
    // 시그모이드 함수 계산
    float sigmoid = 1 / (1 + exp(-x));
    // 시그모이드 값을 기반으로 각도를 계산 (0도에서 180도 사이)
    int angle = startAngle + (int)(sigmoid * (stopAngle - startAngle));
    myServo.write(angle); 
    Serial.print("Sigmoid: ");
    Serial.print(sigmoid);
    Serial.print(", Angle: ");
    Serial.println(angle);
  }
}
