import serial
import time
import nvsmi
from datetime import datetime
import ctypes
 
arduino = serial.Serial(port='COM3', baudrate=115200, timeout=.1)


def write(text):
    arduino.write(bytes(text, 'utf-8'))


def get_uptime():
    lib = ctypes.windll.kernel32
    t = lib.GetTickCount64()
    t = int(str(t)[:-3])
    min, sec = divmod(t, 60)
    hour, min = divmod(min, 60)
    return f"{hour:02}:{min:02}"

while True:
    gpu = next(nvsmi.get_gpus())
    usage = str(gpu.gpu_util).rsplit(".", 1)[0] + "%"
    temp = str(gpu.temperature).rsplit(".", 1)[0] + "C"
    write(f"Temp {temp}\nUsage {usage},Time {datetime.now().strftime('%H:%M')}\nUp {get_uptime()}")
    time.sleep(1)
