
// File: index.xml

// File: classAttysComm.xml


%feature("docstring") AttysComm "
`AttysComm(void *_btAddr=NULL, int _btAddrLen=0)`  

AttysComm contains all the neccessary comms to talk to the Attys on Linux,
Windows and Mac.   AttysComm class contains the device specific definitions and
implements the abstract classes of AttysCommBase. See AttysCommBase for the
definitions there. Instances of this class are automatically created by
AttysScan and the user can ignore definitions here. All relevant user functions
are in AttysCommBase. Use this class only if you have a fixed bluetooth address
(Linux/Win) or a fixed bluetooth device (Mac) and won't need to scan for a
bluetooth device.  

Constructors
------------
* `AttysComm(void *_btAddr=NULL, int _btAddrLen=0)`  
    
    Constructor: Win/Linux: takes the bluetooth device structure and its length
    as an argument. For Mac: just a pointer to the bluetooth device (needs
    typecast to *_btAddr) and provide no length.  

C++ includes: AttysComm.h
";

%feature("docstring") AttysComm::AttysComm "
`AttysComm(void *_btAddr=NULL, int _btAddrLen=0)`  

Constructor: Win/Linux: takes the bluetooth device structure and its length as
an argument. For Mac: just a pointer to the bluetooth device (needs typecast to
*_btAddr) and provide no length.  
";

%feature("docstring") AttysComm::connect "
`connect()`  

Connects to the Attys by opening the socket. Throws char* exception if it fails.  
";

%feature("docstring") AttysComm::closeSocket "
`closeSocket()`  

Closes the socket safely.  
";

%feature("docstring") AttysComm::run "
`run()`  

Thread which does the data acquisition. Do not call directly.  
";

%feature("docstring") AttysComm::quit "
`quit()`  

Call this from the main activity to shut down the connection.  
";

%feature("docstring") AttysComm::sendSyncCommand "
`sendSyncCommand(const char *message, int waitForOK)`  

Sends a command to the Attys. Do not use unless you know exactly what you are
doing.  
";

%feature("docstring") AttysComm::sendInit "
`sendInit()`  

Sends the init sequence to the Attys. Do not use unless you know exactly what
you are doing.  
";

%feature("docstring") AttysComm::start "
`start()`  

Starts the data acquisition by starting the main thread. and sending possibly
init commands.  
";

%feature("docstring") AttysComm::receptionTimeout "
`receptionTimeout()`  

Called from the watchdog after a timeout. Do not call this directly.  
";

%feature("docstring") AttysComm::getBluetoothBinaryAdress "
`getBluetoothBinaryAdress() -> unsigned char *`  

Returns an array of 14 bytes of the bluetooth address.  
";

%feature("docstring") AttysComm::getBluetoothAdressString "
`getBluetoothAdressString(char *s)`  

returns the MAC address as a string.  
";

// File: classAttysCommBase.xml


%feature("docstring") AttysCommBase "
`AttysCommBase()`  

Platform independent definitions for the Attys  

Constructors
------------
* `AttysCommBase()`  
    
    Constructor which is overloaded by AttysComm.  

Attributes
----------
* `NCHANNELS` : `const int`  
    Total number of channels per samples.  

* `nMem` : `const int`  
    Number of entries in the ringbuffer. Buffer for 10secs at 1kHz.  

* `INDEX_Acceleration_X` : `const int`  
    Channel index for X Acceleration.  

* `INDEX_Acceleration_Y` : `const int`  
    Channel index for Y Acceleration.  

* `INDEX_Acceleration_Z` : `const int`  
    Channel index for Z Acceleration.  

* `INDEX_Magnetic_field_X` : `const int`  
    Magnetic field in X direction.  

* `INDEX_Magnetic_field_Y` : `const int`  
    Magnetic field in Y direction.  

* `INDEX_Magnetic_field_Z` : `const int`  
    Magnetic field in Z direction.  

* `INDEX_Analogue_channel_1` : `const int`  
    Index of analogue channel 1.  

* `INDEX_Analogue_channel_2` : `const int`  
    Index of analogue channel 2.  

* `INDEX_GPIO0` : `const int`  
    Index of the internal GPIO pin 1.  

* `INDEX_GPIO1` : `const int`  
    Index of the internal GPIO pin 2.  

* `ADC_RATE_125HZ` : `const int`  
    Constant defining sampling rate of 125Hz.  

