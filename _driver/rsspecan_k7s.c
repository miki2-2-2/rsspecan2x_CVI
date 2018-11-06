/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - K7S FM Stereo
 *  VXIpnp Style Instrument Driver
 *  Original Release: November 2009
 *  By: Martin Krcmar
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"


static ViString ChannelTypeArr[] = {"FMSChanLeft","FMSChanRight","FMSChanMPX","FMSChanMono",
                                 "FMSChanStereo","FMSChanRDS","FMSChanPilot"};
static ViReal64 highPassArr[] = {0.0,20.0,50.0,300.0};
static ViReal64 lowPassArr[] = {10.0,15.0e3,150.0e3,23.0e3,25.0,3.0e3,5.0};
static ViBoolean weightingArr[5][4] = {{VI_FALSE, VI_FALSE, VI_FALSE, VI_FALSE},
                                       {VI_TRUE, VI_FALSE, VI_FALSE, VI_FALSE},
                                       {VI_FALSE, VI_TRUE, VI_FALSE, VI_FALSE},
                                       {VI_FALSE, VI_FALSE, VI_TRUE, VI_FALSE},
                                       {VI_FALSE, VI_FALSE, VI_FALSE, VI_TRUE}};
static ViReal64 deemphasisArr[] = {0.0,25.0e-6,50.0e-6,75.0e-6,750.0e-6};
static ViString traceModesArr[] = {"OFF", "WRIT", "AVER", "MAXH", "MINH", "VIEW", NULL};
static ViString measTypeArr[] = {"FMSMeasADEV","FMSMeasRDEV","FMSMeasSINAD","FMSMeasTHD","FMSMeasAFREQ","FMSMeasXTAL"};
static ViString resultTypeArr[] = {"AFSP","TDO"};

