/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  Network Analyzer
 *
 *  Original Release: March 2010
 *  By: Petr Kominek
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"

static ViString tetraMoveMarkerMeasArr[] = {"PVT","EVMS","EVMC","MERR","PERR","SPEC","ACPM","ACPT","CAPT", NULL};
static ViString evmLimitTypeArr[] = {"All","Data","DHEA","DSP","SPIL","IQOF","FERR","PHAS","MAGE", NULL};
static ViString tetraMeasModArr[] = {"None","Min","max","Aver","Curr", NULL};
static ViString tetraACPTypeArr[] = {"Mod","Trans", NULL};
static ViString tetraACPChannelArr[] = {"0","1","2","3","4","5","6", NULL};
static ViString tetraChannelBWArr[] = {"BW25","BW50","BW100","BW150", NULL};
static ViString tetraMeasurementSelectArr[] = {"PvsT","EVMSYMbol","EVMCARrier","MAGNitude","PHASe","SPECFFT","SPECACPMod","SPECACPUpp","SPECACPLow","CONSSYMbol","CONSCarrier","STATistics","SUMTable", NULL};
static ViString tetraYAxisMeasArr[] = {"ECarrier","ESymbol","Magnitude","Phase"};
static ViString tetraSummaryTableArr[] = {"EVMAll","EVMData","EVMDHEAder","EVMDSPilot","EVMSPILot","IQOFfset","GIMBalance","QUADerror","AMPDroop","FERRor","PHASeerror","MAGerror","BPWR","CRESt", NULL};
static ViString tetraPVTMeasArr[] = {"REFPower","SLOTpower","BEFBpower","RELBeforepower","AFTBpower","RELafterpower", NULL};
static ViString tetraACPMeas[] = {"TETRABANDwidth","TETRARRCosine","TETRAOFFSet","TETRAABSPower","TETRARELPower","TETRADELLimit", NULL};

