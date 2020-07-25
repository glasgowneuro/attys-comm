#ifndef AttysCommBase_H
#define AttysCommBase_H

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <thread>
#include <math.h>
#include <mutex>

#include "attyscomm/base64.h"

/**
 * AttysCommBase contains all definitions which are relevant to the user.
 * Class instances are usually automatically created by AttysScan when
 * it's searching for Attys boxes. AttysScan the provides pointers
 * to the AttysComm instances.
 **/


/**
 * Timeout for the Attys
 **/
#define TIMEOUT_IN_SECS 2

#if defined(__linux__) || defined(__APPLE__)
#if (defined(NDEBUG) || defined(QT_NO_DEBUG))
#define _RPT0(u,v)
#define _RPT1(u,v,w)
#define _RPT2(u,v,w,x)
#else
#define _RPT0(u,v) fprintf(stderr,v)
#define _RPT1(u,v,w) fprintf(stderr,v,w)
#define _RPT2(u,v,w,x) fprintf(stderr,v,w,x)
#endif
#endif


/**
 * type which represents the samples
 **/
typedef float* sample_p;


/**
 * Callback after a sample has arrived.
 * The main class can for example inherit class
 * and implement hasSample.
 **/
struct AttysCommListener {
	/**
	 * Provides the timestamp and an array of all channels.
	 * This is an abstract method and needs to be
	 * overloaded with a real method doing the work.
	 **/
	virtual void hasSample(double, sample_p) = 0;
	// empty destructor in case of a delete
	virtual ~AttysCommListener() {};
};


/**
 * Callback after an error has occurred.
 * This callback is in particular useful after a broken
 * connection has been re-established.
 **/
struct AttysCommMessage {
	/**
	 * Provides the error number and a text message about the error.
	 **/
	virtual void hasMessage(int, const char*) = 0;
	// empty destructor in case of a delete
	virtual ~AttysCommMessage() {};
};


/**
* Platform independent definitions for the Attys
**/
class AttysCommBase
{
public:
	/**
	 * Constructor which is overloaded by AttysComm.
	 **/
	AttysCommBase();

	/**
	 * Destructor which releases memory and closes
	 * any open connection.
	 **/
	virtual ~AttysCommBase();

	/**
	 * Total number of channels per samples.
	 **/
	static const int NCHANNELS = 10;

	/**
	 * Number of entries in the ringbuffer.
	 * Buffer for 10secs at 1kHz.
	 **/
	static const int nMem = 1000 * 10;

	/**
	 * Channel index for X Acceleration.
	 **/
	static const int INDEX_Acceleration_X = 0;

	/**
	 * Channel index for Y Acceleration.
	 **/
	static const int INDEX_Acceleration_Y = 1;

	/**
	 * Channel index for Z Acceleration.
	 **/
	static const int INDEX_Acceleration_Z = 2;

	/**
	 * Magnetic field in X direction.
	 **/
	static const int INDEX_Magnetic_field_X = 3;

	/**
	 * Magnetic field in Y direction.
	 **/
	static const int INDEX_Magnetic_field_Y = 4;

	/**
	 * Magnetic field in Z direction.
	 **/
	static const int INDEX_Magnetic_field_Z = 5;

	/**
	 * Index of analogue channel 1.
	 **/
	static const int INDEX_Analogue_channel_1 = 6;

	/**
	 * Index of analogue channel 2.
	 **/
	static const int INDEX_Analogue_channel_2 = 7;

	/**
	 * Index of the internal GPIO pin 1.
	 **/
	static const int INDEX_GPIO0 = 8;

	/**
	 * Index of the internal GPIO pin 2.
	 **/
	static const int INDEX_GPIO1 = 9;


	/**
	 * Long descriptions of the channels.
	 **/
	const std::string CHANNEL_DESCRIPTION[NCHANNELS] = {
		"Acceleration X",
		"Acceleration Y",
		"Acceleration Z",
		"Magnetic field X",
		"Magnetic field Y",
		"Magnetic field Z",
		"Analogue channel 1",
		"Analogue channel 2"
		"DIN channel 0",
		"DIN channel 1",
		"Charging status"
	};

	/**
	 * Short descriptions of the channels.
	 **/
	const std::string CHANNEL_SHORT_DESCRIPTION[NCHANNELS] = {
		"Acc X",
		"Acc Y",
		"Acc Z",
		"Mag X",
		"Mag Y",
		"Mag Z",
		"ADC 1",
		"ADC 2",
		"DIN 0",
		"DIN 1",
	};

	/**
	 * Units of the channels.
	 **/
	std::string const CHANNEL_UNITS[NCHANNELS] = {
		"m/s^2",
		"m/s^2",
		"m/s^2",
		"T",
		"T",
		"T",
		"V",
		"V",
		" ",
		" ",
	};

