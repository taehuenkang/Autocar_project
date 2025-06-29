## 🚗 `AutoCar_Project` – FSM 기반 AVR 자율주행차

```markdown
# 🚗 AutoCar FSM Project – AVR 기반 자율주행차

A simple FSM-controlled embedded car using **ATmega128a** microcontroller and ultrasonic sensors.

---

## 🎯 Features

- FSM-based auto driving:
  - Forward / Stop / Avoid obstacle / Reverse
- Manual mode via **UART Bluetooth**
- Real-time LCD and FND display
- Interrupt & Timer-based control

---

## 💡 Tech Stack

| Item          | Description                      |
|---------------|----------------------------------|
| MCU           | ATmega128a (AVR)                 |
| Sensors       | 3x Ultrasonic (Left, Center, Right) |
| Display       | LCD1602 (I2C), 4-digit FND       |
| Comm          | UART (Bluetooth), I2C            |
| Language      | C (Atmel Studio)                 |

---

## 🧭 Architecture

```txt
Manual Mode     <----- Bluetooth (UART)
  │
  └──> LCD, FND ←── FSM ←── Ultrasonic Sensors
                      │
                   Timer 0~3: PWM, Delay, Beep, Measurement
🔁 FSM Logic
Forward: Safe distance

Avoid: If front obstacle

Turn left/right: Side detection

Reverse: Stuck state

🔧 Timers Used
Timer	Purpose
Timer0	1ms system tick
Timer1	PWM motor control
Timer2	Ultrasonic echo timer
Timer3	Siren sound

▶️ Run Instructions
Build & flash via Atmel Studio

Use serial Bluetooth app to send:

f: forward / b: backward / l: left / r: right / s: stop
