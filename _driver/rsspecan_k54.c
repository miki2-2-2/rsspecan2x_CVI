/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  RSSpecAn K54 - EMI Measurement Application
 *
 *  Original Release: June 2011
 *  By: Martin Krcmar
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *

 *****************************************************************************/

#include "rsspecan.h"


/// HIFN  This function turns the EMI measurement marker functionality on and
/// HIFN  off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/Turns the EMI measurement marker functionality on and off.
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementMarkerState(
	ViSession instrSession,
	ViBoolean state
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_EMI_FMEAS_STATE, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function selects the detector for a specific marker during the
/// HIFN  final measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR marker/This control selects marker.
/// HIPAR detector/Selects the detector for a specific marker during the final
/// HIPAR detector/measurement.
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementMarkerDetector(
	ViSession instrSession,
	ViInt32 marker,
	ViInt32 detector
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
		2, "Marker");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_MARKER_DETECTOR,
			"Marker", marker - 1, RS_REPCAP_BUF_SIZE, repCap),
		2, "Marker");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_MARKER_DETECTOR, detector),
		3, "Detector");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function initiates a marker peak search.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_EMIFinalMeasurementMarkerPeakSearch(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_EMI_FMEAS_MARKER_PEAK_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function defines the dwell time during the final measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR dwellTime/Defines the dwell time during the final measurement.
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementMarkerDwellTime(
	ViSession instrSession,
	ViReal64 dwellTime
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EMI_FMEAS_DWELL_TIME, dwellTime),
		2, "Dwell Time");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function returns marker limit line condition and vertical
/// HIFN  distance.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR marker/This control selects marker.
/// HIPAR limit/This control selects limit.
/// HIPAR markerCondition/Returns the condition of a marker position in relation to a certain
/// HIPAR markerCondition/limit line.
/// HIPAR markerVerticalDistance/Returns the vertical distance from the marker position to the limit
/// HIPAR markerVerticalDistance/line. The unit is dB. If the marker has been assigned to a different
/// HIPAR markerVerticalDistance/trace than the limit line, the function returns -200.
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementMarkerLimitLine(
	ViSession instrSession,
	ViInt32 marker,
	ViInt32 limit,
	ViInt32* markerCondition,
	ViReal64* markerVerticalDistance
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";
	ViChar repCap_m[RS_REPCAP_BUF_SIZE] = "";
	ViChar repCap_l[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
		2, "Marker");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_MARKER_LIMIT_CONDITION,
			"Marker", marker - 1, RS_REPCAP_BUF_SIZE, repCap_m),
		2, "Marker");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_MARKER_LIMIT_CONDITION,
			"Limit", limit - 1, RS_REPCAP_BUF_SIZE, repCap_l),
		3, "Limit");

	strcpy(repCap, repCap_m);
	strcat(repCap, ",");
	strcat(repCap, repCap_l);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_MARKER_LIMIT_CONDITION, markerCondition),
		4, "Marker Condition");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_MARKER_LIMIT_VERTICAL_DISTANCE, markerVerticalDistance),
		5, "Marker Vertical Distance");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function returns the result of the EMI measurement at the marker
/// HIFN  position.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR marker/This control selects marker.
/// HIPAR result/Returns the result of the EMI measurement at the marker position.
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementMarkerResult(
	ViSession instrSession,
	ViInt32 marker,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
		2, "Marker");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_MARKER_RESULT,
			"Marker", marker - 1, RS_REPCAP_BUF_SIZE, repCap),
		2, "Marker");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_MARKER_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function selects the detector for a specific delta marker during
