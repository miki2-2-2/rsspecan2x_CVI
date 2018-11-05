/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - 
 *                              K70 - Vector Signal Analysis 
 *  VXIpnp Style Instrument Driver
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

/*****************************************************************************
 *  K70 - Vector Signal Analysis
 *****************************************************************************/

#include "rsspecan.h"

/*===========================================================================*/
/* Function: Configure VSA Digital Standard
/* Purpose:  This function selects an automatic setting of all modulation 
/*           parameters according to a standardized transmission method or a 
/*           user-defined transmission method. The standardized
/*           transmission methods are available in the unit (predefined 
/*           standard). The transmission methods are set via a file name 
/*           without extension (string data with quotation marks). Some 
/*           predefined transmission methods can be set via a value also 
/*           (character data without quotation marks) for reasons of 
/*           compatibility with former instrument models.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSADigitalStandard(ViSession    instrSession,
                                                        ViInt32     window,
                                                        ViString    standard,
                                                        ViBoolean   stepWidthAuto,
                                                        ViReal64    stepSize)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD, standard), 
                2, "Standard");
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_FREQUENCY_STEP_AUTO, stepWidthAuto), 
                3, "Step Width Auto");  
    if (!stepWidthAuto)
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_CENTER_STEP, stepSize), 
                4, "Step Size");            
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get VSA Digital Standard Catalog
/* Purpose:  This function reads the names of all digital standards 
/*           available.
/*           
/*           Notes:
/*           
/*           (1) Driver returns an instrument specific error, when catalog 
/*           list is empty.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetVSADigitalStandardCatalog(ViSession   instrSession,
                                                        ViInt32     window,
                                                        ViInt32*    numberofDigitalStandards,
                                                        ViInt32     bufferSize,
                                                        ViChar      digitalStandardsList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *buff;
    ViUInt32    count=0;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
	if (rsspecan_IsFSV (instrSession))
		checkErr (RS_ERROR_INSTRUMENT_MODEL);
		
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K70")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    checkErr (viPrintf (instrSession, "SENS%ld:DDEM:STAN:CAT?\n", window));  
    checkErr (Rs_ReadDataUnknownLength(instrSession, &buff, &count));
    if (bufferSize != 0)
    {
        strncpy(digitalStandardsList, buff, (bufferSize > (ViInt32)count) ? count : bufferSize);
    }
    if (numberofDigitalStandards) *numberofDigitalStandards = count;
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Burst Search Hysteresis
/* Purpose:  This function configures the manual hysteresis level for a 
/*           signal burst search.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchHysteresis(ViSession  instrSession,
                                                            ViInt32     window,
                                                            ViReal64    hysteresisLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
                                   
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_HYSTERESIS_LEVEL,
                hysteresisLevel), 3, "Hysteresis Level");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Burst Search Expert Settings
/* Purpose:  This function defines the settings that are
/*           used to accurately control the behaviour of the burst search 
/*           even under difficult measurement conditions.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchExpertSettings(ViSession  instrSession,
                                                                ViInt32     window,
                                                                ViInt32     usefulLength,
                                                                ViInt32     maxLength,
                                                                ViInt32     runIn,
                                                                ViInt32     runOut,
                                                                ViInt32     minGaplen)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
            
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_LENGTH_MIN,
                usefulLength), 3, "Useful Length");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_LENGTH_MAX,
                maxLength), 4, "Max Length");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_SKIP_RISE,
                runIn), 5, "Run In");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_SKIP_FALL,
                runOut), 6, "Run Out");             
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_GLEN,
                minGaplen), 7, "Min Gaplen");               
                
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
   
/*===========================================================================*/
/* Function: Configure VSA Equalizer
/* Purpose:  This function configures the parameters of an adaptive 
/*           equalizer filter.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAEqualizer(ViSession  instrSession,
                                                ViInt32     window,
                                                ViReal64    speedofConversion,
                                                ViInt32     length)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
           
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_SPEED_CONVERSION,
                speedofConversion), 3, "Speed Of Conversion");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_LENGTH,
                length), 4, "Length");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Equalizer Mode
/* Purpose:  This function configures the mode of an adaptive 
/*           equalizer filter.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAEqualizerMode(ViSession  instrSession,
                                                ViInt32     window,
                                                ViBoolean   state,
                                                ViBoolean   training)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
            
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_STATE,
                state), 3, "State");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_LEARNING,
                training), 4, "Training");              
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*===========================================================================*/
/* Function: VSA Equalizer Operations
/* Purpose:  This function performs the selected operation with equalizer.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_VSAEqualizerOperations(ViSession     instrSession,
                                                    ViInt32     window,
                                                    ViInt32     operation,
                                                    ViString    equalizerName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";      
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (operation, RSSPECAN_VAL_VSA_EQUA_LOAD, RSSPECAN_VAL_VSA_EQUA_RESET))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Operation");
    sprintf (buffer, "Win%ld", window);  
    switch(operation){
        case RSSPECAN_VAL_VSA_EQUA_LOAD:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_LOAD, 
                 equalizerName), 4, "Equalizer Name"); 
        break;
        case RSSPECAN_VAL_VSA_EQUA_SAVE:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_SAVE, 
                 equalizerName), 4, "Equalizer Name");                    
        break;  
        case RSSPECAN_VAL_VSA_EQUA_DELETE:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_DELETE, 
                 equalizerName), 4, "Equalizer Name"); 
        break;  
        case RSSPECAN_VAL_VSA_EQUA_RESET:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_EQUALIZER_RESET, 
                 ""));
        break;           
    }       
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Modulation
/* Purpose:  This function configures the modulation.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulation(ViSession instrSession,
                                                ViInt32     window,
                                                ViReal64    symbolRate,
                                                ViInt32     mode,
                                                ViString    mappingName,
                                                ViBoolean   signalFiltering,
                                                ViInt32     pointsSymbol)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";      
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_VSA_MODULATION_FORMAT_QPSK, RSSPECAN_VAL_VSA_MODULATION_FORMAT_USERQAM))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Mode");        
    sprintf (buffer, "Win%ld", window);  
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_SRATE, 
                 symbolRate), 3, "Symbol Rate");
    switch(mode){
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_QPSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, 
                 RSSPECAN_VAL_MOD_QPSK_NORM));                 
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_DQPSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, 
                 RSSPECAN_VAL_MOD_QPSK_DIFF)); 
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_OQPSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, 
                 RSSPECAN_VAL_MOD_QPSK_OFFS)); 
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_DPI4QPSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, 
                 RSSPECAN_VAL_MOD_QPSK_DPI4)); 
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_BPSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_PSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 
                 2)); 
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_8PSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_PSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, 
                 RSSPECAN_VAL_MOD_PSK_NORM));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 
                 8)); 
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_D8PSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_PSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, 
                 RSSPECAN_VAL_MOD_PSK_DIFF));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 
                 8));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_N3PI8PSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_PSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, 
                 RSSPECAN_VAL_MOD_PSK_N3PI8));                   
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 
                 8));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_MSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_MSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_MSK_FORMAT, 
                 RSSPECAN_VAL_MOD_MSK_NORM));                    
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_DMSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_MSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_MSK_FORMAT, 
                 RSSPECAN_VAL_MOD_MSK_DIFF));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_16QAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_NORM));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 16));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_32QAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_NORM));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 32));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_64QAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_NORM));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 64));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_128QAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_NORM));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 128));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_256QAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_NORM));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 256));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_2FSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_FSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_NSTAT, 
                 2));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_4FSK:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_FSK)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_NSTAT, 
                 4));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_8VSB:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_VSB)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_VSB_NSTAT, 
                 8));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_16DQAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_DIFF));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 16));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_32DQAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_DIFF));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 32));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_64DQAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_DIFF));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 64));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_128DQAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_DIFF));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 128));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_256DQAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_QAM)); 
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, 
                 RSSPECAN_VAL_MOD_QAM_DIFF));                    
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 
                 256));
        break;
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_USERQAM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, 
                 RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM)); 
        break;
    } 
    switch (mode){
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_USERQAM:  
            viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_UQAM_MAPPING, 
                 mappingName), 5, "Mapping Name"); 
        break;
        default:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_MAPPING, 
                 mappingName), 5, "Mapping Name"); 
        break;
    }
    switch (mode){
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_MSK:
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_2FSK:
        case RSSPECAN_VAL_VSA_MODULATION_FORMAT_4FSK:
        break;
        default:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FILTER_STATE, 
                 signalFiltering), 6, "Signal Filtering");                 
        break;
    }       
                 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PRATE, 
                 pointsSymbol), 8, "Points/Symbol");  
               
                 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA FSK Modulation
/* Purpose:  This function configures the FSK modulation parameters
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAFSKModulation(ViSession  instrSession,
                                                    ViInt32     window,
                                                    ViBoolean   deviationCompensation,
                                                    ViReal64    fskDeviation)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";      
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "C%ld", window);  
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV_COMP, 
                 deviationCompensation), 3, "Deviation Compensation");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV, 
                 fskDeviation), 4, "FSK Deviation");                 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Modulation Filters
/* Purpose:  This function configures the modulation filters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationFilters(ViSession  instrSession,
                                                        ViInt32     window,
                                                        ViString    txFilter,
                                                        ViString    receiveFilter,
                                                        ViString    measFilter)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
	if (rsspecan_IsFSV (instrSession))
		checkErr (RS_ERROR_INSTRUMENT_MODEL);
		
    viCheckErr (viPrintf (instrSession, "SENS%ld:DDEM:FILT:MOD '%s','%s','%s'\n",
                window, txFilter, receiveFilter, measFilter));
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get VSA Modulation Filter Catalog
/* Purpose:  This function reads the names of all available filters.
/*           
/*           Notes:
/*           
/*           (1) Driver returns an instrument specific error, when catalog 
/*           list is empty.
/*           
/*           (2) This function is available only in conjunction with option 
/*           FS-K70.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetVSAModulationFilterCatalog(ViSession  instrSession,
                                                        ViInt32     window,
                                                        ViInt32*    numberofFilters,
                                                        ViInt32     bufferSize,
                                                        ViChar      modulationFiltersList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
	if (rsspecan_IsFSV (instrSession))
		checkErr (RS_ERROR_INSTRUMENT_MODEL);
		
	checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
	if (!(strstr (buffer, "K70")))    
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Buffer Size");         
    if ((bufferSize > 0) && (modulationFiltersList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Modulation Filter List");  
       
    viCheckErr (viPrintf (instrSession, ":SENS%ld:DDEM:FILT:CAT?\n", window));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, modulationFiltersList, numberofFilters));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Demodulation
/* Purpose:  This function configures the modulation.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSADemodulation(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     recordingLength,
                                                    ViReal64    recordLength,
                                                    ViBoolean   multiburstSearch,
                                                    ViInt32     sideBand,
                                                    ViBoolean   normalize,
                                                    ViInt32     pointsSymbol)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     length;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
	
    if (rsspecan_invalidViInt32Range (recordingLength, RSSPECAN_VAL_VSA_DEM_RLEN_AUTO_ON, RSSPECAN_VAL_VSA_DEM_RLEN_TIME))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Recording Length");
    sprintf (buffer, "Win%ld", window);  
    switch (recordingLength){
        case RSSPECAN_VAL_VSA_DEM_RLEN_AUTO_ON:
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO, 
                 VI_TRUE)); 
        break;  
        case RSSPECAN_VAL_VSA_DEM_RLEN_SYMBOL:
            length= (ViInt32)recordLength;
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO, 
                 VI_FALSE));    
            viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_SYM, 
                 length), 4, "Record Length");  
        break;
        case RSSPECAN_VAL_VSA_DEM_RLEN_TIME:
            viCheckErr (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO, 
                 VI_FALSE));    
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_S, 
                 recordLength), 4, "Record Length");    
        break;
    }
    if (!rsspecan_IsFSV (instrSession))
	{
	    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_MULTIBURST_STATE, 
                 multiburstSearch), 5, "Multiburst Search");
	}
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_SIDE_BAND, 
                 sideBand), 6, "Side Band");   
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_NORMALIZE, 
                 normalize), 7, "Normalize");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PRATE, 
                 pointsSymbol), 8, "Points/Symbol");    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Evaluation Lines State
/* Purpose:  This function configures the evaluation lines state
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAEvaluationLinesState(ViSession   instrSession,
                                                            ViInt32     window,
                                                            ViBoolean   evaluationLinesState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "C%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_EVAL_LINE_STATE, 
                evaluationLinesState), 3, "Evaluation Lines State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Evaluation Lines
/* Purpose:  This function configures the evaluation lines for measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAEvaluationLines(ViSession    instrSession,
                                                        ViInt32 window,
                                                        ViInt32 evaluationLine,
                                                        ViInt32 lineValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (evaluationLine, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Evaluation Line");        
    sprintf (buffer, "C%ld,L%ld", window, evaluationLine); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_EVAL_LINE, 
                lineValue), 4, "Line Value");    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Force WB Path
/* Purpose:  This function sets the use of signal path of option Bandwidth 
/*           Extension FSQ-B72 for symbol rates below 25
/*           MHz.
/*           
/*           Note(s):
/*           (1) This function is available only in conjuction with FSQ-B72 
/*           option.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAForceWBPath(ViSession    instrSession,
                                                    ViInt32     window,
                                                    ViBoolean   state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_WBAN_STATE, 
                 state), 3, "State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Unit Power
/* Purpose:  This function selects the unit for power in the selected 
/*           measurement window.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitPower(ViSession  instrSession,
                                                    ViInt32 window,
                                                    ViInt32 unitPower)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "C%ld", window); 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_UNIT_POWER, 
                 unitPower), 3, "Unit Power");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Display Fit Trace
/* Purpose:  This function sets the positioning the measurements results on 
/*           the display.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayFitTrace(ViSession    instrSession,
                                                        ViInt32     window,
                                                        ViInt32     trace,
                                                        ViInt32     fit,
                                                        ViInt32     symbolNumber,
                                                        ViInt32     patternPosition)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");        
    sprintf (buffer, "C%ld,TR%ld", window, trace); 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_REF_POINT, 
                 fit), 4, "Fit");
    sprintf (buffer, "Win%ld,TR%ld", window, trace);              
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_SYMBOL_NUMBER, 
                 symbolNumber), 5, "Symbol Number");
    sprintf (buffer, "Win%ld", window);                  
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_SYNC_OFFSET, 
                 patternPosition), 6, "Pattern Position");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Display Trace
/* Purpose:  This function sets the displaying of the trace.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayTrace(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     trace,
                                                    ViInt32     symbol,
                                                    ViInt32     spacing)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");        
    sprintf (buffer, "Win%ld,TR%ld", window, trace); 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_DISPLAY_TRACE_SYMBOL, 
                 symbol), 4, "Symbol");
	sprintf (buffer, "Win%ld", window); 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_TRACE_DISPLAY_SPACING, 
                 spacing), 5, "Spacing");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures whether the trace displays the evaluation of 
/// HIFN  the measured signal or the reference signal (if "Meas & Ref Signal" is 
/// HIFN  used as the signal source).
/// HIFN     
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR traceEvaluation/This control defines whether the trace displays the evaluation of the 
/// HIPAR traceEvaluation/measured signal or the reference signal.
ViStatus _VI_FUNC rsspecan_ConfigureVSATraceEvaluation (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 traceEvaluation)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";      
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "C%ld,TR1", window);  
    
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_TRACE_EVALUATION, 
	                 traceEvaluation), 3, "Trace Evaluation");
 	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Multiple Evaluation
/* Purpose:  This function configures the VSA multiple evaluation.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAMultipleEvaluation(ViSession instrSession,
                                                        ViInt32     window,
                                                        ViBoolean   captureAuto,
                                                        ViBoolean   multiburstState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
		
    sprintf (buffer, "Win%ld", window);
	
	if (!rsspecan_IsFSV (instrSession))
	{
	    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_CAPTURE_AUTO, 
	                 captureAuto), 4, "Capture Auto");
	}
	
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_STATE, 
                 multiburstState), 5, "Multiburst State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*===========================================================================*/
