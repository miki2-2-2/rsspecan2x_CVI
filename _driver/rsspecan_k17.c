/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  K17 Multi-Carrier Group Delay
 *
 *  Original Release: July 2012
 *  By: Martin Krcmar
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"

/*****************************************************************************
 *- RSSpecAn K17 - Multi-Carrier Group Delay --------------------------------*
 *****************************************************************************/

/// HIFN  This function selects the Multi-Carrier Group Delay mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_MultiCarrierGroupDelayMode(
	ViSession	instrSession
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MCGD_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
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
/// HIPAR yigPreselector/This control turns the YIG-preselector on and off.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayInput(
	ViSession	instrSession,
	ViInt32	inputCoupling,
	ViInt32	impedance,
	ViBoolean	highpassFilter,
	ViBoolean	yigPreselector
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AMPL_RF_COUPLING, inputCoupling),
    		2, "Input Coupling");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_INPUT_IMPEDANCE, impedance),
			3, "Impedance");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER, highpassFilter),
			4, "Highpass Filter");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_YIG_FILTER, yigPreselector),
			5, "YIG Preselector");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the reference level and offset.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR referenceLevel/The calibrated vertical position of the captured data used as a
/// HIPAR referenceLevel/reference for amplitude measurements. This is typically set to a value
/// HIPAR referenceLevel/slightly higher than the highest expected signal level.
/// HIPAR referenceLevelOffset/Specifies the offset value to the reference level. The driver uses
/// HIPAR referenceLevelOffset/this value to set the RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET attribute.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayReferenceLevel(
	ViSession	instrSession,
	ViInt32	window,
	ViReal64	referenceLevel,
	ViReal64	referenceLevelOffset
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel),
    		3, "Reference Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, referenceLevelOffset),
			4, "Reference Level Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayMechanicalAttenuator(
	ViSession	instrSession,
	ViBoolean	autoMode,
	ViReal64	value
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_ATTENUATION_AUTO, autoMode),
    		2, "Auto Mode");

	if (!autoMode)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_ATTENUATION, value),
				3, "Value");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the signal characteristics.
/// HIFN
/// HIFN     Note:
/// HIFN     (1) This function is available only with option B25.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/This contorl switches the electronic input attenuation into the signal
/// HIPAR state/path (state ON) or removes it from the signal path (state OFF).
/// HIPAR electronicAttenuatorAuto/This control specifies, that instrument automatically couples the
/// HIPAR electronicAttenuatorAuto/electronic input attenuation to the reference level and the attenuation
/// HIPAR electronicAttenuatorAuto/of the mechanical attenuator (state ON) or switches the input
/// HIPAR electronicAttenuatorAuto/attenuation to manual entry (state OFF).
/// HIPAR manualValue_dB/This control sets the manual input electronic attenuation level.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayElectronicAttenuator(
	ViSession	instrSession,
	ViBoolean	state,
	ViBoolean	electronicAttenuatorAuto,
	ViInt32	manualValue_dB
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AMPL_EATT_STATE, state),
    		2, "State");

	if (state)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AMPL_EATT_AUTO, electronicAttenuatorAuto),
				3, "Electronic Attenuator Auto");
		if (!electronicAttenuatorAuto)
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AMPL_EATT_VALUE, manualValue_dB),
					4, "Manual Value dB");
		}
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the preamplifier for the instrument.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) This function requires FSW-B24 option.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/Switches on or off the preamplifier for the instrument.
/// HIPAR preamplifierLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayPreamplifier(
	ViSession	instrSession,
	ViBoolean	state,
	ViInt32	preamplifierLevel
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AMPL_PREAMPLIFIER, state),
    		2, "State");

	if (state)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AMPL_PREAMPLIFIER_LEVEL, preamplifierLevel),
				3, "Preamplifier Level");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the Y-axis scaling.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) This function specifies settings for Magnitude display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR range/Defines the display range of the Y axis (level axis) in the selected
/// HIPAR range/measurement window with logarithmic scaling (DISP:TRAC:Y:SPAC LOG).
/// HIPAR referencePosition/Defines the position of the reference value in the selected
/// HIPAR referencePosition/measurement window.
/// HIPAR verticalScale/Pass the vertical scale that you want the spectrum analyzer to use.
/// HIPAR yAxisGridMode/Defines the scale type of the Y axis (absolute or relative) in the
/// HIPAR yAxisGridMode/selected measurement window.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayYAxisScaling(
	ViSession	instrSession,
	ViInt32	window,
	ViReal64	range,
	ViReal64	referencePosition,
	ViInt32	verticalScale,
	ViInt32	yAxisGridMode
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_LOG_RANGE, range),
    		3, "Range");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_REF_POSITION, referencePosition),
    		4, "Reference Position");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VERTICAL_SCALE, verticalScale),
    		5, "Vertical Scale");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, yAxisGridMode),
    		6, "Y-Axis Grid Mode");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures displayed range using minimum and maximum
