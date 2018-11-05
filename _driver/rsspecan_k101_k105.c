/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  LTE option K101/K105
 *
 *  Original Release: April - August 2011
 *  By: Martin Krcmar & Nabil Ouddane
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"
#include <math.h>

/// HIFN  This function selects LTE uplink measurement mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_SetLTEUplinkMode(
	ViSession	instrSession
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_MODE, NULL));
	checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_LINK_DIRECTION, RSSPECAN_VAL_LTE_UPLINK));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the general settings concerning the physical
/// HIFN  attributes of the signals to be measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR duplexing/Configures the duplexing mode.
/// HIPAR frequency/The control defines the center frequency of the analyzer.
/// HIPAR bandwidthSettingType/Setting type of bandwidth
/// HIPAR channelBandwidth/Setting channel bandwidth.
/// HIPAR numberofResourceBlocks/Configures the number of resource blocks used for UL.
/// HIPAR cyclicPrefix/Configures the cyclic prefix type for UP.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSignalCharacteristics(
	ViSession	instrSession,
	ViInt32	duplexing,
	ViReal64	frequency,
	ViInt32	bandwidthSettingType,
	ViInt32	channelBandwidth,
	ViInt32	numberofResourceBlocks,
	ViInt32	cyclicPrefix
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_DUPLEXING, duplexing),
    		2, "Duplexing");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_CENTER, frequency),
    		3, "Frequency");

    switch (bandwidthSettingType)
    {
        case RSSPECAN_VAL_LTE_DOWNLINK_CHBW:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_CHANNEL_BANDWIDTH, channelBandwidth),
            		4, "Channel bandwidth");
            break;
        case RSSPECAN_VAL_LTE_DOWNLINK_NRB:
            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_NUMBER_OF_RESOURCE_BLOCKS, numberofResourceBlocks),
            		5, "Number of Resource Blocks");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, bandwidthSettingType), 4, "Bandwidth Setting Type");
            break;
    }

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_CYCLIC_PREFIX, cyclicPrefix),
    		6, "Cyclic Prefix");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures settings related to the reference level and
/// HIFN  the RF attenuation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR referenceLevel/Configures the reference level of the analyzers used in RF mode.
/// HIPAR autoReferenceLevel/Activates or deactivates auto level.
/// HIPAR externalAttenuation/This control specifies the external attenuation or gain applied to the
/// HIPAR externalAttenuation/RF signal. A positive value indicates attenuation, a negative value
/// HIPAR externalAttenuation/indicates gain.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkLevel(
	ViSession	instrSession,
	ViBoolean	autoReferenceLevel,
	ViReal64	referenceLevel,
	ViReal64	externalAttenuation
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_AUTO_REFERENCE_LEVEL, autoReferenceLevel),
    		2, "Auto Reference Level");

	if (autoReferenceLevel == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_REFERENCE_LEVEL, referenceLevel),
				3, "Reference Level");
	}

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, externalAttenuation),
    		4, "External Attenuation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function configures the reference level of the analyzers used in
/// HIFN  BB-mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR referenceLevel/Configures the reference level of the analyzers used in BB-mode.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkLevelInBBMode(
	ViSession	instrSession,
	ViReal64	referenceLevel
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_REFERENCE_LEVEL_IN_BB_MODE, referenceLevel),
    		2, "Reference Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the trigger parameters.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR mode/Sets the trigger mode.
/// HIPAR offset/Defines the length of the trigger delay. A negative delay time
/// HIPAR offset/(pretrigger) can be set.
/// HIPAR holdoff/This control sets the holding time before the next power trigger event.
/// HIPAR hysteresis/This control sets the limit that the hysteresis value has to
/// HIPAR hysteresis/fall below in order to trigger the next measurement.
/// HIPAR triggerLevel/Specifies the level of the external trigger input for which triggering
/// HIPAR triggerLevel/will occur.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTrigger(
	ViSession	instrSession,
	ViInt32	mode,
	ViReal64	offset,
	ViReal64	holdoff,
	ViReal64	hysteresis,
	ViReal64	triggerLevel
)
{
    ViStatus	error = VI_SUCCESS;
	ViAttr 		trig_level_attr[] = {0, RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL,
                                     RSSPECAN_ATTR_LTE_UPLINK_TRIGGER_IF_POWER_LEVEL,
									 0, RSSPECAN_ATTR_TRIGGER_RFP_LEVEL,
									 RSSPECAN_ATTR_LTE_UPLINK_TRIGGER_IF_POWER_LEVEL};

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_TRIGGER_MODE, mode),
    		2, "Mode");

	if (mode != RSSPECAN_VAL_TRIG_MODE_IMMEDIATE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_DELAY, offset),
				3, "Offset");
    }

	if ((mode == RSSPECAN_VAL_TRIG_MODE_IF_POWER) ||
		(mode == RSSPECAN_VAL_TRIG_MODE_POWER)/* ||
		(mode == RSSPECAN_VAL_TRIG_MODE_RF_POWER)*/)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_IFP_OFFSET, holdoff),
				4, "Holdoff");
    }

	if ((mode == RSSPECAN_VAL_TRIG_MODE_IF_POWER) ||
		(mode == RSSPECAN_VAL_TRIG_MODE_POWER))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", RSSPECAN_ATTR_TRIGGER_IFP_HYSTERESIS, hysteresis),
				5, "Hysteresis");
    }

	if ((mode == RSSPECAN_VAL_TRIG_MODE_EXTERNAL) ||
		(mode == RSSPECAN_VAL_TRIG_MODE_IF_POWER) ||
		(mode == RSSPECAN_VAL_TRIG_MODE_POWER) ||
		(mode == RSSPECAN_VAL_TRIG_MODE_RF_POWER))
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win0", trig_level_attr[mode], triggerLevel),
				6, "Trigger Level");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures I/Q settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR swapIQ/Specifies if the IQ data shall be swapped.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkIQ(
	ViSession	instrSession,
	ViBoolean	swapIQ
)
{
    ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_IQ_SWAP, swapIQ),
    		2, "Swap IQ");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures settings related to the input source of the signal to be
/// HIFN  measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR source/Configures the current signal source. Signal sources can be baseband,
/// HIPAR source/digital IQ, Radio Frequency or file.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkInput(
	ViSession	instrSession,
	ViInt32	source
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_INPUT, source),
    		2, "Source");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the auto level track time.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR autoLevelTrackTime/Configures the auto level track time.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkAutoLevelTrackTime(
	ViSession	instrSession,
	ViReal64	autoLevelTrackTime
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_AUTO_LEVEL_TRACK_TIME, autoLevelTrackTime),
    		2, "Auto Level Track Time");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the input attenuation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR attenuation/Specifies the input attenuation (in positive dB). This means that if
/// HIPAR attenuation/10dB is selected, the result is a reduction in the signal level of 10
/// HIPAR attenuation/dB.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkRFAttenuation(
	ViSession	instrSession,
	ViInt32	attenuation
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_RF_ATTENUATION, attenuation),
    		2, "Attenuation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures digital baseband input parameters.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR digitalInputDataRate/This control sets the input date sample rate read by the digital
