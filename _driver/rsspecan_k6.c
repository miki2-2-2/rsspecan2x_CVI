/*****************************************************************************
 *  Rohde&Schwarz Pulse measurement instrument driver - option K6
 *                              
 *  VXIpnp Style Instrument Driver
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

/*****************************************************************************
 *  K6 - Pulse Measurement
 *****************************************************************************/

#include "rsspecan.h" 

static ViString PulsePowerParameterArr[] = {"ALL", "TOP", "BASE", "AMPL", "ON", "AVG", "MIN", "MAX", 
										"PON", "PAVG", "PMIN", "ADP", "ADDB", "RPER", "RDB",
										"OPER", "ODB", "POIN", "PPR", VI_NULL};
static ViString PulseTimingParameterArr[] = {"ALL", "TST", "SETT", "RISE", "FALL", "PWID", 
										"OFF", "DRAT", "DCYC", "PRI", "PRF", VI_NULL};
static ViString PulseFreqParameterArr[] = {"ALL", "POIN", "PPFR", "RERR", "PERR", "DEV", "CRAT", VI_NULL};
static ViString PulsePhaseParameterArr[] = {"ALL", "POIN", "PPPH", "RERR", "PERR", "DEV", VI_NULL};
static ViString PulseYAxisArr[] = {"COUN", "OCC", VI_NULL};
static ViString PulseOnOffArr[] = {"OFF", "ON", VI_NULL}; 
static ViString PulseUnitHZArr[] = {"HZ", "KHZ", "MHZ", "GHZ", VI_NULL}; 
static ViString PulseUnitSArr[] = {"NS", "US", "MS", "S", VI_NULL};
static ViString PulseQueryRangeArr[] = {"SEL", "CURR", "ALL", VI_NULL};
static ViString PulseResultTypeArr[] = {"", ":MIN", ":MAX", ":AVER", ":SDEV", VI_NULL}; 
static ViString PulsePowerResultArr[] = {"", "TOP", "BASE", "AMPL", "ON", "AVG", "MIN", "MAX", 
										"PON", "PAVG", "PMIN", "ADR:PERC", "ADR:DB", "RIPP:PERC", 
										"RIPP:DB", "OVER:PERC", "OVER:DB", "POIN", "PPR", VI_NULL};
static ViString exportCollumnsArr[] = {"SEL", "ALL", VI_NULL};

/// HIFN  This function selects the Pulse measurement mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_PulseMeasurementMode (ViSession instrSession)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckErr(rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_PULSE_MODE, ""));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures pulse period, amplitode droop and pulse modulation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR pulsePeriod/This control defines how a pulse is detected.
/// HIPAR pulseHasDroop/If ON, a pulse can be modeled as having amplitude droop, 
/// HIPAR pulseHasDroop/otherwise the pulse top is assumed to be flat.
/// HIPAR pulseModulation/This control sets the type of pulse modulation which is expected.
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalModel (ViSession instrSession,
                                                      ViInt32 pulsePeriod,
                                                      ViBoolean pulseHasDroop,
                                                      ViInt32 pulseModulation)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_PERIOD,
												pulsePeriod), 2, "Pulse Period");
	
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_DROOP_STATE,
												pulseHasDroop), 3, "Pulse Has Droop");
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_MODULATION,
												pulseModulation), 4, "Pulse Modulation");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function sets automatic pulse signal timing.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalTimingAuto (ViSession instrSession)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckErr(rsspecan_SetAttributeViBoolean (instrSession, "", 
								RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_AUTO_MODE, VI_TRUE)); 
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures manual pulse signal timing.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR minPulseWidth/Defines a minimum pulse width
/// HIPAR maxPulseWidth/Defines a maximum pulse width
/// HIPAR minPulseOffTime/The minimum time the pulse is "off", i.e. the time between successive pulses.
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalTimingManual (ViSession instrSession,
                                                             ViReal64 minPulseWidth,
                                                             ViReal64 maxPulseWidth,
                                                             ViReal64 minPulseOffTime)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
	
	viCheckErr(rsspecan_SetAttributeViBoolean (instrSession, "", 
								RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_AUTO_MODE, VI_FALSE));
    
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Min",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_WIDTH,
												minPulseWidth), 2, "Min Pulse Width");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Max",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_WIDTH,
												maxPulseWidth), 3, "Max Pulse Width");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_OFF_TIME,
												minPulseOffTime), 4, "Min Pulse Off Time");;

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures pulse signal frequency offset.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR autoMode/If enabled, the frequency offset is estimated automatically for each individual pulse.
/// HIPAR value/Defines a known frequency offset to be corrected in the pulse acquisition data.
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalFrequencyOffset (ViSession instrSession,
                                                                ViBoolean autoMode,
                                                                ViReal64 value)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_OFFSET_AUTO_MODE,
												autoMode), 2, "Auto Mode");
	if (!autoMode) 
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_OFFSET_VALUE,
												value), 3, "Value"); 
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures pulse signal chirp rate.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR autoMode/If enabled, the chirp rate is estimated automatically for each individual pulse.
/// HIPAR chirpRate/Defines a known frequency chirp rate (in Hz/us) to be used to generate an ideal 
/// HIPAR chirpRate/pulse waveform for computing frequency and phase error parameters.
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalChirpRate (ViSession instrSession,
                                                          ViBoolean autoMode,
                                                          ViReal64 chirpRate)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_CHIRP_RATE_AUTO_MODE,
												autoMode), 2, "Auto Mode");
	
	if (!autoMode)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SIGNAL_CHIRP_RATE_VALUE,
												chirpRate), 3, "Chirp Rate"); 
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function selects the signal source for measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR source/This control selects the signal source for measurements.
ViStatus _VI_FUNC rsspecan_ConfigurePulseInputSignalSource (ViSession instrSession,
                                                            ViInt32 source)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_INPUT_SIGNAL_SOURCE,
												source), 2, "Source");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN  This function selects the I/Q data file to be used as input for 
