# Arduino UNO/NANO에서는 SerialPort가 열리는 순간 Arduino가 Reset되도록 설계되어있음
# 이러한 이유로 정상적인 사용을 위해서는 별도의 USB to TTL 모듈을 장착해야 정상적인 ATX전원관리가 가능 
# Nano의 GND와 TTL의 GND를 연결하고, Nano의 Tx를 TTL모듈의 Rx로 연결하여 BC250과 통신하도록 연결 
# poweroff권한없이 호출하려면 권한편집 > sudo nano /etc/sudoers
# 맨아래줄추가 > username ALL=(ALL) NOPASSWD: /usr/bin/systemctl poweroff, /usr/bin/shutdown

import serial
import subprocess

# 자신의 시스템에 인식된 tty device명으로 변경하세요.
# '/dev/ttyACM0', '/dev/ttyUSB0'등
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line == "SHELL_SHUTDOWN":
            subprocess.run(["systemctl", "poweroff"], check=True)
