# -*- coding:utf-8 -*-
#from aip import AipFace
import json
import cv2
import base64
import voice_composer
import base64
import requests
import urllib
import urllib2
import sys
import time
import serial
import math
reload(sys)
sys.setdefaultencoding('utf8')
IS_PY3 = sys.version_info.major == 3
if IS_PY3:
    from urllib.request import urlopen
    from urllib.request import Request
    from urllib.error import URLError
    from urllib.parse import urlencode
    from urllib.parse import quote_plus
else:
    import urllib2
    from urllib import quote_plus
    from urllib2 import urlopen
    from urllib2 import Request
    from urllib2 import URLError
    from urllib import urlencode

def photo():
    #相机拍照
    cap = cv2.VideoCapture(0)
    cv2.namedWindow("capture",0);
    cv2.resizeWindow("capture", 320, 240);
    while (1):
        ret, frame = cap.read()
        cv2.imshow("capture", frame)
        input=cv2.waitKey(1)&0xFF
        if (input==ord('s')):
            cv2.imwrite('test.png', frame)
            cv2.destroyAllWindows()
            cap.release()
            break
            
def photo2():
    #相机拍照
    cap = cv2.VideoCapture(0)
    ret, frame = cap.read()
    cv2.imwrite('face.png', frame)
    cap.release()
    cv2.destroyAllWindows()

def iden():#return 1代表识别用户成功
    request_url = "https://aip.baidubce.com/rest/2.0/face/v3/search"
    with open("face.png","rb") as f:
        img = base64.b64encode(f.read())#使用base64进行加密
    img=bytes.decode(img)
    params = "{\"image\":\""+img+"\",\"image_type\":\"BASE64\",\"group_id_list\":\"man\",\"quality_control\":\"LOW\",\"liveness_control\":\"NORMAL\"}"

    access_token = "24.8c25fe0fbfb4b3921c3b474ef779695d.2592000.1564101688.282335-16608541"
    header = {'Content-Type': 'application/json'}
    request_url = request_url + "?access_token=" + access_token
    request = requests.post(url=request_url, data=params, headers=header)
    x = request.text
    x=json.loads(x)
    print(x)
    if(str(x.get('result'))=="None"):
        return 0
    else:
        print(x.get('result').get('user_list')[0].get('user_id'))
        username=x.get('result').get('user_list')[0].get('user_id')

    if(username=='liyanhao'):
        TEXT = "欢迎使用本管家机器人，李彦澔先生"
        print(TEXT)
        voice_composer.playname(TEXT)
        return 1
    elif(username=='dongxiao'):
        TEXT = "欢迎使用本管家机器人，董校先生"
        print(TEXT)
        voice_composer.playname(TEXT)
        return 1
    elif(username=='songjialin'):
        TEXT = "欢迎使用本管家机器人，宋嘉琳女士"
        print(TEXT)
        voice_composer.playname(TEXT)
        return 1
    else:
        return 0
        
def iden1():#return 1代表识别用户成功
    request_url = "https://aip.baidubce.com/rest/2.0/face/v3/search"
    with open("face.png","rb") as f:
        img = base64.b64encode(f.read())#使用base64进行加密
    img=bytes.decode(img)
    params = "{\"image\":\""+img+"\",\"image_type\":\"BASE64\",\"group_id_list\":\"man\",\"quality_control\":\"LOW\",\"liveness_control\":\"NORMAL\"}"

    access_token = "24.8c25fe0fbfb4b3921c3b474ef779695d.2592000.1564101688.282335-16608541"
    header = {'Content-Type': 'application/json'}
    request_url = request_url + "?access_token=" + access_token
    request = requests.post(url=request_url, data=params, headers=header)
    x = request.text
    x=json.loads(x)
    print(x)
    if(str(x.get('result'))=="None"):
        return 2
    else:
        print(x.get('result').get('user_list')[0].get('user_id'))
        username=x.get('result').get('user_list')[0].get('user_id')
        prob=x.get('result').get('user_list')[0].get('score')
    if(username=='liyanhao' and prob>=50):
        TEXT = "李彦澔先生回来了"
        print(TEXT)
        voice_composer.playname(TEXT)
        return 1
    elif(username=='dongxiao' and prob>=50):
        TEXT = "董校先生回来了"
        print(TEXT)
        voice_composer.playname(TEXT)
        return 1
    else:
        return 0

