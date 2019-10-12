#!/usr/bin/python
#coding= utf-8
import time
import serial
import json
import paho.mqtt.publish as publish

ser = serial.Serial("COM6", 9600)
# 选择串口，并设置波特率
host = "139.224.9.212"
topic = "v1/devices/me/telemetry"

def send_date(Id, paylod):
    user = {"username": Id, "password": ''}
    msg = json.dumps(paylod)
    try:
        publish.single(topic, msg, 0, False, host, 1883, Id, auth=user)
    except OSError:
        print("网络异常！")

def json_date():
    json = {"Lux": feedback_data}
    print(json)
    luxData = json
    send_date("TdVJlNS4MvLRsgYU28K2", {"Lux": luxData['Lux']})
    time.sleep(1)


if ser.is_open:
    print("port open success")
    try:
        while 1:
            try:
    # hex(16进制)转换为bytes(2进制)，应注意Python3.7与Python2.7此处转换的不同
                send_data = bytes.fromhex('A5 09 AE')    # 发送数据转换为b'\xff\x01\x00U\x00\x00V'
                ser.write(send_data)   # 发送命令
                time.sleep(0.1)        # 延时，否则len_return_data将返回0，此处易忽视！！！
                len_return_data = ser.inWaiting()  # 获取缓冲数据（接收数据）长度
                if len_return_data:
                    return_data = ser.read(len_return_data)  # 读取缓冲数据
                    # bytes(2进制)转换为hex(16进制)，应注意Python3.7与Python2.7此处转换的不同，并转为字符串后截取所需数据字段，再转为10进制
                    str_return_data = str(return_data.hex())
                    feedback_data = int(str_return_data[-6:-2], 16)/100
                    print("光照强度:",round(feedback_data,1),"Lux")
                    json_date()
            except  KeyboardInterrupt:
                    print("over")
                    time.sleep(10)
    except Exception:
        print("port open failed")
# else:
 #   print("port open failed")