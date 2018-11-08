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
static ViString listFilterTypeArr[] = {"NORM", "CFIL", "RRC", "FFT", "PULS"};
static ViString switchArr[] = {"OFF", "ON"};
static ViString statProbArr[] = {"P0_1", "P1", "P10"};
static ViString measTypeArr[] = {"MEAN", "PEAK", "CFAC"};
static ViString resultsString = "PASSED,FAILED";
static ViString selStatusRegArr[] = {
	"", ":POW", ":LIM1", ":LMAR1", ":SYNC", ":ACPL",
	":FREQ", ":LIM2", ":LMAR2", ":PNO", ":DIQ", ":DCPN",
	":LPN", ":LIM3", ":MOD", ":MOD:CFR", ":MOD:EVM",
	":MOD:FSK", ":MOD:IQRH", ":MOD:MAGN", ":MOD:PHAS",
	":EXT", ":EXT:INFO", ":TRAN"
};
static ViString statusRegArr[] = {
	"OPER:EVEN", "OPER:COND", "QUES:EVEN", "QUES:COND",
	"QUES:POW:EVEN", "QUES:POW:COND", "QUES:LIM:EVEN",
	"QUES:LIM:COND", "QUES:LMAR:EVEN", "QUES:LMAR:COND",
	"QUES:SYNC:EVEN", "QUES:SYNC:COND", "QUES:ACPL:EVEN",
	"QUES:ACPL:COND", "QUES:FREQ:EVEN", "QUES:FREQ:COND",
	"QUES:TRAN:EVEN", "QUES:TRAN:COND", "QUE",
	"QUES:LIM2:EVEN", "QUES:LIM2:COND", "QUES:LMAR2:EVEN",
	"QUES:LMAR2:COND", "QUES:PNO:EVEN", "QUES:PNO:COND",
	"OPER:PCAL:EVEN", "OPER:PCAL:COND", "QUES:DIQ:EVEN",
	"QUES:DIQ:COND", "QUES:DCPN:EVEN", "QUES:DCPN:COND",
	"QUES:LPN:EVEN", "QUES:LPN:COND", "QUES:LIM3:EVEN",
	"QUES:LIM3:COND", "QUES:MOD:EVEN", "QUES:MOD:COND",
	"QUES:MOD:CFR:EVEN", "QUES:MOD:CFR:COND", "QUES:MOD:EVM:EVEN",
	"QUES:MOD:EVM:COND", "QUES:MOD:FSK:EVEN", "QUES:MOD:FSK:COND",
	"QUES:MOD:IQRH:EVEN", "QUES:MOD:IQRH:COND", "QUES:MOD:MAG:EVEN",
	"QUES:MOD:MAG:COND", "QUES:MOD:PHAS:EVEN", "QUES:MOD:PHAS:COND",
	"QUES:EXT", "QUES:EXT:COND", "QUES:EXT:INFO", "QUES:EXT:INFO:COND"
};
static ViString VSAResultRegArr[] = {"Main", "CF", "EVM", "FSK", "IQRHO", "MagErr", "PhaseErr", NULL};
static ViString RegisterPartArr[] = {"RegCondition", "RegEnable", "RegEvent", "RegNTR", "RegPTR", NULL};
static ViString limTypeArr[] = {"LOW", "UPP", NULL};
static ViString directionArr[] = {"LEFT", "RIGH", "ABOV", "BEL", NULL};
static ViString windowTypeArr[] = {
	"DIAG", "MTAB", "PEAK", "RSUM", "SGR", "MAGN", "FREQ", "VECT", "RIMAG",
	"MCAP", "PRES", "PST", "PMAG", "PPH", "PFR", "PTR", "NOIS", "GAIN",
	"TEMP", "YFAC", "PHOT", "PCOL", "RES", "PNO", "RNO", "SNO", "SPUR",
	"GDEL", "PHAS", "RMAG", "RPH", "SPEC", "BITS", "CCON", "CDEP", "CDP",
	"CEVM", "CTAB", "LEV", "PCD", "PPCG", "PSYM", "SCON", "SEVM", "CURR",
	"PPWR", "CBUF", "MEAS", "REF", "EQU", "EVEC", "MACC", "MERR", "SYMB",
	"CMEM", "CONS", "CVC", "EVC", "EVSY", "FSP", "RSD", "RSGL", "SFI", "SFL",
	"PFPP", "ASUM", "BSTR", "CCDF", "FLAT", "FSRS", "EVSU", "FEVS", "IE",
	"IEA", "PSPE", "EVRP", "PVRP", "PVRR", "EVCH", "FDR", "SRES", "PDIS",
	"PSP", NULL
};
static ViString ChannelTypeArr[] = {
	"SANALYZER", "IQ", "PULSE", "ADEM", "MCGD", "NOISE", "PNOISE", "DDEM",
	"BWCD", "MWCD", "BC2K", "MC2K", "MDO", "BDO", "GSM", "WLAN", "LTE", NULL
};
static ViString messageTypeArr[] = {"ALL", "INFO", "WARN", "ERR", "FAT", "MESS", NULL};

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
ViStatus _VI_FUNC rsspecan_init(ViRsrc resourceName, ViBoolean IDQuery,
                                ViBoolean resetDevice, ViSession* newInstrSession)
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
	ViRsrc resourceName,
	ViBoolean IDQuery,
	ViBoolean resetDevice,
	ViString optionString,
	ViSession* newInstrSession
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

ViStatus _VI_FUNC rsspecan_SetSANMode(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SAN_MODE, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure Spectrum Display
/* Purpose:  This function creates, selects and deletes new spectrum displays.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrumDisplay(ViSession instrSession,
                                                    ViInt32 operation,
                                                    ViInt32 spectrumDisplay)
{
	ViStatus error = VI_SUCCESS;
	ViAttr attribute = 0;

	checkErr(RsCore_LockSession(instrSession));

	switch (operation)
	{
	case RSSPECAN_VAL_SAN_CREATE:
		attribute = RSSPECAN_ATTR_SPECTRUM_DISPLAY_CREATE;
		break;

	case RSSPECAN_VAL_SAN_SELECT:
		attribute = RSSPECAN_ATTR_SPECTRUM_DISPLAY_SELECT;
		break;

	case RSSPECAN_VAL_SAN_DELETE:
		attribute = RSSPECAN_ATTR_SPECTRUM_DISPLAY_DELETE;
		break;
	default:
		viCheckParm(VI_ERROR_PARAMETER2, 2, "Operation");
	}
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", attribute, spectrumDisplay),
		3, "Spectrum Display");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStartStop(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViReal64 startFrequency,
                                                       ViReal64 stopFrequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_START, startFrequency),
		3, "Start Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_STOP, stopFrequency),
		4, "Stop Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Center Span
 * Purpose:     This function configures the center frequency
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCenter(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 centerFrequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_CENTER, centerFrequency),
		3, "Center Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCenterSpan(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViReal64 centerFrequency,
                                                        ViReal64 span)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_CENTER, centerFrequency),
		3, "Center Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_SPAN, span),
		4, "Span");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Step Size
 * Purpose:     This function sets the step width of the center frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStepSize(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViReal64 stepSize)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_CENTER_LINK, RSSPECAN_VAL_CENT_FREQ_LINK_OFF));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_CENTER_STEP, stepSize),
		3, "Step Size");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Step Size Auto
 * Purpose:     This function couples the step size of the center frequency
 *              to the span (ON) or sets the value of the center frequency
 *              entered via SENSe<1|2>:]FREQuency:CENTer:STEP (OFF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStepSizeAuto(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViBoolean stepSizeAuto)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_STEP_AUTO, stepSizeAuto),
		3, "Step Size Auto");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Coupling Factor
 * Purpose:     This function couples the step width of the center frequency
 *              to span (span >0) or to the resolution bandwidth (span = 0)
 *              or cancels the couplings and sets the coupling factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCouplingFactor(ViSession instrSession,
                                                            ViInt32 window,
                                                            ViInt32 coupling,
                                                            ViInt32 couplingFactor)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_CENTER_LINK, coupling),
		3, "Coupling");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_CENTER_LINK_FACTOR, couplingFactor),
		4, "Coupling Factor");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Span Full
 * Purpose:     This function sets the frequency span in the analyzer mode
 *              to its maximum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencySpanFull(ViSession instrSession,
                                                      ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_SPAN_FULL, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Frequency Zero Span
 * Purpose:     This function sets the span to 0 Hz. The x-axis becomes the
 *              time axis with the grid lines corresponding to 1/10 of the
 *              current sweep time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyZeroSpan(ViSession instrSession,
                                                      ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_SPAN,
		0.0));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyOffset(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 frequencyOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_OFFSET, frequencyOffset),
		3, "Frequency Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMode(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 frequencyMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MODE, frequencyMode),
		3, "Frequency Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Time
 * Purpose:     This function configures the sweep time values of the spectrum
 *              analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepTime(ViSession instrSession,
                                              ViInt32 window,
                                              ViBoolean sweepTimeAuto,
                                              ViReal64 sweepTime)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SWEEP_TIME_AUTO, sweepTimeAuto),
		3, "Sweep Time Auto");

	if (sweepTimeAuto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SWEEP_TIME, sweepTime),
			4, "Sweep Time");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Coupling Auto
 * Purpose:     This function activates auto coupling of selected coupling
 *              method.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepCouplingAuto(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 sweepCoupling)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViAttr attribute = 0;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	switch (sweepCoupling)
	{
	case RSSPECAN_VAL_COUPLING_RBW:
		attribute = RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_AUTO;
		break;

	case RSSPECAN_VAL_COUPLING_VBW:
		attribute = RSSPECAN_ATTR_VIDEO_BANDWIDTH_AUTO;
		break;

	case RSSPECAN_VAL_COUPLING_SWEEP_TIME:
		attribute = RSSPECAN_ATTR_SWEEP_TIME_AUTO;
		break;
	default:
		viCheckParm(VI_ERROR_PARAMETER3, 3, "Sweep Coupling");
	}

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, attribute, VI_TRUE),
		1, "Dummy");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Coupling
 * Purpose:     This function configures the coupling value
 *              of the spectrum analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepCoupling(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 sweepCoupling,
                                                  ViReal64 couplingValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViAttr attribute = 0;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

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
		viCheckParm(VI_ERROR_PARAMETER3, 3, "Sweep Coupling");
	}

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, attribute, couplingValue),
		4, "Coupling Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Coupling Advanced
 * Purpose:     This function configures advanced parameters of coupling and
 *              sweeping.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepCouplingAdvanced(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 ratioSelection,
                                                          ViReal64 value)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViAttr attribute = 0;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	switch (ratioSelection)
	{
	case RSSPECAN_VAL_COUPLING_RATIO_RBW:
		attribute = RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_RATIO;
		break;
	case RSSPECAN_VAL_COUPLING_RATIO_VBW:
		attribute = RSSPECAN_ATTR_VIDEO_BANDWIDTH_RATIO;
		break;
	default:
		viCheckParm(VI_ERROR_PARAMETER3, 3, "Ratio Selection");
	}

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, attribute, value),
		4, "Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Main PLL Bandwidth
 * Purpose:     This function defines the bandwidth of the main PLL of the
 *              instrument synthesizer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMainPLLBwid(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 PLL)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PLL_BANDWIDTH, PLL),
		3, "PLL");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Filter Type
 * Purpose:     This function sets the filter type for the resolution bandwidth.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFilterType(ViSession instrSession,
                                               ViInt32 window,
                                               ViInt32 filterType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_FILTER_TYPE, filterType),
		3, "Filter Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure VBW Mode
 * Purpose:     This function selects the position of the video filter in the
 *              signal path, provided that the resolution bandwidth is <=100 kHz.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureVBWMode(ViSession instrSession,
                                            ViInt32 window,
                                            ViInt32 videoFilterType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VIDEO_BANDWIDTH_TYPE, videoFilterType),
		3, "Video Filter Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure FFT Filter Mode
 * Purpose:     This function allows to specify FFT filter.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFFTFilterMode(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 FFTFilterMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FFT_FILTER_MODE, FFTFilterMode),
		3, "FFT Filter Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Averaging
 * Purpose:     This function switches on or off the average calculation for
 *              the selected trace in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAveraging(ViSession instrSession,
                                              ViInt32 window,
                                              ViInt32 trace,
                                              ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		trace_range = 4;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AVG_STATE, state),
		4, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureAveragingType(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 averagingType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AVG_TYPE, averagingType),
		3, "Averaging Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Averaging Count
 * Purpose:     This function configures the averaging count.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAveragingCount(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 count)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AVG_COUNT, count),
		3, "Count");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Acquisition
 * Purpose:     This function configures the acquisition attributes of the
 *              spectrum analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAcquisition(ViSession instrSession,
                                                ViInt32 window,
                                                ViBoolean sweepModeContinuous,
                                                ViInt32 numberOfSweeps)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, sweepModeContinuous),
		3, "Sweep Mode Continuous");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_NUMBER_OF_SWEEPS, numberOfSweeps),
		4, "Number Of Sweeps");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Vertical Scale
 * Purpose:     This function configures the vertical scale of analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureVerticalScale(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 verticalScale)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession)
		&& (verticalScale == RSSPECAN_VAL_TRACE_SPACE_PCT))
	viCheckParm(RS_ERROR_INVALID_PARAMETER, 3, "Vertical Scale");

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, verticalScale, 0, 3),
		3, "Vertical Scale");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VERTICAL_SCALE, verticalScale),
		3, "Vertical Scale");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Horizontal Scale
 * Purpose:     This function toggles between linear and logarithmic display
 *              of the x-axis.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHorizontalScale(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 horizontalScale)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HORIZONTAL_SCALE, horizontalScale),
		3, "Horizontal Scale");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Points
 * Purpose:     This function defines the number of measurement points for
 *              one sweep run.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepPoints(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 sweepPoints)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SWEEP_POINTS, sweepPoints),
		3, "Sweep Points");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Get Sweep Number
 * Purpose:     This function returns the current number of started sweeps.
 *              A sweep count value should be set and the device should be
 *              in single sweep mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSweepNumber(ViSession instrSession,
                                          ViInt32 window,
                                          ViInt32* numberOfSweeps)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SWEEP_COUNT_CURRENT, numberOfSweeps),
		3, "Number Of Sweeps");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Sweep Points
 * Purpose:     This function returns the number of measurement points for
 *              one sweep run.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySweepPoints(ViSession instrSession,
                                            ViInt32 window,
                                            ViInt32* sweepPoints)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SWEEP_POINTS, sweepPoints),
		3, "Sweep Points");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function selects the sweep type optimization mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR sweepOptimization/Selects the sweep type optimization mode.
ViStatus _VI_FUNC rsspecan_ConfigureSweepOptimization(ViSession instrSession,
                                                      ViInt32 sweepOptimization)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SWEEP_OPTIMIZATION, sweepOptimization),
		2, "Sweep Optimization");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Sweep Type
 * Purpose:     This function configures the sweep type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSweepType(ViSession instrSession,
                                              ViInt32 sweepType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SWEEP_TYPE, sweepType),
		2, "sweep Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Attenuation
 * Purpose:     This function configures the input attenuation
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAttenuation(ViSession instrSession,
                                                ViInt32 window,
                                                ViBoolean attenuationAuto,
                                                ViReal64 attenuation)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_ATTENUATION_AUTO, attenuationAuto),
		2, "Attenuation Auto");
	if (attenuationAuto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_ATTENUATION, attenuation),
			3, "Attenuation");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Reference Level
 * Purpose:     This function configures the reference level
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevel(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViReal64 referenceLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel),
		2, "Reference Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Reference Level Offset
 * Purpose:     This function configures the offset value to the reference level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevelOffset(ViSession instrSession,
                                                         ViInt32 window,
                                                         ViReal64 referenceLevelOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, referenceLevelOffset),
		3, "Reference Level Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Level
 * Purpose:     This function configures the vertical attributes of the
 *              spectrum analyzer.  This corresponds to attributes like
 *              amplitude units, input attenuation, input impedance, reference
 *              level, and reference level offset.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureLevel(ViSession instrSession,
                                          ViInt32 window,
                                          ViInt32 amplitudeUnits,
                                          ViReal64 inputImpedance,
                                          ViReal64 referenceLevel,
                                          ViReal64 referenceLevelOffset,
                                          ViBoolean attenuationAuto,
                                          ViReal64 attenuation)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AMPLITUDE_UNITS, amplitudeUnits),
		3, "Amplitude Units");
	if (!RsCore_IsInstrumentModel(instrSession, "FMU"))
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_INPUT_IMPEDANCE,
			(ViInt32) inputImpedance), 4, "Input Impedance");
	}
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL, referenceLevel),
		5, "Reference Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, referenceLevelOffset),
		6, "Reference Level Offset");
	if (!RsCore_IsInstrumentModel(instrSession, "FMU"))
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_ATTENUATION_AUTO, attenuationAuto),
			7, "Attenuation Auto");
		if (attenuationAuto == VI_FALSE)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_ATTENUATION, attenuation),
				8, "Attenuation");
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Attenuation Mode
 * Purpose:     This function swicthes the behaviour for the attenuator.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAttenuationMode(ViSession instrSession,
                                                    ViInt32 window, ViInt32 mode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_ATTENUATION_MODE, mode),
		2, "Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Parameter Coupling
 * Purpose:     In operating mode SPECTRUM this function selects the parameter
 *              coupling between the two measurement windows screen A and B.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureParameterCoupling(ViSession instrSession,
                                                      ViInt32 parameterCoupling)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_PARAMETER_COUPLING, parameterCoupling),
		2, "Parameter Coupling");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Reference Level Coupling
 * Purpose:     This function defines wether the reference value for the Y axis
 *              is coupled to the reference level (default) or not.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevelCoupling(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean coupledReferenceLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DISP_REF_VALUE_AUTO, coupledReferenceLevel),
		2, "Coupled Reference Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure IF Output Source
 * Purpose:     This function switches the source of the IF output between
 *              the demodulated signal (2 possible values) and the IF signal.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIFOutputSource(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 outputSource)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_IF_OUTPUT_SOURCE, outputSource),
		2, "Output Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function defines the frequency for the IF output.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR IFOutFrequency/Selects the point at which the frequency axis is truncated.
ViStatus _VI_FUNC rsspecan_ConfigureIFOutFrequency(ViSession instrSession,
                                                   ViReal64 IFOutFrequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_IF_OUTPUT_FREQUENCY, IFOutFrequency),
		2, "IF Out Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function turns the 28 V supply of the noise source on the front panel on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR window/Selects the measurement window.
/// HIPAR noiseSource/Turns the 28 V supply of the noise source on the front panel on and off.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseSource(ViSession instrSession,
                                                ViInt32 window,
                                                ViBoolean noiseSource)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_SOURCE_STATE, noiseSource),
		2, "Noise Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Unit Power
 * Purpose:     This function selects the unit for power in the selected
 *              measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureUnitPower(ViSession instrSession,
                                              ViInt32 window, ViInt32 unitPower)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, unitPower, 0, 7),
		3, "Unit Power");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_UNIT_POWER, unitPower),
		3, "Unit Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function selects an existing measurement channel with the specified name.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR channelName/String containing the name of the channel.
ViStatus _VI_FUNC rsspecan_SelectMeasurementChannel(ViSession instrSession,
                                                    ViString channelName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SELECT_MEASUREMENT_CHANNEL, channelName),
		3, "Channel Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function adds an additional measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR channelType/This control selects the channel type of the new channel.
/// HIPAR channelName/String containing the name of the channel.
ViStatus _VI_FUNC rsspecan_AddMeasurementChannel(ViSession instrSession,
                                                 ViInt32 channelType,
                                                 ViString channelName)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd [100] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelType, RSSPECAN_VAL_CHANNEL_TYPE_SPECTRUM, RSSPECAN_VAL_CHANNEL_TYPE_LTE),
		2, "Channel Type");

	sprintf(cmd, "INST:CRE %s,'%s'", ChannelTypeArr[channelType], channelName);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function duplicates the currently selected measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_DuplicateMeasurementChannel(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_DUPLICATE_MEASUREMENT_CHANNEL, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ReplaceMeasurementChannel(ViSession instrSession,
                                                     ViString oldChannelName,
                                                     ViInt32 newChannelType,
                                                     ViString newChannelName)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd [100] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, newChannelType, RSSPECAN_VAL_CHANNEL_TYPE_SPECTRUM, RSSPECAN_VAL_CHANNEL_TYPE_LTE),
		3, "New Channel Type");

	sprintf(cmd, "INST:CRE:REPL '%s',%s,'%s'", oldChannelName, ChannelTypeArr[newChannelType],
	        newChannelName);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function renames a measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR oldChannelName/String containing the name of the channel you want to rename.
/// HIPAR newChannelName/String containing the new channel name.
ViStatus _VI_FUNC rsspecan_RenameMeasurementChannel(ViSession instrSession,
                                                    ViString oldChannelName,
                                                    ViString newChannelName)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd [100] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

	sprintf(cmd, "INST:REN '%s','%s'", oldChannelName, newChannelName);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function deletes a measurement channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR channelName/String containing the name of the channel you want to delete.
ViStatus _VI_FUNC rsspecan_DeleteMeasurementChannel(ViSession instrSession,
                                                    ViString channelName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_DELETE_MEASUREMENT_CHANNEL, channelName),
		3, "Channel Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function restores the default instrument settings in the current channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_PresetMeasurementChannel(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_PRESET_MEASUREMENT_CHANNEL, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryAllMeasurementChannels(ViSession instrSession,
                                                       ViInt32 arraySize,
                                                       ViInt32 channelType[],
                                                       ViChar channelName[],
                                                       ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;
	ViChar* pstring_value;
	ViInt32 cnt = 0;
	ViChar* p2buf;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "INST:LIST?", &pbuffer));

	p2buf = channelName;
	pstring_value = strtok(pbuffer, ",");
	while (pstring_value && cnt < arraySize)
	{
		sscanf(pstring_value, "'%[^']'", pstring_value);
		channelType[cnt] = RsCore_FindStringIndex(ChannelTypeArr, pstring_value);
		pstring_value = strtok(NULL, ",");

		sscanf(pstring_value, "'%[^']'", pstring_value);
		p2buf += sprintf(p2buf, "%s,", pstring_value);

		pstring_value = strtok(NULL, ",");

		cnt++;
	}
	*--p2buf = '\0';

	if (returnedValues != NULL)
		*returnedValues = cnt;

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trace
 * Purpose:     This function configures the trace to acquire.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTrace(ViSession instrSession,
                                          ViInt32 window,
                                          ViInt32 trace,
                                          ViInt32 traceType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 6),
		3, "Trace");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);
	switch (traceType)
	{
	case RSSPECAN_TRAC_MOD_BLANK:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TRACE_STATE, VI_FALSE));
		break;
	default:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TRACE_STATE, VI_TRUE));
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRACE_TYPE, traceType),
			4, "Trace Type");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trace Detector
 * Purpose:     This function configures the trace detector.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceDetector(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 trace,
                                                  ViBoolean detectorTypeAuto,
                                                  ViInt32 detectorType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		trace_range = 4;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DETECTOR_TYPE_AUTO, detectorTypeAuto),
		4, "Detector Type Auto");
	if (detectorTypeAuto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_DETECTOR_TYPE, detectorType),
			5, "Detector Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trace Reset Behavior
 * Purpose:     This function specifies whether or not the traces with peak
 *              or minimum value detection are reset after specific parameter
 *              changes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceResetBehavior(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 trace,
                                                       ViBoolean reset_atChange)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR%ld", window, trace);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TRACE_RESET_BEHAVIOR, reset_atChange),
		4, "Reset At Change");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Store Trace To File
 * Purpose:     This function stores the selected trace (1 to 3) in the
 *              measurement window indicated by window in a file with ASCII
 *              format.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreTraceToFile(ViSession instrSession,
                                            ViInt32 window,
                                            ViInt32 trace,
                                            ViString fileName)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL") || RsCore_IsInstrumentModel(instrSession, "FSW"))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, fileName), 4, "File Name");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEM:STOR%ld:TRAC %ld,'%s'", window, trace, fileName);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTraceExport(ViSession instrSession,
                                                ViInt32 exportMode,
                                                ViBoolean exportHeader)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TRACE_EXPORT_ALL, exportMode),
		2, "Export Mode");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_VSA_FILE_EXP_HEADER, exportHeader),
		3, "Export Header");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Trace Clear
 * Purpose:  This function clears the traces.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TraceClear(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_DISPLAY_TRACE_CLEAR, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Trace IQ Set
 * Purpose:     This function queries the read-only trace size attribute.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TraceIQSet(ViSession instrSession,
                                      ViInt32 window,
                                      ViReal64 samplingRate,
                                      ViInt32 triggerMode,
                                      ViInt32 triggerSlope,
                                      ViReal64 bandwidth,
                                      ViInt32 pretriggerSamples,
                                      ViInt32 no_ofSamples)

{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(cmd, "TRAC%ld:IQ:SET NORM,%.12lG,%.12lG,%s,%s,%ld,%ld\n",
	        window, bandwidth, samplingRate, rsspecan_rngTriggerSource.rangeValues[triggerMode].cmdString,
	        rsspecan_rngPolarity.rangeValues[triggerSlope].cmdString, pretriggerSamples, no_ofSamples);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 * Function: Trace IQ Sampling Rate
 * Purpose:  This function sets the sampling rate for the I/Q data acquisition.
 *           Thus the sampling rate can be modified without affecting the other
 *           settings.
 *===========================================================================*/
ViStatus _VI_FUNC rsspecan_TraceIQSamplingRate(ViSession instrSession,
                                               ViInt32 window,
                                               ViReal64 samplingRate)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_IQ_SAMPLE_RATE, samplingRate),
		2, "Sampling Rate");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function activates the bandwidth extension option R&S FSW-B160 / U160, if installed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/Activates the bandwidth extension option R&S FSW-B160 / U160, if installed.
ViStatus _VI_FUNC rsspecan_TraceIQMaximumBandwidthExtension(ViSession instrSession,
                                                            ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_IQ_MAX_BANDWIDTH_EXTENSION, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 * Function: Trace IQ Record Length
 * Purpose:  This function sets the record length for the acquired I/Q data.
 *           Increasing the record length automatically also increases the
 *           measurement time.
 *===========================================================================*/
ViStatus _VI_FUNC rsspecan_TraceIQRecordLength(ViSession instrSession,
                                               ViInt32 recordLength)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_IQ_RECORD_LENGTH, recordLength),
		2, "Record Length");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Trace IQ Averaging
 * Purpose:     This function configures the IQ data measurement averaging.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TraceIQAveraging(ViSession instrSession,
                                            ViInt32 window,
                                            ViBoolean averageState,
                                            ViInt32 averageCount)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_IQ_DATA_AVER_STATE, averageState),
		2, "Average State");
	if (averageState == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_IQ_DATA_AVER_COUNT, averageCount),
			3, "Average Count");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Enables or disables the IQ data evaluation mode and is a prerequisite for the IQ Analyzer functions.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR IQDataAnalyzerEnabled/Enables or disables the IQ data evaluation mode and is a prerequisite for the IQ Analyzer functions.

ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataAnalyzerEnabled(ViSession instrSession,
                                                               ViInt32 window,
                                                               ViBoolean IQDataAnalyzerEnabled)
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "buffer", RSSPECAN_ATTR_IQ_DATA_ANALYZER_STATE, IQDataAnalyzerEnabled),
		2, "IQ Data Analyzer Enabled");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Trace IQ Extended Bandwidth
/* Purpose:  This function allows measurement with extended usable signal
/*           bandwidth for sample rates in the range of 20.4 MHz to 40.8 MHz.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_TraceIQExtendedBandwidth(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 filter
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_IQ_EXT_BAND, filter));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Configure Trace IQ Data Acquisition
 * Purpose:     This function configures the trace IQ data acquisition.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataAcquisition(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean traceState)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_IQ_DATA_STATE, traceState),
		3, "Trace State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQStartSynchronization(ViSession instrSession,
                                                                ViInt32 window,
                                                                ViBoolean startSynchronization)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_IQ_START_SYNCHRONIZATION, startSynchronization),
		3, "Start Synchronization");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trace IQ BW Extension
 * Purpose:     This function switches the Bandwidth Extension FSQ-B72 on or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQBWExtension(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViBoolean BWExtensionState)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_IQ_DATA_WBAND_STATE, BWExtensionState),
		3, "BW Extension State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trace IQ Gate
 * Purpose:     This function configures the gate function for IQ data
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQGate(ViSession instrSession,
                                                ViBoolean gate, ViInt32 type,
                                                ViInt32 gateLength,
                                                ViInt32 gateGap,
                                                ViInt32 numberOfGatePeriods)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_IQ_DATA_GATE_STATE, gate),
		2, "Gate");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_IQ_DATA_GATE_TYPE, type),
		3, "Type");

	if (type == RSSPECAN_VAL_GATE_TYPE_EDGE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_IQ_DATA_GATE_LENGTH, gateLength),
			4, "Gate Length");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_IQ_DATA_GATE_GAP, gateGap),
		5, "Gate Gap");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_IQ_DATA_GATE_NUMBER_OF_PERIODS, numberOfGatePeriods),
		6, "Number Of Gate Periods");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trace IQ Data Display Type
 * Purpose:     This function configures the display type of the IQ data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataDisplayType(ViSession instrSession,
                                                           ViInt32 displayType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_IQ_DATA_DISPLAY_TYPE, displayType),
		2, "Display Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Configure Trace IQ Data Display Type
 * Purpose:     This function queries the filter bandwidth of the resampling
 *              filter, which is dependent on the sampling rate.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTraceIQFilterBandwidth(ViSession instrSession,
                                                       ViReal64* filterBandwidth)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_IQ_DATA_FILTER_BANDWIDTH, filterBandwidth),
		2, "Filter Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*
 * List Power Class
 */

