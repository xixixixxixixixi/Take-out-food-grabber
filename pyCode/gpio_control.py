# -*- coding:utf-8 -*-
import serial
import time
ser = serial.Serial('/dev/ttyAMA0', 115200)
print (1)
if ser.isOpen == False:
    ser.open()                # 打开串口
ser.write("e")
print (2)
try:
    while True:
        size = ser.inWaiting()
        #print (3)                # 获得缓冲区字符
        if size != 0:
            response = ser.read(size)  # 读取内容并显示
            #print (3)        
            print (response)
            ser.flushInput()                 # 清空接收缓存区
            time.sleep(0.1)                  # 软件延时
except KeyboardInterrupt:
    ser.close()
