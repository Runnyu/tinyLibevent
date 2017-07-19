#!/usr/bin/python
# -*- coding:utf-8 -*-
import time, socket, sys

def testConnect():
    while True:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 8888))
        s.close()
        time.sleep(3)

def testSend():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 8888))
    while True:
        msg = raw_input(">>> ")
        s.send(msg)
        data = s.recv(1024)
        print data
    s.close()
    

if __name__ == "__main__":
    #testSend()
    #testConnect()
