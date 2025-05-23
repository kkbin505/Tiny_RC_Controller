# ESP01F_LED_Control_Board
 Tiny RC Controlleer is an ESP8285 based control board for micro RC vehicle and flying Wing. It supports CRSF (ExpressLRS) receiver.

  功能特性
✅ 支持 CRSF 协议（如 ELRS 接收机）

✅ 可控制 最多 2 路舵机 + 多路 LED 灯

✅ 支持混控

✅ 支持 方向灯、倒车灯、双闪灯等车灯逻辑

✅ 计划未来支持 Wi-Fi OTA 无线升级和调参

✅ 小巧板载设计，适合嵌入式项目
 

 # 技术规格

| 项目     | 说明                                    |
| ------ | ------------------------------------- |
| 主控芯片   | ESP8285（等效 ESP8266 + Flash）           |
| 通信协议   | CRSF（Crossfire / ELRS）                |
| 舵机输出通道 | 2 路（GPIO12, GPIO13）                   |
| 灯光控制输出 | GPIO4, GPIO5, GPIO14, GPIO15          |
| 姿态传感器  | MPU6050（I2C 接口，GPIO5/SDA & GPIO0/SCL） |
| 串口波特率  | 420000（默认，兼容 CRSF）                    |
| 电源     | 3.3V 供电（建议外接稳压）                       |
| OTA 支持 | 是（Wi-Fi 升级）                           |
| 尺寸     | 超小尺寸，适配定制壳体/载具                        |


🛠️ 开发环境建议

PlatformIO（推荐）

Arduino IDE（也可用）

📎 相关资源
GitHub 地址：https://github.com/yourname/tiny-rc-controller

ELRS 协议说明：ExpressLRS.org

❤️ 作者
Created by [kkbin505]
欢迎使用、修改、分享、提 Issue！