# -*- coding:utf-8 -*-
import voice_composer
import baiduface
import sys
import serial
import time
import textmyself
import json

ser = serial.Serial('/dev/ttyAMA0', 115200)
if ser.isOpen == False:
    ser.open()
reload(sys)
sys.setdefaultencoding('utf8')

#树莓派中设置启动条件来调用这个服务程序
def im_iden():
    while(1):
        TEXT= "请您进行身份认证"
        print(TEXT)
        voice_composer.playname(TEXT)
        baiduface.photo2()
        flag=baiduface.iden()
        if(flag==1):
            break
        elif(flag==0):
            TEXT = "身份验证失败，请再试一次"
            print(TEXT)
            voice_composer.playname(TEXT)
            
def im_iden1():
    while(1):
        baiduface.photo2()
        flag=baiduface.iden1()
        if(flag==1):
            break
        elif(flag==2):
            continue
        elif(flag==0):
            TEXT = "陌生人进来了,陌生人进来了"
            print(TEXT)
            voice_composer.playname(TEXT)
            textmyself.textmyself("Stranger came into your house!")
            ser.write("i")
            break
    
def mn():
    while(1):
        TEXT = "您好,请问您需要些什么"
        print(TEXT)
        voice_composer.playname(TEXT)
        TEXT=voice_composer.voice_iden()
        if("不" in TEXT):
            TEXT = "收到,有需要时请随时吩咐"
            print(TEXT)
            voice_composer.playname(TEXT)
            return 0
        elif("站岗" in TEXT):
            TEXT= "好的，有人来了我会及时提醒您"
            print(TEXT)
            voice_composer.playname(TEXT)
            ser.write("g")
            while(1):
                baiduface.photo2()
                a=json.loads(baiduface.human())
                print(str(a.get('person_num')))
                if(str(a.get('person_num'))>'0'):
                    im_iden1()
                    ser.write("h")
                    while True:
                        size = ser.inWaiting()
                        if size != 0:
                            response = ser.read(size)  # 读取内容并显示      
                            print (response)
                            ser.flushInput() # 清空接收缓存区
                            time.sleep(0.1)
                            if(response=='p'):
                                voice_composer.play("result.wav")
                                return 0
        elif("书" in TEXT):
            TEXT= "收到,这就为您去拿书"
            ser.write("e")
            voice_composer.playname(TEXT)
            while(1):
                baiduface.photo2()
                ang=baiduface.item_detect()
                if(ang<0):# you zhuan
                    if(ang<=-10) and (ang>-20):#xiao you zhaun
                        ser.write("1")
                    elif(ang<=-20) and (ang>=-30):#da you zhaun
                        ser.write("2")
                    else:
                        ser.write("7")
                else:# zuo zhuan
                    if(ang>=10) and (ang<20):#xiao
                        ser.write("3")
                    elif(ang>=20) and (ang<=30):#da
                        ser.write("4")
                    else:
                        ser.write("7")
                size = ser.inWaiting()
                if size != 0:
                    response = ser.read(size)  # 读取内容并显示      
                    print (response)
                    ser.flushInput() # 清空接收缓存区
                    time.sleep(0.1)
                if(response=='9'):
                    break
            while True:
                size = ser.inWaiting()
                if size != 0:
                    response = ser.read(size)  # 读取内容并显示      
                    print (response)
                    ser.flushInput() # 清空接收缓存区
                    time.sleep(0.1)
                    if(response=='q'):
                        TEXT= "书拿来了"
                        print(TEXT)
                        voice_composer.playname(TEXT)
                        return 0
        elif("着火" in TEXT):
            ser.write("i")
            return 0
        elif("聊天" in TEXT):
            TEXT= "好呀，主人，你想要聊些什么？"
            print(TEXT)
            voice_composer.playname(TEXT)
            while(1):
                TEXT = voice_composer.voice_iden()
                if("停止" in TEXT):
                    voice_composer.playname("好的，请问您还需要其它服务吗")
                    TEXT = voice_composer.voice_iden()
                    if("不" in TEXT):
                        #ser.write(7)
                        return 0
                    else:
                        break
                else:
                    TEXT=voice_composer.get_answer(TEXT)
                    print(TEXT)
                    voice_composer.playname(TEXT)
        elif ("菜" in TEXT):
            TEXT = "请把你要识别的食物放在镜头前"
            print(TEXT)
            voice_composer.playname(TEXT)
            baiduface.photo()
            content=baiduface.dish_detect()
            if(str(content.get('result')[0].get('has_calorie'))=='False' or str(content.get('result')[0].get('calorie'))=='-1'):
                TEXT = "这东西不能吃"
                print(TEXT)
                voice_composer.playname(TEXT)
                return 0
            else:
                TEXT="此食物的名字是"+ content.get('result')[0].get('name')+",卡路里为"+ content.get('result')[0].get('calorie')
                print(TEXT)
                voice_composer.playname(TEXT)
                return 0
        elif("备忘录" in TEXT):
            TEXT = "请叙述您想记录的事情"
            print(TEXT)
            voice_composer.playname(TEXT)
            voice_composer.beiwanglu()
            return 0
        elif("提醒" in TEXT):
            voice_composer.play('beiwang.wav')
            return 0
        elif("救命" in TEXT):
            TEXT = "救援正在路上，请务必坚持住"
            print(TEXT)
            voice_composer.playname(TEXT)
            textmyself.textmyself("your parents are in danger, call the ambulance!!!!!!!!!")
            ser.write("i")
            return 0
        elif("文字" in TEXT):
            TEXT = "请把你要识别的文字放在镜头前"
            print(TEXT)
            voice_composer.playname(TEXT)
            baiduface.photo()
            content = baiduface.word_detect()
            TEXT = content
            print(TEXT)
            voice_composer.playname(TEXT)
            return 0
        elif("音乐" in TEXT):
            TEXT = "请说出你想播放的音乐"
            print(TEXT)
            voice_composer.playname(TEXT)
            while(1):
                TEXT = voice_composer.voice_iden()
                if(voice_composer.findmusic(TEXT)==1):
                    voice_composer.playmusic(TEXT)
                    return 0
                    break
                else:
                    continue
        else:
            TEXT="对不起，我没有听清你在说什么，请再说一次"
            print(TEXT)
            voice_composer.playname(TEXT)