/*****************************************************************************
 * Function: FM Stereo Mode
 * Purpose:  This function selects the FM Stereo mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FMStereoMode(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FMS_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Set FM Stereo State
 * Purpose:  This function switches between Stereo FM and Spectrum mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetFMStereoState(
    ViSession   instrSession,
    ViBoolean   state
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMS_STATE, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Trigger
 * Purpose:  This function configures the FM Stereo trigger settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTrigger(
    ViSession   instrSession,
    ViInt32 triggerSource,
    ViReal64    triggerLevel
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMS_TRIGGER_SOURCE, triggerSource),
    		2, "Trigger Source");

    switch (triggerSource)
    {
        case RSSPECAN_VAL_FMS_TRG_IMM:
        break;
        case RSSPECAN_VAL_FMS_TRG_EXT:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, triggerLevel),
            		3, "Trigger Level");
        break;
        case RSSPECAN_VAL_FMS_TRG_IFP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TRIGGER_IFP_LEVEL, triggerLevel),
            		3, "Trigger Level");
        break;
        case RSSPECAN_VAL_FMS_TRG_TIME:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TRIGGER_TIME_INTERVAL, triggerLevel),
            		3, "Trigger Level");
        break;
        case RSSPECAN_VAL_FMS_TRG_LEFT:
        case RSSPECAN_VAL_FMS_TRG_RIGH:
        case RSSPECAN_VAL_FMS_TRG_MPX:
        case RSSPECAN_VAL_FMS_TRG_MONO:
        case RSSPECAN_VAL_FMS_TRG_STER:
        case RSSPECAN_VAL_FMS_TRG_RDS:
        case RSSPECAN_VAL_FMS_TRG_PIL:
            sprintf (buffer, "%s", ChannelTypeArr[triggerSource - 4]);

            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FMS_TRIGGER_LEVEL, triggerLevel),
            		3, "Trigger Level");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, triggerSource), 2, "Trigger Source");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Phase Noise Marker
 * Purpose:  This function configures phase noise marker measurement. To
 *           setup reference point use function
 *           rsspecan_ConfigureReferenceFixedPoint.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoPhaseNoiseMarker(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     marker,
    ViBoolean   state
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
    		3, "Marker");

    sprintf (buffer, "Win%ld,M%ld", window, marker);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_PHASE_NOISE_MARKER_STATE, state),
    		4, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Difference Frequency Distortion
 * Purpose:  This function configures difference frequency distortion
 *           measurement. This function is only available for AF spectrum
 *           measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoDifferenceFrequencyDistortion(
    ViSession   instrSession,
    ViInt32     window,
    ViBoolean   state,
    ViInt32     unit
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_DIFFERENCE_FREQUENCY_DISTORTION_STATE, state),
    		2, "State");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMS_DIFFERENCE_FREQUENCY_DISTORTION_UNIT, unit),
    		3, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: FM Stereo Difference Frequency Distortion Search Signal
 * Purpose:  This function starts the search of the signals required for the
 *           difference frequency distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FMStereoDifferenceFrequencyDistortionSearchSignal(
    ViSession   instrSession,
    ViInt32     window,
    ViUInt32    timeout
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar  buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViInt32     old_timeout = -1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 600000UL), 3, "Timeout");

    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

    sprintf (buffer, "Win%ld", window);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FMS_DIFFERENCE_FREQUENCY_DISTORTION_SEARCH_SIGNAL, NULL));

Error:
    if (old_timeout >= 0)
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Intermodulation Distortion
 * Purpose:  This function configures intermodulation distortion
 *           measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoIntermodulationDistortion(
    ViSession   instrSession,
    ViInt32     window,
    ViBoolean   state,
    ViInt32     unit
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_INTERMODULATION_DISTORTION_STATE, state),
    		2, "State");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMS_INTERMODULATION_DISTORTION_UNIT, unit),
    		3, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: FM Stereo Intermodulation Distortion Search Signal
 * Purpose:  This function starts the search of the signals required for the
 *           intermodulation distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FMStereoIntermodulationDistortionSearchSignal(
    ViSession   instrSession,
    ViInt32     window,
    ViUInt32    timeout
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar  buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViInt32     old_timeout = -1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 600000UL), 3, "Timeout");

    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

    sprintf (buffer, "Win%ld", window);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FMS_INTERMODULATION_DISTORTION_SEARCH_SIGNAL, NULL));

Error:
    if (old_timeout >= 0)
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: FM Stereo AF Auto Scale
 * Purpose:  This function activates automatic scaling of the y-axis for FM
 *           stereo measurements. Currently auto-scaling is only available
 *           for AF measurements. RF power and RF spectrum measurements are
 *           not affected by the auto-scaling.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFAutoScale(
    ViSession   instrSession,
    ViBoolean   state
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMS_AF_AUTO_SCALE, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Display Config
 * Purpose:  This function selects the evaluation method of the measured
 *           data that is to be displayed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoDisplayConfig(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     displayConfiguration
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_FMS_DISPLAY_CONFIG, displayConfiguration),
    		3, "Display Configuration");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Reference Deviation
 * Purpose:  This function configures the reference deviation required for
 *           relative deviation measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoReferenceDeviation(
    ViSession   instrSession,
    ViReal64    referenceDeviation
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMS_REFERENCE_DEVIATION, referenceDeviation),
    		2, "Reference Deviation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Result Summary Setup
 * Purpose:  This function configures result summary display.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoResultSummarySetup(
    ViSession   instrSession,
    ViInt32     channelType,
    ViInt32     detector,
    ViInt32     mode,
    ViBoolean   coupled
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT),
    		2, "Channel Type");

    sprintf (buffer, "%s", ChannelTypeArr[channelType]);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_FMS_RESULT_SUMMARY_DETECTOR, detector),
    		3, "Detector");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_FMS_RESULT_SUMMARY_MODE, mode),
    		4, "Mode");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_RESULT_SUMMARY_COUPLED, coupled),
    		5, "Coupled");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Meas To Ref
 * Purpose:  This function determines the reference deviation from the
 *           current channel measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoMeasToRef(
    ViSession   instrSession,
    ViInt32     channelType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT),
    		2, "Channel Type");

    sprintf (buffer, "%s", ChannelTypeArr[channelType]);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FMS_MEAS_TO_REF, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Measurement Time
 * Purpose:  This function configures analog demodulator measuring time
 *           parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoMeasurementTime(
    ViSession   instrSession,
    ViReal64    measuringTime
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_MTIM, measuringTime),
    		2, "Measuring Time");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo AF Filter
 * Purpose:  This function configures different filter settings for each
 *           channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFFilter(
    ViSession   instrSession,
    ViInt32     channelType,
    ViInt32     highPass,
    ViInt32     lowPass,
    ViInt32     weighting,
    ViInt32     deemphasis,
    ViBoolean   coupled
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT),
    		2, "Channel Type");

    sprintf (buffer, "%s", ChannelTypeArr[channelType]);

    if (highPass == RSSPECAN_VAL_FMS_HPAS_FILT_NONE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_HIGH_PASS_STATE, VI_FALSE),
        		3, "High Pass");
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_HIGH_PASS_STATE, VI_TRUE),
        		3, "High Pass");

        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_HIGH_PASS_FREQUENCY, highPassArr[highPass]),
        		3, "High Pass");
    }

    if (lowPass == RSSPECAN_VAL_FMS_LPAS_FILT_NONE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_STATE, VI_FALSE),
        		4, "Low Pass");
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_STATE, VI_TRUE),
        		4, "Low Pass");

        if ((lowPass == RSSPECAN_VAL_FMS_LPAS_FILT_5PCT) ||
           (lowPass == RSSPECAN_VAL_FMS_LPAS_FILT_10PCT) ||
           (lowPass == RSSPECAN_VAL_FMS_LPAS_FILT_25PCT))
        {
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_FREQUENCY_RELATIVE, lowPassArr[lowPass]),
            		4, "Low Pass");
        }
        else
        {
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_FREQUENCY_ABSOLUTE, lowPassArr[lowPass]),
            		4, "Low Pass");
        }
    }

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_CCITT, weightingArr[weighting][0]),
    		5, "Weighting");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_CCIR_UNWEIGHTED, weightingArr[weighting][1]),
    		5, "Weighting");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_CCIR_WEIGHTED, weightingArr[weighting][2]),
    		5, "Weighting");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_A_WEIGHTED, weightingArr[weighting][3]),
			5, "Weighting");

    if (deemphasis == RSSPECAN_VAL_FMS_DEEMPH_NONE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_DEEMPHASIS_STATE, VI_FALSE),
        		6, "Deemphasis");
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_DEEMPHASIS_STATE, VI_TRUE),
        		6, "Deemphasis");

        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_DEEMPHASIS_TIME_CONSTANT, deemphasisArr[deemphasis]),
        		6, "Deemphasis");
    }
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FMS_AF_FILTER_COUPLED, coupled),
    		7, "Coupled");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: All FM Stereo AF Filters Off
 * Purpose:  This function switches all AF Filters off at once.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AllFMStereoAFFiltersOff(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FMS_ALL_AF_FILTERS_OFF, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo AF Range
 * Purpose:  This function configures deviation settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFRange(
    ViSession   instrSession,
    ViInt32     deviation,
    ViReal64    devperDivision
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VERTICAL_SCALE, deviation),
    		2, "Deviation");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DISP_Y_AXIS_SCALING, devperDivision),
    		3, "Dev Per Division");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo AF Range THD Unit
 * Purpose:  This function selects between % and dB for displaying the
 *           THD measurement result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFRangeTHDUnit(
    ViSession   instrSession,
    ViInt32     unit
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ADEM_UNIT_THD, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo AF Range Dev Unit
 * Purpose:  This function configures the deviation units.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFRangeDevUnit(
    ViSession   instrSession,
    ViInt32 absoluteDevUnit,
    ViInt32 relativeDevUnit
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMS_AF_RANGE_DEV_UNIT_ABSOLUTE, absoluteDevUnit),
    		2, "Absolute Dev Unit");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FMS_AF_RANGE_DEV_UNIT_RELATIVE, relativeDevUnit),
    		3, "Relative Dev Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Time Domain Zoom
 * Purpose:  This function activate and configure the zoom function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTimeDomainZoom(
    ViSession   instrSession,
    ViBoolean   zoomState,
    ViReal64    startTime
)
{
    ViStatus    error = VI_SUCCESS;

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
 * Function: Configure FM Stereo Time Domain Zoom Length
 * Purpose:  This function configures the zoom length settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTimeDomainZoomLength(
    ViSession   instrSession,
    ViInt32     zoomLengthMode,
    ViReal64    zoomLength
)
{
    ViStatus    error = VI_SUCCESS;

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
 * Function: Configure FM Stereo Squelch
 * Purpose:  This function activates and configures the squelch function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoSquelch (ViSession instrSession,
                                                     ViBoolean squelchState,
                                                     ViReal64 squelchLevel)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FMS_SQUELCH, squelchState),
    		2, "Squelch State");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMS_SQUELCH_LEVEL, squelchLevel),
    		3, "Squelch Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Carrier Frequency
 * Purpose:  This function queries the carrier frequency of the stereo
 *           measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoCarrierFrequency(
    ViSession   instrSession,
    ViReal64*   carrierFrequency
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_QUERY_FMS_CARRIER_FREQUENCY, carrierFrequency),
    		2, "Carrier Frequency");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure FM Stereo Trace Modes
 * Purpose:  This function selects the trace modes of the FM stereo to be
 *           measured simultaneously. For each trace a mode can be defined,
 *           however only if the specified channel is currently displayed in
 *           one of the four screens. If a trace mode is set for a channel
 *           that is not displayed, a query error is generated.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTraceModes(
    ViSession   instrSession,
    ViInt32     channelType,
    ViInt32     measurementType,
    ViInt32     traceModes[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7S"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT),
    		2, "Channel Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, RSSPECAN_VAL_FMS_AFSP, RSSPECAN_VAL_FMS_TDOM),
    		3, "Measurement Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, traceModes[0], RSSPECAN_VAL_FMS_RESULT_OFF, RSSPECAN_VAL_FMS_RESULT_VIEW),
    		4, "Trace Modes");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, traceModes[1], RSSPECAN_VAL_FMS_RESULT_OFF, RSSPECAN_VAL_FMS_RESULT_VIEW),
    		4, "Trace Modes");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, traceModes[2], RSSPECAN_VAL_FMS_RESULT_OFF, RSSPECAN_VAL_FMS_RESULT_VIEW),
    		4, "Trace Modes");

    if (measurementType == RSSPECAN_VAL_FMS_AFSP)
    {
        sprintf (buffer, "SENS:SFM:%s:AFSP:TYPE %s,%s,%s",
            ChannelTypeArr[channelType], traceModesArr[traceModes[0]],
            traceModesArr[traceModes[1]], traceModesArr[traceModes[2]]);
    }
    else
    {
        sprintf (buffer, "SENS:SFM:%s:TDO:TYPE %s,%s,%s",
            ChannelTypeArr[channelType], traceModesArr[traceModes[0]],
            traceModesArr[traceModes[1]], traceModesArr[traceModes[2]]);
    }

    checkErr(RsCore_Write(instrSession, buffer));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Phase Noise Result
 * Purpose:  This function queries the result of the phase noise measurement
 *           in the selected measurement window. The measurement will be
 *           switched on, if necessary.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoPhaseNoiseResult(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     marker,
    ViReal64*   phaseNoiseResult
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar  buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
    		3, "Marker");

    sprintf (buffer, "Win%ld,M%ld", window, marker);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_QUERY_FMS_PHASE_NOISE_RESULT, phaseNoiseResult),
    		4, "Phase Noise Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Difference Frequency Distortion Result
 * Purpose:  This function queries the result of the difference frequency
 *           distortion measurement in the specified window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoDifferenceFrequencyDistortionResult(
    ViSession   instrSession,
    ViInt32     window,
    ViReal64    differenceFrequencyDistortion[]
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7S"));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:DFD:RES?", window);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, 2, differenceFrequencyDistortion, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Intermodulation Distortion Result
 * Purpose:  This function queries the result of the intermodulation
 *           distortion measurement in the specified window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoIntermodulationDistortionResult(
    ViSession   instrSession,
    ViInt32     window,
    ViReal64    intermodulationDistortion[]
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7S"));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:IMD:RES?", window);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, 2, intermodulationDistortion, NULL));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Measurement Results
 * Purpose:  This function queries the results of the stereo measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoMeasurementResults(
    ViSession   instrSession,
    ViReal64    absoluteDeviation[],
    ViReal64    relativeDeviation[],
    ViReal64    sinad[],
    ViReal64    thd[],
    ViReal64    modulationFrequency[]
)
{
    ViStatus    error = VI_SUCCESS;
	ViReal64*   data = NULL;
	ViInt32		dataSize, i;
	ViInt32		j = 0;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7S"));

	checkErr(RsCore_QueryBinaryOrAsciiFloatArray(instrSession, "CALC:MARK:FUNC:SFM:RES? SUMM", &data, &dataSize));

	dataSize /= 5;

	for (i = 0; i < dataSize; i++)
	{
		absoluteDeviation[i] = data[j++];
		relativeDeviation[i] = data[j++];
		sinad[i] = data[j++];
		thd[i] = data[j++];
		modulationFrequency[i] = data[j++];
	}

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (data) free(data);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Channel Type Results
 * Purpose:  This function queries the results of the measurement type for
 *           the selected channel in the specified window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoChannelTypeResults(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     channelType,
    ViInt32     measurementType,
    ViReal64*   result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_XTALK),
    		3, "Channel Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, RSSPECAN_VAL_FMS_MEAS_TYPE_ADEV, RSSPECAN_VAL_FMS_MEAS_TYPE_AFR),
    		4, "Measurement Type");

    sprintf (buffer, "Win%ld,%s,%s", window, ChannelTypeArr[channelType], measTypeArr[measurementType]);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_QUERY_FMS_CHANNEL_TYPE_RESULT, result),
    		5, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Channel Type All Results
 * Purpose:  This function queries all available measurement values for the
 *           selected channel in the specified window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoChannelTypeAllResults(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     channelType,
    ViReal64    result[]
)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];
    window;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7S"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT),
			3, "Channel Type");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:MARK:FUNC:SFM:%s:RES? ALL", ChannelTypeArr[channelType]);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, 32001, result, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query FM Stereo Result Values
 * Purpose:  This function reads the result data of the FM stereo signal in
 *           the specified trace mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryFMStereoResultValues(
    ViSession   instrSession,
    ViInt32     channelType,
    ViInt32     measurementType,
    ViInt32     traceMode,
    ViUInt32    timeout,
    ViInt32     arraySize,
    ViReal64    resultValues[],
    ViInt32*    returnedValues
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K7S"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT, RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT),
    		2, "Channel Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, RSSPECAN_VAL_FMS_AFSP, RSSPECAN_VAL_FMS_TDOM),
    		3, "Measurement Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, traceMode, RSSPECAN_VAL_FMS_RESULT_CURR, RSSPECAN_VAL_FMS_RESULT_MIN),
    		4, "Trace Mode");
    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 5, "Timeout");

    snprintf (cmd, RS_MAX_MESSAGE_BUF_SIZE, ":SENS:SFM:%s:%s:RES? %s", ChannelTypeArr[channelType], resultTypeArr[measurementType], traceModesArr[traceMode]);
	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, cmd, timeout, arraySize, resultValues, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

