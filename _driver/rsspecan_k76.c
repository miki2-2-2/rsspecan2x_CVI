/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  K76 - TD-SCDMA BTS Mode
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

/*===========================================================================*/
/* Function: Set TDS BS Measurement
/* Purpose:  This function selects the measurement of the Application
/*           Firmware R&S FS-K76, TD-SCDMA base station test.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetTDSBSMeasurement(ViSession instrSession,
                                               ViInt32 measurement)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_MEAS, measurement),
			2, "Measurement");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Capture
/* Purpose:  This function configures the TDS BS capture settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSCapture(ViSession instrSession,
                                                 ViInt32 setCount,
                                                 ViInt32 setValue,
                                                 ViInt32 captureLength)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SET_COUNT, setCount),
			2, "Set Count");
	if (setCount > 1)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SET_VALUE, setValue),
				3, "Set Value");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CAPTURE_LENGTH, captureLength),
				4, "Capture Length");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Code DOmain Analyzer Capture
/* Purpose:  This function configures the TDS BS capture settings for
/*           Code Domain Analyzer Mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSCDACapture(ViSession instrSession,
                                                    ViInt32 setCount,
                                                    ViInt32 setValue,
                                                    ViInt32 captureLength,
                                                    ViInt32 channel,
                                                    ViInt32 slot)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSQ"))
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SET_COUNT, setCount),
				2, "Set Count");
		if (setCount > 1)
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SET_VALUE, setValue),
					3, "Set Value");
		}
		else
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CAPTURE_LENGTH, captureLength),
					4, "Capture Length");
		}
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CAPTURE_LENGTH, captureLength),
				4, "Capture Length");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SEL_CHAN, channel),
			5, "Channel");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_RESULT_SEL_SLOT, slot),
			6, "Slot");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: TDS BS Adapt Signal Auto Level Time
/* Purpose:  This function starts the autorange routine for the reference
/*           level.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TDSBSAdaptSignalAutoLevelTime(ViSession instrSession,
                                                         ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_ADAPT_SIGN_AUT_LEVEL_TIME, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: TDS BS CDP Level Adjust
/* Purpose:  This function initiates automatic setting of the RF attenuation
/*           and IF gain to the level of the applied signal. The unit is put
/*           into RF ATTEN MANUAL mode to optimize RF attenuation and IF gain
/*           independently of each other. This mode is retained after
/*           changing from TD-SCDMA BTS mode to SPECTRUM mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TDSBSCDPLevelAdjust(ViSession instrSession,
                                               ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TDBS_CDP_LEV_ADJUST, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Measurement
/* Purpose:  This function configures the TDS BS measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSMeasurement(ViSession instrSession,
                                                     ViInt32 standard,
                                                     ViInt32 scramblingCode,
                                                     ViInt32 midableShiftCells,
                                                     ViInt32 sideBand,
                                                     ViBoolean normalize,
                                                     ViReal64 inactiveChannelTreshold,
                                                     ViBoolean invertQ)
{
	ViStatus error = VI_SUCCESS;
	ViBoolean tmp_state = VI_FALSE;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_STANDARD, standard),
			2, "Standard");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_SCODE, scramblingCode),
			3, "Scrambling Code");
	checkErr(rsspecan_GetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_STATE, &tmp_state));
	if (tmp_state == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CELL_SHIFTS, midableShiftCells),
				4, "Midable Shifts");
	}
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_SIDE_BAND, sideBand),
			6, "Side Band");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_NORMALIZE, normalize),
			7, "Normalize");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TDBS_INACT_CHAN_THR, inactiveChannelTreshold),
			8, "Inactive Channel Treshold");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_INVERT_Q, invertQ),
			9, "Invert Q");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure TDS BS Sync To Slot
/* Purpose:  This function configures the TDS BS sync to slot. By default
/*           the R&S FS-K76 determines the phase reference for all downlink
/*           data slots from the downlink pilot channel (DwPCH). For e.g.
/*           beamforming or repeater measurements it might be necessary to
/*           apply different phase offsets to each time slot.Using the DwPCH
/*           as phase reference leads to rotated constellation diagrams and
/*           bad EVM values in these time slots. By activating the new
/*           setting 'SYNC TO SLOT' the R&S FS-K76 determines the phase
/*           reference from the midamble of the selected slot. Thus the data
/*           slots can be phase rotated to each other without degrading the
/*           EVM results. The selected slot must contain at least one data
/*           channel with sufficient power for successful synchronization.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSSyncToSlot(
	ViSession instrSession,
	ViBoolean syncToSlot
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_SYNC_TO_SLOT, syncToSlot),
			2, "Sync To Slot");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function selects the slot synchronization mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR syncToSlotMode/Selects the slot synchronization mode.
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSSyncToSlotMode(ViSession instrSession,
                                                        ViInt32 syncToSlotMode)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_STS_MODE, syncToSlotMode),
			2, "Sync To Slot Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures the phase rotations between the code channels.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR rotateCodeChannels/Allows phase rotations between code channels.
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSSyncToSlotRotateCodeChannels(ViSession instrSession,
                                                                      ViBoolean rotateCodeChannels)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_SYNC_TO_SLOT_ROTATE, rotateCodeChannels),
			2, "Rotate Code Channels");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure TDS BS User Defined CPICH
/* Purpose:  This function configures the TDS BS user defined CPICH.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSUCPICH(
	ViSession instrSession,
	ViBoolean userDefinedCPICH,
	ViInt32 cpichCode,
	ViInt32 cpichPattern
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_CPICH_STATE, userDefinedCPICH),
			2, "User Defined CPICH");

	if (userDefinedCPICH)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CPICH_CODE, cpichCode),
				3, "CPICH Code");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_CPICH_PATTERN, cpichPattern),
				4, "CPICH Pattern");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure TDS BS Power Vs. Time Measurement
/* Purpose:  This function configures the TDS BS Power Vs. Time measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSPVTMeasurement(ViSession instrSession,
                                                        ViInt32 switchingPoint,
                                                        ViInt32 subframes,
                                                        ViBoolean highDynamicState)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_MEAS_PVT_SWITCH_POINT, switchingPoint),
			2, "Switching Point");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_MEAS_PVT_SFRAMES, subframes),
			3, "Subframes");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TDBS_MEAS_PVT_HDYN, highDynamicState),
			4, "High Dynamic State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Get TDS BS Reference Level Adjustment Result
/* Purpose:  This function adjusts the refernce level and returns the
/*           result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetTDSBSReferenceLevelAdjustmentResult(ViSession instrSession,
                                                                  ViUInt32 timeout,
                                                                  ViInt32* result)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");
	checkErr(rsspecan_GetOPCTimeout(instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout(instrSession, timeout));
	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDBS_ADJ_RLEV_QUERY, result),
			2, "Result");

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 *- End Of RSSpecAn K76 -----------------------------------------------------*
 *****************************************************************************/