/// HIPAR digitalInputDataRate/baseband input.
/// HIPAR fullScaleLevel/This control sets the voltage corresponding to the maximum input value
/// HIPAR fullScaleLevel/of the digital baseband input.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkDigitalBaseband(
	ViSession	instrSession,
	ViReal64	digitalInputDataRate,
	ViReal64	fullScaleLevel
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_SRATE, digitalInputDataRate),
    		2, "Digital Input Data Rate");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_RANGE, fullScaleLevel),
			3, "Full Scale Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the LTE MIMO.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_UPLINK_MIMO_ANTENNA
/// HIFN     RSSPECAN_ATTR_LTE_UPLINK_MIMO_PUCCH
/// HIFN     RSSPECAN_ATTR_LTE_UPLINK_MIMO_PUSCH
/// HIFN     RSSPECAN_ATTR_LTE_UPLINK_MIMO_SRS
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     CONFigure[:LTE]:UL:MIMO:ASELection ALL | ANT1 | ANT2 | ANT3 | ANT4
/// HIFN     CONFigure[:LTE]:UL:MIMO:PUCCh:CONFig
/// HIFN     CONFigure[:LTE]:UL:MIMO:PUSCh:CONFig
/// HIFN     CONFigure[:LTE]:UL:MIMO:SRS:CONFig
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR antenna/Configures the antenna selection.
/// HIPAR pucch/Selects the number of antennas for the PUCCH in a MIMO setup.
/// HIPAR pusch/Selects the number of antennas for the PUSCH in a MIMO setup.
/// HIPAR srs/Selects the number of antennas for the sounding reference signal in a
/// HIPAR srs/MIMO setup.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkMIMO(
	ViSession	instrSession,
	ViInt32	antenna,
	ViInt32	pucch,
	ViInt32	pusch,
	ViInt32	srs
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_MIMO_ANTENNA, antenna),
    		2, "Antenna");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_MIMO_PUCCH, pucch),
    		3, "PUCCH");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_MIMO_PUSCH, pusch),
			4, "PUSCH");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_MIMO_SRS, srs),
			2, "SRS");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Restores previously saved demodulation settings. The input file must
/// HIFN  be of type "*.allocation" and depends on the link direction that was
/// HIFN  currently selected when the file was saved. Only files with correct
/// HIFN  link directions can get loaded.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR filePath/Restores previously saved demodulation settings. The input file must
/// HIPAR filePath/be of type "*.allocation" and depends on the link direction that was
/// HIPAR filePath/currently selected when the file was saved. Only files with correct
/// HIPAR filePath/link directions can get loaded.
ViStatus _VI_FUNC rsspecan_LoadLTEUplinkDemodulationSettings(
	ViSession	instrSession,
	ViString	filePath
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_LOAD_DEMODULATION_SETTINGS, filePath),
    		2, "File Path");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Stores the current demodulation settings to a file. The resulting file
/// HIFN  type is "*.allocation". Existing files will be overwritten.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR filePath/Stores the current demodulation settings to a file. The resulting file
/// HIPAR filePath/type is "*.allocation". Existing files will be overwritten.
ViStatus _VI_FUNC rsspecan_StoreLTEUplinkDemodulationSettings(
	ViSession	instrSession,
	ViString	filePath
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_STORE_DEMODULATION_SETTINGS, filePath),
    		2, "File Path");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures processing configuration with respect to how the signal is
/// HIFN  to be measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR channelEstimation/Configures the channel estimation type for uplink.
/// HIPAR compensateDCOffset/Activates or deactivates DC offset compensation when calculating
/// HIPAR compensateDCOffset/measurement results.
/// HIPAR autoDemodulation/Activates or deactivates automatic demodulation.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkDataAnalysis(
	ViSession	instrSession,
	ViInt32	channelEstimation,
	ViBoolean	compensateDCOffset,
	ViBoolean	autoDemodulation
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_CHANNEL_ESTIMATION, channelEstimation),
    		2, "Channel Estimation");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_COMPENSATE_DC_OFFSET, compensateDCOffset),
			3, "Compensate DC Offset");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_AUTO_DEMODULATION, autoDemodulation),
			4, "Auto Demodulation");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN Configures processing configuration with respect to how the signal is to be measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR scramblingOfCodedBits/Activates or deactivates scrambling of coded bits for UL.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkCodeBitsScrambling (ViSession instrSession,
                                                                 ViBoolean scramblingOfCodedBits)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SCRAMBLING_OF_CODED_BITS, scramblingOfCodedBits),
    		2, "Scrambling Of Coded Bits");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function specifies whether the suppressed interference
/// HIFN  synchronization shall be used or not.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR suppressedInterferenceSync/Specifies whether or not the suppressed interference
/// HIPAR suppressedInterferenceSync/synchronization feature shall be used. If this synchronization
/// HIPAR suppressedInterferenceSync/mode is enabled, the synchronization on signals containing more
/// HIPAR suppressedInterferenceSync/than one user equipment (UE) is more robust. Additionally, the
/// HIPAR suppressedInterferenceSync/EVM is lower in case the UEs have different frequency offsets.
/// HIPAR suppressedInterferenceSync/Note that Auto Demodulation is not supported in this
/// HIPAR suppressedInterferenceSync/synchronization mode and the EVM may be higher in case only one
/// HIPAR suppressedInterferenceSync/UE is present in the signal.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSuppressedInterferenceSynchronization
			(ViSession instrSession,
    		ViBoolean suppressedInterferenceSync)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SUPPRESSED_INTERFERENCE_SYNC, suppressedInterferenceSync),
			2, "Suppressed Interference Sync");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures phase and timing tracking.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR phaseTracking/Configures the phase tracking type.
/// HIPAR timing/Specifies whether or not the measurement results should be compensated
/// HIPAR timing/for timing error. When timing compensation is used, the measurement
/// HIPAR timing/results will be compensated for timing error on a per-symbol basis.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTracking(
	ViSession	instrSession,
	ViInt32	phaseTracking,
	ViBoolean	timing
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PHASE_TRACKING, phaseTracking),
    		2, "Phase Tracking");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_TIMING, timing),
			3, "Timing");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Includes or excludes the transient slots present after a switch from
/// HIFN  downlink to uplink in the analysis.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_UPLINK_TRANSIENT_SLOTS
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     [SENSe][:LTE]:UL:DEMod:ATTSlots
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR transientSlots/Includes or excludes the transient slots present after a switch from
/// HIPAR transientSlots/downlink to uplink in the analysis.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTransientSlots(
	ViSession	instrSession,
	ViBoolean	transientSlots
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_TRANSIENT_SLOTS, transientSlots),
    		2, "Transient Slots");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures the TDD UL/DL Allocations.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR tddULDLAllocations/Configures the TDD UL/DL Allocations.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTDDAllocation(
	ViSession	instrSession,
	ViInt32	tddULDLAllocations
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_TDD_UL_DL_ALLOCATIONS, tddULDLAllocations),
    		2, "TDD UL DL Allocations");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures the special subframe configuration for LTE uplink signals.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR specialSubframe/Selects the special subframe configuration for LTE uplink signals.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTDDSpecialSubframe(
	ViSession	instrSession,
	ViInt32	specialSubframe
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_TDD_SPECIAL_SUBFRAME, specialSubframe),
    		2, "Special Subframe");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures physical layer cell identity. There are 504 unique
