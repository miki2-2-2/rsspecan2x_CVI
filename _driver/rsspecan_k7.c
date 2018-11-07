/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - K7 Analog Demod Demodulator
 *  VXIpnp Style Instrument Driver
 *  Original Release: September 2005
 *  By: Martin Koutny (instrument driver core)
 *      Helena Sucha (instrument specific implementation)
 *      Jiri Kominek (DrBase repository & tools)
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"

/*****************************************************************************
 *- RSSpecAn K7 - Analog Demodulator ----------------------------------------*
 *****************************************************************************/

//static ViReal64 rateArr[] = {15.625E3, 31.25E3, 62.5E3, 125E3, 250E3, 500E3, 1E6, 2E6, 4E6, 8E6, 16E6, 32E6};
static ViString ADEMTypeArr[] = {"AM","FM","PM","AM:AFSP","FM:AFSP","PM:AFSP","SPEC","AM:REL","AM:REL:AFSP", NULL};
static ViString resultTypeArr[] = {"OFF", "WRIT", "AVER", "MAXH", "MINH", "VIEW", NULL};
static ViString detectorTypeArr[] = {"PosPeak","NegPeak","AverPeak","RMS"};
static ViString ADemMeasResArr[] = {"XTIM:AM:REL","XTIM:AM","XTIM:SPEC","XTIM:FM",
                                    "XTIM:PM","XTIM:AMS","XTIM:AMS","XTIM:FMS","XTIM:PMS",
                                    "XTIM:FM:AFSP","XTIM:PM:AFSP","XTIM:AM:REL:AFSP","XTIM:RFP:AFSP",
									"XTIM:SUMM",NULL};