/*****************************************************************************
 * Function:    Load IQ Data
 * Purpose:     This function loads the IQ data from the specified file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_LoadIQData(ViSession instrSession,
                                      ViString fileName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LOAD_IQ_DATA, fileName),
		2, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Store IQ Data
 * Purpose:     This function stores the IQ data from/to the specified file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreIQData(ViSession instrSession,
                                       ViString fileName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SAVE_IQ_DATA, fileName),
		2, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure IQ Data Export File Description
 * Purpose:     This function defines a description of the export file which is
 *              stored with the data and also displayed in the file selection
 *              dialog box for I/Q data import and export
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIQDataExportFileDescription(ViSession instrSession,
                                                                ViString description)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_IQ_DATA_EXPORT_FILE_DESCRIPTION, description),
		2, "Description");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryTPISTimeOffset(ViSession instrSession,
                                               ViInt32 trace,
                                               ViInt32* timeOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 2),
		2, "Trace");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "TR%ld", trace);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_QUERY_TPIS_TIME_OFFSET, timeOffset),
		3, "Time Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    List Power Measurement Off
 * Purpose:     This function deactivates the list measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ListPowerMeasurementOff(ViSession instrSession,
                                                   ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_LIST_POW_STATE_OFF, ""));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure List Power Average Type
 * Purpose:     This function sets the average type of the sense list measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureListPowerAverageType(ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 type)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LIST_POW_TYPE, type));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure List Power Sequence
 * Purpose:     This function configures the list of settings (max. 100 entries
 *              for an individual frequency point) for the multiple power
 *              measurement and starts a measurement sequence.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureListPowerSequence(ViSession instrSession,
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
	ViStatus error = VI_SUCCESS;
	ViChar* pwrite_buffer = NULL;
	ViChar* pbuffer;
	ViInt32 i = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, noofListItems, 1, 100),
		3, "No Of List Items");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, analyzerFrequency), 4, "Analyzer Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, referenceLevel), 5, "Reference Level");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, RFInputAttenuation), 6, "RF Input Attenuation");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, RFInputElectronicAttn), 7, "RF Input Electronic Attn");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, filterType), 8, "Filter Type");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, resolutionBandwidth), 9, "Resolutin Bandwidth");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, videoBandwidth), 10, "Video Bandwidth");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, measTime), 11, "Meas Time");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, triggerLevel), 12, "Trigger Level");

	/*
	 * Alloc buffer. Size is calclulated as:
	 * size of header + size required for one entry * number of entries
	 */
	viCheckAlloc(pwrite_buffer = (ViChar*) malloc((size_t)(20 + 240 * noofListItems)));

	pbuffer = pwrite_buffer;

	pbuffer += sprintf(pbuffer, ":SENS%ld:LIST:POW:SEQ ", window);

	for (i = 0; i < noofListItems; i++)
	{
		// Electronic attenuator is not in the signal path.
		if (RFInputElectronicAttn[i] < 0)
		{
			pbuffer += sprintf(pbuffer, "%.12f,%.12f,%.12f,OFF,%s,%.12f,%.12f,%.12f,%.12f,",
			                   analyzerFrequency[i],
			                   referenceLevel[i],
			                   RFInputAttenuation[i],
			                   listFilterTypeArr[filterType[i]], // RFInputEAttn[i],
			                   resolutionBandwidth[i],
			                   videoBandwidth[i],
			                   measTime[i],
			                   triggerLevel[i]);
		}
		else
		{
			pbuffer += sprintf(pbuffer, "%.12f,%.12f,%.12f,%.12f,%s,%.12f,%.12f,%.12f,%.12f,",
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

	*--pbuffer = '\0'; // Remove remaining comma

	sprintf(pbuffer, "\n");

	checkErr(RsCore_Write(instrSession, pwrite_buffer));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (pwrite_buffer) free(pwrite_buffer);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryListPowerSequence(ViSession instrSession,
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
	ViStatus error = VI_SUCCESS;
	ViChar* pwrite_buffer = NULL;
	ViChar* pbuffer;
	ViInt32 i = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, noofListItems, 1, 100),
		3, "No Of List Items");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, analyzerFrequency), 4, "Analyzer Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, referenceLevel), 5, "Reference Level");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, RFInputAttenuation), 6, "RF Input Attenuation");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, RFInputElectronicAttn), 7, "RF Input Electronic Attn");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, filterType), 8, "Filter Type");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, resolutionBandwidth), 9, "Resolutin Bandwidth");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, videoBandwidth), 10, "Video Bandwidth");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, measTime), 11, "Meas Time");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, triggerLevel), 12, "Trigger Level");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, listPowerResults), 13, "List Power Results");

	viCheckAlloc(pwrite_buffer = (ViChar*)malloc((size_t)(20 + 240 * noofListItems)));
	pbuffer = pwrite_buffer;

	pbuffer += sprintf(pbuffer, ":SENS%ld:LIST:POW:SEQ? ", window);

	for (i = 0; i < noofListItems; i++)
	{
		// Electronic attenuator is not in the signal path.
		if (RFInputElectronicAttn[i] < 0)
			pbuffer += sprintf(pbuffer, "%.12f,%.12f,%.12f,OFF,%s,%.12f,%.12f,%.12f,%.12f,",
			                   analyzerFrequency[i], referenceLevel[i], RFInputAttenuation[i],
			                   /* RFInputEAttn[i], */ listFilterTypeArr[filterType[i]], resolutionBandwidth[i],
			                   videoBandwidth[i], measTime[i], triggerLevel[i]);
		else
			pbuffer += sprintf(pbuffer, "%.12f,%.12f,%.12f,%.12f,%s,%.12f,%.12f,%.12f,%.12f,",
			                   analyzerFrequency[i], referenceLevel[i], RFInputAttenuation[i],
			                   RFInputElectronicAttn[i], listFilterTypeArr[filterType[i]], resolutionBandwidth[i],
			                   videoBandwidth[i], measTime[i], triggerLevel[i]);
	}

	*--pbuffer = '\0'; // Remove remaining comma

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, pwrite_buffer, noofListItems, listPowerResults, NULL));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (pwrite_buffer) free(pwrite_buffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query List Power Result
 * Purpose:     This function queries the result of a previous list measurement
 *              as configured and initiated with List Power Sequence.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryListPowerResult(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 noofResults,
                                                ViReal64 listPowerResults[],
                                                ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* ptag;
	ViInt32 index;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, noofResults, 1, INT_MAX),
		3, "No Of Results");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:LIST:POW:RES?", window);
	checkErr(RsCore_QueryViString(instrSession, cmd, buffer));
	ptag = strtok(buffer, ",");
	for (index = 0; ptag; index++)
	{
		if (index < noofResults)
			listPowerResults[index] = atof(ptag);
		ptag = strtok(NULL, ",");
	}
	if (returnedValues)
		*returnedValues = index;
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure List Power Set
 * Purpose:     This function defines the constant settings for the list during
 *              multiple power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureListPowerSet(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean peakMeas,
                                                 ViBoolean RMSMeas,
                                                 ViBoolean AVGMeas,
                                                 ViInt32 triggerMode,
                                                 ViInt32 triggerSlope,
                                                 ViReal64 triggerOffset,
                                                 ViReal64 gateLength)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, peakMeas), 3, "Peak Meas");
	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, RMSMeas), 4, "RMS Meas");
	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, AVGMeas), 5, "AVG Meas");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, triggerMode, 0, 3),
		6, "Trigger Mode");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, triggerSlope, 0, 1),
		7, "Trigger Slope");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, triggerOffset, 0.0, 100.0), 8, "Trigger Offset");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, gateLength, 0.0, 100.0), 9, "Gate Length");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:LIST:POW:SET %s,%s,%s,%s,%s,%.12f,%.12f", window, switchArr[peakMeas], switchArr[RMSMeas], switchArr[AVGMeas],
	         rsspecan_rngTriggerSource.rangeValues[triggerMode].cmdString, rsspecan_rngPolarity.rangeValues[triggerSlope].cmdString,
	         triggerOffset, gateLength);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Copy Trace
 * Purpose:     This function copies one trace array to another trace array.
 *              Any data in the destination trace is over written.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CopyTrace(ViSession instrSession,
                                     ViInt32 window,
                                     ViInt32 destinationTrace,
                                     ViInt32 sourceTrace)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, destinationTrace, 1, trace_range),
		3, "Destination Trace");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, sourceTrace, 1, trace_range),
		4, "Source Trace");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "TRAC%ld:COPY TRACE%ld,TRACE%ld", window,
	         destinationTrace, sourceTrace);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Subtract Traces
 * Purpose:     This function configures subtraction of traces.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSubtractTraces(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViReal64 position,
                                                   ViInt32 mode,
                                                   ViBoolean traceMathState)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 math_mode = RSSPECAN_VAL_LOG;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession) || RsCore_IsInstrumentModel(instrSession, "FSL"))
		math_mode = RSSPECAN_VAL_MPOW;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_LIN, math_mode),
		3, "Mode");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRACE_MATH_POSITION, position),
		3, "Position");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRACE_MATH_MODE, mode),
		4, "Mode");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TRACE_MATH_STATE, traceMathState),
		5, "Trace Math State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Subtract Traces
 * Purpose:     This function subtracts the array elements of trace defined
 *              in argument Trace 2 from TRACE1 of the instrument and stores
 *              the result in the TRACE1.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SubtractTraces(ViSession instrSession,
                                          ViInt32 window,
                                          ViInt32 trace2)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace2, 2, trace_range),
		3, "Trace 2");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MATH (TRACE1-TRACE%ld)", window, trace2);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn Marker Extension Group Functions ==============================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Marker
 * Purpose:     This function enables the active marker on the specified trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarker(ViSession instrSession,
                                           ViInt32 window,
                                           ViInt32 marker,
                                           ViBoolean markerEnabled,
                                           ViInt32 trace)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_ENABLED, markerEnabled),
		4, "Marker Enabled");

	if (markerEnabled == VI_TRUE)
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_TRACE, trace),
		5, "Trace");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Reference Marker
 * Purpose:     This function sets the marker frequency counter resolution and
 *              turns the marker frequency counter on/off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceMarker(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViInt32 trace)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,DM%ld", window, marker);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_DELTA_MARKER_REFERENCE_MARKER, trace),
		4, "Trace");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Frequency Counter
 * Purpose:     This function sets the marker frequency counter resolution and
 *              turns the marker frequency counter on/off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerFrequencyCounter(ViSession instrSession,
                                                           ViInt32 window, ViInt32 marker,
                                                           ViBoolean markerFrequencyCounter,
                                                           ViReal64 frequencyCounterResolution)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_FREQUENCY_COUNTER_ENABLED, markerFrequencyCounter),
		4, "Marker Frequency Counter");

	if (!RsCore_IsInstrumentModel(instrSession, "ZVH"))
	{
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_FREQUENCY_COUNTER_RESOLUTION, frequencyCounterResolution),
			5, "Freqeuncy Counter Resolution");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Marker Frequency Counter Result
 * Purpose:     This function queries the result of the frequency counter
 *              for the indicated marker in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerFrequencyCounterResult(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 marker,
                                                             ViReal64* counterFrequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_COUNT, counterFrequency),
		4, "Counter Freqeuncy");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Probability Marker Position
 * Purpose:     This function positions the selected marker to the given
 *              probability.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ProbabilityMarkerPosition(ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 marker,
                                                     ViReal64 probability)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_PROBABILITY, probability),
		4, "Probability");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Configure a band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR displayMode/Activates the band power marker in the specified window.
/// HIPAR bandPowerEnabled/Defines whether the result is displayed as a power or density.
/// HIPAR span/Defines the span (band) around the marker for which the power is measured.

ViStatus _VI_FUNC rsspecan_ConfigureMarkerBandPower(ViSession instrSession,
                                                    ViBoolean bandPowerEnabled,
                                                    ViInt32 displayMode,
                                                    ViReal64 span)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MARKER_BAND_POWER_STATE, bandPowerEnabled),
		2, "Band Power Enabled");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MARKER_BAND_POWER_MODE, displayMode),
		3, "Display Mode");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MARKER_BAND_POWER_SPAN, span),
		4, "Span");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function returns the value of the band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR result/Queries the value of the band power marker.

ViStatus _VI_FUNC rsspecan_QueryMarkerBandPower(ViSession instrSession,
                                                ViReal64* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_QUERY_MARKER_BAND_POWER_RESULT, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Threshold
 * Purpose:     This function configures marker threshold values. The marker
 *              threshold specifies a lower bound for ALL marker search
 *              functions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchThreshold(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViBoolean thresholdState,
                                                          ViReal64 markerThreshold)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_THRESHOLD_STATE, thresholdState),
		3, "Threshold State");

	if (thresholdState == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_THRESHOLD, markerThreshold),
			4, "Marker Threshold");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Peak Excursion
 * Purpose:     This function configures the marker peak excursion. The marker
 *              peak excursion specifies the minimum amplitude variation that
 *              can be recognized as a peak or minimum by the marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchPeakExcursion(ViSession instrSession,
                                                              ViInt32 window,
                                                              ViReal64 peakExcursion)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PEAK_EXCURSION, peakExcursion),
		3, "Peak Excursion");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Local Oscillator
 * Purpose:     This function configures state of local oscillator supression.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLocalOscillator(ViSession instrSession,
                                                                ViInt32 window,
                                                                ViBoolean localOscillatorSupression)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_LOEX, localOscillatorSupression),
		3, "Local Oscillator Supression");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Limits
 * Purpose:     This function configures marker search limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLimits(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViBoolean searchLimits,
                                                       ViReal64 searchLimitLeft,
                                                       ViReal64 searchLimitRight)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_STATE, searchLimits),
		3, "Serach Limit");
	if (searchLimits == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_LEFT, searchLimitLeft),
			4, "Search Limit Left");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_RIGHT, searchLimitRight),
			5, "Search Limit Right");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureSignalTrack(ViSession instrSession,
                                                ViInt32 window,
                                                ViBoolean signalTrackEnabled,
                                                ViReal64 signalTrackBandwidth,
                                                ViReal64 signalTrackThreshold,
                                                ViInt32 trace)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SIGNAL_TRACK_ENABLED, signalTrackEnabled),
		3, "Signal Track Enabled");
	if (signalTrackEnabled == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SIGNAL_TRACK_BWID, signalTrackBandwidth),
			4, "Signal Track Bandwidth");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SIGNAL_TRACK_THRESHOLD, signalTrackThreshold),
			5, "Signal Track Threshold");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SIGNAL_TRACK_TRACE, trace),
			6, "Trace");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Disable All Markers
 * Purpose:     This function turns off all the markers in selected
 *              measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisableAllMarkers(ViSession instrSession,
                                             ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_AOFF, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Marker Search
 * Purpose:     This function specifies the type of marker search and performs
 *              the search.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MarkerSearch(ViSession instrSession,
                                        ViInt32 window,
                                        ViInt32 marker,
                                        ViInt32 markerSearch)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	switch (markerSearch)
	{
	case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_PEAK, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_NEXT, NULL));
		break;

	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_LEFT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_LEFT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_RIGHT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_RIGHT, NULL));
		break;

	case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_MIN, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_MIN_NEXT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_LEFT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_MIN_LEFT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_RIGHT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_MIN_RIGHT, NULL));
		break;

	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, markerSearch), 4, "Parameter to set");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Marker Peak Auto
 * Purpose:     This function configures an automatic peak search action
 *              for Marker 1 at the end of each particular sweep. This function
 *              may be used during adjustments of a device under test to keep
 *              track of the actual peak marker position and level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MarkerPeakAuto(ViSession instrSession,
                                          ViInt32 window,
                                          ViInt32 markerSearch,
                                          ViBoolean autoPeak)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	switch (markerSearch)
	{
	case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_AUTO, autoPeak),
			4, "Auto Peak");
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_MIN_AUTO, autoPeak),
			4, "Auto Peak");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, markerSearch), 3, "Marker Search");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Move Marker
 * Purpose:     This function moves the active marker to the specified
 *              horizontal position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MoveMarker(ViSession instrSession,
                                      ViInt32 window,
                                      ViInt32 marker,
                                      ViReal64 markerPosition)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_POSITION, markerPosition),
		4, "Marker Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_LinkMarkerToAnotherMarker(ViSession instrSession,
                                                     ViInt32 window, ViInt32 firstMarker,
                                                     ViInt32 secondMarker,
                                                     ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, firstMarker, 1, 16),
		3, "First Marker");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, secondMarker, 1, 16),
		4, "Second Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld,SM%ld", window, firstMarker, secondMarker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_LINK_TO_MARKER, state),
		4, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_LinkDeltaMarkerToMarker(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 firstMarker,
                                                   ViInt32 secondMarker,
                                                   ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, firstMarker, 1, 16),
		3, "First Marker");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, secondMarker, 1, 16),
		4, "Second Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,DM%ld,M%ld", window, firstMarker, secondMarker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DELTA_MARKER_LINK_TO_MARKER, state),
		4, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViBoolean state
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MARKER_LINK_ALL_MARKERS, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Marker
 * Purpose:     This function returns the horizontal position and the marker
 *              amplitude level of the active marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarker(ViSession instrSession,
                                       ViInt32 window,
                                       ViInt32 marker,
                                       ViReal64* markerPosition,
                                       ViReal64* markerAmplitude)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_POSITION, markerPosition),
		4, "Marker Position");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_AMPLITUDE, markerAmplitude),
		5, "Marker Amplitude");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Set Instrument From Marker
 * Purpose:     This function makes the selected marker frequency to be
 *              the center frequency or step width of center frequency.
 *              It can also make the active marker amplitude to be
 *              the reference level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetInstrumentFromMarker(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 marker,
                                                   ViInt32 instrumentSetting)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	switch (instrumentSetting)
	{
	case RSSPECAN_VAL_INSTRUMENT_SETTING_FREQUENCY_CENTER:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_TO_CENTER, NULL));
		break;
	case RSSPECAN_VAL_INSTRUMENT_SETTING_FREQUENCY_STEP:
		{
			if (RsCore_CheckInstrumentModel(instrSession, "FMU|FSVR"))
			{
				checkErr(RS_ERROR_INSTRUMENT_MODEL);
			}

			checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_TO_STEP, NULL));
		}

		break;
	case RSSPECAN_VAL_INSTRUMENT_SETTING_REFERENCE_LEVEL:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_TO_REFERENCE, NULL));
		break;

	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, instrumentSetting), 4, "Parameter to set");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMarkerStepSize(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 stepSize)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_STEP_SIZE, stepSize),
		3, "Step Size");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Zoom
 * Purpose:     This function defines the range to be zoomed around marker 1
 *              in the selected measurement window. Marker 1 is activated first,
 *              if necessary.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerZoom(ViSession instrSession,
                                               ViInt32 window,
                                               ViReal64 markerZoom,
                                               ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 4, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_ZOOM, markerZoom),
		3, "Marker Zoom");

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_LinkMarkerAndDeltamarker(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 1;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_LINK, state),
		4, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMarkerNoiseMeasurement(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViInt32 marker,
                                                           ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_NOISE_STATE, state),
		4, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryMarkerNoiseMeasurementResult(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 marker,
                                                             ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_NOISE_RESULT, result),
		4, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Configure N dB Points
 * Purpose:     This function configures the N dB function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNdBPoints(ViSession instrSession,
                                              ViInt32 window,
                                              ViBoolean ndBState,
                                              ViReal64 ndBLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_NDB_STATE, ndBState),
		3, "N dB State");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_NDB_VAL, ndBLevel),
		4, "N dB Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query N dB Result
 * Purpose:     This function queries the frequency spacing (bandwidth) of
 *              the N-dB-down markers in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBResult(ViSession instrSession,
                                          ViInt32 window,
                                          ViReal64* ndBResult)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_NDB_RESULT, ndBResult),
		3, "N dB Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query N dB Frequencies
 * Purpose:     This function queries the frequency spacing (bandwidth) of
 *              the N-dB-down markers in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBFrequencies(ViSession instrSession,
                                               ViInt32 window,
                                               ViReal64* ndBFrequencyLower,
                                               ViReal64* ndBFrequencyHigher)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, ndBFrequencyLower), 3, "N dB Frequency Lower");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, ndBFrequencyHigher), 3, "N dB Frequency Higher");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:NDBD:FREQ?", window);
	checkErr(RsCore_QueryViString(instrSession, cmd, response));
	sscanf(response, "%le,%le", ndBFrequencyLower, ndBFrequencyHigher);
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query N dB Times
 * Purpose:     This function qqueries the two times of the N-dB-down marker
 /*             in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBTimes(ViSession instrSession,
                                         ViInt32 window,
                                         ViReal64* n_dBTimeLower,
                                         ViReal64* n_dBTimeHigher)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, n_dBTimeLower), 3, "N dB Frequency Lower");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, n_dBTimeHigher), 3, "N dB Frequency Higher");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:NDBD:TIME?", window);
	checkErr(RsCore_QueryViString(instrSession, cmd, response));
	sscanf(response, "%le,%le", n_dBTimeLower, n_dBTimeHigher);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query N dB Q Factor
 * Purpose:     This function queries the Q factor (quality) of the measured
 *              bandwidth for span>0.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNdBQFactor(ViSession instrSession,
                                           ViInt32 window,
                                           ViReal64* n_dBQFactor)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, n_dBQFactor), 3, "N dB Frequency Lower");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_NDB_Q_FACTOR, n_dBQFactor),
		3, "N dB Q Factor");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakList(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 marker,
                                                   ViInt32 peakListCount,
                                                   ViInt32 peakListSort,
                                                   ViInt32 peakListSize,
                                                   ViBoolean automaticPeaksSearch)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_COUNT, peakListCount),
		4, "Peak List Count");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_SORT, peakListSort),
		5, "Peak List Sort");

	if (!RsCore_IsInstrumentModel(instrSession, "FSP|FSL"))
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_SIZE, peakListSize),
			6, "Peak List Size");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_AUTO, automaticPeaksSearch),
			7, "Automatic Peaks Search");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function activates or deactivates the marker peak search.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/This control selects marker.
/// HIPAR peakSearch/Activates or deactivates the marker peak search.
ViStatus _VI_FUNC rsspecan_ActivateMarkerPeakSearch(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViBoolean peakSearch)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 max_marker = 4;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_STATE, peakSearch),
		4, "Peak Search");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListSettings(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViInt32 marker,
                                                           ViBoolean peakLabels,
                                                           ViInt32 maximumPeaks)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 max_marker = 4;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_PEAK_LABELS, peakLabels),
		4, "Peak Labels");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_MAXIMUM_PEAKS, maximumPeaks),
		5, "Maximum Peaks");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Store Marker Peak List
 * Purpose:     This function stores the current marker peak list in a *.dat
 *              file. The file consists of a data section containing the peak
 *              list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreMarkerPeakList(ViSession instrSession,
                                               ViString path)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MARKER_STORE_PEAK_LIST, path),
		2, "Path");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Marker Peak List Found
 * Purpose:     This function reads out the number of maxima found during
 *              the search. If no search for maxima has been performed,
 *              0 is returned.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerPeakListFound(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 marker,
                                                    ViInt32* peakListFound)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_FOUND, peakListFound),
		4, "Peak List Found");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Marker Peak List
 * Purpose:     This function queries the frequency spacing (bandwidth) of
 *              the N-dB-down markers in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryMarkerPeakList(ViSession instrSession,
                                               ViInt32 window,
                                               ViInt32 marker,
                                               ViInt32 arraySize,
                                               ViInt32 peakListSelection,
                                               ViReal64 peakList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	switch (peakListSelection)
	{
	case RSSPECAN_VAL_MARKER_SORT_X:
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK%ld:FUNC:FPE:X?", window, marker);
		break;
	case RSSPECAN_VAL_MARKER_SORT_Y:
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK%ld:FUNC:FPE:Y?", window, marker);
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, peakListSelection), 5, "Peak List Selection");
		break;
	}

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, arraySize, peakList, NULL));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Search Limits Zoom
 * Purpose:     This function configures the limits of the search range for
 *              markers and delta markers to the zoom area.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLimitsZoom(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean searchLimitsZoom)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_ZOOM, searchLimitsZoom),
		3, "Search Limits Zoom");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Demodulation
 * Purpose:     This function configures marker demodulation parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerDemodulation(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 marker,
                                                       ViBoolean state,
                                                       ViInt32 demodulationType,
                                                       ViReal64 markerStopTime,
                                                       ViBoolean continuousDemodulation,
                                                       ViBoolean squelchState,
                                                       ViReal64 squelchLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_DEMOD_STATE, state),
		4, "State");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MARKER_DEMOD_TYPE, demodulationType),
		5, "Demodulation Type");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MARKER_DEMOD_HOLDOFF, markerStopTime),
		6, "Marker Stop Time");

	if (RsCore_IsInstrumentModel(instrSession, "ZVH"))
	{
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_DEMOD_CONT, continuousDemodulation),
			7, "Continuous Demodulation");
	}

	if (!RsCore_IsInstrumentModel(instrSession, "FSL") || !rsspecan_IsFSV(instrSession))
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_DEMOD_SQUELCH_STATE, squelchState),
			8, "Squelch State");
		if (squelchState == VI_TRUE)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_DEMOD_SQUELCH_LEVEL, squelchLevel),
				9, "Squelch Level");
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Peak List Annotation
 * Purpose:     This function activates or deactivates the peak list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListAnnotation(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViBoolean label,
                                                             ViBoolean marker)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_LABEL, label),
		3, "Label");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_MARKER, marker),
		4, "Marker");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Peak List State
 * Purpose:     This function activates or deactivates the peak list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListState(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Marker Save to File
 * Purpose:     This function saves the data of active markers to a file,
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MarkerSaveToFile(ViSession instrSession,
                                            ViString path)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MARKER_FILE_EXPORT, path),
		2, "Path");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ReadMarkerPeakList(ViSession instrSession,
                                              ViInt32 noOfValues,
                                              ViReal64 frequency[],
                                              ViReal64 level[],
                                              ViReal64 deltaLimit[],
                                              ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 i, dataSize;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_dataReadTraceDynSize(instrSession, 1, "FPE", &data, &dataSize));

	dataSize /= 3;

	if (returnedValues)
		*returnedValues = dataSize; // Three result arrays

	for (i = 0; i < dataSize; i++)
	{
		frequency[i] = data[i * 3];
		level[i] = data[i * 3 + 1];
		deltaLimit[i] = data[i * 3 + 2];
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn Delta Marker Group Functions ===================================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Delta Marker
 * Purpose:     This function enables the active marker on the specified trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarker(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 deltaMarker,
                                                ViBoolean state,
                                                ViInt32 trace)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, max_marker),
		3, "Delta Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,DM%ld", window, deltaMarker);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_STATE, state),
		4, "State");

	if (state == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_TRACE, trace),
			5, "Trace");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Delta Marker Position
 * Purpose:     This function configures the delta marker position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarkerPosition(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 deltaMarker,
                                                        ViInt32 mode,
                                                        ViReal64 position)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, max_marker),
		3, "Delta Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,DM%ld", window, deltaMarker);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_MODE, mode),
		4, "Mode");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_POSITION, position),
		5, "Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Delta Marker Position
 * Purpose:     This function returns the horizontal position and the marker
 *              amplitude level of the selected delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryDeltaMarker(ViSession instrSession,
                                            ViInt32 window,
                                            ViInt32 deltaMarker,
                                            ViInt32 mode,
                                            ViReal64* position,
                                            ViReal64* amplitude)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, max_marker),
		3, "Delta Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,DM%ld", window, deltaMarker);
	switch (mode)
	{
	case RSSPECAN_VAL_ABS:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_POSITION, position),
			5, "Position");
		break;
	case RSSPECAN_VAL_REL:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_REL_POSITION, position),
			5, "Position");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 4, "Mode");
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_AMPLITUDE, amplitude),
		6, "Amplitude");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Disable All Delta Markers
 * Purpose:     This function turns off all the delta markers in selected
 *              measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisableAllDeltaMarkers(ViSession instrSession,
                                                  ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_AOFF, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Delta Marker Search
 * Purpose:     This function specifies the type of delta marker search and
 *              performs the search.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeltaMarkerSearch(ViSession instrSession,
                                             ViInt32 window,
                                             ViInt32 deltaMarker,
                                             ViInt32 markerSearch)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, max_marker),
		3, "Delta Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,DM%ld", window, deltaMarker);
	switch (markerSearch)
	{
	case RSSPECAN_VAL_MARKER_SEARCH_HIGHEST:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT, NULL));
		break;

	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_LEFT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_LEFT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_RIGHT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_RIGHT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_MINIMUM:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_MIN, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_LEFT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_LEFT, NULL));
		break;
	case RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_RIGHT:
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_RIGHT, NULL));
		break;

	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, markerSearch), 4, "Parameter to set");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Configure a band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR displayMode/Activates the band power marker
/// HIPAR bandPowerEnabled/efines whether the result is displayed as a power or density.
/// HIPAR span/Defines the span (band) around the marker for which the power is measured.

ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarkerBandPower(ViSession instrSession,
                                                         ViInt32 displayMode,
                                                         ViBoolean bandPowerEnabled,
                                                         ViReal64 span)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_STATE, bandPowerEnabled),
		2, "Band Power Enabled");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_MODE, displayMode),
		3, "Display Mode");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_SPAN, span),
		4, "Span");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function queries the value of the band power marker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR result/Queries the value of the band power marker.

ViStatus _VI_FUNC rsspecan_QueryDeltaMarkerBandPower(ViSession instrSession,
                                                     ViReal64* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_QUERY_DELTA_MARKER_BAND_POWER_RESULT, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Reference Fixed
 * Purpose:     This function configures marker reference fixed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceFixed(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViBoolean fixedReference)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MARKER_REF_STATE, fixedReference),
		3, "Fixed Reference");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Reference Fixed Point
 * Purpose:     This function configures marker reference fixed position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceFixedPoint(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViReal64 refPointLevel,
                                                        ViReal64 refPointLevelOffset,
                                                        ViReal64 refPointFrequencyTime)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_REF_LEVEL_OFFSET, refPointLevelOffset),
		4, "Reference Point Level Offset");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_REF_LEVEL, refPointLevel),
		3, "Reference Point Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MARKER_REF_FREQ, refPointFrequencyTime),
		5, "Reference Point Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Reference Fixed Peak Search
 * Purpose:     This function sets the reference point level for all delta
 *              markers in the selected measurement window for a measurement
 *              with fixed reference point (CALC:DELT:FUNC:FIX:STAT ON) to
 *              the peak of the selected trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReferenceFixedPeakSearch(ViSession instrSession,
                                                    ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_REF_PEAK, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Marker Phase Noise Measurement
 * Purpose:     This function configures phase noise marker measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPhaseNoiseMeasurement(ViSession instrSession,
                                                                ViInt32 window,
                                                                ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PNO_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Phase Noise Peak Search
 * Purpose:     This function sets the reference point level for all delta
 *              markers in the selected measurement window for a measurement
 *              with fixed reference point to the peak of the selected trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PhaseNoisePeakSearch(ViSession instrSession,
                                                ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MARKER_REF_PEAK, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_PhaseNoisePeakSearchAuto(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PNO_AUTO, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Phase Noise Result
 * Purpose:     This function queries the result of the phase-noise measurement
 *              in the selected measurement window. The measurement will be
 *              switched on, if necessary.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryPhaseNoiseResult(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViReal64* phaseNoiseResult)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_REFERENCE_MARKER_PNO_RESULT, phaseNoiseResult),
		3, "Phase Noise Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn System Group Functions =========================================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure Reference Oscillator
 * Purpose:     This function configures the reference oscillator.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureReferenceOscillator(ViSession instrSession,
                                                        ViInt32 source,
                                                        ViReal64 externalFrequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ROSC_SOURCE, source),
		2, "Source");
	if ((source == RSSPECAN_VAL_SOUR_EXT) && (!RsCore_IsInstrumentModel(instrSession, "FSP|FSL")))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ROSC_FREQUENCY, externalFrequency),
			3, "External Frequency");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query Selected Reference Oscillator Source
 * Purpose:     This function returns the current selection of the reference
 *              signal (internal / external) if the external reference source
 *              EXT [INT] is used. The function is only available,
 *              if SENS:ROSC:SOUR EAUT is selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySelectedReferenceOscillatorSource(ViSession instrSession,
                                                                  ViInt32* source)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ROSC_SOURCE_EAUTO, source),
		2, "Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureReferenceOscillatorExternalPLLBandwidth(ViSession instrSession,
                                                                            ViInt32 bandwidth)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ROSC_PLL_BWID, bandwidth),
		2, "Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Speaker Volume
 * Purpose:     This function sets the volume of the built-in loudspeaker
 *              for demodulated signals.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpeakerVolume(ViSession instrSession,
                                         ViReal64 speakerVolume)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64 (instrSession, "",
			RSSPECAN_ATTR_SYST_SPEAKER, speakerVolume / 100.0),
		2, "Speaker Volume");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    System Version
 * Purpose:     This function queries the number of the SCPI version, which
 *              is relevant for the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SystemVersion(ViSession instrSession,
                                         ViInt32 length,
                                         ViChar systemVersion[])

{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_SYST_VERSION,
			length, systemVersion),
		2, "System Version");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ApplicationsVersions(ViSession instrSession,
                                                ViInt32 bufferSize,
                                                ViChar results[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_APPLICATIONS_VERSIONS,
			bufferSize, results),
		3, "Results");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViBoolean applicationSetupRecovery
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_APPL_SETUP_REC, applicationSetupRecovery),
		2, "Application Setup Recovery");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure IF Shift
/* Purpose:  This function configures the shifting of the 1st IF.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureIFShift(
	ViSession instrSession,
	ViInt32 ifShift,
	ViReal64 ifShiftFrequency
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_IF_SHIFT, ifShift),
		2, "If Shift");
	if (ifShift == RSSPECAN_VAL_IF_SHIFT_AUTO)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_IF_SHIFT_FREQ, ifShiftFrequency),
			3, "If Shift Frequency");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function Turns logging of remote commands on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR state/Turns logging of remote commands on and off.
ViStatus _VI_FUNC rsspecan_ConfigureRemoteLoggingState(ViSession instrSession,
                                                       ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_REMOTE_LOGGING_STATE, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: System Shutdown
/* Purpose:  This function shuts down the instrument.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SystemShutdown(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SYSTEM_SHUTDOWN, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function queries the current frontend temperature of the R&S FSW.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR frontendTemperature/Temperature in degrees Celsius.
ViStatus _VI_FUNC rsspecan_QueryFrontendTemperature(
	ViSession instrSession,
	ViReal64* frontendTemperature
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_FRONTEND_TEMPERATURE, frontendTemperature),
		2, "Frontend Temperature");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTriggerSource(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 triggerSource)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if ((triggerSource > RSSPECAN_VAL_TRG_IQP) && !rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_SOURCE, triggerSource),
		2, "Trigger Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure TV or RF Power Trigger Source
 * Purpose:     This function specifies the trigger source that causes the
 *              spectrum analyzer to leave the Wait-for-Trigger state.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTVRFPTriggerSource(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 triggerSource)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_SOURCE_TVRF, triggerSource),
		2, "Trigger Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Trigger
 * Purpose:     This function specifies the delay and polarity for triggering.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTrigger(ViSession instrSession,
                                            ViInt32 window,
                                            ViReal64 triggerDelay,
                                            ViInt32 triggerPolarity)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_DELAY, triggerDelay),
		2, "Trigger Delay");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_SLOPE, triggerPolarity),
		3, "Trigger Polarity");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function specifies the dropout time for triggering.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR dropoutTime/Specifies the dropout time for triggering.
ViStatus _VI_FUNC rsspecan_ConfigureTriggerDropoutTime(ViSession instrSession,
                                                       ViReal64 dropoutTime)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_TRIGGER_DROPOUT_TIME, dropoutTime),
		2, "Dropout Time");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Trigger
 * Purpose:     This function defines the external trigger parameters.
 *              This is applicable when the trigger source is set to external.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalTrigger(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 externalTriggerLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL, externalTriggerLevel),
		2, "External Trigger Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines the external trigger level.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Document parameter here.
/// HIPAR externalTriggerLevel/Defines the external trigger level.
ViStatus _VI_FUNC rsspecan_ConfigureExternalTriggerPort(ViSession instrSession,
                                                        ViInt32 port,
                                                        ViReal64 externalTriggerLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, port, 1, 3),
		2, "Port");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Port%ld", port);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL_PORT, externalTriggerLevel),
		2, "External Trigger Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Delay Compensation
 * Purpose:     This function defines the external trigger delay compensation.
 *              This is applicable when the trigger source is set to external.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDelayCompensation(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViBoolean delayCompensation)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_TRIGGER_DELAY_COMPENSATION, delayCompensation),
		3, "Delay Compensation");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Video Trigger
 * Purpose:     This function specifies the video level and slope for
 *              triggering.  This is applicable when the trigger source is set
 *              to video.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureVideoTrigger(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViReal64 videoTriggerLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VIDEO_TRIGGER_LEVEL, videoTriggerLevel),
		2, "Video Trigger Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure IF Power Trigger
 * Purpose:     This function sets the level of the IF power trigger source.
 *              This is applicable when the trigger source is set to IF Power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIFPowerTrigger(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViReal64 triggerLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_IFP_LEVEL, triggerLevel),
		2, "Trigger Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure IF Power Trigger Parameters
 * Purpose:     This function sets the IF power trigger parameters. This is
 *              applicable when the trigger source is set to IF Power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureIFPowerTriggerParameters(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViReal64 triggerLevel,
                                                             ViReal64 holdoff,
                                                             ViReal64 hysteresis)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_IFP_LEVEL, triggerLevel),
		2, "Trigger Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_IFP_OFFSET, holdoff),
		3, "Holdoff");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_IFP_HYSTERESIS, hysteresis),
		4, "Hysteresis");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function sets the level of the IQ power trigger source.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR triggerLevel/Sets the level of the IQ power trigger.
ViStatus _VI_FUNC rsspecan_ConfigureIQPowerTrigger(ViSession instrSession,
                                                   ViInt32 window, ViReal64 triggerLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_IQP_LEVEL, triggerLevel),
		2, "Trigger Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureRFPowerTrigger(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViReal64 triggerLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_RFP_LEVEL, triggerLevel),
		3, "Trigger Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines the holding time before the next trigger event.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR holdoff/This control sets the holding time before the next trigger event.
ViStatus _VI_FUNC rsspecan_ConfigureRFPowerTriggerHoldoff(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViReal64 holdoff)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_RFP_HOLDOFF, holdoff),
		3, "Holdoff");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Time Trigger
 * Purpose:     This function configures the time trigger. This is applicable
 *              when the trigger source is set to time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTimeTrigger(ViSession instrSession,
                                                ViInt32 window,
                                                ViReal64 timeInterval)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_TIME_INTERVAL, timeInterval),
		2, "Time Interval");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure TV Trigger
 * Purpose:     This function configures the TV Trigger parameters. This is
 *              applicable when the trigger source is set to TV.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTVTrigger(ViSession instrSession,
                                              ViInt32 window,
                                              ViInt32 lineSystem,
                                              ViInt32 synchronization,
                                              ViInt32 horizontalSyncLine,
                                              ViInt32 polarity,
                                              ViBoolean TVFreeRunState)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_TV_LINE_SYSTEM, lineSystem),
		3, "Line System");
	switch (synchronization)
	{
	case RSSPECAN_VAL_TRIG_TV_VFIEL_ALL:
	case RSSPECAN_VAL_TRIG_TV_VFIEL_ODD:
	case RSSPECAN_VAL_TRIG_TV_VFIEL_EVEN:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_TV_VERTICAL_SIGNAL, synchronization),
			4, "Synchronization");
		break;
	case RSSPECAN_VAL_TRIG_TV_HORIZONTAL:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_TV_HORIZONTAL_SIGNAL, horizontalSyncLine),
			5, "Horizontal Sync Line");

		break;
	}
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_TV_VIDEO_POLARITY, polarity),
		6, "Polarity");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_TV_VID_CONT, TVFreeRunState),
		7, "TV Free Run State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Query Trigger Position In Sample
 * Purpose:     This function determines the time offset of the trigger in the
 *              sample (trigger position in sample = TPIS). This value can only
 *              be determined in triggered measurements using external or
 *              IFPower triggers, otherwise the value is 0.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTriggerPositionInSample(ViSession instrSession,
                                                        ViInt32* triggerPositionInSample)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TRIGGER_POSITION_IN_SAMPLE, triggerPositionInSample),
		2, "Trigger Position In Sample");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures the trigger direction as input.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Selects the trigger port to which the output is sent.
ViStatus _VI_FUNC rsspecan_ConfigureTriggerInput(ViSession instrSession,
                                                 ViInt32 port)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, port, 2, 3),
		2, "Port");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Port%ld", port);
	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_DIRECTION, RSSPECAN_VAL_TRIG_DIRECTION_INP));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTriggerOutput(ViSession instrSession,
                                                  ViInt32 port,
                                                  ViInt32 outputType,
                                                  ViInt32 level,
                                                  ViReal64 length)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, port, 2, 3),
		2, "Port");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Port%ld", port);
	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_DIRECTION, RSSPECAN_VAL_TRIG_DIRECTION_OUTP));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_OUTPUT_TYPE, outputType),
		3, "Output Type");

	if (outputType == RSSPECAN_VAL_TRIG_OTYPE_UDEFINED)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_LEVEL, level),
			4, "Level");
	}

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_PULSE_LENGTH, length),
		5, "Length");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function generates a pulse at the trigger output.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR port/Selects the trigger port to which the output is sent.
ViStatus _VI_FUNC rsspecan_GenerateTriggerPulse(ViSession instrSession,
                                                ViInt32 port)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, port, 2, 3),
		2, "Port");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Port%ld", port);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_TRIGGER_PULSE_GENERATE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Spectrogram Window
 * Purpose:  This function selects the current window for the realtime
 *           spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FrequencyMaskWindow(ViSession instrSession,
                                               ViInt32 window)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, window));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Trigger Source
 * Purpose:  This function selects the frequency mask trigger.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskTriggerSource(ViSession instrSession)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL,
		RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_SOURCE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask
 * Purpose:  This function sets the frequency mask triggering name with
             comments.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMask(ViSession instrSession,
                                                  ViString name,
                                                  ViString comment)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_NAME, name),
		2, "Name");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_COMMENT, comment),
		3, "Comment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function sets the directory the instrument stores frequency masks in.
/// HIRET Returns the status code of this operation.
/// HIRET You may use multiple lines for documentation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the
/// HIPAR instrSession/Initialize function to select the desired instrument driver session.
/// HIPAR directory/This control sets the directory to store the frequency mask file in.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskDirectory(ViSession instrSession,
                                                           ViString directory)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_DIRECTORY, directory),
		2, "Directory");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Delete Frequency Mask
 * Purpose:  This function deletes a frequency mask trigger mask.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteFrequencyMask(ViSession instrSession,
                                               ViString name)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_DELETE, name),
		2, "Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Condition
 * Purpose:  This function sets the condition that activates the frequency
             mask.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskCondition(ViSession instrSession,
                                                           ViInt32 condition)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGG_CONDITION, condition),
		2, "Condition");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Shape
 * Purpose:  This function configures the frequency mask shape.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskShape(ViSession instrSession,
                                                       ViInt32 arraySize,
                                                       ViInt32 mode,
                                                       ViReal64 lower[],
                                                       ViReal64 upper[])
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 win = 1;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_MODE, mode),
		2, "Mode");

	checkErr(RsCore_WriteAsciiViReal64Array(instrSession, "CALC:MASK:LOW ", lower, arraySize));
	checkErr(rsspecan_CheckStatus (instrSession));
	checkErr(RsCore_WriteAsciiViReal64Array(instrSession, "CALC:MASK:UPP ", upper, arraySize));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function automatically defines the shape of the frequency
/// HIFN mask trigger mask according to the spectrum that is currently
/// HIFN measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskAuto(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 win = 1;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_UPP_AUTO, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Frequency Mask Shift
 * Purpose:  This function sets the frequency mask shift.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskShift(ViSession instrSession,
                                                       ViInt32 type,
                                                       ViReal64 frequency,
                                                       ViReal64 amplitude)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	switch (type)
	{
	case RSSPECAN_VAL_LOWER:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_LOW_X, frequency),
			3, "Frequency");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_LOW_Y, amplitude),
			4, "Amplitude");
		break;
	case RSSPECAN_VAL_UPPER:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_UPP_X, frequency),
			3, "Frequency");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_UPP_Y, amplitude),
			4, "Amplitude");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 2, "Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function turns the lower or upper frequency mask on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR type/This control selects the upper and the lower mask.
/// HIPAR frequencyMask/This control turns the lower or upper frequency mask on and off.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskState(
	ViSession instrSession,
	ViInt32 type,
	ViBoolean frequencyMask
)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	switch (type)
	{
	case RSSPECAN_VAL_LOWER:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_LOWER_STATE, frequencyMask),
			3, "Frequency Mask");
		break;
	case RSSPECAN_VAL_UPPER:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_UPPER_STATE, frequencyMask),
			3, "Frequency Mask");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 2, "Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function configures the frequency mask span.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR span/This control sets the frequency mask span.
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskSpan(
	ViSession instrSession,
	ViReal64 span
)
{
	ViInt32 win = 1;
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW, &win));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FREQUENCY_MASK_SPAN, span),
		2, "Span");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn Gate Group Functions ===========================================*
 *===========================================================================*/
