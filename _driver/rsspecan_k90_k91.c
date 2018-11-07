/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  WLAN option K90/K91
 *
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
/*  Array definitions   */
static ViString ACPMeas[] = {"CURR","MAXH"};


/*===========================================================================*/
/* Function: Configure WLAN Standard
/* Purpose:  This function configures the Wireless LAN standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanStandard (ViSession   instrSession,
                                                ViInt32 standard)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_STAN, standard),
    		2, "Standard");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN YIG Filter
/* Purpose:  This function switches the YIG filter for image frequency
/*           suppression into the signal path (condition ON) and/or removes
/*           it from the signal path (condition OFF).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWLANYIGFilter(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean   yigFilter)
{
    ViStatus    error = VI_SUCCESS;
   ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_YIG_FILTER, yigFilter),
    		2, "YIG Filter");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN  Trigger
/* Purpose:  This function configures the trigger parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanTrigger(ViSession   instrSession,
                                                ViInt32     mode,
                                                ViReal64    offset,
                                                ViBoolean   autoLevel,
                                                ViReal64    powerLevel,
                                                ViReal64    extTrigLevel)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_TRIG_MODE, mode),
    		2, "Mode");
    switch (mode){
        case RSSPECAN_VAL_TRIG_MODE_IMM:
        break;
        case RSSPECAN_VAL_TRIG_MODE_EXT:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset),
            		3, "Offset");

            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, extTrigLevel),
            		6, "Ext. Trigger Level");
        break;
        case RSSPECAN_VAL_TRIG_MODE_POW:
		case RSSPECAN_VAL_TRIG_MODE_PSEN:
		case RSSPECAN_VAL_TRIG_MODE_RFP:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset),
            		3, "Offset");

            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_TRIG_POW_LEV_AUTO, autoLevel),
            		4, "Auto Level");
            if (!autoLevel)
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_TRIG_POW_LEV, powerLevel),
                		5, "Power Level");

		break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN This function configures STC MIMO antenna assignment of the receive path.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR rxAnalyzer/Defines the analyzer number.
/// HIPAR configuration/This control specifies the antenna assignment of the receive path.
/// HIPAR antennaState/Specifies ON/OFF state of the receive path.
/// HIPAR analyzerIPAddress/Specifies the TCP/IP address in IPV4 format.
ViStatus _VI_FUNC rsspecan_ConfigureWLANSTCMIMO (ViSession instrSession,
                                                 ViInt32 rxAnalyzer,
                                                 ViBoolean antennaState,
                                                 ViString analyzerIPAddress,
                                                 ViInt32 configuration)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, rxAnalyzer, 2, 4),
    		2, "Rx Analyzer");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, configuration, RSSPECAN_VAL_WLAN_ANTENNA1, RSSPECAN_VAL_WLAN_ANTENNA4),
			5, "Configuration");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "AntMatrix%ld", rxAnalyzer);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_WLAN_ANALYZER_STATE, antennaState),
    		3, "Antenna State");

    viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_WLAN_ANALYZER_ADDRESS, analyzerIPAddress),
    		4, "Analyzer IP Address");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_ANTENNA_CONFIGURATION, configuration),
			5, "Configuration");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures how burst synchronization and tracking is performed
/// HIFN for multiple antennas.
/// HIRET Returns the status code of this operation.
/// HIRET You may use multiple lines for documentation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR state/This control configures if antennas are synchronised and tracked
/// HIPAR state/together (ON) or separately (OFF).
ViStatus _VI_FUNC rsspecan_ConfigureWLANSimultaneousCaptureState (ViSession instrSession,
                                                                  ViBoolean state)
{
	ViStatus    error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_ANTENNA_SIMULTANEOUS_CAPTURE_STATE, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Gate
/* Purpose:  This function configures the trigger parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanGate(ViSession  instrSession,
                                            ViBoolean   gating,
                                            ViInt32     mode,
                                            ViReal64    delay,
                                            ViReal64    length,
                                            ViBoolean   linkGateAndMarker)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_EGAT, gating),
    		2, "Gating");
    if (gating)
    {
        viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, 0, 5),
        		3, "Mode");
        switch (mode){
            case RSSPECAN_VAL_WLAN_GATE_TIME:
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_EGAT_HOLD_TIME, delay),
                		4, "Delay");

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_EGAT_LENG_TIME, length),
                		5, "Length");
            break;
            case RSSPECAN_VAL_WLAN_GATE_SAMPLE:
                viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_EGAT_HOLD_SAMP, (ViInt32)delay), 4, "Delay");

                viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_EGAT_LENG_SAMP, (ViInt32)length), 5, "Length");
            break;
        }
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_EGAT_LINK, linkGateAndMarker),
        		6, "Link Gate Marker");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN ACP Mode
/* Purpose:  This function sets the ACP measurement mode to either absolute
/*           or relative.
/*
/*           Note:
/*
/*           (1) This function is available only for IEEE 802.11j standards.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanACPMode(ViSession instrSession,
                                       ViInt32  acpMode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_ACH_MODE, acpMode),
    		2, "ACP Mode");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Sweep
/* Purpose:  This function configures the acquisition.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWLANSweep (ViSession instrSession,
                                               ViBoolean sweepModeContinuous)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, sweepModeContinuous),
    		2, "Sweep Mode Continuous");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function specifies  the Peak Vector Error results are
/// HIFN calculated over the complete burst or just over the PSDU.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR peakVectorError/This control specifies  the Peak Vector Error results are calculated over the complete burst or just over the PSDU.
ViStatus _VI_FUNC rsspecan_ConfigureWLANPeakVectorError (ViSession instrSession,
                                                         ViInt32 peakVectorError)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PVR_MODE, peakVectorError),
    		2, "Peak Vector Error");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN This function configures the measurement range for the peak vector error.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR measRange/Specifies whether the Peak Error Vector results are calculated over the complete burst or just over the PSDU.
ViStatus _VI_FUNC rsspecan_ConfigureWlanMeasRange (ViSession instrSession,
                                                   ViInt32 measRange)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PEAK_ERROR_VECTOR_MEAS_RANGE, measRange),
    		2, "Meas Range");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function specifies the number of antennas.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR numberOfAntennas/This control specifies the number of antennas.
ViStatus _VI_FUNC rsspecan_ConfigureWLANNumberOfAntennas (ViSession instrSession,
                                                          ViInt32 numberOfAntennas)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_NUMBER_OF_ANTENNAS, numberOfAntennas),
    		2, "Number Of Antennas");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function defines the setup how the Tx antenna signals of
/// HIFN the device under test (DUT) are captured by the
/// HIFN analyzer/analyzers.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR signalCapture/This control specifies how the Tx antenna signals of the device under test (DUT) are captured by the analyzer/analyzers.
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMODUTConfiguration (ViSession instrSession,
                                                              ViInt32 signalCapture)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_DUT_CONFIGURATION, signalCapture),
    		2, "Signal Capture");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function specifies method for analyzing MIMO signals.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR MIMOAntennaSignalCapture/This control specifies method for analyzing MIMO signals.
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMOAntennaSignalCapture (ViSession instrSession,
                                                                  ViInt32 MIMOAntennaSignalCapture)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_ANT_SIGNAL_CAPTURE_TYPE, MIMOAntennaSignalCapture),
    		2, "MIMO Antenna Signal Capture");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function specifies the TCP/IP address and module of the
/// HIFN switch unit that can be used for automated sequential MIMO
/// HIFN measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR OSPIPAddress/Specifies the OSP TCP/IP address in IPV4 format.
/// HIPAR OSPSwitchModule/This control specifies module of the switch unit that can be used for automated sequential MIMO measurements.
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMOSequentialCaptureSetup (ViSession instrSession,
                                                                    ViString OSPIPAddress,
                                                                    ViInt32 OSPSwitchModule)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_OSP_IP_ADDRESS, OSPIPAddress),
    		2, "OSP IP Address");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_OSP_SWITCH_MODULE, OSPSwitchModule),
			3, "OSP Switch Module");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the FS-K90 11n MIMO to determine if the payload length
/// HIFN should be taken from the signal field decoding result or from the signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR source/This control specifies if the payload length should be taken
/// HIPAR source/from the signal field decoding result or from the signal.
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMOPayloadLengthSource (ViSession instrSession,
                                                        ViInt32 source)
{
	ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_PAYLOAD_LENGTH_SOURCE, source),
			2, "Source");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function enables or disables use of the RBW filter.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR RBWFilter/This Control enables or disables use of the RBW filter.
ViStatus _VI_FUNC rsspecan_ConfigureWLANRBWFilter (ViSession instrSession,
                                                   ViBoolean RBWFilter)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_TRAC_PHAS, RBWFilter),
    		2, "RBW filter");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*===========================================================================*/
