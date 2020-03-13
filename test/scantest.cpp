#include <stdio.h>
#include "AttysComm.h"
#include "AttysScan.h"


struct MyMessage : public AttysCommMessage {
	virtual void hasMessage(int, const char* s) {
		printf("Message = %s\n",s);
	}
};




int main (int,char**) {
	MyMessage mymessage;
	// scan for Attys
	attysScan.scan(4);
	if (attysScan.getNAttysDevices() < 1) {
		printf("No devices found.\n");
		return 0;
	}
	for(int i=0;i<attysScan.getNAttysDevices();i++) {
		printf("Attys %02d: %s\n",i,attysScan.getAttysName(i));
		attysScan.getAttysComm(i)->start();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	for(int i=0;i<attysScan.getNAttysDevices();i++) {
		attysScan.getAttysComm(i)->quit();
	}
	return 0;
}