/// HIFN  physical-layer cell identities. The physical-layer cell identities are
/// HIFN  grouped into 168 unique physical-layer cell-identity groups, each group
/// HIFN  containing  three unique identities.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR cellIdentityGroup/Cell Identity Group specifies the physical-layer cell identity group.
/// HIPAR identity/Identity specifies the physical-layer identity.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPhysicalLayerCellIdentity(
	ViSession	instrSession,
	ViInt32	cellIdentityGroup,
	ViInt32	identity
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_CELL_IDENTITY_GROUP, cellIdentityGroup),
    		2, "Cell Identity Group");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_CELL_IDENTITY, identity),
			3, "Identity");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Sets the number of subframes that can be configured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR configurableSubframes/Sets the number of subframes that can be configured.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkConfigurableSubframes(
	ViSession	instrSession,
	ViInt32	configurableSubframes
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_CONFIGURABLE_SUBFRAMES, configurableSubframes),
    		2, "Configurable Subframes");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures specified allocation in specified subframe.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR subframe/Selects configurable subframe.
/// HIPAR enablePUCCH/Activates and deactivates the PUCCH in the corresponding subframe.
/// HIPAR modulation/Configures the modulation type for an allocation in a specific UL
/// HIPAR modulation/subframe.
/// HIPAR resourceBlocks/Configures the number of resource blocks for an allocation in a
/// HIPAR resourceBlocks/specific UL subframe.
/// HIPAR resourceBlocksOffset/Configures the resource block offset for an allocation in a specific
/// HIPAR resourceBlocksOffset/UL subframe.
/// HIPAR power/Configures the relative power of an allocation in a specific UL
/// HIPAR power/subframe.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSubframeTable(
	ViSession	instrSession,
	ViInt32	subframe,
	ViBoolean	enablePUCCH,
	ViInt32	modulation,
	ViInt32	resourceBlocks,
	ViInt32	resourceBlocksOffset,
	ViReal64	power
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, subframe, 0, 9),
    		2, "Subframe");
    viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_ENABLE_PUCCH,
    		"LTESubframe", subframe , RS_REPCAP_BUF_SIZE, repCap),
    		2, "Subframe");

    viCheckParm(rsspecan_SetAttributeViInt32 (instrSession, repCap,
                                               RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_ENABLE_PUCCH,
                                               enablePUCCH ? RSSPECAN_VAL_LTE_UPLINK_SUBFRAME_PUCCH : RSSPECAN_VAL_LTE_UPLINK_SUBFRAME_PUSCH), 3, "Enable PUCCH");

	if (enablePUCCH == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_MODULATION, modulation),
				4, "Modulation");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_RESOURCE_BLOCKS, resourceBlocks),
				5, "Resource Blocks");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_RESOURCE_BLOCKS_OFFSET, resourceBlocksOffset),
				6, "Resource Blocks Offset");
    }
	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_POWER, power),
			7, "Power");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures the reference signal for PUSCH and PUCCH.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR relativePowerPUSCH/Configures the relative power of the physical uplink shared channel.
/// HIPAR relativePowerPUCCH/Configures the relative power of the physical uplink control channel.
/// HIPAR groupHopping/Enables or disables group hopping in the 3GPP configuration for LTE
/// HIPAR groupHopping/uplink signals.
/// HIPAR sequenceHopping/Enables or disables sequence hopping in the 3GPP configuration for LTE
/// HIPAR sequenceHopping/uplink signals.
/// HIPAR deltaSequenceShift/Configures the delta sequence shift in the 3GPP configuration of the
/// HIPAR deltaSequenceShift/LTE uplink signal.
/// HIPAR n_dmrs/The n_DMRS parameter can be found in 3GPP TS36.211 V8.5.0, 5.5.2.1.1
/// HIPAR n_dmrs/Reference signal sequence. Currently, n_DMRS is defined as n_DMRS =
/// HIPAR n_dmrs/nDMRS(1) + nDMRS(2).
/// HIPAR enablen_PRS/Enables the use of the pseudo-random sequence n_PRS in the calculation
/// HIPAR enablen_PRS/of the demodulation reference signal (DMRS) index as defined in 3GPP TS
/// HIPAR enablen_PRS/36.211, chapter 5.5.2.1.1.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkReferenceSignal(
	ViSession	instrSession,
	ViReal64	relativePowerPUSCH,
	ViReal64	relativePowerPUCCH,
	ViBoolean	groupHopping,
	ViBoolean	sequenceHopping,
	ViInt32	deltaSequenceShift,
	ViInt32	n_dmrs,
	ViBoolean	enablen_PRS
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_POWER_PUSCH, relativePowerPUSCH),
    		2, "Relative Power PUSCH");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_POWER_PUCCH, relativePowerPUCCH),
			3, "Relative Power PUCCH");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_GROUP_HOPPING, groupHopping),
			4, "Group Hopping");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SEQUENCE_HOPPING, sequenceHopping),
			5, "sequenceHopping");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_DELTA_SEQUENCE_SHIFT, deltaSequenceShift),
			6, "Delta Sequence Shift");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_N_DMRS, n_dmrs),
			7, "n_DMRS");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_N_PRS_STATE, enablen_PRS),
			8, "Enable n_PRS");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures the sounding reference signal.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR present/Configures whether the sounding reference signal is present or not.
/// HIPAR relativePower/Relative Power of the sounding reference signal.
/// HIPAR symbolOffset/Symbol offset specifies the symbol offset of the sounding reference
/// HIPAR symbolOffset/signal relative to the subframe start.
/// HIPAR subcarrierOffset/Subcarrier Offset specifies the offset of the sounding reference
/// HIPAR subcarrierOffset/symbol in frequency direction measured in physical subcarriers.
/// HIPAR numberOfSubcarriers/No. of Subcarriers specifies the length of the sounding reference
/// HIPAR numberOfSubcarriers/signal measured in physical subcarriers.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignal(
	ViSession	instrSession,
	ViBoolean	present,
	ViReal64	relativePower,
	ViInt32	symbolOffset,
	ViInt32	subcarrierOffset,
	ViInt32	numberOfSubcarriers
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_PRESENT, present),
    		2, "Present");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_POWER, relativePower),
			3, "Relative Power");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_SYMBOL_OFFSET, symbolOffset),
    		4, "Symbol Offset");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_SUBCARRIER_OFFSET, subcarrierOffset),
    		5, "Subcarrier Offset");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_SUBCARRIERS, numberOfSubcarriers),
    		6, "Number Of Subcarriers");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function sets the UE specific parameter Frequency Domain Position nRRC.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR frequencyDomainPosition/Sets the UE specific parameter Freq. Domain Position nRRC.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignalNRRC (ViSession instrSession,
                                                                          ViInt32 frequencyDomainPosition)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_N_RRC, frequencyDomainPosition),
    		2, "frequencyDomainPosition");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function turns simultaneous transmission of the Sounding
