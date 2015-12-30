/*
 *  NativeMIDIMac.h
 *  NativeMIDIMac
 *
 *  Created by D. Kadrioski on 3/17/12.
 *  Copyright 2012 Nebiru Software. All rights reserved.
 *
 */

#define EXPORT __attribute__((visibility("default")))

#include <Adobe AIR/Adobe AIR.h>
#include <CoreMIDI/MIDIServices.h>

EXPORT
void initializer(void** extDataToSet, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizerToSet);

EXPORT
void finalizer(void* extData);

/* This encapsulates the receive request for a seqence of sysex messages.
Strictly C and Core Foundation.  */
typedef struct _MySysexSeqReceiveReq {
	MIDIEndpointRef endpointRef;
	MIDIPortRef portRef;
	CFMutableDataRef *outSysexData;
	bool stopped;
} MySysexSeqReceiveReq;