/*****************************************************************************
 * Function: Set TETRA Mode
 * Purpose:  This function selects TETRA 2 analysis mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetTETRAMode(
	ViSession	instrSession
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TETRA_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Signal Characteristics
 * Purpose:  This function configures the general settings concerning the
 *           physical attributes of the signals to be measured.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRASignalCharacteristics(
	ViSession	instrSession,
	ViReal64	frequency,
	ViInt32	slotType,
	ViInt32	channelBandwidth
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_FREQUENCY_CENTER, frequency),
    		2, "Frequency");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_BURST_TYPE, slotType),
			3, "Slot Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_CHANNEL_BANDWIDTH, channelBandwidth),
			4, "Channel Bandwidth");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Level Settings
 * Purpose:  This function configures settings related to the reference
 *           level and the RF attenuation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRALevelSettings(
	ViSession	instrSession,
	ViBoolean	autoLevel,
	ViReal64	level,
	ViReal64	externalAttenuation
)
{
    ViStatus	error = VI_SUCCESS;
    ViInt32    type;
    autoLevel;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_TETRA_INPUT_SELECT, &type));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, externalAttenuation),
			4, "External Attenuation");

    switch (type)
	{
		case RSSPECAN_VAL_BB_INPUT_RF:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_REFERENCE_LEVEL_FOR_RF_MEASUREMENT, level),
				3, "Level");
		break;

		case RSSPECAN_VAL_BB_INPUT_AIQ:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_RANGE, level),
				3, "Level");
		break;

		case RSSPECAN_VAL_BB_INPUT_DIQ:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_REFERENCE_LEVEL_FOR_IQ_MEASUREMENT, level),
				3, "Level");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Data Capture Settings
 * Purpose:  This function configures how much data is to be captured.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRADataCaptureSettings(
	ViSession	instrSession,
	ViReal64	captureTime,
	ViInt32	captureTimeUnit,
	ViBoolean	useNumberOfSlotsToAnalyzer,
	ViInt32	numberOfSlotsToAnalyze
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if (captureTimeUnit == RSSPECAN_VAL_TETRA_UNIT_SECONDS)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME, captureTime),
				2, "Capture Time");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_TEDS_SLOT_DURATION, captureTime),
				2, "Capture Time");
	}

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TETRA_OVERALL_BURST_COUNT, useNumberOfSlotsToAnalyzer),
			3, "Use Number Of Slots To Analyzer");

	if (useNumberOfSlotsToAnalyzer == VI_TRUE)
	{
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_NUMBER_OF_BURST, numberOfSlotsToAnalyze),
			4, "Number Of Slots To Analyze");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Trigger Settings
 * Purpose:  This function configures the trigger parameters.
 *
 *           Note(s):
 *
 *           (1) This function is only available for RF input and analog
 *           baseband input.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRATriggerSettings(
	ViSession	instrSession,
	ViInt32	mode,
	ViReal64	offset,
	ViInt32	offsetUnit,
	ViReal64	extTriggerLevel,
	ViBoolean	autoTriggerLevel,
	ViReal64	powerTriggerLevel
)
{
	ViStatus	error = VI_SUCCESS;
	ViInt32 type;
	powerTriggerLevel;
	autoTriggerLevel;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_TRIGGER_MODE, mode),
			2, "Mode");

	if (offsetUnit == RSSPECAN_VAL_TETRA_UNIT_SECONDS)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset),
				3, "Offset");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32 (instrSession, "",
                                               	   RSSPECAN_ATTR_TETRA_TRIGGER_TEDS_SLOT_DURATION,
                                               	   (ViInt32) offset), 3, "Offset");
	}

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, extTriggerLevel),
			5, "Ext. Trigger Level");

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_TETRA_INPUT_SELECT, &type));

	if (mode == RSSPECAN_VAL_TRIG_MODE_POW)
	{
		switch (type)
		{
			case RSSPECAN_VAL_BB_INPUT_RF:
				viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_TRIGGER_POWER_LEVEL, extTriggerLevel),
						7, "Power Trigger Level");
				break;

			case RSSPECAN_VAL_BB_INPUT_DIQ:
				 viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_TRIGGER_IQ_LEVEL, extTriggerLevel),
				 		7, "Power Trigger Level");
				 break;
		}
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA IQ Settings
 * Purpose:  This function configures I/Q settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAIQSettings(
	ViSession	instrSession,
	ViBoolean	swapIQ
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TETRA_IQ_SETTING, swapIQ),
    		2, "Swap IQ");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Input Settings
 * Purpose:  This function configures whether the RF input or the analog
 *           Baseband input or the digital Baseband input is the currently
 *           selected signal input.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAInputSettings(
	ViSession	instrSession,
	ViInt32	input
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_INPUT_SELECT, input),
    		2, "Input");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Advanced Baseband Settings
 * Purpose:  This function configures details of how the instrument operates
 *           and how measurements are performed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAAdvancedBasebandSettings(
	ViSession	instrSession,
	ViInt32	impedance,
	ViInt32	path,
	ViBoolean	balanced,
	ViBoolean	lowPass,
	ViBoolean	iqDither
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IMPEDANCE, impedance),
    		2, "Impedance");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_SIGNAL_PATH, path),
			3, "Path");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_BALANCED, balanced),
    		4, "Balanced");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IQ_LPAS, lowPass),
			5, "Low Pass");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IQ_DITH, iqDither),
			6, "IQ Dither");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Advanced IQ Digital
 * Purpose:  This function configures settings related to the reference
 *           level and the RF attenuation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAAdvancedIQDigital(
	ViSession	instrSession,
	ViBoolean	autoLevel,
	ViReal64	level,
	ViBoolean	autoSamplingRate,
	ViReal64	samplingRate
)
{
    ViStatus	error = VI_SUCCESS;
    ViInt32 type;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_DIGITAL_INPUT_RANGE_AUTO, autoLevel),
    		2, "Auto Level");

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_TETRA_INPUT_SELECT, &type));
    switch (type)
	{
		case RSSPECAN_VAL_BB_INPUT_RF:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_REFERENCE_LEVEL_FOR_RF_MEASUREMENT, level),
				3, "Level");
		break;

		case RSSPECAN_VAL_BB_INPUT_AIQ:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_RANGE, level),
				3, "Level");
		break;

		case RSSPECAN_VAL_BB_INPUT_DIQ:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_REFERENCE_LEVEL_FOR_IQ_MEASUREMENT, level),
				3, "Level");
		break;
	}

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_DIGITAL_INPUT_SAMPLE_RATE_AUTO, autoSamplingRate),
			4, "Auto Sampling Rate");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_SRATE, samplingRate),
			5, "Sampling Rate");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA IQ Center Frequency
 * Purpose:  This function configures the IQ value for Center Frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAIQCenterFrequency(
	ViSession	instrSession,
	ViReal64	iqCenterFrequency
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_IQ_CENTER_FREQUENCY, iqCenterFrequency),
    		2, "IQ Center Frequency");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Demodulation Settings
 * Purpose:  This function configures the characteristics of the slots to be
 *           considered in the measurement results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRADemodulationSettings(
	ViSession	instrSession,
	ViInt32	slotType,
	ViInt32	payloadModulation,
	ViReal64	demodRRCRolloff,
	ViInt32	maxCarrierOffset
)
{
    ViStatus	error = VI_SUCCESS;
    demodRRCRolloff;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_BURST_TYPE, slotType),
    		2, "Slot Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_PAYLOAD_MODULATION, payloadModulation),
			3, "Payload Modulation");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_MAXIMUM_CARRIER_OFFSET, maxCarrierOffset),
			4, "Max Carrier Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA EVM Settings
 * Purpose:  This function configures the type of calculations and
 *           normalizations that will be performed on the  demodulator's
 *           output.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAEVMSettings(
	ViSession	instrSession,
	ViBoolean	pilotTracking,
	ViBoolean	compensateAmplitudeDroop,
	ViBoolean	compensateIQOffset
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TETRA_PILOT_TRACKING, pilotTracking),
    		2, "Pilot Tracking");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TETRA_COMPENSATE_AMPLITUDE_DROOP, compensateAmplitudeDroop),
			3, "Compensate Amplitude Droop");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TETRA_COMPENSATE_IQ_OFFSET, compensateIQOffset),
			4, "Compensate IQ Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Trace Settings
 * Purpose:  This function configures which traces are displayed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRATraceSettings(
	ViSession	instrSession,
	ViInt32	trace,
	ViBoolean	traceState
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 4),
    		2, "Trace");

    sprintf (buffer, "Win2,TR%ld", trace);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TRACE_STATE, traceState),
			3, "Trace State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Marker
 * Purpose:  This function enables the active marker on the specified trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAMarker(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	marker,
	ViBoolean	markerEnabled,
	ViInt32	trace
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 4),
			3, "Marker");

    sprintf (buffer, "Win%ld,M%ld", window, marker);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_MARKER_ENABLED, markerEnabled),
    		4, "Marker Enabled");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_MARKER_TRACE, trace),
			5, "Trace");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: TETRA Marker Search
 * Purpose:  This function specifies the type of marker search and performs
 *           the search.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TETRAMarkerSearch(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	marker,
	ViInt32	markerSearch
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 4),
			3, "Marker");

    sprintf (buffer, "Win%ld,M%ld", window, marker);

	if (markerSearch == RSSPECAN_VAL_MARKER_SEARCH_HIGHEST)
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK, NULL));
	}
	else
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN, NULL));
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: TETRA Move Marker
 * Purpose:  This function moves the active marker to the specified
 *           horizontal position.
 *
 *           Note(s):
 *
 *           (1) The marker must be ON (use function
 *           rsspecan_ConfigureTETRAMarker).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TETRAMoveMarker(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	marker,
	ViInt32	measurement,
	ViReal64	markerPosition
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 4),
			3, "Marker");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 8),
			4, "Measurement");

    sprintf (buffer, "Win%ld,M%ld,Mkr%s", window, marker, tetraMoveMarkerMeasArr[measurement]);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_MOVE_MARKER, markerPosition),
    		5, "Marker Position");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query TETRA Marker Amplitude
 * Purpose:  This function returns the marker amplitude level of the
 *           selected marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTETRAMarkerAmplitude(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	marker,
	ViReal64*	markerAmplitude
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 4),
			3, "Marker");

    sprintf (buffer, "Win%ld,M%ld", window, marker);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_MARKER_AMPLITUDE, markerAmplitude),
    		2, "Marker Amplitude");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Display Format
 * Purpose:  This function switches the measurement result display between
 *           FULL SCREEN and SPLIT SCREEN.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRADisplayFormat(
	ViSession	instrSession,
	ViInt32	format
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_FORMAT, format),
    		2, "Format");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Display Active Window
 * Purpose:  This function selects the active measurement window.
 *           In FULL SCREEN mode, the measurements are only performed in the
 *           active measurement window. Measurements are therefore initiated
 *           in the active window and result queries (marker, trace data and
 *           other results) answered also in the active window.
 *           Initiating measurements and querying results in the inactive
 *           window yields an error message (execution error).
 *           In split screen mode, the selection of the active window for
 *           result queries is irrelevant.
 *
 *           Note(s):
 *
 *           (1) In FULL SCREEN mode, settings can also be performed in the
 *           inactive measurement window. They become effective as soon as
 *           the corresponding window becomes active.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRADisplayActiveWindow(
	ViSession	instrSession,
	ViInt32	window
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SELECT_WINDOW, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Result Summary Limits
 * Purpose:  This function configures the limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAResultSummaryLimits(
	ViSession	instrSession,
	ViInt32	limitType,
	ViInt32	mode,
	ViReal64	value
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 8),
    		2, "Limit Type");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, 2, 3),
			3, "Mode");

    sprintf (buffer, "EVMLim%s,TETRA%s", evmLimitTypeArr[limitType], tetraMeasModArr[mode]);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_EVM_SUMMARY_LIMITS, value),
    		4, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Result Summary Limits All
 * Purpose:  This function sets all limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAResultSummaryLimitsAll(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	limitValues[]
)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *pbuf;
    ViInt32     i;

	checkErr(RsCore_LockSession(instrSession));

	pbuf = buffer;
    if (arraySize>0)
    {
        pbuf += sprintf (buffer, "CALC:LIM:BURS:ALL ");

		for (i = 0; i < arraySize; i++)
            pbuf += sprintf (pbuf, "%.12f,",limitValues[i]);

		*pbuf = '\0';
    	*--pbuf = '\n';

        checkErr(RsCore_Write(instrSession, buffer));
    }

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get TETRA Result Summary Limits All
 * Purpose:  This function returns all limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTETRAResultSummaryLimitsAll(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	limitValues[],
	ViInt32*	returnedValues
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:PVTT:ALL?", arraySize, limitValues, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA ACP Limits
 * Purpose:  This function configures the ACP Due to Modulation and ACP Due
 *           to Transients relative power limit for the specified channel and
 *
 *           bandwidth in dB.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAACPLimits(
	ViSession	instrSession,
	ViInt32	acp,
	ViInt32	channelNumber,
	ViInt32	channelBandwidth,
	ViReal64	limitValue
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, acp, 0, 1),
    		2, "ACP");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 0, 6),
			3, "Channel Number");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelBandwidth, 0, 3),
			4, "Channel Bandwidth");

    sprintf (buffer, "ACP%s,ACP%s,Ch%s", tetraACPTypeArr[acp], tetraACPChannelArr[channelNumber], tetraChannelBWArr[channelBandwidth]);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_ACP_LIMITS, limitValue),
    		5, "Limit Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Measurement Type
 * Purpose:  This function selects the TETRA measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAMeasurementType(
	ViSession	instrSession,
	ViInt32	measurement
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 12),
    		2, "Measurement");

    sprintf (buffer, "%s", tetraMeasurementSelectArr[measurement]);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_TETRA_MEASUREMENT_TYPE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Y Axis
 * Purpose:  This function configures the Y-axis for the specified trace
 *           display and measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAYAxis(
	ViSession	instrSession,
	ViInt32	measurement,
	ViBoolean	autoScaling,
	ViInt32	unit,
	ViReal64	yCenter,
	ViReal64	ySpan
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 1, 4),
    		2, "Measurement");

    sprintf (buffer, "TETRA%s", tetraYAxisMeasArr[measurement]);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE_AUTO, autoScaling),
    		3, "Auto Scaling");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE_UNIT, unit),
			4, "Unit");

	if (autoScaling == VI_FALSE)
	{
		sprintf (buffer, "TETRA%s,TETRACenter", tetraYAxisMeasArr[measurement]);

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE, yCenter),
				5, "Y-Center");

		sprintf (buffer, "TETRA%s,TETRASpan", tetraYAxisMeasArr[measurement]);

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE, ySpan),
				5, "Y-Span");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Continuous Measurement
 * Purpose:  This function determines whether the trigger system is
 *           continuously initiated (continuous) or performs single
 *           measurements (single)
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAContinuousMeasurement(
	ViSession	instrSession,
	ViBoolean	continuous
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, continuous),
    		2, "Continuous");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Constellation Settings
 * Purpose:  This function configures the constellation settings.
 *
 *           Note(s):
 *
 *           (1) This function is available only if Constellation vs. Symbol
 *           or Constellation vs. Carrier measurement is selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAConstellationSettings(
	ViSession	instrSession,
	ViInt32	symbolstoPlotSelection,
	ViInt32	carrierSelection,
	ViInt32	carrierNumber
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, carrierSelection, 0, 1),
			3, "Carrier Selection");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_CONSTELLATION_SYMBOL_SETTINGS, symbolstoPlotSelection),
    		2, "Symbols to Plot Selection");

	if (carrierSelection == RSSPECAN_VAL_CARRIER_NUM)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_CONSTELLATION_CARRIER_NUMBER, carrierNumber),
				4, "Carrier Number");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA EVM Result Units
 * Purpose:  Specifies the units for EVM results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAEVMResultUnits(
	ViSession	instrSession,
	ViInt32	evmUnits
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_EVM_RESULT_UNITS, evmUnits),
    		2, "EVM Units");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure TETRA Gain Imbalance Result Units
 * Purpose:  Specifies the units for Gain Imbalance Summary Table results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTETRAGainImbalanceResultUnits(
	ViSession	instrSession,
	ViInt32	gainImbalanceUnits
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_GAIN_IMBALANCE_RESULT_UNITS, gainImbalanceUnits),
    		2, "Gain Imbalance Units");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: TETRA Burst Recalc
 * Purpose:  This function causes the measurement results to be recalculated
 *           using the current settings and acquisition data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TETRABurstRecalc(
	ViSession	instrSession
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TETRA_BURST_RECALC, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Read TETRA Trace Data
 * Purpose:  This function returns all the measured data that relates to the
 *           currently selected measurement type. The returned data is
 *           particular to the currently selected measurement type and is
 *           specified in the Trace Data parameter description.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadTETRATraceData(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	trace,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus	error = VI_SUCCESS;
	ViChar      traceName[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	snprintf (traceName, RS_MAX_MESSAGE_BUF_SIZE, "TRACE%ld", trace);

	checkErr(rsspecan_dataReadTrace(instrSession, window, traceName, arraySize, traceData, noofValues));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Read TETRA Bitstream
 * Purpose:  This function returns all the measured data that relates to the
 *           currently selected measurement type. The returned data is
 *           particular to the currently selected measurement type and is
 *           specified in the Trace Data parameter description.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadTETRABitstream(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	trace,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus	error = VI_SUCCESS;
	ViChar      traceName[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	snprintf(traceName, RS_MAX_MESSAGE_BUF_SIZE, "TRACE%ld", trace);
	checkErr(rsspecan_dataReadTrace(instrSession, window, traceName, arraySize, traceData, noofValues));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA Number Of Bursts
 * Purpose:  This function returns the total number of burst found and
 *           analyzed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRANumberOfBursts(
	ViSession	instrSession,
	ViInt32*	numberOfBurst
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TETRA_NUMBER_OF_BURST, numberOfBurst),
    		2, "Number of Burst");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA Measurement Results
 * Purpose:  This function returns the measured current (i.e. of last found
 *           slot), average, minimum or maximum result values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRAMeasuremenResults(
	ViSession	instrSession,
	ViInt32	measurement,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 13),
    		2, "Measurement");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, 1, 4),
			3, "Measurement Type");

    sprintf (buffer, "%s,TETRA%s", tetraSummaryTableArr[measurement], tetraMeasModArr[measurementType]);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_SUMMARY_TABLE_MEASUREMENT_RESULT, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA Measurement Results All
 * Purpose:  Returns the results from the R&S FS-K110 Summary Table
 *           measurement.
 *
 *           Note(s):
 *
 *           (1) The units for the EVM results have to be specified before
 *           with the rsspecan_ConfigureTETRAEVMResultUnits function. The
 *           units
 *           for the gain imbalance results have to be specified before with
 *           the rsspecan_ConfigureTETRAGainImbalanceResultUnits function.
 *
 *           (2) This function does not return all results that the
 *           rsspecan_FetchTETRAMeasuremenResults function can return.
 *           Especially the "current" results (i.e. results of the
 *           last found slot) and the subcarrier results are excluded.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRAMeasurementResultsAll(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	result[],
	ViInt32*	returnedValues
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:PVTT:ALL?", arraySize, result, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA Subcarrier Mean Power
 * Purpose:  Returns the measured current, average, minimum or maximum of
 *           the Mean power in each sub-carrier in dBm.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRASubcarrierMeanPower(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	result[],
	ViInt32*	returnedValues
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, 1, 4),
    		2, "Measurement Type");

    switch (measurementType)
	{
		case RSSPECAN_VAL_TETRA_MEAS_MIN:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCMP:MIN?", arraySize, result, returnedValues));
			break;

		case RSSPECAN_VAL_TETRA_MEAS_MAX:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCMP:MAX?", arraySize, result, returnedValues));
			break;

		case RSSPECAN_VAL_TETRA_MEAS_AVER:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCMP:AVER?", arraySize, result, returnedValues));
			break;

		case RSSPECAN_VAL_TETRA_MEAS_CURR:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCMP?", arraySize, result, returnedValues));
			break;
	}

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA Subcarrier Reference Power
 * Purpose:  Returns the measured current, average, minimum or maximum of
 *           the Reference power in each sub-carrier in dBm.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRASubcarrierReferencePower(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	result[],
	ViInt32*	returnedValues
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, 1, 4),
    		2, "Measurement Type");

    switch (measurementType)
	{
		case RSSPECAN_VAL_TETRA_MEAS_MIN:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCRP:MIN?", arraySize, result, returnedValues));
			break;

		case RSSPECAN_VAL_TETRA_MEAS_MAX:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCRP:MAX?", arraySize, result, returnedValues));
			break;

		case RSSPECAN_VAL_TETRA_MEAS_AVER:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCRP:AVER?", arraySize, result, returnedValues));
			break;

		case RSSPECAN_VAL_TETRA_MEAS_CURR:
			checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:SUMT:SCRP?", arraySize, result, returnedValues));
			break;
	}

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA PVT Measurement
 * Purpose:  This function queries value for the Power versus Time
 *           measurements from the result summary list.
 *
 *           Note(s):
 *
 *           (1) The Power versus Time measurement must be selected to
 *           obtain results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRAPVTMeasurement(
	ViSession	instrSession,
	ViInt32	pvt,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, pvt, 0, 6),
    		2, "PVT");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, 1, 4),
			3, "Measurement Type");

	if (pvt == RSSPECAN_VAL_TETRA_PVT_TRG_TO_SYNC)
	{
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_PVT_TRIGGER_TO_SYNC_TIME, result),
				4, "Result");
	}
	else
	{
    	sprintf (buffer, "%s,TETRA%s", tetraPVTMeasArr[pvt], tetraMeasModArr[measurementType]);

    	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_PVT_MEASUREMENT, result),
    			4, "Result");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA PVT Measurement All
 * Purpose:  Returns the results from the R&S FS-K110 Power versus Time
 *           Summary Table measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRAPVTMeasurementAll(
	ViSession	instrSession,
	ViReal64	results[],
	ViInt32	arraySize,
	ViInt32*	returnedValues
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:PVTT:ALL?", arraySize, results, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA Spectrum FFT Measurement RBW
 * Purpose:  This function returns the RBW used for the most recent Spectrum
 *           FFT measurement. Units are Hertz.
 *
 *           Note(s):
 *
 *           (1) This function is valid only if the Spectrum FFT measurement
 *           is selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRASpectrumFFTMeasurementRBW(
	ViSession	instrSession,
	ViReal64*	result
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_SPECTRUM_FFT_MEASUREMENT_RBW, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch TETRA ACP
 * Purpose:  Returns the measured ACP due to Modulation and ACP due to
 *           Transient results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTETRAACP(
	ViSession	instrSession,
	ViInt32	acp,
	ViInt32	acpMeasurement,
	ViInt32	channelNumber,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, acp, 0, 1),
    		2, "ACP");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, acpMeasurement, 0, 6),
			3, "ACP Measurement");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 0, 6),
			4, "Channel Number");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 2, 4),
			5, "Measurement Type");

	if (acpMeasurement == RSSPECAN_VAL_TETRA_ACP_RBW)
	{
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_FETCH_ACP_MODULATION_RBW, result),
				6, "Result");
	}
	else
	{
		sprintf (buffer, "ACP%s,ACP%s,%s,TETRA%s", tetraACPTypeArr[acp], tetraACPChannelArr[channelNumber], tetraACPMeas[acpMeasurement], tetraMeasModArr[measurementType]);

    	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_FETCH_ACP_MEASUREMENT, result),
    			6, "Result");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get TETRA Trace X Axis Min Max
 * Purpose:  This function returns the minimum and maximum values
 *           respectively for the X-axis data for the specified trace display
 *           and measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTETRATraceXAxisMinMax(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	trace,
	ViInt32	valueType,
	ViReal64*	xAxisValue
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 4),
			3, "Trace");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, valueType, 1, 2),
			4, "Value Type");

    sprintf (buffer, "Win%ld,TR%ld,TETRA%s", window, trace, tetraMeasModArr[valueType]);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TETRA_TRACE_X_AXIS_MIN_MAX, xAxisValue),
    		5, "X Axis Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get TETRA IQ Data Sampling Rate
 * Purpose:  This function returns the sampling rate in Hz for the current
 *           IQ data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTETRAIQDataSamplingRate(
	ViSession	instrSession,
	ViReal64*	iqDataSamplingRate
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TETRA_IQ_SAMPLE_RATE, iqDataSamplingRate),
    		2, "IQ Data Sampling Rate");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get TETRA Result Summary Limit Check Result
 * Purpose:  This function returns the limit check results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTETRAResultSummaryLimitCheckResult(
	ViSession	instrSession,
	ViInt32	limitType,
	ViInt32	mode,
	ViInt32*	result
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 8),
    		2, "Limit Type");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, 2, 3),
			3, "Mode");

    sprintf (buffer, "EVMLim%s,TETRA%s", evmLimitTypeArr[limitType], tetraMeasModArr[mode]);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TETRA_RESULT_SUMMARY_LIMIT_CHECK_RESULT, result),
    		4, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get TETRA Result Summary Limit Check Results All
 * Purpose:  This function returns all limit check results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTETRAResultSummaryLimitCheckResultsAll(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViInt32	results[],
	ViInt32*	returnedValues
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_QueryIntegerArrayToUserBuffer(instrSession, "CALC:LIM:BURS:ALL:RES?", arraySize, results, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get TETRA ACP Limit Check Result
 * Purpose:  This function returns the ACP due to Modulation and ACP Due to
 *           Transients relative power limit check result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTETRAACPLimitCheckResult(
	ViSession	instrSession,
	ViInt32	acp,
	ViInt32	channelNumber,
	ViInt32	measurementType,
	ViInt32*	result
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, acp, 0, 1),
    		2, "ACP");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 0, 6),
			3, "Channel Number");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, 2, 4),
			4, "Measurement Type");

    sprintf (buffer, "ACP%s,ACP%s,TETRA%s", tetraACPTypeArr[acp], tetraACPChannelArr[channelNumber], tetraMeasModArr[measurementType]);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TETRA_ACP_LIMIT_CHECK_RESULT, result),
    		5, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