/// HIFN  Reference Signal (SRS) and ACK/NACK messages (via PUCCH) on and off.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_AN_TX
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     CONFigure[:LTE]:UL:SRS:ANST
/// HIFN
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR anSimultaneousTX/This control turns simultaneous transmission of the Sounding Reference
/// HIPAR anSimultaneousTX/Signal (SRS) and ACK/NACK messages (via PUCCH) on and off.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignalANSimultaneousTX(
	ViSession	instrSession,
	ViBoolean	anSimultaneousTX
)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_AN_TX, anSimultaneousTX),
    		2, "AN Simultaneous TX");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures the sounding reference signal modulation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR parameterAlpha/Defines the cyclic shift in the time domain.
/// HIPAR parameteru/Sets the GCL sequence index.
/// HIPAR mode/Specifies the Mode that is used for the CAZAC sequence.
/// HIPAR parameterq/Specifies the CAZAC sequence for the pilot signal.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignalModulation(
	ViSession	instrSession,
	ViReal64	parameterAlpha,
	ViInt32	parameteru,
	ViInt32	mode,
	ViInt32	parameterq
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_ALPHA_PARAMETER, parameterAlpha),
    		2, "Parameter Alpha");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_U_PARAMETER, parameteru),
			3, "Parameter u");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_MODE, mode),
			4, "Mode");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_Q_PARAMETER, parameterq),
			5, "Parameter q");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures the PUSCH structure.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR frequencyHoppingMode/Configures the frequency hopping mode in the PUSCH structure settings
/// HIPAR frequencyHoppingMode/for UL.
/// HIPAR numberofSubbands/Configures the number of subbands/M in the PUSCH structure settings
/// HIPAR numberofSubbands/for LTE uplink signals.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPUSCHStructure(
	ViSession	instrSession,
	ViInt32	frequencyHoppingMode,
	ViInt32	numberofSubbands
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUSCH_FREQUENCY_HOPPING_MODE, frequencyHoppingMode),
    		2, "Frequency Hopping Mode");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUSCH_SUBBANDS, numberofSubbands),
			3, "Number of Subbands");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures the PUSCH structure.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR hoppingOffset/Defines the frequency hopping offset for the PUSCH.
/// HIPAR infoInHoppingBits/Defines the information in hopping bits of the PUSCH.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPUSCHHopping (ViSession instrSession,
                                                           ViInt32 hoppingOffset,
                                                           ViInt32 infoInHoppingBits)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUSCH_HOPPING_OFFSET, hoppingOffset),
    		2, "Hopping Offset");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUSCH_INFO_IN_HOPPING_BITS, infoInHoppingBits),
			3, "Info In Hopping Bits");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures the PUCCH structure.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR numberofRBsforPUCCH/Number of RBs for PUCCH configures the number of resource blocks for
/// HIPAR numberofRBsforPUCCH/PUCCH.
/// HIPAR deltaShift/Sets the delta shift parameter, i.e. the cyclic shift difference
/// HIPAR deltaShift/between two adjacent PUCCH resource indices with the same orthogonal
/// HIPAR deltaShift/cover sequence (OC).
/// HIPAR deltaOffset/Sets the PUCCH delta offset parameter, i.e. the cyclic shift offset.
/// HIPAR deltaOffset/The value range depends on the selected Cyclic Prefix.
/// HIPAR n_1_cs/Sets the number of cyclic shifts used for PUCCH format 1/1a/1b in a
/// HIPAR n_1_cs/resource block used for a combination of the formats 1/1a/1b and
/// HIPAR n_1_cs/2/2a/2b.
/// HIPAR n_2_RB/Sets bandwidth in terms of resource blocks that are reserved for PUCCH
/// HIPAR n_2_RB/formats 2/2a/2b transmission in each subframe.
/// HIPAR format/Configures the PUCCH format.
/// HIPAR n_pucch/Sets the resource index for PUCCH format 1/1a/1b respectively 2/2a/2b.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPUCCHStructure(
	ViSession	instrSession,
	ViInt32	numberofRBsforPUCCH,
	ViInt32	deltaShift,
	ViInt32	deltaOffset,
	ViInt32	n_1_cs,
	ViInt32	n_2_RB,
	ViInt32	format,
	ViInt32	n_pucch
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_RESOURCE_BLOCKS, numberofRBsforPUCCH),
    		2, "Number of RBs for PUCCH");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_DELTA_SHIFT, deltaShift),
			3, "Delta Shift");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_DELTA_OFFSET, deltaOffset),
			4, "Delta Offset");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_CYCLIC_SHIFTS, n_1_cs),
			5, "N1_cs");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_BANDWIDTH, n_2_RB),
			6, "N2_RB");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_FORMAT, format),
			7, "Format");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PUCCH_RESOURCE_INDEX, n_pucch),
			8, "N_PUCCH");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures the PUCCH structure.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR PRACHConfiguration/Selects the PRACH preamble format.
/// HIPAR restrictedSet/Turns the restricted preamble set for PRACH on and off.
/// HIPAR frequencyOffset/Defines the PRACH frequency offset. The command is available for
/// HIPAR frequencyOffset/preamble formats 0 to 3.
/// HIPAR ncsConfiguration/Defines the Ncs configuration for the PRACH.
/// HIPAR logicalRootSequenceIndex/Defines the PRACH logical root sequence index.
/// HIPAR sequenceIndex/Selects whether the PRACH sequence index will be determined
/// HIPAR sequenceIndex/automatically or manually. In such case use Sequence Index Value
/// HIPAR sequenceIndex/control to set the index.
/// HIPAR sequenceIndexValue/Selects the PRACH sequence index.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPRACHStructure (ViSession instrSession,
                                                             ViInt32 PRACHConfiguration,
                                                             ViBoolean restrictedSet,
                                                             ViInt32 frequencyOffset,
                                                             ViInt32 ncsConfiguration,
                                                             ViInt32 logicalRootSequenceIndex,
                                                             ViInt32 sequenceIndex,
                                                             ViInt32 sequenceIndexValue)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_CONFIGURATION, PRACHConfiguration),
			2, "PRACH Configuration");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_RESTRICTED_SET, restrictedSet),
			3, "Restricted Set");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_FREQUENCY_OFFSET, frequencyOffset),
			4, "Frequency Offset");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_NCS_CONFIGURATION, ncsConfiguration),
			5, "Ncs Configuration");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_LOGICAL_ROOT_SEQ_INDEX, logicalRootSequenceIndex),
			6, "Logical Root SequenceIndex");

	if (sequenceIndex == RSSPECAN_VAL_LTE_UPLINK_PRACH_SEQ_INDEX_AUTO)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_SEQUENCE_INDEX, sequenceIndex),
				7, "Sequence Index");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_SEQUENCE_INDEX_VALUE, sequenceIndexValue),
				8, "Sequence Index Value");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Configures the PRACH preamble mapping.
