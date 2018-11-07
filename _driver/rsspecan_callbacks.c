/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  Callbacks
 *
 *  Original Release: September 2008
 *  By: Jiri Kominek
 *
 *  Should you have any technical questions please contact the hotline of
 *  Rohde & Schwarz Vertriebs-GmbH Rohde & Schwarz Support Center
 *
 *  e-mail: CustomerSupport@rohde-schwarz.com
 *

 *****************************************************************************/

#include "rsspecan.h"
#include <float.h>
#include <math.h>

#pragma warning (disable:4100) // unreferenced formal parameter

/****/

ViStatus rsspecan_doubleQuotedString_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr);
ViStatus rsspecan_doubleQuotedString_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value);


/***/

/*****************************************************************************
 *- Read Callbacks ----------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_empty_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Returns value
 *           from cache
 *****************************************************************************/
ViStatus rsspecan_empty_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return VI_SUCCESS;
}

/*===========================================================================*/
/* Function: Calibration User Callback                                       */
/* Purpose:  Forces OPC sync				                                 */
/*===========================================================================*/
ViStatus rsspecan_Calibration_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttrFlags oldFlags = attr->flags;

	attr->flags |= RS_VAL_WAIT_FOR_OPC_AFTER_WRITES;

	checkErr(RsCore_ReadCallback(instrSession, repCapName, attr));

Error:
	attr->flags = oldFlags;
	return error;
}

/*****************************************************************************
 * Function: rsspecan_stringToBin_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToBin_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_SpecialCallback_BinPatternRead(instrSession, repCapName, attr);
}

/*****************************************************************************
 * Function: rsspecan_stringToHex_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToHex_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_SpecialCallback_HexPatternRead(instrSession, repCapName, attr);
}

/*****************************************************************************
 * Function: rsspecan_hex_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_hex_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_SpecialCallback_HexNumberRead(instrSession, repCapName, attr);
}

/*****************************************************************************
 * Function: rsspecan_displayComment_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Reads string
 *           returned with apostrophes
 *****************************************************************************/
ViStatus rsspecan_displayComment_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_ReadCallback(instrSession, repCapName, attr);
}

/*****************************************************************************
* Function: rsspecan_quotedString_ReadCallback
* Purpose:  This function overrides standard calback funtion. Reads string
*           returned with apostrophes
*****************************************************************************/
ViStatus rsspecan_quotedString_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_ReadCallback(instrSession, repCapName, attr);
}

/*****************************************************************************
* Function: rsspecan_quotedString_ReadCallback
* Purpose:  This function overrides standard calback funtion. Reads string
*           returned with apostrophes
*****************************************************************************/
ViStatus rsspecan_doubleQuotedString_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_ReadCallback(instrSession, repCapName, attr);
}

/*****************************************************************************
* Function: rsspecan_Item1Item2Itemx_ReadCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_Item1Item2Itemx_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_ReadCallback(instrSession, repCapName, attr);
}

/*****************************************************************************
* Function: rsspecan_quotedInt_ReadCallback
* Purpose:  This function overrides standard calback funtion. Reads int
*           returned with apostrophes
*****************************************************************************/
ViStatus rsspecan_quotedInt_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return rsspecan_quotedString_ReadCallback(instrSession, repCapName, attr);
}

/*****************************************************************************
 * Function: rsspecan_DeltaMarkerReferenceMarker_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_DeltaMarkerReferenceMarker_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;

	//TODO: Implement
	return error;
}

/*****************************************************************************
 *- Write Callbacks ---------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_empty_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Does not write
 *           anything to instrument, just updates the cache
 *****************************************************************************/
ViStatus rsspecan_empty_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return VI_SUCCESS;
}

