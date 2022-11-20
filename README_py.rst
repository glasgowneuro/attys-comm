========================
Python API for the Attys
========================

Attys is a lightweight wearable bluetooth data acquisition box which can measure anything from temperature to ECG.

It's cross platform and runs under Linux and Windows

It has two 24 bit analogue channels, an accelerometer and a magnetometer.

http://www.attys.tech


Required packages under Linux
=============================

For Ubuntu Linux install libbluetooth-dev and swig::

      sudo apt-get install libbluetooth-dev swig



Quick start guide
=================

Here are the basic steps how to use the API::

      # load the module
      import pyattyscomm

      # Gets the AttysScan class which scans for Attys via bluetooth
      s = pyattyscomm.AttysScan()

      # Scan for Attys
      s.scan()

      # get the 1st Attys
      c = s.getAttysComm(0)

      # create a datacallback
      class AttysCommCallback(pyattyscomm.SampleCallback):
        def hasSample(self,t,v):
          print(t,v) # so more than just printing!

      cb = AttysCommCallback()

      pyattyscomm.connectCallback(c,cb)

      # Start data acquisition in the background
      c.start()

      # sleep here or start a GUI

      # end the acqu
      c.quit()




API documentation
==================

Query it via the python help system::

    import pyattyscomm
    help(pyattyscomm)


The Python classes have been generated with SWIG. The header files AttysComm.h, AttysCommBase,h and AttysScan.h provide detailed info about the classes:

https://github.com/glasgowneuro/attys-comm








Demos
=====

On github are python examples which show you how
to plot realtime data from the Attys: https://github.com/glasgowneuro/attys-comm/tree/master/examples
