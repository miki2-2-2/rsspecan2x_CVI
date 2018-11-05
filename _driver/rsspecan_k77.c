/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  K77 - TD-SCDMA MS Mode
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
/* Function: Set TDS UE Measurement
/* Purpose:  This function selects the measurement of the Application 
/*           Firmware R&S FS-K77, TD-SCDMA mobile station test.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetTDSUEMeasurement(ViSession    instrSession,
                                                ViInt32 measurement)
{
    ViStatus    error = VI_SUCCESS;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_MEAS, 
															measurement), 2, "Measurement"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Adapt To Signal
/* Purpose:  This function configures the gated sweep mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEAdaptToSignal(ViSession    instrSession,
                                                        ViInt32 startSlot,
                                                        ViInt32 stopSlot)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_ADAPT_SIGN_SLOT_START, startSlot),
                        2, "Start Slot");
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_ADAPT_SIGN_SLOT_STOP, stopSlot), 
                        3, "Stop Slot");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Capture
/* Purpose:  This function configures the TDS BS capture settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUECapture (ViSession instrSession,
                                                  ViInt32 setCount,
                                                  ViInt32 setValue,
                                                  ViInt32 captureLength,
                                                  ViInt32 channel, 
                                                  ViInt32 slot)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model[10] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (strstr (model, "FSQ") != NULL)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_RESULT_SET_COUNT, setCount), 
                            2, "Set Count"); 
        if (setCount>1) 
        {
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_RESULT_SET_VALUE, setValue), 
                            3, "Set Value");
        }
        else
        {
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_CAPTURE_LENGTH, captureLength), 
                            4, "Capture Length");                       
        }
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_CAPTURE_LENGTH, captureLength), 
                        4, "Capture Length");                       
    }
            
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_RESULT_SEL_CHAN, channel),
                        5, "Channel");
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_RESULT_SEL_SLOT, slot), 
                        6, "Slot");
       
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Results
/* Purpose:  This function configures the TDS UE results
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEResults(ViSession  instrSession,
                                                ViInt32 resultType)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    switch (resultType)
    {
        case RSSPECAN_VAL_TDS_RES_CDP:
        case RSSPECAN_VAL_TDS_RES_CDPRAT:
        case RSSPECAN_VAL_TDS_RES_CDEP:
        case RSSPECAN_VAL_TDS_RES_CTAB:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C1", RSSPECAN_ATTR_TDUE_RESULT_TYPE, resultType), 
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
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "C2", RSSPECAN_ATTR_TDUE_RESULT_TYPE, resultType), 
                        2, "Result Type");    
        break;              
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Result Type");     
    }   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: TDS UE Adapt Signal Auto Level Time
/* Purpose:  This function starts the autorange routine for the reference 
/*           level. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TDSUEAdaptSignalAutoLevelTime(ViSession  instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_ADAPT_SIGN_AUT_LEVEL_TIME, VI_NULL));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: TDS UE CDP Level Adjust
/* Purpose:  This function initiates automatic setting of the RF attenuation 
/*           and IF gain to the level of the applied signal. The unit is put 
/*           into RF ATTEN MANUAL mode to optimize RF attenuation and IF gain 
/*           independently of each other. This mode is retained after 
/*           changing from TD-SCDMA BTS mode to SPECTRUM mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TDSUECDPLevelAdjust(ViSession    instrSession, 
                                               ViUInt32         timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));        
    viCheckErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TDUE_CDP_LEV_ADJUST, VI_NULL));
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN Selects the SEM limits according to the 3GPP Standard version 7.5.0 or according to version 7.6.0 or newer.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR release/Selects the SEM limits according to the 3GPP Standard version 7.5.0 or according to version 7.6.0 or newer.

ViStatus _VI_FUNC rsspecan_ConfigureTDSUESEMLimits (ViSession instrSession,
                                                    ViInt32 release)
{
	ViStatus    error = VI_SUCCESS;
	checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
        RSSPECAN_ATTR_TDUE_SEM_LIMITS, release), 2, "Release");    

Error:
 	(void) Rs_UnlockSession(instrSession, VI_NULL);
	return error;
}


/*===========================================================================*/
/* Function: TDS UE Channel Table Operations
/* Purpose:  This function performs the selected operation with channel 
/*           table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TDSUEChannelTableOperations(ViSession    instrSession,
                                                        ViInt32     operation,
                                                        ViString    channelTable)
{
    ViStatus    error = VI_SUCCESS;
    
	checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (operation, RSSPECAN_VAL_TDS_TABLE_OFF, RSSPECAN_VAL_TDS_TABLE_COPY))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Operation");
    if (!channelTable) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Table");
	
	switch(operation)
	{
		case RSSPECAN_VAL_TDS_TABLE_OFF:
	        viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_STATE,
	                VI_FALSE)); 
	    break;
	    case RSSPECAN_VAL_TDS_TABLE_SELECT:
	        viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_STATE,
	                VI_TRUE));
	        viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_SEL, 
	                 channelTable), 3, "Channel Table");                    
	    break;  
	    case RSSPECAN_VAL_TDS_TABLE_NEW:
	        viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_NAME, 
	                 channelTable), 3, "Channel Table"); 
	    break;  
	    case RSSPECAN_VAL_TDS_TABLE_DELETE:
	        viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_DEL, 
	                 channelTable), 3, "Channel Table");
	    break;           
	    case RSSPECAN_VAL_TDS_TABLE_COPY:
	        viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_COPY, 
	                 channelTable), 3, "Channel Table");
	    break;
    }
	
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Channel Table Comment
/* Purpose:  This function defines a comment for the selected channel table 
/*           in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableComment(ViSession  instrSession,
                                                    ViString    channelTableComment)
{
    ViStatus    error = VI_SUCCESS;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_COMM, 
                 	channelTableComment), 2, "Channel Table Comment");
	
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Channel Table Data
/* Purpose:  This function defines a channel table. The whole table is 
/*           defined in one operation. The inactive channels (INACtive) do 
/*           not have to be defined.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableData(ViSession instrSession,
                                                        ViInt32     arraySize,
                                                        ViInt32     channelType[],
                                                        ViInt32     codeClass[],
                                                        ViInt32     codeNumber[],
                                                        ViInt32     modulationType[],
                                                        ViInt32     midableShift[],
                                                        ViBoolean   status[],
                                                        ViInt32     reserved1[],
                                                        ViInt32     reserved2[])
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     i=0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuffer;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
	
    if (!(strstr (buffer, "K77")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);

    if (arraySize<=0)
      viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
    pbuffer = buffer;
	pbuffer += sprintf (pbuffer, "CONF:CDP:CTAB:DATA %ld,%ld,%ld,%ld,%ld,%d,%ld,%ld",
	                channelType[i], codeClass[i], codeNumber[i], modulationType[i], midableShift[i],
					status[i], reserved1[i], reserved2[i]);
	
    for (i=1;i<arraySize; i++)
        pbuffer += sprintf (pbuffer, ",%ld,%ld,%ld,%ld,%ld,%d,%ld,%ld",
                channelType[i], codeClass[i], codeNumber[i], modulationType[i], midableShift[i], status[i], 
                reserved1[i], reserved2[i]);    
    viCheckErr (viPrintf (instrSession,"%s\n", buffer));
    
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Channel Table Order
/* Purpose:  This function selects sorting of the channel table in code 
/*           order or midamble order.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableOrder(ViSession    instrSession,
                                                            ViInt32 order)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_ORDER, 
                 	order), 2, "Order");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Channel Table Midamble Shift
/* Purpose:  This function defines the maximum number of midamble shifts in 
/*           the channel table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableMidambleShift(ViSession instrSession,
                                                        ViInt32 maximumofMidableShifts)
{
    ViStatus    error = VI_SUCCESS;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_CTAB_MSHIFT, 
                 	maximumofMidableShifts), 2, "Maximum of Midable Shifts");
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines the highest modulation to be considered in
/// HIFN the automatic channel search. In low SNR environments it may be
/// HIFN necessary to limit the channel search to lower modulations than
/// HIFN 64QAM.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR highestModulation/Defines the highest modulation to be considered in the automatic channel search. In low SNR environments it may be necessary to limit the channel search to lower modulations than 64QAM.
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableMaxModulation (ViSession instrSession,
                                                                    ViInt32 highestModulation)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
        RSSPECAN_ATTR_TDUE_CTAB_MMAX, highestModulation), 2, "Highest Modulation");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get TDS UE Channel Table Catalog
/* Purpose:  This function reads out the names of all channel tables stored 
/*           on the harddisk in the code domain power measurement mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetTDSUEChannelTableCatalog(ViSession    instrSession,
                                                        ViInt32*    numberofChannelTables,
                                                        ViInt32     bufferSize,
                                                        ViChar      channelTablesList[])
{
    ViStatus    error = VI_SUCCESS;
	ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViUInt32    retCnt;
    ViChar      *buf=VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
	
	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
	
    if (!(strstr (buffer, "K77")))    
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (channelTablesList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Table List");  
    
	if ((strstr (model, "FSW") == NULL)) 
	{
    	viCheckErr (viPrintf (instrSession, ":CONF:CDP:CTAB:CAT?\n"));
	}
	else
	{
		viCheckErr (viPrintf (instrSession, ":CONF:CDP:MS:CTAB:CAT?\n"));
	}
	
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
	
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, channelTablesList, numberofChannelTables));

    checkErr( rsspecan_CheckStatus (instrSession));
Error:
    if (buf) free(buf); 
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Measurement
/* Purpose:  This function configures the TDS UE measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEMeasurement(ViSession  instrSession,
                                                    ViInt32     scramblingCode,
                                                    ViInt32     midableShiftCells,
                                                    ViInt32     sideBand,
                                                    ViBoolean   normalize,
                                                    ViReal64    inactiveChannelTreshold,
                                                    ViBoolean   invertQ)
{
    ViStatus    error = VI_SUCCESS;
    ViBoolean   tmp_state = VI_FALSE;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_SCODE, 
                 scramblingCode), 2, "Scrambling Code");                 
    viCheckErr (rsspecan_GetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDBS_CTAB_STATE, &tmp_state));            
    if (tmp_state == VI_FALSE)
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_CELL_SHIFTS, 
                 midableShiftCells), 3, "Midable Shifts");  
    }             
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_SIDE_BAND, 
                 sideBand), 5, "Side Band"); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDUE_NORMALIZE, 
                 normalize), 6, "Normalize");  
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_TDUE_INACT_CHAN_THR, 
                 inactiveChannelTreshold), 7, "Inactive Channel Treshold"); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDUE_INVERT_Q, 
                 invertQ), 8, "Invert Q");                 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE Sync To Slot
/* Purpose:  This function configures the TDS UE sync to slot. By default 
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
ViStatus _VI_FUNC rsspecan_ConfigureTDSUESyncToSlot(
    ViSession   instrSession,
    ViBoolean   syncToSlot
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
        
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_TDUE_SYNC_TO_SLOT, syncToSlot),
                                                2, "sync To Slot");                                                 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*===========================================================================*/