/*****************************************************************************
 * Function: rsspecan_stringToBin_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToBin_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_BinPatternWrite(instrSession, repCapName, attr, value);
}

/*****************************************************************************
 * Function: rsspecan_stringToHex_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToHex_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_HexPatternWrite(instrSession, repCapName, attr, value);
}

/*****************************************************************************
 * Function: rsspecan_hex_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_hex_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_HexNumberWrite(instrSession, repCapName, attr, value);
}

/*****************************************************************************
 * Function: rsspecan_InstrumentCreate_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_InstrumentCreate_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " 'Spectrum ", value, "'");
}

/*****************************************************************************
* Function: rsspecan_quotedString_WriteCallback
* Purpose: Writes string with single quotes
*****************************************************************************/
ViStatus rsspecan_quotedString_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_WriteCallback(instrSession, repCapName, attr, value);
}

/*******************************************************************************
 * Function: rsspecan_outputSelectionCallback_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Writes string
 *           with apostrophes, when string is FOC, writes it without apostrophes
 *******************************************************************************/
ViStatus rsspecan_outputSelectionCallback_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;

	if (attr->dataType != RS_VAL_STRING)
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
		"rsspecan_outputSelectionCallback_WriteCallback: Data type must be RS_VAL_STRING"));

	if (strcmp((char*)value, "FOC") == 0)
	{
		checkErr(RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " ", value, ""));
	}
	else
	{
		checkErr(RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " '", value, "'"));
	}

Error:
	return error;
}

/*****************************************************************************
* Function: rsspecan_doubleQuotedString_WriteCallback
* Purpose:  Writes string with double quotes
*****************************************************************************/
ViStatus rsspecan_doubleQuotedString_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " \"", value, "\"");
}

/*****************************************************************************
 * Function: rsspecan_FileStateSpecialFormat_WriteCallback
 * Purpose:  This function formats 1, before the argument string
 *****************************************************************************/
ViStatus rsspecan_FileStateSpecialFormat_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " 1,'", value, "'");
}

/*****************************************************************************
* Function: rsspecan_HcopyColorDef_WriteCallback
* Purpose:  Writes int without a space %s%ld
*****************************************************************************/
ViStatus rsspecan_HcopyColorDef_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, "", value, "");
}

/*****************************************************************************
 * Function: rsspecan_quotedInt_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Writes int
 *           with apostrophes
 *****************************************************************************/
ViStatus rsspecan_quotedInt_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return rsspecan_quotedString_WriteCallback(instrSession, repCapName, value, value);
}

/*****************************************************************************
 * Function: rsspecan_FFTPhaseLinePosition_WriteCallback
 * Purpose:  Writes data with DEG suffix
 *****************************************************************************/
ViStatus rsspecan_FFTPhaseLinePosition_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " ", value, "DEG");
}

/*****************************************************************************
* Function: rsspecan_VsaDemodRecLengthSym_WriteCallback
* Purpose:  Writes values with SYM afterwards
*****************************************************************************/
ViStatus rsspecan_VsaDemodRecLengthSym_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " ", value, "SYM");
}

/*****************************************************************************
* Function: rsspecan_VsaDemodRecLengthSec_WriteCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_VsaDemodRecLengthSec_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_SpecialCallback_SurroundParamWrite(instrSession, repCapName, attr, " ", value, "S");
}

/******************************************************************************************************
	rsspecan_DeltaMarkerReferenceMarker_WriteCallback
	Supported data types: RS_VAL_INT32, RS_VAL_REAL64
	Writes as integer, but for 0 it writes "FIX" instead of the number
*****************************************************************************************************/
ViStatus rsspecan_DeltaMarkerReferenceMarker_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];

	// If no command available, exit the function
	if (attr->command == NULL)
		return VI_SUCCESS;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
		if (*(ViInt32 *)value == 0)
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, "FIX");
		else
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, _PERCD, *(ViInt32 *)value);

		break;
	case RS_VAL_REAL64:
		if (fabs(*(ViReal64 *)value) <= DBL_MIN)
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, "FIX");
		else
			RsCore_Convert_ViReal64_To_String(instrSession, RS_MAX_MESSAGE_BUF_SIZE, buffer);

		break;
	default:
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
			"rsspecan_DeltaMarkerReferenceMarker_WriteCallback only accepts type RS_VAL_INT32, RS_VAL_REAL64"));
	}

	checkErr(RsCore_StrCatDynamicString(instrSession, &cmd, " ", buffer, NULL));
	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/*****************************************************************************
*- Check Callbacks ---------------------------------------------------------*
*****************************************************************************/

