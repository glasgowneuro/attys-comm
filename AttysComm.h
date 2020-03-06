class AttysComm;

#ifndef __ATTYS_COMM_H
#define __ATTYS_COMM_H

#include"AttysCommBase.h"

/**
 * AttysComm contains all the neccessary comms to talk to
 * the Attys on Linux, Windows and Mac. \n
 **/

#ifdef __linux__ 
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>
#include<sys/ioctl.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<termios.h>
#include <string>
#define SOCKET int
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#define OutputDebugStringW(s)
#elif _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#include <string>
#else
#endif

#include <thread>   

#include "attyscomm/base64.h"

#pragma once

/**
 * AttysComm class contains the
 * device specific definitions and
 * implements the abstract classes of
 * AttysCommBase. See AttysCommBase
 * for the definitions there.
 * Instances of this class are automatically
 * created by AttysScan and the user
 * can ignore definitions here. All relevant
 * user functions are in AttysCommBase.
 * Use this class only if you have a fixed
 * bluetooth address (Linux/Win) or a fixed
 * bluetooth device (Mac) and won't need
 * to scan for a bluetooth device.
 **/
class AttysComm : public AttysCommBase
{
public:
/**
 * Constructor: Win/Linux: takes the bluetooth device structure
 * and its length as an argument. For Mac: just a pointer to the
 * bluetooth device (needs typecast to *_btAddr) and provide
 * no length.
 **/
	AttysComm(void *_btAddr = NULL, int _btAddrLen = 0) : AttysCommBase() {
		if (_btAddrLen > 0) {
			btAddr = (struct sockaddr *) malloc(_btAddrLen);
			memcpy(btAddr, _btAddr, _btAddrLen);
			btAddrLen = _btAddrLen;
		}
		else {
			btAddr = (struct sockaddr *) _btAddr;
			btAddrLen = 0;
		}
	};

	/**
	 * Connects to the Attys by opening the socket.
	 * Throws char* exception if it fails.
	 **/	
	virtual void connect();

	/**
	 * Closes the socket safely.
	 **/
	virtual void closeSocket();

	/**
	 * Thread which does the data acquisition.
	 * Do not call directly.
	 **/
	virtual void run();

	/**
	 * Call this from the main activity to shut down the connection.
	 **/
	virtual void quit() {
		AttysCommBase::quit();
		closeSocket();
		if (btAddr) {
			free(btAddr);
			btAddr = NULL;
		}
	}

	/**
	 * Sends a command to the Attys.
	 * Do not use unless you know exactly what you are doing.
	 **/
	virtual void sendSyncCommand(const char *message, int waitForOK);

	/**
	 * Sends the init sequence to the Attys. Do not use unless
	 * you know exactly what you are doing.
	 **/
	virtual void sendInit();

	/**
	 * Starts the data acquisition by starting the main thread.
	 * and sending possibly init commands.
	 **/
	virtual void start();

	/**
	 * Called from the watchdog after a timeout. Do not
	 * call this directly.
	 **/
	virtual void receptionTimeout();

public:
	/**
	 * Returns an array of 14 bytes of the bluetooth address.
	 **/
	unsigned char* getBluetoothBinaryAdress();

	/**
	 * returns the MAC address as a string.
	 **/
	void getBluetoothAdressString(char* s);

private:
	///////////////////////////////////////////////////////
	// from here it's private
	struct sockaddr *btAddr = NULL;
	int btAddrLen = 0;
#ifndef __APPLE__
	SOCKET btsocket = 0;
#endif

	static void watchdogThread(AttysComm* attysComm) {
		while (attysComm->doRun) {
			attysComm->watchdogCounter--;
			if (attysComm->watchdogCounter < 1) {
				attysComm->receptionTimeout();
				attysComm->watchdogCounter = 0;
			}
			for (int i = 0; (i < 10) && attysComm->doRun; i++) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
	}



	//////////////////
	// mac
#ifdef __APPLE__
private:
    int sendBT(const char* dataToSend);
	void* rfcommDevice = NULL;
	void* rfcommchannel = NULL;
	void* asyncCommDelegate = NULL;
	AttysComm* delegateCPP = NULL;
	unsigned char rfcommChannelID;
	int tryToConnect();
public:
	char* recBuffer = NULL;
	void getReceivedData(char* buf, int maxlen);
	int connectError = 0;
	int reconnect = 0;
    std::mutex cmdMtx;
#endif


};


#endif
