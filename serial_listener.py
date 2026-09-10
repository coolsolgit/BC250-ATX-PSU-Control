import serial

# Replace '/dev/ttyACM0' with your actual Arduino port
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line == "SHELL_SHUTDOWN":
            os.system("sudo poweroff") 

# ser.close()