/// HIFN  RSSPECAN_ATTR_MARKER_TRACE or function rsspecan_ConfigureMarker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR automaticPreambleMapping/Turns automatic preamble mapping for the PRACH on and off.
/// HIPAR frequencyIndex/Selects the PRACH frequency index.
/// HIPAR halfFrameIndicator/Defines the PRACH half frame indicator.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPRACHPreambleMapping (ViSession instrSession,
                                                                   ViBoolean automaticPreambleMapping,
                                                                   ViInt32 frequencyIndex,
                                                                   ViInt32 halfFrameIndicator)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_AUTOMATIC_PREAMBLE_MAPPING, automaticPreambleMapping),
			2, "Automatic Preamble Mapping");

	if (automaticPreambleMapping == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_FREQUENCY_INDEX, frequencyIndex),
				3, "Frequency Index");

		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_PRACH_HALF_FRAME_INDICATOR, halfFrameIndicator),
				4, "Half Frame Indicator");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function selects the trace the marker is positioned on.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) To assign marker to a trace using a numeric value, use attribute
/// HIFN  RSSPECAN_ATTR_MARKER_TRACE or function rsspecan_ConfigureMarker.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/This control selects marker.
/// HIPAR trace/Selects the trace the marker is positioned on.
ViStatus _VI_FUNC rsspecan_ConfigureMarkerToTrace(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	marker,
	ViInt32	trace
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, window, 0, 16),
    		2, "Window");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
    		3, "Marker");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,M%ld", window, marker);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_ASSIGN_MARKER_TO_TRACE_RAISING_FALLING, trace),
    		4, "Trace");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This command updates the current IQ measurement results to reflect the
/// HIFN  current measurement settings. Note no new IQ data is captured. I.e. the
/// HIFN  measurement settings apply to the IQ data being currently in the
/// HIFN  capture buffer. The command applies exclusively to IQ measurements. It
/// HIFN  requires available IQ data.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
ViStatus _VI_FUNC rsspecan_LTEUplinkUpdateIQResults(
	ViSession	instrSession
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_UPDATE_IQ_RESULTS, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Selects LTE Uplink measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR measurementType/Selects LTE Uplink measurement.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkMeasurementType(
	ViSession	instrSession,
	ViInt32	window,
	ViInt32	measurementType
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_GetAttributeRepCapName(instrSession, RSSPECAN_ATTR_LTE_UPLINK_MEASUREMENT_TYPE,
			"Window", window , RS_REPCAP_BUF_SIZE, repCap),
			2, "Window");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_UPLINK_MEASUREMENT_TYPE, measurementType),
    		3, "Measurement Type");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures the subframe to be analyzed.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR allSubframes/Selects either single subframe or all subframes to be analyzed.
/// HIPAR singleSubframeSelection/Configures the subframe to be analyzed.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSubframeSelection(
	ViSession	instrSession,
	ViBoolean	allSubframes,
	ViInt32	singleSubframeSelection
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    if (allSubframes == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_SELECTION_ALL, allSubframes),
        		2, "All Subframes");
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_SELECTION, singleSubframeSelection),
        		3, "Single Subframe Selection");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This command selects the slot to analyze.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR slotSelection/Selects the slot to analyze. With the Slot Selection,
/// HIPAR slotSelection/slot-specific measurement results can be selected. This setting
/// HIPAR slotSelection/applies to the following measurements: Result Summary, EVM vs
/// HIPAR slotSelection/Carrier, EVM vs Symbol, Inband Emission, Channel Flatness,
/// HIPAR slotSelection/Channel Flatness SRS, Channel Group Delay, Channel Flatness
/// HIPAR slotSelection/Difference, Constellation diagram and DFT Precoded Constellation
/// HIPAR slotSelection/diagram.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSlotSelection (ViSession instrSession,
                                                            ViInt32 slotSelection)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SLOT_SELECTION, slotSelection),
    		2, "Slot Selection");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Specifies the units for EVM results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR evmUnits/Specifies the units for EVM results.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkEVMUnits(
	ViSession	instrSession,
	ViInt32	evmUnits
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_EVM_UNITS, evmUnits),
    		2, "EVM Units");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Specifies if the bit stream gets displayed using bits or using
/// HIFN  symbols.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR bitStreamFormat/Specifies if the bit stream gets displayed using bits or using
/// HIPAR bitStreamFormat/symbols.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkBitStream(
	ViSession	instrSession,
	ViInt32	bitStreamFormat
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_BIT_STREAM_FORMAT, bitStreamFormat),
    		2, "Bit Stream Format");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Specifies the SEM category defined in 3GPP TS 36.101 for Uplink.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR semChannelCategory/Specifies the SEM category defined in 3GPP TS 36.101 for Uplink.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSEMSettings(
	ViSession	instrSession,
	ViInt32	semChannelCategory
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_SEM_CHANNEL, semChannelCategory),
    		2, "SEM Channel Category");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures Adjacent Channel Leakage Ratio measurement settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR assumedAdjChannelCarrier/Selects the ACLR Assumed Adj. Channel Carrier in the general settings
/// HIPAR assumedAdjChannelCarrier/menu.
/// HIPAR noiseCorrection/Activates or deactivates Noise Correction for ACLR measurements.
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkACLRSettings(
	ViSession	instrSession,
	ViInt32	assumedAdjChannelCarrier,
	ViBoolean	noiseCorrection
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_ACLR_ASSUMED_ADJ_CHANNEL_CARRIER, assumedAdjChannelCarrier),
    		2, "Assumed Adj Channel Carrier");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_ACLR_NOISE_CORRECTION, noiseCorrection),
			3, "Noise Correction");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Configures the vertical resolution of the measurement results. The
/// HIFN  scaling you select always applies to the currently active screen and
/// HIFN  the corresponding result display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR autoScaling/Activates or deactivates automatic scaling of Y-Axis.
/// HIPAR perDivision/Defines the distance between two grid lines (scaling per division).
/// HIPAR offset/Defines the point of origin of the y-axis (the offset).
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkYAxisScaling(
	ViSession	instrSession,
	ViBoolean	autoScaling,
	ViReal64	perDivision,
	ViReal64	offset
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_Y_AXIS_SCALING_AUTO, autoScaling),
    		2, "Auto Scaling");

	if (autoScaling == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_Y_AXIS_SCALING_PER_DIVISION, perDivision),
				3, "Per Division");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_Y_AXIS_SCALING_OFFSET, offset),
				4, "Offset");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Queries LTE Uplink measurement result summary.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR frameResult/Selects result type.
