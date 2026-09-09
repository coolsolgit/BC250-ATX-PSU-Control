import serial
import time

# 포트 이름과 통신 속도(Baud rate) 설정
port = "/dev/ttyACM0"
baud = 9600

ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)  # 아두이노 리셋 대기

# 데이터 전송
ser.write(b"Hello Arduino\n")

# 데이터 수신
if ser.in_waiting > 0:
  line = ser.readline().decode("utf-8").strip()
  print("Received:", line)

ser.close()
