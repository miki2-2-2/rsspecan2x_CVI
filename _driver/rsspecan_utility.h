/*****************************************************************************
 *- Utility Function Declarations (Non-Exported) ----------------------------*
 *****************************************************************************/

#include <visa.h>

ViStatus rsspecan_InitAttributes (ViSession vi);
ViStatus rsspecan_DefaultInstrSetup (ViSession openInstrSession);
ViStatus rsspecan_CheckStatus (ViSession vi);
ViStatus rsspecan_WaitForOPC (ViSession vi, ViUInt32 maxTime);
ViInt32 rsspecan_StringIndex(ViString array[], ViChar buff[]); 
ViStatus rsspecan_readDataBlock (ViSession instrSession,
                                  /*@out@*/ViChar **outputBuffer, /*@out@*/ViUInt32 *byteCount);
ViBoolean rsspecan_invalidViInt32Range (ViInt32 val, ViInt32 min, ViInt32 max);
ViBoolean rsspecan_invalidViUInt32Range (ViUInt32 val, ViUInt32 min, ViUInt32 max);
ViBoolean rsspecan_invalidViReal64Range (ViReal64 val, ViReal64 min, ViReal64 max);
ViBoolean rsspecan_invalidViBooleanRange (ViBoolean val);
//ViStatus rsspecan_ReadDataUnknownLength(ViSession instrSession, /*@out@*/ViChar **outBuffer, /*@out@*/ViUInt32 *count);
ViStatus rsspecan_ParseCatalog(ViChar *buffer, ViInt32 size, ViChar *catalogList, ViInt32 *nbValues);
ViStatus rsspecan_ParseLimitLineCatalog(ViChar *buffer, ViInt32 size, ViChar *catalogList, ViInt32 *nbValues);
void rsspecan_SWAP_32(unsigned char *data, long points);  
ViStatus rsspecan_dataReadTrace (ViSession instrSession,
                                 ViInt32 window,
                                 ViString trace, 
                                 ViInt32 arrayLength, 
                                 /*@out@*/ViReal64 _VI_FAR traceData[],
                                 ViPInt32 noofPoints);
ViStatus rsspecan_dataReadTraceOnly (ViSession instrSession,
                                 ViInt32 arrayLength, 
                                 /*@out@*/ViReal64 _VI_FAR traceData[],
                                 ViPInt32 noofPoints);
ViStatus rsspecan_dataAsciiReadTrace (ViSession instrSession,
                                 ViInt32 window,
                                 ViString trace, 
                                 ViInt32 arrayLength, 
                                 /*@out@*/ViReal64 _VI_FAR traceData[],
                                 ViPInt32 noofPoints);
ViStatus _VI_FUNC rsspecan_dataReadTraceIQ (ViSession   instrSession,
                                            ViInt32     bufferSize,
                                            ViPInt32    noofPoints,
                                            /*@out@*/ViReal64 _VI_FAR realPartsI[],
                                            /*@out@*/ViReal64 _VI_FAR imaginaryPartsQ[]);                                 
ViStatus rsspecan_SetOPCTimeout(ViSession   instrSession, ViInt32 timeout);
ViStatus rsspecan_GetOPCTimeout(ViSession   instrSession, ViInt32 *timeout);
ViStatus rsspecan_CheckBeforeRead (ViSession instrSession, ViUInt32 maxTime);
ViStatus rsspecan_ClearBeforeRead (ViSession instrSession);
ViStatus rsspecan_WaitForOPCCallback (ViSession io);
ViStatus rsspecan_GetAttributeRepCapName (ViSession instrSession,
                                ViUInt32 attributeID,
                                ViString repCapNameId,
                                ViInt32 value,
                                ViInt32 bufferSize,
                                ViChar _VI_FAR repCapName[]);
ViBoolean rsspecan_IsFSV (ViSession instrSession);
