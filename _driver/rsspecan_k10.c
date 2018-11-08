/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  FSV Rohde&Schwarz Spectrum Analyzer
 *
 *  Original Release: June 2009
 *  By: Michal Janik
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"

/*****************************************************************************
 *- Value Definition and Range Tables ---------------------------------------*
 *****************************************************************************/
static ViString absRelArr[] = {"ABS", "REL", NULL};
static ViString statusArr[] = {"PASSED", "FAILED", "MARGIN", "EXC", NULL};

static ViString GSMBurstTypeArr[] = {
	"GSMBurstMACC", "GSMBurstPFER",
	"GSMBurstETIM", "GSMBurstMERR",
	"GSMBurstCONS", "GSMBurstPTEM",NULL
};
static ViString GSMSpectrumTypeArr[] = {"GSMSpectMOD", "GSMSpectSWIT",NULL};
static ViString GSMMeasTypeArr[] = {
	"GSMMtRMS", "GSMMtPEAK", "GSMMtMERRRMS", "GSMMtMERRPEAK",
	"GSMMtPERRRMS", "GSMMtPERRPEAK", "GSMMtOSUP", "GSMMtIQOF",
	"GSMMtIQIM", "GSMMtFREQ", "GSMMtBPOW", "GSMMtADR",NULL
};
static ViString GSMMeasModifArr[] = {"GSMMmAVER", "GSMMmCURR", "GSMMmMAX", "GSMMmSDEV",NULL};
static ViString GSMPercentileArr[] = {"GSMPercEVM", "GSMPercMERR", "GSMPercPERR",NULL};
static ViString GSMBurstSelArr[] = {"GSMBSAll", "GSMBSCurr", NULL};
static ViString GSMBurstModifArr[] = {"GSMBMAver", "GSMBMCrest", "GSMBMMax",NULL};
static ViInt32 GSMBandArr[14][2] = {
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_TGSM, 380},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_TGSM, 410},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_GSM, 450},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_GSM, 480},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_GSM, 710},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_GSM, 750},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_TGSM, 810},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_GSM, 850},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_PGSM, 900},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_EGSM, 900},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_RGSM, 900},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_TGSM, 900},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_DCS, 1800},
	{RSSPECAN_VAL_GSM_K10_BAND_CLASS_PCS, 1900}
};
static ViString RBWArr[] = {"30000", "100000", NULL};
static ViString GSMMAgnitudeResultArr[] = {"SCOP", "MEAS", NULL};

