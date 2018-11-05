/*****************************************************************************
 *- Utility Function Declarations (Non-Exported) ----------------------------*
 *****************************************************************************/

#include <visatype.h>

ViStatus rsspecan_InitAttributes (ViSession vi);
ViStatus rsspecan_DefaultInstrSetup (ViSession openInstrSession);
ViStatus rsspecan_CheckStatus (ViSession vi);
ViStatus rsspecan_ParseLimitLineCatalog(ViChar *buffer, ViInt32 size, ViChar *catalogList, ViInt32 *nbValues);
ViStatus rsspecan_SetOPCTimeout(ViSession   instrSession, ViInt32 timeout);
ViStatus rsspecan_GetOPCTimeout(ViSession   instrSession, ViInt32 *timeout);
ViBoolean rsspecan_IsFSV (ViSession instrSession);