def dish_detect():
    request_url = "https://aip.baidubce.com/rest/2.0/image-classify/v2/dish"
    # 二进制方式打开图片文件
    f = open('test.png', 'rb')
    img = base64.b64encode(f.read())

    params = {"image": img}
    params = urlencode(params)

    access_token = "24.7679bf9ad21f76e8079905ee666912eb.2592000.1563957119.282335-16619321"
    header = {'Content-Type': 'application/x-www-form-urlencoded'}
    request_url = request_url + "?access_token=" + access_token
    request = requests.post(url=request_url, data=params, headers=header)
    content = request.text
    content = json.loads(content)
    print(content)
    return content


def word_detect():
    request_url = "https://aip.baidubce.com/rest/2.0/ocr/v1/accurate_basic"
    # 二进制方式打开图文件
    f = open('test.png', 'rb')
    # 参数image：图像base64编码
    img = base64.b64encode(f.read())
    params = {"image": img}
    params = urlencode(params)

    access_token = "24.2a363e3bae57730bbbdd784695c117e0.2592000.1563965537.282335-16623548"
    header = {'Content-Type': 'application/x-www-form-urlencoded'}
    request_url = request_url + "?access_token=" + access_token
    request = requests.post(url=request_url, data=params, headers=header)
    content = request.text
    content = (json.loads(content))['words_result']
    c=''
    for i in range(len(content)):
        c=c+content[i]["words"]
    if(c.isspace() == True):
        c="识别错误"
    print(c)
    print(content)
    return c

'''
人体关键点识别
'''
def human():
    request_url = "https://aip.baidubce.com/rest/2.0/image-classify/v1/body_analysis"

    # 二进制方式打开图片文件
    f = open('face.png', 'rb')
    img = base64.b64encode(f.read())


    params = {"image":img}
    params = urllib.urlencode(params)

    access_token = '24.0adb412c16b57ed6c9f7ec7bf75cfff5.2592000.1565092630.282335-16730442'
    request_url = request_url + "?access_token=" + access_token
    request = urllib2.Request(url=request_url, data=params)
    request.add_header('Content-Type', 'application/x-www-form-urlencoded')
    response = urllib2.urlopen(request)
    content = response.read()
    print(content)
    if content:
        return content
    
def item_detect():
    request_url = "https://aip.baidubce.com/rest/2.0/image-classify/v1/object_detect"
    # 二进制方式打开图片文件
    image=cv2.imread('face.png')
    image=cv2.resize(image,(100,100),interpolation=cv2.INTER_CUBIC)
    cv2.imwrite('item.png', image)
    f = open('item.png', 'rb')
    img = base64.b64encode(f.read())

    params = {"image":img,"with_face":1}
    params = urllib.urlencode(params)

    access_token = '24.0adb412c16b57ed6c9f7ec7bf75cfff5.2592000.1565092630.282335-16730442'
    request_url = request_url + "?access_token=" + access_token
    request = urllib2.Request(url=request_url, data=params)
    request.add_header('Content-Type', 'application/x-www-form-urlencoded')
    response = urllib2.urlopen(request)
    content = response.read()
    content=json.loads(content)
    print(content)
    content=content.get("result")
    content=json.dumps(content)
    content=json.loads(content)
    left=content.get("left")
    width=content.get("width")
    target=(2*left+width)/2
    move=50.0-float(target)
    ang=move/(50*1.73)
    ang=(math.atan(ang))*57.3
    print(ang)
    return ang


"""
import  sys
import ssl
from urllib.request import urlopen
from urllib.request import Request

# client_id 为官网获取的AK， client_secret 为官网获取的SK
host = 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=D0UkKHRU8oTGxmqm8AzFAZN3&client_secret=IM6p0oKQf3igtsWQ7E7OcI15isj1blQF'
request = Request(host)
request.add_header('Content-Type', 'application/json; charset=UTF-8')
response = urlopen(request)
content = response.read()
if (content):
    print(content)
"""

