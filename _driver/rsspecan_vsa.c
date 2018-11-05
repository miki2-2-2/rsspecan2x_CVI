/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver -
 *                              VSA - Vector Signal Analysis
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
 *  VSA - Vector Signal Analysis
 *****************************************************************************/

#include "rsspecan.h"

static ViString VSAimpExpTypeArr[] = {"STAN","MAPP","PATT","FILT","EQU"};
static ViString VSAResultArr[] = {"DDEM:STAT:MPOW","DDEM:STAT:DTTS","DDEM:STAT:ADR","DDEM:STAT:CFER","DDEM:STAT:EVM",
								  "DDEM:STAT:FSK:CFDR","DDEM:STAT:FSK:DERR","DDEM:STAT:FSK:MDEV","DDEM:STAT:IQIM",
								  "DDEM:STAT:GIMB","DDEM:STAT:MERR","DDEM:STAT:OOFF","DDEM:STAT:PERR","DDEM:STAT:PLER",
								  "DDEM:STAT:QERR","DDEM:STAT:RHO","DDEM:STAT:SNR","DDEM:STAT:FDER","DDEM:STAT:FSK:RDEV",
								  "DDEM:STAT:SRER", NULL};
static ViString VSAModifierArr[] =  {"","RMS","AVG","SDEV","TPE","PEAK","ASYM","PCTL","CURR","AVG","RPE","SDEV","PCTL",
									 "PEAK","PAVG","TPE","PSD","PPCT",NULL};
static ViString VSACatPatternArr[] = {"ALL","CURR",NULL};
static ViString VSALimitTypeArr[] = {"Current", "Mean", "Peak", "PeakCurrent", "PeakMean", "PeakPeak", "RMSCurrent", "RMSMean", "RMSPeak", NULL};
static ViString VSAResultTypeArr[] = {"EVM", "PhaseErr", "MagnErr", "CFErr", "RHO", "IQOffset", "FreqErr", "FreqDevErr", NULL};
static ViString	VSAStatisticArr[] = {"", "AVG", "RPE", "SDEV", "PCTL", "PEAK", "PAVG", "TPEAK", "PSD", "PPCT", NULL};
static ViString VSATraceArr[] = {"", "TRACE1", "TRACE2", "TRACE3", "TRACE4", "TRACE5", "TRACE6", "TRACE1R", "TRACE2R", "TRACE3R",
								 "TRACE1I", "TRACE2I", "TRACE3I", "TRACEIQCX", "TRACEIQCY", NULL};
static ViString BERResultTypeArr[] = {"Current","Min","Max","Accumulative",NULL};

/// HIFN  This function switches the instrument into Vector Signal Analysis
/// HIFN  mode.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_SetVSAMode(ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;
	ViInt32     old_timeout = -1;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, 15000));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_VSA_MODE, ""));

Error:
    if (old_timeout >= 0)
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function restores the factory settings of the parameters for the
/// HIFN  R&S FSV-K70 and FSQ-K70 option.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR defaults/Selects the item to be set to factory default.
ViStatus _VI_FUNC rsspecan_SetVSAFactoryDefaults(ViSession  instrSession,
                                                ViInt32 defaults)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV (instrSession))
	{
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, defaults, RSSPECAN_VAL_VSA_FACTORY_GST, RSSPECAN_VAL_VSA_FACTORY_FILT),
				2, "Defaults");
	}
	else
	{
		if ((defaults != RSSPECAN_VAL_VSA_FACTORY_STAN) &&
			(defaults != RSSPECAN_VAL_VSA_FACTORY_PATT) &&
			(defaults != RSSPECAN_VAL_VSA_FACTORY_ALL))
			viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Defaults");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_FACTORY_DEFAULTS, defaults),
			2, "Defaults");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function presets the screens.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
ViStatus _VI_FUNC rsspecan_PresetVSAScreens (ViSession instrSession,
                                             ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_PRESET_SCREENS, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function initiates automatic setting of the RF attenuation and
/// HIFN  IF gain to the level of the applied signal.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR timeout/This control sets the timeout value.
ViStatus _VI_FUNC rsspecan_VSARefLevelAdjust (ViSession instrSession,
                                              ViInt32 window,
                                              ViUInt32 timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViInt32     old_timeout = -1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295), 3, "Timeout");
    sprintf (buffer, "Win%ld", window);

    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_REF_LEVEL_ADJUST, ""));

Error:
    if (old_timeout >= 0)
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function selects an automatic setting of all modulation
/// HIFN  parameters according to a standardized transmission method or a
/// HIFN  user-defined transmission method. The standardized transmission methods
/// HIFN  are available in the instrument as predefined standards.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR digitalStandard/Specifies the file name that contains the transmission method
ViStatus _VI_FUNC rsspecan_VSADigitalStandardLoad(
	ViSession	instrSession,
	ViInt32		window,
	ViString	digitalStandard
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD, digitalStandard),
			3, "Digital Standard");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function stores the current settings of the vector signal
/// HIFN  analysis as a new user-defined digital standard. If the name of the
/// HIFN  digital standard is already in use, an error message is output and a
/// HIFN  new name has to be selected. It is recommended to define a group, a
/// HIFN  comment and a prefix before storing the standard.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR userDefinedStandard/Define name of the user-defined standard. If the name of the digital
/// HIPAR userDefinedStandard/standard is already in use, an error message is output and a new name
/// HIPAR userDefinedStandard/has to be selected.
/// HIPAR group/Enters the group for a new standard. The group is stored with the
/// HIPAR group/standard and is only displayed in the selection menu (manual
/// HIPAR group/operation). When remote control is used, the string is deleted after
/// HIPAR group/the standard has been stored, allowing a new group to be entered for
/// HIPAR group/the next standard.
/// HIPAR comment/Enters the comment for a new standard. The comment is stored with the
/// HIPAR comment/standard and is only displayed in the selection menu (manual
/// HIPAR comment/operation). When remote control is used, the string is deleted after
/// HIPAR comment/the standard has been stored, allowing a new comment to be entered for
/// HIPAR comment/the next standard.
/// HIPAR prefix/Defines the prefix of the sync pattern for a standard. When a sync
/// HIPAR prefix/pattern for the standard is selected later, the patterns containing
/// HIPAR prefix/this string at the start are offered with priority in the selection
/// HIPAR prefix/table (only relevant to manual operation). When remote control is used,
/// HIPAR prefix/the string is deleted after the standard has been stored, allowing a
/// HIPAR prefix/new prefix to be entered for the next standard.
ViStatus _VI_FUNC rsspecan_VSADigitalStandardSave(ViSession instrSession,
                                                ViInt32     window,
                                                ViString    userDefinedStandard,
                                                ViString    group,
                                                ViString    comment,
                                                ViString    prefix)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

	if (!rsspecan_IsFSV (instrSession))
	{
	    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_GROUP, group),
	    		4, "Group");

		viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_PREFIX, prefix),
				6, "Prefix");
	}
    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_COMMENT, comment),
    		5, "Comment");

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_SAVE, userDefinedStandard),
    		3, "User Defined Standard");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function deletes the selected digital standard if it is a
/// HIFN  user-defined standard. If the standard is predefined, it cannot be
/// HIFN  deleted (error message).
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR digitalStandard/Select digital standard if it is a user-defined standard. If the
/// HIPAR digitalStandard/standard is predefined, it cannot be deleted (error message).
ViStatus _VI_FUNC rsspecan_VSADigitalStandardDelete(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViString    digitalStandard)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_DELETE, digitalStandard),
    		3, "Digital Standard");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function restores the default settings of the currently selected
/// HIFN  standard.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
ViStatus _VI_FUNC rsspecan_VSADigitalStandardDefaults(ViSession instrSession,
                                                        ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_DEFAULTS, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the modulation.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR symbolRate/Sets the symbol rate.
/// HIPAR mappingName/Selects the mapping designated by Mapping Name for the digital
/// HIPAR mappingName/demodulation.
/// HIPAR modulation/Selects modulation type/order.
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationSettings(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		modulation,
	ViString	mappingName,
	ViReal64	symbolRate
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_SRATE, symbolRate),
    		3, "Symbol Rate");

    switch(modulation){
		//QPSK
        case RSSPECAN_VAL_VSA_MOD_ORDER_BPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_PSK));
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 2));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_QPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, RSSPECAN_VAL_MOD_QPSK_NORM));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_OQPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, RSSPECAN_VAL_MOD_QPSK_OFFS));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_DQPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, RSSPECAN_VAL_MOD_QPSK_DIFF));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_PI4QPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, RSSPECAN_VAL_MOD_QPSK_NPI4));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_DPI4QPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, RSSPECAN_VAL_MOD_QPSK_DPI4));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_3PI4QPSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT, RSSPECAN_VAL_MOD_QPSK_N3PI4));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_8PSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_PSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, RSSPECAN_VAL_MOD_PSK_NORM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 8));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_D8PSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_PSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, RSSPECAN_VAL_MOD_PSK_DIFF));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 8));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_N3PI8PSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_PSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, RSSPECAN_VAL_MOD_PSK_N3PI8));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 8));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_PI8D8PSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_PSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT, RSSPECAN_VAL_MOD_PSK_PI8D8PSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT, 8));
		break;

		//MSK
        case RSSPECAN_VAL_VSA_MOD_ORDER_MSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_MSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_MSK_FORMAT, RSSPECAN_VAL_MOD_MSK_NORM));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_DMSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_MSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_MSK_FORMAT, RSSPECAN_VAL_MOD_MSK_DIFF));
        break;

		//QAM
        case RSSPECAN_VAL_VSA_MOD_ORDER_16QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_NORM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 16));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_PI4_16QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_NPI4));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 16));
		break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_32QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_NORM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 32));
        break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_NPI4_32QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_MNPI4));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 32));
		break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_64QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_NORM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 64));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_128QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_NORM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 128));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_256QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_QAM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT, RSSPECAN_VAL_MOD_QAM_NORM));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT, 256));
        break;

		//FSK
        case RSSPECAN_VAL_VSA_MOD_ORDER_2FSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_FSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_NSTAT, 2));
        break;
        case RSSPECAN_VAL_VSA_MOD_ORDER_4FSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_FSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_NSTAT, 4));
        break;
		case RSSPECAN_VAL_VSA_MOD_ORDER_8FSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_FSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_NSTAT, 8));
        break;

		//User QAM
        case RSSPECAN_VAL_VSA_MOD_ORDER_USER_2QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM));
			checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_UQAM_MAPPING, "2ary"));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_USER_4QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM));
			checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_UQAM_MAPPING, "4ary"));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_USER_16QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM));
			checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_UQAM_MAPPING, "16ary"));
        break;

        case RSSPECAN_VAL_VSA_MOD_ORDER_USER_32QAM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM));
			checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_UQAM_MAPPING, "32ary"));
        break;

		//VSB
        case RSSPECAN_VAL_VSA_MOD_ORDER_8VSB:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_VSB));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_VSB_NSTAT, 8));
		break;

		//ASK
        case RSSPECAN_VAL_VSA_MOD_ORDER_2ASK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_ASK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_ASK_MODE, 2));
		break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_4ASK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_ASK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_ASK_MODE, 4));
		break;

		//APSK
        case RSSPECAN_VAL_VSA_MOD_ORDER_16APSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_APSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_APSK_MODE, 16));
		break;

		case RSSPECAN_VAL_VSA_MOD_ORDER_32APSK:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_APSK));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_APSK_MODE, 32));
		break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, modulation), 4, "Modulation");
		break;
}

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_MAPPING, mappingName),
    		5, "Mapping Name");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the reference value of the frequency