* `ADC_RATE_250HZ` : `const int`  
    Constant defining sampling rate of 250Hz.  

* `ADC_RATE_500HZ` : `const int`  
    Constant defining sampling rate of 500Hz (Attys2 only).  

* `ADC_DEFAULT_RATE` : `const int`  
    Constant defining the default sampling rate (250Hz).  

* `ADC_SAMPLINGRATE` : `const int`  
    Array of the sampling rates mapping the index to the actual sampling rate.  

* `ADC_GAIN_6` : `const int`  
    Gain index setting it to gain 6.  

* `ADC_GAIN_1` : `const int`  
    Gain index setting it to gain 6.  

* `ADC_GAIN_2` : `const int`  
    Gain index setting it to gain 2.  

* `ADC_GAIN_3` : `const int`  
    Gain index setting it to gain 3.  

* `ADC_GAIN_4` : `const int`  
    Gain index setting it to gain 4.  

* `ADC_GAIN_8` : `const int`  
    Gain index setting it to gain 5.  

* `ADC_GAIN_12` : `const int`  
    Gain index setting it to gain 6.  

* `ADC_CURRENT_6NA` : `const int`  
    Bias current of 6nA.  

* `ADC_CURRENT_22NA` : `const int`  
    Bias current of 22nA.  

* `ADC_CURRENT_6UA` : `const int`  
    Bias current of 6uA.  

* `ADC_CURRENT_22UA` : `const int`  
    Bias current of 22uA.  

* `ADC_MUX_NORMAL` : `const int`  
    Muliplexer routing is normal: ADC1 and ADC2 are connected to the
    sigma/delta.  

* `ADC_MUX_SHORT` : `const int`  
    Muliplexer routing: inputs are short circuited.  

* `ADC_MUX_SUPPLY` : `const int`  
    Muliplexer routing: inputs are connected to power supply.  

* `ADC_MUX_TEMPERATURE` : `const int`  
    Muliplexer routing: ADC measures internal temperature.  

* `ADC_MUX_TEST_SIGNAL` : `const int`  
    Muliplexer routing: ADC measures test signal.  

* `ADC_MUX_ECG_EINTHOVEN` : `const int`  
    Muliplexer routing: both positive ADC inputs are connected together.  

* `ACCEL_2G` : `const int`  
    Setting full scale range of the accelerometer to 2G.  

* `ACCEL_4G` : `const int`  
    Setting full scale range of the accelerometer to 4G.  

* `ACCEL_8G` : `const int`  
    Setting full scale range of the accelerometer to 8G.  

* `ACCEL_16G` : `const int`  
    Setting full scale range of the accelerometer to 16G.  

* `MESSAGE_CONNECTED` : `const int`  
    Message callback: Connected.  

* `MESSAGE_ERROR` : `const int`  
    Message callback: Generic error.  

* `MESSAGE_TIMEOUT` : `const int`  
    Message callback: Reception timeout detected by the watchdog.  

* `MESSAGE_RECONNECTED` : `const int`  
    Message callback: Managed to reconnect.  

* `MESSAGE_RECEIVING_DATA` : `const int`  
    Message callback: Receiving data. One off once the connection has been set
    up.  

* `CHANNEL_DESCRIPTION` : `const std::string`  
    Long descriptions of the channels.  

* `CHANNEL_SHORT_DESCRIPTION` : `const std::string`  
    Short descriptions of the channels.  

* `CHANNEL_UNITS` : `const std::string`  
    Units of the channels.  

* `ADC_GAIN_FACTOR` : `const int`  
    Mmapping between index and actual gain.  

* `ADC_REF` : `const float`  
    The voltage reference of the ADC in volts.  

* `oneG` : `const float`  
    One g in m/s^2.  

* `ACCEL_FULL_SCALE` : `const float`  
    Mapping of the index to the full scale accelerations.  

* `MAG_FULL_SCALE` : `const float`  
    Full scale range of the magnetometer in Tesla.  

* `attysCommMessage` : `AttysCommMessage *`  

C++ includes: AttysCommBase.h
";

%feature("docstring") AttysCommBase::AttysCommBase "
`AttysCommBase()`  

Constructor which is overloaded by AttysComm.  
";

%feature("docstring") AttysCommBase::~AttysCommBase "
`~AttysCommBase()`  

Destructor which releases memory and closes any open connection.  
";

%feature("docstring") AttysCommBase::setAdc_samplingrate_index "
`setAdc_samplingrate_index(int idx)`  