/// HIFN  further measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR fileName/String containing the path and name of the source file. The file
ViStatus _VI_FUNC rsspecan_PulseSelectIQDataFile (ViSession instrSession,
                                                  ViString fileName)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SELECT_IQ_DATA_FILE,
												fileName), 2, "File Name");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures radio frequency input.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR inputCoupling/The RF input of the R&S FSW can be coupled by alternating current (AC) or direct current (DC).
/// HIPAR impedance/This control selects the nominal input impedance.
/// HIPAR highpassFilter/Activates an additional high pass filter for RF input signals from 1...3 GHz.
/// HIPAR YIGPreselector/This control turns the YIG-preselector on and off.
ViStatus _VI_FUNC rsspecan_ConfigurePulseRFInput (ViSession instrSession,
                                                  ViInt32 inputCoupling,
                                                  ViReal64 impedance,
                                                  ViBoolean highpassFilter,
                                                  ViBoolean YIGPreselector)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_AMPL_RF_COUPLING,
												inputCoupling), 2, "Input Coupling");
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_INPUT_IMPEDANCE,
												(ViInt32)impedance), 3, "Impedance");
	
	viCheckParm (rsspecan_SetAttributeViBoolean(instrSession, "",
                                                RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER,
												highpassFilter), 4, "Highpass Filter");
	
	/* Not yet implemented
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_AMPL_YIG_FILTER,
												YIGPreselector), 5, "YIG Preselector");
												 */
    YIGPreselector;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function couples the step size of the center frequency to the span (ON) or sets the 
/// HIFN value of the center frequency entered via "Step Size" (OFF).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR stepSize/Couples the step size of the center frequency to the span (ON) or sets the value 
/// HIPAR stepSize/of the center frequency entered via "Step Size" (OFF).
/// HIPAR stepSizeAuto/Sets the step width of the center frequency if "Step Size Auto" is ON.
ViStatus _VI_FUNC rsspecan_ConfigurePulseFrequencyStepSize (ViSession instrSession,
                                                            ViBoolean stepSizeAuto,
															ViReal64 stepSize)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0",
                                                RSSPECAN_ATTR_FREQUENCY_STEP_AUTO,
												stepSizeAuto), 2, "Step Size Auto");
	if (!stepSizeAuto)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win0",
                                                RSSPECAN_ATTR_FREQUENCY_CENTER_STEP,
												stepSize), 3, "Step Size");
	}																		 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines the mechanical attenuation for RF input.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR value/This control defines the attenuation.
/// HIPAR autoMode/This control couples or decouples the attenuation to the reference level.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMechanicalAttenuator (ViSession instrSession,
                                                               ViBoolean autoMode,
															   ViReal64 value)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0",
                                                RSSPECAN_ATTR_ATTENUATION_AUTO,
												autoMode), 2, "Auto Mode");
	
	if (!autoMode) 
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win0",
                                                RSSPECAN_ATTR_ATTENUATION,
												value), 3, "Value");
	}																		 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the signal characteristics.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR state/This contorl switches the electronic input attenuation into the signal path (state ON) 
/// HIPAR state/or removes it from the signal path (state OFF).
/// HIPAR electronicAttenuatorAuto/This control specifies, that instrument automatically couples the electronic
/// HIPAR electronicAttenuatorAuto/input attenuation to the reference level and the attenuation of the 
/// HIPAR electronicAttenuatorAuto/mechanical attenuator (state ON) or switches the input attenuation to manual entry (state OFF).
/// HIPAR manualValuedB/This control sets the manual input electronic attenuation level.
ViStatus _VI_FUNC rsspecan_ConfigurePulseElectronicAttenuator (ViSession instrSession,
                                                               ViBoolean state,
                                                               ViBoolean electronicAttenuatorAuto,
                                                               ViInt32 manualValuedB)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0",
                                                RSSPECAN_ATTR_AMPL_EATT_STATE,
												state), 2, "State");
	
	if (state) 
	{
		viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "Win0",
                                                RSSPECAN_ATTR_AMPL_EATT_AUTO, electronicAttenuatorAuto), 
												3, "Electronic Attenuator Auto");
		if (!electronicAttenuatorAuto)
		{
			viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Win0",
                                                RSSPECAN_ATTR_AMPL_EATT_VALUE,
												manualValuedB), 3, "Manual Value dB");
		}
	}
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the preamplifier for the instrument.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR state/Switches on or off the preamplifier for the instrument. 
/// HIPAR preamplifierLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigurePulsePreamplifier (ViSession instrSession,
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

/// HIFN This function configures the pulse measurement acquisition.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR filterType/This control defines the type of demodulation filter to be used.
/// HIPAR measurementBandwidth/Defines the measurement bandwidth in Hz.
/// HIPAR measurementTime/Defines how long data is captured for analysis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementAcquisition (ViSession instrSession,
                                                                 ViInt32 filterType,
                                                                 ViReal64 measurementBandwidth,
                                                                 ViReal64 measurementTime)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_ACQUISITION_FILTER_TYPE,
												filterType), 2, "Filter Type");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_ACQUISITION_BANDWIDTH,
												measurementBandwidth), 3, "Measurement Bandwidth");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_SWEEP_TIME,
												measurementTime), 4, "Measurement Time");
																			 
Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse detection.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR referenceSource/This control defines the level to be used as a reference for the pulse detection threshold.
/// HIPAR threshold/Defines a threshold for pulse detection.
/// HIPAR hysteresis/Defines a hysteresis for pulse detection in dB in relation to the defined threshold.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementDetection (ViSession instrSession,
                                                               ViInt32 referenceSource,
                                                               ViReal64 threshold,
                                                               ViReal64 hysteresis)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_DETECTION_REF_SOURCE,
												referenceSource), 2, "Reference Source");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_DETECTION_THRESHOLD,
												threshold), 3, "Threshold");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_DETECTION_HYSTERESIS,
												hysteresis), 4, "Hysteresis");
																			 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN This function configures the pulse detection limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR detectionLimit/If enabled, the number of pulses to be detected is restricted.
