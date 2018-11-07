/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  B71 - FFT Analyzer
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

/*===========================================================================*/
/* Function: Configure Analog Baseband Input
/* Purpose:  This function configures analog baseband input.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandInput(ViSession instrSession,
                                                        ViInt32 basebandInputs)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_SELECTION, basebandInputs),
		2, "Baseband Inputs");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function configures the Analog Baseband interface.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR signalType/Defines the type of calibration signal to be used for Analog Baseband.
/// HIPAR signalOutput/If enabled, the Analog Baseband calibration signal is output to the TRIGGER
/// HIPAR signalOutput/INPUT/OUTPUT connector (Trigger 2) on the front panel of the R&S FSW.
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalCalibration(ViSession instrSession,
                                                                    ViInt32 signalType,
                                                                    ViBoolean signalOutput)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_SERVICE_INPUT_SOURCE, RSSPECAN_VAL_INPUT_AIQ));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_SIGNAL_CALIBRATION_TYPE, signalType),
		2, "Signal Type");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_SIGNAL_CALIBRATION_OUTPUT, signalOutput),
		2, "Signal Output");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Input Measurement Range
/* Purpose:  This function configures the measurement range of the baseband
/*           input. The value range depends on the input impedance. The
/*           measurement range defines the measurable peak voltage (positive
/*           and negative).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandInputMeasurementRange(ViSession instrSession,
                                                                        ViReal64 measurementRange)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BB_INPUT_MEAS_RANGE, measurementRange),
		2, "measurementRange");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Trigger
/* Purpose:  This function configures analog baseband input trigger.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandTrigger(ViSession instrSession,
                                                          ViInt32 trigger)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_TRIGGER, trigger),
		2, "Trigger");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Trigger Level
/* Purpose:  This function configures analog baseband input trigger level.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandTriggerLevel(ViSession instrSession,
                                                               ViInt32 trigger,
                                                               ViReal64 triggerLevel)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (trigger)
	{
	case RSSPECAN_VAL_BB_TRIG_IONLY:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BB_INPUT_TRIGGER_I_LEVEL, triggerLevel),
			3, "Trigger Level");
		break;
	case RSSPECAN_VAL_BB_TRIG_QONLY:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BB_INPUT_TRIGGER_Q_LEVEL, triggerLevel),
			3, "Trigger Level");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, trigger), 2, "Trigger");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Parameters
/* Purpose:  This function configures analog baseband inputs.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandParameters(ViSession instrSession,
                                                             ViBoolean balanced,
                                                             ViInt32 impedance)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_BALANCED, balanced),
		2, "Balanced");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IMPEDANCE, impedance),
		3, "Impedance");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Signal Path
/* Purpose:  This function configures the signal path.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalPath(ViSession instrSession,
                                                             ViInt32 path)

{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_SIGNAL_PATH, path),
		2, "Path");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Signal Source Low Pass
/* Purpose:  This function configures the signal source low pass.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalSourceLowPass(ViSession instrSession,
                                                                      ViBoolean lowPass)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IQ_LPAS, lowPass),
		2, "Low Pass");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the fullscale level. The fullscale
/// HIFN  level defines the maximum power you can input at the Baseband
/// HIFN  Input connector without clipping the signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR mode/This control defines whether the fullscale level (i.e. the
/// HIPAR mode/maximum input power on the Baseband Input connector) is defined
/// HIPAR mode/automatically according to the reference level, or manually.
/// HIPAR level/This control defines the peak voltage at the Baseband Input
/// HIPAR level/connector if the fullscale level is set to manual mode (Mode
/// HIPAR level/control is set to Off).
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandFullscaleLevel(ViSession instrSession,
                                                                 ViBoolean mode,
                                                                 ViReal64 level)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_FULLSCALE_MODE, mode),
		2, "Mode");

	if (mode == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BB_FULLSCALE_LEVEL, level),
			3, "Level");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Analog Baseband Signal Source Dither
/* Purpose:  This function configures the signal source dither.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalSourceDither(ViSession instrSession,
                                                                     ViBoolean iqDither)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IQ_DITH, iqDither),
		2, "IQ Dither");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  Swaps the real (I branch) and the imaginary (Q branch) parts of the signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR swapIQ/Swaps the real (I branch) and the imaginary (Q branch) parts of the signal.
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSwapIQ(ViSession instrSession,
                                                         ViBoolean swapIQ)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_SWAP_IQ, swapIQ),
		2, "Swap IQ");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Set FFT Mode
/* Purpose:  This function selects the FFT Analyzer mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FFTSetFFTMode(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FFT, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Preset
/* Purpose:  This function sets the FFT Analyzer mode to the default state.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FFTPreset(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FFT_PRESET, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Recalc
/* Purpose:  This function restarts the analysis of captured data with new
/*           parameter settings, e.g. resolution bandwidth, window function
/*           or domain. This function is only available for CAPTURE BOTH
/*           DOMAIN ON , SINGLE SWEEP and Recalculation Auto is OFF
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FFTRecalc(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FFT_CAPT_CALC, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure FFT Reference Level
 * Purpose:     This function configures the vertical attributes of the
 *              FFT analyzer.  This corresponds to attributes like
 *              amplitude units, input impedance, reference
 *              level, and reference level offset.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFFTReferenceLevel(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 amplitudeUnits,
                                                      ViReal64 referenceLevel,
                                                      ViReal64 referenceLevelOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AMPLITUDE_UNITS, amplitudeUnits),
		3, "Amplitude Units");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel),
		4, "Reference Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, referenceLevelOffset),
		5, "Reference Level Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Frequency Domain
/* Purpose:  This function configures the parameters of frequency domain.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTFrequencyDomain(ViSession instrSession,
                                                       ViInt32 type,
                                                       ViInt32 window)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_FFT_FORM, type),
		2, "Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FFT_WIN_TYPE, window),
		3, "Window");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Time Domain
/* Purpose:  This function configures the parameters of time domain.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTTimeDomain(ViSession instrSession,
                                                  ViInt32 type)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_SPAN,
		0.0));
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_FFT_FORM, type),
		2, "Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Phase Diagram
/* Purpose:  This function configures the phase diagram.
/*
/*           Note:
/*           (1) This function is available only in the frequency domain of
/*           the magnitude/phase display when the FFT Analyzer mode is
/*           active.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTPhaseDiagram(ViSession instrSession,
                                                    ViInt32 phaseWrap,
                                                    ViReal64 phaseOffset,
                                                    ViInt32 phaseUnit)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (phaseUnit)
	{
	case RSSPECAN_VAL_UNIT_ANGLE_DEG:
		viCheckParm(RsCore_InvalidViReal64Range(instrSession, phaseOffset, -36000.0,36000.0), 3, "Phase Offset");
		break;
	case RSSPECAN_VAL_UNIT_ANGLE_RAD:
		viCheckParm(RsCore_InvalidViReal64Range(instrSession, phaseOffset, -0.0174533, 0.0174533), 3, "Phase Offset");
		break;
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_FFT_FORM, phaseWrap),
		2, "Phase Wrap");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FFT_UNIT, phaseUnit),
		4, "Phase Unit");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "C2", RSSPECAN_ATTR_FFT_OFFS_PHAS, phaseOffset),
		3, "Phase Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Phase Line
/* Purpose:  This function configures the phase diagram.
/*
/*           Note:
/*           (1) This function is available only in the frequency domain of
/*           the magnitude/phase display when the FFT Analyzer mode is
/*           active.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTPhaseLine(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 line,
                                                 ViBoolean phaseLineState,
                                                 ViReal64 phaseLinePosition)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
		3, "Line");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,L%ld", window, line);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_FFT_PLINE_STATE, phaseLineState),
		4, "Phase Line State");
	if (phaseLineState)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FFT_PLINE, phaseLinePosition),
			5, "Phase Line Position");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Signal Source
/* Purpose:  This function configures the signal source.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTSignalSource(ViSession instrSession,
                                                    ViInt32 path,
                                                    ViInt32 iqInput,
                                                    ViBoolean iqBalanced,
                                                    ViBoolean lowPass,
                                                    ViBoolean iqDither)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_SIGNAL_PATH, path),
		2, "Path");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IMPEDANCE, iqInput),
		3, "IQ Input");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_BALANCED, iqBalanced),
		4, "IQ Balanced");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IQ_LPAS, lowPass),
		5, "Low Pass");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BB_INPUT_IQ_DITH, iqDither),
		6, "IQ Dither");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Sweep
/* Purpose:  This function configures the data sweep and capture.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTSweep(ViSession instrSession,
                                             ViBoolean sweepModeContinuous,
                                             ViInt32 sweepCount,
                                             ViInt32 sweepPoints,
                                             ViBoolean capureBothDomain,
                                             ViBoolean recalculateAuto)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win1", RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, sweepModeContinuous),
		2, "Sweep Mode Continuos");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, sweepCount),
		3, "Sweep Count");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_SWEEP_POINTS, sweepPoints),
		4, "Sweep Points");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FFT_CAPT, capureBothDomain),
		5, "Capture Both Domain");
	if (capureBothDomain && !sweepModeContinuous)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FFT_CAPT_AUTO, recalculateAuto),
			6, "Recalculate Auto");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Bandwidth
/* Purpose:  This function configures the bandwidth for FFT Analyze
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTBandwidth(ViSession instrSession,
                                                 ViBoolean resolutionBandwidthAuto,
                                                 ViReal64 resolutionBandwidth,
                                                 ViReal64 resolutionBandwidthRatio,
                                                 ViInt32 resolutionBW1235)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win1", RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_AUTO, resolutionBandwidthAuto),
		2, "Resolution Bandidth Auto");
	if (!resolutionBandwidthAuto)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_RESOLUTION_BANDWIDTH, resolutionBandwidth),
			3, "Resolution Bandidth");
	}
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_RATIO, resolutionBandwidthRatio),
		4, "Resolution Bandidth Ratio");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FFT_BAND_STEP_MODE, resolutionBW1235),
		5, "Resolution BW1235");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: FFT Configure Diagnostic
/* Purpose:  This function configures the system diagnostic
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFFTDiagnostic(ViSession instrSession,
                                                  ViInt32 signalPath,
                                                  ViInt32 signalSource,
                                                  ViReal64 voltage,
                                                  ViReal64 frequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FFT_CAL_SIG, signalPath),
		2, "Signal Path");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FFT_CAL_SIG_SOUR, signalSource),
		3, "Signal Source");
	switch (signalSource)
	{
	case RSSPECAN_VAL_FFT_SIG_SOUR_CALD:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FFT_CAL_DC, voltage),
			4, "Voltage");
	case RSSPECAN_VAL_FFT_SIG_SOUR_CALP:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FFT_CAL_FREQ, frequency),
			5, "Frequency");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
