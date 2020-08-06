#!/usr/bin/env python3

from datetime import datetime
import paho.mqtt.client as mqtt
import json
import threading
import signal
import sys
import tkinter
from uuid import getnode as get_mac

from mqttClientParams import clientParams


class boardStats:
    timeout = 5
    timer = 0
    numBoards = 4
    numPub = [0 for i in range(numBoards)]
    lastPubTime = [datetime(1,1,1)  for i in range(numBoards)]
    ui = [{} for i in range(numBoards)]
        
    def checkTimeout(self):
        now = datetime.now()
        for i in range(self.numBoards):
            if ((now - bstat.lastPubTime[i]).seconds > self.timeout):
                self.ui[i]["status"].configure(text="Status: Offline", fg='red')
            else:
                self.ui[i]["status"].configure(text="Status: Online", fg='green')
        self.root.update()
        self.timer = threading.Timer(0.5, self.checkTimeout)
        self.timer.start()
    

    def cleanUpAndClose(self, sig=2, frame = 0):
        if self.timer:
            self.timer.cancel()
        self.root.destroy()
        sys.exit(0)
    
    def msgCallback(self, client, userdata, message):
        boardId = int(message.topic[8])
        if boardId not in range(self.numBoards):
            return
            
        if message.topic == "/team3/b"+str(boardId)+"/out":
            self.lastPubTime[boardId] = datetime.now()
            self.numPub[boardId] += 1
            self.ui[boardId]["successful"].configure(text="Successful Publishes: " + str(self.numPub[boardId]))
            self.ui[boardId]["time"].configure(text="Time of last publish: " + str(self.lastPubTime[boardId].strftime("%H:%M:%S")))


            
        elif message.topic == "/team3/b"+str(boardId)+"/stats":
            statsList = json.loads(message.payload)

            numRec = 0
            numMissed = 0
            for topic in statsList:
                if topic["topic"] == "/team3/b"+str(boardId)+"/out":
                    self.ui[boardId]["attempted"].configure(text="Attempted Publishes: " + str(topic["#pub"]))
                else:
                    numRec += topic["#rec"]
                    numMissed += topic["#drop"]
            self.ui[boardId]["received"].configure(text="Received Messages: " + str(numRec))
            self.ui[boardId]["missed"].configure(text="Missed Messages: " + str(numMissed))
            
        self.root.update()

    def ui_init(self):
        self.root = tkinter.Tk()
        signal.signal(signal.SIGINT, self.cleanUpAndClose)  # capture ctrl-c
        self.root.protocol("WM_DELETE_WINDOW", self.cleanUpAndClose)
        self.root.geometry("500x800")
        self.root.title("Team 3 Stats")
       
        for i in range(self.numBoards):
            self.ui[i]["status"]     = tkinter.Label(self.root, text = "Status: Offline")
            self.ui[i]["attempted"]  = tkinter.Label(self.root, text = "Attempted Publishes: 0")
            self.ui[i]["successful"] = tkinter.Label(self.root, text = "Successful Publishes: 0")
            self.ui[i]["missed"]     = tkinter.Label(self.root, text = "Missed Messages: 0")
            self.ui[i]["received"]   = tkinter.Label(self.root, text = "Received Messages: 0")
            self.ui[i]["time"]       = tkinter.Label(self.root, text = "Time of last publish: 0")
        
        for i, boardUi in enumerate(self.ui):
            tkinter.Label(self.root, text="\nb"+str(i), font=("Helvitica", 18)).pack()
            for name, element in boardUi.items():
                element.pack()

   
if __name__ == "__main__":
    client = mqtt.Client("stats_monitor_"+str(get_mac()))
    bstat = boardStats()
    bstat.ui_init()
    bstat.checkTimeout()
    client.on_message = bstat.msgCallback
    client.username_pw_set(clientParams["username"], clientParams["password"])
    client.connect(clientParams["host"], port=clientParams["port"])
    client.loop_start()
    client.subscribe("/team3/#")
    tkinter.mainloop()
    client.loop_stop()