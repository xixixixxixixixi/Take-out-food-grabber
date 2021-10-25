# coding=utf-8
import sys
import json
import wave
import pygame
import pyaudio
#from aip import AipSpeech
import requests
import time
import os
import base64
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

API_KEY = 'D0UkKHRU8oTGxmqm8AzFAZN3'
SECRET_KEY = 'IM6p0oKQf3igtsWQ7E7OcI15isj1blQF'

# 发音人选择, 0为普通女声，1为普通男生，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
PER = 2
# 语速，取值0-15，默认为5中语速
SPD = 5
# 音调，取值0-15，默认为5中语调
PIT = 5
# 音量，取值0-9，默认为5中音量
VOL = 8
# 下载的文件格式, 3：mp3(default) 4： pcm-16k 5： pcm-8k 6. wav
AUE = 6

FORMATS = {3: "mp3", 4: "pcm", 5: "pcm", 6: "wav"}
FORMAT = FORMATS[AUE]

CUID = "123456PYTHON"

TTS_URL = 'http://tsn.baidu.com/text2audio'


class DemoError(Exception):
    pass


"""  TOKEN start """

TOKEN_URL = 'http://openapi.baidu.com/oauth/2.0/token'
SCOPE = 'audio_tts_post'  # 有此scope表示有tts能力，没有请在网页里勾选


def fetch_token():#配合电子合成音使用
    #print("fetch token begin")
    params = {'grant_type': 'client_credentials',
              'client_id': API_KEY,
              'client_secret': SECRET_KEY}
    post_data = urlencode(params)
    if (IS_PY3):
        post_data = post_data.encode('utf-8')
    req = Request(TOKEN_URL, post_data)
    try:
        f = urlopen(req, timeout=5)
        result_str = f.read()
    except URLError as err:
        #print('token http response http code : ' + str(err.code))
        result_str = err.read()
    if (IS_PY3):
        result_str = result_str.decode()

    #print(result_str)
    result = json.loads(result_str)
    #print(result)
    if ('access_token' in result.keys() and 'scope' in result.keys()):
        if not SCOPE in result['scope'].split(' '):
            raise DemoError('scope is not correct')
        #print('SUCCESS WITH TOKEN: %s ; EXPIRES IN SECONDS: %s' % (result['access_token'], result['expires_in']))
        return result['access_token']
    else:
        raise DemoError('MAYBE API_KEY or SECRET_KEY not correct: access_token or scope not found in token response')


"""  TOKEN end """

def compose(TEXT):
    token = fetch_token()
    tex = quote_plus(str(TEXT))  # 此处TEXT需要两次urlencode
    #print(tex)
    params = {'tok': token, 'tex': tex, 'per': PER, 'spd': SPD, 'pit': PIT, 'vol': VOL, 'aue': AUE, 'cuid': CUID,
              'lan': 'zh', 'ctp': 1}  # lan ctp 固定参数

    data = urlencode(params)
    #print('test on Web Browser' + TTS_URL + '?' + data)

    req = Request(TTS_URL, data.encode('utf-8'))
    has_error = False
    try:
        f = urlopen(req)
        result_str = f.read()

        headers = dict((name.lower(), value) for name, value in f.headers.items())

        has_error = ('content-type' not in headers.keys() or headers['content-type'].find('audio/') < 0)

    except  URLError as err:
        #print('asr http response http code : ' + str(err.code))
        result_str = err.read()
        has_error = True

    save_file = "error.txt" if has_error else 'result.' + FORMAT
    with open(save_file, 'wb') as of:
        of.write(result_str)

    if has_error:
        if (IS_PY3):
            result_str = str(result_str, 'utf-8')

    print("result saved as :" + save_file)
pygame.mixer.init()
def play(path):#播放目标路径的音乐
    s=pygame.mixer.Sound(path)
    s.play()
    time.sleep(3)
    pygame.mixer.stop()
    """
    wave_obj = simpleaudio.WaveObject.from_wave_file(path)
    play_obj = wave_obj.play()
    play_obj.wait_done()
    if(pygame.mixer.get_busy()):
        time.sleep(3)
    """

def playname(TEXT):#给定字符串播放指定内容
    compose(TEXT)
    play('result.wav')

def get_file_content(filePath):#打开文件
    with open(filePath, 'rb') as fp:
        return fp.read()