/// HIFN  deviation for FSK modulation.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR referenceDeviation/Sets the reference value of the frequency deviation.
/// HIPAR mode/This control defines if Reference Deviation value is absolute or
/// HIPAR mode/relative to the symbol rate.
ViStatus _VI_FUNC rsspecan_ConfigureVSAFSKRefDeviation(
	ViSession	instrSession,
	ViInt32		window,
	ViReal64	referenceDeviation,
	ViInt32		mode
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);

	switch(mode)
	{
		case RSSPECAN_VAL_VSA_REF_DEV_MODE_ABS:
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV, referenceDeviation),
					3, "Reference Deviation");
		break;

		case RSSPECAN_VAL_VSA_REF_DEV_MODE_REL:
			 viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV_REL, referenceDeviation),
			 		3, "Reference Deviation");
		break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 4, "Mode");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the TX filter.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR type/Defines the type of transmit filter.
/// HIPAR alphaBT/Determines the filter characteristic (ALPHA/BT).
ViStatus _VI_FUNC rsspecan_ConfigureVSATXFilter (ViSession instrSession,
                                                 ViInt32 window, ViInt32 type,
                                                 ViReal64 alphaBT)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

	if (type == RSSPECAN_VAL_VSA_TX_FILTER_TYPE_NONE)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_TX_FILTER_STATE, VI_FALSE),
				5, "");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_TX_FILTER_TYPE, type),
				3, "Type");

		if (type <= RSSPECAN_VAL_VSA_TX_FILTER_TYPE_GMSK)
		{
			viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_TX_FILTER_ALPHA, alphaBT),
					4, "Alpha BT");
		}
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function selects a user-defined transmit filter file.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR filterFileName/The name of the Transmit filter file name.
ViStatus _VI_FUNC rsspecan_ConfigureVSAUserDefinedTXFilter(
	ViSession	instrSession,
	ViInt32		window,
	ViString	filterFileName
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	if (filterFileName == NULL)
	{
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_TX_FILTER_STATE, VI_FALSE));
	}
    else
	{
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_TX_FILTER_STATE, VI_TRUE));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_TX_FILTER_TYPE, RSSPECAN_VAL_VSA_TX_FILTER_USER));

		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_VSA_TX_FILTER_USER_FILE, filterFileName),
				3, "Filter File Name");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function reads the names of all mappings stored on the hard disk.
/// HIFN  A mapping describes the assignment of constellation points to symbols.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR numberofMappings/This control returns number of the names of all digital standards
/// HIPAR numberofMappings/available.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the catalog
/// HIPAR bufferSize/list parameter.
/// HIPAR mappingsList/This control returns array of the names of all digital standards
/// HIPAR mappingsList/available in CSV form.
ViStatus _VI_FUNC rsspecan_GetVSAModulationMappingCatalog(ViSession     instrSession,
                                                            ViInt32     window,
                                                            ViInt32*    numberofMappings,
                                                            ViInt32     bufferSize,
                                                            ViChar      mappingsList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViUInt32    retCnt = 0;
    ViChar      *buf=NULL;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K70"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
    		4, "Buffer Size");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, mappingsList), 5, "Mapping List");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":SENS%ld:DDEM:MAPP:CAT?", window);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &buf)); // TODO: Check the response processing
    checkErr(RsCore_ParseCatalog(buf, bufferSize, mappingsList, numberofMappings));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if (buf) free(buf);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function returns the specific modulation level for FQAM.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR specificLevel/This control returns the specific modulation level for FQAM.
ViStatus _VI_FUNC rsspecan_GetVSAModulationUQAMLevel (ViSession instrSession,
                                                      ViInt32 window,
                                                      ViInt32 *specificLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_UQAM_NSTAT, specificLevel),
    		3, "Specific Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function selects the file that contains the user-defined
/// HIFN  modulation to be loaded.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR userModulation/Selects the file that contains the user-defined modulation to be loaded.
ViStatus _VI_FUNC rsspecan_VSALoadUserModulation (ViSession instrSession,
                                                  ViString userModulation)
{
	ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViInt32(instrSession, "Win0", RSSPECAN_ATTR_VSA_MODULATION_FORMAT, RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_VSA_LOAD_USER_MODULATION, userModulation),
			2, "User Modulation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function sets the instrument to look for a bursted or a
/// HIFN  continuous signal.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR signalType/This control sets the instrument to look for a bursted or a continuous
/// HIPAR signalType/signal.
ViStatus _VI_FUNC rsspecan_ConfigureVSASignalType (ViSession instrSession,
                                                   ViInt32 	 window,
                                                   ViInt32 	 signalType)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SIGNAL_TYPE, signalType),
    		3, "Signal Type");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function defines the settings that are used to accurately control
/// HIFN  the behaviour of the burst search even under difficult measurement
/// HIFN  conditions.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR usefulLength/Sets the minimum length of a burst.
/// HIPAR maxLength/Sets the maximum length of a burst. Only those bursts will be
/// HIPAR maxLength/recognized that fall below this length.
/// HIPAR runIn/Sets the length of the rising burst edge which was not considered when
/// HIPAR runIn/evaluating the result.
/// HIPAR runOut/Sets the length of the falling burst edge which is not considered when
/// HIPAR runOut/evaluating the result.
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSignal(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	usefulLength,
	ViInt32	maxLength,
	ViInt32	runIn,
	ViInt32	runOut
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_SKIP_RISE, runIn),
    		5, "Run In");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_SKIP_FALL, runOut),
    		6, "Run Out");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_LENGTH_MIN, usefulLength),
			3, "Useful Length");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_LENGTH_MAX, maxLength),
    		4, "Max Length");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function sets the instrument to look for a pattern.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR patternSignal/Sets the instrument to look for a pattern.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSignalState
             (ViSession instrSession, ViInt32 window, ViBoolean patternSignal)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_PATTERN_SIGNAL_STATE, patternSignal),
    		3, "Pattern Signal");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the pattern offset. If the position of
/// HIFN     the pattern within the burst is known, it is recommended that you
/// HIFN  define the offset. That will accelerate the pattern search and enhance
/// HIFN  the accuracy of the burst search.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR patternOffsetState/This control (de)activates the pattern offset.
/// HIPAR patternOffset/Defines a number of symbols which are ignored before the comparison
/// HIPAR patternOffset/with the pattern starts.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternOffset (ViSession instrSession,
                                                      ViInt32   window,
                                                      ViBoolean patternOffsetState,
                                                      ViInt32   patternOffset)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_PATTERN_OFFSET_STATE, patternOffsetState),
    		3, "Pattern Offset State");

	if (patternOffsetState == VI_TRUE){

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_SYNC_OFFSET, patternOffset),
				4, "Pattern Offset");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures settings of the measured I/Q data.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO
/// HIFN     RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_SYM
/// HIFN  RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_S
/// HIFN     RSSPECAN_ATTR_VSA_MODULATION_PRATE
/// HIFN     RSSPECAN_ATTR_VSA_DEMODULATION_SIDE_BAND
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     [SENSe<1|2>:]DDEMod:RLENgth:AUTO ON | OFF
/// HIFN     [SENSe<1|2>:]DDEMod:RLENgth <value>SYM
/// HIFN     [SENSe<1|2>:]DDEMod:RLENgth <value>S
/// HIFN     [SENSe<1|2>:]DDEMod:SBANd NORMal | INVerse
/// HIFN     [SENSe<1|2>:]DDEMod:NORMalize ON | OFF
/// HIFN     [SENSe<1|2>:]DDEMod:PRATe 1 | 2 | 4 | 8 | 16
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR mode/Selects whether or not is automatic adaptation of recording length
/// HIPAR mode/used. To manually set recording length, select if Symbols or Time units
/// HIPAR mode/are applied (automatic adaptation of recording length will be switched
/// HIPAR mode/to off).
/// HIPAR captureLength/Sets the capture length for further processing, e.g. for burst search.
/// HIPAR captureOversampling/Sets the number of points per symbol.
/// HIPAR swapIQ/Swaps the I and Q values of the signal.
ViStatus _VI_FUNC rsspecan_ConfigureVSADataCaptureSettings(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		mode,
	ViReal64	captureLength,
	ViInt32		captureOversampling,
	ViBoolean	swapIQ
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViInt32     length;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_VSA_DEM_RLEN_AUTO_ON, RSSPECAN_VAL_VSA_DEM_RLEN_TIME),
    		3, "Mode");

    sprintf (buffer, "Win%ld", window);

    switch (mode){
        case RSSPECAN_VAL_VSA_DEM_RLEN_AUTO_ON:
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO, VI_TRUE));
        break;

        case RSSPECAN_VAL_VSA_DEM_RLEN_SYMBOL:
            length= (ViInt32)captureLength;
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO, VI_FALSE));
            viCheckParm(rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_SYM,
                 (ViInt32) captureLength), 4, "Capture Length");
        break;
        case RSSPECAN_VAL_VSA_DEM_RLEN_TIME:
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO, VI_FALSE));
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_S, captureLength),
            		4, "Capture Length");
        break;
    }

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_PRATE, captureOversampling),
			5, "Capture Oversampling");

	if(swapIQ)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_SIDE_BAND, RSSPECAN_VAL_VSA_SBAND_INV),
				6, "SwapI Q");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_SIDE_BAND, RSSPECAN_VAL_VSA_SBAND_NORM),
				6, "Swap IQ");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function links the burst search to the type of signal. When a
/// HIFN  signal is marked as bursted, burst search is switched on automatically.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR burstSearch/Enables or disables burst searches. If "Auto" is selected, burst
/// HIPAR burstSearch/search is enabled only if "Bursted Signal" is selected.
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchAuto (ViSession instrSession,
                                                        ViInt32 window,
                                                        ViInt32 burstSearch)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_AUTO, burstSearch),
    		3, "Burst Search");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the search for a signal burst.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR searchState/Switches the search for a signal burst on or off.
