/*****************************************************************************
 *- RSSpecAn K82 K83 K84 K85 - CDMA -----------------------------------------*
 *****************************************************************************/

#include "rsspecan.h"

#pragma warning(disable:4100) //unreferenced formal parameter

static ViString traceC2KArr[] = {"CTAB",VI_NULL};
static ViString analyzerResultSummaryArr[] = {"SLOT","PTOT","PPIC","RHO", "MACC",
                                              "PCD" ,"IQIM","IQOF","FERR","FERP",
                                              "CERR","TFR", "ACT", "SRAT","CHAN",
                                              "SFAC","TOFF","POFF","CDPR","CDP",
                                              "EVMR","EVMP","RHOP","RHO1","RHO2",
                                              "PPIL","PMAC","PDAT","PPR", "DMTY",
                                              "MACT","DACT","PLEN","PDM", "PDM",
                                              "IPMM","MTYP","PRRI","DRP", "RHO",
                                              "RHOD","RHOM",
                                              "PNF","PAV","TAU","TACT","EVMP",
                                              "CDEP","CDER","CODM","CODP",
                                               VI_NULL};

/*****************************************************************************
 * Function: Adjust C2K Settings
 * Purpose:  This function adjusts the frequency span, the measurement
 *           bandwidths and the detector as required for the number of
 *           channels, the channel bandwidths and the channel spacings selected
 *           in the active power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustC2KSettings(ViSession  instrSession,
                                             ViInt32    window,
                                             ViInt32    adjustSettings, 
                                             ViUInt32   timeout)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Timeout");
    sprintf (buffer, "Win%ld", window);
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));        
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_C2K_ACH_PRES, adjustSettings));


Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Adjust C2K Reference Level
 * Purpose:  This function adapts the reference level to the measured channel
 *           power and - if required - switches on previously the adjacent
 *           channel power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustC2KReferenceLevel(ViSession    instrSession,
                                                   ViInt32      window, 
                                                   ViUInt32     timeout)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");
    sprintf (buffer, "Win%ld", window);
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));        
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                              RSSPECAN_ATTR_C2K_ADJUST_REF_LEVEL, VI_NULL));


Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Select C2K Measurement
 * Purpose:  This function selects the CDMA2000 Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectC2KMeasurement(
    ViSession   instrSession,
    ViInt32 measurementSelect
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if ((measurementSelect == RSSPECAN_VAL_C2K_MEAS_SEL_PVTIME) && (!strstr (buffer, "K84")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_MEAS_SELECT, measurementSelect));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K PCG Settings
 * Purpose:  This function sets the PCG Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KPCGSettings( ViSession   instrSession,
                                                    ViInt32 setCount,
                                                    ViInt32 setToAnalyze)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "FSQ")) 
    {
        viCheckErr (RS_ERROR_INSTRUMENT_MODEL);  
    }
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_SET_COUNT,
                 setCount), 2, "Set Count");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_SET_TO_ANALYZE,
                 setToAnalyze), 3, "Set To Analyze");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Restore C2K SEM Standard Lines
 * Purpose:  This function restores the standard limit lines for the spectrum
 *           emission mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreC2KSEMStandardLines(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_C2K_LIM_RESTORE, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K Signal Statistics Measurement
 * Purpose:  This function configures parameters of signal statistic
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KSignalStatisticMeasurement (ViSession instrSession,
                                                                   ViInt32 measurement,
                                                                   ViInt32 numberofSamples)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (measurement, 0, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Measurement");
 
    switch (measurement){
        case RSSPECAN_VAL_SSTAT_OFF:
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_APD_STATE,
                                                VI_FALSE));
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE,
                                                VI_FALSE));
        break;        
        case RSSPECAN_VAL_SSTAT_APD:      
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_APD_STATE,
                                                VI_TRUE));                                                
        break;
        case RSSPECAN_VAL_SSTAT_CCDF:      
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE,
                                                VI_TRUE));
        break;
        
    }
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_SAMPLES,
                                                numberofSamples), 3, "Number Of Samples"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K Signal Statistics Percent Marker
 * Purpose:  This function configures the percent marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KSignalStatisticsPercentMarker(
    ViSession   instrSession,
    ViInt32 window, 
    ViInt32 marker,
    ViReal64 percentMarker
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    sprintf (buffer, "Win%ld,M%ld", window, marker);

    viCheckErr( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                               RSSPECAN_ATTR_C2K_PERCENT_MARKER, 
                                               percentMarker));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure C2K Channel Table Operations
/* Purpose:  This function performs the selected operation with channel
/*           table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureC2KChannelTableOperations(
    ViSession   instrSession,
    ViInt32 operation,
    ViString    channelTable)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (operation, 0, 4))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Operation");
    if (!channelTable)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Table");

    switch(operation)
    {
        case RSSPECAN_VAL_TABLE_OFF:
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_STATE,
                VI_FALSE));
        break;
        case RSSPECAN_VAL_TABLE_SELECT:
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_STATE,
                VI_TRUE));
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_SELECT,
                channelTable), 3, "Channel Table");
        break;
        case RSSPECAN_VAL_TABLE_NEW:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_NAME,
                channelTable), 3, "Channel Table");
        break;
        case RSSPECAN_VAL_TABLE_DELETE:
            viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                              RSSPECAN_ATTR_C2K_CTABLE_DELETE, VI_NULL));
        break;
        case RSSPECAN_VAL_TABLE_COPY:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_COPY,
                channelTable), 3, "Channel Table");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Operation");
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Restore C2K Channel Table
 * Purpose:  This function restores the predefined tables. They are restored
 *           to the values they had when the device was delivered. With this
 *           restore feature an undo of overwriting these channel tables is
 *           possible.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreC2KChannelTable(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                   RSSPECAN_ATTR_C2K_CTABLE_RESTORE, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure CDMA2000 Channel Table Comment
 * Purpose:  This function defines a comment for the selected channel table in
 *           the code domain power measurement mode.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure C2K Channel Table Operations
 *           (ConfigureC2KChannelTableOperations) and the values of the table
 *           have to be defined with one of the functions Configure C2K BST
 *           Channel Table Data (ConfigureBC2KChannelTableData), Configure
 *           C2K MST Channel Table Data (ConfigureM2KChannelTableData),
 *           Configure EV-DO BST Channel Table Data
 *           (ConfigureBDOChannelTableData), or Configure EV-DO MST Channel
 *           Table Data (ConfigureMDOChannelTableData).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KChannelTableComment(
    ViSession   instrSession,
    ViString    channelTableComment
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_C2K_CTABLE_COMMENT,
                                        channelTableComment), 2, "Channel Table Comment");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query CDMA2000 Channel Table Catalog
 * Purpose:  This function reads out the names of all channel tables stored on
 *           the harddisk in the code domain power measurement mode.
 *
 *           Notes:
 *
 *           (1) Driver returns an instrument specific error, when catalog
 *           list is empty.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryC2KChannelTableCatalog(
    ViSession   instrSession,
    ViInt32*    numberofChannelTables,
    ViInt32 bufferSize,
    ViChar  channelTablesList[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViUInt32    retCnt;
    ViChar      *buf=VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K82")||strstr (buffer, "K83")||strstr (buffer, "K84")||strstr (buffer, "K85")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");
    if ((bufferSize > 0) && (channelTablesList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Table List");

    viCheckErr (viPrintf (instrSession, "CONF:CDP:CTAB:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, channelTablesList, numberofChannelTables));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (buf) free(buf);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K Basic Settings
 * Purpose:  This function configures the basic settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KBasicSettings(
    ViSession   instrSession,
    ViInt32     bandClass,
    ViBoolean   timePhase,
    ViReal64    inactChannelThreshold,
    ViBoolean   invertQ,
    ViInt32     sideBand,
    ViBoolean   normalize
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";   


    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, model));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_BCLASS,
                 bandClass), 2, "Band Class");

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_CDP_TPMEAS,
                 timePhase), 3, "Time Phase");

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_ICTHRES,
                 inactChannelThreshold), 4, "Inact Channel Threshold");

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_CDP_QINVERT,
                 invertQ), 5, "Invert Q");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_SBAND,
                 sideBand), 6, "Side Band");

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_CDP_NORMALIZE,
                 normalize), 7, "Normalize");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
 
/// HIFN This function defines how the pilot channel power is displayed in the Result Summary.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR mode/This control selects the pilot channel display mode.
ViStatus _VI_FUNC rsspecan_ConfigureC2KPowerDisplay (ViSession instrSession,
                                            ViInt32 mode)
{
	ViStatus    error   = VI_SUCCESS;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
											   RSSPECAN_ATTR_C2K_CDP_DISPLAY,
								               mode), 2, "Mode"); 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure CDMA2000 Amplitude Y Scale Division
 * Purpose:  This function defines the scaling of the Y axis in the current
 *           unit.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureC2KAmplitudeYScaleDivision(
    ViSession   instrSession,
    ViReal64    yScaleDiv
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_PDIV,
                 yScaleDiv), 2, "Y Scale Div");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Adjust CDMA2000 Amplitude Reference Level
 * Purpose:  This function adjusts reference level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustC2KAmplitudeReferenceLevel(ViSession   instrSession,
                                                            ViUInt32 timeout)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout"); 
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                 RSSPECAN_ATTR_C2K_CDP_LEVEL_ADJUST, VI_NULL));

Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read C2K Trace Data
 * Purpose:  This function reads trace data out of the instrument. Only TRACE1
 *           or TRACE2 can be queried depending on the display mode.
 *
 *           Notes:
 *
 *           (1) This function is available for option FS-K82, FS-K83, FS-K84
 *           or FS-K85 only.
 *
 *           (2) The associated measurement window is selected with the
 *           function Display Set Active Window (rsspecan_DisplaySetActiveWindow).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadC2KTraceData(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     sourceTrace,
    ViInt32     arrayLength,
    ViInt32*    actualPoints,
    ViReal64    values[]
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      *exBuf = VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K82")||strstr (buffer, "K83")||strstr (buffer, "K84")||strstr (buffer, "K85")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (rsspecan_invalidViInt32Range (sourceTrace, 1, 2))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Source Trace");
    if (!actualPoints)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Actual Points");

    sprintf (buffer, "TRACE%ld", sourceTrace);
    viCheckErr (rsspecan_dataReadTrace (instrSession, window, buffer, arrayLength,
                    values, actualPoints));


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (exBuf) free (exBuf);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read C2K CDP Trace Data
 * Purpose:  This function reads trace data out of the instrument. Data can be
 *           queried depending on the display mode.
 *
 *           (1) CTABLE:
 *           In addition to the results of the channel table which are output
 *           using the :TRACe[:DATA]? TRACE1 command, active timing and phase
 *           offset measurement (refer to CDPower:TPMeas) also has the CTABle
 *           query command which displays the maximum values of the TIMING and
 *           PHASE OFFSET together with the associated channel.
 *
 *           Option FS-K82, FS-K83 - the following values are output:
 *
 *           <Max. time offset in s>, <Code number for max. time>, <Code
 *           class for max. time>, <Max. phase offset in rad>, <Code number
 *           for max. phase>, <Code class for max. phase>, <Reserved 1>, ...,
 *           <Reserved 6>
 *
 *           Option FS-K84 - the following values are output:
 *
 *           <Max. time offset in s>, <Channel Type>, <Code number for max.
 *           time>, <Code class for max. time>, <Max. phase offset in rad>,
 *           <Channe type>, <Code number for max. phase>, <Code class for max.
 *           phase>, <Reserved 1>, ..., <Reserved 4>
 *
 *           Option FS-K85 - the following values are output:
 *
 *           <Max. time offset in s>, <Code number for max. time>, <Code class
 *           for max. time>, <Max. phase offset in rad>, <Code number for max.
 *           phase>, <Code class for max. phase>, <Reserved 1>, ..., <Reserved
 *           6>
 *
 *           Notes:
 *
 *           (1) This function is available for option FS-K82, FS-K83, FS-K84
 *           or FS-K85 only.
 *
 *           (2) The associated measurement window is selected with the
 *           function Display Set Active Window (rsspecan_DisplaySetActiveWindow).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadC2KCDPTraceData(
    ViSession   instrSession,
    ViInt32 window,
    ViInt32 sourceTrace,
    ViInt32 arrayLength,
    ViInt32*    actualPoints,
    ViReal64    values[]
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      *exBuf = VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

     if (!(strstr (buffer, "K82")||strstr (buffer, "K83")||strstr (buffer, "K84")||strstr (buffer, "K85")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (sourceTrace != RSSPECAN_VAL_C2K_CTABLE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Source Trace");
    if (!actualPoints)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Actual Points");

    sprintf (buffer, "%s", traceC2KArr[sourceTrace]);
    viCheckErr (rsspecan_dataReadTrace (instrSession, window, buffer, arrayLength,
                    values, actualPoints));


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (exBuf) free (exBuf);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: C2K BST Mode
 * Purpose:  This function selects the CDMA2000 Base Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BC2KMode(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                 RSSPECAN_ATTR_C2K_MODE, VI_NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BTS SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KSEM(
    ViSession   instrSession,
    ViInt32     limitLineType,
    ViReal64    limitLineValue,
    ViReal64    _30kHz1MHzTransition
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_MODE,
                 limitLineType), 2, "Limit Line Type");

    if (limitLineType == RSSPECAN_VAL_C2K_LIM_MODE_MAN)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_VAL,
                     limitLineValue), 3, "Limit Line Value");
    }
    
    if (!rsspecan_IsFSV (instrSession))
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS,
	                 _30kHz1MHzTransition), 4, "30kHz/1MHz Transition");
	}

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Code Domain Analyzer Results
 * Purpose:  This function selects the measured data that will be displayed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KCodeDomainAnalyzerResults(ViSession   instrSession,
                                                           ViInt32 resultType)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));
    switch (resultType)
    {
        case RSSPECAN_VAL_C2K_FEED_CDP:
        case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
        case RSSPECAN_VAL_C2K_FEED_CDEP:
        case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
        case RSSPECAN_VAL_C2K_FEED_CPOW: 
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");    
        break;      
        case RSSPECAN_VAL_C2K_FEED_MACC: 
        case RSSPECAN_VAL_C2K_FEED_PVSL:
        case RSSPECAN_VAL_C2K_FEED_PVSY:
        case RSSPECAN_VAL_C2K_FEED_BSTR:
        case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
        case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
        case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
        case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
        case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");  
        break;              
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");     
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KCodeDomainAnalyzerSettings(
    ViSession   instrSession,
    ViInt32 captureLength,
    ViInt32 selectChannel,
    ViInt32 selectPCG
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));


    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (!strstr (buffer, "FSP")) // not FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSQ_FSU,
                 captureLength), 2, "Capture Length");
    }
    else // is FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSP,
                 captureLength), 2, "Capture Length");
    }

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT,
                 selectPCG), 4, "Select PCG");
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_CODE,
                 selectChannel), 3, "Select Channel");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute 
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KChannelTableData(
    ViSession   instrSession,
    ViInt32     channelType[],
    ViInt32     codeClass[],
    ViInt32     codeNumber[],
    ViInt32     radioConfiguration[],
    ViInt32     reserved1[],
    ViInt32     reserved2[],
    ViBoolean   status[],
    ViReal64    cdpRelative[],
    ViInt32     numofElements
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     i = 0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K82"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (numofElements<=0)
      viCheckParm(RS_ERROR_INVALID_PARAMETER, 10, "Num of Elements");
    pbuffer = buffer;
    pbuffer += sprintf (pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,0,0,%ld,%.12f",
                channelType[i], codeClass[i], codeNumber[i], radioConfiguration[i], status[i],
                cdpRelative[i]);

    for (i=1;i<numofElements; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%ld,%ld,0,0,%ld,%.12f",
            channelType[i], codeClass[i], codeNumber[i], radioConfiguration[i], status[i],
                cdpRelative[i]);

    viCheckErr (viPrintf (instrSession,"%s\n", buffer));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Advanced Settings
 * Purpose:  This function configures the advanced settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KAdvancedSettings(
    ViSession   instrSession,
    ViInt32 pnOffset,
    ViInt32 baseSpreadingFactor,
    ViInt32 powerReference,
    ViInt32 order
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_PNOFFSET,
                 pnOffset), 2, "PN Offset");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_SFACTOR,
                 baseSpreadingFactor), 3, "Base Spreading Factor");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_PREFERENCE,
                 powerReference), 4, "Power Reference");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_ORDER,
                 order), 5, "Order");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: Configure C2K BST Force 1xEV-DV
 * Purpose:  This function switches the 1xEV-DV system.
 *
 *           Note(s):
 *
 *           (1) This is necessary in the automatic search mode to support
 *           the channel detection. If there is a QPSK modulated channel with
 *           spreadingfactor 32 it is not possible to distinguish whether it
 *           is a data channel (CHAN with radio configuration 35) or a
 *           PACKET DATA CHANNEL (PDCH with radio configuration 10). This
 *           decision is made for the PDCH of the 1xEVDV system if the value
 *           is ON. If the value is OFF (default) the CHAN is detected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KForce1xEVDV(
    ViSession   instrSession,
    ViBoolean   _1xEVDV
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_FORCE_DV,
                 _1xEVDV), 2, "1xEV-DV");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Antenna Settings
 * Purpose:  This function configures the Antenna Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KAntennaSettings(
    ViSession   instrSession,
    ViBoolean   antennaDiversity,
    ViInt32     antennaNumber
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if(antennaDiversity == RSSPECAN_VAL_C2K_ANTENNA_OFF)
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_ANTENNA,
                 antennaDiversity), 2, "Antenna Diversity");
    }
    else
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_ANTENNA,
                 antennaNumber), 3, "Antenna Number");
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST Multi Carrier
 * Purpose:  This function configures the Multi Carrier settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KMultiCarrier(
    ViSession   instrSession,
    ViBoolean   multiCarrier,
    ViBoolean   enhancedAlgorithm,
    ViBoolean   mcFilter,
    ViInt32     filterType,
    ViReal64    rollOff,
    ViReal64    cutOffFrequency
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_STATE,
                 multiCarrier), 2, "Multi Carrier");

    if (multiCarrier == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                    RSSPECAN_ATTR_C2K_MCARRIER_ENH_ALG, enhancedAlgorithm),
                    3, "Enhanced Algorithm");

        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                    RSSPECAN_ATTR_C2K_MCARRIER_FILTER_STATE, mcFilter),
                    4, "MC Filter");

        if (mcFilter == VI_TRUE)
        {
            viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                        RSSPECAN_ATTR_C2K_MCARRIER_FILTER_TYPE, filterType),
                        5, "Filter Type");

            if (filterType == RSSPECAN_VAL_C2K_MCARRIER_FTYPE_RRC)
            {
                viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                            RSSPECAN_ATTR_C2K_MCARRIER_ROLL_OFF, rollOff),
                            6, "Roll Off");

                viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                            RSSPECAN_ATTR_C2K_MCARRIER_CUT_OFF_FREQ, cutOffFrequency),
                            7, "Cut Off Frequency");
            }
        }
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST PICH Marker Settings
 * Purpose:  This function sets selected marker to channel 0.64.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KPichMarkerSettings(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "C1,M1",
                                               RSSPECAN_ATTR_C2K_MARKER_PICH, VI_NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K BST TDPICH Marker Settings
 * Purpose:  This function sets selected marker to channel 16.128.
 *
 *           Notes:
 *
 *           (1) The base spreading factor (RSSPECAN_ATTR_C2K_CDP_SFACTOR
 *           attribute) should be set to 128 before hand.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBC2KTDPichMarkerSettings(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "C1,M1",
                                               RSSPECAN_ATTR_C2K_MARKER_TDPICH, VI_NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Store C2K BTS SEM To File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreBC2KSEMToFile(
    ViSession   instrSession,
    ViString    fileName
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_C2K_LIM_STORE_FILE, 
                            fileName), 2, "File Name");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read C2K BTS SEM Peak List
 * Purpose:  This function returns the Spectrum EM Mask View Peak List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBC2KSEMPeakList(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     numberOfValues,
    ViReal64    frequency[],
    ViReal64    level[],
    ViReal64    deltaLimit[],
    ViInt32*    returnedValues
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     arrayLength;
    ViReal64    *data = VI_NULL;
    ViInt32     retVal;
    ViInt32     i;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    arrayLength = 3*numberOfValues;
    data=malloc (sizeof(ViReal64)*arrayLength);

    viCheckErr (rsspecan_dataReadTrace (instrSession, window, "FINal1", arrayLength,
                    data, &retVal));

    if (returnedValues)
        *returnedValues = retVal / 3; /* Three result arrays */

    retVal = (retVal>arrayLength)?arrayLength:retVal;

    retVal /=3;

    for (i = 0; i < retVal; i++)
    {
        frequency[i] = data[i*3];
        level[i] = data[i*3+1];
        deltaLimit[i] = data[i*3+2];
    }


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (data) free(data);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query C2K BST Code Domain Analyzer Result Summary
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryBC2KCodeDomainAnalyzerResultSummary(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32 resultType,
    ViReal64*   result
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *buf=VI_NULL; 
    
    ViUInt32    local_count     = 0;   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K82"))       
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_SLOT, RSSPECAN_VAL_C2K_EVMPEAK)&&rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_DMTYPE, RSSPECAN_VAL_C2K_DMTYPE))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");

    viCheckErr (viPrintf (instrSession, "CALC%ld:MARK:FUNC:CDP:RES? %s\n",
                window, analyzerResultSummaryArr[resultType]));

    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &local_count));
            
    if ((strstr (buf, "Sync")) != NULL)
        *result =   RSSPECAN_NAN;
    else
        *result = atof(buf);
        
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: C2K MST Mode
 * Purpose:  This function selects the CDMA2000 Mobile Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MC2KMode(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                 RSSPECAN_ATTR_MC2K_MODE, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K MST SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KSEM(
    ViSession   instrSession,
    ViInt32 limitLineType,
    ViReal64    _30kHz1MHzTransition
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_LIM_MODE,
                 limitLineType), 2, "Limit Line Type");

    if (!rsspecan_IsFSV (instrSession))
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS,
	                 _30kHz1MHzTransition), 3, "30kHz/1MHz Transition");
	}

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: ConfigureMC2KCodeDomainAnalyzerResults
 * Purpose:  This function configures the Code Domain Analyzer Results Type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCodeDomainAnalyzerResults(ViSession   instrSession,
                                                                  ViInt32 resultType)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    switch (resultType)
    {
        case RSSPECAN_VAL_C2K_FEED_CDP:
        case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
        case RSSPECAN_VAL_C2K_FEED_CDEP:
        case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");    
        break;      
        case RSSPECAN_VAL_C2K_FEED_MACC: 
        case RSSPECAN_VAL_C2K_FEED_PVSL:
        case RSSPECAN_VAL_C2K_FEED_PVSY:
        case RSSPECAN_VAL_C2K_FEED_BSTR:
        case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
        case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
        case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
        case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
        case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");  
        break;              
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");     
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K MST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCodeDomainAnalyzerSettings(ViSession   instrSession,
                                                                    ViInt32 captureLength,
                                                                    ViInt32 selectChannel,
                                                                    ViInt32 selectPCG,
                                                                    ViInt32 selectMapping)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K83"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "FSP")) // not FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_IQLENGTH_FSQ_FSU,
                 captureLength), 2, "Capture Length");
    }
    else // is FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_IQLENGTH_FSP,
                 captureLength), 2, "Capture Length");
    }
   
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_CODE,
                 selectChannel), 3, "Select Channel");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT,
                 selectPCG), 4, "Select PCG");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING,
                 selectMapping), 5, "Select Mapping");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K MST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute 
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KChannelTableData(
    ViSession   instrSession,
    ViInt32     channelType[],
    ViInt32     codeClass[],
    ViInt32     codeNumber[],
    ViInt32     mapping[],
    ViInt32     reserved1[],
    ViInt32     reserved2[],
    ViBoolean   status[],
    ViReal64    cdpRelative[],
    ViInt32     numofElements
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     i = 0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K83"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (numofElements<=0)
      viCheckParm(RS_ERROR_INVALID_PARAMETER, 10, "Num of Elements");
    
    pbuffer = buffer;
    pbuffer += sprintf (pbuffer,  "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,0,0,%ld,%.12f",
                channelType[i], codeClass[i], codeNumber[i], mapping[i], status[i],
                cdpRelative[i]);

    for (i=1;i<numofElements; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%ld,%ld,0,0,%ld,%.12f",
            channelType[i], codeClass[i], codeNumber[i], mapping[i], status[i],
                cdpRelative[i]);

    viCheckErr (viPrintf (instrSession,"%s\n", buffer));


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K MST Advanced Settings
 * Purpose:  This function configures the Advanced Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KAdvancedSettings(
    ViSession   instrSession,
    ViInt32     baseSpreadingFactor,
    ViInt32     powerReference,
    ViInt32     order,
    ViInt32     selectIQ,
    ViInt32     longCodeMode,
    ViString    longCodeMask,
    ViString    longCodeOffset
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_SFACTOR,
                 baseSpreadingFactor), 2, "Base Spreading Factor");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_PREFERENCE,
                 powerReference), 3, "Power Reference");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_ORDER,
                 order), 4, "Order");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING,
                 selectIQ), 5, "Select IQ");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_LCODE_MODE,
                 longCodeMode), 6, "Long Code Mode");

    if (longCodeMode == RSSPECAN_VAL_MC2K_LCODE_STANDARD)
    {
        viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_LCODE_MASK,
                     longCodeMask), 7, "Long Code Mask");

        viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_LCODE_OFFSET,
                     longCodeOffset), 8, "Long Code Offset");

    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K MST CDP Overview
 * Purpose:  This function configures the CDP Overview
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCDPOverview(
    ViSession   instrSession,
    ViBoolean   codeDomainOverview)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_OVERVIEW,
                 codeDomainOverview), 2, "Code Domain Overview");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure C2K MST PICH Marker Settings
 * Purpose:  This function sets marker to channel 0.16, 0.32 or 0.64,
 *           depending on the base spreading factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMC2KPichMarkerSettings(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "C1,M1",
                                               RSSPECAN_ATTR_MC2K_MARKER_PICH, VI_NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query C2K MST Code Domain Analyzer Result Summary
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMC2KCodeDomainAnalyzerResultSummary(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32 resultType,
    ViReal64*   result
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K83"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_SLOT, RSSPECAN_VAL_C2K_EVMPEAK))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");

    viCheckErr (viQueryf (instrSession, "CALC%ld:MARK:FUNC:CDP:RES? %s\n", "%le",
                window, analyzerResultSummaryArr[resultType], result));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: EV-DO BST Mode
 * Purpose:  This function selects the 1xEV-DO Base Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BDOMode(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                 RSSPECAN_ATTR_BDO_MODE, VI_NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BTS SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOSEM(
    ViSession   instrSession,
    ViInt32     limitLineType,
    ViReal64    limitLineValue,
    ViReal64    _30kHz1MHzTransition
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_MODE,
                 limitLineType), 2, "Limit Line Type");

    if (limitLineType == RSSPECAN_VAL_C2K_LIM_MODE_MAN)
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_VAL,
                 limitLineValue), 3, "Limit Line Value");

    if (!rsspecan_IsFSV (instrSession))
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS,
	                 _30kHz1MHzTransition), 4, "30kHz/1MHz Transition");
	}

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Code Domain Analyzer Result
 * Purpose:  This function selects the measured data that will be displayed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOCodeDomainAnalyzerResults(
    ViSession   instrSession,
    ViInt32 resultType)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    switch (resultType)
    {
        case RSSPECAN_VAL_C2K_FEED_CDP:
        case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
        case RSSPECAN_VAL_C2K_FEED_CDEP:
        case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
        case RSSPECAN_VAL_C2K_FEED_CPOW:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");    
        break;      
        case RSSPECAN_VAL_C2K_FEED_MACC: 
        case RSSPECAN_VAL_C2K_FEED_PVSL:
        case RSSPECAN_VAL_C2K_FEED_PVSY:
        case RSSPECAN_VAL_C2K_FEED_BSTR:
        case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
        case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
        case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
        case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
        case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
        case RSSPECAN_VAL_C2K_FEED_PVCH:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");  
        break;              
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");     
    }

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOCodeDomainAnalyzerSettings(
    ViSession   instrSession,
    ViInt32 captureLength,
    ViInt32 selectCode,
    ViInt32 selectSlot
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (!strstr (buffer, "FSP")) // not FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_IQLENGTH_FSQ_FSU,
                 captureLength), 2, "Capture Length");
    }
    else // is FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSP,
                 captureLength), 2, "Capture Length");
    }
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT,
                 selectSlot), 4, "Select Slot");
        
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_CODE,
                 selectCode), 3, "Select Code");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST CDP Channel Type
 * Purpose:  This function configures the CDP Channel Type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOCDPChannelType(
    ViSession   instrSession,
    ViInt32 channelType,
    ViBoolean   codeDomainOverview,
    ViInt32 mapping,
    ViInt32 select
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_CTYPE,
                 channelType), 2, "Channel Type");

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_BDO_CDP_OVERVIEW,
                 codeDomainOverview), 3, "Code Domain Overview");

    if (codeDomainOverview == VI_FALSE)
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_MMODE,
                     mapping), 4, "Mapping");

        if (((channelType != RSSPECAN_VAL_C2K_CDP_CTYPE_DATA) && (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_AUTO)) || (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_COMPLEX))
            viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_MAPPING,
                        select), 5, "Select");
    }


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Channel Type
 * Purpose:  This function configures the Channel Type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOChannelType(
    ViSession   instrSession,
    ViInt32 channelType,
    ViInt32 mapping,
    ViInt32 select
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_CTYPE,
                 channelType), 2, "Channel Type");

   viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_MMODE,
                     mapping), 3, "Mapping");

   if (((channelType != RSSPECAN_VAL_C2K_CDP_CTYPE_DATA) && (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_AUTO)) || (mapping != RSSPECAN_VAL_C2K_CDP_MMODE_COMPLEX))
            viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_CDP_MAPPING,
                        select), 4, "Select");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Power Reference
 * Purpose:  This function selects the reference power for relative results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureEVDOBSTPowerReference (ViSession instrSession,
                                                          ViInt32 powerReference)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_PREFERENCE,
                 powerReference), 2, "Power Reference");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Power vs Time
 * Purpose:  This function configures the Power vs Time Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOPowerVsTime(
    ViSession   instrSession,
    ViInt32 numberOfHalfslots,
    ViInt32 referenceSlot,
    ViBoolean   burstFit,
    ViInt32 referenceMeanPower,
    ViReal64    referenceValue,
    ViBoolean   restartOnFail
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Win1", RSSPECAN_ATTR_NUMBER_OF_SWEEPS,
                 numberOfHalfslots), 2, "Number Of Halfslots");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_RFSLOT,
                 referenceSlot), 3, "Reference Slot");

    if (referenceSlot == RSSPECAN_VAL_C2K_CDP_RFSLOT_IDLE)
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_BDO_PVT_BURST_FIT,
                     burstFit), 4, "Burst Fit");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_REFERENCE,
                 referenceMeanPower), 5, "Reference Mean Power");

    if (referenceMeanPower != RSSPECAN_VAL_C2K_LIM_PVT_REF_AUTO)
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_VALUE,
                     referenceValue), 6, "Reference Value");

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_BDO_PVT_FAIL_RESTART,
                 restartOnFail), 7, "Restart On Fail");



Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Power vs Time List Evaluation
 * Purpose:  This function enables/disables Power vs Time list evaluation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOPowerVsTimeListEvaluation (ViSession instrSession,
                                                                  ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_BDO_LIM_PVT_LIST,
                 state), 2, "State");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Restore EV-DO BST Power vs Time Standard Lines
 * Purpose:  This function restores the standard limit lines for the power
 *           versus time measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreBDOPowerVsTimeStandardLines(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_BDO_LIM_PVT_RESTORE, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute 
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOChannelTableData(
    ViSession   instrSession,
    ViInt32     channelType[],
    ViInt32     codeClass[],
    ViInt32     codeNumber[],
    ViInt32     modulation[],
    ViInt32     reserved1[],
    ViInt32     reserved2[],
    ViBoolean   status[],
    ViReal64    cdpRelative[],
    ViInt32 numofElements
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     i = 0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K84"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (numofElements<=0)
      viCheckParm(RS_ERROR_INVALID_PARAMETER, 10, "Num of Elements");
    
    pbuffer = buffer;
    pbuffer += sprintf (pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,0,0,%ld,%.12f",
                channelType[i], codeClass[i], codeNumber[i], modulation[i], status[i],
                cdpRelative[i]);

    for (i=1;i<numofElements; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%ld,%ld,0,0,%ld,%.12f",
            channelType[i], codeClass[i], codeNumber[i], modulation[i], status[i],
                cdpRelative[i]);

    viCheckErr (viPrintf (instrSession,"%s\n", buffer));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Advanced Settings
 * Purpose:  This function configures the Advanced Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOAdvancedSettings (ViSession instrSession,
                                                         ViInt32 pnOffset,
                                                         ViBoolean cdpAverage,
                                                         ViInt32 revision)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, model));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_PNOFFSET,
                 pnOffset), 2, "PN Offset");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_BDO_CDP_AVERAGE,
                 cdpAverage), 3, "CDP Average");
             
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_BDO_REVISION,
                 revision), 4, "Revision");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO BST Multi Carrier
 * Purpose:  This function configures the Multi Carrier settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureBDOMultiCarrier(
    ViSession   instrSession,
    ViBoolean   multiCarrier,
    ViBoolean   enhancedAlgorithm,
    ViBoolean   mcFilter,
    ViInt32 filterType,
    ViReal64    rollOff,
    ViReal64    cutOffFrequency
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_C2K_MCARRIER_STATE,
                 multiCarrier), 2, "Multi Carrier");

    if (multiCarrier == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                    RSSPECAN_ATTR_C2K_MCARRIER_ENH_ALG, enhancedAlgorithm),
                    3, "Enhanced Algorithm");

        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                    RSSPECAN_ATTR_C2K_MCARRIER_FILTER_STATE, mcFilter),
                    4, "MC Filter");

        if (mcFilter == VI_TRUE)
        {
            viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                        RSSPECAN_ATTR_C2K_MCARRIER_FILTER_TYPE, filterType),
                        5, "Filter Type");

            if (filterType == RSSPECAN_VAL_C2K_MCARRIER_FTYPE_RRC)
            {
                viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                            RSSPECAN_ATTR_C2K_MCARRIER_ROLL_OFF, rollOff),
                            6, "Roll Off");

                viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                            RSSPECAN_ATTR_C2K_MCARRIER_CUT_OFF_FREQ, cutOffFrequency),
                            7, "Cut Off Frequency");
            }
        }
    }


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Store BDO BTS SEM To File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreBDOSEMToFile(
    ViSession   instrSession,
    ViString    fileName
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_C2K_LIM_STORE_FILE, 
                            fileName), 2, "File Name");
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read EV-DO BTS SEM Peak List
 * Purpose:  This function returns the Spectrum EM Mask View Peak List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBDOSEMPeakList(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     numberOfValues,
    ViReal64    frequency[],
    ViReal64    level[],
    ViReal64    deltaLimit[],
    ViInt32*    returnedValues
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     arrayLength;
    ViReal64    *data = VI_NULL;
    ViInt32     retVal;
    ViInt32     i;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    arrayLength = 3*numberOfValues;
    data=malloc (sizeof(ViReal64)*arrayLength);

    viCheckErr (rsspecan_dataReadTrace (instrSession, window, "FINal1", arrayLength,
                    data, &retVal));

    if (returnedValues)
        *returnedValues = retVal / 3; /* Three result arrays */

    retVal = (retVal>arrayLength)?arrayLength:retVal;

    retVal /=3;

    for (i = 0; i < retVal; i++)
    {
        frequency[i] = data[i*3];
        level[i] = data[i*3+1];
        deltaLimit[i] = data[i*3+2];
    }


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (data) free(data);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read EV-DO BTS Power vs Time List Evaluation
 * Purpose:  This function returns the list evaluation results
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadBDOPowerVsTimeListEvaluation (ViSession instrSession,
                                                                 ViInt32 noOfValues,
                                                                 ViReal64 rangeNumber[],
                                                                 ViReal64 startTime[],
                                                                 ViReal64 stopTime[],
                                                                 ViReal64 avgPowerAbs[],
                                                                 ViReal64 avgPowerRel[],
                                                                 ViReal64 maxPowerAbs[],
                                                                 ViReal64 maxPowerRel[],
                                                                 ViReal64 minPowerAbs[],
                                                                 ViReal64 minPowerRel[],
                                                                 ViReal64 limitCheck[],
                                                                 ViReal64 reserved1[],
                                                                 ViReal64 reserved2[],
                                                                 ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViReal64    *pArray=VI_NULL;
    ViInt32     count=0;
    ViInt32     retCnt;
    ViInt32     i;
    ViInt32     values = noOfValues * 12;  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rangeNumber == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range number");
    if (startTime == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Start Time"); 
    if (stopTime == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Stop Time"); 
    if (avgPowerAbs == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Avg Power Abs"); 
    if (avgPowerRel == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Avg Power Rel"); 
    if (maxPowerAbs == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Max Power Abs"); 
    if (maxPowerRel == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Max Power Rel"); 
    if (minPowerAbs == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Min Power Abs"); 
    if (minPowerRel == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "Min Power Rel"); 
    if (limitCheck == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Limit Check");
    if (reserved1 == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 12, "Reserved 1");
    if (reserved2 == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 13, "Reserved 2"); 
     
    viCheckErr (viPrintf (instrSession, ":CONF:CDP:BTS:PVT:LIST:RES?\n"));
    pArray = malloc (values*sizeof (ViReal64));
    viCheckErr (rsspecan_dataReadTraceOnly (instrSession, values, pArray, &retCnt));
    values = (retCnt>values)?values:retCnt;
    i=0;
    while (count<values)
    {
        rangeNumber[i]=pArray[count];
        count++;
        startTime[i]=pArray[count];
        count++;
        stopTime[i]=pArray[count];
        count++;
        avgPowerAbs[i]=pArray[count];
        count++;
        avgPowerRel[i]=pArray[count];
        count++;
        maxPowerAbs[i]=pArray[count];
        count++;
        maxPowerRel[i]=pArray[count];
        count++;
        minPowerAbs[i]=pArray[count];
        count++;
        minPowerRel[i]=pArray[count];
        count++;
        limitCheck[i]=pArray[count];
        count++;
        reserved1[i]=pArray[count];
        count++;
        reserved2[i]=pArray[count];
        count++;
        i++;
    }             
    if (returnedValues)
        *returnedValues = retCnt/11;
    
    checkErr( rsspecan_CheckStatus (instrSession));     
Error:
    if (pArray) free(pArray); 
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query EV-DO BST Code Domain Analyzer General Results
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryBDOCodeDomainAnalyzerGeneralResults(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32 resultType,
    ViReal64*   result
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *buf=VI_NULL;
    ViUInt32    local_count     = 0;    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, model));

    if (!(strstr (buffer, "K84") || strstr (buffer, "K47")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if ((rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_RHO, RSSPECAN_VAL_C2K_MACCURACY))&&
        (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_FERROR, RSSPECAN_VAL_C2K_TFRAME))&&
        (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_RHOPILOT, RSSPECAN_VAL_C2K_IPMMAX))&&
        (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_RHODATA, RSSPECAN_VAL_C2K_RHOMAC)) )
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");

    viCheckErr (viPrintf (instrSession, "CALC%ld:MARK:FUNC:CDP:RES? %s\n", 
                window, analyzerResultSummaryArr[resultType]));

    
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &local_count));
            
    if ((strstr (buf, "Sync")) != NULL)
        *result =   RSSPECAN_NAN;
    else
        *result = atof(buf);
        
        
    checkErr( rsspecan_CheckStatus (instrSession));
    
    
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query EV-DO BST Code Domain Analyzer Channel Results
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryBDOCodeDomainAnalyzerChannelResults(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32 resultType,
    ViReal64*   result
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K84"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_PCDERROR, RSSPECAN_VAL_C2K_IQOFFSET))
            if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_SRATE, RSSPECAN_VAL_C2K_EVMPEAK))
                if (resultType != RSSPECAN_VAL_C2K_MTYPE)
                    viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");

    viCheckErr (viQueryf (instrSession, "CALC%ld:MARK:FUNC:CDP:RES? %s\n", "%le",
                window, analyzerResultSummaryArr[resultType], result));


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: EV-DO MST Mode
 * Purpose:  This function selects the 1xEV-DO Mobile Station Test mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MDOMode(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                 RSSPECAN_ATTR_MDO_MODE, VI_NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOSEM(
    ViSession   instrSession,
    ViInt32 limitLineType,
    ViReal64    _30kHz1MHzTransition
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_LIM_MODE,
                 limitLineType), 2, "Limit Line Type");

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_C2K_LIM_TRANS,
                 _30kHz1MHzTransition), 3, "30kHz/1MHz Transition");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Code Domain Analyzer Results
 * Purpose:  This function configures the Code Domain Analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOCodeDomainAnalyzerResults(
    ViSession   instrSession,
    ViInt32 resultType)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    switch (resultType)
    {
        case RSSPECAN_VAL_C2K_FEED_CDP:
        case RSSPECAN_VAL_C2K_FEED_CDP_RAT:
        case RSSPECAN_VAL_C2K_FEED_CDEP:
        case RSSPECAN_VAL_C2K_FEED_ERR_CTAB:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");    
        break;      
        case RSSPECAN_VAL_C2K_FEED_MACC: 
        case RSSPECAN_VAL_C2K_FEED_PVSL:
        case RSSPECAN_VAL_C2K_FEED_PVSY:
        case RSSPECAN_VAL_C2K_FEED_BSTR:
        case RSSPECAN_VAL_C2K_FEED_ERR_SUMM:
        case RSSPECAN_VAL_C2K_FEED_ERR_PCD:
        case RSSPECAN_VAL_C2K_FEED_SYMB_CONS:
        case RSSPECAN_VAL_C2K_FEED_SYMB_EVM:
        case RSSPECAN_VAL_C2K_FEED_COMP_CONS:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_C2K_RESULT_TYPE, resultType), 
                        2, "Result Type");  
        break;              
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");     
    }


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Code Domain Analyzer Settings
 * Purpose:  This function sets the Code Domain Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOCodeDomainAnalyzerSettings(ViSession   instrSession,
                                                                ViInt32 captureLength,
                                                                ViInt32 selectChannel,
                                                                ViInt32 selectHalfslot,
                                                                ViInt32 selectMapping)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K85"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "FSP")) // not FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MDO_CDP_IQLENGTH_FSQ_FSU,
                 captureLength), 2, "Capture Length");
    }
    else // is FSP
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MDO_CDP_IQLENGTH_FSP,
                 captureLength), 2, "Capture Length");
    }
   
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MDO_CDP_CODE,
                 selectChannel), 3, "Select Channel");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_C2K_CDP_SLOT,
                 selectHalfslot), 4, "Select Halfslot");
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING,
                 selectMapping), 5, "Select Mapping");
     

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Channel Table Data
 * Purpose:  This function defines the values of the selected channel table in
 *           the code domain power measurement mode. The whole table is defined
 *           in one go. The inactive channels (INACtive) do not have to be
 *           defined. Eight values are specified for a line of table.
 *
 *           Prior to this function, the name of the channel table has to be
 *           defined with function Configure CDMA2000 Channel Table Name
 *           (ConfigureC2KChannelTableName) - attribute 
 *           RSSPECAN_ATTR_C2K_CTABLE_NAME.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOChannelTableData(
    ViSession   instrSession,
    ViInt32 channelType[],
    ViInt32 codeClass[],
    ViInt32 codeNumber[],
    ViInt32 mapping[],
    ViInt32 activity[],
    ViInt32 reserved1[],
    ViBoolean   status[],
    ViInt32 reserved2[],
    ViInt32 numofElements
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     i = 0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K85"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (numofElements<=0)
      viCheckParm(RS_ERROR_INVALID_PARAMETER, 10, "Num of Elements");

    pbuffer = buffer;
    pbuffer += sprintf (pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,%ld,0,%ld,0",
                channelType[i], codeClass[i], codeNumber[i], mapping[i], activity[i],
                status[i]);

    for (i=1;i<numofElements; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%ld,%ld,%ld,0,%ld,0",
            channelType[i], codeClass[i], codeNumber[i], mapping[i], activity[i],
                status[i]);

    viCheckErr (viPrintf (instrSession,"%s\n", buffer));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST Advanced Settings
 * Purpose:  This function configures the Advanced Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOAdvancedSettings(
    ViSession   instrSession,
    ViInt32     powerReference,
    ViInt32     order,
    ViInt32     selectIQ,
    ViBoolean   cdpAverage,
    ViInt32     operation,
    ViString    longCodeI,
    ViString    longCodeQ
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_PREFERENCE,
                 powerReference), 2, "Power Reference");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_ORDER,
                 order), 3, "Order");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_MAPPING,
                 selectIQ), 4, "Select IQ");

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MDO_CDP_AVERAGE,
                 cdpAverage), 5, "CDP Average");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_MDO_CDP_OPERATION,
                 operation), 6, "Operation");

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_MDO_CDP_LCODE_I,
                 longCodeI), 7, "Long Code I");

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_MDO_CDP_LCODE_Q,
                 longCodeQ), 8, "Long Code Q");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure EV-DO MST CDP Overview
 * Purpose:  This function configures the CDP Overview       .
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOCDPOverview(
    ViSession   instrSession,
    ViBoolean   codeDomainOverview)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MC2K_CDP_OVERVIEW,
                 codeDomainOverview), 2, "Code Domain Overview");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*****************************************************************************
 * Function: Configure EV-DO MST PICH Marker Settings
 * Purpose:  This function sets marker to channel 0.16, 0.32 or 0.64,
 *           depending on the base spreading factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMDOPichMarkerSettings(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "C1,M1",
                                               RSSPECAN_ATTR_MC2K_MARKER_PICH, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query EV-DO MST Code Domain Analyzer Result Summary
 * Purpose:  This function queries the measured and calculated values of the
 *           code domain power analysis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMDOCodeDomainAnalyzerResultSummary(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     resultType,
    ViReal64*   result
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!strstr (buffer, "K85"))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_SLOT, RSSPECAN_VAL_C2K_PLENGTH))
        if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_MTYPE, RSSPECAN_VAL_C2K_RHOMAC))
            if (rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_C2K_CDEPEAK, RSSPECAN_VAL_C2K_CODPOWER))
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Type");

    viCheckErr (viQueryf (instrSession, "CALC%ld:MARK:FUNC:CDP:RES? %s\n", "%le",
                window, analyzerResultSummaryArr[resultType], result));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
