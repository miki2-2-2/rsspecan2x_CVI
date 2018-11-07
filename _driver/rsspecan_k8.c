/*===========================================================================*
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  RsSpecAn K8 - Bluetooth Analyzer Group Functions
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

 *===========================================================================*/

#include "rsspecan.h"

/*****************************************************************************
 * Function:    Bluetooth Mode
 * Purpose:     This function selects the Bluetooth analyzer mode
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BTOMode (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_BTO_MODE, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Select BTO Trace
 * Purpose:     This function selects the measurement curve for evaluation
 *              of the modulation characteristics.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectBTOTrace (ViSession instrSession,
                                           ViInt32 trace)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 3),
    		2, "Trace");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "TR%ld", trace);
    checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_BTO_TRAC, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTOMeasurement
 * Purpose:     This function configures the common parameters of measurement
 *              according Bluetooth standard.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOMeasurement (ViSession instrSession,
                                                    ViInt32 geographicalRegion,
                                                    ViInt32 channel,
                                                    ViInt32 outputPowerClass,
                                                    ViInt32 samplesPerSymbols,
                                                    ViInt32 packetType,
                                                    ViReal64 anntennaGain)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_GEOG, geographicalRegion),
    		2, "Geographical Region");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_CHANNEL, channel),
    		3, "Channel");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_PCL, outputPowerClass),
    		4, "Output Power Class");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_PRAT, samplesPerSymbols),
    		5, "Samples Per Symbol");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_PTYPE, packetType),
    		6, "Packet Type");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_EGA_INP, anntennaGain),
    		7, "Antenna Gain");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO AveragingMethod
 * Purpose:     This function configures the averaging area of the average
 *              power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOAveragingMethod (ViSession instrSession,
                                                        ViReal64 startPosition,
                                                        ViReal64 stopPosition)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Start", RSSPECAN_ATTR_BTO_POW_AVER, startPosition),
    		2, "Start Position");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Stop", RSSPECAN_ATTR_BTO_POW_AVER, stopPosition),
    		2, "Stop Position");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    ConfigureBTOSynchronization
 * Purpose:     This function configures the synchronization of the measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOSynchronization (ViSession instrSession,
                                                        ViBoolean findSync,
                                                        ViInt32 syncOffset,
                                                        ViBoolean findBurst,
                                                        ViReal64 burstOffset,
                                                        ViBoolean searchLengthAuto,
                                                        ViReal64 searchLength)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BTO_SEARCH_SYNC, findSync),
    		2, "Find Sync");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_SYNC_OFFS, syncOffset),
    		3, "Sync Offset");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BTO_SEARCH_PULS, findBurst),
    		4, "Find Burst");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_PULS_OFFS, burstOffset),
    		5, "Burst Offset");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BTO_SEARCH_TIME_AUTO, searchLengthAuto),
    		6, "Search Length Auto");
    if (!searchLengthAuto)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_SEARCH_TIME, searchLength),
        		7, "Search Length");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    ConfigureBTOLAP
 * Purpose:     This function configures the LAP for synchronization of the measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOLAP (ViSession instrSession,
                                                        ViInt32 lowest24Bits_ofDevAddress)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_LAP, lowest24Bits_ofDevAddress),
    		2, "Lowest 24 Bits of Dev. Address");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Measurement Mode
 * Purpose:     This function selects the current measurement according to the
 *              Bluetooth standard.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOMeasurementMode (ViSession instrSession,
                                                        ViInt32 measurementMode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if (!RsCore_IsInstrumentModel(instrSession, "FSL"))
    {
    	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementMode, 0, 4),
    			2, "Measurement Mode");
    }

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO, measurementMode),
    		2, "Measurement Mode");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function:    Configure BTO Number Of Adjacent Channels
 * Purpose:     This function selects the number of adjacent channel pairs
 *              during the measurement of the adjacent channel power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTONumberOfAdjacentChannels (ViSession instrSession,
                                                               ViInt32 number_ofAdjacentChannels)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_ACLR_ACP, number_ofAdjacentChannels),
    		2, "Number of Adjacent Channels");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Receive Filter
 * Purpose:     This function selects the receive filter for the signal to
 *              measure.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOReceiveFilter (ViSession instrSession,
                                                      ViInt32 filter)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_FILT_MEAS, filter),
    		2, "Filter");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Sweep Coupling Auto
 * Purpose:     This function activates auto coupling of selected coupling
 *              method.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOSweepCouplingAuto (ViSession instrSession,
                                                          ViInt32 sweepCoupling)
{
    ViStatus    error   = VI_SUCCESS;
    ViAttr      attribute = 0;

    checkErr(RsCore_LockSession(instrSession));

    switch (sweepCoupling)
    {
        case  RSSPECAN_VAL_COUPLING_RBW:
            attribute = RSSPECAN_ATTR_BTO_BWID_AUTO;
            break;

        case RSSPECAN_VAL_COUPLING_VBW:
            attribute = RSSPECAN_ATTR_BTO_VID_AUTO;
            break;

        case RSSPECAN_VAL_COUPLING_SWEEP_TIME:
            attribute = RSSPECAN_ATTR_BTO_SWE_TIME_AUTO;
            break;
        default:
            viCheckParm(VI_ERROR_PARAMETER3, 2, "Sweep Coupling");
    }

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", attribute, VI_TRUE),
    		1, "Dummy");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Sweep Coupling
 * Purpose:     This function configures the coupling value
 *              of the currently selected Bluetooth measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOSweepCoupling (ViSession instrSession,
                                                      ViInt32 sweepCoupling,
                                                      ViReal64 couplingValue)
{
    ViStatus    error   = VI_SUCCESS;
    ViAttr      attribute = 0;
    ViAttr      attribute2 = 0;

    checkErr(RsCore_LockSession(instrSession));

    switch (sweepCoupling)
    {
        case RSSPECAN_VAL_COUPLING_RBW:
            attribute = RSSPECAN_ATTR_BTO_BWID;
            attribute2 = RSSPECAN_ATTR_BTO_BWID_AUTO;
            break;

        case RSSPECAN_VAL_COUPLING_VBW:
            attribute = RSSPECAN_ATTR_BTO_VID;
            attribute2 = RSSPECAN_ATTR_BTO_VID_AUTO;
            break;

        case RSSPECAN_VAL_COUPLING_SWEEP_TIME:
            attribute = RSSPECAN_ATTR_BTO_SWE_TIME;
            attribute2 = RSSPECAN_ATTR_BTO_SWE_TIME_AUTO;
            break;
        default:
            viCheckParm(VI_ERROR_PARAMETER3, 2, "Sweep Coupling");
    }
    checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", attribute2, VI_FALSE));
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", attribute, couplingValue),
    		3, "Coupling Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Acquisition
 * Purpose:     This function configures the acquisition attributes of the
 *              currently selected Bluetooth measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOAcquisition (ViSession instrSession,
                                                    ViBoolean sweepModeContinuous,
                                                    ViInt32 numberOfSweeps)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win1", RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, sweepModeContinuous),
    		2, "Sweep Mode Continuous");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_SWE, numberOfSweeps),
    		3, "Number Of Sweeps");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Trace
 * Purpose:     This function configures the averaging area of the average
 *              power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOTrace (ViSession instrSession,
                                              ViInt32 trace,
                                              ViInt32 traceMode,
                                              ViInt32 traceDetector)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 3),
    		2, "Trace");
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "TR%ld", trace);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_BTO_TRAC_MOD, traceMode),
    		3, "Trace Mode");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_BTO_TRAC_DET, traceDetector),
    		4, "Trace Detector");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Packet Bytes SCO
 * Purpose:     This function selects the number of payload bytes that are
 *              transmitted in a packet.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOPacketBytesSCO (ViSession instrSession,
                                                       ViInt32 packetBytesSCO)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_PBSC, packetBytesSCO),
    		2, "Packet Bytes SCO");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO Display Y-Axis Scaling
 * Purpose:     This function defines the scaling of the y-axis in
 *              the currently selected unit.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTODisplayYAxisScaling (ViSession instrSession,
                                                            ViReal64 scaling)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_PDIV, scaling),
    		2, "Scaling");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:   Configure BTO Analog Demodulation Zoom
 * Purpose:    This function sets analog demodulation parameters such as
 *             zoom state, start time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOAnalogDemodulationZoom (ViSession instrSession,
                                                               ViBoolean zoomState,
                                                               ViReal64 startTime)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_BTO_ZOOM_STATE, zoomState),
    		2, "Zoom State");
    if (zoomState == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_ZOOM_START, startTime),
        		3, "Start Time");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- Measurement -------------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function:    Get BTO Mesurement Status
 * Purpose:     This function returns the status of a preceeding measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOMesurementStatus (ViSession instrSession,
                                                   ViInt32 *status)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_STAT, status),
    		2, "Status");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function:    Get BTO Packet Info
 * Purpose:     This function returns the packet info of of the packet
 *              analyzed by the preceeding measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOPacketInfo (ViSession instrSession,
                                             ViInt32 *type, ViInt32 *length)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_PTYP, type),
    		2, "Type");

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_PLEN, length),
    		3, "Length");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/*****************************************************************************
 * Function:    Get BTO Output Power
 * Purpose:     This function returns the output power results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOOutputPower (ViSession instrSession,
                                              ViInt32 type,
                                              ViReal64 *power)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_MEASTYPE_MIN, RSSPECAN_VAL_MEASTYPE_PEAK),
    		2, "Type");
    switch (type){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_BTO_OPOW_AVER, power),
            		3, "Power");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_BTO_OPOW_AVER, power),
            		3, "Power");
        break;
        case RSSPECAN_VAL_MEASTYPE_PEAK:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_OPOW_PEAK, power),
            		3, "Power");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function:    Get BTO Number Of Exceptions
 * Purpose:     This function returns the number of exceptions which occured
 *              during the adjacent channel power measurement according to
 *              the Bluetooth specification.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTONumberOfExceptions (ViSession instrSession,
                                                     ViInt32 *numberOfExceptions)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_ACLR_EXC, numberOfExceptions),
    		2, "Number Of Exceptions");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Power Of Channels
 * Purpose:     This function the power of selected adjacent channels of
 *              the Bluetooth standard.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOPowerOfChannels (ViSession instrSession,
                                                  ViInt32 arraySize,
                                                  ViReal64 power[],
                                                  ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K8"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 0, INT_MAX),
    		2, "Array Size");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, power), 3, "Power List");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, ":CALC:BTO:ACLR:LIST?", arraySize, power, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Frequency Deviation
 * Purpose:     This function returns the value of frequency deviation for
 *               varying bit patterns of the payload.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOFrequencyDeviation (ViSession instrSession,
                                                     ViInt32 frequencyDeviation,
                                                     ViInt32 type,
                                                     ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_MEASTYPE_MAXMIN, RSSPECAN_VAL_MEASTYPE_AVERMAX),
    		3, "Type");

    switch (type){
        case RSSPECAN_VAL_MEASTYPE_MAXMIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "F%ld,Min", frequencyDeviation);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_MCH_DF_MAX, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAXMAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "F%ld,Max", frequencyDeviation);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_MCH_DF_MAX, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAXAVER:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "F%ld,Aver", frequencyDeviation);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_MCH_DF_MAX, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVERMIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "F%ld,Min", frequencyDeviation);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_MCH_DF_AVER, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVERMAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "F%ld,Max", frequencyDeviation);
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_MCH_DF_AVER, value),
            		4, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Ratio Average Frequency Deviation
 * Purpose:     This function returns the value of ratio of the average
 *              frequency deviations for varying bit patterns of the payload.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTORatioAverageFrequencyDeviation (ViSession instrSession,
                                                                 ViInt32 type,
                                                                 ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if ((type!=RSSPECAN_VAL_MEASTYPE_MIN)&&(type!=RSSPECAN_VAL_MEASTYPE_MAX)&&(type!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Type");

    switch (type){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_BTO_MCH_RAT, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_BTO_MCH_RAT, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Aver", RSSPECAN_ATTR_BTO_MCH_RAT, value),
            		4, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Init Carr Freq Tol
 * Purpose:     This function returns value of the Initial Carrier
 *              Frequency Tolerance.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOInitCarrFreqTol (ViSession instrSession,
                                                  ViInt32 type,
                                                  ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if ((type!=RSSPECAN_VAL_MEASTYPE_MIN)&&(type!=RSSPECAN_VAL_MEASTYPE_MAX)&&(type!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Type");

    switch (type){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Min", RSSPECAN_ATTR_BTO_ICFT, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Max", RSSPECAN_ATTR_BTO_ICFT, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Aver", RSSPECAN_ATTR_BTO_ICFT, value),
            		4, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Percentage Deviation
 * Purpose:     This function returns the percentage of measurements of
 *              the frequency deviation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOPercentageDeviation (ViSession instrSession,
                                                      ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_MCH_DF2_PERC, value),
    		2, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Packet Data Bits
 * Purpose:     This function returns the packet data bits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOPacketDataBits (ViSession instrSession,
                                                 ViUInt32 maximumTime,
                                                 ViInt32 arrayLength,
                                                 ViInt32 *actualPoints,
                                                 ViReal64 values[])
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, maximumTime, 0, 4294967295UL), 2, "Maximum Time");

    checkErr(rsspecan_dataReadTrace (instrSession, 0, "BTOB", arrayLength,
                    values, actualPoints));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Read BTO FM Trace
 * Purpose:     This function returns FM trace with selected oversampling
 *              factor (Pointer per Symbol).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBTOFMTrace (ViSession instrSession,
                                           ViUInt32 maximumTime,
                                           ViInt32 arrayLength,
                                           ViInt32 *actualPoints,
                                           ViReal64 amplitude[])
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, maximumTime, 0, 4294967295UL), 2, "Maximum Time");

    checkErr(rsspecan_dataReadTrace (instrSession, 0, "BTOF", arrayLength,
                    amplitude, actualPoints));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Carr Freq Drift
 * Purpose:     This function returns the Carrier Frequency Drift.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOCarrFreqDrift (ViSession instrSession,
                                                ViInt32 type,
                                                ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if ((type!=RSSPECAN_VAL_MEASTYPE_NONE)&&(type!=RSSPECAN_VAL_MEASTYPE_MAX))
        viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Type");

    switch (type){
        case RSSPECAN_VAL_MEASTYPE_NONE:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_CFDR_RATE, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_CFDR_MAX, value),
            		4, "Value");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Adjust BTO SE Gate Setting
 * Purpose:  This function adjusts the gate settings for the In-band
 *           Spurious Emissions automatically.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustBTOSEGateSetting (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_BTO_IBS_GATE_AUTO, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Configure BTO SE Num Adjacent Channels
 * Purpose:  This function selects the number of adjacent channel pairs during
 *           the In-band Spurious Emissions measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOSENumAdjacentChannels (ViSession instrSession,
                                                              ViInt32 number)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_IBS_ACP, number),
    		2, "Number");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get BTO Channel Power of SE Measurement
 * Purpose:  This function returns the power of the TX channel in the In-band Spurious
 *           Emissions measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOChannelPowerOfSEMeasurement (ViSession instrSession,
                                                              ViInt32 parameter,
                                                              ViReal64 *value)
{
   ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (parameter){
        case RSSPECAN_VAL_BTO_SE_TX:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_IBS_TXCHANNEL, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_SE_HADJ_LOW:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Low", RSSPECAN_ATTR_BTO_IBS_HADJ, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_SE_HADJ_UPP:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "Upp", RSSPECAN_ATTR_BTO_IBS_HADJ, value),
            		4, "Value");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, parameter), 2, "Parameter");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get BTO SE Power Of Adjacent Channels
 * Purpose:  This function returns the power of all adjacent channels in the
 *           In-band Spurious Emissions measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOSEPowerOfAdjacentChannels (ViSession instrSession,
                                                          ViInt32 arraySize,
                                                          ViReal64 power[],
                                                          ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K8"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 0, INT_MAX),
    		2, "Array Size");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, power), 3, "Power List");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, ":CALC:BTO:IBS?", arraySize, power, returnedValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO SE Number of Exceptions
 * Purpose:     This function returns the number of results that exceeded
 *              the specified limits in the In-band Spurious Emissions measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOSENumberOfExceptions (ViSession instrSession,
                                                       ViInt32 *exceptions)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_IBS_EXC, exceptions),
    		2, "Exceptions");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Configure BTO CFS Block Count
 * Purpose:     This function sets the number of blocks to be measured.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTOCFSBlockCount (ViSession instrSession,
                                                      ViInt32 blockCount)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_CFST_BLOCK_COUNT, blockCount),
    		2, "Block Count");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Carrier Frequency Stability Deviation
 * Purpose:     This function returns value of the Carrier Frequency
 *              Stability Frequency Error.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOCFSDeviation (ViSession instrSession,
                                                             ViInt32 parameter,
                                                             ViInt32 type,
                                                             ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (type){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 3, "Type");
    }
    switch (parameter){
        case RSSPECAN_VAL_BTO_CFS_FERR_INITIAL:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_CFST_FERR_INITIAL, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_CFS_FERR_BLOCK:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_CFST_FERR_BLOCK, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_CFS_FERR_TOTAL:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_CFST_FERR_TOTAL, value),
            		4, "Value");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, parameter), 2, "Parameter");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Carrier Frequency Stability Modul. Accuracy
 * Purpose:     This function returns value of the Carrier Frequency
 *              Stability Modulation Accuracy.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTOCFSModulAccuracy (ViSession instrSession,
                                                                 ViInt32 parameter,
                                                                 ViInt32 type,
                                                                 ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (type){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 3, "Type");
    }
    switch (parameter){
        case RSSPECAN_VAL_BTO_CFS_DEVM_RMS:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_CFST_DEVM_RMS, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_CFS_DEVM_99PCT:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_CFST_DEVM_99PCT, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_CFS_DEVM_PEAK:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_BTO_CFST_DEVM_PEAK, value),
            		4, "Value");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, parameter), 2, "Parameter");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Differential Phase Encoding Values
 * Purpose:     This function returns the measured valeus of the differential
 *              phase encoding.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTODifferentialPhaseEncodingValues (ViSession instrSession,
                                                                  ViInt32 parameter,
                                                                  ViInt32 *value)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (parameter){
        case RSSPECAN_VAL_BTO_DPEN_TESTED:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_DPEN_PACKETS, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_DPEN_BER:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_DPEN_BER, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_DPEN_PASSED:
            viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BTO_DPEN_PACKETS_PASSED, value),
            		4, "Value");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, parameter), 2, "Parameter");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function:    Configure BTO RTP Power Measurement
 * Purpose:     This function configurest the start and stop time for power
 *              measurement on slected packet sections.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBTORTPMeasurement (ViSession instrSession,
                                                            ViInt32 packetSection,
                                                            ViReal64 start,
                                                            ViReal64 stop)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (packetSection){
        case RSSPECAN_VAL_BTO_RTP_GFSK:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "SECTION_TIME_GFSK,Start", RSSPECAN_ATTR_BTO_RTP_TIME, start),
            		3, "Start");

            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "SECTION_TIME_GFSK,Stop", RSSPECAN_ATTR_BTO_RTP_TIME, stop),
            		4, "Stop");
        break;
        case RSSPECAN_VAL_BTO_RTP_DPSK:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "SECTION_TIME_DPSK,Start", RSSPECAN_ATTR_BTO_RTP_TIME, start),
            		3, "Start");

            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "SECTION_TIME_DPSK,Stop", RSSPECAN_ATTR_BTO_RTP_TIME, stop),
            		4, "Stop");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, packetSection), 2, "Packet Section");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function:    Get BTO Relative Transmission Power
 * Purpose:     This function returns the transmission power of the Relative
 *              Transmission Power measurement.

 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBTORelativeTransmissionPower (ViSession instrSession,
                                                            ViInt32 parameter,
                                                            ViInt32 modificator,
                                                            ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (modificator){
        case RSSPECAN_VAL_MEASTYPE_MIN:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
        break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, modificator), 3, "Modificator");
    }
    switch (parameter){
        case RSSPECAN_VAL_BTO_RTP_DPSK:
            strcat(repCap, "SECTION_DPSK");
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_RTP, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_RTP_GFSK:
            strcat(repCap, "SECTION_GFSK");
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_RTP, value),
            		4, "Value");
        break;
        case RSSPECAN_VAL_BTO_RTP_RATIO:
            viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_BTO_RTP_RATIO, value),
            		4, "Value");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, parameter), 2, "Parameter");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- End Of RsSpecAn K8 - Bluetooth Analyzer Group Functions------------------*
 *****************************************************************************/