/*****************************************************************************
 * Function:    Configure External Gate
 * Purpose:     This function configures the external gate parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGate(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViBoolean gating,
                                                 ViInt32 gateSource,
                                                 ViInt32 mode, ViInt32 polarity,
                                                 ViReal64 delay,
                                                 ViReal64 length)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_src = RSSPECAN_VAL_EGAT_SOUR_IFP;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		max_src = RSSPECAN_VAL_EGAT_SOUR_VID;
	else if (rsspecan_IsFSV(instrSession))
		max_src = RSSPECAN_VAL_EGAT_SOUR_PSE;
	else if (RsCore_IsInstrumentModel(instrSession, "FSVR"))
		max_src = RSSPECAN_VAL_EGAT_SOUR_RFP;
	else if (RsCore_IsInstrumentModel(instrSession, "FSW"))
		max_src = RSSPECAN_VAL_EGAT_SOUR_IQP;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, gateSource, RSSPECAN_VAL_EGAT_SOUR_EXT, max_src),
		4, "Gate Source");

	if (gating == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_SIGNAL_SOURCE, gateSource),
			4, "Gate Source");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_TRIGGER_TYPE, mode),
			5, "Mode");
		if (gateSource == RSSPECAN_VAL_EGAT_SOUR_EXT)
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_POLARITY, polarity),
				6, "Polarity");
		}
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_HOLD, delay),
			7, "Delay");

		if (mode == RSSPECAN_VAL_EGAT_TRIG_EDGE)
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_LENGTH, length),
			8, "Length");
	}
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE, gating),
		3, "Gating");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Configure External Gate Trace
 * Purpose:     This function configures the external gate trace parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGateTrace(ViSession instrSession,
                                                      ViInt32 trace,
                                                      ViInt32 range,
                                                      ViBoolean traceState,
                                                      ViString comment,
                                                      ViReal64 traceStart,
                                                      ViReal64 traceStop,
                                                      ViReal64 periodLength)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 6),
		2, "Trace");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 3),
		3, "Range");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "TR%ld,ER%ld", trace, range);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_STATE, traceState),
		4, "Trace State");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_COMMENT, comment),
		5, "Comment");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_START, traceStart),
		6, "Trace Start");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_STOP, traceStop),
		7, "Trace Stop");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_PERIOD_LENGTH, periodLength),
		8, "Period Length");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn External Mixing Extension Group Functions =====================*
 *===========================================================================*/

/*****************************************************************************
 * Function:    Configure External Mixer Enabled
 * Purpose:     This function turns external mixing on/off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerEnabled(ViSession instrSession,
                                                         ViBoolean externalMixingEnabled)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_ENABLED, externalMixingEnabled),
		2, "External Mixer Enabled");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Number of Ports
 * Purpose:     This function specifies a 2-port or a 3-port external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerNumberOfPorts(ViSession instrSession,
                                                               ViInt32 numberofPorts)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_NUMBER_OF_PORTS, numberofPorts),
		2, "Number Of Ports");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer
 * Purpose:     This function specifies the mixer harmonic and average
 *              conversion loss.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixer(ViSession instrSession,
                                                  ViInt32 band,
                                                  ViInt32 harmonic,
                                                  ViInt32 harmonicType)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC_BAND, band),
		2, "Band");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC_TYPE, harmonicType),
		4, "Harmonic Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC, harmonic),
		3, "Harmonic");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Query External Mixer RF Start Stop
 * Purpose:     This function queries the frequency at which the external mixer
 *              band starts and stops.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryExternalMixerRFStartStop(ViSession instrSession,
                                                         ViReal64* RFStart,
                                                         ViReal64* RFStop)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_GET_EXTERNAL_MIXER_RF_FREQUENCY_START, RFStart),
		2, "RF Start");

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_GET_EXTERNAL_MIXER_RF_FREQUENCY_STOP, RFStop),
		3, "RF Stop");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Handover Frequency
 * Purpose:     This function defines the frequency at which the mixer switches
 *              from one range to the next (if two different ranges are
 *              selected). The handover frequency for each band can be selected
 *              freely within the overlapping frequency range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerHandoverFrequency(ViSession instrSession,
                                                                   ViReal64 handoverFrequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_HANDOVER_FREQUENCY, handoverFrequency),
		2, "Handover Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer RF Overrange
 * Purpose:     This function specifies whether the frequencies beyond the
 *              defined band limits are to be used.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerRFOverrange(ViSession instrSession,
                                                             ViBoolean RFOverrange)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_RF_OVERRANGE, RFOverrange),
		2, "RF Overrange");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Preset Band
 * Purpose:     This function restores the preset frequency ranges for the
 *              standard waveguide bands.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerPresetBand(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_PRESET_BAND, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Configure External Mixer High Harmonic
 * Purpose:     This function specifies a second (high) harmonic.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerHighHarmonic(ViSession instrSession,
                                                              ViBoolean highHarmonic,
                                                              ViInt32 harmonicOrder)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_HIGH_HARMONIC_STATE, highHarmonic),
		2, "High Harmonic");

	if (highHarmonic == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_HIGH_HARMONIC, harmonicOrder),
			3, "Harmonic Order");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Signal ID Mode
 * Purpose:     This function sets the Signal ID or Auto ID mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerSignalIDMode(ViSession instrSession,
                                                              ViInt32 mode,
                                                              ViReal64 autoIDThreshold)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_SIGNAL_ID_MODE, mode),
		2, "Mode");
	if (mode == RSSPECAN_VAL_STATE_AUTO)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_THRESHOLD, autoIDThreshold),
			3, "Auto ID Threshold");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Bias
 * Purpose:     This function configures the bias current for the low and high
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerBias(ViSession instrSession,
                                                      ViInt32 range, ViReal64 bias)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (range == 1)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_BIAS_LOW, bias),
			2, "Bias");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_BIAS_HIGH, bias),
			2, "Bias");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Conversion Loss
 * Purpose:     This function specifies the conversion loss for selected band.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerConversionLoss(ViSession instrSession,
                                                                ViInt32 range,
                                                                ViInt32 conversionLossMode,
                                                                ViReal64 conversionLoss,
                                                                ViString conversionLossTable)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 2),
		2, "Range");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, conversionLossMode, RSSPECAN_VAL_MIXER_LOSS_AVER, RSSPECAN_VAL_MIXER_LOSS_TABL),
		3, "Conversion Loss Mode");

	if (range == 1)
	{
		if (conversionLossMode == RSSPECAN_VAL_MIXER_LOSS_AVER)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_LOW, conversionLoss),
				4, "Conversion Loss");
		}
		else
		{
			viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_TABLE_LOW, conversionLossTable),
				5, "Conversion Loss Table");
		}
	}
	else
	{
		if (conversionLossMode == RSSPECAN_VAL_MIXER_LOSS_AVER)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_HIGH, conversionLoss),
				4, "Conversion Loss");
		}
		else
		{
			viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_TABLE_HIGH, conversionLossTable),
				5, "Conversion Loss Table");
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure External Mixer Low Power
 * Purpose:     This function selects a conversion loss table and sets it for
 *              the active band.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerLowPower(ViSession instrSession,
                                                          ViReal64 lowPower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_EXTERNAL_MIXER_LO_POWER, lowPower),
		2, "Low POwer");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Create External Mixer Conversion Loss Table
 * Purpose:     This function creates or overwrites conversion loss table of
 *              external mixer.
 *****************************************************************************/
#pragma warning( push )
#pragma warning( disable : 4100)
ViStatus _VI_FUNC rsspecan_CreateExternalMixerConversionLossTable(ViSession instrSession,
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
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuf;
	ViInt32 i;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	// sprintf (buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_NAME, tableName),
		3, "Table Name");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_MIXER, mixerType),
		4, "Mixer Type");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CVL_BAND, band),
		5, "Band");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_MIXER_SNUM, serialNumber),
		6, "Serial Number");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CVL_HARM, harmonic),
		7, "Harmonic");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CVL_PORT_MIXER, ports),
		8, "Ports");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CVL_BIAS, bias),
		9, "Bias");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_COMMENT, comment),
		10, "Comment");

	pbuf = buffer;
	if (numberOfValues > 0)
	{
		pbuf += sprintf(buffer, "CORR:CVL:DATA ");

		for (i = 0; i < numberOfValues; i++)
			pbuf += sprintf(pbuf, "%.12f,%.12f,", frequencyValues[i], conversionLossValues[i]);

		*pbuf = '\0';
		*--pbuf = '\n';

		checkErr(RsCore_Write(instrSession, buffer));
	}
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    External Mixer Conversion Loss Table Select
 * Purpose:     This function selects a conversion loss table of external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableSelect(ViSession instrSession,
                                                                  ViInt32 window,
                                                                  ViString tableName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	//sprintf (buffer, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_NAME, tableName),
		2, "Table Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    External Mixer Conversion Loss Table Delete
 * Purpose:     This function deletes a conversion loss table of external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableDelete(ViSession instrSession,
                                                                  ViInt32 window,
                                                                  ViString tableName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_NAME, tableName),
		2, "Table Name");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CVL_CLEAR, tableName),
		2, "Table Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
#pragma warning( pop )

/*****************************************************************************
 * Function:    External Mixer Conversion Loss Table Delete
 * Purpose:     This function deletes a conversion loss table of external mixer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableCatalog(ViSession instrSession,
                                                                   ViInt32* numberofCVL,
                                                                   ViInt32 bufferSize,
                                                                   ViChar CVLNamesList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K90|B21"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, CVLNamesList), 4, "CVL Names List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, ":CORR:CVL:CAT?", &buf));
	RsCore_TrimString(buf, RS_VAL_TRIM_WHITESPACES_AND_SINGLE_QUOTES);
	checkErr(rsspecan_ParseLimitLineCatalog (buf, bufferSize, CVLNamesList, numberofCVL));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure RF Input Coupling
 * Purpose:     This function switches the input coupling of the RF input
 *              between AC and DC.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureRFInputCoupling(ViSession instrSession,
                                                    ViInt32 RFInputCoupling)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AMPL_RF_COUPLING, RFInputCoupling),
		2, "RF Input Coupling");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Electronic Attenuator
 * Purpose:     This function switches the input coupling of the RF input
 *              between AC and DC.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureElectronicAttenuator(ViSession instrSession,
                                                         ViInt32 input,
                                                         ViInt32 mode,
                                                         ViInt32 manualValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, input, 1, 2),
		2, "Input");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", input);
	switch (mode)
	{
	case RSSPECAN_VAL_EATT_OFF:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_EATT_STATE, VI_FALSE),
			3, "Mode");
		break;
	case RSSPECAN_VAL_EATT_AUTO:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_EATT_STATE, VI_TRUE),
			3, "Mode");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_EATT_AUTO, VI_TRUE),
			3, "Mode");
		break;
	case RSSPECAN_VAL_EATT_MAN:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_EATT_STATE, VI_TRUE),
			3, "Mode");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_EATT_AUTO, VI_FALSE),
			3, "Mode");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AMPL_EATT_VALUE, manualValue),
			4, "Manual Value");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Input Mixer Level
 * Purpose:     This function configures input mixer of the analyzer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureInputMixerLevel(ViSession instrSession,
                                                    ViInt32 input,
                                                    ViBoolean automaticSetup,
                                                    ViReal64 power)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, input, 1, 2),
		2, "Input");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", input);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_MIXER_AUTO, automaticSetup),
		3, "Automatic Setup");
	if (automaticSetup == VI_FALSE)
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_AMPL_MIXER_LEVEL, power),
		4, "Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Configure Preamplifier State
 * Purpose:     This function switches on the preamplifier for the instrument.
 *              The switchable gain is fixed to 20 dB.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePreamplifierState(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_PREAMPLIFIER, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function configures level of pre-amplifier for selected sweep range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR preamplifierLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigurePreamplifierLevel(ViSession instrSession,
                                                      ViInt32 preamplifierLevel)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_AMPL_PREAMPLIFIER_LEVEL, preamplifierLevel),
		3, "Preamplifier Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Create Limit Line
 * Purpose:  This function creates new limit or selects existing one and
 *           configures basic parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CreateLimitLine(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 limit,
	ViString name,
	ViInt32 domain,
	ViString comment,
	ViInt32 assigntoTrace,
	ViBoolean deleteExistingLine
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViChar error_message[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, strlen(name), 0, 8), 4, "Name (string length)");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, strlen(comment), 0, 40), 6, "Comment (string length)");
	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, deleteExistingLine), 8, "Delete Existing Line");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld", limit);

	if (deleteExistingLine == VI_TRUE)
	{
		error = rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_LIMIT_DELETE, "");

		do
		{
			(void)rsspecan_error_query(instrSession, &error, error_message);
		}
		while (error != 0);
	}
	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_LIMIT_NAME, name),
		4, "Name");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_CONTROL_DOMAIN, domain),
		5, "Domain");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_LIMIT_COMMENT, comment),
		6, "Comment");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, limit);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_ASSIGN_TRACE, assigntoTrace),
		7, "Assign to Trace");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Limit Line
 * Purpose:  This function configures specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureLimitLine(
	ViSession instrSession,
	ViInt32 limit,
	ViInt32 type,
	ViInt32 units,
	ViInt32 xAxisInterpolation,
	ViInt32 yAxisInterpolation,
	ViInt32 xAxisScaling,
	ViReal64 margin,
	ViReal64 threshold
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 scaling;
	ViInt32 tmp_units = 14;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		tmp_units = 15;

	if (rsspecan_IsFSV(instrSession))
		tmp_units = 15;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		2, "Limit");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, 0, 1),
		3, "Type");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, units, 0, tmp_units),
		4, "Units");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld", limit);

	scaling = (units == RSSPECAN_VAL_UNIT_DB) ? RSSPECAN_VAL_REL : RSSPECAN_VAL_ABS;

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_CONTROL_SPACING, xAxisInterpolation),
		5, "X Axis Interpolation");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_CONTROL_SCALING, xAxisScaling),
		7, "X Axis Scaling");

	switch (type)
	{
	case RSSPECAN_VAL_LIMIT_LOWER:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_LOWER_SPACING, yAxisInterpolation),
			6, "Y Axis Interpolation");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_LIMIT_LOWER_MARGIN, margin),
			8, "Marging");
		if (scaling == RSSPECAN_VAL_REL)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_LIMIT_LOWER_THRESHOLD, threshold),
				9, "Threshold");
		}
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_LOWER_SCALING, scaling));
		break;

	case RSSPECAN_VAL_LIMIT_UPPER:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_UPPER_SPACING, yAxisInterpolation),
			6, "Y Axis Interpolation");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_LIMIT_UPPER_MARGIN, margin),
			8, "Marging");
		if (scaling == RSSPECAN_VAL_REL)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_LIMIT_UPPER_THRESHOLD, threshold),
				9, "Threshold");
		}
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_UPPER_SCALING, scaling));
		break;
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_UNITS, units),
		4, "Units");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Define Limit Line
 * Purpose:  This function defines specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DefineLimitLine(
	ViSession instrSession,
	ViInt32 limit,
	ViInt32 type,
	ViInt32 count,
	ViReal64 xAxis[],
	ViReal64 amplitude[]
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		2, "Limit");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, 0, 1),
		3, "Type");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, count, 1, INT_MAX),
		4, "Count");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, xAxis), 5, "X Axis");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, amplitude), 6, "Amplitude");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:LIM%ld:CONT ", limit);
	checkErr(RsCore_WriteAsciiViReal64Array(instrSession, cmd, xAxis, count));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:LIM%ld:%s ", limit, limTypeArr[type]);
	checkErr(RsCore_WriteAsciiViReal64Array(instrSession, cmd, amplitude, count));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Select Limit Line
 * Purpose:  This function selects the limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectLimitLine(ViSession instrSession,
                                           ViInt32 window,
                                           ViInt32 limit,
                                           ViString name,
                                           ViBoolean limitEnabled)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, strlen(name), 0, 8), 4, "Name (string length)");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld", limit);
	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_LIMIT_NAME, name),
		4, "Name");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, limit);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_LIMIT_STATE, limitEnabled),
		5, "Limit Enabled");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Enable Limit Check
 * Purpose:  This function switches on or off the limit check and/or limit
 *           line for the selected limit line in the selected measurement
 *           window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_EnableLimitCheck(
	ViSession instrSession,
	ViInt32 window,
	ViInt32 limit,
	ViInt32 type,
	ViBoolean limitEnabled,
	ViBoolean checkEnabled
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 attribute = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit");

	switch (type)
	{
	case RSSPECAN_VAL_LIMIT_LOWER:
		attribute = RSSPECAN_LIMIT_LOWER_STATE;
		break;
	case RSSPECAN_VAL_LIMIT_UPPER:
		attribute = RSSPECAN_LIMIT_UPPER_STATE;
		break;

	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 4, "Type");
	}

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, limit);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_LIMIT_STATE, limitEnabled),
		5, "Limit Enabled");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, (ViUInt32) attribute, checkEnabled), 6, "Check Enabled");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Move Limit Line
 * Purpose:  This function moves specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MoveLimitLine(
	ViSession instrSession,
	ViInt32 limit,
	ViInt32 type,
	ViInt32 method,
	ViReal64 value
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 attributes[3][2] = {
		{RSSPECAN_LIMIT_LOWER_OFFSET,RSSPECAN_LIMIT_LOWER_SHIFT},
		{RSSPECAN_LIMIT_UPPER_OFFSET,RSSPECAN_LIMIT_UPPER_SHIFT},
		{RSSPECAN_LIMIT_CONTROL_OFFSET,RSSPECAN_LIMIT_CONTROL_SHIFT}
	};

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		2, "Limit");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, 0, 2),
		3, "Type");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, method, 0, 1),
		4, "Method");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld", limit);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, (ViUInt32)attributes[type][method], value), 5, "Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Copy Limit Line
 * Purpose:  This function copies specified limit line into another one.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CopyLimitLine(
	ViSession instrSession,
	ViInt32 limit,
	ViInt32 copyTo
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		2, "Limit");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld", limit);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_COPY, copyTo),
		3, "Copy To");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Delete Limit Line
 * Purpose:  This function removes specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteLimitLine(
	ViSession instrSession,
	ViInt32 limit
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		2, "Limit");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld", limit);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_LIMIT_DELETE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Clear Limit Line Results
 * Purpose:  This function deletes the result of the current limit check.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ClearLimitLineResults(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 limit)
{
	ViStatus error = VI_SUCCESS;
	ViChar lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit Line Number");

	sprintf(lim_line_no, "Win%ld,L%ld", window, limit);

	checkErr(rsspecan_SetAttributeViString(instrSession, lim_line_no, RSSPECAN_LIMIT_CLEAR, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Line State
 * Purpose:  This function configures state of specified display line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLineState(ViSession instrSession, ViInt32 window, ViInt32 line, ViInt32 type, ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (type)
	{
	case RSSPECAN_VAL_LINE_THR:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_THRLINE_STATE, state),
			5, "State");
		break;
	case RSSPECAN_VAL_LINE_DISPLAY:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
			3, "Line");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, line);
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DLINE_STATE, state),
			5, "State");
		break;
	case RSSPECAN_VAL_LINE_FREQ:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
			3, "Line");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, line);
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_FLINE_STATE, state),
			5, "State");
		break;
	case RSSPECAN_VAL_LINE_TIME:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
			3, "Line");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, line);
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TLINE_STATE, state),
			5, "State");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 4, "Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Line Position
 * Purpose:  This function configures position of specified display line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLinePosition(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 line,
                                                        ViInt32 type,
                                                        ViReal64 position)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (type)
	{
	case RSSPECAN_VAL_LINE_THR:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_THRLINE_POSITION, position),
			5, "Position");
		break;
	case RSSPECAN_VAL_LINE_DISPLAY:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
			3, "Line");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, line);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DLINE_POSITION, position),
			5, "Position");
		break;
	case RSSPECAN_VAL_LINE_FREQ:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
			3, "Line");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, line);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_FLINE_POSITION, position),
			5, "Position");
		break;
	case RSSPECAN_VAL_LINE_TIME:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, line, 1, 2),
			3, "Line");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, line);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_TLINE_POSITION, position),
			5, "Position");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 4, "Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Active Limit Lines
 * Purpose:  This function queries the names of all active limit lines.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetActiveLimitLines(ViSession instrSession,
                                               ViInt32 stringSize,
                                               ViChar activeLimitLines[],
                                               ViInt32* returnedStringLength)
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, activeLimitLines), 3, "Active Limit Lines");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, ":CALC:LIM:ACT?", &pbuffer));
	checkErr(rsspecan_CheckStatus(instrSession));
	RsCore_TrimString(pbuffer, RS_VAL_TRIM_WHITESPACES_AND_SINGLE_QUOTES);
	checkErr(RsCore_CopyToUserBufferAsciiData(instrSession, activeLimitLines, stringSize, pbuffer));

	if (returnedStringLength)
		*returnedStringLength = strlen(pbuffer);

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Limit Line Catalog
 * Purpose:  This function reads out the names of all limit lines
 *           stored on the harddisk.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetLimitLineCatalog(ViSession instrSession,
                                               ViInt32* number_ofLimitLines,
                                               ViInt32 bufferSize,
                                               ViChar limitLinesList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FSL"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, limitLinesList), 4, "Limit Lines List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "CALC:LIM:CAT?", &buf));
	checkErr(rsspecan_ParseLimitLineCatalog(buf, bufferSize, limitLinesList, number_ofLimitLines));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_DisplaySetActiveWindow(ViSession instrSession,
                                                  ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_SELECT_WINDOW, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Display get Active Window
 * Purpose:  This function returns the active measurement window.
 *****************************************************************************/