/* Function: Configure WLAN FFT Start Offset
/* Purpose:  This function configures the FFT start offset.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWLANFFTStartOffset (ViSession instrSession,
                                                        ViInt32 FFTStartOffset)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_FFT_START_OFFSET, FFTStartOffset),
    		2, "FFT Start Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the mapping.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR spatialMappingMode/This control is used to specify the mapping between streams and antennas.
/// HIPAR powerNormalize/Specifies whether an amplification of the signal power due to the spatial mapping is performed according to the matrix entries.
ViStatus _VI_FUNC rsspecan_ConfigureWLAN_MIMOSpatialMappingConfiguration (ViSession instrSession,
                                                                          ViInt32 spatialMappingMode,
                                                                          ViBoolean powerNormalize)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_SPATIAL_MAPPING_MODE, spatialMappingMode),
    		2, "Spatial Mapping Mode");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_MIMO_POWER_NORMALISE, powerNormalize),
    		3, "Power Normalize");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function specifies User Defined Spatial Mapping - allows
/// HIFN the user to define his own spatial mapping between streams and
/// HIFN antennas.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR TX/This control selects the antenna.
/// HIPAR realValues/Specifies the real data to mapping for all streams.
/// HIPAR imaginaryValues/Specifies the imaginary data to mapping for all streams.
/// HIPAR timeShift/This control specifies the timeshift in [ns] for a specific antenna.
ViStatus _VI_FUNC rsspecan_ConfigureWLAN_MIMO_UDFSpatialMappingAntenna (ViSession instrSession,
                                                                        ViInt32 TX,
                                                                        ViReal64 realValues[],
                                                                        ViReal64 imaginaryValues[],
                                                                        ViReal64 timeShift)
{
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, TX, 1, 4),
    		2, "TX");

	sprintf (cmd, "CONF:WLAN:SMAP:TX%ld %.12lG,%.12lG, %.12lG,%.12lG, %.12lG,%.12lG, %.12lG,%.12lG, %.12lG\n",
		TX, realValues[0], imaginaryValues[0],realValues[1], imaginaryValues[1],
		realValues[2], imaginaryValues[2], realValues[3], imaginaryValues[3], timeShift);

    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function specifies User Defined Spatial Mapping for a
/// HIFN specific stream and antenna (real & imaginary data pair) and
/// HIFN User Defined timeshift for a specific antenna.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR TX/This control selects the antenna.
/// HIPAR stream/This control selects the stream.
/// HIPAR realValue/Specifies the real data to mapping for a specific stream and antenna.
/// HIPAR imaginaryValue/Specifies the imaginary data to mapping for a specific stream and antenna.
/// HIPAR timeShift/This control specifies the timeshift in [ns] for a specific antenna.
ViStatus _VI_FUNC rsspecan_ConfigureWLAN_MIMO_UDFSpatialMappingStream (ViSession instrSession,
                                                                       ViInt32 TX,
                                                                       ViInt32 stream,
                                                                       ViReal64 realValue,
                                                                       ViReal64 imaginaryValue,
                                                                       ViReal64 timeShift)
{
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar      repCap[RS_REPCAP_BUF_SIZE];
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, TX, 1, 4),
    		2, "TX");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, stream, 1, 4),
    		3, "Stream");

	snprintf (cmd, RS_REPCAP_BUF_SIZE, "CONF:WLAN:SMAP:TX%ld:STR%ld %.12lG,%.12lG", TX, stream, realValue, imaginaryValue);

    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

    sprintf (repCap, "TX%ld", TX);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_MIMO_UDFSPATIAL_MAPPING_TIME_SHIFT, timeShift),
    		6, "Time Shift");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*===========================================================================*/