/// HIPAR maximumPulseCount/Defines the maximum number of pulses to be detected.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementDetectionLimit (ViSession instrSession,
                                                                    ViBoolean detectionLimit,
                                                                    ViInt32 maximumPulseCount)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_DETECTION_LIMIT_STATE,
												detectionLimit), 2, "Detection Limit");
	
	if (detectionLimit) 
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_DETECTION_PULSE_COUNT,
												maximumPulseCount), 3, "Maximum Pulse Count");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse measurement reference point.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR reference/Defines the reference point used for specifying the pulse time instant.
/// HIPAR offset/The time instant used for in-pulse measurements e.g. phase or for the pulse timestamp.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementPoint (ViSession instrSession,
                                                           ViInt32 reference,
                                                           ViReal64 offset)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_POINT_TYPE,
												reference), 2, "Reference");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_POINT_OFFSET,
												offset), 3, "offset");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Measurement point results are averaged over a window centered at the 
/// HIFN  measurement point. The length of the averaging window in seconds can be 
/// HIFN  defined. A minimum length of 1 sample is enforced internally.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR averagingWindow/Size of the window around the measurement point used for averaging.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementPointAveragingWindow(
	ViSession	instrSession,
	ViReal64	averagingWindow
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_POINT_AVERAGING_WINDOW,
                                                averagingWindow), 2, "Averaging Window");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN This function configures the pulse measurement reference level.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR reference/Defines the reference for the measurement range definition. 
/// HIPAR length/The estimation range length as a percentage of the pulse top length.
/// HIPAR riseOffset/The offset in seconds from the pulse rising edge at which the estimation range begins.
/// HIPAR fallOffset/The offset in seconds from the pulse falling edge at which the estimation range ends.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementRange (ViSession instrSession,
                                                           ViInt32 reference,
                                                           ViReal64 length,
                                                           ViReal64 riseOffset,
                                                           ViReal64 fallOffset)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_TYPE,
												reference), 2, "Reference");
	
	if (reference == RSSPECAN_VAL_PULSE_REFERENCE_RANGE_CENTER)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_LENGTH,
												length), 3, "Length");
	}
	else
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Left",
	                                                RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_OFFSET,
													riseOffset), 4, "Rise Offset");
	
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Right",
	                                                RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_OFFSET,
													fallOffset), 5, "Fall Offset"); 
	}
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse measurement top (100%) level
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR position/Determines whether the 100% value (from base to top) for the rise and fall 
/// HIPAR position/time measurements is calculated from the Edges.
/// HIPAR measurementAlgorithm/This control defines the algorithm used to detect the pulse top level.
/// HIPAR ripplePortion/Defines a hysteresis for pulse detection in dB in relation to the defined threshold.
ViStatus _VI_FUNC rsspecan_ConfigurePulseTopLevel (ViSession instrSession,
                                                   ViInt32 position,
                                                   ViInt32 measurementAlgorithm,
                                                   ViReal64 ripplePortion)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_TOP_LEVEL_POSITION,
												position), 2, "Position");
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_TOP_LEVEL_ALGORITHM,
												measurementAlgorithm), 3, "Measurement Algorithm");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
	                                            RSSPECAN_ATTR_PULSE_TOP_LEVEL_RIPPLE_PORTION,
												ripplePortion), 4, "Ripple Portion"); 
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse measurement reference level.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR unit/Defines the unit of the pulse amplitude values.
/// HIPAR highThreshold/The upper threshold in percent of the pulse amplitude.
/// HIPAR middleThreshold/The middle threshold in percent of the pulse amplitude.
/// HIPAR middleThreshold/The lower threshold in percent of the pulse amplitude.
/// HIPAR boundary/The boundary in percent of the pulse amplitude.
ViStatus _VI_FUNC rsspecan_ConfigurePulseReferenceLevel (ViSession instrSession,
                                                         ViInt32 unit,
                                                         ViReal64 highThreshold,
                                                         ViReal64 middleThreshold,
                                                         ViReal64 lowerThreshold,
                                                         ViReal64 boundary)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_UNIT,
												unit), 2, "Unit");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "HRef",
                                                RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_THRESHOLD,
												highThreshold), 3, "High Threshold");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "MRef",
	                                            RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_THRESHOLD,
												middleThreshold), 4, "Middle Threshold");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "LRef",
	                                            RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_THRESHOLD,
												lowerThreshold), 5, "Lower Threshold");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
	                                            RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_BOUNDARY,
												boundary), 6, "Boundary");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function selects a particular pulse for which the traces, parameters and results are displayed. 
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR pulseNumber/Selects a particular pulse for which the traces, parameters and results are displayed.
ViStatus _VI_FUNC rsspecan_ConfigurePulseSelection (ViSession instrSession,
                                                    ViInt32 pulseNumber)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_SELECTION,
												pulseNumber), 2, "Pulse Number");  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures whether the result range length is
