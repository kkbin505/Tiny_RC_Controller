# ESP01F_LED_Control_Board
 Tiny RC Controller 是一个基于ESP8285 的解码版，支持双路舵机输出，集成LED灯控，支持大灯，双闪，转向联动，倒车灯联动等混控功能。体积小巧，特别适合1:87静改动项目，支持CRSF协议，可以使用微型CRSF串口接收机.

# 功能特性

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
| 串口波特率  | 420000（默认，兼容 CRSF）                    |
| 电源     | 3.3V 供电（建议外接稳压）                       |


# 🛠️ 开发环境建议

PlatformIO（推荐）

Arduino IDE（也可用）

# 📎 相关资源
GitHub 地址：https://github.com/kkbin505/Tiny_RC_Controller

ELRS 协议说明：ExpressLRS.org

# ❤️ 作者
Created by [kkbin505]
欢迎使用、修改、分享、提 Issue！

# ESP01F_LED_Control_Board

**Tiny RC Controller** is an ESP8285-based control board designed for micro RC vehicles, especially for HO 1:87 or 1:64 model. It supports CRSF (ExpressLRS) receivers.


---

## ✨ Features

- ✅ Supports **CRSF** protocol (e.g., ExpressLRS receivers)
- ✅ Controls up to **2 servos** and **multiple LED channels**
- ✅ Built-in **indicator, reverse light, hazard flash** logic
- ✅ Planned support for **Wi-Fi OTA firmware upgrade & tuning**
- ✅ **Compact onboard design**, perfect for tiny rc projects

---

## 📐 Technical Specifications

| Item             | Description                                   |
|------------------|-----------------------------------------------|
| MCU              | ESP8285 (equivalent to ESP8266 + Flash)       |
| Communication    | CRSF (Crossfire / ExpressLRS)                 |
| Servo Channels   | 2 Channels steering & ESC                |
| LED Outputs      | GPIO4, GPIO5, GPIO14, GPIO15                  |
| UART Baud Rate   | 420000 (default, compatible with CRSF)        |
| Power Supply     | 1s (external regulator recommended)         |


---

## 📎 Resources

- 🔗 GitHub Repository: [Tiny_RC_Controller](https://github.com/kkbin505/Tiny_RC_Controller)
- 📘 CRSF Protocol Reference: [ExpressLRS.org](https://www.expresslrs.org)

---

## ❤️ Author

Created by **[kkbin505](https://github.com/kkbin505)**  
Feel free to use, modify, share, or open an issue if you have any feedback!

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
