/*****************************************************************************
 *------------------------ Utility  -----------------------------------------*
 *****************************************************************************/
#include <float.h>
#include <math.h>
#include "rsspecan.h"

/*****************************************************************************
 *- Callback Declarations ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- Common Callback Declarations --------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_InitAttributes
 * Purpose:  This function inits attributes to the desired values if needed.
 *****************************************************************************/
ViStatus rsspecan_InitAttributes(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	// Instrument Identification
	//Parameter idnModelFullName determines RS_ATTR_INSTRUMENT_MODEL value:
	// VI_FALSE: RS_ATTR_INSTRUMENT_MODEL = "RTO"
	// VI_TRUE: RS_ATTR_INSTRUMENT_MODEL = "RTO2044"
	// This is important for CheckInstrumentModel() function used in all attributes and some hi-level functions
	checkErr(RsCore_QueryAndParseIDNstring(instrSession, RSSPECAN_ATTR_ID_QUERY_RESPONSE, RSSPECAN_SIMULATION_ID_QUERY, VI_FALSE, NULL));

	// Query OPT string, parse the options, remove the duplicates,
	// sort them and store the result string to RS_ATTR_OPTIONS_LIST
	checkErr(RsCore_QueryAndParseOPTstring(instrSession, RSSPECAN_SIMULATION_OPT_QUERY, RS_VAL_OPTIONS_PARSING_KEEP_AFTER_DASH));

	// - Class Driver Identification
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_CLASS_DRIVER_DESCRIPTION, 0, RSSPECAN_CLASS_DRIVER_DESCRIPTION));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_CLASS_DRIVER_PREFIX, 0, RSSPECAN_CLASS_DRIVER_PREFIX));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_CLASS_DRIVER_VENDOR, 0, RSSPECAN_CLASS_DRIVER_VENDOR));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_CLASS_DRIVER_REVISION, 0, RSSPECAN_CLASS_DRIVER_REVISION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION, 0, RSSPECAN_CLASS_SPEC_MAJOR_VERSION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION, 0, RSSPECAN_CLASS_SPEC_MINOR_VERSION));

	// - Driver Identification
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_DESCRIPTION, 0, RSSPECAN_SPECIFIC_DRIVER_DESCRIPTION));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_PREFIX, 0, RSSPECAN_SPECIFIC_DRIVER_PREFIX));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_LOCATOR, 0, RSSPECAN_SPECIFIC_DRIVER_LOCATOR));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_VENDOR, 0, RSSPECAN_SPECIFIC_DRIVER_VENDOR));

	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION, 0, RSSPECAN_CLASS_SPEC_MAJOR_VERSION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION, 0, RSSPECAN_CLASS_SPEC_MINOR_VERSION));

	// - Driver Capabilities
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_SUPPORTED_INSTRUMENT_MODELS, 0, RSSPECAN_SUPPORTED_INSTRUMENT_MODELS));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_GROUP_CAPABILITIES, 0, RSSPECAN_GROUP_CAPABILITIES));
	checkErr(RsCore_SetAttributeViString(instrSession, "", RS_ATTR_FUNCTION_CAPABILITIES, 0, RSSPECAN_FUNCTION_CAPABILITIES));

	// - Version Info
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION, 0, RSSPECAN_MAJOR_VERSION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_MINOR_VERSION, 0, RSSPECAN_MINOR_VERSION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_SPECIFIC_DRIVER_MINOR_MINOR_VERSION, 0, RSSPECAN_MINOR_MINOR_VERSION));

	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_CLASS_DRIVER_MAJOR_VERSION, 0, RSSPECAN_CLASS_SPEC_MAJOR_VERSION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, "", RS_ATTR_CLASS_DRIVER_MINOR_VERSION, 0, RSSPECAN_CLASS_SPEC_MINOR_VERSION));

	checkErr(RsCore_SetSpecificDriverRevision(instrSession, RS_ATTR_SPECIFIC_DRIVER_REVISION));

Error:
	return error;
}

