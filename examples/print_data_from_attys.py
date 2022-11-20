#!/usr/bin/python3
import sys
sys.path.append('cpp')
import pyattyscomm
import numpy as np

class AttysCommCallback(pyattyscomm.SampleCallback):
    def hasSample(self,t,v):
        print(t,v)

cb = AttysCommCallback()

print("Searching for Attys")
s = pyattyscomm.AttysScan()
s.scan()
c = s.getAttysComm(0)
if (c == None):
    print("No Attys found")
    quit()
pyattyscomm.connectCallback(c,cb)
c.start()
input()
c.quit()