/* Function: Configure VSA Multiple Evaluation Zoom
/* Purpose:  This function configures the VSA multiple evaluation zoom parameters.
//*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAMultipleEvaluationZoom(ViSession instrSession,
                                                                ViInt32 window,
                                                                ViInt32 zoomStart,
                                                                ViInt32 zoomLength)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_LENGTH, 
                 zoomLength), 5, "Zoom Length");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_START, 
                 zoomStart), 4, "Zoom Start");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: VSA Search Burst in Capture RAM
/* Purpose:  This function controls demodulation in multiple evaluation and 
/*           searches the burst/sync pattern in the Capture Ram.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_VSASearchBurstInCaptureRam(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 action)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (action, RSSPECAN_VAL_VSA_MBURS_SEARCH_FIRST, RSSPECAN_VAL_VSA_MBURS_SEARCH_START) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Action");        
     
    sprintf (buffer, "Win%ld", window); 
    switch (action){
        case RSSPECAN_VAL_VSA_MBURS_SEARCH_FIRST:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_SEARCH_FIRST, VI_NULL));
        break;
        case RSSPECAN_VAL_VSA_MBURS_SEARCH_NEXT:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_SEARCH_NEXT, VI_NULL));
        break;
        case RSSPECAN_VAL_VSA_MBURS_SEARCH_START:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_VSA_MBURST_SEARCH_START, VI_NULL));
        break;
    }   
        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure VSA Measurement Results
/* Purpose:  This function configures the VSA measurement or reference 
/*           signal result displays.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAMeasurementResults(ViSession instrSession,
                                                        ViInt32     window,
                                                        ViInt32     measurementSignal,
                                                        ViInt32     resultDisplay,
                                                        ViBoolean   spectrum,
                                                        ViBoolean   statistics)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "",
                buffer2[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (measurementSignal, RSSPECAN_VAL_VSA_DATA_SIGNAL_MEAS, RSSPECAN_VAL_VSA_DATA_SIGNAL_REF) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Measurement Signal");        
    if (rsspecan_invalidViInt32Range (resultDisplay, RSSPECAN_VAL_RESULT_DISP_FREQ_ABS, RSSPECAN_VAL_RESULT_DISP_EYE_Q) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Result Display"); 
        
    sprintf (buffer, "C%ld", window);
    sprintf (buffer2, "Win%ld", window);
    switch (measurementSignal){
        case RSSPECAN_VAL_VSA_DATA_SIGNAL_MEAS:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_VSA_RESULT,
                                                      RSSPECAN_VAL_RESULT_XTIM_DDEM_MEAS));
        break;
        case RSSPECAN_VAL_VSA_DATA_SIGNAL_REF:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_VSA_RESULT,
                                                      RSSPECAN_VAL_RESULT_XTIM_DDEM_REF));
        break;
    }       
        
    switch (resultDisplay){
        case RSSPECAN_VAL_RESULT_DISP_FREQ_ABS:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_ABS));
        break;
        
        case RSSPECAN_VAL_RESULT_DISP_FREQ_REL:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_REL));

        break;  
        case RSSPECAN_VAL_RESULT_DISP_RIM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_IQ_CONST:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_CONS));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_IQ_VECT:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_COMP));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_MAGN_ABS:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_ABS));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_MAGN_REL:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_REL));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_PHAS_WRAP:
             viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_PHAS_UWRAP:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_UPHAS));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_EYE_I:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_IEYE));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_EYE_Q:   
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_QEYE));
        break;  
    }
    switch (resultDisplay){ 
        case RSSPECAN_VAL_RESULT_DISP_FREQ_ABS:
        case RSSPECAN_VAL_RESULT_DISP_FREQ_REL:
        case RSSPECAN_VAL_RESULT_DISP_RIM:
        case RSSPECAN_VAL_RESULT_DISP_MAGN_ABS:
        case RSSPECAN_VAL_RESULT_DISP_MAGN_REL: 
        case RSSPECAN_VAL_RESULT_DISP_PHAS_WRAP:
        case RSSPECAN_VAL_RESULT_DISP_PHAS_UWRAP:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, 
                    spectrum), 5, "Spectrum");
            if (!spectrum)      
                viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, 
                    statistics), 6, "Statistics");  
        break;
    }    
                 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Error Results
/* Purpose:  This function configures the VSA error result displays.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAErrorResults(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     resultDisplay,
                                                    ViBoolean   spectrum,
                                                    ViBoolean   statistics)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "",
                buffer2[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (resultDisplay, RSSPECAN_VAL_RESULT_DISP_FREQ_ABS, RSSPECAN_VAL_RESULT_DISP_EVM) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Result Display"); 
    sprintf (buffer, "C%ld", window);
    sprintf (buffer2, "Win%ld", window);    
    switch (resultDisplay){
        case RSSPECAN_VAL_RESULT_DISP_EVM:
        break;
        case RSSPECAN_VAL_RESULT_DISP_IQ_CONST:
        case RSSPECAN_VAL_RESULT_DISP_IQ_VECT:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_VSA_RESULT,
                                                      RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_VECT));
        break;
        default:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_VSA_RESULT,
                                                      RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_MPH));
        break;
    }   
    switch (resultDisplay){
        case RSSPECAN_VAL_RESULT_DISP_FREQ_ABS:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_ABS));
        break;
        
        case RSSPECAN_VAL_RESULT_DISP_FREQ_REL:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_REL));

        break;  
        case RSSPECAN_VAL_RESULT_DISP_RIM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_IQ_CONST:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_CONS));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_IQ_VECT:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_COMP));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_MAGN_ERR:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_PHAS_ERR:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_AMPM_CONV:
             viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_CONV));
        break;  
        case RSSPECAN_VAL_RESULT_DISP_EVM:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_VSA_RESULT,
                                                      RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_VECT));
        break;  
    }
    switch (resultDisplay){ 
        case RSSPECAN_VAL_RESULT_DISP_FREQ_ABS:
        case RSSPECAN_VAL_RESULT_DISP_FREQ_REL:
        case RSSPECAN_VAL_RESULT_DISP_RIM:
        case RSSPECAN_VAL_RESULT_DISP_MAGN_ERR:
        case RSSPECAN_VAL_RESULT_DISP_PHAS_ERR: 
        case RSSPECAN_VAL_RESULT_DISP_EVM:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, 
                         spectrum), 5, "Spectrum");
            if (!spectrum)                       
                viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, 
                         statistics), 6, "Statistics");
        break;
    }   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Symbol Table Results
/* Purpose:  This function configures the VSA symbol table result displays.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSASymbolTableResults(ViSession instrSession,
                                                        ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "C%ld", window);
    
    viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                              RSSPECAN_ATTR_VSA_RESULT,
                                              RSSPECAN_VAL_RESULT_XTIM_DDEM_SYMB));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*===========================================================================*/