Sets the sampling rate using the sampling rate index numbers.  
";

%feature("docstring") AttysCommBase::getSamplingRateInHz "
`getSamplingRateInHz() -> int`  

Gets the sampling rate in Hz (not index number).  
";

%feature("docstring") AttysCommBase::getAdc_samplingrate_index "
`getAdc_samplingrate_index() -> int`  

Gets the sampling rate in form for the index.  
";

%feature("docstring") AttysCommBase::getADCFullScaleRange "
`getADCFullScaleRange(int channel) -> float`  

Gets the ADC full range. This depends on the gain setting of the ADC.  
";

%feature("docstring") AttysCommBase::setAdc0_gain_index "
`setAdc0_gain_index(int idx)`  

Gets the gain index for ADC1.  
";

%feature("docstring") AttysCommBase::setAdc1_gain_index "
`setAdc1_gain_index(int idx)`  

Gets the gain index for ADC2.  
";

%feature("docstring") AttysCommBase::setBiasCurrent "
`setBiasCurrent(int currIndex)`  

Sets the bias current which can be switched on.  
";

%feature("docstring") AttysCommBase::getBiasCurrent "
`getBiasCurrent() -> int`  

Gets the bias current as in index.  
";

%feature("docstring") AttysCommBase::enableCurrents "
`enableCurrents(int pos_ch1, int neg_ch1, int pos_ch2)`  

Switches bias currents on  
";

%feature("docstring") AttysCommBase::setAdc0_mux_index "
`setAdc0_mux_index(int idx)`  
";

%feature("docstring") AttysCommBase::setAdc1_mux_index "
`setAdc1_mux_index(int idx)`  
";

%feature("docstring") AttysCommBase::getAccelFullScaleRange "
`getAccelFullScaleRange() -> float`  

Returns the accelerometer current full scale reading in m/s^2.  
";

%feature("docstring") AttysCommBase::setAccel_full_scale_index "
`setAccel_full_scale_index(int idx)`  

Sets the accelerometer full scale range using the index.  
";

%feature("docstring") AttysCommBase::getMagFullScaleRange "
`getMagFullScaleRange() -> float`  

Returns the full scale magnetometer in Tesla.  
";

%feature("docstring") AttysCommBase::getIsCharging "
`getIsCharging() -> int`  

Charging indicator. Returns one if charging.  
";

%feature("docstring") AttysCommBase::connect "
`connect()=0`  

Connects to the Attys by opening the socket. Throws char* exception if it fails.  
";

%feature("docstring") AttysCommBase::start "
`start()`  

Starts the data acquisition by starting the main thread. and sending possibly
init commands.  
";

%feature("docstring") AttysCommBase::closeSocket "
`closeSocket()=0`  

Closes the socket safely.  
";

%feature("docstring") AttysCommBase::hasActiveConnection "
`hasActiveConnection() -> int`  

Returns one if the connection is active.  
";

%feature("docstring") AttysCommBase::getSampleFromBuffer "
`getSampleFromBuffer() -> sample_p`  

Gets a sample from the ringbuffer. This is a float* array of all channels.  
";

%feature("docstring") AttysCommBase::hasSampleAvailable "
`hasSampleAvailable() -> int`  

Is set to one if samples are available in the ringbuffer.  
";

%feature("docstring") AttysCommBase::resetRingbuffer "
`resetRingbuffer()`  

Resets the ringbuffer to zero content.  
";

%feature("docstring") AttysCommBase::registerCallback "
`registerCallback(AttysCommListener *f)`  

Register a realtime callback function which is called whenever a sample has
arrived. AttysCommListener is an abstract class which needs to implement
hasSample().  
";

%feature("docstring") AttysCommBase::unregisterCallback "
`unregisterCallback()`  

Unregister the realtime sample callback.  
";

%feature("docstring") AttysCommBase::registerMessageCallback "
`registerMessageCallback(AttysCommMessage *f)`  

Callback which is called whenever a special error/event has occurred.  
";

%feature("docstring") AttysCommBase::unregisterMessageCallback "
`unregisterMessageCallback()`  

Unregister the error/event callback.  
";

%feature("docstring") AttysCommBase::quit "
`quit()`  

Call this from the main activity to shut down the connection.  
";

%feature("docstring") AttysCommBase::getBluetoothBinaryAdress "
`getBluetoothBinaryAdress()=0 -> unsigned char *`  