/// HIFN  determined automatically according to the width of the selected
/// HIFN  pulse.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR automaticRangeScaling/If enabled, the result range length is determined automatically
/// HIPAR automaticRangeScaling/according to the width of the selected pulse.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultAutomaticRangeScaling (ViSession instrSession,
                                                                      ViInt32 automaticRangeScaling)
{
	ViStatus	error = VI_SUCCESS;
	
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	if (automaticRangeScaling == RSSPECAN_VAL_PULSE_RESULT_SCALE_ONCE)
	{
		viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_PULSE_RESULT_AUTOMATIC_RANGE_SCALING_ONCE, NULL), 2, "Automatic Range Scaling");
	}
	else
	{
		viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_RESULT_AUTOMATIC_RANGE_SCALING,
												automaticRangeScaling != 0), 2, "Automatic Range Scaling");
	}
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse refernece point and offset.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR reference/Specifies the reference point used to define the result range.
/// HIPAR offset/The offset (in seconds) from the reference point at which the pulse result range is aligned.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultReferencePoint (ViSession instrSession,
                                                               ViInt32 reference,
                                                               ViReal64 offset)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_RESULT_RANGE_REFERENCE,
												reference), 2, "Reference"); 
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_RESULT_RANGE_OFFSET,
												offset), 3, "Offset"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse result range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR alignment/Specifies the alignment with respect to the reference point used to define the result range.
/// HIPAR length/The length of the pulse result range (in seconds).
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultRange (ViSession instrSession,
                                                      ViInt32 alignment,
                                                      ViReal64 length)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_RESULT_RANGE_ALIGNMENT,
												alignment), 2, "Alignment"); 
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_RESULT_RANGE_LENGTH,
												length), 3, "Length"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function selects the unit for angles (for PM display).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR unit/This control selects the unit for angles (for PM display).
ViStatus _VI_FUNC rsspecan_ConfigurePulsePhaseUnit (ViSession instrSession,
                                                    ViInt32 unit)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_PULSE_RESULT_PHASE_UNIT,
												unit), 2, "Unit"); 
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse result Power Parameter Trend.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR axis/Selects parameter group of parameter trend.
/// HIPAR group/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendPower (ViSession instrSession,
                                                                    ViInt32 window,
                                                                    ViInt32 group,
                                                                    ViInt32 axis)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    sprintf (buffer, "Win%ld", window);

    if (group == RSSPECAN_VAL_GROUP_Y)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_POWER_Y_AXIS, 
											axis), 4, "Axis");
	}
	else
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_POWER_X_AXIS, 
											axis), 4, "Axis");
	}
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse result Timing Parameter Trend.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR axis/Selects parameter group of parameter trend.
/// HIPAR group/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendTiming (ViSession instrSession,
                                                                     ViInt32 window,
                                                                     ViInt32 group,
                                                                     ViInt32 axis)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    sprintf (buffer, "Win%ld", window);

    if (group == RSSPECAN_VAL_GROUP_Y)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_TIMING_Y_AXIS, 
											axis), 4, "Axis");
	}
	else
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_TIMING_X_AXIS, 
											axis), 4, "Axis");
	}
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse result Frequency Parameter Trend.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR axis/Selects parameter group of parameter trend.
/// HIPAR group/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendFrequency (ViSession instrSession,
                                                                        ViInt32 window,
                                                                        ViInt32 group,
                                                                        ViInt32 axis)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    sprintf (buffer, "Win%ld", window);

    if (group == RSSPECAN_VAL_GROUP_Y)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_FREQUENCY_Y_AXIS, 
											axis), 4, "Axis");
	}
	else
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_FREQUENCY_X_AXIS, 
											axis), 4, "Axis");
	}
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the pulse result Phase Parameter Trend.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR axis/Selects parameter group of parameter trend.
/// HIPAR group/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendPhase (ViSession instrSession,
                                                                    ViInt32 window,
                                                                    ViInt32 group,
                                                                    ViInt32 axis)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    sprintf (buffer, "Win%ld", window);

    if (group == RSSPECAN_VAL_GROUP_Y)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_PHASE_Y_AXIS, 
											axis), 4, "Axis");
	}
	else
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_PHASE_X_AXIS, 
											axis), 4, "Axis");
	}
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function sets the number of bins used to calculate the histogram.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR numberOfBins/This control sets the number of bins used to calculate the histogram.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionBins(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	numberOfBins
)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    sprintf (buffer, "Win%ld", window);

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
											RSSPECAN_ATTR_PULSE_PARAMETER_DISTRIBUTION_BINS, 
											numberOfBins), 3, "Number Of Bins");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}


/// HIFN  This function configures the Power Parameter Distribution result 
/// HIFN  display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR yAxis/This control defines the parameter for which the trend is displayed on 
/// HIPAR yAxis/the y-axis.
/// HIPAR xAxis/Defines the scaling of the x-axis.  
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionPower (ViSession instrSession,
                                                                           ViInt32 window,
                                                                           ViInt32 xAxis,
                                                                           ViInt32 yAxis)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
	ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (xAxis, RSSPECAN_VAL_PULSE_RESULT_POWER_TOP, 
										RSSPECAN_VAL_PULSE_RESULT_POWER_PPR) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "X Axis");  
    }
	
	if (rsspecan_invalidViInt32Range (yAxis, RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_COUNT, 
										RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_OCCURANCE) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Y Axis");  
    }
	
	if (window == 0)
		sprintf (cmd, "CALC:DIST:POW %s,%s", PulsePowerParameterArr[xAxis], PulseYAxisArr[yAxis]);
	else
    	sprintf (cmd, "CALC%ld:DIST:POW %s,%s", window, PulsePowerParameterArr[xAxis], PulseYAxisArr[yAxis]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;  
}