/* Function: Configure VSA Equalizer Results
/* Purpose:  This function configures adaptive equalizer filter results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSAEqualizerResults(ViSession   instrSession,
                                                        ViInt32     window,
                                                        ViInt32     resultDisplay)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      buffer2[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
          
    if (rsspecan_invalidViInt32Range (resultDisplay, RSSPECAN_VAL_RESULT_DISP_FREQ_ABS, RSSPECAN_VAL_RESULT_EQDISP_CHRESP_LOG) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Display");           
    sprintf (buffer, "C%ld", window); 
    sprintf (buffer2, "Win%ld", window); 
    switch (resultDisplay){
        case RSSPECAN_VAL_RESULT_EQDISP_MAGN_LIN:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XTIM_DDEM_IMP));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VERTICAL_SCALE,
                RSSPECAN_VAL_LIN));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_MAGN_LOG:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XTIM_DDEM_IMP));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VERTICAL_SCALE,
                RSSPECAN_VAL_LOG));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_PHAS_WRAP:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XTIM_DDEM_IMP));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_PHAS_UWRAP:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XTIM_DDEM_IMP));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_UPHAS));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_RIM:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XTIM_DDEM_IMP));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_GDELAY:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                                                      RSSPECAN_VAL_RESULT_XFR_DDEM_RAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_GDEL));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_PRESP_WRAP:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XFR_DDEM_RAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_PRESP_UWRAP:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XFR_DDEM_RAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_UPHAS));
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_FRESP_LIN:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XFR_DDEM_RAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VERTICAL_SCALE,
                RSSPECAN_VAL_LIN));             
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_FRESP_LOG:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XFR_DDEM_RAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VERTICAL_SCALE,
                RSSPECAN_VAL_LOG));                 
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_CHRESP_LIN:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XFR_DDEM_IRAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VERTICAL_SCALE,
                RSSPECAN_VAL_LIN));             
        break;
        case RSSPECAN_VAL_RESULT_EQDISP_CHRESP_LOG:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT,
                RSSPECAN_VAL_RESULT_XFR_DDEM_IRAT));
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2, RSSPECAN_ATTR_VERTICAL_SCALE,
                RSSPECAN_VAL_LOG));                 
        break;

    }   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/*===========================================================================*/
