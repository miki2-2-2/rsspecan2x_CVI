/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  LTE option K100/K102
 *
 *  Original Release: 2011
 *  By: Martin Krcmar  & Nabil Ouddane
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *
 *****************************************************************************/

#include "rsspecan.h"

/*===========================================================================*/
/* Function: Set LTE Downlink Mode
/* Purpose:  This function selects LTE Downlink analyzer mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetLTEDownlinkMode(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_MODE, NULL));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Signal Characteristics
/* Purpose:  This function configures the general settings concerning the
/*           physical attributes of the signals to be measured.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSignalCharacteristics(ViSession instrSession,
                                                                     ViInt32 duplexing,
                                                                     ViReal64 frequency,
                                                                     ViInt32 bandwidthSettingType,
                                                                     ViInt32 channelBandwidth,
                                                                     ViInt32 number_ofResourceBlocks,
                                                                     ViInt32 cyclicPrefix)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_DUPLEXING, duplexing),
		2, "Duplexing");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_CENTER, frequency),
		3, "Frequency");

	switch (bandwidthSettingType)
	{
	case RSSPECAN_VAL_LTE_DOWNLINK_CHBW:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CHANNEL_BANDWIDTH, channelBandwidth),
			4, "Channel bandwidth");
		break;
	case RSSPECAN_VAL_LTE_DOWNLINK_NRB:
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_NUMBER_OF_RESOURCE_BLOCKS, number_ofResourceBlocks),
			5, "Number of Resource Blocks");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, bandwidthSettingType), 4, "Bandwidth Setting Type");
		break;
	}

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CYCLIC_PREFIX, cyclicPrefix),
		6, "Cyclic Prefix");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Level
/* Purpose:  This function configures settings related to the reference level
/*           and the RF attenuation.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkLevel(ViSession instrSession,
                                                     ViReal64 referenceLevel,
                                                     ViBoolean autoReferenceLevel,
                                                     ViReal64 externalAttenuation)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_AUTO_REFERENCE_LEVEL, autoReferenceLevel),
		3, "Auto Reference Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_REFERENCE_LEVEL, referenceLevel),
		2, "Reference Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET, externalAttenuation),
		4, "External Attenuation");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Level In BB Mode
/* Purpose:  This function configures the reference level of the analyzers
/*           used in BB-mode.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkLevelInBBMode(ViSession instrSession,
                                                             ViReal64 referenceLevel)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_REFERENCE_LEVEL_IN_BB_MODE, referenceLevel),
		2, "Reference Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink MIMO
/* Purpose:  This function configures essential antenna configuration
/*           parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkMIMO(ViSession instrSession,
                                                    ViInt32 configuration,
                                                    ViInt32 antennaSelection)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_MIMO_CONFIGURATION, configuration),
		2, "Configuration");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_MIMO_ANTENNA, antennaSelection),
		3, "Antenna Selection");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function selects the measurement.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR measurement/This control selects the measurement.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkMeasurement(
	ViSession instrSession,
	ViInt32 measurement
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_MEASUREMENT, measurement),
		2, "measurement");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink IQ
/* Purpose:  This function configures I/Q settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkIQ(ViSession instrSession,
                                                  ViBoolean swapIQ)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_IQ_SWAP, swapIQ),
		2, "Swap IQ");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Input
/* Purpose:  Configures settings related to the input source of the signal to
/*           be measured.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkInput(ViSession instrSession,
                                                     ViInt32 source)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_INPUT, source),
		2, "Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function turns the CSI reference signal on and off.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_STATE
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     CONFigure[:LTE]:DL:CSIRs:STATe
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/This control switches the CSI reference signal on and off.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkCSIRSState(
	ViSession instrSession,
	ViBoolean state
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_STATE, state),
		2, "state");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  Configures the channel state information reference signal (CSI-RS)
/// HIFN  settings.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_ANTENA_PORTS
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_CONFIGURATION_INDEX
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_OVERWRITE_PDSCH
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_RELATIVE_POWER
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_SUBFRAME_CONFIGURATION
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_FRAME_NUMBER_OFFSET
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     CONFigure[:LTE]:DL:CSIRs:NAP
/// HIFN     CONFigure[:LTE]:DL:CSIRs:CI
/// HIFN     CONFigure[:LTE]:DL:CSIRs:OPDSch
/// HIFN     CONFigure[:LTE]:DL:CSIRs:POWer
/// HIFN     CONFigure[:LTE]:DL:CSIRs:SCI
/// HIFN     CONFigure[:LTE]:DL:SFNO
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR antenaPorts/This control selects the number of antenna ports that transmit the CSI
/// HIPAR antenaPorts/reference signal.
/// HIPAR configurationIndex/This control selects the configuration index for the CSI reference
/// HIPAR configurationIndex/signal.
/// HIPAR overwritePDSCH/This control turns overwriting of PDSCH resource elements for UEs that
/// HIPAR overwritePDSCH/do not consider the CSI reference signal on and off.
/// HIPAR relativePower/This control defines the relative power of the CSI reference signal.
/// HIPAR subframeConfiguration/This control defines the subframe configuration for the CSI reference
/// HIPAR subframeConfiguration/signal.
/// HIPAR frameNumberOffset/This control defines the frame number offset for the positioning
/// HIPAR frameNumberOffset/reference signal.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkCSIRS(
	ViSession instrSession,
	ViInt32 antenaPorts,
	ViInt32 configurationIndex,
	ViBoolean overwritePDSCH,
	ViReal64 relativePower,
	ViInt32 subframeConfiguration,
	ViInt32 frameNumberOffset
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_ANTENA_PORTS, antenaPorts),
		2, "antenaPorts");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_CONFIGURATION_INDEX, configurationIndex),
		3, "configurationIndex");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_OVERWRITE_PDSCH, overwritePDSCH),
		4, "overwritePDSCH");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_RELATIVE_POWER, relativePower),
		5, "relativePower");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_SUBFRAME_CONFIGURATION, subframeConfiguration),
		6, "subframeConfiguration");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_FRAME_NUMBER_OFFSET, frameNumberOffset),
		7, "frameNumberOffset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Load LTE Downlink Demodulation Settings
/* Purpose:  Restores previously saved demodulation settings. The input file
/*           must be of type "*.allocation" and depends on the link direction
/*           that was currently selected when the file was saved. Only files
/*           with correct link directions can get loaded.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_LoadLTEDownlinkDemodulationSettings(ViSession instrSession,
                                                               ViString filePath)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_LOAD_DEMODULATION_SETTINGS, filePath),
		2, "File path");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Store LTE Downlink Demodulation Settings
/* Purpose:  Stores the current demodulation settings to a file. The
/*           resulting file type is "*.allocation". Existing files will be
/*           overwritten.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_StoreLTEDownlinkDemodulationSettings(ViSession instrSession,
                                                                ViString filePath)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_STORE_DEMODULATION_SETTINGS, filePath),
		2, "File path");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Data Analysis
/* Purpose:  Configures processing configuration with respect to how the
/*           signal is to be measured.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkDataAnalysis(ViSession instrSession,
                                                            ViInt32 channelEstimation,
                                                            ViInt32 EVMCalculationMethod,
                                                            ViBoolean scrambling_ofCodedBits,
                                                            ViBoolean autoDemodulation)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CHANNEL_ESTIMATION, channelEstimation),
		2, "Channel Estimation");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_EVM_CALCULATION_METHOD, EVMCalculationMethod),
		3, "EVM Calculation Method");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_SCRAMBLING_OF_CODED_BITS, scrambling_ofCodedBits),
		4, "Scrambling of Coded Bits");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_AUTO_DEMODULATION, autoDemodulation),
		5, "Auto Demodulation");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN This function
/// HIFN - selects the method of identifying the PDSCH resource allocation.
/// HIFN - turns boosting estimation for downlink on and off.
/// HIFN - selects the type of reference data to calculate the EVM for the PDSCH.
/// HIFN - turns the suppression of interference of neighboring carriers on and off (e.g. LTE, WCDMA, GSM etc).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR PDSCHSubframeDetect/Selects the method of identifying the PDSCH resource allocation.
/// HIPAR boostingEstimation/Turns boosting estimation on and off.
/// HIPAR PDSCHReferenceData/Selects the type of reference data to calculate the EVM for the PDSCH.
/// HIPAR multicarrierFilter/Turns the suppression of interference of neighboring carriers on and off (e.g. LTE, WCDMA, GSM etc).
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkDataAnalysisSettings(ViSession instrSession,
                                                                    ViInt32 PDSCHSubframeDetect,
                                                                    ViBoolean boostingEstimation,
                                                                    ViInt32 PDSCHReferenceData,
                                                                    ViBoolean multicarrierFilter)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_SUBFRAME_DETECT, PDSCHSubframeDetect),
		2, "PDSCH Subframe Detect");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_BOOSTING_ESTIMATION, boostingEstimation),
		3, "Boosting Estimation");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_REFERENCE_DATA, PDSCHReferenceData),
		4, "PDSCH Reference Data");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_MULTICARRIER_FILTER, multicarrierFilter),
		5, "Multicarrier Filter");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Tracking
/* Purpose:  Configures phase and timing tracking.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkTracking(ViSession instrSession,
                                                        ViInt32 phaseTracking,
                                                        ViBoolean timing)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHASE_TRACKING, phaseTracking),
		2, "Phase Tracking");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_TIMING, timing),
		3, "Timing");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink TDD Allocation
/* Purpose:  Configures the TDD UL/DL Allocations for DL.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkTDDAllocation(ViSession instrSession,
                                                             ViInt32 TDDULDLAllocations)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_TDD_UL_DL_ALLOCATIONS, TDDULDLAllocations),
		2, "TDD UL/DL Allocations");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  Configures the special subframe configuration for LTE downlink
/// HIFN  signals.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR specialSubframe/Selects the special subframe configuration for LTE downlink signals.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkTDDSpecialSubframe(
	ViSession instrSession,
	ViInt32 specialSubframe
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_TDD_SPECIAL_SUBFRAME, specialSubframe),
		2, "Special Subframe");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure LTE Downlink Physical Layer Cell Identity
/* Purpose:  Configures physical layer cell identity. There are 504 unique
/*           physical-layer cell identities. The physical-layer cell
/*           identities are  grouped into 168 unique physical-layer
/*           cell-identity groups, each group containing  three unique
/*           identities.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPhysicalLayerCellIdentity(ViSession instrSession,
                                                                         ViBoolean autoCellIdentityGroup,
                                                                         ViInt32 cellIdentityGroup,
                                                                         ViInt32 cellIdentity,
                                                                         ViBoolean autoCellIdentity)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (autoCellIdentityGroup)
	{
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CID_GROUP_AUTO, NULL),
			0, "");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CELL_IDENTITY_GROUP, cellIdentityGroup),
			3, "Cell Identity Group");
	}

	if (autoCellIdentity)
	{
		viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CELL_IDENTITY_AUTO, NULL),
			0, "");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CELL_IDENTITY, cellIdentity),
			4, "Auto Cell Identity");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PDSCH Configurable Subframes
/* Purpose:  Sets the number of configurable subframes for DL.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHConfigurableSubframes(ViSession instrSession,
                                                                          ViInt32 configurableSubframes)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_CONFIGURABLE_SUBFRAMES, configurableSubframes),
		2, "Configurable Subframes");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PDSCH Used Allocations
/* Purpose:  Configures the number of used allocations for a specific DL
/*           subframe. The suffix stands for the corresponding subframe
/*           number.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHUsedAllocations(ViSession instrSession,
                                                                    ViInt32 subframe,
                                                                    ViInt32 usedAllocations)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subframe, 0, 9),
		2, "Subframe");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SFR%ld", subframe);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_USED_ALLOCATIONS, usedAllocations),
		3, "Used Allocations");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PDSCH Used Allocation
/* Purpose:  Configures specified allocation in specified subframe.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHUsedAllocation(ViSession instrSession,
                                                                   ViInt32 subframe,
                                                                   ViInt32 usedAllocation,
                                                                   ViInt32 codeword,
                                                                   ViInt32 modulation,
                                                                   ViInt32 resourceBlocks,
                                                                   ViInt32 resourceBlocksOffset,
                                                                   ViReal64 power)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subframe, 0, 9),
		2, "Subframe");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, usedAllocation, 0, 99),
		3, "Used Allocation");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, codeword, 1, 2),
		4, "Codeword");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SFR%ld,AL%ld,CW%ld", subframe, usedAllocation, codeword);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_MODULATION, modulation),
		5, "Modulation");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SFR%ld,AL%ld", subframe, usedAllocation);
	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_RESOURCE_BLOCKS, resourceBlocks),
		6, "Resource Blocks");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_RESOURCE_BLOCKS_OFFSET, resourceBlocksOffset),
		7, "Resource Blocks Offset");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_POWER, power),
		8, "Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function defines the PDSCH start offset for a particular PDSCH
/// HIFN  allocation.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR subframe/Selects configurable subframe.
/// HIPAR usedAllocation/Selects allocation.
/// HIPAR resourceBlocksOffset/This control defines the PDSCH start offset for a particular PDSCH
/// HIPAR resourceBlocksOffset/allocation.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHStartOffset(
	ViSession instrSession,
	ViInt32 subframe,
	ViInt32 usedAllocation,
	ViInt32 resourceBlocksOffset
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, subframe, 0, 9),
		2, "Subframe");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, usedAllocation, 0, 99),
		3, "Used Allocation");

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "SFR%ld,AL%ld", subframe, usedAllocation);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_START_OFFSET, resourceBlocksOffset),
		4, "Resource Blocks Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN Configures the PRB Symbol Offset (Control Region for PDCCH).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR PRBSymbolOffset/Specifies the PRB Symbol Offset (Control Region for PDCCH).
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPRBSymbolOffset(ViSession instrSession,
                                                               ViInt32 PRBSymbolOffset)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PRB_SYMBOL_OFFSET, PRBSymbolOffset),
		2, "PRB Symbol Offset");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Configure LTE Downlink Reference Signal
/* Purpose:  Configures the relative power of the reference signal for DL.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkReferenceSignal(ViSession instrSession,
                                                               ViReal64 referencePower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_REFERENCE_POWER, referencePower),
		2, "Reference Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Synchronisation Signal
/* Purpose:  Configures synchronisation signal.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSynchronisationSignal(ViSession instrSession,
                                                                     ViReal64 pSYNCRelativePower,
                                                                     ViReal64 sSYNCRelativePower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PSYNC_RELATIVE_POWER, pSYNCRelativePower),
		2, "P-SYNC Relative Power");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_SSYNC_RELATIVE_POWER, sSYNCRelativePower),
		3, "S-SYNC Relative Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PBCH
/* Purpose:  Configures physical broadcast channel.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPBCH(ViSession instrSession,
                                                    ViBoolean PBCHPresent,
                                                    ViReal64 PBCHRelativePower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PBCH_STATE, PBCHPresent),
		2, "PBCH Present");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PBCH_RELATIVE_POWER, PBCHRelativePower),
		3, "PBCH Relative Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PCFICH
/* Purpose:  Configures PCFICH.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPCFICH(ViSession instrSession,
                                                      ViBoolean PCFICHPresent,
                                                      ViReal64 PCFICHRelativePower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PCFICH_STATE, PCFICHPresent),
		2, "PCFICH Present");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PCFICH_RELATIVE_POWER, PCFICHRelativePower),
		3, "PCFICH Relative Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PHICH
/* Purpose:  Configures PHICH.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPHICH(ViSession instrSession,
                                                     ViInt32 PHICHNumber_ofGroups,
                                                     ViInt32 PHICHDuration,
                                                     ViReal64 PHICHRelativePower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_NUMBER_OF_GROUPS, PHICHNumber_ofGroups),
		2, "PHICH Number of Groups");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_DURATION, PHICHDuration),
		3, "PHICH Duration");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_RELATIVE_POWER, PHICHRelativePower),
		4, "PHICH Relative Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Configures PHICH Ng parameter. Ng in combination with the number
/// HIFN  of resource blocks defines the number of PHICH groups in a
/// HIFN  downlink subframe.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR PHICHNgParameter/Selects the method that determines the number of PHICH groups in a subframe.
/// HIPAR PHICHNumberOfGroups/Configures the number of PHICH groups for DL.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPHICHNgParameter(ViSession instrSession,
                                                                ViInt32 PHICHNgParameter,
                                                                ViInt32 PHICHNumberOfGroups)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_NG_PARAMETER, PHICHNgParameter),
		2, "PHICH Ng Parameter");

	if (PHICHNgParameter == RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_CUSTOM)
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_NUMBER_OF_GROUPS, PHICHNumberOfGroups),
			3, "PHICH Number Of Groups");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function includes or excludes the use of the PHICH special
/// HIFN  setting for enhanced test models. The special setting is defined
/// HIFN  in 36.141 V9.0.0, 6.1.2.6: For frame structure type 2 the
/// HIFN  factor m_i shall not be set as per TS36.211, Table 6.9-1, but
/// HIFN  instead shall be set to m_i=1 for all transmitted subframes.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR PHICHTDDM_i1/Includes or excludes the use of the PHICH special setting for enhanced test models.
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPHICHEnhancedTestModels(ViSession instrSession,
                                                                       ViBoolean PHICHTDDM_i1)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_ENHANCED_TEST_MODELS, PHICHTDDM_i1),
		2, "PHICH TDD m_i=1");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink PDCCH
/* Purpose:  Configures PDCCH.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDCCH(ViSession instrSession,
                                                     ViInt32 number_ofPDCCHs,
                                                     ViInt32 PDCCHFormat,
                                                     ViReal64 PDCCHRelativePower)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PDCCH_NUMBER_OF_PDCCHS, number_ofPDCCHs),
		2, "Number of PDCCHs");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PDCCH_FORMAT, PDCCHFormat),
		3, "PDCCH Format");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_PDCCH_RELATIVE_POWER, PDCCHRelativePower),
		4, "PDCCH Relative Power");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Configures the PDSCH P_B parameter that defines the
/// HIFN  cell-specific ratio of rho_B to rho_A according to 3GPP TS
/// HIFN  36.213.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR PDSCHPowerRatio/Configures the PDSCH P_B parameter that defines the
/// HIPAR PDSCHPowerRatio/cell-specific ratio of rho_B to rho_A according to 3GPP TS
/// HIPAR PDSCHPowerRatio/36.213
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHPowerRatio(ViSession instrSession,
                                                               ViInt32 PDSCHPowerRatio)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_UPDATE_IQ_RESULTS, PDSCHPowerRatio),
		2, "PDSCH Power Ratio");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Update IQ Results
/* Purpose:  This command updates the current IQ measurement results to
/*           reflect the current measurement settings. Note no new IQ data is
/*           captured. I.e. the measurement settings apply to the IQ data
/*           being currently in the capture buffer. The command applies
/*           exclusively to IQ measurements. It requires available IQ data.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_UpdateIQResults(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_UPDATE_IQ_RESULTS, NULL),
		0, "");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Measurement Type
/* Purpose:  Selects LTE Downlink measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkMeasurementType(ViSession instrSession,
                                                               ViInt32 window,
                                                               ViInt32 measurementType)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld", window);

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_MEASUREMENT_TYPE, measurementType),
		3, "Measurement Type");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Subframe Selection
/* Purpose:  Configures the subframe to be analyzed.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSubframeSelection(ViSession instrSession,
                                                                 ViBoolean allSubframes,
                                                                 ViInt32 singleSubframeSelection)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	if (allSubframes)
	{
		viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_SUBFRAME_SELECTION_ALL, allSubframes),
			2, "All Subframes");
	}
	else
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_SUBFRAME_SELECTION, singleSubframeSelection),
			3, "Single Subframe Selection");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink EVM Units
/* Purpose:  Specifies the units for EVM results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkEVMUnits(ViSession instrSession,
                                                        ViInt32 EVMUnits)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_EVM_UNITS, EVMUnits),
		2, "EVM Units");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Bit Stream
/* Purpose:  Specifies if the bit stream gets displayed using bits or using
/*           symbols.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkBitStream(ViSession instrSession,
                                                         ViInt32 bitStreamFormat)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_BIT_STREAM_FORMAT, bitStreamFormat),
		2, "Bit Stream Format");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink SEM Settings
/* Purpose:  Specifies the SEM category defined in 3GPP TS 36.101 for Uplink.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSEMSettings(ViSession instrSession,
                                                           ViInt32 SEMChannelCategory)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_SEM_CHANNEL, SEMChannelCategory),
		2, "SEM Channel Category");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink ACP Settings
/* Purpose:  Configures adjacent channel power measurement settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkACPSettings(ViSession instrSession,
                                                           ViInt32 assumedAdjChannelCarrier,
                                                           ViBoolean noiseCorrection)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_ACP_ASSUMED_ADJ_CHANNEL_CARRIER, assumedAdjChannelCarrier),
		2, "Assumed Adj. Channel Carrier");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_ACP_NOISE_CORRECTION, noiseCorrection),
		3, "Noise Correction");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure LTE Downlink Constellation Location
/* Purpose:  Defines if the constellation diagram shall use data before or
/*           after the MIMO decoder.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkConstellationLocation(ViSession instrSession,
                                                                     ViInt32 constellationLocation)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_CONSTELLATION_LOCATION, constellationLocation),
		2, "Constellation Location");

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
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkYAxisScaling(
	ViSession instrSession,
	ViBoolean autoScaling,
	ViReal64 perDivision,
	ViReal64 offset
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_Y_AXIS_SCALING_AUTO, autoScaling),
		2, "Auto Scaling");

	if (autoScaling == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_Y_AXIS_SCALING_PER_DIVISION, perDivision),
			3, "Per Division");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_Y_AXIS_SCALING_OFFSET, offset),
			4, "Offset");
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Query LTE Downlink Measurement Result Summary
/* Purpose:  Queries LTE Downlink measurement result summary.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementResultSummary(ViSession instrSession,
                                                                    ViInt32 frameResult,
                                                                    ViReal64* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	switch (frameResult)
	{
	case RSSPECAN_VAL_LTE_DOWNLINK_QPSK:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PDSCH_QPSK_RESULT, result),
			3, "Result");
		break;
	case RSSPECAN_VAL_LTE_DOWNLINK_16QAM:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PDSCH_16QAM_RESULT, result),
			3, "Result");
		break;
	case RSSPECAN_VAL_LTE_DOWNLINK_64QAM:
		viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PDSCH_64QAM_RESULT, result),
			3, "Result");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, frameResult), 2, "Frame result");
		break;
	}

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement EVM All
/* Purpose:  Queries LTE Downlink measurement EVM All result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMAll(ViSession instrSession,
                                                             ViInt32 measurementType,
                                                             ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_EVM_ALL_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement EVM Physical Channel
/* Purpose:  Queries the "EVM Physical Channel" value from the result summary
/*           list.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMPhysicalChannel(ViSession instrSession,
                                                                         ViInt32 measurementType,
                                                                         ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PHYSICAL_CHANNEL_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement EVM Physical Signal
/* Purpose:  Queries the "EVM Physical Signal" value from the result summary
/*           list.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMPhysicalSignal(ViSession instrSession,
                                                                        ViInt32 measurementType,
                                                                        ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PHYSICAL_SIGNAL_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Queries the EVM of all DMRS resource elements with 64QAM modulation of
/// HIFN  the PUSCH.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_EVM_SDSF_AVERAGE
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:SUMMary:EVM:SDSF[:AVERage]?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR result/Returns the EVM of all DMRS resource elements with 64QAM modulation of
/// HIPAR result/the PUSCH.
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMSDSFAverage(
	ViSession instrSession,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_EVM_SDSF_AVERAGE, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement Frequency Error
/* Purpose:  Queries "Frequency Error" value from the result summary list in
/*           Hz.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementFrequencyError(ViSession instrSession,
                                                                     ViInt32 measurementType,
                                                                     ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_FREQUENCY_ERROR_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement Sampling Error
/* Purpose:  Queries "Sampling Error" value from the result summary list in
/*           ppm.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementSamplingError(ViSession instrSession,
                                                                    ViInt32 measurementType,
                                                                    ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_SAMPLING_ERROR_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement IQ Offset
/* Purpose:  Returns the IQ offset from the result summary list in dB.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementIQOffset(ViSession instrSession,
                                                               ViInt32 measurementType,
                                                               ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_IQ_OFFSET_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement IQ Gain Imbalance
/* Purpose:  Returns the IQ gain imbalance from the result summary list in
/*           dB.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementIQGainImbalance(ViSession instrSession,
                                                                      ViInt32 measurementType,
                                                                      ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_IQ_GAIN_IMBALANCE_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement IQ Quadrature Error
/* Purpose:  Returns the IQ quadrature error from the result summary list in
/*           degrees.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementIQQuadratureError(ViSession instrSession,
                                                                        ViInt32 measurementType,
                                                                        ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_IQ_QUADRATURE_ERROR_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement Frame Power
/* Purpose:  Returns the "Frame Power" from the result summary list in dBm.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementFramePower(ViSession instrSession,
                                                                 ViInt32 measurementType,
                                                                 ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_FRAME_POWER_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement Crest Factor
/* Purpose:  Returns the crest factor from the result summary list in dB.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementCrestFactor(ViSession instrSession,
                                                                  ViInt32 measurementType,
                                                                  ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_CREST_FACTOR_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement Trigger To Frame
/* Purpose:  Returns the trigger to frame value.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementTriggerToFrame(ViSession instrSession,
                                                                     ViReal64* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_TRIGGER_TO_FRAME_RESULT, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Measurement OSTP
/* Purpose:  Returns the OSTP value from the result list in dBm.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementOSTP(ViSession instrSession,
                                                           ViReal64* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_OSTP_RESULT, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN Returns the reference signal transmit power (RSTP) from the result list.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR mode/Sets the measurement mode.
/// HIPAR result/Returns the RSTP value from the result list as defined in 3GPP TS 36.141.
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementRSTP(ViSession instrSession,
                                                           ViInt32 mode,
                                                           ViReal64* result)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (mode)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 2, "Mode");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_RSTP_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  Queries the RSSI as shown in the result summary.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_LTE_DOWNLINK_RSSI_RESULT
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:SUMMary:RSSI:MAXimum
/// HIFN     FETCh:SUMMary:RSSI:MINimum
/// HIFN     FETCh:SUMMary:RSSI[:AVERage]?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR mode/Sets the measurement mode.
/// HIPAR result/Queries the RSSI as shown in the result summary.
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementRSSI(
	ViSession instrSession,
	ViInt32 mode,
	ViReal64* result
)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCap[RS_REPCAP_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (mode)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Min");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Max");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		snprintf(repCap, RS_REPCAP_BUF_SIZE, "Aver");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, mode), 2, "Mode");
		break;
	}

	viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_LTE_DOWNLINK_RSSI_RESULT, result),
		3, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/*===========================================================================*/