/// HIPAR burstSearchMode/Switches the vector analyzer so that a measurement is performed only
/// HIPAR burstSearchMode/if a burst is found (BURSt). The driver uses this value to set the
/// HIPAR burstSearchMode/RSSPECAN_ATTR_VSA_BURST_SEARCH_MODE attribute.
/// HIPAR thresholdAuto/Defines which value is to be used as the threshold. ON corresponds to
/// HIPAR thresholdAuto/a relative threshold value defined by the standard.
/// HIPAR thresholdMode/Sets the threshold mode. The driver uses this value to set the
/// HIPAR thresholdMode/RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_MODE attribute.
/// HIPAR thresholdLevel/Sets the threshold for the burst search.. The driver uses this value
/// HIPAR thresholdLevel/to set the RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_LEVEL attribute.
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearch(ViSession    instrSession,
                                                    ViInt32     window,
                                                    ViBoolean   searchState,
                                                    ViInt32     burstSearchMode,
                                                    ViBoolean   thresholdAuto,
                                                    ViInt32     thresholdMode,
                                                    ViReal64    thresholdLevel)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_STATE, searchState),
    		3, "Search State");

    if (searchState)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_MODE, burstSearchMode),
        		4, "Burst Search Mode");

		if (!rsspecan_IsFSV (instrSession))
		{
	        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_AUTO, thresholdAuto),
	        		5, "Threshold Auto");
	        if (!thresholdAuto)
	        {
	            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_MODE, thresholdMode),
	            		6, "Threshold Mode");

	            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_LEVEL, thresholdLevel),
	            		7, "Threshold Level");
	        }
		}
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configure the search tolerance and the min gap length
/// HIFN  and their default values.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR autoConfiguration/Enables or disables burst searches. If switched ON, burst search is
/// HIPAR autoConfiguration/enabled only if "Bursted Signal" is selected.
/// HIPAR tolerance/Sets burst search tolerance.
/// HIPAR minGaplen/Sets the minimum time between two bursts. A minimum time with
/// HIPAR minGaplen/decreased level must occur between two bursts. A new burst is
/// HIPAR minGaplen/recognized only after this time.
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchAutoConfiguration
             (ViSession instrSession, ViInt32 window,
              ViBoolean autoConfiguration, ViInt32 tolerance, ViInt32 minGaplen)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SEARCH_BURST_AUTO_CONFIGURATION, autoConfiguration),
    		3, "autoConfiguration");
	if (autoConfiguration == VI_FALSE)  {

	    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SEARCH_BURST_TOLERANCE, tolerance),
	    		4, "Tolerance");

	    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BURST_SEARCH_GLEN, minGaplen),
	    		5, "Min Gaplen");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries whether burst is available in the signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR found/Returns the burst found result.
ViStatus _VI_FUNC rsspecan_GetVSABurstFound (ViSession instrSession,
                                             ViBoolean *found)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     statusRegisterValue;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidNullPointer(instrSession, found), 2, "Found");
    checkErr(rsspecan_getStatusRegister (instrSession, 11, &statusRegisterValue));
    *found = (ViBoolean)(statusRegisterValue & 1);

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function links the pattern search to the type of signal. When a
/// HIFN  signal is marked as patterned, pattern search is switched on
/// HIFN  automatically.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR patternSearch/Enables or disables pattern searches. If switched ON, pattern search
/// HIPAR patternSearch/is enabled automatically if "Pattern" is selected.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSearchAuto (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViInt32 patternSearch)
{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_SEARCH_AUTO, patternSearch),
    		3, "Pattern Search");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the search for a signal burst.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR searchState/Switches the search for a signal burst on or off.
/// HIPAR patternSearchMode/Switches the vector analyzer so that a measurement is performed only
/// HIPAR patternSearchMode/if a burst is found (BURSt).
/// HIPAR pattern/Sets a predefined sync pattern file. Up to 16 sync pattern files may
/// HIPAR pattern/be given.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSearch(ViSession  instrSession,
                                                    ViInt32     window,
                                                    ViBoolean   searchState,
                                                    ViInt32     patternSearchMode,
                                                    ViString    pattern)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    if (searchState)
    {
        snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:DDEM:SEAR:SYNC:SEL '%s'", window, pattern);
        checkErr(RsCore_Write(instrSession, cmd));
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_SEARCH_MODE, patternSearchMode),
        		4, "Search Mode");
    }
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_SEARCH_STATE, searchState),
    		3, "Search State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the pattern search and the IQ Correlation
/// HIFN  Threshold setting.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR autoConfiguration/Sets the IQ correlation threshold to its default value.
/// HIPAR IQCorrelationThreshold/Sets the IQ correlation threshold for pattern matching in percent. A
/// HIPAR IQCorrelationThreshold/high level means stricter matching.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSearchAutoConfiguration(
             ViSession instrSession,
			 ViInt32   window,
             ViBoolean autoConfiguration,
			 ViReal64  IQCorrelationThreshold)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_SEARCH_PATTERN_AUTO_CONFIGURATION, autoConfiguration),
    		3, "Auto Configuration");

	if (autoConfiguration == VI_FALSE )
	{
	    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_SEARCH_IQ_CORRELATION_THRESHOLD, IQCorrelationThreshold),
	    		4, "IQ Correlation Threshold");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries whether one of the selected sync patterns
/// HIFN  (DDEM:SEAR:SYNC:SEL) is available in the signal. Up to 16 sync pattern
/// HIFN  files can be selected.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the found
/// HIPAR bufferSize/parameter.
/// HIPAR found/Returns the sync pattern found result.
ViStatus _VI_FUNC rsspecan_GetVSAPatternFound(ViSession instrSession,
                                                ViInt32 window,
                                                ViInt32 bufferSize,
                                                ViChar  found[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViUInt32    retCnt;
    ViChar      *buf=NULL;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K70"));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":SENS%ld:DDEM:SEAR:SYNC:FOUN?", window);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &buf)); // TODO: Check the response processing

    if ((ViInt32)retCnt>bufferSize)
    {
        strncpy(found, buf, bufferSize);
        error = retCnt;
    }
	else
    {
        strcpy(found, buf);
    }

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if (buf) free(buf);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function reads the names of all patterns stored on the hard disk.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR selection/Sets the pattern selection.
/// HIPAR numberofPatterns/This control returns number of the names of all digital standards
/// HIPAR numberofPatterns/available.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the catalog
/// HIPAR bufferSize/list parameter.
/// HIPAR patternsList/This control returns array of the names of all digital standards
/// HIPAR patternsList/available in CSV form.
ViStatus _VI_FUNC rsspecan_GetVSAPatternCatalog(ViSession   instrSession,
                                                ViInt32     window,
                                                ViInt32     selection,
                                                ViInt32*    numberofPatterns,
                                                ViInt32     bufferSize,
                                                ViChar      patternsList[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViUInt32    retCnt = 0;
    ViChar      *buf=NULL;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K70"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, selection, RSSPECAN_VAL_VSA_PATTERN_ALL, RSSPECAN_VAL_VSA_PATTERN_CURRENT),
    		3, "Selection");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, bufferSize, 0, INT_MAX),
    		5, "Buffer Size");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, patternsList), 6, "Patterns List");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:DDEM:SEAR:SYNC:CAT? %s", window, VSACatPatternArr[selection]);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &buf)); // TODO: Check the response processing
    checkErr(RsCore_ParseCatalog(buf, bufferSize, patternsList, numberofPatterns));

	patternsList[retCnt] = '\0';

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if (buf) free(buf);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function performs the selected operation with channel table.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR operation/This control selects the type of memory operation to be performed.
/// HIPAR patternName/This control selects a predefined channel table file.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternOperations(ViSession  instrSession,
                                                        ViInt32     window,
                                                        ViInt32     operation,
                                                        ViString    patternName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, operation, RSSPECAN_VAL_SYNC_PATT_ADD, RSSPECAN_VAL_SYNC_PATT_COPY),
    		3, "Operation");

    sprintf (buffer, "Win%ld", window);

    switch(operation){
        case RSSPECAN_VAL_SYNC_PATT_ADD:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_PATT_ADD, patternName),
            		4, "Pattern Name");
        break;
        case RSSPECAN_VAL_SYNC_PATT_SELECT:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_NAME, patternName),
            		4, "Pattern Name");
        break;
        case RSSPECAN_VAL_SYNC_PATT_DELETE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_NAME, patternName),
            		4, "Pattern Name");
			checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_DELETE, ""));
        break;
        case RSSPECAN_VAL_SYNC_PATT_REMOVE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_PATT_REMOVE, patternName),
            		4, "Pattern Name");
        break;
        case RSSPECAN_VAL_SYNC_PATT_REMOVE_ALL:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_PATT_REMOVE_ALL, ""));
        break;
        case RSSPECAN_VAL_SYNC_PATT_COPY:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_COPY, patternName),
            		4, "Pattern Name");
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the selected pattern.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR patternName/This control selects a predefined channel table file.
/// HIPAR description/Sets a text to explain the pattern. The text is displayed only in the
/// HIPAR description/selection menu (manual control). This text is supposed to be short and
/// HIPAR description/concise. Detailed information about the pattern is given in the
/// HIPAR description/comment.
/// HIPAR comment/Defines a comment to a sync pattern.
/// HIPAR degreeofModulation/Sets the degree of modulation (number of permitted states).
/// HIPAR patternData/Sets a sync sequence for the sync pattern. Four values represent a
/// HIPAR patternData/symbol. The value range of a symbol depends on the degree of
/// HIPAR patternData/modulation. FFFF entries are interpreted as "Don't Care Bits".
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSettings(ViSession    instrSession,
                                                        ViInt32     window,
                                                        ViString    patternName,
                                                        ViString    description,
                                                        ViString    comment,
                                                        ViInt32     degreeofModulation,
                                                        ViString    patternData)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_NAME, patternName),
    		3, "Pattern Name");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SENS%ld:DDEM:SEAR:SYNC:DATA '%s'", window, patternData);
    checkErr(RsCore_Write(instrSession, cmd));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_NSTAT, degreeofModulation),
    		6, "Degree Of Modulation");

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_TEXT, description),
    		4, "Description");

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_SYNC_COMMENT, comment),
    		5, "Comment");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the tesult length od modulation
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultLength/Sets the number of displayed symbols (result length).
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultLength(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     resultLength)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_DEMODULATION_RESULT_LENGTH, resultLength),
    		3, "Result Length");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function sets the reference for the result range alignment.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR reference/Selects the reference point.
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultRangeReference(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		trace,
	ViInt32		reference
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 1),
    		3, "Trace");

	sprintf (buffer, "C%ld,TR%ld", window, trace);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_REF_POINT, reference),
			4, "Reference");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function sets the measurements results alignment on the display.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR alignment/Defines where the relevant event (reference point) is to appear in the