def beiwanglu():
    input_filename = "beiwang.wav"  # 麦克风采集的语音输入
    input_filepath =os.path.dirname("voice_composer.py")
    #input_filepath = "D:\\voice\\"  # 输入文件的path
    in_path = input_filepath+ input_filename
    CHUNK = 8192
    FORMAT = pyaudio.paInt16
    CHANNELS = 1  # 声道数
    RATE = 16000  # 采样率
    RECORD_SECONDS = 3
    WAVE_OUTPUT_FILENAME = in_path
    p = pyaudio.PyAudio()

    stream = p.open(format=FORMAT,
                    channels=CHANNELS,
                    rate=RATE,
                    input=True,
                    frames_per_buffer=CHUNK)

    print("*" * 10)
    frames = []
    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        frames.append(data)
    print("*" * 10)
    stream.stop_stream()
    stream.close()
    p.terminate()
    wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    wf.setnchannels(CHANNELS)
    wf.setsampwidth(p.get_sample_size(FORMAT))
    wf.setframerate(RATE)
    wf.writeframes(b''.join(frames))
    wf.close()
    time.sleep(0.1)

def voice_iden():#录音并进行语音到字符串的转换
    while(1):
        input_filename = "voice_input.pcm"  # 麦克风采集的语音输入
        input_filepath =os.path.dirname("voice_composer.py")
        #input_filepath = "D:\\voice\\"  # 输入文件的path
        in_path = input_filepath+ input_filename
        CHUNK = 128
        FORMAT = pyaudio.paInt16
        CHANNELS = 1  # 声道数
        RATE = 16000  # 采样率
        RECORD_SECONDS = 3
        WAVE_OUTPUT_FILENAME = in_path
        p = pyaudio.PyAudio()

        stream = p.open(format=FORMAT,
                        channels=CHANNELS,
                        rate=RATE,
                        input=True,
                        frames_per_buffer=CHUNK)

        print("*" * 10)
        frames = []
        for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
            data = stream.read(CHUNK)
            frames.append(data)
        print("*" * 10)
        stream.stop_stream()
        stream.close()
        p.terminate()
        wf = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
        wf.setnchannels(CHANNELS)
        wf.setsampwidth(p.get_sample_size(FORMAT))
        wf.setframerate(RATE)
        wf.writeframes(b''.join(frames))
        wf.close()
        time.sleep(0.1)

        request_url = "https://vop.baidu.com/pro_api"
        with open("voice_input.pcm", "rb") as f:
            vd = base64.b64encode(f.read())  # 使用base64进行加密
        vd = bytes.decode(vd)
        size = os.path.getsize(r'voice_input.pcm')
        params ={"format":"pcm",
                 "rate":16000,"dev_pid":80001,"channel":1,
                 "token":"24.77b3ff735d74464e3760ea9c15165107.2592000.1564104753.282335-16604772",
                 "cuid":"wate_play",
                 "len":size,
                 "speech":vd,}
        header = {'Content-Tye': 'application/json'}
        request = requests.post(request_url,json.dumps(params),header)
        x = request.text
        result = json.loads(x)
        print(result)
        x=result.get('err_msg')
        if(x=="success."):
            TEXT = ''.join(result.get('result'))
            break
        else:
            TEXT = "对不起，我没有听清你在说什么，请再说一次"
            print(TEXT)
            playname(TEXT)
    return TEXT

def findmusic(TEXT):
    TEXT=TEXT.replace('。', '')
    TEXT=TEXT + '.mp3'
    print(TEXT)
    return os.path.exists(TEXT)

def playmusic(TEXT):
    TEXT=TEXT.replace('。', '')
    TEXT=TEXT + '.mp3'
    pygame.init()
    pygame.mixer.init()
    pygame.mixer.music.load(TEXT)
    pygame.mixer.music.play()
    while(1):
        TEXT=voice_iden()
        if(pygame.mixer.music.get_busy()!=1):
            break
        elif("暂停" in TEXT):
            pygame.mixer.music.pause()
        elif ("继续" in TEXT):
            pygame.mixer.music.unpause()
        elif ("停止" in TEXT):
            pygame.mixer.music.stop()
            break
        elif("退出" in TEXT):
            break
        else:
            continue

def getHtml(url):#配合图灵机器人请求HTML
    page=requests.get(url)
    return page.text

def get_answer(string):#图灵机器人交互
    key="aa0cca2d54684aa4a1f08d3feda7d4e4"
    api = "http://www.tuling123.com/openapi/api?key=" + key + "&info="
    request =api + string
    #print(request)
    response=getHtml(request)
    dic_json=json.loads(response)
    return dic_json['text']