/* Function: Query LTE Downlink Measurement ACP Result
/* Purpose:  This function queries the current result values of the adjacent
/*           channel power measurement. An ACPR (Adjacent channel power
/*           relative) measurement must have previously been run for there to
/*           be summary data available.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementACPResult(ViSession instrSession,
                                                                ViInt32 arraySize,
                                                                ViReal64 result[])
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K100|K102"));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 5, 5),
		2, "Array size");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "CALC1:MARK:FUNC:POW:RES?", arraySize, result, NULL));
	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Detected Cell Identity Group
/* Purpose:  Returns the cell identity group detected by the DSP kernel. If
/*           no valid value has been detected yet, the function will return
/*           -1.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkDetectedCellIdentityGroup(ViSession instrSession,
                                                                     ViInt32* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_DETECTED_CELL_IDENTITY_GROUP, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Detected Cell Identity
/* Purpose:  Returns the cell identity detected by the DSP kernel. If no
/*           valid value has been detected yet, the function will return -1.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkDetectedCellIdentity(ViSession instrSession,
                                                                ViInt32* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_DETECTED_CELL_IDENTITY, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Query LTE Downlink Detected Cyclic Prefix
/* Purpose:  Returns the cyclic prefix detected by the DSP kernel. If no
/*           valid value has been detected yet, the function will return -1.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkDetectedCyclicPrefix(ViSession instrSession,
                                                                ViInt32* result)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_LTE_DOWNLINK_DETECTED_CYCLIC_PREFIX, result),
		2, "Result");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink EVM versus Carrier Trace
/* Purpose:  Returns the EVM-vs.-Carrier-values as list over all carriers.
/*           Depends on the Subframe selection that can be made in the
/*           General Settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkEVMVersusCarrierTrace(ViSession instrSession,
                                                                ViInt32 measurementType,
                                                                ViInt32 arraySize,
                                                                ViReal64 traceData[],
                                                                ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		sprintf(traceName, "TRACE2");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		sprintf(traceName, "TRACE3");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		sprintf(traceName, "TRACE1");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink EVM versus Symbol Trace
/* Purpose:  Returns the EVM-vs.-Symbol-values as list over all symbols.  If
/*           a single subframe is selected in the General Settings, only the
/*           symbols of the selected subframe will be returned.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkEVMVersusSymbolTrace(ViSession instrSession,
                                                               ViInt32 arraySize,
                                                               ViReal64 traceData[],
                                                               ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(traceName, "TRACE1");

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Frequency Error versus Symbol Trace
/* Purpose:  Returns the Frequency Error vs. Symbol values as list over all
/*           symbols in increasing order. If a single subframe is selected in
/*           the "General Settings", only the symbols of the selected
/*           subframe are returned.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkFrequencyErrorVersusSymbolTrace(ViSession instrSession,
                                                                          ViInt32 arraySize,
                                                                          ViReal64 traceData[],
                                                                          ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(traceName, "TRACE1");

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink EVM versus Subframe Trace
/* Purpose:  Returns the EVM-vs.-Symbol-values as list over all subframes.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkEVMVersusSubframeTrace(ViSession instrSession,
                                                                 ViInt32 arraySize,
                                                                 ViReal64 traceData[],
                                                                 ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(traceName, "TRACE1");

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink SEM Trace
/* Purpose:  Returns the spectrum emission mask results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkSEMTrace(ViSession instrSession,
                                                   ViInt32 measurementType,
                                                   ViInt32 arraySize,
                                                   ViReal64 traceData[],
                                                   ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_VALUES:
		sprintf(traceName, "TRACE1");
		break;
	case RSSPECAN_VAL_MEASTYPE_SUMMARY:
		sprintf(traceName, "LIST");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink ACP Trace
/* Purpose:  Returns the ACP power values as vector.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkACPTrace(ViSession instrSession,
                                                   ViInt32 arraySize,
                                                   ViReal64 traceData[],
                                                   ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(traceName, "TRACE1");

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Power Spectrum Trace
/* Purpose:  Returns the signal power in dBm/Hz as list over the considered
/*           frequency span.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkPowerSpectrumTrace(ViSession instrSession,
                                                             ViInt32 arraySize,
                                                             ViReal64 traceData[],
                                                             ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	sprintf(traceName, "TRACE1");

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Power versus RB Reference Signal Trace
/* Purpose:  Returns the reference signal power per RB in dBm as list over
/*           the currently available RBs.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkPowerVersusRBReferenceSignalTrace(ViSession instrSession,
                                                                            ViInt32 measurementType,
                                                                            ViInt32 arraySize,
                                                                            ViReal64 traceData[],
                                                                            ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		sprintf(traceName, "TRACE2");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		sprintf(traceName, "TRACE3");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		sprintf(traceName, "TRACE1");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Power versus RB PDSCH Trace
/* Purpose:  Returns the PDSCH signal power per RB in dBm as list over the
/*           currently available RBs.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkPowerVersusRBPDSCHTrace(ViSession instrSession,
                                                                  ViInt32 measurementType,
                                                                  ViInt32 arraySize,
                                                                  ViReal64 traceData[],
                                                                  ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		sprintf(traceName, "TRACE2");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		sprintf(traceName, "TRACE3");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		sprintf(traceName, "TRACE1");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Channel Flatness Trace
/* Purpose:  Returns the spectrum flatness in dB as list over the considered
/*           frequency span. Depends on the subframe selection that can be
/*           made in the General Settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkChannelFlatnessTrace(ViSession instrSession,
                                                               ViInt32 measurementType,
                                                               ViInt32 arraySize,
                                                               ViReal64 traceData[],
                                                               ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		sprintf(traceName, "TRACE2");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		sprintf(traceName, "TRACE3");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		sprintf(traceName, "TRACE1");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Channel Group Delay Trace
/* Purpose:  Returns the channel group delay in ns as list over the
/*           considered frequency span. Depends on the Subframe selection
/*           that can be made in the General Settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkChannelGroupDelayTrace(ViSession instrSession,
                                                                 ViInt32 measurementType,
                                                                 ViInt32 arraySize,
                                                                 ViReal64 traceData[],
                                                                 ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		sprintf(traceName, "TRACE2");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		sprintf(traceName, "TRACE3");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		sprintf(traceName, "TRACE1");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Channel Flatness Difference Trace
/* Purpose:  Returns the channel flatness difference in dB as list over the
/*           considered frequency span. Depends on the Subframe selection
/*           that can be made in the General Settings.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkChannelFlatnessDifferenceTrace(ViSession instrSession,
                                                                         ViInt32 measurementType,
                                                                         ViInt32 arraySize,
                                                                         ViReal64 traceData[],
                                                                         ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_MIN:
		sprintf(traceName, "TRACE2");
		break;
	case RSSPECAN_VAL_MEASTYPE_MAX:
		sprintf(traceName, "TRACE3");
		break;
	case RSSPECAN_VAL_MEASTYPE_AVER:
		sprintf(traceName, "TRACE1");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Constellation Diagram
/* Purpose:  This measurement represents I and Q data. Data will be returned
/*           as an array of interleaved I and Q data until all data is
/*           exhausted.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkConstellationDiagram(ViSession instrSession,
                                                               ViInt32 bufferSize,
                                                               ViInt32* noofPoints,
                                                               ViReal64 realParts_I[],
                                                               ViReal64 imaginaryParts_Q[])
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize, i;
	ViInt32 j = 0;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_QueryBinaryOrAsciiFloatArray(instrSession, ":TRAC?", &data, &dataSize));
	checkErr(rsspecan_CheckStatus(instrSession));

	dataSize /= 2;

	if (noofPoints)
		*noofPoints = dataSize;

	if (dataSize > bufferSize)
		dataSize = bufferSize;

	for (i = 0; i < dataSize; i++)
	{
		realParts_I[i] = data[j++];
		imaginaryParts_Q[i] = data[j++];
	}

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink CCDF
/* Purpose:  Returns the complementary cumulative distribution function
/*           results in percent as list over the power level in dB.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkCCDF(ViSession instrSession,
                                               ViInt32 measurementType,
                                               ViInt32 arraySize,
                                               ViReal64 traceData[],
                                               ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar traceName[RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

	switch (measurementType)
	{
	case RSSPECAN_VAL_MEASTYPE_PROBABILITY:
		sprintf(traceName, "TRACE1");
		break;
	case RSSPECAN_VAL_MEASTYPE_POWER_STEPS:
		sprintf(traceName, "TRACE2");
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, measurementType), 2, "Measurement Type");
		break;
	}

	checkErr(rsspecan_dataReadTrace (instrSession, 0, traceName, arraySize,
		traceData, noofValues));

	checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Allocation Summary
/* Purpose:  Returns the complementary cumulative distribution function
/*           results in percent as list over the power level in dB. The first
/*           value returned represents the number of following values.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkAllocationSummary(ViSession instrSession,
                                                            ViInt32 arraySize,
                                                            ViInt32 subframeNumber[],
                                                            ViInt32 allocationID[],
                                                            ViInt32 number_ofRB[],
                                                            ViInt32 offsetRB[],
                                                            ViInt32 modulation[],
                                                            ViReal64 power[],
                                                            ViReal64 EVM[],
                                                            ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataSize, i;
	ViInt32 j = 0;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K100|K102"));

	checkErr(RsCore_QueryBinaryOrAsciiFloatArray(instrSession, "TRACE?", &data, &dataSize));
	checkErr(rsspecan_CheckStatus(instrSession));

	dataSize /= 7;

	if (noofValues)
		*noofValues = dataSize;

	if (dataSize > arraySize)
		dataSize = arraySize;

	for (i = 0; i < dataSize; i++)
	{
		subframeNumber[i] = (ViInt32)data[j++];
		allocationID[i] = (ViInt32)data[j++];
		number_ofRB[i] = (ViInt32)data[j++];
		offsetRB[i] = (ViInt32)data[j++];
		modulation[i] = (ViInt32)data[j++];
		power[i] = data[j++];
		EVM[i] = data[j++];
	}

Error:
	if (data) free(data);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Read LTE Downlink Bitstream
/* Purpose:  Bitsteam data will be returned in groups where each group
/*           represents one line of the table.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkBitstream(ViSession instrSession,
                                                    ViInt32 arraySize,
                                                    ViInt32 bitStreamSize,
                                                    ViInt32 subframeNumber[],
                                                    ViInt32 allocationID[],
                                                    ViInt32 codeword[],
                                                    ViInt32 modulation[],
                                                    ViInt32 number_ofSymbols[],
                                                    ViChar bitStreams[],
                                                    ViInt32* noofValues)
{
	ViStatus error = VI_SUCCESS;
	ViChar *pBuffer = NULL, *pValue;
	ViInt32 nType = 0;
	ViInt32 nIndexSymbol = 0, nCurrentSymbol = 0;
	ViInt32 nSymbols = 0;
	ViInt32 nSymbolValue;

	checkErr(RsCore_LockSession(instrSession));

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K100|K102"));

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, "TRACE?", &pBuffer));

	*noofValues = 0;
	pValue = strtok(pBuffer, ",");

	nType = 0;

	while (pValue)
	{
		if (*noofValues >= arraySize)
		{
			viCheckParm(RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
			break;
		}
		else
		{
			switch (nType)
			{
			case 0:
				subframeNumber[*noofValues] = atoi(pValue);
				nType++;
				break;
			case 1:
				allocationID[*noofValues] = atoi(pValue);
				nType++;
				break;
			case 2:
				codeword[*noofValues] = atoi(pValue);
				nType++;
				break;
			case 3:
				modulation[*noofValues] = atoi(pValue);
				nType++;
				break;
			case 4:
				nSymbols = atoi(pValue);
				nCurrentSymbol = 0;
				number_ofSymbols[*noofValues] = nSymbols;
				nType++;
				break;
			case 5: // stream
				sscanf(pValue, "%lx", &nSymbolValue);
				bitStreams[nIndexSymbol + nCurrentSymbol] = (ViChar)nSymbolValue;

				nCurrentSymbol++;
				if (nCurrentSymbol >= nSymbols)
				{
					nIndexSymbol += nSymbols;
					nType = 0;
					(*noofValues)++;
				}

				if (nIndexSymbol + nCurrentSymbol >= bitStreamSize)
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
	if (pBuffer) free(pBuffer);
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