/* Function: Configure WLAN Auto Level Once
/* Purpose:  This function performs the auto level routine immediately one
/*           time regardless of the current state of the Auto Level State.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanAutoLevelOnce(ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_POW_AUTO_ONCE, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Baseband Signal Level
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanBasebandSignalLevel (ViSession instrSession,
                                                  ViBoolean autoLevel,
                                                  ViReal64    autoLevelTime,
                                                  ViReal64 signalLevel,
                                                  ViReal64 extAtt)

{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_POW_AUTO, autoLevel),
    		2, "Auto Level");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_POW_AUTO_SWE_TIME, autoLevelTime),
    		3, "Auto Level Time");

    if (!autoLevel && RsCore_HasInstrumentOptions(instrSession, "K91") &&
		(!RsCore_IsInstrumentModel(instrSession, "FSL") && !rsspecan_IsFSV (instrSession)))
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_POW_EXP_IQ, signalLevel),
        		4, "Signal Level");
    }
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, extAtt),
    		5, "Ext Att");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Channel
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanRFChannelFrequency (ViSession instrSession,
                                                 ViInt32 channelNo,
                                                 ViReal64 frequency)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if (channelNo)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_CHAN, channelNo), 2, "Channel No")
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_FREQUENCY_CENTER, frequency),
        		3, "Frequency");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN RF Signal
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanRFSignalLevel(ViSession   instrSession,
                                                ViBoolean   autoLevel,
                                                ViReal64    autoLevelTime,
                                                ViReal64    signalLevel,
                                                ViReal64     rfAtt,
                                                ViReal64    extAtt)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_POW_AUTO, autoLevel),
    		2, "Auto Level");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_POW_AUTO_SWE_TIME, autoLevelTime),
    		3, "Auto Level Time");
    if ((!autoLevel) && (!RsCore_IsInstrumentModel(instrSession, "FMU")))
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ATTENUATION, rfAtt),
        		5, "RF Att");

        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_POW_EXP_RF, signalLevel),
        		4, "Signal Level");
    }
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, extAtt),
    		6, "Ext Att");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*===========================================================================*/
