#include <Arduino.h>
#include <CrsfSerial.h>
#include <Servo.h>
#include "config.h"

#define NUM_OUTPUTS 6
#define ELRS_ARM_CHANNEL 5
#define PWM_FREQ_HZ 50
#define ON_BOARD_LED_PIN 16

int NUM_SERVOS;

// CRSF channel map to output
constexpr int OUTPUT_MAP[NUM_OUTPUTS] = { 1, 2, 3, 4, 5, 6 };
constexpr int OUTPUT_FAILSAFE[NUM_OUTPUTS] = { 1500, 1500, 1500, 1500, 1000, 1000 };
#define HEAD_LED_PIN 12
constexpr uint8_t OUTPUT_PINS[NUM_OUTPUTS] = { SERVO_PIN, ESC_PIN, HEAD_LED_PIN,LEFT_STEERING_LED_PIN , RIGHT_STEERING_LED_PIN, REVERSE_LED_PIN }; // GPIOs
constexpr uint8_t crsfChannelMap[NUM_OUTPUTS] = {2, 3, 6, 7 , 8 ,3};  // CRSF channels to map

Servo g_Servos[2]; // Only for GPIO 4 and 5
int g_OutputsUs[NUM_OUTPUTS] = {0};

CrsfSerial crsf(Serial);

void outputFailsafeValues(); // <-- add this line

void setPwmOutput(uint8_t ch, int usec) {
  // Convert microseconds to duty (0-255) for analogWrite
  int duty = map(usec, 1000, 2000, 0, 255); // Adjust to match duty range
  analogWrite(OUTPUT_PINS[ch], duty);
  g_OutputsUs[ch] = usec;
}

void servoUpdates(uint8_t ch,int input_Values) {
  if (ch < 2) { // GPIO4, GPIO5 as Servo
    if (g_OutputsUs[ch] == 0) {
      g_Servos[ch].attach(OUTPUT_PINS[ch], 1000, 2000);
    }
    g_Servos[ch].writeMicroseconds(input_Values);
    g_OutputsUs[ch] = OUTPUT_FAILSAFE[ch];
  } else {
    setPwmOutput(ch, input_Values);
  }
}


void outputFailsafeValues() {
  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    servoUpdates(i,OUTPUT_FAILSAFE[i]);
  }
}

void packetChannels() {

  for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
    int ch = OUTPUT_MAP[i];
    int us = ch > 0 ? crsf.getChannel(ch) : (3000 - crsf.getChannel(-ch));
    servoUpdates(i, us);
  }
}

void crsfLinkUp() {
  digitalWrite(ON_BOARD_LED_PIN, HIGH);
}

void crsfLinkDown() {
  digitalWrite(ON_BOARD_LED_PIN, LOW);
  outputFailsafeValues();
}

void setup() {
  Serial.begin(420000);
  pinMode(ON_BOARD_LED_PIN, OUTPUT);
  digitalWrite(ON_BOARD_LED_PIN, LOW);

  // Initialize PWM outputs (GPIO12–15)
  for (int i = 2; i < NUM_OUTPUTS; i++) {
    pinMode(OUTPUT_PINS[i], OUTPUT);
  }

  // Initialize Servo outputs
  for (int i = 0; i< NUM_SERVOS; i++){
    servoUpdates(i,OUTPUT_FAILSAFE[i]);
  }
  // Initialize LED outputs
  for (int i = NUM_SERVOS ; i < NUM_OUTPUTS; i++){
    pinMode(i,OUTPUT);
  }


  crsf.onLinkUp = crsfLinkUp;
  crsf.onLinkDown = crsfLinkDown;
  crsf.onPacketChannels = packetChannels;
  crsf.begin();
}

void loop() {
  crsf.loop();
  for (int i = 0; i < NUM_SERVOS ; i++){
    int val = crsf.getChannel(crsfChannelMap[i]);
    servoUpdates(i,val);  // Send to servo
  }
  digitalWrite(ON_BOARD_LED_PIN, HIGH);
  delay(500);
  digitalWrite(ON_BOARD_LED_PIN, LOW);

}
