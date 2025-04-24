#include <Arduino.h>
#include <CrsfSerial.h>
#include <Servo.h>
#include <config.h>


Servo servo;
Servo esc;
int crsfChannelValues[CRSF_NUM_CHANNELS];

CrsfSerial crsf(Serial);

unsigned long previousMillisRight = 0;   // 上次右LED闪烁的时间
unsigned long previousMillisLeft = 0;    // 上次左LED闪烁的时间
bool steeringLedState = LOW;      // 右LED当前的状态
// bool leftState = LOW;       // 左LED当前的状态

void crsfLinkUp() {
  digitalWrite(ON_BOARD_LED_PIN, HIGH);
}

void crsfLinkDown() {
  digitalWrite(ON_BOARD_LED_PIN, LOW);
  for (uint8_t i = 0; i < CRSF_NUM_CHANNELS; i++) {
    crsfChannelValues[i] = FAIL_SAFE_VALUE;
  }
  digitalWrite(HEAD_LED_PIN, LOW);
  digitalWrite(RIGHT_LED_PIN, LOW);
  digitalWrite(LEFT_LED_PIN, LOW);
  digitalWrite(REVERSE_LED_PIN, LOW);
}

void onCrsfChannels() {
  for (uint8_t i = 0; i < CRSF_NUM_CHANNELS; i++) {
    crsfChannelValues[i] = crsf.getChannel(i);
  }
}

void setup() {
  Serial.begin(420000);
  // LED初始化
  pinMode(ON_BOARD_LED_PIN, OUTPUT);
  digitalWrite(ON_BOARD_LED_PIN, HIGH);
  pinMode(RIGHT_LED_PIN, OUTPUT);
  digitalWrite(RIGHT_LED_PIN, HIGH);
  pinMode(LEFT_LED_PIN , OUTPUT);
  digitalWrite(LEFT_LED_PIN , HIGH);
  pinMode(HEAD_LED_PIN, OUTPUT);
  digitalWrite(HEAD_LED_PIN, HIGH);
  pinMode(REVERSE_LED_PIN, OUTPUT);
  digitalWrite(REVERSE_LED_PIN,HIGH);
  delay(1000);

  for (uint8_t i = 0; i < CRSF_NUM_CHANNELS; i++) {
    crsfChannelValues[i] = FAIL_SAFE_VALUE;
  }

  // 初始化舵机输出
  servo.attach(SERVO_PIN);
  servo.writeMicroseconds(FAIL_SAFE_VALUE);
  esc.attach(ESC_PIN);
  esc.writeMicroseconds(FAIL_SAFE_VALUE);

  // 初始化CRSF
  crsf.onLinkUp = crsfLinkUp;
  crsf.onLinkDown = crsfLinkDown;
  crsf.onPacketChannels = onCrsfChannels;
  crsf.begin();
}

void loop() {
  crsf.loop();

  // 输出 PWM 给舵机
  servo.writeMicroseconds(crsfChannelValues[CRSF_STEERING_CHANNEL]);
  esc.writeMicroseconds(crsfChannelValues[CRSF_ESC_CHANNEL]);

  // Head LightLED
  if (crsfChannelValues[CRSF_HEAD_LIGHT_CHANNEL] > PWM_MID) {
    digitalWrite(HEAD_LED_PIN, HIGH);
  } else {
    digitalWrite(HEAD_LED_PIN, LOW);
  }
  
  // 右LED和左LED的闪烁逻辑
  int steering = crsfChannelValues[CRSF_STEERING_CHANNEL];
  int hazard = crsfChannelValues[CRSF_HAZARD_LIGHT_CHANNEL];
  unsigned long currentMillis = millis();
  if (hazard > PWM_MID) {
    // 双闪模式：交替闪烁
    if (currentMillis - previousMillisRight >= BLINK_INTERVAL_MS) {
      previousMillisRight = currentMillis;
      steeringLedState = !steeringLedState;
  
      digitalWrite(RIGHT_LED_PIN, steeringLedState);
      digitalWrite(LEFT_LED_PIN, steeringLedState);
    }
  } else {
    // 正常转向灯逻辑
    if (currentMillis - previousMillisRight >= BLINK_INTERVAL_MS) {
      previousMillisRight = currentMillis;
      steeringLedState = !steeringLedState;
  
      if (steering > PWM_MID + DEAD_BAND) {
        digitalWrite(RIGHT_LED_PIN, steeringLedState);
        digitalWrite(LEFT_LED_PIN, LOW);
      } else if (steering < PWM_MID - DEAD_BAND) {
        digitalWrite(RIGHT_LED_PIN, LOW);
        digitalWrite(LEFT_LED_PIN, steeringLedState);
      } else {
        digitalWrite(RIGHT_LED_PIN, LOW);
        digitalWrite(LEFT_LED_PIN, LOW);
      }
    }
  }

  // 倒车灯控制
  if (crsfChannelValues[CRSF_ESC_CHANNEL] > PWM_MID + DEAD_BAND) {  // 如果是反转（你可根据 ESC 设定微调）
    digitalWrite(REVERSE_LED_PIN, HIGH);
  } else {
    digitalWrite(REVERSE_LED_PIN, LOW);
  }
  
  // 可选调试输出
  // Serial.println(ch);
}