/// HIPAR alignment/diagram.
/// HIPAR alignValue/Defines shift of the reference point in the displayed range. The
/// HIPAR alignValue/setting is made in percent relative to the size of the display range.
/// HIPAR alignValue/This display range is normalized to 0% (left edge) and 100% (right
/// HIPAR alignValue/edge). If the result length is changed, the percentage remains valid,
/// HIPAR alignValue/and the absolute value in symbols for this shift is modified
/// HIPAR alignValue/correspondingly.
/// HIPAR alignOffset/Defines shifts the display range (relative to the reference time) by
/// HIPAR alignOffset/the number of given symbols. The resolution is 1 symbol. A value >0
/// HIPAR alignOffset/results in a shift towards the right, and a value <0 results in a shift
/// HIPAR alignOffset/towards the left.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayAlignment(ViSession   instrSession,
                                                        ViInt32     window,
                                                        ViInt32     trace,
                                                        ViInt32     alignment,
                                                        ViReal64    alignValue,
                                                        ViInt32     alignOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 3),
    		3, "Trace");

	sprintf (buffer, "C%ld,TR%ld", window, trace);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_ALIGN, alignment),
			4, "Alignment");

	if (!rsspecan_IsFSV (instrSession))
	{
	    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_ALIGN_VALUE, alignValue),
	    		5, "Alignment Value");
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_ALIGN_OFFSET, alignOffset),
			6, "Alignment Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function sets the measurements results alignment on the display.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR value/Sets the number of the symbol which marks the beginning of the
/// HIPAR value/alignment reference source. In effect, this setting defines an offset
/// HIPAR value/of the x-axis.
ViStatus _VI_FUNC rsspecan_ConfigureVSASymbolNoAtReference(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		trace,
	ViInt32		value
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 1),
    		3, "Trace");

    sprintf (buffer, "Win%ld,TR%ld", window, trace);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FIT_SYMBOL_NUMBER, value),
    		4, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures range of the result is to be evaluated,
/// HIFN     either the entire result range or only a specified part of it.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR entireResultRange/Switches both evaluation lines on or off.
/// HIPAR start/Sets the start value for the evaluation range.
/// HIPAR stop/Sets the stop value for the evaluation range.
ViStatus _VI_FUNC rsspecan_ConfigureVSAEvaluationRange(
	ViSession	instrSession,
	ViInt32		window,
	ViBoolean	entireResultRange,
	ViInt32		start,
	ViInt32		stop
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_EVAL_LINE_STATE, entireResultRange),
    		3, "Entire Result Range");

	if(!entireResultRange)
	{
	sprintf (buffer, "C%ld,L1", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_EVAL_LINE, start),
			4, "Start");

	sprintf (buffer, "C%ld,L2", window);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_EVAL_LINE, stop),
			5, "Stop");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the statistics count. Entering 0 as a
/// HIFN  parameter activates "Auto" mode. Entering a number greater than
/// HIFN  0 activates "Manual" mode and sets the statistics count to the
/// HIFN  corresponding number.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR statisticCount/Sets the statistics count.
ViStatus _VI_FUNC rsspecan_ConfigureVSAStatisticCount (ViSession instrSession,
                                                       ViInt32 statisticCount)
{
	ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));  // TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_STATISTIC_COUNT, statisticCount),
    		2, "Statistic Count");

Error:
	return error;
}

/// HIFN  This function configures the result range you want to analyze.
/// HIFN  By default, the R&S FSV shows the results over all result ranges
/// HIFN  that have been captured in the data capturing process and are in
/// HIFN  the R&S FSV's memory. By selecting a range number, you can
/// HIFN  analyze a specific result range, e.g. a particular burst. The
/// HIFN  range depends on the number of result ranges you have captured
/// HIFN  previously. A selection of the result range is possible in
/// HIFN  single sweep mode only.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR resultRangeNumber/Sets the result range to be displayed after a single sweep.
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultRangeNumber (ViSession instrSession,
                                                          ViInt32 resultRangeNumber)
{
	ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));  // TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_RESULT_RANGE_NUMBER, resultRangeNumber),
    		2, "Result Range Number");

Error:
	return error;
}

/// HIFN  This function queries the current statistics counter value which
/// HIFN  indicates how many result ranges have been evaluated. For
/// HIFN  results that use the capture buffer as a source, the number of
/// HIFN  used capture buffers can be queried.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR evaluationType/Selects the evaluation type to be returned.
/// HIPAR currentCounter/Returns the current statistics counter value which indicates how
/// HIPAR currentCounter/many result ranges have been evaluated. For results that use the
/// HIPAR currentCounter/capture buffer as a source, the number of used capture buffers
/// HIPAR currentCounter/can be queried.
ViStatus _VI_FUNC rsspecan_GetVSACurrentStatisticsCounter (ViSession instrSession,
                                                           ViInt32 evaluationType,
                                                           ViInt32 *currentCounter)
{
	ViStatus	error = VI_SUCCESS;
	ViChar      buffer[15] = "";

    checkErr(RsCore_LockSession(instrSession));  // TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, evaluationType, RSSPECAN_VAL_VSA_EVAL_STATISTICS, RSSPECAN_VAL_VSA_EVAL_CAPTURE),
			2, "Evaluation Type");
	viCheckParm(RsCore_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_VSA_CURRENT_STATISTICS_COUNTER,
                                                 "StatisticsCounter",
                                                 evaluationType,
                                                 15,
                                                 buffer), 2, "Evaluation Type");

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_CURRENT_STATISTICS_COUNTER, currentCounter),
    		3, "Current Counter");

Error:
	return error;
}

/// HIFN  This function configures the compensation during demodulation.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR compensateFor/This control selects which distortion will be compensated during
/// HIPAR compensateFor/demodulation.
/// HIPAR compensationState/Sets the state of the compensation.
ViStatus _VI_FUNC rsspecan_ConfigureVSACompensation (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 compensateFor,
                                                     ViBoolean compensationState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

	switch(compensateFor) {
        case RSSPECAN_VAL_VSA_COMPENSATE_IQ_OFFSET:
			 viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_COMPENSATE_IQ_OFFSET, compensationState),
			 		4, "Compensation State");
		break;

		case RSSPECAN_VAL_VSA_COMPENSATE_IQ_IMBAL:
			 viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_COMPENSATE_IQ_IMBALANCE, compensationState),
			 		4, "Compensation State");
		break;

		case RSSPECAN_VAL_VSA_COMPENSATE_ADROOP:
		     viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_COMPENSATE_AMPLITUDE_DROOP, compensationState),
		     		4, "Compensation State");
		break;

		case RSSPECAN_VAL_VSA_COMPENSATE_FSK_DEV_ERR:
			 viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_COMPENSATE_FSK_DEVIATION, compensationState),
			 		4, "Compensation State");
		break;

		case RSSPECAN_VAL_VSA_COMPENSATE_FSK_CFD:
			 viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_COMPENSATE_CARRIER_FREQUENCY_DRIFT, compensationState),
			 		4, "Compensation State");
		break;

		case RSSPECAN_VAL_VSA_COMPENSATE_CHANNEL:
			 viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_VSA_COMPENSATE_CHANNEL, compensationState),
			 		4, "Compensation State");
		break;

		case RSSPECAN_VAL_VSA_COMPENSATE_SR_ERROR:
			 viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_VSA_COMPENSATE_SYMBOL_RATE_ERROR, compensationState),
			 		4, "Compensation State");
		break;

		default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, compensateFor), 3, "Compensate For");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the EVM modulation parameters
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR calculation/Selects the calculation formula for EVM modulation.
ViStatus _VI_FUNC rsspecan_ConfigureVSAEVMModulation(ViSession  instrSession,
                                                    ViInt32 window,
                                                    ViInt32 calculation)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_EVM_CALCULATION, calculation),
    		3, "Calculation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function determines the optimization criteria for the
/// HIFN  demodulation. RMS Min Optimization goal is the minimization of RMS of
/// HIFN  the error vector. EVM Min Optimization goal is the minimization of the
/// HIFN  EVM.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_VSA_MODULATION_OPTIMIZATION
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     [SENSe:]DDEMod:OPTimization
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR optimization/This control determines the optimization criteria for the
/// HIPAR optimization/demodulation. RMS Min Optimization goal is the minimization of RMS of
/// HIPAR optimization/the error vector. EVM Min Optimization goal is the minimization of the
/// HIPAR optimization/EVM.
#pragma warning( push )
#pragma warning( disable : 4100)
// Your function
ViStatus _VI_FUNC rsspecan_ConfigureVSAOptimization(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	optimization
)
{
 	ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_MODULATION_OPTIMIZATION, optimization),
    		3, "Optimization");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
#pragma warning( pop )

/// HIFN  This function configures the calculation of the error vector
/// HIFN  magnitude trace for Offset-QPSK only. It has no effect for all
/// HIFN  other modulations. It has no effect on results based on the MEAS
/// HIFN  or the REF signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR EVMOffset/Selects the calculation of the error vector magnitude trace for
/// HIPAR EVMOffset/Offset-QPSK only.
ViStatus _VI_FUNC rsspecan_ConfigureVSAEVMOffsetState(
	ViSession	instrSession,
	ViInt32	window,
	ViBoolean	EVMOffset
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_EVM_OFFSET, EVMOffset),
    		3, "EVM Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the pattern for synchronization, in order to
/// HIFN  obtain the correct reference signal. Depending on the signal, making
/// HIFN  use of the pattern for synchronization speeds up your measurement
/// HIFN  considerably and makes it more robust against high carrier frequency
/// HIFN  offsets.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR patternforSync/Sets the state of the pattern synchronization.
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternForSync(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		patternforSync
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar    repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	switch (patternforSync)
	{
		case RSSPECAN_VAL_VSA_PATT_SYNC_OFF:
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_MODE, RSSPECAN_VAL_MAN),
					3, "Pattern for Sync");

			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_STATE, VI_FALSE),
					3, "Pattern for Sync");
		break;

		case RSSPECAN_VAL_VSA_PATT_SYNC_ON:
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_MODE, RSSPECAN_VAL_MAN),
					3, "Pattern for Sync");

			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_STATE, VI_TRUE),
					3, "Pattern for Sync");
		break;

		case RSSPECAN_VAL_VSA_PATT_SYNC_AUTO:
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_MODE, RSSPECAN_VAL_AUTO),
					3, "Pattern for Sync");
		break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, patternforSync), 3, "Pattern for Sync");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the fine synchronization.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR autoSet/Sets wheater the fine synchronization is defined automatically.
/// HIPAR mode/Defines the fine synchronization mode used to calculate results, e.g. the bit error rate.
/// HIPAR SER/Defines a maximum symbol error rate for the known data in reference to the analyzed data.
ViStatus _VI_FUNC rsspecan_ConfigureVSAFineSynchronization (ViSession instrSession,
                                                            ViInt32 window,
                                                            ViBoolean autoSet,
                                                            ViInt32 mode,
                                                            ViReal64 SER)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_FINE_SYNC_AUTO, autoSet),
    		3, "Auto Set");

	if (!autoSet)
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FINE_SYNC_MODE, mode),
				4, "Mode");

	if (!autoSet && mode == RSSPECAN_VAL_VSA_FINE_SYNC_KDAT)
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_FINE_SYNC_SER, SER),
				5, "SER");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function queries whether a loaded Known Data file was used for fine synchronization or not.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR result/Queries whether a loaded Known Data file was used for fine synchronization or not.
