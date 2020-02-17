class AttysComm;

#ifndef __ATTYS_COMM_H
#define __ATTYS_COMM_H

#include"AttysCommBase.h"

/**
 * AttysComm contains all the neccessary comms to talk to
 * the Attys on both Linux and Windows.
 *
 * 1) Instantiate the class AttyScan and do a scan
 *    It finds all paired Attys and creates separate AttysComm classes
 * 2) These classes are in in the array attysComm in AttysScan and
 *    the number of them in nAttysDevices.
 * 3) All attysComm are Threads so just start the data acquisition
 *    with start(), for example attysComm[0]->start() for the 1st Attys
 * 4) Get the data either via the RingBuffer functions or register a
 *    callback to get the data as it arrives.
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

class AttysComm : public AttysCommBase
{
public:
	/////////////////////////////////////////////////
	// Constructor: takes the bluetooth device as an argument
	// it then tries to connect to the Attys
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

	virtual void connect();

	virtual void closeSocket();

	virtual void run();

	virtual void quit() {
		AttysCommBase::quit();
		closeSocket();
		if (btAddr) {
			free(btAddr);
			btAddr = NULL;
		}
	}

	virtual void sendSyncCommand(const char *message, int waitForOK);

	virtual void sendInit();

	virtual void start();

	virtual void receptionTimeout();

public:
	// returns an array of 14 bytes
    unsigned char* getBluetoothBinaryAdress();

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
	void *rfcommchannel = NULL;
	AttysComm* delegateCPP = NULL;
	int tryToConnect();
public:
	char* recBuffer = NULL;
	void getReceivedData(char* buf, int maxlen);
	int connectError = 0;
#endif


};


#endif