	///////////////////////////////////////////////////////////////////
	// ADC sampling rate and for the whole system

	/**
	 * Constant defining sampling rate of 125Hz.
	 **/
       	static const int ADC_RATE_125HZ = 0;

	/**
	 * Constant defining sampling rate of 250Hz.
	 **/
	static const int ADC_RATE_250HZ = 1;

	/**
	 * Constant defining sampling rate of 500Hz (Attys2 only).
	 **/
	static const int ADC_RATE_500HZ = 2;

	/**
	 * Constant defining the default sampling rate (250Hz).
	 **/
	static const int ADC_DEFAULT_RATE = ADC_RATE_250HZ;

	/**
	 * Array of the sampling rates mapping the index
	 * to the actual sampling rate.
	 **/
	static const int ADC_SAMPLINGRATE[4];

private:
	// the actual sampling rate in terms of the sampling rate index
	int adc_rate_index;

public:
	/**
	 * Sets the sampling rate using the sampling rate
	 * index numbers.
	 **/
	void setAdc_samplingrate_index(int idx) {
		adc_rate_index = idx;
	}

	/**
	 * Gets the sampling rate in Hz (not index number).
	 **/
	int getSamplingRateInHz() {
		return ADC_SAMPLINGRATE[adc_rate_index];
	}

	/**
	 * Gets the sampling rate in form for the index.
	 **/
	int getAdc_samplingrate_index() {
		return adc_rate_index;
	}


	////////////////////////////////////////////////////////////////////////////
	// ADC gain
	// the strange numbering scheme comes from the ADC's numbering
	// scheme. Index=0 is really a gain factor of 6
	// On the ATttys we refer to channel 1 and 2 which are 0 and 1 here for
	// indexing.

	/**
	 * Gain index setting it to gain 6.
	 **/
	static const int ADC_GAIN_6 = 0;

	/**
	 * Gain index setting it to gain 6.
	 **/
	static const int ADC_GAIN_1 = 1;

	/**
	 * Gain index setting it to gain 2.
	 **/
	static const int ADC_GAIN_2 = 2;

	/**
	 * Gain index setting it to gain 3.
	 **/
	static const int ADC_GAIN_3 = 3;

	/**
	 * Gain index setting it to gain 4.
	 **/
	static const int ADC_GAIN_4 = 4;

	/**
	 * Gain index setting it to gain 5.
	 **/
	static const int ADC_GAIN_8 = 5;

	/**
	 * Gain index setting it to gain 6.
	 **/
	static const int ADC_GAIN_12 = 6;
	
	/**
	 * Mmapping between index and actual gain.
	 **/
	const int ADC_GAIN_FACTOR[7] = { 6, 1, 2, 3, 4, 8, 12 };

	/**
	 * The voltage reference of the ADC in volts.
	 **/
	const float ADC_REF = 2.42F;

	/**
	 * Gets the ADC full range. This depends on the
	 * gain setting of the ADC.
	 **/
	float getADCFullScaleRange(int channel) {
		switch (channel) {
		case 0:
			return ADC_REF / ADC_GAIN_FACTOR[adc0_gain_index];
		case 1:
			return ADC_REF / ADC_GAIN_FACTOR[adc1_gain_index];
		default:
			fprintf(stderr, "getADCFullScaleRange wrong index\n");
			exit(1);
		}
		return 0;
	}

	/**
	 * Gets the gain index for ADC1.
	 **/
	void setAdc0_gain_index(int idx) {
		adc0_gain_index = idx;
	}

	/**
	 * Gets the gain index for ADC2.
	 **/
	void setAdc1_gain_index(int idx) {
		adc1_gain_index = idx;
	}

private:
	// initial gain factor is 1 for both channels
	int adc0_gain_index = ADC_GAIN_1;
	int adc1_gain_index = ADC_GAIN_1;

public:
	/////////////////////////////////////////////////////////////////////
	// Bias currents for resistance measurement
	// selectable bias current index numbers for the ADC inputs
	// used to measure resistance

	/**
	 * Bias current of 6nA.
	 **/
	static const int ADC_CURRENT_6NA = 0;

	/**
	 * Bias current of 22nA.
	 **/
	static const int ADC_CURRENT_22NA = 1;

	/**
	 * Bias current of 6uA.
	 **/
	static const int ADC_CURRENT_6UA = 2;

	/**
	 * Bias current of 22uA.
	 **/
	static const int ADC_CURRENT_22UA = 3;

private:
	int current_index = 0;
	int current_mask = 0;

public:
	/**
	 * Sets the bias current which can be switched on.
	 **/
	void setBiasCurrent(int currIndex) {
		current_index = currIndex;
	}

	/**
	 * Gets the bias current as in index.
	 **/
	int getBiasCurrent() {
		return current_index;
	}