ViStatus _VI_FUNC rsspecan_QueryVSAFineSynchronizationResult (ViSession instrSession,
                                                              ViInt32 window,
                                                              ViInt32 *result)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_FINE_SYNC_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the Known Data File.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR knownDataState/Activates or deactivates the use of the loaded data file.
/// HIPAR dataFileName/Selects the Known Data file.
ViStatus _VI_FUNC rsspecan_ConfigureVSAKnownData (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViBoolean knownDataState,
                                                  ViString dataFileName)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_KNOWN_DATA_STATE, knownDataState),
    		3, "Known Data State");

	if (knownDataState)
		viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_KNOWN_DATA_FILE_NAME, dataFileName),
				3, "data File Name");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the estimation points per symbol. You
/// HIFN  can set the estimation points manually or let the analyzer decide
/// HIFN  which estimation points to use.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR selectsEstimationPointsSym/Selects Estimation Points/Sym.
ViStatus _VI_FUNC rsspecan_ConfigureVSAEstimationPointsSymbol(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		selectsEstimationPointsSym
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar    repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	switch(selectsEstimationPointsSym)
	{
		case RSSPECAN_VAL_VSA_AUTOMATIC_ESTIMATION:
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_AUTOMATIC_ESTIMATION_STATE, VI_TRUE),
					3, "Estimation Mode");
		break;

		case RSSPECAN_VAL_VSA_ESTIMATION_SYMBOL_1:
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_AUTOMATIC_ESTIMATION_STATE, VI_FALSE),
					3, "Estimation Mode");

			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_ESTIMATION_VALUE, 1),
					3, "Estimation Mode");
		break;

		case RSSPECAN_VAL_VSA_ESTIMATION_SYMBOL_2:
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_AUTOMATIC_ESTIMATION_STATE, VI_FALSE),
					3, "Estimation Mode");

			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_ESTIMATION_VALUE, 2),
					3, "Estimation Mode");
		break;

		case RSSPECAN_VAL_VSA_ESTIMATION_CAPTURE_OV:
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_AUTOMATIC_ESTIMATION_STATE, VI_FALSE),
					3, "Estimation Mode");

			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_ESTIMATION_VALUE, 4),
					3, "Estimation Mode");
		break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, selectsEstimationPointsSym), 3, "Estimation Mode");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the measurement filter.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR autoMeasurementFilter/Sets wheater the measurement filter is defined automatically depending
/// HIPAR autoMeasurementFilter/on the TX filter (when set to On).
/// HIPAR type/Defines the measurement filter type, if the Auto setting is not
/// HIPAR type/enabled.
/// HIPAR alphaBT/Defines the roll-off factor (Alpha) or the filter bandwidth (BT).
ViStatus _VI_FUNC rsspecan_ConfigureVSAMeasurementFilter (ViSession instrSession,
                                                          ViInt32 window,
                                                          ViBoolean autoMeasurementFilter,
                                                          ViInt32 type,
                                                          ViReal64 alphaBT)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_AUTO_MEASUREMENT_FILTER, autoMeasurementFilter),
			3, "Auto Measurement Filter");

	if (autoMeasurementFilter == VI_FALSE)
	{
		if (type == RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_NONE)
		{
			viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_STATE, VI_FALSE),
					6, "");
		}
		else
		{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_TYPE, type),
					4, "Type");

			if (type <= RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_RRC)
			{
				viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_ALPHA, alphaBT),
						5, "Alpha BT");
			}
 		}
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function selects the user-defined measurement filter.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR filterFileName/The name of the user-defined filter
ViStatus _VI_FUNC rsspecan_ConfigureVSAUserDefinedMeasurementFilter(
	ViSession	instrSession,
	ViInt32		window,
	ViString	filterFileName
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	if (filterFileName == NULL)
	{
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_STATE, VI_FALSE));
	}
    else
	{
		checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_STATE, VI_TRUE));
		checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_TYPE, RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_USER));

		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_USER_FILE, filterFileName),
				3, "Filter File Name");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the filter characteristic Alpha/BT.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR alphaBt/Sets the filter characteristic (ALPHA/BT). The resolution is 0.05.
ViStatus _VI_FUNC rsspecan_ConfigureVSAFilterCharacteristic(ViSession   instrSession,
                                                ViInt32     window,
                                                ViReal64    alphaBt)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_MODULATION_ALPHABT, alphaBt),
    		3, "Alpha/BT");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function enables or disables selected marker.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR markerNumber/Selects the marker to configure.
/// HIPAR markerState/Enables or disables the selected marker.
ViStatus _VI_FUNC rsspecan_ConfigureVSAMarkerState (ViSession instrSession,
                                                    ViInt32 window,
                                                    ViInt32 markerNumber,
                                                    ViBoolean markerState)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		3, "Marker Number");

    sprintf (buffer, "Win%ld,M%ld", window, markerNumber);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_MARKER_ENABLED, markerState),
    		4, "Marker State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function couples markers between several screens, i.e. uses the
/// HIFN  same stimulus. All screens can be linked with an X-axis scaled in
/// HIFN  symbols or time, except those showing the capture buffer.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR markerNumber/Selects the marker to configure.
/// HIPAR state/Specifies the state of the screen coupling.
ViStatus _VI_FUNC rsspecan_ConfigureVSAScreenCoupling (ViSession instrSession,
                                                       ViInt32 window,
                                                       ViInt32 markerNumber,
                                                       ViBoolean state)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		3, "Marker Number");

	sprintf (buffer, "Win%ld,M%ld", window, markerNumber);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SCREEN_COUPLING, state),
    		4, "Marker State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures whether the marker search works on the real
/// HIFN  or the imag trace.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR search/Specifies whether the marker search works on the real or the imag
/// HIPAR search/trace.
ViStatus _VI_FUNC rsspecan_ConfigureVSAMarkerSearch (ViSession instrSession,
                                                     ViInt32 window,
                                                     ViInt32 search)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_MARKER_SEARCH, search),
			2, "Search");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function searches for the max absolute peak of a trace
/// HIFN  (independent of its sign). This is especially usefull for measurements
/// HIFN  where a zero value is best, and either large positive or large negative
/// HIFN  values indicate a bad signal.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR markerNumber/Selects the marker to configure.
ViStatus _VI_FUNC rsspecan_ConfigureVSAMarkerPeakSearch (ViSession instrSession,
                                                         ViInt32 window,
                                                         ViInt32 markerNumber)

{
    ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		3, "Marker Number");

	sprintf (buffer, "Win%ld,M%ld", window, markerNumber);

	checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_VSA_MARKER_PEAK_SEARCH, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function defines the unit of the full scale level.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR fullScaleLevelUnit/Defines the unit of the full scale level.
ViStatus _VI_FUNC rsspecan_ConfigureVSAFullScaleLevelUnit
             (ViSession instrSession, ViInt32 fullScaleLevelUnit)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_FULL_SCALE_LEVEL_UNIT, fullScaleLevelUnit),
    		2, "Full Scale Level Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function adjusts the reference level for digital input to the
/// HIFN  full scale level automatically if the fullscale level changes.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR adjustReferenceLevel/If enabled, the reference level for digital input is adjusted to the
/// HIPAR adjustReferenceLevel/full scale level automatically if the fullscale level changes.
ViStatus _VI_FUNC rsspecan_VSAAdjustReferenceLevelToFullScaleLevel
             (ViSession instrSession, ViBoolean adjustReferenceLevel)

{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_VSA_ADJUST_REFERENCE_LEVEL_TO_FULL_SCALE_LEVEL, adjustReferenceLevel),
    		2, "Adjust Reference Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
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
ViStatus _VI_FUNC rsspecan_ConfigureVSATraceEval (ViSession instrSession,
                                                        ViInt32 window,
														ViInt32 trace,
                                                        ViInt32 traceEvaluation)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 6),
			3, "Trace");

    sprintf (buffer, "C%ld,TR%ld", window, trace);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_TRACE_EVALUATION, traceEvaluation),
			3, "Trace Evaluation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function control the zero point and scaling of the y
/// HIFN     axis for I/Q, statistical and time displays:.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR scaling/Defines the scaling of the Y axis.
/// HIPAR referenceValue/Defines the reference value for the Y axis of the measurement diagram.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayYAxis(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViReal64    scaling,
                                                    ViReal64    referenceValue)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_SCALE_Y_PDIV, scaling),
    		4, "Scaling");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_DISP_REF_VALUE, referenceValue),
    		5, "Reference Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the y-axis ranges for statistical
/// HIFN  measurements. Use function
/// HIFN  rsspecan_ConfigureVSAResultTypeTransformation to enable
/// HIFN  statistical measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR yUnit/Toggles the scaling of Y axis between percentage and absolute.
/// HIPAR yAxisMaxValue/Defines the upper limit for the y-axis of the diagram in
/// HIPAR yAxisMaxValue/statistical measurements.
/// HIPAR yAxisMinValue/Defines the lower limit for the y-axis of the diagram in
/// HIPAR yAxisMinValue/statistical measurements.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayYAxisStatisticMeasurement (ViSession instrSession,
                                                                           ViInt32 yUnit,
																		   ViReal64 yAxisMaxValue,
                                                                           ViReal64 yAxisMinValue)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));  // TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_UNIT, yUnit),
    		2, "Y Unit");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_MAX, yAxisMaxValue),
    		3, "Y Axis Max Value");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_Y_MIN, yAxisMinValue),
    		4, "Y Axis Min Value");

Error:
	return error;
}

/// HIFN  This function controls the reference position of Y axis.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR referencePosition/Defines the position of the reference value for the X axis.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayYAxisRefPosition(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViReal64    referencePosition)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_DISP_REF_POSITION, referencePosition),
    		3, "Reference Position");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function defines the scaling of the x axis for statistical
/// HIFN  displays.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR quantize/Defines defines the number of bars for the statistical functions.
/// HIPAR scaling/Defines the scaling of the X axis.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayXAxis(ViSession   instrSession,
                                                    ViInt32 window,
                                                    ViInt32 trace,
                                                    ViInt32 quantize,
                                                    ViInt32 scaling)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 1),
    		3, "Trace");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_SCALE_X_BAR_COUNT, quantize),
    		4, "Quantize");
    sprintf (buffer, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SCALE_X_PDIV, scaling),
    		5, "Scaling");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function defines the scaling referenece value of the x axis for
/// HIFN  I/Q and statistical displays.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR referenceValue/Defines the reference value for the X axis of the measurement diagram.
/// HIPAR referencePosition/Defines the position of the reference value for the X axis.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayXAxisReference(ViSession  instrSession,
                                                    ViInt32 window,
                                                    ViInt32 trace,
                                                    ViInt32 referenceValue,
                                                    ViInt32 referencePosition)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 1),
    		3, "Trace");

    sprintf (buffer, "Win%ld,TR%ld", window, trace);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SCALE_X_RVAL, referenceValue),
    		4, "Reference Value");
    sprintf (buffer, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_SCALE_X_RPOS, referencePosition),
    		5, "Reference Position");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the default unit (symbols or seconds) for the
