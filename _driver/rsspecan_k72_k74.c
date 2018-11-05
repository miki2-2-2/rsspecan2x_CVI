/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - K72/K74 - 3GPP FDD Base Station Test
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
 *  K72/K74 - 3GPP FDD Base Station Test
 *****************************************************************************/

#include "rsspecan.h"   

static ViString trace3GBSArr[]={"TRACE1","TRACE1","TRACE1","CTAB","CWCD","PWCD","TRACE2","TRACE2",
                                "TPVS","TPVS","ABIT","TRACE2","TRACE2","TRACE2","TRACE2","TRACE2","TRACE2",
                                "TRACE2","TRACE2","TRACE2","TRACE2","ATRACE2"};   
static ViString result3GBSWCDPArr[]={"PTOT","FERR","TFR","MACC",
        "PCD","EVMR","EVMP","CERR","CSL","SRAT","CHAN","CDP","CDPR","IQOF",
        "IQIM","MTYP","RHO","TOFF","PSYM","ACH","MPIC","RCD","ARCD","RCDQ"}; 
static ViString result3GBSCDPArr[]={"ACP","CPOW","MCAC","OBAN","TDMA","ACLR"};        
/*===========================================================================*/
/* Function: Set 3GPP FDD BS Mode
/* Purpose:  This function switches the instrument into 3GPP FDD BS mode. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Set3GPPBSMode(ViSession  instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_BWCD_MODE, ""));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Set 3GPP FDD BS Measurement
/* Purpose:  This function configures the 3GPP FDD UE additional 
/*           measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Set3GPPBSMeasurement(ViSession   instrSession,
                                                ViInt32 measurement)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_MEAS, 
                measurement), 2, "Measurement");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/// HIFN Switches between Normal mode and Home BS (Home Base Station)
/// HIFN mode for ACP and SEM measurements. Switching this parameter
/// HIFN changes the limits according to the specifications.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR baseStationMode/Selects the base station mode.
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMode (ViSession instrSession,
                                                ViInt32 baseStationMode)
{
	ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GPP_BS_STANDARD, 
                baseStationMode), 2, "Base Station Mode");               
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Measurement
/* Purpose:  This function configures the 3GPP FDD UE measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMeasurement(ViSession instrSession,
                                                        ViInt32 scramblingCode,
                                                        ViInt32 powerReference,
                                                        ViInt32 compareMode,
                                                        ViInt32 syncType,
                                                        ViInt32 sideBand,
                                                        ViBoolean   normalize,
                                                        ViBoolean   hsdpaupa,
                                                        ViReal64    inactiveChannelTreshold,
                                                        ViBoolean   invertQ,
                                                        ViInt32 antenna,
                                                        ViBoolean   powerDifference,
                                                        ViInt32 constellationParameterB,
                                                        ViInt32 timingOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_SCODE, 
            scramblingCode), 2, "Scrambling Code");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_POW_REF, 
                powerReference), 3, "Power Reference");
    if (strstr(model,"FSL")==NULL)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_COMPARE_MODE, 
                compareMode), 4, "Compare Mode");
    }   
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_SYNC_TYPE, 
                syncType), 5, "Sync Type");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_SIDE_BAND, 
                sideBand), 6, "Side Band");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_NORMALIZE, 
                normalize), 7, "Normalize");

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if ((strstr(model,"FSL")!=NULL) || (strstr (buffer, "K74") != VI_NULL)) 
    {
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_HSDPAUPA_STATE, 
                    hsdpaupa), 8, "HSDPA/UPA");
    }
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_INACT_CHAN_THR, 
                inactiveChannelTreshold), 9, "Inactive Channel Threshold"); 
     
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_INVERT_Q, 
                    invertQ), 9, "Invert Q");

	viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_ANTENNA, 
                antenna), 10, "Antenna");  
    
    if ((strstr(model,"FSL")==NULL))
    {
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_POW_DIFFERENCE, 
                powerDifference), 11, "Power Difference"); 

        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CONST_B, 
                    constellationParameterB), 12, "Constellation Parameter B");
    
        if (!rsspecan_IsFSV (instrSession))
        {
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_TIMING_OFFSET, 
                    timingOffset), 13, "Timing Offset");                
        }
    }
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Multiframe
/* Purpose:  This function configures the 3GPP FDD BS Multiframe.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMultiframe(ViSession  instrSession,
                                                    ViInt32 captureLength,
                                                    ViInt32 frameToAnalyze)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";       

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
     
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_MULTIFRAME_CAPTURE_LENGTH, 
                captureLength), 2, "Capture Length");               
   
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_MULTIFRAME_FRAME_ANALYZE, 
                frameToAnalyze), 3, "Frame To Analyze");            
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Multicarrier
/* Purpose:  This function configures the 3GPP FDD BS Multicarrier
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMulticarrier(ViSession    instrSession,
                                                        ViBoolean   multicarrierState,
                                                        ViBoolean   autoscale)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_MCACLR, 
                multicarrierState), 2, "Multicarrier State");   
    if (multicarrierState)              
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_AUTO_SCALE, 
                autoscale), 3, "Autoscale");            
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN  This function switches the single antenna MIMO mode on or off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR mode/Switches the single antenna MIMO mode on or off.

ViStatus _VI_FUNC rsspecan_Configure3GPPBSSingleAntennaMIMOMode (ViSession instrSession,
                                                                 ViBoolean mode)
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "",
        RSSPECAN_ATTR_3GBS_MIMO_MODE, mode), 2, "Mode");    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Results
/* Purpose:  This function configures the 3GPP FDD BS results
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPBSResults(ViSession instrSession,
                                                    ViInt32 channel,
                                                    ViInt32 cpichSlot,
                                                    ViInt32 spreadingFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";      
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                                   RS_ATTR_INSTRUMENT_MODEL,
                                                   RSSPECAN_IO_BUFFER_SIZE, model));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_RESULTS_SELECT_CHAN, 
            channel), 2, "Channel");                
 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_RESULTS_SELECT_CPICH_SLOT, 
                cpichSlot), 3, "CPICH Slot");    
    
    if (strstr (model, "FSL") == NULL)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_RESULTS_SFAC, 
                spreadingFactor), 4, "Spreading Factor");   
    }   
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Result Type
/* Purpose:  This function configures the 3GPP FDD BS result type
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPBSResultType(ViSession instrSession,
                                                     ViInt32 resultType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL") != NULL)
    {
        if ((rsspecan_invalidViInt32Range (resultType, RSSPECAN_VAL_3GBS_RES_CDP, 
                                        RSSPECAN_VAL_3GBS_RES_CTAB) || 
                                        (resultType==RSSPECAN_VAL_3GBS_RES_CDEP)) == VI_TRUE)
        {
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");
        }   
    }
    
    switch (resultType)
    {
        case RSSPECAN_VAL_3GBS_RES_CDP:
        case RSSPECAN_VAL_3GBS_RES_CDPRAT:
        case RSSPECAN_VAL_3GBS_RES_CDEP:
        case RSSPECAN_VAL_3GBS_RES_CTAB:
        case RSSPECAN_VAL_3GBS_RES_SUM: 
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "C1", RSSPECAN_ATTR_3GBS_RESULT, 
                resultType), 2, "Result Type"); 
        break;      
        case RSSPECAN_VAL_3GBS_RES_PCD:
        case RSSPECAN_VAL_3GBS_RES_MACC:
        case RSSPECAN_VAL_3GBS_RES_PVSLA:
        case RSSPECAN_VAL_3GBS_RES_PVSLR:
        case RSSPECAN_VAL_3GBS_RES_BSTR:
        case RSSPECAN_VAL_3GBS_RES_SYMBCONST:
        case RSSPECAN_VAL_3GBS_RES_SYMBEVM:
        case RSSPECAN_VAL_3GBS_RES_FVSL:
        case RSSPECAN_VAL_3GBS_RES_PVSY:
        case RSSPECAN_VAL_3GBS_RES_EVM_MAGN:
        case RSSPECAN_VAL_3GBS_RES_EVM_PHAS:
        case RSSPECAN_VAL_3GBS_RES_COMP_CONS:
        case RSSPECAN_VAL_3GBS_RES_CHIP_EVM:
        case RSSPECAN_VAL_3GBS_RES_CHIP_MAGN:
        case RSSPECAN_VAL_3GBS_RES_CHIP_PHAS:
        case RSSPECAN_VAL_3GBS_RES_PSVSL:
        case RSSPECAN_VAL_3GBS_RES_CDPOVER:
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "C2", RSSPECAN_ATTR_3GBS_RESULT, 
                resultType), 2, "Result Type"); 
        break;              
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");     
    }   
                    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Result Summary View
/* Purpose:  This function sets the view of the Code Domain Result Summary.
/*===========================================================================*/   
ViStatus _VI_FUNC rsspecan_Configure3GPPBSResultSummaryView (ViSession instrSession,
                                                             ViInt32 view)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_3GBS_RESULT_SUM_VIEW, 
                 view), 2, "View");
                 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Y Scale Division
