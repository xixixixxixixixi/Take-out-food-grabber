import requests
import json

def getHtml(url):
    page=requests.get(url)
    return page.text

def get_answer(string):
    key="aa0cca2d54684aa4a1f08d3feda7d4e4"
    api = "http://www.tuling123.com/openapi/api?key=" + key + "&info="
    request =api + string
    #print(request)
    response=getHtml(request)
    dic_json=json.loads(response)
    return dic_json['text']