ViStatus _VI_FUNC rsspecan_DisplayGetActiveWindow(ViSession instrSession,
                                                  ViInt32* window)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, window), 2, "Window");
	checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_QUERY_ACTIVE_WINDOW, window));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Selected Subwindow
 * Purpose:  This function queries the currently selected subwindow.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSelectedSubwindow(ViSession instrSession,
                                                ViInt32* subwindow)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, subwindow), 2, "Subwindow");
	checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_QUERY_SELECT_SUBWINDOW, subwindow));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Focused Area
 * Purpose:  This function moves the focus area to the selected window and
 *           subwindow.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayFocusedArea(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 subwindow)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subwindow, 1, 2),
		3, "Subwindow");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,SW%ld", window, subwindow);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_DISPLAY_FOCUS_AREA, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Control
 * Purpose:  This function scontrols the apperance of display elements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayControl(ViSession instrSession,
                                                   ViBoolean frequency,
                                                   ViBoolean logo,
                                                   ViBoolean time)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_FREQ_STATE, frequency),
		2, "Frequency");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_TIME_STATE, time),
		4, "Time");

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		if (time == VI_FALSE)
		{
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_LOGO_STATE, logo),
				3, "Logo");
		}
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_LOGO_STATE, logo),
			3, "Logo");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Comment
 * Purpose:  This function controls the apperance of display title.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayComment(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViBoolean state,
                                                   ViString comment)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DISP_COMMENT_STATE, state),
		2, "State");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_DISP_COMMENT, comment),
		3, "Comment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Configure Display Comment
 * Purpose:  This function controls the apperance of display title.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayPowerSave(ViSession instrSession,
                                                     ViBoolean state,
                                                     ViInt32 holdoff)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_PWR_SAVE_STATE, state),
		2, "State");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_PWR_SAVE_HOLDOFF, holdoff),
		3, "Hold Off");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Format
 * Purpose:  This function switches the measurement result display between
 *           FULL SCREEN and SPLIT SCREEN.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayFormat(ViSession instrSession,
                                                  ViInt32 format)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_FORMAT, format),
		2, "Format");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Window Size
 * Purpose:  This function switches the measurement window for channel and
 *           adjacent-channel power measurements to full screen or half screen.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayWindowSize(ViSession instrSession,
                                                      ViInt32 windowSize)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_WINDOW_SIZE, windowSize),
		2, "Window Size");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLogRange(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViReal64 range)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_LOG_RANGE, range),
		3, "Range");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Amplitude Grid Mode
 * Purpose:  This function defines the scale type of the Y axis
 *           (absolute or relative) in the selected measurement window.
 *           When SYSTem:DISPlay is set to OFF, this command has no immediate
 *           effect on the screen.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayAmplitudeGridMode(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 yAxisGridMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, yAxisGridMode),
		3, "Range");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Reference Value
 * Purpose:  This function defines the power value assigned to the reference
 *           position in the selected measurement window. This value corresponds
 *           to the parameter REFERENCE VALUE in manual operation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayReferenceValue(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViReal64 referenceValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_REF_VALUE, referenceValue),
		3, "Reference Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Reference Position
 * Purpose:  This function defines the position of the reference value in
 *           the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayReferencePosition(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViReal64 referencePosition)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_REF_POSITION, referencePosition),
		3, "Reference Value Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Single Sweep
 * Purpose:  This function configures the behaviour of the display during a
 *           single sweep.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySingleSweep(ViSession instrSession,
                                                       ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_SINGLE_SWEEP, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Transducer State
 * Purpose:  This function switches on the display of the active transducer
 *           factor or set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayTransducerState(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TFAC_DISPLAY, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Update
 * Purpose:  This function switches on or off the update of all display
 *           elements during remote control.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayUpdate(ViSession instrSession,
                                                  ViBoolean displayInRemote)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_UPDATE, displayInRemote),
		2, "Display In Remote");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySoftFrontpanel(ViSession instrSession,
                                                          ViBoolean softFrontpanel)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_FP_KEYS, softFrontpanel),
		2, "Soft Front Panel");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Autoscale Once
 * Purpose:  This function adapts the diagram limits to the current measurement
 *           results. The value range depends on the selected display.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayAutoscaleOnce(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_DISP_TRACE_Y_AUTO, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Display Y-Axis Scaling
 * Purpose:     This function defines the scaling of the y-axis in
 *              the currently selected unit.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayYAxisScaling(ViSession instrSession,
                                                        ViInt32 window, ViReal64 scaling)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_DISP_Y_AXIS_SCALING, scaling),
		3, "Scaling");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Settings
 * Purpose:  This function configures the display settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySettings(ViSession instrSession,
                                                    ViBoolean statusBarState,
                                                    ViBoolean softkeyBarState,
                                                    ViBoolean toolBarState,
                                                    ViInt32 touchscreenState)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_STATUS_BAR, statusBarState),
		2, "Status Bar State");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_SOFTKEY_BAR, softkeyBarState),
		3, "Softkey Bar State");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DISP_TOOL_BAR, toolBarState),
		4, "Tool Bar State");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_TOUCHSCREEN_STATE, touchscreenState),
		5, "Touchscreen State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Display Theme Catalog
 * Purpose:  This function displays the selection list of available themes for
 *           the screen display.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetDisplayThemeCatalog(ViSession instrSession,
                                                  ViInt32* number_ofThemes,
                                                  ViInt32 bufferSize,
                                                  ViChar themeList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, themeList), 4, "Theme List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "DISP:THEM:CAT?", &buf));
	checkErr(RsCore_ParseCatalog(buf, bufferSize, themeList, number_ofThemes));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Theme Select
 * Purpose:  This function selects a theme for the screen display. The theme
 *           defines the colors used for keys and screen elements, for example.
 *           Use command DISPlay:THEMe:CATalog? to get a list of possible themes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayThemeSelect(ViSession instrSession,
                                                       ViString theme)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_DISP_SELECT_THEME, theme),
		2, "Theme");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Marker Table
 * Purpose:  This function toggles the display of the marker table. With
 *           automatic display, the table is displayed if 2 or more markers
 *           are active.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMarkerTable(ViSession instrSession,
                                                       ViInt32 markerTable)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_MARKER_TABLE, markerTable),
		2, "Marker Table");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Zoom
 * Purpose:  This function configures the display zooming parameters.
 *
 *           Note:
 *           (1) This function is available for R&S FSV instruments only.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayZoom(ViSession instrSession,
                                                ViInt32 window,
                                                ViBoolean zoomState, ViReal64 x1,
                                                ViReal64 y1, ViReal64 x2,
                                                ViReal64 y2)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	viCheckParm(RsCore_InvalidViReal64Range(instrSession, x1, 0.0, 100.0), 4, "x1");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, y1, 0.0, 100.0), 5, "y1");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, x2, 0.0, 100.0), 6, "x2");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, y2, 0.0, 100.0), 7, "y2");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DISPLAY_ZOOM_STATE, zoomState),
		3, "Zoom State");

	if (zoomState == VI_TRUE)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "DISP:WIND%ld:ZOOM:AREA %.12lG,%.12lG,%.12lG,%.12lG", window, x1, y1, x2, y2);
		checkErr(RsCore_Write(instrSession, cmd));

		checkErr(rsspecan_CheckStatus (instrSession));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMultipleZoom(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 zoomWindow,
                                                        ViBoolean enableZoom,
                                                        ViReal64 x1,
                                                        ViReal64 y1,
                                                        ViReal64 x2,
                                                        ViReal64 y2)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, zoomWindow, 1, 4),
		3, "Zoom Window");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, x1, 0.0, 100.0), 5, "x1");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, y1, 0.0, 100.0), 6, "y1");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, x2, 0.0, 100.0), 7, "x2");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, y2, 0.0, 100.0), 8, "y2");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,ZWin%ld", window, zoomWindow);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DISPLAY_MULTIPLE_ZOOM_STATE, enableZoom),
		4, "Enable Zoom");

	if (enableZoom == VI_TRUE)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "DISP:WIND%ld:ZOOM:MULT%ld:AREA %.12lG,%.12lG,%.12lG,%.12lG",
		         window, zoomWindow, x1, y1, x2, y2);
		checkErr(RsCore_Write(instrSession, cmd));

		checkErr(rsspecan_CheckStatus (instrSession));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function turns a measurement window on or off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR state/Turns a measurement window on or off.
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMeasurementWindowState(ViSession instrSession,
                                                                  ViInt32 window,
                                                                  ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_DISP_MEAS_WINDOW_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function queries the currently active measurement window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control returns the index number of the currently active measurement window.
ViStatus _VI_FUNC rsspecan_QueryActiveMeasurementWindow(ViSession instrSession,
                                                        ViInt32* window)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_QUERY_ACTIVE_MEASUREMENT_WINDOW, window),
		2, "Window");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Display Color Default
 * Purpose:  This function resets the screen colors of all display items to
 *           their default settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DisplayColorDefault(ViSession instrSession,
                                               ViInt32 defaultSetting)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, defaultSetting, 1, 2),
		2, "Default Settings");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DEF%ld", defaultSetting);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_DISP_COL_PRESET, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Color
 * Purpose:  This function defines the color table of the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayColor(ViSession instrSession,
                                                 ViInt32 colorMap,
                                                 ViReal64 tint,
                                                 ViReal64 saturation,
                                                 ViReal64 brightness)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorMap, 1, 26),
		2, "Color Map");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, tint, 0.0, 100.0), 3, "TINT");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, saturation, 0.0, 100.0), 4, "SATURATION");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, brightness, 0.0, 100.0), 5, "BRIGHTNESS");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "DISP:CMAP%ld:HSL %.12f,%.12f,%.12f", colorMap, tint / 100.0, saturation / 100.0, brightness / 100.0);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Display Color by Predefined
 * Purpose:  This function defines the color table of the instrument using
 *           predefined colors.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureDisplayColorByPredefined(ViSession instrSession,
                                                             ViInt32 colorMap,
                                                             ViInt32 predefinedColors)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorMap, 1, 26),
		2, "Color Map");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "CM%ld", colorMap);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_DISP_COL_PREDEFINED, predefinedColors),
		3, "Predefined Colors");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_AddWindow(ViSession instrSession, ViInt32 stringSize,
                                     ViString windowName, ViInt32 direction,
                                     ViInt32 windowType,
                                     ViChar returnedWindowName[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd [RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuffer = NULL;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, direction, RSSPECAN_VAL_LAYOUT_DIRECTION_LEFT, RSSPECAN_VAL_LAYOUT_DIRECTION_BELOW),
		4, "Direction");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, windowType, RSSPECAN_VAL_LAYOUT_TYPE_DIAGRAM, RSSPECAN_VAL_LAYOUT_TYPE_PSP),
		5, "Window Type");

	sprintf(cmd, "LAY:ADD? '%s',%s,%s", windowName, directionArr[direction], windowTypeArr[windowType]);

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &pbuffer));

	if (returnedWindowName != NULL)
	{
		RsCore_TrimString(pbuffer, RS_VAL_TRIM_WHITESPACES_AND_SINGLE_QUOTES);
		checkErr(RsCore_CopyToUserBufferAsciiData(instrSession, returnedWindowName, stringSize, pbuffer));
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (pbuffer != NULL) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function removes a window from the display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
ViStatus _VI_FUNC rsspecan_RemoveWindow(ViSession instrSession,
                                        ViString windowName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LAYOUT_WINDOW_REMOVE, windowName),
		2, "Window Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function replaces a window with another one.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
/// HIPAR windowType/Type of result display you want to add.
ViStatus _VI_FUNC rsspecan_ReplaceWindow(ViSession instrSession,
                                         ViString windowName,
                                         ViInt32 windowType)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd [100] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, windowType, RSSPECAN_VAL_LAYOUT_TYPE_DIAGRAM, RSSPECAN_VAL_LAYOUT_TYPE_PSP),
		3, "Window Type");

	sprintf(cmd, "LAY:REPL '%s',%s", windowName, windowTypeArr[windowType]);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function queries the name and index of all active windows.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR bufferSize/This control specifies the size of the buffer.
/// HIPAR results/Returns the name and index of all active windows.
ViStatus _VI_FUNC rsspecan_QueryWindowsList(ViSession instrSession,
                                            ViInt32 bufferSize,
                                            ViChar results[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_LAYOUT_WINDOW_LIST,
			bufferSize, results),
		3, "Results");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function queries the index of a particular display window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
/// HIPAR index/Index number of the window.
ViStatus _VI_FUNC rsspecan_QueryWindowIndex(ViSession instrSession,
                                            ViString windowName, ViInt32* index)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* pbuff;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "LAY:IDEN? '%s'", windowName);
	checkErr(RsCore_QueryViString(instrSession, cmd, response));
	pbuff = strtok(response, ",\n");
	*index = atol(pbuff);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 index,
	ViInt32* windowType
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", index);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LAYOUT_QUERY_WINDOW_TYPE, windowType),
		3, "Window Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function queries the name of a particular display window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR stringSize/This control sets the allocated length of Returned Window Name string.
/// HIPAR index/Index number of the window.
/// HIPAR windowName/String containing the number or name of the window the new window is inserted next to.
ViStatus _VI_FUNC rsspecan_QueryWindowName(ViSession instrSession, ViInt32 stringSize,
                                           ViInt32 index, ViChar windowName[])
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", index);
	viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_LAYOUT_QUERY_WINDOW_NAME,
			stringSize, windowName),
		3, "Window Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function changes the position of a splitter
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR firstWindowName/String containing the number or name of the first window the splitter controls.
/// HIPAR secondWindowName/String containing the number or name of the second window the splitter controls.
/// HIPAR position/New vertical or horizontal position of the splitter in percent of the screen area.
ViStatus _VI_FUNC rsspecan_ConfigureSplitterPosition(ViSession instrSession,
                                                     ViString firstWindowName,
                                                     ViString secondWindowName,
                                                     ViInt32 position)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd [100] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(cmd, "LAY:SPL '%s','%s',%ld", firstWindowName, secondWindowName, position);

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Select Transducer Factor
 * Purpose:  This function generates a transducer factor <name> using
 *           normalized trace data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectTransducerFactor(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViBoolean transducerState,
                                                  ViString transducerName)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_TFAC_SEL_NAME, transducerName),
		4, "Transducer Name");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TFAC_STATE, transducerState),
		3, "Transducer State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Select Transducer Factor
 * Purpose:  This function generates a transducer factor <name> using
 *           normalized trace data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTransducerFactor(ViSession instrSession,
                                                     ViInt32 window,
                                                     ViString name,
                                                     ViInt32 unit,
                                                     ViInt32 interpolation,
                                                     ViString comment,
                                                     ViInt32 noofTestPoints,
                                                     ViReal64 frequencyData[],
                                                     ViReal64 levelData[])

{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* pbuf;
	ViChar* p2buf = NULL;
	ViInt32 i;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_TFAC_SEL_NAME, name),
		3, "Name");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TFAC_UNIT, unit),
		4, "Unit");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_TFAC_SCALING, interpolation),
		5, "Interpolation");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_TFAC_COMMENT, comment),
		6, "Comment");

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, noofTestPoints, 0, INT_MAX),
		7, "No Of Test Points");

	viCheckAlloc (pbuf = (ViChar*) malloc((size_t)(23 + 2 * noofTestPoints * 15 + 300)));

	p2buf = pbuf;

	if (noofTestPoints > 0)
	{
		pbuf += sprintf(pbuf, "SENS%ld:CORR:TRAN:DATA ", window);
		for (i = 0; i < noofTestPoints; i++)
		{
			pbuf += sprintf(pbuf, "%le,%lf,", frequencyData[i], levelData[i]);
		}
		*--pbuf = '\0';
		checkErr(RsCore_Write(instrSession, p2buf));
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (p2buf) free(p2buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Transducer Factor Ref Level Adj
 * Purpose:  This function allows automatic reference level offset adaption
 *           to restore the original dynamic range by also shifting the reference
 *           level by the maximum value of the transducer factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTransducerFactorRefLevAdj(ViSession instrSession,
                                                              ViInt32 window,
                                                              ViBoolean refLevAdj)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_TFAC_ADJ_STATE, refLevAdj),
		3, "Ref Lev Adj");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Transducer Factor Catalog
 * Purpose:  This function reads out the names of all transducer factors
 *           stored on the harddisk.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTransducerFactorCatalog(ViSession instrSession,
                                                      ViInt32* numberofTransducerFactors,
                                                      ViInt32 bufferSize,
                                                      ViChar transducerFactorsList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL|FMU") || (rsspecan_IsFSV(instrSession)))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, transducerFactorsList), 4, "Transducer Factor List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, ":CORR:TRAN:CAT?", &buf));
	checkErr(RsCore_ParseCatalog(buf, bufferSize, transducerFactorsList, numberofTransducerFactors));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Delete Transducer Factor
 * Purpose:  This function deletes the selected transducer factor.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteTransducerFactor(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViString transducerName)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_TFAC_SEL_NAME, transducerName),
		3, "Transducer Name");
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_TFAC_DELETE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Service Version Info
 * Purpose:  This function queries the instrument firmware and option versions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceVersionInfo(ViSession instrSession,
                                              ViInt32 arraySize,
                                              ViChar versionInfo[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_QueryViString(instrSession, "DIAG:SERV:VERS?", arraySize, versionInfo));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: File Decimal Separator
 * Purpose:  This function configures the decimal separator
 *           (decimal point or comma) for floating-point numerals contained
 *           in the file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileDecimalSeparator(ViSession instrSession,
                                                ViInt32 decimalSeparator)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FILE_DEC_SEPARATOR, decimalSeparator),
		2, "Decimal Separator");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Transducer Active
 * Purpose:  This function queries the active transducer factor. Any empty
 *           string is returned, if no transducer factor is active.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetTransducerActive(ViSession instrSession,
                                               ViChar activeTransducer[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "Win0", RSSPECAN_ATTR_TFAC_ACT_NAME,
			512, activeTransducer),
		2, "Active Transducer");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_SelectTransducerSet(ViSession instrSession,
                                               ViString transducerSetName,
                                               ViBoolean transducerSetState)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TSET_SELECT, transducerSetName),
		2, "Transducer Set Name");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TSET_STATE, transducerSetState),
		3, "Transducer Set State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTransducerSet(ViSession instrSession,
                                                  ViString transducerSetName,
                                                  ViString comment,
                                                  ViInt32 unit,
                                                  ViBoolean transducerBreak,
                                                  ViInt32 range,
                                                  ViReal64 startFrequency,
                                                  ViReal64 stopFrequency,
                                                  ViString factors)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 10),
		6, "Range");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TSET_SELECT, transducerSetName),
		2, "Transducer Set Name");

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TSET_COMMENT, comment),
		3, "Comment");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TSET_UNIT, unit),
		4, "Unit");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_TSET_BREAK, transducerBreak),
		5, "Transducer Break");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS:CORR:TSET:RANG%ld %.12lG,%.12lG,%s", range,
	         startFrequency, stopFrequency, factors);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryTransducerSetCatalog(ViSession instrSession,
                                                     ViInt32* numberOfTransducerSets,
                                                     ViInt32 bufferSize,
                                                     ViChar transducerSetsList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, transducerSetsList), 4, "Transducer Set List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, ":CORR:TSET:CAT?", &buf));
	checkErr(RsCore_ParseCatalog(buf, bufferSize, transducerSetsList, numberOfTransducerSets));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function deletes a transducer set.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR transducerSetName/This control selects the transducer set name.