/*****************************************************************************
 *- Coerce Callbacks --------------------------------------------------------*
 *****************************************************************************/


/*****************************************************************************
 *- Range Table Callbacks ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_EVMCalculation_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_EVMCalculation_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (rsspecan_IsFSV(instrSession))
	{
		*rangeTable = &rsspecan_rngVSAModulationEVMCalcFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngVSAModulationEVMCalc;
	}

	return error;
}

/*****************************************************************************
* Function: rsspecan_SpectrogramHistoryDepth_RangeTableCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_SpectrogramHistoryDepth_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSVR"))
	{
		*rangeTable = &rsspecan_rngSpemHistoryBuffDepthFSVR;
	}
	else if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngSpemHistoryBuffDepthFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngSpemHistoryBuffDepth;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_XAxisQuantize_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_XAxisQuantize_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngVSAConfDisplayXQuantizeFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngVSAConfDisplayXQuantize;
	}

	return error;
}

/*****************************************************************************
* Function: rsspecan_TraceRange_RangeTableCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_TraceRange_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV|FSL"))
	{
		*rangeTable = &rsspecan_rngFslTrace;
	}
	else
	{
		*rangeTable = &rsspecan_rngTrace;
	}

	return error;
};

/*****************************************************************************
* Function: rsspecan_AmpUnit_RangeTableCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_AmpUnit_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		*rangeTable = &rsspecan_rngFslAmplitudeUnitsRangeTable;
	}
	else
	{
		*rangeTable = &rsspecan_rngAmplitudeUnitsRangeTable;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_PowStandard_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_PowStandard_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		*rangeTable = &rsspecan_rngFslMeasPowerStandard;
	}
	else if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngFsvMeasPowerStandard;
	}
	else
	{
		*rangeTable = &rsspecan_rngMeasPowerStandard;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_ExtMixerHarmonic_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_ExtMixerHarmonic_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngExtMixerHarmonicFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngExtMixerHarmonic;
	}

	return error;
}

/*****************************************************************************
* Function: rsspecan_BtoChannel_RangeTableCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_BtoChannel_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 region;

	checkErr(rsspecan_GetAttributeViInt32(instrSession,"", RSSPECAN_ATTR_BTO_GEOG, &region));
	switch (region)
	{
	case RSSPECAN_BTO_GEOG_EUR:
	case RSSPECAN_BTO_GEOG_USA:
		*rangeTable = &rsspecan_rngBtoChannel;
		break;
	case RSSPECAN_BTO_GEOG_FRAN:
		*rangeTable = &rsspecan_rngBtoChannelFranc;
		break;
	}

Error:
	return error;
};

ViStatus rsspecan_C2kSetCount_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;
	ViChar mode[RS_MAX_MESSAGE_LEN] = "";

	checkErr(rsspecan_GetAttributeViString(instrSession,"", RSSPECAN_ATTR_GET_INSTR_MODE, RS_MAX_MESSAGE_LEN, mode));
	if ((strcmp(mode, "BC2K") == 0) || (strcmp(mode, "M2CK") == 0))
		*rangeTable = &rsspecan_rngC2KSetCount;
	else
		*rangeTable = &rsspecan_rngBDOSetCount;

Error:
	return error;
}

/*****************************************************************************
 * Function: rsspecan_IQSrate_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_IQSrate_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (!RsCore_HasInstrumentOptions(instrSession, "B72"))
		*rangeTable = &rsspecan_rngIQSRate;
	else
		*rangeTable = &rsspecan_rngIQSRateB72;

	return error;
}

/*****************************************************************************
 * Function: rsspecan_AvgType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_AvgType_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV|FSL"))
	{
		*rangeTable = &rsspecan_rngAvgTypeFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngAvgType;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_InpAmpEattMan_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_InpAmpEattMan_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngInpAmptEattManFSV;
	}
	else if (RsCore_IsInstrumentModel(instrSession, "FSW"))
	{
		*rangeTable = &rsspecan_rngInpAmptEattManFSW;
	}
	else
	{
		*rangeTable = &rsspecan_rngInpAmptEattMan;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_AdemSpecBW_FSV_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_AdemSpecBW_FSV_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngAdemBandResFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngAdemBandRes;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_ZoneType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_ZoneType_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngWiMAXZoneTypeFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngWiMAXZoneType;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_SEListRangeFilterType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_SEListRangeFilterType_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV"))
	{
		*rangeTable = &rsspecan_rngListFiltTypeFSV;
	}
	else
	{
		*rangeTable = &rsspecan_rngListFiltType;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_HCopyLanguage_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_HCopyLanguage_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSV|FSL"))
	{
		*rangeTable = &rsspecan_rngHcopyDeviceLangFSL;
	}
	else
	{
		*rangeTable = &rsspecan_rngHcopyDeviceLang;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_IQSrateFMU_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_IQSrateFMU_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (!RsCore_IsInstrumentModel(instrSession, "FMU|FSQ"))
	{
		*rangeTable = &rsspecan_rngIQSRate;

		if (RsCore_HasInstrumentOptions(instrSession, "B72") || RsCore_IsInstrumentModel(instrSession, "FSV") && RsCore_HasInstrumentOptions(instrSession, "B72"))
			*rangeTable = &rsspecan_rngIQSRateB72;
	}
	else
		*rangeTable = &rsspecan_rngIQSRateFMU;

	return error;
}

/*****************************************************************************
 * Function: rsspecan_DetectorType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_DetectorType_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FMU|FSW"))
		*rangeTable = &rsspecan_rngDetectorTypeFMURangeTable;
	else if (RsCore_IsInstrumentModel(instrSession, "FSV"))
		*rangeTable = &rsspecan_rngDetectorTypeESLRangeTable;
	else
		*rangeTable = &rsspecan_rngDetectorTypeRangeTable;

	return error;
}

/*****************************************************************************
 * Function: rsspecan_BBInput_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_BBInput_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (!RsCore_IsInstrumentModel(instrSession, "FMU"))
		*rangeTable = &rsspecan_rngBBInput;
	else
		*rangeTable = &rsspecan_rngBBInputFMU;

	return error;
}

/*****************************************************************************
 * Function: rsspecan_TriggerRFLevel_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_TriggerRFLevel_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (!RsCore_IsInstrumentModel(instrSession, "FSV"))
		*rangeTable = &rsspecan_rngTrigRFPowerLevel;
	else
		*rangeTable = &rsspecan_rngTrigRFPowerLevelFSV;

	return error;
}

/*****************************************************************************
 * Function: rsspecan_TraceRange_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_NoiseMarkerAssign_RangeTableCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, RsCoreRangeTablePtr* rangeTable)
{
	ViStatus error = VI_SUCCESS;

	if (RsCore_IsInstrumentModel(instrSession, "FSW"))
	{
		*rangeTable = &rsspecan_rngNoiseMarkerAssignFSW;
	}
	else
	{
		*rangeTable = &rsspecan_rngNoiseMarkerAssign;
	}

	return error;
}

/*****************************************************************************
 * Function: rsspecan_Attenuation_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_Attenuation_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];

	// If no command available, exit the function
	if (attr->command == NULL)
		return VI_SUCCESS;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, "%s %ld.0", cmd, (ViInt32)*(ViReal64 *)value);
		break;
	case RS_VAL_REAL64:
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, "%s %.0f", cmd, *(ViReal64 *)value);
		break;
	default:
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
			"rsspecan_Attenuation_WriteCallback only accepts type RS_VAL_INT32, RS_VAL_REAL64"));
	}

	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/*****************************************************************************
* Function: rsspecan_WimaxZoneToUse_ReadCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_WimaxZoneToUse_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	return RsCore_ReadCallback(instrSession, repCapName, attr);
}

/*****************************************************************************
* Function: rsspecan_WimaxZoneToUse_WriteCallback
* Purpose:  This function overrides standard calback funtion.
*****************************************************************************/
ViStatus rsspecan_WimaxZoneToUse_WriteCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	return RsCore_WriteCallback(instrSession, repCapName, attr, value);
}
