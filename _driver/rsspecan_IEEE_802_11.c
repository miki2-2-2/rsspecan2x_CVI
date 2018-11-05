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
static ViString powerMeasurementArr[] = {"ACP", "CPOW", "MCAC", "OBAN", "CN", "CN0", "PPOW", VI_NULL};
static ViString itemArr[] = {"TALL", "IOFF", "GIMB", "QOFF", "TPP", "TCF", "RALL", "RPP", 
                             "RCF", "SALL", "BPIL", "SEAC", "SEDC", "SEPC", "PBER", "EAC", 
                             "EDC", "EPC", "CFER", "SCER","PVER","PEVM","GIOffset","GGIM","QERR",
							 "GCF","CCER","RTIM","FTIM","MPOW","PPOW","GCF", VI_NULL};
static ViString offOnArr[] = {"OFF", "ON", VI_NULL};

/*===========================================================================*/
/* Function: Set WLAN Mode
/* Purpose:  This function selects the WLAN analyzer mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_WlanSetWlanMode (ViSession  instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr (Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WLAN_MODE, ""));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the expected input signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR standard/Sets the standard.
/// HIPAR centerFrequency/Specifies the center frequency of the signal to be measured.
ViStatus _VI_FUNC rsspecan_ConfigureWLANSignal (ViSession instrSession,
                                                ViInt32 standard,
                                                ViReal64 centerFrequency)
{
	ViStatus    error = VI_SUCCESS;
    
	checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm(rsspecan_SetAttributeViInt32 (instrSession, "",
        		RSSPECAN_ATTR_WLAN_STAN, standard), 2, "standard");
	
	viCheckParm(rsspecan_SetAttributeViReal64 (instrSession, "Win0",
        		RSSPECAN_ATTR_FREQUENCY_CENTER, centerFrequency), 2, "Center Frequency");
	
Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures radio frequency input.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR inputCoupling/The RF input of the R&S FSW can be coupled by alternating current (AC) 
/// HIPAR inputCoupling/or direct current (DC).
/// HIPAR impedance/This control selects the nominal input impedance.
/// HIPAR highpassFilter/Activates an additional high pass filter for RF input signals from 
/// HIPAR highpassFilter/1...3 GHz.
/// HIPAR YIGPreselector/This control turns the YIG-preselector on and off.
ViStatus _VI_FUNC rsspecan_ConfigureWLANInputSource (ViSession instrSession,
                                                     ViInt32 inputCoupling,
                                                     ViInt32 impedance,
                                                     ViBoolean highpassFilter,
                                                     ViBoolean YIGPreselector)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_AMPL_RF_COUPLING, 
												inputCoupling), 2, "Input Coupling");
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Win0", RSSPECAN_ATTR_INPUT_IMPEDANCE,
												impedance), 3, "Impedance");
	
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER,
												highpassFilter), 4, "Highpass Filter");
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0", RSSPECAN_ATTR_AMPL_YIG_FILTER,
												YIGPreselector), 5, "YIG Preselector");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the reference level and offset.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR autoLevel/Switches on or off automatic power level detection.
/// HIPAR referenceLevel/The calibrated vertical position of the captured data used as a 
/// HIPAR referenceLevel/reference for amplitude measurements. This is typically set to a value 
/// HIPAR referenceLevel/slightly higher than the highest expected signal level. 
/// HIPAR referenceLevelOffset/Specifies the offset value to the reference level. The driver uses 
/// HIPAR referenceLevelOffset/this value to set the RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET attribute.
ViStatus _VI_FUNC rsspecan_ConfigureWLANReferenceLevel (ViSession instrSession,
                                                       ViBoolean autoLevel,
                                                       ViReal64 referenceLevel,
                                                       ViReal64 referenceLevelOffset)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	sprintf (rep_cap, "Win0");
	
	viCheckParm (rsspecan_SetAttributeViBoolean(instrSession, "", 
                                                RSSPECAN_ATTR_WLAN_POW_AUTO, 
                                                autoLevel), 2, "Auto Level");
	
	if (autoLevel == VI_FALSE)
	{
	    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
	                                                RSSPECAN_ATTR_REFERENCE_LEVEL,
													referenceLevel), 3, "Rreference Level");
	
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
	                                                RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET,
													referenceLevelOffset), 4, "Reference Level Offset");
	}

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines the mechanical attenuation for RF input.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR autoMode/This control couples or decouples the attenuation to the reference 
/// HIPAR autoMode/level.
/// HIPAR value/This control defines the attenuation.
ViStatus _VI_FUNC rsspecan_ConfigureWLANMechanicalAttenuation (ViSession instrSession,
                                                             ViBoolean autoMode,
                                                             ViReal64 value)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0", RSSPECAN_ATTR_ATTENUATION_AUTO, 
												autoMode), 2, "Auto Mode");
	
    if (!autoMode)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win0", RSSPECAN_ATTR_ATTENUATION,
												    value), 3, "Value"); 
	}
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Electronic Attenuator
/* Purpose:  This function configures the signal characteristics.
/*           
/*           Note:
/*           (1) This function is available only with option B25.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanElectronicAtt(ViSession   instrSession,
                                            ViBoolean   state,
                                            ViBoolean   attenuatorAuto,
                                            ViInt32    manualValue_dB)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "Win1", RSSPECAN_ATTR_AMPL_EATT_STATE, state), 2, "State");
    if (state)
        viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "Win1", RSSPECAN_ATTR_AMPL_EATT_AUTO, attenuatorAuto), 3, "Attenuator Auto");
    if (!attenuatorAuto)
        viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_AMPL_EATT_VALUE, manualValue_dB), 4, "Manual Value [dB]");
        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the preamplifier for the instrument.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR state/Switches on or off the preamplifier for the instrument. 
/// HIPAR preamplifierLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureWLANPreamplifier (ViSession instrSession,
                                                     ViBoolean state,
                                                     ViInt32 preamplifierLevel)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0",
                                                RSSPECAN_ATTR_AMPL_PREAMPLIFIER,
												state), 2, "State");
	
	if (state) 
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_AMPL_PREAMPLIFIER_LEVEL,
												preamplifierLevel), 3, "Preamplifier Level");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures how much and which data is to be captured 
/// HIFN  during the default WLAN measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR sampleRate/Sets the sampling rate for the I/Q data acquisition.
/// HIPAR captureTime/Specifies the duration (and therefore the amount of data) to be 
/// HIPAR captureTime/captured in the capture buffer. If the capture time is too short, 
/// HIPAR captureTime/demodulation will fail.
/// HIPAR swapIQ/Defines whether or not the recorded IQ pairs should be swapped (I/Q) 
/// HIPAR swapIQ/before being processed. 
/// HIPAR filterOutAdjacentChannels/If activated (default), only the useful signal is analyzed, all signal 
/// HIPAR filterOutAdjacentChannels/data in adjacent channels is removed by the filter.
ViStatus _VI_FUNC rsspecan_ConfigureWLANSignalCapture (ViSession instrSession,
                                                       ViReal64 sampleRate,
                                                       ViReal64 captureTime,
                                                       ViBoolean swapIQ,
                                                       ViBoolean filterOutAdjacentChannels)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_WLAN_SRAT, 
												sampleRate), 2, "Sample Rate");
	
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME,
												captureTime), 3, "Capture Time");
	
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_SWAP_IQ,
												swapIQ), 4, "Swap IQ");
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_RBW_FILTER,
												filterOutAdjacentChannels), 5, "filter Out Adjacent Channels");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Synchronization settings have an effect on which parts of the input 
/// HIFN  signal are processed during the WLAN measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR powerIntervalSearch/If enabled, the R&S FSW WLAN application initially performs a coarse 
/// HIPAR powerIntervalSearch/burst search on the input signal in which increases in the power vs 
/// HIPAR powerIntervalSearch/time trace are detected. Further time consuming processing is then only 
/// HIPAR powerIntervalSearch/performed where bursts are assumed. This improves the measurement speed 
/// HIPAR powerIntervalSearch/for signals with low duty cycle rates.
/// HIPAR FFTStartOffset/This control specifies the FFT start offset.
ViStatus _VI_FUNC rsspecan_ConfigureWLANSynchronization (ViSession instrSession,
                                                         ViBoolean powerIntervalSearch,
                                                         ViInt32 FFTStartOffset)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_POWER_INTERVAL_SEARCH, 
												powerIntervalSearch), 2, "Power Interval Search");
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_FFT_START_OFFSET,
												FFTStartOffset), 3, "FFT Start Offset");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines whether channel estimation will be done in 
/// HIFN  preamble and payload or only in preamble. The effect of this is most 
/// HIFN  noticeable for the EVM measurement results, where the results will be 
/// HIFN  improved when this feature is enabled.
/// HIFN  However, this functionality is not supported by the IEEE 802.11 
/// HIFN  standard and must be disabled if the results are to be measured 
/// HIFN  strictly according to the standard.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR channelEstimationRange/Specifies the signal range used to estimate the channels.
ViStatus _VI_FUNC rsspecan_ConfigureWLANChannelEstimation (ViSession instrSession,
                                                           ViBoolean channelEstimationRange)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_CEST, 
												channelEstimationRange), 2, "Channel Estimation Range"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure WLAN Tracking
/* Purpose:  This function configures the tracking parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanTracking(ViSession  instrSession,
                                                ViBoolean   phase,
                                                ViBoolean   timing,
                                                ViBoolean   level)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_TRAC_PHAS, phase), 2, "Phase");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_TRAC_TIME, timing), 3, "Time");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_TRAC_LEV, level), 4, "Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures how the pilots of the signal is determined.
/// HIRET Returns the status code of this operation.
/// HIRET You may use multiple lines for documentation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR pilots/This control configures how the pilots of the signal is determined.
ViStatus _VI_FUNC rsspecan_ConfigureWLANTrackingPilots (ViSession instrSession,
                                                        ViInt32 pilots)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
								RSSPECAN_ATTR_WLAN_TRAC_PILOTS, pilots), 2, "Pilots");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the Burst.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR burst/This control selects the PPDU for which the trace data is queried 
/// HIPAR burst/(using TRACe<n>[:DATA]) for the "EVM vs Symbol" and "EVM vs Carrier" 
/// HIPAR burst/result displays if [SENSe:]BURSt:SELect:STATe is ON.
/// HIPAR state/Determines whether a selected PPDU (using [SENSe:]BURSt:SELect) is 
/// HIPAR state/considered or ignored.
ViStatus _VI_FUNC rsspecan_ConfigureWlanBurst(
	ViSession	instrSession,
	ViInt32	burst,
	ViBoolean	state
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_WLAN_BURST_SELECT,
                                               burst), 2, "Burst");
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_WLAN_BURST_STATE,
                                                 state), 3, "State");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function specifies the units for PPDU length results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR unit/Specifies the units for PPDU length results.
ViStatus _VI_FUNC rsspecan_ConfigureWlanBurstUnit(
	ViSession	instrSession,
	ViInt32	unit
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_WLAN_BURST_UNIT,
                                               unit), 2, "Unit");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function activates or deactivates the compensation for I/Q 
/// HIFN  mismatch (gain imbalance, quadrature offset, I/Q skew, see chapter 
/// HIFN  3.1.1.5, "I/Q Mismatch", on page 19).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR iqCompensation/Activates or deactivates the compensation for I/Q mismatch.
ViStatus _VI_FUNC rsspecan_ConfigureWlanIQCompensation(
	ViSession	instrSession,
	ViBoolean	iqCompensation
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_WLAN_IQ_COMPENSATION,
                                                 iqCompensation), 2, "IQ Compensation");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function configures the demodulation parameters for IEEE 802.11a.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR PPDUAnalysisMode/If this parameter sets the burst to analyze auto demodulation state.
/// HIPAR PPDUFormatToMeasure/This control specifies whether how signals are analyzed.
/// HIPAR channelBandwidthToMeasure/This control is used to specify which bursts are analyzed according to 
/// HIPAR channelBandwidthToMeasure/STBC streams.
/// HIPAR PSDUModulationToUse/This control specifies whether how signals are analyzed.
/// HIPAR PSDUModulation/This control specifies which PSDUs are to be analyzed depending on 
/// HIPAR PSDUModulation/their modulation. Only PSDUs using the selected modulation are 
/// HIPAR PSDUModulation/considered in result analysis.
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11a (ViSession instrSession,
                                                             ViBoolean PPDUAnalysisMode,
                                                             ViInt32 PPDUFormatToMeasure,
                                                             ViInt32 channelBandwidthToMeasure,
                                                             ViInt32 PSDUModulationToUse,
                                                             ViInt32 PSDUModulation)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_BCON_AUTO, 
												PPDUAnalysisMode), 2, "PPDU Analysis Mode");
	
    if (PPDUAnalysisMode == VI_FALSE)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE,
													PPDUFormatToMeasure), 3, "PPDU Format To Measure");
	
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEASURE,
													channelBandwidthToMeasure), 4, "Channel Bandwidth To Measure");
	
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX_TO_USE_FBUR, 
												PSDUModulationToUse), 5, "PSDU Modulation To Use");
		
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_PSDU_MODULATION,
													PSDUModulation), 6, "PSDU Modulation");
	}
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the demodulation parameters for IEEE 802.11n.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR PPDUAnalysisMode/If this parameter sets the burst to analyze auto demodulation state.
/// HIPAR PPDUFormatToMeasure/This control specifies whether how signals are analyzed.
/// HIPAR channelBandwidthToMeasure/This control is used to specify which bursts are analyzed according to 
/// HIPAR channelBandwidthToMeasure/STBC streams.
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11n (ViSession instrSession,
                                                             ViBoolean PPDUAnalysisMode,
                                                             ViInt32 PPDUFormatToMeasure,
                                                             ViInt32 channelBandwidthToMeasure)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_BCON_AUTO, 
												PPDUAnalysisMode), 2, "PPDU Analysis Mode");
	
    if (PPDUAnalysisMode == VI_FALSE)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE_802_11N,
													PPDUFormatToMeasure), 3, "PPDU Format To Measure");

		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEASURE_802_11N,
													channelBandwidthToMeasure), 4, "Channel Bandwidth To Measure");  
	}	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the Modulation and Coding
/// HIFN     Scheme (MCS) index parameters for IEEE 802.11n demodulation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR MCSIndexMode/This control is used to specify how bursts are analyzed according to 
/// HIPAR MCSIndexMode/the MCS index.
/// HIPAR MCSIndex/This Control allows the user to define the Modulation and
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nMCSIndex (ViSession instrSession,
                                                                     ViInt32 MCSIndexMode,
                                                                     ViInt32 MCSIndex)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX_TO_USE_FBUR, 
												MCSIndexMode), 2, "MCS Index Mode");
	
    if (MCSIndexMode == RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_MEAS || MCSIndexMode == RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_DEMOD)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX,
												MCSIndex), 3, "MCS Index");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures which PPDUs are analyzed according to STBC 
/// HIFN  streams.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR STBCField/This control is used to specify which PPDUs are analyzed according to 
/// HIPAR STBCField/STBC streams.
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nSTBCField (ViSession instrSession,
                                                                      ViInt32 STBCField)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_STBC, 
												STBCField), 2, "STBC Field"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the PPDUs taking part in the analysis 
/// HIFN  depending on the guard interval length.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR guardIntervalLength/This control is used to specify the guard interval length of the
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nGuardIntervalLength (ViSession instrSession,
                                                                              ViInt32 guardIntervalLength)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_GUARD_INTERVAL_LENGTH, 
												guardIntervalLength), 2, "Guard Interval Length"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures which PPDUs are analyzed according to Ness 
/// HIFN  (Extension Spatial Streams) field content.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR extensionSpatialStreams/This control is used to specify the PPDUs taking part in the analysis 
/// HIPAR extensionSpatialStreams/according to the Ness field content.
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nExtensionSpatialStreams (ViSession instrSession,
                                                                                    ViInt32 extensionSpatialStreams)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_EXT_SPATIAL_STREAMS, 
												extensionSpatialStreams), 2, "Extension Spatial Streams");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the demodulation parameters for IEEE
/// HIFN  802.11b, g (DSSS).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR PPDUFormatPSDUModulation/Defines which PPDU formats/modulations are to be included in the
/// HIPAR PPDUFormatPSDUModulation/analysis.
/// HIPAR PPDUFormat/This control specifies how signals are analyzed.
/// HIPAR PSDUModulation/This control specifies which PSDUs are to be analyzed depending
/// HIPAR PSDUModulation/on their modulation.
#pragma warning( push )
#pragma warning( disable : 4100)
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11b_g_DSSS (ViSession instrSession,
                                                                    ViInt32 PPDUFormatPSDUModulation,
                                                                    ViInt32 PPDUFormat,
                                                                    ViInt32 PSDUModulation)
{
	ViStatus	error = VI_SUCCESS;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE_802_11B_G, 
												PPDUFormatPSDUModulation), 2, "PPDU Format/PSDU Modulation");
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_PSDU_MODULATION_802_11B_G, 
												PSDUModulation), 4, "PSDU Modulation");

	Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
#pragma warning( pop )

/// HIFN  This function configures the demodulation parameters for IEEE 
/// HIFN  802.11ac.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR PPDUAnalysisMode/If this parameter sets the burst to analyze auto demodulation state.
/// HIPAR PPDUFormatToMeasure/This control specifies whether how signals are analyzed.
/// HIPAR channelBandwidthToMeasure/This control is used to specify which bursts are analyzed according to 
/// HIPAR channelBandwidthToMeasure/STBC streams.
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11ac (ViSession instrSession,
                                                              ViBoolean PPDUAnalysisMode,
                                                              ViInt32 PPDUFormatToMeasure,
                                                              ViInt32 channelBandwidthToMeasure)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_BCON_AUTO, 
												PPDUAnalysisMode), 2, "PPDU Analysis Mode");
	
    if (PPDUAnalysisMode == VI_FALSE)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE_802_11AC,
													PPDUFormatToMeasure), 3, "PPDU Format To Measure");
	
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEASURE_802_11AC,
													channelBandwidthToMeasure), 4, "Channel Bandwidth To Measure");  
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the Modulation and Coding
/// HIFN     Scheme (MCS) index parameters for IEEE 802.11ac demodulation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR MCSIndexMode/This control is used to specify how bursts are analyzed according to 
/// HIPAR MCSIndexMode/the MCS index.
/// HIPAR MCSIndex/This Control allows the user to define the Modulation and
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acMCSIndex (ViSession instrSession,
                                                                      ViInt32 MCSIndexMode,
                                                                      ViInt32 MCSIndex)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX_TO_USE_FBUR, 
												MCSIndexMode), 2, "MCS Index Mode");
	
    if (MCSIndexMode == RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_MEAS || MCSIndexMode == RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_DEMOD)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_MCS_INDEX,
												MCSIndex), 3, "MCS Index");
	}
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures which PPDUs are analyzed according to STBC 
/// HIFN  streams.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR STBCField/This control is used to specify which PPDUs are analyzed according to 
/// HIPAR STBCField/STBC streams.
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acSTBCField (ViSession instrSession,
                                                                       ViInt32 STBCField)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_STBC, 
												STBCField), 2, "STBC Field");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the PPDUs taking part in the analysis 
/// HIFN  depending on the guard interval length.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR guardIntervalLength/This control is used to specify the guard interval length of the
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acGuardIntervalLength (ViSession instrSession,
                                                                               ViInt32 guardIntervalLength)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_GUARD_INTERVAL_LENGTH, 
												guardIntervalLength), 2, "Guard Interval Length");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the Nsts parameters for IEEE 802.11ac 
/// HIFN  demodulation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR nstsIndexMode/This control is used to specify how bursts are analyzed according to 
/// HIPAR nstsIndexMode/the Nsts index.
/// HIPAR nstsIndex/This control allows the user to define the Nsts index, of the bursts 
/// HIPAR nstsIndex/taking part in the analysis, manually. 
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acNsts (ViSession instrSession,
                                                                  ViInt32 nstsIndexMode,
                                                                  ViInt32 nstsIndex)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_NSTS_INDEX_MODE, 
												nstsIndexMode), 2, "NSTS Index Mode"); 
	
	if (nstsIndexMode == RSSPECAN_VAL_WLAN_NSTS_INDEX_MEAS || nstsIndexMode == RSSPECAN_VAL_WLAN_NSTS_INDEX_DEMOD)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_NSTS_INDEX, 
												nstsIndex), 3, "NSTS Index");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the evaluation range statistics.
/// HIFN     
/// HIFN     If  the statistic count is enabled, the specified number of PPDUs is 
/// HIFN  taken into consideration for the statistical evaluation (maximally the 
/// HIFN  number of PPDUs detected in the current capture buffer).
/// HIFN     
/// HIFN     If disabled, all valid PPDUs in the current capture buffer are 
/// HIFN  considered. Note that in this case, the number of PPDUs contributing to 
/// HIFN  the current results may vary extremely.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR PPDUStatisticCount/Specifies whether a specified number of bursts is to be captured and 
/// HIPAR PPDUStatisticCount/analyzed.
/// HIPAR noOfPPDUsToAnalyze/Sets the number of PPDUs for the statistical evaluation.
ViStatus _VI_FUNC rsspecan_ConfigureWLANEvaluationRangeStatistics (ViSession instrSession,
                                                                   ViBoolean PPDUStatisticCount,
                                                                   ViInt32 noOfPPDUsToAnalyze)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_WLAN_BURS_COUN_STAT, 
												PPDUStatisticCount), 2, "PPDU Statistic Count"); 
	if (PPDUStatisticCount)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_BURS_COUN, 
												noOfPPDUsToAnalyze), 3, "No Of PPDUs To Analyze");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function allows the user to specify the PPDUs to be analyzed. It 
/// HIFN  provides also settings to adapt the synchronisation onto the channel 
/// HIFN  conditions.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR sourceOfPayloadLength/This control is used to specify which signal source is used to 
/// HIPAR sourceOfPayloadLength/determine the payload length of a PPDU.
/// HIPAR equalPPDULength/Enables/disables equal burst length to analyze.
/// HIPAR minNumberOfDataSymbols/Specifies the number of data symbols required for PPDUs to qualify for 
/// HIPAR minNumberOfDataSymbols/measurement analysis.
/// HIPAR maxNumberOfDataSymbols/Specifies the maximum number of data symbols required for PPDUs to 
/// HIPAR maxNumberOfDataSymbols/qualify for measurement analysis.
ViStatus _VI_FUNC rsspecan_ConfigureWLANEvaluationRangeTimeDomain (ViSession instrSession,
                                                                   ViInt32 sourceOfPayloadLength,
                                                                   ViBoolean equalPPDULength,
                                                                   ViInt32 minNumberOfDataSymbols,
                                                                   ViInt32 maxNumberOfDataSymbols)
{
	ViStatus    error = VI_SUCCESS;
	ViInt32     standard = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckErr (rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_STAN, &standard));
	
	if (standard != 0)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_SOURCE_OF_PAYLOAD_LENGTH, 
													sourceOfPayloadLength), 2, "source Of Payload Length"); 
	}
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_SYM_EQU, 
												equalPPDULength), 3, "equal PPDU Length");
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Min", RSSPECAN_ATTR_WLAN_SYM, 
												minNumberOfDataSymbols), 4, "Min Number Of Data Symbols"); 
	
	if (equalPPDULength == VI_FALSE)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Max", RSSPECAN_ATTR_WLAN_SYM, 
													maxNumberOfDataSymbols), 5, "Max Number Of Data Symbols");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Gain Imbalance Unit
/* Purpose:  This function specifies the units for gain imbalance results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanGainImbalanceUnit(ViSession instrSession,
                                                 ViInt32 unit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_WLAN_UNIT_GIMB, unit), 2, "Unit");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function specifies the units for EVM limits and results.    
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR unit/This parameter specifies the units for EVM limits and results. 
ViStatus _VI_FUNC rsspecan_ConfigureWlanEVMUnit (ViSession instrSession,
                                                 ViInt32 unit)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_UNIT_EVM, 
												unit), 2, "Unit");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Display
/* Purpose:  This function configures the display parameters. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanDisplay(ViSession   instrSession,
                                                ViBoolean   displayTable)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "Win1", RSSPECAN_ATTR_WLAN_DISP_TABL, displayTable), 2, "Display Table");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines which items are displayedin the Result Summary. 
/// HIFN  Note that the results are always calculated, regardless of their 
/// HIFN  visibility in the Result Summary.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR item/Selects the item to be included in Result Summary.
/// HIPAR displayItem/This parameter selects whether the results summary item is displayed. 
ViStatus _VI_FUNC rsspecan_ConfigureWlanResultSummaryDisplay(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	item,
	ViBoolean	displayItem
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar      buffer [1024] = "";
	ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "FSW")))
        viCheckErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (rsspecan_invalidViInt32Range (window, 0, 16) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");  
    } 
	if (rsspecan_invalidViInt32Range (item, RSSPECAN_VAL_WLAN_ITEM_TX_ALL, RSSPECAN_VAL_WLAN_ITEM_CREST_FACTOR_DSSS) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Item");  
    } 

	if (window == 0)
		sprintf (cmd, "DISP:WIND:TABL:ITEM %s,%s\n", itemArr[item], offOnArr[displayItem]);
	else
		sprintf (cmd, "DISP:WIND%ld:TABL:ITEM %s,%s\n", window, itemArr[item], offOnArr[displayItem]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN Limit
/* Purpose:  This function configures the limit.
/*      
/*           Note:
/*           (1) This function is not valid for standard IEEE 802.11b 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWlanLimit(ViSession     instrSession,
                                                ViInt32     window,
                                                ViInt32     limit,
                                                ViInt32     limitType,
                                                ViInt32     mode,
                                                ViReal64    value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WLAN_LIM_FERR, RSSPECAN_VAL_WLAN_LIM_EVM_PIL))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Type");
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
        case RSSPECAN_VAL_WLAN_LIM_FERR:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FERR, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_SYMB:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_SYMB, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_IQOFF:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_IQOF, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM_ALL:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_EVM_ALL, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM_DATA:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_EVM_DATA, 
            value), 6, "Value");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM_PIL:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_EVM_PIL, 
            value), 6, "Value");
        break;
    }    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure WLAN All Limits
/* Purpose:  This function sets all limits.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureWLANAllLimits (ViSession instrSession,
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

    if (!strstr (buffer, "K91"))
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
    
    pbuffer += sprintf (pbuffer, "CALC%ld:LIMit%ld:BURSt:ALL ", window, limit);
    
    for (i=0;i<arraySize;i++)
         pbuffer += sprintf (pbuffer,"%.12f,", values[i]);
         
    *--pbuffer = '\0';
    
    viCheckErr (viPrintf (instrSession, "%s\n", buffer));
    
    
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN All Limits
/* Purpose:  This function returns all limits.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanAllLimits(ViSession   instrSession,
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

    if (!(strstr (buffer, "K90")||strstr (buffer, "K91")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (!values)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Values");    
    }
        
    viCheckErr (viQueryf (instrSession, ":CALC%ld:LIM%ld:BURS:ALL?\n","%s", window, limit, buffer));
                        
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

/// HIFN  This function selects a common RF measurement for WLAN signals.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR RFMeasurement/Selects the RF measurement.
ViStatus _VI_FUNC rsspecan_SelectWlanRFMeasurement (ViSession instrSession,
                                                    ViInt32 RFMeasurement)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	switch (RFMeasurement)
	{
		case RSSPECAN_VAL_WLAN_MEAS_ACPR:
			viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_ACPR, 
												VI_NULL));
		break;
		case RSSPECAN_VAL_WLAN_MEAS_MASK:
			viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_MASK, 
												VI_NULL));
		break;
		case RSSPECAN_VAL_WLAN_MEAS_OBAN:
			viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WLAN_SPEC_OCCUPIED_BANDWIDTH, 
												VI_NULL));
		break;
		case RSSPECAN_VAL_WLAN_MEAS_CCDF:
			viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_WLAN_STAT_CCDF, 
												VI_NULL));
		break;
		default:
			viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "RF Measurement"); 
		break;
    }
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Read WLAN Trace Data
/* Purpose:  This function returns all the measured data that relates to the 
/*           currently selected measurement type. The returned data is 
/*           particular to the currently selected measurement type and is 
/*           specified in the Trace Data parameter description.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadWlanTraceData(ViSession   instrSession,
                                              ViInt32   sourceTrace,
                                              ViInt32   arraySize,
                                              ViReal64 _VI_FAR traceData[],
                                              ViPInt32  noofValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer=VI_NULL;
        
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K90")||strstr (buffer, "K91")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    if (rsspecan_invalidViInt32Range (sourceTrace, 1, 6))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Source Trace");
    }
    sprintf (buffer, "TRACE%ld", sourceTrace);
    
    viCheckErr (rsspecan_dataReadTrace (instrSession,1, buffer, arraySize, 
                    traceData, noofValues));
      
    checkErr( rsspecan_CheckStatus (instrSession));           
    
Error:
    if (pbuffer) free(pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    
    return error;
}

/*===========================================================================*/
/* Function: Read WLAN Memory IQ Data
/* Purpose:  This function returns all the IQ data associated with the 
/*           measurement acquisition time. The result values are scaled 
/*           linear in unit Volt and correspond to the voltage at the RF 
/*           input of the instrument. The command returns a comma-separated 
/*           list of the measured voltage values in floating point format 
/*           (Comma Separated Values = CSV). The number of values returned is 
/*           2 * number of samples, the first half being the I values, the 
/*           second half the Q values.
/*           
/*           Notes:
/*           (1) If there are not I/Q data available in memory because the 
/*           corresponding measurement had not been started, the function 
/*           will cause a Query Error.
/*           
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadWlanMemoryIQData(ViSession   instrSession,
                                                ViInt32     offsetSamples,
                                                ViInt32     noofSamples,
                                                ViInt32     bufferSize,
                                                ViInt32*    noofPoints,
                                                ViReal64    realPartsI[],
                                                ViReal64    imaginaryPartsQ[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     num;
    ViReal64    *data=VI_NULL;
    ViInt32     data_num;
    ViInt32     i;
        
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K90")||strstr (buffer, "K91")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    checkErr (viPrintf(instrSession, ":FORM REAL,32;*CLS;:TRAC:IQ:DATA:MEM? %ld,%ld\n",
                offsetSamples, noofSamples));
    data_num = noofSamples * 2;
    data = malloc (sizeof (ViReal64) * data_num); 
    /*viCheckErr (rsspecan_dataReadTraceIQ (instrSession, bufferSize, noofPoints, 
                    realPartsI, imaginaryPartsQ));*/
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
/* Function: Read WLAN SEM Results
/* Purpose:  This function returns the Spectrum Emission Mask summary results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadWlanSEMResults (ViSession instrSession,
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
    
    viCheckErr (viPrintf (instrSession, "FORM ASCII;:TRAC:DATA? LIST\n"));
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
/* Function: Fetch WLAN Burst All
/* Purpose:  This function returns all the results.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanBurstAll(ViSession   instrSession,
                                             ViReal64    results[],
                                             ViInt32*    returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K90")||strstr (buffer, "K91")))
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
/* Function: Fetch WLAN Burst Power
/* Purpose:  This function returns the power measurement value, measured 
/*           during the measurement.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPower(ViSession   instrSession,
                                                ViInt32     powerMeasurement,
                                                ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (powerMeasurement, RSSPECAN_VAL_WLAN_MEAS_PREA, RSSPECAN_VAL_WLAN_MEAS_CREST_AVER))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Power Measurement");
    }
    
    switch (powerMeasurement){
        case RSSPECAN_VAL_WLAN_MEAS_PREA:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_WLAN_FETC_PRE, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_MEAS_PAYL:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_WLAN_FETC_PAYL, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_MEAS_RMS_MAX:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "Max", RSSPECAN_ATTR_WLAN_FETC_RMS, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_MEAS_RMS_MIN:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "Min", RSSPECAN_ATTR_WLAN_FETC_RMS, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_MEAS_RMS_AVER:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "Aver", RSSPECAN_ATTR_WLAN_FETC_RMS, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_MEAS_PEAK:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_WLAN_FETC_PEAK, 
                    value), 3, "Value");
        break;            
        case RSSPECAN_VAL_WLAN_MEAS_CREST_MAX:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "Max", RSSPECAN_ATTR_WLAN_FETC_CRES, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WLAN_MEAS_CREST_MIN:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "Min", RSSPECAN_ATTR_WLAN_FETC_CRES, 
                    value), 3, "Value");                    
        break;
        case RSSPECAN_VAL_WLAN_MEAS_CREST_AVER:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "Aver", RSSPECAN_ATTR_WLAN_FETC_CRES, 
                    value), 3, "Value");                    
        break;
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WLAN Burst Error
/* Purpose:  This function returns the burst errors.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanBurstError(ViSession    instrSession,
                                                ViInt32     errorType,
                                                ViInt32     mode,
                                                ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (errorType, RSSPECAN_VAL_WLAN_ERR_FREQ, RSSPECAN_VAL_WLAN_ERR_SYMB))
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
        case RSSPECAN_VAL_WLAN_ERR_FREQ:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_FERR, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_ERR_SYMB:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_SYMB, 
                    value), 3, "Value");
        break;
    }    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WLAN IQ Impairments
/* Purpose:  This function returns the burst errors.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanIQImp(ViSession     instrSession,
                                            ViInt32     iqImpairment,
                                            ViInt32     mode,
                                            ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (iqImpairment, RSSPECAN_VAL_WLAN_IQ_OFFS, RSSPECAN_VAL_WLAN_IQ_QUAD))
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
        case RSSPECAN_VAL_WLAN_IQ_OFFS:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_IQOF, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_IQ_IMBA:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_GIMB, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_IQ_QUAD:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_QUAD, 
                    value), 3, "Value");
        break;
     } 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Fetch WLAN EVM
/* Purpose:  This function returns the EVM measurement value.
/*           
/*           Note:
/*           (1) If no measurement has been performed yet, a query error 
/*           will occur.
/*           
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchWlanEVM(ViSession   instrSession,
                                        ViInt32     evm,
                                        ViInt32     mode,
                                        ViReal64*   value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (evm, RSSPECAN_VAL_WLAN_EVM_ALL, RSSPECAN_VAL_WLAN_EVM_PEAK))
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
        case RSSPECAN_VAL_WLAN_EVM_ALL:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_EVM_ALL, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_EVM_DATA:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_EVM_DATA, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_EVM_PILOT:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_EVM_PILOT, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_EVM:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_EVM, 
                    value), 3, "Value");
        break;
        case RSSPECAN_VAL_WLAN_EVM_PEAK:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FETC_EVM_DIR, 
                    value), 3, "Value");
        break;
     }   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function returns the number of analyzed PPDUs from the current 
/// HIFN  capture buffer or in all measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR type/Sets the count type. The driver uses this value to select the 
/// HIPAR type/attribute.
/// HIPAR count/This control returns the number of analyzed PPDUs from the current 
/// HIPAR count/capture buffer or in all measurements.
ViStatus _VI_FUNC rsspecan_FetchWlanBurstCount (ViSession instrSession,
                                                ViInt32 type, ViInt32 *count)
{
	ViStatus	error = VI_SUCCESS;
    

    checkErr (Rs_LockSession (instrSession, VI_NULL)); 

    if (rsspecan_invalidViInt32Range (type, RSSPECAN_VAL_WLAN_CURRENT, RSSPECAN_VAL_WLAN_ALL) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "type");
    }      
    
	if (type == RSSPECAN_VAL_WLAN_CURRENT)
	{
    	viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_FETC_BURS_COUNT, 
												count), 3, "Count");
    }
    else
	{
    	viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_WLAN_FETC_BURST_COUNT_ALL, 
												count), 3, "Count");
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function returns the number of symbols in each analyzed PPDU.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR symbolCount/This control returns the number of symbols in each analyzed PPDU.
/// HIPAR noOfValues/Returns the number of symbols.
ViStatus _VI_FUNC rsspecan_FetchWlanSymbolCount (ViSession instrSession,
                                                 ViInt32 arraySize,
                                                 ViInt32 symbolCount[],
                                                 ViInt32 *noOfValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
	ViChar      *pbuffer;
	int 		i = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	checkErr (viQueryf (instrSession, "FETC:SYMB:COUN?\n","%s", buffer));
	
	pbuffer = strtok(buffer, ",");
	
	while (pbuffer && i<arraySize)
    {
        symbolCount[i++] = atol (pbuffer);
		pbuffer = strtok(NULL, ",\n"); 
    } 
	if (noOfValues) *noOfValues = i;
	checkErr( rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function returns the length of the analyzed PPDUs from the 
/// HIFN  current measurement. If the number of PPDUs to analyze is greater than 
/// HIFN  the number of PPDUs that can be captured in one buffer, this command 
/// HIFN  only returns the lengths of the PPDUs in the current capture buffer.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR length/This control returns the length of the analyzed PPDUs from the current 
/// HIPAR length/measurement.
ViStatus _VI_FUNC rsspecan_FetchWlanBurstLength(
	ViSession	instrSession,
	ViInt32*	length
)
{
	ViStatus    error = VI_SUCCESS;

    checkErr (Rs_LockSession (instrSession, VI_NULL));
        
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", 
					RSSPECAN_ATTR_WLAN_FETC_BURST_LENGTH, length), 3, "Length");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}

/// HIFN  This function returns the average, maximum or minimum of the "Peak 
/// HIFN  Power per PPDU" (in dBm). All analyzed PPDUs, up to the statistic 
/// HIFN  length, take part in the statistical evaluation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR mode/Sets the measurement mode.
/// HIPAR value/This control returns the selected power value .
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPeak(
	ViSession	instrSession,
	ViInt32	mode,
	ViReal64*	value
)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

	switch (mode)
	{
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
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");    
    }

	viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
		RSSPECAN_ATTR_WLAN_FETCH_BURST_PEAK, value), 3, "Value");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function returns the average, maximum or minimum of the "Payload 
/// HIFN  Power per PPDU" (in dBm). All analyzed PPDUs, up to the statistic 
/// HIFN  length, take part in the statistical evaluation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR mode/Sets the measurement mode.
/// HIPAR value/This control returns the selected power value .
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPayload(
	ViSession	instrSession,
	ViInt32	mode,
	ViReal64*	value
)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

	switch (mode)
	{
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
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");    
    }

	viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
		RSSPECAN_ATTR_WLAN_FETCH_BURST_PAYLOAD, value), 3, "Value");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function returns the average, maximum or minimum of the "Peak 
/// HIFN  Power per PPDU" (in dBm). All analyzed PPDUs, up to the statistic 
/// HIFN  length, take part in the statistical evaluation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR mode/Sets the measurement mode.
/// HIPAR value/This control returns the selected power value .
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPreamble(
	ViSession	instrSession,
	ViInt32	mode,
	ViReal64*	value
)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

	switch (mode)
	{
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
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Mode");    
    }

	viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
		RSSPECAN_ATTR_WLAN_FETCH_BURST_PREAMBLE, value), 3, "Value");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function returns the start position of each analyzed PPDU in the 
/// HIFN  current capture buffer.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR value/This control returns the start position of each analyzed PPDU in the 
/// HIPAR value/current capture buffer.
ViStatus _VI_FUNC rsspecan_FetchWlanBurstStart(
	ViSession	instrSession,
	ViReal64*	value
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_WLAN_FETCH_BURST_START,
                                                value), 2, "Value");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function returns the power measurement results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR powerMeasurement/Selects the ACP (adjacent channel power) standard. 
/// HIPAR arraySize/Defines the memory size allocated for Results. 
/// HIPAR results/Returns the measured values:
/// HIPAR returnedValues/Returns number of returned values in Results array.
ViStatus _VI_FUNC rsspecan_GetWlanPowerResults (ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 powerMeasurement,
                                                ViInt32 arraySize,
                                                ViReal64 results[],
                                                ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      command[RSSPECAN_IO_BUFFER_SIZE] = "";
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
	ViChar      *pbuffer;
	int 		i = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	if (rsspecan_invalidViInt32Range (powerMeasurement, RSSPECAN_VAL_MEAS_POW_ACP, RSSPECAN_VAL_MEAS_POW_PPOW) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Power Measurement");
    } 
	
	sprintf (command, "CALC%ld:MARK:FUNC:POW:RES? %s", window, powerMeasurementArr[powerMeasurement]);
	checkErr (viQueryf (instrSession, command, "%s", buffer));
	
	pbuffer = strtok(buffer, ",");
	
	while (pbuffer && i<arraySize)
    {
        results[i++] = atof (pbuffer);
		pbuffer = strtok(NULL, ",\n"); 
    }
	
	if (*returnedValues) *returnedValues = i;
	
	checkErr( rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN Limit Check Result
/* Purpose:  This function returns the limit results
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanLimitCheckResult(ViSession   instrSession,
                                                ViInt32     window,
                                                ViInt32     limit,
                                                ViInt32     limitType,
                                                ViInt32     mode,
                                                ViInt32*    result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (rsspecan_invalidViInt32Range (limitType, RSSPECAN_VAL_WLAN_LIM_FERR, RSSPECAN_VAL_WLAN_LIM_EVM_PIL))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Type");
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
        case RSSPECAN_VAL_WLAN_LIM_FERR:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_FERR_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_SYMB:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_SYMB_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_IQOFF:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_IQOF_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM_ALL:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_EVM_ALL_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM_DATA:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_EVM_DATA_RES, 
            result), 6, "Result");
        break;
        case RSSPECAN_VAL_WLAN_LIM_EVM_PIL:
            viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_EVM_PIL_RES, 
            result), 6, "Result");
        break;
    }    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN ACP Channel Limit Check Results
/* Purpose:  This function queries the state of the limit check for the
/*           adjacent or alternate channels in an ACLR measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanACPChannLimitCheckResults(ViSession     instrSession,
                                                    ViInt32         window,
                                                    ViInt32         limit,
                                                    ViInt32         channelType,
                                                    ViInt32        results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
	ViChar      *p2buf = NULL;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    if (rsspecan_invalidViInt32Range (channelType, RSSPECAN_VAL_WLAN_ACP_ADJ, RSSPECAN_VAL_WLAN_ACP_ALT))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Type");
    } 
	
    switch (channelType){
        case RSSPECAN_VAL_WLAN_ACP_ADJ:
            viCheckErr( viQueryf (instrSession, "CALC%ld:LIM%ld:ACP:ACH:RES?\n", "%s", window, limit, buffer));
        break;
        case RSSPECAN_VAL_WLAN_ACP_ALT:
            viCheckErr( viQueryf (instrSession, "CALC%ld:LIM%ld:ACP:ALT:RES?\n", "%s", window, limit, buffer));
        break;
    }
	
	p2buf = strtok (buffer, ",\n\r");
	
	if (strcmp (p2buf, "PASSED") == 0)
		results[0] = 1;
	else
		results[0] = 0;
	
	p2buf = strtok (NULL, ",\r\n");
         
	if (strcmp (p2buf, "PASSED") == 0)
		results[1] = 1;
	else
		results[1] = 0;
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get WLAN All Limit Check Results
/* Purpose:  This function returns all limit results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanAllLimitCheckResults(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 limit,
                                                    ViInt32 results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViInt32     maxIdx = 12;
    ViChar      *pbuffer;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K90")||strstr (buffer, "K91")))
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
/* Function: Get WLAN Limit Check
/* Purpose:  This function returns the result of the limit check of the 
/*           limit line indicated in the selected
/*           measurement window.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetWlanLimitCheck(ViSession    instrSession,
                                                    ViInt32     window,
                                                    ViInt32     limit,
                                                    ViInt32     *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    } 
    
    sprintf (buffer, "C%ld,L%ld", window, limit);
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_WLAN_LIM_FAIL_RES, 
            result), 4, "Result");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