/// HIPAR result/Returns selected result. Depending on the EVM unit selected in the
/// HIPAR result/"General Settings" menu the value will be either in dB or in percent.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementResultSummary(
	ViSession	instrSession,
	ViInt32	frameResult,
	ViReal64*	result
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];
    ViAttr	attr[8] = {RSSPECAN_ATTR_LTE_UPLINK_EVM_PUSCH_QPSK_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_PUSCH_16QAM_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_PUSCH_64QAM_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_DMRS_PUSCH_QPSK_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_DMRS_PUSCH_16QAM_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_DMRS_PUSCH_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_PUCCH_RESULT,
					   RSSPECAN_ATTR_LTE_UPLINK_EVM_PRACH_RESULT};

    checkErr(RsCore_LockSession(instrSession));

	switch (frameResult)
    {
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_QPSK_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_16QAM_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_PUCCH_AVER:
        case RSSPECAN_VAL_LTE_UPLINK_PRACH_AVER:
            sprintf (buffer, "Aver");
            break;
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_QPSK_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_16QAM_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_PUCCH_MAX:
        case RSSPECAN_VAL_LTE_UPLINK_PRACH_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_QPSK_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_16QAM_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_PUCCH_MIN:
        case RSSPECAN_VAL_LTE_UPLINK_PRACH_MIN:
            sprintf (buffer, "Min");
            break;
        default:
        viCheckParm(RsCore_InvalidViInt32Value(instrSession, frameResult), 2, "Frame Result");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64 (instrSession, buffer,
		attr[(int) fmod(frameResult,8)], result), 3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Queries LTE Uplink measurement EVM All result.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result. Depending on the EVM unit selected in the
/// HIPAR result/"General Settings" menu the value will be either in dB or in percent.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementEVMAll(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_EVM_ALL_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Queries the "EVM Physical Channel" value from the result summary list.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result. Depending on the EVM unit selected in the
/// HIPAR result/"General Settings" menu the value will be either in dB or in percent.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementEVMPhysicalChannel(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_EVM_PHYSICAL_CHANNEL_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Queries the "EVM Physical Signal" value from the result summary list.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result. Depending on the EVM unit selected in the
/// HIPAR result/"General Settings" menu the value will be either in dB or in percent.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementEVMPhysicalSignal(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_EVM_PHYSICAL_SIGNAL_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Queries "Frequency Error" value from the result summary list in Hz.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementFrequencyError(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_FREQUENCY_ERROR_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Queries "Sampling Error" value from the result summary list in ppm.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementSamplingError(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_SAMPLING_ERROR_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the IQ offset from the result summary list in dB.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementIQOffset(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_IQ_OFFSET_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the IQ gain imbalance from the result summary list in dB.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementIQGainImbalance(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_IQ_GAIN_IMBALANCE_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the IQ quadrature error from the result summary list in
/// HIFN  degrees.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementIQQuadratureError(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_IQ_QUADRATURE_ERROR_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the "Frame Power" from the result summary list in dBm.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR result/Returns selected result.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementFramePower(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_FRAME_POWER_RESULT, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Returns the crest factor from the result summary list in dB.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR result/Returns the crest factor from the result summary list in dB.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementCrestFactor(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "Min");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "Max");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "Aver");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_LTE_UPLINK_CREST_FACTOR_RESULT, result),
			3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the trigger to frame value.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR result/Returns the trigger to frame value.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementTriggerToFrame(
	ViSession	instrSession,
	ViReal64*	result
)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_TRIGGER_TO_FRAME_RESULT, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function queries the current result values of the adjacent
/// HIFN  channel power measurement. An ACLR (adjacent channel leakgae ratio)
/// HIFN  measurement must have previously been run for there to be summary data
/// HIFN  available.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/This control defines size of 'Result' array.
/// HIPAR result/Adjacent channel power values are output in dB. The returned list is
/// HIPAR result/variable length depending on the number of channels to be measured,
/// HIPAR result/i.e. if the number of channels is 2 then the list will contain 5
/// HIPAR result/results (main channel plus two results each for each adjacent channel
/// HIPAR result/specified in following order).
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementACLRResult(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	result[]
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar*     pbuffer = NULL;
    ViChar*     pstring_value;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 5, 5),
    		2, "Array size");

    checkErr(RsCore_QueryViStringUnknownLength(instrSession, "CALC1:MARK:FUNC:POW:RES?", &pbuffer)); // TODO: Check the response processing

    pstring_value = strtok(pbuffer, ",");
    sscanf(pstring_value,"%le",&result[0]);

    pstring_value = strtok(NULL, ",");
    sscanf(pstring_value,"%le",&result[1]);

    pstring_value = strtok(NULL, ",");
    sscanf(pstring_value,"%le",&result[2]);

    pstring_value = strtok(NULL, ",");
    sscanf(pstring_value,"%le",&result[3]);

    pstring_value = strtok(NULL, ",");
    sscanf(pstring_value,"%le",&result[4]);

    if (pbuffer)
        free(pbuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the cell identity group detected by the DSP kernel. If no
/// HIFN  valid value has been detected yet, the function will return -1.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR result/Returns the cell identity group detected by the DSP kernel. If no
/// HIPAR result/valid value has been detected yet, the control will return -1.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkDetectedCellIdentityGroup(
	ViSession	instrSession,
	ViInt32*	result
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_DETECTED_CELL_IDENTITY_GROUP, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the cell identity detected by the DSP kernel. If no valid
/// HIFN  value has been detected yet, the function will return -1.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR result/Returns the cell identity detected by the DSP kernel. If no valid
/// HIPAR result/value has been detected yet, the control will return -1.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkDetectedCellIdentity(
	ViSession	instrSession,
	ViInt32*	result
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_UPLINK_DETECTED_CELL_IDENTITY, result),
    		2, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Returns the cyclic prefix detected by the DSP kernel. If no valid
