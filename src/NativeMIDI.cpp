#include "NativeMIDI.h"
#include <cstring>

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "pthread.h"

//#import "Foundation/Foundation.h"
//#include "CoreMIDI/MIDIServices.h"

#include "FlashRuntimeExtensions.h"

//------------------------------------------------------------------------------
#define SYSEX_LENGTH 8419

#define MIN(a,b)    ((a) < (b) ? (a) : (b))
#define MAX(a,b)    ((a) > (b) ? (a) : (b))

uint32_t isSupportedInOS = 1;

const uint8_t* IN_DATA      = (const uint8_t*)"IN_DATA";
const uint8_t* MSG_INFO     = (const uint8_t*)"MSG_INFO";
const uint8_t* MSG_ERROR    = (const uint8_t*)"MSG_ERROR";
const uint8_t* IN_LONGDATA  = (const uint8_t*)"IN_LONGDATA";
const uint32_t SYSEX_SIZE   = 8419;

FREContext dllContext;

MIDIClientRef midiClientRef = NULL;

MIDIEndpointRef midiEndpointRef_I = NULL;
MIDIEndpointRef midiEndpointRef_O = NULL;

MIDIPortRef midiPortRef_I = NULL;
MIDIPortRef midiPortRef_O = NULL;

int previousInputIdx  = -1;
int previousOutputIdx = -1;

// Thread safe stuff for receiving sysex.
//pthread_mutex_t mutex;
CFMutableDataRef outSysexData;
MySysexSeqReceiveReq seqReq;
bool continueSysEx = false;