/// HIFN  This function configures the Timing Parameter Distribution result 
/// HIFN  display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR yAxis/Defines the parameter for which the trend is displayed on the y-axis.
/// HIPAR xAxis/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionTiming(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	xAxis,
	ViInt32	yAxis
)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
	ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (xAxis, RSSPECAN_VAL_PULSE_RESULT_TIMING_TST, 
										RSSPECAN_VAL_PULSE_RESULT_TIMING_PRF) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "X Axis");  
    }
	
	if (rsspecan_invalidViInt32Range (yAxis, RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_COUNT, 
										RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_OCCURANCE) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Y Axis");  
    }
	
	if (window == 0)
		sprintf (cmd, "CALC:DIST:TIM %s,%s", PulseTimingParameterArr[xAxis], PulseYAxisArr[yAxis]);
	else
    	sprintf (cmd, "CALC%ld:DIST:TIM %s,%s", window, PulseTimingParameterArr[xAxis], PulseYAxisArr[yAxis]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}


/// HIFN  This function configures the Frequency Parameter Distribution result 
/// HIFN  display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR yAxis/Defines the parameter for which the trend is displayed on the y-axis.
/// HIPAR xAxis/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionFrequency(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	xAxis,
	ViInt32	yAxis
)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
	ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (xAxis, RSSPECAN_VAL_PULSE_RESULT_FREQ_POIN, 
										RSSPECAN_VAL_PULSE_RESULT_FREQ_CRAT) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "X Axis");  
    }
	
	if (rsspecan_invalidViInt32Range (yAxis, RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_COUNT, 
										RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_OCCURANCE) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Y Axis");  
    }
	
	if (window == 0)
		sprintf (cmd, "CALC:DIST:FREQ %s,%s", PulseFreqParameterArr[xAxis], PulseYAxisArr[yAxis]);
	else
    	sprintf (cmd, "CALC%ld:DIST:FREQ %s,%s", window, PulseFreqParameterArr[xAxis], PulseYAxisArr[yAxis]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}


/// HIFN  This function configures the Phase Parameter Distribution result 
/// HIFN  display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR yAxis/Defines the parameter for which the trend is displayed on the y-axis.
/// HIPAR xAxis/Defines the scaling of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionPhase(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	xAxis,
	ViInt32	yAxis
)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
	ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (rsspecan_invalidViInt32Range (xAxis, RSSPECAN_VAL_PULSE_RESULT_PHASE_POIN, 
										RSSPECAN_VAL_PULSE_RESULT_PHASE_DEV) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "X Axis");  
    }
	
	if (rsspecan_invalidViInt32Range (yAxis, RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_COUNT, 
										RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_OCCURANCE) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Y Axis");  
    }
	
	if (window == 0)
		sprintf (cmd, "CALC:DIST:PHAS %s,%s", PulsePhaseParameterArr[xAxis], PulseYAxisArr[yAxis]);
	else
    	sprintf (cmd, "CALC%ld:DIST:PHAS %s,%s", window, PulsePhaseParameterArr[xAxis], PulseYAxisArr[yAxis]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}

/// HIFN  This function configures parameter to be displayed on the x-axis.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_FREQUENCY
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     CALCulate<n>:PSPectrum:FREQuency
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the frequency parameter.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumFrequency(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	parameter
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	rep_cap[5] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (window, 0, 16) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");
    }

    sprintf (rep_cap, "Win%ld", window);  

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_FREQUENCY,
                                               parameter), 3, "Parameter");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function configures parameter to be displayed on the x-axis.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_PHASE
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     CALCulate<n>:PSPectrum:PHASe
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the phase parameter.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumPhase(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	parameter
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	rep_cap[5] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (window, 0, 16) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");
    }

    sprintf (rep_cap, "Win%ld", window);  

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_PHASE,
                                               parameter), 3, "Parameter");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function configures parameter to be displayed on the x-axis.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_POWER
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     CALCulate<n>:PSPectrum:POWer
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the power parameter.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumPower(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	parameter
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	rep_cap[5] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (window, 0, 16) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");
    }

    sprintf (rep_cap, "Win%ld", window);  

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_POWER,
                                               parameter), 3, "Parameter");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function configures parameter to be displayed on the x-axis.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_TIMING
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     CALCulate<n>:PSPectrum:TIMing
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the timing parameter.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumTiming(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	parameter
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	rep_cap[5] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (window, 0, 16) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");
    }

    sprintf (rep_cap, "Win%ld", window);  

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_TIMING,
                                               parameter), 3, "Parameter");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN This function shows the power parameter in result table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR visibility/Shows / hides the parameter in the result table.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTablePower (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViInt32 parameter,
                                                           ViBoolean visibility)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_POWER_ALL, 
										RSSPECAN_VAL_PULSE_RESULT_POWER_PPR) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Parameter");  
    }
	
	sprintf (rep_cap, "Win%ld,%s", window, PulsePowerParameterArr[parameter]);
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, rep_cap,
                                                RSSPECAN_ATTR_PULSE_RESULT_TABLE_POWER,
												visibility), 4, "visibility");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function shows the timing parameter in result table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR visibility/Shows / hides the parameter in the result table.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTableTiming (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViInt32 parameter,
                                                            ViBoolean visibility,
                                                            ViInt32 scaling)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [100] = "";
    ViChar      buffer [1024] = "";
	ViChar*     p2buf = NULL;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION)
    
    if (rsspecan_invalidViInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_TIMING_ALL, 
										RSSPECAN_VAL_PULSE_RESULT_TIMING_PRF) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Parameter");  
    }
	
	if (rsspecan_invalidViInt32Range (scaling, RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_HZ_NS, 
										RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_GHZ_S) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Scaling");  
    }
	
	if (window == 0)
		p2buf = cmd + sprintf (cmd, "CALC:TABL:TIM:%s %s", PulseTimingParameterArr[parameter], 
															PulseOnOffArr[visibility]);
	else
    	p2buf = cmd + sprintf (cmd, "CALC%ld:TABL:TIM:%s %s", window, PulseTimingParameterArr[parameter], 
															PulseOnOffArr[visibility]);
	if ((parameter != RSSPECAN_VAL_PULSE_RESULT_TIMING_DCYC) && (parameter != RSSPECAN_VAL_PULSE_RESULT_TIMING_DRAT))
	{
		if (parameter == RSSPECAN_VAL_PULSE_RESULT_TIMING_PRF)
			sprintf (p2buf, ",%s", PulseUnitHZArr[scaling]);
		else
			sprintf (p2buf, ",%s", PulseUnitSArr[scaling]); 
	}
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function shows the frequency parameter in result table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR visibility/Shows / hides the parameter in the result table.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTableFrequency (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViInt32 parameter,
                                                               ViBoolean visibility,
                                                               ViInt32 scaling)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [100] = "";
    ViChar      buffer [1024] = "";
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION)
    
    if (rsspecan_invalidViInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_FREQ_ALL, 
										RSSPECAN_VAL_PULSE_RESULT_FREQ_CRAT) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Parameter");  
    }
	
	if (rsspecan_invalidViInt32Range (scaling, RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_HZ_NS, 
										RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_GHZ_S) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Scaling");  
    } 
	
	if (window == 0)
		sprintf (cmd, "CALC:TABL:FREQ:%s %s,%s", PulseFreqParameterArr[parameter], 
												PulseOnOffArr[visibility], PulseUnitHZArr[scaling]);
	else
    	sprintf (cmd, "CALC%ld:TABL:FREQ:%s %s,%s", window, PulseFreqParameterArr[parameter], 
												PulseOnOffArr[visibility], PulseUnitHZArr[scaling]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function shows the phase parameter in result table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR visibility/Shows / hides the parameter in the result table.
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTablePhase (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViInt32 parameter,
                                                           ViBoolean visibility)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_PHASE_ALL, 
										RSSPECAN_VAL_PULSE_RESULT_PHASE_DEV) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Parameter");  
    }
	
	sprintf (rep_cap, "Win%ld,%s", window, PulsePhaseParameterArr[parameter]);
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, rep_cap,
                                                RSSPECAN_ATTR_PULSE_RESULT_TABLE_PHASE,
												visibility), 4, "visibility");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function exports result table data from the specified window to 