/*****************************************************************************
 * Function: Analog Demod Mode
 * Purpose:  This function selects the Analog Demod Demodulator mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ADemodMode(ViSession   instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FMDEM_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Set Analog Demod State
 * Purpose:  This function activates the Analog Demod demodulator of the instrument. The
 *           instrument will be set to timedomain measurement (span = 0) at the
 *           current center frequency. The detector will be set to
 *           SAMPle, the demodulator itself will be set up according to the
 *           parameters of command [SENSe:]ADEMod:SET.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetADemodState(ViSession   instrSession,
                                          ViBoolean   state)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_STATE, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Select Analog Demod Result
 * Purpose:  This function selects Analog Demod demodulator measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectADemodResult (ViSession instrSession,
                                                  ViInt32 operatingMode,
                                                  ViInt32 trace)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViInt32     tmp_trace=3;
    ViInt32     tmp_mode=12;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7"));
    if (RsCore_IsInstrumentModel(instrSession, "FSL") || rsspecan_IsFSV (instrSession))
    {
        tmp_trace=6;
        tmp_mode=11;
    }
/*    else
    {
        if (operatingMode== 3)
            viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Operating Mode");
    }*/
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, operatingMode, 0, tmp_mode),
    		2, "Operating Mode");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 0, tmp_trace),
    		3, "Trace");
    switch (operatingMode)
    {
        case RSSPECAN_VAL_FM_FEED_AM_REL:
        case RSSPECAN_VAL_FM_FEED_AM:
        case RSSPECAN_VAL_FM_FEED_SPEC:
        case RSSPECAN_VAL_FM_FEED_FM:
        case RSSPECAN_VAL_FM_FEED_PM:
             sprintf (cmd, ":CALC1:FEED '%s'\n", ADemMeasResArr[operatingMode]);
        break;
        case RSSPECAN_VAL_FM_FEED_AMS_REL:
            sprintf (cmd, ":CALC1:FEED '%s%ld:REL'\n", ADemMeasResArr[operatingMode], trace);
        break;
        case RSSPECAN_VAL_FM_FEED_AMS:
        case RSSPECAN_VAL_FM_FEED_FMS:
        case RSSPECAN_VAL_FM_FEED_PMS:
        case RSSPECAN_VAL_FM_FEED_FM_AFSP:
        case RSSPECAN_VAL_FM_FEED_PM_AFSP:
        case RSSPECAN_VAL_FM_FEED_AM_REL_AFSP:
        case RSSPECAN_VAL_FM_FEED_RFP_AFSP:
		case RSSPECAN_VAL_FSM_FEED_SUMM:
            sprintf (cmd, ":CALC1:FEED '%s%ld'\n", ADemMeasResArr[operatingMode], trace);
        break;
    }

    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Trigger
 * Purpose:  This function configures the Analog Demodulation Trigger
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_ConfigureADemodTrigger (ViSession instrSession,
                                               ViInt32 triggerSource,
                                               ViReal64 triggerOffset,
                                               ViInt32 triggerPolarity,
                                               ViReal64 triggerLevel)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMDEM_TRIGGER_SOURCE, triggerSource),
    		2, "Trigger Source");

    switch (triggerSource){
        case RSSPECAN_VAL_TRG_IMM:
        case RSSPECAN_VAL_TRG_EXT:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, triggerOffset),
            		3, "Trigger Offset");
        break;
        case RSSPECAN_VAL_TRG_IFP:
           viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, triggerOffset),
           		3, "Trigger Offset");

           viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_IFP_LEVEL, triggerLevel),
           		5, "Trigger Level");
        break;
        case RSSPECAN_VAL_TRG_FM:
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ADEM_TRIGGER_FM_LEVEL, triggerLevel),
                		5, "Trigger Level");
        break;
        case RSSPECAN_VAL_TRG_AM:
             viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ADEM_TRIGGER_AM_LEVEL_ABS, triggerLevel),
             		5, "Trigger Level");
        break;
        case RSSPECAN_VAL_TRG_PM:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ADEM_TRIGGER_PM_LEVEL, triggerLevel),
            		5, "Trigger Level");
        break;
        case RSSPECAN_VAL_TRG_AMR:
             viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ADEM_TRIGGER_AM_LEVEL_REL, triggerLevel),
             		5, "Trigger Level");
        break;
    }

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_SLOPE, triggerPolarity),
    		4, "Trigger Polarity");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod
 * Purpose:  This function configures the Analog Demod demodulator.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodSet(
    ViSession   instrSession,
    ViReal64 sampleRate,
    ViInt32 recordLength,
    ViInt32 triggerSource,
    ViInt32 triggerSlope,
    ViInt32 pretriggerSamples,
    ViInt32 numberOfMeas
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViInt32     rec_len=130560;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7"));

    if (RsCore_IsInstrumentModel(instrSession, "FSL"))
        rec_len=480001;

    viCheckParm(RsCore_InvalidViReal64Range(instrSession, sampleRate, 122.0703125, 32.0e+6), 2, "Sample Rate");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, recordLength, 1, rec_len),
    		3, "Record Length");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, triggerSource, 0, 1),
    		4, "Trigger Source");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, triggerSlope, 0, 1),
    		5, "Trigger Slope");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, pretriggerSamples, -65024, 130560), 6, "Pretrigger Samples");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, numberOfMeas, 0, 32767),
    		7, "Number of Measurements");

    sprintf (cmd, "SENS:ADEM:SET %.12fHZ,%ld,%s,%s,%ld,%ld\n",
        sampleRate, recordLength, rsspecan_rngFmTriggerSource.rangeValues[triggerSource].cmdString,
        rsspecan_rngPolarity.rangeValues[triggerSlope].cmdString, pretriggerSamples, numberOfMeas);

    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Result Type
 * Purpose:  This function sets the result types to be measured
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodResultType(
    ViSession   instrSession,
    ViInt32 demodulationType,
    ViInt32 resultTypes[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, demodulationType, 0, 8),
    		2, "Demodulation Type");

	if (rsspecan_IsFSV (instrSession))
	{
        sprintf (cmd, "SENS:ADEM:%s:TYPE %s,%s,%s,%s,%s,%s",
            ADEMTypeArr[demodulationType], resultTypeArr[resultTypes[0]],
        resultTypeArr[resultTypes[1]], resultTypeArr[resultTypes[2]],resultTypeArr[resultTypes[3]],
		resultTypeArr[resultTypes[4]],resultTypeArr[resultTypes[5]]);
	}
	else
	{
		if (!RsCore_IsInstrumentModel(instrSession, "FSL"))
		{
			sprintf(cmd, "SENS:ADEM:%s:TYPE %s,%s,%s",
				ADEMTypeArr[demodulationType], resultTypeArr[resultTypes[0]],
				resultTypeArr[resultTypes[1]], resultTypeArr[resultTypes[2]]);
		}
		else
		{
			sprintf(cmd, "SENS:ADEM:%s:TYPE %s,%s,%s,%s",
				ADEMTypeArr[demodulationType], resultTypeArr[resultTypes[0]],
				resultTypeArr[resultTypes[1]], resultTypeArr[resultTypes[2]], resultTypeArr[resultTypes[3]]);
		}
	}
    checkErr(RsCore_Write(instrSession, cmd));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Filter
 * Purpose:  This function selects demodulation bandwidth AF filters of Analog Demod
 *           demodulation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilter(
    ViSession   instrSession,
    ViInt32     filterType,
    ViBoolean   filterState,
    ViReal64    filterFrequency,
    ViReal64    amfmDeemphasisValue
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (filterType)
    {
        case RSSPECAN_VAL_FMDEM_FILTER_HP:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_HPAS_STAT, filterState),
            		3, "Filter State");
            if (filterState)
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_HPAS_FREQ, filterFrequency),
                		4, "Filter Frequency");
            break;

        case RSSPECAN_VAL_FMDEM_FILTER_LP:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_LPAS_STAT, filterState),
            		3, "Filter State");
            if (filterState)
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_LPAS_FREQ, filterFrequency),
                		4, "Filter Frequency");
            break;

        case RSSPECAN_VAL_FMDEM_FILTER_DEEMP:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_DEMP_STAT, filterState),
            		3, "Filter State");
            if (filterState)
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_DEMP_TCON, amfmDeemphasisValue),
                		5, "Deemphasis Value");
            break;
        case RSSPECAN_VAL_FMDEM_FILTER_LP_REL:

            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_FILT_LPAS_STAT, filterState),
            		3, "Filter State");
            if (filterState)
            {
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ADEM_FILT_LPAS_FREQ_REL, filterFrequency),
                		4, "Filter Frequency");
            }
        break;

        default: viCheckParm(RsCore_InvalidViInt32Value(instrSession, filterType), 2, "Filter Type");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function selects the cutoff frequency of the low/high pass filter for the specified evaluation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR filterType/This control selects the filter type.
