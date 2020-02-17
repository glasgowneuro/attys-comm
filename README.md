# AttysCOMM

The C++ & Python API for the Attys: http://www.attys.tech

The library is cross platform: It's for Linux, Windows and Mac.

![alt tag](ecu_attys_daq_board.png)

## Installation instructions

### Linux and MacOS

```
cmake .
make
sudo make install
```

This will generate: a dynamic library libattyscomm and a static
one called libattyscomm_static.a.


### Windows:
Under windows only the static library is generated which
should be used for your code development.
```
cmake -G "Visual Studio 15 2017 Win64" .
```
and then start Visual C++ and compile it.


## Usage

A small test program is in the `test` directory which scans
for an Attys and then prints the incoming data to stdout.
Type `cmake .`, `make` and then `./attystest` to run it.

Here is a step by guide how to code it:

1. scan for Attys
```
int ret = attysScan.scan();
```

2. Check the number of Attys detected
```
attysScan.nAttysDevices
```

3. If devices have been detected they show up as an array.
`attysScan.attysComm[0,1,2,etc]` points to the AttysComm instances.

4. Set the parameters, for example:
```
attysScan.attysComm[0]->setAdc_samplingrate_index(AttysComm::ADC_RATE_250HZ);
```

5. Register a callback (optional)
```
attysCallback = new AttysCallback(this);
attysScan.attysComm[0]->registerCallback(attysCallback);
```
You need to overload the virtual function of the callback in your program.

6. Start data acquisition
```
attysScan.attysComm[0]->start();
```

7. Check if ringbuffer contains data and wait till true
```
attysScan.attysComm[n]->hasSampleAvilabale();
```

8. Get samples from buffer
```
float* values = attysScan.attysComm[n]->getSampleFromBuffer();
```

9. go back to 7)

10. Ending the program:
```
attysScan.attysComm[n]->quit();
```


## Python (SWIG)

This libary is fast and multi threaded. It performs
the data acquisition in the background while python can then
do the postprocessing.

Pre-compiled packages for both Linux and Windows are available.

### Linux

#### Python package (pip):

Make sure you have the bluetooth development libraries:
```
sudo apt-get install libbluetooth-dev
```
and then install with:

```
pip3 install pyattyscomm
```

#### From source

You need to have swig-3.x installed. Then run in the `cpp` directory:

```
cmake .
make
make install
./setup.py install
```

and then you can load the module `pyattyscomm` system-wide!


### Windows

#### Python package (pip):

In the python console type:

```
pip install pyattyscomm
```

### From source

Install `swig` and re-run the C++ installation.
Make sure to select "Release" in Visual Studio as python
is usually not installed with its debug libraries.
After compilation you get:

- `Release\_pyattyscomm.exp`
- `Release\_pyattyscomm.pyd`
- `pyattyscomm.py`

Install them with:
```
python setup.py install
```

### How to use

The python API is identical to the C++ one.
All the definitions are in AttysComm.h and AttysScan.h.

Here is an example:

```
# load the module
import pyattyscomm

# Gets the AttysScan class which scans for Attys via bluetooth
s = pyattyscomm.AttysScan()

# Scan for Attys
s.scan()

# get the 1st Attys
c = s.getAttysComm(0)

# if an attys has been found c points to it. Otherwise it's None.

# Start data acquisition in the background
c.start()

# Now we just read data at our convenience in a loop or timer or thread

while (not c.hasSampleAvilabale()):
        # do something else or nothing
	a = a + 1
    # getting a sample
    sample = c.getSampleFromBuffer()

    # do something with the sample
    print(sample)
```

### Demo programs

There are demo programs which show you how to read/plot data with pyattyscomm:

```
readdata_demo.py
realtime_plot_demo.py
```


![alt tag](pyattyscomm.png)


Enjoy!

http://www.attys.tech
