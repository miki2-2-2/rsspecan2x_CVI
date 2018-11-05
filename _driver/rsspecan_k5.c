
/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  RSSpecAn K5 - GSM/EDGE MS/BS Test 
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

 *****************************************************************************/

#include "rsspecan.h"


static ViString absRelArr[]={"ABS","REL", VI_NULL};   
static ViString statusArr[]={"PASSED","FAILED","MARGIN","EXC", VI_NULL};

/*****************************************************************************
 * Function:    GSM Mode
 * Purpose:     This function selects the GSM/EDGE analyzer for mobile and 
 *              base station tests.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GsmMode (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_GSM_MODE, VI_NULL));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}



/*****************************************************************************
 * Function:    Configure GSM Trigger
 * Purpose:     This function configures the GSM Trigger Adjustment.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmTrigger (ViSession instrSession,
                                                ViInt32 triggerSource,
                                                ViReal64 triggerValue)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    if ((triggerSource<RSSPECAN_VAL_GSM_TRIG_FRUN)||(triggerSource>RSSPECAN_VAL_GSM_TRIG_RFP))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Trigger Source");
        
    switch (triggerSource){
        case RSSPECAN_VAL_GSM_TRIG_FRUN:
            viCheckErr (rsspecan_SetAttributeViString(instrSession,"",RSSPECAN_ATTR_GSM_SYNC_ADJ_IMM, ""));
        break;    
        case RSSPECAN_VAL_GSM_TRIG_EXT: 
            viCheckParm (rsspecan_SetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_SYNC_ADJ_EXT, triggerValue), 3, "Trigger Value");
        break;    
        case RSSPECAN_VAL_GSM_TRIG_IFP: 
            viCheckParm (rsspecan_SetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_SYNC_ADJ_IFP, triggerValue), 3, "Trigger Value");
        break;    
        case RSSPECAN_VAL_GSM_TRIG_RFP: 
            viCheckParm (rsspecan_SetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_SYNC_ADJ_RFP, triggerValue), 3, "Trigger Value");
        break;  
    }    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Demodulation
 * Purpose:     This function configures the the major parameters of the 
 *              demodulator in the GSM/EDGE application.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmDemodulation (ViSession instrSession,
                                                     ViInt32 modulationType,
                                                     ViInt32 multiSlot,
                                                     ViInt32 samplesPerSymbols,
                                                     ViBoolean syncSearch,
                                                     ViBoolean burstSearch,
                                                     ViReal64 burstSearchThreshold,
                                                     ViInt32 midamble,
                                                     ViString userSequence)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    if (rsspecan_invalidViInt32Range (midamble, RSSPECAN_VAL_GSM_TSC_0, RSSPECAN_VAL_GSM_TSC_AB2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Midamble");

    if ((syncSearch == VI_FALSE) && (burstSearch == VI_FALSE))
        viCheckErr (RS_ERROR_INVALID_CONFIGURATION);
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_CHAN_TSC, midamble), 8, "Midamble");
    switch (midamble){
        case RSSPECAN_VAL_GSM_TSC_AB0:
        case RSSPECAN_VAL_GSM_TSC_AB1:
        case RSSPECAN_VAL_GSM_TSC_AB2:
        break;
        default:
            viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_MTYP, modulationType), 2, "Modulation Type");
            viCheckParm (rsspecan_SetAttributeViBoolean(instrSession,"",RSSPECAN_ATTR_GSM_BSE, burstSearch), 6, "Burst Search");
            viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_SLOT_MULT, multiSlot), 3, "Multi Slot");
    }
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_PRAT, samplesPerSymbols), 4, "Samples Per Symbols");
    viCheckParm (rsspecan_SetAttributeViBoolean(instrSession,"",RSSPECAN_ATTR_GSM_SSE, syncSearch), 5, "Sync Search");
    viCheckParm (rsspecan_SetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_BSTH, burstSearchThreshold), 7, "Burst Search Threshold");
    
    if (midamble == RSSPECAN_VAL_GSM_USER)
    {
      switch (modulationType){
        case RSSPECAN_VAL_GSM_MTYP_GMSK:
            if ((strlen(userSequence)!=26)||(strspn (userSequence, "01")))
                viCheckParm( RS_ERROR_INVALID_PARAMETER, 9, "User Sequence");    
        break;
        case RSSPECAN_VAL_GSM_MTYP_EDGE:
            if ((strlen(userSequence)!=78)||(strspn (userSequence, "01")))  
                viCheckParm( RS_ERROR_INVALID_PARAMETER, 9, "User Sequence");  
        break;
      }
      viCheckParm (rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_GSM_TSC_USER, userSequence), 10, "User Sequence");
    }    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}



/*****************************************************************************
 * Function:    Configure GSM Multi Carrier Mode
 * Purpose:     This function sets the multi carrier mode to ON or OFF.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmMultiCarrierMode (ViSession instrSession,
                                                     ViBoolean multiCarrierMode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    viCheckParm (rsspecan_SetAttributeViBoolean(instrSession,"",RSSPECAN_ATTR_GSM_MCAR, multiCarrierMode), 2, "Multi Carrier MOde");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Transient Spectrum Limit Type
 * Purpose:     This function selects the type of limit check of the measurement 
 *              of the spectrum due to switching transients.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmTransSpecLimitType (ViSession instrSession,
                                                  ViInt32 limitType)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_SWIT_LIM, limitType), 2, "Limit Type");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM IF/RF Trigger As IQ Power
 * Purpose:     This function sets the force the IF-power or with FSP-B6 
 *              RF-power trigger.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmIFRFTriggerAsIQPower (ViSession instrSession,
                                                             ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm (rsspecan_SetAttributeViBoolean(instrSession,"",RSSPECAN_ATTR_GSM_SYNC_IQPOWER, state), 2, "State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Modulation Spectrum List Average
 * Purpose:     This function selects linear and logarithmic averaging in 
 *              the modulation spectrum list measurement. In LIN mode voltages
 *              are averaged. In LOG mode levels.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmModulationSpectrumListAverage (ViSession instrSession,
                                                                      ViInt32 listAverage)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession, "",
                RSSPECAN_ATTR_GSM_MODULATION_SPECTRUM_LIST_AVER_TYPE, listAverage), 2, "List Average");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Restore GSM Limit Lines
 * Purpose:     This function restores the GSM/EDGE limit lines.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreGsmLimitLines (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_GSM_REST, ""));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Extended Slot State
 * Purpose:     This function activates the extended slot configuration.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotState (ViSession instrSession,
                                                          ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_STATE, state),
        2, "State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Extended Slot
 * Purpose:     This function configures the GSM Extended Slot for Multi 
 *              Slot measurements. These values are standard specific
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlot (ViSession instrSession,
                                                             ViInt32 standard,
                                                             ViInt32 triggerReference,
                                                             ViInt32 referenceMidable,
                                                             ViBoolean absoluteLevelState,
                                                             ViReal64 limitBase1,
                                                             ViReal64 limitBase2)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_STANDARD, standard),
        2, "Standard");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_TRIG_REFERENCE, triggerReference),
        3, "Trigger Reference");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_MIDAMBLE_REFERENCE, referenceMidable),
        4, "Reference Midable");
    if (standard >= RSSPECAN_VAL_GSM_EXTSLOT_GSM900)
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_STATE, absoluteLevelState),
            5, "Absolute Level State");
        if (absoluteLevelState == VI_TRUE)
        {
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "BAS1", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_VALUE, limitBase1),
            6, "Limit Base 1");
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "BAS2", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_VALUE, limitBase2),
            7, "Limit Base 2");
        }
    }   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Extended Slot Common Settings
 * Purpose:     This function configures the GSM Extended Slot for Multi 
 *              Slot measurements. These configurations are common for all 
 *              standards.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotCommonSettings (ViSession instrSession,
                                                     ViBoolean longSlots,
                                                     ViInt32 arraySize,
                                                     ViInt32 longSlotsValues[],
                                                     ViBoolean onlyOneFrame)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    if (rsspecan_invalidViInt32Range (arraySize, 2, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LONG_SLOT, longSlots),
        2, "Long Slots");
    viCheckErr (viPrintf (instrSession, "CONF:ECON:LSL:VAL %ld,%ld\n", longSlotsValues[0], longSlotsValues[1]));
    checkErr( rsspecan_CheckStatus (instrSession)); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_OFRAME, onlyOneFrame),
        3, "Only One Frame");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Configure GSM Extended Slot Parameters
 * Purpose:     This function configures the slot specific parameters for 
 *              GSM Extended Slot for Multi Slot measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotParameters (ViSession instrSession,
                                                               ViInt32 slotNumber,
                                                               ViInt32 modulation,
                                                               ViInt32 referenceLevelMode,
                                                               ViReal64 referenceLevelValue,
                                                               ViInt32 limitLineCtrlLevel,
                                                               ViString limitLineLower,
                                                               ViString limitLineUpper)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    if (rsspecan_invalidViInt32Range (slotNumber, 0, 7) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Slot Number");
    
    sprintf (buffer, "SL%ld", slotNumber);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_MODULATION, modulation),
        3, "Modulation");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_RLEV_MODE, referenceLevelMode),
        4, "Reference Level Mode");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_RLEV_VALUE, referenceLevelValue),
        5, "Reference Level Value");
    if (modulation != RSSPECAN_VAL_EXT_SLOT_MODUL_OFF)   
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT_CLEV, limitLineCtrlLevel),
        6, "Limit Line Ctrl Level");
        sprintf (buffer, "SL%ld,Low", slotNumber);
        viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT, limitLineLower),
        7, "Limit Line Lower");
        sprintf (buffer, "SL%ld,Upp", slotNumber); 
        viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT, limitLineUpper),
        8, "Limit Line Upper");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Measurement
 * Purpose:     This function configures the GSM Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmMeasurement (ViSession instrSession,
                                                    ViInt32 measurementMode)
{
    ViStatus    error = VI_SUCCESS;
    ViAttr      attr[] = {RSSPECAN_ATTR_GSM_BURS_PFER,RSSPECAN_ATTR_GSM_BURS_MACC,
                          RSSPECAN_ATTR_GSM_BURS_POW,RSSPECAN_ATTR_GSM_BURS_PTEM,
                          RSSPECAN_ATTR_GSM_SPEC_MOD,RSSPECAN_ATTR_GSM_SPEC_SWIT,
                          RSSPECAN_ATTR_GSM_SPUR}  ;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    if ((measurementMode<RSSPECAN_VAL_GSM_MEAS_PFER)||(measurementMode>RSSPECAN_VAL_GSM_MEAS_SPUR))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Measurement Mode");
        
    checkErr (rsspecan_SetAttributeViString(instrSession,"",attr[measurementMode],VI_NULL));        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM PVT Measurement
 * Purpose:     This function configures the the major parameters of the demodulator 
 *              in the GSM/EDGE application.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmPVTMeasurement (ViSession instrSession,
                                                       ViInt32 burstSection,
                                                       ViBoolean highResolutionMode,
                                                       ViInt32 filterBandwidth,
                                                       ViInt32 transitionNo)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     tmp_burstSection;  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    viCheckErr (rsspecan_GetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_SEL, &tmp_burstSection)); 
    if (tmp_burstSection == RSSPECAN_VAL_GSM_BURST_SEC_FULL)
        viCheckErr (rsspecan_SetAttributeViBoolean(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_TMHR, VI_FALSE));            
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_SEL, burstSection), 2, "Burst Section");
    switch (burstSection) {
        case RSSPECAN_VAL_GSM_BURST_SEC_FULL:
            viCheckParm (rsspecan_SetAttributeViBoolean(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_TMHR, highResolutionMode), 3, "High Resolution Mode");      
        break;
        case RSSPECAN_VAL_GSM_BURST_SEC_FRZ:
            viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_FRZ, transitionNo), 5, "Transition No");  
        break;  
    }
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_SEL, burstSection), 2, "Burst Section");
    if ((burstSection != RSSPECAN_VAL_GSM_BURST_SEC_FULL) || (highResolutionMode == VI_FALSE))
                viCheckParm (rsspecan_SetAttributeViInt32(instrSession,"",RSSPECAN_ATTR_GSM_BURS_PTEM_FILT, filterBandwidth), 4, "Filter Bandwidth");    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Level Time Values
 * Purpose:     This function starts a measurement sequence which automatically 
 *              adjust the level and trigger timing of the input signal to 
 *              the analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmLevelTime (ViSession instrSession,
                                             ViUInt32   timeout,  
                                             ViBoolean *status,
                                             ViReal64 *signalPowerdBm,
                                             ViReal64 *triggerTimes,
                                             ViReal64 *triggerLeveldBmV,
                                             ViReal64 *reserved)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      tmp_stat_text[10];
    ViInt32     tmp_status;
    ViReal64    tmp_signalPowerdBm,
                tmp_triggerTimes,
                tmp_triggerLeveldBmV,
                tmp_reserved;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
        
    viCheckErr (viPrintf (instrSession, "READ:AUTO:LEVT?;*OPC\n"));
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout)); 
    viCheckErr (viScanf (instrSession, "%[^,],%le,%le,%le,%le", 
                &tmp_stat_text, &tmp_signalPowerdBm, &tmp_triggerTimes, &tmp_triggerLeveldBmV, &tmp_reserved));
                    
    if ((tmp_status = rsspecan_StringIndex (statusArr, tmp_stat_text)) < 0)
        viCheckErr(RS_ERROR_UNEXPECTED_RESPONSE); 
    
    
    if (status) *status = (tmp_status==0)?VI_FALSE:VI_TRUE;
    if (signalPowerdBm) *signalPowerdBm=tmp_signalPowerdBm;
    if (triggerTimes) *triggerTimes=tmp_triggerTimes;
    if (triggerLeveldBmV) *triggerLeveldBmV=tmp_triggerLeveldBmV;
    if (reserved) *reserved=tmp_reserved;
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Phase Freqeuncy Error Values
 * Purpose:     This function starts the Phase Frequency Error measurement 
 *              and returns the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmPhaseFreqErrorValues (ViSession instrSession,
                                                        ViUInt32    timeout,
                                                        ViInt32 measurement,
                                                        ViInt32 modifier,
                                                        ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if ((measurement<RSSPECAN_VAL_GSM_PERR_RMS)||(measurement>RSSPECAN_VAL_GSM_IQIM))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Measurement");
    if ((modifier!=RSSPECAN_VAL_MEASTYPE_MAX)&&(modifier!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Modifier");        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    switch (modifier){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PERR_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_PERR_RMS_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_PERR_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_PERR_PEAK_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_FERR:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_FERR_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_IQOF_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_IQIM_MAX,value), 4, "Value");
                break;
            }
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PERR_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_PERR_RMS_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_PERR_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_PERR_PEAK_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_FERR:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_FERR_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_IQOF_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_IQIM_AVER,value), 4, "Value");
                break;
            }
        break;
    }    
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Phase Freqeuncy Error Values
 * Purpose:     This function returns the Phase Frequency Error measurement 
 *              result.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_FetchGsmPhaseFreqErrorValues (ViSession instrSession,
                                                         ViInt32 measurement,
                                                         ViInt32 modifier,
                                                         ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    if ((measurement<RSSPECAN_VAL_GSM_PERR_RMS)||(measurement>RSSPECAN_VAL_GSM_IQIM))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Measurement");
    if ((modifier!=RSSPECAN_VAL_MEASTYPE_MAX)&&(modifier!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Modifier");        

    switch (modifier){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PERR_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_PERR_RMS_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_PERR_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_PERR_PEAK_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_FERR:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_FERR_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_IQOF_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_IQIM_MAX,value), 4, "Value");
                break;
            }
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PERR_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_PERR_RMS_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_PERR_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_PERR_PEAK_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_FERR:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_FERR_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_IQOF_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_IQIM_AVER,value), 4, "Value");
                break;
            }
        break;
    }    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}


/*****************************************************************************
 * Function:    Read GSM Modulation Accuracy Values
 * Purpose:     This function starts the Modulation Accuracy measurement and 
 *              returns the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmModulAccValues (ViSession instrSession,
                                                  ViUInt32  timeout, 
                                                  ViInt32 measurement,
                                                  ViInt32 modifier,
                                                  ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1;
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if ((measurement<RSSPECAN_VAL_GSM_MACC_RMS)||(measurement>RSSPECAN_VAL_GSM_MACC_IQIM))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Measurement");
    if ((modifier!=RSSPECAN_VAL_MEASTYPE_MAX)&&(modifier!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Modifier");        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
   
    switch (modifier){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            switch (measurement){
                case RSSPECAN_VAL_GSM_MACC_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_RMS_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_PEAK_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_OSUP:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_OSUP_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PERC:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_PERC_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_FREQ:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_FREQ_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQOF_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQIM_MAX,value), 4, "Value");
                break;
            }
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            switch (measurement){
                case RSSPECAN_VAL_GSM_MACC_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_RMS_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_PEAK_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_OSUP:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_OSUP_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PERC:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_PERC_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_FREQ:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_FREQ_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQOF_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQIM_AVER,value), 4, "Value");
                break;
            }
        break;
    }        
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Modulation Accuracy Values
 * Purpose:     This function returns the results of the Modulation Accuracy 
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchGsmModulAccValues (ViSession instrSession,
                                                   ViInt32 measurement,
                                                   ViInt32 modifier,
                                                   ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    
    if ((measurement<RSSPECAN_VAL_GSM_MACC_RMS)||(measurement>RSSPECAN_VAL_GSM_MACC_IQIM))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Measurement");
    if ((modifier!=RSSPECAN_VAL_MEASTYPE_MAX)&&(modifier!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Modifier");        

    switch (modifier){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            switch (measurement){
                case RSSPECAN_VAL_GSM_MACC_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_RMS_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PEAK_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_OSUP:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_OSUP_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PERC:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PERC_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_FREQ:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_FREQ_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQOF_MAX,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQIM_MAX,value), 4, "Value");
                break;
            }
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            switch (measurement){
                case RSSPECAN_VAL_GSM_MACC_RMS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_RMS_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PEAK:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PEAK_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_OSUP:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_OSUP_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_PERC:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PERC_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_FREQ:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_FREQ_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQOF:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQOF_AVER,value), 4, "Value");
                break;
                case RSSPECAN_VAL_GSM_MACC_IQIM:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQIM_AVER,value), 4, "Value");
                break;
            }
        break;
    }        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}
/*****************************************************************************
 * Function:    Read GSM Ptemplate Ref
 * Purpose:     This function starts the premeasurement of power vs. time and 
 *              read out the results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmPtempRef (ViSession instrSession,
                                            ViUInt32    timeout,
                                            ViReal64 returnedValues[])
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if (!returnedValues)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Returned Values");    
        
    viCheckErr( viPrintf (instrSession,"READ:BURS:PTEM:REF?;*OPC\n"));     
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout)); 
    viCheckErr( viScanf (instrSession,"%le,%le,%le", &returnedValues[0],
                            &returnedValues[1], &returnedValues[2]));                             
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Power vs. Time Values
 * Purpose:     This function starts the Power Vs. Time measurement and 
 *              returns the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmPowVsTimeValues (ViSession instrSession,
                                                   ViUInt32 timeout,
                                                   ViInt32 measurement,
                                                   ViInt32 modifier,
                                                   ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if ((measurement!=RSSPECAN_VAL_GSM_PVT_TRGS))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Measurement");
    if ((modifier!=RSSPECAN_VAL_MEASTYPE_MAX)&&(modifier!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Modifier");        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    switch (modifier){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PVT_TRGS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_PTEM_TRGS_MAX,value), 4, "Value");
                break;
             }
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PVT_TRGS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_READ_BURST_PTEM_TRGS_AVER,value), 4, "Value");
                break;
             }
        break;
    }
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Extended Slot Ptemplate Ref
 * Purpose:     This function starts the pre-measurement of power vs time and 
 *              reads out the result in the extended slot configuration mode.
 * 
 *              Note(s):
 *              (1) This function is available only if Power Vs. Time is
 *              selected with attribute RSSPECAN_ATTR_GSM_BURS_PTEM.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmExtendedSlotPtempRef (ViSession instrSession,
                                                        ViUInt32 timeout,
                                                        ViInt32 arraySize,
                                                        ViInt32 slotNumber[],
                                                        ViReal64 measuredLevel[],
                                                        ViReal64 correctedLevel[],
                                                        ViReal64 resolutionBandwidth[])
{
	ViStatus	error = VI_SUCCESS;
	ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if (!arraySize)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Array Size");        
    
    viCheckErr (viPrintf (instrSession, "READ:BURS:PTEM:REF:ECON:IMM?;*OPC\n"));
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    cnt=0;
    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize){
            sscanf(pstring_value,"%ld",&slotNumber[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&measuredLevel[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&correctedLevel[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&resolutionBandwidth[cnt]);
            pstring_value = strtok(NULL, ","); 
        } 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Ptemplate Ref
 * Purpose:     This function reads out the results of the premeasurement of
 *              power vs. time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchGsmPtempRef (ViSession instrSession,
                                             ViReal64 returnedValues[])
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    
    if (!returnedValues)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Power List");    
        
    viCheckErr( viQueryf (instrSession,"FETC:BURS:PTEM:REF?\n", "%le,%le,%le", &returnedValues[0],
                            &returnedValues[1], &returnedValues[2]));     
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Power vs. Time Values
 * Purpose:     This function returns the Power Vs. Time measurement results.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_FetchGsmPowVsTimeValues (ViSession instrSession,
                                                    ViInt32 measurement,
                                                    ViInt32 modifier,
                                                    ViReal64 *value)
{
    ViStatus    error = VI_SUCCESS;

    
    checkErr( Rs_LockSession (instrSession, VI_NULL));   

    if ((measurement!=RSSPECAN_VAL_GSM_PVT_TRGS))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Measurement");
    if ((modifier!=RSSPECAN_VAL_MEASTYPE_MAX)&&(modifier!=RSSPECAN_VAL_MEASTYPE_AVER))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Modifier");        
    
    switch (modifier){
        case RSSPECAN_VAL_MEASTYPE_MAX:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PVT_TRGS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_PTEM_TRGS_MAX,value), 4, "Value");
                break;
             }
        break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            switch (measurement){
                case RSSPECAN_VAL_GSM_PVT_TRGS:
                    viCheckParm (rsspecan_GetAttributeViReal64(instrSession,"",RSSPECAN_ATTR_GSM_FETC_BURST_PTEM_TRGS_AVER,value), 4, "Value");
                break;
             }
        break;
    } 
Error:
    
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Extended Slot Ptemplate Ref
 * Purpose:     This function reads out the result in the extended slot 
 *              configuration mode.
 * 
 *              Note(s):
 *              (1) This function is available only if Power Vs. Time is
 *              selected with attribute RSSPECAN_ATTR_GSM_BURS_PTEM.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchGsmExtendedSlotPtempRef (ViSession instrSession,
                                                         ViInt32 arraySize,
                                                         ViInt32 slotNumber[],
                                                         ViReal64 measuredLevel[],
                                                         ViReal64 correctedLevel[],
                                                         ViReal64 resolutionBandwidth[])
{
	ViStatus	error = VI_SUCCESS;
	ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    if (!arraySize)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
    
    viCheckErr (viPrintf (instrSession, "FETC:BURS:PTEM:REF:ECON:IMM?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    cnt=0;
    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize){
            sscanf(pstring_value,"%ld",&slotNumber[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&measuredLevel[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&correctedLevel[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&resolutionBandwidth[cnt]);
            pstring_value = strtok(NULL, ","); 
        } 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Spectrum Modulation Ref  
 * Purpose:     This function starts the premeasurement of the modulation due 
 *              to spectrum measurement and reads out the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmSpecModRef (ViSession instrSession,  
                                              ViUInt32  timeout, 
                                              ViReal64 returnedValues[])
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if (!returnedValues)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Returned Values");    
        
    viCheckErr( viPrintf (instrSession,"READ:SPEC:MOD:REF?;*OPC\n"));  
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout));  
    viCheckErr( viScanf (instrSession,"%le,%le,%le", &returnedValues[0],
                            &returnedValues[1], &returnedValues[2])); 
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Modulation Spectrum
 * Purpose:     This function starts the measurement of the modulation spectrum 
 *              of the base station or mobile and reads out the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmModSpectrum (ViSession instrSession,
                                               ViInt32 arraySize,
                                               ViUInt32 timeout,
                                               ViInt32 index[],
                                               ViReal64 startFrequencies[],
                                               ViReal64 stopFrequencies[],
                                               ViReal64 levels[],
                                               ViReal64 limits[],
                                               ViInt32 absRel[],
                                               ViInt32 status[],
                                               ViInt32 *num_ofResults)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt;
    ViChar      tmp_str[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");
    if (!num_ofResults)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 10, "Num Of Results");        
    
    viCheckErr (viPrintf (instrSession, "READ:SPEC:MOD?;*OPC\n"));
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    cnt=0;
    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize){
            sscanf(pstring_value,"%ld",&index[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&startFrequencies[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&stopFrequencies[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&levels[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&limits[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,]",tmp_str);
            absRel[cnt]=rsspecan_StringIndex (absRelArr, tmp_str);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,'\n''\r']",tmp_str);
            status[cnt]=rsspecan_StringIndex (statusArr, tmp_str);
            pstring_value = strtok(NULL, ",");
        } 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    *num_ofResults=cnt;
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Spectrum Modulation Ref
 * Purpose:     This function reads out the results of the premeasurement 
 *              of modulation due to spectrum measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchGsmSpecModRef (ViSession instrSession,
                                               ViReal64 returnedValues[])
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    
    if (!returnedValues)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Returned Values"); 
        
    viCheckErr( viQueryf (instrSession,"FETC:SPEC:MOD:REF?\n", "%le,%le,%le", &returnedValues[0],
                            &returnedValues[1], &returnedValues[2]));  
    checkErr( rsspecan_CheckStatus (instrSession));                         
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}




/*****************************************************************************
 * Function:    Fetch GSM Modulation Spectrum
 * Purpose:     This function starts the measurement of the modulation spectrum 
 *              of the base station or mobile and reads out the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchGsmModSpectrum (ViSession instrSession,
                                                ViInt32 arraySize,
                                                ViInt32 index[],
                                                ViReal64 startFrequencies[],
                                                ViReal64 stopFrequencies[],
                                                ViReal64 levels[],
                                                ViReal64 limits[],
                                                ViInt32 absRel[],
                                                ViInt32 status[],
                                                ViInt32 *num_ofResults)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt;
    ViChar      tmp_str[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    if (!num_ofResults)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 10, "Num Of Results");        
    
    viCheckErr (viPrintf (instrSession, "FETC:SPEC:MOD?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    cnt=0;
    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize){
            sscanf(pstring_value,"%ld",&index[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&startFrequencies[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&stopFrequencies[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&levels[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&limits[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,]",tmp_str);
            absRel[cnt]=rsspecan_StringIndex (absRelArr, tmp_str);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,'\n''\r']",tmp_str);
            status[cnt]=rsspecan_StringIndex (statusArr, tmp_str);
            pstring_value = strtok(NULL, ",");
        } 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    *num_ofResults=cnt;
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Spectrum Transient Ref
 * Purpose:     This function starts the premeasurement of the modulation due 
 *               to switching transients (TRA) measurement and reads out the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmSpecTransRef (ViSession instrSession,
                                                ViUInt32    timeout,
                                                ViReal64 returnedValues[])
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));   
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    if (!returnedValues)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Returned Values");   
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    
    viCheckErr( viPrintf (instrSession,"READ:SPEC:SWIT:REF?;*OPC\n"));      
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout));        
    viCheckErr( viScanf (instrSession,"%le,%le,%le", &returnedValues[0],
                            &returnedValues[1], &returnedValues[2]));
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Read GSM Transient Spectrum
 * Purpose:     This function starts the measurement of the transient spectrum 
 *              of the mobile or base station and reads out the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadGsmTransSpectrum (ViSession instrSession,
                                                 ViInt32 arraySize,
                                                 ViUInt32   timeout,
                                                 ViInt32 index[],
                                                 ViReal64 startFrequencies[],
                                                 ViReal64 stopFrequencies[],
                                                 ViReal64 levels[],
                                                 ViReal64 limits[],
                                                 ViInt32 absRel[],
                                                 ViInt32 status[],
                                                 ViInt32 *num_ofResults)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt;
    ViChar      tmp_str[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");
    if (!num_ofResults)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 10, "Num Of Results");        
    
    viCheckErr (viPrintf (instrSession, "READ:SPEC:SWIT?;*OPC\n"));
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout)); 
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    cnt=0;
    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize){
            sscanf(pstring_value,"%ld",&index[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&startFrequencies[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&stopFrequencies[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&levels[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&limits[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,]",tmp_str);
            absRel[cnt]=rsspecan_StringIndex (absRelArr, tmp_str);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,'\n''\r']",tmp_str);
            status[cnt]=rsspecan_StringIndex (statusArr, tmp_str);
            pstring_value = strtok(NULL, ",");
        } 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    *num_ofResults=cnt;
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Fetch GSM Transient Spectrum
 * Purpose:     This function reads out the result of the measurement of 
 *              the transient spectrum of the mobile or base station.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchGsmTransSpectrum (ViSession instrSession,
                                                  ViInt32 arraySize,
                                                  ViInt32 index[],
                                                  ViReal64 startFrequencies[],
                                                  ViReal64 stopFrequencies[],
                                                  ViReal64 levels[],
                                                  ViReal64 limits[],
                                                  ViInt32 absRel[],
                                                  ViInt32 status[],
                                                  ViInt32 *num_ofResults)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt;
    ViChar      tmp_str[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    if (!num_ofResults)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 10, "Num Of Results");        
    
    viCheckErr (viPrintf (instrSession, "FETC:SPEC:SWIT?\n"));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    cnt=0;
    pstring_value = strtok(pbuffer, ",");  
    while (pstring_value){
        if (cnt<arraySize){
            sscanf(pstring_value,"%ld",&index[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&startFrequencies[cnt]);
            pstring_value = strtok(NULL, ",");
            sscanf(pstring_value,"%le",&stopFrequencies[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&levels[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value,"%le",&limits[cnt]);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,]",tmp_str);
            absRel[cnt]=rsspecan_StringIndex (absRelArr, tmp_str);
            pstring_value = strtok(NULL, ","); 
            sscanf(pstring_value, "%[^,'\n''\r']",tmp_str);
            status[cnt]=rsspecan_StringIndex (statusArr, tmp_str);
            pstring_value = strtok(NULL, ",");
        } 
        cnt++;
    }
    if (pbuffer) free(pbuffer);
    
    *num_ofResults=cnt;
    checkErr( rsspecan_CheckStatus (instrSession)); 
Error:
    (void) (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 *- User defined callbacks --------------------------------------------------*
 *****************************************************************************/


/*****************************************************************************
 *- End Of RSSpecAn K5 - GSM/EDGE MS/BS Test --------------------------------*
 *****************************************************************************/