/// HIPAR filterFrequency/This control selects the cutoff frequency of the low pass filter
/// HIPAR filterFrequency/for the specified evaluation.
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilterManual (ViSession instrSession,
                                                        ViInt32 filterType,
                                                        ViReal64 filterFrequency)
{
	ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, filterType, RSSPECAN_VAL_FMDEM_FILTER_HP, RSSPECAN_VAL_FMDEM_FILTER_LP),
			2, "Filter Type");

	if (filterType == RSSPECAN_VAL_FMDEM_FILTER_HP)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Highpass", RSSPECAN_ATTR_ADEM_FILT_FREQ_MANUAL, filterFrequency),
        		3, "Filter Frequency");
    }
	else
	{
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Lowpass", RSSPECAN_ATTR_ADEM_FILT_FREQ_MANUAL, filterFrequency),
        		3, "Filter Frequency");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Filter Weighting
 * Purpose:  This function selects weighting filter.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilterWeighting (ViSession instrSession,
                                                          ViBoolean CCITT,
                                                          ViBoolean CCIR)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_ADEM_FILT_CCITT, CCITT),
    		2, "CCITT");
    if (CCITT == VI_FALSE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_ADEM_FILT_CCIR, CCIR),
        		3, "CCIR");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Filter A Weighting
 * Purpose:  This function activates/deactivates the "A" weighting filter.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilterAWeighting (ViSession instrSession,
                                                            ViBoolean aWeightingFilter)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_ANALOG_DEMOD_FILTER_A_WEIGHTED_ENABLED, aWeightingFilter),
    		2, "A Weighting Filter");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Zoom
 * Purpose:  This function sets analog demodulation parameters such as zoom
 *           state, start time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodZoom(
    ViSession   instrSession,
    ViBoolean   zoomState,
    ViReal64    startTime
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_ZOOM, zoomState),
    		2, "Zoom State");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_ZOOM_START, startTime),
    		3, "Start Time");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Zoom Legth
 * Purpose:  This function configures the zoom length settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodZoomLength (ViSession instrSession,
                                                      ViInt32 zoomLengthMode,
                                                      ViReal64 zoomLength)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMDEM_ZOOM_LENGTH_MODE, zoomLengthMode),
    		2, "Zoom Length Mode");

	if (zoomLengthMode == RSSPECAN_VAL_MAN)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_ZOOM_LENGTH, zoomLength),
				3, "Zoom Length");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Squelch
 * Purpose:  This function activates and configures the squelch function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodSquelch (ViSession instrSession,
                                                        ViBoolean squelchState,
                                                        ViReal64 squelchLevel)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMDEM_SQUELCH, squelchState),
    		2, "Squelch State");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_SQUELCH_LEVEL, squelchLevel),
    		3, "Squelch Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod THD Unit
 * Purpose:  This function selects between % and dB for displaying the
 *           THD measurement result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodTHDUnit (ViSession instrSession,
                                                   ViInt32 unit)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ADEM_UNIT_THD, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Analog Demod Auto Tune
 * Purpose:  This function activates an automatic signal search.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ADemAutoTune (ViSession instrSession,
                                        ViUInt32 timeout)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     old_timeout=-1;

    checkErr(RsCore_LockSession(instrSession));

     checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
        checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ADEM_SIGNAL_SEARCH, ""));

