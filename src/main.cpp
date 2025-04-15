#include <Arduino.h>
#include <CrsfSerial.h>
#include <config.h>

#define ON_BOARD_LED_PIN 16
#define CRSF_HEAD_LIGHT_CHANNEL 5
#define CRSF_STEERING_CHANNEL 4
#define PWM_MID 1500
#define RIGHT_LED_PIN 4
#define LEFT_LED_PIN  5
#define DEAD_BANBD  200

CrsfSerial crsf(Serial);

unsigned long previousMillisRight = 0;   // 上次右LED闪烁的时间
unsigned long previousMillisLeft = 0;    // 上次左LED闪烁的时间
bool steeringLedState = LOW;      // 右LED当前的状态
// bool leftState = LOW;       // 左LED当前的状态

void setup() {
  Serial.begin(420000);
  pinMode(ON_BOARD_LED_PIN, OUTPUT);
  digitalWrite(ON_BOARD_LED_PIN, LOW);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  digitalWrite(RIGHT_LED_PIN, LOW);
  pinMode(LEFT_LED_PIN , OUTPUT);
  digitalWrite(LEFT_LED_PIN , LOW);
  crsf.begin();
}

void loop() {
  crsf.loop();

  int ch = crsf.getChannel(CRSF_HEAD_LIGHT_CHANNEL);
  if (ch > PWM_MID) {
    digitalWrite(ON_BOARD_LED_PIN, HIGH);
  } else {
    digitalWrite(ON_BOARD_LED_PIN, LOW);
  }
  ch = crsf.getChannel(CRSF_STEERING_CHANNEL);
  // 右LED和左LED的闪烁逻辑
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisRight >= BLINK_INTERVAL_MS) {
    previousMillisRight = currentMillis;
    steeringLedState = !steeringLedState;  // 切换右LED状态
    if (ch > PWM_MID + DEAD_BANBD) {
      digitalWrite(RIGHT_LED_PIN, steeringLedState);
      digitalWrite(LEFT_LED_PIN, LOW);  // 确保左LED熄灭
    }else if (ch < PWM_MID - DEAD_BANBD) {
      digitalWrite(RIGHT_LED_PIN, LOW);
      digitalWrite(LEFT_LED_PIN, steeringLedState);  // 确保右LED熄灭
    }else {
      digitalWrite(RIGHT_LED_PIN, LOW);
      digitalWrite(LEFT_LED_PIN , LOW);
    }
  }
  

  // 可选调试输出
  // Serial.println(ch);
}
