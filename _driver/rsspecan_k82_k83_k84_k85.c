/*****************************************************************************
 *- RSSpecAn K82 K83 K84 K85 - CDMA -----------------------------------------*
 *****************************************************************************/

#include "rsspecan.h"

#pragma warning(disable:4100) //unreferenced formal parameter

static ViString traceC2KArr[] = {"CTAB",NULL};
static ViString analyzerResultSummaryArr[] = {
	"SLOT", "PTOT", "PPIC", "RHO", "MACC",
	"PCD", "IQIM", "IQOF", "FERR", "FERP",
	"CERR", "TFR", "ACT", "SRAT", "CHAN",
	"SFAC", "TOFF", "POFF", "CDPR", "CDP",
	"EVMR", "EVMP", "RHOP", "RHO1", "RHO2",
	"PPIL", "PMAC", "PDAT", "PPR", "DMTY",
	"MACT", "DACT", "PLEN", "PDM", "PDM",
	"IPMM", "MTYP", "PRRI", "DRP", "RHO",
	"RHOD", "RHOM",
	"PNF", "PAV", "TAU", "TACT", "EVMP",
	"CDEP", "CDER", "CODM", "CODP",
	NULL
};

/*****************************************************************************
 * Function: Adjust C2K Settings
 * Purpose:  This function adjusts the frequency span, the measurement
 *           bandwidths and the detector as required for the number of
 *           channels, the channel bandwidths and the channel spacings selected
 *           in the active power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustC2KSettings(ViSession instrSession,
                                             ViInt32 window,
                                             ViInt32 adjustSettings,
                                             ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 4, "Timeout");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));
	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_C2K_ACH_PRES, adjustSettings));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Adjust C2K Reference Level
 * Purpose:  This function adapts the reference level to the measured channel
 *           power and - if required - switches on previously the adjacent
 *           channel power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustC2KReferenceLevel(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_C2K_ADJUST_REF_LEVEL, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Select C2K Measurement
 * Purpose:  This function selects the CDMA2000 Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectC2KMeasurement(
	ViSession instrSession,
	ViInt32 measurementSelect
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_MEAS_SELECT, measurementSelect));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K PCG Settings
 * Purpose:  This function sets the PCG Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KPCGSettings(ViSession instrSession,
                                                   ViInt32 setCount,
                                                   ViInt32 setToAnalyze)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSQ"));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_SET_COUNT, setCount),
			2, "Set Count");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_SET_TO_ANALYZE, setToAnalyze),
			3, "Set To Analyze");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Restore C2K SEM Standard Lines
 * Purpose:  This function restores the standard limit lines for the spectrum
 *           emission mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreC2KSEMStandardLines(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_LIM_RESTORE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K Signal Statistics Measurement
 * Purpose:  This function configures parameters of signal statistic
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KSignalStatisticMeasurement(ViSession instrSession,
                                                                  ViInt32 measurement,
                                                                  ViInt32 numberofSamples)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 2),
			2, "Measurement");

	switch (measurement)
	{
	case RSSPECAN_VAL_SSTAT_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_APD_STATE, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_FALSE));
		break;
	case RSSPECAN_VAL_SSTAT_APD:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_APD_STATE, VI_TRUE));
		break;
	case RSSPECAN_VAL_SSTAT_CCDF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_TRUE));
		break;
	}
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_SAMPLES, numberofSamples),
			3, "Number Of Samples");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K Signal Statistics Percent Marker
 * Purpose:  This function configures the percent marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KSignalStatisticsPercentMarker(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 marker,
	ViReal64 percentMarker
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);

	checkErr(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_C2K_PERCENT_MARKER, percentMarker));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure C2K Channel Table Operations
/* Purpose:  This function performs the selected operation with channel
/*           table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureC2KChannelTableOperations(
	ViSession instrSession,
	ViInt32 operation,
	ViString channelTable)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, operation, 0, 4),
			2, "Operation");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, channelTable), 3, "Channel Table");

	switch (operation)
	{
	case RSSPECAN_VAL_TABLE_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_STATE, VI_FALSE));
		break;
	case RSSPECAN_VAL_TABLE_SELECT:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_STATE, VI_TRUE));
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_SELECT, channelTable),
				3, "Channel Table");
		break;
	case RSSPECAN_VAL_TABLE_NEW:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_NAME, channelTable),
				3, "Channel Table");
		break;
	case RSSPECAN_VAL_TABLE_DELETE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_DELETE, NULL));
		break;
	case RSSPECAN_VAL_TABLE_COPY:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_COPY, channelTable),
				3, "Channel Table");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, operation), 2, "Operation");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Restore C2K Channel Table
 * Purpose:  This function restores the predefined tables. They are restored
 *           to the values they had when the device was delivered. With this
 *           restore feature an undo of overwriting these channel tables is
 *           possible.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreC2KChannelTable(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_RESTORE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure CDMA2000 Channel Table Comment
 * Purpose:  This function defines a comment for the selected channel table in
 *           the code domain power measurement mode.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure C2K Channel Table Operations
 *           (ConfigureC2KChannelTableOperations) and the values of the table
 *           have to be defined with one of the functions Configure C2K BST
 *           Channel Table Data (ConfigureBC2KChannelTableData), Configure
 *           C2K MST Channel Table Data (ConfigureM2KChannelTableData),
 *           Configure EV-DO BST Channel Table Data
 *           (ConfigureBDOChannelTableData), or Configure EV-DO MST Channel
 *           Table Data (ConfigureMDOChannelTableData).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KChannelTableComment(
	ViSession instrSession,
	ViString channelTableComment
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_COMMENT, channelTableComment),
			2, "Channel Table Comment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query CDMA2000 Channel Table Catalog
 * Purpose:  This function reads out the names of all channel tables stored on
 *           the harddisk in the code domain power measurement mode.
 *
 *           Notes:
 *
 *           (1) Driver returns an instrument specific error, when catalog
 *           list is empty.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryC2KChannelTableCatalog(
	ViSession instrSession,
	ViInt32* numberofChannelTables,
	ViInt32 bufferSize,
	ViChar channelTablesList[]
)
{
	ViStatus error = VI_SUCCESS;
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K82|K83|K84|K85"));

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
			3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, channelTablesList), 4, "Channel Table List");

	checkErr(RsCore_QueryCatalog(instrSession, "CONF:CDP:CTAB:CAT?", bufferSize, channelTablesList, numberofChannelTables));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
		(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K Basic Settings
 * Purpose:  This function configures the basic settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KBasicSettings(
	ViSession instrSession,
	ViInt32 bandClass,
	ViBoolean timePhase,
	ViReal64 inactChannelThreshold,
	ViBoolean invertQ,
	ViInt32 sideBand,
	ViBoolean normalize
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_BCLASS, bandClass),
			2, "Band Class");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_CDP_TPMEAS, timePhase),
			3, "Time Phase");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_CDP_ICTHRES, inactChannelThreshold),
			4, "Inact Channel Threshold");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_CDP_QINVERT, invertQ),
			5, "Invert Q");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_SBAND, sideBand),
			6, "Side Band");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_CDP_NORMALIZE, normalize),
			7, "Normalize");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines how the pilot channel power is displayed in the Result Summary.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR mode/This control selects the pilot channel display mode.
ViStatus _VI_FUNC rsspecan_ConfigureC2KPowerDisplay(ViSession instrSession,
                                                    ViInt32 mode)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_DISPLAY, mode),
			2, "Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure CDMA2000 Amplitude Y Scale Division
 * Purpose:  This function defines the scaling of the Y axis in the current
 *           unit.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KAmplitudeYScaleDivision(
	ViSession instrSession,
	ViReal64 yScaleDiv
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_PDIV, yScaleDiv),
			2, "Y Scale Div");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Adjust CDMA2000 Amplitude Reference Level
 * Purpose:  This function adjusts reference level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustC2KAmplitudeReferenceLevel(ViSession instrSession,
                                                            ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_CDP_LEVEL_ADJUST, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read C2K Trace Data
 * Purpose:  This function reads trace data out of the instrument. Only TRACE1
 *           or TRACE2 can be queried depending on the display mode.
 *
 *           Notes:
 *
 *           (1) This function is available for option FS-K82, FS-K83, FS-K84
 *           or FS-K85 only.
 *
 *           (2) The associated measurement window is selected with the
 *           function Display Set Active Window (rsspecan_DisplaySetActiveWindow).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadC2KTraceData(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 sourceTrace,
	ViInt32 arrayLength,
	ViInt32* actualPoints,
	ViReal64 values[]
)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K82|K83|K84|K85"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, sourceTrace, 1, 2),
			3, "Source Trace");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, actualPoints), 5, "Actual Points");

	sprintf(traceName, "TRACE%ld", sourceTrace);
	checkErr(rsspecan_dataReadTrace(instrSession, window, traceName, arrayLength, values, actualPoints));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
		(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read C2K CDP Trace Data
 * Purpose:  This function reads trace data out of the instrument. Data can be
 *           queried depending on the display mode.
 *
 *           (1) CTABLE:
 *           In addition to the results of the channel table which are output
 *           using the :TRACe[:DATA]? TRACE1 command, active timing and phase
 *           offset measurement (refer to CDPower:TPMeas) also has the CTABle
 *           query command which displays the maximum values of the TIMING and
 *           PHASE OFFSET together with the associated channel.
 *
 *           Option FS-K82, FS-K83 - the following values are output:
 *
 *           <Max. time offset in s>, <Code number for max. time>, <Code
 *           class for max. time>, <Max. phase offset in rad>, <Code number
 *           for max. phase>, <Code class for max. phase>, <Reserved 1>, ...,
 *           <Reserved 6>
 *
 *           Option FS-K84 - the following values are output:
 *
 *           <Max. time offset in s>, <Channel Type>, <Code number for max.
 *           time>, <Code class for max. time>, <Max. phase offset in rad>,
 *           <Channe type>, <Code number for max. phase>, <Code class for max.
 *           phase>, <Reserved 1>, ..., <Reserved 4>
 *
 *           Option FS-K85 - the following values are output:
 *
 *           <Max. time offset in s>, <Code number for max. time>, <Code class
 *           for max. time>, <Max. phase offset in rad>, <Code number for max.
 *           phase>, <Code class for max. phase>, <Reserved 1>, ..., <Reserved
 *           6>
 *
 *           Notes:
 *
 *           (1) This function is available for option FS-K82, FS-K83, FS-K84
 *           or FS-K85 only.
 *
 *           (2) The associated measurement window is selected with the
 *           function Display Set Active Window (rsspecan_DisplaySetActiveWindow).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadC2KCDPTraceData(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 sourceTrace,
	ViInt32 arrayLength,
	ViInt32* actualPoints,
	ViReal64 values[]
)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K82|K83|K84|K85"));

	if (sourceTrace != RSSPECAN_VAL_C2K_CTABLE)
	viCheckParm(RS_ERROR_INVALID_PARAMETER, 3, "Source Trace");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, actualPoints), 5, "Actual Points");

	sprintf(traceName, "%s", traceC2KArr[sourceTrace]);
	checkErr(rsspecan_dataReadTrace(instrSession, window, traceName, arrayLength, values, actualPoints));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: C2K BST Mode
 * Purpose:  This function selects the CDMA2000 Base Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BC2KMode(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BTS SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KSEM(
	ViSession instrSession,
	ViInt32 limitLineType,
	ViReal64 limitLineValue,
	ViReal64 _30kHz1MHzTransition
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_LIM_MODE, limitLineType),
			2, "Limit Line Type");

	if (limitLineType == RSSPECAN_VAL_C2K_LIM_MODE_MAN)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_LIM_VAL, limitLineValue),
				3, "Limit Line Value");
	}

	if (!rsspecan_IsFSV(instrSession))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS, _30kHz1MHzTransition),
				4, "30kHz/1MHz Transition");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Code Domain Analyzer Results
 * Purpose:  This function selects the measured data that will be displayed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KCodeDomainAnalyzerResults(ViSession instrSession,
                                                                  ViInt32 resultType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (resultType)
	{
	case RSSPECAN_VAL_C2K_FEED_CDP:
	case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
	case RSSPECAN_VAL_C2K_FEED_CDEP:
	case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
	case RSSPECAN_VAL_C2K_FEED_CPOW:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	case RSSPECAN_VAL_C2K_FEED_MACC:
	case RSSPECAN_VAL_C2K_FEED_PVSL:
	case RSSPECAN_VAL_C2K_FEED_PVSY:
	case RSSPECAN_VAL_C2K_FEED_BSTR:
	case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
	case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
	case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
	case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
	case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 2, "Result Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KCodeDomainAnalyzerSettings(
	ViSession instrSession,
	ViInt32 captureLength,
	ViInt32 selectChannel,
	ViInt32 selectPCG
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (!RsCore_IsInstrumentModel(instrSession, "FSP")) // not FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSQ_FSU, captureLength),
				2, "Capture Length");
	}
	else // is FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSP, captureLength),
				2, "Capture Length");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT, selectPCG),
			4, "Select PCG");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_CODE, selectChannel),
			3, "Select Channel");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KChannelTableData(
	ViSession instrSession,
	ViInt32 channelType[],
	ViInt32 codeClass[],
	ViInt32 codeNumber[],
	ViInt32 radioConfiguration[],
	ViInt32 reserved1[],
	ViInt32 reserved2[],
	ViBoolean status[],
	ViReal64 cdpRelative[],
	ViInt32 numofElements
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuffer = cmd;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K82"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, numofElements, 1, INT_MAX),
			10, "Num of Elements");
	pbuffer += sprintf(pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,0,0,%d,%.12f",
	                   channelType[i], codeClass[i], codeNumber[i], radioConfiguration[i], status[i] == VI_FALSE ? 0 : 1,
	                   cdpRelative[i]);

	for (i = 1; i < numofElements; i++)
		pbuffer += sprintf(pbuffer, ",%ld,%ld,%ld,%ld,0,0,%d,%.12f",
		                   channelType[i], codeClass[i], codeNumber[i], radioConfiguration[i], status[i] == VI_FALSE ? 0 : 1,
		                   cdpRelative[i]);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Advanced Settings
 * Purpose:  This function configures the advanced settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KAdvancedSettings(
	ViSession instrSession,
	ViInt32 pnOffset,
	ViInt32 baseSpreadingFactor,
	ViInt32 powerReference,
	ViInt32 order
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_PNOFFSET, pnOffset),
			2, "PN Offset");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_SFACTOR, baseSpreadingFactor),
			3, "Base Spreading Factor");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_PREFERENCE, powerReference),
			4, "Power Reference");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_ORDER, order),
			5, "Order");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Configure C2K BST Force 1xEV-DV
 * Purpose:  This function switches the 1xEV-DV system.
 *
 *           Note(s):
 *
 *           (1) This is necessary in the automatic search mode to support
 *           the channel detection. If there is a QPSK modulated channel with
 *           spreadingfactor 32 it is not possible to distinguish whether it
 *           is a data channel (CHAN with radio configuration 35) or a
 *           PACKET DATA CHANNEL (PDCH with radio configuration 10). This
 *           decision is made for the PDCH of the 1xEVDV system if the value
 *           is ON. If the value is OFF (default) the CHAN is detected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KForce1xEVDV(
	ViSession instrSession,
	ViBoolean _1xEVDV
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_FORCE_DV, _1xEVDV),
			2, "1xEV-DV");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Antenna Settings
 * Purpose:  This function configures the Antenna Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KAntennaSettings(
	ViSession instrSession,
	ViBoolean antennaDiversity,
	ViInt32 antennaNumber
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (antennaDiversity == RSSPECAN_VAL_C2K_ANTENNA_OFF)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_ANTENNA, antennaDiversity),
				2, "Antenna Diversity");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_ANTENNA, antennaNumber),
				3, "Antenna Number");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Multi Carrier
 * Purpose:  This function configures the Multi Carrier settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KMultiCarrier(
	ViSession instrSession,
	ViBoolean multiCarrier,
	ViBoolean enhancedAlgorithm,
	ViBoolean mcFilter,
	ViInt32 filterType,
	ViReal64 rollOff,
	ViReal64 cutOffFrequency
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_STATE, multiCarrier),
			2, "Multi Carrier");

	if (multiCarrier == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_ENH_ALG, enhancedAlgorithm),
				3, "Enhanced Algorithm");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_FILTER_STATE, mcFilter),
				4, "MC Filter");

		if (mcFilter == VI_TRUE)
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_FILTER_TYPE, filterType),
					5, "Filter Type");

			if (filterType == RSSPECAN_VAL_C2K_MCARRIER_FTYPE_RRC)
			{
				viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_ROLL_OFF, rollOff),
						6, "Roll Off");

				viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_CUT_OFF_FREQ, cutOffFrequency),
						7, "Cut Off Frequency");
			}
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST PICH Marker Settings
 * Purpose:  This function sets selected marker to channel 0.64.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KPichMarkerSettings(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "C1,M1", RSSPECAN_ATTR_C2K_MARKER_PICH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K BST TDPICH Marker Settings
 * Purpose:  This function sets selected marker to channel 16.128.
 *
 *           Notes:
 *
 *           (1) The base spreading factor (RSSPECAN_ATTR_C2K_CDP_SFACTOR
 *           attribute) should be set to 128 before hand.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KTDPichMarkerSettings(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "C1,M1", RSSPECAN_ATTR_C2K_MARKER_TDPICH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Store C2K BTS SEM To File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreBC2KSEMToFile(
	ViSession instrSession,
	ViString fileName
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_LIM_STORE_FILE, fileName),
			2, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read C2K BTS SEM Peak List
 * Purpose:  This function returns the Spectrum EM Mask View Peak List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBC2KSEMPeakList(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 numberOfValues,
	ViReal64 frequency[],
	ViReal64 level[],
	ViReal64 deltaLimit[],
	ViInt32* returnedValues
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 arrayLength;
	ViReal64* data = NULL;
	ViInt32 retVal;
	ViInt32 i;

	checkErr(RsCore_LockSession(instrSession));

	arrayLength = 3 * numberOfValues;
	data = malloc(sizeof(ViReal64) * arrayLength);

	checkErr(rsspecan_dataReadTrace(instrSession, window, "FINal1", arrayLength, data, &retVal));

	if (returnedValues)
		*returnedValues = retVal / 3; // Three result arrays

	retVal = (retVal > arrayLength) ? arrayLength : retVal;

	retVal /= 3;

	for (i = 0; i < retVal; i++)
	{
		frequency[i] = data[i * 3];
		level[i] = data[i * 3 + 1];
		deltaLimit[i] = data[i * 3 + 2];
	}

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query C2K BST Code Domain Analyzer Result Summary
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryBC2KCodeDomainAnalyzerResultSummary(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 resultType,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K82"));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:CDP:RES? %s", window, analyzerResultSummaryArr[resultType]);
	checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &buf));

	if ((strstr(buf, "Sync")) != NULL)
		*result = RS_VAL_NAN_VI_REAL64;
	else
		*result = atof(buf);

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: C2K MST Mode
 * Purpose:  This function selects the CDMA2000 Mobile Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MC2KMode(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MC2K_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K MST SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KSEM(
	ViSession instrSession,
	ViInt32 limitLineType,
	ViReal64 _30kHz1MHzTransition
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_LIM_MODE, limitLineType),
			2, "Limit Line Type");

	if (!rsspecan_IsFSV(instrSession))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS, _30kHz1MHzTransition),
				3, "30kHz/1MHz Transition");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: ConfigureMC2KCodeDomainAnalyzerResults
 * Purpose:  This function configures the Code Domain Analyzer Results Type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCodeDomainAnalyzerResults(ViSession instrSession,
                                                                  ViInt32 resultType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (resultType)
	{
	case RSSPECAN_VAL_C2K_FEED_CDP:
	case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
	case RSSPECAN_VAL_C2K_FEED_CDEP:
	case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	case RSSPECAN_VAL_C2K_FEED_MACC:
	case RSSPECAN_VAL_C2K_FEED_PVSL:
	case RSSPECAN_VAL_C2K_FEED_PVSY:
	case RSSPECAN_VAL_C2K_FEED_BSTR:
	case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
	case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
	case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
	case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
	case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 2, "Result Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K MST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCodeDomainAnalyzerSettings(ViSession instrSession,
                                                                   ViInt32 captureLength,
                                                                   ViInt32 selectChannel,
                                                                   ViInt32 selectPCG,
                                                                   ViInt32 selectMapping)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K83"));

	if (!RsCore_IsInstrumentModel(instrSession, "FSP")) // not FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_IQLENGTH_FSQ_FSU, captureLength),
				2, "Capture Length");
	}
	else // is FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_IQLENGTH_FSP, captureLength),
				2, "Capture Length");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_CODE, selectChannel),
			3, "Select Channel");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT, selectPCG),
			4, "Select PCG");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING, selectMapping),
			5, "Select Mapping");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K MST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KChannelTableData(
	ViSession instrSession,
	ViInt32 channelType[],
	ViInt32 codeClass[],
	ViInt32 codeNumber[],
	ViInt32 mapping[],
	ViInt32 reserved1[],
	ViInt32 reserved2[],
	ViBoolean status[],
	ViReal64 cdpRelative[],
	ViInt32 numofElements
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuffer = cmd;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K83"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, numofElements, 1, INT_MAX),
			10, "Num of Elements");

	pbuffer += sprintf(pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,0,0,%d,%.12f",
	                   channelType[i], codeClass[i], codeNumber[i], mapping[i], status[i] == VI_FALSE ? 0 : 1, cdpRelative[i]);

	for (i = 1; i < numofElements; i++)
		pbuffer += sprintf(pbuffer, ",%ld,%ld,%ld,%ld,0,0,%d,%.12f",
		                   channelType[i], codeClass[i], codeNumber[i], mapping[i], status[i] == VI_FALSE ? 0 : 1, cdpRelative[i]);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K MST Advanced Settings
 * Purpose:  This function configures the Advanced Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KAdvancedSettings(
	ViSession instrSession,
	ViInt32 baseSpreadingFactor,
	ViInt32 powerReference,
	ViInt32 order,
	ViInt32 selectIQ,
	ViInt32 longCodeMode,
	ViString longCodeMask,
	ViString longCodeOffset
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_SFACTOR, baseSpreadingFactor),
			2, "Base Spreading Factor");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_PREFERENCE, powerReference),
			3, "Power Reference");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_ORDER, order),
			4, "Order");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING, selectIQ),
			5, "Select IQ");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_LCODE_MODE, longCodeMode),
			6, "Long Code Mode");

	if (longCodeMode == RSSPECAN_VAL_MC2K_LCODE_STANDARD)
	{
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_LCODE_MASK, longCodeMask),
				7, "Long Code Mask");

		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_LCODE_OFFSET, longCodeOffset),
				8, "Long Code Offset");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K MST CDP Overview
 * Purpose:  This function configures the CDP Overview
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCDPOverview(
	ViSession instrSession,
	ViBoolean codeDomainOverview)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_OVERVIEW, codeDomainOverview),
			2, "Code Domain Overview");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure C2K MST PICH Marker Settings
 * Purpose:  This function sets marker to channel 0.16, 0.32 or 0.64,
 *           depending on the base spreading factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KPichMarkerSettings(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "C1,M1", RSSPECAN_ATTR_MC2K_MARKER_PICH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query C2K MST Code Domain Analyzer Result Summary
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMC2KCodeDomainAnalyzerResultSummary(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 resultType,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K83"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, RSSPECAN_VAL_C2K_SLOT, RSSPECAN_VAL_C2K_EVMPEAK),
			3, "Result Type");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:CDP:RES? %s", window, analyzerResultSummaryArr[resultType]);
	checkErr(RsCore_QueryViReal64(instrSession, cmd, result));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: EV-DO BST Mode
 * Purpose:  This function selects the 1xEV-DO Base Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BDOMode(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_BDO_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BTS SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOSEM(
	ViSession instrSession,
	ViInt32 limitLineType,
	ViReal64 limitLineValue,
	ViReal64 _30kHz1MHzTransition
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_LIM_MODE, limitLineType),
			2, "Limit Line Type");

	if (limitLineType == RSSPECAN_VAL_C2K_LIM_MODE_MAN)
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_LIM_VAL, limitLineValue),
			3, "Limit Line Value");

	if (!rsspecan_IsFSV(instrSession))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS, _30kHz1MHzTransition),
				4, "30kHz/1MHz Transition");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Code Domain Analyzer Result
 * Purpose:  This function selects the measured data that will be displayed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOCodeDomainAnalyzerResults(
	ViSession instrSession,
	ViInt32 resultType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (resultType)
	{
	case RSSPECAN_VAL_C2K_FEED_CDP:
	case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
	case RSSPECAN_VAL_C2K_FEED_CDEP:
	case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
	case RSSPECAN_VAL_C2K_FEED_CPOW:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	case RSSPECAN_VAL_C2K_FEED_MACC:
	case RSSPECAN_VAL_C2K_FEED_PVSL:
	case RSSPECAN_VAL_C2K_FEED_PVSY:
	case RSSPECAN_VAL_C2K_FEED_BSTR:
	case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
	case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
	case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
	case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
	case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
	case RSSPECAN_VAL_C2K_FEED_PVCH:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 2, "Result Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOCodeDomainAnalyzerSettings(
	ViSession instrSession,
	ViInt32 captureLength,
	ViInt32 selectCode,
	ViInt32 selectSlot
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (!RsCore_IsInstrumentModel(instrSession, "FSP")) // not FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_IQLENGTH_FSQ_FSU, captureLength),
				2, "Capture Length");
	}
	else // is FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSP, captureLength),
				2, "Capture Length");
	}
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT, selectSlot),
			4, "Select Slot");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_CODE, selectCode),
			3, "Select Code");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST CDP Channel Type
 * Purpose:  This function configures the CDP Channel Type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOCDPChannelType(
	ViSession instrSession,
	ViInt32 channelType,
	ViBoolean codeDomainOverview,
	ViInt32 mapping,
	ViInt32 select
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_CTYPE, channelType),
			2, "Channel Type");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BDO_CDP_OVERVIEW, codeDomainOverview),
			3, "Code Domain Overview");

	if (codeDomainOverview == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_MMODE, mapping),
				4, "Mapping");

		if (((channelType != RSSPECAN_VAL_C2K_CDP_CTYPE_DATA) && (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_AUTO)) || (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_COMPLEX))
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_MAPPING, select),
				5, "Select");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Channel Type
 * Purpose:  This function configures the Channel Type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOChannelType(
	ViSession instrSession,
	ViInt32 channelType,
	ViInt32 mapping,
	ViInt32 select
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_CTYPE, channelType),
			2, "Channel Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_MMODE, mapping),
			3, "Mapping");

	if (((channelType != RSSPECAN_VAL_C2K_CDP_CTYPE_DATA) && (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_AUTO)) || (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_COMPLEX))
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_CDP_MAPPING, select),
			4, "Select");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Power Reference
 * Purpose:  This function selects the reference power for relative results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureEVDOBSTPowerReference(ViSession instrSession,
                                                          ViInt32 powerReference)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_PREFERENCE, powerReference),
			2, "Power Reference");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Power vs Time
 * Purpose:  This function configures the Power vs Time Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOPowerVsTime(
	ViSession instrSession,
	ViInt32 numberOfHalfslots,
	ViInt32 referenceSlot,
	ViBoolean burstFit,
	ViInt32 referenceMeanPower,
	ViReal64 referenceValue,
	ViBoolean restartOnFail
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, numberOfHalfslots),
			2, "Number Of Halfslots");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_RFSLOT, referenceSlot),
			3, "Reference Slot");

	if (referenceSlot == RSSPECAN_VAL_C2K_CDP_RFSLOT_IDLE)
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BDO_PVT_BURST_FIT, burstFit),
			4, "Burst Fit");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_REFERENCE, referenceMeanPower),
			5, "Reference Mean Power");

	if (referenceMeanPower != RSSPECAN_VAL_C2K_LIM_PVT_REF_AUTO)
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_VALUE, referenceValue),
			6, "Reference Value");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BDO_PVT_FAIL_RESTART, restartOnFail),
			7, "Restart On Fail");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Power vs Time List Evaluation
 * Purpose:  This function enables/disables Power vs Time list evaluation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOPowerVsTimeListEvaluation(ViSession instrSession,
                                                                 ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_LIST, state),
			2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Restore EV-DO BST Power vs Time Standard Lines
 * Purpose:  This function restores the standard limit lines for the power
 *           versus time measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreBDOPowerVsTimeStandardLines(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_RESTORE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOChannelTableData(
	ViSession instrSession,
	ViInt32 channelType[],
	ViInt32 codeClass[],
	ViInt32 codeNumber[],
	ViInt32 modulation[],
	ViInt32 reserved1[],
	ViInt32 reserved2[],
	ViBoolean status[],
	ViReal64 cdpRelative[],
	ViInt32 numofElements
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuffer = cmd;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K84"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, numofElements, 1, INT_MAX),
			10, "Num of Elements");

	pbuffer += sprintf(pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,0,0,%d,%.12f",
	                   channelType[i], codeClass[i], codeNumber[i], modulation[i], status[i] == VI_FALSE ? 0 : 1, cdpRelative[i]);

	for (i = 1; i < numofElements; i++)
		pbuffer += sprintf(pbuffer, ",%ld,%ld,%ld,%ld,0,0,%d,%.12f",
		                   channelType[i], codeClass[i], codeNumber[i], modulation[i], status[i] == VI_FALSE ? 0 : 1, cdpRelative[i]);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Advanced Settings
 * Purpose:  This function configures the Advanced Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOAdvancedSettings(ViSession instrSession,
                                                        ViInt32 pnOffset,
                                                        ViBoolean cdpAverage,
                                                        ViInt32 revision)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_PNOFFSET, pnOffset),
			2, "PN Offset");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BDO_CDP_AVERAGE, cdpAverage),
			3, "CDP Average");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BDO_REVISION, revision),
			4, "Revision");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Multi Carrier
 * Purpose:  This function configures the Multi Carrier settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOMultiCarrier(
	ViSession instrSession,
	ViBoolean multiCarrier,
	ViBoolean enhancedAlgorithm,
	ViBoolean mcFilter,
	ViInt32 filterType,
	ViReal64 rollOff,
	ViReal64 cutOffFrequency
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_STATE, multiCarrier),
			2, "Multi Carrier");

	if (multiCarrier == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_ENH_ALG, enhancedAlgorithm),
				3, "Enhanced Algorithm");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_FILTER_STATE, mcFilter),
				4, "MC Filter");

		if (mcFilter == VI_TRUE)
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_FILTER_TYPE, filterType),
					5, "Filter Type");

			if (filterType == RSSPECAN_VAL_C2K_MCARRIER_FTYPE_RRC)
			{
				viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_ROLL_OFF, rollOff),
						6, "Roll Off");

				viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_CUT_OFF_FREQ, cutOffFrequency),
						7, "Cut Off Frequency");
			}
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Store BDO BTS SEM To File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreBDOSEMToFile(
	ViSession instrSession,
	ViString fileName
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_C2K_LIM_STORE_FILE, fileName),
			2, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read EV-DO BTS SEM Peak List
 * Purpose:  This function returns the Spectrum EM Mask View Peak List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBDOSEMPeakList(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 numberOfValues,
	ViReal64 frequency[],
	ViReal64 level[],
	ViReal64 deltaLimit[],
	ViInt32* returnedValues
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 arrayLength;
	ViReal64* data = NULL;
	ViInt32 retVal;
	ViInt32 i;

	checkErr(RsCore_LockSession(instrSession));

	arrayLength = 3 * numberOfValues;
	data = malloc(sizeof(ViReal64) * arrayLength);

	checkErr(rsspecan_dataReadTrace(instrSession, window, "FINal1", arrayLength, data, &retVal));

	if (returnedValues)
		*returnedValues = retVal / 3; // Three result arrays

	retVal = (retVal > arrayLength) ? arrayLength : retVal;

	retVal /= 3;

	for (i = 0; i < retVal; i++)
	{
		frequency[i] = data[i * 3];
		level[i] = data[i * 3 + 1];
		deltaLimit[i] = data[i * 3 + 2];
	}

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read EV-DO BTS Power vs Time List Evaluation
 * Purpose:  This function returns the list evaluation results
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBDOPowerVsTimeListEvaluation(ViSession instrSession,
                                                            ViInt32 noOfValues,
                                                            ViReal64 rangeNumber[],
                                                            ViReal64 startTime[],
                                                            ViReal64 stopTime[],
                                                            ViReal64 avgPowerAbs[],
                                                            ViReal64 avgPowerRel[],
                                                            ViReal64 maxPowerAbs[],
                                                            ViReal64 maxPowerRel[],
                                                            ViReal64 minPowerAbs[],
                                                            ViReal64 minPowerRel[],
                                                            ViReal64 limitCheck[],
                                                            ViReal64 reserved1[],
                                                            ViReal64 reserved2[],
                                                            ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize, i;
	ViInt32 j = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, rangeNumber), 2, "Range number");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, startTime), 3, "Start Time");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, stopTime), 4, "Stop Time");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, avgPowerAbs), 5, "Avg Power Abs");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, avgPowerRel), 6, "Avg Power Rel");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, maxPowerAbs), 7, "Max Power Abs");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, maxPowerRel), 8, "Max Power Rel");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, minPowerAbs), 9, "Min Power Abs");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, minPowerRel), 10, "Min Power Rel");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, limitCheck), 11, "Limit Check");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, reserved1), 12, "Reserved 1");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, reserved2), 13, "Reserved 2");

	checkErr(RsCore_QueryFloatArray(instrSession, ":CONF:CDP:BTS:PVT:LIST:RES?", &data, &dataSize));

	dataSize /= 12;

	if (returnedValues)
		*returnedValues = dataSize;

	if (dataSize > noOfValues)
		dataSize = noOfValues;

	for (i = 0; i < dataSize; i++)
	{
		rangeNumber[i] = data[j++];
		startTime[i] = data[j++];
		stopTime[i] = data[j++];
		avgPowerAbs[i] = data[j++];
		avgPowerRel[i] = data[j++];
		maxPowerAbs[i] = data[j++];
		maxPowerRel[i] = data[j++];
		minPowerAbs[i] = data[j++];
		minPowerRel[i] = data[j++];
		limitCheck[i] = data[j++];
		reserved1[i] = data[j++];
		reserved2[i] = data[j++];
	}

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query EV-DO BST Code Domain Analyzer General Results
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryBDOCodeDomainAnalyzerGeneralResults(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 resultType,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K84|K47"));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:CDP:RES? %s", window, analyzerResultSummaryArr[resultType]);
	checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &buf));

	if ((strstr(buf, "Sync")) != NULL)
		*result = RS_VAL_NAN_VI_REAL64;
	else
		*result = atof(buf);

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query EV-DO BST Code Domain Analyzer Channel Results
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryBDOCodeDomainAnalyzerChannelResults(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 resultType,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K84"));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:CDP:RES? %s", window, analyzerResultSummaryArr[resultType]);
	checkErr(RsCore_QueryViReal64(instrSession, cmd, result));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: EV-DO MST Mode
 * Purpose:  This function selects the 1xEV-DO Mobile Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MDOMode(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MDO_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOSEM(
	ViSession instrSession,
	ViInt32 limitLineType,
	ViReal64 _30kHz1MHzTransition
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_LIM_MODE, limitLineType),
			2, "Limit Line Type");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS, _30kHz1MHzTransition),
			3, "30kHz/1MHz Transition");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Code Domain Analyzer Results
 * Purpose:  This function configures the Code Domain Analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOCodeDomainAnalyzerResults(
	ViSession instrSession,
	ViInt32 resultType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (resultType)
	{
	case RSSPECAN_VAL_C2K_FEED_CDP:
	case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
	case RSSPECAN_VAL_C2K_FEED_CDEP:
	case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	case RSSPECAN_VAL_C2K_FEED_MACC:
	case RSSPECAN_VAL_C2K_FEED_PVSL:
	case RSSPECAN_VAL_C2K_FEED_PVSY:
	case RSSPECAN_VAL_C2K_FEED_BSTR:
	case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
	case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
	case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
	case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
	case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 2, "Result Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOCodeDomainAnalyzerSettings(ViSession instrSession,
                                                                  ViInt32 captureLength,
                                                                  ViInt32 selectChannel,
                                                                  ViInt32 selectHalfslot,
                                                                  ViInt32 selectMapping)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K85"));

	if (!RsCore_IsInstrumentModel(instrSession, "FSP")) // not FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MDO_CDP_IQLENGTH_FSQ_FSU, captureLength),
				2, "Capture Length");
	}
	else // is FSP
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MDO_CDP_IQLENGTH_FSP, captureLength),
				2, "Capture Length");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MDO_CDP_CODE, selectChannel),
			3, "Select Channel");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT, selectHalfslot),
			4, "Select Halfslot");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING, selectMapping),
			5, "Select Mapping");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOChannelTableData(
	ViSession instrSession,
	ViInt32 channelType[],
	ViInt32 codeClass[],
	ViInt32 codeNumber[],
	ViInt32 mapping[],
	ViInt32 activity[],
	ViInt32 reserved1[],
	ViBoolean status[],
	ViInt32 reserved2[],
	ViInt32 numofElements
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuffer = cmd;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K85"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, numofElements, 1, INT_MAX),
			10, "Num of Elements");

	pbuffer += sprintf(pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,%ld,0,%d,0",
	                   channelType[i], codeClass[i], codeNumber[i], mapping[i], activity[i], status[i] == VI_FALSE ? 0 : 1);

	for (i = 1; i < numofElements; i++)
		pbuffer += sprintf(pbuffer, ",%ld,%ld,%ld,%ld,%ld,0,%d,0",
		                   channelType[i], codeClass[i], codeNumber[i], mapping[i], activity[i], status[i] == VI_FALSE ? 0 : 1);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Advanced Settings
 * Purpose:  This function configures the Advanced Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOAdvancedSettings(
	ViSession instrSession,
	ViInt32 powerReference,
	ViInt32 order,
	ViInt32 selectIQ,
	ViBoolean cdpAverage,
	ViInt32 operation,
	ViString longCodeI,
	ViString longCodeQ
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_PREFERENCE, powerReference),
			2, "Power Reference");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_ORDER, order),
			3, "Order");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING, selectIQ),
			4, "Select IQ");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MDO_CDP_AVERAGE, cdpAverage),
			5, "CDP Average");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MDO_CDP_OPERATION, operation),
			6, "Operation");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MDO_CDP_LCODE_I, longCodeI),
			7, "Long Code I");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MDO_CDP_LCODE_Q, longCodeQ),
			8, "Long Code Q");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST CDP Overview
 * Purpose:  This function configures the CDP Overview       .
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOCDPOverview(
	ViSession instrSession,
	ViBoolean codeDomainOverview)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MC2K_CDP_OVERVIEW, codeDomainOverview),
			2, "Code Domain Overview");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST PICH Marker Settings
 * Purpose:  This function sets marker to channel 0.16, 0.32 or 0.64,
 *           depending on the base spreading factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOPichMarkerSettings(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "C1,M1", RSSPECAN_ATTR_MC2K_MARKER_PICH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query EV-DO MST Code Domain Analyzer Result Summary
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMDOCodeDomainAnalyzerResultSummary(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 resultType,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K85"));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:CDP:RES? %s", window, analyzerResultSummaryArr[resultType]);
	checkErr(RsCore_QueryViReal64(instrSession, cmd, result));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
