#include <stdio.h>
#include "AttysComm.h"
#include "AttysScan.h"

int main (int,char**)
{
	// scan for Attys
	attysScan.scan();
	if (attysScan.nAttysDevices < 1) {
		fprintf(stderr,"No devices found.\n");
		return 0;
	}
	// start the data acquisition in the background
	attysScan.attysComm[0]->start();
	while (1) {
		// wait for a sample
		while (!(attysScan.attysComm[0]->hasSampleAvilabale()));
		// get the sample from the buffer
		float* values = attysScan.attysComm[0]->getSampleFromBuffer();
		// print it to stdout
		for(int i = 0; i < attysScan.attysComm[0]->NCHANNELS; i++) {
			printf("%f\t",values[i]);
		}
		printf("\n");
	}
	return 0;
}
