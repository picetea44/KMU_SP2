// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// Configurable parameters
#define SND_VEL 346.0     // Sound velocity at 24 Celsius degree (m/sec)
#define INTERVAL 25      // Sampling interval (msec)
#define PULSE_DURATION 10 // Ultra-sound pulse duration (usec)
#define _DIST_MIN 100.0   // Minimum distance to be measured (mm)
#define _DIST_MAX 300.0   // Maximum distance to be measured (mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // Maximum echo waiting time (usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // Coefficient to convert duration to distance

unsigned long last_sampling_time;   // Last sampling time (msec)

void setup() {
  // Initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // Sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // Sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // Turn off sonar
  
  // Initialize serial port
  Serial.begin(57600);
}

void loop() {
  float distance;
  float temp_distance; // 거리 정보 임시 저장 
  unsigned int brightness; // 0 : LED 최대 밝기, 255 : LED 꺼짐
  

  // Wait until next sampling time
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  // Read the distance
  distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // Handle out-of-range distances
  if (distance == 0.0 || distance > _DIST_MAX) {
    distance = _DIST_MAX + 10.0;
    temp_distance = _DIST_MAX;
    brightness = 0; 
    analogWrite(PIN_LED, brightness);        
  } else if (distance < _DIST_MIN) {
    distance = _DIST_MIN - 10.0;
    temp_distance = _DIST_MIN;
    brightness = 0;
    analogWrite(PIN_LED, brightness);
  } else {
    
    temp_distance = distance;

    if (distance <= 200) {
      // 200mm 이하에서의 밝기
      brightness = (temp_distance - 100.0) * (255.0 / 100.0);  // 값이 작아질수록 밝기 증가
    } else {
      // 200mm ~ 300mm에서의 밝기 
      brightness = (300.0 - temp_distance) * (255.0 / 100.0);  // 값이 커질수록 밝기 증가 
    }

    
    analogWrite(PIN_LED, brightness);
   
  }

  // Output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(", Distance:"); Serial.print(distance);
  Serial.print(", Max:");      Serial.print(_DIST_MAX);
  Serial.print(", temp_distance:");      Serial.print(temp_distance);
  Serial.print(", Brightness:");      Serial.print(brightness);
  Serial.println("");

  // Update last sampling time
  last_sampling_time += INTERVAL;
}

// Function to get a distance reading from the ultrasonic sensor (in mm)
float USS_measure(int TRIG, int ECHO) {
  // Send pulse
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  // Measure the duration of the pulse in microseconds and convert to distance in mm
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;

  // Example of pulse duration to distance conversion (target distance = 17.3m)
  // pulseIn(ECHO, HIGH, timeout) returns microseconds (round-trip time of sound)
  // One-way distance = (pulseIn() / 1,000,000) * SND_VEL / 2 (meters)
  // To get mm, multiply by 1,000 -> SCALE = 0.001 * 0.5 * SND_VEL
  //
  // Example: if pulseIn() returns 100,000 (0.1 second, round-trip distance = 34.6m)
  //          distance = 100,000 * 0.001 * 0.5 * 346 = 17,300 mm  ==> 17.3m
}
