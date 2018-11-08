/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - K15 Avionics Measurement
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
 *- RSSpecAn K15 - Avionics Measurement -------------------------------------*
 *****************************************************************************/

static ViString avionicsSummary[] = {"30", "9960", "ID", "90", "150", "90+150"};

/*****************************************************************************
 * Function: Avionics Mode
 * Purpose:  This function sets the avionics measurements operating mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AvionicsMode(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_AVI_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics Demodulation Mode
 * Purpose:  This function sets the avionics demodulation mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDemodulationMode(ViSession instrSession,
                                                             ViInt32 demodulationMode)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AVI_DEMOD_MODE, demodulationMode),
			2, "Demodulation Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics Distortion Result
 * Purpose:  This function elects the type of evaluation of the measured data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDistortionResult(ViSession instrSession,
                                                             ViInt32 type)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AVI_DISTORTION_RESULT, type),
			2, "Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics THD Unit
 * Purpose:  This function selects between % and dB for displaying the THD
 *           measurement result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsTHDUnit(ViSession instrSession,
                                                    ViInt32 unit)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ADEM_UNIT_THD, unit),
			2, "Unit");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics DDM Unit
 * Purpose:  This function selects the unit for ILS DDM reading.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDDMUnit(ViSession instrSession,
                                                    ViInt32 unit)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_AVI_DDM_UNIT, unit),
			2, "Unit");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics Input
 * Purpose:  This function selects the audio input - and thus the BASEBAND mode
 *           - on (AUDio) or off (RF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsInput(ViSession instrSession,
                                                  ViInt32 window, ViInt32 input)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AVI_INPUT, input),
			3, "Input");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics Demodulation Bandwidth
 * Purpose:  This function configures the demodulation bandwidth settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDemodulationBandwidth(ViSession instrSession,
                                                                  ViInt32 window,
                                                                  ViBoolean dbAuto,
                                                                  ViReal64 demodulationBandwidth)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AVI_DEMOD_BWID_AUTO, dbAuto),
			3, "Auto");

	if (dbAuto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_AVI_DEMOD_BWID, demodulationBandwidth),
				4, "Demodulation Bandwidth");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics AF Frequency Start Stop
 * Purpose:  This function configures the frequency range of the result display
 *           AF spectrum using start frequency and stop frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFFrequencyStartStop(ViSession instrSession,
                                                                 ViReal64 startFrequency,
                                                                 ViReal64 stopFrequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_START, startFrequency),
			2, "Start Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_STOP, stopFrequency),
			3, "Stop Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics AF Frequency Center Span
 * Purpose:  This function configures the frequency range of the result display
 *           AF spectrum using the center frequency and the frequency span.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFFrequencyCenterSpan(ViSession instrSession,
                                                                  ViReal64 centerFrequency,
                                                                  ViReal64 span)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_CENTER, centerFrequency),
			2, "Center Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_SPAN, span),
			3, "Span");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics AF Full Span
 * Purpose:  This function sets the maximum span for the display of the AF spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFFullSpan(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FMDEM_AF_FULL_SPAN, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics AF Param
 * Purpose:  This function configures bandwidth resolutionfor display of the AF
 *           spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFParam(ViSession instrSession,
                                                    ViReal64 resolutionBandwidth)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FMDEM_BAND_RES, resolutionBandwidth),
			2, "Resolution Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Avionics Distortion Measurement
 * Purpose:  This function configures the distortion measurement on or off.
 *           Configures the measurement frequency of the harmonic distortion
 *           measurement to a single signal within the audio frequency range.
 *           Also configures the fundamental frequency of the harmonic
 *           distortion measurement and the maximum frequency of the harmonic
 *           distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDistortionMeasurement(ViSession instrSession,
                                                                  ViBoolean SHDState,
                                                                  ViReal64 SHDHarmonicFrequency,
                                                                  ViInt32 THDFundamentalFrequency,
                                                                  ViReal64 THDMaxFrequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AVI_HARM_DIST_STATE, SHDState),
			2, "SHD State");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_HARM_DIST_FREQ, SHDHarmonicFrequency),
			3, "SHD Harmonic Frequency");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AVI_THD_FUND_FREQ, THDFundamentalFrequency),
			4, "THD Fundamental Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_THD_MAX_FREQ, THDMaxFrequency),
			5, "THD Max Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics RF Frequency
 * Purpose:  This function reads RF frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsRFFrequency(ViSession instrSession,
                                                    ViReal64* RFFrequencyResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_RF_FREQ_RESULT, RFFrequencyResult),
			2, "RF Frequency Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics Carrier Offset
 * Purpose:  This function reads carrier offset, this mean frequency offset
 *           between measured frequency and tuned frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsCarrierOffset(ViSession instrSession,
                                                      ViReal64* carrierOffsetResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_CARR_OFFS_RESULT, carrierOffsetResult),
			2, "Carrier Offset Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics RF Level
 * Purpose:  This function reads the RF level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsRFLevel(ViSession instrSession,
                                                ViReal64* RFLevelResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_RF_LEVEL_RESULT, RFLevelResult),
			2, "RF Level Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics AM
 * Purpose:  This function queries the modulation depth and AF frequency
 *           measurement at the frequency modulated VOR/ILS signal component.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsAM(ViSession instrSession,
                                           ViInt32 AMDepth,
                                           ViReal64* AMDepthResult,
                                           ViInt32 AMFrequency,
                                           ViReal64* AMFrequencyResult)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, AMDepth, 0, 5),
			2, "AM Depth");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, AMFrequency, 0, 4),
			4, "AM Frequency");

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K15"));

	sprintf(cmd, "CALC1:AVI:AM:DEPT? '%s'", avionicsSummary[AMDepth]);
	checkErr(rsspecan_QueryViReal64(instrSession, cmd, AMDepthResult));

	sprintf(cmd, "CALC1:AVI:AM:FREQ? '%s'", avionicsSummary[AMFrequency]);
	checkErr(rsspecan_QueryViReal64(instrSession, cmd, AMFrequencyResult));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics THD
 * Purpose:  This function queries the distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsTHD(ViSession instrSession,
                                            ViInt32 THD, ViReal64* THDResult)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if ((THD != RSSPECAN_VAR_AVI_DEMOD_SUMM_30) &&
	(THD != RSSPECAN_VAR_AVI_DEMOD_SUMM_ID) &&
	(THD != RSSPECAN_VAR_AVI_DEMOD_SUMM_90_150))
	{
		viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "THD");
	}

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K15"));

	sprintf(cmd, "CALC1:AVI:THD:RES? '%s'", avionicsSummary[THD]);
	checkErr(rsspecan_QueryViReal64(instrSession, cmd, THDResult));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics Difference In Depth
 * Purpose:  This function reads DDM (= difference in depth of modulation)
 *           between 90 Hz and 150 Hz AM signal (m90 Hz - m150 Hz). Only
 *           available when ILS demodulation is switched on.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsDifferenceInDepth(ViSession instrSession,
                                                          ViReal64* DDMResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_DDM_RESULT, DDMResult),
			2, "DDM Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics FM
 * Purpose:  This function reads frequency deviation and AF frequency
 *           measurement at the frequency modulated VOR 30 Hz signal component.
 *           Only available when VOR demodulation is switched on.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsFM(ViSession instrSession,
                                           ViReal64* FMDeviationResult,
                                           ViReal64* FMFrequencyResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_FM_DEVIATION_RESULT, FMDeviationResult),
			2, "FM Deviation Result");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_FM_FREQ_RESULT, FMFrequencyResult),
			3, "FM Frequency Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics Phase
 * Purpose:  This function reads VOR or ILS Phase.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsPhase(ViSession instrSession,
                                              ViReal64* phaseResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_PHASE_RESULT, phaseResult),
			2, "Phase Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics Input Level
 * Purpose:  This function reads input level(Pk).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsInputLevel(ViSession instrSession,
                                                   ViReal64* inputLevelResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_INPUT_LEVEL_RESULT, inputLevelResult),
			2, "Input Level Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics SDM
 * Purpose:  This function reads selective measurement of the sum-modulation
 *           depth (SDM = Sum in Depth of Modulation); arithmetic sum of the
 *           modulation depth of the 90 Hz and the 150 Hz components without
 *           any influence of the phase between the components.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsSDM(ViSession instrSession,
                                            ViReal64* SDMResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_SDM_RESULT, SDMResult),
			2, "SDM Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Avionics SHD
 * Purpose:  This function reads the result of the manual distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryAvionicsSHD(ViSession instrSession,
                                            ViReal64* SHDResult)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_AVI_SHD_RESULT, SHDResult),
			2, "SHD Result");

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