/// HIFN  x axis.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR unit/Sets the time unit.
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitTime(ViSession   instrSession,
                                                ViInt32     window,
                                                ViInt32     unit)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_UNIT_TIME, unit),
    		3, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function sets the displaying of the trace.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR unit/Switches between a linear in %, linear in dB and a logarithmic scale in the
/// HIPAR unit/selected measurement window.
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitYAxis(ViSession   instrSession,
                                        ViInt32     window,
                                        ViInt32     trace,
                                        ViInt32     unit)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 1),
    		3, "Trace");

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_TRACE_DISPLAY_SPACING, unit),
    		4, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function the default unit for angles.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR unitAngle/Sets the default unit for angles.
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitAngle(ViSession  instrSession,
                                                ViInt32     window,
                                                ViInt32     unitAngle)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_UNIT_ANGLE, unitAngle),
    		3, "Unit Angle");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function transfers the following from other FSQ analyzers or from
/// HIFN  external programs (MAPWIZ, FILTWIZ) via file operations:
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR type/Selects the item to be import.
/// HIPAR name/Selects name of external file.
/// HIPAR path/Selects path to which the internal files are copied
ViStatus _VI_FUNC rsspecan_VSAImport(ViSession  instrSession,
                                    ViInt32     type,
                                    ViString    name,
                                    ViString    path)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV (instrSession))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_VSA_IMPEXP_STANDARDS, RSSPECAN_VAL_VSA_IMPEXP_EQUALIZERS),
			2, "Type");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, name), 3, "Name");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, path), 4, "Path");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "DDEM:IMP:%s '%s','%s'", VSAimpExpTypeArr[type], name, path);
    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function transfers the following from other R&S FSQ analyzers or
/// HIFN  from external programs (MAPWIZ, FILTWIZ) via file operations:
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR type/Selects the item to be import.
/// HIPAR name/Selects name of internal file.
/// HIPAR path/Selects path to which the internal files are copied
ViStatus _VI_FUNC rsspecan_VSAExport(ViSession  instrSession,
                                    ViInt32     type,
                                    ViString    name,
                                    ViString    path)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV (instrSession))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_VSA_IMPEXP_STANDARDS, RSSPECAN_VAL_VSA_IMPEXP_EQUALIZERS),
			2, "Type");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, name), 3, "Name");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, path), 4, "Path");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "DDEM:EXP:%s '%s','%s'", VSAimpExpTypeArr[type], name, path);
    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function exports the data to file in defined format.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR decimalSeparator/Defines which decimal separator (decimal point or comma) is to be used
/// HIPAR decimalSeparator/for outputting measurement data to the file in ASCII format.
/// HIPAR header/Defines if a file header (including start frequency, sweep time,
/// HIPAR header/detector, etc.) is created or not. A small header with the instrument
/// HIPAR header/model, the version and the date is always transferred.
/// HIPAR dataMode/Defines which data are transferred, raw I/Q data or trace data.
/// HIPAR dataRaw/Defines the output format of the RAW data file export function. The
/// HIPAR dataRaw/driver uses this value to set the
/// HIPAR dataRaw/RSSPECAN_ATTR_VSA_FILE_DATA_RAW_MODE attribute.
/// HIPAR fileName/Specifies full path of the file, where trace data will be stored.
ViStatus _VI_FUNC rsspecan_ConfigureVSAFileExport(ViSession instrSession,
                                                ViInt32     window,
                                                ViInt32     trace,
                                                ViInt32     decimalSeparator,
                                                ViBoolean   header,
                                                ViInt32     dataMode,
                                                ViInt32     dataRaw,
                                                ViString    fileName)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 3),
    		3, "Trace");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, fileName), 7, "File Name");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_FILE_DEC_SEPARATOR, decimalSeparator),
    		4, "Decimal Separator");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_VSA_FILE_EXP_HEADER, header),
    		5, "Header");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_FILE_DATA_MODE, dataMode),
    		6, "Data Mode");

    if ((dataMode == RSSPECAN_VAL_EXP_MODE_RAW) && (!rsspecan_IsFSV (instrSession)))
    {    viCheckParm(rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_VSA_FILE_DATA_RAW_MODE,
                 dataRaw), 7, "Data Raw");
    }
    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "FORM ASC;MMEM:STOR%ld:TRAC %ld,'%s'", window, trace, fileName);
    checkErr(RsCore_Write(instrSession, cmd));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function loads I/Q RAW data files. The file name includes
/// HIFN  indication of the path and the drive name. The path name complies with
/// HIFN  DOS conventions. This function is only available if RAW data with
/// HIFN  binary format is selected (":FORM:DEXP:MODE RAW",  ":FORM:DEXP:RAW:FORM
/// HIFN  BIN" ).
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR fileName/Specifies full path of the file, where trace data are stored.
ViStatus _VI_FUNC rsspecan_ConfigureVSAFileImport (ViSession instrSession,
                                                   ViString fileName)
{
	ViStatus	error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	if (rsspecan_IsFSV (instrSession))
		checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidNullPointer(instrSession, fileName), 2, "File Name");

	checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_FILE_DATA_MODE, RSSPECAN_VAL_EXP_MODE_RAW));
	checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_VSA_FILE_DATA_RAW_MODE, RSSPECAN_VAL_EXP_RAW_MODE_BIN));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "MMEM:LOAD:TRAC 1,'%s'", fileName);
    checkErr(RsCore_Write(instrSession, cmd));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function restores the default limits and deactivates all checks.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR type/Selects the limit type.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsDefault(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		type
)
{
    ViStatus error = VI_SUCCESS;
	window;
	type;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, NULL, RSSPECAN_ATTR_VSA_LIMIT_MODULATION_ACCURACY_DEFAULTS, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for EVM limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR evmRMS/EVM RMS limit value.
/// HIPAR checkRMS/Switches the limit check On or Off.
/// HIPAR evmPeak/EVM Peak limit value.
/// HIPAR checkPeak/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsEVM(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	evmRMS,
	ViBoolean	checkRMS,
	ViReal64	evmPeak,
	ViBoolean	checkPeak
)
{
    ViStatus error = VI_SUCCESS;
	ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
			3, "Limit Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "EVM", VSALimitTypeArr[limitType+6]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkRMS),
			5, "Check RMS");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+6]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_EVM, evmRMS),
    		4, "EVM RMS");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "EVM", VSALimitTypeArr[limitType+3]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkPeak),
			7, "Check Peak");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+3]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_EVM, evmPeak),
    		6, "EVM Peak");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for phase error limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR phaseErrorRMS/Phase error RMS limit value.
/// HIPAR checkRMS/Switches the limit check On or Off.
/// HIPAR phaseErrorPeak/Phase error peak limit value.
/// HIPAR checkPeak/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsPhaseError(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	phaseErrorRMS,
	ViBoolean	checkRMS,
	ViReal64	phaseErrorPeak,
	ViBoolean	checkPeak
)
{
    ViStatus error = VI_SUCCESS;
	ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
			3, "Limit Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "PhaseErr", VSALimitTypeArr[limitType+6]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkRMS),
			5, "Check RMS");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+6]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_PHASE_ERROR, phaseErrorRMS),
    		4, "Phase Error RMS");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "PhaseErr", VSALimitTypeArr[limitType+3]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkPeak),
			7, "Check Peak");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+3]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_PHASE_ERROR, phaseErrorPeak),
    		6, "Phase Error Peak");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for magnitude error limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR magnitudeErrorRMS/Magnitude error RMS limit value.
/// HIPAR checkRMS/Switches the limit check On or Off.
/// HIPAR magnitudeErrorPeak/Magnitude error peak limit value.
/// HIPAR checkPeak/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsMagnitudeError(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	magnitudeErrorRMS,
	ViBoolean	checkRMS,
	ViReal64	magnitudeErrorPeak,
	ViBoolean	checkPeak
)
{
    ViStatus error = VI_SUCCESS;
	ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
			3, "Limit Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "MagnErr", VSALimitTypeArr[limitType+6]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkRMS),
			5, "Check RMS");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+6]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_MAGNITUDE_ERROR, magnitudeErrorRMS),
    		4, "Magnitude Error RMS");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "MagnErr", VSALimitTypeArr[limitType+3]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkPeak),
			7, "Check Peak");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+3]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_MAGNITUDE_ERROR, magnitudeErrorPeak),
    		6, "Magnitude Error Peak");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for carrier frequency error limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR carrierFrequencyError/Carrier frequency error limit value.
/// HIPAR check/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsCarrierFrequencyError(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	carrierFrequencyError,
	ViBoolean	check
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
    		3, "Limit Type");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "CFErr", VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, check),
    		5, "Check");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_CARRIER_FREQUENCY_ERROR, carrierFrequencyError),
    		4, "Ccarrier Frequency Error");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for RHO limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR rho/RHO limit value.
/// HIPAR check/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsRHO(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	rho,
	ViBoolean	check
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
    		3, "Limit Type");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "RHO", VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, check),
    		5, "Check");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_RHO, rho),
    		4, "RHO");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for IQ Offset limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR iqOffset/IQ offset limit value.
/// HIPAR check/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsIQOffset(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	iqOffset,
	ViBoolean	check
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
    		3, "Limit Type");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window,  "IQOffset", VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, check),
    		5, "Check");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_IQ_OFFSET, iqOffset),
    		4, "IQ Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for frequency error limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR frequencyErrorRMS/Frequency error RMS limit value
/// HIPAR checkRMS/Switches the limit check On or Off.
/// HIPAR frequencyErrorPeak/Frequency error Peak limit value.
/// HIPAR checkPeak/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsFrequencyError(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	frequencyErrorRMS,
	ViBoolean	checkRMS,
	ViReal64	frequencyErrorPeak,
	ViBoolean	checkPeak
)
{
    ViStatus error = VI_SUCCESS;
	ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
			3, "Limit Type");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "FreqErr", VSALimitTypeArr[limitType+6]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkRMS),
			5, "Frequency Error RMS");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+6]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_FREQUENCY_ERROR, frequencyErrorRMS),
    		4, "Frequency Error RMS");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "FreqErr", VSALimitTypeArr[limitType+3]);
	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, checkPeak),
			7, "Check Peak");

   snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType+3]);
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_FREQUENCY_ERROR, frequencyErrorPeak),
			6, "Frequency Error Peak");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the value for center frequency deviation