/// HIFN  the final measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR deltaMarker/This control selects delta marker.
/// HIPAR detector/Selects the detector for a specific delta marker during the final
/// HIPAR detector/measurement.
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementDeltaMarkerDetector(
	ViSession instrSession,
	ViInt32 deltaMarker,
	ViInt32 detector
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 16),
		2, "Delta Marker");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_DETECTOR,
			"DeltaMarker", deltaMarker - 1, RS_REPCAP_BUF_SIZE, repCap),
		2, "Delta Marker");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_DETECTOR, detector),
		3, "Detector");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function initiates a delta marker peak search.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_EMIFinalMeasurementDeltaMarkerPeakSearch(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_PEAK_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function returns delta marker limit line condition and vertical
/// HIFN  distance.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR deltaMarker/This control selects delta marker.
/// HIPAR limit/This control selects limit.
/// HIPAR deltaMarkerCondition/Returns the condition of a delta marker position in relation to a
/// HIPAR deltaMarkerCondition/certain limit line.
/// HIPAR deltaMarkerVerticalDistance/Returns the vertical distance from the delta marker position to the
/// HIPAR deltaMarkerVerticalDistance/limit line. The unit is dB. If the delta marker has been assigned to a
/// HIPAR deltaMarkerVerticalDistance/different trace than the limit line, the function returns -200.
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementDeltaMarkerLimitLine(
	ViSession instrSession,
	ViInt32 deltaMarker,
	ViInt32 limit,
	ViInt32* deltaMarkerCondition,
	ViReal64* deltaMarkerVerticalDistance
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";
	ViChar repCap_m[RS_REPCAP_BUF_SIZE] = "";
	ViChar repCap_l[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 16),
		2, "Delta Marker");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_LIMIT_CONDITION,
			"DeltaMarker", deltaMarker - 1, RS_REPCAP_BUF_SIZE, repCap_m),
		2, "Delta Marker");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_LIMIT_CONDITION,
			"Limit", limit - 1, RS_REPCAP_BUF_SIZE, repCap_l),
		3, "Limit");

	strcpy(repCap, repCap_m);
	strcat(repCap, ",");
	strcat(repCap, repCap_l);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_LIMIT_CONDITION, deltaMarkerCondition),
		4, "Delta Marker Condition");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_LIMIT_VERTICAL_DISTANCE, deltaMarkerVerticalDistance),
		5, "Delta Marker Vertical Distance");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function returns the result of the EMI measurement at the delta
/// HIFN  marker position.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR deltaMarker/This control selects delta marker.
/// HIPAR result/Returns the result of the EMI measurement at the delta marker
/// HIPAR result/position.
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementDeltaMarkerResult(
	ViSession instrSession,
	ViInt32 deltaMarker,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 16),
		2, "Delta Marker");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_RESULT,
			"DeltaMarker", deltaMarker - 1, RS_REPCAP_BUF_SIZE, repCap),
		2, "DeltaMarker");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function selects the V-network to be controlled via user port.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR vNetworkType/This control selects the V-network to be controlled via user port.
ViStatus _VI_FUNC rsspecan_ConfigureEMILISNVnetworkType(
	ViSession instrSession,
	ViInt32 vNetworkType
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EMI_LISN_V_NETWORK_TYPE, vNetworkType),
		2, "V-Network Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function turns the 150 kHz highpass filter for the ENV216 network
/// HIFN  on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR highPassFilter/Turns the 150 kHz highpass filter for the ENV216 network on and off.
ViStatus _VI_FUNC rsspecan_ConfigureEMILISNVnetworkHighPassFilter(
	ViSession instrSession,
	ViBoolean highPassFilter
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_EMI_LISN_V_NETWORK_HIGH_PASS_FILTER, highPassFilter),
		2, "High Pass Filter");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function selects the phase of the LISN you want to measure.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR phase/This control selects the phase of the LISN you want to measure.
ViStatus _VI_FUNC rsspecan_ConfigureEMILISNVnetworkPhase(
	ViSession instrSession,
	ViInt32 phase
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EMI_LISN_V_NETWORK_PHASE, phase),
		2, "Phase");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function sets the filter type for the resolution bandwidth.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR filterType/Sets the filter type for the resolution bandwidth.
ViStatus _VI_FUNC rsspecan_ConfigureEMIBandwithFilterType(
	ViSession instrSession,
	ViInt32 filterType
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EMI_RESOLUTION_BANDWIDTH_FILTER_TYPE, filterType),
		2, "Filter Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 *- End Of RSSpecAn K54 - EMI Measurement Application -----------------------*
 *****************************************************************************/
