# Autocar_project
BOARD: ATmega128a


| 기술 | 설명 |
| --- | --- |
| **UART** | 블루투스 수동 제어 통신 |
| **I2C** | LCD 제어 (I2C LCD 1602) |
| **PWM** | 모터 속도 제어 및 사이렌 (Timer1, Timer3) |
| **타이머/인터럽트** | Timer0(overflow), Timer1(PWM), Timer2(거리측정), Timer3(사이렌) |
| **초음파 센서** | 장애물 거리 감지 (좌/우/중앙) |
| **FSM 기반 알고리즘** | 주행 상태 전환 및 회피 로직 |
| **FND 디스플레이** | 수신 명령 시 상태 표시 |


### 🔹 수동 모드

- 블루투스를 통한 명령어 수신
    - `f` (forward), `l` (left), `b` (backward), `r` (right), `s` (stop)
- LCD: `Chill Driver` 출력
- FND: 명령어에 따른 상태 출력
- 사이렌 및 회피 기능 없음

### 🔸 자동 모드

- 주기적 초음파 거리 측정 (`ultra_check >= 80`)
- 장애물 거리 기반 회피, 후진, 방향전환
- LCD: `Mi chill Driver` 출력
- 사이렌 ON (`Auto_siren()`)
- 거리 기반 FSM 알고리즘 실행
