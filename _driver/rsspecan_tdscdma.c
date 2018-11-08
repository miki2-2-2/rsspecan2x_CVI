#include "rsspecan.h"

/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  K76 - 77 - TD-SCDMA
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

static ViString resultCDPArr[] = {
	"SLOT", "PDAT", "PD1", "PD2", "PMID", "RHO", "MACC", "PCD", "FERR",
	"CERR", "TFR", "IQOF", "IQIM", "ACT", "SRAT", "CHAN", "SFAC", "CDPR",
	"CDP", "EVMR", "EVMP", "ARCD", "CMAP", "CSL", "MPIC", "MTYP",
	"PSYM", "PTOT", "TOFF",NULL
};
static ViString resultCDPArrFSW[] = {
	"SLOT", "PDAT", "PD1", "PD2", "PMID", "RHO", "MACC", "PCD", "FERR",
	"CERR", "TFR", "IQOF", "IQIM", "ACT", "SRAT", "CHAN", "SFAC", "CDPR",
	"CDP", "EVMR", "EVMP", "ARCD", "CMAP", "CSL", "MPIC", "MTYP",
	"PSYM", "PTOT", "TOFF",NULL
};

/*===========================================================================*/
/* Function: Set TDS BS Mode
/* Purpose:  This function sets the TD-SCDMA BS (BTDScdma) mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetTDSBSMode(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_MODE, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Set TDS UE Mode
/* Purpose:  This function sets the TD–SCDMA MS (MTDScdma) mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetTDSUEMode(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDUE_MODE, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Select TDS Measurement
/* Purpose:  This function selects the measurement of the Application
/*           Firmware R&S FS-K76,K77, TD-SCDMA mobile station test.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SelectTDSMeasurement(ViSession instrSession,
                                                ViInt32 measurement)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_MEAS, measurement),
			2, "Measurement");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the TDS BS capture settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR setCount/Sets the number of sets to be captured and stored in the instrument's
/// HIPAR setCount/memory.
/// HIPAR setValue/Sets the SET for which the results are evaluated.
/// HIPAR captureLength/Specifies the number of slots that are captured by one measurement. If
/// HIPAR captureLength/more than one set is to be captured (Set Count parameter), the number
/// HIPAR captureLength/of slots is automatically set to the maximum of 64.
/// HIPAR invertQ/Inverts the sign of the signal Q component.
ViStatus _VI_FUNC rsspecan_ConfigureTDSSignalCapture(ViSession instrSession,
                                                     ViBoolean invertQ,
                                                     ViInt32 setCount,
                                                     ViInt32 setValue,
                                                     ViInt32 captureLength)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_INVERT_Q, invertQ),
			2, "Invert Q");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SET_COUNT, setCount),
			3, "Set Count");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SET_VALUE, setValue),
			4, "Set Value");

	if (setCount == 1)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CAPTURE_LENGTH, captureLength),
				5, "Capture Length");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function defines which channel, slot or set is evaluated in the
/// HIFN  result display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR channel/Sets the code number.
/// HIPAR slot/Sets the slot.
ViStatus _VI_FUNC rsspecan_ConfigureTDSEvaluationRange(ViSession instrSession,
                                                       ViInt32 channel,
                                                       ViInt32 slot)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SEL_CHAN, channel),
			2, "Channel");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SEL_SLOT, slot),
			3, "Slot");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Adapt To Signal
/* Purpose:  This function configures the gated sweep mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSAdaptToSignal(ViSession instrSession,
                                                       ViInt32 startSlot,
                                                       ViInt32 stopSlot)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_ADAPT_SIGN_SLOT_START, startSlot),
			2, "Start Slot");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_ADAPT_SIGN_SLOT_STOP, stopSlot),
			3, "Stop Slot");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function automatically adjusts the reference level and the
/// HIFN  trigger offset to subframe start to their optimum levels for the
/// HIFN  current signal. This prevents overloading the R&S FSW.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR timeout/This control sets the timeout value.
ViStatus _VI_FUNC rsspecan_TDSAdaptSignalAutoLevelTime(ViSession instrSession,
                                                       ViInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_ADAPT_SIGN_AUT_LEVEL_TIME, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures synchronization of channels in the input
/// HIFN  signal to detect timing off-sets in the slot spacings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/Sets the scrambling code of the base station.
/// HIPAR midambleShiftCells/instrument session.
/// HIPAR scramblingCode/Sets the maximum number of midamble shifts.
/// HIPAR syncTo/Defines the phase reference. For a successful synchronization, the
/// HIPAR syncTo/selected slot must contain at least one data channel with sufficient
/// HIPAR syncTo/power. Not available for Power vs Time measurements.
/// HIPAR rotateCodeChannels/Allows phase rotations between code channels.
ViStatus _VI_FUNC rsspecan_ConfigureTDSSynchronization(ViSession instrSession,
                                                       ViInt32 scramblingCode,
                                                       ViInt32 midambleShiftCells,
                                                       ViInt32 syncTo,
                                                       ViBoolean rotateCodeChannels)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_SCODE, scramblingCode),
			2, "scramblingCode");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CELL_SHIFTS, midambleShiftCells),
			3, "Midamble Shift Cells");

	viCheckParm(rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDBS_SYNC_TO_SLOT,
		(ViBoolean) syncTo), 4, "Sync To");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_SYNC_TO_SLOT_ROTATE, rotateCodeChannels),
			5, "Rotate Code Channels");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function selects the slot synchronization mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR syncToSlotMode/Selects the phase reference to be used.
ViStatus _VI_FUNC rsspecan_ConfigureTDSUESyncToSlotMode(ViSession instrSession,
                                                        ViInt32 syncToSlotMode)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_STS_MODE, syncToSlotMode),
			2, "Sync To Slot Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the minimum power that a single channel must
/// HIFN  have compared to the total signal in order to be recognized as an
/// HIFN  active channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR inactiveChannelTreshold/Sets the threshold above which a channel is regarded as active. The
/// HIPAR inactiveChannelTreshold/level refers to total signal power.
ViStatus _VI_FUNC rsspecan_ConfigureTDSInactiveChannelThreshold(ViSession instrSession,
                                                                ViReal64 inactiveChannelTreshold)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TDBS_INACT_CHAN_THR, inactiveChannelTreshold),
			2, "InactiveChannel Treshold");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the TDS BS code domain analyzer settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR normalize/Switches elimination of IQ offset on or off.
/// HIPAR codePowerDisplay/Switches between showing the absolute or relative power.
ViStatus _VI_FUNC rsspecan_ConfigureTDSCodeDomainSettings(ViSession instrSession,
                                                          ViBoolean normalize,
                                                          ViInt32 codePowerDisplay)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_NORMALIZE, normalize),
			2, "Normalize");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_POWER_DISPLAY, codePowerDisplay),
			3, "Code Power Display");

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
/// HIPAR YIGPreselector/This control turns the YIG-preselector on and off.
ViStatus _VI_FUNC rsspecan_ConfigureTDSInput(ViSession instrSession,
                                             ViInt32 inputCoupling,
                                             ViInt32 impedance,
                                             ViBoolean highpassFilter,
                                             ViBoolean YIGPreselector)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AMPL_RF_COUPLING, inputCoupling),
			2, "Input Coupling");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_INPUT_IMPEDANCE, impedance),
			3, "Impedance");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER, highpassFilter),
			4, "Highpass Filter");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_YIG_FILTER, YIGPreselector),
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
ViStatus _VI_FUNC rsspecan_ConfigureTDSReferenceLevel(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViReal64 referenceLevel,
                                                      ViReal64 referenceLevelOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel),
			3, "Rreference Level");

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
ViStatus _VI_FUNC rsspecan_ConfigureTDSMechanicalAttenuator(ViSession instrSession,
                                                            ViBoolean autoMode,
                                                            ViReal64 value)
{
	ViStatus error = VI_SUCCESS;

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
/// HIPAR manualValuedB/This control sets the manual input electronic attenuation level.
ViStatus _VI_FUNC rsspecan_ConfigureTDSElectronicAttenuator(ViSession instrSession,
                                                            ViBoolean state,
                                                            ViBoolean electronicAttenuatorAuto,
                                                            ViInt32 manualValuedB)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_EATT_STATE, state),
			2, "State");

	if (state)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_EATT_AUTO, electronicAttenuatorAuto),
				3, "Electronic Attenuator Auto");
		if (!electronicAttenuatorAuto)
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_AMPL_EATT_VALUE, manualValuedB),
					3, "Manual Value dB");
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the preamplifier for the instrument.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/Switches on or off the preamplifier for the instrument.
/// HIPAR preamplifierLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureTDSPreamplifier(ViSession instrSession,
                                                    ViBoolean state,
                                                    ViInt32 preamplifierLevel)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_AMPL_PREAMPLIFIER, state),
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

/// HIFN  This function configures displayed range using minimum and maximum
/// HIFN  values.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR maximumValue/Defines the minimum value of the y-axis for the selected result
/// HIPAR maximumValue/display.
/// HIPAR minimumValue/Defines the maximum value of the y-axis for the selected result
/// HIPAR minimumValue/display.
ViStatus _VI_FUNC rsspecan_ConfigureTDSYAxisScalingMaxMin(ViSession instrSession,
                                                          ViReal64 maximumValue,
                                                          ViReal64 minimumValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_TDBS_VERTICAL_SCALE_MAX_MIN, maximumValue),
			2, "Maximum Value");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_TDBS_VERTICAL_SCALE_MAX_MIN, minimumValue),
			3, "minimum Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: TDS BS Channel Table Operations
/* Purpose:  This function performs the selected operation with channel
/*           table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TDSBSChannelTableOperations(ViSession instrSession,
                                                       ViInt32 operation,
                                                       ViString channelTable)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, operation, RSSPECAN_VAL_TDS_TABLE_OFF, RSSPECAN_VAL_TDS_TABLE_COPY),
			2, "Operation");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, channelTable), 3, "Channel Table");

	switch (operation)
	{
	case RSSPECAN_VAL_TDS_TABLE_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_STATE, VI_FALSE));
		break;
	case RSSPECAN_VAL_TDS_TABLE_SELECT:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_STATE, VI_TRUE));
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_SEL, channelTable),
				3, "Channel Table");
		break;
	case RSSPECAN_VAL_TDS_TABLE_NEW:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_NAME, channelTable),
				3, "Channel Table");
		break;
	case RSSPECAN_VAL_TDS_TABLE_DELETE:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_DEL, channelTable),
				3, "Channel Table");
		break;
	case RSSPECAN_VAL_TDS_TABLE_COPY:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_COPY, channelTable),
				3, "Channel Table");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Channel Table Comment
/* Purpose:  This function defines a comment for the selected channel table
/*           in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableComment(ViSession instrSession,
                                                             ViString channelTableComment)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_COMM, channelTableComment),
			2, "Channel Table Comment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Channel Table Data
/* Purpose:  This function defines a channel table. The whole table is
/*           defined in one operation. The inactive channels (INACtive) do
/*           not have to be defined.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableData(ViSession instrSession,
                                                          ViInt32 arraySize,
                                                          ViInt32 channelType[],
                                                          ViInt32 codeClass[],
                                                          ViInt32 codeNumber[],
                                                          ViInt32 modulationType[],
                                                          ViInt32 midambleShift[],
                                                          ViBoolean status[],
                                                          ViInt32 reserved1[],
                                                          ViInt32 reserved2[])
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuffer = cmd;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 1, INT_MAX),
			2, "Array Size");

	pbuffer += sprintf(pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,%ld,%d,%ld,%ld",
	                   channelType[i], codeClass[i], codeNumber[i], modulationType[i], midambleShift[i], status[i],
	                   reserved1[i], reserved2[i]);
	for (i = 1; i < arraySize; i++)
		pbuffer += sprintf(pbuffer, ",%ld,%ld,%ld,%ld,%ld,%d,%ld,%ld",
		                   channelType[i], codeClass[i], codeNumber[i], modulationType[i], midambleShift[i], status[i],
		                   reserved1[i], reserved2[i]);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Channel Table Order
/* Purpose:  This function selects sorting of the channel table in code
/*           order or midamble order.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableOrder(ViSession instrSession,
                                                           ViInt32 order)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_ORDER, order),
			2, "Order");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Channel Table Midamble Shift
/* Purpose:  This function defines the maximum number of midamble shifts in
/*           the channel table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableMidambleShift(ViSession instrSession,
                                                                   ViInt32 maximumofMidableShifts)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_MSHIFT, maximumofMidableShifts),
			2, "Maximum of Midable Shifts");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines the highest modulation to be considered in
/// HIFN the automatic channel search. In low SNR environments it may be
/// HIFN necessary to limit the channel search to lower modulations than
/// HIFN 64QAM.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR highestModulation/Defines the highest modulation to be considered in the automatic channel search. In low SNR environments it may be necessary to limit the channel search to lower modulations than 64QAM.
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableMaxModulation(ViSession instrSession,
                                                                   ViInt32 highestModulation)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_MMAX, highestModulation),
			2, "Highest Modulation");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Get TDS BS Channel Table Catalog
/* Purpose:  This function reads out the names of all channel tables stored
/*           on the harddisk in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetTDSBSChannelTableCatalog(ViSession instrSession,
                                                       ViInt32* numberofChannelTables,
                                                       ViInt32 bufferSize,
                                                       ViChar channelTablesList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
			3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, channelTablesList), 4, "Channel Table List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, ":CONF:CDP:CTAB:CAT?", &buf));
	checkErr(RsCore_ParseCatalog(buf, bufferSize, channelTablesList, numberofChannelTables));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the TDS BS Power Vs. Time measurement
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR switchingPoint/Sets the switching point between uplink and downlink slots.
/// HIPAR subframes/Sets the number of subframes for averaging.
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSPowerVsTime(ViSession instrSession,
                                                     ViInt32 switchingPoint,
                                                     ViInt32 subframes)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_MEAS_PVT_SWITCH_POINT, switchingPoint),
			2, "Switching Point");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_MEAS_PVT_SFRAMES, subframes),
			3, "Subframes");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function returns the list evaluation results for Power vs Time
/// HIFN  measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR timeout/This control sets the timeout value.
/// HIPAR results/Returns the list evaluation results for Power vs Time measurements.
ViStatus _VI_FUNC rsspecan_GetTDSBSPowerVsTimeResult(ViSession instrSession,
                                                     ViInt32 timeout,
                                                     ViReal64* results)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, "CONF:CDP:PVT:LIST:RES?", timeout, 10, results, NULL));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Get TDS BS Result
/* Purpose:  This function adjusts the refernce level and returns the result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetTDSBSResult(ViSession instrSession,
                                          ViInt32 type,
                                          ViInt32 timeout,
                                          ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_TDS_RESULT_SLOT, RSSPECAN_VAL_TDS_RESULT_ARCDE),
			2, "Type");
	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, result), 4, "Result");
	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:MARK:FUNC:CDP:RES? %s", resultCDPArr[type]);
	checkErr(RsCore_QueryViReal64WithOpc(instrSession, cmd, timeout, result));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Get TDS UE Result
/* Purpose:  This function adjusts the refernce level and returns the
/*           result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetTDSUEResult(ViSession instrSession,
                                          ViInt32 type,
                                          ViUInt32 timeout,
                                          ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_TDS_RESULT_SLOT, RSSPECAN_VAL_TDS_RESULT_TOFFSET),
			2, "Type");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, result), 4, "Result");

	if (!RsCore_IsInstrumentModel(instrSession, "FSW"))
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:MARK:FUNC:CDP:RES? %s", resultCDPArr[type]);
	}
	else
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:MARK:FUNC:CDP:RES? %s", resultCDPArrFSW[type]);
	}

	checkErr(RsCore_QueryViReal64WithOpc (instrSession, cmd, timeout, result));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch TDS BS Trace
/* Purpose:  This function returns the measured values of TDS BS
/*           measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchTDSBSTrace(ViSession instrSession,
                                           ViInt32 trace,
                                           ViInt32 arrayLength,
                                           ViInt32* actualPoints,
                                           ViReal64 values[])
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 2),
			2, "Trace");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, actualPoints), 4, "Actual Points");

	sprintf(traceName, "TRACE%ld", trace);
	checkErr(rsspecan_dataReadTrace(instrSession, 0, traceName, arrayLength, values, actualPoints));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch TDS UE Trace
/* Purpose:  This function returns the measured values of TDS BS
/*           measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchTDSUETrace(ViSession instrSession,
                                           ViInt32 trace,
                                           ViInt32 arrayLength,
                                           ViInt32* actualPoints,
                                           ViReal64 values[])
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K76|K77"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 2),
			2, "Trace");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, actualPoints), 4, "Actual Points");

	sprintf(traceName, "TRACE%ld", trace);
	checkErr(rsspecan_dataReadTrace(instrSession, 0, traceName, arrayLength, values, actualPoints));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Results
/* Purpose:  This function configures the TDS BS results
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSResults(ViSession instrSession,
                                                 ViInt32 resultType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (resultType)
	{
	case RSSPECAN_VAL_TDS_RES_CDP:
	case RSSPECAN_VAL_TDS_RES_CDPRAT:
	case RSSPECAN_VAL_TDS_RES_CDEP:
	case RSSPECAN_VAL_TDS_RES_CTAB:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_TDBS_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	case RSSPECAN_VAL_TDS_RES_MACC:
	case RSSPECAN_VAL_TDS_RES_PVSLA:
	case RSSPECAN_VAL_TDS_RES_PVSLR:
	case RSSPECAN_VAL_TDS_RES_PVSYM:
	case RSSPECAN_VAL_TDS_RES_BSTR:
	case RSSPECAN_VAL_TDS_RES_SUM:
	case RSSPECAN_VAL_TDS_RES_PCD:
	case RSSPECAN_VAL_TDS_RES_SYMBCONST:
	case RSSPECAN_VAL_TDS_RES_SYMBEVM:
	case RSSPECAN_VAL_TDS_RES_COMPCONST:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_TDBS_RESULT_TYPE, resultType),
				2, "Result Type");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 2, "Result Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 *- End Of RSSpecAn TD-SCDMA ------------------------------------------------*
 *****************************************************************************/
