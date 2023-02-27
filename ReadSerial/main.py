import serial
import time
import asyncio


loop = asyncio.get_event_loop()

ser = serial.Serial('COM4', 9800, timeout=1)
f = open("data.txt", "w+") 

for i in range(1000):
    # Reading all bytes available bytes till EOL
    line = ser.readline() 
    if line:
        # Converting Byte Strings into unicode strings
        try:
            string = str(line.decode("utf-8"))
        except UnicodeDecodeError:
            string = str(line.decode("utf-8"))

        #every other line will be either the speed of the car, or the time of day

        f.write("""time: "%s" """ % string)

        print(string)
 
ser.close()
f.close()