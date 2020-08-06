# -*- coding: utf-8 -*-
"""
Created on Sun Jul 12 21:37:12 2020

@author: Justin Desai
"""

import paho.mqtt.client as mqtt #import the client1
from mqttClientParams import clientParams
import time, json

statsDict = {}
def on_message(client, userdata, message):
    if "stat" in message.topic:
        statsDict[message.topic] = json.loads(str(message.payload.decode("utf-8")))
        #print(statsDict)

broker_address=clientParams["host"]
print("creating new instance")

client = mqtt.Client("P1") #create new instance

client.on_message=on_message #attach function to callback

print("connecting to broker")
client.username_pw_set(clientParams["username"], clientParams["password"])
client.connect(broker_address, port=clientParams["port"]) #connect to broker


client.loop_start() #start the loop

print("Subscribing to topic","/team3/cc3200/ToggleLEDCmdL1")
client.subscribe("/team3/#")

# Test 1: board receives all messages
print("Running Test 1.", end='')
                 
i = 0
while i < 10:
    client.publish("/team3/test/test1","msg" + str(i))
    time.sleep(1) # wait
    i += 1
    print('.', end='')

recMsgs = statsDict['/team3/b0/stat']
recMsgs = recMsgs[len(recMsgs) - 1]['#rec']
                  
if recMsgs != 10:
    print("Test 1 Fail!")
else:
    print("Test 1 Pass!")
    
# Test 2: board receives messages skipping sequence numbers
print("Running Test 2.", end='')    

client.publish("/team3/test/test2","msg1") #TODO: properly send messages in correct format
print('.', end='')

time.sleep(5)

client.publish("/team3/test/test2","msg2")
print('.', end='')

dropMsgs = statsDict['/team3/b0/stat']
dropMsgs = dropMsgs[len(dropMsgs) - 1]['#drop']

if recMsgs != 1:
    print("Test 2 Fail!")
else:
    print("Test 2 Pass!")

client.loop_stop() #stop the loop