ViStatus _VI_FUNC rsspecan_DeleteTransducerSet(ViSession instrSession,
                                               ViString transducerSetName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TSET_SELECT, transducerSetName),
		2, "Transducer Set Name");

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_TSET_DELETE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function queries the active transducer set. Any empty
/// HIFN  string is returned, if no transducer set is active.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR activeTransducerSet/This control the active transducer set. Any empty string is
/// HIPAR activeTransducerSet/returned, if no transducer set is active.
ViStatus _VI_FUNC rsspecan_QueryActiveTransducerSet(ViSession instrSession,
                                                    ViChar activeTransducerSet[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_TSET_ACTIVE,
			512, activeTransducerSet),
		2, "Active Transducer Set");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Hardcopy Device Setup
 * Purpose:  This function selects the printer output medium and the data
 *           format of the printout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyDevice(ViSession instrSession,
                                                   ViInt32 device,
                                                   ViInt32 destination,
                                                   ViInt32 pageOrientation)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, device, 1, 2),
		2, "Device");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, destination, 0, 6),
		3, "Destination");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DE%ld", device);
	switch (destination)
	{
	case RSSPECAN_VAL_HCOPY_DEST_BMP:
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_MEM));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, RSSPECAN_VAL_HCOPY_DEVICE_LANG_BMP));
		break;
	case RSSPECAN_VAL_HCOPY_DEST_WMF:
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_MEM));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, RSSPECAN_VAL_HCOPY_DEVICE_LANG_WMF));
		break;
	case RSSPECAN_VAL_HCOPY_DEST_EMF:
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_MEM));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, RSSPECAN_VAL_HCOPY_DEVICE_LANG_EWMF));
		break;
	case RSSPECAN_VAL_HCOPY_DEST_CLP:
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_CLP));
		break;
	case RSSPECAN_VAL_HCOPY_DEST_PRN:

		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, RSSPECAN_VAL_HCOPY_DEVICE_LANG_GDI));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_PRN));
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_ORIENTATION, pageOrientation),
			4, "Page Orientation");
		break;
	case RSSPECAN_VAL_HCOPY_DEST_JPG:
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_MEM));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, RSSPECAN_VAL_HCOPY_DEVICE_LANG_JPG));
		break;
	case RSSPECAN_VAL_HCOPY_DEST_PNG:
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION, RSSPECAN_VAL_HCOPY_DEVICE_MEM));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM, RSSPECAN_VAL_HCOPY_DEVICE_LANG_PNG));
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Abort
 * Purpose:  This function aborts a running hardcopy output.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyAbort(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_HCOPY_ABORT, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Print
 * Purpose:  This function starts a hardcopy output of the selected items.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyPrint(ViSession instrSession,
                                         ViInt32 device,
                                         ViInt32 items)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, device, 1, 2),
		2, "Device");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, items, 0, 4),
		3, "Items");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DE%ld", device);
	switch (items)
	{
	case RSSPECAN_VAL_HCOP_ALL:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_HCOPY_PRINT_SCREEN, NULL));
		break;
	case RSSPECAN_VAL_HCOP_TABLE:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_HCOPY_PRINT_TAB, VI_TRUE));
		break;
	case RSSPECAN_VAL_HCOP_TRACE:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_HCOPY_PRINT_TRACE, VI_TRUE));
		break;
	}
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_HCOPY_PRINT, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_HardcopyPrintNext(ViSession instrSession,
                                             ViInt32 device)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, device, 1, 2),
		2, "Device");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DE%ld", device);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_HCOPY_PRINT_NEXT, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Set File Name
 * Purpose:  This function defines a destination file for the hadcopy output.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopySetFileName(ViSession instrSession,
                                               ViString name)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_HCOPY_FILE_NAME, name));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Get Printer List
 * Purpose:  This function queries the list of printers under Windows NT.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyGetPrinterList(ViSession instrSession,
                                                  ViInt32 bufferSize,
                                                  ViChar printerList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* p2buf;
	ViInt32 count;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 1, INT_MAX),
		2, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, printerList), 3, "Printer List");

	p2buf = printerList;
	count = 0;
	checkErr(rsspecan_GetAttributeViString (instrSession, "", RSSPECAN_ATTR_HCOPY_PRINTER_FIRST,
		RS_MAX_MESSAGE_BUF_SIZE, buffer));

	while (strlen(buffer) > 1)
	{
		count += (ViInt32)strlen(buffer);
		count++; //add one position for comma
		if (count <= bufferSize)
			p2buf += sprintf(p2buf, "%s,", buffer);
		checkErr(rsspecan_GetAttributeViString (instrSession, "", RSSPECAN_ATTR_HCOPY_PRINTER_NEXT,
			RS_MAX_MESSAGE_BUF_SIZE, buffer));
	}
	*--p2buf = '\0';

	if (count > bufferSize)
		error = count;

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Set Printer
 * Purpose:  This function selects one of the printers installed under
 *           Windows NT. Prior to use this function get list of available
 *           printers using function Hardcopy Get Printer List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopySetPrinter(ViSession instrSession,
                                              ViInt32 device,
                                              ViString printerName)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, device, 1, 2),
		2, "Device");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DE%ld", device);
	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_HCOPY_PRINTER, printerName),
		3, "Printer Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Comment
 * Purpose:  This function defines the comment text for measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyComment(ViSession instrSession,
                                           ViString comment)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "Win0", RSSPECAN_ATTR_HCOPY_COMM_SCR, comment));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Time Date
 * Purpose:  This function includes or removes the time and date in the printout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyTimeDate(ViSession instrSession,
                                            ViInt32 device,
                                            ViBoolean time_andDateStamp)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DE%ld", device);
	checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_HCOPY_TIME_DATE, time_andDateStamp));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Hardcopy Color Default
 * Purpose:  This function resets the colors for a hardcopy to the selected
 *           default settings. DEFault1(SCREEN COLORS, but background white),
 *           DEFault2 (OPTIMIZED COLOR SET) and DEFault3 (USER DEFINED). The
 *           numeric suffix in CMAP is not significant.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_HardcopyColor(ViSession instrSession,
                                         ViBoolean color,
                                         ViInt32 defaultSetting)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "DEF%ld", defaultSetting);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_HCOPY_COLOR, color),
		2, "Color");

	if (color == VI_TRUE)
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_COLOR_DEF, defaultSetting),
		3, "Default Setting");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Hardcopy Color
 * Purpose:  This function defines the color table in USER DEFINED COLORS
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyColor(ViSession instrSession,
                                                  ViInt32 colorMap,
                                                  ViReal64 tint,
                                                  ViReal64 saturation,
                                                  ViReal64 brightness)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorMap, 1, 26),
		2, "Color Map");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, tint, 0.0, 100.0), 3, "TINT");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, saturation, 0.0, 100.0), 4, "SATURATION");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, brightness, 0.0, 100.0), 5, "BRIGHTNESS");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "HCOP:CMAP%ld:HSL %.12f,%.12f,%.12f", colorMap, tint / 100.0, saturation / 100.0, brightness / 100.0);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Hardcopy Color by Predefined
 * Purpose:  This function defines the color table in USER DEFINED COLORS
 *           using predefined color values.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyColorByPredefined(ViSession instrSession,
                                                              ViInt32 colorMap,
                                                              ViInt32 predefinedColors)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, colorMap, 1, 26),
		2, "Color Map");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "CM%ld", colorMap);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_HCOPY_COLOR_PREDEFINED, predefinedColors),
		3, "Predefined Colors");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: File Decimal Separator
 * Purpose:  This function configures the decimal separator
 *           (decimal point or comma) for floating-point numerals contained
 *           in the file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileManagerOperations(ViSession instrSession,
                                                 ViInt32 operation,
                                                 ViString source,
                                                 ViString destination)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, operation, 0, 8),
		2, "Operation");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, source), 3, "Source");

	switch (operation)
	{
	case RSSPECAN_VAL_FILE_NEW:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_MAKE_DIR, source));
		break;
	case RSSPECAN_VAL_FILE_COPY:
		viCheckParm(RsCore_InvalidNullPointer(instrSession, destination), 4, "Destination");
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":MMEM:COPY '%s','%s'", source, destination);
		checkErr(RsCore_Write(instrSession, cmd));
		break;
	case RSSPECAN_VAL_FILE_RENAME:
		viCheckParm(RsCore_InvalidNullPointer(instrSession, destination), 4, "Destination");
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":MMEM:MOVE '%s','%s'", source, destination);
		checkErr(RsCore_Write(instrSession, cmd));
		break;
	case RSSPECAN_VAL_FILE_DELETE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_DELETE, source));
		break;
	case RSSPECAN_VAL_FILE_RDIR:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_DELETE_DIR, source));
		break;
	case RSSPECAN_VAL_FILE_FORMAT:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_FORMAT_DISK, "a:"));
		break;
	case RSSPECAN_VAL_FILE_CDIR:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH, source));
		break;
	case RSSPECAN_VAL_FILE_CDISC:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH_DEVICE, source));
		break;
	case RSSPECAN_VAL_FILE_DELETE_IMM:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_DELETE_IMMEDIATE, source));
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: File Directory
 * Purpose:  This function reads the indicated directory. According to DOS
             convention, wild card characters can be entered in order to
             query eg. a list of all file of a certain type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileDirectory(ViSession instrSession,
                                         ViString directory,
                                         ViInt32 bufferSize,
                                         ViChar output[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, directory), 2, "Directory");
	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEM:CAT? '%s'", directory);
	checkErr(rsspecan_QueryViString(instrSession, cmd, bufferSize, output));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: File Directory Path
 * Purpose:  This function queries the directories and files in the given path.
 *           According to DOS convention, wild card characters can be entered
 *           in order to query eg. a list of all file of a certain type.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FileDirectoryPath(ViSession instrSession,
                                             ViString directory,
                                             ViInt32 bufferSize,
                                             ViChar output[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, directory), 2, "Directory");
	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":MMEM:CAT:LONG? '%s'", directory);
	checkErr(rsspecan_QueryViString(instrSession, cmd, bufferSize, output));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read To File From Instrument
 * Purpose:  This function is used to read data from the instrument and write
 *           it to a user specified file on the host computer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadToFileFromInstrument(ViSession instrSession,
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
ViStatus _VI_FUNC rsspecan_WriteFromFileToInstrument(ViSession instrSession,
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
ViStatus _VI_FUNC rsspecan_DataSetFileOperations(ViSession instrSession,
                                                 ViInt32 operation,
                                                 ViString path)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, operation, 0, 7),
		2, "Operation");

	switch (operation)
	{
	case RSSPECAN_VAL_MEM_SAVE:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_SAVE, path),
			2, "Path");
		break;
	case RSSPECAN_VAL_MEM_RECALL:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_RECALL, path),
			2, "Path");
		break;
	case RSSPECAN_VAL_MEM_DELETE:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_DATA_CLEAR, path),
			2, "Path");
		break;
	case RSSPECAN_VAL_MEM_STARTUP:
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_STARTUP_RECALL, path),
			2, "Path");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Data Set File Save Next
 * Purpose:  This function stores the current device settings in a *.FSL.dfl
 *           file. The file name used in the last saving process is automatically
 *           counted up to the next unused name.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileSaveNext(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_SAVE_NEXT, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Data Set File Comment
 * Purpose:  This function defines a comment ( max. 60 characters) for a device
 *           setting to be stored.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileComment(ViSession instrSession,
                                              ViString comment)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_EDIT_COMM, comment),
		2, "Comment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Data Set File Clear All
 * Purpose:  This function defines a comment ( max. 60 characters) for a device
 *           setting to be stored.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileClearAll(ViSession instrSession,
                                               ViString directory)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH, directory),
		2, "Directory");
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_DATA_CLEAR_ALL, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Data Set File Select Items
 * Purpose:  This function defines a comment ( max. 60 characters) for a device
 *           setting to be stored.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DataSetFileSelectItems(ViSession instrSession,
                                                  ViInt32 itemSelector,
                                                  ViBoolean itemState)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (itemSelector)
	{
	case RSSPECAN_VAL_FILE_ITEMS_NONE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_NONE, NULL));
		break;
	case RSSPECAN_VAL_FILE_ITEMS_DEFAULT:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_DEFAULT, NULL));
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALL:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_ALL, NULL));
		break;
	case RSSPECAN_VAL_FILE_ITEMS_HWSETTINGS:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_HWSETTINGS, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALLTRACES:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_TRAC, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALLLINES:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_LINE, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_SOUR_CAL_DATA:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_SOURCE_CAL_DATA, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALLTRAN:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_TRAN, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_SPECTROGRAM:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SPEM, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_IQ_DATA:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_INCLUDE_IQ_DATA, itemState),
			3, "Item State");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, itemSelector), 2, "Item Selector");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_DataSetFileSelectItemsFromChannel(ViSession instrSession,
                                                             ViInt32 itemSelector,
                                                             ViBoolean itemState)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (itemSelector)
	{
	case RSSPECAN_VAL_FILE_ITEMS_NONE:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_NONE, NULL));
		break;
	case RSSPECAN_VAL_FILE_ITEMS_DEFAULT:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_DEFAULT, NULL));
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALL:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_ALL, NULL));
		break;
	case RSSPECAN_VAL_FILE_ITEMS_HWSETTINGS:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_HWSETTINGS, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALLTRACES:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_TRAC, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALLLINES:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_LINE, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_ALLTRAN:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_TRAN, itemState),
			3, "Item State");
		break;
	case RSSPECAN_VAL_FILE_ITEMS_SPECTROGRAM:
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SPEM, itemState),
			3, "Item State");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, itemSelector), 2, "Item Selector");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines whether the channel-specific settings that will be loaded with the
/// HIFN rsspecan_DataSetFileOperations function will replace the current channel or activate a new channel.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR mode/This control selects the load mode.
ViStatus _VI_FUNC rsspecan_DataSetLoadMode(ViSession instrSession, ViInt32 mode)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DATA_SET_LOAD_MODE, mode),
		2, "Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines whether the data from the entire instrument
/// HIFN or only from the current channel is stored.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR mode/This control selects the store mode.
ViStatus _VI_FUNC rsspecan_DataSetStoreMode(ViSession instrSession, ViInt32 mode)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DATA_SET_STORE_MODE, mode),
		2, "Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 softkeyNumber,
	ViString filename,
	ViString label
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, softkeyNumber, 1, 8),
		2, "Softkey Number");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEM:USER%ld '%s','%s'", softkeyNumber, filename, label);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_Calibration(ViSession instrSession,
                                       ViBoolean sync,
                                       ViUInt32 timeout,
                                       ViInt32* result)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, sync), 2, "Sync");
	if (sync == VI_TRUE)
	{
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, timeout, 0, 600000),
			3, "Timeout");
		checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
		checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

		viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CALIBRATION, result),
			2, "Result");
	}
	else
	{
		checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CALIBRATION, RSSPECAN_VAL_CAL_ALL));
	}

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Abort
 * Purpose:  This function aborts the acquisition of correction data and
 *           restores the last complete correction data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationAbort(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CALIBRATION_ABORT, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Result
 * Purpose:  This function reads the results of the correction data acquisition.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationResult(ViSession instrSession,
                                             ViInt32 arraySize,
                                             ViChar result[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	//viCheckParm(rsspecan_GetAttributeViString (instrSession, "",
	//        RSSPECAN_ATTR_CALIBRATION_RESULT_QUERY, arraySize, result), 2, "Result");
	checkErr(rsspecan_QueryViString(instrSession, "CAL:RES?", arraySize, result));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration State
 * Purpose:  This function determines whether the current calibration data
 *           are taken into account by the instrument (ON) or not (OFF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationState(ViSession instrSession,
                                            ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CALIBRATION_STATE, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: YIG Correction
 * Purpose:  This function switches on or off the automatic correction of
 *           the YIG filter frequency drift.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_YIGCorrection(ViSession instrSession,
                                         ViInt32 window,
                                         ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_CORR_YIG, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Calibration Probe Catalog
 * Purpose:  This function queries the names of all probe data sets stored in
 *           the FMU.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetCalibrationProbeCatalog(ViSession instrSession,
                                                      ViInt32* number_ofDataSets,
                                                      ViInt32 bufferSize,
                                                      ViChar dataSetList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FMU"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		3, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, dataSetList), 4, "Data Set List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "SENS:PROB:CAT?", &buf));
	checkErr(RsCore_ParseCatalog(buf, bufferSize, dataSetList, number_ofDataSets));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe State
 * Purpose:  This function activates a selected probe data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeState(ViSession instrSession,
                                                 ViString probeDataSetName,
                                                 ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT, probeDataSetName),
		2, "Probe Data Set Name");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Settings
 * Purpose:  This function configures the probe mode, DC voltage and pulse
 *           frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeSettings(ViSession instrSession,
                                                    ViInt32 probeMode,
                                                    ViReal64 value)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (probeMode)
	{
	case RSSPECAN_VAL_PROBE_MODE_DC:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_DC_VOLTAGE, value),
			3, "Value");
		break;
	case RSSPECAN_VAL_PROBE_MODE_PULS:
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_PULSE_FREQ, value),
			3, "Value");
		break;
	case RSSPECAN_VAL_PROBE_MODE_COMP:
		break;
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_ADJUST_MODE, probeMode),
		2, "Probe Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Manual Compensation
 * Purpose:  This function configures the calibration probe manual compensation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeManualCompensation(ViSession instrSession,
                                                              ViBoolean manualProbeAdjustment)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_MAN_COMPENSATION, manualProbeAdjustment),
		2, "Manual Probe Adjustment");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Compensation
 * Purpose:  This function configures the calibration probe compensation.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeCompensation(ViSession instrSession,
                                                        ViBoolean frequencyResponseComp,
                                                        ViBoolean gainErrorCompensation)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_FREQ_RESP_COMPENSATION, frequencyResponseComp),
		2, "Frequency Response Comp");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_GAIN_ERROR_COMPENSATION, gainErrorCompensation),
		3, "Gain Error Compensation");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_StartProbeCalibration(ViSession instrSession,
                                                 ViString probeDataSetName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	// STAT:QUES:SYNC:ENAB, STAT:QUES:SYNC:PTR, STAT:QUES:SYNC:NTR
	checkErr(rsspecan_setStatusRegister (instrSession, 1, 4, 12288, 12288, 0));

	// STAT:OPER:ENAB, STAT:OPER:PTR, STAT:OPER:NTR
	checkErr(rsspecan_setStatusRegister (instrSession, 1, 0, 2048, 2048, 0));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_START, probeDataSetName),
		2, "Probe Data Set Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Probe Calibration Status
 * Purpose:  This function returns the status of the probe calibration.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetProbeCalibrationStatus(ViSession instrSession,
                                                     ViUInt32 timeout,
                                                     ViBoolean* probeCalibrationStatusI,
                                                     ViBoolean* probeCalibrationStatusQ)
{
	ViStatus error = VI_SUCCESS;
	ViUInt32 counter = 0;
	ViInt32 status = 0;
	ViInt32 response = 0;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

	*probeCalibrationStatusI = VI_FALSE;
	*probeCalibrationStatusQ = VI_FALSE;

	do
	{
		checkErr(RsCore_QueryViInt32(instrSession, "*STB?", &response));

		if (response & 0x8)
		{
			checkErr(rsspecan_getStatusRegister (instrSession, 10, &status));
			if (status & 4096) // 12. bit
				*probeCalibrationStatusI = VI_TRUE;
			if (status & 8192) // 13. bit
				*probeCalibrationStatusQ = VI_TRUE;
			break;
		}

		RsCore_Delay(0.001);

		counter++;

		if (counter >= timeout)
			return VI_ERROR_TMO;

		/* Wait while STB is non zero value. Only Error/Event Queue
		   and STAT:QUES bits are taken into consideration. */
	}
	while ((response & 12) == 0);

	// (check Error(Event) Queue bit in STB)
	if ((response & 0x4) != 0)
		return RS_ERROR_INSTRUMENT_STATUS;

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Check
 * Purpose:  This function queries, if the calibration of the selected probe
 *           data set passed or failed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeCheck(ViSession instrSession,
                                                 ViString probeDataSetName,
                                                 ViInt32* probeCheck)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT, probeDataSetName),
		2, "Probe Data Set Name");

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_CHECK, probeCheck),
		3, "Probe Check");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Result
 * Purpose:  This function queries the calibration result of the selected
 *           probe data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeResult(ViSession instrSession,
                                                  ViString probeDataSetName,
                                                  ViInt32 arraySize,
                                                  ViChar result[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FMU"));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT, probeDataSetName),
		2, "Probe Data Set Name");

	checkErr(rsspecan_QueryViString(instrSession, "SENS:PROB:RES?", arraySize, result));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Rename
 * Purpose:  This function renames existing probe data sets.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeRename(ViSession instrSession,
                                                  ViString probeDataSetSource,
                                                  ViString probeDataSetDestination)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FMU"));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS:PROB:MOVE '%s','%s'", probeDataSetSource, probeDataSetDestination);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Calibration Probe Delete
 * Purpose:  This function deletes the specified probe data set.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CalibrationProbeDelete(ViSession instrSession,
                                                  ViString probeDataSetName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_CAL_PROBE_DELETE, probeDataSetName),
		2, "Probe Data Set Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function selects the probe connector.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR probeConnector/Selects the connector.
ViStatus _VI_FUNC rsspecan_SelectProbeConnector(ViSession instrSession,
                                                ViInt32 probeConnector)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_PROBE_CONNECTOR, probeConnector),
		2, "Probe Connector");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function activates a connected probe.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR state/This conrol activates a connected probe.
ViStatus _VI_FUNC rsspecan_ProbeActivation(ViSession instrSession,
                                           ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_PROBE_ACTIVATION, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines which action is taken when the probe's micro button is pressed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR microButtonAction/This control selects which action is taken when the probe's
/// HIPAR microButtonAction/micro button is pressed.
ViStatus _VI_FUNC rsspecan_ProbeMicroButtonAction(ViSession instrSession,
                                                  ViInt32 microButtonAction)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 probe = 0;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, "",
		RSSPECAN_ATTR_PROBE_CONNECTOR, &probe));

	checkErr(RsCore_GetAttributeRepCapName (instrSession,
		RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION,
		"Probe",
		probe - 1,
		10,
		repCap));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION, microButtonAction),
		2, "microButtonAction");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_GetProbeInformation(ViSession instrSession, ViInt32 bufferSize,
                                               ViInt32* state, ViChar name[],
                                               ViChar serialNumber[],
                                               ViChar partNumber[])
{
	ViStatus error = VI_SUCCESS;
	ViInt32 probe = 0;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";
	ViChar repCap_2[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, "",
		RSSPECAN_ATTR_PROBE_CONNECTOR, &probe));

	checkErr(RsCore_GetAttributeRepCapName (instrSession,
		RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION,
		"Probe",
		probe - 1,
		10,
		repCap));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PROBE_CONNECTION_STATE, state),
		3, "state");

	if (*state == 1)
	{
		viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_PROBE_NAME,
				bufferSize, name),
			4, "name");

		strcpy(repCap_2, repCap);
		strcat(repCap, ",Serial");

		viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_PROBE_NUMBER,
				bufferSize, serialNumber),
			5, "serialNumber");

		strcat(repCap_2, ",Part");

		viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap_2, RSSPECAN_ATTR_PROBE_NUMBER,
				bufferSize, partNumber),
			6, "partNumber");
	}
	else
	{
		sprintf(name, "Not detected");
		sprintf(serialNumber, "Not detected");
		sprintf(partNumber, "Not detected");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function returns the type of the probe.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR probeConnector/Selects the connector.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the Probe type parameter.
/// HIPAR probeType/This control returns the probe type.
ViStatus _VI_FUNC rsspecan_GetProbeType(ViSession instrSession,
                                        ViInt32 probeConnector,
                                        ViInt32 bufferSize,
                                        ViChar probeType[])
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_PROBE_TYPE,
			"Probe", probeConnector - 1, RS_REPCAP_BUF_SIZE, repCap),
		2, "Probe Connector");

	viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_PROBE_TYPE,
			bufferSize, probeType),
		4, "Probe Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Control Lines State
 * Purpose:  This function toggles the control lines of the user ports between
 *           INPut and OUTPut.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureControlLinesState(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_INP_UPORT_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Output Control Lines
 * Purpose:  This function sets the control lines of the user ports.
 *****************************************************************************/
#pragma warning( push )
#pragma warning( disable : 4100)
ViStatus _VI_FUNC rsspecan_ConfigureOutputControlLines(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViString userPorts)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_OUT_UPORT_VALUE, userPorts),
		3, "User Ports");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
#pragma warning( pop )

/*****************************************************************************
 * Function: Configure Output Control Lines
 * Purpose:  This function sets the control lines of the user ports.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetControlLines(ViSession instrSession,
                                           ViInt32 window,
                                           ViInt32 lines,
                                           ViChar userPorts[])
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, lines, 0, 1),
		3, "Lines");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	switch (lines)
	{
	case RSSPECAN_VAL_USER_PORT_INP:
		viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_INP_UPORT_VALUE,
				8, userPorts),
			3, "User Ports");
		break;
	case RSSPECAN_VAL_USER_PORT_OUT:
		viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_OUT_UPORT_VALUE,
				8, userPorts),
			3, "User Ports");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Auto Adjust
 * Purpose:  This function configures the auto adjust settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjust(ViSession instrSession,
                                               ViInt32 adjustSettings,
                                               ViInt32 measurementTimeMode,
                                               ViReal64 measurementTime)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSW"))
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "FSW");
	else
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "FSV");

	switch (adjustSettings)
	{
	case RSSPECAN_VAL_ADJ_ALL:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_AUTO_ADJUST_ALL, NULL));
		break;
	case RSSPECAN_VAL_ADJ_FREQ:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_AUTO_ADJUST_FREQ, NULL));
		break;
	case RSSPECAN_VAL_ADJ_LEV:
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_AUTO_ADJUST_LEVEL, NULL));
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, adjustSettings), 2, "Adjust Settings");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_AUTO_ADJUST_LEVEL_MEAS_TIME_MODE, measurementTimeMode),
		3, "Measurement Time Mode");

	if (measurementTimeMode == RSSPECAN_VAL_MAN)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_AUTO_ADJUST_LEVEL_MEAS_TIME, measurementTime),
			4, "Measurement Time");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjustHystersis(ViSession instrSession,
                                                        ViReal64 lowerHystersis,
                                                        ViReal64 upperHystersis)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Low", RSSPECAN_ATTR_AUTO_ADJUST_HYSTERSIS, lowerHystersis),
		2, "Lower Hystersis");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Upp", RSSPECAN_ATTR_AUTO_ADJUST_HYSTERSIS, upperHystersis),
		3, "Upper Hystersis");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function defines the behaviour of the measurement when adjusting a setting automatically.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR state/Defines the behaviour of the measurement when adjusting a setting automatically.
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjustTriggerState(ViSession instrSession,
                                                           ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AUTO_ADJUST_TRIGGER_STATE, state),
		2, "state");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Y-Axis Auto Scale
 * Purpose:  This function initiates auto scaling for screens.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_YAxisAutoScale(ViSession instrSession,
                                          ViInt32 autoScaleScreen)

{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, autoScaleScreen, RSSPECAN_VAL_AUTOSCALE_ALL, RSSPECAN_VAL_AUTOSCALE_D),
		2, "Auto Scale Screen");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", autoScaleScreen);

	if (autoScaleScreen == RSSPECAN_VAL_AUTOSCALE_ALL)
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_AUTO_SCALE_ALL_SCREENS, NULL));
	}
	else
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_AUTO_SCALE_SINGLE_SCREEN, NULL));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: rsspecan_SetAttribute<type> Functions
 * Purpose:  These functions enable the instrument driver user to set
 *           attribute values directly.  There are typesafe versions for
 *           ViInt32, ViReal64, ViString, ViBoolean, and ViSession datatypes.
 *****************************************************************************/

