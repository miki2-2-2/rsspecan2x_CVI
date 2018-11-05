/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  WiMAX option K92/K93
 *
 *  Original Release: June 2006
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
#pragma warning(disable:4100) //unreferenced formal parameter
/*  Array definitions   */
static ViString ACPMeas[] = {"CURR","MAXH"}; 
static ViString switchArr[] = {"OFF","ON"};
static ViString predefMapArr[] = {"PredMapConf","PredMapIQDL","PredMapIQUL","PredMapSMU"};

/*===========================================================================*/
/* Function: Set WiMAX Mode
/* Purpose:  This function selects the WiMAX analyzer mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetWiMAXMode(ViSession   instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WIMAX_MODE, ""));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Standard
/* Purpose:  This function configures the Wireless LAN standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXStandard(ViSession instrSession,
                                                  ViInt32   standard)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_STAN, standard), 2, "Standard");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Electronic Attenuator
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXElectronicAtt(ViSession    instrSession,
                                            ViBoolean   state,
                                            ViBoolean   electronicAttenuatorAuto,
                                            ViInt32    manualValue_dB)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_EATT_STATE, state), 2, "State");
    if (state)
        viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_EATT_AUTO, electronicAttenuatorAuto), 3, "Attenuator Auto");
    if (!electronicAttenuatorAuto)
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_AMPL_EATT_VALUE, manualValue_dB), 4, "Manual Value [dB]");
        
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX YIG Filter
/* Purpose:  This function switches the YIG filter for image frequency 
/*           suppression into the signal path (condition ON) and/or removes 
/*           it from the signal path (condition OFF).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXYIGFilter(ViSession    instrSession,
                                                    ViInt32 window,
                                                    ViBoolean   yigFilter)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rep_cap [10];

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    sprintf (rep_cap,"Win%ld",window);
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, rep_cap, RSSPECAN_ATTR_AMPL_YIG_FILTER, yigFilter), 
                                                2, "YIG Filter");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX YIG Filter Auto
/* Purpose:  This function switches the YIG filter for image frequency 
/*           suppression into the signal path (condition ON) and/or removes 
/*           it from the signal path (condition OFF).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXYIGFilterAuto (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViBoolean yigFilterAuto)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rep_cap [10];

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    sprintf (rep_cap,"Win%ld",window);
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, rep_cap, RSSPECAN_ATTR_WIMAX_YIG_AUTO, yigFilterAuto), 
                                                2, "YIG Filter Auto");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*===========================================================================*/
