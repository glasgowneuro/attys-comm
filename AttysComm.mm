#include "AttysComm.h"

#include <assert.h>

#include <chrono>
#include <thread>

#import <Foundation/Foundation.h>

#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothSDPUUID.h>
#import <IOBluetooth/objc/IOBluetoothRFCOMMChannel.h>
#import <IOBluetoothUI/objc/IOBluetoothDeviceSelectorController.h>


@interface AsyncCommDelegate : NSObject <IOBluetoothRFCOMMChannelDelegate> {
@public
    AttysComm* delegateCPP;
}
@end

@implementation AsyncCommDelegate {
}

-(void)rfcommChannelOpenComplete:(IOBluetoothRFCOMMChannel *)rfcommChannel status:(IOReturn)error
{
    
    if ( error != kIOReturnSuccess ) {
        _RPT1(0,"Error - could not open the RFCOMM channel. Error code = %08x.\n",error);
        delegateCPP->setConnected(0);
        delegateCPP->connectError = 1;
	delegateCPP->reconnect = 0;
        return;
    }
    else{
        _RPT0(0,"Connected.\n");
        delegateCPP->setConnected(1);
        delegateCPP->connectError = 0; 
	delegateCPP->reconnect = 0;
   }
    
}

-(void)rfcommChannelData:(IOBluetoothRFCOMMChannel *)rfcommChannel data:(void *)dataPointer length:(size_t)dataLength
{
    if (delegateCPP->isInitialising()) {
        delegateCPP->cmdMtx.lock();
        if (delegateCPP->recBuffer) {
            delete delegateCPP->recBuffer;
        }
        delegateCPP->recBuffer = new char[dataLength+1];
        strncpy(delegateCPP->recBuffer,(char*)dataPointer,dataLength);
        delegateCPP->recBuffer[dataLength] = 0;
        delegateCPP->cmdMtx.unlock();
    } else {
        char tmp[dataLength+1];
        strncpy(tmp,(char*)dataPointer,dataLength);
        tmp[dataLength] = 0;
        delegateCPP->processRawAttysData(tmp);
    }
}


@end


