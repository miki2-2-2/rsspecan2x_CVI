/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  B17 - Digital Baseband Input
 *
 *  Original Release: October 2007
 *  By: Jiri Kominek (instrument driver core)
 *      Helena Kominkova (instrument specific implementation)
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
/* Function: Configure Digital Baseband Input
/* Purpose:  This function configures digital baseband input.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInput(ViSession instrSession,
                                                         ViInt32 basebandInputs)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_BB_INPUT_SELECTION, basebandInputs),
		2, "Baseband Inputs");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}


/// HIFN  If enabled, the reference level for digital input is adjusted to the full scale level automatically if the fullscale level changes.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR basebandInputCoupling/If enabled, the reference level for digital input is adjusted to the full scale level automatically if the fullscale level changes.

ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputCoupling(ViSession instrSession,
                                                                 ViBoolean basebandInputCoupling)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_COUPLING, basebandInputCoupling),
		2, "Baseband Input Coupling");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/// HIFN  This function activates/deactivates auto set of digital baseband input parameters.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR autoRange/This control activates/deactivates the automatically adjust of digital in full scale.
/// HIPAR autoSampleRate/This control activates/deactivates the automatically adjust of digital in sample rate.

ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputParametersAuto(ViSession instrSession,
                                                                       ViBoolean autoRange,
                                                                       ViBoolean autoSampleRate)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_RANGE_AUTO, autoRange),
		2, "Digital Input Range Auto");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_SAMPLE_RATE_AUTO, autoSampleRate),
		3, "Auto Sample Rate");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Digital Baseband Input Parameters
/* Purpose:  This function configures digital baseband input parameters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputParameters(ViSession instrSession,
                                                                   ViReal64 range,
                                                                   ViReal64 sampleRate)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_RANGE, range),
		2, "Range");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_INPUT_SRATE, sampleRate),
		3, "Sample Rate");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Digital Baseband Output
/* Purpose:  This function configures the IQ data output of the Digital
/*           Baseband interface.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandOutput(ViSession instrSession,
                                                          ViBoolean output)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_DIGITAL_BASEBAND_OUTPUT_STATE, output),
		2, "Output");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Digital Baseband Full Scale Level
/* Purpose:  This function configures the full scale level.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandFullScaleLevel(ViSession instrSession,
                                                                  ViReal64 fullScaleLevel)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_BASEBAND_FULL_SCALE_LEVEL, fullScaleLevel),
		2, "Full Scale Level");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*****************************************************************************
 * Function: Query Digital Baseband Input Output Configuration
 * Purpose:  This function returns the current configuration and status of the
 *           Digital Baseband input or output of option R&S FSQ-B17. The returned
 *           string includes the name of the connected device, the serial
 *           number of this device, the port name and the sample rate,
 *           separated by comma, e.g. "AMU200A,100266,Out A, 80000000.0".
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryDigitalBasebandInputOutputConfiguration(ViSession instrSession,
                                                                        ViInt32 digitalBasebandSelect,
                                                                        ViInt32 arraySize,
                                                                        ViChar configuration[])
{
	ViStatus error = VI_SUCCESS;
	ViAttr attr = 0;

	checkErr(RsCore_LockSession(instrSession));

	switch (digitalBasebandSelect)
	{
	case RSSPECAN_VAL_DIQ_IN:
		attr = RSSPECAN_ATTR_QUERY_DIGITAL_INPUT_CONF;
		break;
	case RSSPECAN_VAL_DIQ_OUT:
		attr = RSSPECAN_ATTR_QUERY_DIGITAL_OUTPUT_CONF;
		break;
	default:
		viCheckParm(RsCore_InvalidViInt32Value(instrSession, digitalBasebandSelect), 2, "Digital Baseband Select");
	}

	checkErr(rsspecan_GetAttributeViString (instrSession, "", attr,
		arraySize, configuration));

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Digital Baseband Input Trigger Source
/* Purpose:  This function selects the trigger source for the start of a
/*           sweep
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputTriggerSource
(ViSession instrSession, ViInt32 triggerSource)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_TRIGGER_SOURCE, triggerSource),
		2, "Trigger Source");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}

/*===========================================================================*/
/* Function: Configure Digital Baseband Input Trigger Parameters
/* Purpose:  This function sets the Digital Baseband Input trigger
/*           parameters. This is applicable when the trigger source is set to
/*           Baseband Power.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputTriggerParameters
(ViSession instrSession, ViReal64 triggerLevel, ViReal64 holdoff)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_TRIGGER_LEVEL, triggerLevel),
		2, "Trigger Level");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_TRIGGER_HOLDOFF, holdoff),
		3, "Holdoff");

Error:
	(void)RsCore_UnlockSession(instrSession);
	return error;
}
