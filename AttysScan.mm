#include "AttysScan.h"

#import <Foundation/Foundation.h>

#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothRFCOMMChannel.h>
#import <IOBluetoothUI/objc/IOBluetoothDeviceSelectorController.h>

#include <thread>

// convenience class which can be used globally
AttysScan attysScan;

/**
 * Scans for Attys Devices
 * Fills up the variables
 * Returns zero on success
 **/
int AttysScan::scan(int maxAttysDevs) {
    attysComm = new AttysComm*[maxAttysDevs];
    assert(attysComm != nullptr);
    for (int devNo = 0; devNo < maxAttysDevs; devNo++) {
        attysComm[devNo] = nullptr;
    }
    
    nAttysDevices = 0;
    
    _RPT0(0,"Attempting to connect\n");
    
    NSArray *deviceArray = [IOBluetoothDevice pairedDevices];
    if ( ( deviceArray == nil ) || ( [deviceArray count] == 0 ) ) {
	if (statusCallback) {
			statusCallback->message(SCAN_NODEV, "No Attys paired");
		}
	return 0;
    }
    
    _RPT1(0,"We have %lu paired device(s).\n",(unsigned long)[deviceArray count]);
    
    // let's probe how many we have
    nAttysDevices = 0;
    for (int i = 0; (i < [deviceArray count]) && (i < maxAttysDevs); i++) {
        IOBluetoothDevice *device = [deviceArray objectAtIndex:i];
        _RPT1(0,"device name = %s ",[device.name UTF8String]);
        char name[256];
        strcpy(name,[device.name UTF8String]);
        if (strstr(name, "GN-ATTYS") != 0) {
            _RPT0(0, "! Found one. ");
            // allocate a socket
            attysComm[nAttysDevices] = new AttysComm((__bridge void*)device);
            if (attysComm[nAttysDevices] == NULL) {
                break;
            }
            try {
	    	if (statusCallback) {
			char tmp[256];
			sprintf(tmp,"connecting to %s",name);
			statusCallback->message(SCAN_CONNECTING, tmp);
		}
                attysComm[nAttysDevices]->connect();
		attysComm[nAttysDevices]->setAttysName(name);
                nAttysDevices++;
                _RPT0(0, "\n");
	    	if (statusCallback) {
			char tmp[256];
			sprintf(tmp,"connected to %s",name);
			statusCallback->message(SCAN_CONNECTED, tmp);
		}
            }
            catch (const char *msg) {
                if (statusCallback) {
                    statusCallback->message(SCAN_CONNECTERR, msg);
                }
                attysComm[nAttysDevices]->closeSocket();
                delete attysComm[nAttysDevices];
                attysComm[nAttysDevices] = NULL;
                _RPT0(0, "Connection error.\n");
            }
        }
        else {
            _RPT0(0, "\n");
        }
    }
    sortAttys();

    // get them both to sync
    for (int devNo = 0; devNo < nAttysDevices; devNo++) {
        attysComm[devNo]->resetRingbuffer();
    }
    
    return 0;
}


AttysScan::~AttysScan() {
    if (!attysComm) return;
    for (int devNo = 0; devNo < nAttysDevices; devNo++) {
        if (attysComm[devNo]) {
            delete attysComm[devNo];
            attysComm[devNo] = NULL;
        }
    }
    delete[] attysComm;
}