//------------------------------------------------------------------------------
void dispatchEvent( const uint8_t* level, const uint8_t* code ){
    FREDispatchStatusEventAsync( dllContext, code, level );
}
//------------------------------------------------------------------------------
void showMessage(const uint8_t* msg){
    dispatchEvent( MSG_INFO, msg );
}
//------------------------------------------------------------------------------
void showError(const uint8_t* msg){
    dispatchEvent( MSG_ERROR, msg );
}
//------------------------------------------------------------------------------
void processOSStatus(const OSStatus status, const uint8_t* msg){
    if (status != noErr) {
        char str[1024];
        str[0] = '\0';
        
        strcat( str, (const char *)msg );
        strcat( str, ": " );
        /*strcat( str, (const char *)GetMacOSStatusErrorString(status) );
         strcat( str, " - " );
         strcat( str, (const char *)GetMacOSStatusCommentString(status) );*/
        showError( (const uint8_t*)str );
        
        // Also try to report the specific error code.  Could be a number or string.
        str[0] = '\0';
        // see if it appears to be a 4-char-code
        *(UInt32 *)(str + 1) = CFSwapInt32HostToBig(status);
        if (isprint(str[1]) && isprint(str[2]) && isprint(str[3]) && isprint(str[4])) {
            str[0] = str[5] = '\'';
            str[6] = '\0';
        } else {
            // no, format it as an integer
            sprintf(str, "%d", (int)status);
        }
        showError( (const uint8_t*)str );
    }
}
//------------------------------------------------------------------------------
void sysExComplete(){
    char str[1024];
    
    //CFMutableDataRef outSysexData = *(((MySysexSeqReceiveReq *)readProcRefCon)->outSysexData);
    
    showMessage( (const uint8_t*) "done." );
    sprintf( str, "sysExLength=%d", (int)CFDataGetLength( outSysexData ) );
    showMessage((const uint8_t*) str);
    
    
    unsigned char aBuffer[ SYSEX_LENGTH ];
    CFDataGetBytes(outSysexData,CFRangeMake(0, CFDataGetLength( outSysexData )), aBuffer);
    
    for (unsigned int i = 0; i < CFDataGetLength( outSysexData ); ++i) {
        sprintf( str, "0x%02X", aBuffer[i]);
        dispatchEvent( IN_LONGDATA, (const uint8_t*) str );
        
    }
    
    // Reset.
    outSysexData = CFDataCreateMutable( NULL, 0 );
    continueSysEx = false;
}
//------------------------------------------------------------------------------
void midiInputCallback(const MIDIPacketList *list, void *readProcRefCon, void *srcConnRefCon){
    //pthread_mutex_lock( &mutex );
    
    CFMutableDataRef sysexData = *(((MySysexSeqReceiveReq *)readProcRefCon)->outSysexData);
    
    // get first packet in the list
    const MIDIPacket *packet = &list->packet[0];
    
    unsigned char status;
    
    for (unsigned int i = 0; i < list->numPackets; ++i) {
        
        // Check if this is the end of a continued SysEx message
        if (continueSysEx) {
            CFDataAppendBytes( sysexData, (const UInt8 *)packet->data, packet->length );
            
            // Check if the last byte is SysEx End.
            continueSysEx = (packet->data[packet->length - 1] != 0xF7);
            
            if (!continueSysEx /*|| CFDataGetLength( sysexData ) == SYSEX_LENGTH*/) {
                // Sysex is complete.
                
                sysExComplete();
            }
            
        } else {
            // First byte should be status
            status = packet->data[0];
            if (status == 0xF0) {
                // MIDI SysEx then we copy the rest of the message into the SysEx message buffer
                CFDataAppendBytes( sysexData, (const UInt8 *)packet->data, packet->length );
                
                // Check whether the message at the end is the end of the SysEx
                continueSysEx = (packet->data[packet->length - 1] != 0xF7);
                
                if (!continueSysEx /*|| CFDataGetLength( sysexData ) == SYSEX_LENGTH*/) {
                    // Sysex is complete.
                    
                    sysExComplete();
                }
                
            } else {
                unsigned long n;
                char str[1024];
                switch (status & 0xF0) {
                    case 0x80: // Note Off
                    case 0x90: // Note On
                    case 0xA0: // Aftertouch
                    case 0xB0: // Change Control
                    case 0xC0: // Program Change
                    case 0xD0: // Channel Pressure
                    case 0xE0: // Pitch Wheel
                        // Combine the 3 bytes; status, data1, data2
                        n = packet->data[0] | (packet->data[1] << 8) | (packet->data[2] << 16);
                        sprintf( str, "dwParam1=%ld, dwParam2=0", n );
                        dispatchEvent( IN_DATA, (const uint8_t*) str );
                        break;
                        
                    default:
                        //NSLog(@"Some other message");
                        break;
                }
            }
            
        }
        packet = MIDIPacketNext( packet );
    }
    
    //pthread_mutex_unlock( &mutex );
}
//------------------------------------------------------------------------------
FREObject connectMidiIn(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    
    // default return is false
    FRENewObjectFromBool( 0, &result );
    
    // Get device index that was passed in.
    uint32_t nDeviceIdx = 0;
    FREGetObjectAsUint32( argv[0], &nDeviceIdx );
    
    if (previousInputIdx != -1) {
        // Deselect previous source
        MIDIEndpointRef src = MIDIGetSource( previousInputIdx );
        if (src != 0) {
            processOSStatus( MIDIPortDisconnectSource( midiPortRef_I, src ), (const uint8_t*)"MIDIPortDisconnectSource" );
            previousInputIdx = -1;
            
        }
    }
    previousInputIdx = nDeviceIdx;
    
    midiEndpointRef_I = MIDIGetSource( nDeviceIdx );
    if (midiEndpointRef_I){
        processOSStatus( MIDIPortConnectSource( midiPortRef_I, midiEndpointRef_I, NULL ), (const uint8_t*)"MIDIPortConnectSource" );
    }else {
        showError( (const uint8_t*)"Input port not found by index." );
    }
    
    FRENewObjectFromBool( (midiEndpointRef_I) ? 1 : 0, &result );
    return result;
}
//------------------------------------------------------------------------------
FREObject disconnectMidiIn(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    FRENewObjectFromBool( 1, &result );
    
    if (previousInputIdx != -1) {
        // Deselect previous source
        MIDIEndpointRef src = MIDIGetSource( previousInputIdx );
        if (src != 0) {
            processOSStatus( MIDIPortDisconnectSource( midiPortRef_I, src ), (const uint8_t*)"MIDIPortDisconnectSource" );
            previousInputIdx = -1;
        }
    }
    
    return result;
}
//------------------------------------------------------------------------------
FREObject connectMidiOut(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    
    // default return is false
    FRENewObjectFromBool( 0, &result );
    
    // Get device index that was passed in.
    uint32_t nDeviceIdx = 0;
    FREGetObjectAsUint32( argv[0], &nDeviceIdx );
    
    if (previousOutputIdx != -1) {
        // Deselect previous source
        midiEndpointRef_O = NULL;
        
        previousOutputIdx = -1;
    }
    previousOutputIdx = nDeviceIdx;
    
    midiEndpointRef_O = MIDIGetDestination( nDeviceIdx );
    
    if(midiEndpointRef_O){
    }else {
        showError( (const uint8_t*)"Output port not found by idx." );
    }
    
    FRENewObjectFromBool( (midiEndpointRef_O) ? 1 : 0, &result );
    return result;
}
//------------------------------------------------------------------------------
FREObject disconnectMidiOut(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    FRENewObjectFromBool( 1, &result );
    
    if (previousOutputIdx != -1) {
        // Deselect previous source
        midiEndpointRef_O = NULL;
        
        previousOutputIdx = -1;
    }
    
    return result;
}
//------------------------------------------------------------------------------
FREObject isSupported(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    FRENewObjectFromBool( isSupportedInOS, &result);
    return result;
}
//------------------------------------------------------------------------------
FREObject getInterfaceList(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    
    char str[2048];
    
    str[0] = '\0';
    
    //showMessage( (const uint8_t*) "Inside getInterfaceList." );
    
    ItemCount destCount = MIDIGetNumberOfDestinations();
    for (ItemCount i = 0 ; i < destCount ; ++i) {
        
        // Grab a reference to a destination endpoint
        MIDIEndpointRef dest = MIDIGetDestination(i);
        if (dest) {
            
            CFStringRef pname;
            char name[64];
            
            if (noErr == MIDIObjectGetStringProperty(dest, kMIDIPropertyDisplayName, &pname)){
                CFStringGetCString(pname, name, sizeof(name), 0);
                CFRelease(pname);
                
                strcat( str, "O " ); // Lines starts with 'O' for OUTPUT
                strcat( str, (const char *)name );
                strcat( str, "\n" );
            }
        }
    }
    
    // Virtual sources and destinations don't have entities
    ItemCount sourceCount = MIDIGetNumberOfSources();
    for (ItemCount i = 0 ; i < sourceCount ; ++i) {
        
        MIDIEndpointRef source = MIDIGetSource(i);
        if (source) {
            
            CFStringRef pname;
            char name[64];
            
            if (noErr == MIDIObjectGetStringProperty(source, kMIDIPropertyDisplayName, &pname)){
                CFStringGetCString(pname, name, sizeof(name), 0);
                CFRelease(pname);
                
                strcat( str, "I " ); // Lines starts with 'I' for INPUT
                strcat( str, (const char *)name );
                strcat( str, "\n" );
            }
        }
    }
    
    // Stuff the string into the adobe object.
    FRENewObjectFromUTF8(strlen(str)+1, (const uint8_t *)str, &result);
    
    return result;
}
//------------------------------------------------------------------------------
void sysexCompletionProc(MIDISysexSendRequest* request){
    free(request);
}
//------------------------------------------------------------------------------
FREObject transmitSysEx(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    OSStatus status;
    
    char str[1024];
    str[0] = '\0';
    
    // Pull out the AS3 array.
    FREObject arr = argv[0]; // array
    
    uint32_t arr_len; // array length
    FREGetArrayLength(arr, &arr_len);
    
    // create a C array for us to shove the values into
    Byte sysEx[SYSEX_SIZE];
    
    // Loop over the AS3 array and shove the values into our C array that is used as the buffer.
    int32_t i = 0;
    for(i=0; i<arr_len; i++){
        // get an element at index
        FREObject element;
        FREGetArrayElementAt(arr, i, &element);
        
        // get an int value out of the element
        uint32_t value;
        FREGetObjectAsUint32(element, &value);
        
        // stuff it into our sysex array
        sysEx[i] = value;
    }
    
    struct MIDISysexSendRequest *sysexReq = (MIDISysexSendRequest*)malloc(sizeof(struct MIDISysexSendRequest) + arr_len);
    
    sysexReq->destination = midiEndpointRef_O;
    // Copy the data into a malloc buffer so that it is not volatile.
    sysexReq->data = ((unsigned char *)sysexReq) + sizeof(struct MIDISysexSendRequest);
    memcpy((void *)sysexReq->data, sysEx, arr_len);
    sysexReq->bytesToSend = arr_len;
    sysexReq->complete = false;
    sysexReq->completionProc = sysexCompletionProc;
    sysexReq->completionRefCon = NULL;
    status = MIDISendSysex( sysexReq );
    
    processOSStatus( status, (const uint8_t*)"Sending SysEx" );
    
    FRENewObjectFromBool( status == noErr, &result);
    
    return result;
}
//------------------------------------------------------------------------------
FREObject transmitMessage(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]){
    FREObject result;
    OSStatus status;
    uint32_t dwMsg = 0;
    
    FREGetObjectAsUint32( argv[0], &dwMsg );
    
    Byte buffer[128];
    MIDIPacketList *packetlist = (MIDIPacketList *)buffer;
    MIDIPacket *currentpacket = MIDIPacketListInit(packetlist);
    int messageSize = 3; //Note On is a three-byte message
    Byte msg[3] = {dwMsg & 0xFF, (dwMsg >> 8) & 0xFF, (dwMsg >> 16) & 0xFF}; //{0x99, 80, 120} status, note num, velocity;
    MIDITimeStamp timestamp = 0;
    currentpacket = MIDIPacketListAdd(packetlist, sizeof(buffer), currentpacket, timestamp, messageSize, msg);
    status = MIDISend(midiPortRef_O, midiEndpointRef_O, packetlist);	
    
    processOSStatus( status, (const uint8_t*)"transmitMessage" );
    
    FRENewObjectFromBool( status == noErr, &result);
    
    return result;
}
//------------------------------------------------------------------------------