/// HIFN  an ASCii file (.DAT).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR exportCollumns/Selects the columns to be stored in file.
/// HIPAR filename/This parameter defines the path and file name of the target file.
ViStatus _VI_FUNC rsspecan_ExportPulseResultTable(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	exportCollumns,
	ViString	filename
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (window, 1, 16) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");
    }
	
	if (rsspecan_invalidViInt32Range (exportCollumns, RSSPECAN_VAL_PULSE_COLLUMNS_SELECTED, RSSPECAN_VAL_PULSE_COLLUMNS_ALL) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Export Collumns");
    }
	
    viCheckErr (viPrintf (instrSession, "MMEM:STOR%ld:TABL %s,'%s'\n", window, exportCollumnsArr[exportCollumns], filename));
	
	viCheckErr (rsspecan_CheckStatus (instrSession));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN The y-axis is scaled automatically according to the current measurement settings and results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
/// HIPAR autoScale/The y-axis is scaled automatically/manualy.
ViStatus _VI_FUNC rsspecan_ConfigurePulseAutomaticGridScaling (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViInt32 trace,
                                                               ViBoolean autoScale)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 1) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "trace");  
    }
	
	sprintf (rep_cap, "Win%ld,TR%ld", window, trace);
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, rep_cap,
                                                RSSPECAN_ATTR_PULSE_AUTO_GRID_SCALING,
												autoScale), 4, "autoScale");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines the scaling using absolute minimum and maximum values.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR maximum/This control defines the maximum value of the y-axis for the selected result display.
/// HIPAR minimum/This control defines the minimum value of the y-axis for the selected result display.
ViStatus _VI_FUNC rsspecan_ConfigurePulseAbsoluteScaling (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViReal64 minimum,
                                                          ViReal64 maximum)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap_min [100] = "";
	ViChar      rep_cap_max [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
	
	sprintf (rep_cap_min, "Win%ld,Min", window);
	sprintf (rep_cap_max, "Win%ld,Max", window);
	
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap_min,
                                                RSSPECAN_ATTR_PULSE_ABSOLUTE_SCALING,
												minimum), 3, "minimum");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap_max,
                                                RSSPECAN_ATTR_PULSE_ABSOLUTE_SCALING,
												maximum), 4, "Maximum"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines the scaling relative to a reference value, 
/// HIFN with a specified value range per division.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
/// HIPAR perDivison/This control defines the value range to be displayed per division 
/// HIPAR perDivison/of the diagram (1/10 of total range).
/// HIPAR referencePosition/This control defines the vertical position of the reference level 
/// HIPAR referencePosition/on the display grid.
/// HIPAR referenceValue/This control defines the reference value to be displayed at the
/// HIPAR referenceValue/specified reference position.
ViStatus _VI_FUNC rsspecan_ConfigurePulseRelativeScaling (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 trace,
                                                          ViReal64 perDivison,
                                                          ViReal64 referencePosition,
                                                          ViReal64 referenceValue)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 1) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "trace");  
    }
	
	sprintf (rep_cap, "Win%ld", window);
	
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                RSSPECAN_ATTR_DISP_Y_AXIS_SCALING,
												perDivison), 4, "Per Divison");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                RSSPECAN_ATTR_DISP_REF_POSITION,
												referencePosition), 5, "Reference Position");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                RSSPECAN_ATTR_REFERENCE_LEVEL,
												referenceValue), 6, "Reference Value");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines the minimum and maximum reference values assigned 
