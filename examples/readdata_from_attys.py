#!/usr/bin/python3
import sys
sys.path.append('cpp')
import pyattyscomm

print("Searching for Attys")
s = pyattyscomm.AttysScan()
s.scan()
c = s.getAttysComm(0)
if (c == None):
    print("No Attys found")
    quit()
c.start()
while True:
    while (not c.hasSampleAvilabale()):
        pass
    sample = c.getSampleFromBuffer()
    print(sample)
