class AttysScan;

#ifndef __ATTYS_SCAN_H
#define __ATTYS_SCAN_H


/**
 * AttysScan scans for Attys devices and then
 * creates an array of AttysComm intances which
 * are ready to be used
 *
 **/

#include "AttysComm.h"

#pragma once

// global variable for convenience
extern AttysScan attysScan;


/**
 * Max number of Attys Devices
 **/
#define MAX_ATTYS_DEVS 4


// callback which reports the status of the scanner
struct AttysScanListener {
	virtual ~AttysScanListener() {};
	virtual void message(const int, const char *) = 0;
};


class AttysScan {

public:
	~AttysScan();

/**
 * Scans for all attys devices and fills the global
 * variables above
 * returns 0 on success
 **/
	int scan(int maxAttys = MAX_ATTYS_DEVS);

	static const int SCAN_CONNECTED = 0;
	static const int SCAN_SEARCHING = 1;
	static const int SCAN_NODEV = 2;
	static const int SCAN_SOCKETERR = 3;
	static const int SCAN_CONNECTING = 4;
	static const int SCAN_CONNECTERR = 5;

	/**
	* Register callback which reports the scanning progress for
	* example for a splash screen.
	**/
	void registerCallback(AttysScanListener* f) {
		statusCallback = f;
	}

	/**
	* Unregisters the callback
	**/
	void unregisterCallback() {
		statusCallback = NULL;
	}

	/**
	* Obtains the pointer to a valid AttysComm class which has been
	* successfully detected while scanning.
	**/
	AttysComm* getAttysComm(int i) {
		return attysComm[i];
	}

	/**
	* Gets the Attys name as reported by the bluetooth manager
	**/
	char* getAttysName(int i) {
		return attysName[i];
	}

	/**
	* Returns the number of Attys devices
	**/
	int getNAttysDevices() {
		return nAttysDevices;
	}

	AttysScanListener* statusCallback = NULL;
	int nAttysDevices = 0;
	char** attysName = NULL;
	AttysComm** attysComm = NULL;

};


#endif