/* ViInt32 */
ViStatus _VI_FUNC rsspecan_SetAttributeViInt32(ViSession instrSession, ViString channelName,
                                               ViUInt32 attributeId, ViInt32 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViInt32(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViReal64 */
ViStatus _VI_FUNC rsspecan_SetAttributeViReal64(ViSession instrSession, ViString channelName,
                                                ViUInt32 attributeId, ViReal64 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViReal64(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViString */
ViStatus _VI_FUNC rsspecan_SetAttributeViString(ViSession instrSession, ViString channelName,
                                                ViUInt32 attributeId, ViString value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViString(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViBoolean */
ViStatus _VI_FUNC rsspecan_SetAttributeViBoolean(ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViBoolean value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_SetAttributeViBoolean(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViSession */
ViStatus _VI_FUNC rsspecan_SetAttributeViSession(ViSession instrSession, ViString channelName,
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
ViStatus _VI_FUNC rsspecan_GetAttributeViInt32(ViSession instrSession, ViString channelName,
                                               ViUInt32 attributeId, ViInt32* value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);
	error = RsCore_GetAttributeViInt32(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViReal64 */
ViStatus _VI_FUNC rsspecan_GetAttributeViReal64(ViSession instrSession, ViString channelName,
                                                ViUInt32 attributeId, ViReal64* value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_GetAttributeViReal64(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViString */
ViStatus _VI_FUNC rsspecan_GetAttributeViString(ViSession instrSession, ViString channelName,
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
ViStatus _VI_FUNC rsspecan_GetAttributeViBoolean(ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViBoolean* value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_GetAttributeViBoolean(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViSession */
ViStatus _VI_FUNC rsspecan_GetAttributeViSession(ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViSession* value)
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
ViStatus _VI_FUNC rsspecan_CheckAttributeViInt32(ViSession instrSession, ViString channelName,
                                                 ViUInt32 attributeId, ViInt32 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViInt32(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViReal64 */
ViStatus _VI_FUNC rsspecan_CheckAttributeViReal64(ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViReal64 value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViReal64(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViString */
ViStatus _VI_FUNC rsspecan_CheckAttributeViString(ViSession instrSession, ViString channelName,
                                                  ViUInt32 attributeId, ViString value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViString(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViBoolean */
ViStatus _VI_FUNC rsspecan_CheckAttributeViBoolean(ViSession instrSession, ViString channelName,
                                                   ViUInt32 attributeId, ViBoolean value)
{
	ViStatus error = VI_SUCCESS;
	(void)RsCore_LockSession(instrSession);

	error = RsCore_CheckAttributeViBoolean(instrSession, channelName, attributeId, RS_VAL_DIRECT_USER_CALL, value);

	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/* ViSession */
ViStatus _VI_FUNC rsspecan_CheckAttributeViSession(ViSession instrSession, ViString channelName,
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
ViStatus _VI_FUNC rsspecan_GetAttributeRepeatedCapabilityIds(ViSession instrSession,
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
ViStatus _VI_FUNC rsspecan_GetAttributeRepeatedCapabilityIdNames(ViSession instrSession,
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
ViStatus _VI_FUNC rsspecan_ReadYTrace(ViSession instrSession,
                                      ViInt32 window,
                                      ViInt32 trace,
                                      ViUInt32 maximumTime,
                                      ViInt32 arrayLength,
                                      ViInt32* actualPoints,
                                      ViReal64 amplitude[])

{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL") || rsspecan_IsFSV(instrSession))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");
	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, maximumTime, 0, 4294967295UL), 4, "Maximum Time");

	checkErr(rsspecan_Initiate (instrSession, window, maximumTime));

	sprintf(traceName, "TRACE%ld", trace);
	checkErr(rsspecan_dataReadTrace (instrSession, window, traceName, arrayLength,
		amplitude, actualPoints));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_FetchYTrace(ViSession instrSession,
                                       ViInt32 window,
                                       ViInt32 trace,
                                       ViInt32 arrayLength,
                                       ViInt32* actualPoints,
                                       ViReal64 amplitude[])
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 trace_range = 3;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL") || rsspecan_IsFSV(instrSession))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");

	sprintf(traceName, "TRACE%ld", trace);

	checkErr(rsspecan_dataReadTrace (instrSession,window, traceName, arrayLength,
		amplitude, actualPoints));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ReadYTracePrevious(ViSession instrSession,
                                              ViInt32 window, ViInt32 trace,
                                              ViInt32 offsetSweepPoint,
                                              ViInt32 numberOfSweeps,
                                              ViInt32 arrayLength,
                                              ViInt32* actualPoints,
                                              ViReal64 amplitude[])
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 trace_range = 6;

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		3, "Trace");

	sprintf(traceName, ":MEM? TRACE%ld,%ld,%ld", trace, offsetSweepPoint, numberOfSweeps);

	checkErr(rsspecan_dataReadTrace (instrSession, window, traceName, arrayLength,
		amplitude, actualPoints));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Reads the x values (time in seconds) of the measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR arrayLength/Number of array points requested.
/// HIPAR actualPoints/Number of points actually returned in the array.
/// HIPAR x/A user allocated buffer into which the trace X values is stored.
ViStatus _VI_FUNC rsspecan_FetchXTrace(ViSession instrSession,
                                       ViInt32 trace, ViInt32 arrayLength,
                                       ViInt32* actualPoints,
                                       ViReal64 x[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 6),
		2, "Timeout");

	sprintf(cmd, "FORM REAL,32;TRAC:DATA:X? TRACe%ld", trace);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, arrayLength, x, actualPoints));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Get Current Results
 * Purpose:     This function returns the current X and Y results of the sweep.
 *              During a sweep the last measured values are read out.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetCurrentResults(ViSession instrSession,
                                             ViInt32 window,
                                             ViReal64* xValue,
                                             ViReal64* yValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL|FMU") || rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	viCheckParm(RsCore_InvalidNullPointer(instrSession, xValue), 3, "X Value");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, yValue), 4, "Y Value");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "TRAC%ld:IMM:RES?", window);
	checkErr(RsCore_QueryViString(instrSession, cmd, response));
	sscanf(response, "%le,%le", xValue, yValue);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Get Current Y Result
 * Purpose:     This function returns the current Y results of the sweep. During
 *              a sweep the last measured value is read out.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetCurrentYResult(ViSession instrSession,
                                             ViInt32 window,
                                             ViReal64* level)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_Y_RESULT, level),
		3, "Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Read Trace IQ Data
 * Purpose:     This function returns the current X and Y results of the sweep.
 *              During a sweep the last measured values are read out.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadTraceIQData(ViSession instrSession,
                                           ViInt32 window,
                                           ViUInt32 timeout,
                                           ViInt32 bufferSize,
                                           ViInt32* noofPoints,
                                           ViReal64 realPartsI[],
                                           ViReal64 imaginaryPartsQ[])
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize = 0, i;
	ViInt32 j = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		checkErr(RsCore_Write(instrSession, ":FORM REAL,32"));
	}
	else
	{
		checkErr(RsCore_Write(instrSession, ":FORM REAL,32;:TRAC:IQ:DATA:FORM IQP"));
	}

	if (window == 0)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "TRAC:IQ:DATA?");
	}
	else
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "TRAC%ld:IQ:DATA?", window);
	}

	checkErr(RsCore_QueryFloatArray(instrSession, cmd, &data, &dataSize));

	dataSize /= 2;

	if (noofPoints)
		*noofPoints = dataSize;

	if (dataSize > bufferSize)
		dataSize = bufferSize;

	for (i = 0; i < dataSize; i++)
	{
		realPartsI[i] = data[j++];
		imaginaryPartsQ[i] = data[j++];
	}

	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_FetchTraceIQData(ViSession instrSession,
                                            ViInt32 offsetSamples,
                                            ViInt32 noofSamples,
                                            ViInt32 bufferSize,
                                            ViInt32* noofPoints,
                                            ViReal64 realPartsI[],
                                            ViReal64 imaginaryPartsQ[])
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize = 0, i;
	ViInt32 j = 0;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		checkErr(RsCore_Write(instrSession, ":FORM REAL,32"));
	}
	else
	{
		checkErr(RsCore_Write(instrSession, ":FORM REAL,32;:TRAC:IQ:DATA:FORM IQP"));
	}

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "TRAC:IQ:DATA:MEM? %ld,%ld", offsetSamples, noofSamples);
	checkErr(RsCore_QueryFloatArray(instrSession, cmd, &data, &dataSize));

	dataSize /= 2;

	if (noofPoints)
		*noofPoints = dataSize;

	if (dataSize > bufferSize)
		dataSize = bufferSize;

	for (i = 0; i < dataSize; i++)
	{
		realPartsI[i] = data[j++];
		imaginaryPartsQ[i] = data[j++];
	}

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 window,
	ViInt32 limit,
	ViInt32* state
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 8),
		3, "Limit");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", window, limit);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_CHECK_RESULT, state),
		4, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_Initiate(ViSession instrSession,
                                    ViInt32 window,
                                    ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_INIT, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Initiate Spurious
 * Purpose:     This function starts a new spurious measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_InitiateSpurious(ViSession instrSession,
                                            ViInt32 window,
                                            ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_INIT_SPURIOUS, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Initiate Spectrum Emission Mask Measurement
 * Purpose:     This function starts a Spectrum Emission Mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_InitiateSEMMeasurement(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_INIT_SEM_MEAS, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_Abort(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_ABORT, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_Continue(ViSession instrSession,
                                    ViInt32 window,
                                    ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_INIT_CONMEAS, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/******************************************************************************
 * Function: Send Software Trigger
 * Purpose:  This function sends a software trigger to the instrument.  If the
 *           instrument is not configured to receive a software trigger, this
 *           function returns an error.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SendSoftwareTrigger(ViSession instrSession,
                                               ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_INIT_SW_TRIGGER, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViUInt32 timeout
)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_INIT_SEQUENCER, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ABORT_SEQUENCER, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_REFRESH_SEQUENCER, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViBoolean state
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEQUENCER_STATE, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 sequencerMode
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEQUENCER_MODE, sequencerMode),
		2, "Sequencer Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTimeDomainPowerMeasurement(ViSession instrSession,
                                                               ViInt32 window,
                                                               ViBoolean peak,
                                                               ViBoolean RMS,
                                                               ViBoolean mean,
                                                               ViBoolean standardDeviation,
                                                               ViBoolean average,
                                                               ViBoolean maxHold,
                                                               ViInt32 power)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_PEAK, peak),
		3, "Peak");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_RMS, RMS),
		4, "RMS");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_MEAN, mean),
		5, "Mean");
	if (mean == VI_TRUE)
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_SDEV, standardDeviation),
		6, "Standard Deviation");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_AVG, average),
		7, "Average");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_MAX_HOLD, maxHold),
		8, "Max Hold");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_MODE, power),
		9, "Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Configure Time Domain Power Measurement State
 * Purpose:  This function switches on or off the previously selected time
 *           domain power measurements. Thus one or several measurements can
 *           be first selected and then switched on and off together with this
 *          attribute.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTimeDomainPowerMeasurementState(ViSession instrSession,
                                                                    ViInt32 window,
                                                                    ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Query Time Domain Power Result
 * Purpose:  This function queries selected time domain power measurement
 *           result in the indicated measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTDomPowerResult(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 measurement,
                                                ViInt32 resultType,
                                                ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 3),
		3, "Measurement");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, 0, 2),
		4, "Result Type");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	switch (measurement)
	{
	case RSSPECAN_VAL_TDOM_PEAK:
		switch (resultType)
		{
		case RSSPECAN_VAL_TDOM_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_PEAK_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_AVG_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_AVG_PEAK_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_RESULT, result),
				5, "Result");
			break;
		}
		break;
	case RSSPECAN_VAL_TDOM_RMS:
		switch (resultType)
		{
		case RSSPECAN_VAL_TDOM_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_RMS_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_AVG_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_AVG_RMS_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_RMS_RESULT, result),
				5, "Result");
			break;
		}
		break;
	case RSSPECAN_VAL_TDOM_MEAN:
		switch (resultType)
		{
		case RSSPECAN_VAL_TDOM_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_MEAN_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_AVG_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_AVG_MEAN_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_MEAN_RESULT, result),
				5, "Result");
			break;
		}
		break;
	case RSSPECAN_VAL_TDOM_SDEV:
		switch (resultType)
		{
		case RSSPECAN_VAL_TDOM_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_SDEV_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_AVG_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_AVG_SDEV_RESULT, result),
				5, "Result");
			break;
		case RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT:
			viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_SDEV_RESULT, result),
				5, "Result");
			break;
		}
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryTDomPowerPulseSequenceResult(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViReal64 timeOffsetofFirstPulse,
                                                             ViReal64 measurementTime,
                                                             ViReal64 period,
                                                             ViInt32 numberofPulses,
                                                             ViReal64 result[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, numberofPulses, 1, INT_MAX),
		6, "Number Of Pulses");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, result), 7, "Result");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:MSUM? %.12f,%.12f,%.12f,%ld", window, timeOffsetofFirstPulse, measurementTime, period, numberofPulses);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, numberofPulses, result, NULL));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_TimeDomainPowerMeasurementSetReference(ViSession instrSession,
                                                                  ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_SET_REFERENCE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Time Domain Power Measurement All Off
 * Purpose:  This function switches off all time domain measurements in
 *           the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TimeDomainPowerMeasurementAllOff(ViSession instrSession,
                                                            ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_TDOM_AOFF, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn Power Measurement Group Functions ==============================*
 *===========================================================================*/

/*****************************************************************************
 * Function: Select Power Measurement
 * Purpose:  This function configures the power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SelectPowerMeasurement(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViBoolean state,
                                                  ViInt32 powerMeasurement)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	if (state == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_SELECT, powerMeasurement),
			4, "Power Measurement");
	}
	else
	{
		checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_OFF, NULL));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Power Results
 * Purpose:  This function returns the power measurement results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryPowerResults(ViSession instrSession,
                                             ViInt32 window,
                                             ViInt32 powerMeasurement,
                                             ViInt32 arraySize,
                                             ViReal64 results[],
                                             ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, powerMeasurement, 0, 6),
		3, "Power Measurement");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 0, INT_MAX),
		4, "Array Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 5, "Results");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:POW:RES? %s",
	         window, rsspecan_rngMeasPowerSelect.rangeValues[powerMeasurement].cmdString);
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, arraySize, results, returnedValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Channel Power Standard Catalog
 * Purpose:  This function returns all predefined ACP standards and all user
 *           standards.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryChannelPowerStandardCatalog(ViSession instrSession,
                                                            ViInt32 bufferSize,
                                                            ViChar powerStandardCatalog[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_QueryViString(instrSession, "CALC:MARK:FUNC:POW:STAN:CAT?", bufferSize, powerStandardCatalog));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Channel Power User Standard
 * Purpose:  This function configures the user standard for channel power
 *           measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerUserStandard(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 operation,
                                                             ViString userStandardName)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViAttr attr_arr[3] = {
		RSSPECAN_ATTR_ACP_USER_STD_SET,
		RSSPECAN_ATTR_ACP_USER_STD_SAVE,
		RSSPECAN_ATTR_ACP_USER_STD_DELETE
	};

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, operation, RSSPECAN_VAL_USTD_SET, RSSPECAN_VAL_USTD_DEL),
		3, "Operation");

	if (operation == RSSPECAN_ATTR_ACP_USER_STD_SET)
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, attr_arr[operation], userStandardName),
		4, "User Standard Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Power Adjust Reference Level
 * Purpose:  This function sets the reference value to the currently measured
 *           channel power.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PowerAdjustReferenceLevel(ViSession instrSession,
                                                     ViInt32 window,
                                                     ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_REF_VALUE, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_PowerPresetMeasurement(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViInt32 channelPowerType,
                                                  ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_PRESET, channelPowerType),
		3, "Channel Power Type");

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Channel Power Standard
 * Purpose:  This function configures the standard for channel power measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerStandard(ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 ACPStandard)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_STANDARD, ACPStandard),
		3, "ACP Standard");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Power Measurement
 * Purpose:  This function configures the adjacent power channels measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerMeasurement(ViSession instrSession, ViInt32 window, ViInt32 noOfAdjChannels, ViInt32 channelMode, ViInt32 powerMode, ViBoolean resultMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_NUM, noOfAdjChannels),
		3, "No Of Adj Channels");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_MODE, channelMode),
		4, "Channel Mode");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_MODE, powerMode),
		5, "Power Mode");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_RESULT_MODE, resultMode),
		6, "Result Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Power Channel Spacing
 * Purpose:  This function configures the adjacent power channel spacing.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelSpacing(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 channel,
                                                        ViInt32 channelNumber,
                                                        ViReal64 spacing)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channel, 0, 2),
		3, "Channel");

	switch (channel)
	{
	case RSSPECAN_VAL_ACP_TX:
		if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		{
			viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 1),
				4, "Channel");
		}
		else
		{
			viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 11),
				4, "Channel");
		}
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,CH%ld", window, channelNumber);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_CHANNEL_SPACING, spacing),
			5, "Spacing");
		break;
	case RSSPECAN_VAL_ACP_ADJ:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_SPACING, spacing),
			5, "Spacing");
		break;
	case RSSPECAN_VAL_ACP_ALT:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 11),
			4, "Channel");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,CH%ld", window, channelNumber);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_SPACING, spacing),
			5, "Spacing");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Power Channel Bandwidth
 * Purpose:  This function configures the adjacent power channel bandwidth.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelBandwidth(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 channel,
                                                          ViInt32 channelNumber,
                                                          ViReal64 bandwidth)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channel, 0, 2),
		3, "Channel");

	switch (channel)
	{
	case RSSPECAN_VAL_ACP_TX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_CHANNEL_BANDWIDTH, bandwidth),
			5, "Bandwidth");
		break;
	case RSSPECAN_VAL_ACP_ADJ:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_BANDWIDTH, bandwidth),
			5, "Bandwidth");
		break;
	case RSSPECAN_VAL_ACP_ALT:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 11),
			4, "Channel");
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,CH%ld", window, channelNumber);
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_BANDWIDTH, bandwidth),
			5, "Bandwidth");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function configures the power channel name.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR channel/Selects the channel type.
/// HIPAR channelNumber/Sets the channel number.
/// HIPAR name/Sets the power channel name.

ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelName(ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 channel,
                                                     ViInt32 channelNumber,
                                                     ViString name)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViAttr attr[] = {
		RSSPECAN_ATTR_MEAS_POW_CHANNEL_NAME,
		RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_NAME,
		RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_NAME
	};

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channel, RSSPECAN_VAL_ACP_TX, RSSPECAN_VAL_ACP_ALT),
		3, "Channel");

	if (channel == RSSPECAN_VAL_ACP_ADJ)
	{
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	}
	else if (channel == RSSPECAN_VAL_ACP_TX)
	{
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 18),
			4, "Channel Number");

		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TX%ld", window, channelNumber);
	}
	else
	{
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 11),
			4, "Channel Number");

		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,CH%ld", window, channelNumber);
	}

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, attr[channel], name),
		5, "Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ACPChannelPowerAutoAdjust(ViSession instrSession,
                                                     ViInt32 window,
                                                     ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerNoiseCorrection(ViSession instrSession,
                                                                ViInt32 window,
                                                                ViBoolean noiseCorrectionState)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_NOISE_CORR, noiseCorrectionState),
		3, "Noise Correction State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function selects on, off or auto correction of the instrument inherent noise
/// HIFN for channel power measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR window/This control selects the measurement window.
/// HIPAR mode/Activates or deactivates a noise correction measurement.
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerNoiseCorrectionAuto(ViSession instrSession,
                                                                    ViInt32 window,
                                                                    ViInt32 mode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_NOISE_CORR_AUTO, mode),
		3, "mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Channel Power Trace
 * Purpose:  This function assigns the channel/adjacent channel power measurement
 *           of the indicated trace in the selected measurement window. The
 *           corresponding trace should be active, ie its states should
 *           differ from blank.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerTrace(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 traceNumber)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_TRACE, traceNumber),
		3, "Trace Number");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Power Channel Weighting Filter
 * Purpose:  This function configures the power channel weighting filters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelWeightingFilters(ViSession instrSession,
                                                                 ViInt32 filterType,
                                                                 ViBoolean filterState,
                                                                 ViReal64 filterValue)
{
	ViStatus error = VI_SUCCESS;
	ViAttr attr_state[4] = {
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALL_STATE,
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ACH_STATE,
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALT_STATE,
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_TX_CHANNEL_STATE
	};
	ViAttr attr_val[4] = {
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALL_ALPHA,
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ACH_ALPHA,
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALT_ALPHA,
		RSSPECAN_ATTR_SEM_WEIGHT_FILTER_TX_CHANNEL_ALPHA
	};

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, filterType, RSSPECAN_VAL_WFILTER_ALL, RSSPECAN_VAL_WFILTER_TX),
		2, "Filter Type");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", attr_state[filterType], filterState),
		3, "Filter State");

	if (filterState == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", attr_val[filterType], filterValue),
			4, "Filter Value");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureACPFastACPState(ViSession instrSession,
                                                    ViInt32 window,
                                                    ViBoolean fastACPState)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_HSP, fastACPState),
		3, "Fast ACP State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: ACP Channel Power Adjust Ref Level Offset
 * Purpose:  This function defines an additional level offset to be taken into
 *           account for the analyzer's reference level setting. It is only
 *           available if Fast ACP is not active and no predefine standard is
 *           selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ACPChannelPowerAdjustRefLevelOffset(ViSession instrSession,
                                                               ViInt32 window,
                                                               ViReal64 adjustRefLevelOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL_OFFSET, adjustRefLevelOffset),
		3, "Adjust Ref Level Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: ACP Adjust Reference Level To Channel Power
 * Purpose:  This function defines an additional level offset to be taken into
 *           account for the analyzer's reference level setting. It is only
 *           available if Fast ACP is not active and no predefine standard is
 *           selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ACPAdjustReferenceLevelToChannelPower(ViSession instrSession,
                                                                 ViInt32 window,
                                                                 ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295), 2, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL_ADJ, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure ACP Limit State
 * Purpose:  This function switches on and off the limit check for adjacent
 *           channel power measurements in the selected measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPLimitState(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_LIMIT_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure ACP Adjacent Channel Limit
 * Purpose:  This function defines the limit value of the adjacent channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPAdjacentChannelLimit(ViSession instrSession,
                                                            ViInt32 window,
                                                            ViBoolean relativeState,
                                                            ViReal64 relativeValue,
                                                            ViBoolean absoluteState,
                                                            ViReal64 absoluteValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViReal64Range(instrSession, relativeValue, 0.0, 100.0), 4, "Relative Value");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, absoluteValue, -200.0, 200.0), 6, "Absolute Value");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ACH %.12lf,0.0", window, relativeValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_LIMIT_REL_STATE, relativeState),
			3, "Relative State");

		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ACH:ABS %.12lf,0.0", window, absoluteValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
		if (relativeState == VI_FALSE)
		{
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_LIMIT_ABS_STATE, absoluteState),
				5, "Absolute State");
		}
	}
	else
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ACH %.12lf,0.0", window, relativeValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_LIMIT_REL_STATE, relativeState),
			3, "Relative State");
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ACH:ABS %.12lf,0.0", window, absoluteValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_LIMIT_ABS_STATE, absoluteState),
			5, "Absolute State");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure ACP Alternate Channel Limit
 * Purpose:  This function defines the limit value of the alternate adjacent
 *           channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPAlternateChannelLimit(ViSession instrSession,
                                                             ViInt32 window,
                                                             ViInt32 channelNumber,
                                                             ViBoolean relativeState,
                                                             ViReal64 relativeValue,
                                                             ViBoolean absoluteState,
                                                             ViReal64 absoluteValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 11),
		3, "Channel Number");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, relativeValue, 0.0, 100.0), 5, "Relative Value");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, absoluteValue, -200.0, 200.0), 7, "Absolute Value");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,CH%ld", window, channelNumber);

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_REL_STATE, relativeState),
			4, "Relative State");
		if (relativeState)
		{
			snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ALT%ld %.12lf,0.0", window,
			         channelNumber, relativeValue);
			checkErr(RsCore_Write(instrSession, cmd));
			checkErr(rsspecan_CheckStatus (instrSession));
		}

		if (absoluteState)
		{
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_ABS_STATE, absoluteState),
				6, "Absolute State");

			snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ALT%ld:ABS %.12lf,0.0", window,
			         channelNumber, absoluteValue);
			checkErr(RsCore_Write(instrSession, cmd));
			checkErr(rsspecan_CheckStatus (instrSession));
		}
	}
	else
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ALT%ld %.12lf,0.0", window,
		         channelNumber, relativeValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_REL_STATE, relativeState),
			4, "Relative State");
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ALT%ld:ABS %.12lf,0.0", window,
		         channelNumber, absoluteValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_ABS_STATE, absoluteState),
			6, "Absolute State");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query ACP Alternate Channel Limit Check Results
 * Purpose:  This function returns the adjacent channel limit results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryACPAdjacentChannelLimitCheckResults(ViSession instrSession,
                                                                    ViInt32 window,
                                                                    ViInt32* lowerResult,
                                                                    ViInt32* upperResult)
{
	ViStatus error = VI_SUCCESS;
	ViChar buf1[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar buf2[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, lowerResult), 3, "Lower Result");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, upperResult), 4, "Upper Result");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ACH:RES?", window);
	checkErr(RsCore_QueryViString(instrSession, cmd, response));
	sscanf(response, "%[^,],%[^\r\n]", buf1, buf2);

	checkErr(RsCore_GetIndexOfToken(resultsString, ',', buf1, lowerResult, NULL));
	checkErr(RsCore_GetIndexOfToken(resultsString, ',', buf2, upperResult, NULL));
	checkErr(rsspecan_CheckStatus(instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query ACP Alternate Channel Limit Check Results
 * Purpose:  This function defines the limit value of the alternate adjacent
 *           channel.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryACPAlternateChannelLimitCheckResults(ViSession instrSession,
                                                                     ViInt32 window,
                                                                     ViInt32 channelNumber,
                                                                     ViInt32* lowerResult,
                                                                     ViInt32* upperResult)
{
	ViStatus error = VI_SUCCESS;
	ViChar buf1[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar buf2[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, channelNumber, 1, 11),
		3, "Channel Number");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, lowerResult), 4, "Lower Result");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, upperResult), 5, "Upper Result");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:LIM:ACP:ALT%ld:RES?", window, channelNumber);
	checkErr(RsCore_QueryViString(instrSession, cmd, response));
	sscanf(response, "%[^,],%[^\r\n]", buf1, buf2);

	checkErr(RsCore_GetIndexOfToken(resultsString, ',', buf1, lowerResult, NULL));
	checkErr(RsCore_GetIndexOfToken(resultsString, ',', buf2, upperResult, NULL));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Multicarrier ACP Power
 * Purpose:  This function configures the adjacent power channels measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureMulticarrierACPPower(ViSession instrSession, ViInt32 window, ViInt32 noOfAdjChannels, ViInt32 noOfTXChannels, ViInt32 channelMode,
                                                         ViInt32 powerMode, ViBoolean resultMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_NUM, noOfAdjChannels),
		3, "No Of Adj Channels");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_CARR_SIG_NUM, noOfTXChannels),
		4, "No Of TX Channels");
	if (noOfAdjChannels > 0)
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_MODE, channelMode),
		5, "Channel Mode");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_MODE, powerMode),
		6, "Power Mode");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_RESULT_MODE, resultMode),
		7, "Result Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureACPReferenceChannelAuto(ViSession instrSession,
                                                            ViInt32 window,
                                                            ViInt32 reference)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_REF_CHAN_SEL_AUTO, reference),
		3, "Reference");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure ACP Reference Channel Man
 * Purpose:  This function selects a transmission channel to be used as a
 *           reference channel in relative adjacent-channel power measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureACPReferenceChannelMan(ViSession instrSession,
                                                           ViInt32 window,
                                                           ViInt32 channel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_ADJ_REF_TXCHANNEL, channel),
		3, "Channel");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function defines the number of subblocks, i.e. groups of
/// HIFN  transmission channels in MSR signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR numberOfSubblocks/Defines the number of subblocks, i.e. groups of transmission channels
/// HIPAR numberOfSubblocks/in MSR signal.
ViStatus _VI_FUNC rsspecan_ConfigureMSRNumberOfSubblocks(ViSession instrSession,
                                                         ViInt32 numberOfSubblocks)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MSR_NUMBER_OF_SUBBLOCKS, numberOfSubblocks),
		2, "Number Of Subblocks");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMSRSubblockDefinition(ViSession instrSession,
                                                          ViInt32 subblock,
                                                          ViReal64 centerFrequency,
                                                          ViReal64 RFBandwidth,
                                                          ViInt32 txChannelCount)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subblock, 1, 5),
		2, "Subblock");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SB%ld", subblock);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_SUBBLOCK_CENTER_FREQUENCY, centerFrequency),
		3, "Center Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_SUBBLOCK_RF_BANDWIDTH, RFBandwidth),
		4, "RF Bandwidth");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MSR_SUBBLOCK_TX_CHANNEL_COUNT, txChannelCount),
		5, "Tx Channel Count");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMSRTxChannelDefinition(ViSession instrSession,
                                                           ViInt32 subblock,
                                                           ViInt32 txChannel,
                                                           ViReal64 txCenterFrequency,
                                                           ViInt32 technology,
                                                           ViReal64 txBandwidth,
                                                           ViBoolean weightingFilterState,
                                                           ViReal64 rollOffFactor)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subblock, 1, 5),
		2, "Subblock");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, txChannel, 1, 5),
		3, "Tx Channel");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SB%ld,TX%ld", subblock, txChannel);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_TX_CENTER_FREQUENCY, txCenterFrequency),
		4, "Tx Center Frequency");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MSR_TX_TECHNOLOGY, technology),
		5, "Technology");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_TX_BANDWIDTH, txBandwidth),
		6, "Tx Bandwidth");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MSR_TX_WEIGHTING_FILTER_STATE, weightingFilterState),
		7, "Weighting Filter State");

	if (weightingFilterState == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_TX_WEIGHTING_FILTER_ROLL_OFF_FACTOR, rollOffFactor),
			8, "Roll Off Factor");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryMSRTxName(ViSession instrSession,
                                          ViInt32 subblock, ViInt32 txChannel,
                                          ViInt32 bufferSize,
                                          ViChar name[])
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subblock, 1, 5),
		2, "Subblock");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, txChannel, 1, 5),
		3, "Tx Channel");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SB%ld,TX%ld", subblock, txChannel);

	viCheckParm(rsspecan_GetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MSR_TX_CHANNEL_NAME,
			bufferSize, name),
		4, "Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMSRGapChannel(ViSession instrSession,
                                                  ViInt32 gap,
                                                  ViReal64 channelSpacing,
                                                  ViReal64 channelBandwidths,
                                                  ViBoolean weightingFilterState,
                                                  ViReal64 rollOffFactor)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, gap, 1, 2),
		2, "Gap");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Gap%ld", gap);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_GAP_CHANNEL_SPACING, channelSpacing),
		3, "channelSpacing");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_GAP_CHANNEL_BANDWIDTHS, channelBandwidths),
		4, "channelBandwidths");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MSR_GAP_WEIGHTING_FILTER_STATE, weightingFilterState),
		5, "Weighting Filter State");

	if (weightingFilterState == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MSR_GAP_WEIGHTING_FILTER_ROLL_OFF_FACTOR, rollOffFactor),
			6, "Roll Off Factor");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMSRGapLimitChecking(ViSession instrSession,
                                                        ViInt32 gap,
                                                        ViBoolean relativeState,
                                                        ViReal64 relativeValue,
                                                        ViBoolean absoluteState,
                                                        ViReal64 absoluteValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, gap, 1, 2),
		2, "Gap");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Gap%ld", gap);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MSR_GAP_LIMIT_RELATIVE_STATE, relativeState),
		3, "Relative State");
	if (relativeState)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:LIM:ACP:GAP%ld:REL %.12lf,0.0", gap, relativeValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
	}

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MSR_GAP_LIMIT_ABSOLUTE_STATE, absoluteState),
		5, "Absolute State");
	if (absoluteState)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:LIM:ACP:GAP%ld:ABS %.12lf,0.0", gap, absoluteValue);
		checkErr(RsCore_Write(instrSession, cmd));
		checkErr(rsspecan_CheckStatus (instrSession));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureOccupiedBandwidth(ViSession instrSession,
                                                      ViInt32 window,
                                                      ViReal64 powerBandwidth,
                                                      ViReal64 channelBandwidth)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_BANDWIDTH, powerBandwidth),
		4, "Power Bandwidth");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_POW_CHANNEL_BANDWIDTH, channelBandwidth),
		5, "Channel Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*
 *= RsSpecAn Signal Statistics Group Functions ==============================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Configure Signal Statistic Measurement
 * Purpose:  This function configures parameters of signal statistic measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurement(ViSession instrSession,
                                                               ViInt32 measurement,
                                                               ViInt32 numberofSamples,
                                                               ViReal64 resolutionBandwidth)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurement, 0, 2),
		2, "Measurement");

	switch (measurement)
	{
	case RSSPECAN_VAL_SSTAT_OFF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_APD_STATE, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_FALSE));
		break;
	case RSSPECAN_VAL_SSTAT_APD:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_APD_STATE, VI_TRUE));
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_SAMPLES, numberofSamples),
			4, "Number Of Samples");
		break;
	case RSSPECAN_VAL_SSTAT_CCDF:
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_APD_STATE, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_TRUE));
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_SAMPLES, numberofSamples),
			3, "Number Of Samples");
		break;
	}
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_RESOLUTION_BANDWIDTH, resolutionBandwidth),
		4, "Resolution Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Sweep
 * Purpose:  This function configures the acquisition attributes of signal statistic.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticSweep(ViSession instrSession,
                                                         ViBoolean sweepModeContinuous)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, sweepModeContinuous),
		2, "Sweep Mode Continuous");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Mean Power Position
 * Purpose:  This function defines the relative X position of the mean power
 *           value for the CCDF measurement. The default position is 0% (left
 *           corner of the grid). This function is only available if the statistics
 *           measurement function CCDF is switched on.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeanPowerPosition(ViSession instrSession,
                                                                     ViReal64 meanPowerPosition)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_MEAN_POWER_POS, meanPowerPosition),
		2, "Mean Power Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Signal Statistic Measurement Adjust Settings
 * Purpose:  This function optimizes the level setting of the instrument
 *           depending on the measured peak power, in order to obtain maximum
 *           instrument sensitivity.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementAdjustSettings(ViSession instrSession,
                                                                    ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_ADJ, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Signal Statistic Measurement Reference Level Adjust
 * Purpose:  This function adjusts the reference level to get an optimized
 *           sensitivity. Only the reference level is changed all other setting
 *           parameters are kept.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementRefLevelAdjust(ViSession instrSession,
                                                                    ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_REF_LEVEL_ADJUST, NULL));

Error:
	if (old_timeout >= 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Measurement X Axis
 * Purpose:  This function configures parameters of signal statistic
 *           measurement X-Axis scaling.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurementXAxis(ViSession instrSession,
                                                                    ViReal64 xAxisRefLevel,
                                                                    ViReal64 xAxisRange)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_X_REF, xAxisRefLevel),
		2, "X Axis Ref Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_X_RANGE, xAxisRange),
		3, "X Axis Range");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Signal Statistic Measurement Y Axis
 * Purpose:  This function configures parameters of signal statistic
 *           measurement Y Axis scaling.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurementYAxis(ViSession instrSession, ViInt32 yUnit, ViReal64 yAxisMaxValue, ViReal64 yAxisMinValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (!RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_UNIT, yUnit),
			2, "Y Unit");
	}
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_MAX, yAxisMaxValue),
		3, "Y Axis Max Value");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_MIN, yAxisMinValue),
		4, "Y Axis Min Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Signal Statistic Measurement Preset Scaling
 * Purpose:  This function optimizes the level setting of the instrument
 *           depending on the measured peak power, in order to obtain maximum
 *           instrument sensitivity.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementPresetScaling(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_PRESET, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Signal Statistic CCDF Level
 * Purpose:  This function reads out the level values for the probabilities
 *           0.1%, 1% and 10%.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySignalStatisticCCDFLevel(ViSession instrSession,
                                                         ViInt32 trace,
                                                         ViInt32 probability,
                                                         ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FSL"));
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 3),
		2, "Trace");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, probability, 0, 3),
		3, "Probability");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, result), 4, "Result");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:STAT:CCDF:X%ld? %s", trace, statProbArr[probability]);
	checkErr(RsCore_QueryViReal64(instrSession, cmd, result));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Signal Statistic Result
 * Purpose:  This function reads out the results of statistical measurements
 *           of a recorded trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySignalStatisticResult(ViSession instrSession,
                                                      ViInt32 trace,
                                                      ViInt32 resultType,
                                                      ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 trace_range = 3;
	ViString statisticMeasType[] = {"Mean", "Peak", "CrestFactor"};
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
		trace_range = 4;

	if (rsspecan_IsFSV(instrSession))
		trace_range = 6;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, trace_range),
		2, "Trace");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "TR%ld,Stat%s", trace, statisticMeasType[resultType]);

	checkErr(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_STAT_RESULT, result));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureModDepthMeasurement(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_MDEPTH_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Query Modulation Depth
 * Purpose:  This function queries the AM modulation depth in the indicated
 *           measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryModulationDepth(ViSession instrSession,
                                                ViInt32 window,
                                                ViReal64* modulationDepth)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_MDEPTH_RESULT, modulationDepth),
		3, "Modulation Depth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Modulation Depth Signal Search
 * Purpose:  This function starts the search of the signals required for
 *           the modulation depth measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ModulationDepthSignalSearch(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 marker)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_MDEPTH_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureTOIMeasurement(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_MEAS_TOI_STATE, state),
		3, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query TOI
 * Purpose:  This function queries the third-order intercept point measurement
 *           in the indicated measurement window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryTOI(ViSession instrSession,
                                    ViInt32 window,
                                    ViReal64* TOIMeasurement)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_MEAS_TOI_RESULT, TOIMeasurement),
		3, "TOI Measurement");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: TOI Signal Search
 * Purpose:  This function starts the search of the signals required for the
 *           third order intercept measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TOISignalSearch(ViSession instrSession,
                                           ViInt32 window, ViInt32 marker)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];
	ViInt32 max_marker = 4;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
		max_marker = 16;

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, max_marker),
		3, "Marker");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_MEAS_TOI_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure TOI Positioning
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureTOIPositioning(ViSession instrSession,
                                                   ViInt32 window,
                                                   ViInt32 interceptMarkersPositioning)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MEAS_TOI_POSITIONING, interceptMarkersPositioning),
		3, "Intercept Markers Positioning");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*
 *= RsSpecAn Harmonic Distortion Group Functions ============================*
 *===========================================================================*/

/*****************************************************************************
 * Function: Configure Harmonic Distortion Measurement
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureHDistMeasurement(ViSession instrSession,
                                                     ViBoolean state,
                                                     ViInt32 noofHarmonics,
                                                     ViBoolean harmonicRBWAuto)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_HDIST_STATE, state),
		3, "State");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_HDIST_NOOFHARM, noofHarmonics),
		4, "No Of Harmonic");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_HDIST_RBWAUTO, harmonicRBWAuto),
		5, "Harmonic RBW Auto");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Harmonic Distortion Measurement
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_AdjustHDistSettings(ViSession instrSession,
                                               ViUInt32 timeout)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 old_timeout = -1;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");
	checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
	checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MEAS_HDIST_PRESET, NULL));

Error:
	if (old_timeout > 0)
		rsspecan_SetOPCTimeout(instrSession, old_timeout);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Harmonic Distortion
 * Purpose:  This function queries the results of the total harmonic distortion
 *           (THD).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryHDist(ViSession instrSession,
                                      ViReal64* harmonicDistortion_in_percent,
                                      ViReal64* harmonicDistortion_in_dB)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, harmonicDistortion_in_percent), 2, "Harmonic Distortion in percent");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, harmonicDistortion_in_dB), 3, "Harmonic Distortion in dB");

	checkErr(RsCore_QueryViString(instrSession, ":CALC:MARK:FUNC:HARM:DIST? TOT", response));
	sscanf(response, "%le,%le", harmonicDistortion_in_percent, harmonicDistortion_in_dB);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Harmonic Distortion Harmonics List
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryHDistHarmonicsList(ViSession instrSession,
                                                   ViReal64 harmonicsList[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, harmonicsList), 2, "Harmonic List");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "CALC:MARK:FUNC:HARM:LIST?", 255, harmonicsList, NULL));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Harmonic Distortion Bandwidth List
 * Purpose:  This function returns the resolution bandwidth values used by the
 *           harmonic measurment. A separate value for every harmonic frequency
 *           is automatically calculated. It is possible to query the calculated
 *           values with this function.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryHDistBandwidthList(ViSession instrSession,
                                                   ViInt32 arraySize,
                                                   ViReal64 resultValues[],
                                                   ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 1, INT_MAX),
		2, "Array Size");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "CALC:MARK:FUNC:HARM:BAND:LIST?", arraySize, resultValues, returnedValues));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_BurstPowerSequence(ViSession instrSession,
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
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	if (analyzerFrequencyHz < 0.0)
	viCheckParm(RS_ERROR_INVALID_PARAMETER, 3, "Analyzer Frequency [Hz]");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, resolutionBandwidthHz, 10.0, 10.0e+6),
		4, "Resolution Bandwidth [Hz]");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, measTimes, 1.0e-6, 30.0),
		5, "Meas Time [s]");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, triggerLevel, 0.0, 100.0),
		7, "Trigger Level");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, triggerOffsets, 125.0e-9, 100.0),
		8, "Trigger Offset [s]");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type_ofMeas, 0, 1),
		9, "Type Of Meas");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, noofMeasurements, 1, 501),
		10, "No Of Measurements");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:MPOW:SEQ %.12lG,%.12lG,%.12lG,%s,%.12lG,%.12lG,%s,%ld",
	         window, analyzerFrequencyHz, resolutionBandwidthHz, measTimes, rsspecan_rngTriggerSource.rangeValues[triggerSource].cmdString,
	         triggerLevel, triggerOffsets, measTypeArr[type_ofMeas], noofMeasurements);
	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Burst Power Filter Type
 * Purpose:  This function the filter type for the measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_BurstPowerFilterType(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 filterType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_MPOW_FTYPE, filterType),
		2, "Filter Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Burst Power Sequence
 * Purpose:  This function configures the instrument setup for multiple burst
 *           power measurement, starts a measurement sequence and reads the
 *           results. To reduce the setting time, the setup is performed
 *           simultaneously for all selected parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBurstPowerSequence(ViSession instrSession,
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
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	if (analyzerFrequencyHz < 0.0)
	viCheckParm(RS_ERROR_INVALID_PARAMETER, 3, "Analyzer Frequency [Hz]");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, resolutionBandwidthHz, 10.0, 10.0e+6), 4, "Resolution Bandwidth [Hz]");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, measTimes, 1.0e-6, 30.0), 5, "Meas Time [s]");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, triggerLevel, 0.0, 100.0), 7, "Trigger Level");
	viCheckParm(RsCore_InvalidViReal64Range(instrSession, triggerOffsets, 125.0e-9, 100.0), 8, "Trigger Offset [s]");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type_ofMeas, 0, 1),
		9, "Type Of Meas");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, no_ofMeasurements, 1, 501),
		10, "No Of Measurements");
	viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 11, "Timeout");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, burstPowerResults), 12, "Burst Power Results");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":SENS%ld:MPOW:SEQ? %.12lf,%.12lf,%.12lf,%s,%.12lf,%.12lf,%s,%ld",
	         window, analyzerFrequencyHz, resolutionBandwidthHz, measTimes, rsspecan_rngTriggerSource.rangeValues[triggerSource].cmdString,
	         triggerLevel, triggerOffsets, measTypeArr[type_ofMeas], no_ofMeasurements);
	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, cmd, timeout, no_ofMeasurements, burstPowerResults, NULL));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get Burst Power Results
 * Purpose:  This function configures harmonic distortion measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetBurstPowerResults(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 no_ofResults,
                                                ViReal64 burstPowerResults[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, no_ofResults, 1, INT_MAX),
		3, "No Of Results");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, burstPowerResults), 4, "burstPowerResults");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:MPOW:RES?", window);
	checkErr(RsCore_QueryFloatArrayToUserBufferWithOpc(instrSession, cmd, 0, no_ofResults, burstPowerResults, NULL));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_GetBurstPwrResultMin(ViSession instrSession,
                                                ViInt32 window,
                                                ViReal64* powerResultMin)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MPOW_MIN, powerResultMin),
		3, "Power Result Min");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
#pragma warning( pop )

/*****************************************************************************
 * Function: Query ViBoolean
 * Purpose:  This function queries the ViBoolean value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryViBoolean(ViSession instrSession,
                                          ViString command,
                                          ViBoolean* value)
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
ViStatus _VI_FUNC rsspecan_QueryViInt32(ViSession instrSession,
                                        ViString command,
                                        ViInt32* value)
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
ViStatus _VI_FUNC rsspecan_QueryViReal64(ViSession instrSession,
                                         ViString command,
                                         ViReal64* value)
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
ViStatus _VI_FUNC rsspecan_QueryViString(ViSession instrSession,
                                         ViString command,
                                         ViInt32 bufferSize,
                                         ViChar _VI_FAR value[])
{
	ViStatus error = VI_SUCCESS;
	ViStatus warning = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, (ViInt32)strlen(command), 1, 10000000), 2, "Command (null string length)");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, value), 3, "Value");

	checkErr(RsCore_QueryViStringUnknownLengthToUserBuffer(instrSession, command, bufferSize, value, NULL));
	checkErrKeepWarn(rsspecan_CheckStatus(instrSession));

Error:
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
ViStatus _VI_FUNC rsspecan_setStatusRegister(ViSession instrSession,
                                             ViInt32 registerOper, ViInt32 selStatusReg, ViInt32 enable,
                                             ViInt32 PTransition, ViInt32 NTransition)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, registerOper, 0, 2),
		2, "Register Oper");

	switch (registerOper)
	{
	case 0:
		checkErr(RsCore_Write(instrSession, ":STAT:PRES"));
		break;
	case 1:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, selStatusReg, 0, 23),
			3, "Questionable Register");

		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":STAT:QUES%s:ENAB %ld;PTR %ld;NTR %ld", selStatusRegArr[selStatusReg], enable, PTransition, NTransition);
		checkErr(RsCore_Write(instrSession, cmd));
		break;
	case 2:
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, enable, 0, 65535),
			4, "Enable");
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, PTransition, 0, 65535),
			5, "PTransition");
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, NTransition, 0, 65535),
			6, "NTransition");

		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":STAT:OPER:ENAB %ld;PTR %ld;NTR %ld", enable, PTransition, NTransition);
		checkErr(RsCore_Write(instrSession, cmd));
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Get Status Register                                             */
/* Purpose:  This function contains the commands for the querying status     */
/*           reporting system.                                               */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_getStatusRegister(ViSession instrSession,
                                             ViInt32 statusReg, ViPInt32 registerValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, statusReg, 0, 52),
		2, "Status Register");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":STAT:%s?", statusRegArr[statusReg]);
	checkErr(RsCore_QueryViInt32(instrSession, cmd, registerValue));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_setVSAModulationStatusRegister(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 registerOperation,
                                                          ViInt32 questionableRegister,
                                                          ViInt32 enable,
                                                          ViInt32 PTransition,
                                                          ViInt32 NTransition)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[40];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, registerOperation, 1, 1),
		3, "Register Operation");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, questionableRegister, 0, 6),
		4, "Questionable Register");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,RegEnable", window, VSAResultRegArr[questionableRegister]);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER, enable),
		5, "Enable");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,RegNTR", window, VSAResultRegArr[questionableRegister]);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER, PTransition),
		6, "PTransition");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,RegPTR", window, VSAResultRegArr[questionableRegister]);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER, NTransition),
		7, "NTransition");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_getVSAModulationStatusRegister(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 questionableRegister,
                                                          ViInt32 statusRegistersQuery,
                                                          ViInt32* registerValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[40];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, questionableRegister, 0, 6),
		4, "Questionable Register");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, statusRegistersQuery, 0, 1),
		4, "Status Registers Query");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, VSAResultRegArr[questionableRegister], RegisterPartArr[statusRegistersQuery]);
	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER, registerValue),
		5, "Register Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Condition Frequency Register                                    */
/* Purpose:  This function reads Condition Frequency Register.               */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionFrequencyRegister(ViSession instrSession,
                                                      ViPInt32 statusRegisterValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, statusRegisterValue), 2, "Status Register Value");

	checkErr(rsspecan_getStatusRegister(instrSession, 15, statusRegisterValue));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Condition Limit Register                                        */