/*===========================================================================*/
/* Function: GSM K10 Mode
/* Purpose:  This function selects the GSM/EDGE analyzer for mobile and base
/*           station tests.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GSMK10Mode(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession))
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODE, NULL));
	}
	else
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODE_FSV, NULL));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 DUT
/* Purpose:  This function specifies the type of device to be measured.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DUT(
	ViSession instrSession,
	ViInt32 deviceUnderTest
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_DUT, deviceUnderTest),
			2, "Device Under Test");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Signal Characteristics
/* Purpose:  This function configures the signal characteristics.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10SignalCharacteristics(
	ViSession instrSession,
	ViInt32 bandClass,
	ViInt32 arfcn
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bandClass, RSSPECAN_VAL_GSM_K10_FREQ_BAND_TGSM_380, RSSPECAN_VAL_GSM_K10_FREQ_BAND_PCS_1900),
			2, "Band Class");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_SIG_CHAR_MODE, GSMBandArr[bandClass][0]),
			2, "Band Class");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_SIG_CHAR_BAND, GSMBandArr[bandClass][1]),
			2, "Band Class");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_SIG_CHAR_BAND_ARFC, arfcn),
			3, "ARFCN");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Level Settings
/* Purpose:  This function configures the level settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10LevelSettings(
	ViSession instrSession,
	ViInt32 automaticReferenceLevel,
	ViReal64 referenceLevel,
	ViReal64 externalAttenuation,
	ViInt32 powerClass,
	ViInt32 staticPCL
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (automaticReferenceLevel)
	{
	case RSSPECAN_VAL_GSM_K10_AUTO_OFF:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_PWR_LEV, VI_FALSE),
				2, "Automatic Reference Level");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel),
				3, "Reference Level");
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ONCE:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_PWR_LEV_ONCE, ""),
				2, "Automatic Reference Level");
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ON:
	default:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_PWR_LEV, VI_TRUE),
				2, "Automatic Reference Level");
	}

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_PWR_LEV, automaticReferenceLevel > 0),
		2, "Automatic Reference Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, externalAttenuation),
			4, "External Attenuation");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_LEVEL_SETTINGS_PWR_CLASS, powerClass),
			5, "Power Class");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_LEVEL_SETTINGS_POW_STAT, staticPCL),
			6, "Static PCL");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Capture Settings
/* Purpose:  This function configures the capture settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10CaptureSettings(
	ViSession instrSession,
	ViInt32 signalSource,
	ViReal64 captureTime,
	ViInt32 triggerMode,
	ViReal64 triggerOffset,
	ViReal64 triggerLevel,
	ViInt32 statisticCount
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_SELECTION, signalSource),
			2, "Signal Source");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_TIME, captureTime),
			3, "Capture Time");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_SOURCE, triggerMode),
			4, "Trigger Mode");

	switch (triggerMode)
	{
	case RSSPECAN_VAL_TRG_IFP:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_IFP_LEVEL, triggerLevel),
				6, "Trigger Level");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, triggerOffset),
				5, "Trigger Offset");

		break;
	case RSSPECAN_VAL_TRG_EXT:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, triggerLevel),
				6, "Trigger Level");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, triggerOffset),
				5, "Trigger Offset");

		break;
	}
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, statisticCount),
			7, "Statistic Count");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function returns the current Statistic Count.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR statisticCount/Returns the current Statistic Count.
ViStatus _VI_FUNC rsspecan_QueryGSMK10StatisticCount(ViSession instrSession,
                                                     ViInt32* statisticCount)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_STATISTIC_COUNT, statisticCount),
			2, "Statistic Count");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function returns the currently reached number of data
/// HIFN  acquisitions that contribute to the Trigger to Sync result. It
/// HIFN  can be used to track the progress of the averaging process until
/// HIFN  it reaches the set "Statistic Count"
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR numberOfAcquisitions/Returns the currently reached number of data acquisitions that
/// HIPAR numberOfAcquisitions/contribute to the Trigger to Sync result.
ViStatus _VI_FUNC rsspecan_QueryGSMK10TriggerToSyncNumberOfAcquisitions(ViSession instrSession,
                                                                        ViInt32* numberOfAcquisitions)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_TRIGGER_TO_SYNC_ACQUISITIONS, numberOfAcquisitions),
			2, "Number Of Acquisitions");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Sets the IQ correlation threshold. The IQ correlation threshold
/// HIFN decides whether a burst is accepted if "Measure only on Sync"
/// HIFN is activated. If the correlation value between the ideal IQ
/// HIFN signal of the given TSC and the measured TSC is below the IQ
/// HIFN correlation threshold, then the application reports "Sync not
/// HIFN found" in the status bar. Additionally, such bursts are ignored
/// HIFN if "Measure only on Sync"  is activated.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR correlationThreshold/Sets the IQ correlation threshold. The IQ correlation threshold decides whether a burst is accepted if "Measure only on Sync"  is activated. If the correlation value between the ideal IQ signal of the given TSC and the measured TSC is below the IQ correlation threshold, then the application reports "Sync not found" in the status bar. Additionally, such bursts are ignored if "Measure only on Sync"  is activated.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10CorrelationThreshold(ViSession instrSession,
                                                               ViReal64 correlationThreshold)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_GSM_K10_CORRELATION_THRESHOLD, correlationThreshold),
			2, "Correlation Threshold");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Synchronization
/* Purpose:  This function configures the synchronization settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Synchronization(ViSession instrSession,
                                                          ViInt32 synchronization,
                                                          ViBoolean measureOnlyOnSync)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_SYNCHRONIZATION, synchronization),
			2, "Synchronization");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MEASURE_ONLY_ON_SYNC, measureOnlyOnSync),
			3, "Measure Only On Sync");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Auto Track Time
/* Purpose:  This function configures the capture settings for RF input in
/*           more detail.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AutoTrackTime(
	ViSession instrSession,
	ViReal64 autoTrackTime
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_GSM_K10_CAP_SET_TRACK_TIME, autoTrackTime),
			2, "Auto Track Time");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Specifies if the IQ data shall be swapped. Swapping I and Q
/// HIFN inverts the sideband. Try this function if the TSC can not be
/// HIFN found.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR swapIQ/Specifies if the IQ data shall be swapped. Swapping I and Q inverts the sideband. Try this function if the TSC can not be found.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10SwapIQ(ViSession instrSession,
                                                 ViBoolean swapIQ)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_IQ_SWAP, swapIQ),
			2, "Swap IQ");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines the ratio to be zoomed around the marker 1.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR zoomFactor/Defines the ratio to be zoomed around the marker 1.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10MarkerZoom(ViSession instrSession,
                                                     ViInt32 zoomFactor)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MARKER_ZOOM_FACTOR, zoomFactor),
			2, "Zoom Factor");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Selects how the modulation and transient spectrum measurement is
/// HIFN performed and displayed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR resultDisplay/Selects how the modulation and transient spectrum measurement is performed and displayed.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10ResultDisplay(ViSession instrSession,
                                                        ViInt32 resultDisplay)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_RESULT_DISPLAY, resultDisplay),
			2, "Result Display");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Auto Settings
/* Purpose:  This function configures the parameters to be set
/*           automatically.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AutoSettings(
	ViSession instrSession,
	ViInt32 level,
	ViInt32 frameConfiguration,
	ViInt32 trigger
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (level)
	{
	case RSSPECAN_VAL_GSM_K10_AUTO_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_LEVEL, VI_FALSE));
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ON:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_LEVEL, VI_TRUE));
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ONCE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_LEVEL_ONCE, ""));
		break;
	default:
		viCheckParm(VI_ERROR_PARAMETER2, 2, "Level");
	}
	switch (frameConfiguration)
	{
	case RSSPECAN_VAL_GSM_K10_AUTO_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_FRAME, VI_FALSE));
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ON:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_FRAME, VI_TRUE));
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ONCE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_FRAME_ONCE, ""));
		break;
	default:
		viCheckParm(VI_ERROR_PARAMETER3, 3, "Frame Configuration");
	}
	switch (trigger)
	{
	case RSSPECAN_VAL_GSM_K10_AUTO_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_TRIGGER, VI_FALSE));
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ON:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_TRIGGER, VI_TRUE));
		break;
	case RSSPECAN_VAL_GSM_K10_AUTO_ONCE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_AUTO_SET_TRIGGER_ONCE, ""));
		break;
	default:
		viCheckParm(VI_ERROR_PARAMETER2, 4, "Trigger");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Demodulation Single Slot Measurements
/* Purpose:  This function configures the slot to be measured in single-slot
/*           measurements relative to the GSM frame boundary.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DemodulationSingleSlotMeasurements(
	ViSession instrSession,
	ViInt32 slotToMeasure
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_DEMOD_SINGLE_SLOT_MEAS, slotToMeasure),
			2, "Slot To Measure");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Demodulation Multi Slot Measurements
/* Purpose:  This function configures the parameters for the measurement
/*           interval for multi-slot measurements.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DemodulationMultiSlotMeasurements(
	ViSession instrSession,
	ViInt32 numberOfSlotsToMeasure,
	ViInt32 firstSlotToMeasure
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_DEM_MULTI_SLOT_MEAS_SLOT_NUM, numberOfSlotsToMeasure),
			2, "Number Of Slots To Measure");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_DEM_MULTI_SLOT_MEAS_FIRST_SLOT, firstSlotToMeasure),
			3, "First Slot To Measure");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Demodulation Equal Timeslot Length
/* Purpose:  This function if activated, all slots of a frame have the same
/*           length (8 x 156.26 normal symbol periods). If deactivated, slots
/*           number 0 and 4 of a frame have a longer duration, all other a
/*           shorter duration compared to the "equal slot length" (157, 156,
/*           156, 156, 157, 156, 156, 156 normal symbol periods).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DemodulationEqualTimeslotLength(ViSession instrSession,
                                                                          ViBoolean equalTimeslotLength)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_DEMODULATION_EQUAL_TIMESLOT_LENGTH, equalTimeslotLength),
			2, "Equal Timeslot Length");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Burst
/* Purpose:  This function configures a burst at a single slot.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Burst(
	ViSession instrSession,
	ViInt32 slot,
	ViBoolean active,
	ViInt32 burstType,
	ViInt32 pcl,
	ViInt32 modulation,
	ViInt32 filter,
	ViInt32 tsc
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			3, "Slot");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld", slot);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SLOT_STATE, active),
			3, "Active");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TYPE_BURST, burstType),
			4, "Burst Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_PCL, pcl),
			5, "PCL");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_MOD, modulation),
			6, "Modulation");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TSC, tsc),
			8, "TSC");
	switch (burstType)
	{
	case RSSPECAN_VAL_GSM_K10_BURST_NORMAL:
		break;
	case RSSPECAN_VAL_GSM_K10_BURST_HIGHER_RATE:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_FILTER, filter),
				7, "Filter");
		break;
	case RSSPECAN_VAL_GSM_K10_BURST_ACCESS:
		if (modulation != RSSPECAN_VAL_GSM_K10_MOD_GMSK)
		{
			viCheckParm(RS_ERROR_INVALID_PARAMETER, 6, "Modulation");
		}
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Sets the bits of the user definable TSC. The number of bits must
/// HIFN be in accordance with the defined burst type and modulation.
/// HIFN "CONFigure:MS:CHANnel:SLOT0:TSC USER" must be defined first.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR userTSC/Sets the bits of the user definable TSC
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10UserTSC(ViSession instrSession,
                                                  ViInt32 slot,
                                                  ViString userTSC)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			2, "Slot");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld", slot);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TSC_USER_VALUE, userTSC),
			3, "User TSC");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures a burst at a single slot.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR slot/Selects the slot.
/// HIPAR active/Activates or deactivates selected slot.
/// HIPAR timingAdvance/This control sets the timing advance.
/// HIPAR sync/Selects the training (synchronization) sequence TS.
/// HIPAR userTSC/Sets the bits of the user definable TSC. The number of bits must
/// HIPAR userTSC/be in accordance with the defined burst type and modulation.
/// HIPAR userTSC/"CONFigure:MS:CHANnel:SLOT0:TSC USER" must be defined first.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AccessBurst(ViSession instrSession,
                                                      ViInt32 slot,
                                                      ViBoolean active,
                                                      ViInt32 timingAdvance,
                                                      ViInt32 sync,
                                                      ViString userTSC)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			2, "Slot");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld", slot);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SLOT_STATE, active),
			3, "Active");

	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TYPE_BURST, RSSPECAN_VAL_GSM_K10_BURST_ACCESS));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TIMING_ADVANCE, timingAdvance),
			4, "Timing Advance");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SYNC, sync),
			5, "Sync");

	if (sync == RSSPECAN_VAL_SYNC_USER)
	{
		viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TSC_USER_VALUE, userTSC),
				6, "User TSC");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures a burst for AQPSK modulation with Normal burst type at a single slot.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR slot/Selects the slot.
/// HIPAR active/Activates or deactivates selected slot.
/// HIPAR SCPIR/This control sets the Subchannel Power Imbalance Ratio (SCPIR).
/// HIPAR subchannel1TSC/Selects the training sequence for subchannel 1 used by the mobile or base station.
/// HIPAR subchannel1UserTSC/Sets the bits of the user definable TSC for subchannel 1. The
/// HIPAR subchannel1UserTSC/number of bits must be in accordance with the defined burst type
/// HIPAR subchannel1UserTSC/and modulation. "CONFigure:MS:CHANnel:SLOT0:SUBChannel1:TSC
/// HIPAR subchannel1UserTSC/USER" must be defined first.
/// HIPAR subchannel2TSC/Selects the training sequence for subchannel 2 used by the mobile or base station.
/// HIPAR subchannel2UserTSC/Sets the bits of the user definable TSC for subchannel 2. The
/// HIPAR subchannel2UserTSC/number of bits must be in accordance with the defined burst type
/// HIPAR subchannel2UserTSC/and modulation. "CONFigure:MS:CHANnel:SLOT0:SUBChannel2:TSC
/// HIPAR subchannel2UserTSC/USER" must be defined first.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AQPSKBurst(ViSession instrSession,
                                                     ViInt32 slot,
                                                     ViBoolean active,
                                                     ViReal64 SCPIR,
                                                     ViInt32 subchannel1TSC,
                                                     ViString subchannel1UserTSC,
                                                     ViInt32 subchannel2TSC,
                                                     ViString subchannel2UserTSC)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViChar repCap2[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			2, "Slot");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld", slot);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SLOT_STATE, active),
			3, "Active");

	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_TYPE_BURST, RSSPECAN_VAL_GSM_K10_BURST_NORMAL));
	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_MOD, RSSPECAN_VAL_GSM_K10_MOD_AQPSK));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SCPIR, SCPIR),
			4, "SCPIR");

	strcpy(repCap2, repCap);
	strcat(repCap, ",Sub1");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SUBCH_TSC, subchannel1TSC),
			5, "Subchannel 1 TSC");

	if (subchannel1TSC == RSSPECAN_VAL_TSC_SLOT_USER)
	{
		viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_BURST_SUBCH_TSC_USER_VALUE, subchannel1UserTSC),
				6, "Subchannel 1 User TSC");
	}

	strcat(repCap2, ",Sub2");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap2, RSSPECAN_ATTR_GSM_K10_BURST_SUBCH_TSC, subchannel2TSC),
			7, "Subchannel 2 TSC");

	if (subchannel2TSC == RSSPECAN_VAL_TSC_SLOT_USER)
	{
		viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap2, RSSPECAN_ATTR_GSM_K10_BURST_SUBCH_TSC_USER_VALUE, subchannel2UserTSC),
				8, "Subchannel 2 User TSC");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures the parameters for the Trigger to Sync measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR numberOfBins/This parameter specifies the number of bins for the histogram of
/// HIPAR numberOfBins/the "Trigger to Sync" measurement.
/// HIPAR adaptiveDataSize/This parameter spcifies the number of measurements (I/Q captures)
/// HIPAR adaptiveDataSize/after which the x-axis is fixed for the histogram calculation.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10TriggerToSync(ViSession instrSession,
                                                        ViInt32 numberOfBins,
                                                        ViInt32 adaptiveDataSize)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_TRIGGER_TO_SYNC_BINS_COUNT, numberOfBins),
			2, "Number Of Bins");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_TRIGGER_TO_SYNC_ADAPTIVE_DATA_SIZE, adaptiveDataSize),
			3, "Adaptive Data Size");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN Selects whether the list results (power and limit values) of the "Wide Modulation
/// HIFN Spectrum" measurement are returned in a relative (dB) or absolute (dBm) unit.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR unit/Selects the results unit.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10WideSpectrumResultsUnit(ViSession instrSession,
                                                                  ViInt32 unit)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_WIDE_SPECTRUM_UNIT, unit),
			2, "Unit");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures whether a trace is displayed or not, and in which mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace.
/// HIPAR traceType/The type of trace to take.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Trace(ViSession instrSession,
                                                ViInt32 window, ViInt32 trace,
                                                ViInt32 traceType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 4),
			3, "Trace");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_TRACE_TYPE, traceType),
			4, "Trace Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Measurement
/* Purpose:  This function configures the GSM Measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Measurement(
	ViSession instrSession,
	ViInt32 measurementMode
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementMode)
	{
	case RSSPECAN_VAL_GSM_K10_MACC: // (0) - Modulation Accuracy
	case RSSPECAN_VAL_GSM_K10_PFER: // (1) - Phase Error vs Time
	case RSSPECAN_VAL_GSM_K10_ETIM: // (2) - EVM vs Time
	case RSSPECAN_VAL_GSM_K10_MERR: // (3) - Magnitude Error
	case RSSPECAN_VAL_GSM_K10_CONST: // (4) - Constellation
	case RSSPECAN_VAL_GSM_K10_PTEM: // (5) - Power vs. Time

		snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s", GSMBurstTypeArr[measurementMode]);

		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_MEAS_BURST_TYPE, NULL));
		break;
	case RSSPECAN_VAL_GSM_K10_MOD: // (6) - Modulation Spectrum
	case RSSPECAN_VAL_GSM_K10_SWIT: // (7) - Transient Spectrum

		snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s", GSMSpectrumTypeArr[measurementMode - 6]);

		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_GSM_K10_MEAS_SPECT_TYPE, NULL));
		break;
	case RSSPECAN_VAL_GSM_K10_WIDE: // (8) - Wide Spectrum
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_MEASUREMENT_WIDE_SPECTRUM, NULL),
				2, "GSM K10 Measurement Wide Spectrum");
		break;
	case RSSPECAN_VAL_GSM_K10_TS: // (9) - Trigger to sync
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_MEASUREMENT_TRIGGER_TO_SYNC, NULL),
				2, "GSM K10 Measurement Trigger to Sync");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Power vs Time Measurement
/* Purpose:  This function configures the burst section to be measured.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGsmK10PVTMeasurement(
	ViSession instrSession,
	ViInt32 burstSection
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_PWR_VS_TIME_MEAS, burstSection),
			2, "Burst Section");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Power vs Time Filter
/* Purpose:  This function configures the filter which shall be used for the
/*           Power vs Time measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10PVTFilter(ViSession instrSession,
                                                    ViInt32 powerVsTimeFilter)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_POWER_VS_TIME_FILTER, powerVsTimeFilter),
			2, "Power Vs Time Filter");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Controls the time-alignment of the limit lines for the "Power vs Time" measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR alignment/Controls the time-alignment of the limit lines for the "Power vs Time" measurement.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10PVTAlignment(ViSession instrSession,
                                                       ViInt32 alignment)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_POWER_VS_TIME_ALIGNMENT, alignment),
			2, "Alignment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures the Symbol Decision and Tail & TSC Bits from Advanced Measurement Settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR symbolDecision/Determines how the symbols are detected in the demodulator.
/// HIPAR tailTSCBits/Selects whether the detected Tail and TSC bits or the standard bits are used to construct the ideal signal.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Demodulation(ViSession instrSession,
                                                       ViInt32 symbolDecision,
                                                       ViInt32 tailTSCBits)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_DEMODULATION_SYMBOL_DECISION, symbolDecision),
			2, "Symbol Decision");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_DEMODULATION_TAIL_TSC_BITS, tailTSCBits),
			3, "Tail TSC Bits");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures whether several measurement results can
/// HIFN be calculated on the same captured I/Q data in parallel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR multipleMeasurementMode/This control activates the multiple measurement mode.
/// HIPAR powerVsTime/This control switches the calculation of the (graph and list) results of the "Power vs Time" measurement.
/// HIPAR demodulation/This control switches on or off the calculation of the results of the Modulation Accuracy,
/// HIPAR demodulation/EVM vs Time, Phase Error vs Time and Magnitude Error vs Time measurements.
/// HIPAR constellation/This control switches on or off the calculation of the results of the "Constellation" measurement.
/// HIPAR modulationSpectrum/This control switches on or off the calculation of the results of the "Modulation Spectrum" measurement.
/// HIPAR transientSpectrum/This control switches on or off the calculation of the results of the "Transient Spectrum" measurement.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10MultiMeasurement(ViSession instrSession,
                                                           ViBoolean multipleMeasurementMode,
                                                           ViBoolean powerVsTime,
                                                           ViBoolean demodulation,
                                                           ViBoolean constellation,
                                                           ViBoolean modulationSpectrum,
                                                           ViBoolean transientSpectrum)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_STATE, multipleMeasurementMode),
			2, "Multiple Measurement Mode");

	if (multipleMeasurementMode == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_POWER_VS_TIME, powerVsTime),
				3, "Power Vs Time");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_DEMODULATION, demodulation),
				4, "Demodulation");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_CONSTELLATION, constellation),
				5, "Constellation");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_MODULATION_SPECTRUM, modulationSpectrum),
				6, "Modulation Spectrum");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_TRANSIENT_SPECTRUM, transientSpectrum),
				7, "Transient Spectrum");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure GSM K10 Modulation Transient Spectrum
/* Purpose:  This function configures the limit check of the spectrum trace.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10ModulationTransientSpectrum(ViSession instrSession,
                                                                      ViBoolean enableLeftLimit,
                                                                      ViBoolean enableRightLimit)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_ENABLE_LEFT_LIMIT, enableLeftLimit),
			2, "Enable Left Limit");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_ENABLE_RIGHT_LIMIT, enableRightLimit),
			3, "Enable Right Limit");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function configures the Modulation and Transient Spectrum measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR filterType/Selects the filter type,
/// HIPAR transientReferencePower/Controls how the reference power of the "Transient Spectrum" measurement is measured.
/// HIPAR offsetFrequency/Offset frequency in Hz.
/// HIPAR modulationRBWAt1800kHz/Controls the resolution bandwidth (RBW) and video bandwidth (VBW) used in
/// HIPAR modulationRBWAt1800kHz/the Modulation Spectrum and Wide Modulation Spectrum measurements at offset
/// HIPAR modulationRBWAt1800kHz/frequencies of +/- 1800 kHz from the carrier.
/// HIPAR highDynamic/When "High Dynamic" is activated, a measurement of the instrument's inherent noise is
/// HIPAR highDynamic/automatically carried out.
/// HIPAR modulationFrequencyList/For Wide Modulation Spectrum measurements, this control sets whether offset
/// HIPAR modulationFrequencyList/frequencies are measured up to 1800 kHz or 5800 kHz.
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10ModulationTransientSpectrumAdditional(ViSession instrSession,
                                                                                ViInt32 filterType,
                                                                                ViInt32 transientReferencePower,
                                                                                ViInt32 offsetFrequency,
                                                                                ViInt32 modulationRBWAt1800kHz,
                                                                                ViBoolean highDynamic,
                                                                                ViInt32 modulationFrequencyList)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_FILTER_TYPE, filterType),
			2, "Filter Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_REF_POWER, transientReferencePower),
			3, "transient Reference Power");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CONF:SPEC:MOD:LIST:BAND:RES %ld,%s", offsetFrequency, RBWArr[modulationRBWAt1800kHz]);
	checkErr(RsCore_Write(instrSession, cmd));
	checkErr(rsspecan_CheckStatus(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_HIGH_DYNAMIC, highDynamic),
			6, "highDynamic");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_FREQUENCY_LIST, modulationFrequencyList),
			7, "Modulation Frequency List");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure GSM K10 Multi Carrier
/* Purpose:  This function configures the settings related to measurements on
/*           multi-carrier base stations.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10MultiCarrier(ViSession instrSession,
                                                       ViBoolean multiCarrierBTS,
                                                       ViInt32 numberOfActiveCarriers,
                                                       ViInt32 BTSClass,
                                                       ViInt32 pvTFilter)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_BTS, multiCarrierBTS),
			2, "Multi Carrier BTS");

	if (multiCarrierBTS == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_NUMBER_OF_ACTIVE_CHANNELS, numberOfActiveCarriers),
				3, "Number Of Active Carriers");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_BTS_CLASS, BTSClass),
				4, "BTS Class");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_FILTER, pvTFilter),
				5, "PvT Filter");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN DThis function repeats the evaluation of the data currently in the capture buffer without capturing
/// HIFN new data. This is useful after changing settings, for example filters, patterns or evaluation ranges.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
ViStatus _VI_FUNC rsspecan_GSMK10RefreshCapturedData(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_K10_REFRESH_CAPTURED_DATA, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Read GSM K10 Measurement Results
/* Purpose:  This function starts the Modulation Accuracy measurement and
/*           returns the result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadGSMK10MeasurementResults(
	ViSession instrSession,
	ViUInt32 timeout,
	ViInt32 measurement,
	ViInt32 modifier,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL, 5, "Result");
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));

	switch (measurement)
	{
	case RSSPECAN_VAL_GSM_K10_MEAS_EVM_RMS:
	case RSSPECAN_VAL_GSM_K10_MEAS_EVM_PEAK:
	case RSSPECAN_VAL_GSM_K10_MEAS_MERR_RMS:
	case RSSPECAN_VAL_GSM_K10_MEAS_MERR_PEAK:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERR_RMS:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERR_PEAK:
	case RSSPECAN_VAL_GSM_K10_MEAS_OSUPP:
	case RSSPECAN_VAL_GSM_K10_MEAS_IQOF:
	case RSSPECAN_VAL_GSM_K10_MEAS_IQIM:
	case RSSPECAN_VAL_GSM_K10_MEAS_FREQ:
	case RSSPECAN_VAL_GSM_K10_MEAS_BPOW:
	case RSSPECAN_VAL_GSM_K10_MEAS_ADR:
		{
			viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_GSM_K10_MOD_AVG, RSSPECAN_VAL_GSM_K10_MOD_DEV),
					5, "Modifier");

			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s,%s", GSMMeasTypeArr[measurement], GSMMeasModifArr[modifier]);

			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_READ_GSM_K10_MEAS_RES, result),
					6, "Result");
			break;
		}
	case RSSPECAN_VAL_GSM_K10_MEAS_PERC_EVM:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERC_MERR:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERC_PERR:
		{
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s", GSMPercentileArr[measurement - 12]);

			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_READ_GSM_K10_MEAS_RES_95_PERC, result),
					6, "Result");
			break;
		}

	case RSSPECAN_VAL_GSM_K10_MEAS_TRIGGER_TO_SYNC:
		{
			viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_GSM_K10_MOD_AVG, RSSPECAN_VAL_GSM_K10_MOD_DEV),
					5, "Modifier");

			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s", GSMMeasModifArr[modifier]);

			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_READ_GSM_K10_TRIGGER_TO_SYNC_RESULTS, result),
					6, "Result");
			break;
		}

	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurement), 4, "Measurement");
	}

Error:
	rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch GSM K10 Measurement Results
/* Purpose:  This function starts the Modulation Accuracy measurement and
/*           returns the result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchGSMK10MeasurementResults(
	ViSession instrSession,
	ViInt32 measurement,
	ViInt32 modifier,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL, 5, "Result");

	switch (measurement)
	{
	case RSSPECAN_VAL_GSM_K10_MEAS_EVM_RMS:
	case RSSPECAN_VAL_GSM_K10_MEAS_EVM_PEAK:
	case RSSPECAN_VAL_GSM_K10_MEAS_MERR_RMS:
	case RSSPECAN_VAL_GSM_K10_MEAS_MERR_PEAK:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERR_RMS:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERR_PEAK:
	case RSSPECAN_VAL_GSM_K10_MEAS_OSUPP:
	case RSSPECAN_VAL_GSM_K10_MEAS_IQOF:
	case RSSPECAN_VAL_GSM_K10_MEAS_IQIM:
	case RSSPECAN_VAL_GSM_K10_MEAS_FREQ:
	case RSSPECAN_VAL_GSM_K10_MEAS_BPOW:
	case RSSPECAN_VAL_GSM_K10_MEAS_ADR:
		{
			viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_GSM_K10_MOD_AVG, RSSPECAN_VAL_GSM_K10_MOD_DEV),
					4, "Modifier");

			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s,%s", GSMMeasTypeArr[measurement], GSMMeasModifArr[modifier]);

			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FETCH_GSM_K10_MEAS_RES, result),
					5, "Result");
			break;
		}
	case RSSPECAN_VAL_GSM_K10_MEAS_PERC_EVM:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERC_MERR:
	case RSSPECAN_VAL_GSM_K10_MEAS_PERC_PERR:
		{
			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s", GSMPercentileArr[measurement - 12]);

			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FETCH_GSM_K10_MEAS_RES_95_PERC, result),
					5, "Result");
			break;
		}
	case RSSPECAN_VAL_GSM_K10_MEAS_TRIGGER_TO_SYNC:
		{
			viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_GSM_K10_MOD_AVG, RSSPECAN_VAL_GSM_K10_MOD_DEV),
					4, "Modifier");

			snprintf(repCap, RS_REPCAP_BUF_SIZE, "%s", GSMMeasModifArr[modifier]);

			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FETCH_GSM_K10_TRIGGER_TO_SYNC_RESULTS, result),
					5, "Result");
			break;
		}

	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurement), 3, "Measurement");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read GSM K10 Modulation Accuracy Results
/* Purpose:  This function starts the measurement and returns all the
/*           results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadGSMK10ModulationAccuracyResults(
	ViSession instrSession,
	ViUInt32 timeout,
	ViInt32 arraySize,
	ViReal64 result[],
	ViInt32* returnedValues
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL,4,"Result");

	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, "READ:BURS:ALL?", timeout, arraySize, result, returnedValues));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch GSM K10 Modulation Accuracy Results
/* Purpose:  This function returns all the results of the Modulation
/*           Accuracy table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchGSMK10ModulationAccuracyResults(
	ViSession instrSession,
	ViInt32 arraySize,
	ViReal64 result[],
	ViInt32* returnedValues
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "FETC:BURS:ALL?", arraySize, result, returnedValues));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read GSM K10 Burst Slot Results
/* Purpose:  This function starts a measurement and returns the result of
/*           the burst power for the selected slot.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadGSMK10BurstSlotResults(
	ViSession instrSession,
	ViUInt32 timeout,
	ViInt32 slot,
	ViInt32 burstSelection,
	ViInt32 modifier,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = 0;
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL, 6, "Result");

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			3, "Slot");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, burstSelection, RSSPECAN_VAL_GSM_K10_BURST_ALL, RSSPECAN_VAL_GSM_K10_BURST_CURR),
			3, "burstSelection");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_GSM_K10_BURST_MOD_AVG_PWR, RSSPECAN_VAL_GSM_K10_BURST_MOD_MAX_PWR),
			4, "modifier");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld,%s,%s", slot, GSMBurstSelArr[burstSelection], GSMBurstModifArr[modifier]);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_READ_GSM_K10_BURST_SLOT_RES, result),
			6, "Result");

Error:
	rsspecan_SetOPCTimeout(instrSession, old_timeout);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch GSM K10 Burst Slot Results
/* Purpose:  This function returns the result of the burst power for the
/*           selected slot.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchGSMK10BurstSlotResults(
	ViSession instrSession,
	ViInt32 slot,
	ViInt32 burstSelection,
	ViInt32 modifier,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL, 5, "Result");

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			2, "Slot");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, burstSelection, RSSPECAN_VAL_GSM_K10_BURST_ALL, RSSPECAN_VAL_GSM_K10_BURST_CURR),
			3, "burstSelection");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_GSM_K10_BURST_MOD_AVG_PWR, RSSPECAN_VAL_GSM_K10_BURST_MOD_MAX_PWR),
			4, "modifier");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld,%s,%s", slot, GSMBurstSelArr[burstSelection], GSMBurstModifArr[modifier]);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FETCH_GSM_K10_BURST_SLOT_RES, result),
			5, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function starts a measurement and returns "Delta to Sync"
/// HIFN value for the selected slot. This function is only available
/// HIFN when the "Power vs Time" measurement is selected.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR timeout/This control sets the timeout value.
/// HIPAR slot/Selects the slot.
/// HIPAR result/Returns the value.
ViStatus _VI_FUNC rsspecan_ReadGSMK10BurstSlotDeltaToSync(ViSession instrSession,
                                                          ViUInt32 timeout,
                                                          ViInt32 slot,
                                                          ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = 0;
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL, 4, "Result");

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			3, "Slot");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld", slot);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_READ_GSM_K10_BURST_SLOT_DELTA_TO_SYNC, result),
			4, "Result");

Error:
	rsspecan_SetOPCTimeout(instrSession, old_timeout);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Returns "Delta to Sync" value for the selected slot. This
/// HIFN function is only available when the "Power vs Time" measurement
/// HIFN is selected.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR slot/Selects the slot.
/// HIPAR result/Returns the value.
ViStatus _VI_FUNC rsspecan_FetchGSMK10BurstSlotDeltaToSync(ViSession instrSession,
                                                           ViInt32 slot,
                                                           ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(result == NULL, 3, "Result");

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, slot, 0, 7),
			2, "Slot");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "GSMS%ld", slot);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FETCH_GSM_K10_BURST_SLOT_DELTA_TO_SYNC, result),
			3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function starts the measurement and returns the measured reference power and resolution bandwidth.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR timeout/This control sets the timeout value.
/// HIPAR spectrumType/Sets the type of returned value.
/// HIPAR level1/Returns the measured reference power in dBm.
/// HIPAR level2/Returns the measured reference power in dBm.
/// HIPAR resolutionBW/Returns the resolution bandwidth used to measure the reference power in Hz.
ViStatus _VI_FUNC rsspecan_ReadGSMK10SpectrumReferencePower(ViSession instrSession,
                                                            ViUInt32 timeout,
                                                            ViInt32 spectrumType,
                                                            ViReal64* level1,
                                                            ViReal64* level2,
                                                            ViReal64* resolutionBW)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViReal64 data[3];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, spectrumType, RSSPECAN_VAL_GSM_K10_MOD_SPEC, RSSPECAN_VAL_GSM_K10_WIDE_SPEC),
			3, "Spectrum Type");

	switch (spectrumType)
	{
	case RSSPECAN_VAL_GSM_K10_MOD_SPEC:
		strcpy(cmd, "READ:SPEC:MOD:REF?");
		break;
	case RSSPECAN_VAL_GSM_K10_TRANS_SPEC:
		strcpy(cmd, "READ:SPEC:SWIT:REF?");
		break;
	case RSSPECAN_VAL_GSM_K10_WIDE_SPEC:
		strcpy(cmd, "READ:WSP:MOD:REF?");
		break;
	}

	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, cmd, timeout, 3, data, NULL));
	*level1 = data[0];
	*level2 = data[1];
	*resolutionBW = data[2];

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function reads out the result of the measured reference power and resolution bandwidth.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR spectrumType/Sets the type of returned value.
/// HIPAR level1/Returns the measured reference power in dBm.
/// HIPAR level2/Returns the measured reference power in dBm.
/// HIPAR resolutionBW/Returns the resolution bandwidth used to measure the reference power in Hz.
ViStatus _VI_FUNC rsspecan_FetchGSMK10SpectrumReferencePower(ViSession instrSession,
                                                             ViInt32 spectrumType,
                                                             ViReal64* level1,
                                                             ViReal64* level2,
                                                             ViReal64* resolutionBW)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViReal64 data[3];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, spectrumType, RSSPECAN_VAL_GSM_K10_MOD_SPEC, RSSPECAN_VAL_GSM_K10_WIDE_SPEC),
			3, "Spectrum Type");

	switch (spectrumType)
	{
	case RSSPECAN_VAL_GSM_K10_MOD_SPEC:
		strcpy(cmd, "READ:SPEC:MOD:REF?");
		break;
	case RSSPECAN_VAL_GSM_K10_TRANS_SPEC:
		strcpy(cmd, "READ:SPEC:SWIT:REF?");
		break;
	case RSSPECAN_VAL_GSM_K10_WIDE_SPEC:
		strcpy(cmd, "READ:WSP:MOD:REF?");
		break;
	}

	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, cmd, 0, 3, data, NULL));
	*level1 = data[0];
	*level2 = data[1];
	*resolutionBW = data[2];

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read GSM K10 Modulation Spectrum Results
/* Purpose:  This function starts the measurement and reads out the result
/*           of the measurement of the modulation spectrum of the mobile or
/*           base station.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadGSMK10ModulationSpectrumResults(ViSession instrSession,
                                                               ViInt32 arraySize,
                                                               ViUInt32 timeout,
                                                               ViInt32 index[],
                                                               ViReal64 startFrequencies[],
                                                               ViReal64 stopFrequencies[],
                                                               ViReal64 levels[],
                                                               ViReal64 limits[],
                                                               ViInt32 absRel[],
                                                               ViInt32 status[],
                                                               ViInt32* num_ofResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt;
	ViChar tmp_str[10];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, num_ofResults), 10, "Num Of Results");

	checkErr(RsCore_QueryViStringUnknownLengthWithOpc(instrSession, "READ:SPEC:MOD?", timeout, &pbuffer));

	cnt = 0;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value)
	{
		if (cnt < arraySize)
		{
			sscanf(pstring_value, "%ld", &index[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &startFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &stopFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &levels[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &limits[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,]", tmp_str);
			absRel[cnt] = RsCore_FindStringIndex(absRelArr, tmp_str);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,'\n''\r']", tmp_str);
			status[cnt] = RsCore_FindStringIndex(statusArr, tmp_str);
			pstring_value = strtok(NULL, ",");
		}
		cnt++;
	}

	*num_ofResults = cnt;
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (pbuffer) free(pbuffer);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch GSM K10 Modulation Spectrum Results
/* Purpose:  This function reads out the result of the measurement of the
/*           modulation spectrum of the mobile or base station.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchGSMK10ModulationSpectrumResults(ViSession instrSession,
                                                                ViInt32 arraySize,
                                                                ViInt32 index[],
                                                                ViReal64 startFrequencies[],
                                                                ViReal64 stopFrequencies[],
                                                                ViReal64 levels[],
                                                                ViReal64 limits[],
                                                                ViInt32 absRel[],
                                                                ViInt32 status[],
                                                                ViInt32* num_ofResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt;
	ViChar tmp_str[10];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, num_ofResults), 10, "Num Of Results");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "FETC:SPEC:MOD?", &pbuffer));

	cnt = 0;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value)
	{
		if (cnt < arraySize)
		{
			sscanf(pstring_value, "%ld", &index[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &startFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &stopFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &levels[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &limits[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,]", tmp_str);
			absRel[cnt] = RsCore_FindStringIndex(absRelArr, tmp_str);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,'\n''\r']", tmp_str);
			status[cnt] = RsCore_FindStringIndex(statusArr, tmp_str);
			pstring_value = strtok(NULL, ",");
		}
		cnt++;
	}

	*num_ofResults = cnt;
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read GSM K10 Transient Spectrum Results
/* Purpose:  This function starts the measurement and reads out the result
/*           of the measurement of the transient spectrum of the mobile or
/*           base station.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadGSMK10TransientSpectrumResults(ViSession instrSession,
                                                              ViInt32 arraySize,
                                                              ViUInt32 timeout,
                                                              ViInt32 index[],
                                                              ViReal64 startFrequencies[],
                                                              ViReal64 stopFrequencies[],
                                                              ViReal64 levels[],
                                                              ViReal64 limits[],
                                                              ViInt32 absRel[],
                                                              ViInt32 status[],
                                                              ViInt32* num_ofResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt;
	ViChar tmp_str[10];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, num_ofResults), 10, "Num Of Results");

	checkErr(RsCore_QueryViStringUnknownLengthWithOpc(instrSession, "READ:SPEC:SWIT?", timeout, &pbuffer));

	cnt = 0;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value)
	{
		if (cnt < arraySize)
		{
			sscanf(pstring_value, "%ld", &index[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &startFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &stopFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &levels[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &limits[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,]", tmp_str);
			absRel[cnt] = RsCore_FindStringIndex(absRelArr, tmp_str);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,'\n''\r']", tmp_str);
			status[cnt] = RsCore_FindStringIndex(statusArr, tmp_str);
			pstring_value = strtok(NULL, ",");
		}
		cnt++;
	}

	*num_ofResults = cnt;
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Fetch GSM K10 Transient Spectrum Results
/* Purpose:  This function reads out the result of the measurement of the
/*           transient spectrum of the mobile or base station.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchGSMK10TransientSpectrumResults(ViSession instrSession,
                                                               ViInt32 arraySize,
                                                               ViInt32 index[],
                                                               ViReal64 startFrequencies[],
                                                               ViReal64 stopFrequencies[],
                                                               ViReal64 levels[],
                                                               ViReal64 limits[],
                                                               ViInt32 absRel[],
                                                               ViInt32 status[],
                                                               ViInt32* num_ofResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt;
	ViChar tmp_str[10];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, num_ofResults), 10, "Num Of Results");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "FETC:SPEC:SWIT?", &pbuffer));

	cnt = 0;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value)
	{
		if (cnt < arraySize)
		{
			sscanf(pstring_value, "%ld", &index[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &startFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &stopFrequencies[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &levels[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &limits[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,]", tmp_str);
			absRel[cnt] = RsCore_FindStringIndex(absRelArr, tmp_str);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,'\n''\r']", tmp_str);
			status[cnt] = RsCore_FindStringIndex(statusArr, tmp_str);
			pstring_value = strtok(NULL, ",");
		}
		cnt++;
	}

	*num_ofResults = cnt;
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function  starts the measurement and reads out the result of the measurement of the "Wide Modulation Spectrum"  of the mobile or base station.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR arraySize/Defines the number of elements allocated in arrays
/// HIPAR timeout/Timeout value
/// HIPAR index/Reserved for future use.
/// HIPAR absoluteOffsetFrequencies1/Absolute offset frequencies 1.
/// HIPAR absoluteOffsetFrequencies2/Absolute offset frequencies 2.
/// HIPAR levels/Returns array of measured maximum levels of the partial range or measured levels at the test point.
/// HIPAR limits/Returns limits in the partial range or measured levels at the test point.
/// HIPAR absRel/Returns array of ABS/REL unit indicators
/// HIPAR status/Limit checks array of results.
/// HIPAR numOfResults/Number of results, received from the instrument.
ViStatus _VI_FUNC rsspecan_ReadGSMK10WideSpectrumResults(ViSession instrSession,
                                                         ViInt32 arraySize,
                                                         ViUInt32 timeout,
                                                         ViInt32 index[],
                                                         ViReal64 absoluteOffsetFrequencies1[],
                                                         ViReal64 absoluteOffsetFrequencies2[],
                                                         ViReal64 levels[],
                                                         ViReal64 limits[],
                                                         ViInt32 absRel[],
                                                         ViInt32 status[],
                                                         ViInt32* numOfResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt;
	ViChar tmp_str[10];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, numOfResults), 11, "Num Of Results");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "READ:WSP:MOD?", &pbuffer));

	cnt = 0;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value)
	{
		if (cnt < arraySize)
		{
			sscanf(pstring_value, "%ld", &index[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &absoluteOffsetFrequencies1[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &absoluteOffsetFrequencies2[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &levels[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &limits[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,]", tmp_str);
			absRel[cnt] = RsCore_FindStringIndex(absRelArr, tmp_str);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,'\n''\r']", tmp_str);
			status[cnt] = RsCore_FindStringIndex(statusArr, tmp_str);
			pstring_value = strtok(NULL, ",");
		}
		cnt++;
	}

	*numOfResults = cnt;
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function reads out the result of the measurement of the "Wide Modulation Spectrum"  of the mobile or base station.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR arraySize/Defines the number of elements allocated in arrays
/// HIPAR index/Reserved for future use.
/// HIPAR absoluteOffsetFrequencies1/Absolute offset frequencies 1.
/// HIPAR absoluteOffsetFrequencies2/Absolute offset frequencies 2.
/// HIPAR levels/Returns array of measured maximum levels of the partial range or measured levels at the test point.
/// HIPAR limits/Returns limits in the partial range or measured levels at the test point.
/// HIPAR absRel/Returns array of ABS/REL unit indicators
/// HIPAR status/Limit checks array of results.
/// HIPAR numOfResults/Number of results, received from the instrument.
ViStatus _VI_FUNC rsspecan_FetchGSMK10WideSpectrumResults(ViSession instrSession,
                                                          ViInt32 arraySize,
                                                          ViInt32 index[],
                                                          ViReal64 absoluteOffsetFrequencies1[],
                                                          ViReal64 absoluteOffsetFrequencies2[],
                                                          ViReal64 levels[],
                                                          ViReal64 limits[],
                                                          ViInt32 absRel[],
                                                          ViInt32 status[],
                                                          ViInt32* numOfResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt;
	ViChar tmp_str[10];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, numOfResults), 11, "Num Of Results");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "FETC:WSP:MOD?", &pbuffer));

	cnt = 0;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value)
	{
		if (cnt < arraySize)
		{
			sscanf(pstring_value, "%ld", &index[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &absoluteOffsetFrequencies1[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &absoluteOffsetFrequencies2[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &levels[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%le", &limits[cnt]);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,]", tmp_str);
			absRel[cnt] = RsCore_FindStringIndex(absRelArr, tmp_str);
			pstring_value = strtok(NULL, ",");
			sscanf(pstring_value, "%[^,'\n''\r']", tmp_str);
			status[cnt] = RsCore_FindStringIndex(statusArr, tmp_str);
			pstring_value = strtok(NULL, ",");
		}
		cnt++;
	}

	*numOfResults = cnt;
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function reads out the gated "Modulation Spectrum" or "Wide
/// HIFN Modulation Spectrum" measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR timeout/This control sets the timeout value.
/// HIPAR triggerOffset/Calculated trigger offset, based on the user-defined "Trigger Offset" and "Frame Configuration", such that 50-90% of the active part of the "Slot to measure" (excluding TSC) is measured.
/// HIPAR gateLength/Calculated gate length, based on the user-defined "Trigger Offset" and "Frame Configuration", such that 50-90% of the active part of the "Slot to measure" (excluding TSC) is measured
ViStatus _VI_FUNC rsspecan_ReadGSMK10WideSpectrumGating(ViSession instrSession,
                                                        ViUInt32 timeout,
                                                        ViReal64* triggerOffset,
                                                        ViReal64* gateLength)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* p2buf;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, triggerOffset), 3, "Trigger Offset");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, gateLength), 4, "Gate Length");

	checkErr(RsCore_QueryViStringUnknownLengthWithOpc(instrSession, "READ:SPEC:WMOD:GAT?", timeout, &pbuffer));
	checkErr(rsspecan_CheckStatus(instrSession));

	p2buf = strtok(pbuffer, ",");

	*triggerOffset = atof(p2buf);

	p2buf = strtok(NULL, ",");

	*gateLength = atof(p2buf);

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function queries the positions of the slots to measure (slot
/// HIFN  scope) in the current capture buffer (indicated by blue bars in the
/// HIFN  result display).
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) This function is available only in conjunction with K10 option.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     -
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:MCAPture:SLOTs:MEASure?
/// HIFN     FETCh:MCAPture:SLOTs:SCOPe?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/This control defines the number of elements allocated in each array.
/// HIPAR resultType/Selects the result type.
/// HIPAR position/The x-value (in seconds) of the i-th slot to measure (scope).
/// HIPAR length/The length of the i-th slot to measure (scope) (in seconds).
/// HIPAR numOfResults/This control returns number of results, received from the instrSession.
ViStatus _VI_FUNC rsspecan_FetchGSMK10MagnitudeCaptureResults(ViSession instrSession,
                                                              ViInt32 arraySize,
                                                              ViInt32 resultType,
                                                              ViReal64 position[],
                                                              ViReal64 length[],
                                                              ViInt32* numOfResults)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViReal64* data = NULL;
	ViInt32 dataSize, i;
	ViInt32 j = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, resultType, RSSPECAN_VAL_GSM_K10_SLOT_SCOPE,
		RSSPECAN_VAL_GSM_K10_SLOT_MEASURE), 3, "Result Type");

	viCheckParm(position == NULL, 3, "Position");
	viCheckParm(length == NULL, 3, "Length");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "FETC:MCAP:SLOT:%s?", GSMMAgnitudeResultArr[resultType]);
	checkErr(RsCore_QueryFloatArray(instrSession, cmd, &data, &dataSize));
	checkErr(rsspecan_CheckStatus(instrSession));

	dataSize /= 2;

	if (numOfResults)
		*numOfResults = dataSize;

	if (dataSize > arraySize)
		dataSize = arraySize;

	for (i = 0; i < dataSize; i++)
	{
		position[i] = data[j++];
		length[i] = data[j++];
	}

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