/// HIFN to the reference position in the specified window
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
/// HIPAR minimum/This control defines the minimum reference value assigned 
/// HIPAR minimum/to the reference position in the specified window.
/// HIPAR maximum/This control defines the maximum reference value assigned 
/// HIPAR maximum/to thereference position in the specified window.
ViStatus _VI_FUNC rsspecan_ConfigurePulseReferenceValues (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 trace,
                                                          ViReal64 minimum,
                                                          ViReal64 maximum)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap_min [100] = "";
	ViChar      rep_cap_max [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 1) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "trace");  
    }
	
	sprintf (rep_cap_min, "Win%ld,TR%ld,Min", window, trace);
	sprintf (rep_cap_max, "Win%ld,TR%ld,Max", window, trace);
	
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap_min,
                                                RSSPECAN_ATTR_PULSE_REFERENCE_VALUE_MIN_MAX,
												minimum), 4, "Minimum");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap_max,
                                                RSSPECAN_ATTR_PULSE_REFERENCE_VALUE_MIN_MAX,
												maximum), 5, "Maximum"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR markerNumber/Selects the marker to configure.
/// HIPAR markerState/Enables or disables the selected marker.
/// HIPAR stimulus/Horizontal position.
ViStatus _VI_FUNC rsspecan_ConfigurePulseMarker (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 markerNumber,
                                                 ViBoolean markerState,
                                                 ViReal64 stimulus)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (markerNumber, 1, 4) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker Number");  
    }
	
	sprintf (rep_cap, "Win%ld,M%ld", window, markerNumber);
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, rep_cap,
                                                RSSPECAN_ATTR_MARKER_ENABLED,
												markerState), 4, "Marker State");
	
	if (markerState)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                RSSPECAN_ATTR_MARKER_POSITION,
												stimulus), 5, "Stimulus");
	}

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures delta marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR deltaMarker/Selects the delta marker to configure.
/// HIPAR deltaMarkerState/Enables or disables the selected delta marker.
/// HIPAR stimulus/Horizontal position.
ViStatus _VI_FUNC rsspecan_ConfigurePulseDeltaMarker (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 deltaMarker,
                                                      ViBoolean deltaMarkerState,
                                                      ViReal64 stimulus)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (deltaMarker, 1, 4) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Delta Marker");  
    }
	
	sprintf (rep_cap, "Win%ld,DM%ld", window, deltaMarker);
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, rep_cap,
                                                RSSPECAN_ATTR_REFERENCE_MARKER_STATE,
												deltaMarkerState), 4, "Delta Marker State");
	if (deltaMarkerState)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                RSSPECAN_ATTR_REFERENCE_MARKER_POSITION,
												stimulus), 5, "Stimulus");
	}

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the ids of the detected pulses
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR queryRange/Selects the query range.
/// HIPAR arraySize/This parameter determines the size of array for Pulse IDs values.
/// HIPAR pulseIDs/This control returns the ids of the detected pulses.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryPulseIDs (ViSession instrSession,
                                          ViInt32 queryRange, ViInt32 arraySize,
                                          ViInt32 pulseIDs[],
                                          ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
	if (rsspecan_invalidViUInt32Range (queryRange, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Query Range");
            
    
    viCheckErr (viPrintf (instrSession, "SENS:PULS:ID? %s\n", PulseQueryRangeArr[queryRange]));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value)
	{
        if (cnt<arraySize)
            sscanf (pstring_value,"%ld",&pulseIDs[cnt]);
        pstring_value = strtok(NULL, ","); 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    *returnedValues = cnt;
    checkErr( rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function returns the detected pulse numbers.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR queryRange/Selects the query range.
/// HIPAR arraySize/This parameter determines the size of array for Pulse Numbers values.
/// HIPAR pulseNumbers/This control returns the detected pulse numbers.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryPulseNumbers (ViSession instrSession,
                                              ViInt32 queryRange,
                                              ViInt32 arraySize,
                                              ViInt32 pulseNumbers[],
                                              ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
	if (rsspecan_invalidViUInt32Range (queryRange, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Query Range");
            
    
    viCheckErr (viPrintf (instrSession, "SENS:PULS:NUMB? %s\n", PulseQueryRangeArr[queryRange]));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value)
	{
        if (cnt<arraySize)
            sscanf (pstring_value, "%ld", &pulseNumbers[cnt]);
        pstring_value = strtok (NULL, ","); 
        cnt++;
    }
    if (pbuffer) free (pbuffer);
    
    *returnedValues = cnt;
    checkErr (rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function returns the calculated pulse power parameters values.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR resultType/Selects measurement type.
/// HIPAR queryRange/Selects the query range.
/// HIPAR arraySize/This parameter determines the size of array result values.
/// HIPAR results/Returns selected power results.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryPulseResultPower (ViSession instrSession,
                                                  ViInt32 parameter,
                                                  ViInt32 resultType,
                                                  ViInt32 queryRange,
                                                  ViInt32 arraySize,
                                                  ViReal64 results[],
                                                  ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
	if (rsspecan_invalidViUInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_POWER_TOP, 
										RSSPECAN_VAL_PULSE_RESULT_POWER_PPR) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Parameter");
	
	if (rsspecan_invalidViUInt32Range (resultType, RSSPECAN_VAL_PULSE_MEASTYPE_CURR, 
										RSSPECAN_VAL_PULSE_MEASTYPE_SDEV) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");
	
	if (rsspecan_invalidViUInt32Range (queryRange, RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED, 
										RSSPECAN_VAL_PULSE_QUERY_RANGE_ALL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");
	
	if (queryRange == RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED && 
									!(resultType == RSSPECAN_VAL_PULSE_MEASTYPE_CURR)) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");    
    
    viCheckErr (viPrintf (instrSession, "SENS:PULS:POW:%s%s? %s\n", PulsePowerResultArr[parameter],
										PulseResultTypeArr[resultType], PulseQueryRangeArr[queryRange]));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok (pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize)
            sscanf (pstring_value, "%le", &results[cnt]);
        pstring_value = strtok (NULL, ","); 
        cnt++;
    }
    if (pbuffer) free (pbuffer);
    
    *returnedValues = cnt;
    checkErr (rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function returns the calculated pulse timing parameters values.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR resultType/Selects measurement type.
/// HIPAR queryRange/Selects the query range.
/// HIPAR arraySize/This parameter determines the size of array result values.
/// HIPAR results/Returns selected timing results.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryPulseResultTiming (ViSession instrSession,
                                                   ViInt32 parameter,
                                                   ViInt32 resultType,
                                                   ViInt32 queryRange,
                                                   ViInt32 arraySize,
                                                   ViReal64 results[],
                                                   ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
	if (rsspecan_invalidViUInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_TIMING_TST, 
										RSSPECAN_VAL_PULSE_RESULT_TIMING_PRF) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Parameter");
	
	if (rsspecan_invalidViUInt32Range (resultType, RSSPECAN_VAL_PULSE_MEASTYPE_CURR, 
										RSSPECAN_VAL_PULSE_MEASTYPE_SDEV) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");
	
	if (rsspecan_invalidViUInt32Range (queryRange, RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED, 
										RSSPECAN_VAL_PULSE_QUERY_RANGE_ALL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");
	
	if (queryRange == RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED && 
									!(resultType == RSSPECAN_VAL_PULSE_MEASTYPE_CURR)) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");    
    
    viCheckErr (viPrintf (instrSession, "SENS:PULS:TIM:%s%s? %s\n", PulseTimingParameterArr[parameter],
										PulseResultTypeArr[resultType], PulseQueryRangeArr[queryRange]));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok (pbuffer, ",");  
    while (pstring_value)
	{
        if (cnt<arraySize)
            sscanf (pstring_value, "%le", &results[cnt]);
        pstring_value = strtok (NULL, ","); 
        cnt++;
    }
    if (pbuffer) free (pbuffer);
    
    *returnedValues = cnt;
    checkErr (rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function returns the calculated pulse frequency parameters values.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR resultType/Selects measurement type.
/// HIPAR queryRange/Selects the query range.
/// HIPAR arraySize/This parameter determines the size of array result values.
/// HIPAR results/Returns selected frequency results.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryPulseResultFrequency (ViSession instrSession,
                                                      ViInt32 parameter,
                                                      ViInt32 resultType,
													  ViInt32 queryRange,
                                                      ViInt32 arraySize,
                                                      ViReal64 results[],
                                                      ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
	if (rsspecan_invalidViUInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_FREQ_POIN, 
										RSSPECAN_VAL_PULSE_RESULT_FREQ_CRAT) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Parameter");
	
	if (rsspecan_invalidViUInt32Range (resultType, RSSPECAN_VAL_PULSE_MEASTYPE_CURR, 
										RSSPECAN_VAL_PULSE_MEASTYPE_SDEV) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");
	
	if (rsspecan_invalidViUInt32Range (queryRange, RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED, 
										RSSPECAN_VAL_PULSE_QUERY_RANGE_ALL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");
	
	if (queryRange == RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED && 
									!(resultType == RSSPECAN_VAL_PULSE_MEASTYPE_CURR)) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");    
    
    viCheckErr (viPrintf (instrSession, "SENS:PULS:FREQ:%s%s? %s\n", PulseFreqParameterArr[parameter],
										PulseResultTypeArr[resultType], PulseQueryRangeArr[queryRange]));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok (pbuffer, ",");  
    while (pstring_value)
	{
        if (cnt<arraySize)
            sscanf (pstring_value, "%le", &results[cnt]);
        pstring_value = strtok (NULL, ","); 
        cnt++;
    }
    if (pbuffer) free (pbuffer);
    
    *returnedValues = cnt;
    checkErr (rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function returns the calculated pulse phase parameters values.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR parameter/This control selects the power parameter.
/// HIPAR resultType/Selects measurement type.
/// HIPAR queryRange/Selects the query range.
/// HIPAR arraySize/This parameter determines the size of array result values.
/// HIPAR results/Returns selected phase results.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryPulseResultPhase (ViSession instrSession,
                                                  ViInt32 parameter,
                                                  ViInt32 resultType,
                                                  ViInt32 queryRange,
                                                  ViInt32 arraySize,
                                                  ViReal64 results[],
                                                  ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K6")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
	if (rsspecan_invalidViUInt32Range (parameter, RSSPECAN_VAL_PULSE_RESULT_PHASE_POIN, 
										RSSPECAN_VAL_PULSE_RESULT_PHASE_DEV) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Parameter");
	
	if (rsspecan_invalidViUInt32Range (resultType, RSSPECAN_VAL_PULSE_MEASTYPE_CURR, 
										RSSPECAN_VAL_PULSE_MEASTYPE_SDEV) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");
	
	if (rsspecan_invalidViUInt32Range (queryRange, RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED, 
										RSSPECAN_VAL_PULSE_QUERY_RANGE_ALL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");
	
	if (queryRange == RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED && 
									!(resultType == RSSPECAN_VAL_PULSE_MEASTYPE_CURR)) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Query Range");    
    
    viCheckErr (viPrintf (instrSession, "SENS:PULS:PHAS:%s%s? %s\n", PulsePhaseParameterArr[parameter],
										PulseResultTypeArr[resultType], PulseQueryRangeArr[queryRange]));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok (pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize)
            sscanf (pstring_value, "%le", &results[cnt]);
        pstring_value = strtok (NULL, ","); 
        cnt++;
    }
    if (pbuffer) free (pbuffer);
    
    *returnedValues = cnt;
    checkErr (rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


