/* Function: Configure WiMAX Trigger
/* Purpose:  This function configures the trigger parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTrigger (ViSession  instrSession,
                                                ViInt32     mode,
                                                ViReal64    offset,
                                                ViBoolean   autoTriggerLevel,
                                                ViReal64    powerTriggerLevel,
                                                ViReal64    extTriggerLevel)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_TRIG_MODE, mode), 2, "Mode");
    switch (mode){
        case RSSPECAN_VAL_TRIG_MODE_IMM:
        break;
        case RSSPECAN_VAL_TRIG_MODE_EXT:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset), 3, "Offset");
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, extTriggerLevel), 6, "Ext. Trigger Level");
        break;
        case RSSPECAN_VAL_TRIG_MODE_POW:
            if (!autoTriggerLevel)
                viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_TRIG_POW_LEV, powerTriggerLevel), 5, "Power Level");
		case RSSPECAN_VAL_TRIG_MODE_PSEN:
		case RSSPECAN_VAL_TRIG_MODE_RFP:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset), 3, "Offset");
            viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_TRIG_POW_LEV_AUTO, autoTriggerLevel), 4, "Auto Level");
        break;
		case RSSPECAN_VAL_TRIG_MODE_EFAL:
		case RSSPECAN_VAL_TRIG_MODE_ERIS:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset), 3, "Offset");
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, extTriggerLevel), 6, "Ext. Trigger Level");
        break;
    }    
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures STC MIMO antenna matrix.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR autoAssignment/Specifies automatic detection of antenna configuration.
/// HIPAR rxAnalyzer/Defines the analyzer number.
/// HIPAR configuration/Specifies the antenna configuration of the receive path.
/// HIPAR analyzerIPAddress/Specifies the TCP/IP address in IPV4 format.
/// HIPAR autoAssignment/ON/OFF state of the receive path

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSTCMIMO (ViSession instrSession,
                                                  ViBoolean autoAssignment,
                                                  ViInt32 rxAnalyzer,
                                                  ViBoolean antennaState,
                                                  ViString analyzerIPAddress,
                                                  ViInt32 configuration)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   

	checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "",
		RSSPECAN_ATTR_WIMAX_ANTENNA_AUTO_DETECTION, autoAssignment), 2, "Auto Assignment");

	if (autoAssignment == VI_FALSE)
	{
		if (rsspecan_invalidViInt32Range (rxAnalyzer, 2, 4) == VI_TRUE)
			viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "rx Analyzer");

		sprintf (buffer, "AntMatrix%ld", rxAnalyzer); 
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer,
	    RSSPECAN_ATTR_WIMAX_ANTENNA_STATE, antennaState), 4, "Antenna State"); 

		viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer,
	    RSSPECAN_ATTR_WIMAX_ANTENNA_ADDRESS, analyzerIPAddress), 5, "Analyzer IP Address"); 

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer,
	    RSSPECAN_ATTR_WIMAX_ANTENNA_CONFIGURATION, configuration), 6, "Configuration");
	}

Error:
 	(void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Gate
/* Purpose:  This function configures the trigger parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXGate(ViSession instrSession,
                                            ViBoolean   gating,
                                            ViInt32     mode,
                                            ViReal64    delay,
                                            ViReal64    length,
                                            ViBoolean   linkGateAndMarker)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_EGAT, gating), 2, "Gating");
    if (gating)
    {
        if (rsspecan_invalidViInt32Range (mode, 0, 5))
        {
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
        }
        switch (mode){
            case RSSPECAN_VAL_WLAN_GATE_TIME:
                viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_EGAT_HOLD_TIME, delay), 4, "Delay");
                viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_EGAT_LENG_TIME, length), 5, "Length");
            break;
            case RSSPECAN_VAL_WLAN_GATE_SAMPLE:
                viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_EGAT_HOLD_SAMP, (ViInt32)delay), 4, "Delay");
                viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_EGAT_LENG_SAMP, (ViInt32)length), 5, "Length");
            break;
        }
        viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_EGAT_LINK, linkGateAndMarker), 6, "Link Gate Marker");
    }   
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX ACP
/* Purpose:  This function configures the ACP measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXACP (ViSession instrSession,
                                              ViInt32 acpMode,
                                              ViInt32 numberOfChannels,
                                              ViBoolean noiseCorrection)
{
    ViStatus    error = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_ACH_MODE, acpMode), 2, "ACP Mode");
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_ACH_ACP, numberOfChannels), 
                                            3, "Number Of Channels");
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_ACP_NOISE_CORR, noiseCorrection), 
                                            4, "Noise Correction");     
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX ACP Channel Bandwidth
/* Purpose:  This function configures the ACP channel bandwidth.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXACPChannelBandwidth (ViSession instrSession,
                                                              ViInt32 channel,
                                                              ViInt32 channelNumber,
                                                              ViReal64 bandwidth)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
  
    switch (channel){
        case RSSPECAN_VAL_ACP_TX:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_ACH_BW_CHAN, bandwidth), 
                                            4, "Bandwidth");        
        break;
        case RSSPECAN_VAL_ACP_ADJ:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_ACH_BW_ACH, bandwidth), 
                                            4, "Bandwidth");
        break;
        case RSSPECAN_VAL_ACP_ALT:
            if (rsspecan_invalidViInt32Range (channelNumber, 1, 4) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Number");  
            sprintf (buffer, "A%ld", channelNumber); 
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ACH_BW_ALT, bandwidth), 
                                            5, "Bandwidth");    
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Channel");         
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX ACP Channel Spacing
/* Purpose:  This function configures the ACP channel spacing.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXACPChannelSpacing (ViSession instrSession,
                                                            ViInt32 channel,
                                                            ViInt32 channelNumber,
                                                            ViReal64 spacing)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
  
    switch (channel){
        case RSSPECAN_VAL_ACP_ADJ:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_ACH_SPAC_ACH, spacing), 
                                            4, "Spacing");
        break;
        case RSSPECAN_VAL_ACP_ALT:
            if (rsspecan_invalidViInt32Range (channelNumber, 1, 4) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Number");  
            sprintf (buffer, "A%ld", channelNumber); 
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ACH_SPAC_ALT, spacing), 
                                            4, "Spacing");  
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Channel");         
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Level
/* Purpose:  This function configures the reference level, and reference 
/*           level offset.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXLevel(ViSession    instrSession,
                                                ViInt32     measurement,
                                                ViBoolean   autoLevel,
                                                ViReal64    referenceLevel,
                                                ViReal64    externalAttenuation)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (measurement, RSSPECAN_VAL_WIMAX_MEASUREMENT_RF, RSSPECAN_VAL_WIMAX_MEASUREMENT_IQ))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Measurement");
    }
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_AUTO, autoLevel), 
                                                3, "Auto Level");
    if (!autoLevel)
    {
        switch (measurement){
            case RSSPECAN_VAL_WIMAX_MEASUREMENT_RF:
                viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_RLEV_RF, referenceLevel), 
                                                4, "Reference Level");  
            break;
            case RSSPECAN_VAL_WIMAX_MEASUREMENT_IQ:
                viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_RLEV_IQ, referenceLevel), 
                                                4, "Reference Level");  
            break;
        }
    }       
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, externalAttenuation), 
                                                5, "External Attenuation");
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/*===========================================================================*/
/* Function: Configure WiMAX SEM Mode
/* Purpose:  This function sets the Spectrum Emission Mask (SEM) analysis to 
/*           be UL or DL (uplink or downlink).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSEMMode (ViSession instrSession,
                                                  ViInt32 linkMode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ESP_ANALYSIS, linkMode),
            2, "Link Mode");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX FS/BW
/* Purpose:  This function allows a user-defined Channel Bandwidth to Sampling 
/*           Rate relationship.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFSBW (ViSession instrSession,
                                               ViBoolean FSBWAuto,
                                               ViReal64 samplingRateFs,
                                               ViReal64 channelBandwidthBW)
{
    ViStatus    error = VI_SUCCESS;
    


    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_FSBW_RATIO_AUTO, FSBWAuto),
        2, "FS/BW Auto");
    if (FSBWAuto==VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_FSBW_SRATE, samplingRateFs),
            3, "Sampling Rate Fs");
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_FSBW_CHANNEL_BW, channelBandwidthBW),
            4, "Channel Bandwidth BW");
    }   
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Number Of Antennas
/* Purpose:  This function specifies the number of antennas.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXNumberOfAntennas(
    ViSession   instrSession,
    ViInt32 numberOfAntennas
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_NUMBER_OF_ANTENNAS,
            numberOfAntennas), 2, "Number Of Antennas");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Auto Level Once
/* Purpose:  This function performs the auto level routine immediately one 
/*           time regardless of the current state of the Auto Level State.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXAutoLevelOnce(ViSession    instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_AUTO_ONCE, ""));
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Baseband Signal Level
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXBasebandSignalLevel (ViSession instrSession,
                                                  ViBoolean autoLevel,
                                                  ViReal64   autoLevelTime,
                                                  ViReal64 signalLevel,
                                                  ViReal64 extAtt)
                                                 
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_AUTO, autoLevel), 2, "Auto Level");
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_AUTO_SWE_TIME, autoLevelTime), 3, "Auto Level Time");
    if ((!autoLevel)&& (strstr(model, "FSL")==NULL))
    {
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_EXP_IQ, signalLevel), 4, "Signal Level");
    }   
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, extAtt), 5, "Ext Att");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX RF Channel Frequency
/* Purpose:  This function the channel frequency.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXRFChannelFrequency(ViSession   instrSession,
                                                ViInt32     channelNo,
                                                ViReal64    frequency)
                                            
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (channelNo)
    {
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_CHAN, channelNo), 2, "Channel No");
    }   
    else
    {
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_CENTER, frequency), 3, "Frequency");
    }   


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX RF Signal Level
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXRFSignalLevel(ViSession    instrSession,
                                                ViBoolean   autoLevel,
                                                ViReal64    autoLevelTime,
                                                ViReal64    signalLevel,
                                                ViReal64    rfAtt,
                                                ViReal64    extAtt)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_AUTO, autoLevel), 2, "Auto Level");
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_AUTO_SWE_TIME, autoLevelTime), 3, "Auto Level Time");
    if ((!autoLevel) && (strstr(model, "FMU") == NULL))
    {
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ATTENUATION, rfAtt), 5, "RF Att");
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_POW_EXP_RF, signalLevel), 4, "Signal Level");
    }    
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, extAtt), 6, "Ext Att");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX RF Signal Level
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXReferenceLevel (ViSession instrSession,
                                                   ViReal64 *referenceLevel)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_GetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel), 
                2, "Reference Level");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Marker Enabled
/* Purpose:  This function enables the active marker on the specified trace.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkerEnabled(ViSession    instrSession,
                                                        ViInt32     window,
                                                        ViInt32     marker,
                                                        ViBoolean   markerEnabled,
                                                        ViInt32     trace)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (marker, 1, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_ENABLED, 
                markerEnabled), 4, "Marker Enabled");
    if (markerEnabled == VI_TRUE)                
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_MARKER_TRACE, 
                trace), 5, "Trace");
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Marker Position
/* Purpose:  This function configures the marker position.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkPosition(ViSession     instrSession,
                                                        ViInt32     window,
                                                        ViInt32     marker,
                                                        ViInt32     position,
                                                        ViReal64    value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (marker, 1, 1))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");
    }
    if (rsspecan_invalidViInt32Range (position, RSSPECAN_VAL_X, RSSPECAN_VAL_Y))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Position");
    }
    sprintf (buffer, "Win%ld,M%ld", window, marker);   
    switch (position){
        case RSSPECAN_VAL_X:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_MARKER_POSITION, value), 5, "Value");
        break; 
        case RSSPECAN_VAL_Y:
            viCheckParm( rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_WIMAX_MARK_Y, value), 5, "Value");
        break;    
    }    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Marker Position for Constellation
/* Purpose:  This function positions the selected marker to the indicated 
/*           symbol/carrier.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkerPositionConstellation(ViSession  instrSession,
                                                                    ViInt32 window,
                                                                    ViInt32 marker,
                                                                    ViInt32 type,
                                                                    ViInt32 position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (marker, 1, 1))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");
    }
    if (rsspecan_invalidViInt32Range (type, RSSPECAN_VAL_WIMAX_CONS_SYM, RSSPECAN_VAL_WIMAX_CONS_BURST))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Type");
    }
    sprintf (buffer, "C%ld,M%ld", window, marker);    
    switch (type){
        case RSSPECAN_VAL_WIMAX_CONS_SYM:
            viCheckParm( rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_MARK_SYMB, position), 5, "Position");
        break;    
        case RSSPECAN_VAL_WIMAX_CONS_CARR:
            viCheckParm( rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_MARK_CARR, position), 5, "Position");
        break; 
        case RSSPECAN_VAL_WIMAX_CONS_BURST:
            viCheckParm( rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_MARK_BURST, position), 5, "Position");
        break;
    }    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Marker Zoom
/* Purpose:  This function defines the ratio to be zoomed around the 
/*           selected marker in the selected measurement window.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkerZoom(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViReal64    zoomRatio)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);     
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_MARKER_ZOOM, zoomRatio), 4, "Zoom Ratio");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Set WiMAX Marker To Peak
/* Purpose:  This function set the selected marker to the min/max peak in 
/*           the current trace.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetWiMAXMarkerPeak(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 marker,
                                                ViInt32 position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (marker, 1, 1))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");
    }
    if (rsspecan_invalidViInt32Range (position, RSSPECAN_VAL_MARKER_SEARCH_HIGHEST, RSSPECAN_VAL_MARKER_SEARCH_MINIMUM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Position");
    }
    sprintf (buffer, "Win%ld,M%ld", window, marker);    
    switch (position){                                       

        case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN, ""));
        break;    
        case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK, ""));
        break;    
    }    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMax Disable All Markers
/* Purpose:  This function turns off all the markers in selected measurement 
/*           window.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXDisableAllMarkers(ViSession instrSession,
                                                    ViInt32 window)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MARKER_AOFF, 
                VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Demodulation
/* Purpose:  This function configures the WiMax demodulation parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDemodulation(ViSession instrSession,
                                                    ViBoolean   useSignalFieldContent,
                                                    ViInt32     modulationDetectionMode,
                                                    ViInt32     demodulator,
                                                    ViInt32     linkMode,
                                                    ViBoolean   channelEstimation,
                                                    ViBoolean   equalBurstLength,
                                                    ViInt32     minNumberofDataSymbols,
                                                    ViInt32     maxNumberofDataSymbols)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (minNumberofDataSymbols, 1, 1366))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Min Number of Data Symbols");
    }
    if (rsspecan_invalidViInt32Range (maxNumberofDataSymbols, 1, 1366))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Max Number of Data Symbols");
    }
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (strstr (buffer, "K92"))
    {
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_SIGS, useSignalFieldContent),
                                                    2, "Use Signal Field Content");
    }                                                   
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_BCON_AUTO, modulationDetectionMode), 
                                                    3, "Modulation Detection Mode");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_BAN, demodulator), 4, "Demodulator");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_LMODE, linkMode), 5, "Link Mode");          
  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_CEST, channelEstimation), 6, "Channel Estimation");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_SYM_EQU, equalBurstLength), 7, "Equal Burst Length");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "Min", RSSPECAN_ATTR_WIMAX_SYM, minNumberofDataSymbols), 8, "Min Number of Data Symbols");
    if (!equalBurstLength)
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "Max", RSSPECAN_ATTR_WIMAX_SYM, maxNumberofDataSymbols), 9, "Max Number of Data Symbols");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Configures the zone that will be used for auto demodulation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR autoDemodulationZone/Configures the zone that will be used for auto demodulation.

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXAutoDemodulation (ViSession instrSession,
                                                           ViInt32 autoDemodulationZone)
{
	ViStatus    error = VI_SUCCESS;
	checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
		RSSPECAN_ATTR_WIMAX_AUTO_DEMODULATION_ZONE, autoDemodulationZone), 2, "Auto Demodulation Zone");	
    
Error:
 	(void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Demodulation for OFDMA
/* Purpose:  This function configures the WiMAX OFDMA demodulation parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDemodulationOFDMA (ViSession instrSession,
                                                            ViInt32 channelEstimation,
                                                            ViReal64 FFTOffset)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_CEST_OFDMA, channelEstimation),
                                                2, "Channel Estimation");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_WIMAX_FFT_OFFSET, FFTOffset),
                                                3, "FFT Offset");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Subchannelization
/* Purpose:  This function configures the WiMax subchannelization 
/*           parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSubchannelization(ViSession    instrSession,
                                                            ViBoolean   state,
                                                            ViInt32 subchannel,
                                                            ViInt32 upLinkPhysicalModifier )
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_SUBCHANNEL_STATE, state), 
                                                2, "State");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SUBCHANNEL, subchannel),
                                                3, "Subchannel");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SUBCHANNEL_ULPH, upLinkPhysicalModifier),
                                                4, "Uplink Physical Modifier");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Tracking
/* Purpose:  This function configures the tracking parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTracking(ViSession instrSession,
                                                    ViInt32 basedOn, 
                                                    ViBoolean   phase,
                                                    ViBoolean   timing,
                                                    ViBoolean   level)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_CEST, basedOn), 2, "Based On");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_PHAS, phase), 3, "Phase");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_TIME, timing), 4, "Time");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_LEV, level), 5, "Level");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX OFDMA/WiBro Tracking
/* Purpose:  This function configures the tracking parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXOFDMATracking (ViSession instrSession,
                                                        ViInt32 basedOn,
                                                        ViBoolean phase,
                                                        ViBoolean timing,
                                                        ViInt32 trackingPilot)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_CEST, basedOn), 2, "Based On");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_PHAS, phase), 3, "Phase");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_TRAC_TIME, timing), 4, "Time");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_TRACK_PILOTS, trackingPilot), 
                5, "Tracking Pilot");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Enables/disables compensate crosstalk for MIMO.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR compansateCrosstalk/Enables/disables compensate crosstalk for MIMO.

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSTCMIMODemodulation (ViSession instrSession,
                                                              ViBoolean compansateCrosstalk)
{
	ViStatus    error = VI_SUCCESS;
	checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "",
		RSSPECAN_ATTR_WIMAX_COMPENSATE_CROSSTALK, compansateCrosstalk), 2, "Compensate Crosstalk");
	
Error:
 	(void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}

/// HIFN This function specifies how the information about the burst boosting factors is derived.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR burstBoosting/Specifies if the processing of the Channel Estimation uses the user selected Phase, Timing and Level settings or whether these settings are determined by the DSP, in which case all tracking options are used.

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXBurstBoosting (ViSession instrSession,
                                                        ViInt32 burstBoosting)
{
	ViStatus    error = VI_SUCCESS;

	checkErr( Rs_LockSession (instrSession, VI_NULL));
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
		RSSPECAN_ATTR_WIMAX_BURST_BOOSTING, burstBoosting), 2, "Burst Boosting");
Error:
  (void) Rs_UnlockSession(instrSession, VI_NULL);
  return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Select Filter
/* Purpose:  This function selects the TX and RX filters. The names of the 
/*           filters correspond to the file names returned by 
/*           RSSPECAN_ATTR_WIMAX_FILT_CAT attribute.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSelectFilter(ViSession instrSession,
                                            ViString    txFilterName,
                                            ViString    rxFilterName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr(model, "FSL")==NULL)
    {
        checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

        if (!(strstr (buffer, "K92")))
            viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    }   
    
    checkErr (viPrintf (instrSession, "DEM:FILT:MOD '%s','%s'\n", txFilterName, rxFilterName));
    checkErr( rsspecan_CheckStatus (instrSession));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX Filter Catalog
/* Purpose:  This function reads the names of all available filters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXFilterCatalog(ViSession  instrSession,
                                                ViInt32 bufferSize,
                                                ViChar  filterList[],
                                                ViInt32*    returnedLength)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *buff;
    ViUInt32     count=0;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr(model, "FSL")==NULL)
    {
        checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

        if (!(strstr (buffer, "K92")))
            viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    }   
    
    checkErr (viPrintf (instrSession, "DEM:FILT:CAT?\n"));
    checkErr (Rs_ReadDataUnknownLength(instrSession, &buff, &count));
    if (bufferSize != 0)
    {
        strncpy(filterList, buff, ((ViUInt32)bufferSize > count) ? count : bufferSize);
    }
    if (returnedLength) *returnedLength = count;
    checkErr( rsspecan_CheckStatus (instrSession));       
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Display
/* Purpose:  This function configures the display parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDisplay(ViSession  instrSession,
                                                ViBoolean   displayTable)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "Win1", RSSPECAN_ATTR_WIMAX_DISP_TABL, displayTable), 2, "Display Table");
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Display Result Table
/* Purpose:  This function displays the requested page for the results table. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDisplayResultTable (ViSession instrSession,
                                                             ViInt32 page)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_DISP_TABL_LIST, page), 
        2, "Page");
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
 
/*===========================================================================*/
/* Function: Configure WiMAX Limit
/* Purpose:  This function configures the limit.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXLimit(ViSession    instrSession,
                                                ViInt32     window,
                                                ViInt32     limit,
                                                ViInt32     limitType,
                                                ViInt32     mode,
                                                ViReal64    value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if ((strstr(model, "FSL")!=NULL)&&(strstr(buffer,"K93")!=NULL)) 
    {
        if (rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WIMAX_LIM_FERR, RSSPECAN_VAL_WIMAX_LIM_USERR))
        {
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Type");
        } 
    }
    else
    {
        if ((rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WIMAX_LIM_FERR, RSSPECAN_VAL_WIMAX_LIM_IQOFF)&&
            rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WIMAX_LIM_USERR, RSSPECAN_VAL_WIMAX_LIM_USERR)))
        {
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Type");
        } 
    }   
    if ((mode!=RSSPECAN_VAL_MEASTYPE_MAX) && (mode!=RSSPECAN_VAL_MEASTYPE_AVER))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Mode");
    }
    sprintf (buffer, "C%ld,L%ld", window, limit);
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            strcat (buffer, ",Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            strcat (buffer, ",Aver");
        break;
    }
    
    switch (limitType){
        case RSSPECAN_VAL_WIMAX_LIM_FERR:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FERR, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_SYMB:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_SYMB, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_SST:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_SST, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_EVM_ALL:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_EVM_ALL, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_EVM_DATA:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_EVM_DATA, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_IQOFF:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_IQOF, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_BERP:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_BERP, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_USERR:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_USERR, 
            value), 6, "Value");
        break;
    }    


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX All Limits
/* Purpose:  This function sets all limits.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXAllLimits (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 limit,
                                                    ViInt32 arraySize,
                                                    ViReal64 values[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     i=0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (!values)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Values");    
    }
    pbuffer = buffer;
    
    pbuffer += sprintf (pbuffer, "CALC%ld:LIM%ld:BURS:ALL ", window, limit);
    
    for (i=0;i<arraySize;i++)
         pbuffer += sprintf (pbuffer,"%f,", values[i]);
         
    *--pbuffer = '\0';
    
    viCheckErr (viPrintf (instrSession, "%s\n", buffer));
    
    
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Get WiMAX Spectrum Max Overstepping
/* Purpose:  This function returns the value at the maximum overstepping of 
/*           the spectrum mask limits.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXSpectrumMaxOverstepping(ViSession    instrSession,
                                                            ViInt32     window,
                                                            ViInt32     limit,
                                                            ViInt32     channelType,
                                                            ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (rsspecan_invalidViInt32Range (channelType, RSSPECAN_VAL_FREQ, RSSPECAN_VAL_POW))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Type");
    } 
    
    sprintf (buffer, "C%ld,L%ld", window, limit);
    switch (channelType){
        case RSSPECAN_VAL_FREQ:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_SPEC_MASK_X, 
                    value), 6, "Value");
        break;
        case RSSPECAN_VAL_POW:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_SPEC_MASK_Y, 
                    value), 6, "Value");
        break;
    }    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX All Limits
/* Purpose:  This function returns all limits.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXAllLimits(ViSession  instrSession,
                                            ViInt32     window,
                                            ViInt32     limit,
                                            ViReal64    values[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     maxIdx = 12;
    ViInt32     i=0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93"))) 
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (!values)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Values");    
    }
        
    checkErr (viQueryf (instrSession, ":CALC%ld:LIM%ld:BURS:ALL?\n","%s", window, limit, buffer));
                        
    pbuffer = strtok(buffer, ",");
    while ((pbuffer) && (i < maxIdx)) 
    {
        values[i++] = atof (pbuffer);
        pbuffer = strtok(NULL, ",");        
    }            
    
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Frame
/* Purpose:  This function configures the frame parameters
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrame (ViSession instrSession,
                                                ViInt32 IDCell,
                                                ViInt32 ULSubframeFrameNumber,
                                                ViReal64 TDDFrame,
                                                ViInt32 ULControlRegionLength)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_DL_IDCELL, IDCell),
                    2, "ID Cell");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_UL_FRAME, ULSubframeFrameNumber), 
                    3, "UL Subframe Frame Number");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_WIMAX_TDD_TTG, TDDFrame), 
                4, "TDD Frame");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_UL_FRAME_CRLEN, ULControlRegionLength), 
                    5, "UL Control Region Length");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Frame ID Cell Auto
/* Purpose:  This function configures that the IDCell number is determined by
/            analysis
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameIDCellAuto (ViSession instrSession,
                                                          ViBoolean IDCellAuto)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WIMAX_DOWN_LINK_ID_CELL_AUTO,
                                                 IDCellAuto), 2, "ID Cell Auto");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure WiMAX Frame Predefined Map
/* Purpose:  This function configures the predefined map mode for auto demod.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFramePredefinedMap (ViSession instrSession,
                                                             ViInt32 predefinedMap,
                                                             ViBoolean predefinedMapState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (predefinedMap, RSSPECAN_VAL_WIMAX_PREDEF_MAP_CONFIG, RSSPECAN_VAL_WIMAX_PREDEF_MAP_SMU))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Predefined Map");
    }
    
    sprintf (buffer, "%s", predefMapArr[predefinedMap]);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FRAME_PREDEFINED_MAP,
                                                 predefinedMapState), 3, "Predefined Map State");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX FrameOFDMA MAP
/* Purpose:  This function configures whether the DSP should perform a 
/*           pre-analysis to determine the Zone, Burst and Downlink
/*           sub–Channel Map for the current captured signal prior to
/*           calculating the results for OFDMA.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameOFDMAMAP (ViSession instrSession,
                                                   ViInt32 useforAnalysis)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_BCON_AUTO, useforAnalysis),
                    2, "Use For Analysis");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*===========================================================================*/
