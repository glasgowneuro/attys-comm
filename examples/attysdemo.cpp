#include <stdio.h>
#include "AttysComm.h"
#include "AttysScan.h"

int main (int,char**)
{
	// scan for Attys
	attysScan.scan();
	if (attysScan.getNAttysDevices() < 1) {
		fprintf(stderr,"No devices found.\n");
		return 0;
	}
	// start the data acquisition in the background
	attysScan.getAttysComm(0)->start();
	while (1) {
		// wait for a sample
		while (!(attysScan.getAttysComm(0)->hasSampleAvilabale()));
		// get the sample from the buffer
		float* values = attysScan.getAttysComm(0)->getSampleFromBuffer();
		// print it to stdout
		for(int i = 0; i < attysScan.getAttysComm(0)->NCHANNELS; i++) {
			printf("%f\t",values[i]);
		}
		printf("\n");
		fflush(stdout);
	}
	return 0;
}