void AttysComm::getReceivedData(char* buf, int maxlen) {
    for(int i=0; (i < 100) && ((!isConnected) && (doRun)); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (!doRun) return;
    if (!isConnected) {
        connectError = 1;
        return;
    }
    cmdMtx.lock();
    if (recBuffer) {
        strncpy(buf,recBuffer,maxlen);
        delete recBuffer;
        recBuffer = NULL;
        _RPT1(0,"Received:%s\n",buf);
    } else {
        strncpy(buf,"",maxlen);
    }
    cmdMtx.unlock();
}


int AttysComm::sendBT(const char* dataToSend)
{
    return [(__bridge IOBluetoothRFCOMMChannel*)rfcommchannel writeSync:(void*)dataToSend length:strlen(dataToSend)];
}

int AttysComm::tryToConnect() {
    isConnected = 0;
    connectError = 0;
    _RPT0(0,"Trying to connect.\n");
    if (mainThread) {
        doRun = 0;
        _RPT0(0,"Waiting for the thread to terminate.\n");
        mainThread->join();
        _RPT0(0,"Deleting thread.\n");
        delete mainThread;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    _RPT0(0,"Starting main thread.\n");
    doRun = 1;
    mainThread = new std::thread(AttysCommBase::execMainThread, this);
    for(int i =0; (i < 100) && ((!isConnected) && (doRun)); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (connectError) {
            doRun = 0;
            return -1;
        }
    }
    if (!doRun) return -1;
    if ((!connectError) && isConnected) {
        return 0;
    } else {
        return -1;
    }
}

void AttysComm::connect() {
    if (tryToConnect()) {
        throw "Connection error";
    }
}


void AttysComm::closeSocket() {
    if (!rfcommchannel) return;
    _RPT0(0,"Closing RFCOMM channel.\n");
    IOBluetoothRFCOMMChannel *chan = (__bridge IOBluetoothRFCOMMChannel*) rfcommchannel;
    if (chan) {
        [chan closeChannel];
        rfcommchannel = NULL;
    }
    _RPT0(0,"RFCOMM channel closed.\n");
}


void AttysComm::sendSyncCommand(const char *msg, int waitForOK) {
    char cr[] = "\n";
    int ret = 0;
    // 10 attempts
    while (doRun) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        _RPT1(0,"Sending: %s", msg);
        ret = sendBT(msg);
        if (kIOReturnSuccess != ret) {
            if (attysCommMessage) {
                attysCommMessage->hasMessage(MESSAGE_ERROR, "Message transmit error");
            }
	    return;
        }
        ret = sendBT(cr);
        if (kIOReturnSuccess != ret) {
            if (attysCommMessage) {
                attysCommMessage->hasMessage(MESSAGE_ERROR, "Message transmit error");
            }
	    return;
        }
        if (!waitForOK) {
            return;
        }
        for (int i = 0; i < 100; i++) {
            char linebuffer[8192];
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            getReceivedData(linebuffer, 8191);
            ret = strlen(linebuffer);
            if ((ret > 2) && (ret < 5)) {
                _RPT1(0,">>%s<<\n",linebuffer);
                if (strstr(linebuffer, "OK")) {
                    _RPT0(0, " -- OK received\n");
                    return;
                }
            }
        }
        _RPT0(0, " -- no OK received!\n");
    }
}


void AttysComm::sendInit() {
    _RPT0(0,"Sending Init\n");
    // flag to prevent the data receiver to mess it up!
    initialising = 1;
    strcpy(inbuffer, "");
    char rststr[] = "\n\n\n\n\r";
    sendBT(rststr);
    
    // everything platform independent
    sendInitCommandsToAttys();
    
    strcpy(inbuffer, "");
    initialising = 0;
    _RPT0(0,"Init finished. Waiting for data.\n");
}

void AttysComm::start() {
    sendInit();
    if (attysCommMessage) {
            attysCommMessage->hasMessage(delegateCPP->MESSAGE_RECEIVING_DATA, "Receiving data now.");
    }
}

void AttysComm::run() {
    IOBluetoothDevice *device = (__bridge IOBluetoothDevice *)btAddr;
    _RPT0(0,"Getting service UUID.\n");
    IOBluetoothSDPUUID *sppServiceUUID = [IOBluetoothSDPUUID uuid16:kBluetoothSDPUUID16ServiceClassSerialPort];
    _RPT0(0,"Getting service record.\n");
    IOBluetoothSDPServiceRecord     *sppServiceRecord = [device getServiceRecordForUUID:sppServiceUUID];
    if ( sppServiceRecord == nil ) {
        _RPT0(0,"Service record is NULL.\n");
        connectError = 1;
        doRun = 0;
        return;
    }
    _RPT0(0,"Getting channel ID.\n");
    if ( [sppServiceRecord getRFCOMMChannelID:&rfcommChannelID] != kIOReturnSuccess ) {
        _RPT0(0,"Could not get channel ID.\n");
        connectError = 1;
        doRun = 0;
        return;
    }
    
    AsyncCommDelegate* rfCommDelegate = [[AsyncCommDelegate alloc] init];
    rfCommDelegate->delegateCPP = this;
    asyncCommDelegate = (__bridge void*) rfCommDelegate;

    IOBluetoothRFCOMMChannel *chan = NULL;
    _RPT0(0,"Opening RFCOMM channel!\n");
    if ( [device openRFCOMMChannelAsync:&chan withChannelID:rfcommChannelID delegate:rfCommDelegate] != kIOReturnSuccess ) {
        _RPT0(0,"Fatal - could not open the rfcomm.\n");
        connectError = 1;
        doRun = 0;
        if (chan) {
            [chan setDelegate:nil];
            [chan closeChannel];
        }
        return;
    }
    
    rfcommchannel = (__bridge void*) chan;
    
    _RPT0(0,"RFCOMM channel is open.\n");
    
    watchdogCounter = 2 * TIMEOUT_IN_SECS * getSamplingRateInHz();
    std::thread watchdog(AttysComm::watchdogThread, this);
    
    doRun = 1;
    while (doRun) {
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
    }
    
    watchdog.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    _RPT0(0, "Closing channel.\n");
    [chan setDelegate:nil];
    [chan closeChannel];
    _RPT0(0, "Channel closed.\n");
    asyncCommDelegate = NULL;
}



void AttysComm::receptionTimeout() {
    if (reconnect) return;
    setConnected(0);
    _RPT0(0, "Timeout.\n");
    if (attysCommMessage) {
        attysCommMessage->hasMessage(MESSAGE_TIMEOUT, "reception timeout to Attys");
    }
    IOBluetoothDevice *device = (__bridge IOBluetoothDevice *)btAddr;
    IOBluetoothRFCOMMChannel* chan = (__bridge IOBluetoothRFCOMMChannel*)rfcommchannel;
    if (!asyncCommDelegate) {
	fprintf(stderr,"delegate null!\n");
	return;
    }
    [chan closeChannel];
    if ( [device openRFCOMMChannelAsync:&chan withChannelID:rfcommChannelID delegate:((__bridge AsyncCommDelegate*)asyncCommDelegate)] != kIOReturnSuccess ) {
        _RPT0(0,"Fatal - could not open the rfcomm.\n");
        return;
    }
    rfcommchannel = (__bridge void*) chan;
    reconnect = 1;
    while (doRun & reconnect) {
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    sendInit();
    correctSampleNumberAfterTimeout();
    _RPT0(0, "Reconnected.\n");
}





unsigned char* AttysComm::getBluetoothBinaryAdress() {
    IOBluetoothDevice *device = (__bridge IOBluetoothDevice *)btAddr;
    return (unsigned char*)[device getAddress];
}


void AttysComm::getBluetoothAdressString(char* s) {
    IOBluetoothDevice *device = (__bridge IOBluetoothDevice *)btAddr;
    strcpy(s,[device.addressString UTF8String]);
}