/* Function: Configure WLAN Marker Enabled
/* Purpose:  This function enables the active marker on the specified trace.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWLANMarkerEnabled(ViSession instrSession,
                                                        ViInt32     window,
                                                        ViInt32     marker,
                                                        ViBoolean   markerEnabled,
                                                        ViInt32     trace)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_ENABLED, markerEnabled),
    		4, "Marker Enabled");
    if (markerEnabled == VI_TRUE)
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_TRACE, trace),
        		5, "Trace");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Marker Position
/* Purpose:  This function configures the marker position.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkPosition(ViSession   instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViInt32 position,
                                                    ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, position, RSSPECAN_VAL_WLAN_POS_X, RSSPECAN_VAL_WLAN_POS_Y),
    		4, "Position");
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
    switch (position){
        case RSSPECAN_VAL_WLAN_POS_X:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_POSITION, value),
            		5, "Value");
        break;
        case RSSPECAN_VAL_WLAN_POS_Y:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_MARK_Y, value),
            		5, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Marker Position for Constellation
/* Purpose:  This function positions the selected marker to the indicated
/*           symbol/carrier.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkerPositionConstellation(ViSession   instrSession,
                                                                    ViInt32 window,
                                                                    ViInt32 marker,
                                                                    ViInt32 type,
                                                                    ViInt32 position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_WLAN_CONS_SYM, RSSPECAN_VAL_WLAN_CONS_CARR),
    		4, "Type");
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,M%ld", window, marker);
    switch (type){
        case RSSPECAN_VAL_WLAN_CONS_SYM:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_MARK_SYMB, position),
            		5, "Position");
        break;
        case RSSPECAN_VAL_WLAN_CONS_CARR:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_MARK_CARR, position),
            		5, "Position");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Marker Zoom
/* Purpose:  This function defines the ratio to be zoomed around the
/*           selected marker in the selected measurement window.
/*
/*           Note:
/*           (1) This function is only available for the following result
/*           displays:
/*
/*             Constellation versus Carrier
/*             Constellation versus Symbol
/*             PVT Full Burst
/*             PVT Rising / Falling
/*             Magnitude Capture Buffer
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkerZoom(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViReal64 zoomRatio)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_ZOOM, zoomRatio),
    		4, "Zoom Ratio");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Marker Burst Symbol
/* Purpose:  This function positions the selected marker to the indicated
/*           symbol in the indicated burst (Constellation vs Symbol and EVM
/*           vs Symbol).
/*
/*           Note:
/*           (1) This function is only valid for the following 802.11b
/*           measurements:
/*
/*                  Constellation vs Symbol
/*                  EVM vs Symbol
/*
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkerBurstSymbol(ViSession   instrSession,
                                                            ViInt32 window,
                                                            ViInt32 marker,
                                                            ViInt32 burstNumber,
                                                            ViInt32 symbolNumber)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K90|K91"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK%ld:BSYM %ld,%ld", window, marker, burstNumber, symbolNumber);
    checkErr(RsCore_Write(instrSession, cmd));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Set WLAN Marker To Peak
/* Purpose:  This function set the selected marker to the min/max peak in
/*           the current trace.
/*
/*           Note:
/*           (1) This function is only available for the Spectrum Flatness
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetWlanMarkerPeak(ViSession   instrSession,
                                            ViInt32 window,
                                            ViInt32 marker,
                                            ViInt32 position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, position, RSSPECAN_VAL_MARKER_SEARCH_HIGHEST, RSSPECAN_VAL_MARKER_SEARCH_MINIMUM),
    		4, "Position");
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);

    switch (position){
        case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
            checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_MIN, ""));
        break;
        case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
            checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_PEAK, ""));
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: WLAN Disable All Markers
/* Purpose:  This function turns off all the markers in selected measurement
/*           window.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WlanDisableAllMarkers(ViSession  instrSession,
                                                    ViInt32 window)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
    checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_AOFF, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/*===========================================================================*/
/* Function: Configure WLAN Demodulation
/* Purpose:  This function configures the demodulation parameters.
/*
/*           Note:
/*           (1) This function is not available for IEEE 802.11b standard.
/*
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanDemodulation(ViSession   instrSession,
                                                    ViBoolean   useSignalFieldContent,
                                                    ViInt32     burstType,
                                                    ViBoolean   autoDemodulation,
                                                    ViInt32     demodulator,
                                                    ViBoolean   channelEstimation,
                                                    ViBoolean   equalBurstLength,
                                                    ViInt32 minNumberofDataSymbols,
                                                    ViInt32 maxNumberofDataSymbols)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     is_k91 = 0;

    checkErr(RsCore_LockSession(instrSession));

	is_k91 = RsCore_HasInstrumentOptions(instrSession, "K91");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, minNumberofDataSymbols, 1, 1366),
    		8, "Min Number of Data Symbols");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, maxNumberofDataSymbols, 1, 1366),
    		9, "Max Number of Data Symbols");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_BTYP, burstType),
    		3, "Burst Type");

    if (is_k91)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_BCON_AUTO, autoDemodulation),
        		4, "Auto Demodulation");
    }

    if (!autoDemodulation)
    {
        if (is_k91)
        {
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_SIGS, useSignalFieldContent),
            		2, "Use Signal Field Content");
        }
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_BAN, demodulator),
        		5, "Demodulator");
    }
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_CEST, channelEstimation),
    		6, "Channel Estimation");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_SYM_EQU, equalBurstLength),
    		7, "Equal Burst Length");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Min", RSSPECAN_ATTR_WLAN_SYM, minNumberofDataSymbols),
    		8, "Min Number of Data Symbols");
    if (!equalBurstLength)
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Max", RSSPECAN_ATTR_WLAN_SYM, maxNumberofDataSymbols),
        		9, "Max Number of Data Symbols");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN PLCP Demodulation
/* Purpose:  This function configures the demodulation parameters for PLCP
/*            Burst Type demodulation..
/*
/*           Note:
/*           (1) This function is available only for IEEE 802.11b/g(PLCP) standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPDemodulation(ViSession instrSession,
                                                        ViBoolean   usePLCPHeaderContent,
                                                        ViInt32     burstType,
                                                        ViBoolean   autoDemodulation,
                                                        ViInt32     demodulator,
                                                        ViInt32     lengthType,
                                                        ViBoolean   equalBurstLength,
                                                        ViInt32     minPayloadLength,
                                                        ViInt32     maxPayloadLength)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_BTYP, burstType),
    		3, "Burst Type");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_BCON_AUTO, autoDemodulation),
    		4, "Auto Demodulation");

    if (!autoDemodulation)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_BAN, demodulator),
        		5, "Demodulator");

        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_SIGS, usePLCPHeaderContent),
        		2, "Use Signal Field Content");
    }

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, lengthType, RSSPECAN_VAL_WLAN_DEM_DUR_MS, RSSPECAN_VAL_WLAN_DEM_DUR_BYTE),
    		3, "Length Type");

    switch (lengthType){
        case RSSPECAN_VAL_WLAN_DEM_DUR_MS:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_DUR_EQU, equalBurstLength),
            		7, "Equal Burst Length");

            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Min", RSSPECAN_ATTR_WLAN_DUR, minPayloadLength),
            		8, "Min Payload Length");
            if (!equalBurstLength)
                viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Max", RSSPECAN_ATTR_WLAN_DUR, maxPayloadLength),
                		9, "Max Payload Length");
        break;
        case RSSPECAN_VAL_WLAN_DEM_DUR_BYTE:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_DBYT_EQU, equalBurstLength),
            		7, "Equal Burst Length");

            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Min", RSSPECAN_ATTR_WLAN_DBYT, minPayloadLength),
            		8, "Min Payload Length");
            if (!equalBurstLength)
            {
                viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Max", RSSPECAN_ATTR_WLAN_DBYT, maxPayloadLength),
                		9, "Max Payload Length");
            }
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Select Filter
/* Purpose:  This function selects the TX and RX filters. The names of the
/*           filters correspond to the file names returned by
/*           RSSPECAN_ATTR_WLAN_FILT_CAT attribute.
/*
/*           Note:
/*           (1) This function is available only for IEEE 802.11b & g
/*           standards
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanSelectFilter(ViSession   instrSession,
                                            ViString    txFilterName,
                                            ViString    rxFilterName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K90|K91"));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "DEM:FILT:MOD '%s','%s'", txFilterName, rxFilterName);
    checkErr(RsCore_Write(instrSession, cmd));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/// HIFN This function configures the equalizer filter length.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR filterLength/The equalizer filter length in chips.
ViStatus _VI_FUNC rsspecan_ConfigureWLANFFTEqualizerFilterLength (ViSession instrSession,
                                                                  ViInt32 filterLength)
{
	ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_EQUALIZER_FILTER_LENGTH, filterLength),
    		2, "Filter Length");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/*===========================================================================*/