/* Purpose:  This function defines the scaling of the y-axis in dB.
/*===========================================================================*/   
ViStatus _VI_FUNC rsspecan_Configure3GPPBSYScaleDivision (ViSession instrSession,
                                                          ViReal64 yScaleDivision)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_3GPP_PDIV, 
                yScaleDivision), 2, " Y Scale Division");
                 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: 3GPP FDD BS A Sequence
/* Purpose:  This function starts the following sequence:
/*           - Adjusts the reference level.
/*           - Searches automatically the scrambling code that leads to the
/*             highest signal power and stores it as new scrambling code.
/*           - Changes into the Result Summary result display.
/*===========================================================================*/   
ViStatus _VI_FUNC rsspecan_Set3GPPBSASequence (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout=-1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, 30000));    
    
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GPP_ASEQ, 
                NULL));
                 
Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Search 3GPP FDD BS Scrambling Code
/* Purpose:  This function searches automatically the scrambling code that 
/*           leads to the highest signal power and stores it as new scrambling 
/*           code for further measurements.
/*===========================================================================*/   
ViStatus _VI_FUNC rsspecan_Search3GPPBSScramblingCode (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GPP_SCODE_SEARCH, 
                NULL));
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function configures the base station.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR carrier/Specifies the carrier number.
/// HIPAR antenna/Specifies by the antenna number.
/// HIPAR CPICHCodeNumber/Sets the CPICH code number.
/// HIPAR CPICHPattern/Sets the CPICH pattern.
/// HIPAR frequencyOffset/Sets the frequency offset.
/// HIPAR scramblingCode/Sets the scrambling code.
ViStatus _VI_FUNC rsspecan_Configure3GPPTAERBTSCarrier (ViSession instrSession,
                                                        ViInt32 carrier,
                                                        ViInt32 antenna,
                                                        ViInt32 CPICHCodeNumber,
                                                        ViInt32 CPICHPattern,
                                                        ViInt32 frequencyOffset,
                                                        ViInt32 scramblingCode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (carrier, 1, 23))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Carrier");
    if (rsspecan_invalidViInt32Range (antenna, 1, 2))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Antenna");        
      
	sprintf (buffer, "CT%ld,AT%ld", carrier, antenna);
	
	viCheckErr ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_3GPP_TAER_CPICH_CODE_NUMBER, CPICHCodeNumber)); 

	viCheckErr ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_3GPP_TAER_ANTENNA_PATTERN, CPICHPattern)); 

	sprintf (buffer, "CT%ld", carrier);

	viCheckErr ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
				RSSPECAN_ATTR_3GPP_TAER_FREQUENCY_OFFSET, frequencyOffset)); 
	
	viCheckErr ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
				RSSPECAN_ATTR_3GPP_TAER_SCRAMBLING_CODE, scramblingCode)); 

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function requests the number of base stations configured by
/// HIFN the user.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR numberOfBTS/Returns Number of BTS. The maximum number of base stations to be configured depends on the type of device.
ViStatus _VI_FUNC rsspecan_Query3GPPTAERBTSNumber (ViSession instrSession,
                                                   ViInt32 *numberOfBTS)
{
	ViStatus	error = VI_SUCCESS;

	viCheckErr ( rsspecan_GetAttributeViInt32 (instrSession, "", 
                RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER, numberOfBTS)); 

	
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function manipulates with the number of base stations for
/// HIFN the time alignment error measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR carrier/Specifies the carrier number.
/// HIPAR operation/Specifies the operation for manipulate with the number of base stations.
ViStatus _VI_FUNC rsspecan_Set_3GPPTAERBTSOperation (ViSession instrSession,
                                                  ViInt32 carrier,
                                                  ViInt32 operation)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (carrier, 1, 23))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Carrier");
      
	sprintf (buffer, "CT%ld", carrier);

	switch(operation){
        case RSSPECAN_VAL_3GPP_BTS_NUMBER_INSERT:
			viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER_INSERT, VI_NULL)); 
        break;
        case RSSPECAN_VAL_3GPP_BTS_NUMBER_DELETE:
			viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER_DELETE, VI_NULL)); 
        break;
        case RSSPECAN_VAL_3GPP_BTS_NUMBER_DELETE_ALL:
			viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER_DELETE_ALL, VI_NULL)); 
        break;
	}

	
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function shows saved tables.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the catalog list parameter.
/// HIPAR catalogTablesList/This control returns saved tables.
ViStatus _VI_FUNC rsspecan_Query3GPPTAERTableCatalog (ViSession instrSession,
                                                      ViInt32 bufferSize,
                                                      ViChar catalogTablesList[])
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_GetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_3GPP_TAER_TABLE_CATALOG, bufferSize,
                                                catalogTablesList),
                                                3, "Catalog Tables List");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