/*FREObject hello(FREContext ctx, void *data, uint32_t argc, FREObject argv[]) {
    FREObject result;
    const char *str = "HELLO";
    FRENewObjectFromUTF8((uint32_t)strlen(str)+1, (const uint8_t *)str, &result);
    return result;
}*/

FRENamedFunction aneFunctions[] = {
    //{ (const uint8_t*) "hello", 0, hello },
    { (const uint8_t*) "getInterfaceList", 0, getInterfaceList },
    { (const uint8_t*) "isSupported", 0, isSupported },
    { (const uint8_t*) "connectMidiIn", 0, connectMidiIn },
    { (const uint8_t*) "connectMidiOut", 0, connectMidiOut },
    { (const uint8_t*) "disconnectMidiIn", 0, disconnectMidiIn },
    { (const uint8_t*) "disconnectMidiOut", 0, disconnectMidiOut },
    { (const uint8_t*) "transmitSysEx", 0, transmitSysEx },
    { (const uint8_t*) "transmitMessage", 0, transmitMessage }
};

void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions) {
    *numFunctions = sizeof( aneFunctions ) / sizeof( FRENamedFunction );
    *functions = aneFunctions;
    
    dllContext = ctx;
    
    OSStatus status;
    
    // Create the client.
    //showMessage( (const uint8_t*) "Creating midi client." );
    
    
    status = MIDIClientCreate( CFSTR("MIDIClient"), NULL, NULL, &midiClientRef );
    if(status != noErr){
        processOSStatus( status, (const uint8_t*)"MIDIClientCreate" );
        return;
    }
    
    // Thread safe stuff
    // Create mutex before starting receive port.
    //pthread_mutex_init( &mutex, NULL );
    outSysexData = CFDataCreateMutable( NULL, 0 );
    
    MySysexSeqReceiveReq *_seqReq = &seqReq;
    
    _seqReq->stopped = false;
    _seqReq->outSysexData = &outSysexData;
    
    // Create an input port
    status = MIDIInputPortCreate( midiClientRef, CFSTR("Input"), midiInputCallback, _seqReq, &midiPortRef_I );
    if (status != noErr) {
        processOSStatus( status, (const uint8_t*)"MIDIInputPortCreate" );
        return;
    }
    
    // Create an output port
    status = MIDIOutputPortCreate( midiClientRef, CFSTR("Output"), &midiPortRef_O );
    if (status != noErr) {
        processOSStatus( status, (const uint8_t*)"MIDIOutputPortCreate" );
        return;
    }
 }