/// HIFN  values.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) This function specifies settings for Phase and Group Delay
/// HIFN  displays.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR maximumValue/Defines the minimum value of the y-axis for the selected result
/// HIPAR maximumValue/display.
/// HIPAR miniumumValue/Defines the maximum value of the y-axis for the selected result
/// HIPAR miniumumValue/display.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayYAxisScalingMaxMin(
	ViSession	instrSession,
	ViInt32	window,
	ViReal64	maximumValue,
	ViReal64	miniumumValue
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,Max", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MCGD_VERTICAL_SCALE_MAX_MIN, maximumValue),
    		2, "Maximum Value");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,Min", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MCGD_VERTICAL_SCALE_MAX_MIN, miniumumValue),
			3, "Miniumum Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the Y-axis scaling.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) This function specifies settings for Phase and Group Delay
/// HIFN  displays.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR scaling/Defines the grid spacing on the Y-axis for all diagrams, where
/// HIPAR scaling/possible.
/// HIPAR referencePosition/Defines the position of the reference value in the selected
/// HIPAR referencePosition/measurement window.
/// HIPAR referenceValue/Defines the power value assigned to the reference position in the
/// HIPAR referenceValue/grid.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayYAxisScalingReferencePerDivision(
	ViSession	instrSession,
	ViInt32	window,
	ViReal64	scaling,
	ViReal64	referencePosition,
	ViReal64	referenceValue
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_Y_AXIS_SCALING, scaling),
    		2, "Scaling");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_REF_POSITION, referencePosition),
			3, "Reference Position");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_REF_VALUE, referenceValue),
			4, "Reference Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function selects the unit for angles (for PM display).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR unit/This control selects the unit for angles (for PM display).
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayPhaseUnit(
	ViSession	instrSession,
	ViInt32	unit
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MCGD_RESULT_PHASE_UNIT, unit),
    		2, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the carrier settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR numberOfCarriers/Sets the number of carriers.
/// HIPAR carrierSpacing/Sets the carrier spacing in Hz between the multiple carriers.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelaySignalDescription(
	ViSession	instrSession,
	ViInt32	numberOfCarriers,
	ViReal64	carrierSpacing
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MCGD_CARRIER_COUNT, numberOfCarriers),
    		2, "Number Of Carriers");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MCGD_CARRIER_SPACING, carrierSpacing),
    		3, "Carrier Spacing");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the measurement time.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR mode/Enables or disables automatic measurement time selection.
/// HIPAR measurementTime/Sets the measurement time.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayMeasurementTime(
	ViSession	instrSession,
	ViBoolean	mode,
	ViReal64	measurementTime
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MCGD_MEASUREMENT_TIME_AUTO, mode),
    		2, "Mode");

	if (mode == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MCGD_MEASUREMENT_TIME, measurementTime),
				3, "Measurement Time");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the measurement parameters for group delay
/// HIFN  calculation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR sweepCount/The number of sweeps to take.
/// HIPAR groupDelay/Defines how the group delay is calculated.
/// HIPAR carrierOffset/Defines the offset of the carrier compared to the center frequency so
/// HIPAR carrierOffset/the offset can be compensated in the display.
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelaySettings(
	ViSession	instrSession,
	ViInt32	sweepCount,
	ViInt32	groupDelay,
	ViInt32	carrierOffset
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, sweepCount),
    		2, "Sweep Count");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MCGD_MODE_ABS_REL, groupDelay),
			3, "Group Delay");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MCGD_CARRIER_OFFSET_MODE, carrierOffset),
			4, "Carrier Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries the offset in Hz of the carriers compared to the