/* Function: Configure WiMAX Frame Downlink Preamble Index
/* Purpose:  This function specifies the preamble index calculation.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDLFramePreambleIndex (ViSession instrSession,
                                                               ViInt32 mode,
                                                               ViInt32 preamblePattern)
{  

    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_DL_PREAMBLE_MODE, mode),
                    2, "Mode");
    if (mode == RSSPECAN_VAL_WIMAX_PRE_USER)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_DL_PREAMBLE_INDEX, preamblePattern), 
                    3, "Preamble Pattern");
    }   
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Frame Downlink Used Subchannel
/* Purpose:  This function specifies the allowable logical sub channel usage of 
/*           the transmission spectrum for one of the three downlink PUSC 
/*           segments.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameDLUsedSubchannel (ViSession instrSession,
                                                                ViInt32 segment,
                                                                ViInt32 subchannelUsage)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (segment, 1, 3))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Segment");
    }
    sprintf (buffer, "SEG%ld", segment);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_DL_SUBCHANNEL, 
                    subchannelUsage), 3, "Subchannel Usage");
                        

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Frame Uplink Allocated Subchannel
/* Purpose:  This function specifies the allowable logical sub channel bitmap 
/*           usage of the transmission spectrum for one of the three uplink 
/*           segments. Note that the indexes specified in the GUI are 0 based 
/*           where as under remote control they a 1 based, i.e The first 
/*           segment in the GUI is labelled segement 0 and would be accessed 
/*           with the command.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameULAllocatedSubchannel (ViSession instrSession,
                                                                     ViInt32 segment,
                                                                     ViInt32 bitmap,
                                                                     ViInt32 logicalSubchannel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (segment, 1, 3))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Segment");
    }
    if (rsspecan_invalidViInt32Range (bitmap, 1, 18))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Bitmap");
    }
    sprintf (buffer, "SEG%ld,B%ld", segment, bitmap);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_UL_SUBCHANNEL_BITMAP, 
                    logicalSubchannel), 3, "Logical Subchannel");
                        

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Load WiMAX Frame Data
/* Purpose:  This function loads a K93 zone frame setup from an xml file 
/*           created from a SMU signal generator, with support for 
/*           the IEEE 80216e/D11 standard.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_LoadWiMAXFrameData (ViSession instrSession,
                                               ViString fileName)
{
    ViStatus    error = VI_SUCCESS;
    if (!fileName)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "File Name");   
    }
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_FRAME_LOAD, fileName));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Store WiMAX Frame Data
/* Purpose:  This function sends a locally created and stored K93 zone frame 
/*           file name, with support for the IEEE 80216e/D11 standard, to the 
/*           SMU signal generator. If the command succeeds, the SMU will generate 
/*           the waveform associated with the file sent.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_StoreWiMAXFrameData (ViSession instrSession,
                                                ViString fileName)
{
    ViStatus    error = VI_SUCCESS;
    if (!fileName)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "File Name");   
    }
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_FRAME_SAVE, fileName));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Create WiMAX New Zone
/* Purpose:  This function defines a new zone. It accepts only seven arguments 
/*           which make up all the input settings to create a new zone. Up to 
/*           26 user zones can be defined.
/*           New zones can only be appended to the end of the existing defined 
/*           zones. For example if 4 zones are already defined, then the suffix 
/*           required to enter a new zone is 5.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_CreateWiMAXNewZone (ViSession instrSession,
                                               ViInt32 zone, ViBoolean analyze,
                                               ViInt32 type, ViInt32 segment,
                                               ViInt32 length, ViInt32 offset,
                                               ViInt32 permbase, ViInt32 PRBS_ID)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    if (rsspecan_invalidViBooleanRange (analyze))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Analyze");
    }
    if (rsspecan_invalidViInt32Range (type, 0, 4))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Type");
    }
    if (rsspecan_invalidViInt32Range (segment, 0, 2))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Segment");
    }
     
    sprintf (buffer, ":CONF:WIM:ZONE%ld:CONT %s,%s,%ld,%ld,%ld,%ld,%ld\n", zone, 
                        switchArr[analyze], rsspecan_rngWiMAXZoneTypeFSV.rangeValues[type].cmdString, 
                        segment, length, offset, permbase, PRBS_ID);
    viCheckErr (viWrite (instrSession, (ViBuf)buffer, (ViUInt32) strlen (buffer), NULL));
    checkErr( rsspecan_CheckStatus (instrSession)); 
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;                        
}

/*===========================================================================*/
/* Function: Configure WiMAX Zone
/* Purpose:  This function configures zone parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZone (ViSession instrSession,
                                               ViInt32 zone, 
                                               ViBoolean analyze,
                                               ViInt32 type, 
                                               ViInt32 segment,
                                               ViInt32 length, 
                                               ViInt32 offset,
                                               ViInt32 permbase, 
                                               ViInt32 PRBS_ID)
{
    ViStatus    error = VI_SUCCESS; 
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    sprintf (buffer, "Z%ld", zone);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_ANALYZE_STATE, analyze), 
                3, "Analyze");
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_TYPE, type), 
                4, "Type");
                    
    switch (type){
        case RSSPECAN_VAL_WIMAX_ZONE_TYPE_DLPUSC:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_SEGMENT_NUM, segment), 
                5, "Segment");
        break;
        default:
        break;
    }   
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_SYMBOL_COUNT, length), 
                6, "Length");

    //Not supported now
    //  viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_SYMBOL_OFFSET, offset), 
    //          7, "Offset");
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_PERM_BASE, permbase), 
                8, "Permbase");
    switch (type){
        case RSSPECAN_VAL_WIMAX_ZONE_TYPE_DLFUSC:
        case RSSPECAN_VAL_WIMAX_ZONE_TYPE_DLPUSC:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_PRBS, PRBS_ID), 
                9, "PRBS ID");  
        break;
        default:
        break;
    }   
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;   
}

/// HIFN  Specifies the STC Mode for any uplink zone (uplink MIMO only).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR zone/Selects the user defined zone.
/// HIPAR STCMode/Specifies the STC Mode for any uplink zone (uplink MIMO only).

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneSTCMode (ViSession instrSession,
                                                      ViInt32 zone,
                                                      ViInt32 STCMode)
{
	ViStatus    error = VI_SUCCESS;
  ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   

	checkErr( Rs_LockSession (instrSession, VI_NULL));

	if (rsspecan_invalidViInt32Range (zone, 1, 26) == VI_TRUE)
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");

	sprintf (buffer, "Z%ld", zone); 
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer,
		RSSPECAN_ATTR_WIMAX_UPLINK_ZONE_STC_MODE, STCMode), 3, "STC Mode");
    
Error:
 	(void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}


/*===========================================================================*/
/* Function: Configure WiMAX Zone Space Time Coding
/* Purpose:  This function configures the space time coding.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneSpaceTimeCoding(
    ViSession   instrSession,
    ViInt32 zone,
    ViInt32 spaceTimeCoding
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    
    sprintf (buffer, "Z%ld", zone); 
    
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_SPACE_TIME_CODING, spaceTimeCoding), 
                3, "Space Time Coding");  
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Zone Matrix
/* Purpose:  This function specifies the matrix configuration for the zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneMatrix (ViSession instrSession,
                                                     ViInt32 zone,
                                                     ViInt32 matrixType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    
    sprintf (buffer, "Z%ld", zone); 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                               RSSPECAN_ATTR_WIMAX_ZONE_MATRIX, 
                                               matrixType), 3, "Matrix Type");  
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Zone Martix
/* Purpose:  This function specifies the matrix configuration for the zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneMartix (ViSession instrSession,
                                                     ViInt32 zone,
                                                     ViInt32 matrixType)
{
    ViStatus    error = VI_SUCCESS;
    
    viCheckErr (rsspecan_ConfigureWiMAXZoneMatrix (instrSession, zone, matrixType));
    
Error:
    return error;
}

/*===========================================================================*/
/* Function: Create WiMAX New Zone Burst
/* Purpose:  This function can be used to enter a burst definition that is to 
/*           be associated with a specific zone. It accepts eight arguments 
/*           which make up all the input settings to create a new burst.
/*           A zone may have up to 32 bursts defined within it.
/*           New bursts can only be appended to the end of the existing burst
/*           list. For example if 4 bursts are already defined, then the
/*           suffix required to enter a new burst is 5.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_CreateWiMAXNewZoneBurst (ViSession instrSession,
                                                    ViInt32 zone, 
                                                    ViInt32 burst,
                                                    ViInt32 modulation,
                                                    ViInt32 subchannels,
                                                    ViInt32 symbols,
                                                    ViInt32 slotDuration,
                                                    ViInt32 subchannelOffset,
                                                    ViInt32 symbolOffset,
                                                    ViReal64 burstPower,
                                                    ViInt32 burstType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    if (rsspecan_invalidViInt32Range (burst, 1, 32))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Burst");
    }
    if (rsspecan_invalidViInt32Range (modulation, 0, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Modulation");
    }
    if (rsspecan_invalidViInt32Range (burstType, 0, 5))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Burst Type");
    }
    viCheckErr (viPrintf (instrSession, ":CONF:WIM:ZONE%ld:BURS%ld:CONT %s,%ld,%ld,%ld,%ld,%ld,%.12lf,%s\n", zone, burst,  
                        rsspecan_rngWiMAXZoneBurstFormat.rangeValues[modulation].cmdString, subchannels, symbols, slotDuration, 
                        subchannelOffset, 
                        symbolOffset, burstPower, rsspecan_rngWiMAXZoneBurstType.rangeValues[burstType].cmdString));
    checkErr( rsspecan_CheckStatus (instrSession)); 
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;                        
}

/*===========================================================================*/
/* Function: Configure WiMAX Zone Burst
/* Purpose:  This function configures burst with a specific zone. A zone may 
/*           have up to 32 bursts defined within it.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurst (ViSession instrSession,
                                                    ViInt32 zone, 
                                                    ViInt32 burst,
                                                    ViInt32 modulation,
                                                    ViInt32 subchannels,
                                                    ViInt32 symbols,
                                                    ViInt32 slotDuration,
                                                    ViReal64 burstPower,
                                                    ViInt32 burstType,
                                                    ViBoolean offsetAuto,
                                                    ViInt32 subchannelOffset,
                                                    ViInt32 symbolOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViInt32     tmp_zone = RSSPECAN_VAL_WIMAX_ZONE_TYPE_ULPUSC;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
     sprintf (buffer, "Z%ld", zone); 
    if ((strstr(model, "FSL") != NULL))
    {
        viCheckErr(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_TYPE, &tmp_zone));       
    }
    sprintf (buffer, "Z%ld,BU%ld", zone, burst); 
    
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_FORMAT, modulation), 
                4, "Modulation");
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_SYMBOL_COUNT, symbols), 
                6, "Symbols");
    if (burstType == RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_ULMAP)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_SLOT_DURATION, slotDuration), 
                7, "Slot Duration");
    } 
    else
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_SUBC_COUNT, subchannels), 
                5, "Subchannels");
    }
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_POWER, burstPower), 
                8, "Burst Power");
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_TYPE, burstType), 
                9, "Burst Type");
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_OFFSET_AUTO, offsetAuto), 
                10, "Offset Auto");
    if (offsetAuto == VI_FALSE)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_SUBC_OFFSET, subchannelOffset), 
                11, "Subchannel Offset");
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_SYMBOL_OFFSET, symbolOffset), 
                12, "Symbol Offset");
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Zone Burst Matrix
/* Purpose:  This function specifies the matrix configuration for the burst.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurstMatrix (ViSession instrSession,
                                                          ViInt32 zone,
                                                          ViInt32 burst,
                                                          ViInt32 matrixType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    if (rsspecan_invalidViInt32Range (burst, 1, 32))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Burst");
    }
    
    sprintf (buffer, "Z%ld,BU%ld", zone, burst);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                               RSSPECAN_ATTR_WIMAX_ZONE_BURST_MATRIX, 
                                               matrixType), 4, "Matrix Type");  
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Zone Burst Martix
/* Purpose:  This function specifies the matrix configuration for the burst.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurstMartix (ViSession instrSession,
                                                          ViInt32 zone,
                                                          ViInt32 burst,
                                                          ViInt32 matrixType)
{
    ViStatus    error = VI_SUCCESS;
    
    viCheckErr (rsspecan_ConfigureWiMAXZoneBurstMatrix (instrSession, zone, 
                                               burst, matrixType));
    
Error:
    return error;
}

/// HIFN  Specifies the STC mode for the burst (uplink MIMO only).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR zone/Selects the user defined zone.
/// HIPAR burst/Selects the user defined burst.
/// HIPAR STCMode/Specifies the STC mode for the burst (uplink MIMO only).

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurstSTCMode (ViSession instrSession,
                                                           ViInt32 zone,
																													 ViInt32 burst,
                                                           ViInt32 STCMode)
{
	ViStatus    error = VI_SUCCESS;
  ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   

	checkErr( Rs_LockSession (instrSession, VI_NULL));

	
	if (rsspecan_invalidViInt32Range (zone, 1, 26) == VI_TRUE)
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");

	if (rsspecan_invalidViInt32Range (burst, 1, 32) == VI_TRUE)
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Burst");

	sprintf (buffer, "Z%ld,BU%ld", zone, burst); 	
	
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer,
		RSSPECAN_ATTR_WIMAX_MIMO_BURST_STC_MODE, STCMode), 4, "STC Mode");
		
Error:
 	(void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Ext Frame Zone Generator Address
/* Purpose:  This function allows an external SMU signal generator to be connected 
/*           via TCPIP to K93, so that frame zone setup can be downloaded 
/*           directly to K93.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXExtFrameZoneGeneratorAddress (ViSession instrSession,
                                                                       ViString TCPIPAddress)
{
    ViStatus    error = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SMU_ADDRESS, TCPIPAddress), 
                2, "TCP/IP Address");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX Zone Count
/* Purpose:  This function returns the current count of user defined zones.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXZoneCount (ViSession instrSession,
                                              ViInt32 *count)
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_ZONE_COUNT_QUERY, count), 
                3, "Count");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX Zone Burst Count
/* Purpose:  This function returns the current count of user defined bursts 
/*           within the specified zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXZoneBurstCount (ViSession instrSession,
                                                   ViInt32 zone, 
                                                   ViInt32 *count)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    
    sprintf (buffer, "Z%ld", zone); 
    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_COUNT_QUERY, count), 
                3, "Count");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMAX Zone Delete
/* Purpose:  This function deletes a specific zone. If the specified zone is 
/*           within a larger list of zones, then all following zones will be 
/*           shuffled down to take up the space occupied by the deleted zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXZoneDelete (ViSession instrSession,
                                               ViInt32 zone)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    
    sprintf (buffer, "Z%ld", zone); 
    viCheckErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_DELETE, VI_NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: WiMAX Zone Burst Delete
/* Purpose:  This function deletes a specific burst from within the specified 
/*           zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXZoneBurstDelete (ViSession instrSession,
                                                 ViInt32 zone, 
                                                 ViInt32 burst)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    if (rsspecan_invalidViInt32Range (burst, 1, 32))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Burst");
    }
    
    sprintf (buffer, "Z%ld,BU%ld", zone, burst); 
    viCheckErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_DELETE, VI_NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMAX Zone Reset
/* Purpose:  This function removes all zone and burst data.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXZoneReset (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_ZONE_RESET, VI_NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMAX Zone Burst Reset
/* Purpose:  This function removes all burst data associated with a specific zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXZoneBurstReset (ViSession instrSession,
                                                ViInt32 zone)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (zone, 1, 26))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Zone");
    }
    
    sprintf (buffer, "Z%ld", zone); 
    viCheckErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_WIMAX_ZONE_BURST_RESET, VI_NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Measurement
/* Purpose:  This function configures the measurement parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMeas(ViSession     instrSession,
                                              ViBoolean overallBurstCount,
                                              ViInt32 noofBursttoAnalyze,
                                              ViReal64 captureTime,
                                              ViBoolean swapIQ,
                                              ViInt32 sweepCount,
                                              ViBoolean sweepTimeAutoMaskACP,
                                              ViReal64 sweepTimeMaskACP,
                                              ViInt32 frequencyBand,
                                              ViReal64 channelBandwidth,
                                              ViInt32 guardRatio)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_BURS_COUN_STAT, overallBurstCount), 2, "Overall Burst Count");
    if (overallBurstCount)
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_BURS_COUN, noofBursttoAnalyze), 3, "No of Burst to Analyze");
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_FBAN, frequencyBand), 
                                                9, "Frequency Band");  
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_BAND_CHAN, channelBandwidth), 
                                                10, "Channel Bandwidth");  
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME , captureTime), 4, "Capture Time");
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_SWAP_IQ, swapIQ), 5, "Swap IQ");
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, sweepCount), 6, "Sweep Count");
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_ACPR_TIME_AUTO, sweepTimeAutoMaskACP),
                7, "Sweep Time Auto (Mask/ACP)");
    if (sweepTimeAutoMaskACP == VI_FALSE)
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_ACPR_TIME, sweepTimeMaskACP), 
                                                8, "Sweep Time (Mask/ACP)");  
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_IGR, guardRatio), 
                                                11, "Guard Ratio"); 

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Measurement for OFDMA
/* Purpose:  This function configures the measurement parameters for OFDMA Standard. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMeasOFDMA (ViSession instrSession,
                                                    ViReal64 captureTime,
                                                    ViBoolean swapIQ,
                                                    ViInt32 sweepCount,
                                                    ViBoolean sweepTimeAutoMaskACP,
                                                    ViReal64 sweepTimeMaskACP,
                                                    ViInt32 frequencyBand,
                                                    ViReal64 channelBandwidth,
                                                    ViInt32 guardRatio,
                                                    ViInt32 FFTSize,
                                                    ViBoolean captureCountState,
                                                    ViInt32 numberofSubframestoAnalyze)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (frequencyBand, RSSPECAN_VAL_WIMAX_FBAN_UNSP, RSSPECAN_VAL_WIMAX_FBAN_WCS))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Frequency Band");
    }
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME , captureTime), 2, "Capture Time");
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_SWAP_IQ, swapIQ), 3, "Swap IQ");
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, sweepCount), 4, "Sweep Count");
     viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_ACPR_TIME_AUTO, sweepTimeAutoMaskACP),
                5, "Sweep Time Auto (Mask/ACP)");
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_ACPR_TIME, sweepTimeMaskACP), 
                                                6, "Sweep Time (Mask/ACP)");  
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_FBAN, frequencyBand), 
                                                7, "Frequency Band");  
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_BAND_CHAN, channelBandwidth), 
                                                8, "Channel Bandwidth");                                                   
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_IGR, guardRatio), 
                                                9, "Guard Ratio"); 
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_NFFT, FFTSize), 
                                                10, "FFT Size"); 
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_ZONE_COUNT_STATE, captureCountState), 
                                                11, "Capture Count State"); 
    if (captureCountState==VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_ZONE_COUNT, numberofSubframestoAnalyze), 
                                                12, "Number Of Frames to Analyze"); 
    }
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure WiMAX Measurement for WiBro
/* Purpose:  This function configures the measurement parameters for WiBro Standard. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMeasWiBro (ViSession instrSession,
                                                    ViReal64 captureTime,
                                                    ViBoolean swapIQ,
                                                    ViInt32 sweepCount,
                                                    ViBoolean sweepTimeAutoMaskACP,
                                                    ViReal64 sweepTimeMaskACP,
                                                    ViReal64 channelBandwidth,
                                                    ViBoolean captureCountState,
                                                    ViInt32 numberofSubframestoAnalyze)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME , captureTime), 2, "Capture Time");
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_SWAP_IQ, swapIQ), 3, "Swap IQ");
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, sweepCount), 4, "Sweep Count");
     viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_ACPR_TIME_AUTO, sweepTimeAutoMaskACP),
                6, "Sweep Time Auto (Mask/ACP)");
    if (sweepTimeAutoMaskACP == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_ACPR_TIME, sweepTimeMaskACP), 
                                                    6, "Sweep Time (Mask/ACP)");  
    }   
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_BAND_CHAN, channelBandwidth), 
                                                7, "Channel Bandwidth");   
    
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_WIMAX_ZONE_COUNT_STATE, captureCountState), 
                                                8, "Capture Count State"); 
    if (captureCountState==VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_ZONE_COUNT, numberofSubframestoAnalyze), 
                                                9, "Number Of Frames to Analyze"); 
    }
 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX PVT Measurement
/* Purpose:  This function sets measurement type to be Power Versus Time and 
/*           configures measurement parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXPVTMeas(ViSession  instrSession,
                                                ViInt32 risingFalling,
                                                ViInt32 burstSelection)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_PVT_SEL, risingFalling), 2, "Rising/Falling");
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_PVT, ""));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_BURS_PVT_BURS, burstSelection),
                                                3, "Burst Selection");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX PVT Measurement
/* Purpose:  This function sets measurement type to be Power Versus Time and 
/*           configures measurement parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXPVTMeasOFDMA (ViSession instrSession,
                                                       ViInt32 risingFalling)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_PVT_SEL, risingFalling), 2, "Rising/Falling");
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_PVT, ""));
   

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX EVM Measurement
/* Purpose:  This function sets measurement type to be EVM.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXEVMMeasurement(ViSession   instrSession,
                                                        ViInt32     mode,
                                                        ViBoolean   autoScaling,
                                                        ViReal64    perDivision,
                                                        ViInt32     unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_WIMAX_EVM_MODE_SYMBOL, RSSPECAN_VAL_WIMAX_EVM_MODE_FREQ))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");
    }
     
    switch (mode){
        case RSSPECAN_VAL_WIMAX_EVM_MODE_SYMBOL:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_EVM_ESYM, ""));
            viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_UNIT_EVM, unit), 5, "Unit");
        break;
        case RSSPECAN_VAL_WIMAX_EVM_MODE_CARRIER:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_EVM_ECAR, ""));
            viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_UNIT_EVM, unit), 5, "Unit");
        break;
        case RSSPECAN_VAL_WIMAX_EVM_MODE_PHAS:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_PRE, "")); 
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_PRE_SEL, RSSPECAN_VAL_PRE_PHAS));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_UNIT_PRE, RSSPECAN_VAL_UNIT_DEG));
        break;
        case RSSPECAN_VAL_WIMAX_EVM_MODE_FREQ:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_PRE, "")); 
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_PRE_SEL, RSSPECAN_VAL_PRE_FREQ));
            viCheckErr( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_UNIT_PRE, RSSPECAN_VAL_UNIT_HZ));            
        break;
    }
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "Win2,TR1", RSSPECAN_ATTR_WIMAX_DISP_AUTO_Y, autoScaling), 3, "Auto Scaling");
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "Win2,TR1", RSSPECAN_ATTR_WIMAX_Y_PDIV, perDivision), 4, "Per Division");
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Spectrum Measurement
/* Purpose:  This function sets measurement type to be Spectrum.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSpectrumMeasurement(ViSession  instrSession,
                                                             ViInt32    mode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
        if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_WIMAX_MODE_IEEE, RSSPECAN_VAL_WIMAX_MODE_PREAMBLE))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");
    }
    switch (mode){
        case RSSPECAN_VAL_WIMAX_MODE_IEEE:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK_SEL,
                                                                            RSSPECAN_VAL_SPEC_IEEE));
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_ETSI:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK_SEL,
                                                      RSSPECAN_VAL_SPEC_ETSI));
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_ETSI5:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK_SEL,
                                                      RSSPECAN_VAL_SPEC_ETSI5));
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_ETSI10:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK_SEL,
                                                      RSSPECAN_VAL_SPEC_ETSI10));
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_FFT:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_FFT, ""));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_FLAT:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_FLAT,
                                                      RSSPECAN_VAL_WIMAX_FLAT_FLAT));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_FLAT_GRDEL:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_FLAT,
                                                      RSSPECAN_VAL_WIMAX_FLAT_GRD));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_FLAT_DIFF:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_FLAT,
                                                      RSSPECAN_VAL_WIMAX_FLAT_DIF));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_ACPR_ABS:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_ACPR, ""));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_ACPR_SEL,
                                                      RSSPECAN_VAL_ABS));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_ACPR_REL:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_ACPR, ""));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_ACPR_SEL,
                                                      RSSPECAN_VAL_REL));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_MASK:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_MASK, ""));
        break;
        case RSSPECAN_VAL_WIMAX_MODE_PREAMBLE:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WIMAX_SPEC_PREAMBLE, ""));
        break;
    }
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Constellation Measurement
/* Purpose:  This function configures the Constellation measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXConstellationMeasurement(ViSession instrSession,
                                                                    ViInt32 constellationMode,
                                                                    ViInt32 carrierSelection,
                                                                    ViInt32 carrierNumber)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (constellationMode, RSSPECAN_VAL_WIMAX_CONS_SYM, RSSPECAN_VAL_WIMAX_CONS_CARR))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Constelation Mode");
    }
    if (rsspecan_invalidViInt32Range (carrierSelection, RSSPECAN_VAL_SELECTION_ALL, RSSPECAN_VAL_SELECTION_NUM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Carrier Selection");
    }
    switch (constellationMode){
        case RSSPECAN_VAL_WIMAX_CONS_SYM:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_CONS_CSYM, ""));
            switch (carrierSelection){
                case RSSPECAN_VAL_SELECTION_NUM:
                    if (rsspecan_invalidViInt32Range (carrierNumber, -100, 100) || (carrierNumber == 0))
                    {
                        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Carrier Number");
                    }
                    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Carrier", RSSPECAN_ATTR_WIMAX_CONS_SEL_NR, carrierNumber), 4, "Carrier Number");                        
                break; 
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "Carrier",
                                                               RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                               RSSPECAN_VAL_SELECTION_ALL), 
                                                              3, "Carrier Selection");
                break;
                case RSSPECAN_VAL_SELECTION_PIL:
                    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "Carrier",
                                                               RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                               RSSPECAN_VAL_SELECTION_PIL), 3, "Carrier Selection");
                break;
             }   
        break;
        case RSSPECAN_VAL_WLAN_CONS_CARR:
            viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_CONS_CCAR, ""));
        break;
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Constellation Selection
/* Purpose:  This function configures the Constellation Selection for 
*            IEEE 802.16e-2005 OFDMA & WiBro.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXConstellationSelection (ViSession instrSession,
                                                                 ViInt32 modulation,
                                                                 ViInt32 burstSelection,
                                                                 ViInt32 burstNumber,
                                                                 ViInt32 carrierSelection,
                                                                 ViInt32 carrierNumber,
                                                                 ViInt32 symbolSelection,
                                                                 ViInt32 symbolNumber)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (burstSelection, RSSPECAN_VAL_SELECTION_ALL, RSSPECAN_VAL_SELECTION_NUM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Burst Selection");
    }
    if (rsspecan_invalidViInt32Range (carrierSelection, RSSPECAN_VAL_SELECTION_ALL, RSSPECAN_VAL_SELECTION_NUM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Carrier Selection");
    }
    if ((symbolSelection!=RSSPECAN_VAL_SELECTION_ALL)&&(symbolSelection!=RSSPECAN_VAL_SELECTION_NUM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Symbol Selection");
    }
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "Constellation", RSSPECAN_ATTR_WIMAX_FORMAT_SELECT,
                                                               modulation), 3, "Modulation");
                                                               
    switch (burstSelection){
                case RSSPECAN_VAL_SELECTION_NUM:
                    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Burst", RSSPECAN_ATTR_WIMAX_CONS_SEL_NR, 
                    burstNumber), 4, "Burst Number");                        
                break; 
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Burst",
                                                               RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                               RSSPECAN_VAL_SELECTION_ALL));
                break;
                case RSSPECAN_VAL_SELECTION_PIL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Burst",
                                                               RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                               RSSPECAN_VAL_SELECTION_PIL));
                break;
                
    }   
    switch (carrierSelection){
                case RSSPECAN_VAL_SELECTION_NUM:
                    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Carrier", RSSPECAN_ATTR_WIMAX_CONS_SEL_NR, 
                    carrierNumber), 6, "Carrier Number");                        
                break; 
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Carrier",
                                                               RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                               RSSPECAN_VAL_SELECTION_ALL));
                break;
                case RSSPECAN_VAL_SELECTION_PIL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Carrier",
                                                              RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                              RSSPECAN_VAL_SELECTION_PIL));
                break;
                
    }
    switch (symbolSelection){
                case RSSPECAN_VAL_SELECTION_NUM:
                    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Symbol", RSSPECAN_ATTR_WIMAX_CONS_SEL_NR, 
                    symbolNumber), 8, "Symbol Number");                        
                break; 
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Symbol",
                                                              RSSPECAN_ATTR_WIMAX_CONS_SEL,
                                                              RSSPECAN_VAL_SELECTION_ALL));
                break;
    }  

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Specify whether the Constellation diagram should be normalized.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR normalization/Activates or deactivates normalization of the Constellation diagram.
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXConstellationDiagram (ViSession instrSession,
                                                                     ViBoolean normalization)

{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", 
		RSSPECAN_ATTR_WIMAX_CONSTELLATION_DIAGRAM, normalization), 2, "Normalization");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Bitstream Selection
/* Purpose:  This function configures the Bitstream Selection for 
/*           IEEE 802.16e-2005 OFDMA & WiBro.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXBitstreamSelection (ViSession instrSession,
                                                             ViInt32 modulation,
                                                             ViInt32 burstSelection,
                                                             ViInt32 burstNumber,
                                                             ViInt32 symbolSelection,
                                                             ViInt32 symbolNumber)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (burstSelection, RSSPECAN_VAL_SELECTION_NUM, RSSPECAN_VAL_SELECTION_PIL))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Burst Selection");
    }
    if (rsspecan_invalidViInt32Range (symbolSelection, RSSPECAN_VAL_SELECTION_NUM, RSSPECAN_VAL_SELECTION_ALL))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Symbol Selection");
    }
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "Bitstream", RSSPECAN_ATTR_WIMAX_FORMAT_SELECT,
                                                               modulation), 3, "Modulation");
                     
    switch (burstSelection){
                case RSSPECAN_VAL_SELECTION_NUM:
                    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Burst", RSSPECAN_ATTR_WIMAX_BSTR_SEL_NR, 
                    burstNumber), 4, "Burst Number");                        
                break; 
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Burst",
                                                               RSSPECAN_ATTR_WIMAX_BSTR_SEL,
                                                               RSSPECAN_VAL_SELECTION_ALL));
                break;
                case RSSPECAN_VAL_SELECTION_PIL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Burst",
                                                               RSSPECAN_ATTR_WIMAX_BSTR_SEL,
                                                               RSSPECAN_VAL_SELECTION_PIL));
                break;
                
    }   
    switch (symbolSelection){
                case RSSPECAN_VAL_SELECTION_NUM:
                    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Symbol", RSSPECAN_ATTR_WIMAX_BSTR_SEL_NR, 
                    symbolNumber), 8, "Symbol Number");                        
                break; 
                case RSSPECAN_VAL_SELECTION_ALL:
                    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Symbol",
                                                              RSSPECAN_ATTR_WIMAX_BSTR_SEL,
                                                              RSSPECAN_VAL_SELECTION_ALL));
                break;
    }  

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX Statistics Measurement
/* Purpose:  This function configures the statistics measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXStatisticsMeasurement(ViSession    instrSession,
                                                                ViInt32     mode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_WIMAX_STAT_CCDF, RSSPECAN_VAL_WIMAX_STAT_BSUM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");
    }
    switch (mode){
        case RSSPECAN_VAL_WIMAX_STAT_CCDF:
            viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WIMAX_STAT_CCDF, ""));
        break;
        case RSSPECAN_VAL_WIMAX_STAT_BSTR:
            viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WIMAX_STAT_BSTR, ""));
        break;
        case RSSPECAN_VAL_WIMAX_STAT_BSUM:
            viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WIMAX_STAT_BSUM, ""));
        break;
    }
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX IQ Measurement Parameters
/* Purpose:  This function configures the IQ Trace parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXIQMeasurementParameters(ViSession  instrSession,
                                                                ViReal64    sampleRate,
                                                                ViInt32     filter)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_SRAT, sampleRate), 2, "Sample Rate");
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
    if (strstr (buffer, "B72"))
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_FLAT, filter), 3, "Filter");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX SEM Measurement
/* Purpose:  This function configures the Spectrum Emission Mask measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSEMMeasurement (ViSession instrSession,
                                                        ViInt32 type,
                                                        ViString fileName,
                                                        ViInt32 powerClass)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_SEM_TYPE, type), 2, "Type");
    if (type == RSSPECAN_VAL_SEM_USER)
    {
        viCheckParm( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_SEM_FILE, fileName), 3, "File Name");
    }
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_SEM_CLASS, powerClass), 4, "Power Class");          
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX TTC Frame
/* Purpose:  This function sets the frame that the Time to Capture Buffer 
/*           marker is to be set to.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTTCFrame (ViSession instrSession,
                                                   ViInt32 frame)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WIMAX_TTC_FRAME, frame), 
        2, "Frame");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the frame length measure setup.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR referencePowerType/Sets the type of the reference power for the 'subframe length’ calculation.
/// HIPAR userRefPower/Sets the reference power of the 'subframe length' calculation.
/// HIPAR referencePowerOffset/Sets the type of the reference power offset of the 'subframe length’ calculation.
/// HIPAR userRefPowerOffset/Sets the reference power offset of the 'subframe length' calculation.
/// HIPAR averaging/Sets the smoothing factor of the 'subframe length’ calculation. It may only be odd.

ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTTCSubFrameLengthMeasure (ViSession instrSession,
                                                                   ViInt32 referencePowerType,
                                                                   ViReal64 userRefPower,
                                                                   ViInt32 referencePowerOffset,
                                                                   ViReal64 userRefPowerOffset,
                                                                   ViInt32 averaging)
{		
	ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
    		RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER_TYPE, referencePowerType), 2, "Reference Power Type");	
	
	if (referencePowerType == RSSPECAN_VAL_WIMAX_SUBFRAME_RPTY_USER)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "",
    		RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER, userRefPower), 3, "User Reference Power");
	}
	
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
    	RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER_OFFSET_TYPE, referencePowerOffset), 4, "Reference Power Offset Type");
	
	if (referencePowerOffset == RSSPECAN_VAL_WIMAX_SUBFRAME_RPOT_USER)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "",
	    	RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER_OFFSET, userRefPowerOffset), 5, "User Ref. Power Offset");
	}
    
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
        RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_AVERAGING, averaging), 6, "Averaging");
		
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function returns the result of the 'subframe length’ calculation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR subframeLength/Returns the result of the 'subframe length’ calculation.

ViStatus _VI_FUNC rsspecan_GetWiMAXTTCSubFrameLength (ViSession instrSession,
                                                      ViInt32 *subframeLength)
{
	ViStatus	error = VI_SUCCESS;
	checkErr( Rs_LockSession (instrSession, VI_NULL));	
	
	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "",
  		RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_LENGTH, subframeLength), 2, "Subframe Length");  

	Error:
 (void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX TTC Start
/* Purpose:  This function sets the 'time to capture buffer' calculation to be 
/*           with respect to either FFT or frame.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTTCStart (ViSession instrSession,
                                                   ViInt32 start)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                               RSSPECAN_ATTR_WIMAX_TTC_START, 
                                               start), 2, "Start");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX ACP Measurements
/* Purpose:  This function returns the result values of the adjacent channel 
/*           power measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXACPMeasurement(ViSession instrSession,
                                                ViInt32     window,
                                                ViInt32     marker,
                                                ViInt32     mode,
                                                ViReal64    values[])
{
    ViStatus    error   = VI_SUCCESS; 
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     maxIdx = 12;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (marker, 1, 1))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");
    } 
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_ACP_CURR, RSSPECAN_VAL_ACP_MAX))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Mode");
    } 
    if (!values)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Values");    
    }
        
    checkErr (viQueryf (instrSession, ":CALC%ld:MARK%ld:FUNC:POW:RES:%s?\n","%s", window, marker, ACPMeas[mode], buffer));
                        
    pbuffer = strtok(buffer, ",");
    i=0;
    
    while ((pbuffer) && (i < maxIdx)) 
    {
        values[i++] = atof (pbuffer);
        pbuffer = strtok(NULL, ",");        
    }            
    
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Load WiMAX IQ Data
/* Purpose:  This function loads the IQ data from the specified .iqw file.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_LoadWiMAXIQData(ViSession    instrSession,
                                            ViString    fileName)
{
    ViStatus    error = VI_SUCCESS;
    if (!fileName)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "File Name");   
    }
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LOAD_IQ_DATA, fileName));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Store WiMAX IQ Data
/* Purpose:  This function stores the IQ data from/to the specified .iqw 
/*           file.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_StoreWiMAXIQData(ViSession   instrSession,
                                            ViString    fileName)
{
    ViStatus    error = VI_SUCCESS;
    if (!fileName)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "File Name");   
    }
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SAVE_IQ_DATA, fileName));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMAX Burst Recalc
/* Purpose:  This function forces the IQ measurement results to be 
/*           recalculated according to the current settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXBurstImm (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
        
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "C1", RSSPECAN_ATTR_WIMAX_CALC_BURS, ""));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMAX Refresh IQ Measurement
/* Purpose:  This function updates the current IQ measurement results to reflect 
/*           the current measurement settings. Note no new IQ data is captured.
/*           I.e. the measurement settings apply to the IQ data being currently 
/*           in the capture buffer. The command applies exclusively to IQ 
/*           measurements. It requires available IQ data.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXRefreshIQMeasurement (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
        
    viCheckErr( rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_WIMAX_REFRESH, ""));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: WiMAX Get Marker TTC
/* Purpose:  This function returns the time to the start of the first frame in 
/*           the capture buffer.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WiMAXGetMarkerTTC (ViSession instrSession,
                                              ViInt32 window,
                                              ViInt32 markerNumber,
                                              ViReal64 *time)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (markerNumber, 1, 1))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker Number");
    }    
    sprintf (buffer, "Win%ld,M%ld", window, markerNumber);  
    
    viCheckParm( rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_WIMAX_QUERY_TTC, time),
                4, "Time");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Read WiMAX Trace Data
/* Purpose:  This function returns all the measured data that relates to the 
/*           currently selected measurement type. The returned data is 
/*           particular to the currently selected measurement type and is 
/*           specified in the Trace Data parameter description.
/*           
/*           Attribute(s):
/*           no attribute
/*           
/*           Remote-control command(s):
/*           TRACE[:DATA]? TRACE1 | TRACE2 | TRACE3 | TRACE4 | TRACE5 | 
/*           TRACE6
/*           :FORM REAL,32
/*           *CLS
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadWiMAXTraceData(ViSession instrSession,
                                            ViInt32     sourceTrace,
                                            ViInt32     arraySize,
                                            ViReal64    traceData[],
                                            ViInt32*    noofValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))  
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    if (rsspecan_invalidViInt32Range (sourceTrace, 1, 6))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Source Trace");
    }
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    sprintf (buffer, "TRACE%ld", sourceTrace);
    viCheckErr (rsspecan_dataReadTrace (instrSession, 1, buffer, arraySize, 
                    traceData, noofValues));
    checkErr( rsspecan_CheckStatus (instrSession));           

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Read WiMAX Memory IQ Data
/* Purpose:  This function returns all the IQ data associated with the 
/*           measurement acquisition time. The result values are scaled 
/*           linear in unit Volt and correspond to the voltage at the RF 
/*           input of the instrument. The command returns a comma-separated 
/*           list of the measured voltage values in floating point format 
/*           (Comma Separated Values = CSV). The number of values returned is 
/*           2 * number of samples, the first half being the I values, the 
/*           second half the Q values.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadWiMAXMemoryIQData(ViSession  instrSession,
                                                ViInt32     offsetSamples,
                                                ViInt32     noofSamples,
                                                ViInt32     bufferSize,
                                                ViInt32*    noofPoints,
                                                ViReal64    realPartsI[],
                                                ViReal64    imaginaryPartsQ[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE]= "";
    ViInt32     num; 
    ViReal64    *data=VI_NULL;              
    ViInt32     data_num; 
    ViInt32     i;
    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))  
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    checkErr (viPrintf(instrSession, ":TRAC:IQ:DATA:MEM? %ld,%ld\n",
                offsetSamples, noofSamples));
    data_num = noofSamples * 2;
    data = malloc (sizeof (ViReal64) * data_num); 
    
	viCheckErr (rsspecan_dataReadTraceOnly (instrSession, data_num, data, &num));
    
    if (noofPoints) *noofPoints=num/2;

    num = (noofSamples>bufferSize)?bufferSize:noofSamples;

    for (i=0;i<num;i++)
    {
        realPartsI[i]=data[2*i];
        imaginaryPartsQ[i]=data[2*i+1];
    }

	checkErr( rsspecan_CheckStatus (instrSession));                      
Error:
    if (data) free (data); 
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Read WiMAX SEM Results
/* Purpose:  This function returns the Spectrum Emission Mask summary results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadWiMAXSEMResults (ViSession instrSession,
                                                ViInt32 noOfValues,
                                                ViReal64 index[],
                                                ViReal64 startFrequencyBand[],
                                                ViReal64 stopFrequencyBand[],
                                                ViReal64 resolutionBandwidth[],
                                                ViReal64 limitFailFrequency[],
                                                ViReal64 powerAbs[],
                                                ViReal64 powerRel[],
                                                ViReal64 limitDistance[],
                                                ViReal64 failureFlag[],
                                                ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *buffer = VI_NULL;
    ViChar      *pbuf=VI_NULL;
    ViInt32     count=0;
    ViUInt32    retCnt;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (index == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Index");
    if (startFrequencyBand == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Start Frequency Band"); 
    if (stopFrequencyBand == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Stop Frequency Band"); 
    if (resolutionBandwidth == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Resolution Bandwidth"); 
    if (limitFailFrequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Limit Fail Frequency"); 
    if (powerAbs == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Power Abs");
    if (powerRel == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Power Rel"); 
    if (limitDistance == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Limit Distance"); 
    if (failureFlag == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "Failure Flag");
    
    viCheckErr (viPrintf (instrSession, "TRAC:DATA? LIST\n"));
    viCheckErr( Rs_ReadDataUnknownLength(instrSession, &buffer, &retCnt));
    
    pbuf=strtok(buffer, ",");
    if (pbuf)
        do{
            if (count<noOfValues)
            {
                sscanf (pbuf, "%le", &index[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &startFrequencyBand[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &stopFrequencyBand[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &resolutionBandwidth[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &limitFailFrequency[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &powerAbs[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &powerRel[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &limitDistance[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &failureFlag[count]);
                pbuf=strtok(NULL, ",");
            }               
            else
            {
                for (i=0;i<11;i++)
                    pbuf=strtok(NULL, ","); 
            }       
            count++;
        }while (pbuf);  

    if (returnedValues)
        *returnedValues = count;
     
    checkErr( rsspecan_CheckStatus (instrSession));     
Error:
    if (buffer) free(buffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX Burst All
/* Purpose:  This function returns all the results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXBurstAll(ViSession instrSession,
                                            ViReal64    results[],
                                            ViInt32*    returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    checkErr (viQueryf (instrSession, "FETC:BURS:ALL?\n","%s", buffer));
                        
    pbuffer = strtok(buffer, ",");
    i=0;
    if (results)
    { 
        while (pbuffer) 
        {
            results[i++] = atof (pbuffer);
            pbuffer = strtok(NULL, ",");        
        }            
    }
    else
    {
        while (pbuffer)
        {
            i++;
            pbuffer = strtok(NULL, ",");
         }   
    }
    if (returnedValues) *returnedValues=i;
    
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX Burst Power
/* Purpose:  This function returns the power measurement value, measured 
/*           during the measurement.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXBurstPower(ViSession   instrSession,
                                                ViInt32 powerMeasurement,
                                                ViInt32     mode,
                                                ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");    
    }
    
    switch (powerMeasurement){
       case RSSPECAN_VAL_WIMAX_MEAS_RMS:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_RMS, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_MEAS_CREST:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_CRES, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WIMAX_MEAS_RMS_ALL:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_RMS_ALL, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WIMAX_MEAS_RMS_PILOT:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_RMS_PILOT, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WIMAX_MEAS_RMS_DATA:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_RMS_DATA, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WIMAX_MEAS_RMS_DLPR:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_RMS_DLPR, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WIMAX_MEAS_RMS_NPIL:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETCH_NPIL_RMS, 
                    value), 3, "Value");                    
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Power Measurement");
    }
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX Burst Error
/* Purpose:  This function returns the burst errors.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXBurstError(ViSession   instrSession,
                                                ViInt32     errorType,
                                                ViInt32     mode,
                                                ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (errorType, RSSPECAN_VAL_WIMAX_ERR_FREQ, RSSPECAN_VAL_WIMAX_ERR_SYMB))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Error Type");
    }
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
    }
   
    switch (errorType){
        case RSSPECAN_VAL_WIMAX_ERR_FREQ:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_FERR, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_ERR_SYMB:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_SYMB, 
                    value), 3, "Value");
        break;
    }    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX IQ Impairments
/* Purpose:  This function returns the burst errors.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXIQImp(ViSession    instrSession,
                                            ViInt32     iqImpairment,
                                            ViInt32     mode,
                                            ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (iqImpairment, RSSPECAN_VAL_WIMAX_IQ_OFFS, RSSPECAN_VAL_WIMAX_IQ_QUAD))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "IQ Impairment");
    }

    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
    }
    
    switch (iqImpairment){
        case RSSPECAN_VAL_WIMAX_IQ_OFFS:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_IQOF, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_IQ_IMBA:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_GIMB, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_IQ_QUAD:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_QUAD, 
                    value), 3, "Value");
        break;
     }   
 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX EVM
/* Purpose:  This function returns the EVM measurement value.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXEVM(ViSession  instrSession,
                                        ViInt32     evm,
                                        ViInt32     mode,
                                        ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (evm, RSSPECAN_VAL_WIMAX_EVM_ALL, RSSPECAN_VAL_WIMAX_EVM_PREAM))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "EVM");
    }
    
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
    }

    switch (evm){
        case RSSPECAN_VAL_WIMAX_EVM_ALL:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_EVM_ALL, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_EVM_DATA:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_EVM_DATA, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_EVM_PILOT:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_EVM_PILOT, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_EVM:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_EVM, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WIMAX_EVM_PREAM:
            viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_EVM_PREAMBLE,
                    value), 3, "Value");
        break;
     }   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX Subscriber Station Time
/* Purpose:  This function returns the WiMax specific timing value.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXSubscriberStationTime(ViSession    instrSession,
                                                            ViInt32     mode,
                                                            ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_MEASTYPE_MIN, RSSPECAN_VAL_MEASTYPE_MAX) &&
        (mode!=RSSPECAN_VAL_MEASTYPE_AVER))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");
    }
    
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
    }
    
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_SST, 
                            value), 3, "Value");
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX CINR Values
/* Purpose:  This function returns the Carrier Interference Noise Ratio 
/*           values.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXCINRValues(ViSession   instrSession,
                                                ViReal64    values[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    checkErr (viQueryf (instrSession, "FETC:BURS:CINR?\n","%s", buffer));
                        
    pbuffer = strtok(buffer, ",");
    i=0;
    if (values)
    { 
        while (pbuffer) 
        {
            values[i++] = atof (pbuffer);
            pbuffer = strtok(NULL, ",");        
        }            
    }
    else
    {
        while (pbuffer)
        {
            i++;
            pbuffer = strtok(NULL, ",");
         }   
    }
    checkErr( rsspecan_CheckStatus (instrSession));  

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX RSSI Values
/* Purpose:  This function returns the Received Signal Strength Indication 
/*           values.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXRSSIValues(ViSession   instrSession,
                                                ViReal64    values[])
{
     ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    checkErr (viQueryf (instrSession, "FETC:BURS:RSSI?\n","%s", buffer));
                        
    pbuffer = strtok(buffer, ",");
    i=0;
    if (values)
    { 
        while (pbuffer) 
        {
            values[i++] = atof (pbuffer);
            pbuffer = strtok(NULL, ",");        
        }            
    }
    else
    {
        while (pbuffer)
        {
            i++;
            pbuffer = strtok(NULL, ",");
         }   
    }
    checkErr( rsspecan_CheckStatus (instrSession));  

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX Count
/* Purpose:  This function returns the count of analyzed burst or number of 
/*           symbols of analyzed burst.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXCount( ViSession   instrSession,
                                            ViInt32 type,
                                            ViInt32*    count)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (type, RSSPECAN_VAL_WIMAX_BURS, RSSPECAN_VAL_WIMAX_STAT_ZONE))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");
    }
    
    switch (type){
        case RSSPECAN_VAL_WIMAX_BURS:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_FETC_BURS_COUNT, 
                            count), 3, "Count");    
        break;
        case RSSPECAN_VAL_WIMAX_SYMB:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_FETC_SYMB_COUNT, 
                            count), 3, "Count");    
        break;
        case RSSPECAN_VAL_WIMAX_ZONE:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_FETC_ZONE_COUNT, 
                            count), 3, "Count");    
        break;
        case RSSPECAN_VAL_WIMAX_STAT_ZONE:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WIMAX_FETC_ZONE_STAT_COUNT, 
                            count), 3, "Count"); 
        break;
   }     

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX List Result Units
/* Purpose:  This function specifies the units for table results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXTimeDomainPreamble (ViSession instrSession,
                                                         ViInt32 result,
                                                         ViReal64 *minimum,
                                                         ViReal64 *average,
                                                         ViReal64 *maximum)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    tmp_min = 0,
                tmp_max = 0,
                tmp_aver = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    switch (result){
        case RSSPECAN_VAL_WIMAX_PREA:
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_WIMAX_FETC_TDOM_PREAMBLE, &tmp_min));
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_WIMAX_FETC_TDOM_PREAMBLE, &tmp_max));
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Aver", RSSPECAN_ATTR_WIMAX_FETC_TDOM_PREAMBLE, &tmp_aver));
        break;
        case RSSPECAN_VAL_WIMAX_SUBF:
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_WIMAX_FETC_TDOM_SUBFRAME, &tmp_min));
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_WIMAX_FETC_TDOM_SUBFRAME, &tmp_max));
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Aver", RSSPECAN_ATTR_WIMAX_FETC_TDOM_SUBFRAME, &tmp_aver));
        break;
        case RSSPECAN_VAL_WIMAX_ZONE:
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_WIMAX_FETC_TDOM_ZONE, &tmp_min));
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_WIMAX_FETC_TDOM_ZONE, &tmp_max));
            viCheckErr(rsspecan_GetAttributeViReal64(instrSession, "Aver", RSSPECAN_ATTR_WIMAX_FETC_TDOM_ZONE, &tmp_aver));
        break;
        default:
              viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result");  
    }
    if (minimum)
       *minimum = tmp_min;  
    if (average)
        *average = tmp_aver;
    if (maximum)
        *maximum = tmp_max;
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WiMAX Unmodulated Subcarrier Error
/* Purpose:  This function returns the Unmodulate Subcarrier Error for 
/*           the analysed zone.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWiMAXUnmodulatedSubcarrierError (ViSession instrSession,
                                                                 ViInt32 mode,
                                                                 ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_MEASTYPE_MIN, RSSPECAN_VAL_MEASTYPE_MAX) &&
        (mode!=RSSPECAN_VAL_MEASTYPE_AVER))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");
    }
    
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
    }
    
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FETC_USERR, 
                            value), 3, "Value");
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WiMAX List Result Units
/* Purpose:  This function specifies the units for table results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXListResultUnit(ViSession   instrSession,
                                                        ViInt32     unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_WIMAX_TABLE_UNIT, unit), 2, "Unit");     
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*===========================================================================*/
/* Function: Configure WiMAX RSSI CINR Averaging
/* Purpose:  This function specifies the RSSI, CINR averaging parameter.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXRSSICINRAvg (ViSession instrSession,
                                                      ViReal64 averagingParameter)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_WIMAX_RSSI_AVG_PARAM, averagingParameter),
    2, "Averaging Parameter");     
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX Limit Check Result
/* Purpose:  This function returns the limit results
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXLimitCheckResult(ViSession   instrSession,
                                                ViInt32     window,
                                                ViInt32     limit,
                                                ViInt32     limitType,
                                                ViInt32     mode,
                                                ViInt32*    result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";   
    ViInt32     lim_type = RSSPECAN_VAL_WIMAX_LIM_IQOFF;
        
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    if ((strstr(model, "FSL")!=NULL)&&(strstr(buffer,"K93")!=NULL))
    {
        lim_type = RSSPECAN_VAL_WIMAX_LIM_BERP; 
    }
    
    
    if (rsspecan_invalidViInt32Range (limit, 1, 10))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if ((strstr(model, "FSL")!=NULL)&&(strstr(buffer,"K93")!=NULL)) 
    {
        if (rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WIMAX_LIM_FERR, RSSPECAN_VAL_WIMAX_LIM_USERR))
        {
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Type");
        } 
    }
    else
    {
        if ((rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WIMAX_LIM_FERR, RSSPECAN_VAL_WIMAX_LIM_IQOFF)&&
            rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WIMAX_LIM_USERR, RSSPECAN_VAL_WIMAX_LIM_USERR)))
        {
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Type");
        } 
    }   
    sprintf (buffer, "C%ld,L%ld", window, limit);  
    switch (mode){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            strcat (buffer, ",Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            strcat (buffer, ",Aver");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Mode");
    }
    
    switch (limitType){
        case RSSPECAN_VAL_WIMAX_LIM_FERR:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_FERR_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_SYMB:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_SYMB_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_SST:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_SST_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_EVM_ALL:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_EVM_ALL_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_EVM_DATA:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_EVM_DATA_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_IQOFF:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_IQOF_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_BERP:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_BERP_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WIMAX_LIM_USERR:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_USERR_RES, 
            result), 6, "Result");
        break;
        
    }    
  

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX All Limit Check Results
/* Purpose:  This function returns all limit results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXAllLimitCheckResults(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     limit,
                                                    ViInt32     results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViInt32     maxIdx = 12;
    ViChar      *pbuffer;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K92")||strstr (buffer, "K93")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    
    checkErr (viQueryf (instrSession, "CALC%ld:LIM%ld:BURS:ALL:RESULT?\n","%s", window, limit, buffer));
    
    i=0;                    
    pbuffer = strtok(buffer, ",");
    while ((pbuffer) && (i < maxIdx))   
    {
        results[i++] = atol (pbuffer);
        pbuffer = strtok(NULL, ",");    
    }   
    
    
    checkErr( rsspecan_CheckStatus (instrSession));  

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WiMAX Limit Check
/* Purpose:  This function returns the result of the limit check of the 
/*           limit line indicated in the selected measurement window.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWiMAXLimitCheck(ViSession instrSession,
                                                    ViInt32     window,
                                                    ViInt32     limit,
                                                    ViInt32*    result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    
    sprintf (buffer, "C%ld,L%ld", window, limit);
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WIMAX_LIM_FAIL_RES, 
            result), 4, "Result");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

