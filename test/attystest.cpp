#include <stdio.h>
#include "AttysComm.h"
#include "AttysScan.h"


struct MySample : public AttysCommListener {
	virtual void hasSample(double t, float *values) {
		// print it to stdout
		printf("%f\t",t);
		for(int i = 0; i < AttysCommBase::NCHANNELS; i++) {
			printf("%f\t",values[i]);
		}
		printf("\n");
	}
};


struct MyMessage : public AttysCommMessage {
	virtual void hasMessage(int, const char* s) {
		printf("Message = %s\n",s);
	}
};




int main (int,char**) {
	MySample mysample;
	MyMessage mymessage;
	// scan for Attys
	attysScan.scan();
	if (attysScan.getNAttysDevices() < 1) {
		printf("No devices found.\n");
		return 0;
	}
	attysScan.getAttysComm(0)->registerCallback(&mysample);
	attysScan.getAttysComm(0)->registerMessageCallback(&mymessage);
	// start the data acquisition in the background
	attysScan.getAttysComm(0)->start();
	getchar();
	attysScan.getAttysComm(0)->quit();
	return 0;
}