/// HIFN  center frequency.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR carrierOffset/Queries the offset in Hz of the carriers compared to the center
/// HIPAR carrierOffset/frequency.
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayCarrierOffset(
	ViSession	instrSession,
	ViReal64*	carrierOffset
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MCGD_CARRIER_OFFSET, carrierOffset),
    		2, "Carrier Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function starts a new sweep to perform a calibration measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_InitiateMultiCarrierGroupDelayCalibration(
	ViSession	instrSession
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MCGD_INITIATE_CALIBRATION, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries the calibration status of the Multi Carrier
/// HIFN  Group Delay application.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR calibrationStatus/Queries the calibration status of the Multi Carrier Group Delay
/// HIPAR calibrationStatus/application.
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayCalibrationStatus(
	ViSession	instrSession,
	ViInt32*	calibrationStatus
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MCGD_CALIBRATION_STATUS, calibrationStatus),
    		2, "Calibration Status");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function stores the current calibration data to the selected
/// HIFN  file.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) The change the decimal separator use attribute
/// HIFN  RSSPECAN_ATTR_FILE_DEC_SEPARATOR or function
/// HIFN  rsspecan_FileDecimalSeparator.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR fileName/Defines the path and file name of the .csv file that will contain the
/// HIPAR fileName/calibration data.
ViStatus _VI_FUNC rsspecan_StoreMultiCarrierGroupDelayCalibrationData(
	ViSession	instrSession,
	ViString	fileName
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MCGD_STORE_CALIBRATION_DATA, fileName),
    		2, "FileName");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function loads the calibration data stored in the selected file
/// HIFN  and replaces the current data.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) The change the decimal separator use attribute
/// HIFN  RSSPECAN_ATTR_FILE_DEC_SEPARATOR or function
/// HIFN  rsspecan_FileDecimalSeparator.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR fileName/Defines the path and file name of the .csv file that will contain the
/// HIPAR fileName/calibration data.
ViStatus _VI_FUNC rsspecan_LoadMultiCarrierGroupDelayCalibrationData(
	ViSession	instrSession,
	ViString	fileName
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MCGD_LOAD_CALIBRATION_DATA, fileName),
    		2, "FileName");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function stores the currently displayed measurement results (i.e.
/// HIFN  all traces in all windows) to the selected file (.csv format).
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) The change the decimal separator use attribute
/// HIFN  RSSPECAN_ATTR_FILE_DEC_SEPARATOR or function
/// HIFN  rsspecan_FileDecimalSeparator.
/// HIFN
/// HIFN     (2) To store the results of an individual trace in ASCII format
/// HIFN  (.txt), use function rsspecan_StoreMultiCarrierGroupDelayTraceToFile.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR fileName/Defines the path and file name of the .csv file that will contain the
/// HIPAR fileName/calibration data.
ViStatus _VI_FUNC rsspecan_StoreMultiCarrierGroupDelayMeasurementResults(
	ViSession	instrSession,
	ViString	fileName
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MCGD_STORE_MEASUREMENT_RESULTS, fileName),
    		2, "FileName");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function stores the selected trace in the measurement window
/// HIFN  indicated by window in a file with ASCII format.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
/// HIPAR fileName/Specifies full path of the file, where trace data will be stored.
ViStatus _VI_FUNC rsspecan_StoreMultiCarrierGroupDelayTraceToFile(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	trace,
	ViString	fileName
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEM:STOR%ld:TRAC %ld,'%s'", window, trace, fileName);
    checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries the trace length.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
/// HIPAR numberOfTracePoints/Queries the trace length.
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayNumberOfTracePoints(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	trace,
	ViInt32*	numberOfTracePoints
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MCGD_TRACE_POINTS, numberOfTracePoints),
    		4, "Number Of Trace Points");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries the measurement bandwidth in Hz.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR bandwidth/Queries the measurement bandwidth in Hz.
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayMeasurementBandwidth(
	ViSession	instrSession,
	ViReal64*	bandwidth
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MCGD_BANDWIDTH, bandwidth),
    		2, "Bbandwidth");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function returns the record length determined by the current
/// HIFN  measurement settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR recordLength/Returns the record length determined by the current measurement
/// HIPAR recordLength/settings.
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayRecordLength(
	ViSession	instrSession,
	ViInt32*	recordLength
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MCGD_RECORD_LENGTH, recordLength),
    		2, "Record Length");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function returns the sample rate set up for current measurement
/// HIFN  settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR sampleRate/Returns the sample rate set up for current measurement settings.
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelaySampleRate(
	ViSession	instrSession,
	ViReal64*	sampleRate
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MCGD_SAMPLE_RATE, sampleRate),
    		2, "Sample Rate");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- Callback Definition   ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- End Of RSSpecAn K15 - Avionics Measurement ------------------------------*
 *****************************************************************************/
