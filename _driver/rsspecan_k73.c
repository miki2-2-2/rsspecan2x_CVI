/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver -
 *                              K73 - 3GPP FDD Code Domain Power Measurements
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

/*****************************************************************************
 *  K73 - 3GPP FDD Code Domain Power Measurements
 *****************************************************************************/

#include "rsspecan.h"
static ViString trace3GUEArr[]={"TRACE1","TRACE1","TRACE1","TRACE1","CTAB","TRACE2","TRACE2","TRACE2",
                                "TRACE2","ABIT","TRACE2","TRACE2","TRACE2","TRACE2","TRACE2","TRACE2","TRACE2",
                                "TRACE2","TRACE2","TRACE2","TRACE2","CEVM"};
static ViString result3GUECDPArr[]={"PTOT","FERR","TFR","MACC",
        "PCD","EVMR","EVMP","CERR","CSL","SRAT","CHAN","CDP","CDPR","IQOF",
        "IQIM","CMAP","PSYM","RHO","TOFF","EVMB","EVM","MTYP","ACH","MPIC","ARCD"};
/*===========================================================================*/
/* Function: Set 3GPP FDD UE Mode
/* Purpose:  This function switches the instrument into 3GPP FDD UE mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Set3GPPUEMode(ViSession  instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_3GUE_MODE, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Set  3GPP FDD UE Measurement
/* Purpose:  This function configures the 3GPP FDD UE additional
/*           measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Set3GPPUEMeasurement(ViSession   instrSession,
                                                ViInt32 measurement)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_MEAS, measurement),
    		2, "Measurement");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Measurement
/* Purpose:  This function configures the 3GPP FDD UE measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPUEMeasurement(ViSession instrSession,
                                                    ViInt32   scramblingCode,
                                                    ViInt32   scramblingType,
                                                    ViInt32   measurementBase,
                                                    ViInt32   slot,
                                                    ViInt32   sideBand,
                                                    ViBoolean normalize,
                                                    ViBoolean rrcFilter,
                                                    ViBoolean hsdpaupa,
                                                    ViBoolean eliminateTailChips,
                                                    ViReal64  inactiveChannelThreshold,
                                                    ViBoolean invertQ,
                                                    ViInt32 EVMMeasInterval)
{
    ViStatus    error = VI_SUCCESS;
    invertQ;
    inactiveChannelThreshold;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_SCR_CODE, scramblingCode),
    		2, "Scrambling Code");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_SCR_TYPE, scramblingType),
    		3, "Scrambling Type");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_BASE, measurementBase),
    		4, "Measurement Base");

    if (measurementBase == RSSPECAN_VAL_3GUE_CDP_BASE_FRAME)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_SLOT, slot),
        		5, "Slot");
    }

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_SIDEBAND, sideBand),
    		6, "Side Band");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_NORM, normalize),
    		7, "Normalize");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_RRC_FILTER, rrcFilter),
    		8, "RRC Filter");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_HSDPAUPA_STATE, hsdpaupa),
    		9, "HSDPA/UPA");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_ELIMINATE_TAIL_CHIPS, eliminateTailChips),
    		10, "Eliminate Tail Chips");
	if (!rsspecan_IsFSV (instrSession))
	{
    	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_EVM_MEAS_INTERVAL, EVMMeasInterval),
    			13, "EVM Meas Interval");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Multiframe
/* Purpose:  This function configures the 3GPP FDD BS Multiframe.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPUEMultiframe(ViSession  instrSession,
                                                    ViInt32 captureLength,
                                                    ViInt32 frameToAnalyze)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_MULTIFRAME_CAPTURE_LENGTH, captureLength),
    		2, "Capture Length");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_MULTIFRAME_FRAME_ANALYZE, frameToAnalyze),
    		3, "Frame To Analyze");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Results
/* Purpose:  This function configures the 3GPP FDD UE results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPUEResults(ViSession instrSession,
                                                ViInt32 selectIQ,
                                                ViInt32 channel,
                                                ViInt32 spreadingFactor)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_SEL_COMP, selectIQ),
    		2, "Select I/Q");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_CHAN_CODE, channel),
    		3, "Channel");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_3GUE_CDP_SFAC, spreadingFactor),
    		4, "Spreading Factor");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Results
/* Purpose:  This function configures the 3GPP FDD UE result type.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPUEResultType(ViSession instrSession,
                                                     ViInt32 resultType)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (resultType)
    {
        case RSSPECAN_VAL_3GUE_RES_CDP:
        case RSSPECAN_VAL_3GUE_RES_CDPRAT:
        case RSSPECAN_VAL_3GUE_RES_CDPOVER:
        case RSSPECAN_VAL_3GUE_RES_CDEP:
        case RSSPECAN_VAL_3GUE_RES_CTAB:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_3GUE_RESULT, resultType),
            		2, "Result Type");
        break;
        case RSSPECAN_VAL_3GUE_RES_SUM:
        case RSSPECAN_VAL_3GUE_RES_PCD:
        case RSSPECAN_VAL_3GUE_RES_MACC:
        case RSSPECAN_VAL_3GUE_RES_PVSLA:
        case RSSPECAN_VAL_3GUE_RES_PVSLR:
        case RSSPECAN_VAL_3GUE_RES_BSTR:
        case RSSPECAN_VAL_3GUE_RES_PVSY:
        case RSSPECAN_VAL_3GUE_RES_SYMBCONST:
        case RSSPECAN_VAL_3GUE_RES_SYMBEVM:
        case RSSPECAN_VAL_3GUE_RES_FVSL:
        case RSSPECAN_VAL_3GUE_RES_PSVSL:
        case RSSPECAN_VAL_3GUE_RES_EVM_MAGN:
        case RSSPECAN_VAL_3GUE_RES_EVM_PHAS:
        case RSSPECAN_VAL_3GUE_RES_COMP_CONS:
        case RSSPECAN_VAL_3GUE_RES_CHIP_EVM:
        case RSSPECAN_VAL_3GUE_RES_CHIP_MAGN:
        case RSSPECAN_VAL_3GUE_RES_CHIP_PHAS:
		    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_3GUE_RESULT, resultType),
		    		2, "Result Type");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 2, "Result Type");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Channel Table Operations
/* Purpose:  This function performs the selected operation with channel
/*           table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableOperations(ViSession    instrSession,
                                                            ViInt32 operation,
                                                            ViString    channelTable)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidNullPointer(instrSession, channelTable), 3, "Channel Table");

    switch(operation){
        case RSSPECAN_VAL_CDP_TABLE_OFF:
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB, VI_FALSE));
        break;
        case RSSPECAN_VAL_CDP_TABLE_SELECT:
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB, VI_TRUE));
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB_SEL, channelTable),
            		3, "Channel Table");
        break;
        case RSSPECAN_VAL_CDP_TABLE_NEW:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB_NAME, channelTable),
            		3, "Channel Table");
        break;
        case RSSPECAN_VAL_CDP_TABLE_DELETE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB_DEL, channelTable),
            		3, "Channel Table");
        break;
        case RSSPECAN_VAL_CDP_TABLE_COPY:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB_COPY, channelTable),
            		3, "Channel Table");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, operation), 2, "Operation");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Channel Table Comment
/* Purpose:  This function defines a comment for the selected channel table
/*           in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableComment(ViSession   instrSession,
                                                        ViString    channelTableComment)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB_COMMENT, channelTableComment),
    		2, "Channel Table Comment");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Channel Table Data
/* Purpose:  This function defines the values of the selected channel table
/*           in the code domain power measurement mode. Six values are specified
/*           for a line of table.
/*
/*           Prior to this function, the name of the channel table has to be
/*           defined with function 3GPP FDD UE CDP Channel Table Operations
/*           (ChannelTable3GPPFDDUEOperations).
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableData(ViSession  instrSession,
                                                    ViInt32 arraySize,
                                                    ViInt32 codeClass[],
                                                    ViInt32 codeNumber[],
                                                    ViInt32 channelMapping[],
                                                    ViInt32 pilotLength[],
                                                    ViBoolean   status[],
                                                    ViReal64    cdpRelative[])
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     i = 0;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *pbuffer;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K73"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 1, INT_MAX),
    		2, "Array Size");
    pbuffer=buffer;
    pbuffer += sprintf (pbuffer, "CONF:WCDP:MS:CTAB:DATA %ld,%ld,%ld,%ld,%ld,%.12f",
                codeClass[i], codeNumber[i], channelMapping[i], pilotLength[i], status[i],
                cdpRelative[i]);

    for (i=1;i<arraySize; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%ld,%ld,%ld,%.12f",
            codeClass[i], codeNumber[i], channelMapping[i], pilotLength[i], status[i],
                cdpRelative[i]);

    checkErr(RsCore_Write(instrSession, buffer));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD UE CDP Channel Table High Speed Channel
/* Purpose:  This function activates/deactivates the high speed channel in
/*           active channel table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableHighSpeedChannel(ViSession  instrSession,
                                                                ViBoolean   state)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_3GUE_WCDP_CTAB_HSDP, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Query 3GPP FDD UE CDP Channel Table Catalog
/* Purpose:  This function reads out the names of all channel tables stored
/*           on the harddisk in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Query3GPPFDDUEChannelTableCatalog(ViSession   instrSession,
                                                        ViInt32*    numberofChannelTables,
                                                        ViInt32 bufferSize,
                                                        ViChar  channelTablesList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *buf=NULL;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K73"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
    		3, "Buffer Size");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, channelTablesList), 4, "Channel Table List");

    checkErr(RsCore_QueryViStringUnknownLength(instrSession, ":CONF:WCDP:MS:CTAB:CAT?", &buf));
    checkErr(RsCore_ParseCatalog(buf, bufferSize, channelTablesList, numberofChannelTables));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if (buf) free(buf);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Get 3GPP FDD UE Trace
/* Purpose:  This function initiates a signal acquisition based on the
/*           present instrument configuration.  It then waits for the
/*           acquisition to complete, and returns the trace as an array of
/*           amplitude values. The amplitude array returns data that
/*           represent the amplitude of the signals of the sweep from the
/*           start frequency to the stop frequency (in frequency domain, in
/*           time domain the amplitude array is ordered from beginning of
/*           sweep to end).  The Amplitude Units attribute determines the
/*           units of the points in the amplitude array.  This function
/*           resets the sweep count.
/*
/*           Notes:
/*
/*           (1) If the spectrum analyzer did not complete the acquisition
/*           within the time period the user specified with the maxTime
/*           parameter, the function returns Max Time Exceeded error.
/*
/*           (2) If the data was captured in an uncalibrated spectrum
/*           analyzer mode, the function will generate a Measure Uncalibrated
/*           warning flag.
/*
/*           Attribute(s):
/*           no attribute
/*
/*           Remote-control command(s):
/*           INITiate[:IMMediate]
/*           TRACe<1|2>[:DATA] ABITstream | CWCDp | TPVSlot | CTABle
/*           :FORM REAL,32
/*           *CLS
/*
/*
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPUETrace(ViSession instrSession,
                                        ViInt32 window,
                                        ViInt32 trace,
                                        ViInt32 arrayLength,
                                        ViInt32*    actualPoints,
                                        ViReal64    values[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *exBuf = NULL;

    checkErr(RsCore_LockSession(instrSession));

     checkErr(RsCore_CheckInstrumentOptions(instrSession, "K73"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, RSSPECAN_VAL_3GUE_TRAC_CDP, RSSPECAN_VAL_3GUE_TRAC_EVM_RMS),
    		3, "Trace");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, actualPoints), 5, "Actual Points");

    checkErr(rsspecan_dataReadTrace (instrSession, window, trace3GUEArr[trace], arrayLength,
                    values, actualPoints));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if (exBuf) free (exBuf);
    (void)RsCore_UnlockSession(instrSession);

    return error;
}

/*===========================================================================*/
/* Function: Get 3GPP FDD UE CDP Result
/* Purpose:  This function returns the Phase Spot Noise Y Position.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPUEResult(ViSession    instrSession,
                                            ViInt32     type,
                                            ViReal64*   result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K73"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_CDP_RES_PTOT, RSSPECAN_VAL_CDP_RES_ARCD),
    		2, "Type");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, result), 3, "Result");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:MARK:FUNC:WCDP:MS:RES? %s", result3GUECDPArr[type]);
    checkErr(RsCore_QueryViReal64(instrSession, cmd, result));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/*****************************************************************************
 *- User defined callbacks --------------------------------------------------*
 *****************************************************************************/
