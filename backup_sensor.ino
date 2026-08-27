/*
  Ultrasonic Automotive Backup Sensor
  Author: Shivam Shah
  Description: Dynamically maps distance to a 9-LED bar graph and piezo buzzer alert.
*/

const int PING_PIN = 13;
const int BUZZER_PIN = 11;
const int LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
const int NUM_LEDS = 9;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }
}

void loop() {
  long duration = getPingDuration();
  long cm = duration / 29 / 2;

  Serial.print(cm);
  Serial.println(" cm");

  updateFeedback(cm);
  delay(50);
}

long getPingDuration() {
  pinMode(PING_PIN, OUTPUT);
  digitalWrite(PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(PING_PIN, LOW);

  pinMode(PING_PIN, INPUT);
  return pulseIn(PING_PIN, HIGH);
}

void updateFeedback(long cm) {
  if (cm >= 90 || cm <= 0) {
    for (int i = 0; i < NUM_LEDS; i++) digitalWrite(LED_PINS[i], LOW);
    noTone(BUZZER_PIN);
    return;
  }

  // Calculate number of active LEDs based on distance range (10cm - 80cm)
  int ledsToLit = map(constrain(cm, 10, 80), 80, 10, 1, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], i < ledsToLit ? HIGH : LOW);
  }

  // Calculate tone frequency and delay interval dynamically
  int toneFreq = map(constrain(cm, 10, 80), 80, 10, 300, 500);
  int delayGap = map(constrain(cm, 10, 80), 80, 10, 1000, 10);

  tone(BUZZER_PIN, toneFreq);
  delay(100);
  noTone(BUZZER_PIN);
  delay(delayGap);
}