/*****************************************************************************
 * Function: rsspecan_DefaultInstrSetup
 * Purpose:  This function sends a default setup to the instrument. The
 *           rsspecan_reset function calls this function. The
 *           rsspecan_RsInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter. This function is
 *           useful for configuring settings that other instrument driver
 *           functions require.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
ViStatus rsspecan_DefaultInstrSetup(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	// Set all the attributes to the default state. Do not update inherent attributes!
	checkErr(RsCore_ApplyAttributeDefaults(instrSession, VI_FALSE));

	// Init attributes
	checkErr(rsspecan_InitAttributes(instrSession));

	rsSession->fastSweepInstrument = 0;
	checkErr(RsCore_ResetRegistersEseSre(instrSession));

Error:
	return error;
}

/*****************************************************************************
 * Function: rsspecan_CheckStatus
 * Purpose:  This function checks the status of the instrument to detect
 *           whether the instrument has encountered an error. This function
 *           is called at the end of most exported driver functions. If the
 *           instrument reports an error, this function returns
 *           RS_ERROR_INSTRUMENT_SPECIFIC. The user can set the
 *           rsspecan_ATTR_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE to disable this
 *           check and increase execution speed.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
ViStatus rsspecan_CheckStatus(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_QueryInstrStatus(instrSession) && !RsCore_Simulating(instrSession))
	{
		checkErr(RsCore_CheckStatus(instrSession, VI_SUCCESS));
	}

Error:
	return error;
}

/*===========================================================================*/
/* Function: Parse Limit Line Catalog                                        */
/* Purpose:  Parse Limit Line Catalog returned from instrument and removes   */
/*           from it all unneeded parameters from this list.                 */
/*===========================================================================*/
ViStatus rsspecan_ParseLimitLineCatalog(ViChar* buffer, ViInt32 size, ViChar* catalogList, ViInt32* nbValues)
{
	ViStatus rsspecan_status = VI_SUCCESS;
	ViChar* plist_of_files = 0;
	ViChar* ptag = NULL;
	ViChar* pend = NULL;
	ViInt32 i = 0;

	if (*buffer != '\0')
	{
		sscanf(buffer, "%*ld,%*ld,%[^\r\n]", buffer);
		plist_of_files = (ViChar*)malloc(strlen(buffer) + 1);
		pend = plist_of_files;
		*pend = '\0';
		ptag = strtok(buffer, ",");
		while (ptag != NULL && rsspecan_status == VI_SUCCESS)
		{
			i++;
			pend += sprintf(pend, "%s,", ptag + 1);
			ptag = strtok(NULL, ",");
			if (ptag == NULL)
				rsspecan_status = RS_ERROR_INVALID_VALUE;
			else
				ptag = strtok(NULL, ",");
		}
	}

	*--pend = '\0';

	if (nbValues) *nbValues = i;

	strncpy(catalogList, plist_of_files, size);

	free(plist_of_files);

	return rsspecan_status;
}

/*===========================================================================*/
/* Function: Set OPC Timeout                                                 */
/* Purpose:  Sets new OPC timeout                                            */
/*===========================================================================*/
ViStatus rsspecan_SetOPCTimeout(ViSession instrSession, ViInt32 timeout)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, timeout));

Error:
	return error;
}

/*===========================================================================*/
/* Function: Get OPC Timeout                                                 */
/* Purpose:  Gets new OPC timeout                                            */
/*===========================================================================*/
ViStatus rsspecan_GetOPCTimeout(ViSession instrSession, ViInt32* timeout)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, timeout));

Error:
	return error;
}

/// HIFN Returns true for all FSV compatible instruments
/// HIRET Returns true for all FSV compatible instruments
/// HIPAR instrSession/Session.
ViBoolean rsspecan_IsFSV(ViSession instrSession)
{
	return RsCore_IsInstrumentModel(instrSession, "FSV|FSW|FSVR");
}

/*===========================================================================*/
/* Function: Read Trace Data                                                 */
/* Purpose:  This function reads out trace data from the instrument.         */
/*           It works with ASCII or binary float data                        */
/*===========================================================================*/
ViStatus rsspecan_dataReadTrace(ViSession instrSession, ViInt32 window, ViString trace, ViInt32 arrayLength, ViReal64 traceData[], ViPInt32 noofPoints)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* responseArray = NULL;

	checkErr(rsspecan_dataReadTraceDynSize(instrSession, window, trace, &responseArray, noofPoints));
	checkErr(RsCore_CopyToUserBufferViReal64Array(instrSession, traceData, arrayLength, responseArray, *noofPoints));

Error:
	if (responseArray) free(responseArray);
	return error;
}

/*===========================================================================*/
/* Function: Read Trace Data Dynamic size                                    */
/* Purpose:  This function reads out trace data from the instrument.         */
/*           It works with ASCII or binary float data                        */
/*===========================================================================*/
ViStatus rsspecan_dataReadTraceDynSize(ViSession instrSession, ViInt32 window, ViString trace, ViReal64** traceData, ViInt32* noofPoints)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_Write(instrSession, ":FORM REAL,32"));

	if (window == 0)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":TRAC? %s", trace);
	}
	else
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":TRAC%ld? %s", window, trace);
	}

	checkErr(RsCore_QueryFloatArray(instrSession, cmd, traceData, noofPoints));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	return error;
}