/// HIFN This function deletes the specified table or saves a time
/// HIFN alignment error table or loads a list save in advance and runs
/// HIFN it for time alignment error measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR tableName/This control specifies table for operation.
/// HIPAR operation/Spcifies the operation with the number of base stations.
ViStatus _VI_FUNC rsspecan_Set_3GPPTAERTableOperation (ViSession instrSession,
                                                    ViInt32 operation,
                                                    ViString tableName)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	switch(operation){
        case RSSPECAN_VAL_3GPP_TAER_TABLE_DELETE:
			viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_3GPP_TAER_TABLE_DELETE, tableName)); 
        break;
        case RSSPECAN_VAL_3GPP_TAER_TABLE_SAVE:
			viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_3GPP_TAER_TABLE_SAVE, tableName)); 
        break;
	}

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function loads a list save in advance and runs it for time
/// HIFN alignment error measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR tableName/This control specifies table for operation.
ViStatus _VI_FUNC rsspecan_Set_3GPPTAERTablePreset (ViSession instrSession,
                                                    ViString tableName)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_3GPP_TAER_TABLE_PRESET, tableName)); 

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Channel Table Operations
/* Purpose:  This function performs the selected operation with channel 
/*           table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDBSChannelTableOperations(ViSession    instrSession,
                                                            ViInt32     operation,
                                                            ViString    channelTable)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
        
    if (!channelTable) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Table");
    
    switch(operation){
        case RSSPECAN_VAL_CDP_TABLE_OFF:
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB,
                VI_FALSE)); 
        break;
        case RSSPECAN_VAL_CDP_TABLE_SELECT:
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB,
                VI_TRUE));
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB_SEL, 
                 channelTable), 3, "Channel Table");                    
        break;  
        case RSSPECAN_VAL_CDP_TABLE_NEW:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB_NAME, 
                 channelTable), 3, "Channel Table"); 
        break;  
        case RSSPECAN_VAL_CDP_TABLE_DELETE:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB_DEL, 
                 VI_NULL));
        break;           
        case RSSPECAN_VAL_CDP_TABLE_COPY:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB_COPY, 
                 channelTable), 3, "Channel Table");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Operation");
        break;
        
    }       
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Channel Table Comment
/* Purpose:  This function defines a comment for the selected channel table 
/*           in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDBSChannelTableComment(ViSession   instrSession,
                                                        ViString    channelTableComment)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_3GBS_WCDP_CTAB_HEAD_COMM, 
                 channelTableComment), 2, "Channel Table Comment");
                 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure 3GPP FDD BS WCDP Channel Table Data
/* Purpose:  This function defines the values of the selected channel table 
/*           in the code domain power measurement mode:
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDBSChannelTableData (ViSession instrSession,
                                                      ViInt32 arraySize,
                                                      ViInt32 codeClass[],
                                                      ViInt32 codeNumber[],
                                                      ViBoolean TFCI[],
                                                      ViInt32 timingOffset[],
                                                      ViInt32 pilotLength[],
                                                      ViInt32 channelType[],
                                                      ViBoolean status[],
                                                      ViReal64 CDPRelative[])
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     i=0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    if (strstr(buffer,"FSL")!=NULL) 
    {
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    }   
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K72")||strstr (buffer, "K74")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (arraySize<=0)
      viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
    pbuffer = buffer;
    pbuffer += sprintf (pbuffer, "CONF:WCDP:CTAB:DATA %ld,%ld,%d,%ld,%ld,%d,%ld,%.12f",
                codeClass[i], codeNumber[i], TFCI[i], timingOffset[i], pilotLength[i], channelType[i], 
                status[i], CDPRelative[i]);
    for (i=1;i<arraySize; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%d,%ld,%ld,%d,%ld,%.12f",
            codeClass[i], codeNumber[i], TFCI[i], timingOffset[i], pilotLength[i], channelType[i], 
                status[i], CDPRelative[i]);    
    viCheckErr (viPrintf (instrSession,"%s\n", buffer));
    
    checkErr( rsspecan_CheckStatus (instrSession));         
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Query 3GPP FDD BS WCDP Channel Table Catalog
/* Purpose:  This function reads out the names of all channel tables stored 
/*           on the harddisk in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Query3GPPFDDBSChannelTableCatalog(ViSession   instrSession,
                                                        ViInt32*    numberofChannelTables,
                                                        ViInt32     bufferSize,
                                                        ViChar      channelTablesList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt;
    ViChar      *buf=VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    if (strstr(buffer,"FSL")!=NULL) 
    {
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    }   
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K72")||strstr (buffer, "K74")))    
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (channelTablesList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Table List");  
       
    viCheckErr (viPrintf (instrSession, ":CONF:WCDP:CTAB:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, channelTablesList, numberofChannelTables));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Set 3GPP FDD BS Marker To Channel
/* Purpose:  This function sets the marker to selected channel
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Set3GPPBSMarkerToChannel(ViSession   instrSession,
                                                    ViInt32 window,
                                                    ViInt32 channel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (channel, 0, 1))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel");        
      
    switch (channel){
        case RSSPECAN_VAL_3GPP_MARKER_CPICH:
            sprintf (buffer, "C%ld,Cpich", window);
            viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GBS_MARKER_TO_CHANNEL, VI_NULL)); 
        break;
        case RSSPECAN_VAL_3GPP_MARKER_PCCPCH:
            sprintf (buffer, "C%ld,Pccpch", window);
            viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GBS_MARKER_TO_CHANNEL, VI_NULL)); 
        break;
    }    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Set 3GPP FDD BS Delta Marker To Channel
/* Purpose:  This function sets the delta marker to selected channel
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Set3GPPBSDeltaMarkerToChannel (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 channel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (channel, 0, 1))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel");        
      
    switch (channel){
        case RSSPECAN_VAL_3GPP_MARKER_CPICH:
            sprintf (buffer, "C%ld,Cpich", window);
            viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GBS_REFERENCE_MARKER_TO_CHANNEL, VI_NULL)); 
        break;
        case RSSPECAN_VAL_3GPP_MARKER_PCCPCH:
            sprintf (buffer, "C%ld,Pccpch", window);
            viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_3GBS_REFERENCE_MARKER_TO_CHANNEL, VI_NULL)); 
        break;
    }    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Read  3GPP FDD BS Trace
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
/*           TRACe<1|2>[:DATA] ABITstream | CWCDp | TPVSlot | CTABle | PWCDp
/*           :FORM REAL,32
/*           *CLS
/*           
/*           
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPBSTrace(ViSession instrSession,
                                            ViInt32 window,
                                            ViInt32 trace,
                                            ViInt32 arrayLength,
                                            ViInt32*    actualPoints,
                                            ViReal64    values[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *exBuf = VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

     if (!(strstr (buffer, "K72")||strstr (buffer, "K74")))    
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    if (rsspecan_invalidViInt32Range (trace, RSSPECAN_VAL_3GBS_TRAC_CDP, RSSPECAN_VAL_3GBS_TRAC_ATRACE2))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    if (!actualPoints)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Actual Points");          
    
    viCheckErr (rsspecan_dataReadTrace (instrSession, window, trace3GBSArr[trace], arrayLength, 
                    values, actualPoints));
                    
    checkErr( rsspecan_CheckStatus (instrSession));           
    
Error:
    if (exBuf) free (exBuf);
    Rs_UnlockSession(instrSession, VI_NULL);
    
    return error;
}

/*===========================================================================*/
/* Function: Get 3GPP FDD BS WCDP Result
/* Purpose:  This function returns the WCDP measurement Results
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPBSResult(ViSession    instrSession,
                                            ViInt32     type,
                                            ViReal64*   result)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K72")||strstr (buffer, "K74")||strstr (buffer, "K44")))    
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (rsspecan_invalidViInt32Range (type, RSSPECAN_VAL_WCDP_RES_PTOT, RSSPECAN_VAL_WCDP_RES_RCDQ))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");
    
    if (!result)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result");        

    viCheckErr (viQueryf (instrSession, "CALC:MARK:FUNC:WCDP:RES? %s\n","%le", 
                result3GBSWCDPArr[type], result));
    checkErr( rsspecan_CheckStatus (instrSession));    
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get 3GPP FDD BS CDP Result
/* Purpose:  This function returns the power result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPBSCDPResult(ViSession instrSession,
                                            ViInt32     type,
                                            ViInt32     arraySize,
                                            ViReal64    results[],
                                            ViInt32     *returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf[RSSPECAN_IO_BUFFER_SIZE]; 
    ViInt32     i;
    ViChar      *buffer = VI_NULL;
    ViChar      *pbuff = VI_NULL;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buf));

    if (!(strstr (buf, "K72")||strstr (buf, "K74")||strstr (buf, "K44")))    
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    if ((strstr(buf,"K44")==NULL)) {
        if (rsspecan_invalidViInt32Range (type, 0, 3))
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");
        }
    else {
        if (rsspecan_invalidViInt32Range (type, 0, 5))
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");
        }
    
    if (arraySize<0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Array Size"); 
    if ((arraySize>0) && (results == NULL))    
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Results");
    
    viCheckErr( viPrintf (instrSession, "CALC:MARK:FUNC:POW:RES? %s\n", 
        result3GBSCDPArr[type]));
    
    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &buffer, VI_NULL));  
    
    pbuff = strtok (buffer, ",");
    i = 0;
    do{
        if (i<arraySize) results[i] = atof (pbuff);
        pbuff = strtok (NULL, ",");
        i++;
    }while(pbuff != NULL);   
    
    if (returnedValues) *returnedValues = i;
    
    checkErr( rsspecan_CheckStatus (instrSession));    
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get 3GPP FDD BS CDP Scrambling Code Results
/* Purpose:  This function returns result table of the automatic search sequence 
/*           containing the highest power values calculated and the corresponding
/*           scrambling codes.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPBSCDPScramblingCodeResult (ViSession instrSession,
                                                             ViInt32 arraySize,
                                                             ViInt32 scrambingCode[],
                                                             ViReal64 power[],
                                                             ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pbuffer = NULL;
    ViChar*     pstring_value = NULL; 
    ViInt32     idx=0;
    ViUInt32    retCnt;
    ViChar      buf[RSSPECAN_IO_BUFFER_SIZE];  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buf));
    
    if ((strstr (buf, "FSL") == NULL) && !rsspecan_IsFSV (instrSession))
    {
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    }
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buf));

    if (!strstr (buf, "K72"))    
    {
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    }
    if ((arraySize>0)&&(!scrambingCode))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Scrambling Code");  
    if ((arraySize>0)&&(!power))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "WiPowerdow");  
    
    viCheckErr (viPrintf(instrSession, "*CLS;:CDP:LCODe:SEAR:LIST?\n"));   
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &pbuffer, &retCnt));
    
    if (strlen(pbuffer) > 1)
    {
        pstring_value = strtok(pbuffer, ",");

        while (pstring_value)
        {
            if (idx<arraySize)
            {
                //sscanf (pstring_value, "%ld,%[^,],%Le", &scrambingCode[idx], buf, &power[idx]);
                scrambingCode[idx] = atoi (pstring_value);
                pstring_value = strtok(NULL, ",");   
                pstring_value = strtok(NULL, ",");   
                power[idx] = atof (pstring_value);
                pstring_value = strtok(NULL, ",");   
            }
            //pstring_value = strtok(NULL, ",");   
            //pstring_value = strtok(NULL, ",");   
            //pstring_value = strtok(NULL, ",");          
            idx++;
        }
    }
    if (returnedValues)
    {
        *returnedValues=idx;
    }       
    
    checkErr( rsspecan_CheckStatus (instrSession));     
    
Error:
    if (pbuffer) free(pbuffer);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get 3GPP FDD BS CDP List Evaluation Result
/* Purpose:  This function queries the list evaluation results. All results 
/*           are float values.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_Get3GPPBSCDPListEvaluationResult (ViSession instrSession,
                                                             ViInt32 noOfValues,
                                                             ViReal64 rangeNumber[],
                                                             ViReal64 startFrequency[],
                                                             ViReal64 stopFrequency[],
                                                             ViReal64 resolutionBandwidth[],
                                                             ViReal64 frequency[],
                                                             ViReal64 powerAbs[],
                                                             ViReal64 powerRel[],
                                                             ViReal64 delta[],
                                                             ViInt32 limitCheck[],
                                                             ViReal64 reserved1[],
                                                             ViReal64 reserved2[],
                                                             ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    //ViChar      *buffer = VI_NULL;
    //ViChar      *pbuf=VI_NULL;
    ViInt32     count=0;
    ViUInt32    retCnt;
    ViUInt32     i;
    ViChar      buf[RSSPECAN_IO_BUFFER_SIZE];  
    ViReal64    *values = NULL;
    ViInt32     arrayLength,
                actualPoints;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buf));
    
    if ((strstr (buf, "FSL") == NULL) && !rsspecan_IsFSV (instrSession))
    {
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    }
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buf));

    if (!(strstr (buf, "K72")))    
    {
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);;
    }

    if (rangeNumber == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range number");
    if (startFrequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Start Frequency"); 
    if (stopFrequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Stop Frequency"); 
    if (resolutionBandwidth == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Resolution Bandwidth"); 
    if (frequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Frequency"); 
    if (powerAbs == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Power Abs");
    if (powerRel == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Power Rel"); 
    if (delta == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Delta"); 
    if (limitCheck == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "Limit Check");
    if (reserved1 == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Reserved 1");
    if (reserved2 == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 12, "Reserved 2"); 
    
    arrayLength = noOfValues*11;
    values = malloc(arrayLength*sizeof (ViReal64));
    //viCheckErr (viPrintf (instrSession, "TRAC:DATA? LIST\n"));
    //viCheckErr( Rs_ReadDataUnknownLength(instrSession, &buffer, &retCnt));
    //For R&S FSL are values returned in REAL,32 format
    viCheckErr (rsspecan_dataReadTrace (instrSession, 0, "LIST", arrayLength, 
                    values, &actualPoints));
    count=0;
    i=0;
    retCnt=(arrayLength<actualPoints)?arrayLength: actualPoints;
    
    do{
        rangeNumber[count] = values[i++];
        startFrequency[count] = values[i++];
        stopFrequency[count] = values[i++];
        resolutionBandwidth[count] = values[i++];
        frequency[count] = values[i++];
        powerAbs[count] = values[i++];
        powerRel[count] = values[i++];
        delta[count] = values[i++];
        limitCheck[count] = (ViInt32) values[i++];
        reserved1[count] = values[i++];
        reserved2[count] = values[i++];
        count++;
    }while (i<retCnt);  
 
    if (returnedValues)
        *returnedValues = actualPoints/11;
     
    checkErr( rsspecan_CheckStatus (instrSession));     
Error:
    if (values) free(values);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function queries the result of the Time Alignment Error measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR type/This control sets the result.
/// HIPAR result/Returns the measured result.

ViStatus _VI_FUNC rsspecan_Get3GPPBSTimeAlignmentErrorResult (ViSession instrSession,
                                                              ViInt32 type,
                                                              ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (type != RSSPECAN_VAL_TAER_RES_TAER)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");
    if (!result)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result");        

    viCheckErr (viQueryf (instrSession, "CALC:MARK:FUNC:TAER:RES? TAER\n","%le", result));
    
    checkErr( rsspecan_CheckStatus (instrSession));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 *- User defined callbacks --------------------------------------------------*
 *****************************************************************************/