Error:
    if (old_timeout >= 0)
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function selects the trace specified by the index <t> in the
/// HIFN  window specified by the index <n>. Only traces that are active in the
/// HIFN  specified result display can be selected. The selected trace is used to
/// HIFN  determine the "Result Summary" for the corresponding result display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
ViStatus _VI_FUNC rsspecan_SelectAnalogDemodTrace(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	trace
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, window, 1, 16),
    		2, "Window");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 6),
			3, "Trace");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);

    checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_ADEM_SELECT_TRACE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function can configure the Analog Demodulation application to use
/// HIFN  predefined standard settings. This allows for quick and easy
/// HIFN  configuration for commonly performed measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR operation/This control selects the operation to be performed.
/// HIPAR standard/Specifies the file name. If you have stored the file in a subdirectory
/// HIPAR standard/of the directory mentioned above, you have to include the relative path
/// HIPAR standard/to the file.
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPredefinedStandard(
	ViSession	instrSession,
	ViInt32	operation,
	ViString	standard
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (operation)
	{
        case RSSPECAN_VAL_ADEM_PRESET_LOAD:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ADEM_PRESET_LOAD, standard),
            		3, "Standard");
        break;
        case RSSPECAN_VAL_ADEM_PRESET_STORE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ADEM_PRESET_STORE, standard),
            		3, "Standard");
        break;
        case RSSPECAN_VAL_ADEM_PRESET_RESTORE:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ADEM_PRESET_RESTORE, NULL));
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, operation), 2, "Operation");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the online output of analog demodulator.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR outputState/Enables or disables online demodulation output to the IF/VIDEO/DEMOD
/// HIPAR outputState/output connector on the rear panel of the R&S FSW.
/// HIPAR outputSelection/Selects the result display whose results are output.
/// HIPAR ACCutoffFrequency/Defines the cutoff frequency for the AC highpass filter.
/// HIPAR phonesOutput/In addition to sending the output to the IF/VIDEO/DEMOD OUTPUT connector
/// HIPAR phonesOutput/(on the rear panel of the R&S FSW), it can also be output to headphones
/// HIPAR phonesOutput/connected on the front panel (PHONES connector).
ViStatus _VI_FUNC rsspecan_ConfigureADemodOnlineOutput (ViSession instrSession,
                                                        ViBoolean outputState,
                                                        ViString outputSelection,
                                                        ViReal64 ACCutoffFrequency,
                                                        ViBoolean phonesOutput)
{
	ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ANALOG_DEMOD_OUTPUT_STATE, outputState),
    		2, "Output State");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ANALOG_DEMOD_OUTPUT_SELECTION, outputSelection),
			3, "Output Selection");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ANALOG_DEMOD_OUTPUT_CUTOFF_FREQUENCY, ACCutoffFrequency),
			4, "AC Cutoff Frequency");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_ANALOG_DEMOD_PHONE_OUTPUT, phonesOutput),
			5, "Phones Output");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod PM Phase Wrap
 * Purpose:  This function sets PM phase wrap
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPMPhaseWrap (ViSession instrSession,
                                                    ViInt32 phaseWrap)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ADEM_PHASE_WRAP, phaseWrap),
    		2, "Phase Wrap");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod PM Phase Units
 * Purpose:  This function sets PM phase units
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPMPhaseUnits (ViSession instrSession,
                                                        ViInt32 PMUnits)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ADEM_UNIT_ANGLE, PMUnits),
    		2, "PM Units");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod PM Zero Phase Reference Point
 * Purpose:  This function sets PM zero phase reference point
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPMZeroPhaseReferencePoint (ViSession instrSession,
                                                                          ViReal64 referencePoint)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ADEM_PM_RPO_X, referencePoint),
    		2, "Reference Point");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod Bandwidth
 * Purpose:  This function configures analog demodulator bandwidth parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodBandwidth(ViSession   instrSession,
                                                    ViReal64    demodulationBandwidth)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_BAND_DEM, demodulationBandwidth),
    		2, "Demodulation Bandwidth");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod AF Coupling
 * Purpose:  This function configures analog demodulator AF Coupling.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFCoupling (ViSession instrSession,
                                                      ViInt32 afCoupling)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_COUP, afCoupling),
    		2, "AF Coupling");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Configure Analog Demod Measuring Time
 * Purpose:  This function configures analog demodulator measuring time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodMeasuringTime (ViSession instrSession,
                                                         ViReal64 measuringTime)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_MTIM, measuringTime),
    		2, "Measuring Time");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod AF Frequency Start Stop
 * Purpose:  This function configures the frequency range of the result
 *           display AF spectrum using start frequency and stop frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFFrequencyStartStop(
    ViSession   instrSession,
    ViReal64    startFrequency,
    ViReal64    stopFrequency
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_START, startFrequency),
    		2, "Start Frequency");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_STOP, stopFrequency),
    		3, "Stop Frequency");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod AF Frequency Center Span
 * Purpose:  This function configures the frequency range of the result
 *           display AF spectrum using the center frequency and the frequency
 *           span. This function modifies the AF Frequency Start and AF
 *           Frequency Stop attributes as follows:
 *
 *               AF Frequency Start = CenterFrequency - span/2
 *               AF Frequency Stop = CenterFrequency + span/2
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFFrequencyCenterSpan(
    ViSession   instrSession,
    ViReal64    centerFrequency,
    ViReal64    span
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_CENTER, centerFrequency),
    		2, "Center Frequency");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_SPAN, span),
    		3, "Span");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
