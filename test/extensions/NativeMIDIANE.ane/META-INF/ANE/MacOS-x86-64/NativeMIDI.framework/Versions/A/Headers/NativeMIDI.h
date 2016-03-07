
#include "FlashRuntimeExtensions.h"
#include <CoreMIDI/MIDIServices.h>
#ifdef _WIN32
#define aneExport __declspec(dllexport)
#elif __APPLE__
#define aneExport __attribute__((visibility("default")))
#endif
extern "C" {

//FREObject hello(FREContext ctx, void *data, uint32_t argc, FREObject argv[]);

void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions);
void contextFinalizer(FREContext ctx);
aneExport void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer);
aneExport void finalizer(void* extData);
    
/* This encapsulates the receive request for a seqence of sysex messages.
 * Strictly C and Core Foundation.
 */
typedef struct _MySysexSeqReceiveReq {
    MIDIEndpointRef endpointRef;
    MIDIPortRef portRef;
    CFMutableDataRef *outSysexData;
    bool stopped;
} MySysexSeqReceiveReq;

}
