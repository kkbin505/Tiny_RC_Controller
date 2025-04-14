#include <Arduino.h>
#include <CrsfSerial.h>
#include <Servo.h>
#include "config.h"

#define NUM_OUTPUTS 6
#define NUM_SERVOS 2
#define ELRS_ARM_CHANNEL 5
#define PWM_FREQ_HZ 50
#define ON_BOARD_LED_PIN 16

// CRSF channel map to output
constexpr int OUTPUT_MAP[NUM_OUTPUTS] = { 1, 2, 3, 4, 5, 6 };
constexpr int OUTPUT_FAILSAFE[NUM_OUTPUTS] = { 1500, 1500, 1500, 1500, 1000, 1000 };
#define HEAD_LED_PIN 12
constexpr uint8_t OUTPUT_PINS[NUM_OUTPUTS] = { SERVO_PIN, ESC_PIN, HEAD_LED_PIN,LEFT_STEERING_LED_PIN , RIGHT_STEERING_LED_PIN, REVERSE_LED_PIN }; // GPIOs
constexpr uint8_t crsfChannelMap[NUM_OUTPUTS] = {2, 3, 6, 2 , 2 ,3};  // CRSF channels to map

Servo my_Servos[NUM_SERVOS]; // Only for GPIO 4 and 5
int g_OutputsUs[NUM_OUTPUTS] = {0};

static CrsfSerial crsf(Serial);

// 存储每个通道上次切换的时间
unsigned long blinkTimers[NUM_OUTPUTS] = {0};

// 状态位：记录当前是 HIGH 还是 LOW
bool blinkStates[NUM_OUTPUTS] = {0};


void setPwmOutput(uint8_t ch, int channel_Value) {
  // Convert microseconds to duty (0-255) for analogWrite
  int duty = map(channel_Value, CRSF_CHANNEL_VALUE_1000, CRSF_CHANNEL_VALUE_2000, 0, 255); // Adjust to match duty range
  analogWrite(OUTPUT_PINS[ch], duty);
}

void servoSetup(uint8_t ch) {
  if (ch < NUM_SERVOS) { 
    my_Servos[ch].attach(OUTPUT_PINS[ch]);
  }
}

void servosOutput(uint8_t ch, int input_Values){
  int channel_Value = map(input_Values, CRSF_CHANNEL_VALUE_1000, CRSF_CHANNEL_VALUE_2000, 1000,2000);
  my_Servos[ch].writeMicroseconds(channel_Value);
}

void outputFailsafeValues() {
  for (uint8_t i = 0; i < NUM_SERVOS; i++) {
    servosOutput(i,OUTPUT_FAILSAFE[i]);
  }
}

void packetChannels() {

  for (uint8_t i = 0; i < NUM_OUTPUTS; i++) {
    unsigned long now = millis();
    int ch = OUTPUT_MAP[i];
    int channel_Value = ch > 0 ? crsf.getChannel(crsfChannelMap[ch]) : (3000 - crsf.getChannel(-ch));
    if ( i < NUM_SERVOS){
      servosOutput(i, channel_Value);
    }else if(i == 2){ // Headlight
      if (channel_Value > CRSF_CHANNEL_VALUE_MID){
        digitalWrite(OUTPUT_PINS[i],HIGH);
      }else{
        digitalWrite(OUTPUT_PINS[i],LOW);
      }
    }else if( i == 3){ // Leftsteering LED
      if (channel_Value > CRSF_CHANNEL_VALUE_MID + CHANNEL_MARGINE){
        if (now - blinkTimers[i] >= BLINK_INTERVAL_MS) {
          blinkTimers[i] = now;
          blinkStates[i] = !blinkStates[i]; // 翻转状态
          digitalWrite(OUTPUT_PINS[i], blinkStates[i] ? HIGH : LOW);
        }
      }else{
        // 如果不满足闪烁条件，保持 LOW
        digitalWrite(OUTPUT_PINS[i], LOW);
        blinkStates[i] = false;
      }
    }else if( i == 4){ // RightSteering LED
      if (channel_Value < CRSF_CHANNEL_VALUE_MID - CHANNEL_MARGINE){
        if (now - blinkTimers[i] >= BLINK_INTERVAL_MS) {
          blinkTimers[i] = now;
          blinkStates[i] = !blinkStates[i]; // 翻转状态
          digitalWrite(OUTPUT_PINS[i], blinkStates[i] ? HIGH : LOW);
        }
      }else{
        // 如果不满足闪烁条件，保持 LOW
        digitalWrite(OUTPUT_PINS[i], LOW);
        blinkStates[i] = false;
      }
    }else if( i == 5){ // Reverse LED
      if (channel_Value < CRSF_CHANNEL_VALUE_MID - CHANNEL_MARGINE){
        digitalWrite(OUTPUT_PINS[i],HIGH);
      }else{
        digitalWrite(OUTPUT_PINS[i],LOW);
      }
    }
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
    servosOutput(i,OUTPUT_FAILSAFE[i]);
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
  digitalWrite(ON_BOARD_LED_PIN, HIGH);
  delay(500);
  digitalWrite(ON_BOARD_LED_PIN, LOW);

}