/*****************************************************************************
 * Function: Configure Analog Demod AF Full Span
 * Purpose:  This function sets the maximum span for the display of the AF spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFFullSpan(ViSession   instrSession)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_FULL_SPAN, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod AF Param
 * Purpose:  This function configures bandwidth resolution and span for
 *           display of the AF spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFParam(
    ViSession   instrSession,
    ViReal64    resolutionBandwidth)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_BAND_RES, resolutionBandwidth),
    		2, "Resolution Bandwidth");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
/*****************************************************************************
 * Function: Configure Analog Demod RF Param
 * Purpose:  This function configures bandwidth resolution and span for
 *           display of the RF spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodRFParam(
    ViSession   instrSession,
    ViReal64    resolutionBandwidth,
    ViReal64    span
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_BAND_RES, resolutionBandwidth),
    		2, "Resolution Bandwidth");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_SPEC_SPAN, span),
    		3, "Span");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog Demod RF Zoom
 * Purpose:  This function configures the frequency range for result display
 *           of RF spectrum determined from Analog Demod demodulation data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureADemodRFZoom(
    ViSession   instrSession,
    ViReal64    zoom
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_SPEC_ZOOM, zoom),
    		2, "Zoom");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Analog Demod Currently Set Value
 * Purpose:  This function reads selected value from instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetADemodCurrentlySetValue(
    ViSession   instrSession,
    ViInt32 value,
    ViReal64*   result
)
{
    ViStatus    error   = VI_SUCCESS;
    ViReal64    l_value = 0;

    checkErr(RsCore_LockSession(instrSession));

    switch (value)
    {
        case RSSPECAN_VAL_FMDEM_RLEN:
            viCheckParm(rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_FMDEM_RLEN, &l_value),
                                                    3, "Result");
            break;

        case RSSPECAN_VAL_FMDEM_SRATE:
            viCheckParm(rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_FMDEM_SRATE, &l_value),
                                                    3, "Result");
        break;
        default: viCheckParm(RsCore_InvalidViInt32Value(instrSession, value), 2, "Value");
    }

    if (result)
        *result = l_value;
    else
        viCheckParm(RS_ERROR_INVALID_PARAMETER, 3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Analog Demod Result Values
 * Purpose:  This function returns analog demodulation result values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetADemodResultValues(
    ViSession   instrSession,
    ViInt32 demodulationType,
    ViInt32 resultType,
    ViUInt32   timeout,
    ViInt32 arraySize,
    ViReal64  resultValues[],
    ViInt32*    returnedValues
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, demodulationType, 0, 8),
    		2, "Demodulation Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, 1, 4),
    		3, "Result Type");
    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 4, "Timeout");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":SENS:ADEM:%s:RES? %s", ADEMTypeArr[demodulationType], resultTypeArr[resultType]);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, arraySize, resultValues, returnedValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Analog Demod Offset
 * Purpose:  This function calculates the Analog Demod offset of the current measured
 *           data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetADemodOffset(
    ViSession   instrSession,
    ViInt32 resultType,
    ViReal64*   value
)
{
    ViStatus    error   = VI_SUCCESS;
    ViString    fmOffsetArr[]={"Imm","Aver"};
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, 0, 1),
    		2, "Result Type");
    
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "FMOff%s", fmOffsetArr[resultType]);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_ADEM_FM_OFFSET, value),
    		3, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);

    return error;
}

/*****************************************************************************
 * Function: Get Analog Demod Marker Modulation Value
 * Purpose:  This function returns analog modulation measurement summary.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetADemodMarkerModulationValue(
    ViSession   instrSession,
    ViInt32 markerNumber,
    ViInt32 demodulationType,
    ViInt32 detectorType,
    ViInt32 trace,
    ViReal64*   value
)
{
    ViStatus    error   = VI_SUCCESS;
	ViChar      repCap[RS_REPCAP_BUF_SIZE];
    ViInt32		tmp_trace = 3;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, demodulationType, 0, 2),
    		3, "Demodulation Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, detectorType, 0, 3),
    		4, "Detector Type");

    if (RsCore_IsInstrumentModel(instrSession, "FSL") || rsspecan_IsFSV (instrSession))
    {
        tmp_trace=6;
    }

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, tmp_trace),
			5, "Trace");

    snprintf (repCap, RS_REPCAP_BUF_SIZE, "M%ld,Analog%s,TR%ld,ResDet%s",
        markerNumber,ADEMTypeArr[demodulationType], trace,detectorTypeArr[detectorType]);
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_ADEM_SUMM_RES, value),
    		6, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);

    return error;
}

/*****************************************************************************
 * Function: Get Analog Demod Marker Measurement Value
 * Purpose:  This function returns analog modulation measurement summary.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetADemodMarkerMeasValue(
    ViSession   instrSession,
    ViInt32 markerNumber,
    ViInt32 measurement,
    ViInt32 trace,
    ViReal64*   value
)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      repCap[RS_REPCAP_BUF_SIZE];
    ViInt32     attribute;
	ViInt32		tmp_trace = 3;
    ViInt32     attrs[] = {RSSPECAN_ATTR_FMDEM_AFR_RES,RSSPECAN_ATTR_FMDEM_FERR_RES,
                           RSSPECAN_ATTR_FMDEM_SIN_RES, RSSPECAN_ATTR_FMDEM_THD_RES,
                           RSSPECAN_ATTR_FMDEM_CARR_RES};

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
			2, "Marker Number");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 4),
    		3, "Measurement");
    if (RsCore_IsInstrumentModel(instrSession, "FSL") || rsspecan_IsFSV (instrSession))
    {
        tmp_trace=6;
    }

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, tmp_trace),
			4, "Trace");

    attribute = attrs[measurement];

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "M%ld,TR%ld", markerNumber, trace);
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, attribute, value),
    		6, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- Callback Definition   ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- End Of RSSpecAn K7 - Analog Demodulator -------------------------------------*
 *****************************************************************************/