/* Purpose:  This function reads Condition Limit Register.                   */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionLimitRegister(ViSession instrSession,
                                                  ViInt32 window,
                                                  ViPInt32 statusRegisterValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, statusRegisterValue), 3, "Status Register Value");

	switch (window)
	{
	case 1:
		checkErr(rsspecan_getStatusRegister(instrSession, 7, statusRegisterValue));
		break;
	case 2:
		checkErr(rsspecan_getStatusRegister(instrSession, 20, statusRegisterValue));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Condition Limit Margin Register                                 */
/* Purpose:  This function reads Condition Limit Margin Register.            */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionLimitMarginRegister(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViPInt32 statusRegisterValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, statusRegisterValue), 3, "Status Register Value");

	switch (window)
	{
	case 1:
		checkErr(rsspecan_getStatusRegister(instrSession, 9, statusRegisterValue));
		break;
	case 2:
		checkErr(rsspecan_getStatusRegister(instrSession, 22, statusRegisterValue));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Condition ACP Limit Register                                    */
/* Purpose:  This function reads Condition ACP Limit Register.               */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionACPLimitRegister(ViSession instrSession,
                                                     ViPInt32 statusRegisterValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, statusRegisterValue), 2, "Status Register Value");

	checkErr(rsspecan_getStatusRegister(instrSession, 13, statusRegisterValue));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Condition Power Register                                        */
/* Purpose:  This function reads Condition Power Register.                   */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionPowerRegister(ViSession instrSession,
                                                  ViPInt32 statusRegisterValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, statusRegisterValue), 2, "Status Register Value");

	checkErr(rsspecan_getStatusRegister(instrSession, 5, statusRegisterValue));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Condition Sync Register                                         */
/* Purpose:  This function reads Condition Sync Register.                    */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConditionSyncRegister(ViSession instrSession,
                                                 ViPInt32 statusRegisterValue)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, statusRegisterValue), 2, "Status Register Value");

	checkErr(rsspecan_getStatusRegister(instrSession, 11, statusRegisterValue));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*End of Status Register Subsystem*/

/*****************************************************************************
 * Function: rsspecan_close
 * Purpose:  This function closes the instrument.
 *
 *           Note:  This function must unlock the session before calling
 *           Rs_Dispose.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_close(ViSession instrSession)
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
ViStatus _VI_FUNC rsspecan_ConfigureSEState(ViSession instrSession,
                                            ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");

	if (state == VI_TRUE)
	{
		checkErr(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE, RSSPECAN_VAL_SWE_MODE_LIST));
	}
	else
		checkErr(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE, RSSPECAN_VAL_SWE_MODE_AUTO));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Peak Search
 * Purpose:     This function configures the peak search for spurious emission
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEPeakSearch(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViReal64 margin,
                                                 ViInt32 peaksPerRange)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_PEAK_SEARCH_MARGIN, margin),
		3, "Margin");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SE_PEAK_SEARCH_PEAKS, peaksPerRange),
		4, "Peaks Per Range");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Search Peak
 * Purpose:     This function searches the peaks in spurious emissions data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SESearchPeak(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "Win1", RSSPECAN_ATTR_SE_PEAK_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Start Stop
 * Purpose:     This function configures the start and stop frequency for the
 *              specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListStartStop(ViSession instrSession,
                                                         ViInt32 range,
                                                         ViReal64 startFrequency,
                                                         ViReal64 stopFrequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_START, startFrequency),
		3, "Start Frequency");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_STOP, stopFrequency),
		4, "Stop Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Filter Type
 * Purpose:     This function configures the filter type for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListFilterType(ViSession instrSession,
                                                          ViInt32 range,
                                                          ViInt32 filter)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_FILT_TYPE, filter),
		3, "Filter");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Bandwidth
 * Purpose:     This function configures the resolution and video bandwidth
 *              values for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListBandwidth(ViSession instrSession,
                                                         ViInt32 range,
                                                         ViReal64 resolutionBandwidth,
                                                         ViReal64 videoBandwidth)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_BAND, resolutionBandwidth),
		3, "Resolution Bandwidth");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_BAND_VID, videoBandwidth),
		4, "Video Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Time
 * Purpose:     This function configures the sweep time values for the
 *              specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepSweepListTime(ViSession instrSession,
                                                         ViInt32 range,
                                                         ViBoolean sweepTimeAuto,
                                                         ViReal64 sweepTime)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_SWE_TIME_AUTO, sweepTimeAuto),
		3, "Sweep Time Auto");
	if (sweepTimeAuto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_SWE_TIME, sweepTime),
			4, "Sweep Time");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Detector
 * Purpose:     This function configures the detector value for the specified
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListDetector(ViSession instrSession,
                                                        ViInt32 range,
                                                        ViInt32 detector)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_DET, detector),
		3, "Detector");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List Reference Level
 * Purpose:     This function configures the reference level for the specified
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListReferenceLevel(ViSession instrSession,
                                                              ViInt32 range,
                                                              ViReal64 refLevelValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_RLEV, refLevelValue),
		3, "Ref Level Value");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emisssions Sweep List RF Attenuation
 * Purpose:     This function configures the RF attenuation settings for the
 *              specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListRFAttenuation(ViSession instrSession,
                                                             ViInt32 range,
                                                             ViBoolean attenuatorAuto,
                                                             ViReal64 manualAttenuatorValuedB)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_INP_ATT_AUTO, attenuatorAuto),
		3, "Attenuator Auto");
	if (attenuatorAuto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_INP_ATT, manualAttenuatorValuedB),
			4, "Manual Attenuator Value dB");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Preamplifier
 * Purpose:     This function configures state of pre-amplifier for selected
 *              sweep range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListPreamplifier(ViSession instrSession,
                                                            ViInt32 range,
                                                            ViBoolean preamplifier)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "B23|B24|B25"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_INP_GAIN_STAT, preamplifier));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures level of pre-amplifier for selected sweep range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR range/Sets the configured range.
/// HIPAR preamplificationLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListPreamplifierLevel(ViSession instrSession,
                                                                 ViInt32 range,
                                                                 ViInt32 preamplificationLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_INP_GAIN_LEVEL, preamplificationLevel));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Sweep
 * Purpose:     This function configures the sweep settings for the specified
 *              range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListSweep(ViSession instrSession,
                                                     ViInt32 range,
                                                     ViInt32 sweepPoints,
                                                     ViBoolean stopAfterSweep)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_POINTS, sweepPoints),
		3, "Sweep Points");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_BREAK, stopAfterSweep),
		4, "Stop After Sweep");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Transducer Factor
 * Purpose:     This function configures a transducer for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListTransducerFactor(ViSession instrSession,
                                                                ViInt32 range,
                                                                ViString transducerFactor)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	if (strlen(transducerFactor) > 0)
	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_TRAN, transducerFactor),
		3, "Transducer Factor");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Configure Spurious Emissions Sweep List Limits
 * Purpose:     This function configures the limits for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListLimits(ViSession instrSession,
                                                      ViInt32 range,
                                                      ViBoolean limitChecking,
                                                      ViReal64 limit,
                                                      ViReal64 absoluteLimitStart,
                                                      ViReal64 absoluteLimitStop)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_STATE, limitChecking),
		3, "Limit Checking");
	if (limitChecking == VI_TRUE)
	{
		if (RsCore_IsInstrumentModel(instrSession, "FSL|FSP|FSQ|FSU") || rsspecan_IsFSV(instrSession))
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_START, absoluteLimitStart),
				5, "Absolute Limit Start");

			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_STOP, absoluteLimitStop),
				6, "Absolute Limit Stop");
		}
		else
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_LIMIT, limit),
				4, "Limit");
		}
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_SEStandardCatalog(ViSession instrSession,
                                             ViInt32 arraySize,
                                             ViInt32* numberOfStandards,
                                             ViChar catalogList[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* buf = NULL;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 0, INT_MAX),
		2, "Array Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, catalogList), 4, "Catalog List");

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "SENS:LIST:STAN:CAT?", &buf));
	checkErr(RsCore_ParseCatalog(buf, arraySize, catalogList, numberOfStandards));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (buf) free(buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Standard Save
 * Purpose:     This function stores the current spurious emission measurement
 *              configuration to a file on the R&S FSQ hard disk. The file
 *              format is *.xml. The default path is "D:\R_S\INSTR\SPURS_STD".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardSave(ViSession instrSession,
                                          ViString standardSave)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SE_STANDARD_SAVE, standardSave),
		2, "Standard Save");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Standard Load
 * Purpose:     This function loads the current spurious emission measurement
 *              configuration to a file on the R&S FSQ hard disk. The file
 *              format is *.xml. The default path is "D:\R_S\INSTR\SPURS_STD".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardLoad(ViSession instrSession,
                                          ViString standardLoad)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SE_STANDARD_LOAD, standardLoad),
		2, "Standard Load");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Standard Delete
 * Purpose:     This function deletes a file that contains a spurious emission
 *              measurement configuration. The default path to the file is
 *              "D:\R_S\INSTR\SPURS_STD". The file name has to be specified
 *              without the *.xml file extension.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEStandardDelete(ViSession instrSession,
                                            ViString standardDelete)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SE_STANDARD_DELETE, standardDelete),
		2, "Standard Delete");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Spurious Emissions Delete Range
 * Purpose:     This function deletes a range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEDeleteRange(ViSession instrSession,
                                         ViInt32 range)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_SE_LIST_RANG_DEL, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function:    Get Spurious Emissions Number Of List Ranges
 * Purpose:     This function returns the number of defined ranges of the spurious
 *              measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSENumberOfListRanges(ViSession instrSession,
                                                   ViInt32* numberOfRanges)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SE_LIST_RANG_COUNT, numberOfRanges),
		2, "Number Of Ranges");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Store Spurious Emissions to File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSEToFile(ViSession instrSession,
                                         ViString fileName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SE_STORE_FILE, fileName),
		3, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read Spurious Peak List
 * Purpose:  This function returns the spurious emissions peak list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEPeakList(ViSession instrSession,
                                          ViInt32 noOfValues,
                                          ViReal64 frequency[],
                                          ViReal64 level[],
                                          ViReal64 deltaLimit[],
                                          ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize = 0;
	ViInt32 i;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));
	checkErr(rsspecan_dataReadTraceDynSize(instrSession, 1, "SPUR", &data, &dataSize));
	dataSize /= 3;
	if (returnedValues)
		*returnedValues = dataSize;

	for (i = 0; i < dataSize; i++)
	{
		frequency[i] = data[i * 3];
		level[i] = data[i * 3 + 1];
		deltaLimit[i] = data[i * 3 + 2];
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Spurious Emissions List Evaluation
 * Purpose:  This function activates or deactivates spurious emission
 *           measurement LIST EVALUATION.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEListEvaluation(ViSession instrSession,
                                            ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SE_LIST_EVAL_STATE, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read Spurious Emissions List Evaluation
 * Purpose:  This function returns the resulting peak list of the spurious
 *           emission measurement LIST EVALUATION.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEListEvaluation(ViSession instrSession,
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
                                                ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize = 0;
	ViInt32 i, j = 0;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, rangeNumber), 2, "Range number");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, startFrequency), 3, "Start Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, stopFrequency), 4, "Stop Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, resolutionBandwidth), 5, "Resolution Bandwidth");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, frequency), 6, "Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, levelAbs), 7, "Level Abs");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, levelRel), 8, "Level Rel");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, delta), 9, "Delta");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, limitCheck), 10, "Limit Check");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, reserved1), 11, "Reserved 1");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, reserved2), 12, "Reserved 2");

	checkErr(RsCore_QueryFloatArray(instrSession, "TRAC:DATA? LIST", &data, &dataSize));

	dataSize /= 11;
	if (returnedValues)
		*returnedValues = dataSize;

	if (dataSize > noOfValues)
		dataSize = noOfValues;

	for (i = 0; i < dataSize; i++)
	{
		rangeNumber[i] = data[j++];
		startFrequency[i] = data[j++];
		stopFrequency[i] = data[j++];
		resolutionBandwidth[i] = data[j++];
		frequency[i] = data[j++];
		levelAbs[i] = data[j++];
		levelRel[i] = data[j++];
		delta[i] = data[j++];
		limitCheck[i] = data[j++];
		reserved1[i] = data[j++];
		reserved2[i] = data[j++];
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM State
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMState(ViSession instrSession,
                                             ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViBooleanRange(instrSession, state), 3, "State");

	if (state == VI_TRUE)
	{
		checkErr(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE, RSSPECAN_VAL_SWE_MODE_ESP));
	}
	else
	{
		checkErr(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_SWEEP_MODE, RSSPECAN_VAL_SWE_MODE_AUTO));
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM
 * Purpose:  This function configures the Spectrum EM Mask Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEM(ViSession instrSession,
                                        ViInt32 limitLineType,
                                        ViReal64 limitLineValue,
                                        ViReal64 _30kHz1MHzTransition)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_ESP_MODE, limitLineType),
		2, "Limit Line Type");

	if (limitLineType == RSSPECAN_VAL_ESP_MODE_MAN)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ESP_MAN_VAL, limitLineValue),
			3, "Limit Line Value");
	}

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ESP_TRAN, _30kHz1MHzTransition),
		4, "30kHz/1MHz Transition");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Peak Search
 * Purpose:  This function configures the peak search for spectrum emission
 *           mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPeakSearch(ViSession instrSession,
                                                  ViReal64 margin,
                                                  ViInt32 peaksPerRange)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SEM_PEAK_SEARCH_MARGIN, margin),
		2, "Margin");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_PEAK_SEARCH_PEAKS, peaksPerRange),
		3, "Peaks Per Range");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Restore SEM Standard Lines
 * Purpose:  This function restores the standard limit lines for the spectrum
 *           emission mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreSEMStandardLines(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_ESP_RESTORE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Search Peak
 * Purpose:  This function searches the peaks in spectrum emission data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMSearchPeak(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "C1", RSSPECAN_ATTR_ESP_PEAK_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Store SEM To File
 * Purpose:  This function stores the peak list of selected trace in ASCII
 *           format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSEMToFile(ViSession instrSession,
                                          ViString fileName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LIM_STORE_FILE, fileName),
		2, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read SEM Peak List
 * Purpose:  This function returns the Spectrum EM Mask View Peak List.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEMPeakList(ViSession instrSession,
                                           ViInt32 window,
                                           ViInt32 finalMeasurement,
                                           ViInt32 noOfValues,
                                           ViReal64 frequency[],
                                           ViReal64 level[],
                                           ViReal64 deltaLevel[],
                                           ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 arrayLength;
	ViReal64* data = NULL;
	ViInt32 retVal;
	ViInt32 i;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	arrayLength = 3 * noOfValues;
	data = malloc(sizeof(ViReal64) * arrayLength);

	sprintf(traceName, "FINal%ld", finalMeasurement);

	checkErr(rsspecan_dataReadTrace (instrSession, window, traceName, arrayLength,
		data, &retVal));

	if (returnedValues)
		*returnedValues = retVal / 3; // Three result arrays

	retVal = (retVal > arrayLength) ? arrayLength : retVal;

	retVal /= 3;

	for (i = 0; i < retVal; i++)
	{
		frequency[i] = data[i * 3];
		level[i] = data[i * 3 + 1];
		deltaLevel[i] = data[i * 3 + 2];
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get SEM Value
 * Purpose:  This function returns the X/Y -value at the maximum overstepping
 *           of the spectrum emission mask limits.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSEMValue(ViSession instrSession,
                                       ViInt32 selection,
                                       ViReal64* value)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, selection, 0, 1),
		2, "Selection");
	switch (selection)
	{
	case RSSPECAN_VAL_X:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ESP_QUERY_X, value),
			3, "Value");

		break;
	case RSSPECAN_VAL_Y:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_ESP_QUERY_X, value),
			3, "Value");

		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Spectrum Emissions Mask List Evaluation
 * Purpose:  This function activates or deactivates spectrum emission mask
 *           measurement LIST EVALUATION.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMListEvaluation(ViSession instrSession,
                                             ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEM_LIST_EVAL_STATE, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Read SEM List Evaluation Results
 * Purpose:  This function returns the rreturns the list evaluation results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSEMListEvaluationResults(ViSession instrSession,
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
                                                        ViInt32* returnedValues)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize;
	ViInt32 i, j = 0;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "!FMU"));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, rangeNumber), 2, "Range number");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, startFrequency), 3, "Start Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, stopFrequency), 4, "Stop Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, resolutionBandwidth), 5, "Resolution Bandwidth");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, frequency), 6, "Frequency");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, powerAbs), 7, "Power Abs");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, powerRel), 8, "Power Rel");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, delta), 9, "Delta");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, limitCheck), 10, "Limit Check");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, reserved1), 11, "Reserved 1");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, reserved2), 12, "Reserved 2");

	checkErr(RsCore_QueryFloatArray(instrSession, "TRAC:DATA? LIST", &data, &dataSize));

	dataSize /= 11;

	if (returnedValues)
		*returnedValues = dataSize;

	if (dataSize > noOfValues)
		dataSize = noOfValues;

	for (i = 0; i < dataSize; i++)
	{
		rangeNumber[i] = data[j++];
		startFrequency[i] = data[j++];
		stopFrequency[i] = data[j++];
		resolutionBandwidth[i] = data[j++];
		frequency[i] = data[j++];
		powerAbs[i] = data[j++];
		powerRel[i] = data[j++];
		delta[i] = data[j++];
		limitCheck[i] = data[j++];
		reserved1[i] = data[j++];
		reserved2[i] = data[j++];
	}

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Mark All Peaks
 * Purpose:  This function marks all peaks with blue squares in the diagram.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMMarkAllPeaks(ViSession instrSession,
                                           ViBoolean state)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEM_MARK_ALL_PEAKS, state),
		2, "State");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Reference Range
 * Purpose:  This function configures the reference range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMReferenceRange(ViSession instrSession,
                                                      ViInt32 powerReferenceType,
                                                      ViReal64 txBandwidth,
                                                      ViBoolean RRCFilterState,
                                                      ViReal64 RRCFilterAlpha)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_POWER_REF_TYPE, powerReferenceType),
		2, "Power Reference Type");

	if (powerReferenceType == RSSPECAN_VAL_SEM_REF_TYPE_CPOW)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SEM_BANDWIDTH, txBandwidth),
			3, "TX Bandwidth");

		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEM_RRC_FILTER_STATE, RRCFilterState),
			4, "RRC Filter State");

		if (RRCFilterState == VI_TRUE)
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SEM_RRC_FILTER_ALPHA_VALUE, RRCFilterAlpha),
			5, "RRC Filter Alpha");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query SEM Reference Range Position
 * Purpose:  This function returns the current position (number) of the
 *           reference range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySEMReferenceRangePosition(ViSession instrSession,
                                                          ViInt32* referenceRangePosition)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_REF_RANGE_POSITION, referenceRangePosition),
		2, "Reference Range Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 bandCategory,
	ViReal64 baseStationRFBandwidth,
	ViBoolean gsm_edgePresent,
	ViBoolean lteFDD1_4MHz_3MHzPresent
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_MSR_BAND_CATEGORY, bandCategory),
		2, "Band Category");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SEM_MSR_RF_BANDWIDTH, baseStationRFBandwidth),
		3, "Base Station RF Bandwidth");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEM_MSR_GSM_PRESENT, gsm_edgePresent),
		4, "Gsm Edge Present");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEM_MSR_LTE_PRESENT, lteFDD1_4MHz_3MHzPresent),
		5, "LTE FDD 1.4MHz 3MHz Present");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  This function configures the SEM sweep list according to the MSR
/// HIFN  settings defined by function rsspecan_ConfigureSEMMSRSettings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_ApplySEMMSRSettings(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_MSR_APPLY, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Configure Fast SEM
 * Purpose:  This function activates Fast SEM mode to accelerate spectrum
 *           emission mask measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureFastSEM(ViSession instrSession,
                                            ViBoolean fastSEM)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SEM_FAST_SEM, fastSEM),
		2, "Fast SEM");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Preset Standard
 * Purpose:  This function selects the specified XML file under
 *           C:\r_s\instr\sem_std. If the file is stored in a subdirectory,
 *           include the relative path.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPresetStandard(ViSession instrSession,
                                                      ViString presetStandard)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_PRESET_STANDARD, presetStandard),
		2, "Preset Standard");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Standard Save
 * Purpose:  This function stores the current spectrum emission mask
 *           configuration to a file in the R&S FSQ memory.  The file format
 *           is *.xml. The default path is "D:\R_S\INSTR\SPURS_STD".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMStandardSave(ViSession instrSession,
                                           ViString saveStandard)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_PRESET_STANDARD_SAVE, saveStandard),
		2, "Save Standard");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Standard Delete
 * Purpose:  This function deletes a file that contains a spectrum emission
 *           mask measurement configuration. The default path to the file is
 *           "D:\R_S\INSTR\SPURS_STD". The file name has to be specified
 *           without the *.xml file extension.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMStandardDelete(ViSession instrSession,
                                             ViString deleteStandard)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_PRESET_STANDARD_DELETE, deleteStandard),
		2, "Delete Standard");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Restore SEM Standard Files
 * Purpose:  This function copies the XML files from the C:\R_S\instr\sem_backup
 *           folder to the C:\R_S\instr\sem_std folder. Files of the same name
 *           are overwritten.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_RestoreSEMStandardFiles(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_RESTORE_STANDARD, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Store SEM Preset Settings
 * Purpose:  This function stores the current settings as presettings in the
 *           specified XML file under C:\r_s\instr\sem_backup.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSEMPresetSettings(ViSession instrSession,
                                                  ViString settingsFileName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_STORE_PRESET, settingsFileName),
		2, "Settings File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Save List To File
 * Purpose:  This function stores the current list evaluation results in a *.dat
 *           file. The file consists of a data section containing the list
 *           evaluation results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMSaveListToFile(ViSession instrSession,
                                             ViString fileName)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SEM_LIST_FILE_EXPORT, fileName),
		2, "File Name");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeStartStop(ViSession instrSession,
                                                      ViInt32 range,
                                                      ViReal64 rangeStart,
                                                      ViReal64 rangeStop)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_FREQ_START, rangeStart),
		3, "Range Start");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_FREQ_STOP, rangeStop),
		4, "Range Stop");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Filter Type
 * Purpose:  This function configures the filter type for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeFilterType(ViSession instrSession,
                                                       ViInt32 range,
                                                       ViInt32 filterType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_FILTER_TYPE, filterType),
		3, "Filter Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Bandwidth
 * Purpose:  This function configures the resolution and video bandwidth values
 *           for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeBandwidth(ViSession instrSession,
                                                      ViInt32 range,
                                                      ViReal64 resolutionBandwidth,
                                                      ViReal64 videoBandwidth)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_BANDWIDTH, resolutionBandwidth),
		3, "Resolution Bandwidth");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_VBW, videoBandwidth),
		4, "Video Bandwidth");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Sweep Time
 * Purpose:  This function configures the sweep time values for the specified
 *           range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeSweepTime(ViSession instrSession,
                                                      ViInt32 range,
                                                      ViBoolean sweepTimeMode,
                                                      ViReal64 sweepTime)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_SWEEP_TIME_AUTO, sweepTimeMode),
		3, "Sweep Time Mode");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_SWEEP_TIME, sweepTime),
		4, "Sweep Time");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Reference Level
 * Purpose:  This function configures the reference level for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeReferenceLevel(ViSession instrSession,
                                                           ViInt32 range,
                                                           ViReal64 referenceLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_REF_LEVEL, referenceLevel),
		3, "Reference Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Range RF Attenuation
 * Purpose:  This function configures the RF attenuation setting for the
 *           specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRFAttenuation(ViSession instrSession,
                                                          ViInt32 range,
                                                          ViBoolean RFAttenuationMode,
                                                          ViReal64 RFAttenuator,
                                                          ViBoolean preamplifiier)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_INPUT_ATTENUATION_AUTO, RFAttenuationMode),
		3, "RF Attenuation Mode");
	if (RFAttenuationMode == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_INPUT_ATTENUATION, RFAttenuator),
			4, "RF Attenuator");
	}
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_PREAMPLIFIER, preamplifiier),
		5, "Preamplifiier");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function configures level of pre-amplifier for selected sweep range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR range/Sets the configured range.
/// HIPAR preamplificationLevel/Selects the preamplification level.
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRFPreamplifierLevel(ViSession instrSession,
                                                                ViInt32 range,
                                                                ViInt32 preamplificationLevel)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_PREAMPLIFIER_LEVEL, preamplificationLevel),
		3, "Preamplification Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeTransducerFactor(ViSession instrSession,
                                                             ViInt32 range,
                                                             ViString transducerFactor)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_TRANSDUCER, transducerFactor),
		3, "Transducer Factor");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Range Limits
 * Purpose:  This function configures the limit setting for the specified range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeLimits(ViSession instrSession,
                                                   ViInt32 range,
                                                   ViInt32 limitCheckType,
                                                   ViReal64 absoluteLimitStart,
                                                   ViReal64 absoluteLimitStop,
                                                   ViReal64 relativeLimitStart,
                                                   ViReal64 relativeLimitStop)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_RANGE_LIMIT_CHECK_TYPE, limitCheckType),
		3, "Limit Check Type");

	switch (limitCheckType)
	{
	case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_ABS:
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_ABS_LIMIT, absoluteLimitStart),
				4, "Absolute Limit Start");

			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_ABS_LIMIT, absoluteLimitStop),
				5, "Absolute Limit Stop");
			break;
		}
	case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_REL:
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT, relativeLimitStart),
				6, "Relative Limit Start");

			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT, relativeLimitStop),
				7, "Relative Limit Stop");
			break;
		}
	case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_AND:
	case RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_OR:
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_ABS_LIMIT, absoluteLimitStart),
				4, "Absolute Limit Start");

			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_ABS_LIMIT, absoluteLimitStop),
				5, "Absolute Limit Stop");

			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT, relativeLimitStart),
				6, "Relative Limit Start");

			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT, relativeLimitStop),
				7, "Relative Limit Stop");
			break;
		}
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, limitCheckType), 4, "Type");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 range,
	ViInt32 powerClass,
	ViInt32 startFunction,
	ViReal64 absoluteLimitStart,
	ViInt32 stopFunction,
	ViReal64 absoluteLimitStop
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap_power_class[RS_REPCAP_BUF_SIZE] = "";
	ViChar repCap[RS_REPCAP_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, powerClass, 1, 4),
		3, "Power Class");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM,
			"Range", range - 1, RS_REPCAP_BUF_SIZE, repCap),
		2, "Range");
	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM,
			"PClass", powerClass - 1, RS_REPCAP_BUF_SIZE, repCap_power_class),
		3, "Power Class");

	strcat(repCap, ",");
	strcat(repCap, repCap_power_class);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_FUNCTION, startFunction),
		4, "Start Function");

	if (startFunction == RSSPECAN_VAL_SEM_REL_LIMIT_FUNC_MAX)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM, absoluteLimitStart),
			5, "Absolute Limit Start");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT_FUNCTION, stopFunction),
		6, "Stop Function");

	if (startFunction == RSSPECAN_VAL_SEM_REL_LIMIT_FUNC_MAX)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT_MAXIMUM, absoluteLimitStop),
			7, "Absolute Limit Stop");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Get SEM Range Count
 * Purpose:  This function returns the number of defined ranges.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSEMRangeCount(ViSession instrSession,
                                            ViInt32* rangeCount)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_RANGE_COUNT, rangeCount),
		2, "Range Count");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Insert New Range
 * Purpose:  This function inserts a new range before or after the specified
 *           range. The range numbers are updated accordingly.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMInsertNewRange(ViSession instrSession,
                                             ViInt32 range, ViInt32 insertRange)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SEM_INSERT_NEW_RANGE, insertRange),
		3, "Insert Range");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: SEM Delete Range
 * Purpose:  This function deletes the specified range. The range numbers are
 *           updated accordingly. The reference range cannot be deleted.
 *           A minimum of three ranges is mandatory.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SEMDeleteRange(ViSession instrSession,
                                          ViInt32 range)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, range, 1, 20),
		2, "Range");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "R%ld", range);

	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_SEM_DELETE_RANGE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Used Power Classes
 * Purpose:  This function sets the power classes used in the spectrum emission
 *           mask measurement. It is only possible to use power classes for
 *           which limits are defined. Also, either only one power class at a
 *           time or all power classes together can be selected.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMUsedPowerClasses(ViSession instrSession,
                                                        ViInt32 powerClassNumber,
                                                        ViInt32 usedPowerClass)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, powerClassNumber, 1, 4),
		2, "Power Class Number");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "PC%ld", powerClassNumber);

	if (usedPowerClass == RSSPECAN_VAL_ALL)
	{
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "PC1", RSSPECAN_ATTR_SEM_POWER_CLASS_USED, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "PC2", RSSPECAN_ATTR_SEM_POWER_CLASS_USED, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "PC3", RSSPECAN_ATTR_SEM_POWER_CLASS_USED, VI_FALSE));
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, "PC4", RSSPECAN_ATTR_SEM_POWER_CLASS_USED, VI_FALSE));
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SEM_POWER_CLASS_USED,
			(ViBoolean) usedPowerClass), 3, "Used Power Class");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Power Class Count
 * Purpose:  This function sets the number of power classes to be defined.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassCount(ViSession instrSession,
                                                       ViInt32 count)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SEM_POWER_CLASS_COUNT, count),
		3, "Count");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Power Class Limits
 * Purpose:  This function configures the power class settings of the Spectrum
 *           Emission Mask measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassLimits(ViSession instrSession,
                                                        ViInt32 powerClassNumber,
                                                        ViInt32 limitState,
                                                        ViReal64 lowerLevelLimit,
                                                        ViReal64 upperLevelLimit)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, powerClassNumber, 1, 4),
		2, "Power Class Number");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "PC%ld", powerClassNumber);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SEM_POWER_CLASS_LIMIT_STATE, limitState),
		3, "Limit State");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_POWER_CLASS_LOWER_LEVEL_LIMIT, lowerLevelLimit),
		4, "Lower Level Limit");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SEM_POWER_CLASS_UPPER_LEVEL_LIMIT, upperLevelLimit),
		5, "Upper Level Limit");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure SEM Power Class All Limits
 * Purpose:  This function sets up to 4 power classes in one step.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassAllLimits(ViSession instrSession,
                                                           ViInt32 numberOfLimits,
                                                           ViReal64 limits[])
{
	ViStatus error = VI_SUCCESS;
	ViChar* pbuffer;
	ViChar* p2buf;
	ViInt32 i = 0;

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession) || (!RsCore_IsInstrumentModel(instrSession, "FSL")))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	pbuffer = (ViChar*)malloc(150 * numberOfLimits + 30);

	p2buf = pbuffer + sprintf(pbuffer, "CALC:LIM:ESP:LIM ");

	for (i = 0; i < numberOfLimits; i++)
		p2buf += sprintf(p2buf, "%lf,", limits[i]);

	*--p2buf = '\0';

	checkErr(RsCore_Write(instrSession, pbuffer));

	free(pbuffer);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query SEM Power Class All Limits
 * Purpose:  This function returns up to 4 power classes in one step.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QuerySEMPowerClassAllLimits(ViSession instrSession,
                                                       ViReal64 limits[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession) || (!RsCore_IsInstrumentModel(instrSession, "FSL")))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	checkErr(RsCore_QueryFloatArrayToUserBuffer (instrSession, "CALC:LIM:ESP:LIM?", 1001, limits, NULL));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function selects the MSRA mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_MSRAMode(
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MSRA_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 window,
	ViReal64* intervalStart,
	ViReal64* intervalStop
)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* p2buff;

	checkErr(RsCore_LockSession(instrSession));

	sprintf(cmd, "CALC:MSRA:WIND%ld:IVAL?", window);

	checkErr(rsspecan_QueryViString (instrSession, cmd, RS_MAX_MESSAGE_BUF_SIZE, response));

	p2buff = strtok(response, ",");
	if (p2buff != NULL)
		*intervalStart = atof(p2buff);

	p2buff = strtok(NULL, ",");
	if (p2buff != NULL)
		*intervalStop = atof(p2buff);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_QueryMSRAAllAnalysisInterval(ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 arraySize,
                                                        ViReal64 intervalStart[],
                                                        ViReal64 intervalStop[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViReal64* data = NULL;
	ViInt32 dataSize = 0, i;
	ViInt32 j = 0;

	checkErr(RsCore_LockSession(instrSession));

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC:MSRA:WIND%ld:MIV?", window);
	checkErr(RsCore_QueryFloatArray(instrSession, cmd, &data, &dataSize));

	dataSize /= 2;

	if (dataSize > arraySize)
		dataSize = arraySize;

	for (i = 0; i < dataSize; i++)
	{
		intervalStart[i] = data[j++];
		intervalStop[i] = data[j++];
	}

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_MSRA_REFRESH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 offset
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MSRA_CAPTURE_OFFSET, offset),
		2, "Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function:    Receiver Search Peak
 * Purpose:     This function searches the peaks in spurious emissions data.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReceiverPeakSearch(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "Win1", RSSPECAN_ATTR_SE_PEAK_SEARCH, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function shows/hides analysis line position and sets line position.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR showLine/This control hides or displays the analysis line in the time-based MSRA windows.
/// HIPAR position/This control defines the position of the analysis line for all
/// HIPAR position/time-based windows in all MSRA applications and the MSRA Master.
ViStatus _VI_FUNC rsspecan_ConfigureMSRAAnalysisLine(ViSession instrSession,
                                                     ViBoolean showLine,
                                                     ViReal64 position)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MSRA_SHOW_ANALYSIS_LINE, showLine),
		2, "Show Line");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MSRA_ANALYSIS_LINE_POSITION, position),
		2, "Position");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureMSRADataEvaluation(ViSession instrSession,
                                                       ViInt32 dataEvaluation)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MSRA_DATA_EVALUATION, dataEvaluation),
		2, "Data Evaluation");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*
 *= RsSpecAn Service Group Functions ========================================*
 *===========================================================================*/
