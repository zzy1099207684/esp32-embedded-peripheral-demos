import serial
import re

ser = serial.Serial('/dev/ttyACM0', 5*1024000)  # Change this to your serial port.
data = ""
capturing = False

while True:
    line = ser.readline().decode('utf-8', errors='ignore')

    if "JPEG_START" in line:
        data = ""
        capturing = True
    elif "JPEG_END" in line:
        # Convert hex text back to binary JPEG bytes.
        jpeg_bytes = bytes.fromhex(data)
        with open('image.jpg', 'wb') as f:
            f.write(jpeg_bytes)
        print(f"Saved image, size: {len(jpeg_bytes)} bytes")
        capturing = False
    elif capturing:
        data += line.strip()
