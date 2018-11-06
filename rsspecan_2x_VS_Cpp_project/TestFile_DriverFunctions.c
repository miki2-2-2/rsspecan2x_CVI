//==============================================================================
// Include files

#include "rsspecan.h"
#include "TestFile_DriverFunctions.h"
#include "TestFile_Utilities.h"
#include <time.h>
#include "rscore.h"

//==============================================================================
// Types
#pragma warning (disable : 4101)
ViStatus Test_DriverFunctions(ViString resourceName, ViString optionsString)
{

	ViStatus error = VI_SUCCESS;
	ViSession io = 0;
	ViInt32	*dynInt32Array = NULL;
	ViReal64 doubleValue;
	ViInt32 integerValue;
	ViBoolean booleanValue;
	ViInt32 arraySize, opc;
	ViChar	stringValue[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar	filePath[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar message[2 * RS_MAX_MESSAGE_BUF_SIZE];
	ViChar randomString[2 * RS_MAX_MESSAGE_BUF_SIZE];
	double      elapsed_time_ms;
	double      ms_divider = (double)CLOCKS_PER_SEC / 1000.0;
	clock_t     start_time = clock();
	ViReal64	*dynArray = NULL, measFreq, measAmpl, measHigh;
	ViReal64	start, increment;
	ViInt32		points, samplePerPoint;
	ViReal64	*waveform = NULL;
	int repetition;
	ViUInt32 visaToutMs;
	RsCoreSessionPtr rsSession = NULL;

	InfoMessage("\n------ Test_DriverFunctions --------\n");
	snprintf(stringValue, RS_MAX_MESSAGE_BUF_SIZE, "ResourceName: '%s' optstring: '%s'\n", resourceName, optionsString);
	InfoMessage(stringValue);

	GenerateRandomString(randomString, 2 * RS_MAX_MESSAGE_BUF_SIZE);

	ClearTestErrors();
	printf("      Initializing instrument '%s' ... ", resourceName);
	checkErr(rsspecan_InitWithOptions(resourceName, VI_FALSE, VI_TRUE, optionsString, &io));
	printf("success.\n\n");
	//rsSession->ioSegmentSize = 4096;
	checkErr(RsCore_GetRsSession(io, &rsSession));

	checkErr(rsspecan_ConfigureAutoSystemErrQuery(io, VI_TRUE));

	printf("Enabling remote display update ... ");
	checkErr(rsspecan_ConfigureDisplayUpdate(io, VI_TRUE));
	printf("success.\n\n");

	printf("Reading instrument *IDN? string ... ");
	checkErr(rsspecan_IDQueryResponse(io, RS_MAX_MESSAGE_BUF_SIZE, stringValue));
	printf("success.\n\n");
	printf("Connected instrument: %s\n\n", stringValue);

	checkErr(rsspecan_ConfigureErrorChecking(io, VI_FALSE, VI_TRUE, VI_TRUE));
	checkErr(rsspecan_SetVISATimeout(io, 1000));
	
	printf("Closing the session to instrument '%s' ... ", resourceName);
	checkErr(rsspecan_close(io));
	printf("success.\n\n");

Error:
	DisplayResult(io, error);
	return error;
}