import serial
import time

# Replace '/dev/ttyACM0' with your actual Arduino port
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line == "SHUTDOWN":
            os.system("sudo poweroff") 

ser.close()