	/**
	 * Switches bias currents on
	 **/
	void enableCurrents(int pos_ch1, int neg_ch1, int pos_ch2) {
		current_mask = 0;
		if (pos_ch1) {
			current_mask = (int)(current_mask | (int)0b00000001);
		}
		if (neg_ch1) {
			current_mask = (int)(current_mask | (int)0b00000010);
		}
		if (pos_ch2) {
			current_mask = (int)(current_mask | (int)0b00000100);
		}
	}


	//////////////////////////////////////////////////////////////////////////////
	// selectable different input mux settings
	// for the ADC channels

	/**
	 * Muliplexer routing is normal: ADC1 and ADC2 are connected to the sigma/delta.
	 **/
	static const int ADC_MUX_NORMAL = 0;
	
	/**
	 * Muliplexer routing: inputs are short circuited.
	 **/
	static const int ADC_MUX_SHORT = 1;
	
	/**
	 * Muliplexer routing: inputs are connected to power supply.
	 **/
	static const int ADC_MUX_SUPPLY = 3;
	
	/**
	 * Muliplexer routing: ADC measures internal temperature.
	 **/
	static const int ADC_MUX_TEMPERATURE = 4;

	/**
	 * Muliplexer routing: ADC measures test signal.
	 **/
	static const int ADC_MUX_TEST_SIGNAL = 5;

	/**
	 * Muliplexer routing: both positive ADC inputs are connected together.
	 **/
	static const int ADC_MUX_ECG_EINTHOVEN = 6;

private:
	int adc0_mux_index;
	int adc1_mux_index;

public:
	void setAdc0_mux_index(int idx) {
		adc0_mux_index = idx;
	}

	void setAdc1_mux_index(int idx) {
		adc1_mux_index = idx;
	}


	/**
	 * Temperature
	 **/
	static float phys2temperature(float volt) {
		return (float)((volt - 145300E-6) / 490E-6 + 25);
	}


	///////////////////////////////////////////////////////////////////////////////
	// accelerometer

	/**
	 * Setting full scale range of the accelerometer to 2G.
	 **/
	static const int ACCEL_2G = 0;

	/**
	 * Setting full scale range of the accelerometer to 4G.
	 **/
	static const int ACCEL_4G = 1;

	/**
	 * Setting full scale range of the accelerometer to 8G.
	 **/
	static const int ACCEL_8G = 2;

	/**
	 * Setting full scale range of the accelerometer to 16G.
	 **/
	static const int ACCEL_16G = 3;

	/**
	 * One g in m/s^2.
	 **/
	const float oneG = 9.80665F; // m/s^2

	/**
	 * Mapping of the index to the full scale accelerations.
	 **/
	const float ACCEL_FULL_SCALE[4] = { 2 * oneG, 4 * oneG, 8 * oneG, 16 * oneG }; // m/s^2

private:
	int accel_full_scale_index;

public:
	/**
	 * Returns the accelerometer current full scale reading in m/s^2.
	 **/
	float getAccelFullScaleRange() {
		return ACCEL_FULL_SCALE[accel_full_scale_index];
	}


	/**
	 * Sets the accelerometer full scale range using the index.
	 **/
	void setAccel_full_scale_index(int idx) {
		accel_full_scale_index = idx;
	}
	

	///////////////////////////////////////////////////
	// magnetometer
	//

	/**
	 * Full scale range of the magnetometer in Tesla.
	 **/
	const float MAG_FULL_SCALE = 4800.0E-6F; // TESLA

	/**
	 * Returns the full scale magnetometer in Tesla.
	 **/
	float getMagFullScaleRange() {
		return MAG_FULL_SCALE;
	}


	/**
	 * Charging indicator. Returns one if charging.
	 **/
	int getIsCharging() {
		return isCharging;
	}


	///////////////////////////////////////////////////////////////////////
	// message listener
	// sends error/success messages back
	// for MessageListener
	// here are the messages:

	/**
	 * Message callback: Connected.
	 **/
	static const int MESSAGE_CONNECTED = 0;

	/**
	 * Message callback: Generic error.
	 **/
	static const int MESSAGE_ERROR = 1;

	/**
	 * Message callback: Reception timeout detected by the watchdog.
	 **/
	static const int MESSAGE_TIMEOUT = 7;

	/**
	 * Message callback: Managed to reconnect.
	 **/	
	static const int MESSAGE_RECONNECTED = 8;

	/**
	 * Message callback: Receiving data. 
         * One off once the connection has been set up.
	 **/
	static const int MESSAGE_RECEIVING_DATA = 9;

	/**
	 * Connects to the Attys by opening the socket.
	 * Throws char* exception if it fails.
	 **/
	virtual void connect() = 0;

	/**
	 * Starts the data acquisition by starting the main thread.
	 * and sending possibly init commands.
	 **/
	virtual void start() {
	       	start_time = 0;
		sampleNumber = 0;
	};

