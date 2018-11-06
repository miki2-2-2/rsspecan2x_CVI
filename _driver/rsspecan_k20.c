/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - K20 Cable TV
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
 *- RSSpecAn K20 -  Cable TV Analyzer ---------------------------------------*
 *****************************************************************************/

//static ViReal64 rateArr[] = {15.625E3, 31.25E3, 62.5E3, 125E3, 250E3, 500E3, 1E6, 2E6, 4E6, 8E6, 16E6, 32E6};
static ViString DTVMeasArr[]={"MERRms","MERPeak","EVMRms","EVMPeak"};
static ViString ATVCarrierResultArr[]={"RefPow","NoiseFloorCorr","FreqRel","","NoiseRefBw"};

/*****************************************************************************
 * Function: CATV Mode
 * Purpose:  This function sets the Cable TV Measurements operating mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVMode (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure CATV Channel Setup
 * Purpose:  This function selects TV channel table.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVChannelSetup (ViSession instrSession,
                                                      ViString channelTableName,
                                                      ViInt32 measurementChannel)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_TV_CHANNEL, channelTableName),
    		2, "Channel Table Name");

    if (strlen(channelTableName)>0)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_CHANNEL, measurementChannel),
        		3, "Measurement Channel");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure CATV Channel Frequency
 * Purpose:  This function configures TV channel frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVChannelFrequency (ViSession instrSession,
                                                          ViReal64 RFFrequency,
                                                          ViInt32 axisLabeling)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_RF_FREQUENCY, RFFrequency),
    		2, "RF Frequency");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_SWEEP_SPACING, axisLabeling),
    		3, "Axis Labeling");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure CATV Channel Frequency Step Size
 * Purpose:  This function configures TV channel frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVChannelFrequencyStepSize (ViSession instrSession,
                                                                  ViReal64 stepSize)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_RF_FREQUENCY_STEP_SIZE, stepSize),
    		2, "Step Size");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure CATV Modulation Standard
 * Purpose:  This function configures modulation standard.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVModulationStandard (ViSession instrSession,
                                                            ViString modulationStandard,
                                                            ViInt32 signalType,
                                                            ViInt32 sidebandPosition)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_MODUL_STANDARD, modulationStandard),
    		2, "Modulation Standard");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_MODUL_STANDARD_SIG_TYPE, signalType),
    		3, "Signal Type");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_SIDE_BAND, sidebandPosition),
    		4, "Sideband Position");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: CATV Channel Adjust Attenuation
 * Purpose:  This function adjusts the input attenuator.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVChannelAdjustAtt (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "Win0", RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: CATV Autoscale
 * Purpose:  This function activates the automatic scaling for the Hum and Tilt
 *           measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVAutoscale (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_Y_SCALE_AUTO, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure CATV Marker State
 * Purpose:  This function enables or disables selected marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVMarkerState (ViSession instrSession,
                                                     ViInt32 markerNumber,
                                                     ViBoolean markerState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    sprintf (buffer, "Win0,M%ld", markerNumber);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_MARKER_ENABLED, markerState),
    		2, "Marker State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure CATV Delta Marker State
 * Purpose:  This function enables or disables selected delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDeltaMarkerState (ViSession instrSession,
                                                        ViInt32 deltaMarkerNumber,
                                                        ViBoolean deltaMarkerState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarkerNumber, 1, 4),
    		2, "Delta Marker Number");
    sprintf (buffer, "Win0,DM%ld", deltaMarkerNumber);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_STATE, deltaMarkerState),
    		2, "Delta Marker State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Measurement
 * Purpose:  This function defines the analog TV measurement type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAMeasurement (ViSession instrSession,
                                                      ViInt32 measurement)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_MEAS, measurement),
    		2, "Measurement");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Modulation Standard
 * Purpose:  This function defines the analog TV standard options.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAModulStandard (ViSession instrSession,
                                                        ViInt32 TVStandard,
                                                        ViInt32 soundSystem,
                                                        ViInt32 groupDelay,
                                                        ViInt32 colorSystem,
                                                        ViInt32 barField,
                                                        ViInt32 barLine,
                                                        ViInt32 barLineType,
                                                        ViInt32 quietField,
                                                        ViInt32 quietLine)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, TVStandard, 0, 6),
    		2, "Delta Marker Number");

    switch (TVStandard){
        case RSSPECAN_VAL_ATV_STAN_BG:    //B/G
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 0, 2),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 0, 7),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 0, 2),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 0, 0),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 1),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 1),
            		9, "Quiet Field");
        break;
        case RSSPECAN_VAL_ATV_STAN_DK:   //D/K
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 3, 6),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 7, 10),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 0, 2),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 0, 0),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 1),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 1),
            		9, "Quiet Field");
        break;
        case RSSPECAN_VAL_ATV_STAN_I:    //I
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 7, 8),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 7, 7),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 0, 0),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 0, 0),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 1),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 1),
            		9, "Quiet Field");
        break;
        case RSSPECAN_VAL_ATV_STAN_K1:     //K1
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 3, 6),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 7, 11),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 2, 2),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 0, 0),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 1),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 1),
            		9, "Quiet Field");
        break;
        case RSSPECAN_VAL_ATV_STAN_L:     //L
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 9, 10),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 7, 11),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 2, 2),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 0, 0),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 1),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 1),
            		9, "Quiet Field");
        break;
        case RSSPECAN_VAL_ATV_STAN_M:      //M
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 11, 14),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 7, 12),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 0, 1),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 1, 2),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 2),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 2),
            		9, "Quiet Field");
        break;
        case RSSPECAN_VAL_ATV_STAN_N:      //N
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, soundSystem, 11, 14),
            		3, "Sound System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, groupDelay, 7, 12),
            		4, "Group Delay");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorSystem, 0, 0),
            		5, "Color System");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barLineType, 0, 0),
            		8, "Bar Line Type");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, barField, 1, 1),
            		6, "Bar Field");
            viCheckParm(RsCore_InvalidViInt32Range(instrSession, quietField, 1, 1),
            		9, "Quiet Field");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, TVStandard), 2, "TV Standard");
    }

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_STANDARD, TVStandard),
    		2, "TV STandard");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_SOUND_SYSTEM, soundSystem),
    		3, "Sound System");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_MODUL_STANDARD_GDELAY, groupDelay),
    		4, "Group Delay");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_COLOR, colorSystem),
    		5, "Color System");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_BAR_FIELD, barField),
    		6, "Bar Field");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_BAR_LINE, barLine),
    		7, "Bar Line");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_BAR_LINE_TYPE, barLineType),
    		7, "Bar Line Type");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_QUIET_LINE, quietLine),
    		8, "Quiet Line");

    viCheckParm(rsspecan_SetAttributeViInt32 (instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_QLINE_FIELD,
                    quietField), 9, "Quiet Field")

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Video Scope Line
 * Purpose:  This function configures the Video Scope line
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVideoScopeLine (ViSession instrSession,
                                                         ViInt32 line)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_VIDEO_SCOPE_LINE, line),
    		2, "Line");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Configure Analog TV Video Scope Field
 * Purpose:  This function configures the Video Scope field
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVideoScopeField (ViSession instrSession,
                                                          ViInt32 activeField)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_CATV_ATV_TRIGGER_VIDEO_SCOPE_FIELD, activeField),
    		2, "Active Field");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV C/N Noise Floor Correction
 * Purpose:  This function configures the noise floor correction
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNNoiseFloorCorrection (ViSession instrSession,
                                                               ViBoolean noiseFloorCorrection,
                                                               ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = 0;
    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_NOISE_FLOOR_CORRECTION, noiseFloorCorrection),
    		5, "Noise Floor Correction");

Error:
    rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV C/N Measurement
 * Purpose:  This function configures the Carrier to Noise Ratio measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNMeasurement (ViSession instrSession,
                                                         ViInt32 measCarrier,
                                                         ViInt32 measurementMethod,
                                                         ViReal64 noiseReferenceBandwidth)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = 0;
    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, 60000));

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_CARR_MEAS, measCarrier),
    		2, "Meas Carrier");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_MEAS_MODE, measurementMethod),
    		3, "Measurement Method");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_BWID, noiseReferenceBandwidth),
    		4, "Noise Reference Bandwidth");

Error:
    rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV C/N Reference Power
 * Purpose:  This function configures the reference power for Carrier to
 *           Noise Ratio measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNReferencePower (ViSession instrSession,
                                                           ViInt32 referencePower,
                                                           ViInt32 referenceChannel,
                                                           ViReal64 powerManual)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_REF_POWER_MODE, referencePower),
    		2, "Reference Power");
    switch (referencePower){
        case RSSPECAN_VAL_ATV_RPOW_MODE_RCH:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_REF_CHANNEL_MANUAL, referenceChannel),
            		3, "Reference Channel");
        break;
        case RSSPECAN_VAL_ATV_RPOW_MODE_MAN:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_REF_POWER_MANUAL, powerManual),
            		4, "Power Manual");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV C/N Measurement Frequencies
 * Purpose:  This function defines the measurement frequencies
 *           (center frequencies and span values), and activates or deactivates
 *           them for the C/N measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNMeasurementFrequencies (ViSession instrSession,
                                                                    ViInt32 tableRow,
                                                                    ViBoolean state,
                                                                    ViReal64 centerFrequency,
                                                                    ViReal64 span)
{
    ViStatus    error   = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, tableRow, 1, 10),
    		2, "Table Row");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":ATV:CN:TABL%ld:MFR %ld,%.12lf,%.12lf\n", tableRow, (state==VI_FALSE)?0:1, centerFrequency, span);
	checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Analog TV C/N Next Meas Frequency
 * Purpose:  This function switches from one frequency range to the next
 *           according to the defined measurement frequencies.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVACNNextMeasFrequency (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "CN", RSSPECAN_ATTR_CATV_ATV_FREQ_NEXT, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CSO Measurement
 * Purpose:  This function configures the Carrier to Second Order Beat
 *           Ratio measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSOMeasurement (ViSession instrSession,
                                                         ViInt32 measCarrier,
                                                         ViInt32 measurementMethod)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_CARR_MEAS, measCarrier),
    		2, "Meas Carrier");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_MEAS_MODE, measurementMethod),
    		3, "Measurement Method");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CSO Reference Power
 * Purpose:  This function configures the reference power for Carrier to
 *           Second Order Beat Ratio measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSOReferencePower (ViSession instrSession,
                                                            ViInt32 referencePower,
                                                            ViInt32 referenceChannel,
                                                            ViReal64 powerManual)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_REF_POWER_MODE, referencePower),
    		2, "Reference Power");
    switch (referencePower){
        case RSSPECAN_VAL_ATV_RPOW_MODE_RCH:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_REF_CHANNEL_MANUAL, referenceChannel),
            		3, "Reference Channel");
        break;
        case RSSPECAN_VAL_ATV_RPOW_MODE_MAN:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_REF_POWER_MANUAL, powerManual),
            		4, "Power Manual");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CSO Measurement Frequencies
 * Purpose:  This function defines the measurement frequencies
 *           (center frequencies and span values), and activates or deactivates
 *           them for the CSO measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSOMeasurementFrequencies (ViSession instrSession,
                                                                    ViInt32 tableRow,
                                                                    ViBoolean state,
                                                                    ViReal64 centerFrequency,
                                                                    ViReal64 span)
{
    ViStatus    error   = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, tableRow, 1, 10),
    		2, "Table Row");
    viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":ATV:CSO:TABL%ld:MFR %ld,%.12lf,%.12lf", tableRow, (state==VI_FALSE)?0:1, centerFrequency, span);
	checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CSO Noise Floor Correction
 * Purpose:  This function configures the noise floor correction
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSONoiseFloorCorrection (ViSession instrSession,
                                                                  ViBoolean noiseFloorCorrection,
                                                                  ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = 0;
    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_NOISE_FLOOR_CORRECTION, noiseFloorCorrection),
    		5, "Noise Floor Correction");

Error:
    rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Analog TV CSO Next Meas Frequency
 * Purpose:  This function switches from one frequency range to the next
 *           according to the defined measurement frequencies.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVACSONextMeasFrequency (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "CSO", RSSPECAN_ATTR_CATV_ATV_FREQ_NEXT, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CTB Measurement
 * Purpose:  This function configures the Carrier to Composite Triple Beat
 *           Ratio measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBMeasurement (ViSession instrSession,
                                                         ViInt32 measCarrier)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CTB", RSSPECAN_ATTR_CATV_ATV_CARR_MEAS, measCarrier),
    		2, "Meas Carrier");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CTB Reference Power
 * Purpose:  This function configures the reference power for Carrier to
 *           Composite Triple Beat Ratio measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBReferencePower (ViSession instrSession,
                                                            ViInt32 referencePower,
                                                            ViInt32 referenceChannel,
                                                            ViReal64 powerManual)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CTB", RSSPECAN_ATTR_CATV_ATV_REF_POWER_MODE, referencePower),
    		2, "Reference Power");
    switch (referencePower){
        case RSSPECAN_VAL_ATV_RPOW_MODE_RCH:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "CTB", RSSPECAN_ATTR_CATV_ATV_REF_CHANNEL_MANUAL, referenceChannel),
            		3, "Reference Channel");
        break;
        case RSSPECAN_VAL_ATV_RPOW_MODE_MAN:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "CTB", RSSPECAN_ATTR_CATV_ATV_REF_POWER_MANUAL, powerManual),
            		4, "Power Manual");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CTB Measurement Frequencies
 * Purpose:  This function defines the measurement frequencies
 *           (center frequencies and span values), and activates or deactivates
 *           them for the CTB measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBMeasurementFrequencies (ViSession instrSession,
                                                                    ViInt32 tableRow,
                                                                    ViBoolean state,
                                                                    ViReal64 centerFrequency,
                                                                    ViReal64 span)
{
    ViStatus    error   = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, tableRow, 1, 10),
    		2, "Table Row");
    viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":ATV:CTB:TABL%ld:MFR %ld,%.12lf,%.12lf\n", tableRow, (state==VI_FALSE)?0:1, centerFrequency, span);
	checkErr(RsCore_Write(instrSession, cmd));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV CTB Noise Floor Correction
 * Purpose:  This function configures the noise floor correction
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBNoiseFloorCorrection (ViSession instrSession,
                                                                  ViBoolean noiseFloorCorrection,
                                                                  ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = 0;
    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "CTB", RSSPECAN_ATTR_CATV_ATV_NOISE_FLOOR_CORRECTION, noiseFloorCorrection),
    		5, "Noise Floor Correction");

Error:
    rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Analog TV CTB Next Meas Frequency
 * Purpose:  This function switches from one frequency range to the next
 *           according to the defined measurement frequencies.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVACTBNextMeasFrequency (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "CTB", RSSPECAN_ATTR_CATV_ATV_FREQ_NEXT, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Carrier Limit
 * Purpose:  This function sets the limit of the carrier power measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACarrierLimit (ViSession instrSession,
                                                       ViInt32 measurement,
                                                       ViInt32 limitType,
                                                       ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar      buffer2[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 5),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }

    switch (measurement){
        case RSSPECAN_VAL_ATV_VCP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCP, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_VCF:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCF, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_SC1PR:
          // strcat (buffer, ",SC1");
			strcat (buffer2, "SC1, buffer");
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer2, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_REL_POWER, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_SC1IF:
            strcat (buffer, ",SC1");
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_FREQ_OFFSET, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_SC2PR:
            strcat (buffer, ",SC2");
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_REL_POWER, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_SC2IF:
            strcat (buffer, ",SC2");
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_FREQ_OFFSET, limitValue),
            		4, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Carrier Ratio Limit
 * Purpose:  This function sets the limit for the carrier-to-noise,
 *           carrier-to-second order beat or carrier-to-composite triple beat
 *           ratio.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVACarrierRatioLimit (ViSession instrSession,
                                                            ViInt32 measurement,
                                                            ViInt32 limitType,
                                                            ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 2, 4),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }
    switch (measurement){
        case RSSPECAN_VAL_ATV_MEAS_CN:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_CN, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_MEAS_CSO:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_CSO, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_MEAS_CTB:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_LIM_CTB, limitValue),
            		4, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Hum Limit
 * Purpose:  This function sets the limit for the hum values
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAHumLimit (ViSession instrSession,
                                                   ViInt32 limitType,
                                                   ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		2, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_HUM_UNIT, RSSPECAN_VAL_UNIT_DB));
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_LIM_HUM_LOW, limitValue),
            		3, "Limit Value");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_HUM_UNIT, RSSPECAN_VAL_UNIT_PCT));
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_LIM_HUM_UPP, limitValue),
            		3, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Vision Modulation Limit
 * Purpose:  This function sets the limit for the vision modulation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVisionModLimit (ViSession instrSession,
                                                         ViInt32 limit,
                                                         ViInt32 limitType,
                                                         ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 0, 2),
    		2, "Limit");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }
    switch (limit){
        case RSSPECAN_VAL_ATV_VCP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_VCP, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_RPC:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_RPC, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_ATV_MDEP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_MDEP, limitValue),
            		4, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Vision Modulation Carrier Power Unit
 * Purpose:  This function defines the unit of the absolute vision carrier power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVisionModulationCarrierPowerUnit (ViSession instrSession,
                                                                           ViInt32 unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_VCP_UNIT, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Analog TV Hum Unit
 * Purpose:  This function defines the unit of the Hum measurement values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVAHumUnit (ViSession instrSession,
                                                  ViInt32 unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_ATV_HUM_UNIT, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

// Digital TV
/*****************************************************************************
 * Function: Configure Digital TV Measurement
 * Purpose:  This function defines the digital measurement type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDMeasurement (ViSession instrSession,
                                                      ViInt32 measurement)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_DTV_MEAS, measurement),
    		2, "Measurement");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Modulation Standard
 * Purpose:  This function defines the modulation properties of a TV signal
 *           (as used in a TV channel). The signal type is the characteristic
 *           parameter of this set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDModulStandard (ViSession instrSession,
                                                        ViInt32 TVStandard,
                                                        ViInt32 rollOff,
                                                        ViInt32 constellationParameter,
                                                        ViReal64 symbolRate)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_DTV_STANDARD, TVStandard),
    		2, "TV Standard");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_DTV_FILTER_ALPHA, rollOff),
    		3, "Roll Off");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_DTV_FORMAT, constellationParameter),
    		4, "Constellation Parameter");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_SRATE, symbolRate),
    		5, "Symbol Rate");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Shoulder Attenuation
 * Purpose:  This function configures the Shoulder Attenuation for Spectrum
 *           measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDShoulderAttenuation (ViSession instrSession,
                                                              ViBoolean shoulderAttenuation)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CATV_DTV_SATT, shoulderAttenuation),
    		2, "Shoulder Attenuation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Echo Pattern
 * Purpose:  This function configures the Echo Pattern measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDEchoPattern (ViSession instrSession,
                                                      ViBoolean zoomState,
                                                      ViInt32 zoomFactor,
                                                      ViReal64 velocityFactor)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CATV_EPAT_ZOOM_STATE, zoomState),
    		2, "Zoom State");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_EPAT_ZOOM_FACTOR, zoomFactor),
    		3, "Zoom Factor");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_EPAT_RVEL, velocityFactor),
    		4, "Velocity Factor");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Modul Error Zoom
 * Purpose:  This function configures the Modulation Errors Zoom.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDModulErrorZoom (ViSession instrSession,
                                                         ViInt32 zoom)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_MERR_ZOOM, zoom),
    		2, "Zoom");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Overview Zoom
 * Purpose:  This function configures the Overview Zoom.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDOverviewZoom (ViSession instrSession,
                                                       ViInt32 zoom)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_OVER_ZOOM, zoom),
    		2, "Zoom");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Constellation Diagram Zoom
 * Purpose:  This function configures the Constellatin Diagram Zoom.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDConstellationDiagramZoom (ViSession instrSession,
                                                                   ViInt32 zoomDiagram)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_QUAD_ZOOM, zoomDiagram),
    		2, "Zoom");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Equalizer
 * Purpose:  This function configures the digital TV equalizer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDEqualizer (ViSession instrSession,
                                                    ViBoolean state,
                                                    ViBoolean freeze)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CATV_DTV_EQUALIZER_STATE, state),
    		2, "State");
    if (state == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CATV_DTV_EQUALIZER_FREEZE, freeze),
        		3, "Freeze");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Signal Statistics No Samples
 * Purpose:  This function sets the number of measurement points to be acquired
 *           for the statistical measurement functions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDSignalStatisticsNoSamples (ViSession instrSession,
                                                                    ViInt32 no_ofSamples)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_SAMPLES, no_ofSamples),
    		2, "No of Samples");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Signal Statistics Scaling
 * Purpose:  This function changes the scaling parameters for statistical
 *           measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDSignalStatisticsScaling (ViSession instrSession,
                                                                  ViInt32 scalingMode,
                                                                  ViReal64 scalingValue)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, scalingMode, 0, 6),
    		2, "Scaling Mode");

    switch (scalingMode){
        case RSSPECAN_VAL_STAT_DEFAULT:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_PRESET, NULL));
        break;
        case RSSPECAN_VAL_STAT_ADJ:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_ADJ, NULL));
        break;
        case RSSPECAN_VAL_STAT_XRLEV:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_X_REF, scalingValue),
            		3, "Scaling Value");
        break;
        case RSSPECAN_VAL_STAT_XRANGE:
             viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_X_RANGE, scalingValue),
             		3, "Scaling Value");
        break;
        case RSSPECAN_VAL_STAT_YMAX:
             viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_MAX, scalingValue),
             		3, "Scaling Value");
        break;
        case RSSPECAN_VAL_STAT_YMIN:
             viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_MIN, scalingValue),
             		3, "Scaling Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV CCDF Percent Marker
 * Purpose:  This function positions the selected marker in the selected window
 *           to the given probability.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDCCDFPercentMark (ViSession instrSession,
                                                          ViInt32 markerNumber,
                                                          ViReal64 positionValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    sprintf (buffer, "Win0,M%ld", markerNumber);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_MARKER_PROBABILITY, positionValue),
    		3, "Position Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Digital TV Reset Equalizer
 * Purpose:  This function resets the equalizer and all equalizer parameters
 *           are set to their default values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CATVDResetEqualizer (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_DTV_EQUALIZER_RESET, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Overview Limit
 * Purpose:  This function rets the limit of the overview measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDOverLimit (ViSession instrSession,
                                                    ViInt32 measurement,
                                                    ViInt32 limitType,
                                                    ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 5),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");

    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }

    switch (measurement){
        case RSSPECAN_VAL_DTV_MERR:
        case RSSPECAN_VAL_DTV_MERP:
        case RSSPECAN_VAL_DTV_EVMR:
        case RSSPECAN_VAL_DTV_EVMP:
            sprintf (buffer, "%s", DTVMeasArr[measurement]);
            switch (limitType){
                case RSSPECAN_VAL_LIM_LOW:
                    checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR_UNIT, RSSPECAN_VAL_UNIT_DB));
                    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_ERR_LOW, limitValue),
                    		4, "Limit Value");
                break;
                case RSSPECAN_VAL_LIM_UPP:
                    checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR_UNIT, RSSPECAN_VAL_UNIT_PCT));
                    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_ERR_UPP, limitValue),
                    		4, "Limit Value");
                break;
            }
        break;
        case RSSPECAN_VAL_DTV_CFOF:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_CARR_LIM_FREQ_OFFSET, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_DTV_SROF:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_SYMB_RATE_OFFSET, limitValue),
            		4, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Channel Power Limit
 * Purpose:  This function sets the limit of the channel Power measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDChanPowLimit (ViSession instrSession,
                                                       ViInt32 limitType,
                                                       ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");

    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Low", RSSPECAN_ATTR_CATV_DTV_LIM_CHANNEL_POWER, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Upp", RSSPECAN_ATTR_CATV_DTV_LIM_CHANNEL_POWER, limitValue),
            		4, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Digital TV Modulation Errors Limit
 * Purpose:  This function sets the limit of the modulation errors measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDModErrLimit (ViSession instrSession,
                                                      ViInt32 measurement,
                                                      ViInt32 limitType,
                                                      ViReal64 limitValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 7),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");

    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }

    switch (measurement){
        case RSSPECAN_VAL_DTV_MERR:
        case RSSPECAN_VAL_DTV_MERP:
        case RSSPECAN_VAL_DTV_EVMR:
        case RSSPECAN_VAL_DTV_EVMP:
            sprintf (buffer, "%s", DTVMeasArr[measurement]);
            switch (limitType){
                case RSSPECAN_VAL_LIM_LOW:
                    checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR_UNIT, RSSPECAN_VAL_UNIT_DB));
                    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_ERR_LOW, limitValue),
                    		4, "Limit Value");
                break;
                case RSSPECAN_VAL_LIM_UPP:
                    checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR_UNIT, RSSPECAN_VAL_UNIT_PCT));
                    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_ERR_UPP, limitValue),
                    		4, "Limit Value");
                break;
            }
        break;
        case RSSPECAN_VAL_DTV_IMB:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_IMB, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_DTV_QERR:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_QERR, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_DTV_SUP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_SUPP, limitValue),
            		4, "Limit Value");
        break;
        case RSSPECAN_VAL_DTV_PJIT:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_PJIT, limitValue),
            		4, "Limit Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Digital TV Echo Pattern Unit
 * Purpose:  This function defines the unit of the Echo Pattern measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDEchoPatternUnit (ViSession instrSession,
                                                     ViInt32 unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_DTV_EPAT_UNIT, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Digital TV Units
 * Purpose:  This function defines the unit of measured values in
 *           Overview/Modulation Errors measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVDUnit (ViSession instrSession,
                                          ViInt32 measurement,
                                          ViInt32 unit)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 3),
    		2, "Measurement");
    sprintf (buffer, "%s", DTVMeasArr[measurement]);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR_UNIT, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

//TV
/*****************************************************************************
 * Function: Configure TV Analyzer Measurement
 * Purpose:  This function defines the TV analyzer measurement type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVTVMeasurement (ViSession instrSession,
                                                       ViInt32 measurement)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_TV_MEAS, measurement),
    		2, "Measurement");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Configure TV Analyzer Standard for Titl Measurement
 * Purpose:  This function activates/deactivates a modulation standard for
 *           the Tilt measurement
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureCATVTVStandardForTitlMeasurement (ViSession instrSession,
                                                                      ViString modulationStandard,
                                                                      ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");
    if (state == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_TV_MOD_STANDARD_TILT_ACTIVATE, modulationStandard),
        		2, "Modulation Standard");
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CATV_TV_MOD_STANDARD_TILT_DEACTIVATE, modulationStandard),
        		2, "Modulation Standard");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Query Analog TV Carrier Result
 * Purpose:  This function reads the measurement values (power, frequency)
 *           of the Carrier Power measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierResult (ViSession instrSession,
                                               ViInt32 measurement,
                                               ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 5),
    		2, "Measurement");
    switch (measurement){
        case RSSPECAN_VAL_ATV_VCP:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_CARR_VCPA, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_ATV_VCF:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_CARR_VCF_VALUE, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC1PR:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "SC1", RSSPECAN_ATTR_CATV_ATV_CARR_REL_POWER_VALUE, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC1IF:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "SC1", RSSPECAN_ATTR_CATV_ATV_CARR_FREQ_OFFSET_VALUE, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC2PR:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "SC2", RSSPECAN_ATTR_CATV_ATV_CARR_REL_POWER_VALUE, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC2IF:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "SC2", RSSPECAN_ATTR_CATV_ATV_CARR_FREQ_OFFSET_VALUE, result),
            		3, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Carrier All Results
 * Purpose:  This function reads all measurement values (power, frequency) of
 *           the Carrier Power measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierAllResults (ViSession instrSession,
                                                   ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 2, "Results");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "CALC:ATV:RES:CARR? ALL", 6, results, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Carrier Ratio Result
 * Purpose:  This function returns the resultfor the carrier-to-noise,
 *           carrier-to-second order beat or carrier-to-composite triple beat
 *           ratio measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierRatioResult (ViSession instrSession,
                                                    ViInt32 measurement,
                                                    ViInt32 resultType,
                                                    ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 2, 4),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, 0, 4),
    		3, "Result Type");
    switch (resultType){
        case RSSPECAN_VAL_ATV_RES_CR:
            switch (measurement){
                case RSSPECAN_VAL_ATV_MEAS_CN:
                    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_CN_CN_VALUE, result),
                    		4, "Result");
                break;
                case RSSPECAN_VAL_ATV_MEAS_CSO:
                    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_CSO_CSO_VALUE, result),
                    		4, "Result");
                break;
                case RSSPECAN_VAL_ATV_MEAS_CTB:
                    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_CTB_CTB_VALUE, result),
                    		4, "Result");
                break;
            }
        break;
        default:
            sprintf (buffer,"%s,%s", rsspecan_rngCatvAtvMeas.rangeValues[measurement].cmdString,ATVCarrierResultArr[resultType]);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_VALUE, result),
            		4, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Carrier All Ratio Results
 * Purpose:  This function reads all measurement values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierAllRatioResults (ViSession instrSession,
                                                        ViInt32 measurement,
                                                        ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 2, 4),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 3, "Results");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":CALC:ATV:RES:%s? ALL", rsspecan_rngCatvAtvMeas.rangeValues[measurement].cmdString);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, 32001, results, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Hum Result
 * Purpose:  This function reads the measurement values of the Hum measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVAHumResult (ViSession instrSession,
                                           ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_ATV_HUM_VALUE, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Vision Modulation Result
 * Purpose:  This function reads the measurement values of the Vision
 *           Modulation measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVAVisionModulationResult (ViSession instrSession,
                                                        ViInt32 measurement,
                                                        ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 2),
    		2, "Measurement");
    switch (measurement){
        case RSSPECAN_VAL_ATV_VCP:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "VisCarrPow", RSSPECAN_ATTR_CATV_ATV_VMOD_RESULT_VALUE, result),
            		2, "Result");
        break;
        case RSSPECAN_VAL_ATV_RPC:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "ResPicture", RSSPECAN_ATTR_CATV_ATV_VMOD_RESULT_VALUE, result),
            		2, "Result");
        break;
        case RSSPECAN_VAL_ATV_MDEP:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Depth", RSSPECAN_ATTR_CATV_ATV_VMOD_RESULT_VALUE, result),
            		2, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Query Analog TV Vision All Modulation Results
 * Purpose:  This function reads the measurement values of the Vision Modulation
 *           measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVAVisionAllModulationResults (ViSession instrSession,
                                                            ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 2, "Results");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "CALC:ATV:RES:VMOD? ALL", 32001, results, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Carrier Limit Result
 * Purpose:  This function returns the check limit result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierLimitResult (ViSession instrSession,
                                                         ViInt32 measurement,
                                                         ViInt32 limitType,
                                                         ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar        repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 5),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }

    switch (measurement){
        case RSSPECAN_VAL_ATV_VCP:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCP_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_VCF:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCF_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC1PR:
            //strcat (buffer,",SC1");
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "SC1,%s", buffer);
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_REL_POWER_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC1IF:
            //strcat (buffer,",SC1");
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "SC1,%s", buffer);
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_FREQ_OFFSET_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC2PR:
            //strcat (buffer,",SC2");
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "SC2,%s", buffer);
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_REL_POWER_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_SC2IF:
            //strcat (buffer,",SC2");
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "SC2,%s", buffer);
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_CATV_ATV_CARR_LIM_FREQ_OFFSET_RESULT, result),
            		4, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Carrier Ratio Limit Result
 * Purpose:  This function returns the check limit result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierRatioLimitResult (ViSession instrSession,
                                                              ViInt32 measurement,
                                                              ViInt32 limitType,
                                                              ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 2, 4),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 0),
    		3, "Limit Type");

    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }
    switch (measurement){
        case RSSPECAN_VAL_ATV_MEAS_CN:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_CN_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_MEAS_CSO:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_CSO_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_ATV_MEAS_CTB:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_CTB_RESULT, result),
            		4, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Analog TV Hum Limit Result
 * Purpose:  This function returns the limit check for the Hum measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVAHumLimitResult (ViSession instrSession,
                                                     ViInt32 limitType,
                                                     ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "Low", RSSPECAN_ATTR_CATV_ATV_LIM_HUM_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "Upp", RSSPECAN_ATTR_CATV_ATV_LIM_HUM_RESULT, result),
            		4, "Result");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, limitType), 2, "Limit Type");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Query Analog TV Vision Modulation Limit Result
 * Purpose:  This function returns the check limit result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVAVisionModulationLimitResult (ViSession instrSession,
                                                                  ViInt32 measurement,
                                                                  ViInt32 limitType,
                                                                  ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 2),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");

    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }
    switch (measurement){
        case RSSPECAN_VAL_ATV_VCP:
            strcat (buffer, ",VisCarrPow");
        break;
        case RSSPECAN_VAL_ATV_RPC:
            strcat (buffer, ",ResPicture");
        break;
        case RSSPECAN_VAL_ATV_MDEP:
            strcat (buffer, ",Depth");
        break;
    }
    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_MDEP_RESULT, result),
    		4, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Spectrum Result
 * Purpose:  This function reads the measurement value of spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDSpectrumResult (ViSession instrSession,
                                                     ViInt32 measurement,
                                                     ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 1),
    		2, "Measurement");
    switch (measurement){
        case RSSPECAN_VAL_DTV_SPEC_SAL:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "SAttLow", RSSPECAN_ATTR_CATV_DTV_SPEC, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_SPEC_SAUP:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "SAttUpp", RSSPECAN_ATTR_CATV_DTV_SPEC, result),
            		3, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Channel Power Result
 * Purpose:  This function returns the result value of Channel Power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDChanelPowerResult (ViSession instrSession,
                                                        ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_CHANNEL_POWER_RESULT, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Overview Result
 * Purpose:  This function returns the result value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDOverviewResult (ViSession instrSession,
                                                     ViInt32 measurement,
                                                     ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 5),
    		2, "Measurement");

    switch (measurement){
        case RSSPECAN_VAL_DTV_MERR:
        case RSSPECAN_VAL_DTV_MERP:
        case RSSPECAN_VAL_DTV_EVMR:
        case RSSPECAN_VAL_DTV_EVMP:
            sprintf (buffer, "%s", DTVMeasArr[measurement]);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_CFOF:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_OVER_CARR_FREQ_OFFSET, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_SROF:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_OVER_SRATE_OFFSET, result),
            		3, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Overview All Results
 * Purpose:  This function returns the result values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDOverviewAllResults (ViSession instrSession,
                                                         ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 2, "Results");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, ":CALC:DTV:RES? ALL", 32001, results, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Modulation Errors Result
 * Purpose:  This function returns the result value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDModulationErrorsResult (ViSession instrSession,
                                                             ViInt32 measurement,
                                                             ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 7),
    		2, "Measurement");

    switch (measurement){
        case RSSPECAN_VAL_DTV_MERR:
        case RSSPECAN_VAL_DTV_MERP:
        case RSSPECAN_VAL_DTV_EVMR:
        case RSSPECAN_VAL_DTV_EVMP:
            sprintf (buffer, "%s", DTVMeasArr[measurement]);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_ERROR, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_IMB:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_MERR_IMB, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_QERR:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_MERR_QERR, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_SUP:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_MERR_CSUP, result),
            		3, "Result");
        break;
        case RSSPECAN_VAL_DTV_PJIT:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CATV_DTV_MERR_PJIT, result),
            		3, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Modulation Errors All Result
 * Purpose:  This function returns the result value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDModulationErrorsAllResults (ViSession instrSession,
                                                                 ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 2, "Results");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, ":CALC:DTV:RES? ALL", 32001, results, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Signal Statistics Result
 * Purpose:  This function reads out the results of statistical measurements
 *           of a recorded trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDSignalStatisticsResult (ViSession instrSession,
                                                             ViInt32 traceNumber,
                                                             ViInt32 resultType,
                                                             ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViString   statisticMeasType[] = {"Mean","Peak","CrestFactor"};

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, traceNumber, 1, 4),
    		2, "Trace");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, 0, 2),
    		3, "Result Type");
    sprintf (buffer, "TR%ld,Stat%s", traceNumber, statisticMeasType[resultType]);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_STAT_RESULT, result),
    		4, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Query Digital TV Signal Statistics All Results
 * Purpose:  This function reads out the results of statistical measurements
 *           of a recorded trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDSignalStatisticsAllResults (ViSession instrSession,
                                                                 ViInt32 traceNumber,
                                                                 ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K20"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, traceNumber, 1, 4),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 3, "Results");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":CALC:STAT:RES%ld? ALL", traceNumber);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, 32001, results, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Overview Limit Result
 * Purpose:  This function returns the check limit result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDOverviewLimitResult (ViSession instrSession,
                                                          ViInt32 measurement,
                                                          ViInt32 limitType,
                                                          ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar        repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 5),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }

    switch (measurement){
        case RSSPECAN_VAL_DTV_MERR:
        case RSSPECAN_VAL_DTV_MERP:
        case RSSPECAN_VAL_DTV_EVMR:
        case RSSPECAN_VAL_DTV_EVMP:
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s,%s", DTVMeasArr[measurement], buffer);
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_CATV_DTV_LIM_ERROR_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_DTV_CFOF:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_CARR_LIM_FREQ_OFFSET_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_DTV_SROF:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_SYMB_RATE_OFFSET_RESULT, result),
            		4, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Channel Power Limit Result
 * Purpose:  This function returns the check limit result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDChannelPowerLimitResult (ViSession instrSession,
                                                              ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CATV_DTV_LIM_CHANNEL_POWER_RESULT, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Digital TV Modulation Errors Limit Result
 * Purpose:  This function returns the check limit result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryCATVDModulationErrorsLimitResult (ViSession instrSession,
                                                                  ViInt32 measurement,
                                                                  ViInt32 limitType,
                                                                  ViInt32 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar        repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 7),
    		2, "Measurement");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, 0, 1),
    		3, "Limit Type");
    switch (limitType){
        case RSSPECAN_VAL_LIM_LOW:
            sprintf (buffer, "Low");
        break;
        case RSSPECAN_VAL_LIM_UPP:
            sprintf (buffer, "Upp");
        break;
    }

    switch (measurement){
        case RSSPECAN_VAL_DTV_MERR:
        case RSSPECAN_VAL_DTV_MERP:
        case RSSPECAN_VAL_DTV_EVMR:
        case RSSPECAN_VAL_DTV_EVMP:
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s,%s", DTVMeasArr[measurement], buffer);
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_CATV_DTV_LIM_ERROR_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_DTV_IMB:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_IMB_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_DTV_QERR:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_LIM_QERR_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_DTV_SUP:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_CATV_DTV_CARR_LIM_CARR_SUPP_RESULT, result),
            		4, "Result");
        break;
        case RSSPECAN_VAL_DTV_PJIT:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "Upp", RSSPECAN_ATTR_CATV_DTV_LIM_PJIT_RESULT, result),
            		4, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 *- Callback Definition   ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- End Of RSSpecAn K20 - Cable TV Analyzer ---------------------------------*
 *****************************************************************************/