void contextFinalizer(FREContext ctx) {
    return;
}

void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer) {
    *ctxInitializer = &contextInitializer;
    *ctxFinalizer = &contextFinalizer;
}


void finalizer(void* extData) {
    FREContext nullCTX;
    nullCTX = 0;
    
    
    if (midiEndpointRef_I) {
        showMessage( (const uint8_t*) "Disposing of input." );
        processOSStatus( MIDIPortDisconnectSource( midiPortRef_I, midiEndpointRef_I ), (const uint8_t*)"MIDIPortDisconnectSource" );
        processOSStatus( MIDIEndpointDispose(      midiEndpointRef_I ),                (const uint8_t*)"MIDIEndpointDispose" );
        processOSStatus( MIDIPortDispose(          midiPortRef_I ),                    (const uint8_t*)"MIDIPortDispose" );
        
        midiEndpointRef_I = NULL;
        midiPortRef_I     = NULL;
        
        // Destroy mutex after stopping receive port.
        //pthread_mutex_destroy( &mutex );
        CFRelease( outSysexData );
    }
    
    if (midiEndpointRef_O) {
        showMessage( (const uint8_t*) "Disposing of output." );
        processOSStatus( MIDIEndpointDispose( midiEndpointRef_O ), (const uint8_t*)"MIDIEndpointDispose" );
        processOSStatus( MIDIPortDispose(     midiPortRef_O ),     (const uint8_t*)"MIDIPortDispose" );
        
        midiEndpointRef_O = NULL;
        midiPortRef_O     = NULL;
    }
    
    if(midiClientRef){
        showMessage( (const uint8_t*) "Disposing of midi client." );
        processOSStatus( MIDIClientDispose( midiClientRef ), (const uint8_t*)"MIDIClientDispose" );
        midiClientRef = NULL;
    }else{
        showMessage( (const uint8_t*) "Could not dispose of midi client." );
    }
    
    
    contextFinalizer(nullCTX);
    
    return;
}