/// HIFN  value has been detected yet, the function will return -1.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR result/Returns the cyclic prefix detected by the DSP kernel. If no valid
/// HIPAR result/value has been detected yet, the control will return -1.
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkDetectedCyclicPrefix(
	ViSession	instrSession,
	ViInt32*	result
)
{
    ViStatus	error = VI_SUCCESS;
	ViChar		buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_Write(instrSession, "FETC:CYCP?"));

	viCheckErr(viRead (instrSession, (ViBuf) buffer, 10, NULL));

	if (strncmp (buffer, "NORM", 4) == 0)
		*result = 0;
	else if (strncmp (buffer, "EXT", 3) == 0)
		*result = 1;
	else
		*result = atoi (buffer);

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the EVM-vs.-Carrier-values as list over all carriers. Depends
/// HIFN  on the Subframe selection that can be made in the General Settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the EVM-vs.-Carrier-values as list over all carriers. Depends
/// HIPAR traceData/on the Subframe selection that can be made in the General Settings.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkEVMVersusCarrierTrace(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "TRACE2");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "TRACE3");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "TRACE1");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the EVM-vs.-Symbol-values as list over all symbols.  If a
/// HIFN  single subframe is selected in the General Settings, only the symbols
/// HIFN  of the selected subframe will be returned.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the EVM-vs.-Symbol-values as list over all symbols.  If a
/// HIPAR traceData/single subframe is selected in the General Settings, only the symbols
/// HIPAR traceData/of the selected subframe will be returned.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkEVMVersusSymbolTrace(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    sprintf (buffer, "TRACE1");

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the EVM vs. Subframe values as list over all subframes.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the EVM-vs.-Symbol-values as list over all subframes.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkEVMVersusSubframeTrace(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    sprintf (buffer, "TRACE1");

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the spectrum emission mask results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the spectrum emission mask results.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkSEMTrace(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_VALUES:
            sprintf (buffer, "TRACE1");
            break;
        case RSSPECAN_VAL_MEASTYPE_SUMMARY:
            sprintf (buffer, "LIST");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the Adjacent Channel Leakage Ratio values as vector.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the ACP power values as vector.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkACLRTrace(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    sprintf (buffer, "TRACE1");

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the relative inband emission of the current slot.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the relative inband emission of the current slot.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkInbandEmissionTrace(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_RESOURCE_BLOCK_INDEXES:
            sprintf (buffer, "TRACE1");
            break;
        case RSSPECAN_VAL_MEASTYPE_INBAND_EMISSION:
            sprintf (buffer, "TRACE2");
            break;
        case RSSPECAN_VAL_MEASTYPE_UPPER_LIMIT_LINE:
            sprintf (buffer, "TRACE3");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the signal power in dBm/Hz as list over the considered
/// HIFN  frequency span.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the signal power in dBm/Hz as list over the considered
/// HIPAR traceData/frequency span.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkPowerSpectrumTrace(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    sprintf (buffer, "TRACE1");

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the spectrum flatness in dB as list over the considered
/// HIFN  frequency span. Depends on the subframe selection that can be made in
/// HIFN  the General Settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the spectrum flatness in dB as list over the considered
/// HIPAR traceData/frequency span. Depends on the subframe selection that can be made in
/// HIPAR traceData/the General Settings.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkChannelFlatnessTrace(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "TRACE2");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "TRACE3");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "TRACE1");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the channel group delay in ns as list over the considered
/// HIFN  frequency span. Depends on the Subframe selection that can be made in
/// HIFN  the General Settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the channel group delay in ns as list over the considered
/// HIPAR traceData/frequency span. Depends on the Subframe selection that can be made in
/// HIPAR traceData/the General Settings.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkChannelGroupDelayTrace(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "TRACE2");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "TRACE3");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "TRACE1");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the channel flatness difference in dB as list over the
/// HIFN  considered frequency span. Depends on the Subframe selection that can
/// HIFN  be made in the General Settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the channel flatness difference in dB as list over the
/// HIPAR traceData/considered frequency span. Depends on the Subframe selection that can
/// HIPAR traceData/be made in the General Settings.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkChannelFlatnessDifferenceTrace(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_MIN:
            sprintf (buffer, "TRACE2");
            break;
        case RSSPECAN_VAL_MEASTYPE_MAX:
            sprintf (buffer, "TRACE3");
            break;
        case RSSPECAN_VAL_MEASTYPE_AVER:
            sprintf (buffer, "TRACE1");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This measurement represents I and Q data. Data will be returned as an
/// HIFN  array of interleaved I and Q data until all data is exhausted. By
/// HIFN  default all detected data-points will be returned. The amount of data
/// HIFN  can be narrowed by changing settings in the "Constellation Selection"
/// HIFN  menu. Only data points that meet the requirements specified in the
/// HIFN  "Constellation Selection" menu will be returned. The the constellation
/// HIFN  data will be sweeped and returned in the following order:
/// HIFN     Subframe0 -> Symbol0 -> First Carrier to last Carrier of Symbol 0
/// HIFN     Subframe0 -> Symbol1 -> First Carrier0 to last Carrier of Symbol 1
/// HIFN     Subframe0 -> ...
/// HIFN     Subframe0 -> Last Symbol of Subframe0 -> First Carrier to last Carrier
/// HIFN     Subframe1 -> Symbol0 -> First Carrier to last Carrier of Symbol 0
/// HIFN     Subframe1 -> Symbol1 -> First Carrier to last Carrier of Symbol 1
/// HIFN     Subframe1 -> ...
/// HIFN     Subframe1 -> Last Symbol of Subframe0 -> First Carrier to last Carrier
/// HIFN     ...
/// HIFN     Last Subframe
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR bufferSize/Pass the number of doubles in the array you specify for the real parts
/// HIPAR bufferSize/and imaginary parameters.
/// HIPAR noofPoints/Returns the number of I/Q trace data points.
/// HIPAR realParts_I/Returns an array of trace data (I-value x, real part) point. The
/// HIPAR realParts_I/buffer must contain at least as many elements as the value you specify
/// HIPAR realParts_I/with the Buffer Size parameter.
/// HIPAR imaginaryParts_Q/Returns an array of trace data (Q-value y, imaginary part) point. The
/// HIPAR imaginaryParts_Q/buffer must contain at least as many elements as the value you specify
/// HIPAR imaginaryParts_Q/with the Buffer Size parameter.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkConstellationDiagram(
	ViSession	instrSession,
	ViInt32	bufferSize,
	ViInt32*	noofPoints,
	ViReal64	realParts_I[],
	ViReal64	imaginaryParts_Q[]
)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    *pBuffer = NULL;
    ViInt32     i;
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    pBuffer = (ViReal64*) malloc (sizeof(ViReal64) * bufferSize * 2);

    checkErr(rsspecan_dataReadTrace (instrSession, 0, "TRACE1", bufferSize * 2, pBuffer, noofPoints));

    *noofPoints /= 2;

    for ( i = 0; i < *noofPoints && i < bufferSize; i++ )
    {
        realParts_I[i] = pBuffer[2*i];
        imaginaryParts_Q[i] = pBuffer[2*i + 1];
    }

    if ( pBuffer )
        free(pBuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This measurement represents I and Q data. Data will be returned as an
/// HIFN  array of interleaved I and Q data until all data is exhausted. The the
/// HIFN  constellation data will be sweeped and returned in the following order:
/// HIFN
/// HIFN     Subframe0 -> Symbol0 -> First Carrier to last Carrier of Symbol 0
/// HIFN     Subframe0 -> Symbol1 -> First Carrier0 to last Carrier of Symbol 1
/// HIFN     Subframe0 -> ...
/// HIFN     Subframe0 -> Last Symbol of Subframe0 -> First Carrier to last Carrier
/// HIFN     Subframe1 -> Symbol0 -> First Carrier to last Carrier of Symbol 0
/// HIFN     Subframe1 -> Symbol1 -> First Carrier to last Carrier of Symbol 1
/// HIFN     Subframe1 -> ...
/// HIFN     Subframe1 -> Last Symbol of Subframe0 -> First Carrier to last Carrier
/// HIFN     ...
/// HIFN     Last Subframe
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR bufferSize/Pass the number of doubles in the array you specify for the real parts
/// HIPAR bufferSize/and imaginary parameters.
/// HIPAR noofPoints/Returns the number of I/Q trace data points.
/// HIPAR realParts_I/Returns an array of trace data (I-value x, real part) point. The
/// HIPAR realParts_I/buffer must contain at least as many elements as the value you specify
/// HIPAR realParts_I/with the Buffer Size parameter.
/// HIPAR imaginaryParts_Q/Returns an array of trace data (Q-value y, imaginary part) point. The
/// HIPAR imaginaryParts_Q/buffer must contain at least as many elements as the value you specify
/// HIPAR imaginaryParts_Q/with the Buffer Size parameter.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkDFTPrecodedConstellation(
	ViSession	instrSession,
	ViInt32	bufferSize,
	ViInt32*	noofPoints,
	ViReal64	realParts_I[],
	ViReal64	imaginaryParts_Q[]
)
{
    ViStatus    error = VI_SUCCESS;
    ViReal64    *pBuffer = NULL;
    ViInt32     i;
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    pBuffer = (ViReal64*) malloc (sizeof(ViReal64) * bufferSize * 2);

    checkErr(rsspecan_dataReadTrace (instrSession, 0, "TRACE1", bufferSize * 2, pBuffer, noofPoints));

    *noofPoints /= 2;

    for ( i = 0; i < *noofPoints && i < bufferSize; i++ )
    {
        realParts_I[i] = pBuffer[2*i];
        imaginaryParts_Q[i] = pBuffer[2*i + 1];
    }

    if ( pBuffer )
        free(pBuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the complementary cumulative distribution function results in
/// HIFN  percent as list over the power level in dB. The first value returned
/// HIFN  represents the number of following values. Trace1 will return the
/// HIFN  probablility-values (Y-Axis) while Trace2 will return the corresponding
/// HIFN  power-level-steps (X-Axis).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurementType/Selects measurement type.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR traceData/Returns the complementary cumulative distribution function results in
/// HIPAR traceData/percent as list over the power level in dB. The first value returned
/// HIPAR traceData/represents the number of following values. Trace1 will return the
/// HIPAR traceData/probablility-values (Y-Axis) while Trace2 will return the corresponding
/// HIPAR traceData/power-level-steps (X-Axis).
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkCCDF(
	ViSession	instrSession,
	ViInt32	measurementType,
	ViInt32	arraySize,
	ViReal64	traceData[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      option[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    switch (measurementType)
    {
        case RSSPECAN_VAL_MEASTYPE_PROBABILITY:
            sprintf (buffer, "TRACE1");
            break;
        case RSSPECAN_VAL_MEASTYPE_POWER_STEPS:
            sprintf (buffer, "TRACE2");
            break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
            break;
    }

    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize, traceData, noofValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Returns the complementary cumulative distribution function results in
/// HIFN  percent as list over the power level in dB. The first value returned
/// HIFN  represents the number of following values. Trace1 will return the
/// HIFN  probablility-values (Y-Axis) while Trace2 will return the corresponding
/// HIFN  power-level-steps (X-Axis).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR subframeNumber/Subframe number.
/// HIPAR allocationID/ Allocation ID where:
/// HIPAR numberofRB/Number of RB.
/// HIPAR offsetRB/Number of RB.
/// HIPAR modulation/ Modulation
/// HIPAR power/Power in dBm.
/// HIPAR EVM/EVM in dB or percent.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkAllocationSummary(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViInt32	subframeNumber[],
	ViInt32	allocationID[],
	ViInt32	numberofRB[],
	ViInt32	offsetRB[],
	ViInt32	modulation[],
	ViReal64	power[],
	ViReal64	EVM[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *pBuffer = NULL, *pValue = NULL;
    ViInt32     nType = 0;
    ViUInt32     ret_cnt = 0;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    checkErr(RsCore_QueryViStringUnknownLength(instrSession, "TRACE? TRACE1", &pBuffer)); // TODO: Check the response processing

    *noofValues = 0;
    pValue = strtok(pBuffer, ",");

    nType = 0;

    while ( pValue )
    {

        if ( *noofValues >= arraySize )
        {
            viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
            break;
        }
        else
        {
            switch ( nType )
            {
                case 0:
                    subframeNumber[*noofValues] = atoi(pValue);
                    break;
                case 1:
                    allocationID[*noofValues] = atoi(pValue);
                    break;
                case 2:
                    numberofRB[*noofValues] = atoi(pValue);
                    break;
                case 3:
                    offsetRB[*noofValues] = atoi(pValue);
                    break;
                case 4:
                    modulation[*noofValues] = atoi(pValue);
                    break;
                case 5:
                    power[*noofValues] = atof(pValue);
                    break;
                case 6:
                    EVM[*noofValues] = atof(pValue);
                    break;
            }

            nType++;
            if ( nType > 6 )
            {
                nType = 0;
                (*noofValues)++;
            }

            pValue = strtok(NULL, ",");
        }
    }

    if ( pBuffer )
        free(pBuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  Bitsteam data will be returned in groups where each group represents
/// HIFN  one line of the table. The result data has to be interpreted
/// HIFN  differently for downlink and uplink and additionally it is important to
/// HIFN  distinguish between bitstream and hexadecimal data format.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/Number of array points requested.
/// HIPAR bitStreamSize/Size of bitstream array.
/// HIPAR subframeNumber/Subframe number.
/// HIPAR modulation/Type of modulation.
/// HIPAR numberofSymbols/Power in dBm.
/// HIPAR bitStreams/Bit stream of binary numbers. Overall number of bytes required for
/// HIPAR bitStreams/this array is defined by multiplication of number of datablocks and its
/// HIPAR bitStreams/corresponding sizes.
/// HIPAR noofValues/Returns the number of trace data values.
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkBitstream(
	ViSession	instrSession,
	ViInt32	arraySize,
	ViInt32	bitStreamSize,
	ViInt32	subframeNumber[],
	ViInt32	modulation[],
	ViInt32	numberofSymbols[],
	ViChar	bitStreams[],
	ViInt32*	noofValues
)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      *pBuffer = NULL, *pValue = NULL;
    ViInt32     nType = 0;
    ViUInt32    ret_cnt = 0;
    ViInt32     nIndexSymbol = 0, nCurrentSymbol = 0;
    ViInt32     nSymbols = 0;
    ViInt32     nSymbolValue;
	ViChar		*p2bitStreams = bitStreams;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentOptions(instrSession, "K101|K105"));

    checkErr(RsCore_QueryViStringUnknownLength(instrSession, "TRACE? TRACE1", &pBuffer)); // TODO: Check the response processing

    *noofValues = 0;
    pValue = strtok(pBuffer, ",");

    nType = 0;

    while ( pValue )
    {

        if ( *noofValues >= arraySize )
        {
            viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
            break;
        }
        else
        {
            switch ( nType )
            {
                case 0:
                    subframeNumber[*noofValues] = atoi(pValue);
                    nType++;
                    break;
                case 1:
                    modulation[*noofValues] = atoi(pValue);
                    nType++;
                    break;
                case 2:
                    nSymbols = atoi(pValue);
                    nCurrentSymbol = 0;
                    numberofSymbols[*noofValues] = nSymbols;
                    nType++;
                    break;
                case 3: // stream
                    sscanf(pValue, "%x", &nSymbolValue);
                    //bitStreams[nIndexSymbol + nCurrentSymbol] = (ViChar) nSymbolValue;
					p2bitStreams += sprintf (p2bitStreams, "%d", nSymbolValue);

                    nCurrentSymbol++;
                    if ( nCurrentSymbol >= nSymbols )
                    {
                        nIndexSymbol += nSymbols;
                        nType = 0;
                        (*noofValues)++;
                    }

                    if ( nIndexSymbol + nCurrentSymbol >= bitStreamSize )
                    {
                        viCheckParm(RS_ERROR_INVALID_PARAMETER, 3, "Array Size of BitStream");
                    }

                    break;
            }

            pValue = strtok(NULL, ",");
        }
    }

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if ( pBuffer )
        free(pBuffer);

    (void)RsCore_UnlockSession(instrSession);
    return error;
}

