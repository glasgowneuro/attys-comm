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
 * Callback which reports the status of the scanner
 **/
struct AttysScanListener {
	virtual ~AttysScanListener() {};
	virtual void message(const int, const char *) = 0;
};


/**
 * Scans for Attys and creates instances of AttysComm
 * for every detected/paired Attys. There is no need
 * to create instances of AttysComm yourself. This
 * is done by this class automatically.
 **/
class AttysScan {

public:
	~AttysScan();

	/**
	* Scans for the specified number of devices and connects to them.
	* By default only for one Attys.
	* returns 0 on success
	**/
	int scan(int maxAttys = 1);

	/**
         * Message index that the connection to an attys has been
         * successful.
         **/
	static const int SCAN_CONNECTED = 0;

	/**
	 * Message index that AttysScan is actively scanning
         **/
	static const int SCAN_SEARCHING = 1;

	/**
         * Message index that no Attys has been detected
         **/
	static const int SCAN_NODEV = 2;

	/**
         * Message that the socket could not be opened
         **/
	static const int SCAN_SOCKETERR = 3;

	/**
	 * In the process of connecting
	 **/
	static const int SCAN_CONNECTING = 4;

	/**
	 * Connection error during scanning
	 **/
	static const int SCAN_CONNECTERR = 5;

	/**
	* Max number of Attys Devices
	**/
	static const int MAX_ATTYS_DEVS = 4;

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
		return attysComm[i]->getAttysName();
	}

	/**
	* Returns the number of Attys devices
	**/
	int getNAttysDevices() {
		return nAttysDevices;
	}


private:

	void sortAttys() {
		// brute force sort
		for(int j = 0; j < (nAttysDevices-1); j++) {
			for(int i = 0; i < (nAttysDevices-1); i++) {
				if (strcmp(attysComm[i]->getAttysName(),
					   attysComm[i+1]->getAttysName()) > 0) {
					AttysComm* ac = attysComm[i];
					attysComm[i] = attysComm[i+1];
				attysComm[i+1] = ac;
				}
			}
		}
	}
		    
	AttysScanListener* statusCallback = NULL;
	int nAttysDevices = 0;
	AttysComm** attysComm = NULL;
};


#endif
