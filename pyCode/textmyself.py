# -*- coding:utf-8 -*-
from twilio.rest import Client
account = 'ACf2a471e378c4b7d0c6d4b4eeb5b08dc1'
token = '85ed78ae3d48a2aee2100c28000fdde1'
myNumber='+8618742526500'
twilioNumber='+12029309747'
 
 
def textmyself(message):
    client = Client(account, token)
    message = client.messages.create(to=myNumber, from_=twilioNumber,
                                 body=message)
                                 
#textmyself("123")