/*****************************************************************************
 * Function: Service Configure Input
 * Purpose:  This function configures the input for servising purposes.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureInput(ViSession instrSession,
                                                 ViInt32 window,
                                                 ViInt32 input, ViReal64 level)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_SOURCE, input),
		3, "Input");
	if ((input == RSSPECAN_VAL_INPUT_CAL) && !(RsCore_IsInstrumentModel(instrSession, "FSL") || (rsspecan_IsFSV(instrSession))))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_LEVEL, level),
			4, "Level");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Input
 * Purpose:  This function configures pulsed calibration signal. This function
 *           takes effect only if the RF input has been set to the internal
 *           reference signal using rsspecan_ServiceConfigureInput
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigurePulsedInput(ViSession instrSession,
                                                       ViInt32 window,
                                                       ViBoolean state,
                                                       ViReal64 frequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_PULSED_STATE, state),
		3, "State");

	if ((rsspecan_IsFSV(instrSession)) || RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SERVICE_PULSE_CAL_FREQ_FSV, frequency),
			4, "Frequency");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_PULSED_RATE, frequency),
			4, "Frequency");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Input
 * Purpose:  This function configures rectangle calibration signal. This function
 *           takes effect only if the RF input has been set to the internal
 *           reference signal using rsspecan_ServiceConfigureInput.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureRectangleInput(ViSession instrSession,
                                                          ViInt32 window,
                                                          ViBoolean state,
                                                          ViReal64 frequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_RECTANGLE_STATE, state),
		3, "State");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_RECTANGLE_RATE, frequency),
		4, "Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Baseband Source
 * Purpose:  This function selects the baseband signal source.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureBasebandSource(ViSession instrSession,
                                                          ViInt32 source)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SERVICE_BASEBAND_SIGNAL_SOURCE, source),
		2, "Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Pulsed IQ Calibration Signal
 * Purpose:  This function configures calibration signals. This function takes
 *           effect only if the RF input has been set to the internal reference
 *           signal using rsspecan_ServiceConfigureInput.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigurePulsedIQCalibrationSignal(ViSession instrSession,
                                                                     ViInt32 destination,
                                                                     ViReal64 frequency)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SERVICE_PULSE_CAL_DESTINATION, destination),
		2, "Destination");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SERVICE_PULSE_CAL_FREQ, frequency),
		3, "Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Pulsed IQ Calibration Signal
 * Purpose:  This function selects the voltage for the DC Cal signal in Volt.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureDCIQCalibrationSignal(ViSession instrSession,
                                                                 ViReal64 voltage)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SERVICE_DC_CAL_VOLTAGE, voltage),
		2, "Voltage");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Noise
 * Purpose:  This function switches the 28-V supply of the noise source at
 *           the rear panel on or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureNoise(ViSession instrSession,
                                                 ViBoolean noise)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SERVICE_NOISE_SOURCE, noise),
		2, "Noise");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure YIG Filter
 * Purpose:  This function witches the YIG filter for image rejection into
 *           the signal path (ON) or removes it from the signal path (OFF).
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureYIGFilter(ViSession instrSession,
                                              ViInt32 window,
                                              ViBoolean YIGFilter)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_AMPL_YIG_FILTER, YIGFilter),
		3, "YIG Filter");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN This function turns the highpass filter at the RF input on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR highpassFilter/Turns the highpass filter at the RF input on and off.
ViStatus _VI_FUNC rsspecan_ConfigureHighpassFilter(ViSession instrSession,
                                                   ViBoolean highpassFilter)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER, highpassFilter),
		3, "Highpass Filter");

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_InputProtectionReset(ViSession instrSession,
                                                ViInt32 window)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);
	checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_AMPL_PROTECTION_RESET, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Configure Service Comb Generator
 * Purpose:  This function configures the comb generator. This function takes
 *           effect only if the internal reference signal is set using
 *           rsspecan_ServiceConfigureInput.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceConfigureCombGenerator(ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 combFrequency)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSL"));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SERVICE_INPUT_COMB_FREQUENCY, combFrequency),
		3, "Frequency");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Service HW Info
 * Purpose:  This function queries the contents of the module info table.
 *           Table lines are output as string data and are separated by commas.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ServiceHWInfo(ViSession instrSession,
                                         ViInt32 arraySize,
                                         ViChar HWInfo[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	//viCheckParm(rsspecan_GetAttributeViString (instrSession, "",
	//            RSSPECAN_ATTR_SERVICE_HW_INFO, arraySize, HWInfo), 2, "HW Info");
	checkErr(rsspecan_QueryViString(instrSession, "DIAG:SERV:HWIN?", arraySize, HWInfo));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Network Map Drive
 * Purpose:  This function maps a drive to a server or server folder of the
 *           network. As a prerequisite in Microsoft networks, sharing of
 *           this server or server folder must be enabled.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkMapDrive(ViSession instrSession,
                                           ViString driveLetter,
                                           ViString hostNameIPAddress,
                                           ViBoolean useOptionalParameters,
                                           ViString userName, ViString password,
                                           ViBoolean reconnectAtLogon)
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* p2buf = cmd;

	checkErr(RsCore_LockSession(instrSession));

	if (!RsCore_IsInstrumentModel(instrSession, "FSL") && !rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	p2buf += sprintf(p2buf, "MMEM:NETW:MAP '%s','%s'", driveLetter, hostNameIPAddress);

	if (useOptionalParameters == VI_TRUE)
	{
		p2buf += sprintf(p2buf, ",'%s','%s',%s", userName, password, switchArr[reconnectAtLogon]);
	}

	checkErr(RsCore_Write(instrSession, cmd));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	if (p2buf) free(p2buf);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Network Disconnect Drive
 * Purpose:  This function disconnects the selected drive.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkDisconnectDrive(ViSession instrSession,
                                                  ViString driveLetter)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NETWORK_DISCONNECT_DRIVE, driveLetter),
		2, "Drive Letter");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Network Unused Drives List
 * Purpose:  This function lists all unused network drive names.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkUnusedDrivesList(ViSession instrSession,
                                                   ViInt32 arraySize, ViChar unusedDrives[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_UNUSED_NETWORK_DRIVES,
			arraySize, unusedDrives),
		3, "Unused Drives");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Network Used Drives List
 * Purpose:  This function lists all mapped network drives.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkUsedDrivesList(ViSession instrSession,
                                                 ViInt32 arraySize, ViChar usedDrives[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViString(instrSession, "", RSSPECAN_ATTR_USED_NETWORK_DRIVES,
			arraySize, usedDrives),
		3, "Used Drives");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Network Show Folder Info
 * Purpose:  This function sets if the folder info is showed in the list.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NetworkShowFolderInfo(ViSession instrSession,
                                                 ViBoolean showFolderInfo)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_SHOW_NETWORK_FOLDER_INFO, showFolderInfo),
		2, "Show Folder Info");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Reset
 * Purpose:  This function resets the instrument.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_reset(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_Write(instrSession, "*RST"));
	checkErr(RsCore_QueryViInt32(instrSession, "*OPC?", NULL));

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
    ViSession   vi      = RsCore_ViSession(instrSession);

    checkErr(Rs_LockSession (vi, NULL));
    checkErr(rsspecan_reset(instrSession));
    checkErr(Rs_ApplyDefaultSetup(vi));

Error:
    (void) Rs_UnlockSession(vi, NULL);
    return error;
}
*/
/****************************************************************************
 *  Function: Disable
 *  Purpose:  This function places the instrument in a quiescent state as
 *            quickly as possible.
 ****************************************************************************/
ViStatus _VI_FUNC rsspecan_Disable(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_Simulating(instrSession) == 0)
	{
		checkErr(RS_ERROR_FUNCTION_NOT_SUPPORTED);
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: Self Test
 * Purpose:  This function executes the instrument self-test and returns the
 *           result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_self_test(ViSession instrSession, ViInt16* testResult,
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
ViStatus _VI_FUNC rsspecan_SelfTestResults(ViSession instrSession,
                                           ViInt32 bufferSize,
                                           ViChar results[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_QueryViString(instrSession, "DIAG:SERV:STES:RES?", bufferSize, results));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Revision Query
 * Purpose:  This function returns the driver and instrument revisions.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_revision_query(ViSession instrSession, ViChar driverRev[],
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
ViStatus _VI_FUNC rsspecan_setCheckStatus(ViSession instrSession,
                                          ViBoolean statusChecking)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_SetAttributeViBoolean (instrSession, "", RS_ATTR_QUERY_INSTRUMENT_STATUS, 0, statusChecking));

Error:
	return error;
}

/*****************************************************************************
 * Function: Option Checking
 * Purpose:  This function switches ON/OFF option checking of the instrument
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_setCheckOption(ViSession instrSession,
                                          ViBoolean state)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	rsSession->optionChecking = state;

Error:
	return error;
}

/// HIFN This function switches ON/OFF various error checkings performed inside of the driver.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR optionChecking/This control switches option checking On or Off.
/// HIPAR rangeChecking/This control enables or disables range checking.
/// HIPAR statusChecking/This control enables or disables instrument state checking
ViStatus _VI_FUNC rsspecan_ConfigureErrorChecking(ViSession instrSession,
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
ViStatus _VI_FUNC rsspecan_MemorySize(ViSession instrSession,
                                      ViInt32* memorySize)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSL") || (rsspecan_IsFSV(instrSession)))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidNullPointer(instrSession, memorySize), 2, "Memory Size");

	checkErr(rsspecan_QueryViInt32(instrSession, "SYST:MSIZe? MBO", memorySize));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Error Query
 * Purpose:  This function queries the instrument error queue and returns
 *           the result.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_error_query(ViSession instrSession, ViInt32* errCode,
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
ViStatus _VI_FUNC rsspecan_ErrorList(ViSession instrSession,
                                     ViInt32 bufferSize,
                                     ViChar errors[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
		2, "Buffer Size");
	viCheckParm(RsCore_InvalidNullPointer(instrSession, errors), 3, "Null address for Errors");

	checkErr(rsspecan_GetAttributeViString( instrSession, "", RSSPECAN_ATTR_SYST_ERR_LIST,
		bufferSize, errors));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ErrorListSpecificType(ViSession instrSession,
                                                 ViInt32 messageType,
                                                 ViString channelName,
                                                 ViInt32 bufferSize,
                                                 ViChar messages[])
{
	ViStatus error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* pbuffer = NULL;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, messageType, RSSPECAN_VAL_MESSAGE_TYPE_ALL, RSSPECAN_VAL_MESSAGE_TYPE_MESSAGE),
		2, "Message Type");

	if (channelName == NULL)
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SYST:ERR:EXT? %s", messageTypeArr[messageType]);
	}
	else
	{
		snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SYST:ERR:EXT? %s,\'%s\'", messageTypeArr[messageType], channelName);
	}

	checkErr(rsspecan_QueryViString(instrSession, cmd, bufferSize, messages));

Error:
	if (pbuffer) free(pbuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Error Message
 * Purpose:  This function translates the error codes returned by this
 *           instrument driver into user-readable strings.
 *
 *           Note:  The caller can pass NULL for the vi parameter.  This
 *           is useful if one of the init functions fail.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_error_message(ViSession instrSession, ViStatus errorCode,
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
ViStatus _VI_FUNC rsspecan_ClearInstrumentErrors(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SYST_ERR_CLEAR, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Preset
 * Purpose:  This function initiates an instrument reset.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_Preset(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SYST_PRESET, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
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
	ViSession instrSession,
	ViInt32 presetOperatingMode
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_PRESET_OPERATING_MODE, presetOperatingMode),
		2, "Preset Operating Mode");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*****************************************************************************
 * Function: ID String Factory
 * Purpose:  This function sets the *IDN string to the factory default
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_IDStringFactory(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SYST_ID_FACTORY, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Get ID String Factory
 * Purpose:  This function returns "1" for the factory default state and "0"
 *           if the ID string is changed.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetIDStringFactory(ViSession instrSession,
                                              ViInt32* factoryDefault)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SYST_ID_FACTORY_QUERY, factoryDefault));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Set Timeout
 * Purpose:  This function sets the OPC timeout.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetTimeout(ViSession instrSession,
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
ViStatus _VI_FUNC rsspecan_GetTimeout(ViSession instrSession,
                                      ViUInt32* timeout)
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
                                          ViUInt32* VISATimeout)
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
ViStatus _VI_FUNC rsspecan_LockLocalKey(ViSession instrSession,
                                        ViBoolean lockLocalKey)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LOCK_LOCAL_KEY, lockLocalKey),
		2, "Lock Local Key");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function clears status.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_ClearStatus(
	ViSession instrSession
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
ViStatus _VI_FUNC rsspecan_IDQueryResponse(ViSession instrSession,
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
	ViSession instrSession
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
	ViSession instrSession,
	ViInt32* opc
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_QueryViInt32(instrSession, "*OPC?", opc));

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
ViStatus _VI_FUNC rsspecan_SetFastSweepMode(ViSession instrSession,
                                            ViInt32 fastSweepMode)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, fastSweepMode, RSSPECAN_VAL_FAST_SWEEP_NORMAL, RSSPECAN_VAL_FAST_SWEEP_FAST),
		2, "Fast Sweep Mode");

	rsSession->fastSweepInstrument = fastSweepMode;

Error:
	return error;
}

/*****************************************************************************
 * Function: Get Error and Clear Error Functions
 * Purpose:  These functions enable the instrument driver user to
 *           get or clear the error information the driver associates with the
 *           RS session.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetError(ViSession instrSession,
                                    ViStatus* errorCode,
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
	(void)RsCore_UnlockSession(instrSession); // IGNORE: ERROR!!! Missing Lock
	return error;
}

ViStatus _VI_FUNC rsspecan_ClearError(ViSession instrSession)
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

ViStatus _VI_FUNC rsspecan_WriteInstrData(ViSession instrSession, ViString writeBuffer)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	error = RsCore_Write(instrSession, writeBuffer);

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

ViStatus _VI_FUNC rsspecan_ReadInstrData(ViSession instrSession, ViInt32 numBytes,
                                         ViChar rdBuf[], ViUInt32* bytesRead)
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

/// HIFN  Calls SYST:ERR? automatically in CheckStatus() when Instrument Status Error is detected
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsrfsiggen_init or
/// HIPAR instrSession/rsxxx_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session
ViStatus _VI_FUNC rsspecan_ConfigureAutoSystemErrQuery(ViSession instrSession,
                                                       ViBoolean autoSystErrQuery)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	rsSession->autoSystErrQuery = autoSystErrQuery;

Error:
	return error;
}

/*****************************************************************************
 *-------------------- Utility Functions (Not Exported) ---------------------*
 *****************************************************************************/


/*****************************************************************************
 *------------------------ Global Session Callbacks -------------------------*
 *****************************************************************************/

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