/* Function: Get WLAN Filter Catalog
/* Purpose:  This function reads the names of all available filters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanFilterCatalog(ViSession   instrSession,
                                            ViInt32 bufferSize,
                                            ViChar  filterList[],
                                            ViInt32*    returnedLength)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K90|K91"));
    checkErr(rsspecan_QueryViString(instrSession, "DEM:FILT:CAT?", bufferSize, filterList));
    if (returnedLength)
		*returnedLength = strlen(filterList);

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function allows the user to specify the bursts to be
/// HIFN analyzed. It provides also settings to adapt the synchronisation
/// HIFN onto the channel conditions.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR burstTypeToMeasure/This control specifies whether how signals are analyzed.
/// HIPAR channelBandwidthToMeasure/This control is used to specify which bursts are analyzed according to STBC streams.
/// HIPAR guardIntervalLength/This control is used to specify the guard interval length of the bursts taking part in the analysis.
/// HIPAR STBCField/This control is used to specify which bursts are analyzed according to STBC streams.
/// HIPAR extensionSpatialStreams/This control is used to specify which bursts are analyzed according to extension spatial streams.
/// HIPAR sourceOfPayloadLength/This control is used to specify how the payload length of the bursts to analyze is determined.
ViStatus _VI_FUNC rsspecan_ConfigureWLANBurstsToAnalyzeSettings (ViSession instrSession,
                                                                 ViInt32 burstTypeToMeasure,
                                                                 ViInt32 channelBandwidthToMeasure,
                                                                 ViInt32 guardIntervalLength,
                                                                 ViInt32 STBCField,
                                                                 ViInt32 extensionSpatialStreams,
                                                                 ViInt32 sourceOfPayloadLength)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_BURST_TYPE_TO_MEASURE, burstTypeToMeasure),
    		2, "Burst Type To Measure");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEAS, channelBandwidthToMeasure),
    		3, "Channel Bandwidth To Measure");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_GUARD_INTERVAL_LEGTH, guardIntervalLength),
    		4, "Guard Interval Length");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_STBC, STBCField),
    		5, "STBC Field");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_EXT_SPATIAL_STREAMS, extensionSpatialStreams),
    		6, "Extension Spatial Streams");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SOURCE_OF_PAYLOAD_LENGTH, sourceOfPayloadLength),
    		7, "Source Of Payload Length");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN This function specifies how bursts are analyzed acoording to the
/// HIFN MCS index and defines the Modulation and Coding Scheme (MCS)
/// HIFN index, of the bursts taking part in the analysis, manually.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR MCSIndexToUse/This control is used to specify how bursts are analyzed according to the MCS index.
/// HIPAR MCSIndex/This Control allows the user to define the Modulation and Coding Scheme (MCS) index, of the bursts taking part in the analysis, manually. This field is enabled for MCS index to use Meas only the specified MCS or Demod all with specified MCS.
ViStatus _VI_FUNC rsspecan_ConfigureWLANMCS (ViSession instrSession,
                                             ViInt32 MCSIndexToUse,
                                             ViInt32 MCSIndex)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX_TO_USE_FBUR, MCSIndexToUse),
    		2, "MCS Index To Use");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX, MCSIndex),
    		3, "MCS Index");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN PLCP Limit
/* Purpose:  This function configures the limit.
/*
/*           Note:
/*           (1) This function is valid only if standard IEEE 802.11b or
/*           IEEE 802.11g (PLCP) is selected.
/*
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPLimit(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     limit,
                                                    ViInt32     limitType,
                                                    ViInt32     mode,
                                                    ViReal64    value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
    		3, "Limit");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_WLAN_LIM_RTIME, RSSPECAN_VAL_WLAN_LIM_EVM),
    		4, "Limit Type");
    if ((mode!=RSSPECAN_VAL_MEASTYPE_MAX) && (mode!=RSSPECAN_VAL_MEASTYPE_AVER))
    {
        viCheckParm(RS_ERROR_INVALID_PARAMETER, 5, "Mode");
    }
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,L%ld", window, limit);
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            strcat (repCap, ",Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            strcat (repCap, ",Aver");
        break;
    }

    switch (limitType){
        case RSSPECAN_VAL_WLAN_LIM_RTIME:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_TRIS, value),
            		6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_FTIME:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_TFAL, value),
            		6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_EVM, value),
            		6, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Get ACP Channel Limit
/* Purpose:  This function returns the ACP channel limit.
/*
/*           Note:
/*           (1) This function is available only for IEEE 802.11j standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanACPChannelLimit(ViSession     instrSession,
                                                    ViInt32     window,
                                                    ViInt32     limit,
                                                    ViInt32     channelType,
                                                    ViReal64*   value
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
    		3, "Limit");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_WLAN_ACP_ADJ, RSSPECAN_VAL_WLAN_ACP_ALT),
    		4, "Channel Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,L%ld", window, limit);
    switch (channelType){
        case RSSPECAN_VAL_WLAN_ACP_ADJ:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_ACP_ADJ, value),
            		6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_ACP_ALT:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_ACP_ALT, value),
            		6, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN Spectrum Max Overstepping
/* Purpose:  This function returns the alue at the maximum overstepping of
/*           the spectrum mask limits.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanSpectrumMaxOverstepping(ViSession     instrSession,
                                                            ViInt32     window,
                                                            ViInt32     limit,
                                                            ViInt32     channelType,
                                                            ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
    		3, "Limit");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_FREQ, RSSPECAN_VAL_POW),
    		4, "Channel Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,L%ld", window, limit);
    switch (channelType){
        case RSSPECAN_VAL_FREQ:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_SPEC_MASK_X, value),
            		6, "Value");
        break;
        case RSSPECAN_VAL_POW:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_SPEC_MASK_Y, value),
            		6, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Measurement
/* Purpose:  This function configures the measurement parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanMeas(ViSession   instrSession,
                                            ViBoolean   overallBurstCount,
                                            ViInt32     noofBursttoAnalyze,
                                            ViReal64    captureTime,
                                            ViBoolean   swapIQ,
                                            ViInt32     sweepCount)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_BURS_COUN_STAT, overallBurstCount),
    		2, "Overall Burst Count");
    if (overallBurstCount)
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_BURS_COUN, noofBursttoAnalyze),
        		3, "No of Burst to Analyze");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME, captureTime),
    		4, "Capture Time");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WLAN_SWAP_IQ, swapIQ),
    		5, "Swap IQ");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, sweepCount),
    		6, "Sweep Count");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN PVT Measurement
/* Purpose:  This function sets measurement type to be Power Versus Time
/*           and configures measurement parameters
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanPVTMeas(ViSession   instrSession,
                                                ViInt32 risingFalling)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PVT_SEL, risingFalling),
    		2, "Rising/Falling");
    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_PVT, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN PVT for IEEE 802.11b
/* Purpose:  This function sets measurement type to be Power Versus Time and
/*           configures measurement parameters for IEEE802.11b standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPPVT(ViSession   instrSession,
                                                ViInt32 referencePower,
                                                ViInt32 ramp,
                                                ViInt32 averageLength)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PVT_RPOW, referencePower),
    		2, "Reference Power");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PVT_SEL, ramp),
    		3, "Ramp");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PVT_AVER, averageLength),
    		4, "Average Length");
    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_PVT, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN EVM Measurement
/* Purpose:  This function sets measurement type to be EVM.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanEVMMeasurement(ViSession   instrSession,
                                                        ViInt32     mode,
                                                        ViBoolean   autoScaling,
                                                        ViReal64    perDivision,
                                                        ViInt32     unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_WLAN_EVM_MODE_SYMBOL, RSSPECAN_VAL_WLAN_EVM_MODE_FREQ),
    		2, "Mode");

    switch (mode){
        case RSSPECAN_VAL_WLAN_EVM_MODE_SYMBOL:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_EVM_ESYM, ""));
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_UNIT_EVM, unit),
            		5, "Unit");
        break;
        case RSSPECAN_VAL_WLAN_EVM_MODE_CARRIER:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_EVM_ECAR, ""));
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_UNIT_EVM, unit),
            		5, "Unit");
        break;
        case RSSPECAN_VAL_WLAN_EVM_MODE_PHAS:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_PRE, ""));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PRE_SEL, RSSPECAN_VAL_PRE_PHAS));
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_UNIT_PRE, unit),
            		5, "Unit");
        break;
        case RSSPECAN_VAL_WLAN_EVM_MODE_FREQ:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_PRE, ""));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_PRE_SEL, RSSPECAN_VAL_PRE_FREQ));
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_UNIT_PRE, unit),
            		5, "Unit");
        break;
    }
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win2,TR1", RSSPECAN_ATTR_WLAN_DISP_AUTO_Y, autoScaling),
    		3, "Auto Scaling");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win2,TR1", RSSPECAN_ATTR_WLAN_Y_PDIV, perDivision),
    		4, "Per Division");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Spectrum Measurement
/* Purpose:  This function sets measurement type to be Spectrum.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanSpectrumMeasurement(ViSession   instrSession,
                                                            ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_WLAN_SPEC_MODE_IEEE, RSSPECAN_VAL_WLAN_SPEC_MODE_FLAT),
    		2, "Mode");
    switch (mode){
        case RSSPECAN_VAL_WLAN_SPEC_MODE_IEEE:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_MASK_SEL, RSSPECAN_VAL_SPEC_IEEE));
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WLAN_SPEC_MODE_ETSI:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_MASK_SEL, RSSPECAN_VAL_SPEC_ETSI));
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WLAN_SPEC_MODE_FFT:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_FFT, ""));
        break;
        case RSSPECAN_VAL_WLAN_SPEC_MODE_ACPR:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_ACPR, ""));
        break;
        case RSSPECAN_VAL_WLAN_SPEC_MODE_FLAT:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_FLAT, ""));
        break;
        case RSSPECAN_VAL_WLAN_SPEC_MODE_SEL_FLAT:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_SEL, RSSPECAN_VAL_WLAN_SPEC_MODE_SEL_FLAT));
        break;
        case RSSPECAN_VAL_WLAN_SPEC_MODE_SEL_GRD:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_SEL, RSSPECAN_VAL_WLAN_SPEC_MODE_SEL_GRD));
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function sets the Spectrum Flatness channel selection to
/// HIFN either Effective or Physical.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR mode/Sets the sets the Spectrum Flatness channel selection to either Effective or Physical. The driver uses this value to set the RSSPECAN_ATTR_WLAN_SPEC_CSEL attribute.
ViStatus _VI_FUNC rsspecan_ConfigureWLANChannelSelect (ViSession instrSession,
                                                       ViInt32 mode)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_CSEL, mode),
    		2, "Mode");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*===========================================================================*/
