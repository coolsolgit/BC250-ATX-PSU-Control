import serial

# BC250 Linux에서(bazzite chchyOS)에 시리얼포트(ttyUSB0등)에 Listening을 걸면 BC250이 리부팅됨
# Nano호환보드의 USB칩인 CH320G문제인지 리눅스커널의 CH320드라이버 문제인지 아직 몰라 사용보류
# Replace '/dev/ttyACM0' with your actual Arduino port
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line == "SHELL_SHUTDOWN":
            os.system("sudo poweroff") 

# ser.close()