	/**
	 * Closes the socket safely.
	 **/
	virtual void closeSocket() = 0;

	////////////////////////////////////////////
	// connection info

	/**
	 * Returns one if the connection is active.
	 **/
	int hasActiveConnection() {
		return isConnected && (!initialising);
	}
    

	/**
	 * Gets a sample from the ringbuffer. This is a float* array of all channels.
	 **/
	sample_p getSampleFromBuffer();

	/**
	 * Is set to one if samples are available in the ringbuffer.
	 **/
	int hasSampleAvailable() {
		return (inPtr != outPtr);
	}

	/**
	 * Resets the ringbuffer to zero content.
	 **/
	void resetRingbuffer() {
		inPtr = 0;
		outPtr = 0;
	}

public:
	////////////////////////////////////////////////

	/**
	 * Register a realtime callback function which is called
	 * whenever a sample has arrived.
	 * AttysCommListener is an abstract class which
         * needs to implement hasSample().
	 **/
	void registerCallback(AttysCommListener* f) {
		callbackInterface = f;
	}

	/** 
	 * Unregister the realtime sample callback.
	 **/
	void unregisterCallback() {
		callbackInterface = NULL;
	}


	/**
	 * Callback which is called
	 * whenever a special error/event has occurred.
	 **/
	void registerMessageCallback(AttysCommMessage* f) {
		attysCommMessage = f;
	}

	/**
	 * Unregister the error/event callback.
	 **/
	void unregisterMessageCallback() {
		attysCommMessage = NULL;
	}

	/**
	 * Call this from the main activity to shut down the connection.
	 **/
	void quit();

	/**
	 * Returns an array of 14 bytes of the bluetooth address.
	 **/
	virtual unsigned char* getBluetoothBinaryAdress() = 0;

	/**
	 * returns the MAC address as a string.
	 **/
	virtual void getBluetoothAdressString(char* s) = 0;

	/**
	 * Returns the name of the Attys
	 **/
	char* getAttysName() {
		return attysName;
	}

	/**
	 * Sets the name of the Attys
	 **/
	void setAttysName(char *s) {
		strncpy(attysName, s, 255);
	}

protected:
	///////////////////////////////////////////////////////
	// from here it's private
	AttysCommListener* callbackInterface = NULL;
	int doRun = 0;
	int inPtr = 0;
	int outPtr = 0;
	int isConnected = 0;
	// ringbuffer
	float ringBuffer[nMem][NCHANNELS];
	long data[NCHANNELS];
	char raw[1024];
	float sample[NCHANNELS];
	char inbuffer[65536];
	int adcMuxRegister[2];
	int adcGainRegister[2];
	int adcCurrNegOn[2];
	int adcCurrPosOn[2];
	int expectedTimestamp = 0;
	int correctTimestampDifference = 0;
	int connectionEstablished;
	int isCharging = 0;
	int watchdogCounter = 0;
	int initialising = 1;
	int highSpeed = 0;
	unsigned long sampleNumber = 0;
	char attysName[256] = "";
	std::thread* mainThread = NULL;

	static void execMainThread(AttysCommBase *thr) {
		thr->run();
	}

	virtual void sendSyncCommand(const char *message, int checkOK) = 0;

	void sendSamplingRate();

	void sendFullscaleAccelRange();

	void sendCurrentMask();

	void sendBiasCurrent();

	void sendGainMux(int channel, int gain, int mux);

	void setADCGain(int channel, int gain) {
		sendGainMux(channel, gain, adcMuxRegister[channel]);
	}

	void setADCMux(int channel, int mux) {
		sendGainMux(channel, adcGainRegister[channel], mux);
	}

	void sendInitCommandsToAttys();

	virtual void sendInit() = 0;

	virtual void run() = 0;

	static void watchdogThread(AttysCommBase* attysCommBase) {
		while (attysCommBase->doRun) {
			if (!(attysCommBase->initialising)) {
				attysCommBase->watchdogCounter--;
				if (attysCommBase->watchdogCounter < 1) {
					attysCommBase->receptionTimeout();
					attysCommBase->watchdogCounter = 0;
				}
				// wait for 1 sec
				for (int i = 0; (i < 10) && attysCommBase->doRun; i++) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			} else {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}

	virtual void receptionTimeout() = 0;

	void correctSampleNumberAfterTimeout();

	long unsigned int start_time = 0;

private:

	void decodeStandardDataPacket();

	void decodeHighSpeedDataPacket();

public:
	// these need to be public because they are called from objC but
	// should be be touched.
	
	void processRawAttysData(const char* data);

	// needed for the delegate
	int isInitialising() {
		return initialising;
	}

	void setConnected(int c) {
		isConnected = c;
	}

	AttysCommMessage* attysCommMessage = NULL;
};

#endif
