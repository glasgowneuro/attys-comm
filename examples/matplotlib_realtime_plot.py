#!/usr/bin/python3
"""
Plots one channel of the Attys in two different windows. Requires pyqtgraph.

"""

import sys
sys.path.append('cpp')
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pyattyscomm

# read from 2nd Analogue channel
channel = pyattyscomm.AttysComm.INDEX_Analogue_channel_1


# now let's plot the data
fig, ax = plt.subplots()
# that's our plotbuffer
plotbuffer = np.zeros(500)
# plots an empty line
line, = ax.plot(plotbuffer)
# axis
ax.set_ylim(-2, 2)


# receives the data from the generator below
def update(data):
    global plotbuffer
    global ax
    plotbuffer=np.append(plotbuffer,data)
    # only keep the 500 newest ones and discard the old ones
    plotbuffer=plotbuffer[-500:]
    # set the new 500 points of channel 9
    line.set_ydata(plotbuffer)
    return line,


class AttysCommCallback(pyattyscomm.SampleCallback):
    def hasSample(self,t,v):
        global plotbuffer
        plotbuffer.append(v[channel])

        
s = pyattyscomm.AttysScan()
s.scan()
c = s.getAttysComm(0)
if not c:
    print("No Attys connected and/or paired")
    sys.exit()

    
cb = AttysCommCallback()
pyattyscomm.connectCallback(c,cb)

c.start()

# start the animation
ani = animation.FuncAnimation(fig, update, interval=100)

# show it
plt.show()

c.quit()

print("finished")
