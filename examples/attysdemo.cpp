#include <stdio.h>
#include "AttysComm.h"
#include "AttysScan.h"


struct MySample : public AttysCommListener {
	virtual void hasSample(double t, float *values) {
		// print it to stdout
		printf("%5.3f: ",t);
		for(int i = 0; i < AttysCommBase::NCHANNELS; i++) {
			if ( (i == AttysCommBase::INDEX_Magnetic_field_X) ||
			     (i == AttysCommBase::INDEX_Magnetic_field_Y) ||
			     (i == AttysCommBase::INDEX_Magnetic_field_Z) ) {
				printf("%1.3fE-3  ",values[i]*1000);				
			} else {
				printf("%1.3f  ",values[i]);
			}
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
	attysScan.getAttysComm(0)->setAdc_samplingrate_index(AttysCommBase::ADC_RATE_500HZ);
	attysScan.getAttysComm(0)->start();
	getchar();
	attysScan.getAttysComm(0)->quit();
	return 0;
}
