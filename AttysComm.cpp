#include "AttysComm.h"

#include <assert.h>

#include <chrono>
#include <thread>

void AttysComm::connect() {
	if (btAddr == NULL) throw "Bluetooth structure is NULL";
	if (btAddrLen == 0) throw "Bluetooth structure length is zero.";

	// allocate a socket
#ifdef __linux__ 
	btsocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
#elif _WIN32
	btsocket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (INVALID_SOCKET == btsocket) {
		_RPT0(0, "=CRITICAL= | socket() call failed.\n");
		throw "socket() call failed: cannot create socket!";
	}
#endif

	// connect to server
	int status = ::connect(btsocket, btAddr, btAddrLen);

	if (status == 0) {
		_RPT0(0, "Connect successful\n");
		if (attysCommMessage) {
			attysCommMessage->hasMessage(MESSAGE_CONNECTED, "Connected");
		}
		return;
	}
	_RPT1(0, "Connect failed: status=%d\n",status);
	if (attysCommMessage) {
		attysCommMessage->hasMessage(MESSAGE_ERROR, "Connect failed");
	}
	closeSocket();
	throw "Connect failed";
}

void AttysComm::start() {
	if (mainThread) {
		return;
	}
	mainThread = new std::thread(AttysCommBase::execMainThread, this);
	AttysCommBase::start();
}


void AttysComm::closeSocket() {
#ifdef __linux__ 
	shutdown(btsocket, SHUT_RDWR);
	close(btsocket);
#elif _WIN32
	shutdown(btsocket, SD_BOTH);
	closesocket(btsocket);
#else
#endif
}


void AttysComm::sendSyncCommand(const char *message, int waitForOK) {
	char cr[] = "\n";
	int ret = 0;
	// 10 attempts
	for (int k = 0; k < 10; k++) {
		_RPT1(0, "Sending: %s", message);
		ret = send(btsocket, message, (int)strlen(message), 0);
		if (ret < 0) {
			if (attysCommMessage) {
				attysCommMessage->hasMessage(MESSAGE_ERROR, "Message transmit error");
			}
		}
		send(btsocket, cr, (int)strlen(cr), 0);
		if (!waitForOK) {
			return;
		}
		for (int i = 0; i < 100; i++) {
			char linebuffer[8192];
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			ret = recv(btsocket, linebuffer, 8191, 0);
			if (ret < 0) {
				if (attysCommMessage) {
					attysCommMessage->hasMessage(MESSAGE_ERROR, "could receive OK");
				}
			}
			if ((ret > 2) && (ret < 5)) {
				linebuffer[ret] = 0;
				//fprintf(stderr,">>%s<<\n",linebuffer);
				linebuffer[ret] = 0;
				if (strstr(linebuffer, "OK")) {
					_RPT0(0, " -- OK received\n");
					return;
				}
			}
		}
		_RPT0(0, " -- no OK received!\n");
	}
}


void AttysComm::getBluetoothAdressString(char* s) {
  		sprintf(s, "%02X:%02X:%02X:%02X:%02X:%02X",
			(unsigned char)(btAddr->sa_data[0]),
			(unsigned char)(btAddr->sa_data[1]),
			(unsigned char)(btAddr->sa_data[2]),
			(unsigned char)(btAddr->sa_data[3]),
			(unsigned char)(btAddr->sa_data[4]),
			(unsigned char)(btAddr->sa_data[5])
		);
	}


unsigned char* AttysComm::getBluetoothBinaryAdress() {
  return (unsigned char*)(btAddr->sa_data);
}





void AttysComm::sendInit() {
	_RPT0(0,"Sending Init\n");
	// flag to prevent the data receiver to mess it up!
	initialising = 1;
#ifdef _WIN32
	u_long iMode = 1;
	ioctlsocket(btsocket, FIONBIO, &iMode);
#else
	fcntl(btsocket, F_SETFL, fcntl(btsocket, F_GETFL, 0) | O_NONBLOCK);
#endif
	strcpy(inbuffer, "");
	char rststr[] = "\n\n\n\n\r";
	send(btsocket, rststr, (int)strlen(rststr), 0);

	// everything platform independent
	sendInitCommandsToAttys();

#ifdef _WIN32
	iMode = 0;
	ioctlsocket(btsocket, FIONBIO, &iMode);
#else
	fcntl(btsocket, F_SETFL, fcntl(btsocket, F_GETFL, 0) & ~O_NONBLOCK);
#endif
	strcpy(inbuffer, "");
	initialising = 0;
	_RPT0(0,"Init finished. Waiting for data.\n");
}


void AttysComm::run() {
	char recvbuffer[8192];

	sendInit();

	doRun = 1;

	if (attysCommMessage) {
		attysCommMessage->hasMessage(MESSAGE_RECEIVING_DATA, "Receiving data now");
	}

	isConnected = 1;

	watchdogCounter = TIMEOUT_IN_SECS * getSamplingRateInHz();
	std::thread watchdog(AttysComm::watchdogThread, this);

	// Keep listening to the InputStream until an exception occurs
	while (doRun) {

		while (initialising && doRun) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		int ret = recv(btsocket, recvbuffer, sizeof(recvbuffer), 0);
		if (ret < 0) {
			if (attysCommMessage) {
				attysCommMessage->hasMessage(MESSAGE_ERROR, "Data reception error");
			}
		}
		if (ret > 0) {
			recvbuffer[ret] = 0;
			processRawAttysData(recvbuffer);
		}
	}

	watchdog.join();

	isConnected = 0;
};


void AttysComm::receptionTimeout() {
	_RPT0(0, "Timeout.\n");
	if (attysCommMessage) {
		attysCommMessage->hasMessage(MESSAGE_TIMEOUT, "reception timeout to Attys");
	}
	initialising = 1;
	closeSocket();
	while (doRun) {
		try {
			connect();
			break;
		}
		catch (const char *) {
			_RPT0(0, "Reconnect failed. Sleeping for 1 sec.\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	if (!doRun) {
		return;
	}
	sendInit();
	initialising = 0;
	if (attysCommMessage) {
		attysCommMessage->hasMessage(MESSAGE_RECONNECTED, "reconnected to Attys");
	}
	correctSampleNumberAfterTimeout();
	_RPT0(0, "Reconnected.\n");
}