/* Function: Configure WLAN Constellation Measurement
/* Purpose:  This function configures the Constellation measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanConstellationMeasurement(ViSession   instrSession,
                                                    ViInt32 constelationMode,
                                                    ViInt32 carrierSelection,
                                                    ViInt32 carrierNumber)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, constelationMode, RSSPECAN_VAL_WLAN_CONS_SYM, RSSPECAN_VAL_WLAN_CONS_CARR),
    		2, "Constelation Mode");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, carrierSelection, RSSPECAN_VAL_SELECTION_ALL, RSSPECAN_VAL_SELECTION_NUM),
    		3, "Carrier Selection");
    switch (constelationMode){
        case RSSPECAN_VAL_WLAN_CONS_SYM:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_CONS_CSYM, ""));
            switch (carrierSelection){
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_CONS_CARR_SEL, RSSPECAN_VAL_SELECTION_ALL),
                    		3, "Carrier Selection");
                break;
                case RSSPECAN_VAL_SELECTION_PIL:
                    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_CONS_CARR_SEL, RSSPECAN_VAL_SELECTION_PIL),
                    		3, "Carrier Selection");
                break;
                case RSSPECAN_VAL_SELECTION_NUM:
                    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_CONS_CARR_SEL_NR, carrierNumber),
                    		4, "Carrier Number");
                break;
             }
        break;
        case RSSPECAN_VAL_WLAN_CONS_CARR:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_CONS_CCAR, ""));
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Constellation Measurement
/* Purpose:  This function configures the Constellation measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPConstellation(ViSession   instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_CONS_CSYM, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Statistics Measurement
/* Purpose:  This function configures the Statistics measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanStatisticsMeasurement(ViSession   instrSession,
                                                        ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_WLAN_STAT_CCDF, RSSPECAN_VAL_WLAN_STAT_SFI),
    		2, "Mode");
    switch (mode){
        case RSSPECAN_VAL_WLAN_STAT_CCDF:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_STAT_CCDF, ""));
        break;
        case RSSPECAN_VAL_WLAN_STAT_BSTR:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_STAT_BSTR, ""));
        break;
        case RSSPECAN_VAL_WLAN_STAT_SFI:
            checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_STAT_SFI, ""));
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN IQ Measurement Parameters
/* Purpose:  This function configures the IQ Trace parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanIQMeasurementParameters(ViSession   instrSession,
                                                                ViReal64    sampleRate,
                                                                ViInt32 filter)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WLAN_SRAT, sampleRate),
    		2, "Sample Rate");

	if (RsCore_HasInstrumentOptions(instrSession, "B72"))
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_FLAT, filter),
				3, "Filter");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN SEM Measurement
/* Purpose:  This function configures the Spectrum Emission Mask measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanSEMMeasurement (ViSession instrSession,
                                                        ViInt32 type,
                                                        ViString fileName,
                                                        ViInt32 powerClass)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SEM_TYPE, type),
    		2, "Type");
    if (type == RSSPECAN_VAL_SEM_USER)
    {
        viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WLAN_SEM_FILE, fileName),
        		3, "File Name");
    }
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_SEM_CLASS, powerClass),
    		4, "Power Class");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN ACP Measurements
/* Purpose:  This function returns the result values of the adjacent channel
/*           power measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanACPMeasurement(ViSession   instrSession,
                                                ViInt32     window,
                                                ViInt32     marker,
                                                ViInt32     mode,
                                                ViReal64    values[])
{
    ViStatus    error = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K90|K91"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 1),
    		3, "Marker");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_ACP_CURR, RSSPECAN_VAL_ACP_MAX),
    		4, "Mode");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, values), 5, "Values");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":CALC%ld:MARK%ld:FUNC:POW:RES:%s?", window, marker, ACPMeas[mode]);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, 12, values, NULL));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Load WLAN IQ Data
/* Purpose:  This function loads the IQ data from the specified .iqw file.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_LoadWlanIQData (ViSession instrSession,
                                           ViString fileName)
{
    ViStatus    error = VI_SUCCESS;
    viCheckParm(RsCore_InvalidNullPointer(instrSession, fileName), 2, "File Name");

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LOAD_IQ_DATA, fileName));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Store WLAN Store IQ Data
/* Purpose:  This function stores the IQ data to the specified .iqw file.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_StoreWlanIQData (ViSession instrSession,
                                            ViString fileName)
{
    ViStatus    error = VI_SUCCESS;
    viCheckParm(RsCore_InvalidNullPointer(instrSession, fileName), 2, "File Name");

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SAVE_IQ_DATA, fileName));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: WLAN Burst Recalc
/* Purpose:  This function forces the IQ measurement results to be
/*           recalculated according to the current settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WlanBurstImm (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "C1", RSSPECAN_ATTR_WLAN_CALC_BURS, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WLAN Burst Time
/* Purpose:  This function returns the burst rise/falling time.
/*
/*           Note:
/*           (1) If no measurement has been performed yet, a query error
/*           will occur.
/*
/*           (2) This function is available only for IEEE 802.11b standard
/*
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanBurstTime(ViSession     instrSession,
                                                ViInt32     time,
                                                ViInt32     mode,
                                                ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, time, RSSPECAN_VAL_WLAN_TIME_FALL, RSSPECAN_VAL_WLAN_TIME_RISE),
    		2, "Time");
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 3, "Mode");
    }

    switch (time){
        case RSSPECAN_VAL_WLAN_TIME_FALL:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_FETC_TFAL, value),
            		3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_TIME_RISE:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_FETC_TRIS, value),
            		3, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function returns the Burst Error Rate for Pilots (average,
/// HIFN minimum or maximum value) in % for the 802.11n MIMO standard.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR mode/Sets the measurement mode.
/// HIPAR value/This control returns the selected power value .
ViStatus _VI_FUNC rsspecan_FetchWlanBurstErrorRateForPilots (ViSession instrSession,
                                                             ViInt32 mode,
                                                             ViReal64 *value)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	switch (mode)
	{
        case RSSPECAN_VAL_MEASTYPE_MIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 2, "Mode");
    }

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_WLAN_FETC_BERPILOT, value),
			3, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WLAN Count
/* Purpose:  This function returns returns the count of analyzed burst or
/*            number of symbols of analyzed burst.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanCount(ViSession   instrSession,
                                        ViInt32     type,
                                        ViInt32*   count)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_WLAN_BURS, RSSPECAN_VAL_WLAN_SYMB),
    		2, "Type");

    switch (type){
        case RSSPECAN_VAL_WLAN_BURS:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_FETC_BURS_COUNT, count),
            		3, "Count");
        break;
        case RSSPECAN_VAL_WLAN_SYMB:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_FETC_SYMB_COUNT, count),
            		3, "Count");
        break;
   }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN PLCP Limit Check Result
/* Purpose:  This function returns the limit result
/*
/*           Note:
/*           (1) This function is valid only if standard IEEE 802.11b/d (PLCP) is
/*           selected.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanPLCPLimitCheckResult(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     limit,
                                                    ViInt32     limitType,
                                                    ViInt32     mode,
                                                    ViInt32*    result)
{
    ViStatus    error = VI_SUCCESS;
     ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
    		3, "Limit");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_WLAN_LIM_RTIME, RSSPECAN_VAL_WLAN_LIM_EVM),
    		4, "Limit Type");
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,L%ld", window, limit);
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            strcat (repCap, ",Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            strcat (repCap, ",Aver");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 5, "Mode");
    }

    switch (limitType){
        case RSSPECAN_VAL_WLAN_LIM_RTIME:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_TRIS_RES, result),
            		6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_FTIME:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_TFAL_RES, result),
            		6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_EVM_RES, result),
            		6, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN ACP Channel Limit Check Result
/* Purpose:  This function returns the ACP channel limit.
/*
/*           Note:
/*           (1) This function is available only for IEEE 802.11j standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanACPChannLimitCheckResult(ViSession     instrSession,
                                                    ViInt32         window,
                                                    ViInt32         limit,
                                                    ViInt32         channelType,
                                                    ViInt32*        result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
    		3, "Limit");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_WLAN_ACP_ADJ, RSSPECAN_VAL_WLAN_ACP_ALT),
    		4, "Channel Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,L%ld", window, limit);
    switch (channelType){
        case RSSPECAN_VAL_WLAN_ACP_ADJ:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_ACP_ADJ_RES, result),
            		6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_ACP_ALT:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_WLAN_ACP_ALT_RES, result),
            		6, "Result");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

