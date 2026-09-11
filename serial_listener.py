# BC250 Linux에서(bazzite chchyOS)에 시리얼포트(ttyUSB0등)에 Listening을 걸면 BC250이 리부팅됨
# Nano호환보드의 USB칩인 CH320G문제인지 리눅스커널의 CH320드라이버 문제인지 아직 몰라 사용보류
# 하나의 Arduino로 시리얼포트까지 쓰니 문제가 생김, 결국 별도의 시리얼포트(USB to TTL)을 구매해서 BC250에 장착해서 해결
# poweroff권한없이 호출하려면 권한편집 > sudo nano /etc/sudoers
# 맨아래줄추가 > username ALL=(ALL) NOPASSWD: /usr/bin/systemctl poweroff, /usr/bin/shutdown

import serial
import subprocess

# Replace '/dev/ttyACM0' with your actual Arduino port
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line == "SHELL_SHUTDOWN":
            subprocess.run(["systemctl", "poweroff"], check=True)