/* Function: Configure TDS UE CPICH
/* Purpose:  This function configures the TDS UE user defined CPICH.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEUCPICH(
    ViSession   instrSession,
    ViBoolean   userDefinedCPICH,
    ViInt32 cpichCode,
    ViInt32 cpichPattern
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDUE_CPICH_STATE, 
                 userDefinedCPICH), 2, "User Defined CPICH");

    if (userDefinedCPICH) {
    
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_CPICH_CODE, 
                 cpichCode), 3, "CPICH Code");

        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_CPICH_PATTERN, 
                 cpichPattern), 4, "CPICH Pattern"); 
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure TDS UE Power Vs. Time Measurement
/* Purpose:  This function configures the TDS UE Power Vs. Time measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEPVTMeasurement(ViSession   instrSession,
                                                        ViInt32     subframes,
                                                        ViBoolean   highDynamicState)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_MEAS_PVT_SFRAMES, 
                 subframes), 2, "Subframes"); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_TDUE_MEAS_PVT_HDYN, 
                 highDynamicState), 3, "High Dynamic State"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get TDS UE Reference Level Adjustment Result
/* Purpose:  This function adjusts the refernce level and returns the 
/*           result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetTDSUEReferenceLevelAdjustmentResult(ViSession instrSession,
                                              ViUInt32 timeout,
                                              ViInt32*  result)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout"); 
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TDUE_ADJ_RLEV_QUERY, 
                 result), 2, "Result"); 
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 *- End Of RSSpecAn K77 -----------------------------------------------------*
 *****************************************************************************/