Returns an array of 14 bytes of the bluetooth address.  
";

%feature("docstring") AttysCommBase::getBluetoothAdressString "
`getBluetoothAdressString(char *s)=0`  

returns the MAC address as a string.  
";

%feature("docstring") AttysCommBase::getAttysName "
`getAttysName() -> char *`  

Returns the name of the Attys  
";

%feature("docstring") AttysCommBase::setAttysName "
`setAttysName(char *s)`  

Sets the name of the Attys  
";

%feature("docstring") AttysCommBase::processRawAttysData "
`processRawAttysData(const char *data)`  
";

%feature("docstring") AttysCommBase::isInitialising "
`isInitialising() -> int`  
";

%feature("docstring") AttysCommBase::setConnected "
`setConnected(int c)`  
";

%feature("docstring") AttysCommBase::phys2temperature "
`phys2temperature(float volt) -> float`  

Temperature  
";

// File: structAttysCommListener.xml


%feature("docstring") AttysCommListener "

Callback after a sample has arrived. The main class can for example inherit
class and implement hasSample.  

C++ includes: AttysCommBase.h
";

%feature("docstring") AttysCommListener::hasSample "
`hasSample(double, sample_p)=0`  

Provides the timestamp and an array of all channels. This is an abstract method
and needs to be overloaded with a real method doing the work.  
";

%feature("docstring") AttysCommListener::~AttysCommListener "
`~AttysCommListener()`  
";

// File: structAttysCommMessage.xml


%feature("docstring") AttysCommMessage "

Callback after an error has occurred. This callback is in particular useful
after a broken connection has been re-established.  

C++ includes: AttysCommBase.h
";

%feature("docstring") AttysCommMessage::hasMessage "
`hasMessage(int, const char *)=0`  

Provides the error number and a text message about the error.  
";

%feature("docstring") AttysCommMessage::~AttysCommMessage "
`~AttysCommMessage()`  
";

// File: classAttysScan.xml


%feature("docstring") AttysScan "

Scans for Attys and creates instances of AttysComm for every detected/paired
Attys. There is no need to create instances of AttysComm yourself. This is done
by this class automatically.  

Attributes
----------
* `SCAN_CONNECTED` : `const int`  
    Message index that the connection to an attys has been successful.  

* `SCAN_SEARCHING` : `const int`  
    Message index that AttysScan is actively scanning  

* `SCAN_NODEV` : `const int`  
    Message index that no Attys has been detected  

* `SCAN_SOCKETERR` : `const int`  
    Message that the socket could not be opened  

* `SCAN_CONNECTING` : `const int`  
    In the process of connecting  

* `SCAN_CONNECTERR` : `const int`  
    Connection error during scanning  

* `MAX_ATTYS_DEVS` : `const int`  
    Max number of Attys Devices  

C++ includes: AttysScan.h
";

%feature("docstring") AttysScan::~AttysScan "
`~AttysScan()`  
";

%feature("docstring") AttysScan::scan "
`scan(int maxAttys=1) -> int`  

Scans for the specified number of devices and connects to them. By default only
for one Attys. returns 0 on success  
";

%feature("docstring") AttysScan::registerCallback "
`registerCallback(AttysScanListener *f)`  

Register callback which reports the scanning progress for example for a splash
screen.  
";

%feature("docstring") AttysScan::unregisterCallback "
`unregisterCallback()`  

Unregisters the callback  
";

%feature("docstring") AttysScan::getAttysComm "
`getAttysComm(int i) -> AttysComm *`  

Obtains the pointer to a valid AttysComm class which has been successfully
detected while scanning.  
";

%feature("docstring") AttysScan::getAttysName "
`getAttysName(int i) -> char *`  

Gets the Attys name as reported by the bluetooth manager  
";

%feature("docstring") AttysScan::getNAttysDevices "
`getNAttysDevices() -> int`  

Returns the number of Attys devices  
";

// File: structAttysScanListener.xml


%feature("docstring") AttysScanListener "

Callback which reports the status of the scanner  

C++ includes: AttysScan.h
";

%feature("docstring") AttysScanListener::~AttysScanListener "
`~AttysScanListener()`  
";

%feature("docstring") AttysScanListener::message "
`message(const int, const char *)=0`  
";

// File: AttysComm_8h.xml

// File: AttysCommBase_8h.xml

// File: AttysScan_8h.xml

// File: LICENSE.xml