/* Function: Configure VSA Raw Signal
/* Purpose:  This function configures the VSA measurement or reference 
/*           signal result displays.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSARawSignal(ViSession  instrSession,
                                                ViInt32     window,
                                                ViInt32     resultDisplay,
                                                ViBoolean   spectrum,
                                                ViBoolean   statistics)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "",
                buffer2[RSSPECAN_IO_BUFFER_SIZE] = "";
                
    checkErr( Rs_LockSession (instrSession, VI_NULL));
            
    if (rsspecan_invalidViInt32Range (resultDisplay, RSSPECAN_VAL_RESULT_DISP_FREQ_ABS, 
                                        RSSPECAN_VAL_RESULT_DISP_MAGN_RAW) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Result Display");           
    sprintf (buffer, "C%ld", window); 
    sprintf (buffer2, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                              RSSPECAN_ATTR_VSA_RESULT,
                                              RSSPECAN_VAL_RESULT_TCAP));
    switch (resultDisplay){
        case RSSPECAN_VAL_RESULT_DISP_FREQ_ABS:
             viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, 
                 spectrum), 4, "Spectrum");
            if (!spectrum)
                viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, 
                 statistics), 5, "Statistics");
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_ABS));
            
        break;
        case RSSPECAN_VAL_RESULT_DISP_FREQ_REL:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                       RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                       RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, 
                 spectrum), 4, "Spectrum");
            if (!spectrum)
                viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, 
                 statistics), 5, "Statistics");     
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_REL));
        break;
        case RSSPECAN_VAL_RESULT_DISP_RIM:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, 
                 spectrum), 4, "Spectrum");
            if (!spectrum)
                viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, 
                 statistics), 5, "Statistics");                                                       
        break;
        case RSSPECAN_VAL_RESULT_DISP_MAGN_RAW:
            viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_VSA_RESULT_FORMAT,
                                                      RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
        break;
    }   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get X Marker Position
/* Purpose:  This function returns the absolute X marker position for AM & PM 
*            conversion measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetVSAXMarkerPosition (ViSession instrSession,
                                                  ViInt32 window, 
                                                  ViInt32 marker,
                                                  ViReal64 *position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
       
    if (rsspecan_invalidViInt32Range (marker, 1, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");      
    sprintf (buffer, "Win%ld,M%ld", window, marker);     
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_X_MARKER_POSITION, position), 
                2, "Position");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure VSA Compression Point Measurement
/* Purpose:  This function compression point measurement. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureVSACompressionPointMeasurement(ViSession    instrSession,
                                                                    ViInt32     window,
                                                                    ViBoolean   state,
                                                                    ViReal64    value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "C%ld", window);       
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_CPOINT_STATE, state), 
                3, "State");
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_CPOINT_VALUE, value), 
                4, "Value");                
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get VSA Compression Point Result
/* Purpose:  This function returns teh compression point measurement 
/*           results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_GetVSACompressionPointResults(ViSession  instrSession,
                                                        ViInt32     window,
                                                        ViInt32     type,
                                                        ViReal64    results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViReal64    tmp_real = 0.0;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";    

    checkErr( Rs_LockSession (instrSession, VI_NULL));

	viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
	if (rsspecan_IsFSV (instrSession))
		checkErr (RS_ERROR_INSTRUMENT_MODEL);
		
    if (results == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Results");        
    sprintf (buffer, "C%ld", window);       

    checkErr( rsspecan_CheckStatus (instrSession));
    switch (type){
        case RSSPECAN_VAL_VSA_CPOINT_PSHIFT:
            viCheckErr (rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_CPOINT_PHASE_SHIFT, &tmp_real)); 
            results[0]=tmp_real;
        break;
        case RSSPECAN_VAL_VSA_CPOINT_POWER:
            viCheckErr (rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_VSA_CPOINT_POWER, &tmp_real));   
            results[0]=tmp_real;
        break;
        case RSSPECAN_VAL_VSA_CPOINT_DATA:
            viCheckErr (viQueryf(instrSession, "CALC%ld:MARK:FUNC:CPO:DATA?\n","%#lf", 
            window, results));  
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Type");            
    }
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