/// HIFN  error limits.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR limitType/Selects the limit with which the currently measured, mean or peak
/// HIPAR limitType/value is to be compared.
/// HIPAR freqDevError/Center frequency deviation error limit value.
/// HIPAR check/Switches the limit check On or Off.
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsFrequencyDeviationError(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		limitType,
	ViReal64	freqDevError,
	ViBoolean	check
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, RSSPECAN_VAL_VSA_LIMIT_PEAK),
    		3, "Limit Type");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, "FreqDevErr", VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE, check),
    		5, "Check");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s", window, VSALimitTypeArr[limitType]);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_FREQUENCY_DEVIATION_ERROR, freqDevError),
    		4, "Frequency Deviation Error");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function activates/deactivates the window.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR windowState/This control activates/deactivates the window.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayWindowState
             (ViSession instrSession, ViInt32 window, ViBoolean windowState)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_DISPLAY_STATE, windowState),
			3, "Window State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the display of the decision instants (time
/// HIFN  when the signals occurred) on the trace.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR highlightSymbols/If enabled, the symbol instants are highlighted as squares in the
/// HIPAR highlightSymbols/screen for measured and reference signals in time (normal) display, as
/// HIPAR highlightSymbols/well as error displays.
ViStatus _VI_FUNC rsspecan_ConfigureVSAHighlightSymbols(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32 	trace,
	ViBoolean	highlightSymbols
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 1),
    		3, "Trace");

    sprintf (buffer, "Win%ld,TR%ld", window, trace);

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_TRACE_SYMBOL_STATE, highlightSymbols),
			4, "Highlight Symbols");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the VSA measurement or reference signal
/// HIFN  result displays.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Sets the displayed measurement.
ViStatus _VI_FUNC rsspecan_ConfigureVSACaptureBuffer(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		resultType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "",
				buffer2[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
	sprintf (buffer2, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_TCAP));

	switch(resultType)
	{
		case RSSPECAN_VAL_VSA_RESULT_DISP_MAG_ABS:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_RI:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
		break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ABS:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, RSSPECAN_VAL_ABS));
		break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_IQ_VECT:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_COMP));
		break;
		case RSSPECAN_VAL_VSA_RESULT_DISP_MOV:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_MOV));
		break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 3, "Result Type");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the VSA measurement result displays.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Sets the displayed measurement.
ViStatus _VI_FUNC rsspecan_ConfigureVSAMeasurementSignal(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		resultType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "",
				buffer2[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
	sprintf (buffer2, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_XTIM_DDEM_MEAS));

	switch(resultType)
	{
        case RSSPECAN_VAL_VSA_RESULT_DISP_MAG_ABS:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, RSSPECAN_VAL_ABS));
        break;

        case RSSPECAN_VAL_VSA_RESULT_DISP_MAG_REL:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, RSSPECAN_VAL_REL));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_PHA_WRAP:
             checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS));
        break;

        case RSSPECAN_VAL_VSA_RESULT_DISP_PHA_UNWRAP:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_UPHAS));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ABS:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, RSSPECAN_VAL_ABS));
        break;

        case RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_REL:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE, RSSPECAN_VAL_REL));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_RI:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_EYE_R:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_IEYE));
        break;

        case RSSPECAN_VAL_VSA_RESULT_DISP_EYE_I:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_QEYE));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_EYE_FREQ:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_FEYE));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_IQ_CONST:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_CONS));
        break;

        case RSSPECAN_VAL_VSA_RESULT_DISP_IQ_VECT:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_COMP));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_CONST_FREQ:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_CONF));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_VECT_FREQ:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_CONVF));
        break;
		case RSSPECAN_VAL_VSA_RESULT_DISP_IQ_CON_ROT:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_RCON));
        break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 3, "Result Type");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the VSA measurement or reference signal
/// HIFN  result displays.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Sets the displayed measurement.
ViStatus _VI_FUNC rsspecan_ConfigureVSASymbols(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	resultType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	resultType;

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
	checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_XTIM_DDEM_SYMB));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the VSA measurement or reference signal
/// HIFN  result displays.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_VSA_RESULT
/// HIFN     RSSPECAN_ATTR_VSA_RESULT_FORMAT
/// HIFN     RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE
/// HIFN     RSSPECAN_ATTR_VSA_SPECTRUM_STATE
/// HIFN     RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     CALCulate<screen>:FEED 'XTIM:DDEM:ERR:VECT'
/// HIFN     CALCulate<1|2>:FORMat
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Sets the displayed measurement.
ViStatus _VI_FUNC rsspecan_ConfigureVSAErrorVector(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	resultType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "",
				buffer2[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
	sprintf (buffer2, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_VECT));

	switch(resultType)
	{
		case RSSPECAN_VAL_VSA_RESULT_DISP_EVM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_RI:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_IQ_VECT:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_COMP));
        break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 3, "Result Type");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the VSA measurement or reference signal
/// HIFN  result displays.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Sets the displayed measurement.
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationErrors(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	resultType
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "",
				buffer2[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
	sprintf (buffer2, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_MPH));

	switch(resultType)
	{
		case RSSPECAN_VAL_VSA_RESULT_DISP_MAG_ERR:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_PHA_ERR:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS));
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ERR_ABS:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
			checkErr(rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_ABS))
        break;

		case RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ERR_REL:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ));
			checkErr(rsspecan_SetAttributeViInt32 (instrSession, buffer2,
                                                      RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE,
                                                      RSSPECAN_VAL_REL))
        break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 3, "Result Type");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the VSA signal source to modulation accuracy.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationAccuracy
             (ViSession instrSession, ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "C%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_MACC),
			3, "");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
/// HIFN  This function configures the VSA signal source to modulation accuracy.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Sets the displayed measurement.
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationAccuracyAdditional (ViSession instrSession,
                                                                     ViInt32 window,
                                                                     ViInt32 resultType)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "",
				buffer2[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "C%ld", window);
	sprintf (buffer2, "Win%ld", window);

	checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_RESULT, RSSPECAN_VAL_RESULT_MACC));

	switch(resultType)
	{
		case RSSPECAN_VAL_VSA_RESULT_FORMAT_RSUM:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_RSUM));
        break;

		case RSSPECAN_VAL_VSA_RESULT_FORMAT_BER:
			checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer2, RSSPECAN_ATTR_VSA_RESULT_FORMAT, RSSPECAN_VAL_VSA_RESULT_FORMAT_BER));
        break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultType), 3, "Result Type");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/// HIFN  This function configures the evaluation method of the
/// HIFN  measurement results.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window
/// HIPAR transformation/Sets the result type transformation

ViStatus _VI_FUNC rsspecan_ConfigureVSAResultTypeTransformation(
	ViSession instrSession,
	ViInt32   window,
	ViInt32   transformation

)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	sprintf (buffer, "C%ld", window);

	switch (transformation)
	{
		case RSSPECAN_VAL_VSA_TRANSFORMATION_NORMAL:
			checkErr(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, VI_FALSE));
			checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_FALSE));
		break;

		case RSSPECAN_VAL_VSA_TRANSFORMATION_SPEC:
			 checkErr(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, VI_TRUE));
			checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_FALSE));
		break;

		case RSSPECAN_VAL_VSA_TRANSFORMATION_STAT:
			checkErr(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_VSA_SPECTRUM_STATE, VI_FALSE));
			checkErr(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE, VI_TRUE));
		break;

		default:
			viCheckParm(RsCore_InvalidViInt32Value(instrSession, transformation), 3, "Transformation");
		break;
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures whether only the symbol points or all points
/// HIFN  are considered for the statistical calculations.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR oversampling/This control sets the oversampling state.
/// HIPAR statisticalMode/This control defines whether only the symbol points or all points are
/// HIPAR statisticalMode/considered for the statistical calculations.
ViStatus _VI_FUNC rsspecan_ConfigureVSACaptureOversampling
             (ViSession instrSession, ViInt32 window, ViInt32 oversampling,
              ViInt32 statisticalMode)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_POINTS_PER_SYMBOL_AUTO, oversampling),
			3, "Oversampling");

	if (oversampling == RSSPECAN_VAL_MAN)
	{
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_CAPTURE_OVERSAMPLING, statisticalMode),
					4, "Statistical Mode");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the points per symbol settings.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR pointsPerSymbol/This control turns auto mode of points per symbol on or off.
/// HIPAR numberOfPointsPerSymbol/This control determines the number of points to be displayed per
/// HIPAR numberOfPointsPerSymbol/symbol if manual mode is selected.
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayPointPerSymbol
             (ViSession instrSession, ViInt32 window, ViInt32 pointsPerSymbol,
              ViInt32 numberOfPointsPerSymbol)

{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_POINTS_PER_SYMBOL_AUTO, pointsPerSymbol),
			3, "Points Per Symbol");

	if (pointsPerSymbol == RSSPECAN_VAL_MAN) {
			viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_POINTS_PER_SYMBOL, numberOfPointsPerSymbol),
					4, "number Of Points Per Symbol");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function repeats the evaluation of the data currently in the
