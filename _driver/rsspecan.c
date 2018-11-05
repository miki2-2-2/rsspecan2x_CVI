/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  VXIpnp Style Instrument Driver
 *  Original Release: July 2008
 *  By: Martin Koutny (instrument driver core)
 *      Helena Sucha (instrument specific implementation)
 *      Jiri Kominek (DrBase repository & tools)
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *  Modification History:
 *     see ChangeLog
 *                     
 *****************************************************************************/
#include <math.h>
#include <float.h>
#include "rsspecan.h"
#include "time.h"
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
    #define ANSI
#endif
#ifndef ANSI                /* UNIX compatible */
    #include <sys/timeb.h>
    #include <sys/types.h>
    #include <unistd.h>
    #define Sleep sleep
#else                       /* ANSI compatible */
        #include <windows.h>
#endif


/*****************************************************************************
 *- Value Definition and Range Tables ---------------------------------------*
 *****************************************************************************/
static ViString listFilterTypeArr[] = {"NORM","CFIL","RRC","FFT","PULS"};
static ViString switchArr[] = {"OFF","ON"};
static ViString statProbArr[] = {"P0_1","P1","P10"};
static ViString measTypeArr[] = {"MEAN","PEAK","CFAC"};
static ViString resultsString = "PASSED,FAILED";
static ViString selStatusRegArr[] = {"",":POW",":LIM1",":LMAR1",":SYNC",":ACPL",
                                     ":FREQ",":LIM2",":LMAR2",":PNO",":DIQ",":DCPN",
                                     ":LPN",":LIM3", ":MOD", ":MOD:CFR", ":MOD:EVM",
                                     ":MOD:FSK", ":MOD:IQRH", ":MOD:MAGN", ":MOD:PHAS",
                                     ":EXT",":EXT:INFO",":TRAN"};
static ViString statusRegArr[] = {"OPER:EVEN","OPER:COND","QUES:EVEN","QUES:COND",
                                  "QUES:POW:EVEN","QUES:POW:COND","QUES:LIM:EVEN",
                                  "QUES:LIM:COND","QUES:LMAR:EVEN","QUES:LMAR:COND",
                                  "QUES:SYNC:EVEN","QUES:SYNC:COND","QUES:ACPL:EVEN",
                                  "QUES:ACPL:COND","QUES:FREQ:EVEN","QUES:FREQ:COND",
                                  "QUES:TRAN:EVEN","QUES:TRAN:COND","QUE",
                                  "QUES:LIM2:EVEN","QUES:LIM2:COND","QUES:LMAR2:EVEN",
                                  "QUES:LMAR2:COND","QUES:PNO:EVEN","QUES:PNO:COND",
                                  "OPER:PCAL:EVEN","OPER:PCAL:COND","QUES:DIQ:EVEN",
                                  "QUES:DIQ:COND","QUES:DCPN:EVEN","QUES:DCPN:COND",
                                  "QUES:LPN:EVEN","QUES:LPN:COND","QUES:LIM3:EVEN",
                                  "QUES:LIM3:COND", "QUES:MOD:EVEN", "QUES:MOD:COND",
                                  "QUES:MOD:CFR:EVEN", "QUES:MOD:CFR:COND", "QUES:MOD:EVM:EVEN",
                                  "QUES:MOD:EVM:COND", "QUES:MOD:FSK:EVEN", "QUES:MOD:FSK:COND",
                                  "QUES:MOD:IQRH:EVEN", "QUES:MOD:IQRH:COND", "QUES:MOD:MAG:EVEN",
                                   "QUES:MOD:MAG:COND", "QUES:MOD:PHAS:EVEN","QUES:MOD:PHAS:COND",
                                    "QUES:EXT","QUES:EXT:COND","QUES:EXT:INFO","QUES:EXT:INFO:COND"};
static ViString VSAResultRegArr[] =  {"Main", "CF", "EVM", "FSK", "IQRHO", "MagErr", "PhaseErr", VI_NULL};
static ViString RegisterPartArr[] =  {"RegCondition", "RegEnable", "RegEvent", "RegNTR", "RegPTR", VI_NULL};
static ViString limTypeArr[] = {"LOW", "UPP", VI_NULL};
static ViString directionArr[] =  {"LEFT", "RIGH", "ABOV", "BEL", VI_NULL};
static ViString windowTypeArr[] =  {"DIAG", "MTAB", "PEAK", "RSUM", "SGR", "MAGN", "FREQ", "VECT", "RIMAG",
                                    "MCAP", "PRES", "PST", "PMAG", "PPH", "PFR", "PTR", "NOIS", "GAIN", 
                                    "TEMP", "YFAC", "PHOT", "PCOL", "RES", "PNO", "RNO", "SNO", "SPUR",
                                    "GDEL", "PHAS", "RMAG", "RPH", "SPEC", "BITS", "CCON", "CDEP", "CDP",
                                    "CEVM", "CTAB", "LEV", "PCD", "PPCG", "PSYM", "SCON", "SEVM", "CURR", 
                                    "PPWR", "CBUF", "MEAS", "REF", "EQU", "EVEC", "MACC", "MERR", "SYMB", 
                                    "CMEM", "CONS", "CVC", "EVC", "EVSY", "FSP", "RSD", "RSGL", "SFI", "SFL", 
                                    "PFPP", "ASUM", "BSTR", "CCDF", "FLAT", "FSRS", "EVSU", "FEVS", "IE", 
                                    "IEA", "PSPE", "EVRP", "PVRP", "PVRR", "EVCH", "FDR", "SRES", "PDIS", 
                                    "PSP", VI_NULL};
static ViString ChannelTypeArr[] =  {"SANALYZER", "IQ", "PULSE", "ADEM", "MCGD", "NOISE", "PNOISE", "DDEM", 
                                     "BWCD", "MWCD", "BC2K", "MC2K", "MDO", "BDO", "GSM", "WLAN", "LTE", VI_NULL};
static ViString messageTypeArr[] =  {"ALL", "INFO", "WARN", "ERR", "FAT", "MESS", VI_NULL};

/*****************************************************************************
 *- Callback Declarations ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- User-Callable Functions (Exportable Functions) --------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_init
 * Purpose:  VXIplug&play required function. Calls the
 *           rsspecan_InitWithOptions function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_init (ViRsrc resourceName, ViBoolean IDQuery,
                                 ViBoolean resetDevice, ViSession *newInstrSession)
{
	ViStatus error = VI_SUCCESS;

	if (newInstrSession == NULL)
	{
		(void)RsCore_SetErrorInfo(0, VI_FALSE, RS_ERROR_INVALID_PARAMETER,
		                          VI_ERROR_PARAMETER4, "Null address for Instrument Handle");

		checkErr(RS_ERROR_INVALID_PARAMETER);
	}

	checkErr(rsspecan_InitWithOptions(resourceName, IDQuery, resetDevice, "", newInstrSession));

Error:
	return error;
}

/*****************************************************************************
 * Function: rsspecan_InitWithOptions
 * Purpose:  This function creates a new RS session and calls the
 *           RsInit function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_InitWithOptions(
    ViRsrc          resourceName,
    ViBoolean       IDQuery,
    ViBoolean       resetDevice,
    ViString        optionString,
    ViSession      *newInstrSession
)
{
	ViStatus error = VI_SUCCESS;
	ViSession instrSession = 0;
	RsCoreSessionPtr rsSession = NULL;

	if (newInstrSession == NULL)
	{
		(void)RsCore_SetErrorInfo(0, VI_FALSE, RS_ERROR_INVALID_PARAMETER, VI_ERROR_PARAMETER5,
		                          "Null address for Instrument Handle");
		checkErr(RS_ERROR_INVALID_PARAMETER);
	}

	*newInstrSession = 0;

	checkErr(RsCore_NewSpecificDriver(
		resourceName, // VISA Resource Name
		"rsspecan", // driver prefix
		optionString, // Init Options string, is applied to the session settings
		rsspecan_RsCoreAttributeList, // List of all attributes from the rsxxx_AttrPropertiesList
		0, // WriteDelay
		0, // ReadDelay
		1000000, // IO Segment Size
		RS_VAL_OPCWAIT_STBPOLLING, // OPC Wait Mode
		RSSPECAN_OPC_TIMEOUT, // OPC timeout
		5000, // VISA Timeout
		600000, // Self-test timeout
		RS_VAL_BIN_FLOAT_FORMAT_SINGLE_4BYTES, // BinaryFloatNumbersFormat
		RS_VAL_BIN_INTEGER_FORMAT_INT32_4BYTES, // binaryIntegerNumbersFormat
		&instrSession));

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	// No SCPI command has been sent yet.
	// Call viClear before sending any SCPI command
	checkErr(RsCore_ViClear(instrSession));

	/* --- Here perform settings that are default for this driver,
	but can be overwritten by the optionsString settings */

	rsSession->autoSystErrQuery = VI_FALSE;

	// Parse option string and optionally sets the initial state of the following session attributes
	checkErr(RsCore_ApplyOptionString(instrSession, optionString));
	checkErr(RsCore_BuildRepCapTable(instrSession, rsspecan_RsCoreRepCapTable));

	//Parameter idnModelFullName determines RS_ATTR_INSTRUMENT_MODEL value:
	// VI_FALSE: RS_ATTR_INSTRUMENT_MODEL = "RTO"
	// VI_TRUE: RS_ATTR_INSTRUMENT_MODEL = "RTO2044"
	// This is important for CheckInstrumentModel() function used in all attributes and some hi-level functions
	checkErr(RsCore_QueryAndParseIDNstring(instrSession, RSSPECAN_ATTR_ID_QUERY_RESPONSE, RSSPECAN_SIMULATION_ID_QUERY, VI_FALSE, NULL));

	// Query OPT string, parse the options, remove the duplicates,
	// sort them and store the result string to RS_ATTR_OPTIONS_LIST
	checkErr(RsCore_QueryAndParseOPTstring(instrSession, RSSPECAN_SIMULATION_OPT_QUERY, RS_VAL_OPTIONS_PARSING_KEEP_AFTER_DASH));

	// Default Instrument Setup + optional *RST
	if (resetDevice == VI_TRUE)
	{
		checkErr(rsspecan_reset(instrSession));
	}
	else
	{
		checkErr(rsspecan_DefaultInstrSetup(instrSession));
	}

	if (IDQuery == VI_TRUE)
	{
		checkErr(RsCore_FitsIDNpattern(instrSession, RSSPECAN_ATTR_ID_QUERY_RESPONSE, RSSPECAN_VALID_ID_RESPONSE_STRING, NULL));
	}

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (error < VI_SUCCESS)
	{
		if (error == RS_ERROR_INSTRUMENT_STATUS && rsSession->autoSystErrQuery == VI_FALSE)
		{
			rsSession->autoSystErrQuery = VI_TRUE;
			(void)rsspecan_CheckStatus(instrSession);
			rsSession->autoSystErrQuery = VI_FALSE;
		}

		if (instrSession != 0)
			(void)RsCore_ViClose(instrSession);
	}
	else
	{
		*newInstrSession = instrSession;
	}

	return error;
}

/*===========================================================================*
 *= RsSpecAn Base Capabilities Group Functions =============================*
 *===========================================================================*/
/*===========================================================================*/
/* Function: Set Spectrum Analysis Mode
/* Purpose:  This function selects the spectrum analyzer mode.
/*===========================================================================*/

ViStatus _VI_FUNC rsspecan_SetSANMode (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_SAN_MODE, ""));
     
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure Spectrum Display
/* Purpose:  This function creates, selects and deletes new spectrum displays.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrumDisplay (ViSession instrSession,
                                                     ViInt32 operation,
                                                     ViInt32 spectrumDisplay)
{
    ViStatus    error   = VI_SUCCESS;
    ViAttr      attribute = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    switch (operation)
    {
        case  RSSPECAN_VAL_SAN_CREATE:
            attribute = RSSPECAN_ATTR_SPECTRUM_DISPLAY_CREATE;
            break;
            
        case RSSPECAN_VAL_SAN_SELECT:
            attribute = RSSPECAN_ATTR_SPECTRUM_DISPLAY_SELECT;
            break;
            
        case RSSPECAN_VAL_SAN_DELETE:
            attribute = RSSPECAN_ATTR_SPECTRUM_DISPLAY_DELETE;
            break;
        default:
            viCheckParm (VI_ERROR_PARAMETER2, 2, "Operation");

    }
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                               attribute, spectrumDisplay),
                                               3, "Spectrum Display");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Start Stop
 * Purpose:     This function configures the frequency range of the spectrum
 *              analyzer using start frequency and stop frequency.  If start
 *              frequency is equal to the stop frequency, then the spectrum
 *              analyzer is in time-domain mode.  In auto-coupled mode,
 *              resolution bandwidth (RBW), video bandwidth (VBW), and sweep
 *              time may be affected by this function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStartStop (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViReal64 startFrequency,
                                                        ViReal64 stopFrequency)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_START, 
                startFrequency), 3, "Start Frequency");

    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_STOP, 
                stopFrequency), 4, "Stop Frequency");


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Center Span
 * Purpose:     This function configures the center frequency 
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCenter (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViReal64 centerFrequency)
{
    ViStatus    error   = VI_SUCCESS; 
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_CENTER, 
                centerFrequency), 3, "Center Frequency");


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Center Span
 * Purpose:     This function configures the frequency range of the spectrum
 *              analyzer using the center frequency and the frequency Span.
 *              If span corresponds to zero hertz, then the spectrum analyzer
 *              is in time-domain.  
 *
 *              In auto-coupled mode, RBW, VBW and Sweep time may be affected
 *              by this function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCenterSpan (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViReal64 centerFrequency,
                                                         ViReal64 span)
{
    ViStatus    error   = VI_SUCCESS; 
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_CENTER, 
                centerFrequency), 3, "Center Frequency");

    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_SPAN, 
                span), 4, "Span");


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Step Size
 * Purpose:     This function sets the step width of the center frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStepSize (ViSession instrSession,
                                                       ViInt32 window,
                                                       ViReal64 stepSize)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_CENTER_LINK,
                                              RSSPECAN_VAL_CENT_FREQ_LINK_OFF));

    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
            RSSPECAN_ATTR_FREQUENCY_CENTER_STEP, stepSize), 3, "Step Size");

    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Step Size Auto
 * Purpose:     This function couples the step size of the center frequency 
 *              to the span (ON) or sets the value of the center frequency 
 *              entered via SENSe<1|2>:]FREQuency:CENTer:STEP (OFF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStepSizeAuto (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean stepSizeAuto)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
            RSSPECAN_ATTR_FREQUENCY_STEP_AUTO, stepSizeAuto), 3, "Step Size Auto");

    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Coupling Factor
 * Purpose:     This function couples the step width of the center frequency 
 *              to span (span >0) or to the resolution bandwidth (span = 0) 
 *              or cancels the couplings and sets the coupling factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCouplingFactor (ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 coupling,
                                                             ViInt32 couplingFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_CENTER_LINK,
                                              coupling), 3, "Coupling");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_CENTER_LINK_FACTOR, 
                                            couplingFactor), 4, "Coupling Factor");

    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Span Full
 * Purpose:     This function sets the frequency span in the analyzer mode 
 *              to its maximum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencySpanFull (ViSession instrSession,
                                                       ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_SPAN_FULL,
                                               VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Zero Span
 * Purpose:     This function sets the span to 0 Hz. The x-axis becomes the 
 *              time axis with the grid lines corresponding to 1/10 of the 
 *              current sweep time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyZeroSpan (ViSession instrSession,
                                                       ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckErr( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_SPAN,
                                               0.0));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Offset
 * Purpose:     This function configures the frequency offset attribute of 
 *              the spectrum analyzer. This function affects the setting of 
 *              the spectrum analyzer's absolute frequencies, such as start, 
 *              stop, center, and marker. It does not affect values, which 
 *              are the difference of frequencies, such as span and delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyOffset (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViReal64 frequencyOffset)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_FREQUENCY_OFFSET, frequencyOffset),
                                                3, "Frequency Offset");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Mode
 * Purpose:     This function switches between frequency domain (SWEep) and 
 *              time domain (CW | FIXed) in the analyzer mode. For CW and FIXed, 
 *              the frequency setting is via command FREQuency:CENTer. In the 
 *              SWEep mode, the setting is via commands FREQuency:STARt, STOP, 
 *              CENTer and SPAN. In EMI receiver mode, it switches between 
 *              single frequency measurement (CW) and scan (SCAN).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMode (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 frequencyMode)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_FREQUENCY_MODE, frequencyMode),
                                                3, "Frequency Mode");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Time
 * Purpose:     This function configures the sweep time values of the spectrum 
 *              analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepTime (ViSession instrSession,
                                               ViInt32 window,
                                               ViBoolean sweepTimeAuto,
                                               ViReal64 sweepTime)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SWEEP_TIME_AUTO, sweepTimeAuto),
                                                3, "Sweep Time Auto");
    
    if (sweepTimeAuto == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_SWEEP_TIME, sweepTime),
                                                    4, "Sweep Time");
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Coupling Auto
 * Purpose:     This function activates auto coupling of selected coupling
 *              method.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepCouplingAuto (ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 sweepCoupling)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViAttr      attribute = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    switch (sweepCoupling)
    {
        case  RSSPECAN_VAL_COUPLING_RBW:
            attribute = RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_AUTO;
            break;
            
        case RSSPECAN_VAL_COUPLING_VBW:
            attribute = RSSPECAN_ATTR_VIDEO_BANDWIDTH_AUTO;
            break;
            
        case RSSPECAN_VAL_COUPLING_SWEEP_TIME:
            attribute = RSSPECAN_ATTR_SWEEP_TIME_AUTO;
            break;
        default:
            viCheckParm (VI_ERROR_PARAMETER3, 3, "Sweep Coupling");
    }
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                attribute, VI_TRUE),
                                                1, "Dummy");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Coupling
 * Purpose:     This function configures the coupling value
 *              of the spectrum analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepCoupling (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 sweepCoupling,
                                                   ViReal64 couplingValue)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViAttr      attribute = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    switch (sweepCoupling)
    {
        case RSSPECAN_VAL_COUPLING_RBW:
            attribute = RSSPECAN_ATTR_RESOLUTION_BANDWIDTH;
            break;
            
        case RSSPECAN_VAL_COUPLING_VBW:
            attribute = RSSPECAN_ATTR_VIDEO_BANDWIDTH;
            break;
            
        case RSSPECAN_VAL_COUPLING_SWEEP_TIME:
            attribute = RSSPECAN_ATTR_SWEEP_TIME;
            break;
        default:
            viCheckParm (VI_ERROR_PARAMETER3, 3, "Sweep Coupling");
    }
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                            attribute, couplingValue),
                                            4, "Coupling Value");                                  
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Coupling Advanced
 * Purpose:     This function configures advanced parameters of coupling and 
 *              sweeping.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepCouplingAdvanced (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViInt32 ratioSelection,
                                                           ViReal64 value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViAttr      attribute = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    switch (ratioSelection)
    {
        case RSSPECAN_VAL_COUPLING_RATIO_RBW:
            attribute = RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_RATIO;
            break;
        case RSSPECAN_VAL_COUPLING_RATIO_VBW:
            attribute = RSSPECAN_ATTR_VIDEO_BANDWIDTH_RATIO;
            break;
        default:
            viCheckParm (VI_ERROR_PARAMETER3, 3, "Ratio Selection");
    }
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                attribute, value),
                                                4, "Value");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Main PLL Bandwidth
 * Purpose:     This function defines the bandwidth of the main PLL of the 
 *              instrument synthesizer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMainPLLBwid (ViSession instrSession,
                                                 ViInt32 window, 
                                                 ViInt32 PLL)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_PLL_BANDWIDTH, PLL),
                                                3, "PLL");
                                                    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Filter Type
 * Purpose:     This function sets the filter type for the resolution bandwidth.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFilterType (ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 filterType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if (((filterType == RSSPECAN_VAL_FILT_TYPE_P5) || (filterType == RSSPECAN_VAL_FILT_TYPE_P5D)) && 
        ((strstr(model, "FSL") != NULL)))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if ((filterType == RSSPECAN_VAL_FILT_TYPE_PULSE) && 
        !((strstr(model, "FSL") != NULL)))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if ((filterType == RSSPECAN_VAL_FILT_TYPE_CISP) && 
        (strstr(model, "FSV") == NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if ((filterType == RSSPECAN_VAL_FILT_TYPE_MIL) && 
        (strstr(model, "FSV") == NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_FILTER_TYPE,
                                                filterType), 3, "Filter Type"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure VBW Mode
 * Purpose:     This function selects the position of the video filter in the 
 *              signal path, provided that the resolution bandwidth is <=100 kHz.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureVBWMode (ViSession instrSession,
                                             ViInt32 window,
                                             ViInt32 videoFilterType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_VIDEO_BANDWIDTH_TYPE,
                                                videoFilterType), 3, "Video Filter Type");
                                                    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure FFT Filter Mode
 * Purpose:     This function allows to specify FFT filter.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFFTFilterMode (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 FFTFilterMode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_FFT_FILTER_MODE,
                                                FFTFilterMode), 3, "FFT Filter Mode");
                                                    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Averaging
 * Purpose:     This function switches on or off the average calculation for 
 *              the selected trace in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAveraging (ViSession instrSession,
                                               ViInt32 window, 
                                               ViInt32 trace,
                                               ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL") != NULL)
        trace_range=4;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    }
    sprintf (buffer, "Win%ld,TR%ld", window, trace);  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_AVG_STATE,
                                                state), 4, "State");
                                                    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Averaging Type
 * Purpose:     This function selects the type of average function. If Video 
 *              is selected, the logarithmic power is averaged and, if Linear 
 *              is selected, the power values are averaged before they are 
 *              converted to logarithmic values.
 *              The type of average calculation is equally set for all traces in
 *              one measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAveragingType (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 averagingType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_AVG_TYPE,
                                                averagingType), 3, "Averaging Type");
                                                    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Averaging Count
 * Purpose:     This function configures the averaging count.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAveragingCount (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 count)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_AVG_COUNT,
                                                count), 3, "Count");    
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Acquisition
 * Purpose:     This function configures the acquisition attributes of the
 *              spectrum analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAcquisition (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean sweepModeContinuous,
                                                 ViInt32 numberOfSweeps)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS,
                                                sweepModeContinuous), 3, "Sweep Mode Continuous");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_NUMBER_OF_SWEEPS,
                                                numberOfSweeps), 4, "Number Of Sweeps");                                                

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Vertical Scale
 * Purpose:     This function configures the vertical scale of analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureVerticalScale (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 verticalScale)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     scale_range = 1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if ((strstr (model, "FSL") != NULL) || 
        (rsspecan_IsFSV (instrSession)))
        scale_range=3;
    
    if (rsspecan_IsFSV (instrSession)
        && (verticalScale == RSSPECAN_VAL_TRACE_SPACE_PCT))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Vertical Scale");
    
    if (rsspecan_invalidViInt32Range (verticalScale, 0, scale_range) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Vertical Scale");
    }
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_VERTICAL_SCALE,
                                                verticalScale), 3, "Vertical Scale");                                                  

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Horizontal Scale
 * Purpose:     This function toggles between linear and logarithmic display 
 *              of the x-axis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHorizontalScale (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 horizontalScale)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_HORIZONTAL_SCALE,
                                                horizontalScale), 3, "Horizontal Scale"); 
                                                   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Points
 * Purpose:     This function defines the number of measurement points for 
 *              one sweep run.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepPoints (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 sweepPoints)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SWEEP_POINTS,
                                                sweepPoints), 3, "Sweep Points"); 
                                                   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Get Sweep Number
 * Purpose:     This function returns the current number of started sweeps. 
 *              A sweep count value should be set and the device should be 
 *              in single sweep mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSweepNumber (ViSession instrSession,
                                           ViInt32 window,
                                           ViInt32 *numberOfSweeps)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SWEEP_COUNT_CURRENT,
                                                numberOfSweeps), 3, "Number Of Sweeps"); 
                                                   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query Sweep Points
 * Purpose:     This function returns the number of measurement points for 
 *              one sweep run.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySweepPoints (ViSession instrSession,
                                             ViInt32 window,
                                             ViInt32 *sweepPoints)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                   RS_ATTR_INSTRUMENT_MODEL, 10, model));
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, 
                                            RSSPECAN_ATTR_SWEEP_POINTS,
                                            sweepPoints), 3, "Sweep Points");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
   return error;
}

/// HIFN This function selects the sweep type optimization mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR sweepOptimization/Selects the sweep type optimization mode.
ViStatus _VI_FUNC rsspecan_ConfigureSweepOptimization (ViSession instrSession,
                                                       ViInt32 sweepOptimization)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_SWEEP_OPTIMIZATION,
                                                sweepOptimization), 2, "Sweep Optimization"); 
                                                   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Type
 * Purpose:     This function configures the sweep type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepType (ViSession instrSession,
                                               ViInt32 sweepType)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_SWEEP_TYPE,
                                                sweepType), 2, "sweep Type"); 
                                                   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Attenuation
 * Purpose:     This function configures the input attenuation
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAttenuation (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean attenuationAuto,
                                                 ViReal64 attenuation)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_ATTENUATION_AUTO,
                                                attenuationAuto), 2, "Attenuation Auto");                                                 
    if (attenuationAuto == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_ATTENUATION,
                                                attenuation), 3, "Attenuation");                                                
    }
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Level
 * Purpose:     This function configures the reference level
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevel (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 referenceLevel)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_LEVEL,
                                                referenceLevel), 2, "Reference Level"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Level Offset
 * Purpose:     This function configures the offset value to the reference level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevelOffset (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViReal64 referenceLevelOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET,
                                                referenceLevelOffset), 3, "Reference Level Offset"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Level
 * Purpose:     This function configures the vertical attributes of the
 *              spectrum analyzer.  This corresponds to attributes like
 *              amplitude units, input attenuation, input impedance, reference
 *              level, and reference level offset.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureLevel (ViSession instrSession,
                                           ViInt32 window, 
                                           ViInt32 amplitudeUnits,
                                           ViReal64 inputImpedance,
                                           ViReal64 referenceLevel,
                                           ViReal64 referenceLevelOffset,
                                           ViBoolean attenuationAuto,
                                           ViReal64 attenuation)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    
    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_AMPLITUDE_UNITS,
                                                amplitudeUnits), 3, "Amplitude Units"); 
    if (strstr(model, "FMU") == NULL)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_INPUT_IMPEDANCE,
                                                (ViInt32) inputImpedance), 4, "Input Impedance"); 
    }
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_LEVEL,
                                                referenceLevel), 5, "Reference Level"); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET,
                                                referenceLevelOffset), 6, "Reference Level Offset"); 
    if (strstr(model, "FMU") == NULL)
    {
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                    RSSPECAN_ATTR_ATTENUATION_AUTO,
                                                    attenuationAuto), 7, "Attenuation Auto");                                                 
        if (attenuationAuto == VI_FALSE)
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_ATTENUATION,
                                                    attenuation), 8, "Attenuation");                                                
        }
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Attenuation Mode
 * Purpose:     This function swicthes the behaviour for the attenuator.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAttenuationMode (ViSession instrSession,
                                                     ViInt32 window, ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    sprintf (buffer, "Win%ld", window);   
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                   RS_ATTR_INSTRUMENT_MODEL, 10, model));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                   RSSPECAN_ATTR_ATTENUATION_MODE,
                                   mode), 2, "Mode"); 
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
   return error;
}

/*****************************************************************************
 * Function:    Configure Parameter Coupling
 * Purpose:     In operating mode SPECTRUM this function selects the parameter
 *              coupling between the two measurement windows screen A and B.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureParameterCoupling (ViSession instrSession,
                                                       ViInt32 parameterCoupling)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_PARAMETER_COUPLING,
                                                parameterCoupling), 2, "Parameter Coupling"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Level Coupling
 * Purpose:     This function defines wether the reference value for the Y axis 
 *              is coupled to the reference level (default) or not.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevelCoupling (ViSession instrSession,
                                                            ViInt32 window, 
                                                            ViBoolean coupledReferenceLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    sprintf (buffer, "Win%ld", window);   
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_DISP_REF_VALUE_AUTO,
                                                coupledReferenceLevel), 2, "Coupled Reference Level"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure IF Output Source
 * Purpose:     This function switches the source of the IF output between 
 *              the demodulated signal (2 possible values) and the IF signal.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIFOutputSource (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 outputSource)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    sprintf (buffer, "Win%ld", window);   
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_IF_OUTPUT_SOURCE,
                                                outputSource), 2, "Output Source"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function defines the frequency for the IF output.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR IFOutFrequency/Selects the point at which the frequency axis is truncated.
ViStatus _VI_FUNC rsspecan_ConfigureIFOutFrequency (ViSession instrSession,
                                                    ViReal64 IFOutFrequency)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));  
 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_IF_OUTPUT_FREQUENCY,
                                                IFOutFrequency), 2, "IF Out Frequency"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function turns the 28 V supply of the noise source on the front panel on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR window/Selects the measurement window.
/// HIPAR noiseSource/Turns the 28 V supply of the noise source on the front panel on and off.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseSource (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean noiseSource)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));  
    
    sprintf (buffer, "Win%ld", window);   
 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_NOISE_SOURCE_STATE,
                                                noiseSource), 2, "Noise Source"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Unit Power
 * Purpose:     This function selects the unit for power in the selected 
 *              measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureUnitPower (ViSession instrSession,
                                               ViInt32 window, ViInt32 unitPower)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model[10] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                   RS_ATTR_INSTRUMENT_MODEL, 10, model));
    
    if (rsspecan_IsFSV (instrSession))
        if (rsspecan_invalidViInt32Range (unitPower, 0, 7) == VI_TRUE)
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Unit Power");

    sprintf (buffer, "C%ld", window); 
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_UNIT_POWER, 
                 unitPower), 3, "Unit Power");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function selects an existing measurement channel with the specified name.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR channelName/String containing the name of the channel.
ViStatus _VI_FUNC rsspecan_SelectMeasurementChannel (ViSession instrSession,
                                            ViString channelName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm (rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SELECT_MEASUREMENT_CHANNEL, 
                 channelName), 3, "Channel Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function adds an additional measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR channelType/This control selects the channel type of the new channel.
/// HIPAR channelName/String containing the name of the channel.
ViStatus _VI_FUNC rsspecan_AddMeasurementChannel (ViSession instrSession,
                                         ViInt32 channelType,
                                         ViString channelName)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
    ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "FSW")))
        viCheckErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (rsspecan_invalidViInt32Range (channelType, RSSPECAN_VAL_CHANNEL_TYPE_SPECTRUM, 
                                                RSSPECAN_VAL_CHANNEL_TYPE_LTE) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Channel Type");  
    } 
    
    sprintf (cmd, "INST:CRE %s,'%s'", ChannelTypeArr[channelType], channelName);
    
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN This function duplicates the currently selected measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_DuplicateMeasurementChannel (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                            RSSPECAN_ATTR_DUPLICATE_MEASUREMENT_CHANNEL, VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function replaces a measurement channel with another one.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR oldChannelName/String containing the name of the channel you want to replace.
/// HIPAR newChannelType/This control selects the channel type of the new channel.
/// HIPAR newChannelName/String containing the name of the new channel.
ViStatus _VI_FUNC rsspecan_ReplaceMeasurementChannel (ViSession instrSession,
                                             ViString oldChannelName,
                                             ViInt32 newChannelType,
                                             ViString newChannelName)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
    ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "FSW")))
        viCheckErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (rsspecan_invalidViInt32Range (newChannelType, RSSPECAN_VAL_CHANNEL_TYPE_SPECTRUM, 
                                                RSSPECAN_VAL_CHANNEL_TYPE_LTE) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "New Channel Type");  
    } 
    
    sprintf (cmd, "INST:CRE:REPL '%s',%s,'%s'", oldChannelName, ChannelTypeArr[newChannelType], 
                                                                            newChannelName);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function renames a measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR oldChannelName/String containing the name of the channel you want to rename.
/// HIPAR newChannelName/String containing the new channel name.
ViStatus _VI_FUNC rsspecan_RenameMeasurementChannel (ViSession instrSession,
                                            ViString oldChannelName,
                                            ViString newChannelName)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer [1024] = "";
    ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "FSW")))
        viCheckErr (RS_ERROR_INSTRUMENT_MODEL);
    
    sprintf (cmd, "INST:REN '%s','%s'", oldChannelName, newChannelName);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function deletes a measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR channelName/String containing the name of the channel you want to delete.
ViStatus _VI_FUNC rsspecan_DeleteMeasurementChannel (ViSession instrSession,
                                            ViString channelName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_DELETE_MEASUREMENT_CHANNEL, 
                 channelName), 3, "Channel Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function restores the default instrument settings in the current channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_PresetMeasurementChannel (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_PRESET_MEASUREMENT_CHANNEL, 
                 VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries all active measurement channels.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/This parameter determines the size of array Channel Type and Channel Name
/// HIPAR channelType/This control returns the channel types of all channels.
/// HIPAR channelName/This control returns strings containing names of channels.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_QueryAllMeasurementChannels (ViSession instrSession,
                                               ViInt32 arraySize,
                                               ViInt32 channelType[],
                                               ViChar channelName[],
                                               ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;
    ViChar*     p2buf;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "FSW")))
        viCheckErr (RS_ERROR_INSTRUMENT_MODEL);
    
    viCheckErr (viPrintf (instrSession, "INST:LIST?\n"));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    p2buf = channelName;
    pstring_value = strtok (pbuffer, ",");  
    while (pstring_value && cnt<arraySize)
        {
        sscanf (pstring_value, "'%[^']'", pstring_value);
            channelType[cnt] = rsspecan_StringIndex (ChannelTypeArr, pstring_value);
            pstring_value = strtok (NULL, ",");
        
        sscanf (pstring_value, "'%[^']'", pstring_value);
        p2buf += sprintf (p2buf, "%s,", pstring_value);
        
            pstring_value = strtok (NULL, ","); 

        cnt++;
    }
    *--p2buf='\0';
    
    if (returnedValues != NULL)
    *returnedValues = cnt;
    
    checkErr (rsspecan_CheckStatus (instrSession));
    
Error:
    if (pbuffer) free (pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Trace
 * Purpose:     This function configures the trace to acquire.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTrace (ViSession instrSession,
                                           ViInt32 window, 
                                           ViInt32 trace,
                                           ViInt32 traceType)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 6) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    }
    sprintf (buffer, "Win%ld,TR%ld", window, trace); 
    switch (traceType){
        case RSSPECAN_TRAC_MOD_BLANK:
            viCheckErr ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                        RSSPECAN_ATTR_TRACE_STATE,
                                                        VI_FALSE));
        break;
        default:
            viCheckErr ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                        RSSPECAN_ATTR_TRACE_STATE,
                                                        VI_TRUE));
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRACE_TYPE,
                                                traceType), 4, "Trace Type"); 
        break;
    }
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Trace Detector
 * Purpose:     This function configures the trace detector.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceDetector (ViSession instrSession,
                                                   ViInt32 window, 
                                                   ViInt32 trace,
                                                   ViBoolean detectorTypeAuto,
                                                   ViInt32 detectorType)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL") != NULL)
        trace_range=4;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    }
    sprintf (buffer, "Win%ld,TR%ld", window, trace); 
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_DETECTOR_TYPE_AUTO,
                                                detectorTypeAuto), 4, "Detector Type Auto");
    if (detectorTypeAuto == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_DETECTOR_TYPE,
                                                detectorType), 5, "Detector Type"); 
    }
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Trace Reset Behavior
 * Purpose:     This function specifies whether or not the traces with peak 
 *              or minimum value detection are reset after specific parameter 
 *              changes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceResetBehavior (ViSession instrSession,
                                                ViInt32 window, 
                                                ViInt32 trace,
                                                ViBoolean reset_atChange)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL") != NULL)
        trace_range=6;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    }
    sprintf (buffer, "Win%ld,TR%ld", window, trace); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRACE_RESET_BEHAVIOR,
                                                reset_atChange), 4, "Reset At Change"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Store Trace To File
 * Purpose:     This function stores the selected trace (1 to 3) in the 
 *              measurement window indicated by window in a file with ASCII 
 *              format.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreTraceToFile (ViSession instrSession,
                                             ViInt32 window, 
                                             ViInt32 trace,
                                             ViString fileName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL") != NULL) || (strstr (model, "FSW") != NULL))
        trace_range=6;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    if (fileName == VI_NULL) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "File Name");

    viCheckErr (viPrintf (instrSession,"MMEM:STOR%ld:TRAC %ld,'%s'\n", window, trace, fileName));
        
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function selects the data to be included in a data export file.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR exportMode/Selects the data to be included in a data export file.
/// HIPAR exportHeader/If enabled, additional instrument and measurement settings are 
/// HIPAR exportHeader/included in the header of the export file for result data. If disabled, 
/// HIPAR exportHeader/only the pure result data from the selected traces and tables is 
/// HIPAR exportHeader/exported.
ViStatus _VI_FUNC rsspecan_ConfigureTraceExport (ViSession instrSession,
                                                    ViInt32 exportMode,
                                                    ViBoolean exportHeader)
{
    ViStatus    error   = VI_SUCCESS; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_TRACE_EXPORT_ALL, 
                exportMode), 2, "Export Mode");
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_VSA_FILE_EXP_HEADER, 
                exportHeader), 3, "Export Header");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Trace Clear
 * Purpose:  This function clears the traces.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TraceClear (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_DISPLAY_TRACE_CLEAR, ""));
     
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Trace IQ Set
 * Purpose:     This function queries the read-only trace size attribute.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TraceIQSet (ViSession instrSession,
                                       ViInt32 window, 
                                       ViReal64 samplingRate,
                                       ViInt32 triggerMode, 
                                       ViInt32 triggerSlope,
                                       ViReal64 bandwidth,
                                       ViInt32 pretriggerSamples,
                                       ViInt32 no_ofSamples)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      cmd[RSSPECAN_IO_BUFFER_SIZE] = "";
     
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    //if (rsspecan_invalidViReal64Range (samplingRate, 10.0e3, 326.4e6) == VI_TRUE)
    //    viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Sampling Rate");
    if ((rsspecan_invalidViInt32Range (triggerMode, 0, 4) == VI_TRUE)||(triggerMode==2))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Trigger Mode");
    if (rsspecan_invalidViInt32Range (triggerSlope, 0, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Trigger Slope");  
    
    /*if (rsspecan_invalidViReal64Range (bandwidth, 10.0, 50.0e6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Bandwidth");  
    
    if (rsspecan_invalidViInt32Range (pretriggerSamples, -16744447, 65023) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Pretrigger Samples");
    if (strstr(buffer,"B102")!=NULL)
    {
        if (rsspecan_invalidViInt32Range (no_ofSamples, 1, 1409285632) == VI_TRUE)
           viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "No Of Samples");
    }   
    else
    {
        if (strstr(buffer,"B100")!=NULL)
        {
            if (rsspecan_invalidViInt32Range (no_ofSamples, 1, 469761536) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "No Of Samples");
        }
        else
        {
            if (rsspecan_invalidViInt32Range (no_ofSamples, 1, 16776704) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "No Of Samples");   
        }
    }   
    */
    sprintf (cmd, "TRAC%ld:IQ:SET NORM,%.12lG,%.12lG,%s,%s,%ld,%ld\n",
            window, bandwidth, samplingRate, rsspecan_rngTriggerSource.rangeValues[triggerMode].cmdString,
            rsspecan_rngPolarity.rangeValues[triggerSlope].cmdString, pretriggerSamples, no_ofSamples);
    
    viCheckErr (viWrite(instrSession, (ViBuf)cmd, (ViUInt32)strlen(cmd), NULL));
    
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 * Function: Trace IQ Sampling Rate
 * Purpose:  This function sets the sampling rate for the I/Q data acquisition. 
 *           Thus the sampling rate can be modified without affecting the other
 *           settings. 
 *===========================================================================*/
ViStatus _VI_FUNC rsspecan_TraceIQSamplingRate (ViSession instrSession,
                                                ViInt32 window,
                                                ViReal64 samplingRate)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_IQ_SAMPLE_RATE, 
                                                samplingRate), 2, "Sampling Rate");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function activates the bandwidth extension option R&S FSW-B160 / U160, if installed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR state/Activates the bandwidth extension option R&S FSW-B160 / U160, if installed.
ViStatus _VI_FUNC rsspecan_TraceIQMaximumBandwidthExtension (ViSession instrSession,
                                                    ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErrElab( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_IQ_MAX_BANDWIDTH_EXTENSION, 
                                                state), "State"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 * Function: Trace IQ Record Length
 * Purpose:  This function sets the record length for the acquired I/Q data.
 *           Increasing the record length automatically also increases the
 *           measurement time.
 *===========================================================================*/
ViStatus _VI_FUNC rsspecan_TraceIQRecordLength (ViSession instrSession,
                                                ViInt32 recordLength)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Win1",
        RSSPECAN_ATTR_IQ_RECORD_LENGTH, recordLength), 2, "Record Length");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Trace IQ Averaging
 * Purpose:     This function configures the IQ data measurement averaging.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TraceIQAveraging (ViSession instrSession,
                                             ViInt32 window,
                                             ViBoolean averageState,
                                             ViInt32 averageCount)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_IQ_DATA_AVER_STATE, averageState),
                                                2, "Average State");
    if (averageState == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_IQ_DATA_AVER_COUNT, averageCount),
                                                3, "Average Count");    
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Enables or disables the IQ data evaluation mode and is a prerequisite for the IQ Analyzer functions.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR IQDataAnalyzerEnabled/Enables or disables the IQ data evaluation mode and is a prerequisite for the IQ Analyzer functions.

ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataAnalyzerEnabled (ViSession instrSession,
                                                                ViInt32 window,
                                                                ViBoolean IQDataAnalyzerEnabled)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "buffer",
        RSSPECAN_ATTR_IQ_DATA_ANALYZER_STATE, IQDataAnalyzerEnabled), 2, "IQ Data Analyzer Enabled"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Trace IQ Extended Bandwidth
/* Purpose:  This function allows measurement with extended usable signal 
/*           bandwidth for sample rates in the range of 20.4 MHz to 40.8 MHz. 
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TraceIQExtendedBandwidth(
    ViSession   instrSession,
    ViInt32 window,
    ViInt32 filter
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckErr ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_IQ_EXT_BAND, filter));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/*****************************************************************************
 * Function:    Configure Trace IQ Data Acquisition
 * Purpose:     This function configures the trace IQ data acquisition.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataAcquisition (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViBoolean traceState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_IQ_DATA_STATE, traceState),
                                                3, "Trace State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Configure Trace IQ Start Synchronization
 * Purpose:     This function switches on or off the synchronization of the
 *              start of I/Q data acquisition to the trigger event. Prior to
 *              this command, I/Q data acquisition has to be switched. The
 *              sampling rate must be 32 MHz. The synchronization ensures the
 *              correct calculation of I/Q average values as the data aquisition
 *              is always started with the same phase reference in relation to
 *              the trigger event
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQStartSynchronization (ViSession instrSession,
                                                                 ViInt32 window,
                                                                 ViBoolean startSynchronization)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_IQ_START_SYNCHRONIZATION,
                                                startSynchronization),
                                                3, "Start Synchronization");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Trace IQ BW Extension
 * Purpose:     This function switches the Bandwidth Extension FSQ-B72 on or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQBWExtension (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViBoolean BWExtensionState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_IQ_DATA_WBAND_STATE, BWExtensionState),
                                                3, "BW Extension State");
                                                 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Trace IQ Gate
 * Purpose:     This function configures the gate function for IQ data
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQGate (ViSession instrSession,
                                                 ViBoolean gate, ViInt32 type,
                                                 ViInt32 gateLength,
                                                 ViInt32 gateGap,
                                                 ViInt32 numberOfGatePeriods)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                 RSSPECAN_ATTR_IQ_DATA_GATE_STATE, 
                                                 gate), 2, "Gate");
                                                 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                               RSSPECAN_ATTR_IQ_DATA_GATE_TYPE, 
                                               type), 3, "Type");
                                                 
    if (type == RSSPECAN_VAL_GATE_TYPE_EDGE)
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                   RSSPECAN_ATTR_IQ_DATA_GATE_LENGTH, 
                                                   gateLength), 4, "Gate Length");
    }
                                                 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                               RSSPECAN_ATTR_IQ_DATA_GATE_GAP, 
                                               gateGap), 5, "Gate Gap");
                                                 
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                               RSSPECAN_ATTR_IQ_DATA_GATE_NUMBER_OF_PERIODS, 
                                               numberOfGatePeriods), 6, "Number Of Gate Periods");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Trace IQ Data Display Type
 * Purpose:     This function configures the display type of the IQ data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataDisplayType (ViSession instrSession,
                                                            ViInt32 displayType)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                               RSSPECAN_ATTR_IQ_DATA_DISPLAY_TYPE, 
                                               displayType), 2, "Display Type");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Configure Trace IQ Data Display Type
 * Purpose:     This function queries the filter bandwidth of the resampling
 *              filter, which is dependent on the sampling rate.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTraceIQFilterBandwidth (ViSession instrSession,
                                                        ViReal64 *filterBandwidth)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "", 
                                               RSSPECAN_ATTR_IQ_DATA_FILTER_BANDWIDTH, 
                                               filterBandwidth), 2, "Filter Bandwidth");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/* 
 * List Power Class
 */

/*****************************************************************************
 * Function:    Load IQ Data
 * Purpose:     This function loads the IQ data from the specified file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_LoadIQData (ViSession instrSession,
                                       ViString fileName)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_LOAD_IQ_DATA, fileName), 
                                                2, "File Name");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Store IQ Data
 * Purpose:     This function stores the IQ data from/to the specified file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreIQData (ViSession instrSession,
                                        ViString fileName)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_SAVE_IQ_DATA, fileName), 
                                                2, "File Name");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure IQ Data Export File Description
 * Purpose:     This function defines a description of the export file which is
 *              stored with the data and also displayed in the file selection
 *              dialog box for I/Q data import and export
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIQDataExportFileDescription (ViSession instrSession,
                                                                 ViString description)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                    RSSPECAN_ATTR_IQ_DATA_EXPORT_FILE_DESCRIPTION, description), 
                                                2, "Description");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Query TPIS Time Offset
 * Purpose:     This function determines the time offset of the trigger in the
 *              sample (trigger position in sample = TPIS). This value can only
 *              be determined in triggered I/Q measurements using external or
 *              IFPower triggers, otherwise the value is 0. The value is not
 *              user-definable.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTPISTimeOffset (ViSession instrSession,
                                                    ViInt32 trace,
                                                    ViInt32 *timeOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (trace, 1, 2) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Trace")
    }
    sprintf (buffer, "TR%ld", trace);  
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_QUERY_TPIS_TIME_OFFSET, timeOffset),
                                                3, "Time Offset");
                                                 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    List Power Measurement Off
 * Purpose:     This function deactivates the list measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ListPowerMeasurementOff (ViSession instrSession,
                                                    ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_LIST_POW_STATE_OFF, ""));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure List Power Average Type
 * Purpose:     This function sets the average type of the sense list measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureListPowerAverageType (ViSession instrSession,
                                                 ViInt32 window, 
                                                 ViInt32 type)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckErr ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_LIST_POW_TYPE, type));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure List Power Sequence
 * Purpose:     This function configures the list of settings (max. 100 entries
 *              for an individual frequency point) for the multiple power
 *              measurement and starts a measurement sequence.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureListPowerSequence (ViSession instrSession,
                                              ViInt32 window,
                                              ViInt32 noofListItems,
                                              ViReal64 analyzerFrequency[],
                                              ViReal64 referenceLevel[],
                                              ViReal64 RFInputAttenuation[],
                                              ViReal64 RFInputElectronicAttn[],
                                              ViInt32 filterType[],
                                              ViReal64 resolutionBandwidth[],
                                              ViReal64 videoBandwidth[],
                                              ViReal64 measTime[],
                                              ViReal64 triggerLevel[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pwrite_buffer = NULL;
    ViChar*     pbuffer;
    ViInt32     i = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (noofListItems, 1, 100) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "No Of List Items");   
    if (analyzerFrequency == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Analyzer Frequency");  
    if (referenceLevel == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Reference Level");  
    if (RFInputAttenuation == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "RF Input Attenuation");  
    if (RFInputElectronicAttn == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "RF Input Electronic Attn");  
    if (filterType == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Filter Type");  
    if (resolutionBandwidth == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Resolutin Bandwidth");  
    if (videoBandwidth == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "Video Bandwidth");  
    if (measTime == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Meas Time");  
    if (triggerLevel == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 12, "Trigger Level");  

    /*
     * Alloc buffer. Size is calclulated as:
     * size of header + size required for one entry * number of entries
     */
    pwrite_buffer = (ViChar*) malloc((size_t)(20 + 240 * noofListItems));
    if (pwrite_buffer == NULL)
        return VI_ERROR_ALLOC;

    pbuffer = pwrite_buffer;

    pbuffer += sprintf (pbuffer, ":SENS%ld:LIST:POW:SEQ ", window);

    for (i = 0; i < noofListItems; i++)
    {
        /* Electronic attenuator is not in the signal path. */
        if (RFInputElectronicAttn[i] < 0)
        {
            pbuffer += sprintf (pbuffer, "%.12f,%.12f,%.12f,OFF,%s,%.12f,%.12f,%.12f,%.12f,",
                                analyzerFrequency[i],
                                referenceLevel[i],
                                RFInputAttenuation[i],
                                listFilterTypeArr[filterType[i]],/* RFInputEAttn[i], */
                                resolutionBandwidth[i],
                                videoBandwidth[i],
                                measTime[i],
                                triggerLevel[i]);
        }
        else
        {
            pbuffer += sprintf (pbuffer, "%.12f,%.12f,%.12f,%.12f,%s,%.12f,%.12f,%.12f,%.12f,",
                                analyzerFrequency[i],
                                referenceLevel[i],
                                RFInputAttenuation[i],
                                RFInputElectronicAttn[i],
                                listFilterTypeArr[filterType[i]],
                                resolutionBandwidth[i],
                                videoBandwidth[i],
                                measTime[i],
                                triggerLevel[i]);
        }
    }

    *--pbuffer = '\0'; /* Remove remaining comma */

    pbuffer += sprintf (pbuffer, "\n");

    viCheckErr(viWrite (instrSession, (ViBuf) pwrite_buffer, (ViUInt32)strlen (pwrite_buffer), NULL));
    
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    if (pwrite_buffer) free (pwrite_buffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query List Power Sequence
 * Purpose:     This function returns the list of settings 
 *              (max. 100 entries for an individual frequency point) for the 
 *              multiple power measurement, starts a measurement sequence and 
 *              returns the measured data. To reduce the setting time, all 
 *              indicated parameters are set up simultaneously at each test 
 *              point.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryListPowerSequence (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 noofListItems,
                                                 ViReal64 analyzerFrequency[],
                                                 ViReal64 referenceLevel[],
                                                 ViReal64 RFInputAttenuation[],
                                                 ViReal64 RFInputElectronicAttn[],
                                                 ViInt32 filterType[],
                                                 ViReal64 resolutionBandwidth[],
                                                 ViReal64 videoBandwidth[],
                                                 ViReal64 measTime[],
                                                 ViReal64 triggerLevel[],
                                                 ViReal64 listPowerResults[])

{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pwrite_buffer = NULL;
    ViChar*     pbuffer;
    ViInt32     i = 0,
                results;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (noofListItems, 1, 100) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "No Of List Items");   
    if (analyzerFrequency == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Analyzer Frequency");  
    if (referenceLevel == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Reference Level");  
    if (RFInputAttenuation == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "RF Input Attenuation");  
    if (RFInputElectronicAttn == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "RF Input Electronic Attn");  
    if (filterType == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Filter Type");  
    if (resolutionBandwidth == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Resolutin Bandwidth");  
    if (videoBandwidth == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "Video Bandwidth");  
    if (measTime == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Meas Time");  
    if (triggerLevel == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 12, "Trigger Level"); 
    if (listPowerResults == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 13, "List Power Results");         
    
 
    pwrite_buffer = (ViChar*) malloc((size_t)(20 + 240 * noofListItems));
    if (pwrite_buffer == NULL)
        return VI_ERROR_ALLOC;

    pbuffer = pwrite_buffer;

    pbuffer += sprintf (pbuffer, "*CLS;:SENS%ld:LIST:POW:SEQ? ", window);

    for (i=0; i<noofListItems; i++)
        {
        /* Electronic attenuator is not in the signal path. */
        if (RFInputElectronicAttn[i] < 0)
            pbuffer += sprintf (pbuffer, "%.12f,%.12f,%.12f,OFF,%s,%.12f,%.12f,%.12f,%.12f,",
                                analyzerFrequency[i], referenceLevel[i], RFInputAttenuation[i],
                                /* RFInputEAttn[i], */ listFilterTypeArr[filterType[i]], resolutionBandwidth[i],
                                videoBandwidth[i], measTime[i], triggerLevel[i]);
        else
            pbuffer += sprintf (pbuffer, "%.12f,%.12f,%.12f,%.12f,%s,%.12f,%.12f,%.12f,%.12f,",
                                analyzerFrequency[i], referenceLevel[i], RFInputAttenuation[i],
                                RFInputElectronicAttn[i], listFilterTypeArr[filterType[i]], resolutionBandwidth[i],
                                videoBandwidth[i], measTime[i], triggerLevel[i]);
        }

    *--pbuffer = '\0'; /* Remove remaining comma */

    pbuffer += sprintf (pbuffer, "\n");

    viCheckErr (viWrite (instrSession, (ViBuf) pwrite_buffer, (ViUInt32) strlen (pwrite_buffer), NULL));

    //viCheckErr (rsspecan_WaitForOPC (instrSession, timeout)); 
    /* Read float numbers separated by commas x 3 possibly activated detectors */
    
    
    results = (noofListItems);
    viCheckErr (viScanf (instrSession, "%,#lf", &results, listPowerResults));
    
    checkErr( rsspecan_CheckStatus (instrSession)); 
    
Error:
    if (pwrite_buffer) free (pwrite_buffer); 
    
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query List Power Result
 * Purpose:     This function queries the result of a previous list measurement 
 *              as configured and initiated with List Power Sequence.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_QueryListPowerResult (ViSession instrSession,
                                               ViInt32 window,
                                               ViInt32 noofResults,
                                               ViReal64 listPowerResults[],
                                               ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     ptag;
    ViInt32     index;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (noofResults<=0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "No Of Results");
        
    viCheckErr (viQueryf(instrSession, "SENS%ld:LIST:POW:RES?\n","%s",window, buffer));
    ptag = strtok(buffer, ",");
    for (index = 0; ptag; index++)
    {
        if (index < noofResults)
            listPowerResults[index] = atof(ptag);
        ptag = strtok(NULL, ",");
    }
    if (returnedValues)
        *returnedValues=index;
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure List Power Set
 * Purpose:     This function defines the constant settings for the list during
 *              multiple power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureListPowerSet (ViSession instrSession,
                                         ViInt32 window, 
                                         ViBoolean peakMeas,
                                         ViBoolean RMSMeas, 
                                         ViBoolean AVGMeas,
                                         ViInt32 triggerMode,
                                         ViInt32 triggerSlope,
                                         ViReal64 triggerOffset,
                                         ViReal64 gateLength)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViBooleanRange (peakMeas) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Peak Meas"); 
    if (rsspecan_invalidViBooleanRange (RMSMeas) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "RMS Meas");
    if (rsspecan_invalidViBooleanRange (AVGMeas) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "AVG Meas");
    if (rsspecan_invalidViInt32Range (triggerMode, 0, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Trigger Mode");   
    if (rsspecan_invalidViInt32Range (triggerSlope, 0, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Trigger Slope");           
    if (rsspecan_invalidViReal64Range (triggerOffset, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Trigger Offset");
    if (rsspecan_invalidViReal64Range (gateLength, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Gate Length");
        
    viCheckErr (viPrintf (instrSession, "SENS%ld:LIST:POW:SET %s,%s,%s,%s,%s,%.12f,%.12f\n",
            window, switchArr[peakMeas], switchArr[RMSMeas], switchArr[AVGMeas],
            rsspecan_rngTriggerSource.rangeValues[triggerMode].cmdString, rsspecan_rngPolarity.rangeValues[triggerSlope].cmdString,
            triggerOffset, gateLength));
            
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Copy Trace
 * Purpose:     This function copies one trace array to another trace array.  
 *              Any data in the destination trace is over written.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CopyTrace (ViSession instrSession, 
                                      ViInt32 window,
                                      ViInt32 destinationTrace,
                                      ViInt32 sourceTrace)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL") != NULL))
        trace_range=6;
    
    if (rsspecan_invalidViInt32Range (destinationTrace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Destination Trace");
    if (rsspecan_invalidViInt32Range (sourceTrace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Source Trace");        
    
    viCheckErr(viPrintf(instrSession, "TRAC%ld:COPY TRACE%ld,TRACE%ld\n", window, 
    destinationTrace, sourceTrace));
    
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Subtract Traces
 * Purpose:     This function configures subtraction of traces.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSubtractTraces (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 position,
                                                    ViInt32 mode,
                                                    ViBoolean traceMathState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     math_mode = RSSPECAN_VAL_LOG;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if ((rsspecan_IsFSV (instrSession)) ||
        (strstr (model, "FSL") != NULL))
        math_mode = RSSPECAN_VAL_MPOW;
        
    if (rsspecan_invalidViInt32Range (mode, RSSPECAN_VAL_LIN, math_mode) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Mode");
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_TRACE_MATH_POSITION, 
                    position), 3, "Position");
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_TRACE_MATH_MODE, 
                mode), 4, "Mode");
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_TRACE_MATH_STATE, 
                traceMathState), 5, "Trace Math State");
                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Subtract Traces
 * Purpose:     This function subtracts the array elements of trace defined 
 *              in argument Trace 2 from TRACE1 of the instrument and stores 
 *              the result in the TRACE1.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SubtractTraces (ViSession instrSession,
                                           ViInt32 window, 
                                           ViInt32 trace2)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL") != NULL))
        trace_range=6;
    
    if (rsspecan_invalidViInt32Range (trace2, 2, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace 2");
    
    viCheckErr(viPrintf(instrSession, "CALC%ld:MATH (TRACE1-TRACE%ld)\n", window, trace2));
                
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn Marker Extension Group Functions ==============================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Marker
 * Purpose:     This function enables the active marker on the specified trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarker (ViSession instrSession,
                                                   ViInt32 window, 
                                                   ViInt32 marker,
                                                   ViBoolean markerEnabled,
                                                   ViInt32 trace)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;
    
    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_ENABLED, 
                markerEnabled), 4, "Marker Enabled");
    
    if (markerEnabled == VI_TRUE)
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_MARKER_TRACE, 
                trace), 5, "Trace");
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Marker
 * Purpose:     This function sets the marker frequency counter resolution and
 *              turns the marker frequency counter on/off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceMarker (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 marker,
                                                     ViInt32 trace)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,DM%ld", window, marker); 

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
        RSSPECAN_ATTR_DELTA_MARKER_REFERENCE_MARKER, trace), 4, "Trace");
Error:
 (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Frequency Counter
 * Purpose:     This function sets the marker frequency counter resolution and
 *              turns the marker frequency counter on/off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerFrequencyCounter (ViSession instrSession,
                                                            ViInt32 window, ViInt32 marker,
                                                            ViBoolean markerFrequencyCounter,
                                                            ViReal64 frequencyCounterResolution)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_FREQUENCY_COUNTER_ENABLED, 
                markerFrequencyCounter), 4, "Marker Frequency Counter");
    
    if (strstr (model, "ZVH") == NULL)
    {
        sprintf (buffer, "Win%ld", window);                
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_FREQUENCY_COUNTER_RESOLUTION, 
                    frequencyCounterResolution), 5, "Freqeuncy Counter Resolution");
    }
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query Marker Frequency Counter Result
 * Purpose:     This function queries the result of the frequency counter 
 *              for the indicated marker in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerFrequencyCounterResult (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViInt32 marker,
                                                              ViReal64 *counterFrequency)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window,marker); 
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_COUNT, 
                counterFrequency), 4, "Counter Freqeuncy");
              
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Probability Marker Position
 * Purpose:     This function positions the selected marker to the given 
 *              probability.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ProbabilityMarkerPosition (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViInt32 marker,
                                                               ViReal64 probability)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_PROBABILITY, 
                probability), 4, "Probability");
   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN Configure a band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR displayMode/Activates the band power marker in the specified window.
/// HIPAR bandPowerEnabled/Defines whether the result is displayed as a power or density.
/// HIPAR span/Defines the span (band) around the marker for which the power is measured.

ViStatus _VI_FUNC rsspecan_ConfigureMarkerBandPower (ViSession instrSession,
                                                     ViBoolean bandPowerEnabled,
                                                     ViInt32 displayMode,
                                                     ViReal64 span)
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
  viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "",
      RSSPECAN_ATTR_MARKER_BAND_POWER_STATE, bandPowerEnabled), 2, "Band Power Enabled");
  viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
      RSSPECAN_ATTR_MARKER_BAND_POWER_MODE, displayMode), 3, "Display Mode");
  viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "",
      RSSPECAN_ATTR_MARKER_BAND_POWER_SPAN, span), 4, "Span");  
        
Error:
 (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function returns the value of the band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR result/Queries the value of the band power marker.

ViStatus _VI_FUNC rsspecan_QueryMarkerBandPower (ViSession instrSession,
                                                 ViReal64 *result)
{ 
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "",
        RSSPECAN_ATTR_QUERY_MARKER_BAND_POWER_RESULT, result), 2, "Result");    
        
Error:
 (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Threshold
 * Purpose:     This function configures marker threshold values. The marker
 *              threshold specifies a lower bound for ALL marker search
 *              functions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchThreshold (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean thresholdState,
                                                           ViReal64 markerThreshold)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_THRESHOLD_STATE, 
                thresholdState), 3, "Threshold State");

    if (thresholdState == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_THRESHOLD, 
                markerThreshold), 4, "Marker Threshold");
    }
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Peak Excursion
 * Purpose:     This function configures the marker peak excursion. The marker
 *              peak excursion specifies the minimum amplitude variation that 
 *              can be recognized as a peak or minimum by the marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchPeakExcursion (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViReal64 peakExcursion)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 

    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_PEAK_EXCURSION, 
                peakExcursion), 3, "Peak Excursion");
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Local Oscillator
 * Purpose:     This function configures state of local oscillator supression.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLocalOscillator (ViSession instrSession,
                                                                 ViInt32 window,
                                                                 ViBoolean localOscillatorSupression)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_LOEX, 
                localOscillatorSupression), 3, "Local Oscillator Supression");
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Limits
 * Purpose:     This function configures marker search limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLimits (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViBoolean searchLimits,
                                                        ViReal64 searchLimitLeft,
                                                        ViReal64 searchLimitRight)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_STATE, 
                searchLimits), 3, "Serach Limit");
    if (searchLimits == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_LEFT, 
                    searchLimitLeft), 4, "Search Limit Left");
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_RIGHT, 
                    searchLimitRight), 5, "Search Limit Right");
    }
              

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Signal Track
 * Purpose:     This function turns signal-tracking on/off. 
 *              With signal track activated, the maximum signal is determined
 *              after each frequency sweep and the center frequency is set to
 *              the frequency of this signal. Thus with drifting signals the
 *              center frequency follows the signal.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalTrack (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean signalTrackEnabled,
                                                 ViReal64 signalTrackBandwidth,
                                                 ViReal64 signalTrackThreshold,
                                                 ViInt32 trace)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_SIGNAL_TRACK_ENABLED, 
                signalTrackEnabled), 3, "Signal Track Enabled");
    if (signalTrackEnabled == VI_TRUE)
    {                
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_SIGNAL_TRACK_BWID, 
                    signalTrackBandwidth), 4, "Signal Track Bandwidth");
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_SIGNAL_TRACK_THRESHOLD, 
                    signalTrackThreshold), 5, "Signal Track Threshold");
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_SIGNAL_TRACK_TRACE, 
                    trace), 6, "Trace");
    }          
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Disable All Markers
 * Purpose:     This function turns off all the markers in selected 
 *              measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisableAllMarkers (ViSession instrSession,
                                                ViInt32 window)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MARKER_AOFF, 
                VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}

/*****************************************************************************
 * Function:    Marker Search
 * Purpose:     This function specifies the type of marker search and performs 
 *              the search.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MarkerSearch (ViSession instrSession,
                                         ViInt32 window,
                                         ViInt32 marker, 
                                         ViInt32 markerSearch)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;
    
    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    switch (markerSearch)
    {
            case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK, 
                                     VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_NEXT, 
                                     VI_NULL));
            break;

            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_LEFT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_LEFT, 
                                     VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_RIGHT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_RIGHT, 
                                     VI_NULL));
                         break;

            case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_SEARCH_MIN, VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_SEARCH_MIN_NEXT, VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_LEFT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_SEARCH_MIN_LEFT, VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_RIGHT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_SEARCH_MIN_RIGHT, VI_NULL));
            break;

            default: 
                viCheckParm ( RS_ERROR_INVALID_PARAMETER, 4, "Parameter to set");
            break;
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Marker Peak Auto
 * Purpose:     This function configures an automatic peak search action
 *              for Marker 1 at the end of each particular sweep. This function
 *              may be used during adjustments of a device under test to keep
 *              track of the actual peak marker position and level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MarkerPeakAuto (ViSession instrSession,
                                           ViInt32 window, 
                                           ViInt32 markerSearch,
                                           ViBoolean autoPeak)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    switch (markerSearch){
        case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_AUTO, 
                autoPeak), 4, "Auto Peak");
        break;
        case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN_AUTO, 
                autoPeak), 4, "Auto Peak");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker Search"); 
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Move Marker
 * Purpose:     This function moves the active marker to the specified
 *              horizontal position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MoveMarker (ViSession instrSession,
                                       ViInt32 window,
                                       ViInt32 marker, 
                                       ViReal64 markerPosition)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_POSITION, 
                markerPosition), 4, "Marker Position");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function links normal marker <FirstMarker> to any active normal marker <SecondMarker>.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR firstMarker/This control selects first marker.
/// HIPAR state/Specifies the marker link state.
/// HIPAR secondMarker/This control selects second marker.
ViStatus _VI_FUNC rsspecan_LinkMarkerToAnotherMarker (ViSession instrSession,
                                             ViInt32 window, ViInt32 firstMarker,
                                             ViInt32 secondMarker,
                                             ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (firstMarker, 1, 16) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "First Marker");
    
    if (rsspecan_invalidViInt32Range (secondMarker, 1, 16) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Second Marker");
    
    sprintf (buffer, "Win%ld,M%ld,SM%ld", window, firstMarker, secondMarker); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_LINK_TO_MARKER, 
                                                state), 4, "State");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function links delta marker <FirstMarker> to any active normal marker <SecondMarker>.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR firstMarker/This control selects first marker.
/// HIPAR state/Specifies the marker link state.
/// HIPAR secondMarker/This control selects second marker.
ViStatus _VI_FUNC rsspecan_LinkDeltaMarkerToMarker (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 firstMarker,
                                                  ViInt32 secondMarker,
                                                  ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (firstMarker, 1, 16) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "First Marker");
    
    if (rsspecan_invalidViInt32Range (secondMarker, 1, 16) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Second Marker");
    
    sprintf (buffer, "Win%ld,DM%ld,M%ld", window, firstMarker, secondMarker); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_DELTA_MARKER_LINK_TO_MARKER, 
                                                state), 4, "State");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function sets whether all markers will be linked across all 
/// HIFN  display windows.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR state/Sets whether all markers will be linked across all display windows.
ViStatus _VI_FUNC rsspecan_LinkAllMarkers(
    ViSession   instrSession,
    ViBoolean   state
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_MARKER_LINK_ALL_MARKERS,
                                                 state), 2, "State");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/*****************************************************************************
 * Function:    Query Marker
 * Purpose:     This function returns the horizontal position and the marker
 *              amplitude level of the active marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarker (ViSession instrSession,
                                        ViInt32 window,
                                        ViInt32 marker, 
                                        ViReal64 *markerPosition,
                                        ViReal64 *markerAmplitude)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;
    
    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_POSITION, 
                markerPosition), 4, "Marker Position");
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_AMPLITUDE, 
                markerAmplitude), 5, "Marker Amplitude"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Set Instrument From Marker
 * Purpose:     This function makes the selected marker frequency to be 
 *              the center frequency or step width of center frequency. 
 *              It can also make the active marker amplitude to be 
 *              the reference level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetInstrumentFromMarker (ViSession instrSession,
                                                    ViInt32 window, 
                                                    ViInt32 marker,
                                                    ViInt32 instrumentSetting)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;
    
    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        

    
    sprintf (buffer, "Win%ld,M%ld", window, marker);  
    switch(instrumentSetting)
    {
        case RSSPECAN_VAL_INSTRUMENT_SETTING_FREQUENCY_CENTER:
            viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                RSSPECAN_ATTR_MARKER_TO_CENTER, VI_NULL));
        break;
        case RSSPECAN_VAL_INSTRUMENT_SETTING_FREQUENCY_STEP: {
            if ((strstr(model, "FMU") != NULL) || (strstr(model, "FSVR") != NULL))  {
               checkErr (RS_ERROR_INSTRUMENT_MODEL); 
            }
            else {
               viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                RSSPECAN_ATTR_MARKER_TO_STEP, VI_NULL));
           }
        }
        
        break;
        case RSSPECAN_VAL_INSTRUMENT_SETTING_REFERENCE_LEVEL:
              viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                RSSPECAN_ATTR_MARKER_TO_REFERENCE, VI_NULL));
        break;
        
        default: 
            viCheckParm ( RS_ERROR_INVALID_PARAMETER, 4, "Parameter to set");
        break;
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

 /*===========================================================================*
 *= RsSpecAn Marker Type Extension Group Functions =========================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Marker Step Size
 * Purpose:     This function defines the step size of the rotary knob for 
 /*             marker or delta marker value changes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerStepSize (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 stepSize)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_MARKER_STEP_SIZE, 
                stepSize), 3, "Step Size");
   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Zoom
 * Purpose:     This function defines the range to be zoomed around marker 1 
 *              in the selected measurement window. Marker 1 is activated first, 
 *              if necessary.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_ConfigureMarkerZoom (ViSession instrSession,
                                                ViInt32 window,
                                                ViReal64 markerZoom,
                                                ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Timeout"); 
        
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));

    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_MARKER_ZOOM, 
                markerZoom), 3, "Marker Zoom");
    
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
 
/*****************************************************************************
 * Function:    Link Marker And Delta Marker
 * Purpose:     This function switches on and off the delta marker when delta 
 *              marker 1 is selected. The corresponding marker becomes the delta 
 *              marker when delta marker 2 to 4 is selected. If the corresponding 
 *              marker is not activated, it will be activated and positioned on the
 *              maximum of the measurement curve.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_LinkMarkerAndDeltamarker (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 marker,
                                                     ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 1;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_MARKER_LINK, 
                state), 4, "State");
   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*
 *= RsSpecAn Marker Functions Extension Group Functions =====================*
 *===========================================================================*/
 
/*****************************************************************************
 * Function:    Configure Marker Noise Measurement
 * Purpose:     This function switches the noise measurement on or off for 
 *              all markers of the indicated measurement window. 
 *              The noise power density is measured at the position of the markers.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerNoiseMeasurement (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViInt32 marker,
                                                            ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker);  
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_MARKER_NOISE_STATE, 
                state), 4, "State");
   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query Marker Noise Measurement Result
 * Purpose:     This function queries the result of the noise measurement.
 *              A complete sweep with synchronization to the sweep end must be
 *              performed between switching on the function and querying the
 *              measured value in order to obtain a valid query result. This is
 *              only possible in single sweep mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerNoiseMeasurementResult (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViInt32 marker,
                                                              ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker);  
    viCheckParm( rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_MARKER_NOISE_RESULT, 
                result), 4, "Result");
   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}




/*****************************************************************************
 * Function:    Configure N dB Points
 * Purpose:     This function configures the N dB function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNdBPoints (ViSession instrSession,
                                               ViInt32 window,
                                               ViBoolean ndBState,
                                               ViReal64 ndBLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_NDB_STATE,  ndBState), 
                3, "N dB State");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_NDB_VAL,  ndBLevel), 
                4, "N dB Level");                
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query N dB Result
 * Purpose:     This function queries the frequency spacing (bandwidth) of 
 *              the N-dB-down markers in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBResult (ViSession instrSession,
                                           ViInt32 window, 
                                           ViReal64 *ndBResult)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_GetAttributeViReal64(instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_NDB_RESULT,  ndBResult), 
                3, "N dB Result");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query N dB Frequencies
 * Purpose:     This function queries the frequency spacing (bandwidth) of 
 *              the N-dB-down markers in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBFrequencies (ViSession instrSession,
                                                ViInt32 window,
                                                ViReal64 *ndBFrequencyLower,
                                                ViReal64 *ndBFrequencyHigher)
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (!ndBFrequencyLower)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "N dB Frequency Lower");    
    if (!ndBFrequencyHigher)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "N dB Frequency Higher");         
   
    viCheckErr( viQueryf(instrSession, "CALC%ld:MARK:FUNC:NDBD:FREQ?\n","%Le,%Le", window, ndBFrequencyLower, ndBFrequencyHigher ));
    
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query N dB Times
 * Purpose:     This function qqueries the two times of the N-dB-down marker 
 /*             in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBTimes (ViSession instrSession,
                                          ViInt32 window,
                                          ViReal64 *n_dBTimeLower,
                                          ViReal64 *n_dBTimeHigher)
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (!n_dBTimeLower)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "N dB Frequency Lower");    
    if (!n_dBTimeHigher)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "N dB Frequency Higher");         
   
    viCheckErr( viQueryf(instrSession, "CALC%ld:MARK:FUNC:NDBD:TIME?\n","%Le,%Le", window, n_dBTimeLower, n_dBTimeHigher ));
    
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query N dB Q Factor
 * Purpose:     This function queries the Q factor (quality) of the measured 
 *              bandwidth for span>0.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBQFactor (ViSession instrSession,
                                            ViInt32 window,
                                            ViReal64 *n_dBQFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (!n_dBQFactor)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "N dB Frequency Lower");    
    
    sprintf (buffer, "Win%ld", window); 
    
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_NDB_Q_FACTOR,
                                                n_dBQFactor), 3, "N dB Q Factor");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Peak List
 * Purpose:     This function searches the selected trace for the indicated 
 *              number of maxima. The number of maxima found depends on 
 *              the waveform and value set for the Peak Excursion parameter, 
 *              however, a maximum number of 50 maxima are determined. 
 *              Only the signals which exceed their surrounding values at 
 *              least by the value indicated by the peak excursion parameter 
 *              will be recognized as maxima. Therefore, the number of maxima 
 *              found is not automatically the same as the number of maxima 
 *              desired.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakList (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViInt32 peakListCount,
                                                    ViInt32 peakListSort,
                                                    ViInt32 peakListSize,
                                                    ViBoolean automaticPeaksSearch)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker);    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_PEAK_LIST_COUNT, peakListCount),
                                                4, "Peak List Count");   
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_PEAK_LIST_SORT, peakListSort),
                                                5, "Peak List Sort");
    
    if ((strstr (model, "FSP") == NULL) && (strstr (model, "FSL") == NULL))
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                   RSSPECAN_ATTR_MARKER_PEAK_LIST_SIZE, peakListSize),
                                                   6, "Peak List Size");
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                     RSSPECAN_ATTR_MARKER_PEAK_LIST_AUTO, automaticPeaksSearch),
                                                     7, "Automatic Peaks Search");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}




/// HIFN This function activates or deactivates the marker peak search.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/This control selects marker.
/// HIPAR peakSearch/Activates or deactivates the marker peak search.
ViStatus _VI_FUNC rsspecan_ActivateMarkerPeakSearch (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 marker,
                                                     ViBoolean peakSearch)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     max_marker = 4;
    ViChar      rep_cap [15] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (rep_cap, "Win%ld,M%ld", window, marker);    

    viCheckParm(rsspecan_SetAttributeViBoolean (instrSession, rep_cap,
        RSSPECAN_ATTR_MARKER_PEAK_LIST_STATE, peakSearch), 4, "Peak Search");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/// HIFN This function configures the settings of peak list - switches on
/// HIFN the peak labels and sets the maximum number of peaks.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/This control selects marker.
/// HIPAR peakLabels/If enabled, the peaks are labelled by the corresponding marker number in the diagram.
/// HIPAR maximumPeaks/Sets the maximum number of peaks the marker peak list may contain, 
/// HIPAR maximumPeaks/i.e. how many peaks are determined at the most.
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListSettings (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViInt32 marker,
                                                            ViBoolean peakLabels,
                                                            ViInt32 maximumPeaks)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     max_marker = 4;
    ViChar      rep_cap [15] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (rep_cap, "Win%ld,M%ld", window, marker);    

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, rep_cap,
        RSSPECAN_ATTR_MARKER_PEAK_LIST_PEAK_LABELS, peakLabels), 4, "Peak Labels");
    
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, rep_cap,
        RSSPECAN_ATTR_MARKER_PEAK_LIST_MAXIMUM_PEAKS, maximumPeaks), 5, "Maximum Peaks"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Store Marker Peak List
 * Purpose:     This function stores the current marker peak list in a *.dat 
 *              file. The file consists of a data section containing the peak 
 *              list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreMarkerPeakList (ViSession instrSession,
                                                ViString path)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_MARKER_STORE_PEAK_LIST, path),
                                                2, "Path");   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query Marker Peak List Found
 * Purpose:     This function reads out the number of maxima found during 
 *              the search. If no search for maxima has been performed, 
 *              0 is returned.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerPeakListFound (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 marker,
                                                     ViInt32 *peakListFound)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker);    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_PEAK_LIST_FOUND, peakListFound),
                                                4, "Peak List Found");   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query Marker Peak List
 * Purpose:     This function queries the frequency spacing (bandwidth) of 
 *              the N-dB-down markers in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerPeakList (ViSession instrSession,
                                                ViInt32 window, 
                                                ViInt32 marker,
                                                ViInt32 arraySize,
                                                ViInt32 peakListSelection,
                                                ViReal64 peakList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *buffer = VI_NULL;
    ViChar      *pbuff=VI_NULL;
    ViInt32     i;
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    switch (peakListSelection){
        case RSSPECAN_VAL_MARKER_SORT_X:
            viCheckErr( viPrintf (instrSession, "CALC%ld:MARK%ld:FUNC:FPE:X?\n", window, marker));   
        break;
        case RSSPECAN_VAL_MARKER_SORT_Y:
            viCheckErr( viPrintf (instrSession, "CALC%ld:MARK%ld:FUNC:FPE:Y?\n", window, marker));   
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Peak List Selection"); 
        break;    
    }    
    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &buffer, VI_NULL));                                             
    pbuff = strtok (buffer, ",");
    i = 0;
    do{
        peakList[i] = atof (pbuff);
        pbuff = strtok (NULL, ",");
        i++;
    }while((pbuff != NULL) && (i<arraySize));   
    
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    if (buffer) free(buffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Limits Zoom
 * Purpose:     This function configures the limits of the search range for 
 *              markers and delta markers to the zoom area.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLimitsZoom (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViBoolean searchLimitsZoom)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_ZOOM, searchLimitsZoom),
                                                3, "Search Limits Zoom");   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Demodulation
 * Purpose:     This function configures marker demodulation parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerDemodulation (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 marker,
                                                        ViBoolean state,
                                                        ViInt32 demodulationType,
                                                        ViReal64 markerStopTime,
                                                        ViBoolean continuousDemodulation,
                                                        ViBoolean squelchState,
                                                        ViReal64 squelchLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker);    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MARKER_DEMOD_STATE, state),
                                                4, "State");   
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_MARKER_DEMOD_TYPE, demodulationType),
                                                5, "Demodulation Type");                                                 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_MARKER_DEMOD_HOLDOFF, markerStopTime),
                                                6, "Marker Stop Time"); 
    if (strstr (model, "ZVH") != NULL)
    {
        sprintf (buffer, "Win%ld", window); 
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                    RSSPECAN_ATTR_MARKER_DEMOD_CONT, continuousDemodulation),
                                                    7, "Continuous Demodulation");
    }

    if ((strstr (model, "FSL") == NULL) || !rsspecan_IsFSV (instrSession))
    {
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                    RSSPECAN_ATTR_MARKER_DEMOD_SQUELCH_STATE, squelchState),
                                                    8, "Squelch State"); 
        if (squelchState == VI_TRUE)
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_MARKER_DEMOD_SQUELCH_LEVEL, squelchLevel),
                                                    9, "Squelch Level");                                                  
        }                                            
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Peak List Annotation
 * Purpose:     This function activates or deactivates the peak list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListAnnotation (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViBoolean label,
                                                              ViBoolean marker)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);    
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                 RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_LABEL, label),
                                                 3, "Label");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                 RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_MARKER, marker),
                                                 4, "Marker");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Peak List State
 * Purpose:     This function activates or deactivates the peak list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListState (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);    
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                 RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_STATE, state),
                                                 3, "State");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Marker Save to File
 * Purpose:     This function saves the data of active markers to a file,
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MarkerSaveToFile (ViSession instrSession,
                                             ViString path)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_MARKER_FILE_EXPORT,
                path), 2, "Path");  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read Marker Peak List
 * Purpose:  This function returns the peak list found with
 *           CALCulate<1|2>:MARKer<1...4>:FUNCtion:FPEaks[:IMMediate]. The
 *           order of the values (X1, Y1, X2, Y2,...) depends on the sort
 *           order defined with  iCALCulate<1|2>:MARKer<1...4>:
 *           FUNCtion:FPEaks:SORT
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_ReadMarkerPeakList (ViSession instrSession,
                                                 ViInt32 noOfValues,
                                                 ViReal64 frequency[],
                                                 ViReal64 level[],
                                                 ViReal64 deltaLimit[],
                                                 ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     arrayLength = 0;
    ViReal64    *data = VI_NULL;
    ViInt32     retVal = 0;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
       
    arrayLength = 3*noOfValues;
    data = (ViReal64 *) malloc (sizeof(ViReal64)*arrayLength);
    if (data == NULL)
        return VI_ERROR_ALLOC;
    
   

    viCheckErr (rsspecan_dataReadTrace (instrSession, 1, "FPE", arrayLength, 
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
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn Delta Marker Group Functions ===================================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Delta Marker
 * Purpose:     This function enables the active marker on the specified trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarker (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 deltaMarker,
                                                        ViBoolean state,
                                                        ViInt32 trace)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (deltaMarker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Delta Marker");        
    
    sprintf (buffer, "Win%ld,DM%ld", window, deltaMarker);    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_MARKER_STATE, state),
                                                4, "State");
    
    if (state == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                            RSSPECAN_ATTR_REFERENCE_MARKER_TRACE, trace),
                                            5, "Trace");
    }           
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Delta Marker Position
 * Purpose:     This function configures the delta marker position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarkerPosition (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 deltaMarker,
                                                         ViInt32 mode,
                                                         ViReal64 position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (deltaMarker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Delta Marker");        
    
    sprintf (buffer, "Win%ld,DM%ld", window, deltaMarker);    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                            RSSPECAN_ATTR_REFERENCE_MARKER_MODE, mode),
                                            4, "Mode");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_MARKER_POSITION, position),
                                                5, "Position");
                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Delta Marker Position
 * Purpose:     This function returns the horizontal position and the marker 
 *              amplitude level of the selected delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryDeltaMarker (ViSession instrSession,
                                             ViInt32 window, 
                                             ViInt32 deltaMarker,
                                             ViInt32 mode, 
                                             ViReal64 *position,
                                             ViReal64 *amplitude)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;
    
    if (rsspecan_invalidViInt32Range (deltaMarker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Delta Marker");        
    
    sprintf (buffer, "Win%ld,DM%ld", window, deltaMarker);  
    switch (mode){
        case RSSPECAN_VAL_ABS:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_MARKER_POSITION, position),
                                                5, "Position");   
        break;
        case RSSPECAN_VAL_REL:
            viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_MARKER_REL_POSITION, position),
                                                5, "Position");   
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Mode"); 
    }        
    
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_REFERENCE_MARKER_AMPLITUDE, amplitude),
                                                6, "Amplitude");
                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Disable All Delta Markers
 * Purpose:     This function turns off all the delta markers in selected 
 *              measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisableAllDeltaMarkers (ViSession instrSession,
                                                   ViInt32 window)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_AOFF, 
                VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);   
    return error;
}



/*****************************************************************************
 * Function:    Delta Marker Search
 * Purpose:     This function specifies the type of delta marker search and 
 *              performs the search.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeltaMarkerSearch (ViSession instrSession,
                                              ViInt32 window,
                                              ViInt32 deltaMarker,
                                              ViInt32 markerSearch)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;
    
    if (rsspecan_invalidViInt32Range (deltaMarker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Delta Marker");        
    
    sprintf (buffer, "Win%ld,DM%ld", window, deltaMarker); 
    switch (markerSearch)
    {
            case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK, 
                                     VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT, 
                                     VI_NULL));
            break;

            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_LEFT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_LEFT, 
                                     VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_RIGHT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_RIGHT, 
                                     VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_MIN, 
                                     VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT, VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_LEFT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_LEFT, VI_NULL));
            break;
            case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_RIGHT:
                viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_RIGHT, VI_NULL));
            break;

            default: 
                viCheckParm ( RS_ERROR_INVALID_PARAMETER, 4, "Parameter to set");
            break;
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Configure a band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR displayMode/Activates the band power marker
/// HIPAR bandPowerEnabled/efines whether the result is displayed as a power or density.
/// HIPAR span/Defines the span (band) around the marker for which the power is measured.

ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarkerBandPower (ViSession instrSession,
                                                          ViInt32 displayMode,
                                                          ViBoolean bandPowerEnabled,
                                                          ViReal64 span)
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "",
        RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_STATE, bandPowerEnabled), 2, "Band Power Enabled");        
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "",
        RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_MODE, displayMode), 3, "Display Mode");
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "",
        RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_SPAN, span), 4, "Span");       

Error:
 (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the value of the band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR result/Queries the value of the band power marker.

ViStatus _VI_FUNC rsspecan_QueryDeltaMarkerBandPower (ViSession instrSession,
                                                      ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "",
      RSSPECAN_ATTR_QUERY_DELTA_MARKER_BAND_POWER_RESULT, result), 2, "Result");
Error:
 (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Fixed
 * Purpose:     This function configures marker reference fixed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceFixed (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean fixedReference)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_REF_STATE,  fixedReference), 3, "Fixed Reference");  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Fixed Point
 * Purpose:     This function configures marker reference fixed position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceFixedPoint (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViReal64 refPointLevel,
                                                         ViReal64 refPointLevelOffset,
                                                         ViReal64 refPointFrequencyTime)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
            RSSPECAN_ATTR_MARKER_REF_LEVEL_OFFSET,  refPointLevelOffset), 
            4, "Reference Point Level Offset");   
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
            RSSPECAN_ATTR_MARKER_REF_LEVEL,  refPointLevel), 3, "Reference Point Level");       
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
            RSSPECAN_ATTR_MARKER_REF_FREQ,  refPointFrequencyTime), 
            5, "Reference Point Frequency");                  

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Reference Fixed Peak Search
 * Purpose:     This function sets the reference point level for all delta 
 *              markers in the selected measurement window for a measurement 
 *              with fixed reference point (CALC:DELT:FUNC:FIX:STAT ON) to 
 *              the peak of the selected trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReferenceFixedPeakSearch (ViSession instrSession,
                                                     ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_REF_PEAK, 
                VI_NULL));
   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Marker Phase Noise Measurement
 * Purpose:     This function configures phase noise marker measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPhaseNoiseMeasurement (ViSession instrSession,
                                                                 ViInt32 window,
                                                                 ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, buffer, 
                RSSPECAN_ATTR_REFERENCE_MARKER_PNO_STATE,  state), 3, "State");

    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Phase Noise Peak Search
 * Purpose:     This function sets the reference point level for all delta 
 *              markers in the selected measurement window for a measurement 
 *              with fixed reference point to the peak of the selected trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PhaseNoisePeakSearch (ViSession instrSession,
                                                 ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString(instrSession, buffer, 
                RSSPECAN_ATTR_MARKER_REF_PEAK,  VI_NULL));
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Phase Noise Peak Search Auto
 * Purpose:     This function sests an automatic peak search action for 
 *              the reference fixed marker 1 at the end of each particular sweep. 
 *              This function may be used for tracking of a drifting source 
 *              whilst phase noise measurement. The delta marker 2 which shows 
 *              the phase noise measurement result keeps the delta frequency value. 
 *              Therefore the phase noise measurement in a certain offset is valid
 *              although the source is drifting. Only when the marker 2 is
 *              reaching the border of the span the delta marker value is
 *              adjusted to be within the span. Choose a larger span in such
 *              situations.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PhaseNoisePeakSearchAuto (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, buffer, 
                RSSPECAN_ATTR_REFERENCE_MARKER_PNO_AUTO,  state), 2, "State");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Query Phase Noise Result
 * Purpose:     This function queries the result of the phase-noise measurement 
 *              in the selected measurement window. The measurement will be 
 *              switched on, if necessary.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryPhaseNoiseResult (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViReal64 *phaseNoiseResult)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_GetAttributeViReal64(instrSession, buffer, 
                RSSPECAN_ATTR_REFERENCE_MARKER_PNO_RESULT,  phaseNoiseResult), 
                3, "Phase Noise Result");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn System Group Functions =========================================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Reference Oscillator
 * Purpose:     This function configures the reference oscillator.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceOscillator (ViSession instrSession,
                                                         ViInt32 source,
                                                         ViReal64 externalFrequency)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model[10] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                   RS_ATTR_INSTRUMENT_MODEL, 10, model));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_ROSC_SOURCE, source),
                                                2, "Source");
    if ((source == RSSPECAN_VAL_SOUR_EXT) && 
       (((strstr (model, "FSP") == NULL) || (strstr (model, "FSL") == NULL))))
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_ROSC_FREQUENCY, externalFrequency),
                                                3, "External Frequency");                                                 
    }
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Query Selected Reference Oscillator Source
 * Purpose:     This function returns the current selection of the reference 
 *              signal (internal / external) if the external reference source 
 *              EXT [INT] is used. The function is only available, 
 *              if SENS:ROSC:SOUR EAUT is selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySelectedReferenceOscillatorSource (ViSession instrSession,
                                                                   ViInt32 *source)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_ROSC_SOURCE_EAUTO, source),
                                                2, "Source");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Reference Oscillator External PLL Bandwidth
 * Purpose:     This function controls the PLL bandwidth used to synchronize 
 *              with the external reference signal. It is only available if the 
 *              external reference frequency is set to 10 MHz. The reference 
 *              settings are not changed if a preset occurs to maintain the 
 *              specific setup of a test system.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceOscillatorExternalPLLBandwidth (ViSession instrSession,
                                                                             ViInt32 bandwidth)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_ROSC_PLL_BWID, bandwidth),
                                                2, "Bandwidth");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Speaker Volume
 * Purpose:     This function sets the volume of the built-in loudspeaker 
 *              for demodulated signals.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpeakerVolume (ViSession instrSession,
                                          ViReal64 speakerVolume)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_SYST_SPEAKER, speakerVolume / 100.0),
                                                2, "Speaker Volume");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    System Version
 * Purpose:     This function queries the number of the SCPI version, which 
 *              is relevant for the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SystemVersion (ViSession instrSession,
                                          ViInt32 length, 
                                          ViChar systemVersion[])

{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_GetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_SYST_VERSION, length,
                                                systemVersion),
                                                2, "System Version");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN  This function reads the version information of all available measurement applications.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR bufferSize/This control specifies the size of the buffer.
/// HIPAR results/Returns the version information of all available measurement
/// HIPAR results/applications. The lines of the result table are output as string
/// HIPAR results/data separated by commas.
ViStatus _VI_FUNC rsspecan_ApplicationsVersions (ViSession instrSession,
                                                 ViInt32 bufferSize,
                                                 ViChar results[])
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_GetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_APPLICATIONS_VERSIONS, bufferSize,
                                                results), 3, "Results");

    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*===========================================================================*/
/* Function: Application Setup Recovery
/* Purpose:  This function controls the instrument behaviour when changing 
/*           the active application, e.g from SPECTRUM to FM DEMOD and back 
/*           from FM DEMOD to SPECTRUM. In the default state OFF a few
/*           parameters of the current analyzer setting are passed to the 
/*           application (e.g. center frequency, level settings) or from the 
/*           application back to the analyzer mode. If APPL SETUP RECOVERY is 
/*           switched ON, the settings of the applications are independent of 
/*           each other. Leaving the FM DEMOD application will restore the 
/*           previous state of the ANALYZER.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ApplicationSetupRecovery(
    ViSession   instrSession,
    ViBoolean   applicationSetupRecovery
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm( rsspecan_SetAttributeViBoolean(instrSession, "", 
                RSSPECAN_ATTR_APPL_SETUP_REC,  applicationSetupRecovery), 2, "Application Setup Recovery");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Configure IF Shift
/* Purpose:  This function configures the shifting of the 1st IF.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureIFShift(
    ViSession   instrSession,
    ViInt32 ifShift,
    ViReal64    ifShiftFrequency
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
        
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_IF_SHIFT, ifShift),
                                                2, "If Shift");
    if (ifShift == RSSPECAN_VAL_IF_SHIFT_AUTO)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_IF_SHIFT_FREQ, ifShiftFrequency),
                                                3, "If Shift Frequency");                                                 
    }
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function Turns logging of remote commands on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR state/Turns logging of remote commands on and off.
ViStatus _VI_FUNC rsspecan_ConfigureRemoteLoggingState (ViSession instrSession,
                                                        ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_REMOTE_LOGGING_STATE, 
                                                state), 2, "State");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: System Shutdown
/* Purpose:  This function shuts down the instrument.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SystemShutdown (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_SYSTEM_SHUTDOWN, VI_NULL));
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function queries the current frontend temperature of the R&S FSW.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR frontendTemperature/Temperature in degrees Celsius.
ViStatus _VI_FUNC rsspecan_QueryFrontendTemperature(
    ViSession   instrSession,
    ViReal64*   frontendTemperature
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_FRONTEND_TEMPERATURE,
                                                frontendTemperature), 2, "Frontend Temperature");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/*===========================================================================*
 *= RsSpecAn Trigger Group Functions ========================================*
 *===========================================================================*/
/*****************************************************************************
 * Function:    Configure Trigger Source
 * Purpose:     This function specifies the trigger source that causes the
 *              spectrum analyzer to leave the Wait-for-Trigger state.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTriggerSource (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 triggerSource)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    if ((triggerSource > RSSPECAN_VAL_TRG_IQP) && !rsspecan_IsFSV (instrSession)) 
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_SOURCE,
                                                triggerSource),
                                                2, "Trigger Source");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure TV or RF Power Trigger Source
 * Purpose:     This function specifies the trigger source that causes the
 *              spectrum analyzer to leave the Wait-for-Trigger state.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTVRFPTriggerSource (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 triggerSource)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_SOURCE_TVRF,
                                                triggerSource),
                                                2, "Trigger Source");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Trigger
 * Purpose:     This function specifies the delay and polarity for triggering.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTrigger (ViSession instrSession,
                                             ViInt32 window,
                                             ViReal64 triggerDelay,
                                             ViInt32 triggerPolarity)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);             
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_DELAY,
                                                triggerDelay),
                                                2, "Trigger Delay");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_SLOPE,
                                                triggerPolarity),
                                                3, "Trigger Polarity");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


/// HIFN This function specifies the dropout time for triggering.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR dropoutTime/Specifies the dropout time for triggering.
ViStatus _VI_FUNC rsspecan_ConfigureTriggerDropoutTime (ViSession instrSession,
                                                        ViReal64 dropoutTime)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_TRIGGER_DROPOUT_TIME,
                                                dropoutTime),
                                                2, "Dropout Time");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Trigger
 * Purpose:     This function defines the external trigger parameters. 
 *              This is applicable when the trigger source is set to external.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalTrigger (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViReal64 externalTriggerLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL,
                                                externalTriggerLevel),
                                                2, "External Trigger Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function defines the external trigger level.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Document parameter here.
/// HIPAR externalTriggerLevel/Defines the external trigger level.
ViStatus _VI_FUNC rsspecan_ConfigureExternalTriggerPort (ViSession instrSession,
                                                         ViInt32 port,
                                                         ViReal64 externalTriggerLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    if (rsspecan_invalidViInt32Range (port, 1, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Port");

    sprintf (buffer, "Port%ld", port);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL_PORT,
                                                externalTriggerLevel),
                                                2, "External Trigger Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Delay Compensation
 * Purpose:     This function defines the external trigger delay compensation. 
 *              This is applicable when the trigger source is set to external.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDelayCompensation (ViSession instrSession,
                                                       ViInt32 window,
                                                       ViBoolean delayCompensation)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             

    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_EXTERNAL_TRIGGER_DELAY_COMPENSATION,
                                                delayCompensation),
                                                3, "Delay Compensation");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Video Trigger
 * Purpose:     This function specifies the video level and slope for
 *              triggering.  This is applicable when the trigger source is set
 *              to video.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureVideoTrigger (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViReal64 videoTriggerLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_VIDEO_TRIGGER_LEVEL,
                                                videoTriggerLevel),
                                                2, "Video Trigger Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}
/*****************************************************************************
 * Function:    Configure IF Power Trigger
 * Purpose:     This function sets the level of the IF power trigger source.
 *              This is applicable when the trigger source is set to IF Power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIFPowerTrigger (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 triggerLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_IFP_LEVEL,
                                                triggerLevel),
                                                2, "Trigger Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure IF Power Trigger Parameters
 * Purpose:     This function sets the IF power trigger parameters. This is 
 *              applicable when the trigger source is set to IF Power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIFPowerTriggerParameters (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViReal64 triggerLevel,
                                                              ViReal64 holdoff,
                                                              ViReal64 hysteresis)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_IFP_LEVEL,
                                                triggerLevel),
                                                2, "Trigger Level");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_IFP_OFFSET,
                                                holdoff),
                                                3, "Holdoff");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_IFP_HYSTERESIS,
                                                hysteresis),
                                                4, "Hysteresis");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function sets the level of the IQ power trigger source.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR triggerLevel/Sets the level of the IQ power trigger.
ViStatus _VI_FUNC rsspecan_ConfigureIQPowerTrigger (ViSession instrSession,
                                           ViInt32 window, ViReal64 triggerLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_IQP_LEVEL,
                                                triggerLevel),
                                                2, "Trigger Level");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure RF Power Trigger
 * Purpose:     This function specifies sets the level of the RF power 
 *              trigger source. This is applicable when the trigger source is 
 *              set to RF Power. 
 *              Note:
 *              This function is available only with FSP-B6 option
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureRFPowerTrigger (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 triggerLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_RFP_LEVEL,
                                                triggerLevel),
                                                3, "Trigger Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function defines the holding time before the next trigger event.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR holdoff/This control sets the holding time before the next trigger event.
ViStatus _VI_FUNC rsspecan_ConfigureRFPowerTriggerHoldoff (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViReal64 holdoff)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_RFP_HOLDOFF,
                                                holdoff), 3, "Holdoff");
Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Time Trigger
 * Purpose:     This function configures the time trigger. This is applicable
 *              when the trigger source is set to time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTimeTrigger (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViReal64 timeInterval)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);             
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_TIME_INTERVAL,
                                                timeInterval),
                                                2, "Time Interval");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure TV Trigger
 * Purpose:     This function configures the TV Trigger parameters. This is 
 *              applicable when the trigger source is set to TV.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTVTrigger (ViSession instrSession,
                                               ViInt32 window,
                                               ViInt32 lineSystem,
                                               ViInt32 synchronization,
                                               ViInt32 horizontalSyncLine,
                                               ViInt32 polarity,
                                               ViBoolean TVFreeRunState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window);  

    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_TRIGGER_TV_LINE_SYSTEM,
                                                lineSystem), 3, "Line System");        
    switch (synchronization){
        case RSSPECAN_VAL_TRIG_TV_VFIEL_ALL:
        case RSSPECAN_VAL_TRIG_TV_VFIEL_ODD:
        case RSSPECAN_VAL_TRIG_TV_VFIEL_EVEN:
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_TRIGGER_TV_VERTICAL_SIGNAL,
                                                synchronization), 4, "Synchronization");    
        break;
        case RSSPECAN_VAL_TRIG_TV_HORIZONTAL :
                viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_TRIGGER_TV_HORIZONTAL_SIGNAL,
                                                horizontalSyncLine), 5, "Horizontal Sync Line");    
        
        break;                                                                  
    }       
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_TRIGGER_TV_VIDEO_POLARITY,
                                                polarity), 6, "Polarity");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_TRIGGER_TV_VID_CONT,
                                                TVFreeRunState), 7, "TV Free Run State");   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


/*****************************************************************************
 * Function:    Query Trigger Position In Sample
 * Purpose:     This function determines the time offset of the trigger in the
 *              sample (trigger position in sample = TPIS). This value can only
 *              be determined in triggered measurements using external or
 *              IFPower triggers, otherwise the value is 0.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTriggerPositionInSample (ViSession instrSession,
                                                         ViInt32 *triggerPositionInSample)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", 
        RSSPECAN_ATTR_TRIGGER_POSITION_IN_SAMPLE, triggerPositionInSample), 
        2, "Trigger Position In Sample");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


/// HIFN This function configures the trigger direction as input.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Selects the trigger port to which the output is sent.
ViStatus _VI_FUNC rsspecan_ConfigureTriggerInput (ViSession instrSession,
                                                  ViInt32 port)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    if (rsspecan_invalidViInt32Range (port, 2, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Port");

    sprintf (buffer, "Port%ld", port);             
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_TRIGGER_DIRECTION,
                                                RSSPECAN_VAL_TRIG_DIRECTION_INP));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function selects the trigger output type, length and if
/// HIFN user defined output type is selected, also a level.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Selects the trigger port to which the output is sent.
/// HIPAR outputType/Selects the type of signal generated at the trigger output.
/// HIPAR level/Selects signal level.
/// HIPAR length/Pulse length in seconds.
ViStatus _VI_FUNC rsspecan_ConfigureTriggerOutput (ViSession instrSession,
                                                   ViInt32 port,
                                                   ViInt32 outputType,
                                                   ViInt32 level,
                                                   ViReal64 length)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    if (rsspecan_invalidViInt32Range (port, 2, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Port");

    sprintf (buffer, "Port%ld", port);             
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                     RSSPECAN_ATTR_TRIGGER_DIRECTION,
                                     RSSPECAN_VAL_TRIG_DIRECTION_OUTP));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                    RSSPECAN_ATTR_TRIGGER_OUTPUT_TYPE, outputType), 3, "Output Type");
    
    if (outputType == RSSPECAN_VAL_TRIG_OTYPE_UDEFINED)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                    RSSPECAN_ATTR_TRIGGER_LEVEL, level), 4, "Level");
    }
    
    viCheckParm( rsspecan_SetAttributeViReal64(instrSession, buffer, 
                    RSSPECAN_ATTR_TRIGGER_PULSE_LENGTH, length), 5, "Length");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
} 

/// HIFN This function generates a pulse at the trigger output.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Selects the trigger port to which the output is sent.
ViStatus _VI_FUNC rsspecan_GenerateTriggerPulse (ViSession instrSession,
                                                 ViInt32 port)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    if (rsspecan_invalidViInt32Range (port, 2, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Port");

    sprintf (buffer, "Port%ld", port);             
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                           RSSPECAN_ATTR_TRIGGER_PULSE_GENERATE, NULL));
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Spectrogram Window
 * Purpose:  This function selects the current window for the realtime
 *           spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FrequencyMaskWindow (ViSession instrSession,
                                              ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW,
                                                window));                                                
    
Error:
     Rs_UnlockSession(instrSession, VI_NULL);
     return error;  
}

/*****************************************************************************
 * Function: Configure Frequency Mask Trigger Source
 * Purpose:  This function selects the frequency mask trigger.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskTriggerSource (ViSession instrSession)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, 
        RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
     
    sprintf (buffer, "Win%ld", win); 

    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
        RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_SOURCE, NULL));

    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask
 * Purpose:  This function sets the frequency mask triggering name with
             comments.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMask (ViSession instrSession,
                                                   ViString name,
                                                   ViString comment)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_NAME, name), 
                        2, "Name"); 
    
    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_COMMENT, comment), 
                        3, "Comment");    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/// HIFN This function sets the directory the instrument stores frequency masks in.
/// HIRET Returns the status code of this operation.
/// HIRET You may use multiple lines for documentation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the
/// HIPAR instrSession/Initialize function to select the desired instrument driver session.
/// HIPAR directory/This control sets the directory to store the frequency mask file in.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskDirectory (ViSession instrSession,
                                                            ViString directory)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_DIRECTORY, directory), 
                        2, "Directory"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Delete Frequency Mask
 * Purpose:  This function deletes a frequency mask trigger mask.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteFrequencyMask (ViSession instrSession,
                                                ViString name)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_DELETE, name), 
                        2, "Name"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);  
    return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Condition
 * Purpose:  This function sets the condition that activates the frequency
             mask.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskCondition (ViSession instrSession,
                                                            ViInt32 condition)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGG_CONDITION, condition), 
                        2, "Condition"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);  
    return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Shape
 * Purpose:  This function configures the frequency mask shape.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskShape (ViSession instrSession,
                                                        ViInt32 arraySize, 
                                                        ViInt32 mode,
                                                        ViReal64 lower[],
                                                        ViReal64 upper[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViChar      *p2buf = NULL;
    ViInt32     i = 0;
    ViInt32     win = 1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_MODE, mode), 
                        2, "Mode"); 
    
    p2buf = buffer + sprintf (buffer, "CALC:MASK:LOW ");
    
    for (i = 0; i < arraySize * 2; i++)
        p2buf += sprintf (p2buf, "%.12lG,", lower[i]);
    
    *p2buf = '\0';
    *--p2buf = '\n';
    
    checkErr (viWrite (instrSession, (ViBuf)buffer, (ViUInt32) strlen (buffer), NULL));
    
    checkErr( rsspecan_CheckStatus (instrSession));
    
    p2buf = buffer + sprintf (buffer, "CALC:MASK:UPP ");
    
    for (i = 0; i < arraySize * 2; i++)
        p2buf += sprintf (p2buf, "%.12lG,", upper[i]);
    
    *p2buf = '\0';
    *--p2buf = '\n';
    
    checkErr (viWrite (instrSession, (ViBuf)buffer, (ViUInt32) strlen (buffer), NULL));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
    
}

/// HIFN This function automatically defines the shape of the frequency
/// HIFN mask trigger mask according to the spectrum that is currently
/// HIFN measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskAuto (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViInt32     win = 1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_UPP_AUTO,NULL)); 
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Shift 
 * Purpose:  This function sets the frequency mask shift.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskShift (ViSession instrSession,
                                                        ViInt32 type,
                                                        ViReal64 frequency,
                                                        ViReal64 amplitude)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
     switch (type){
        case RSSPECAN_VAL_LOWER:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_LOW_X, 
                frequency), 3, "Frequency");
            
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_LOW_Y, 
                amplitude), 4, "Amplitude"); 
        break;
        case RSSPECAN_VAL_UPPER:
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_UPP_X, 
                frequency), 3, "Frequency");
            
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_UPP_Y, 
                amplitude), 4, "Amplitude"); 
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");     
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN  This function turns the lower or upper frequency mask on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR type/This control selects the upper and the lower mask.
/// HIPAR frequencyMask/This control turns the lower or upper frequency mask on and off.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskState(
    ViSession   instrSession,
    ViInt32 type,
    ViBoolean   frequencyMask
)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    switch (type)
    {
        case RSSPECAN_VAL_LOWER:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_LOWER_STATE, 
                frequencyMask), 3, "Frequency Mask");
        break;
        case RSSPECAN_VAL_UPPER:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_FREQUENCY_MASK_UPPER_STATE, 
                frequencyMask), 3, "Frequency Mask");
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Type");     
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN  This function configures the frequency mask span.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR span/This control sets the frequency mask span.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskSpan(
    ViSession   instrSession,
    ViReal64    span
)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));
    
    sprintf (buffer, "Win%ld",win);
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_FREQUENCY_MASK_SPAN,
                                                span), 2, "Span");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/*===========================================================================*
 *= RsSpecAn Gate Group Functions ===========================================*
 *===========================================================================*/
/*****************************************************************************
 * Function:    Configure External Gate
 * Purpose:     This function configures the external gate parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGate (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViBoolean gating,
                                                  ViInt32 gateSource,
                                                  ViInt32 mode, ViInt32 polarity,
                                                  ViReal64 delay,
                                                  ViReal64 length)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_src = RSSPECAN_VAL_EGAT_SOUR_IFP;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);   
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL") != NULL))
        max_src = RSSPECAN_VAL_EGAT_SOUR_VID;
    
    if (rsspecan_IsFSV (instrSession)) max_src = RSSPECAN_VAL_EGAT_SOUR_PSE;
 
    if (strstr (model, "FSVR") != NULL) max_src = RSSPECAN_VAL_EGAT_SOUR_RFP;
    
    if (strstr (model, "FSW") != NULL) max_src = RSSPECAN_VAL_EGAT_SOUR_IQP;
    
    if (rsspecan_invalidViInt32Range (gateSource, RSSPECAN_VAL_EGAT_SOUR_EXT, max_src) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Gate Source");
    

    if (gating == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_EXTERNAL_GATE_SIGNAL_SOURCE,
                                                    gateSource), 4, "Gate Source");   
         
        
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_EXTERNAL_GATE_TRIGGER_TYPE,
                                                    mode), 5, "Mode"); 
        if (gateSource == RSSPECAN_VAL_EGAT_SOUR_EXT) { 
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_EXTERNAL_GATE_POLARITY,
                                                    polarity), 6, "Polarity");     
        }
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                        RSSPECAN_ATTR_EXTERNAL_GATE_HOLD,
                                                        delay), 7, "Delay"); 
        
        if (mode==RSSPECAN_VAL_EGAT_TRIG_EDGE)                                                    
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_EXTERNAL_GATE_LENGTH,
                                                    length), 8, "Length");                                                     
    }                                                    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_EXTERNAL_GATE,
                                                gating),
                                                3, "Gating");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


/*****************************************************************************
 * Function:    Configure External Gate Trace
 * Purpose:     This function configures the external gate trace parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGateTrace (ViSession instrSession,
                                                       ViInt32 trace,
                                                       ViInt32 range,
                                                       ViBoolean traceState,
                                                       ViString comment,
                                                       ViReal64 traceStart,
                                                       ViReal64 traceStop,
                                                       ViReal64 periodLength)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (trace, 1, 6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Trace");
        
    if (rsspecan_invalidViInt32Range (range, 1, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Range");
        
    sprintf (buffer, "TR%ld,ER%ld", trace, range);   
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                 RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_STATE,
                                                 traceState), 4, "Trace State");

    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer,
                                                 RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_COMMENT,
                                                 comment), 5, "Comment");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                 RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_START,
                                                 traceStart), 6, "Trace Start");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                 RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_STOP,
                                                 traceStop), 7, "Trace Stop");

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                 RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_PERIOD_LENGTH,
                                                 periodLength), 8, "Period Length");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*===========================================================================*
 *= RsSpecAn External Mixing Extension Group Functions =====================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure External Mixer Enabled
 * Purpose:     This function turns external mixing on/off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerEnabled (ViSession instrSession,
                                                          ViBoolean externalMixingEnabled)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_ENABLED,
                                                externalMixingEnabled),
                                                2, "External Mixer Enabled");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Number of Ports
 * Purpose:     This function specifies a 2-port or a 3-port external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerNumberOfPorts (ViSession instrSession,
                                                                ViInt32 numberofPorts)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_NUMBER_OF_PORTS,
                                                numberofPorts), 2, "Number Of Ports");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer
 * Purpose:     This function specifies the mixer harmonic and average
 *              conversion loss.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixer (ViSession instrSession,
                                                   ViInt32 band,
                                                   ViInt32 harmonic,
                                                   ViInt32 harmonicType)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC_BAND,
                                                band), 2, "Band");                                                
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC_TYPE,
                                                harmonicType), 4, "Harmonic Type");         
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC,
                                                harmonic), 3, "Harmonic");        
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Query External Mixer RF Start Stop
 * Purpose:     This function queries the frequency at which the external mixer
 *              band starts and stops.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryExternalMixerRFStartStop (ViSession instrSession,
                                                          ViReal64 *RFStart,
                                                          ViReal64 *RFStop)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                 RSSPECAN_ATTR_GET_EXTERNAL_MIXER_RF_FREQUENCY_START,
                                                 RFStart), 2, "RF Start");
    
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                 RSSPECAN_ATTR_GET_EXTERNAL_MIXER_RF_FREQUENCY_STOP,
                                                 RFStop), 3, "RF Stop");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Handover Frequency
 * Purpose:     This function defines the frequency at which the mixer switches
 *              from one range to the next (if two different ranges are
 *              selected). The handover frequency for each band can be selected
 *              freely within the overlapping frequency range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerHandoverFrequency (ViSession instrSession,
                                                                    ViReal64 handoverFrequency)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                 RSSPECAN_ATTR_EXTERNAL_MIXER_HANDOVER_FREQUENCY,
                                                 handoverFrequency), 2, "Handover Frequency");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer RF Overrange
 * Purpose:     This function specifies whether the frequencies beyond the
 *              defined band limits are to be used.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerRFOverrange (ViSession instrSession,
                                                              ViBoolean RFOverrange)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_EXTERNAL_MIXER_RF_OVERRANGE,
                                                 RFOverrange), 2, "RF Overrange");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Preset Band
 * Purpose:     This function restores the preset frequency ranges for the
 *              standard waveguide bands.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerPresetBand (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_EXTERNAL_MIXER_PRESET_BAND,
                                               NULL));
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}


/*****************************************************************************
 * Function:    Configure External Mixer High Harmonic
 * Purpose:     This function specifies a second (high) harmonic.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerHighHarmonic (ViSession instrSession,
                                                               ViBoolean highHarmonic,
                                                               ViInt32 harmonicOrder)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_EXTERNAL_MIXER_HIGH_HARMONIC_STATE,
                                                 highHarmonic), 2, "High Harmonic");
    
    if (highHarmonic == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                                     RSSPECAN_ATTR_EXTERNAL_MIXER_HIGH_HARMONIC,
                                                     harmonicOrder), 3, "Harmonic Order");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Signal ID Mode
 * Purpose:     This function sets the Signal ID or Auto ID mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerSignalIDMode (ViSession instrSession,
                                                       ViInt32 mode,
                                                       ViReal64 autoIDThreshold)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_SIGNAL_ID_MODE,
                                                mode), 2, "Mode");                                                
    if (mode==RSSPECAN_VAL_STATE_AUTO)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_THRESHOLD,
                                                autoIDThreshold), 3, "Auto ID Threshold");        
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Bias
 * Purpose:     This function configures the bias current for the low and high
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerBias (ViSession instrSession,
                                                       ViInt32 range, ViReal64 bias)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (range == 1)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                    RSSPECAN_ATTR_EXTERNAL_MIXER_BIAS_LOW,
                                                    bias), 2, "Bias");
    }
    else
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_BIAS_HIGH,
                                                bias), 2, "Bias");        
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Conversion Loss
 * Purpose:     This function specifies the conversion loss for selected band.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerConversionLoss (ViSession instrSession,
                                                                 ViInt32 range, 
                                                                 ViInt32 conversionLossMode,
                                                                 ViReal64 conversionLoss,
                                                                 ViString conversionLossTable)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (range, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
        
    if (rsspecan_invalidViInt32Range (conversionLossMode, RSSPECAN_VAL_MIXER_LOSS_AVER, RSSPECAN_VAL_MIXER_LOSS_TABL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Conversion Loss Mode");
        
    if (range == 1)
    {
        if (conversionLossMode == RSSPECAN_VAL_MIXER_LOSS_AVER)
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                    RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_LOW,
                                                    conversionLoss), 4, "Conversion Loss");
        }
        else
        {
            viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                    RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_TABLE_LOW,
                                                    conversionLossTable), 5, "Conversion Loss Table");                                                
        }
    }
    else
    {
        if (conversionLossMode == RSSPECAN_VAL_MIXER_LOSS_AVER)
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                    RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_HIGH,
                                                    conversionLoss), 4, "Conversion Loss");
        }
        else
        {
            viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                    RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_TABLE_HIGH,
                                                    conversionLossTable), 5, "Conversion Loss Table");        
        }
    }
        
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Low Power
 * Purpose:     This function selects a conversion loss table and sets it for 
 *              the active band.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerLowPower (ViSession instrSession,
                                                           ViReal64 lowPower)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_EXTERNAL_MIXER_LO_POWER,
                                                lowPower), 2, "Low POwer");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Create External Mixer Conversion Loss Table
 * Purpose:     This function creates or overwrites conversion loss table of 
 *              external mixer.
 *****************************************************************************/
#pragma warning( push )
#pragma warning( disable : 4100)
ViStatus _VI_FUNC rsspecan_CreateExternalMixerConversionLossTable (ViSession instrSession,
                                                                   ViInt32 window,
                                                                   ViString tableName,
                                                                   ViString mixerType,
                                                                   ViInt32 band,
                                                                   ViString serialNumber,
                                                                   ViInt32 harmonic,
                                                                   ViInt32 ports,
                                                                   ViReal64 bias,
                                                                   ViString comment,
                                                                   ViInt32 numberOfValues,
                                                                   ViReal64 frequencyValues[],
                                                                   ViReal64 conversionLossValues[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuf;
    ViInt32     i;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if ((strstr(model, "FMU") != NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

   // sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_CVL_NAME, 
                tableName), 3, "Table Name");
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_CVL_MIXER, 
                mixerType), 4, "Mixer Type");                
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_CVL_BAND, 
                band), 5, "Band");     
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_CVL_MIXER_SNUM, 
                serialNumber), 6, "Serial Number"); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_CVL_HARM, 
                harmonic), 7, "Harmonic");                
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_CVL_PORT_MIXER, 
                ports), 8, "Ports");      
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_CVL_BIAS, 
                bias), 9, "Bias");
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_CVL_COMMENT, 
                comment), 10, "Comment"); 
    
    pbuf = buffer;
    if (numberOfValues>0)
    {
        pbuf += sprintf (buffer, "CORR:CVL:DATA ");
        
        for (i = 0; i < numberOfValues; i++)
            pbuf += sprintf (pbuf, "%.12f,%.12f,", frequencyValues[i], conversionLossValues[i]);       
        
        *pbuf = '\0';
        *--pbuf = '\n';
        
        viCheckErr (viPrintf (instrSession, "%s\n", buffer));
    }    
    checkErr( rsspecan_CheckStatus (instrSession)); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    External Mixer Conversion Loss Table Select
 * Purpose:     This function selects a conversion loss table of external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableSelect (ViSession instrSession,
                                                                   ViInt32 window,
                                                                   ViString tableName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    //sprintf (buffer, "Win%ld", window);   
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_CVL_NAME,
                                                tableName),
                                                2, "Table Name");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    External Mixer Conversion Loss Table Delete
 * Purpose:     This function deletes a conversion loss table of external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableDelete (ViSession instrSession,
                                                                   ViInt32 window,
                                                                   ViString tableName)
{
    ViStatus    error = VI_SUCCESS; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_CVL_NAME,
                                                tableName),
                                                2, "Table Name");
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_CVL_CLEAR,
                                                tableName),
                                                2, "Table Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}
#pragma warning( pop )

/*****************************************************************************
 * Function:    External Mixer Conversion Loss Table Delete
 * Purpose:     This function deletes a conversion loss table of external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableCatalog (ViSession instrSession,
                                                                    ViInt32 *numberofCVL,
                                                                    ViInt32 bufferSize,
                                                                    ViChar CVLNamesList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf = VI_NULL;
    ViChar      *buf2 = VI_NULL;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";   
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if ((strstr(model, "FMU") != NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (!((strstr (buffer, "K90")  != NULL) || (strstr (buffer, "B21") != NULL)))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (CVLNamesList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "CVL Names List");  
       
    viCheckErr (viPrintf (instrSession, ":CORR:CVL:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    //viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, CVLNamesList, numberofCVL));
    buf2 = malloc (retCnt);
    sscanf(buf, "'%[^']'", buf2);
    viCheckErr (rsspecan_ParseLimitLineCatalog (buf2, bufferSize, CVLNamesList, numberofCVL));
    
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    if (buf2) free(buf2);
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}

/*****************************************************************************
 * Function:    Configure RF Input Coupling
 * Purpose:     This function switches the input coupling of the RF input 
 *              between AC and DC.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureRFInputCoupling (ViSession instrSession,
                                                     ViInt32 RFInputCoupling)
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_AMPL_RF_COUPLING,
                                                RFInputCoupling),
                                                2, "RF Input Coupling");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function:    Configure Electronic Attenuator
 * Purpose:     This function switches the input coupling of the RF input 
 *              between AC and DC.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureElectronicAttenuator (ViSession instrSession,
                                                          ViInt32 input,
                                                          ViInt32 mode,
                                                          ViInt32 manualValue)
{
    ViStatus    error = VI_SUCCESS;  
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (input, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Input");
 
    sprintf (buffer, "Win%ld", input);    
    switch (mode){
        case RSSPECAN_VAL_EATT_OFF:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_AMPL_EATT_STATE,
                                                         VI_FALSE), 3, "Mode");  
        break;
        case RSSPECAN_VAL_EATT_AUTO:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_AMPL_EATT_STATE,
                                                         VI_TRUE), 3, "Mode");  
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_AMPL_EATT_AUTO,
                                                         VI_TRUE), 3, "Mode");                                                           
        break;
        case RSSPECAN_VAL_EATT_MAN:
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_AMPL_EATT_STATE,
                                                         VI_TRUE), 3, "Mode");  
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_AMPL_EATT_AUTO,
                                                         VI_FALSE), 3, "Mode");                                                           
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                         RSSPECAN_ATTR_AMPL_EATT_VALUE,
                                                         manualValue), 4, "Manual Value");                                                          
        break;
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Input Mixer Level
 * Purpose:     This function configures input mixer of the analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureInputMixerLevel (ViSession instrSession,
                                                ViInt32 input,
                                                ViBoolean automaticSetup,
                                                ViReal64 power)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (input, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Input");
 
    sprintf (buffer, "Win%ld", input);    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_AMPL_MIXER_AUTO,
                                                automaticSetup), 3, "Automatic Setup");
    if (automaticSetup == VI_FALSE)
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_AMPL_MIXER_LEVEL,
                                                power), 4, "Power");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}



/*****************************************************************************
 * Function:    Configure Preamplifier State
 * Purpose:     This function switches on the preamplifier for the instrument.
 *              The switchable gain is fixed to 20 dB.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePreamplifierState (ViSession instrSession,
                                                       ViInt32 window,
                                                       ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_AMPL_PREAMPLIFIER,
                                                state), 2, "State");
                                       
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/// HIFN This function configures level of pre-amplifier for selected sweep range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR preamplifierLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigurePreamplifierLevel (ViSession instrSession,
                                                                   ViInt32 preamplifierLevel)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                RSSPECAN_ATTR_AMPL_PREAMPLIFIER_LEVEL, preamplifierLevel), 3, "Preamplifier Level");                                                  
                                                          
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Create Limit Line
 * Purpose:  This function creates new limit or selects existing one and 
 *           configures basic parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CreateLimitLine(
    ViSession   instrSession,
    ViInt32 window,
    ViInt32 limit,
    ViString    name,
    ViInt32 domain,
    ViString    comment,
    ViInt32 assigntoTrace,
    ViBoolean deleteExistingLine
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      error_message[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    }
    
    if (strlen (name) > 8)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Name");
    }

    if (strlen (comment) > 40)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Comment");
    }

    if (rsspecan_invalidViBooleanRange (deleteExistingLine) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Delete Existing Line");
    }
    sprintf (buffer, "L%ld", limit);
    
    if (deleteExistingLine==VI_TRUE)
    {
        error = rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_DELETE,"");
    
        do
        {
            (void) rsspecan_error_query (instrSession, &error, error_message);
        } while (error != 0);
    }
    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_NAME, name), 4, "Name");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_CONTROL_DOMAIN,domain), 5, "Domain");
    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_COMMENT,comment), 6, "Comment");

    sprintf (buffer, "Win%ld,L%ld", window, limit);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_ASSIGN_TRACE, assigntoTrace), 7, "Assign to Trace");


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Limit Line
 * Purpose:  This function configures specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureLimitLine(
    ViSession   instrSession,
    ViInt32 limit,
    ViInt32 type,
    ViInt32 units,
    ViInt32 xAxisInterpolation,
    ViInt32 yAxisInterpolation,
    ViInt32 xAxisScaling,
    ViReal64    margin,
    ViReal64    threshold
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     scaling;
    ViChar      model[10] = "";
    ViInt32     tmp_units= 14;
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                   RS_ATTR_INSTRUMENT_MODEL, 10, model));
    if (strstr (model, "FSL") != NULL)
        tmp_units=15;
    if (rsspecan_IsFSV (instrSession))
        tmp_units=15;
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }
    
    if (rsspecan_invalidViInt32Range (type, 0, 1) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Type");
    }
    if (rsspecan_invalidViInt32Range (units, 0, tmp_units) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Units");
    }

    sprintf (buffer, "L%ld", limit);
    
    scaling = (units == RSSPECAN_VAL_UNIT_DB ) ? RSSPECAN_VAL_REL : RSSPECAN_VAL_ABS;
    
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_LIMIT_CONTROL_SPACING, xAxisInterpolation), 5, "X Axis Interpolation");
    viCheckParm (rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_LIMIT_CONTROL_SCALING, xAxisScaling), 7, "X Axis Scaling");

    switch (type)
    {
        case RSSPECAN_VAL_LIMIT_LOWER:
            viCheckParm (rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_LIMIT_LOWER_SPACING, yAxisInterpolation), 6, "Y Axis Interpolation");
            viCheckParm (rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_LIMIT_LOWER_MARGIN, margin), 8, "Marging");
            if (scaling == RSSPECAN_VAL_REL)
            {
                viCheckParm (rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_LIMIT_LOWER_THRESHOLD, threshold), 9, "Threshold");
            }   
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession,buffer,RSSPECAN_LIMIT_LOWER_SCALING, scaling));
            break;
            
        case RSSPECAN_VAL_LIMIT_UPPER:
            viCheckParm (rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_LIMIT_UPPER_SPACING, yAxisInterpolation), 6, "Y Axis Interpolation");
            viCheckParm (rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_LIMIT_UPPER_MARGIN, margin), 8, "Marging");
            if (scaling == RSSPECAN_VAL_REL)
            {
                viCheckParm (rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_LIMIT_UPPER_THRESHOLD, threshold), 9, "Threshold");
            }
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession,buffer,RSSPECAN_LIMIT_UPPER_SCALING, scaling));
            break;
    }

    viCheckParm (rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_LIMIT_UNITS, units), 4, "Units");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Define Limit Line
 * Purpose:  This function defines specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DefineLimitLine(
    ViSession   instrSession,
    ViInt32 limit,
    ViInt32 type,
    ViInt32 count,
    ViReal64    xAxis[],
    ViReal64    amplitude[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *pbuffer= VI_NULL;
    ViChar      *p2buf;
    ViInt32     i;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }
    if (rsspecan_invalidViInt32Range (type, 0, 1) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Type");
    }
    if (count < 1)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Count");
    }
    if (xAxis == NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "X Axis");
    }
    if (amplitude == NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Amplitude");
    }

    viCheckAlloc (pbuffer = (ViChar*) malloc ((size_t)(count * 20 + 200)));
    
    p2buf = pbuffer + sprintf (pbuffer, "CALC:LIM%ld:CONT ", limit);
    
    for (i = 0; i < count; i++)
        p2buf += sprintf (p2buf, "%le,", xAxis[i]);
    
    *p2buf = '\0';
    *--p2buf = '\n';
    
    viCheckErr (viWrite (instrSession, (ViBuf) pbuffer, (ViUInt32) strlen (pbuffer), VI_NULL));

    p2buf = pbuffer + sprintf (pbuffer, "CALC:LIM%ld:%s ", limit, limTypeArr [type]);
    
    for (i = 0; i < count; i++)
        p2buf += sprintf (p2buf, "%le,", amplitude[i]);
    
    *p2buf = '\0';
    *--p2buf = '\n';
    
    viCheckErr (viWrite (instrSession, (ViBuf) pbuffer, (ViUInt32) strlen (pbuffer), VI_NULL));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer) free (pbuffer);
    
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Select Limit Line
 * Purpose:  This function selects the limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectLimitLine (ViSession instrSession,
                                            ViInt32 window, 
                                            ViInt32 limit,
                                            ViString name,
                                            ViBoolean limitEnabled)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    }
    if (strlen (name) > 8)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Name");
    }
    
    sprintf (buffer, "L%ld", limit); 
    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_NAME, name), 4, "Name");
    sprintf (buffer, "Win%ld,L%ld", window, limit); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_LIMIT_STATE, limitEnabled), 5, "Limit Enabled");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Enable Limit Check
 * Purpose:  This function switches on or off the limit check and/or limit 
 *           line for the selected limit line in the selected measurement 
 *           window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_EnableLimitCheck(
    ViSession   instrSession,
    ViInt32 window,
    ViInt32 limit,
    ViInt32 type,
    ViBoolean   limitEnabled,
    ViBoolean   checkEnabled
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     attribute = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    }

    switch (type)
    {
        case RSSPECAN_VAL_LIMIT_LOWER:
            attribute = RSSPECAN_LIMIT_LOWER_STATE;
            break;
        case RSSPECAN_VAL_LIMIT_UPPER:
            attribute = RSSPECAN_LIMIT_UPPER_STATE;
            break;
    
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Type");
    }
    
    sprintf (buffer, "Win%ld,L%ld", window, limit);

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_LIMIT_STATE, limitEnabled), 5, "Limit Enabled");
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, (ViUInt32) attribute, checkEnabled), 6, "Check Enabled");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Move Limit Line
 * Purpose:  This function moves specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MoveLimitLine(
    ViSession   instrSession,
    ViInt32 limit,
    ViInt32 type,
    ViInt32 method,
    ViReal64    value
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     attributes[3][2] = {{RSSPECAN_LIMIT_LOWER_OFFSET,RSSPECAN_LIMIT_LOWER_SHIFT},
                                   {RSSPECAN_LIMIT_UPPER_OFFSET,RSSPECAN_LIMIT_UPPER_SHIFT},
                                   {RSSPECAN_LIMIT_CONTROL_OFFSET,RSSPECAN_LIMIT_CONTROL_SHIFT}};

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }

    if (rsspecan_invalidViInt32Range (type, 0, 2) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Type");
    }

    if (rsspecan_invalidViInt32Range (method, 0, 1) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Method");
    }

    sprintf (buffer, "L%ld", limit);

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, (ViUInt32)attributes[type][method], value), 5, "Value");


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Copy Limit Line
 * Purpose:  This function copies specified limit line into another one.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CopyLimitLine(
    ViSession   instrSession,
    ViInt32 limit,
    ViInt32 copyTo
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }

    sprintf (buffer, "L%ld", limit);

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_COPY, copyTo), 3, "Copy To");


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Delete Limit Line
 * Purpose:  This function removes specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteLimitLine(
    ViSession   instrSession,
    ViInt32 limit
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }

    sprintf (buffer, "L%ld", limit);

    viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_DELETE, VI_NULL));


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/*****************************************************************************
 * Function: Clear Limit Line Results
 * Purpose:  This function deletes the result of the current limit check.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ClearLimitLineResults (ViSession instrSession,
                                                  ViInt32 window, 
                                                  ViInt32 limit)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Limit Line Number");
    
    sprintf(lim_line_no,"Win%ld,L%ld",window, limit);

    viCheckErr(rsspecan_SetAttributeViString (instrSession, lim_line_no,
                                               RSSPECAN_LIMIT_CLEAR, VI_NULL));


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Line State
 * Purpose:  This function configures state of specified display line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLineState (ViSession instrSession,ViInt32 window,ViInt32 line,ViInt32 type,ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    switch (type){
        case RSSPECAN_VAL_LINE_THR:
            sprintf (buffer, "Win%ld", window);    
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_THRLINE_STATE,
                                                         state), 5, "State"); 
        break;
        case RSSPECAN_VAL_LINE_DISPLAY:
            if (rsspecan_invalidViInt32Range (line, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Line");
            sprintf (buffer, "Win%ld,L%ld", window, line);
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_DLINE_STATE,
                                                         state), 5, "State"); 
        break;
        case RSSPECAN_VAL_LINE_FREQ:
            if (rsspecan_invalidViInt32Range (line, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Line");
            sprintf (buffer, "Win%ld,L%ld", window, line);
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_FLINE_STATE,
                                                         state), 5, "State"); 
        break;
        case RSSPECAN_VAL_LINE_TIME:
            if (rsspecan_invalidViInt32Range (line, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Line");
            sprintf (buffer, "Win%ld,L%ld", window, line);
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                         RSSPECAN_ATTR_TLINE_STATE,
                                                         state), 5, "State"); 
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Type");     
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Line Position
 * Purpose:  This function configures position of specified display line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLinePosition (ViSession instrSession,
                                                 ViInt32 window, 
                                                 ViInt32 line,
                                                 ViInt32 type, 
                                                 ViReal64 position)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    switch (type){
        case RSSPECAN_VAL_LINE_THR:
            sprintf (buffer, "Win%ld", window);    
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                         RSSPECAN_ATTR_THRLINE_POSITION,
                                                         position), 5, "Position");                                                           
        break;
        case RSSPECAN_VAL_LINE_DISPLAY:
            if (rsspecan_invalidViInt32Range (line, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Line");
            sprintf (buffer, "Win%ld,L%ld", window, line);
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                         RSSPECAN_ATTR_DLINE_POSITION,
                                                         position), 5, "Position");                                                           
        break;
        case RSSPECAN_VAL_LINE_FREQ:
            if (rsspecan_invalidViInt32Range (line, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Line");
            sprintf (buffer, "Win%ld,L%ld", window, line);
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                         RSSPECAN_ATTR_FLINE_POSITION,
                                                         position), 5, "Position");                                                              
        break;
        case RSSPECAN_VAL_LINE_TIME:
            if (rsspecan_invalidViInt32Range (line, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Line");
            sprintf (buffer, "Win%ld,L%ld", window, line);
             viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                         RSSPECAN_ATTR_TLINE_POSITION,
                                                         position), 5, "Position");                                                          
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Type");     
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Active Limit Lines
 * Purpose:  This function queries the names of all active limit lines.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetActiveLimitLines (ViSession instrSession,
                                                ViInt32 stringSize,
                                                ViChar activeLimitLines[],
                                                ViInt32 *returnedStringLength)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *pbuffer,
                *pbuf;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    ViInt32    retCnt;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));

    if (strstr (model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (!activeLimitLines)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Active Limit Lines");
    
    viCheckErr( viPrintf (instrSession, ":CALC:LIM:ACT?\n")); 
    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &pbuffer, VI_NULL)); 
    retCnt = (ViUInt32) (strrchr (pbuffer, '\'') - pbuffer);
    if (returnedStringLength) 
    {
        *returnedStringLength = retCnt;
    }   
    
    retCnt=(stringSize>retCnt)?retCnt:stringSize;
    pbuf=pbuffer;
    pbuf++;
    strncpy(activeLimitLines, pbuf, retCnt-1);
    activeLimitLines[retCnt-1]='\0';
    
    if (pbuffer) free(pbuffer);
    
    checkErr( rsspecan_CheckStatus (instrSession)); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Limit Line Catalog
 * Purpose:  This function reads out the names of all limit lines 
 *           stored on the harddisk.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetLimitLineCatalog (ViSession instrSession,
                                                ViInt32 *number_ofLimitLines,
                                                ViInt32 bufferSize,
                                                ViChar limitLinesList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (strstr (model, "FSL") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (limitLinesList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Limit Lines List");  
       
    viCheckErr (viPrintf (instrSession, "CALC:LIM:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseLimitLineCatalog(buf, bufferSize, limitLinesList, number_ofLimitLines));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}

/*===========================================================================*
 *= RsSpecAn Display Control Group Functions ================================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Display Set Active Window
 * Purpose:  This function selects the active measurement window.
 *           In FULL SCREEN mode, the measurements are only performed in the
 *           active measurement window. Measurements are therefore initiated
 *           in the active window and result queries (marker, trace data and
 *           other results) answered also in the active window.
 *           Initiating measurements and querying results in the inactive
 *           window yields an error message (execution error).
 *           In split screen mode, the selection of the active window for
 *           result queries is irrelevant.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisplaySetActiveWindow (ViSession instrSession,
                                                   ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);      
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_SELECT_WINDOW,
                                                VI_NULL));                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Display get Active Window
 * Purpose:  This function returns the active measurement window.
 *****************************************************************************/

ViStatus _VI_FUNC rsspecan_DisplayGetActiveWindow (ViSession instrSession,
                                                   ViInt32 *window)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (window == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Window");
    }
    viCheckErr( rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_QUERY_ACTIVE_WINDOW,
                                                window));                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Get Selected Subwindow
 * Purpose:  This function queries the currently selected subwindow.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSelectedSubwindow (ViSession instrSession,
                                                 ViInt32 *subwindow)
{
    ViStatus    error = VI_SUCCESS;
    
     checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (subwindow == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Subwindow");
    }
    viCheckErr( rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_QUERY_SELECT_SUBWINDOW,
                                                subwindow));                                                

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Display Focused Area
 * Purpose:  This function moves the focus area to the selected window and
 *           subwindow.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayFocusedArea (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 subwindow)
{
    ViStatus    error = VI_SUCCESS; 
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (subwindow, 1, 2) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Subwindow");
    }
    sprintf (buffer, "Win%ld,SW%ld", window,subwindow);      
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_DISPLAY_FOCUS_AREA,
                                                VI_NULL));                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Display Control
 * Purpose:  This function scontrols the apperance of display elements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayControl (ViSession instrSession,
                                                    ViBoolean frequency,
                                                    ViBoolean logo,
                                                    ViBoolean time)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));

    viCheckParm ( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_FREQ_STATE,
                                                frequency), 2, "Frequency");                                                
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                    RSSPECAN_ATTR_DISP_TIME_STATE,
                                                    time), 4, "Time");
    if (strstr(model, "FSL"))
    {
        if (time == VI_FALSE)
        {
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_LOGO_STATE,
                                                logo), 3, "Logo");  
        }       
    }
    else
    {
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_LOGO_STATE,
                                                logo), 3, "Logo");      
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Display Comment
 * Purpose:  This function controls the apperance of display title.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayComment (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean state,
                                                    ViString comment)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);      
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_DISP_COMMENT_STATE,
                                                state), 2, "State"); 
    viCheckParm( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_DISP_COMMENT,
                                                comment), 3, "Comment");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}




/*****************************************************************************
 * Function: Configure Display Comment
 * Purpose:  This function controls the apperance of display title.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayPowerSave (ViSession instrSession,
                                                      ViBoolean state,
                                                      ViInt32 holdoff)
{
    ViStatus    error = VI_SUCCESS;
  
    checkErr( Rs_LockSession (instrSession, VI_NULL));
  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_PWR_SAVE_STATE,
                                                state), 2, "State"); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_PWR_SAVE_HOLDOFF,
                                                holdoff), 3, "Hold Off");                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Display Format
 * Purpose:  This function switches the measurement result display between 
 *           FULL SCREEN and SPLIT SCREEN.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayFormat (ViSession instrSession,
                                                   ViInt32 format)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_FORMAT,
                                                format), 2, "Format"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Display Window Size
 * Purpose:  This function switches the measurement window for channel and 
 *           adjacent-channel power measurements to full screen or half screen.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayWindowSize (ViSession instrSession,
                                                       ViInt32 windowSize)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
  
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_DISP_WINDOW_SIZE,
                                                windowSize), 2, "Window Size"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Display Log Range
 * Purpose:  This function defines the display range of the Y axis (level axis)
 *           in the selected measurement window with logarithmic scaling
 *           (DISP:TRAC:Y:SPAC LOG). For linear scaling, 
 *           (DISP:TRAC:Y:SPAC LIN | PERC) the display range is fixed and 
 *           cannot be modified.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLogRange (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViReal64 range)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_DISP_LOG_RANGE, 
                range), 3, "Range");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Amplitude Grid Mode
 * Purpose:  This function defines the scale type of the Y axis 
 *           (absolute or relative) in the selected measurement window.
 *           When SYSTem:DISPlay is set to OFF, this command has no immediate
 *           effect on the screen.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayAmplitudeGridMode (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViInt32 yAxisGridMode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, yAxisGridMode), 
                3, "Range");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Reference Value
 * Purpose:  This function defines the power value assigned to the reference 
 *           position in the selected measurement window. This value corresponds 
 *           to the parameter REFERENCE VALUE in manual operation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayReferenceValue (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViReal64 referenceValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                RSSPECAN_ATTR_DISP_REF_VALUE, referenceValue), 
                3, "Reference Position");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Reference Position
 * Purpose:  This function defines the position of the reference value in 
 *           the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayReferencePosition (ViSession instrSession,
                                                                   ViInt32 window,
                                                                   ViReal64 referencePosition)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                RSSPECAN_ATTR_DISP_REF_POSITION, referencePosition), 
                3, "Reference Value Position");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Single Sweep
 * Purpose:  This function configures the behaviour of the display during a 
 *           single sweep.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySingleSweep (ViSession instrSession,
                                                        ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                RSSPECAN_ATTR_DISP_SINGLE_SWEEP, state), 
                2, "State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Transducer State
 * Purpose:  This function switches on the display of the active transducer 
 *           factor or set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayTransducerState (ViSession instrSession,
                                                             ViInt32 window,
                                                             ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_TFAC_DISPLAY, state), 
                3, "State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Update
 * Purpose:  This function switches on or off the update of all display 
 *           elements during remote control.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayUpdate (ViSession instrSession,
                                                   ViBoolean displayInRemote)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                RSSPECAN_ATTR_DISP_UPDATE, displayInRemote), 
                2, "Display In Remote");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Soft Frontpanel
 * Purpose:  This function activates or deactivates the display of the front 
 *           panel keys on the screen. With the display activated, 
 *           the instrument can be operated on the screen using the mouse 
 *           by pressing the corresponding buttons. This may be useful if 
 *           the instrument is operated in a detached station by means of a 
 *           remote program such as PCANYWHERE.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySoftFrontpanel (ViSession instrSession,
                                                           ViBoolean softFrontpanel)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", 
                RSSPECAN_ATTR_DISP_FP_KEYS, softFrontpanel), 
                2, "Soft Front Panel");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Autoscale Once
 * Purpose:  This function adapts the diagram limits to the current measurement 
 *           results. The value range depends on the selected display.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayAutoscaleOnce (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_DISP_TRACE_Y_AUTO, NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Display Y-Axis Scaling
 * Purpose:     This function defines the scaling of the y-axis in 
 *              the currently selected unit.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayYAxisScaling (ViSession instrSession,
                                                         ViInt32 window, ViReal64 scaling)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_DISP_Y_AXIS_SCALING,
                                              scaling), 3,"Scaling");
                                               
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Settings
 * Purpose:  This function configures the display settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySettings (ViSession instrSession,
                                                     ViBoolean statusBarState,
                                                     ViBoolean softkeyBarState,
                                                     ViBoolean toolBarState,
                                                     ViInt32 touchscreenState)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_DISP_STATUS_BAR,
                                              statusBarState), 2, "Status Bar State");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_DISP_SOFTKEY_BAR,
                                              softkeyBarState), 3, "Softkey Bar State");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_DISP_TOOL_BAR,
                                              toolBarState), 4, "Tool Bar State");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_DISP_TOUCHSCREEN_STATE,
                                              touchscreenState), 5, "Touchscreen State");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Display Theme Catalog
 * Purpose:  This function displays the selection list of available themes for 
 *           the screen display.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetDisplayThemeCatalog (ViSession instrSession,
                                                   ViInt32 *number_ofThemes,
                                                   ViInt32 bufferSize,
                                                   ViChar themeList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (!rsspecan_IsFSV (instrSession))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (themeList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Theme List");  
       
    viCheckErr (viPrintf (instrSession, "DISP:THEM:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, themeList, number_ofThemes));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Theme Select
 * Purpose:  This function selects a theme for the screen display. The theme 
 *           defines the colors used for keys and screen elements, for example. 
 *           Use command DISPlay:THEMe:CATalog? to get a list of possible themes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayThemeSelect (ViSession instrSession,
                                                        ViString theme)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_DISP_SELECT_THEME, theme), 2, "Theme");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Marker Table
 * Purpose:  This function toggles the display of the marker table. With 
 *           automatic display, the table is displayed if 2 or more markers 
 *           are active.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMarkerTable (ViSession instrSession,
                                                        ViInt32 markerTable)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                RSSPECAN_ATTR_DISP_MARKER_TABLE, markerTable), 2, "Marker Table");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Zoom
 * Purpose:  This function configures the display zooming parameters.
 *           
 *           Note:
 *           (1) This function is available for R&S FSV instruments only.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayZoom (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean zoomState, ViReal64 x1,
                                                 ViReal64 y1, ViReal64 x2,
                                                 ViReal64 y2)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";     
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    if (!rsspecan_IsFSV (instrSession))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);   
    
    if (rsspecan_invalidViReal64Range (x1, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "x1");
    if (rsspecan_invalidViReal64Range (y1, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "y1"); 
    if (rsspecan_invalidViReal64Range (x2, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "x2");
    if (rsspecan_invalidViReal64Range (y2, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "y2");  
       
    sprintf (buffer, "Win%ld", window);  

    viCheckParm ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_DISPLAY_ZOOM_STATE, zoomState),
                3, "Zoom State");
    
    if (zoomState == VI_TRUE) {
        
        viCheckErr( viPrintf (instrSession, "DISP:WIND%ld:ZOOM:AREA %.12lG,%.12lG,%.12lG,%.12lG\n", window, x1, y1, x2, y2));
        
        checkErr( rsspecan_CheckStatus (instrSession)); 
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
  

/// HIFN This function configures the multiple display zooming parameters.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR window/Selects the measurement window.
/// HIPAR zoomWindow/Selects the zoom area.
/// HIPAR enableZoom/Turns the multiple zoom on and off
/// HIPAR x1/X coordinate of lower left corner.
/// HIPAR y1/Y coordinate of lower left corner.
/// HIPAR x2/X coordinate of upper right corner.
/// HIPAR y2/Y coordinate of upper right corner.
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMultipleZoom (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 zoomWindow,
                                                         ViBoolean enableZoom,
                                                         ViReal64 x1,
                                                         ViReal64 y1,
                                                         ViReal64 x2,
                                                         ViReal64 y2)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";     
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    if (!rsspecan_IsFSV (instrSession))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);   
    
    if (rsspecan_invalidViInt32Range (zoomWindow, 1, 4) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Zoom Window");
    if (rsspecan_invalidViReal64Range (x1, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "x1");
    if (rsspecan_invalidViReal64Range (y1, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "y1"); 
    if (rsspecan_invalidViReal64Range (x2, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "x2");
    if (rsspecan_invalidViReal64Range (y2, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "y2");  
       
    sprintf (buffer, "Win%ld,ZWin%ld", window, zoomWindow);  

    viCheckParm ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_DISPLAY_MULTIPLE_ZOOM_STATE, enableZoom),
                4, "Enable Zoom");
    
    if (enableZoom == VI_TRUE) {
        
        viCheckErr( viPrintf (instrSession, "DISP:WIND%ld:ZOOM:MULT%ld:AREA %.12lG,%.12lG,%.12lG,%.12lG\n", window, zoomWindow, x1, y1, x2, y2));
        
        checkErr( rsspecan_CheckStatus (instrSession)); 
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function turns a measurement window on or off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR state/Turns a measurement window on or off.
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMeasurementWindowState (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_DISP_MEAS_WINDOW_STATE,
                                                state), 3, "State");

Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the currently active measurement window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control returns the index number of the currently active measurement window.
ViStatus _VI_FUNC rsspecan_QueryActiveMeasurementWindow (ViSession instrSession,
                                                ViInt32 *window)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_QUERY_ACTIVE_MEASUREMENT_WINDOW,
                                                window), 2, "Window"); 
Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Display Color Default
 * Purpose:  This function resets the screen colors of all display items to 
 *           their default settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisplayColorDefault (ViSession instrSession,
                                                   ViInt32 defaultSetting)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (defaultSetting, 1, 2) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Default Settings");
    }
    sprintf (buffer, "DEF%ld", defaultSetting);  
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_DISP_COL_PRESET, VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Color
 * Purpose:  This function defines the color table of the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayColor (ViSession instrSession,
                                                  ViInt32 colorMap,
                                                  ViReal64 tint,
                                                  ViReal64 saturation,
                                                  ViReal64 brightness)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (colorMap, 1, 26) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Color Map");
    if (rsspecan_invalidViReal64Range (tint, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "TINT");
    if (rsspecan_invalidViReal64Range (saturation, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "SATURATION"); 
    if (rsspecan_invalidViReal64Range (brightness, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "BRIGHTNESS");        
    
    viCheckErr( viPrintf (instrSession, "DISP:CMAP%ld:HSL %.12f,%.12f,%.12f\n",
                colorMap, tint/100.0, saturation/100.0, brightness/100.0));

    checkErr( rsspecan_CheckStatus (instrSession)); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Display Color by Predefined
 * Purpose:  This function defines the color table of the instrument using 
 *           predefined colors.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayColorByPredefined (ViSession instrSession,
                                                              ViInt32 colorMap,
                                                              ViInt32 predefinedColors)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (colorMap, 1, 26) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Color Map");
    }
    sprintf (buffer, "CM%ld", colorMap);  
    
    viCheckParm ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_DISP_COL_PREDEFINED, predefinedColors),
                3, "Predefined Colors");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function adds a window to the display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR stringSize/This control sets the allocated length of Returned Window Name string.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
/// HIPAR direction/Direction the window is added relative to the window set with "Window Name".
/// HIPAR windowType/Type of result display you want to add.
/// HIPAR returnedWindowName/Returns the name of the new window.
ViStatus _VI_FUNC rsspecan_AddWindow (ViSession instrSession, ViInt32 stringSize,
                                      ViString windowName, ViInt32 direction,
                                      ViInt32 windowType,
                                      ViChar returnedWindowName[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [RSSPECAN_IO_BUFFER_SIZE] = "";
    size_t      length;
    ViChar*     pbuffer = NULL;
    ViChar*     p2buf = NULL;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (direction, RSSPECAN_VAL_LAYOUT_DIRECTION_LEFT, RSSPECAN_VAL_LAYOUT_DIRECTION_BELOW) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Direction");
    if (rsspecan_invalidViInt32Range (windowType, RSSPECAN_VAL_LAYOUT_TYPE_DIAGRAM, RSSPECAN_VAL_LAYOUT_TYPE_PSP) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Window Type");  
        
    sprintf (cmd, "LAY:ADD? '%s',%s,%s", windowName, directionArr[direction], windowTypeArr[windowType]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, NULL));
    
    if (returnedWindowName != NULL)
    {
        size_t len = strstr (pbuffer+1,"'") - pbuffer - 1;
        length = (size_t) stringSize - 1 <= len ? (size_t) stringSize-1 : len;
        p2buf = strtok (pbuffer, "'");
        strncpy (returnedWindowName, p2buf, length);
    }
 
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer != NULL) free (pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN This function removes a window from the display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
ViStatus _VI_FUNC rsspecan_RemoveWindow (ViSession instrSession,
                                         ViString windowName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    
    viCheckParm ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_LAYOUT_WINDOW_REMOVE, windowName),
                2, "Window Name");                

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function replaces a window with another one.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
/// HIPAR windowType/Type of result display you want to add.
ViStatus _VI_FUNC rsspecan_ReplaceWindow (ViSession instrSession,
                                          ViString windowName,
                                          ViInt32 windowType)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (windowType, RSSPECAN_VAL_LAYOUT_TYPE_DIAGRAM, RSSPECAN_VAL_LAYOUT_TYPE_PSP) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Window Type");  
        
    sprintf (cmd, "LAY:REPL '%s',%s", windowName, windowTypeArr[windowType]);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the name and index of all active windows.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR bufferSize/This control specifies the size of the buffer.
/// HIPAR results/Returns the name and index of all active windows.
ViStatus _VI_FUNC rsspecan_QueryWindowsList (ViSession instrSession,
                                             ViInt32 bufferSize,
                                             ViChar results[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckParm (rsspecan_GetAttributeViString (instrSession, "", RSSPECAN_ATTR_LAYOUT_WINDOW_LIST, 
                                                                bufferSize, results), 3, "Results");                


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function queries the index of a particular display window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
/// HIPAR index/Index number of the window.
ViStatus _VI_FUNC rsspecan_QueryWindowIndex (ViSession instrSession,
                                             ViString windowName, ViInt32 *index)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [100] = "";
    ViChar      *buffer = VI_NULL;
    ViChar      *pbuff = VI_NULL; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (cmd, "LAY:IDEN? '%s'", windowName);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &buffer, NULL));
    
    pbuff = strtok (buffer, ",\n");
    *index = atol(pbuff);
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function queries the window type of the window specified by the 
/// HIFN  index <n>
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR index/Index number of the window.
/// HIPAR windowType/Type of result display you want to add.
ViStatus _VI_FUNC rsspecan_QueryWindowType(
    ViSession   instrSession,
    ViInt32 index,
    ViInt32*    windowType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar  rep_cap[10] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    sprintf (rep_cap, "Win%ld", index);  

    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_LAYOUT_QUERY_WINDOW_TYPE,
                                               windowType), 3, "Window Type");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN This function queries the name of a particular display window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR stringSize/This control sets the allocated length of Returned Window Name string.
/// HIPAR index/Index number of the window.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to. 
ViStatus _VI_FUNC rsspecan_QueryWindowName (ViSession instrSession, ViInt32 stringSize,
                                            ViInt32 index, ViChar windowName[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", index); 
    viCheckParm (rsspecan_GetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_LAYOUT_QUERY_WINDOW_NAME, 
                                                                stringSize, windowName), 3, "Window Name");
    
                                               
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function changes the position of a splitter
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR firstWindowName/String containing the number or name of the first window the splitter controls.
/// HIPAR secondWindowName/String containing the number or name of the second window the splitter controls. 
/// HIPAR position/New vertical or horizontal position of the splitter in percent of the screen area.
ViStatus _VI_FUNC rsspecan_ConfigureSplitterPosition (ViSession instrSession,
                                                      ViString firstWindowName,
                                                      ViString secondWindowName,
                                                      ViInt32 position)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [100] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
        
    sprintf (cmd, "LAY:SPL '%s','%s',%ld", firstWindowName, secondWindowName, position);
    
    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Select Transducer Factor
 * Purpose:  This function generates a transducer factor <name> using 
 *           normalized trace data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectTransducerFactor (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViBoolean transducerState,
                                                   ViString transducerName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_TFAC_SEL_NAME, transducerName),
                4, "Transducer Name");                

    viCheckParm ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_TFAC_STATE, transducerState),
                3, "Transducer State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Select Transducer Factor
 * Purpose:  This function generates a transducer factor <name> using 
 *           normalized trace data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTransducerFactor (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViString name,
                                                      ViInt32 unit,
                                                      ViInt32 interpolation,
                                                      ViString comment,
                                                      ViInt32 noofTestPoints,
                                                      ViReal64 frequencyData[],
                                                      ViReal64 levelData[])

{
    ViStatus    error = VI_SUCCESS;
    ViChar      rep_cap[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *pbuf = NULL;
    ViChar      *p2buf = NULL;
    ViInt32     i;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (rep_cap, "Win%ld", window);  
    
    viCheckParm ( rsspecan_SetAttributeViString (instrSession, rep_cap, 
                RSSPECAN_ATTR_TFAC_SEL_NAME, name),
                3, "Name");
    viCheckParm ( rsspecan_SetAttributeViInt32 (instrSession, rep_cap, 
                RSSPECAN_ATTR_TFAC_UNIT, unit),
                4, "Unit");                
    viCheckParm ( rsspecan_SetAttributeViInt32 (instrSession, rep_cap, 
                RSSPECAN_ATTR_TFAC_SCALING, interpolation),
                5, "Interpolation");    
    viCheckParm ( rsspecan_SetAttributeViString (instrSession, rep_cap, 
                RSSPECAN_ATTR_TFAC_COMMENT, comment),
                6, "Comment");                    
                    
    if (noofTestPoints < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "No Of Test Points");
    
    viCheckAlloc (pbuf = (ViChar*) malloc((size_t)(23 + 2 * noofTestPoints * 15 + 300)));
    
    p2buf = pbuf;
    
    if (noofTestPoints > 0)
    {
        pbuf += sprintf (pbuf, "SENS%ld:CORR:TRAN:DATA ", window);
        for (i=0;i< noofTestPoints;i++)
        {
            pbuf += sprintf (pbuf, "%le,%lf,", frequencyData[i], levelData[i]); 
        }
        *--pbuf = '\0';    
        viCheckErr (viPrintf (instrSession, "%s\n", p2buf));
    }     
    
    checkErr( rsspecan_CheckStatus (instrSession)); 

Error:
    if (p2buf) free (p2buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Transducer Factor Ref Level Adj
 * Purpose:  This function allows automatic reference level offset adaption 
 *           to restore the original dynamic range by also shifting the reference 
 *           level by the maximum value of the transducer factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTransducerFactorRefLevAdj (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViBoolean refLevAdj)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_TFAC_ADJ_STATE, refLevAdj),
                3, "Ref Lev Adj");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Transducer Factor Catalog
 * Purpose:  This function reads out the names of all transducer factors 
 *           stored on the harddisk.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTransducerFactorCatalog (ViSession instrSession,
                                                       ViInt32 *numberofTransducerFactors,
                                                       ViInt32 bufferSize,
                                                       ViChar transducerFactorsList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if ((strstr (model, "FSL") != NULL) || (strstr (model, "FMU") != NULL) ||
        (rsspecan_IsFSV (instrSession)))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (transducerFactorsList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Transducer Factor List");  
       
    viCheckErr (viPrintf (instrSession, ":CORR:TRAN:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, transducerFactorsList, numberofTransducerFactors));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}


/*****************************************************************************
 * Function: Delete Transducer Factor
 * Purpose:  This function deletes the selected transducer factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteTransducerFactor (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViString transducerName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_TFAC_SEL_NAME, transducerName),
                3, "Transducer Name");                
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_TFAC_DELETE, VI_NULL)); 
                
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Service Version Info
 * Purpose:  This function queries the instrument firmware and option versions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceVersionInfo (ViSession instrSession,
                                               ViInt32 arraySize,
                                               ViChar versionInfo[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *pbuffer=VI_NULL;
    ViUInt32    ret_val;
    ViUInt32    length;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (viPrintf (instrSession, "DIAG:SERV:VERS?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_val));
    length=((ViUInt32)arraySize<=ret_val)?arraySize-1:(ViUInt32)ret_val;
    memcpy (versionInfo,pbuffer,length);
    versionInfo[length]='\0';
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer) free(pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: File Decimal Separator
 * Purpose:  This function configures the decimal separator 
 *           (decimal point or comma) for floating-point numerals contained 
 *           in the file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileDecimalSeparator (ViSession instrSession,
                                           ViInt32 decimalSeparator)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_FILE_DEC_SEPARATOR, 
                 decimalSeparator), 2, "Decimal Separator");
        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Transducer Active
 * Purpose:  This function queries the active transducer factor. Any empty 
 *           string is returned, if no transducer factor is active.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTransducerActive (ViSession instrSession,
                                                ViChar activeTransducer[])
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm (rsspecan_GetAttributeViString (instrSession, "Win0", RSSPECAN_ATTR_TFAC_ACT_NAME, 512,
                 activeTransducer), 2, "Active Transducer");
        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function selects a transducer set and turns that transducer set on or off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR transducerSetState/This control sets the name of the transducer set. If the name
/// HIPAR transducerSetState/does not exist yet, the instrument creates a transducer set by
/// HIPAR transducerSetState/that name.
/// HIPAR transducerSetName/This control switches the selected transducer set on or off.
ViStatus _VI_FUNC rsspecan_SelectTransducerSet (ViSession instrSession,
                                                ViString transducerSetName,
                                                ViBoolean transducerSetState)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_TSET_SELECT, transducerSetName),
                2, "Transducer Set Name");                

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
                RSSPECAN_ATTR_TSET_STATE, transducerSetState),
                3, "Transducer Set State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function selects a transducer set and turns that transducer set on or off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR transducerSetName/This control sets the name of the transducer set. If the name
/// HIPAR transducerSetName/does not exist yet, the R&S  ESR creates a transducer set by
/// HIPAR transducerSetName/that name.
/// HIPAR comment/Defines a comment for a transducer set.
/// HIPAR unit/This control selects the unit of a transducer set.
/// HIPAR transducerBreak/This control turns a transducer break on and off.
/// HIPAR range/Selects the range.
/// HIPAR startFrequency/Defines the start frequency of the frequency range.
/// HIPAR stopFrequency/Defines the stop frequency of the frequency range.
/// HIPAR factors/String containing the name of the transducer factor.
ViStatus _VI_FUNC rsspecan_ConfigureTransducerSet (ViSession instrSession,
                                                   ViString transducerSetName,
                                                   ViString comment,
                                                   ViInt32 unit,
                                                   ViBoolean transducerBreak,
                                                   ViInt32 range,
                                                   ViReal64 startFrequency,
                                                   ViReal64 stopFrequency,
                                                   ViString factors)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (range, 1, 10) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Range");
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_TSET_SELECT, transducerSetName),
                2, "Transducer Set Name");
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_TSET_COMMENT, comment),
                3, "Comment");                    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                RSSPECAN_ATTR_TSET_UNIT, unit),
                4, "Unit");                
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
                RSSPECAN_ATTR_TSET_BREAK, transducerBreak),
                5, "Transducer Break");    
                    
    viCheckErr (viPrintf (instrSession, "SENS:CORR:TSET:RANG%ld %.12lG,%.12lG,%s\n", range,
                                                startFrequency, stopFrequency, factors));
    
    checkErr (rsspecan_CheckStatus (instrSession)); 
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function reads out the names of all transducer sets stored
/// HIFN  on the harddisk.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR numberOfTransducerSets/This control returns number of names of all transducer sets.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the catalog 
/// HIPAR bufferSize/list parameter.
/// HIPAR transducerSetsList/This control returns the names of all transducer sets stored on the 
/// HIPAR transducerSetsList/hard disk.
ViStatus _VI_FUNC rsspecan_QueryTransducerSetCatalog (ViSession instrSession,
                                                      ViInt32 *numberOfTransducerSets,
                                                      ViInt32 bufferSize,
                                                      ViChar transducerSetsList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;

    checkErr (Rs_LockSession (instrSession, VI_NULL)); 
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (transducerSetsList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Transducer Set List");  
       
    viCheckErr (viPrintf (instrSession, ":CORR:TSET:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, transducerSetsList, numberOfTransducerSets));

    checkErr (rsspecan_CheckStatus (instrSession));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}

/// HIFN  This function deletes a transducer set.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR transducerSetName/This control selects the transducer set name.
ViStatus _VI_FUNC rsspecan_DeleteTransducerSet (ViSession instrSession,
                                                ViString transducerSetName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_TSET_SELECT, transducerSetName),
                2, "Transducer Set Name");

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_TSET_DELETE, VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function queries the active transducer set. Any empty
/// HIFN  string is returned, if no transducer set is active.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR activeTransducerSet/This control the active transducer set. Any empty string is
/// HIPAR activeTransducerSet/returned, if no transducer set is active.
ViStatus _VI_FUNC rsspecan_QueryActiveTransducerSet (ViSession instrSession,
                                                     ViChar activeTransducerSet[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm (rsspecan_GetAttributeViString (instrSession, "", RSSPECAN_ATTR_TSET_ACTIVE, 512,
                 activeTransducerSet), 2, "Active Transducer Set");
        
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Hardcopy Device Setup
 * Purpose:  This function selects the printer output medium and the data 
 *           format of the printout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyDevice (ViSession instrSession,
                                                    ViInt32 device,
                                                    ViInt32 destination,
                                                    ViInt32 pageOrientation)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
     
    if (rsspecan_invalidViInt32Range (device, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Device");
    if (rsspecan_invalidViInt32Range (destination, 0, 6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Destination");
        
    sprintf (buffer, "DE%ld", device);
    switch(destination){
        case  RSSPECAN_VAL_HCOPY_DEST_BMP:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                                  RSSPECAN_VAL_HCOPY_DEVICE_MEM));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, 
                                                    RSSPECAN_VAL_HCOPY_DEVICE_LANG_BMP));                
        break;
        case  RSSPECAN_VAL_HCOPY_DEST_WMF:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                                  RSSPECAN_VAL_HCOPY_DEVICE_MEM));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_LANG_WMF));
        break;
        case  RSSPECAN_VAL_HCOPY_DEST_EMF:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                              RSSPECAN_VAL_HCOPY_DEVICE_MEM));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_LANG_EWMF));
        break;
        case  RSSPECAN_VAL_HCOPY_DEST_CLP:
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                                 RSSPECAN_VAL_HCOPY_DEVICE_CLP));
        break;
        case  RSSPECAN_VAL_HCOPY_DEST_PRN:
            
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_LANG_GDI));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                                  RSSPECAN_VAL_HCOPY_DEVICE_PRN));
            viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                  RSSPECAN_ATTR_HCOPY_DEVICE_ORIENTATION,
                                                  pageOrientation), 4, "Page Orientation");
        break;
        case  RSSPECAN_VAL_HCOPY_DEST_JPG:
                 viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_MEM));
                 viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_LANG_JPG));
        break;
        case  RSSPECAN_VAL_HCOPY_DEST_PNG:
                viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,  RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_MEM));
            viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                      RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM,
                                                      RSSPECAN_VAL_HCOPY_DEVICE_LANG_PNG));
        break;
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Abort
 * Purpose:  This function aborts a running hardcopy output.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyAbort (ViSession instrSession)
{
    
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_HCOPY_ABORT, VI_NULL));                
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Print
 * Purpose:  This function starts a hardcopy output of the selected items.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyPrint (ViSession instrSession,
                                          ViInt32 device, 
                                          ViInt32 items)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (device, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Device");
    if (rsspecan_invalidViInt32Range (items, 0, 4) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Items");
        
    sprintf (buffer, "DE%ld", device);
    switch (items){
        case RSSPECAN_VAL_HCOP_ALL:
            viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_HCOPY_PRINT_SCREEN, VI_NULL));    
        break;
        case RSSPECAN_VAL_HCOP_TABLE:
            viCheckErr ( rsspecan_SetAttributeViBoolean (instrSession, "Win0", 
                RSSPECAN_ATTR_HCOPY_PRINT_TAB, VI_TRUE));
        break;
        case RSSPECAN_VAL_HCOP_TRACE:
            viCheckErr ( rsspecan_SetAttributeViBoolean (instrSession, "Win0", 
                RSSPECAN_ATTR_HCOPY_PRINT_TRACE, VI_TRUE));
        break;
    }
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, buffer, 
                RSSPECAN_ATTR_HCOPY_PRINT, VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Print Next
 * Purpose:  This function starts a hardcopy output. The numeric suffix selects 
 *           which printer configuration (device 1 or 2) is to be used for the
 *           hardcopy output. If there is no suffix, configuration 1 is
 *           automatically selected. If the output is printed to a file (see
 *           HCOPy:DESTination<1|2> command), the file name used in the last
 *           saving process is automatically counted up to the next unused
 *           name.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyPrintNext (ViSession instrSession,
                                              ViInt32 device)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (device, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Device");
        
    sprintf (buffer, "DE%ld", device);
    
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_HCOPY_PRINT_NEXT, NULL));                
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Set File Name
 * Purpose:  This function defines a destination file for the hadcopy output.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopySetFileName (ViSession instrSession,
                                                ViString name)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_HCOPY_FILE_NAME, name));                
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Get Printer List
 * Purpose:  This function queries the list of printers under Windows NT.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyGetPrinterList (ViSession instrSession,
                                                   ViInt32 bufferSize,
                                                   ViChar printerList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *p2buf;
    ViInt32     count;
    
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (bufferSize<=0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Buffer Size"); 
    if (printerList == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Printer List");
    
    p2buf=printerList;
    count=0;
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "", RSSPECAN_ATTR_HCOPY_PRINTER_FIRST,
                RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    while (strlen(buffer) > 1)
    {
        count+=(ViInt32)strlen(buffer);
        count++; //add one position for comma
        if (count<=bufferSize)
            p2buf += sprintf (p2buf, "%s,", buffer);
        viCheckErr (rsspecan_GetAttributeViString (instrSession, "", RSSPECAN_ATTR_HCOPY_PRINTER_NEXT,
                RSSPECAN_IO_BUFFER_SIZE, buffer));      
    }
    *--p2buf='\0';
    
    if (count>bufferSize)
        error = count;      
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Set Printer
 * Purpose:  This function selects one of the printers installed under 
 *           Windows NT. Prior to use this function get list of available 
 *           printers using function Hardcopy Get Printer List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopySetPrinter (ViSession instrSession,
                                               ViInt32 device,
                                               ViString printerName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (device, 1, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Device");

    sprintf (buffer, "DE%ld", device);
    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_HCOPY_PRINTER,
                                                printerName), 3, "Printer Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Comment
 * Purpose:  This function defines the comment text for measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyComment (ViSession instrSession,
                                            ViString comment)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckErr ( rsspecan_SetAttributeViString (instrSession, "Win0", 
                RSSPECAN_ATTR_HCOPY_COMM_SCR, comment));    
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Time Date
 * Purpose:  This function includes or removes the time and date in the printout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyTimeDate (ViSession instrSession,
                                             ViInt32 device,
                                             ViBoolean time_andDateStamp)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "DE%ld", device);
    viCheckErr ( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                RSSPECAN_ATTR_HCOPY_TIME_DATE, time_andDateStamp));    
 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Hardcopy Color Default
 * Purpose:  This function resets the colors for a hardcopy to the selected 
 *           default settings. DEFault1(SCREEN COLORS, but background white),
 *           DEFault2 (OPTIMIZED COLOR SET) and DEFault3 (USER DEFINED). The
 *           numeric suffix in CMAP is not significant.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyColor (ViSession instrSession,
                                          ViBoolean color,
                                          ViInt32 defaultSetting)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "DEF%ld", defaultSetting);  
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_HCOPY_COLOR, color), 
                2, "Color");
    
    if (color == VI_TRUE)
        viCheckParm ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_HCOPY_COLOR_DEF, defaultSetting), 3, "Default Setting");                
                 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Hardcopy Color
 * Purpose:  This function defines the color table in USER DEFINED COLORS
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyColor (ViSession instrSession,
                                                   ViInt32 colorMap,
                                                   ViReal64 tint,
                                                   ViReal64 saturation,
                                                   ViReal64 brightness)
{
   ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (colorMap, 1, 26) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Color Map");
    if (rsspecan_invalidViReal64Range (tint, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "TINT");
    if (rsspecan_invalidViReal64Range (saturation, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "SATURATION"); 
    if (rsspecan_invalidViReal64Range (brightness, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "BRIGHTNESS");        
    
    viCheckErr( viPrintf (instrSession, "HCOP:CMAP%ld:HSL %.12f,%.12f,%.12f\n",
                colorMap, tint/100.0, saturation/100.0, brightness/100.0));

    checkErr( rsspecan_CheckStatus (instrSession)); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Hardcopy Color by Predefined
 * Purpose:  This function defines the color table in USER DEFINED COLORS 
 *           using predefined color values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyColorByPredefined (ViSession instrSession,
                                                               ViInt32 colorMap,
                                                               ViInt32 predefinedColors)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (colorMap, 1, 26) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Color Map");
    
    sprintf (buffer, "CM%ld", colorMap);  
    
    viCheckParm ( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_HCOPY_COLOR_PREDEFINED, predefinedColors),
                3, "Predefined Colors");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: File Decimal Separator
 * Purpose:  This function configures the decimal separator 
 *           (decimal point or comma) for floating-point numerals contained 
 *           in the file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileManagerOperations (ViSession instrSession,
                                                  ViInt32 operation,
                                                  ViString source,
                                                  ViString destination)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (operation, 0, 8) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Operation");
    if (!source) 
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Source");
    
    switch(operation){
        case RSSPECAN_VAL_FILE_NEW:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_MAKE_DIR,
                source));   
        break;
        case RSSPECAN_VAL_FILE_COPY:
            if (!destination) 
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Destination");
            viCheckErr (viPrintf (instrSession, ":MMEM:COPY '%s','%s'\n", source, destination));
        break;  
        case RSSPECAN_VAL_FILE_RENAME:
            if (!destination) 
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Destination");
            viCheckErr (viPrintf (instrSession, ":MMEM:MOVE '%s','%s'\n", source, destination));
        break;  
        case RSSPECAN_VAL_FILE_DELETE:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_DELETE, 
                 source));
        break;           
        case RSSPECAN_VAL_FILE_RDIR:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_DELETE_DIR, 
                 source));
        break;       
        case RSSPECAN_VAL_FILE_FORMAT:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_FORMAT_DISK, 
                 "a:"));
        break;  
        case RSSPECAN_VAL_FILE_CDIR:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH, 
                 source));
        break;
        case RSSPECAN_VAL_FILE_CDISC:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH_DEVICE, 
                 source));
        break;
        case RSSPECAN_VAL_FILE_DELETE_IMM:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_DELETE_IMMEDIATE, 
                 source));
        break;
    }       
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: File Directory
 * Purpose:  This function reads the indicated directory. According to DOS 
             convention, wild card characters can be entered in order to 
             query eg. a list of all file of a certain type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileDirectory (ViSession instrSession,
                                          ViString directory, 
                                          ViInt32 bufferSize,
                                          ViChar output[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *pbuffer = VI_NULL;
    ViUInt32    count = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (!directory)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Directory");
    viCheckErr (viPrintf(instrSession, "*CLS;:MMEM:CAT? '%s'\n", directory));

    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &pbuffer, &count)); 
    checkErr( rsspecan_CheckStatus (instrSession));
    //strncpy(output, pbuffer, (size_t) bufferSize);
    if (count>=(ViUInt32)bufferSize)
    {
        strncpy(output, pbuffer, (size_t) (bufferSize-1));
        output[bufferSize-1]='\0';
        error = (ViStatus) (count+1);
    } 
    else
    {
        strncpy(output, pbuffer, (size_t) count); 
        output[count]='\0';   
    }
Error:
    if (pbuffer) free (pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: File Directory Path
 * Purpose:  This function queries the directories and files in the given path. 
 *           According to DOS convention, wild card characters can be entered 
 *           in order to query eg. a list of all file of a certain type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileDirectoryPath (ViSession instrSession,
                                              ViString directory,
                                              ViInt32 bufferSize,
                                              ViChar output[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *pbuffer = VI_NULL;
    ViUInt32    count = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (!directory)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Directory");
    viCheckErr (viPrintf(instrSession, "*CLS;:MMEM:CAT:LONG? '%s'\n", directory));
    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &pbuffer, &count)); 
    checkErr( rsspecan_CheckStatus (instrSession));
    
    if (count>=(ViUInt32)bufferSize)
    {
        strncpy(output, pbuffer, (size_t) (bufferSize-1));     
        output[bufferSize-1]='\0';
        error = (ViStatus) (count+1);
    } 
    else
    {
        strncpy(output, pbuffer, (size_t) count); 
        output[count]='\0';   
    }
Error:
    if (pbuffer) free (pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read To File From Instrument
 * Purpose:  This function is used to read data from the instrument and write 
 *           it to a user specified file on the host computer.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_ReadToFileFromInstrument (ViSession instrSession,
                                                     ViString source,
                                                     ViString destination)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEMory:DATA? '%s'", source);
	checkErr(RsCore_QueryBinaryDataBlockToFile(instrSession, cmd, destination, RS_VAL_TRUNCATE));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Write From File To Instrument
 * Purpose:  This function is used to read data from the host computer and 
 *           write it to a user specified file in the instrument.

 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_WriteFromFileToInstrument (ViSession instrSession,
                                                      ViString source,
                                                      ViString destination)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEM:DATA '%s',", destination);
	checkErr(RsCore_WriteBinaryDataFromFile(instrSession, cmd, source));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Data Set File Operations
 * Purpose:  This function allows to provide data set operation for Save/Recall 
 *           items of memory subsystem.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileOperations (ViSession instrSession,
                                                  ViInt32 operation,
                                                  ViString path)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (operation, 0, 7) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Operation");
    
    switch(operation){
        case RSSPECAN_VAL_MEM_SAVE:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_SAVE,
                path), 2, "Path");  
        break;
        case RSSPECAN_VAL_MEM_RECALL:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_RECALL,
                path), 2, "Path");  
        break;  
        case RSSPECAN_VAL_MEM_DELETE:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_DATA_CLEAR,
                path), 2, "Path");  
        break;  
        case RSSPECAN_VAL_MEM_STARTUP:
            viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_STARTUP_RECALL,
                path), 2, "Path");  
        break;           
    }       
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Data Set File Save Next
 * Purpose:  This function stores the current device settings in a *.FSL.dfl 
 *           file. The file name used in the last saving process is automatically 
 *           counted up to the next unused name.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileSaveNext (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_SAVE_NEXT,
                NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Data Set File Comment
 * Purpose:  This function defines a comment ( max. 60 characters) for a device 
 *           setting to be stored.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileComment (ViSession instrSession,
                                               ViString comment)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_EDIT_COMM,
                comment), 2, "Comment");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error; 
}

/*****************************************************************************
 * Function: Data Set File Clear All
 * Purpose:  This function defines a comment ( max. 60 characters) for a device 
 *           setting to be stored.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileClearAll (ViSession instrSession,
                                                ViString directory)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH,
                directory), 2, "Directory");
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_DATA_CLEAR_ALL,
                VI_NULL));              
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error; 
}


/*****************************************************************************
 * Function: Data Set File Select Items
 * Purpose:  This function defines a comment ( max. 60 characters) for a device 
 *           setting to be stored.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileSelectItems (ViSession instrSession,
                                                   ViInt32 itemSelector,
                                                   ViBoolean itemState)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    switch(itemSelector){
        case RSSPECAN_VAL_FILE_ITEMS_NONE:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_NONE,
                VI_NULL));  
        break;
        case RSSPECAN_VAL_FILE_ITEMS_DEFAULT:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_DEFAULT,
                VI_NULL));  
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALL:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_ALL,
                VI_NULL));  
        break;
        case RSSPECAN_VAL_FILE_ITEMS_HWSETTINGS:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_HWSETTINGS,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALLTRACES:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_TRAC,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALLLINES:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_LINE,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_SOUR_CAL_DATA:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_SOURCE_CAL_DATA,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALLTRAN:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_TRAN,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_SPECTROGRAM:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SPEM,
                itemState), 3, "Item State");  
        break;          
        case RSSPECAN_VAL_FILE_ITEMS_IQ_DATA:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_INCLUDE_IQ_DATA,
                itemState), 3, "Item State");  
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Item Selector");
    }   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error; 
}

/// HIFN This function add/remove settings to/from the list of partial datasets 
/// HIFN of a currently selected channel setting to be stored/loaded.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR itemSelector/This control selects the items to save or recall.
/// HIPAR itemState/This control sets if selected item is active or not for to save/recall
ViStatus _VI_FUNC rsspecan_DataSetFileSelectItemsFromChannel (ViSession instrSession,
                                                     ViInt32 itemSelector,
                                                     ViBoolean itemState)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    switch(itemSelector){
        case RSSPECAN_VAL_FILE_ITEMS_NONE:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_NONE,
                VI_NULL));  
        break;
        case RSSPECAN_VAL_FILE_ITEMS_DEFAULT:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_DEFAULT,
                VI_NULL));  
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALL:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_ALL,
                VI_NULL));  
        break;
        case RSSPECAN_VAL_FILE_ITEMS_HWSETTINGS:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_HWSETTINGS,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALLTRACES:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_TRAC,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALLLINES:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_LINE,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_ALLTRAN:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_TRAN,
                itemState), 3, "Item State");   
        break;
        case RSSPECAN_VAL_FILE_ITEMS_SPECTROGRAM:
            viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SPEM,
                itemState), 3, "Item State");  
        break;          
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Item Selector");
    }  
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines whether the channel-specific settings that will be loaded with the 
/// HIFN rsspecan_DataSetFileOperations function will replace the current channel or activate a new channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR mode/This control selects the load mode.
ViStatus _VI_FUNC rsspecan_DataSetLoadMode (ViSession instrSession, ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                        RSSPECAN_ATTR_DATA_SET_LOAD_MODE,
                                        mode), 2, "Mode");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines whether the data from the entire instrument 
/// HIFN or only from the current channel is stored.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR mode/This control selects the store mode.
ViStatus _VI_FUNC rsspecan_DataSetStoreMode (ViSession instrSession, ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
                                        RSSPECAN_ATTR_DATA_SET_STORE_MODE,
                                        mode), 2, "Mode");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines the settings file to be loaded when the 
/// HIFN  specified user-definable softkey is selected. The settings file must 
/// HIFN  exist, otherwise an error occurs.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR softkeyNumber/Number of the softkey in the "User" menu.
/// HIPAR filename/This parameter defines the path and file name of the settings to be 
/// HIPAR filename/loaded.
/// HIPAR label/This parameter defines the label of the user-definable softkey.
ViStatus _VI_FUNC rsspecan_ConfigureUserSettingsSoftkey(
    ViSession   instrSession,
    ViInt32 softkeyNumber,
    ViString    filename,
    ViString    label
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (softkeyNumber, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Softkey Number");
    }

    viCheckErr (viPrintf (instrSession, "MMEM:USER%ld '%s','%s'\n", softkeyNumber, filename, label));
    
    viCheckErr (rsspecan_CheckStatus (instrSession));
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*===========================================================================*
 *= RsSpecAn Calibration Group Functions ====================================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Calibration
 * Purpose:  This function initiates the acquisition of system error correction 
 *           data. A "0" is returned if the acquisition was successful.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_Calibration (ViSession instrSession,
                                        ViBoolean sync, 
                                        ViUInt32 timeout,
                                        ViInt32 *result)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     old_timeout=-1;
    ViChar response[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViBooleanRange (sync) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Sync");
    if (sync == VI_TRUE)
    {   
        if (rsspecan_invalidViInt32Range (timeout, 0, 600000) == VI_TRUE)
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");     
        viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));   
        viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout));  
        
        checkErr (Rs_ClearBeforeRead(instrSession));
        viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_CALIBRATION, result), 2, "Result");
        
        checkErr (Rs_WriteInstrData(instrSession, "*ESR?\n"));
        checkErr (Rs_ReadInstrData(instrSession, 10, response, NULL));
    }
    else
    {
        checkErr (Rs_ClearBeforeRead(instrSession));
        checkErr (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_CALIBRATION, RSSPECAN_VAL_CAL_ALL));
    }   
    
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);  
    
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Abort
 * Purpose:  This function aborts the acquisition of correction data and 
 *           restores the last complete correction data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationAbort (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_CALIBRATION_ABORT, VI_NULL));
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Result
 * Purpose:  This function reads the results of the correction data acquisition.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationResult (ViSession instrSession,
                                              ViInt32 arraySize, 
                                              ViChar result[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *pbuffer=VI_NULL;
    ViUInt32    ret_val;
    ViUInt32    length;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    //viCheckParm (rsspecan_GetAttributeViString (instrSession, "", 
    //        RSSPECAN_ATTR_CALIBRATION_RESULT_QUERY, arraySize, result), 2, "Result");
    viCheckErr (viPrintf (instrSession, "CAL:RES?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_val));
    length=((ViUInt32)arraySize<=ret_val)?(ViUInt32)arraySize-1:ret_val;
    memcpy (result,pbuffer,length);
    result[length]='\0';
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration State
 * Purpose:  This function determines whether the current calibration data 
 *           are taken into account by the instrument (ON) or not (OFF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationState (ViSession instrSession,
                                             ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_CALIBRATION_STATE, state), 2, "State");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: YIG Correction
 * Purpose:  This function switches on or off the automatic correction of 
 *           the YIG filter frequency drift.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_YIGCorrection (ViSession instrSession,
                                          ViInt32 window, 
                                          ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, 
            RSSPECAN_ATTR_CORR_YIG, state), 3, "State");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Calibration Probe Catalog
 * Purpose:  This function queries the names of all probe data sets stored in 
 *           the FMU.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetCalibrationProbeCatalog (ViSession instrSession,
                                                       ViInt32 *number_ofDataSets,
                                                       ViInt32 bufferSize,
                                                       ViChar dataSetList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (!(strstr (model, "FMU")))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (bufferSize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Buffer Size");         
    if ((bufferSize > 0) && (dataSetList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Data Set List");  
       
    viCheckErr (viPrintf (instrSession, "SENS:PROB:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, bufferSize, dataSetList, number_ofDataSets));

    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe State
 * Purpose:  This function activates a selected probe data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeState (ViSession instrSession,
                                                  ViString probeDataSetName,
                                                  ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT, probeDataSetName), 2, "Probe Data Set Name");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_STATE, state), 3, "State");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Settings
 * Purpose:  This function configures the probe mode, DC voltage and pulse 
 *           frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeSettings (ViSession instrSession,
                                                     ViInt32 probeMode,
                                                     ViReal64 value)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    switch (probeMode)
    {
        case RSSPECAN_VAL_PROBE_MODE_DC:
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
                         RSSPECAN_ATTR_CAL_PROBE_DC_VOLTAGE, value), 3, "Value");
        break;
        case RSSPECAN_VAL_PROBE_MODE_PULS:
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
                         RSSPECAN_ATTR_CAL_PROBE_PULSE_FREQ, value), 3, "Value");
        break;
        case RSSPECAN_VAL_PROBE_MODE_COMP:
        break;
    }
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_ADJUST_MODE, probeMode), 2, "Probe Mode");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Manual Compensation
 * Purpose:  This function configures the calibration probe manual compensation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeManualCompensation (ViSession instrSession,
                                                               ViBoolean manualProbeAdjustment)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_MAN_COMPENSATION, manualProbeAdjustment), 
            2, "Manual Probe Adjustment");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Compensation
 * Purpose:  This function configures the calibration probe compensation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeCompensation (ViSession instrSession,
                                                         ViBoolean frequencyResponseComp,
                                                         ViBoolean gainErrorCompensation)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_FREQ_RESP_COMPENSATION, frequencyResponseComp), 
            2, "Frequency Response Comp");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_GAIN_ERROR_COMPENSATION, gainErrorCompensation), 
            3, "Gain Error Compensation");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Start Probe Calibration
 * Purpose:  This function starts a probe calibration using the specified file 
 *           name to generate a probe data set at the end of the calibration 
 *           process. During the probe calibration process, manual operations 
 *           may be required (see -chapter 4, softkey PROBE CAL START). The 
 *           Status Questionable Sync Register bits 12 and 13 are set if related
 *           user operation has to take place. After adjusting the probes of
 *           input I or Q, the calibration process has to be continued by the
 *           command "INIT:CONM". The probe data set is automatically
 *           activated and can be switched off again using the
 *           SENS:PROBE:STATE ON | OFF command.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StartProbeCalibration (ViSession instrSession,
                                                  ViString probeDataSetName)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    // STAT:QUES:SYNC:ENAB, STAT:QUES:SYNC:PTR, STAT:QUES:SYNC:NTR
    viCheckErr (rsspecan_setStatusRegister (instrSession, 1, 4, 12288, 12288, 0));
    
    // STAT:OPER:ENAB, STAT:OPER:PTR, STAT:OPER:NTR
    viCheckErr (rsspecan_setStatusRegister (instrSession, 1, 0, 2048, 2048, 0));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_START, probeDataSetName), 2, "Probe Data Set Name");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Probe Calibration Status
 * Purpose:  This function returns the status of the probe calibration.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetProbeCalibrationStatus (ViSession instrSession,
                                                      ViUInt32   timeout,
                                                      ViBoolean *probeCalibrationStatusI,
                                                      ViBoolean *probeCalibrationStatusQ)
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    counter     = 0;
    ViInt32     status      = 0;
    ViUInt16    response    = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");  
        
    *probeCalibrationStatusI = VI_FALSE;
    *probeCalibrationStatusQ = VI_FALSE;

    do
    {
        error = viReadSTB (instrSession, &response); 
    
        if (response & 0x8)
        {
            checkErr (rsspecan_getStatusRegister (instrSession, 10, &status));
            if (status & 4096) // 12. bit
                *probeCalibrationStatusI = VI_TRUE;
            if (status & 8192) // 13. bit
                *probeCalibrationStatusQ = VI_TRUE;
            break;
        }
    
        Rs_Delay (0.001);
    
        counter++;

        if (counter >= timeout)
            return VI_ERROR_TMO;

        /* Wait while STB is non zero value. Only Error/Event Queue
           and STAT:QUES bits are taken into consideration. */
    
    } while ((response & 12) == 0);
    
    /* (check Error(Event) Queue bit in STB) */
    if ((response & 0x4) != 0) 
        return RS_ERROR_INSTRUMENT_STATUS;
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Check
 * Purpose:  This function queries, if the calibration of the selected probe 
 *           data set passed or failed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeCheck (ViSession instrSession,
                                                  ViString probeDataSetName,
                                                  ViInt32 *probeCheck)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT, probeDataSetName), 2, "Probe Data Set Name");
    
    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_CHECK, probeCheck), 3, "Probe Check");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Result
 * Purpose:  This function queries the calibration result of the selected 
 *           probe data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeResult (ViSession instrSession,
                                                   ViString probeDataSetName,
                                                   ViInt32 arraySize,
                                                   ViChar result[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *pbuffer= VI_NULL;
    ViUInt32    ret_val;
    ViUInt32    length;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (!(strstr (model, "FMU")))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT, probeDataSetName), 2, "Probe Data Set Name");
    
    viCheckErr (viPrintf (instrSession, "SENS:PROB:RES?\n"));
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_val));
    
    length = ((ViUInt32)arraySize <= ret_val) ? (ViUInt32)arraySize - 1 : ret_val;
    memcpy (result, pbuffer, length);
    result[length] = '\0';
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Rename
 * Purpose:  This function renames existing probe data sets.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeRename (ViSession instrSession,
                                                   ViString probeDataSetSource,
                                                   ViString probeDataSetDestination)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (!(strstr (model, "FMU")))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    viCheckErr (viPrintf (instrSession, "SENS:PROB:MOVE '%s','%s'\n",
                                         probeDataSetSource, probeDataSetDestination));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Calibration Probe Delete
 * Purpose:  This function deletes the specified probe data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeDelete (ViSession instrSession,
                                                   ViString probeDataSetName)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_CAL_PROBE_DELETE, probeDataSetName), 2, "Probe Data Set Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function selects the probe connector.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR probeConnector/Selects the connector.
ViStatus _VI_FUNC rsspecan_SelectProbeConnector (ViSession instrSession,
                                                 ViInt32 probeConnector)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
            RSSPECAN_ATTR_PROBE_CONNECTOR, probeConnector), 2, "Probe Connector");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function activates a connected probe.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR state/This conrol activates a connected probe.
ViStatus _VI_FUNC rsspecan_ProbeActivation (ViSession instrSession,
                                            ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
                                                RSSPECAN_ATTR_PROBE_ACTIVATION, 
                                                state), 2, "State");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines which action is taken when the probe's micro button is pressed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR microButtonAction/This control selects which action is taken when the probe's
/// HIPAR microButtonAction/micro button is pressed.
ViStatus _VI_FUNC rsspecan_ProbeMicroButtonAction (ViSession instrSession,
                                                              ViInt32 microButtonAction)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     probe = 0;
    ViChar      rep_cap[10] = "";
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViInt32 (instrSession, "",
                                              RSSPECAN_ATTR_PROBE_CONNECTOR, &probe));
    
    viCheckErr (rsspecan_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION,
                                                 "Probe",
                                                 probe - 1,
                                                 10,
                                                 rep_cap));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap, 
                                                RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION, 
                                                microButtonAction), 2, "microButtonAction");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function returns the probe informations.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the catalog list parameter.
/// HIPAR state/This control shows whether a probe is connected to the instrument's RF INPUT 
/// HIPAR state/and USB connectors and was recognized by the R&S FSV
/// HIPAR name/This control returns the name of the connected probe.
/// HIPAR serialNumber/This control returns the serial number of the connected probe.
/// HIPAR partNumber/This control returns the material part number of the connected probe.
ViStatus _VI_FUNC rsspecan_GetProbeInformation (ViSession instrSession, ViInt32 bufferSize,
                                                ViInt32 *state, ViChar name[],
                                                ViChar serialNumber[],
                                                ViChar partNumber[])
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     probe = 0;
    ViChar      rep_cap[20] = "";
    ViChar      rep_cap_2[20] = "";
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViInt32 (instrSession, "",
                                              RSSPECAN_ATTR_PROBE_CONNECTOR, &probe));
    
    viCheckErr (rsspecan_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION,
                                                 "Probe",
                                                 probe - 1,
                                                 10,
                                                 rep_cap));

    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, rep_cap, 
                                                RSSPECAN_ATTR_PROBE_CONNECTION_STATE, 
                                                state), 3, "state");
    
    if (*state == 1)
    {
        viCheckParm (rsspecan_GetAttributeViString (instrSession, rep_cap, 
                                                    RSSPECAN_ATTR_PROBE_NAME, 
                                                    bufferSize, name), 4, "name");
    
        strcpy (rep_cap_2, rep_cap);
        strcat (rep_cap, ",Serial");
        
        viCheckParm (rsspecan_GetAttributeViString (instrSession, rep_cap, 
                                                    RSSPECAN_ATTR_PROBE_NUMBER, 
                                                    bufferSize, serialNumber), 5, "serialNumber");

        strcat (rep_cap_2, ",Part");
        
        viCheckParm (rsspecan_GetAttributeViString (instrSession, rep_cap_2, 
                                                    RSSPECAN_ATTR_PROBE_NUMBER, 
                                                    bufferSize, partNumber), 6, "partNumber");
    } 
    else
    {
        sprintf (name, "Not detected");
        sprintf (serialNumber, "Not detected"); 
        sprintf (partNumber, "Not detected"); 
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function returns the type of the probe.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR probeConnector/Selects the connector.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the Probe type parameter.
/// HIPAR probeType/This control returns the probe type.
ViStatus _VI_FUNC rsspecan_GetProbeType (ViSession instrSession,
                                         ViInt32 probeConnector,
                                         ViInt32 bufferSize,
                                         ViChar probeType[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      rep_cap[20] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_PROBE_TYPE,
                                                 "Probe",
                                                 probeConnector - 1,
                                                 10,
                                                 rep_cap), 2, "Probe Connector");

    viCheckParm (rsspecan_GetAttributeViString (instrSession, rep_cap, 
                                                    RSSPECAN_ATTR_PROBE_TYPE, 
                                                    bufferSize, probeType), 4, "Probe Type");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Control Lines State
 * Purpose:  This function toggles the control lines of the user ports between 
 *           INPut and OUTPut.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureControlLinesState (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, 
            RSSPECAN_ATTR_INP_UPORT_STATE, state), 3, "State");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Output Control Lines
 * Purpose:  This function sets the control lines of the user ports.
 *****************************************************************************/
#pragma warning( push )
#pragma warning( disable : 4100)
ViStatus _VI_FUNC rsspecan_ConfigureOutputControlLines (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViString userPorts)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", 
            RSSPECAN_ATTR_OUT_UPORT_VALUE, userPorts), 3, "User Ports");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
#pragma warning( pop )

/*****************************************************************************
 * Function: Configure Output Control Lines
 * Purpose:  This function sets the control lines of the user ports.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetControlLines (ViSession instrSession,
                                            ViInt32 window, 
                                            ViInt32 lines,
                                            ViChar userPorts[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (lines, 0, 1) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Lines");
    }
    sprintf (buffer, "Win%ld", window);  
    
    switch (lines){
        
        case RSSPECAN_VAL_USER_PORT_INP:
            viCheckParm (rsspecan_GetAttributeViString (instrSession, buffer, 
                    RSSPECAN_ATTR_INP_UPORT_VALUE, 8, userPorts), 3, "User Ports");
        break;
        case RSSPECAN_VAL_USER_PORT_OUT:
            viCheckParm (rsspecan_GetAttributeViString (instrSession, buffer, 
                    RSSPECAN_ATTR_OUT_UPORT_VALUE, 8, userPorts), 3, "User Ports");
        break;
    }       

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Auto Adjust
 * Purpose:  This function configures the auto adjust settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjust (ViSession instrSession,
                                                ViInt32 adjustSettings,
                                                ViInt32 measurementTimeMode,
                                                ViReal64 measurementTime)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));

    if (strstr (model, "FSW") != NULL)
        sprintf (buffer, "FSW");
    else  sprintf (buffer, "FSV"); 
    
    switch (adjustSettings)
    {
        case RSSPECAN_VAL_ADJ_ALL:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                    RSSPECAN_ATTR_AUTO_ADJUST_ALL, NULL));
        break;
        case RSSPECAN_VAL_ADJ_FREQ:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                    RSSPECAN_ATTR_AUTO_ADJUST_FREQ, NULL));
        break;
        case RSSPECAN_VAL_ADJ_LEV:
            viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                    RSSPECAN_ATTR_AUTO_ADJUST_LEVEL, NULL));
        break;
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Adjust Settings"); 
    }
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
            RSSPECAN_ATTR_AUTO_ADJUST_LEVEL_MEAS_TIME_MODE, measurementTimeMode), 3, "Measurement Time Mode");
    
    if (measurementTimeMode == RSSPECAN_VAL_MAN)
    {   
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                RSSPECAN_ATTR_AUTO_ADJUST_LEVEL_MEAS_TIME, measurementTime), 4, "Measurement Time");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the auto adjust hystersis settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR lowerHystersis/Defines a lower threshold the signal must drop below before the reference 
/// HIPAR lowerHystersis/level is automatically adjusted when the "Auto Level" function is performed
/// HIPAR upperHystersis/Defines an upper threshold the signal must exceed before the reference 
/// HIPAR upperHystersis/level is automatically adjusted when the "Auto Level" function is performed.
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjustHystersis (ViSession instrSession,
                                                         ViReal64 lowerHystersis,
                                                         ViReal64 upperHystersis)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Low", 
                RSSPECAN_ATTR_AUTO_ADJUST_HYSTERSIS, lowerHystersis), 2, "Lower Hystersis");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Upp", 
                RSSPECAN_ATTR_AUTO_ADJUST_HYSTERSIS, upperHystersis), 3, "Upper Hystersis");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function defines the behaviour of the measurement when adjusting a setting automatically.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR state/Defines the behaviour of the measurement when adjusting a setting automatically.
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjustTriggerState (ViSession instrSession,
                                                            ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
                        RSSPECAN_ATTR_AUTO_ADJUST_TRIGGER_STATE, state), 2, "state");  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Y-Axis Auto Scale
 * Purpose:  This function initiates auto scaling for screens.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_YAxisAutoScale (ViSession instrSession,
                                           ViInt32 autoScaleScreen)

{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (autoScaleScreen, RSSPECAN_VAL_AUTOSCALE_ALL, RSSPECAN_VAL_AUTOSCALE_D) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Auto Scale Screen");
    }
    
    sprintf (buffer, "Win%ld", autoScaleScreen);  
    
    if (autoScaleScreen == RSSPECAN_VAL_AUTOSCALE_ALL)
    {   viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                                   RSSPECAN_ATTR_AUTO_SCALE_ALL_SCREENS,
                                                   NULL));
    }
    else 
    {
        viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                   RSSPECAN_ATTR_AUTO_SCALE_SINGLE_SCREEN,
                                                   NULL));
    }
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: rsspecan_SetAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to set
 *           attribute values directly.  There are typesafe versions for
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.
 *****************************************************************************/

/* ViInt32 */
ViStatus _VI_FUNC rsspecan_SetAttributeViInt32 (ViSession instrSession, ViString channelName,
                                                ViUInt32 attributeId, ViInt32 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViInt32(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViReal64 */
ViStatus _VI_FUNC rsspecan_SetAttributeViReal64 (ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViReal64 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViReal64(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViString */
ViStatus _VI_FUNC rsspecan_SetAttributeViString (ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViString value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViString(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViBoolean */
ViStatus _VI_FUNC rsspecan_SetAttributeViBoolean (ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViBoolean value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViBoolean(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViSession */
ViStatus _VI_FUNC rsspecan_SetAttributeViSession (ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViSession value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViSession(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: rsspecan_GetAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to get
 *           attribute values directly.  There are typesafe versions for
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession attributes.
 *****************************************************************************/

/* ViInt32 */
ViStatus _VI_FUNC rsspecan_GetAttributeViInt32 (ViSession instrSession, ViString channelName,
                                                ViUInt32 attributeId, ViInt32 *value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);
	error = RsCore_GetAttributeViInt32(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViReal64 */
ViStatus _VI_FUNC rsspecan_GetAttributeViReal64 (ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViReal64 *value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_GetAttributeViReal64(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViString */
ViStatus _VI_FUNC rsspecan_GetAttributeViString (ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViInt32 bufSize,
                                                 ViChar value[])
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_GetAttributeViString(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, bufSize,
	                                    value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViBoolean */
ViStatus _VI_FUNC rsspecan_GetAttributeViBoolean (ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViBoolean *value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_GetAttributeViBoolean(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViSession */
ViStatus _VI_FUNC rsspecan_GetAttributeViSession (ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViSession *value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_GetAttributeViSession(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: rsspecan_CheckAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to check
 *           attribute values directly.  These functions check the value you
 *           specify even if you set the rsspecan_ATTR_RANGE_CHECK
 *           attribute to VI_FALSE.  There are typesafe versions for ViInt32,
 *           ViReal64, ViString, ViBoolean, and ViSession datatypes.
 *****************************************************************************/

/* ViInt32 */
ViStatus _VI_FUNC rsspecan_CheckAttributeViInt32 (ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViInt32 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViInt32(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViReal64 */
ViStatus _VI_FUNC rsspecan_CheckAttributeViReal64 (ViSession instrSession, ViString channelName,
                                                   ViUInt32 attributeId, ViReal64 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViReal64(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViString */
ViStatus _VI_FUNC rsspecan_CheckAttributeViString (ViSession instrSession, ViString channelName,
                                                   ViUInt32 attributeId, ViString value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViString(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViBoolean */
ViStatus _VI_FUNC rsspecan_CheckAttributeViBoolean (ViSession instrSession, ViString channelName,
                                                    ViUInt32 attributeId, ViBoolean value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViBoolean(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViSession */
ViStatus _VI_FUNC rsspecan_CheckAttributeViSession (ViSession instrSession, ViString channelName,
                                                    ViUInt32 attributeId, ViSession value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViSession(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Rs_GetAttributeRepCapNameId
 * Purpose:  This function returns the repeated capability id(s) that
 *           belongs to the attribute you specify. More then one id is listed
 *           in comma separated string.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetAttributeRepeatedCapabilityIds (ViSession instrSession,
                                                            ViUInt32 attributeID,
                                                            ViInt32 bufferSize,
                                                            ViChar _VI_FAR repeatedCapabilityIds[])
{
	ViStatus error = VI_SUCCESS;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 1, RS_MAX_SHORT_MESSAGE_BUF_SIZE),
			3, "Buffer Size");

	checkErr(RsCore_GetAttributeRepCapNameIds(instrSession, attributeID, bufferSize, repeatedCapabilityIds));

Error:
	return error;
}

/*****************************************************************************
 * Function: Get Attribute Repeated Capability Id Name(s)
 * Purpose:  This function returns the repeated capability name(s) that
 *           belongs to the attribute and single repeated capability id you
 *           may specify. More then one name is listed in comma separated
 *           string.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetAttributeRepeatedCapabilityIdNames (ViSession instrSession,
                                                                ViUInt32 attributeID,
                                                                ViString repeatedCapabilityId,
                                                                ViInt32 bufferSize,
                                                                ViChar _VI_FAR repeatedCapabilityIdNames[])
{
	ViStatus error = VI_SUCCESS;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 1, RS_MAX_SHORT_MESSAGE_BUF_SIZE),
			4, "Buffer Size");

	checkErr(RsCore_GetAttributeRepCapNamesAll(instrSession,
		attributeID,
		repeatedCapabilityId,
		bufferSize,
		repeatedCapabilityIdNames));

Error:
	return error;
}

/*****************************************************************************
 * Function:    Read Y Trace
 * Purpose:     This function initiates a signal acquisition based on the
 *              present instrument configuration.  It then waits for the
 *              acquisition to complete, and returns the trace as an array of
 *              amplitude values. The amplitude array returns data that
 *              represent the amplitude of the signals of the sweep from the
 *              start frequency to the stop frequency (in frequency domain, in
 *              time domain the amplitude array is ordered from beginning of
 *              sweep to end).  The Amplitude Units attribute determines the
 *              units of the points in the amplitude array.  This function
 *              resets the sweep count.
 *
 *              If the spectrum analyzer did not complete the acquisition
 *              within the time period the user specified with the maxTime
 *              parameter, the function returns the Max Time Exceeded error.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadYTrace (ViSession instrSession,
                                       ViInt32 window,
                                       ViInt32 trace,
                                       ViUInt32 maximumTime,
                                       ViInt32 arrayLength,
                                       ViInt32 *actualPoints,
                                       ViReal64 amplitude[])

{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL") != NULL) || 
        (rsspecan_IsFSV (instrSession)))
        trace_range=6;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");
    if (rsspecan_invalidViUInt32Range (maximumTime, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Maximum Time");         
       
    checkErr( rsspecan_Initiate (instrSession, window, maximumTime));
   
    sprintf (buffer, "TRACE%ld", trace);
    viCheckErr (rsspecan_dataReadTrace (instrSession, window, buffer, arrayLength, 
                    amplitude, actualPoints));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Fetch Y Trace
 * Purpose:     This function returns the trace the spectrum analyzer acquires.
 *              The trace is from a previously initiated acquisition.  You use
 *              the rsspecan_Initiate function to start an acquisition.  You use
 *              the rsspecan_AcquisitionStatus function to determine when the
 *              acquisition is complete.
 *
 *              You can call the Read Y Trace function instead of the Initiate
 *              function. This function starts an acquisition, waits for the
 *              acquisition to complete, and returns the trace in one function
 *              call.
 *
 *              The array returns data that represents the amplitude of the
 *              signals of the sweep from the start frequency to the stop
 *              frequency (in frequency domain, in time domain the amplitude
 *              array is ordered from beginning of sweep to end).  The
 *              Amplitude Units attribute determines the units of the points
 *              in the amplitude array.
 *
 *              This function does not check the instrument status. Typically,
 *              the end-user calls this function only in a sequence of calls
 *              to other low-level driver functions. The sequence performs one
 *              operation. The end-user uses the low-level functions to
 *              optimize one or more aspects of interaction with the
 *              instrument. Call the rsspecan_error_query function at the
 *              conclusion of the sequence to check the instrument status.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchYTrace (ViSession instrSession,
                                        ViInt32 window, 
                                        ViInt32 trace,
                                        ViInt32 arrayLength,
                                        ViInt32 *actualPoints,
                                        ViReal64 amplitude[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     trace_range = 3;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL") != NULL) || 
        (rsspecan_IsFSV (instrSession)))
        trace_range=6;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");

    sprintf (buffer, "TRACE%ld", trace);
    
    viCheckErr (rsspecan_dataReadTrace (instrSession,window, buffer, arrayLength, 
                    amplitude, actualPoints));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the previously acquired trace data for the specified trace from the memory.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR offsetSweepPoint/The offset in sweep points related to the start of the measurement 
/// HIPAR offsetSweepPoint/at which data retrieval is to start.
/// HIPAR numberOfSweeps/Number of sweep points to be retrieved from the trace.
/// HIPAR arrayLength/Number of array points requested.
/// HIPAR actualPoints/Number of points actually returned in the array.
/// HIPAR amplitude/A user allocated buffer into which the trace amplitudes is stored.
ViStatus _VI_FUNC rsspecan_ReadYTracePrevious (ViSession instrSession,
                                               ViInt32 window, ViInt32 trace,
                                               ViInt32 offsetSweepPoint,
                                               ViInt32 numberOfSweeps,
                                               ViInt32 arrayLength,
                                               ViInt32 *actualPoints,
                                               ViReal64 amplitude[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViInt32     trace_range = 6;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (!rsspecan_IsFSV (instrSession))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Trace");

    sprintf (buffer, ":MEM? TRACE%ld,%ld,%ld", trace, offsetSweepPoint, numberOfSweeps);
    
    viCheckErr (rsspecan_dataReadTrace (instrSession, window, buffer, arrayLength, 
                    amplitude, actualPoints));

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN Reads the x values (time in seconds) of the measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR arrayLength/Number of array points requested.
/// HIPAR actualPoints/Number of points actually returned in the array.
/// HIPAR x/A user allocated buffer into which the trace X values is stored.
ViStatus _VI_FUNC rsspecan_FetchXTrace (ViSession instrSession,
                                              ViInt32 trace, ViInt32 arrayLength,
                                              ViInt32 *actualPoints,
                                              ViReal64 x[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (trace, 1, 6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
    
    sprintf (buffer, "*CLS;FORM REAL,32;TRAC:DATA:X? TRACe%ld\n", trace);
    
    viCheckErr (viPrintf (instrSession, buffer));
    viCheckErr (rsspecan_dataReadTraceOnly (instrSession, arrayLength, x, actualPoints));
    
    checkErr( rsspecan_CheckStatus (instrSession));     
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Get Current Results
 * Purpose:     This function returns the current X and Y results of the sweep. 
 *              During a sweep the last measured values are read out.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetCurrentResults (ViSession instrSession,
                                         ViInt32 window, 
                                         ViReal64 *xValue,
                                         ViReal64 *yValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
  
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));

    if ((strstr (model, "FSL") != NULL) || 
        (strstr (model, "FMU") != NULL) || 
        (rsspecan_IsFSV (instrSession)))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if (!xValue)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "X Value");
    if (!yValue)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Y Value");
        
    viCheckErr( viQueryf (instrSession, "TRAC%ld:IMM:RES?\n", "%le,%le", window, xValue, yValue)); 
    
    checkErr( rsspecan_CheckStatus (instrSession));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Get Current Y Result
 * Purpose:     This function returns the current Y results of the sweep. During
 *              a sweep the last measured value is read out.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetCurrentYResult (ViSession instrSession,
                                       ViInt32 window, 
                                       ViReal64 *level)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_Y_RESULT, level),
                3, "Level"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Read Trace IQ Data
 * Purpose:     This function returns the current X and Y results of the sweep. 
 *              During a sweep the last measured values are read out.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadTraceIQData (ViSession instrSession,
                                            ViInt32 window, 
                                            ViUInt32 timeout,
                                            ViInt32 bufferSize,
                                            ViInt32 *noofPoints,
                                            ViReal64 realPartsI[],
                                            ViReal64 imaginaryPartsQ[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      cmd [100] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");  
        
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               100, cmd));
    
    if (strstr (cmd, "FSL") != NULL)
    {
        sprintf (cmd, ":FORM REAL,32;*CLS;:TRAC%ld:IQ:DATA?\n", window);
    }
    else
    {
        sprintf (cmd, ":FORM REAL,32;:TRAC:IQ:DATA:FORM COMP;*CLS;:TRAC%ld:IQ:DATA?\n",
                    window);
    }

    viCheckErr (viWrite (instrSession, (ViBuf) cmd, (ViUInt32) strlen (cmd), NULL));

    viCheckErr (rsspecan_CheckBeforeRead(instrSession, timeout));

    viCheckErr (rsspecan_dataReadTraceIQ (instrSession, bufferSize, noofPoints, 
                    realPartsI, imaginaryPartsQ));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Fetch Trace IQ Data
 * Purpose:     This function permits the readout of previously acquired 
 *              (and frequency response corrected) I/Q data from the memory, 
 *              with indication of the offset related to the start of measurement 
 *              and with indication of the number of measurement values. 
 *              Therefore a previously acquired data set can be read out in smaller
 *              portions. The result values are scaled linear in unit Volt and
 *              correspond to the voltage at the RF input of the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchTraceIQData (ViSession instrSession,
                                             ViInt32 offsetSamples,
                                             ViInt32 noofSamples,
                                             ViInt32 bufferSize,
                                             ViInt32 *noofPoints,
                                             ViReal64 realPartsI[],
                                             ViReal64 imaginaryPartsQ[])
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckErr (viPrintf (instrSession, ":FORM REAL,32;*CLS;:TRAC:IQ:DATA:MEM? %ld,%ld\n",
                offsetSamples, noofSamples));
                
    viCheckErr (rsspecan_dataReadTraceIQ (instrSession, bufferSize, noofPoints, 
                    realPartsI, imaginaryPartsQ));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Read Memory IQ Large Data
 * Purpose:     This function sets the transfer parameters and reads 
 *              the data header.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadMemoryIQLargeData (ViSession instrSession,
                                                  ViInt32 window, 
                                                  ViUInt32  timeout,
                                                  ViInt32 format,
                                                  ViInt32 dataOrder,
                                                  ViInt32 *no_ofPoints)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    ViInt32     count = 0;
    ViInt32     lenLength;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
    if (rsspecan_invalidViInt32Range (format, 0, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Format");        
    if (rsspecan_invalidViInt32Range (dataOrder, 0, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Data Order");         
        
    switch (format){
        case RSSPECAN_VAL_FORMAT_ASC:
            viCheckErr (viPrintf (instrSession, ":FORM ASC\n"));
        break;  
        case RSSPECAN_VAL_FORMAT_REAL:
            viCheckErr (viPrintf (instrSession, ":FORM REAL,32\n"));
        break;
    }
    
    sprintf (buffer, "Win%ld", window); 
    if (strstr(model, "FSL") == NULL)
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_IQ_DATA_FORMAT, dataOrder),
                4, "Data Order");
    }   
    viCheckErr (viPrintf (instrSession, ":TRAC%ld:IQ:DATA?\n", window));
    viCheckErr (rsspecan_CheckBeforeRead(instrSession, timeout));   
    switch (format){
        case RSSPECAN_VAL_FORMAT_ASC:
            count=-1;   
        break;  
        case RSSPECAN_VAL_FORMAT_REAL:
            checkErr (viRead (instrSession, (ViPBuf)buffer, 2, (ViPUInt32) &count));                
            if (sscanf (buffer, "#%1ld", &lenLength) != 1) 
                viCheckErr (VI_ERROR_INV_RESPONSE);
            checkErr (viRead (instrSession, (ViPBuf)buffer, lenLength, (ViPUInt32) &count));
            buffer[lenLength]='\0';
            if (sscanf (buffer, "%lu", &count) != 1) 
                viCheckErr (VI_ERROR_INV_RESPONSE);
        break;
    }
    if (no_ofPoints) *no_ofPoints = count;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Read Memory IQ Large Data Block
 * Purpose:     This function will read I/Q trace data according format 
 *              defined with function rsspecan_ReadMemoryIQLargeData.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadMemoryIQLargeDataBlock (ViSession instrSession,
                                                       ViInt32 bufferSize,
                                                       ViChar returnedData[],
                                                       ViInt32 *returnedCounts)
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    count;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    error = viRead (instrSession, (ViPBuf)returnedData, bufferSize, &count);
    
    if (returnedCounts)
        *returnedCounts=count;

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*****************************************************************************
 * Function: Get Limit Check Result
 * Purpose:  This function queries the result of the limit check of the limit 
 *           line indicated in the selected measurement window. It should be 
 *           noted that a complete sweep must have been performed for obtaining 
 *           a valid result. A synchronization with  *OPC, *OPC? or *WAI should 
 *           therefore be provided.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetLimitCheckResult(
    ViSession   instrSession,
    ViInt32 window,
    ViInt32 limit,
    ViInt32*    state
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    }

    sprintf (buffer, "Win%ld,L%ld", window, limit);

    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_CHECK_RESULT, state), 4, "State");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Initiate
 * Purpose:     This function initiates an acquisition. After calling this
 *              function, the spectrum analyzer leaves the Idle state and
 *              waits for a trigger.
 *
 *              This function does not check the instrument status. Typically,
 *              the end-user calls this function only in a sequence of calls
 *              to other low-level driver functions. The sequence performs one
 *              operation. The end-user uses the low-level functions to
 *              optimize one or more aspects of interaction with the
 *              instrument.  Call the rsspecan_error_query function at the
 *              conclusion of the sequence to check the instrument status.
 *              Call rsspecan_AcquisitionStatus to determine when the
 *              acquisition is complete.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_Initiate (ViSession  instrSession, 
                                     ViInt32    window,
                                     ViUInt32   timeout)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";     
    ViInt32     old_timeout=-1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout));    
    sprintf (buffer, "Win%ld", window);
    
    viCheckErr (rsspecan_ClearBeforeRead (instrSession));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_INIT, 
                VI_NULL));


Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Initiate Spurious
 * Purpose:     This function starts a new spurious measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_InitiateSpurious (ViSession instrSession,
                                             ViInt32 window,
                                             ViUInt32   timeout)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";     
    ViInt32     old_timeout=-1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
        
    sprintf (buffer, "Win%ld", window);
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_INIT_SPURIOUS, 
                VI_NULL));


Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout); 
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Initiate Spectrum Emission Mask Measurement
 * Purpose:     This function starts a Spectrum Emission Mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_InitiateSEMMeasurement (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";     
    ViInt32     old_timeout=-1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout));    
    sprintf (buffer, "Win%ld", window);
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_INIT_SEM_MEAS, 
                VI_NULL));


Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Abort
 * Purpose:     This function aborts a previously initiated measurement and
 *              returns the spectrum analyzer to the idle state.
 *
 *              This function does not check the instrument status. Typically,
 *              the end-user calls this function only in a sequence of calls
 *              to other low-level driver functions. The sequence performs one
 *              operation. The end-user uses the low-level functions to
 *              optimize one or more aspects of interaction with the
 *              instrument. To check the instrument status, call the
 *              rsspecan_error_query function at the conclusion of the sequence.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_Abort (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_MEAS_ABORT, 
                VI_NULL));


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Continue
 * Purpose:     This function continues a stopped measurement at the current 
 *              position in single sweep mode. The function is useful especially 
 *              for trace functions MAXHold, MINHold and AVERage if the previous 
 *              results are not to be cleared with Sweep Count > 0 or Average 
 *              Count > 0 on restarting the measurement (INIT:IMMediate resets 
 *              the previous results on restarting the measurement).
 *              The single-sweep mode is automatically switched on.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_Continue (ViSession instrSession, 
                                     ViInt32 window,
                                     ViUInt32   timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout=-1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");  
        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    sprintf (buffer, "Win%ld", window);
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_INIT_CONMEAS, VI_NULL));   

    
Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout); 
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/******************************************************************************
 * Function: Send Software Trigger
 * Purpose:  This function sends a software trigger to the instrument.  If the
 *           instrument is not configured to receive a software trigger, this
 *           function returns an error.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SendSoftwareTrigger (ViSession instrSession,
                                     ViUInt32   timeout)
{

    ViStatus    error   = VI_SUCCESS;
    ViInt32     old_timeout=-1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");  
        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_INIT_SW_TRIGGER, 
                VI_NULL));


Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout); 
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;

}

/// HIFN  This function starts a new sequence of measurements by the Sequencer.
/// HIFN     
/// HIFN     Notes:
/// HIFN     
/// HIFN     (1) Before this function can be executed, the Sequencer must be 
/// HIFN  activated.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR timeout/This control sets the timeout value.
ViStatus _VI_FUNC rsspecan_InitiateSequencer(
    ViSession   instrSession,
    ViUInt32    timeout
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     old_timeout=-1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
        
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout));    
    
    viCheckErr (rsspecan_ClearBeforeRead (instrSession));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_INIT_SEQUENCER, VI_NULL));

Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN  This function stops the currently active sequence of measurements. The 
/// HIFN  Sequencer itself is not deactivated, so you can start a new sequence 
/// HIFN  immediately.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_AbortSequencer(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_ABORT_SEQUENCER, NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function is only available if the Sequencer is deactivated 
/// HIFN  (SYSTem:SEQuencer OFF) and only in MSRA mode. The data in the capture 
/// HIFN  buffer is re-evaluated by all active MSRA applications.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_RefreshSequencer(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_REFRESH_SEQUENCER, NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function turns the Sequencer on and off. The Sequencer must be 
/// HIFN  active before any other Sequencer commands (INIT:SEQ...) are executed, 
/// HIFN  otherwise an error will occur.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR state/This control switches the state of the sequencer. 
ViStatus _VI_FUNC rsspecan_ConfigureSequencerState(
    ViSession   instrSession,
    ViBoolean   state
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_SEQUENCER_STATE,
                                                 state), 2, "State");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function selects the way the R&S FSW application performs 
/// HIFN  measurements sequentially. Before this function can be executed, the 
/// HIFN  Sequencer must be activated.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR sequencerMode/This control selects the way the R&S FSW application performs 
/// HIPAR sequencerMode/measurements sequentially.
ViStatus _VI_FUNC rsspecan_ConfigureSequencerMode(
    ViSession   instrSession,
    ViInt32 sequencerMode
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_SEQUENCER_MODE,
                                               sequencerMode), 2, "Sequencer Mode");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*===========================================================================*
 *= RsSpecAn Spectrum Analyzer Measurements Group Functions =================*
 *===========================================================================*/
/*===========================================================================*
 *= RsSpecAn Time Domain Power Group Functions ==============================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Configure Time Domain Power Measurement
 * Purpose:  This function configures parameters of time domain power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTimeDomainPowerMeasurement (ViSession instrSession,
                                                                ViInt32 window,
                                                                ViBoolean peak,
                                                                ViBoolean RMS,
                                                                ViBoolean mean,
                                                                ViBoolean standardDeviation,
                                                                ViBoolean average,
                                                                ViBoolean maxHold,
                                                                ViInt32 power)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_PEAK,
                                                peak), 3, "Peak"); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_RMS,
                                                RMS), 4, "RMS");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_MEAN,
                                                mean), 5, "Mean");
    if (mean == VI_TRUE)
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_SDEV,
                                                standardDeviation), 6, "Standard Deviation");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_AVG,
                                                average), 7, "Average");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_MAX_HOLD,
                                                maxHold), 8, "Max Hold"); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_MODE,
                                                power), 9, "Power");                                                
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 



/*****************************************************************************
 * Function: Configure Time Domain Power Measurement State
 * Purpose:  This function switches on or off the previously selected time 
 *           domain power measurements. Thus one or several measurements can 
 *           be first selected and then switched on and off together with this
 *          attribute.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTimeDomainPowerMeasurementState (ViSession instrSession,
                                                                     ViInt32 window,
                                                                     ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_MEAS_TDOM_STATE,
                                                state), 3, "State"); 
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 
 
 

/*****************************************************************************
 * Function: Query Time Domain Power Result
 * Purpose:  This function queries selected time domain power measurement 
 *           result in the indicated measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTDomPowerResult (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 measurement,
                                                 ViInt32 resultType,
                                                 ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (measurement, 0, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Measurement");
    if (rsspecan_invalidViInt32Range (resultType, 0, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Result Type");        
    
    sprintf (buffer, "Win%ld", window);  
    switch (measurement){
        case RSSPECAN_VAL_TDOM_PEAK:
            switch (resultType){
                case RSSPECAN_VAL_TDOM_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_PEAK_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_AVG_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_AVG_PEAK_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_RESULT,
                                                                result), 5, "Result");
                break;
            }    
        break;
        case RSSPECAN_VAL_TDOM_RMS:
            switch (resultType){
                case RSSPECAN_VAL_TDOM_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_RMS_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_AVG_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_AVG_RMS_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_RMS_RESULT,
                                                                result), 5, "Result");
                break;
            }    
        break;
        case RSSPECAN_VAL_TDOM_MEAN:
            switch (resultType){
                case RSSPECAN_VAL_TDOM_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_MEAN_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_AVG_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_AVG_MEAN_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_MEAN_RESULT,
                                                                result), 5, "Result");
                break;
            }    
        break;
        case RSSPECAN_VAL_TDOM_SDEV:
            switch (resultType){
                case RSSPECAN_VAL_TDOM_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_SDEV_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_AVG_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_AVG_SDEV_RESULT,
                                                                result), 5, "Result");
                break;
                case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
                    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                                RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_SDEV_RESULT,
                                                                result), 5, "Result");
                break;
            }    
        break;
    }    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}




/*****************************************************************************
 * Function: Query Time Domain Power Pulse Sequence Result
 * Purpose:  This function is used to determine the power of a sequence of signal 
 *           pulses having the same interval, as are typical for the slots of a GSM
 *           signal, for example. The number of pulses to be measured as well
 *           as the measurement time and the period can be set. To define the
 *           position of the first pulse in the trace, a suitable offset can
 *           be entered.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_QueryTDomPowerPulseSequenceResult (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViReal64 timeOffsetofFirstPulse,
                                                              ViReal64 measurementTime,
                                                              ViReal64 period,
                                                              ViInt32 numberofPulses,
                                                              ViReal64 result[])
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (numberofPulses<=0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Number Of Pulses");
    if (result == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Result");        
    
    viCheckErr(viPrintf (instrSession, "CALC%ld:MARK:FUNC:MSUM? %.12f,%.12f,%.12f,%ld\n", 
                 window, timeOffsetofFirstPulse, measurementTime, period, numberofPulses));
    viCheckErr (viScanf (instrSession, "%,#lf", &numberofPulses, result));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Time Domain Power Measurement Set Reference
 * Purpose:  With this function the currently measured average value and RMS
 *           value are declared as reference values for relative measurements
 *           in the indicated measurement window. If the function of Averaging 
 *           or Peak Hold is switched on, the current value is the accumulated 
 *           measurement value at the time considered.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_TimeDomainPowerMeasurementSetReference (ViSession instrSession,
                                                                   ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_TDOM_SET_REFERENCE,
                                                VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Time Domain Power Measurement All Off
 * Purpose:  This function switches off all time domain measurements in 
 *           the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TimeDomainPowerMeasurementAllOff (ViSession instrSession,
                                                             ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_TDOM_AOFF,
                                                VI_NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn Power Measurement Group Functions ==============================*
 *===========================================================================*/

/*****************************************************************************
 * Function: Select Power Measurement
 * Purpose:  This function configures the power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectPowerMeasurement (ViSession instrSession,
                                                   ViInt32 window,
                                                   ViBoolean state,
                                                   ViInt32 powerMeasurement)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViBooleanRange (state) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "State");
        
    sprintf (buffer, "Win%ld", window); 
    if (state == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_POW_SELECT,
                                                    powerMeasurement), 4, "Power Measurement");
    }                                                    
    else
    {
        viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_OFF,
                                                VI_NULL));
    }                                                

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Power Results
 * Purpose:  This function returns the power measurement results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryPowerResults (ViSession instrSession,
                                            ViInt32 window,
                                            ViInt32 powerMeasurement,
                                            ViInt32 arraySize,
                                            ViReal64 results[],
                                            ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *buffer = VI_NULL;
    ViChar      *pbuff = VI_NULL; 
    ViInt32     i;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 

    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_invalidViInt32Range (powerMeasurement, 0, 6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Power Measurement");
    
    if (arraySize<0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Array Size"); 
    if ((arraySize>0) && (results == NULL))    
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Results"); 
    
    viCheckErr( viPrintf (instrSession, "CALC%ld:MARK:FUNC:POW:RES? %s\n", window, 
        rsspecan_rngMeasPowerSelect.rangeValues[powerMeasurement].cmdString));
    
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
    if (buffer) free(buffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Channel Power Standard Catalog
 * Purpose:  This function returns all predefined ACP standards and all user 
 *           standards.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryChannelPowerStandardCatalog (ViSession instrSession,
                                                             ViInt32 bufferSize,
                                                             ViChar powerStandardCatalog[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *pbuffer = VI_NULL;
    ViUInt32    count = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (viPrintf(instrSession, "CALC:MARK:FUNC:POW:STAN:CAT?\n"));
    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &pbuffer, &count)); 
    checkErr( rsspecan_CheckStatus (instrSession));
    
    if (count>=(ViUInt32)bufferSize)
    {
        strncpy(powerStandardCatalog, pbuffer, (size_t) (bufferSize-1));
        powerStandardCatalog[bufferSize-1]='\0';
        error = (ViStatus) (count+1);
    } 
    else
    {
        strncpy(powerStandardCatalog, pbuffer, (size_t) count); 
        powerStandardCatalog[count]='\0';   
    }
Error:
    if (pbuffer) free (pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Channel Power User Standard
 * Purpose:  This function configures the user standard for channel power 
 *           measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerUserStandard (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViInt32 operation,
                                                              ViString userStandardName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViAttr      attr_arr[3] = {RSSPECAN_ATTR_ACP_USER_STD_SET,
                               RSSPECAN_ATTR_ACP_USER_STD_SAVE,
                               RSSPECAN_ATTR_ACP_USER_STD_DELETE};
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (operation, RSSPECAN_VAL_USTD_SET, RSSPECAN_VAL_USTD_DEL) == VI_TRUE)  
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Operation");
   
    if (operation == RSSPECAN_ATTR_ACP_USER_STD_SET)
        sprintf (buffer, "Win%ld", window); 
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, buffer, attr_arr[operation],
                                                userStandardName), 4, "User Standard Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Power Adjust Reference Level
 * Purpose:  This function sets the reference value to the currently measured 
 *           channel power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PowerAdjustReferenceLevel (ViSession instrSession,
                                                      ViInt32 window, 
                                                      ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");         
    
    sprintf (buffer, "Win%ld", window); 
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_REF_VALUE,
                                                VI_NULL));

Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Power Preset Measurement
 * Purpose:  This function adjusts the frequency span, the measurement 
 *           bandwidths and the detector as required for the number of channels, 
 *           the channel bandwidths and the channel spacings selected in 
 *           the active power measurement. If necessary, adjacent-channel 
 *           power measurement is switched on prior to the adjustment.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PowerPresetMeasurement (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 channelPowerType,
                                                     ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout = -1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout"); 
    sprintf (buffer, "Win%ld", window); 
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_PRESET,
                                                channelPowerType), 3, "Channel Power Type");
   

Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Channel Power Standard
 * Purpose:  This function configures the standard for channel power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerStandard (ViSession instrSession,
                                                 ViInt32 window, 
                                                 ViInt32 ACPStandard)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_STANDARD,
                                                ACPStandard), 3, "ACP Standard");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Power Measurement
 * Purpose:  This function configures the adjacent power channels measurement.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_ConfigurePowerMeasurement (ViSession instrSession,ViInt32 window,ViInt32 noOfAdjChannels,ViInt32 channelMode,ViInt32 powerMode,ViBoolean resultMode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_NUM,
                                                noOfAdjChannels), 3, "No Of Adj Channels");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_MODE,
                                                channelMode), 4, "Channel Mode");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_MODE,
                                                powerMode), 5, "Power Mode");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_RESULT_MODE,
                                                resultMode), 6, "Result Mode");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Power Channel Spacing
 * Purpose:  This function configures the adjacent power channel spacing.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelSpacing (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 channel,
                                                    ViInt32 channelNumber,
                                                    ViReal64 spacing)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
     viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_invalidViInt32Range (channel, 0, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel");        
    
    switch (channel){
        case RSSPECAN_VAL_ACP_TX:
            if (strstr (model, "FSL") != NULL)
            {
                if (rsspecan_invalidViInt32Range (channelNumber, 1, 1) == VI_TRUE)
                    viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel");
            }
            else
            {
                if (rsspecan_invalidViInt32Range (channelNumber, 1, 11) == VI_TRUE)
                    viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel");  
            }
            sprintf (buffer, "Win%ld,CH%ld", window, channelNumber); 
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_CHANNEL_SPACING,
                                                spacing), 5, "Spacing");
        break;
        case RSSPECAN_VAL_ACP_ADJ:
            sprintf (buffer, "Win%ld", window); 
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_SPACING,
                                                spacing), 5, "Spacing");
        break;
        case RSSPECAN_VAL_ACP_ALT:
            if (rsspecan_invalidViInt32Range (channelNumber, 1, 11) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel");  
            sprintf (buffer, "Win%ld,CH%ld", window, channelNumber); 
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_SPACING,
                                                spacing), 5, "Spacing");
        break;
    }    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Power Channel Bandwidth
 * Purpose:  This function configures the adjacent power channel bandwidth.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelBandwidth (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 channel,
                                                      ViInt32 channelNumber,
                                                      ViReal64 bandwidth)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_invalidViInt32Range (channel, 0, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel");        
    
    switch (channel){
        case RSSPECAN_VAL_ACP_TX:
            sprintf (buffer, "Win%ld", window); 
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_CHANNEL_BANDWIDTH,
                                                bandwidth), 5, "Bandwidth");
        break;
        case RSSPECAN_VAL_ACP_ADJ:
            sprintf (buffer, "Win%ld", window); 
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_BANDWIDTH,
                                                bandwidth), 5, "Bandwidth");
        break;
        case RSSPECAN_VAL_ACP_ALT:
            if (rsspecan_invalidViInt32Range (channelNumber, 1, 11) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel");  
            sprintf (buffer, "Win%ld,CH%ld", window, channelNumber); 
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_BANDWIDTH,
                                                bandwidth), 5, "Bandwidth");
        break;
    }

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN  This function configures the power channel name.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR channel/Selects the channel type.
/// HIPAR channelNumber/Sets the channel number.
/// HIPAR name/Sets the power channel name.

ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelName (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 channel,
                                                      ViInt32 channelNumber,
                                                      ViString name)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViAttr      attr[] = {RSSPECAN_ATTR_MEAS_POW_CHANNEL_NAME,
                          RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_NAME,
                          RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_NAME}; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (channel, RSSPECAN_VAL_ACP_TX, RSSPECAN_VAL_ACP_ALT) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel");

    if (channel == RSSPECAN_VAL_ACP_ADJ)
    {
        sprintf (buffer, "Win%ld", window);
    }
    else if (channel == RSSPECAN_VAL_ACP_TX)
    {
        if (rsspecan_invalidViInt32Range (channelNumber, 1, 18) == VI_TRUE)
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Number");

        sprintf (buffer, "Win%ld,TX%ld", window, channelNumber);
    }
    else
    {
        if (rsspecan_invalidViInt32Range (channelNumber, 1, 11) == VI_TRUE)
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Channel Number");
    
        sprintf (buffer, "Win%ld,CH%ld", window, channelNumber);
    }

    viCheckParm( rsspecan_SetAttributeViString(instrSession, buffer, attr[channel], name),5, "Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: ACP Channel Power Auto Adjust
 * Purpose:  This function adapts the reference level to the measured channel 
 *           power and � if required - switches on previously the adjacent 
 *           channel power measurement. This ensures that the signal path of 
 *           the instrument is not overloaded. Since the measurement bandwidth 
 *           is significantly smaller than the signal bandwidth in channel power
 *           measurements, the signal path can be overloaded although the
 *           trace is still significantly below the reference level. If the
 *           measured channel power equals the reference level, the signal
 *           path is not overloaded.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ACPChannelPowerAutoAdjust (ViSession instrSession,
                                                      ViInt32 window, 
                                                      ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     old_timeout = -1;   
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Timeout");
        
    sprintf (buffer, "Win%ld", window); 
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL,
                                                VI_NULL));

Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Channel Power Noise Correction
 * Purpose:  This function switches on or off the correction of the instrument 
 *           inherent noise for channel power measurement. On activating this
 *           function, a reference measurement of the instrument inherent
 *           noise is performed. The measured noise power is then subtracted
 *           from the power in the examined channel. The measurement of the
 *           instrument inherent noise is then repeated after any change of
 *           the center frequency, resolution bandwidth, sweep time and level
 *           setting.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerNoiseCorrection (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean noiseCorrectionState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_NOISE_CORR,
                                                noiseCorrectionState), 3, "Noise Correction State");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function selects on, off or auto correction of the instrument inherent noise 
/// HIFN for channel power measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init 
/// HIPAR instrSession/or rsspecan_InitWithOptions function. 
/// HIPAR window/This control selects the measurement window.
/// HIPAR mode/Activates or deactivates a noise correction measurement.
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerNoiseCorrectionAuto (ViSession instrSession,
                                                                     ViInt32 window,
                                                                     ViInt32 mode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_NOISE_CORR_AUTO,
                                                mode), 3, "mode");
   
Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Channel Power Trace
 * Purpose:  This function assigns the channel/adjacent channel power measurement 
 *           of the indicated trace in the selected measurement window. The
 *           corresponding trace should be active, ie its states should
 *           differ from blank.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerTrace (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 traceNumber)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_TRACE,
                                                traceNumber), 3, "Trace Number");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Power Channel Weighting Filter
 * Purpose:  This function configures the power channel weighting filters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelWeightingFilters (ViSession instrSession,
                                                                  ViInt32 filterType,
                                                                  ViBoolean filterState,
                                                                  ViReal64 filterValue)
{
    ViStatus    error = VI_SUCCESS;
    ViAttr      attr_state[4] = {RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALL_STATE,
                                 RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ACH_STATE,
                                 RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALT_STATE,
                                 RSSPECAN_ATTR_SEM_WEIGHT_FILTER_TX_CHANNEL_STATE};
    ViAttr      attr_val[4] = {RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALL_ALPHA,
                               RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ACH_ALPHA,
                               RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALT_ALPHA,
                               RSSPECAN_ATTR_SEM_WEIGHT_FILTER_TX_CHANNEL_ALPHA};
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (filterType, RSSPECAN_VAL_WFILTER_ALL, RSSPECAN_VAL_WFILTER_TX) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Filter Type");
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "", attr_state[filterType], filterState),
                                                 3, "Filter State");
    
    if (filterState == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", attr_val[filterType], filterValue),
                                                    4, "Filter Value");
    }
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure ACP Fast ACP State
 * Purpose:  This function switches on or off the high-speed channel/adjacent 
 *           channel power measurement.
 *           The measurement itself is performed in the time domain on the
 *           center frequencies of the individual channels. The command 
 *           automatically switches to the time domain and back. Depending on 
 *           the selected mobile radio standard, weighting filters with sqrt(cos) 
 *           characteristic or very steep-sided channel filters are used for band 
 *           limitation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPFastACPState (ViSession instrSession, 
                                    ViInt32 window,
                                    ViBoolean fastACPState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_HSP,
                                                fastACPState), 3, "Fast ACP State");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: ACP Channel Power Adjust Ref Level Offset
 * Purpose:  This function defines an additional level offset to be taken into 
 *           account for the analyzer's reference level setting. It is only 
 *           available if Fast ACP is not active and no predefine standard is 
 *           selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ACPChannelPowerAdjustRefLevelOffset (ViSession instrSession,
                                                                ViInt32 window,
                                                                ViReal64 adjustRefLevelOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL_OFFSET,
                                                adjustRefLevelOffset), 3, "Adjust Ref Level Offset");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: ACP Adjust Reference Level To Channel Power
 * Purpose:  This function defines an additional level offset to be taken into 
 *           account for the analyzer's reference level setting. It is only 
 *           available if Fast ACP is not active and no predefine standard is 
 *           selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ACPAdjustReferenceLevelToChannelPower (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";     
    ViInt32     old_timeout = -1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");  
        
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    sprintf (buffer, "Win%ld", window); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL_ADJ,
                                                VI_NULL));
    
Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure ACP Limit State
 * Purpose:  This function switches on and off the limit check for adjacent 
 *           channel power measurements in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPLimitState (ViSession instrSession,
                                          ViInt32 window, 
                                          ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_LIMIT_STATE,
                                                state), 3, "State");
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure ACP Adjacent Channel Limit
 * Purpose:  This function defines the limit value of the adjacent channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPAdjacentChannelLimit (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean relativeState,
                                                    ViReal64 relativeValue,
                                                    ViBoolean absoluteState,
                                                    ViReal64 absoluteValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];    
     
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model)); 
    
    if (rsspecan_invalidViReal64Range (relativeValue, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Relative Value");
    if (rsspecan_invalidViReal64Range (absoluteValue, -200.0, 200.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Absolute Value");         
    
    sprintf (buffer, "Win%ld", window); 
    
    if (strstr (model, "FSL") != NULL)   
    { 
        viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ACH %.12lf,0.0\n", window, relativeValue));
        checkErr( rsspecan_CheckStatus (instrSession));
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_LIMIT_REL_STATE,
                                                relativeState), 3, "Relative State");

       viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ACH:ABS %.12lf,0.0\n", window, absoluteValue));
       checkErr( rsspecan_CheckStatus (instrSession));
       if (relativeState == VI_FALSE)
       {
           viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_ACP_LIMIT_ABS_STATE,
                                                    absoluteState), 5, "Absolute State"); 
       }
    }
    else
    {
        viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ACH %.12lf,0.0\n", window, relativeValue));
        checkErr( rsspecan_CheckStatus (instrSession));
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_ACP_LIMIT_REL_STATE,
                                                    relativeState), 3, "Relative State");
       viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ACH:ABS %.12lf,0.0\n", window, absoluteValue));
       checkErr( rsspecan_CheckStatus (instrSession));
       viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_ACP_LIMIT_ABS_STATE,
                                                    absoluteState), 5, "Absolute State"); 
    }
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure ACP Alternate Channel Limit
 * Purpose:  This function defines the limit value of the alternate adjacent 
 *           channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPAlternateChannelLimit (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 channelNumber,
                                                     ViBoolean relativeState,
                                                     ViReal64 relativeValue,
                                                     ViBoolean absoluteState,
                                                     ViReal64 absoluteValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));

    if (rsspecan_invalidViInt32Range (channelNumber, 1, 11) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Number");        
    if (rsspecan_invalidViReal64Range (relativeValue, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Relative Value");
    if (rsspecan_invalidViReal64Range (absoluteValue, -200.0, 200.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Absolute Value");         
    
    sprintf (buffer, "Win%ld,CH%ld", window, channelNumber); 
    
    if (strstr (model, "FSL") != NULL)   
    {   
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_REL_STATE,
                                                    relativeState), 4, "Relative State");   
        if (relativeState)
        {
            viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ALT%ld %.12lf,0.0\n", window, 
                                channelNumber, relativeValue));
            checkErr( rsspecan_CheckStatus (instrSession));                             
        }       
        
        if (absoluteState)
        {
            viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_ABS_STATE,
                                                absoluteState), 6, "Absolute State"); 
            
            viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ALT%ld:ABS %.12lf,0.0\n", window, 
                                                        channelNumber, absoluteValue));
            checkErr( rsspecan_CheckStatus (instrSession));   
        }   
    }
    else
    {
        viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ALT%ld %.12lf,0.0\n", window, 
                            channelNumber, relativeValue));
        checkErr( rsspecan_CheckStatus (instrSession));                             
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_REL_STATE,
                                                    relativeState), 4, "Relative State");
        viCheckErr (viPrintf (instrSession, "CALC%ld:LIM:ACP:ALT%ld:ABS %.12lf,0.0\n", window, 
                                                    channelNumber, absoluteValue));
        checkErr( rsspecan_CheckStatus (instrSession));   
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                    RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_ABS_STATE,
                                                    absoluteState), 6, "Absolute State"); 
    }                                           
   
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query ACP Alternate Channel Limit Check Results
 * Purpose:  This function returns the adjacent channel limit results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryACPAdjacentChannelLimitCheckResults (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViInt32 *lowerResult,
                                                              ViInt32 *upperResult)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf1[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      buf2[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (!lowerResult)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Lower Result");
    if (!upperResult)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Upper Result");        
        
    viCheckErr (viQueryf (instrSession, "CALC%ld:LIM:ACP:ACH:RES?\n","%[^,],%[^\r\n]", 
    window, buf1, buf2));
    viCheckErr (Rs_GetIndexOfToken (resultsString, ",", (ViUInt32 *)lowerResult, buf1));     
    viCheckErr (Rs_GetIndexOfToken (resultsString, ",", (ViUInt32 *)upperResult, buf2));
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}

/*****************************************************************************
 * Function: Query ACP Alternate Channel Limit Check Results
 * Purpose:  This function defines the limit value of the alternate adjacent 
 *           channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryACPAlternateChannelLimitCheckResults (ViSession instrSession,
                                                               ViInt32 window,
                                                               ViInt32 channelNumber,
                                                               ViInt32 *lowerResult,
                                                               ViInt32 *upperResult)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buf1[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      buf2[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (channelNumber, 1, 11) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Channel Number");        
    if (lowerResult == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Lower Result");
    if (upperResult == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Upper Result");        
        
    viCheckErr (viQueryf (instrSession, "CALC%ld:LIM:ACP:ALT%ld:RES?\n","%[^,],%[^\r\n]", 
    window, channelNumber, buf1, buf2));
    viCheckErr (Rs_GetIndexOfToken (resultsString, ",", (ViUInt32 *)lowerResult, buf1));     
    viCheckErr (Rs_GetIndexOfToken (resultsString, ",", (ViUInt32 *)upperResult, buf2));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
}

/*****************************************************************************
 * Function: Configure Multicarrier ACP Power
 * Purpose:  This function configures the adjacent power channels measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMulticarrierACPPower (ViSession instrSession,ViInt32 window,ViInt32 noOfAdjChannels,ViInt32 noOfTXChannels,ViInt32 channelMode,ViInt32 powerMode,ViBoolean resultMode)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_NUM,
                                                noOfAdjChannels), 3, "No Of Adj Channels");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_CARR_SIG_NUM,
                                                noOfTXChannels), 4, "No Of TX Channels");
    if (noOfAdjChannels>0)
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_MODE,
                                                channelMode), 5, "Channel Mode");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_MODE,
                                                powerMode), 6, "Power Mode");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_RESULT_MODE,
                                                resultMode), 7, "Result Mode");                                                
                                                

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure ACP Reference Channel Auto
 * Purpose:  Activates the automatic selection of a transmission channel to
 *           be used as a reference channel in relative adjacent-channel
 *           power measurements. The transmission channel with the highest
 *           power, the transmission channel with the lowest power, or the
 *           transmission channel nearest to the adjacent channels can be
 *           defined as a reference channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPReferenceChannelAuto (ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 reference)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_REF_CHAN_SEL_AUTO,
                                                reference), 3, "Reference");
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure ACP Reference Channel Man
 * Purpose:  This function selects a transmission channel to be used as a 
 *           reference channel in relative adjacent-channel power measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPReferenceChannelMan (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViInt32 channel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_ADJ_REF_TXCHANNEL,
                                                channel), 3, "Channel");
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines the number of subblocks, i.e. groups of 
/// HIFN  transmission channels in MSR signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR numberOfSubblocks/Defines the number of subblocks, i.e. groups of transmission channels 
/// HIPAR numberOfSubblocks/in MSR signal.
ViStatus _VI_FUNC rsspecan_ConfigureMSRNumberOfSubblocks (ViSession instrSession,
                                                          ViInt32 numberOfSubblocks)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_MSR_NUMBER_OF_SUBBLOCKS,
                                                numberOfSubblocks), 2, "Number Of Subblocks");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  Subblocks are groups of transmit channels in an MSR signal. Up to 5 
/// HIFN  subblocks can be defined. They are defined as an RF bandwidth around a 
/// HIFN  center frequency with a specific number of transmit channels (max. 18).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR centerFrequency/Defines the center of the specified MSR subblock. Note that the 
/// HIPAR centerFrequency/position of the subblock also affects the position of the adjacent gap 
/// HIPAR centerFrequency/(CACLR) channels.
/// HIPAR subblock/This control selects the subblock number.
/// HIPAR RFBandwidth/Defines the bandwidth of the individual MSR subblock. Note that 
/// HIPAR RFBandwidth/subblock ranges also affect the position of the adjacent gap channels 
/// HIPAR RFBandwidth/(CACLR).
/// HIPAR txChannelCount/Defines the bandwidth of the individual MSR subblock. Note that 
/// HIPAR txChannelCount/subblock ranges also affect the position of the adjacent gap channels 
/// HIPAR txChannelCount/(CACLR).
ViStatus _VI_FUNC rsspecan_ConfigureMSRSubblockDefinition (ViSession instrSession,
                                                           ViInt32 subblock,
                                                           ViReal64 centerFrequency,
                                                           ViReal64 RFBandwidth,
                                                           ViInt32 txChannelCount)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (subblock, 1, 5) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Subblock"); 
    
    sprintf (buffer, "SB%ld", subblock); 
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_SUBBLOCK_CENTER_FREQUENCY,
                                                centerFrequency), 3, "Center Frequency");
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_SUBBLOCK_RF_BANDWIDTH,
                                                RFBandwidth), 4, "RF Bandwidth");
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_SUBBLOCK_TX_CHANNEL_COUNT,
                                                txChannelCount), 5, "Tx Channel Count"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  As opposed to common ACLR channel definitions, the TX channels are 
/// HIFN  defined at absolute frequencies, rather than by a spacing relative to 
/// HIFN  the (common) center frequency.
/// HIFN  Each transmit channel can be assigned a different technology, used to 
/// HIFN  predefine the required bandwidth.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR subblock/This control selects the subblock number.
/// HIPAR txChannel/Tx channel number.
/// HIPAR txCenterFrequency/This control defines the (absolute) center frequency of the specified 
/// HIPAR txCenterFrequency/MSR Tx channel.
/// HIPAR technology/This control defines the technology used for transmission by the 
/// HIPAR technology/specified MSR Tx channel.
/// HIPAR txBandwidth/Defines the bandwidth of the specified MSR Tx channel.
/// HIPAR weightingFilterState/This control turns the weighting filter transmission channel on and 
/// HIPAR weightingFilterState/off.
/// HIPAR rollOffFactor/This control defines the roll-off factor for the specified 
/// HIPAR rollOffFactor/transmission channel's weighting filter.
ViStatus _VI_FUNC rsspecan_ConfigureMSRTxChannelDefinition (ViSession instrSession,
                                                            ViInt32 subblock,
                                                            ViInt32 txChannel,
                                                            ViReal64 txCenterFrequency,
                                                            ViInt32 technology,
                                                            ViReal64 txBandwidth,
                                                            ViBoolean weightingFilterState,
                                                            ViReal64 rollOffFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (subblock, 1, 5) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Subblock"); 
    
    if (rsspecan_invalidViInt32Range (txChannel, 1, 5) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Tx Channel");
    
    sprintf (buffer, "SB%ld,TX%ld", subblock, txChannel); 
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_TX_CENTER_FREQUENCY,
                                                txCenterFrequency), 4, "Tx Center Frequency");
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_TX_TECHNOLOGY,
                                                technology), 5, "Technology");
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_TX_BANDWIDTH,
                                                txBandwidth), 6, "Tx Bandwidth");
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_TX_WEIGHTING_FILTER_STATE,
                                                weightingFilterState), 7, "Weighting Filter State"); 
    
    if (weightingFilterState == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_TX_WEIGHTING_FILTER_ROLL_OFF_FACTOR,
                                                rollOffFactor), 8, "Roll Off Factor"); 
    }
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function queries the name of the specified MSR Tx channel.
/// HIFN  In MSR ACLR measurements, TX channel names correspond to the specified 
/// HIFN  technology, followed by a consecutive number. The assigned subblock 
/// HIFN  (A,B,C,D,E) is indicated as a prefix (e.g. A: WCDMA1). Channel names 
/// HIFN  cannot be defined manually.    
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR subblock/This control selects the subblock number.
/// HIPAR txChannel/Tx channel number.
/// HIPAR name/String containing the name of the channel.
ViStatus _VI_FUNC rsspecan_QueryMSRTxName (ViSession instrSession,
                                           ViInt32 subblock, ViInt32 txChannel,
                                           ViInt32 bufferSize,
                                           ViChar name[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (subblock, 1, 5) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Subblock"); 
    
    if (rsspecan_invalidViInt32Range (txChannel, 1, 5) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Tx Channel");
    
    sprintf (buffer, "SB%ld,TX%ld", subblock, txChannel); 
    
    viCheckParm( rsspecan_GetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_MSR_TX_CHANNEL_NAME, 
                                                bufferSize, name), 4, "Name"); 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines the distance from subblock to the specified gap 
/// HIFN  channel, the bandwidth of the specified MSR gap (CACLR) channel, and 
/// HIFN  the weighting filter.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR gap/This control selects the measurement window.
/// HIPAR channelSpacing/Defines the distance from subblock to the specified gap channel.
/// HIPAR channelBandwidths/Defines the bandwidth of the specified MSR gap (CACLR) channel.
/// HIPAR weightingFilterState/This control turns the weighting filter for the specified gap (CACLR) 
/// HIPAR weightingFilterState/channel on and off.
/// HIPAR rollOffFactor/This control defines the roll-off factor for the specified gap (CACLR) 
/// HIPAR rollOffFactor/channel's weighting filter.
ViStatus _VI_FUNC rsspecan_ConfigureMSRGapChannel (ViSession instrSession,
                                                   ViInt32 gap,
                                                   ViReal64 channelSpacing,
                                                   ViReal64 channelBandwidths,
                                                   ViBoolean weightingFilterState,
                                                   ViReal64 rollOffFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (gap, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Gap"); 
    
    sprintf (buffer, "Gap%ld", gap); 
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_GAP_CHANNEL_SPACING,
                                                channelSpacing), 3, "channelSpacing");
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_GAP_CHANNEL_BANDWIDTHS,
                                                channelBandwidths), 4, "channelBandwidths");
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_GAP_WEIGHTING_FILTER_STATE,
                                                weightingFilterState), 5, "Weighting Filter State"); 
    
    if (weightingFilterState == VI_TRUE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_GAP_WEIGHTING_FILTER_ROLL_OFF_FACTOR,
                                                rollOffFactor), 6, "Roll Off Factor"); 
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines the limit value of the gap (CACLR) channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR gap/This control selects the measurement window.
/// HIPAR relativeState/Turns the relative limit check for the specified gap (CACLR) channel 
/// HIPAR relativeState/on and off.
/// HIPAR relativeValue/Defines the relative limit of the specified gap (CACLR) channel. The 
/// HIPAR relativeValue/reference value for the relative limit is the measured channel power.
/// HIPAR absoluteState/Turns the absolute limit check for the specified gap (CACLR) channel 
/// HIPAR absoluteState/on and off.
/// HIPAR absoluteValue/Defines the absolute limit of the specified gap (CACLR) channel.
ViStatus _VI_FUNC rsspecan_ConfigureMSRGapLimitChecking (ViSession instrSession,
                                                         ViInt32 gap,
                                                         ViBoolean relativeState,
                                                         ViReal64 relativeValue,
                                                         ViBoolean absoluteState,
                                                         ViReal64 absoluteValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (gap, 1, 2) == VI_TRUE)
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Gap"); 
    
    sprintf (buffer, "Gap%ld", gap); 
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_GAP_LIMIT_RELATIVE_STATE,
                                                relativeState), 3, "Relative State");
    if (relativeState)
    {
        viCheckErr (viPrintf (instrSession, "CALC:LIM:ACP:GAP%ld:REL %.12lf,0.0\n", gap, relativeValue));
        checkErr (rsspecan_CheckStatus (instrSession));
    }
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MSR_GAP_LIMIT_ABSOLUTE_STATE,
                                                absoluteState), 5, "Absolute State"); 
    if (absoluteState)
    {
        viCheckErr (viPrintf (instrSession, "CALC:LIM:ACP:GAP%ld:ABS %.12lf,0.0\n", gap, absoluteValue));
        checkErr (rsspecan_CheckStatus (instrSession));
    }
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*
 *= RsSpecAn Occupied Bandwidth Group Functions =============================*
 *===========================================================================*/

/*****************************************************************************
 * Function: Configure Occupied Bandwidth
 * Purpose:  This function defines the percentage of the power with respect 
 *           to the total power. This value is the basis for the occupied 
 *           bandwidth measurement (command: POWer:ACHannel:PRESet OBW).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureOccupiedBandwidth (ViSession instrSession,
                                                       ViInt32 window,
                                                       ViReal64 powerBandwidth,
                                                       ViReal64 channelBandwidth)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_BANDWIDTH,
                                                powerBandwidth), 4, "Power Bandwidth");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_POW_CHANNEL_BANDWIDTH,
                                                channelBandwidth), 5, "Channel Bandwidth");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/*===========================================================================*
 *= RsSpecAn Signal Statistics Group Functions ==============================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Configure Signal Statistic Measurement
 * Purpose:  This function configures parameters of signal statistic measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurement (ViSession instrSession,
                                                                ViInt32 measurement,
                                                                ViInt32 numberofSamples,
                                                                ViReal64 resolutionBandwidth)
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
                                                RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE,
                                                VI_FALSE));
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_APD_STATE,
                                                VI_TRUE));                                                
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_SAMPLES,
                                                numberofSamples), 4, "Number Of Samples");                                                
        break;
        case RSSPECAN_VAL_SSTAT_CCDF:      
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_APD_STATE,
                                                VI_FALSE));
            viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE,
                                                VI_TRUE));
            viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_SAMPLES,
                                                numberofSamples), 3, "Number Of Samples");                                                
        break;
    }
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "Win0",
                                                RSSPECAN_ATTR_RESOLUTION_BANDWIDTH,
                                                resolutionBandwidth), 4, "Resolution Bandwidth"); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Sweep
 * Purpose:  This function configures the acquisition attributes of signal statistic.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticSweep (ViSession instrSession,
                                                          ViBoolean sweepModeContinuous)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "Win0", 
                                                RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS,
                                                sweepModeContinuous), 2, "Sweep Mode Continuous");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Mean Power Position
 * Purpose:  This function defines the relative X position of the mean power 
 *           value for the CCDF measurement. The default position is 0% (left 
 *           corner of the grid). This function is only available if the statistics
 *           measurement function CCDF is switched on.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeanPowerPosition (ViSession instrSession,
                                                                      ViReal64 meanPowerPosition)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_MEAS_STAT_MEAN_POWER_POS,
                                                meanPowerPosition), 2, "Mean Power Position");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Signal Statistic Measurement Adjust Settings
 * Purpose:  This function optimizes the level setting of the instrument 
 *           depending on the measured peak power, in order to obtain maximum 
 *           instrument sensitivity.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementAdjustSettings (ViSession instrSession, 
                                                                     ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
     if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout"); 
        
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_ADJ,
                                                VI_NULL));

Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Signal Statistic Measurement Reference Level Adjust
 * Purpose:  This function adjusts the reference level to get an optimized 
 *           sensitivity. Only the reference level is changed all other setting 
 *           parameters are kept.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementRefLevelAdjust (ViSession instrSession, 
                                                                     ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout = -1; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");  
        
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_REF_LEVEL_ADJUST,
                                                VI_NULL));

Error:
    if (old_timeout >= 0)    
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Measurement X Axis
 * Purpose:  This function configures parameters of signal statistic 
 *           measurement X-Axis scaling.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurementXAxis (ViSession instrSession,
                                                                     ViReal64 xAxisRefLevel,
                                                                     ViReal64 xAxisRange)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_X_REF,
                                                xAxisRefLevel), 2, "X Axis Ref Level");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_X_RANGE,
                                                xAxisRange), 3, "X Axis Range");                                                
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Measurement Y Axis
 * Purpose:  This function configures parameters of signal statistic 
 *           measurement Y Axis scaling.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurementYAxis (ViSession instrSession,ViInt32 yUnit,ViReal64 yAxisMaxValue,ViReal64 yAxisMinValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL") == NULL)
    {
        viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                    RSSPECAN_ATTR_MEAS_STAT_Y_UNIT,
                                                    yUnit), 2, "Y Unit");    
    }
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_Y_MAX,
                                                yAxisMaxValue), 3, "Y Axis Max Value");  
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_Y_MIN,
                                                yAxisMinValue), 4, "Y Axis Min Value");                                                  
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Signal Statistic Measurement Preset Scaling
 * Purpose:  This function optimizes the level setting of the instrument 
 *           depending on the measured peak power, in order to obtain maximum 
 *           instrument sensitivity.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementPresetScaling (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_STAT_PRESET,
                                                VI_NULL));
   

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Signal Statistic CCDF Level
 * Purpose:  This function reads out the level values for the probabilities 
 *           0.1%, 1% and 10%.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySignalStatisticCCDFLevel (ViSession instrSession,
                                                          ViInt32 trace,
                                                          ViInt32 probability,
                                                          ViReal64 *result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    if (rsspecan_invalidViInt32Range (trace, 1, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Trace");
    if (rsspecan_invalidViInt32Range (probability, 0, 3) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Probability"); 
    if (result == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Result");         
    
    viCheckErr (viQueryf (instrSession, "CALC:STAT:CCDF:X%ld? %s\n", "%le", 
                trace, statProbArr[probability], result));
                
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Signal Statistic Result
 * Purpose:  This function reads out the results of statistical measurements 
 *           of a recorded trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySignalStatisticResult (ViSession instrSession,
                                                       ViInt32 trace,
                                                       ViInt32 resultType,
                                                       ViReal64 *result)
{
    ViStatus   error = VI_SUCCESS;
    ViInt32    trace_range = 3;
    ViString   statisticMeasType[] = {"Mean","Peak","CrestFactor"};  
    ViChar     buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));
    if (strstr (buffer, "FSL"))
        trace_range=4;
        
    if (rsspecan_IsFSV (instrSession))
        trace_range=6;
        
    if (rsspecan_invalidViInt32Range (trace, 1, trace_range) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Trace");
   
    sprintf (buffer, "TR%ld,Stat%s", trace, statisticMeasType[resultType]); 
    
    viCheckErr( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_STAT_RESULT,
                                                result));


Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/*===========================================================================*
 *= RsSpecAn Modulation Depth Group Functions ===============================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Configure Modulation Depth Measurement
 * Purpose:  This function switches on the measurement of the AM modulation 
 *           depth. An AM-modulated carrier is required on the screen for 
 *           correct operation. If necessary, marker 1 is previously activated 
 *           and set to the largest signal available.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureModDepthMeasurement (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                            RSSPECAN_ATTR_MEAS_MDEPTH_STATE,
                                            state), 3, "State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 
 

/*****************************************************************************
 * Function: Query Modulation Depth
 * Purpose:  This function queries the AM modulation depth in the indicated 
 *           measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryModulationDepth (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViReal64 *modulationDepth)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                            RSSPECAN_ATTR_MEAS_MDEPTH_RESULT,
                                            modulationDepth), 3, "Modulation Depth");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Modulation Depth Signal Search
 * Purpose:  This function starts the search of the signals required for 
 *           the modulation depth measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ModulationDepthSignalSearch (ViSession instrSession,
                                                        ViInt32 window, 
                                                        ViInt32 marker)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_MDEPTH_SEARCH,
                                                NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn Third Order Intercept Group Functions ==========================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Configure TOI Measurement
 * Purpose:  This function initiates the measurement of the third-order 
 *           intercept point.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTOIMeasurement (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_TOI_STATE,
                                                state), 3, "State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query TOI
 * Purpose:  This function queries the third-order intercept point measurement 
 *           in the indicated measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTOI (ViSession instrSession, 
                                     ViInt32 window,
                                     ViReal64 *TOIMeasurement)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_TOI_RESULT,
                                                TOIMeasurement), 3, "TOI Measurement");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: TOI Signal Search
 * Purpose:  This function starts the search of the signals required for the 
 *           third order intercept measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TOISignalSearch (ViSession instrSession,
                                            ViInt32 window, ViInt32 marker)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     max_marker = 4;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (rsspecan_IsFSV (instrSession))
        max_marker = 16;

    if (rsspecan_invalidViInt32Range (marker, 1, max_marker) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Marker");        
    
    sprintf (buffer, "Win%ld,M%ld", window, marker); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_TOI_SEARCH,
                                                NULL));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure TOI Positioning
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTOIPositioning (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 interceptMarkersPositioning)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window); 
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                RSSPECAN_ATTR_MEAS_TOI_POSITIONING,
                                                interceptMarkersPositioning), 3, 
                                                "Intercept Markers Positioning");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn Harmonic Distortion Group Functions ============================*
 *===========================================================================*/

/*****************************************************************************
 * Function: Configure Harmonic Distortion Measurement
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHDistMeasurement (ViSession instrSession,
                                                      ViBoolean state,
                                                      ViInt32 noofHarmonics,
                                                      ViBoolean harmonicRBWAuto)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_HDIST_STATE,
                                                state), 3, "State");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_HDIST_NOOFHARM,
                                                noofHarmonics), 4, "No Of Harmonic");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_HDIST_RBWAUTO,
                                                harmonicRBWAuto), 5, "Harmonic RBW Auto");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Harmonic Distortion Measurement
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustHDistSettings (ViSession instrSession,
                                                ViUInt32    timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     old_timeout=-1;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout"); 
    viCheckErr( rsspecan_GetOPCTimeout (instrSession, &old_timeout));  
    viCheckErr( rsspecan_SetOPCTimeout (instrSession, timeout)); 
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_MEAS_HDIST_PRESET,
                                                VI_NULL));
   
Error:
    if (old_timeout > 0) 
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Harmonic Distortion
 * Purpose:  This function queries the results of the total harmonic distortion 
 *           (THD).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryHDist (ViSession instrSession,
                                       ViReal64 *harmonicDistortion_in_percent,
                                       ViReal64 *harmonicDistortion_in_dB)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if (strstr(model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (!harmonicDistortion_in_percent)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Harmonic Distortion in percent");
     if (!harmonicDistortion_in_dB)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Harmonic Distortion in dB");                 
    
    viCheckErr (viQueryf (instrSession, ":CALC:MARK:FUNC:HARM:DIST? TOT\n", "%le,%le", 
                harmonicDistortion_in_percent, harmonicDistortion_in_dB));
                
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Harmonic Distortion Harmonics List
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryHDistHarmonicsList (ViSession instrSession,
                                                    ViReal64 harmonicsList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if (strstr(model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (harmonicsList == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Harmonic List");
    
    viCheckErr (viQueryf (instrSession, "CALC:MARK:FUNC:HARM:LIST?\n", "%,255lf", 
                harmonicsList));
                
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query Harmonic Distortion Bandwidth List
 * Purpose:  This function returns the resolution bandwidth values used by the 
 *           harmonic measurment. A separate value for every harmonic frequency 
 *           is automatically calculated. It is possible to query the calculated
 *           values with this function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryHDistBandwidthList (ViSession instrSession,
                                                    ViInt32 arraySize,
                                                    ViReal64 resultValues[],
                                                    ViInt32 *returnedValues)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     ptag;
    ViInt32     index;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_IsFSV (instrSession))
        return RS_ERROR_INSTRUMENT_MODEL;
    
    if (arraySize <= 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
        
    viCheckErr (viQueryf(instrSession, "CALC:MARK:FUNC:HARM:BAND:LIST?\n","%s", buffer));
    
    ptag = strtok(buffer, ",");
    for (index = 0; ptag; index++)
    {
        if (index < arraySize)
            resultValues[index] = atof(ptag);
        ptag = strtok(NULL, ",");
    }
    if (returnedValues)
        *returnedValues=index;
    checkErr( rsspecan_CheckStatus (instrSession));  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*
 *= RsSpecAn Burst Power Group Functions ====================================*
 *===========================================================================*/

/*****************************************************************************
 * Function: Burst Power Sequence
 * Purpose:  This function configures the instrument setup for multiple burst 
 *           power measurement and starts a measurement sequence. To reduce the
 *           setting time, the setup is performed simultaneously for all
 *           selected parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BurstPowerSequence (ViSession instrSession,
                                               ViInt32 window,
                                               ViReal64 analyzerFrequencyHz,
                                               ViReal64 resolutionBandwidthHz,
                                               ViReal64 measTimes,
                                               ViInt32 triggerSource,
                                               ViReal64 triggerLevel,
                                               ViReal64 triggerOffsets,
                                               ViInt32 type_ofMeas,
                                               ViInt32 noofMeasurements)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if (strstr(model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if (analyzerFrequencyHz<0.0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Analyzer Frequency [Hz]");
    if (rsspecan_invalidViReal64Range (resolutionBandwidthHz, 10.0, 10.0e+6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Resolution Bandwidth [Hz]");
    if (rsspecan_invalidViReal64Range (measTimes, 1.0e-6, 30.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Meas Time [s]");        
    if ((rsspecan_invalidViInt32Range (triggerSource, RSSPECAN_VAL_TRG_EXT, RSSPECAN_VAL_TRG_EXT) == VI_TRUE)
            && (rsspecan_invalidViInt32Range (triggerSource, RSSPECAN_VAL_TRG_VID,RSSPECAN_VAL_TRG_VID) == VI_TRUE))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Trigger Source");
    if (rsspecan_invalidViReal64Range (triggerLevel, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Trigger Level");  
    if (rsspecan_invalidViReal64Range (triggerOffsets, 125.0e-9, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Trigger Offset [s]");  
    if (rsspecan_invalidViInt32Range (type_ofMeas, 0, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Type Of Meas");
    if (rsspecan_invalidViInt32Range (noofMeasurements, 1, 501) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "No Of Measurements");
        
    viCheckErr( viPrintf (instrSession, "SENS%ld:MPOW:SEQ %.12lG,%.12lG,%.12lG,%s,%.12lG,%.12lG,%s,%ld\n",
    window, analyzerFrequencyHz,resolutionBandwidthHz,measTimes,rsspecan_rngTriggerSource.rangeValues[triggerSource].cmdString,
    triggerLevel,triggerOffsets,measTypeArr[type_ofMeas],noofMeasurements));
   
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Burst Power Filter Type
 * Purpose:  This function the filter type for the measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BurstPowerFilterType (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 filterType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";  
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);  
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_MPOW_FTYPE,
                                                filterType), 2, "Filter Type"); 
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Get Burst Power Sequence
 * Purpose:  This function configures the instrument setup for multiple burst 
 *           power measurement, starts a measurement sequence and reads the
 *           results. To reduce the setting time, the setup is performed
 *           simultaneously for all selected parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBurstPowerSequence (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViReal64 analyzerFrequencyHz,
                                                  ViReal64 resolutionBandwidthHz,
                                                  ViReal64 measTimes,
                                                  ViInt32 triggerSource,
                                                  ViReal64 triggerLevel,
                                                  ViReal64 triggerOffsets,
                                                  ViInt32 type_ofMeas,
                                                  ViInt32 no_ofMeasurements,
                                                  ViUInt32 timeout,
                                                  ViReal64 burstPowerResults[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if (strstr(model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if (analyzerFrequencyHz<0.0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Analyzer Frequency [Hz]");
    if (rsspecan_invalidViReal64Range (resolutionBandwidthHz, 10.0, 10.0e+6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Resolution Bandwidth [Hz]");
    if (rsspecan_invalidViReal64Range (measTimes, 1.0e-6, 30.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Meas Time [s]");        
    if ((rsspecan_invalidViInt32Range (triggerSource, RSSPECAN_VAL_TRG_EXT, RSSPECAN_VAL_TRG_EXT) == VI_TRUE)
            && (rsspecan_invalidViInt32Range (triggerSource, RSSPECAN_VAL_TRG_VID,RSSPECAN_VAL_TRG_VID) == VI_TRUE))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Trigger Source");
    if (rsspecan_invalidViReal64Range (triggerLevel, 0.0, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Trigger Level");  
    if (rsspecan_invalidViReal64Range (triggerOffsets, 125.0e-9, 100.0) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Trigger Offset [s]");  
    if (rsspecan_invalidViInt32Range (type_ofMeas, 0, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Type Of Meas");
    if (rsspecan_invalidViInt32Range (no_ofMeasurements, 1, 501) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "No Of Measurements");
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Timeout");        
    if (burstPowerResults == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 12, "Burst Power Results");        
        
    viCheckErr( viPrintf (instrSession, "*CLS;:SENS%ld:MPOW:SEQ? %.12lf,%.12lf,%.12lf,%s,%.12lf,%.12lf,%s,%ld;*OPC\n",
    window, analyzerFrequencyHz,resolutionBandwidthHz,measTimes,rsspecan_rngTriggerSource.rangeValues[triggerSource].cmdString,
    triggerLevel,triggerOffsets,measTypeArr[type_ofMeas],no_ofMeasurements));
    viCheckErr( rsspecan_WaitForOPC (instrSession, timeout));
    viCheckErr( viScanf (instrSession, "%,#lf", &no_ofMeasurements, burstPowerResults));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Burst Power Results
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBurstPowerResults (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 no_ofResults,
                                                 ViReal64 burstPowerResults[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
 
    if (strstr(model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if (no_ofResults<=0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "No Of Results");
    if (burstPowerResults == NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "burstPowerResults");        
        
    viCheckErr( viQueryf (instrSession, "*WAI;SENS%ld:MPOW:RES?\n", "%,#lf",
    window, &no_ofResults, burstPowerResults));
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Burst Power Result Min
 * Purpose:  This function queries the minimum power value in a multiple burst 
 *           power measurement as configured and initiated with Burst Power
 *           Sequence (confBurstPwrSeq). The unit used for the return values
 *           is always dBm. If no measurement result is available, the
 *           function will return a query error.
 *****************************************************************************/
#pragma warning( push )
#pragma warning( disable : 4100)
ViStatus _VI_FUNC rsspecan_GetBurstPwrResultMin (ViSession instrSession,
                                                 ViInt32 window,
                                                 ViReal64 *powerResultMin)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    
    
    viCheckParm( rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_MPOW_MIN, 
                powerResultMin), 3, "Power Result Min");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
#pragma warning( pop )

/*****************************************************************************
 * Function: Query ViBoolean
 * Purpose:  This function queries the ViBoolean value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryViBoolean (ViSession instrSession,
                                           ViString command, 
                                           ViBoolean *value)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, (ViInt32)strlen(command), 1, 10000000), 2, "Command (null string length)");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, value), 3, "Value");

	checkErr(RsCore_QueryViBoolean(instrSession, command, value));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query ViInt32
 * Purpose:  This function queries the ViInt32 value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryViInt32 (ViSession instrSession,
                                         ViString command, 
                                         ViInt32 *value)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, (ViInt32)strlen(command), 1, 10000000), 2, "Command (null string length)");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, value), 3, "Value");

	checkErr(RsCore_QueryViInt32(instrSession, command, value));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query ViReal64
 * Purpose:  This function queries the ViReal64 value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryViReal64 (ViSession instrSession,
                                          ViString command, 
                                          ViReal64 *value)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, (ViInt32)strlen(command), 1, 10000000), 2, "Command (null string length)");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, value), 3, "Value");

	checkErr(RsCore_QueryViReal64(instrSession, command, value));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Query ViString
 * Purpose:  This function queries the ViString value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryViString (ViSession instrSession,
                                          ViString command, 
                                          ViInt32 bufferSize,
                                          ViChar _VI_FAR value[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* response = NULL;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, (ViInt32)strlen(command), 1, 10000000), 2, "Command (null string length)");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, value), 3, "Value");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, command, &response));

	checkErr(rsspecan_CheckStatus(instrSession));

	checkErr(RsCore_CopyToUserBufferAsciiData(instrSession, value, bufferSize, response));

Error:
	if (response)
		free(response);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*Status Register Subsystem */
/*===========================================================================*/
/* Function: Set Status Register                                             */
/* Purpose:  This Function sets the Enable, NTransition, and PTransition bits*/
/*           of the Main, Power, Limit, Left Margin, and Sync status         */
/*           questionable registers.                                         */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_setStatusRegister (ViSession instrSession,
                ViInt32 registerOper, ViInt32 selStatusReg, ViInt32 enable,
                ViInt32 PTransition, ViInt32 NTransition)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));     
    if (rsspecan_invalidViInt32Range (registerOper, 0, 2) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Register Oper");
    }
    
    switch (registerOper) {
        case 0:
            viCheckErr (viPrintf (instrSession, ":STAT:PRES\n"));
        break;
        case 1:
            if (rsspecan_invalidViInt32Range (selStatusReg, 0, 23))
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Questionable Register");
            

            viCheckErr (viPrintf (instrSession, ":STAT:QUES%s:ENAB %ld;PTR %ld"
                ";NTR %ld\n", selStatusRegArr[selStatusReg], enable, PTransition,
                NTransition));
        break;
        case 2:
            if (rsspecan_invalidViInt32Range (enable, 0, 65535))
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Enable");
            if (rsspecan_invalidViInt32Range (PTransition, 0, 65535))
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "PTransition");
            if (rsspecan_invalidViInt32Range (NTransition, 0, 65535))
                viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "NTransition");

            viCheckErr (viPrintf (instrSession, ":STAT:OPER:ENAB %ld;PTR %ld"
                ";NTR %ld\n", enable, PTransition, NTransition));
        break;
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Get Status Register                                             */
/* Purpose:  This function contains the commands for the querying status     */
/*           reporting system.                                               */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_getStatusRegister (ViSession instrSession,
                ViInt32 statusReg, ViPInt32 registerValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (statusReg, 0, 52) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Status Register");
    }
    
    viCheckErr( viQueryf (instrSession, ":STAT:%s?\n", "%ld",
        statusRegArr[statusReg], registerValue));
    checkErr( rsspecan_CheckStatus (instrSession)); 

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function sets the Enable, NTransition, and PTransition bits
/// HIFN of the Main, and Carrier Frequency, FSK, IQ Offset &  RHO,
/// HIFN Magnitude Error, Phase Error.
/// HIFN This register comprises information about any limit violations
/// HIFN that may occur after demodulation in any of the four windows.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR registerOperation/Specifies the operation to perform.
/// HIPAR questionableRegister/This control selects the registers that will be configured.
/// HIPAR enable/Document This control sets the enable bits of the status register.
/// HIPAR PTransition/This control sets the PTransition bits (edge detectors) of the status register.
/// HIPAR NTransition/This control sets the NTransition bits (edge detectors) of the status register.
ViStatus _VI_FUNC rsspecan_setVSAModulationStatusRegister (ViSession instrSession,
                                                           ViInt32   window,
                                                           ViInt32   registerOperation,
                                                           ViInt32   questionableRegister,
                                                           ViInt32   enable,
                                                           ViInt32   PTransition,
                                                           ViInt32   NTransition)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap[40];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (registerOperation, 1, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Register Operation");
    
    if (rsspecan_invalidViInt32Range (questionableRegister, 0, 6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Questionable Register");
    
    sprintf(rep_cap, "Win%ld,%s,RegEnable", window, VSAResultRegArr[questionableRegister]);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER,
                                               enable
                                               ), 5, "Enable");
    
    sprintf(rep_cap, "Win%ld,%s,RegNTR", window, VSAResultRegArr[questionableRegister]);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER,
                                               PTransition
                                               ), 6, "PTransition");
    
    sprintf(rep_cap, "Win%ld,%s,RegPTR", window, VSAResultRegArr[questionableRegister]);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER,
                                               NTransition
                                               ), 7, "NTransition");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function contains the commands for the querying status reporting system.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR questionableRegister/ This control selects the registers that will be configured.
/// HIPAR statusRegistersQuery/This control selects the registers to be queried.
/// HIPAR registerValue/Returns content of selected register.
ViStatus _VI_FUNC rsspecan_getVSAModulationStatusRegister (ViSession instrSession,
                                                           ViInt32   window,
                                                           ViInt32   questionableRegister,
                                                           ViInt32   statusRegistersQuery,
                                                           ViInt32*  registerValue)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      rep_cap[40];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (questionableRegister, 0, 6) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Questionable Register");
    
    if (rsspecan_invalidViInt32Range (statusRegistersQuery, 0, 1) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Status Registers Query");
    
    sprintf(rep_cap, "Win%ld,%s,%s", window, VSAResultRegArr[questionableRegister], RegisterPartArr[statusRegistersQuery]);
    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER,
                                               registerValue
                                               ), 5, "Register Value");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*===========================================================================*/
/* Function: Condition Frequency Register                                    */
/* Purpose:  This function reads Condition Frequency Register.               */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionFrequencyRegister (ViSession instrSession,
                                                    ViPInt32 statusRegisterValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (statusRegisterValue == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Status Register Value");
    }
    
    viCheckErr( rsspecan_getStatusRegister(instrSession, 15, statusRegisterValue));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Condition Limit Register                                        */
/* Purpose:  This function reads Condition Limit Register.                   */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionLimitRegister (ViSession instrSession,
                                                ViInt32 window,
                                                ViPInt32 statusRegisterValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (statusRegisterValue == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Status Register Value");
    }
    
    switch(window){
        case 1:
            viCheckErr( rsspecan_getStatusRegister(instrSession, 7, statusRegisterValue));
        break;
        case 2:
            viCheckErr( rsspecan_getStatusRegister(instrSession, 20, statusRegisterValue));
    }       

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Condition Limit Margin Register                                 */
/* Purpose:  This function reads Condition Limit Margin Register.            */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionLimitMarginRegister (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViPInt32 statusRegisterValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (statusRegisterValue == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Status Register Value");
    }
    
    switch(window){
        case 1:
            viCheckErr( rsspecan_getStatusRegister(instrSession, 9, statusRegisterValue));
        break;
        case 2:
            viCheckErr( rsspecan_getStatusRegister(instrSession, 22, statusRegisterValue));
    }       

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;    
        
}
/*===========================================================================*/
/* Function: Condition ACP Limit Register                                    */
/* Purpose:  This function reads Condition ACP Limit Register.               */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionACPLimitRegister (ViSession instrSession,
                                                   ViPInt32 statusRegisterValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (statusRegisterValue == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Status Register Value");
    }
    
    viCheckErr( rsspecan_getStatusRegister(instrSession, 13, statusRegisterValue));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Condition Power Register                                        */
/* Purpose:  This function reads Condition Power Register.                   */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionPowerRegister (ViSession instrSession,
                                                ViPInt32 statusRegisterValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (statusRegisterValue == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Status Register Value");
    }
    
    viCheckErr( rsspecan_getStatusRegister(instrSession, 5, statusRegisterValue));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Condition Sync Register                                         */
/* Purpose:  This function reads Condition Sync Register.                    */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionSyncRegister (ViSession instrSession,
                                               ViPInt32 statusRegisterValue)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (statusRegisterValue == VI_NULL)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Status Register Value");
    }
    
    viCheckErr( rsspecan_getStatusRegister(instrSession, 11, statusRegisterValue));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}
/*End of Status Register Subsystem*/

/*****************************************************************************
 * Function: rsspecan_RsClose
 * Purpose:  This function performs all of the drivers clean-up operations
 *           except for closing the RS session.  This function is called by
 *           rsspecan_close or by an RS class driver.
 *
 *           Note:  This function must close the I/O session and set
 *           RS_ATTR_IO_SESSION to 0.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RsClose (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
    ViSession   io      = 0;

    /* Do not lock here. The caller manages the lock. */

    checkErr( Rs_GetAttribute (instrSession, NULL, RS_ATTR_IO_SESSION, 0, (ViInt32) sizeof (io), &io));

Error:

    (void) Rs_SetAttribute (instrSession, NULL, RS_ATTR_IO_SESSION, 0, NULL);
    
    if(io != 0)
        {
        ViSession   rmSession = 0;

        checkErr( Rs_GetAttribute (instrSession, NULL, RS_ATTR_VISA_RM_SESSION, 0, (ViInt32) sizeof (rmSession), &rmSession));
        
        (void) viClose (io);
        (void) viClose (rmSession);
        }
    
    return error;
}

/*****************************************************************************
 * Function: rsspecan_close
 * Purpose:  This function closes the instrument.
 *
 *           Note:  This function must unlock the session before calling
 *           Rs_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_close (ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_ViClose(instrSession));

Error:
	(void)RsCore_Dispose(instrSession);
	(void)RsCore_UnlockSession(instrSession);

	return error;
}

/*===========================================================================*
 *= RsSpecAn Spurious Emissions Group Functions =============================*
 *===========================================================================*/
/*****************************************************************************
 * Function:    Configure Spurious Emissions State
 * Purpose:     This function switches the spurious emissions measurement on
 *              or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEState (ViSession instrSession,
                                             ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViBooleanRange (state) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "State");   
        
    if (state == VI_TRUE)
    {
        viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Win0",
                                                  RSSPECAN_ATTR_SWEEP_MODE,
                                                  RSSPECAN_VAL_SWE_MODE_LIST));
    }
    else
        viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Win0",
                                                  RSSPECAN_ATTR_SWEEP_MODE,
                                                  RSSPECAN_VAL_SWE_MODE_AUTO));
    
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Peak Search
 * Purpose:     This function configures the peak search for spurious emission 
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEPeakSearch (ViSession instrSession,
                                                                 ViInt32 window,
                                                                 ViReal64 margin,
                                                                 ViInt32 peaksPerRange)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "Win%ld", window);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer,
                                                  RSSPECAN_ATTR_SE_PEAK_SEARCH_MARGIN,
                                                  margin), 3, "Margin");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer,
                                                  RSSPECAN_ATTR_SE_PEAK_SEARCH_PEAKS,
                                                  peaksPerRange), 4, "Peaks Per Range");
    
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Search Peak
 * Purpose:     This function searches the peaks in spurious emissions data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SESearchPeak (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "Win1",
                                                  RSSPECAN_ATTR_SE_PEAK_SEARCH,
                                                  VI_NULL));
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Start Stop
 * Purpose:     This function configures the start and stop frequency for the 
 *              specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListStartStop (ViSession instrSession,
                                                          ViInt32 range,
                                                          ViReal64 startFrequency,
                                                          ViReal64 stopFrequency)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_START, startFrequency),
                                                3, "Start Frequency");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_STOP, stopFrequency),
                                                4, "Stop Frequency");
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Filter Type
 * Purpose:     This function configures the filter type for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListFilterType (ViSession instrSession,
                                                           ViInt32 range,
                                                           ViInt32 filter)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_FILT_TYPE, filter),
                                                3, "Filter");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Bandwidth
 * Purpose:     This function configures the resolution and video bandwidth 
 *              values for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListBandwidth (ViSession instrSession,
                                                          ViInt32 range,
                                                          ViReal64 resolutionBandwidth,
                                                          ViReal64 videoBandwidth)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_BAND, resolutionBandwidth),
                                                3, "Resolution Bandwidth");                                                        
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_BAND_VID, videoBandwidth),
                                                4, "Video Bandwidth");                                                  
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Time
 * Purpose:     This function configures the sweep time values for the 
 *              specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepSweepListTime (ViSession instrSession,
                                                          ViInt32 range,
                                                          ViBoolean sweepTimeAuto,
                                                          ViReal64 sweepTime)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_SWE_TIME_AUTO, sweepTimeAuto),
                                                3, "Sweep Time Auto");                                                        
    if (sweepTimeAuto == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_SWE_TIME, sweepTime),
                                                4, "Sweep Time");                                                  
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Detector
 * Purpose:     This function configures the detector value for the specified 
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListDetector (ViSession instrSession,
                                                         ViInt32 range,
                                                         ViInt32 detector)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_DET, detector),
                                                3, "Detector");                                                        
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Reference Level
 * Purpose:     This function configures the reference level for the specified 
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListReferenceLevel (ViSession instrSession,
                                                               ViInt32 range,
                                                               ViReal64 refLevelValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_RLEV, refLevelValue),
                                                3, "Ref Level Value");                                                        
    
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List RF Attenuation
 * Purpose:     This function configures the RF attenuation settings for the 
 *              specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListRFAttenuation (ViSession instrSession,
                                                              ViInt32 range,
                                                              ViBoolean attenuatorAuto,
                                                              ViReal64 manualAttenuatorValuedB)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_INP_ATT_AUTO, attenuatorAuto),
                                                3, "Attenuator Auto");                                                        
    if (attenuatorAuto == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_INP_ATT, manualAttenuatorValuedB),
                                                4, "Manual Attenuator Value dB");                                                  
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Preamplifier
 * Purpose:     This function configures state of pre-amplifier for selected
 *              sweep range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListPreamplifier (ViSession instrSession,
                                                                   ViInt32 range,
                                                                   ViBoolean preamplifier)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
    if ((strstr (buffer, "B23") == VI_NULL) && (strstr (buffer, "B24") == VI_NULL) && (strstr (buffer, "B25") == VI_NULL)) 
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
        
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);

    checkErr( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_INP_GAIN_STAT, preamplifier));  
                                                
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function configures level of pre-amplifier for selected sweep range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR range/Sets the configured range.
/// HIPAR preamplificationLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListPreamplifierLevel (ViSession instrSession,
                                                                  ViInt32 range,
                                                                  ViInt32 preamplificationLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    checkErr( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_SE_LIST_RANG_INP_GAIN_LEVEL, preamplificationLevel));
                                                          
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Sweep
 * Purpose:     This function configures the sweep settings for the specified 
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListSweep (ViSession instrSession,
                                                      ViInt32 range,
                                                      ViInt32 sweepPoints,
                                                      ViBoolean stopAfterSweep)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_POINTS, sweepPoints),
                                                3, "Sweep Points");                                                  
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_BREAK, stopAfterSweep),
                                                4, "Stop After Sweep");                                                        
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Transducer Factor
 * Purpose:     This function configures a transducer for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListTransducerFactor (ViSession instrSession,
                                                                 ViInt32 range,
                                                                 ViString transducerFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    if (strlen(transducerFactor)>0)
        viCheckParm( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_TRAN, transducerFactor),
                                                3, "Transducer Factor");   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Limits
 * Purpose:     This function configures the limits for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListLimits (ViSession instrSession,
                                                       ViInt32 range,
                                                       ViBoolean limitChecking,
                                                       ViReal64 limit,
                                                       ViReal64 absoluteLimitStart,
                                                       ViReal64 absoluteLimitStop)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_STATE, limitChecking),
                                                3, "Limit Checking");                                                 
    if (limitChecking == VI_TRUE)                                               
    {
        if ((strstr (model, "FSL") != NULL) || 
            (rsspecan_IsFSV (instrSession)) ||
            (strstr (model, "FSP") != NULL) || 
            (strstr (model, "FSQ") != NULL) || 
            (strstr (model, "FSU") != NULL))
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_START, absoluteLimitStart),
                                                5, "Absolute Limit Start");
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_STOP, absoluteLimitStop),
                                                6, "Absolute Limit Stop");
        }
        else
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SE_LIST_RANG_LIMIT, limit),
                                                4, "Limit");
        }
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    SE Standard Catalog
 * Purpose:     This function queries the names of customized spurious emission
 *              measurement configuration files currently stored on the hard
 *              disk of the R&S FSQ. The command queries the files that are in
 *              the folder "D:\R_S\INSTR\SPURS_STD". The file extension (*.xml)
 *              is not returned.     
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardCatalog (ViSession instrSession,
                                              ViInt32 arraySize,
                                              ViInt32* numberOfStandards,
                                              ViChar catalogList[])
{
    ViStatus    error = VI_SUCCESS;
    ViUInt32    retCnt = 0;
    ViChar      *buf=VI_NULL;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    if (arraySize < 0)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Array Size");         
    if ((arraySize > 0) && (catalogList == VI_NULL))
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Catalog List");
    
    viCheckErr (viPrintf (instrSession, "SENS:LIST:STAN:CAT?\n"));
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &buf, &retCnt));
    viCheckErr (rsspecan_ParseCatalog(buf, arraySize, catalogList, numberOfStandards));
    
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    if (buf) free(buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);      
    return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Standard Save
 * Purpose:     This function stores the current spurious emission measurement
 *              configuration to a file on the R&S FSQ hard disk. The file
 *              format is *.xml. The default path is "D:\R_S\INSTR\SPURS_STD".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardSave (ViSession instrSession,
                                           ViString standardSave)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_SE_STANDARD_SAVE, standardSave),
                                                2, "Standard Save");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Standard Load
 * Purpose:     This function loads the current spurious emission measurement
 *              configuration to a file on the R&S FSQ hard disk. The file
 *              format is *.xml. The default path is "D:\R_S\INSTR\SPURS_STD".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardLoad (ViSession instrSession,
                                           ViString standardLoad)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_SE_STANDARD_LOAD, standardLoad),
                                                2, "Standard Load");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Standard Delete
 * Purpose:     This function deletes a file that contains a spurious emission
 *              measurement configuration. The default path to the file is
 *              "D:\R_S\INSTR\SPURS_STD". The file name has to be specified
 *              without the *.xml file extension.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardDelete (ViSession instrSession,
                                             ViString standardDelete)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "", 
                                                RSSPECAN_ATTR_SE_STANDARD_DELETE, standardDelete),
                                                2, "Standard Delete");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Delete Range
 * Purpose:     This function deletes a range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEDeleteRange (ViSession instrSession,
                                        ViInt32 range)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer,
                                               RSSPECAN_ATTR_SE_LIST_RANG_DEL, VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Get Spurious Emissions Number Of List Ranges
 * Purpose:     This function returns the number of defined ranges of the spurious
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSENumberOfListRanges (ViSession instrSession,
                                                  ViInt32 *numberOfRanges)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_SE_LIST_RANG_COUNT, numberOfRanges),
                                               2, "Number Of Ranges");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Store Spurious Emissions to File
 * Purpose:  This function stores the peak list of selected trace in ASCII 
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSEToFile (ViSession instrSession,
                                                         ViString fileName)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_SE_STORE_FILE, fileName),
                                               3, "File Name");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read Spurious Peak List
 * Purpose:  This function returns the spurious emissions peak list.
 *****************************************************************************/
 ViStatus _VI_FUNC rsspecan_ReadSEPeakList (ViSession instrSession,
                                                 ViInt32 noOfValues,
                                                 ViReal64 frequency[],
                                                 ViReal64 level[],
                                                 ViReal64 deltaLimit[],
                                                 ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     arrayLength = 0;
    ViReal64    *data = VI_NULL;
    ViInt32     retVal = 0;
    ViInt32     i;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
       
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    arrayLength = 3*noOfValues;
    data = (ViReal64 *) malloc (sizeof(ViReal64)*arrayLength);
    if (data == NULL)
        return VI_ERROR_ALLOC;
    
   

    viCheckErr (rsspecan_dataReadTrace (instrSession, 1, "SPUR", arrayLength, 
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
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Spurious Emissions List Evaluation
 * Purpose:  This function activates or deactivates spurious emission 
 *           measurement LIST EVALUATION.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEListEvaluation (ViSession instrSession,
                                             ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                               RSSPECAN_ATTR_SE_LIST_EVAL_STATE, state),
                                               2, "State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read Spurious Emissions List Evaluation
 * Purpose:  This function returns the resulting peak list of the spurious 
 *           emission measurement LIST EVALUATION.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEListEvaluation (ViSession instrSession,
                                                 ViInt32 noOfValues,
                                                 ViReal64 rangeNumber[],
                                                 ViReal64 startFrequency[],
                                                 ViReal64 stopFrequency[],
                                                 ViReal64 resolutionBandwidth[],
                                                 ViReal64 frequency[],
                                                 ViReal64 levelAbs[],
                                                 ViReal64 levelRel[],
                                                 ViReal64 delta[],
                                                 ViReal64 limitCheck[],
                                                 ViReal64 reserved1[],
                                                 ViReal64 reserved2[],
                                                 ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *buffer = VI_NULL;
    ViChar      *pbuf=VI_NULL;
    ViInt32     count=0;
    ViUInt32    retCnt;
    ViInt32     i;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (rangeNumber == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range number");
    if (startFrequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Start Frequency"); 
    if (stopFrequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Stop Frequency"); 
    if (resolutionBandwidth == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Resolution Bandwidth"); 
    if (frequency == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Frequency"); 
    if (levelAbs == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 7, "Level Abs");
    if (levelRel == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 8, "Level Rel"); 
    if (delta == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 9, "Delta"); 
    if (limitCheck == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 10, "Limit Check");
    if (reserved1 == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 11, "Reserved 1");
    if (reserved2 == VI_NULL) viCheckParm (RS_ERROR_INVALID_PARAMETER, 12, "Reserved 2"); 
     
    viCheckErr (viPrintf (instrSession, "TRAC:DATA? LIST\n"));
    viCheckErr( Rs_ReadDataUnknownLength(instrSession, &buffer, &retCnt));
    
    pbuf=strtok(buffer, ",");
    if (pbuf)
        do{
            if (count<noOfValues)
            {
                sscanf (pbuf, "%le", &rangeNumber[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &startFrequency[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &stopFrequency[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &resolutionBandwidth[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &frequency[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &levelAbs[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &levelRel[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &delta[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &limitCheck[count]);
                pbuf=strtok(NULL, ",");
                sscanf (pbuf, "%le", &reserved1[count]);
                pbuf=strtok(NULL, ","); 
                sscanf (pbuf, "%le", &reserved2[count]);
                pbuf=strtok(NULL, ","); 
            }               
            else
            {
                for (i=0;i<11;i++)
                    pbuf=strtok(NULL, ","); 
            }       
            count++;
        }while (pbuf);  

    if (returnedValues)
        *returnedValues = count;
     
    checkErr( rsspecan_CheckStatus (instrSession));     
Error:
    if (buffer) free(buffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM State
 * Purpose:  This function configures the Spectrum EM Mask Measurement.  
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMState (ViSession instrSession,
                                              ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViBooleanRange (state) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "State");   
        
    if (state == VI_TRUE)
    {
        viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Win0",
                                                  RSSPECAN_ATTR_SWEEP_MODE,
                                                  RSSPECAN_VAL_SWE_MODE_ESP));
    }
    else
        viCheckErr( rsspecan_SetAttributeViInt32 (instrSession, "Win0",
                                                  RSSPECAN_ATTR_SWEEP_MODE,
                                                  RSSPECAN_VAL_SWE_MODE_AUTO));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.  
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEM (ViSession instrSession,
                                         ViInt32 limitLineType,
                                         ViReal64 limitLineValue,
                                         ViReal64 _30kHz1MHzTransition)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_ESP_MODE,
                 limitLineType), 2, "Limit Line Type");

    if (limitLineType == RSSPECAN_VAL_ESP_MODE_MAN)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_ESP_MAN_VAL,
                     limitLineValue), 3, "Limit Line Value");
    }
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_ESP_TRAN,
                 _30kHz1MHzTransition), 4, "30kHz/1MHz Transition");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Peak Search
 * Purpose:  This function configures the peak search for spectrum emission 
 *           mask measurement. 
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPeakSearch (ViSession instrSession,
                                                   ViReal64 margin,
                                                   ViInt32 peaksPerRange)
{
    ViStatus    error = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
   
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                  RSSPECAN_ATTR_SEM_PEAK_SEARCH_MARGIN,
                                                  margin), 2, "Margin");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                                  RSSPECAN_ATTR_SEM_PEAK_SEARCH_PEAKS,
                                                  peaksPerRange), 3, "Peaks Per Range");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Restore SEM Standard Lines
 * Purpose:  This function restores the standard limit lines for the spectrum 
 *           emission mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreSEMStandardLines (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_ESP_RESTORE,
                 NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: SEM Search Peak
 * Purpose:  This function searches the peaks in spectrum emission data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMSearchPeak (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_SetAttributeViString (instrSession, "C1", RSSPECAN_ATTR_ESP_PEAK_SEARCH,
                 NULL));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Store SEM To File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSEMToFile (ViSession instrSession,
                                           ViString fileName)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_LIM_STORE_FILE, 
                            fileName), 2, "File Name");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read SEM Peak List 
 * Purpose:  This function returns the Spectrum EM Mask View Peak List. 
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEMPeakList (ViSession instrSession,
                                            ViInt32 window, 
                                            ViInt32 finalMeasurement,
                                            ViInt32 noOfValues,
                                            ViReal64 frequency[],
                                            ViReal64 level[],
                                            ViReal64 deltaLevel[],
                                            ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     arrayLength;
    ViReal64    *data = VI_NULL;
    ViInt32     retVal;
    ViInt32     i;
    ViChar      model[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    
    if (strstr (model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    arrayLength = 3*noOfValues;
    data=malloc (sizeof(ViReal64)*arrayLength);
    
    sprintf (buffer, "FINal%ld", finalMeasurement);

    viCheckErr (rsspecan_dataReadTrace (instrSession, window, buffer, arrayLength,
                    data, &retVal));

    if (returnedValues)
        *returnedValues = retVal / 3; /* Three result arrays */

    retVal = (retVal>arrayLength)?arrayLength:retVal;

    retVal /=3;

    for (i = 0; i < retVal; i++)
    {
        frequency[i] = data[i*3];
        level[i] = data[i*3+1];
        deltaLevel[i] = data[i*3+2];
    }


    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (data) free(data);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get SEM Value
 * Purpose:  This function returns the X/Y -value at the maximum overstepping 
 *           of the spectrum emission mask limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSEMValue (ViSession instrSession,
                                        ViInt32 selection, 
                                        ViReal64 *value)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (selection, 0, 1) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Selection");
    }
    switch (selection){
        case RSSPECAN_VAL_X:
            viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_ESP_QUERY_X, 
                            value), 3, "Value");
    
        break;
        case RSSPECAN_VAL_Y:
            viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_ESP_QUERY_X, 
                            value), 3, "Value");
    
        break;
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: Spectrum Emissions Mask List Evaluation
 * Purpose:  This function activates or deactivates spectrum emission mask
 *           measurement LIST EVALUATION.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMListEvaluation (ViSession instrSession,
                                             ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                               RSSPECAN_ATTR_SEM_LIST_EVAL_STATE, state),
                                               2, "State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Read SEM List Evaluation Results
 * Purpose:  This function returns the rreturns the list evaluation results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEMListEvaluationResults (ViSession instrSession,
                                                         ViInt32 noOfValues,
                                                         ViReal64 rangeNumber[],
                                                         ViReal64 startFrequency[],
                                                         ViReal64 stopFrequency[],
                                                         ViReal64 resolutionBandwidth[],
                                                         ViReal64 frequency[],
                                                         ViReal64 powerAbs[],
                                                         ViReal64 powerRel[],
                                                         ViReal64 delta[],
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
    ViInt32     values = noOfValues * 11;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    
    if (strstr (model, "FMU") != NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

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
     
    viCheckErr (viPrintf (instrSession, "TRAC:DATA? LIST\n"));
    pArray = malloc (values*sizeof (ViReal64));
    viCheckErr (rsspecan_dataReadTraceOnly (instrSession, values, pArray, &retCnt));
    values = (retCnt>values)?values:retCnt;
    i=0;
    while (count<values)
    {
        rangeNumber[i]=pArray[count];
        count++;
        startFrequency[i]=pArray[count];
        count++;
        stopFrequency[i]=pArray[count];
        count++;
        resolutionBandwidth[i]=pArray[count];
        count++;
        frequency[i]=pArray[count];
        count++;
        powerAbs[i]=pArray[count];
        count++;
        powerRel[i]=pArray[count];
        count++;
        delta[i]=pArray[count];
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
 * Function: SEM Mark All Peaks
 * Purpose:  This function marks all peaks with blue squares in the diagram.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMMarkAllPeaks (ViSession instrSession,
                                            ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                               RSSPECAN_ATTR_SEM_MARK_ALL_PEAKS, state),
                                               2, "State");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Reference Range
 * Purpose:  This function configures the reference range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMReferenceRange (ViSession instrSession,
                                                       ViInt32 powerReferenceType,
                                                       ViReal64 txBandwidth,
                                                       ViBoolean RRCFilterState,
                                                       ViReal64 RRCFilterAlpha)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_SEM_POWER_REF_TYPE, powerReferenceType),
                                               2, "Power Reference Type");
    
    if (powerReferenceType == RSSPECAN_VAL_SEM_REF_TYPE_CPOW)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                   RSSPECAN_ATTR_SEM_BANDWIDTH, txBandwidth),
                                                   3, "TX Bandwidth");
    
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                                   RSSPECAN_ATTR_SEM_RRC_FILTER_STATE, RRCFilterState),
                                                   4, "RRC Filter State");
    
        if (RRCFilterState == VI_TRUE)
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "",
                                                       RSSPECAN_ATTR_SEM_RRC_FILTER_ALPHA_VALUE, RRCFilterAlpha),
                                                       5, "RRC Filter Alpha");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query SEM Reference Range Position
 * Purpose:  This function returns the current position (number) of the 
 *           reference range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySEMReferenceRangePosition (ViSession instrSession,
                                                           ViInt32 *referenceRangePosition)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_SEM_REF_RANGE_POSITION, referenceRangePosition),
                                               2, "Reference Range Position");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures multi-standard radio (MSR) measurements, 
/// HIFN  which allow you to perform SEM tests on multiple carriers using 
/// HIFN  different digital standards.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR bandCategory/This control defines the band category for MSR measurements, i.e. the 
/// HIPAR bandCategory/combination of available carriers to measure.
/// HIPAR baseStationRFBandwidth/This control defines the RF bandwidth of the base station for MSR 
/// HIPAR baseStationRFBandwidth/measurements.
/// HIPAR gsm_edgePresent/This control defines whether a GSM/Edge carrier is located at the edge 
/// HIPAR gsm_edgePresent/of the specified RF bandwidth. In this case, the specification demands 
/// HIPAR gsm_edgePresent/specific limits for the SEM ranges.
/// HIPAR lteFDD1_4MHz_3MHzPresent/This control defines whether an LTE FDD 1.4 MHz or 3 MHz carrier is 
/// HIPAR lteFDD1_4MHz_3MHzPresent/located at the edge  of the specified RF bandwidth. In this case, the 
/// HIPAR lteFDD1_4MHz_3MHzPresent/specification demands specific limits for the SEM ranges.
ViStatus _VI_FUNC rsspecan_ConfigureSEMMSRSettings(
    ViSession   instrSession,
    ViInt32 bandCategory,
    ViReal64    baseStationRFBandwidth,
    ViBoolean   gsm_edgePresent,
    ViBoolean   lteFDD1_4MHz_3MHzPresent
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_SEM_MSR_BAND_CATEGORY,
                                               bandCategory), 2, "Band Category");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_SEM_MSR_RF_BANDWIDTH,
                                                baseStationRFBandwidth), 3, "Base Station RF Bandwidth");
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_SEM_MSR_GSM_PRESENT,
                                                 gsm_edgePresent), 4, "Gsm Edge Present");
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_SEM_MSR_LTE_PRESENT,
                                                 lteFDD1_4MHz_3MHzPresent), 5, "LTE FDD 1.4MHz 3MHz Present");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function configures the SEM sweep list according to the MSR 
/// HIFN  settings defined by function rsspecan_ConfigureSEMMSRSettings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_ApplySEMMSRSettings(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_SEM_MSR_APPLY, NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*****************************************************************************
 * Function: Configure Fast SEM
 * Purpose:  This function activates Fast SEM mode to accelerate spectrum 
 *           emission mask measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFastSEM (ViSession instrSession,
                                             ViBoolean fastSEM)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, "",
                                               RSSPECAN_ATTR_SEM_FAST_SEM, fastSEM),
                                               2, "Fast SEM");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Preset Standard
 * Purpose:  This function selects the specified XML file under 
 *           C:\r_s\instr\sem_std. If the file is stored in a subdirectory, 
 *           include the relative path.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPresetStandard (ViSession instrSession,
                                                       ViString presetStandard)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_SEM_PRESET_STANDARD, presetStandard),
                                               2, "Preset Standard");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: SEM Standard Save
 * Purpose:  This function stores the current spectrum emission mask
 *           configuration to a file in the R&S FSQ memory.  The file format
 *           is *.xml. The default path is "D:\R_S\INSTR\SPURS_STD".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMStandardSave (ViSession instrSession,
                                                     ViString saveStandard)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_SEM_PRESET_STANDARD_SAVE, saveStandard),
                                                2, "Save Standard");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: SEM Standard Delete
 * Purpose:  This function deletes a file that contains a spectrum emission
 *           mask measurement configuration. The default path to the file is
 *           "D:\R_S\INSTR\SPURS_STD". The file name has to be specified
 *           without the *.xml file extension.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMStandardDelete (ViSession instrSession,
                                                       ViString deleteStandard)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_SEM_PRESET_STANDARD_DELETE, deleteStandard),
                                                2, "Delete Standard");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Restore SEM Standard Files
 * Purpose:  This function copies the XML files from the C:\R_S\instr\sem_backup 
 *           folder to the C:\R_S\instr\sem_std folder. Files of the same name 
 *           are overwritten.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreSEMStandardFiles (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_SEM_RESTORE_STANDARD, NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Store SEM Preset Settings
 * Purpose:  This function stores the current settings as presettings in the 
 *           specified XML file under C:\r_s\instr\sem_backup.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSEMPresetSettings (ViSession instrSession,
                                                   ViString settingsFileName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_SEM_STORE_PRESET, settingsFileName),
                                               2, "Settings File Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: SEM Save List To File
 * Purpose:  This function stores the current list evaluation results in a *.dat 
 *           file. The file consists of a data section containing the list 
 *           evaluation results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMSaveListToFile (ViSession instrSession,
                                              ViString fileName)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_SEM_LIST_FILE_EXPORT, fileName),
                                               2, "File Name");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Start Stop
 * Purpose:  This function configures the start and stop frequency for the
 *           specified range. In order to change the start/stop frequency of 
 *           the first/last range, select the appropriate span. If you set a 
 *           span that is smaller than the overall span of the ranges, the 
 *           measurement includes only the ranges that lie within the defined 
 *           span and have a minimum span of 20 Hz. The first and last range are
 *           adapted to the given span as long as the minimum span of 20 Hz
 *           is not violated.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeStartStop (ViSession instrSession,
                                                       ViInt32 range,
                                                       ViReal64 rangeStart,
                                                       ViReal64 rangeStop)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_FREQ_START, rangeStart),
                                                3, "Range Start");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_FREQ_STOP, rangeStop),
                                                4, "Range Stop");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Filter Type
 * Purpose:  This function configures the filter type for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeFilterType (ViSession instrSession,
                                                        ViInt32 range,
                                                        ViInt32 filterType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_FILTER_TYPE, filterType),
                                                3, "Filter Type");

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Bandwidth
 * Purpose:  This function configures the resolution and video bandwidth values 
 *           for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeBandwidth (ViSession instrSession,
                                                       ViInt32 range,
                                                       ViReal64 resolutionBandwidth,
                                                       ViReal64 videoBandwidth)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_BANDWIDTH, resolutionBandwidth),
                                                3, "Resolution Bandwidth");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_VBW, videoBandwidth),
                                                4, "Video Bandwidth");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Sweep Time
 * Purpose:  This function configures the sweep time values for the specified 
 *           range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeSweepTime (ViSession instrSession,
                                                       ViInt32 range,
                                                       ViBoolean sweepTimeMode,
                                                       ViReal64 sweepTime)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_SWEEP_TIME_AUTO, sweepTimeMode),
                                                3, "Sweep Time Mode");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_SWEEP_TIME, sweepTime),
                                                4, "Sweep Time");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Reference Level
 * Purpose:  This function configures the reference level for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeReferenceLevel (ViSession instrSession,
                                                            ViInt32 range,
                                                            ViReal64 referenceLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_REF_LEVEL, referenceLevel),
                                                3, "Reference Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range RF Attenuation
 * Purpose:  This function configures the RF attenuation setting for the 
 *           specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRFAttenuation (ViSession instrSession,
                                                           ViInt32 range,
                                                           ViBoolean RFAttenuationMode,
                                                           ViReal64 RFAttenuator,
                                                           ViBoolean preamplifiier)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_INPUT_ATTENUATION_AUTO, RFAttenuationMode),
                                                3, "RF Attenuation Mode");
    if (RFAttenuationMode == VI_FALSE)
    {
        viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                    RSSPECAN_ATTR_SEM_RANGE_INPUT_ATTENUATION, RFAttenuator),
                                                    4, "RF Attenuator");
    }
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_PREAMPLIFIER, preamplifiier),
                                                5, "Preamplifiier");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function configures level of pre-amplifier for selected sweep range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR range/Sets the configured range.
/// HIPAR preamplificationLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRFPreamplifierLevel (ViSession instrSession,
                                                                 ViInt32 range,
                                                                 ViInt32 preamplificationLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                RSSPECAN_ATTR_SEM_RANGE_PREAMPLIFIER_LEVEL, preamplificationLevel), 3, "Preamplification Level");                                                  
                                                          
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function: Configure SEM Range Transducer Factor
 * Purpose:  This function configures a transducer for the specified range. 
 *           You can only choose a transducer that fulfills the following 
 *           conditions:
 *           - The transducer overlaps or equals the span of the range.
 *           - The x-axis is linear.
 *           - The unit is dB.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeTransducerFactor (ViSession instrSession,
                                                              ViInt32 range,
                                                              ViString transducerFactor)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_TRANSDUCER, transducerFactor),
                                                3, "Transducer Factor");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Limits
 * Purpose:  This function configures the limit setting for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeLimits (ViSession instrSession,
                                                    ViInt32 range,
                                                    ViInt32 limitCheckType,
                                                    ViReal64 absoluteLimitStart,
                                                    ViReal64 absoluteLimitStop,
                                                    ViReal64 relativeLimitStart,
                                                    ViReal64 relativeLimitStop)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
 
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_SEM_RANGE_LIMIT_CHECK_TYPE, limitCheckType),
                                                3, "Limit Check Type");
    
    switch (limitCheckType)
    {
        case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_ABS:
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_START_FREQ_ABS_LIMIT, absoluteLimitStart),
                                                4, "Absolute Limit Start");
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_ABS_LIMIT, absoluteLimitStop),
                                                5, "Absolute Limit Stop");
            break;
        }
        case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_REL:
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT, relativeLimitStart),
                                                6, "Relative Limit Start");
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT, relativeLimitStop),
                                                7, "Relative Limit Stop");
            break;
        }
        case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_AND:
        case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_OR:
        {
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_START_FREQ_ABS_LIMIT, absoluteLimitStart),
                                                4, "Absolute Limit Start");
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_ABS_LIMIT, absoluteLimitStop),
                                                5, "Absolute Limit Stop");
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT, relativeLimitStart),
                                                6, "Relative Limit Start");
            viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT, relativeLimitStop),
                                                7, "Relative Limit Stop");
            break;
        }
        default:
            viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Type");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function configures the limit setting for the specified range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR range/Selects the range.
/// HIPAR powerClass/This control sets the power class for which the limit is defined.
/// HIPAR startFunction/Defines the function to be used to determine the relative limit line 
/// HIPAR startFunction/start value.
/// HIPAR absoluteLimitStart/This control sets an absolute limit value at the start frequency of 
/// HIPAR absoluteLimitStart/the specified range to be used in addition to the relative limit if the 
/// HIPAR absoluteLimitStart/MAX function is enabled.
/// HIPAR stopFunction/Defines the function to be used to determine the relative limit line 
/// HIPAR stopFunction/stop value.
/// HIPAR absoluteLimitStop/This control sets an absolute limit value at the stop frequency of the 
/// HIPAR absoluteLimitStop/specified range to be used in addition to the relative limit if the MAX 
/// HIPAR absoluteLimitStop/function is enabled.
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRelativeLimitsFunctions(
    ViSession   instrSession,
    ViInt32 range,
    ViInt32 powerClass,
    ViInt32 startFunction,
    ViReal64    absoluteLimitStart,
    ViInt32 stopFunction,
    ViReal64    absoluteLimitStop
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar  rep_cap_power_class[15] = "";
    ViChar  rep_cap[25] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (range, 1, 20))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    }

    if (rsspecan_invalidViInt32Range (powerClass, 1, 4))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Power Class");
    }

    viCheckParm (rsspecan_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM,
                                                 "Range",
                                                 range - 1,
                                                 10,
                                                 rep_cap), 2, "Range");

    viCheckParm (rsspecan_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM,
                                                 "PClass",
                                                 powerClass - 1,
                                                 10,
                                                 rep_cap_power_class), 3, "Power Class");
    
    strcat (rep_cap, ",");
    strcat (rep_cap, rep_cap_power_class);

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_FUNCTION,
                                               startFunction), 4, "Start Function");
    
    if (startFunction == RSSPECAN_VAL_SEM_REL_LIMIT_FUNC_MAX)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                    RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM,
                                                    absoluteLimitStart), 5, "Absolute Limit Start");
    }
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap,
                                               RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT_FUNCTION,
                                               stopFunction), 6, "Stop Function");
    
    if (startFunction == RSSPECAN_VAL_SEM_REL_LIMIT_FUNC_MAX)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap,
                                                    RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT_MAXIMUM,
                                                    absoluteLimitStop), 7, "Absolute Limit Stop");
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*****************************************************************************
 * Function: Get SEM Range Count
 * Purpose:  This function returns the number of defined ranges.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSEMRangeCount (ViSession instrSession,
                                             ViInt32 *rangeCount)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_SEM_RANGE_COUNT, rangeCount),
                                                2, "Range Count");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: SEM Insert New Range
 * Purpose:  This function inserts a new range before or after the specified 
 *           range. The range numbers are updated accordingly.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMInsertNewRange (ViSession instrSession,
                                              ViInt32 range, ViInt32 insertRange)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_INSERT_NEW_RANGE, insertRange),
                                                3, "Insert Range");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: SEM Delete Range
 * Purpose:  This function deletes the specified range. The range numbers are 
 *           updated accordingly. The reference range cannot be deleted. 
 *           A minimum of three ranges is mandatory.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMDeleteRange (ViSession instrSession,
                                           ViInt32 range)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (range, 1, 20) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Range");
    sprintf (buffer, "R%ld", range);
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_DELETE_RANGE, NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Used Power Classes
 * Purpose:  This function sets the power classes used in the spectrum emission 
 *           mask measurement. It is only possible to use power classes for 
 *           which limits are defined. Also, either only one power class at a 
 *           time or all power classes together can be selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMUsedPowerClasses (ViSession instrSession,
                                                         ViInt32 powerClassNumber,
                                                         ViInt32 usedPowerClass)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (powerClassNumber, 1, 4) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Power Class Number");
    
    sprintf (buffer, "PC%ld", powerClassNumber);
    
    if (usedPowerClass == RSSPECAN_VAL_ALL)
    {
        viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "PC1", 
                                                    RSSPECAN_ATTR_SEM_POWER_CLASS_USED, 
                                                    VI_FALSE)); 
        viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "PC2", 
                                                    RSSPECAN_ATTR_SEM_POWER_CLASS_USED, 
                                                    VI_FALSE)); 
        viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "PC3", 
                                                    RSSPECAN_ATTR_SEM_POWER_CLASS_USED, 
                                                    VI_FALSE)); 
        viCheckErr( rsspecan_SetAttributeViBoolean (instrSession, "PC4", 
                                                    RSSPECAN_ATTR_SEM_POWER_CLASS_USED, 
                                                    VI_FALSE)); 
        
    }
    else
    {
        viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, 
                                                    RSSPECAN_ATTR_SEM_POWER_CLASS_USED, 
                                                    (ViBoolean) usedPowerClass), 3, "Used Power Class");
        
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Power Class Count
 * Purpose:  This function sets the number of power classes to be defined.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassCount (ViSession instrSession,
                                                        ViInt32 count)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, "", 
                                                RSSPECAN_ATTR_SEM_POWER_CLASS_COUNT, 
                                                count), 3, "Count");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Power Class Limits
 * Purpose:  This function configures the power class settings of the Spectrum 
 *           Emission Mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassLimits (ViSession instrSession,
                                                         ViInt32 powerClassNumber,
                                                         ViInt32 limitState,
                                                         ViReal64 lowerLevelLimit,
                                                         ViReal64 upperLevelLimit)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (powerClassNumber, 1, 4) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Power Class Number");
    
    sprintf (buffer, "PC%ld", powerClassNumber);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_POWER_CLASS_LIMIT_STATE, 
                                                limitState), 3, "Limit State");
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_POWER_CLASS_LOWER_LEVEL_LIMIT, 
                                                lowerLevelLimit), 4, "Lower Level Limit");
    
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SEM_POWER_CLASS_UPPER_LEVEL_LIMIT, 
                                                upperLevelLimit), 5, "Upper Level Limit");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure SEM Power Class All Limits
 * Purpose:  This function sets up to 4 power classes in one step.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassAllLimits (ViSession instrSession,
                                                            ViInt32 numberOfLimits,
                                                            ViReal64 limits[])
{
    ViStatus    error = VI_SUCCESS; 
    ViChar      *pbuffer;
    ViChar      *p2buf;
    ViInt32     i = 0;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (!rsspecan_IsFSV (instrSession) || (strstr (model, "FSL") == NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    pbuffer = (ViChar*) malloc (150 * numberOfLimits + 30);

    p2buf = pbuffer + sprintf (pbuffer, "CALC:LIM:ESP:LIM ");
    
    for (i = 0; i < numberOfLimits; i++)
        p2buf += sprintf (p2buf, "%lf,", limits[i]);
    
    *--p2buf = '\0';
    
    viCheckErr (viPrintf (instrSession, "%s\n", pbuffer));
    
    free (pbuffer);
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Query SEM Power Class All Limits
 * Purpose:  This function returns up to 4 power classes in one step.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySEMPowerClassAllLimits (ViSession instrSession,
                                                        ViReal64 limits[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      *p2buff;
    ViInt32     i = 0;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (!rsspecan_IsFSV (instrSession) || (strstr (model, "FSL") == NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    viCheckErr( rsspecan_QueryViString (instrSession, 
                                        "CALC:LIM:ESP:LIM?", 
                                        RSSPECAN_IO_BUFFER_SIZE, 
                                        buffer));

    p2buff = strtok (buffer, ",");
    
    while (p2buff)
    {
        limits[i++] = atof (p2buff);
        p2buff = strtok (NULL, ",");
    }
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function selects the MSRA mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_MSRAMode(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_MSRA_MODE, NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function queries the analysis interval for the current window. 
/// HIFN  This function is only available in application measurement channels, 
/// HIFN  not the MSRA View or MSRA Master.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR intervalStart/Returns start value of the analysis interval.
/// HIPAR intervalStop/Returns stop value of the analysis interval.
ViStatus _VI_FUNC rsspecan_QueryMSRAAnalysisInterval(
    ViSession   instrSession,
    ViInt32 window,
    ViReal64*   intervalStart,
    ViReal64*   intervalStop
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar  buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar  rd_buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar* p2buff;

    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "CALC:MSRA:WIND%ld:IVAL?", window);

    viCheckErr (rsspecan_QueryViString (instrSession, buffer, RSSPECAN_IO_BUFFER_SIZE, rd_buffer));
    
    p2buff = strtok (rd_buffer, ",");
    if (p2buff != NULL)
        *intervalStart = atof (p2buff);

    p2buff = strtok (NULL, ",");
    if (p2buff != NULL)
        *intervalStop = atof (p2buff);
    
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function queries all analysis intervals for the current
/// HIFN  window. This function is only available in application
/// HIFN  measurement channels, not the MSRA View or MSRA Master.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR arraySize/Defines the number of values to be queried. The number should be
/// HIPAR arraySize/equal or higher than the sizes of array parameters.
/// HIPAR intervalStart/Returns an array of start values of all analysis intervals.
/// HIPAR intervalStop/Returns an array of stop values of all analysis intervals.
ViStatus _VI_FUNC rsspecan_QueryMSRAAllAnalysisInterval (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 arraySize,
                                                         ViReal64 intervalStart[],
                                                         ViReal64 intervalStop[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar  buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar  rd_buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar* p2buff;
    ViInt32 i = 0;

    checkErr (Rs_LockSession (instrSession, VI_NULL));
    
    sprintf (buffer, "CALC:MSRA:WIND%ld:MIV?", window);

    viCheckErr (rsspecan_QueryViString (instrSession, buffer, RSSPECAN_IO_BUFFER_SIZE * 4, rd_buffer));
    
    p2buff = strtok (rd_buffer, ",");
    
    while (p2buff && i < arraySize)
    {
        if (p2buff != NULL)
            intervalStart[i] = atof (p2buff);
        else
            viCheckErr (RS_ERROR_UNEXPECTED_RESPONSE);
        
        p2buff = strtok (NULL, ",");
        
        if (p2buff != NULL)
            intervalStop[i] = atof (p2buff);
        else
            viCheckErr (RS_ERROR_UNEXPECTED_RESPONSE);
        
        p2buff = strtok (NULL, ",");
        i++;
    }
    checkErr( rsspecan_CheckStatus (instrSession));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function is only available if the Sequencer is deactivated 
/// HIFN  (SYSTem:SEQuencer OFF) and only for applications in MSRA mode, not the 
/// HIFN  MSRA Master. The data in the capture buffer is re-evaluated by the 
/// HIFN  currently active application only. The
/// HIFN     results for any other applications remain unchanged. The application 
/// HIFN  channel must be selected before this function can be executed (with 
/// HIFN  attribute RSSPECAN_ATTR_SELECT_MEASUREMENT_CHANNEL or function 
/// HIFN  rsspecan_SelectMeasurementChannel).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
ViStatus _VI_FUNC rsspecan_MSRARefresh(
    ViSession   instrSession
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_MSRA_REFRESH, NULL));
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function is only available for applications in MSRA mode, not for 
/// HIFN  the MSRA Master. It has a similar effect as the trigger offset in other 
/// HIFN  measurements.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR offset/This control defines the time offset between the capture buffer start 
/// HIPAR offset/and the start of the extracted application data. The offset must be a 
/// HIPAR offset/positive value, as the application can only analyze data that is 
/// HIPAR offset/contained in the capture buffer.
ViStatus _VI_FUNC rsspecan_ConfigureMSRACaptureOffset(
    ViSession   instrSession,
    ViInt32 offset
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_MSRA_CAPTURE_OFFSET,
                                               offset), 2, "Offset");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*****************************************************************************
 * Function:    Receiver Search Peak
 * Purpose:     This function searches the peaks in spurious emissions data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReceiverPeakSearch (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "Win1",
                                                  RSSPECAN_ATTR_SE_PEAK_SEARCH,
                                                  VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN This function shows/hides analysis line position and sets line position.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR showLine/This control hides or displays the analysis line in the time-based MSRA windows.
/// HIPAR position/This control defines the position of the analysis line for all
/// HIPAR position/time-based windows in all MSRA applications and the MSRA Master.
ViStatus _VI_FUNC rsspecan_ConfigureMSRAAnalysisLine (ViSession instrSession,
                                                      ViBoolean showLine,
                                                      ViReal64 position)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                               RSSPECAN_ATTR_MSRA_SHOW_ANALYSIS_LINE,
                                               showLine), 2, "Show Line");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                               RSSPECAN_ATTR_MSRA_ANALYSIS_LINE_POSITION,
                                               position), 2, "Position"); 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN This function defines whether the captured I/Q data is evaluated
/// HIFN directly, or if it is converted (via FFT) to spectral or time
/// HIFN data first.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR dataEvaluation/Defines whether the captured I/Q data is evaluated directly, or
/// HIPAR dataEvaluation/if it is converted (via FFT) to spectral or time data first.
ViStatus _VI_FUNC rsspecan_ConfigureMSRADataEvaluation (ViSession instrSession,
                                                        ViInt32 dataEvaluation)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_MSRA_DATA_EVALUATION,
                                               dataEvaluation), 2, "Data Evaluation"); 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*===========================================================================*
 *= RsSpecAn Service Group Functions ========================================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Service Configure Input
 * Purpose:  This function configures the input for servising purposes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureInput (ViSession instrSession, 
                                                  ViInt32 window,
                                                  ViInt32 input, ViReal64 level)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));

    sprintf (buffer, "Win%ld", window);

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_SERVICE_INPUT_SOURCE, input), 3, "Input");
    if ((input== RSSPECAN_VAL_INPUT_CAL) && !((strstr (model, "FSL") != NULL) || (rsspecan_IsFSV (instrSession))))
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_SERVICE_INPUT_LEVEL, level), 4, "Level");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Input
 * Purpose:  This function configures pulsed calibration signal. This function 
 *           takes effect only if the RF input has been set to the internal 
 *           reference signal using rsspecan_ServiceConfigureInput
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigurePulsedInput (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViBoolean state,
                                                        ViReal64 frequency)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    sprintf (buffer, "Win%ld", window);

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, 
            RSSPECAN_ATTR_SERVICE_INPUT_PULSED_STATE, state), 3, "State");
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if ((rsspecan_IsFSV (instrSession)) || (strstr (model, "FSL") != NULL))
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                RSSPECAN_ATTR_SERVICE_PULSE_CAL_FREQ_FSV, frequency), 4, "Frequency");
    }
    else
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, 
                RSSPECAN_ATTR_SERVICE_INPUT_PULSED_RATE, frequency), 4, "Frequency");
    }
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Input
 * Purpose:  This function configures rectangle calibration signal. This function 
 *           takes effect only if the RF input has been set to the internal
 *           reference signal using rsspecan_ServiceConfigureInput.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureRectangleInput (ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean state,
                                                           ViReal64 frequency)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    sprintf (buffer, "Win%ld", window);

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, 
            RSSPECAN_ATTR_SERVICE_INPUT_RECTANGLE_STATE, state), 3, "State");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, 
            RSSPECAN_ATTR_SERVICE_INPUT_RECTANGLE_RATE, frequency), 4, "Frequency");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Baseband Source
 * Purpose:  This function selects the baseband signal source.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureBasebandSource (ViSession instrSession,
                                                           ViInt32 source)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
            RSSPECAN_ATTR_SERVICE_BASEBAND_SIGNAL_SOURCE, source), 2, "Source");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Pulsed IQ Calibration Signal
 * Purpose:  This function configures calibration signals. This function takes 
 *           effect only if the RF input has been set to the internal reference 
 *           signal using rsspecan_ServiceConfigureInput.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigurePulsedIQCalibrationSignal (ViSession instrSession,
                                                                      ViInt32 destination,
                                                                      ViReal64 frequency)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
            RSSPECAN_ATTR_SERVICE_PULSE_CAL_DESTINATION, destination), 2, "Destination");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
            RSSPECAN_ATTR_SERVICE_PULSE_CAL_FREQ, frequency), 3, "Frequency");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Pulsed IQ Calibration Signal
 * Purpose:  This function selects the voltage for the DC Cal signal in Volt.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureDCIQCalibrationSignal (ViSession instrSession,
                                                                  ViReal64 voltage)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
            RSSPECAN_ATTR_SERVICE_DC_CAL_VOLTAGE, voltage), 2, "Voltage");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Noise
 * Purpose:  This function switches the 28-V supply of the noise source at 
 *           the rear panel on or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureNoise (ViSession instrSession,
                                                  ViBoolean noise)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_SERVICE_NOISE_SOURCE, noise), 2, "Noise");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure YIG Filter
 * Purpose:  This function witches the YIG filter for image rejection into 
 *           the signal path (ON) or removes it from the signal path (OFF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureYIGFilter (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViBoolean YIGFilter)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    sprintf (buffer, "Win%ld", window);
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, 
            RSSPECAN_ATTR_AMPL_YIG_FILTER, YIGFilter), 3, "YIG Filter");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}



/// HIFN This function turns the highpass filter at the RF input on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR highpassFilter/Turns the highpass filter at the RF input on and off.
ViStatus _VI_FUNC rsspecan_ConfigureHighpassFilter (ViSession instrSession,
                                                    ViBoolean highpassFilter)
{
    ViStatus    error   = VI_SUCCESS;
 
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
            RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER, highpassFilter), 3, "Highpass Filter");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Input Protection Reset
 * Purpose:  The instrument is equipped with an overload protection
 *           mechanism. This mechanism becomes active as soon as the power at
 *           the input mixer exceeds a value of 27 dBm. It ensures that the
 *           connection between RF input and input mixer is cut off.
 *           The function resets the attenuator into the state that it had
 *           before the overload condition was detected. It re-connects the
 *           RF input with the input mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_InputProtectionReset (ViSession instrSession,
                                                        ViInt32 window)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    sprintf (buffer, "Win%ld", window);
    viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, 
            RSSPECAN_ATTR_AMPL_PROTECTION_RESET, VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Service Comb Generator
 * Purpose:  This function configures the comb generator. This function takes 
 *           effect only if the internal reference signal is set using
 *           rsspecan_ServiceConfigureInput.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureCombGenerator (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 combFrequency)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      model[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               10, model));
    if (strstr (model, "FSL") == NULL)
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    sprintf (buffer, "Win%ld", window);

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, 
            RSSPECAN_ATTR_SERVICE_INPUT_COMB_FREQUENCY, combFrequency), 3, "Frequency");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Service HW Info
 * Purpose:  This function queries the contents of the module info table. 
 *           Table lines are output as string data and are separated by commas.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceHWInfo (ViSession instrSession,
                                          ViInt32 arraySize, 
                                          ViChar HWInfo[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *pbuffer=VI_NULL;
    ViUInt32    ret_val;
    ViUInt32     length;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    //viCheckParm (rsspecan_GetAttributeViString (instrSession, "", 
    //            RSSPECAN_ATTR_SERVICE_HW_INFO, arraySize, HWInfo), 2, "HW Info");
    viCheckErr (viPrintf (instrSession, "DIAG:SERV:HWIN?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_val));
    length=((ViUInt32)arraySize<=ret_val)?(ViUInt32)arraySize-1:ret_val;
    memcpy (HWInfo,pbuffer,length);
    HWInfo[length]='\0';
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer) free(pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Network Map Drive
 * Purpose:  This function maps a drive to a server or server folder of the
 *           network. As a prerequisite in Microsoft networks, sharing of
 *           this server or server folder must be enabled.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkMapDrive (ViSession instrSession,
                                            ViString driveLetter,
                                            ViString hostNameIPAddress,
                                            ViBoolean useOptionalParameters,
                                            ViString userName, ViString password,
                                            ViBoolean reconnectAtLogon)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "\0";
    ViChar      model[10];
    ViChar      *p2buf = buffer;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if ((strstr (model, "FSL") == NULL) && !rsspecan_IsFSV (instrSession))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    p2buf += sprintf (p2buf, "MMEM:NETW:MAP '%s','%s'", driveLetter, hostNameIPAddress); 
    
    if (useOptionalParameters == VI_TRUE)
    {
        p2buf += sprintf (p2buf, ",'%s','%s',%s", userName, password, switchArr[reconnectAtLogon]);
    }
    
    p2buf += sprintf (p2buf, "\n");
    
    viCheckErr (viPrintf (instrSession, buffer));
    
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (p2buf) free(p2buf);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Network Disconnect Drive
 * Purpose:  This function disconnects the selected drive.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkDisconnectDrive (ViSession instrSession,
                                                   ViString driveLetter)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_NETWORK_DISCONNECT_DRIVE,
                                                driveLetter), 2, "Drive Letter");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Network Unused Drives List
 * Purpose:  This function lists all unused network drive names.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkUnusedDrivesList (ViSession instrSession,
                                                    ViInt32 arraySize, ViChar unusedDrives[])
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_UNUSED_NETWORK_DRIVES,
                                                arraySize, unusedDrives), 3, "Unused Drives");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Network Used Drives List
 * Purpose:  This function lists all mapped network drives.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkUsedDrivesList (ViSession instrSession,
                                                  ViInt32 arraySize, ViChar usedDrives[])
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_USED_NETWORK_DRIVES,
                                                arraySize, usedDrives), 3, "Used Drives");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Network Show Folder Info
 * Purpose:  This function sets if the folder info is showed in the list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkShowFolderInfo (ViSession instrSession,
                                                  ViBoolean showFolderInfo)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_SHOW_NETWORK_FOLDER_INFO,
                                                showFolderInfo), 2, "Show Folder Info");
    

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Reset
 * Purpose:  This function resets the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_reset (ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_Simulating(instrSession) == VI_FALSE)
	{
		checkErr(rsspecan_ClearStatus(instrSession));
		checkErr(RsCore_Write(instrSession, "*RST"));
		checkErr(RsCore_QueryViStringShort(instrSession, "*OPC?", NULL));
	}

	checkErr(rsspecan_DefaultInstrSetup(instrSession));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:

	(void)RsCore_UnlockSession(instrSession);

	return error;
}

/*****************************************************************************
 * Function: rsspecan_ResetWithDefaults
 * Purpose:  This function resets the instrument and applies default settings
 *           from the RS Configuration Store based on the logical name
 *           from which the session was created.
 *****************************************************************************/
/*ViStatus _VI_FUNC rsspecan_ResetWithDefaults (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
    ViSession   vi      = Rs_ViSession(instrSession);

    checkErr( Rs_LockSession (vi, VI_NULL));
    checkErr( rsspecan_reset(instrSession));
    checkErr( Rs_ApplyDefaultSetup(vi));

Error:
    (void) Rs_UnlockSession(vi, VI_NULL);
    return error;
}
*/
/****************************************************************************
 *  Function: Disable
 *  Purpose:  This function places the instrument in a quiescent state as
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC rsspecan_Disable (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (Rs_Simulating(instrSession) == 0)
        {
         viCheckErr( RS_ERROR_FUNCTION_NOT_SUPPORTED); 
        }
Error:
    (void) Rs_UnlockSession (instrSession, VI_NULL);
    return error;
}



/*****************************************************************************
 * Function: Self Test
 * Purpose:  This function executes the instrument self-test and returns the
 *           result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_self_test (ViSession instrSession, ViInt16 *testResult,
                                      ViChar testMessage[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, testResult), 2, "Null address for Test Result");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, testMessage), 3, "Null address for Test Message");

	checkErr(RsCore_SelfTest(instrSession, testResult, 48, testMessage));

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Self Test Results
 * Purpose:  This function executes the instrument self-test and returns the
 *           result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelfTestResults (ViSession instrSession,
                                            ViInt32 bufferSize,
                                            ViChar results[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      command[] = "DIAG:SERV:STES:RES?\n";
    ViChar      *buffer = NULL;
    ViUInt32     retCnt=0;
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (viWrite (instrSession, (ViBuf) command, (ViUInt32) strlen (command), NULL));
    //checkErr (viRead (instrSession, (ViBuf) results, (ViUInt32) bufferSize, NULL));
    viCheckErr(Rs_ReadDataUnknownLength (instrSession, &buffer, &retCnt));
    if ((ViInt32)retCnt>bufferSize)
        strncpy (results, buffer, bufferSize);
    else
        strcpy (results, buffer);    
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (buffer) free (buffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;

}

/*****************************************************************************
 * Function: Revision Query
 * Purpose:  This function returns the driver and instrument revisions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_revision_query (ViSession instrSession, ViChar driverRev[],
                                           ViChar instrRev[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, driverRev), 2, "Null address for Driver Revision");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, instrRev), 3, "Null address for Instrument Revision");

	checkErr(RsCore_GetAttributeViString(instrSession, NULL, RSSPECAN_ATTR_SPECIFIC_DRIVER_REVISION,
		0, 256, driverRev));
	checkErr(RsCore_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_INSTRUMENT_FIRMWARE_REVISION,
		0, 256, instrRev));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Instrument Status Checking                                                 
 * Purpose:  This function switches ON/OFF instrument status checking of the 
 *           instrument 
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_setCheckStatus (ViSession instrSession,
                               ViBoolean statusChecking)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     p2value         = 0;
    
    p2value = (ViInt32) statusChecking;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_QUERY_INSTRUMENT_STATUS, 0, &p2value));

Error:
    return error;
}
/*****************************************************************************
 * Function: Option Checking                                                 
 * Purpose:  This function switches ON/OFF option checking of the instrument 
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_setCheckOption (ViSession instrSession,
                                        ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);

    sessionProperties -> optionChecking = state;

    return error;
}

/// HIFN This function switches ON/OFF various error checkings performed inside of the driver.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR optionChecking/This control switches option checking On or Off.
/// HIPAR rangeChecking/This control enables or disables range checking.
/// HIPAR statusChecking/This control enables or disables instrument state checking
ViStatus _VI_FUNC rsspecan_ConfigureErrorChecking (ViSession instrSession,
                                                  ViBoolean optionChecking,
                                                  ViBoolean rangeChecking,
                                                  ViBoolean statusChecking)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	rsSession->optionChecking = optionChecking;
	checkErr(RsCore_SetAttributeViBoolean(instrSession, "", RS_ATTR_QUERY_INSTRUMENT_STATUS, 0, statusChecking));
	checkErr(RsCore_SetAttributeViBoolean(instrSession, "", RS_ATTR_RANGE_CHECK, 0, rangeChecking));

Error:
	return error;
}

/*****************************************************************************
 * Function: Memory Size
 * Purpose:  This function returns the memory size installed on related boards.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MemorySize (ViSession instrSession,
                                       ViInt32 *memorySize)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      model[10];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               10, model));
    
    if ((strstr (model, "FSL") != NULL) || (rsspecan_IsFSV (instrSession)))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);

    if (memorySize == VI_NULL)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Memory Size");
        
    viCheckErr( viPrintf (instrSession,"SYST:MSIZe? MBO\n"));
    viCheckErr( viScanf (instrSession,"%ld", memorySize));    

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Error Query
 * Purpose:  This function queries the instrument error queue and returns
 *           the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_error_query (ViSession instrSession, ViInt32 *errCode,
                                        ViChar errMessage[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, errCode), 2, "Null address for Error Code");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, errMessage), 3, "Null address for Error Message");

	checkErr(RsCore_ErrorQueryAll(instrSession, errCode, 1024, errMessage));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Error List
 * Purpose:  This function reads all system messages and returns a list of 
 *           comma separated strings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ErrorList (ViSession instrSession,
                                      ViInt32 bufferSize, 
                                      ViChar errors[])
{
    ViStatus    error   = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (bufferSize < 0)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Buffer Size");
    if (errors == VI_NULL)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Null address for Errors");

    viCheckErr (rsspecan_GetAttributeViString( instrSession, "", RSSPECAN_ATTR_SYST_ERR_LIST, 
        bufferSize, errors));

Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/// HIFN This function queries all system messages, or all messages of a defined type, 
/// HIFN displayed in the status bar for a specific measurement channel (application).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize 
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR messageType/This control selects the message type.
/// HIPAR channelName/String containing the name of the channel. 
/// HIPAR bufferSize/Sets the size of Messages string.
/// HIPAR messages/String containing all messages of the selected type for the specified measurement 
/// HIPAR messages/channel. Each message is separated by a comma and inserted in parentheses.
/// HIPAR messages/If no messages are available, empty parentheses are returned.
ViStatus _VI_FUNC rsspecan_ErrorListSpecificType (ViSession instrSession,
                                                  ViInt32 messageType,
                                                  ViString channelName,
                                                  ViInt32 bufferSize,
                                                  ViChar messages[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      model[10];
    ViChar      *pbuffer = NULL;
    ViUInt32    retCnt = 0;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               10, model));
    
    if ((strstr (model, "FSW") == NULL))
        checkErr (RS_ERROR_INSTRUMENT_MODEL);
    
    if (rsspecan_invalidViInt32Range (messageType, RSSPECAN_VAL_MESSAGE_TYPE_ALL, RSSPECAN_VAL_MESSAGE_TYPE_MESSAGE) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Message Type");
    }
    
    if (channelName == VI_NULL)
    {
        viCheckErr (viPrintf (instrSession,"SYST:ERR:EXT? %s\n", messageTypeArr[messageType]));
    }
    else
    {
        viCheckErr (viPrintf (instrSession,"SYST:ERR:EXT? %s,\'%s\'\n", 
                                                    messageTypeArr[messageType], channelName));
    }
    
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &retCnt));
    
    retCnt = ((ViUInt32) bufferSize>retCnt)?retCnt:bufferSize;
    
    strncpy (messages, pbuffer, retCnt); 

    checkErr (rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer) free(pbuffer);
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Error Message
 * Purpose:  This function translates the error codes returned by this
 *           instrument driver into user-readable strings.
 *
 *           Note:  The caller can pass VI_NULL for the vi parameter.  This
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_error_message (ViSession instrSession, ViStatus errorCode,
                                          ViChar errorMessage[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	static RsCoreStringValueTable errorTable =
	{
		RSSPECAN_ERROR_CODES_AND_MSGS,
		{0, NULL}
	};

	// all VISA and RS error codes are handled as well as codes in the table
	viCheckParm(RsCore_InvalidNullPointer(instrSession, errorMessage), 3, "Null address for Error Message");

	checkErr(RsCore_GetSpecificDriverStatusDesc(instrSession, errorCode, errorMessage, 256, errorTable));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Clear Instrument Errors
 * Purpose:  This function deletes all entries in the table SYSTEM MESSAGES.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ClearInstrumentErrors (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_SYST_ERR_CLEAR, VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Preset
 * Purpose:  This function initiates an instrument reset.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_Preset (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_SYST_PRESET, VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function defines the operating mode that is activated when you 
/// HIFN  switch on the R&S FSW or press the PRESET key.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR presetOperatingMode/This control sets the type of limit check for all ranges.
ViStatus _VI_FUNC rsspecan_ConfigurePresetOperatingMode(
    ViSession   instrSession,
    ViInt32 presetOperatingMode
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_PRESET_OPERATING_MODE,
                                               presetOperatingMode), 2, "Preset Operating Mode");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*****************************************************************************
 * Function: ID String Factory
 * Purpose:  This function sets the *IDN string to the factory default 
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_IDStringFactory (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", 
                RSSPECAN_ATTR_SYST_ID_FACTORY, VI_NULL));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get ID String Factory
 * Purpose:  This function returns "1" for the factory default state and "0" 
 *           if the ID string is changed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetIDStringFactory (ViSession instrSession,
                                               ViInt32 *factoryDefault)
{
    ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_GetAttributeViInt32 (instrSession, "", 
                RSSPECAN_ATTR_SYST_ID_FACTORY_QUERY, factoryDefault));
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Set Timeout
 * Purpose:  This function sets the OPC timeout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetTimeout (ViSession instrSession,
                                       ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, (ViInt32)timeout));

Error:
	return error;
}

/*****************************************************************************
 * Function: Get Timeout
 * Purpose:  This function returns the OPC timeout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTimeout (ViSession instrSession,
                                       ViUInt32 *timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 timeoutInt;

	checkErr(RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, &timeoutInt));
	*timeout = (ViUInt32)timeoutInt;

Error:
	return error;
}

/// HIFN This function specifies the minimum timeout value to use (in
/// HIFN milliseconds) when accessing the device associated with the
/// HIFN given session.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  
/// HIPAR VISATimeout/This control sets the timeout value.
ViStatus _VI_FUNC rsspecan_SetVISATimeout(ViSession instrSession,
                                  ViUInt32 VISATimeout)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_SetVisaTimeout(instrSession, VISATimeout));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function returns the minimum timeout value to use (in
/// HIFN milliseconds) when accessing the device associated with the
/// HIFN given session.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  
/// HIPAR VISATimeout/This control gets the timeout value.
ViStatus _VI_FUNC rsspecan_GetVISATimeout(ViSession instrSession,
                                  ViUInt32 *VISATimeout)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_GetVisaTimeout(instrSession, VISATimeout));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Lock Local Key
 * Purpose:  This function is used to activate the LLO (local lockout) or to
 *           return to the local mode (GTL go to local). When set to ON, the
 *           local button on the instrument is deactivated.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_LockLocalKey (ViSession instrSession,
                                         ViBoolean lockLocalKey)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", 
        RSSPECAN_ATTR_LOCK_LOCAL_KEY, lockLocalKey), 2, "Lock Local Key");
                              
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function clears status.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_ClearStatus(
    ViSession   instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_ClearStatus(instrSession));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function returns the ID Query response string.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR bufferSize/Size of IDQueryResponse buffer
/// HIPAR idQueryResponse/Returns the ID Query response string. The array should consist of at 
/// HIPAR idQueryResponse/least 256 elements.
ViStatus _VI_FUNC rsspecan_IDQueryResponse (ViSession instrSession,
                                         ViUInt32 bufferSize,
                                         ViChar IDQueryResponse[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_QueryViString(instrSession, "*IDN?", 256, IDQueryResponse));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Stops further command processing until all commands sent before *WAI 
/// HIFN  have been executed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_ProcessAllPreviousCommands(
    ViSession   instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_WriteInstrData(instrSession, "*WAI"));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function queries the OPC.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR opc/Queries the OPC.
ViStatus _VI_FUNC rsspecan_QueryOPC(
    ViSession   instrSession,
    ViInt32*    opc
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_QueryViInt32(instrSession, "*OPC?", OPC));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function sets fast sweep mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR fastSweepMode/Fast sweep mode
ViStatus _VI_FUNC rsspecan_SetFastSweepMode (ViSession instrSession,
                                             ViInt32 fastSweepMode)
{
    ViStatus    error = VI_SUCCESS;
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
//  viCheckParm(rsspecan_invalidViInt32Range (instrSession, fastSweepMode, RSSPECAN_VAL_FAST_SWEEP_NORMAL, RSSPECAN_VAL_FAST_SWEEP_FAST), 2, "Fast Sweep Mode");
    
    if (rsspecan_invalidViInt32Range (fastSweepMode, RSSPECAN_VAL_FAST_SWEEP_NORMAL, RSSPECAN_VAL_FAST_SWEEP_FAST) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Fast Sweep Mode");  
      
    sessionProperties->fastSweepInstrument = fastSweepMode;
Error:
    return error;
}

/*****************************************************************************
 * Function: Get Error and Clear Error Functions
 * Purpose:  These functions enable the instrument driver user to
 *           get or clear the error information the driver associates with the
 *           RS session.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetError (ViSession instrSession,
                                     ViStatus *errorCode,
                                     ViInt32 bufferSize,
                                     ViChar description[])
{
	ViStatus error = VI_SUCCESS;

	// Lock Session - do not jump to the end, even with invalid instrSession the function continues further
	(void)(RsCore_LockSession(instrSession));

	// Test for nulls and acquire error data
	viCheckParm(RsCore_InvalidNullPointer(instrSession, errorCode), 2, "Null address for Error");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, description), 4, "Null address for Description");

	checkErr(RsCore_GetErrorCompleteDescription(instrSession, &rsspecan_error_message, errorCode, bufferSize, description));

Error:
	// Unlock Session
	(void)RsCore_UnlockSession(instrSession);  // IGNORE: ERROR!!! Missing Lock
	return error;
}

ViStatus _VI_FUNC rsspecan_ClearError (ViSession instrSession)
{
	return RsCore_ClearErrorInfo(instrSession);
}

/*****************************************************************************
 * Function: WriteInstrData and ReadInstrData Functions
 * Purpose:  These functions enable the instrument driver user to
 *           write and read commands directly to and from the instrument.
 *
 *           Note:  These functions bypass the RS attribute state caching.
 *                  WriteInstrData invalidates the cached values for all
 *                  attributes.
 *****************************************************************************/

ViStatus _VI_FUNC rsspecan_WriteInstrData (ViSession instrSession, ViString writeBuffer)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	error = RsCore_Write(instrSession, writeBuffer);

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

ViStatus _VI_FUNC rsspecan_ReadInstrData (ViSession instrSession, ViInt32 numBytes,
                                          ViChar rdBuf[], ViUInt32 *bytesRead)
{
	ViStatus error = VI_SUCCESS;
	ViChar* responseString = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_ReadViStringUnknownLength(instrSession, &responseString));
	checkErr(RsCore_CopyToUserBufferAsciiData(instrSession, rdBuf, numBytes, responseString));
	*bytesRead = (ViUInt32)strlen(responseString);

Error:
	if (responseString)
		free(responseString);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/




/*****************************************************************************
 *------------------------ Global Session Callbacks -------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: Rs_CheckStatusCallback
 * Purpose:  This function queries the instrument to determine if it has
 *           encountered an error. If the instrument has encountered an
 *           error, this function returns the RS_ERROR_INSTRUMENT_SPECIFIC
 *           error code. This function is called by the
 *           Rs_CheckStatus utility function. The
 *           Rs_SetAttribute and Rs_GetAttribute functions invoke this
 *           function when the optionFlags parameter includes the
 *           RS_VAL_DIRECT_USER_CALL flag.
 *
 *           The user can disable calls to this function by setting the
 *           RSSPECAN_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE.  The driver can
 *           disable the check status callback for a particular attribute by
 *           setting the RS_VAL_DONT_CHECK_STATUS flag.
 *
 *****************************************************************************/
ViStatus rsspecan_CheckStatusCallback (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt16     stb     = 0;
    ViChar      buffer[5];
    ViUInt32    ret_cnt = 0;

    viCheckErr (viWrite (instrSession, (ViBuf) "*STB?\n", 6, NULL));
    viCheckErr (viRead (instrSession, (ViBuf) buffer, 4, &ret_cnt));
    buffer[ret_cnt] = '\0';
    stb = (ViInt16) atol (buffer);

    if ((stb & IEEE_488_2_ERROR_MASK) != 0)
    {
        viCheckErr (RS_ERROR_INSTR_SPECIFIC);
    }

Error:

    return error;
}

/*****************************************************************************
 * Function: Rs_WaitForOPCCallback
 * Purpose:  This function waits until the instrument has finished processing
 *           all pending operations. This function is called by the
 *           Rs_WaitForOPC utility function. The RS engine invokes
 *           this function in the following two cases:
 *           - Before invoking the read callback for attributes for which the
 *             RS_VAL_WAIT_FOR_OPC_BEFORE_READS flag is set.
 *           - After invoking the write callback for attributes for which the
 *             RS_VAL_WAIT_FOR_OPC_AFTER_WRITES flag is set.
 *
 *****************************************************************************/
ViStatus rsspecan_WaitForOPCCallback (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
    ViUInt16    response    = 0;
    ViUInt16    intfc   = VI_INTF_GPIB;
    ViInt32     opc_timeout = 0;
    clock_t     start_time = clock();
    ViReal64    opc_timeout_ms = 0.0;
    double      elapsed_time_ms;
    double      ms_divider = (double)CLOCKS_PER_SEC / 1000.0;

    checkErr( Rs_GetAttribute (instrSession, VI_NULL, RS_ATTR_OPC_TIMEOUT, 0,
        (ViInt32) sizeof (opc_timeout), &opc_timeout));

    opc_timeout_ms = (ViReal64) opc_timeout;

    viCheckErr( viGetAttribute (instrSession, VI_ATTR_INTF_TYPE, &intfc));

    switch (intfc)
    {
        case VI_INTF_ASRL: /* Serial intfc */
            
            /* Not supported */

        break;
        default:

            do
            {
                error = viReadSTB (instrSession, &response);
                if ((response & IEEE_488_2_STB_OPC_POLL_MASK) != 0)
                    break;

                elapsed_time_ms = (double)(clock() - start_time)/ms_divider;
                if (elapsed_time_ms > opc_timeout_ms)
                    return VI_ERROR_TMO;
                
                if (elapsed_time_ms < 10.0)
                {/* Do it as fast as possible, therefore no Sleep */
                }
                else if (elapsed_time_ms < 100.0)
                    Sleep (1);
                else if (elapsed_time_ms < 1000.0)
                    Sleep (5);
                else if (elapsed_time_ms < 5000.0)
                    Sleep (10);
                else if (elapsed_time_ms < 10000.0)
                    Sleep (50);
                else if (elapsed_time_ms < 20000.0)
                    Sleep (100);
                else if (elapsed_time_ms < 30000.0)
                    Sleep (500);
                else
                    Sleep (1000);
                
            } while ((response & IEEE_488_2_STB_OPC_POLL_MASK) == 0);

            if ((response & IEEE_488_2_ERROR_MASK) != 0)
            {
                error =  RS_ERROR_INSTRUMENT_STATUS;
                
            }
        }
Error:
    return error;
}    

/*****************************************************************************
 *- User Defined Attribute Callbacks ----------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- User Callbacks ----------------------------------------------------------*
 *****************************************************************************/


/*****************************************************************************
 *- Range Tables ------------------------------------------------------------*
 *****************************************************************************/


/*****************************************************************************
 *------------------- End Instrument Driver Source Code ---------------------*
 *****************************************************************************/