/// HIFN  capture buffer without capturing new data. This is useful after
/// HIFN  changing settings, for example filters, patterns or evaluation ranges.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_VSARefreshCapturedData(
	ViSession	instrSession
)
{
    ViStatus error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_VSA_REFRESH_CAPTURED_DATA, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function returns the first value of the X axis in symbols or
/// HIFN  time, depending on the unit setting for the X axis.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR trace/Selects the trace for Detector definition.
/// HIPAR result/Returns the first X-Axis value.
ViStatus _VI_FUNC rsspecan_GetVSAXAxisFirstValue(ViSession  instrSession,
                                                ViInt32     window,
                                                ViInt32     trace,
                                                ViReal64*   result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, trace, 1, 6),
    		3, "Trace");
    sprintf (buffer, "Win%ld,TR%ld", window, trace);
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_X_FIRST_VALUE_QUERY, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function returns the results of the error measurement carried out
/// HIFN  for digital demodulation.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR type/This control sets the result.
/// HIPAR modifier/This control sets the result. Available values depend on instrument
/// HIPAR modifier/used, refer to the notes below.
/// HIPAR result/Returns the measured result.
ViStatus _VI_FUNC rsspecan_GetVSAResult(ViSession   instrSession,
                                        ViInt32     window,
                                        ViInt32     type,
                                        ViInt32     modifier,
                                        ViReal64*   result)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_VSA_MPOW, RSSPECAN_VAL_VSA_SR_ERROR),
    		3, "Type");

    if (!rsspecan_IsFSV (instrSession))

	{
		switch(type){
	        case RSSPECAN_VAL_VSA_MPOW:
	            viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_VSA_MODIF_NONE, RSSPECAN_VAL_VSA_MODIF_ASYM),
	            		4, "Modifier");
	        break;
	        case RSSPECAN_VAL_VSA_DTTS:
	        case RSSPECAN_VAL_VSA_ADR:
	        case RSSPECAN_VAL_VSA_FSK_CDR:
	        case RSSPECAN_VAL_VSA_FSK_MDEV:
	        case RSSPECAN_VAL_VSA_MERR:
	        case RSSPECAN_VAL_VSA_OOFF:
	        case RSSPECAN_VAL_VSA_PLER:
	        case RSSPECAN_VAL_VSA_QERR:
	        case RSSPECAN_VAL_VSA_RHO:
	        case RSSPECAN_VAL_VSA_SNR:
	        case RSSPECAN_VAL_VSA_IQIM:
	        case RSSPECAN_VAL_VSA_GIMB:
	            viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_VSA_MODIF_NONE, RSSPECAN_VAL_VSA_MODIF_SDEV),
	            		4, "Modifier");
	        break;
	        case RSSPECAN_VAL_VSA_CFER:
	        case RSSPECAN_VAL_VSA_FSK_DERR:
	        case RSSPECAN_VAL_VSA_PERR:
	            viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_VSA_MODIF_NONE, RSSPECAN_VAL_VSA_MODIF_TPE),
	            		4, "Modifier");
	        break;
	        case RSSPECAN_VAL_VSA_EVM:
	            viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_VSA_MODIF_NONE, RSSPECAN_VAL_VSA_MODIF_PCTL),
	            		4, "Modifier");
	        break;
			default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 4, "Modifier");
	    }
    }
	else  // FSV instrument
	{
		switch(type){
			case RSSPECAN_VAL_VSA_MPOW:
			case RSSPECAN_VAL_VSA_ADR:
			case RSSPECAN_VAL_VSA_OOFF:
			case RSSPECAN_VAL_VSA_QERR:
			case RSSPECAN_VAL_VSA_RHO:
			case RSSPECAN_VAL_VSA_IQIM:
			case RSSPECAN_VAL_VSA_GIMB:
			case RSSPECAN_VAL_VSA_CFER:
			case RSSPECAN_VAL_VSA_DTTS:
			case RSSPECAN_VAL_VSA_FSK_CDR:
			case RSSPECAN_VAL_VSA_FSK_MDEV:
			case RSSPECAN_VAL_VSA_PLER:
			case RSSPECAN_VAL_VSA_FDER:
			case RSSPECAN_VAL_VSA_FSK_RDEV:
	        case RSSPECAN_VAL_VSA_SR_ERROR:
			    viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_VSA_MODIF_FSV_CURR, RSSPECAN_VAL_VSA_MODIF_FSV_PCTL),
			    		4, "Modifier");
	        break;
			case RSSPECAN_VAL_VSA_EVM:
			case RSSPECAN_VAL_VSA_SNR:
			case RSSPECAN_VAL_VSA_MERR:
			case RSSPECAN_VAL_VSA_PERR:
			case RSSPECAN_VAL_VSA_FSK_DERR:
				viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_VSA_MODIF_FSV_CURR, RSSPECAN_VAL_VSA_MODIF_FSV_PPCT),
						4, "Modifier");
			break;
			default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 4, "Modifier");
	 	}
	}

	viCheckParm(RsCore_InvalidNullPointer(instrSession, result), 5, "Result");

    if ((modifier == RSSPECAN_VAL_VSA_MODIF_NONE) || (modifier == RSSPECAN_VAL_VSA_MODIF_FSV_CURR))
	{
        snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:%s?", window, VSAResultArr[type]);
        checkErr(RsCore_QueryViReal64(instrSession, cmd, result));
	}
    else
	{
        checkErr(viQueryf(instrSession, "CALC%ld:MARK:FUNC:%s? %s\n", "%le", window,
			                                               VSAResultArr[type], VSAModifierArr[modifier], result));
	}

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries all results of the result summary.
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR results/Returns all result summary results.
ViStatus _VI_FUNC rsspecan_GetVSAAllResults (ViSession instrSession,
                                             ViInt32 window,
                                             ViReal64 results[])
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViChar      *pbuffer = NULL,
                *p2buf = NULL,
                *p2buf2 = NULL;
	ViInt32     i;

    checkErr(RsCore_LockSession(instrSession));

    if (!rsspecan_IsFSV (instrSession))
        checkErr(RS_ERROR_INSTRUMENT_MODEL);

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K70"));

	viCheckParm(RsCore_InvalidNullPointer(instrSession, results), 3, "Results");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK:FUNC:DDEM:STAT:ALL?", window);
	checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &pbuffer)); // TODO: Check the response processing
    p2buf = pbuffer;
    p2buf2 = pbuffer;
    i=0;
    while (p2buf2){
        p2buf2=strchr (p2buf, ',');
        if (((p2buf2-p2buf)==0)||(strlen(p2buf)<=1))
        {
                results[i]=RS_VAL_NAN_VI_REAL64;
        }
        else
        {
            sscanf (p2buf, "%le", &results[i]);
        }
        i++;
        if (p2buf2)
        {
            p2buf = p2buf2+1;
        }
    }

checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function returns all the measured data that relates to the
/// HIFN  currently selected measurement type. The returned data is particular to
/// HIFN  the currently selected measurement type and is specified in the Trace
/// HIFN  Data parameter description.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR sourceTrace/Select trace to read.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns an array of trace data. Number and format of the measurement
/// HIPAR traceData/values for operating mode VSA:
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadVSATraceData(ViSession   instrSession,
                                            ViInt32     window,
                                            ViInt32     sourceTrace,
                                            ViInt32     arraySize,
                                            ViReal64    traceData[],
                                            ViInt32*    noofValues)
{
    ViStatus    error = VI_SUCCESS;
	ViInt32     maxTrace = RSSPECAN_VAL_IQC_TRACE_X;

    checkErr(RsCore_LockSession(instrSession));

	if (strstr (model, "FSQ") != NULL) maxTrace = RSSPECAN_VAL_TRACE_4;

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, sourceTrace, RSSPECAN_VAL_TRACE_1, maxTrace),
    		3, "Source Trace");

    checkErr(rsspecan_dataReadTrace (instrSession,window, VSATraceArr[sourceTrace], arraySize,
                    traceData, noofValues));
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function queries the BER results.
/// HIRET Returns the status code of this operation.
/// HIRET You may use multiple lines for documentation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR window/This control selects the measurement window.
/// HIPAR resultType/Selects the result type.
/// HIPAR bitErrorRate/Returns the Bit Error Rate (error bits / number of analyzed bits)
/// HIPAR numberOfErrors/Returns the number of analyzed errors.
/// HIPAR numberOfBits/Returns the number of analyzed bits.
ViStatus _VI_FUNC rsspecan_QueryVSABERResults (ViSession instrSession,
                                               ViInt32 window,
                                               ViInt32 resultType,
                                               ViReal64 *bitErrorRate,
                                               ViInt32 *numberOfErrors,
                                               ViInt32 *numberOfBits)
{
	ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, RSSPECAN_VAL_VSA_BER_RESULT_CURRENT, RSSPECAN_VAL_VSA_BER_RESULT_ACCUMULATIVE),
			3, "Result Type");

	sprintf (buffer, "C%ld,%s", window, BERResultTypeArr[resultType]);

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_VSA_BER_BIT_ERROR_RATE, bitErrorRate),
			4, "Bit Error Rate");

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BER_NUMBER_OF_ERRORS, numberOfErrors),
			5, "Number Of Errors");

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_VSA_BER_NUMBER_OF_BITS, numberOfBits),
			6, "Number Of Bits");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function queries whether the limit for the specified result type
/// HIFN  and limit type was violated.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR measurementType/Selects the measurement limit result.
/// HIPAR limitType/Selects the limit type.
/// HIPAR checkResults/Returns limit state code.
ViStatus _VI_FUNC rsspecan_QueryVSAModulationAccuracyLimitsCheckResults(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		measurementType,
	ViInt32		limitType,
	ViInt32*	checkResults
)
{
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE] = "";
	ViInt32		maxStat= RSSPECAN_VAL_VSA_LIMIT_PEAK_PEAK;

    checkErr(RsCore_LockSession(instrSession));

	if((measurementType == RSSPECAN_VAL_VSA_RESULT_RHO) ||
	   (measurementType == RSSPECAN_VAL_VSA_RESULT_CF_ERROR) ||
	   (measurementType == RSSPECAN_VAL_VSA_RESULT_IQ_OFFSET) ||
	   (measurementType == RSSPECAN_VAL_VSA_RESULT_FSK_DEV_ERROR))
	{
		maxStat = RSSPECAN_VAL_VSA_LIMIT_PEAK;
	}

   	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, RSSPECAN_VAL_VSA_RESULT_EVM, RSSPECAN_VAL_VSA_RESULT_FSK_DEV_ERROR),
   			3, "Measurement Type");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, limitType, RSSPECAN_VAL_VSA_LIMIT_CURRENT, maxStat),
			4, "Limit Type");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,%s,%s", window, VSAResultTypeArr[measurementType], VSALimitTypeArr[limitType]);

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_VSA_QUERY_MOD_ACC_LIMIT_CHECK_RESULT, checkResults),
			5, "Check Results");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries the modulation accuracy statisitc result.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/Selects the marker to configure.
/// HIPAR measurementType/Selects the measurement result.
/// HIPAR statistic/Selects the queried statisitc.
/// HIPAR value/Returns statistc value.
ViStatus _VI_FUNC rsspecan_QueryVSAModulationAccuracyStatisticResults(
	ViSession	instrSession,
	ViInt32		window,
	ViInt32		marker,
	ViInt32		measurementType,
	ViInt32		statistic,
	ViReal64*	value
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32		maxStat= RSSPECAN_VAL_VSA_STAT_95_RMS;
	ViString	measTypeArr[] = {"EVM", "PERR", "MERR", "CFER", "RHO", "OOFF", "FSK:DERR", "FDER", "FSK:MDEV", "FSK:RDEV",
							 	 "MPOW", "FSK:CFDR", "SNR", "IQIM", "GIMB", "QERR", "ADR"};

    checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV (instrSession)) checkErr(RS_ERROR_INSTRUMENT_MODEL);

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 4),
			3, "Marker");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementType, RSSPECAN_VAL_VSA_RESULT_EVM, RSSPECAN_VAL_VSA_RESULT_AMPLITUDE_DROOP),
			4, "Measurement Type");

	if((measurementType == RSSPECAN_VAL_VSA_RESULT_EVM) ||
	   (measurementType == RSSPECAN_VAL_VSA_RESULT_PHASE_ERROR) ||
	   (measurementType == RSSPECAN_VAL_VSA_RESULT_FREQ_ERROR) ||
	   (measurementType == RSSPECAN_VAL_VSA_RESULT_MOD_ERROR))
	{
		maxStat = RSSPECAN_VAL_VSA_STAT_MAX_95_RMS;
	}

   	viCheckParm(RsCore_InvalidViInt32Range(instrSession, statistic, RSSPECAN_VAL_VSA_STAT_AVERAGE_SWEEP, maxStat),
   			5, "Statistic");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:MARK%ld:FUNC:DDEM:STAT:%s? %s", window,
																				 marker,
																				 measTypeArr[measurementType],
																				 VSAStatisticArr[statistic]);
	checkErr(RsCore_Write(instrSession, cmd));
	checkErr(viScanf(instrSession, "%f", value));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
