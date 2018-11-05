/*****************************************************************************
 *- RSSpecAn K30 - Noise Figure and Gain Measurement ------------------------*
 *****************************************************************************/

#include "rsspecan.h"
#include "rsspecan_utility.h"

static ViString measTypeArr[] = {"LOW","UPP", NULL};
static ViString fetchTypeArr[]  = {"FIG","TEMP","GAIN", NULL};
static ViString resultArr[]  = {"GAIN", "NOIS", "PCOL", "PHOT", "TEMP", "YFAC", NULL};
static ViString markerResultArr[]  = {"NoiseGAIN", "NoiseNOIS", "NoisePCOL", "NoisePHOT", "NoiseTEMP", "NoiseYFAC", "NoiseUNC", NULL};

/*****************************************************************************
 * Function: Noise Mode
 * Purpose:  This function selects the Noise Figure and Gain Measurement mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_NoiseMode(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Frequency Settings
 * Purpose:  This function configures the Noise Frequency Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseFrequencySettings(
    ViSession   instrSession,
    ViReal64    startFrequency,
    ViReal64    stopFrequency,
    ViReal64    stepFrequency,
    ViInt32     mode,
    ViReal64    fixedLO,
    ViReal64    fixedIF,
    ViReal64    imageRejection,
    ViReal64    fixedFrequency
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViReal64Range(instrSession, startFrequency, 0.0, 8.0e+9), 2, "Start Frequency");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, stopFrequency, 0.0, 8.0e+9), 3, "Stop Frequency");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, stepFrequency, 1.0, 8.0e+9), 4, "Step Frequency");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, mode, RSSPECAN_VAL_NOISE_DUT_MODE_DIRECT, RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_LO_ABS),
    		5, "Measurement Mode");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, fixedLO, 0.0, 999.99e+9), 6, "Fixed LO");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, fixedIF, 0.0, 8.0e+9), 7, "Fixed IF");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, imageRejection, 0.0, 999.99e+9), 8, "Image Rejection");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, fixedFrequency, 0.0, 8.0e+9), 9, "Fixed Frequency");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_START, startFrequency),
    		2, "Start Frequency");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_STOP, stopFrequency),
    		3, "Stop Frequency");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_FREQ_STEP, stepFrequency),
    		4, "Step Frequency");

	if ((strstr (model, "FSL") != NULL) || rsspecan_IsFSV (instrSession))
    {
        if ((mode == RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_IF_PLUS) || (mode == RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_IF_ABS))
        {
            checkErr(RS_ERROR_INSTRUMENT_MODEL);
        }
    }
    switch (mode)
    {
        case RSSPECAN_VAL_NOISE_DUT_MODE_DIRECT:
            {
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_MODE_DUT, RSSPECAN_VAL_NOISE_CONF_DUT_AMPLIFIER));
            }
            break;
        case RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_IF_PLUS:
            {
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC, RSSPECAN_VAL_NOISE_CONF_SYST_LOSC_VARIABLE));
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_MODE_DUT, RSSPECAN_VAL_NOISE_CONF_DUT_DOWNCONV));

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_INT_FREQ, fixedIF),
                		7, "Fixed IF");

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_EREJ, imageRejection),
                		8, "Image Rejection");
            }
            break;
        case RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_IF_ABS:
            {
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC, RSSPECAN_VAL_NOISE_CONF_SYST_LOSC_VARIABLE));
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_MODE_DUT, RSSPECAN_VAL_NOISE_CONF_DUT_UPCONV));

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_INT_FREQ, fixedIF),
                		7, "Fixed IF");

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_EREJ, imageRejection),
                		8, "Image Rejection");
            }
            break;
        case RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_LO_PLUS:
            {
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC, RSSPECAN_VAL_NOISE_CONF_SYST_LOSC_FIXED));
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_MODE_DUT, RSSPECAN_VAL_NOISE_CONF_DUT_UPCONV));

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC_FREQ, fixedLO),
                		6, "Fixed LO");

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_EREJ, imageRejection),
                		8, "Image Rejection");
            }
            break;
        case RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_LO_ABS:
            {
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC, RSSPECAN_VAL_NOISE_CONF_SYST_LOSC_FIXED));
                checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_MODE_DUT, RSSPECAN_VAL_NOISE_CONF_DUT_DOWNCONV));

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC_FREQ, fixedLO),
                		6, "Fixed LO");

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_EREJ, imageRejection),
                		8, "Image Rejection");
            }
            break;
        default:viCheckErrElab(RS_ERROR_INVALID_VALUE, "The value of Mode is invalid.");
            break;
    }

	if (strstr (model, "FSW") == NULL) // not for FSW
	{
	    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_FREQ_FIXED, fixedFrequency),
	    		9, "Fixed Frequency");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Frequency Table
 * Purpose:  This function configures the Noise Frequency Table.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseFrequencyTable(
    ViSession   instrSession,
    ViInt32     numberOfValues,
    ViReal64    receiverFrequencyValues[],
    ViReal64    localOscilatorFrequencyVal[],
    ViReal64    intermediateFrequencyValues[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     i = 0;
    ViChar      buffer[4*RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *p2buffer = NULL;
    ViUInt32    retCnt    = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, numberOfValues, 1, 100),
    		2, "Number Of Values");

    p2buffer = buffer;

    p2buffer += sprintf (p2buffer, "SENS:FREQ:LIST:DATA ");

    if (numberOfValues > 0)
    {
        for (i = 0; i < numberOfValues; i++)
        {
            p2buffer += sprintf (p2buffer, "%.12f,",receiverFrequencyValues[i]);
            p2buffer += sprintf (p2buffer, "%.12f,",localOscilatorFrequencyVal[i]);
            p2buffer += sprintf (p2buffer, "%.12f,",intermediateFrequencyValues[i]);
        }

        *p2buffer = '\0';
        *--p2buffer = '\n';

        checkErr(viWrite (instrSession, (ViBuf)buffer, (ViUInt32) (p2buffer - buffer), &retCnt));
    }

    //free(p2buffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Measurement Mode
 * Purpose:  This function configures the Noise Measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseMeasurementMode(
    ViSession   instrSession,
    ViInt32     measurementMode
)
{
    ViStatus    error   = VI_SUCCESS;
    ViAttr      attr[] = {RSSPECAN_ATTR_NOISE_CONF_CORR,
                          RSSPECAN_ATTR_NOISE_CONF_LIST_CONT,
                          RSSPECAN_ATTR_NOISE_CONF_LIST_SING,
                          RSSPECAN_ATTR_NOISE_CONF_SING,
                          RSSPECAN_ATTR_NOISE_CONF_FREQ_CONT,
                          RSSPECAN_ATTR_NOISE_CONF_FREQ_SING};

    checkErr(RsCore_LockSession(instrSession));

	if (RsCore_IsInstrumentModel(instrSession, "FSW"))
	{
		viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementMode, RSSPECAN_VAL_NOISE_SECOND_STAGE_CORR_MEAS, RSSPECAN_VAL_NOISE_SINGLE_FREQ_MEAS),
				2, "Measurement Mode");
	}
	else
	{
	    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementMode, RSSPECAN_VAL_NOISE_SECOND_STAGE_CORR_MEAS, RSSPECAN_VAL_NOISE_SINGLE_FREQ_SINGLE_MEAS),
	    		2, "Measurement Mode");
	}

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", attr[measurementMode], NULL),
    		2, "Measurement Type");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Calibration
 * Purpose:  This function configures the Noise Calibration.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseCalibration(
    ViSession   instrSession,
    ViBoolean   _2ndStageCorrection
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_STAT, _2ndStageCorrection),
    		2, "2nd Stage Correction");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Analyzer Settings
 * Purpose:  This function configures the Noise Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseAnalyzerSettings(
    ViSession   instrSession,
    ViReal64    resolutionBandwidth,
    ViReal64    sweepTime,
    ViReal64    settlingTime,
    ViInt32     average,
    ViReal64    RFAttenuation,
    ViReal64    range,
    ViBoolean   automaticRefLevel,
    ViReal64    refLevel,
    ViBoolean   preselector,
    ViBoolean   preamplifier
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViReal64Range(instrSession, resolutionBandwidth, 10.0, 50.0e+6), 2, "Resolution Bandwidth");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, sweepTime, 1.0e-6, 16.0e+3), 3, "SweepTime");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, settlingTime, 0.0, 20.0), 4, "Settling Time");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, average, 1, 32767),
    		5, "Average");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, RFAttenuation, 0.0, 75.0), 6, "RF Attenuation");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, range, 10.0, 999.99), 7, "Range");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, refLevel, -130, 5), 9, "Ref Level");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_RESOLUTION_BANDWIDTH, resolutionBandwidth),
    		2, "Resolution Bandwidth");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_SWEEP_TIME, sweepTime),
    		3, "Sweep Time");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_DUT_SETT_TIME, settlingTime),
    		4, "Settling Time");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Win1", RSSPECAN_ATTR_NUMBER_OF_SWEEPS, average),
    		5, "Average");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "TR1", RSSPECAN_ATTR_NOISE_DISP_TRAC_REF_LEV_STAT, automaticRefLevel),
    		8, "Automatic Ref Level");

    if (automaticRefLevel == VI_FALSE)
    {
        viCheckParm(rsspecan_SetAttributeViReal64 (instrSession, "Win1",
                                                    RSSPECAN_ATTR_REFERENCE_LEVEL,
                                                    refLevel), 9, "Ref Level")
    }
    else
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_DUT_GAIN, range),
        		7, "Range");
    }

    if (!RsCore_IsInstrumentModel(instrSession, "FMU"))
    {
    	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Win1", RSSPECAN_ATTR_ATTENUATION, RFAttenuation),
    			6, "RF Attenuation");
    }

    if (strstr (buffer, "B2,") != NULL)
    {
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_INP_PRES_STAT, preselector),
    		10, "Pre-selector");
    }

    if ((strstr (buffer, "B25") != NULL) ||
        (strstr (buffer, "B2,") != NULL) ||
        (strstr (buffer, "B22,") != NULL))
    {
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_INP_GAIN_STAT, preamplifier),
    		11, "Pre-amplifier");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Generator Settings
 * Purpose:  This function configures the Noise Generator Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGeneratorSettings(
    ViSession   instrSession,
    ViBoolean   automaticControl,
    ViReal64    level,
    ViString    generatorSelect,
    ViInt32     GPIBAddress,
    ViBoolean   initBeforeMeasurement
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, GPIBAddress, 0, 30),
    		2, "GPIB Address");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_GEN_CONT_STAT, automaticControl),
    		2, "Automatic Control");

    if (automaticControl == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_EXT_POW, level),
        		3, "Level");

        viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_GEN_TYPE, generatorSelect),
        		4, "Generator Select");

        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_GPIB_GEN_ADDR, GPIBAddress),
        		5, "GPIB Address");

        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_GEN_INIT_AUTO, initBeforeMeasurement),
        		6, "Init Before Measurement");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures whether a sequence of GPIB commands is
/// HIFN sent to an external signal generator at the end of each
/// HIFN measurement (after a single sweep or after a measurement is
/// HIFN aborted) to switch the generator's RF output off and end remote
/// HIFN control.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR automaticSwitchOff/If enabled, the generator's RF output is switched OFF after each
/// HIPAR automaticSwitchOff/measurement. The remote mode is also automatically ended after
/// HIPAR automaticSwitchOff/each measurement. Otherwise, RF output must be switched off and
/// HIPAR automaticSwitchOff/remote mode ended manually.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGeneratorAutoSwitchOff (ViSession instrSession,
                                                                 ViBoolean automaticSwitchOff)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_CONF_GEN_AUTO_SWITCH_OFF, automaticSwitchOff),
    		2, "Automatic Switch Off");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Configure Noise Generator Frequency
 * Purpose:  This function configures the Noise Generator Frequency.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGeneratorFrequency(
    ViSession   instrSession,
    ViReal64    offsetNr1,
    ViReal64    offsetNr2,
    ViInt32 numerator,
    ViInt32 denominator
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "OFF1", RSSPECAN_ATTR_NOISE_EXT_FREQ_OFFS, offsetNr1),
    		2, "Offset Nr 1");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "OFF2", RSSPECAN_ATTR_NOISE_EXT_FREQ_OFFS, offsetNr2),
    		3, "Offset Nr 2");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_EXT_FREQ_NUM, numerator),
    		4, "Numerator");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_EXT_FREQ_DEN, denominator),
    		5, "Denominator");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the Noise Generator Single Frequency settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init
/// HIPAR instrSession/or rsspecan_InitWithOptions function.
/// HIPAR singleFrequency/Defines the frequency for single frequency measurements.
/// HIPAR coupledToSweepList/Couples or decouples the frequency to the contents of the sweep list.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseSingleFrequency (ViSession instrSession,
                                                          ViReal64 singleFrequency,
                                                          ViBoolean coupledToSweepList)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_SINGLE_FREQUENCY, singleFrequency),
    		2, "Single Frequency");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_SINGLE_FREQUENCY_COUPLING, coupledToSweepList),
    		3, "Coupled To Sweep List");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise ENR Settings
 * Purpose:  This function configures the Noise ENR Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRSettings(
    ViSession   instrSession,
    ViInt32     selectionMode,
    ViReal64    ENRConstant,
    ViReal64    roomTemperature
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, selectionMode, RSSPECAN_VAL_NOISE_ERN_MODE_SEL_TABLE, RSSPECAN_VAL_NOISE_ERN_MODE_SEL_SPOT),
    		2, "Selection Mode");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, ENRConstant, -999.99, 999.99), 3, "ENR Constant");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, roomTemperature, 10.0, 1000.0), 4, "Room Temperature");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_MODE, selectionMode),
    		2, "Selection Mode");

    if( selectionMode == RSSPECAN_VAL_NOISE_ERN_MODE_SEL_SPOT)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_SPOT, ENRConstant),
        		3, "ENR Constant");
    }

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_TEMP, roomTemperature),
    		4, "Room Temperature");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise ENR Table
 * Purpose:  This function configures the Noise ENR Table.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRTable(
    ViSession   instrSession,
    ViInt32     numberOfValues,
    ViReal64    receiverFrequencyValues[],
    ViReal64    ENRValues[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     i = 0;
    ViChar      buffer[4*RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *p2buffer = NULL;
    ViUInt32    retCnt          = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, numberOfValues, 1, 100),
    		2, "Number Of Values");

    p2buffer = buffer;

    p2buffer += sprintf (p2buffer, "SENS:CORR:ENR:TABL:DATA ");

    if (numberOfValues > 0)
    {
        for (i = 0; i < numberOfValues; i++)
        {
            p2buffer += sprintf (p2buffer, "%.12f,",receiverFrequencyValues[i]);
            p2buffer += sprintf (p2buffer, "%.12f,",ENRValues[i]);
        }

        *p2buffer = '\0';
        *--p2buffer = '\n';

        checkErr(viWrite (instrSession, (ViBuf)buffer, (ViUInt32) (p2buffer - buffer), &retCnt));
    }

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function selects or deletes a ENR table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR operation/Selects the table operation.
/// HIPAR tableName/String containing the name of the table.
ViStatus _VI_FUNC rsspecan_NoiseENRTableOperations (ViSession instrSession,
                                                 ViInt32 operation,
                                                 ViString tableName)
{
	ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch(operation){
        case RSSPECAN_VAL_NOISE_TABLE_SELECT:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_TABLE_SELECT, tableName),
            		3, "Table Name");
        break;
        case RSSPECAN_VAL_NOISE_TABLE_DELETE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_TABLE_DELETE, tableName),
            		3, "Table Name");
        break;
		default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, operation), 2, "Operation");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the common ENR mode on/off. When it is
/// HIFN on the ENR characteristics defined for the measurement stage are
/// HIFN also used for the calibration stage.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR commonMode/Select the ERN common mode.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRCommonMode (ViSession instrSession,
                                                        ViBoolean commonMode)
{
	ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_COMMON_MODE, commonMode),
    		2, "Common Mode");

Error:
	return error;
}

/// HIFN This function configures the Noise ENR calibration Settings
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR selectionMode/Specify whether a single ENR value applies for all measured
/// HIPAR selectionMode/frequencies or whether an ENR list table is to be used (ENR
/// HIPAR selectionMode/values specified at specific input frequencies).
/// HIPAR ENRConstant/Specifies the ENR value applicable to the Noise source for all
/// HIPAR ENRConstant/input frequencies. This value provides a simple mechanism to
/// HIPAR ENRConstant/enter an ENR value that is applicable throughout a range of
/// HIPAR ENRConstant/measurement frequencies.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRCalibrationSettings (ViSession instrSession,
                                                                 ViInt32 selectionMode,
                                                                 ViReal64 ENRConstant)
{
	ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_CALIBRATION_MODE, selectionMode),
    		2, "Selection Mode");

    if( selectionMode == RSSPECAN_VAL_NOISE_ERN_MODE_SEL_SPOT)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_CALIBRATION_SPOT, ENRConstant),
        		3, "ENR Constant");
    }

Error:
	return error;
}

/// HIFN This function selects a ENR calibration table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR tableName/String containing the name of the table.
ViStatus _VI_FUNC rsspecan_SelectNoiseENRCalibrationTable (ViSession instrSession,
                                                           ViString tableName)
{
	ViStatus	error = VI_SUCCESS;

	checkErr(RsCore_LockSession(instrSession));

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_ENR_CALIBRATION_TABLE_SELECT, tableName),
    		3, "Table Name");

Error:
	return error;
}

/*****************************************************************************
 * Function: Configure Noise Loss Input Settings
 * Purpose:  This function configures the Noise Loss Input Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossInputSettings(
    ViSession   instrSession,
    ViInt32     selectionMode,
    ViReal64    lossInputConstant
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, selectionMode, RSSPECAN_VAL_NOISE_LOSS_MODE_TABLE, RSSPECAN_VAL_NOISE_LOSS_MODE_SPOT),
    		2, "Selection Mode");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, lossInputConstant, -999.99, 999.99), 3, "Loss Input Constant");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Inp", RSSPECAN_ATTR_NOISE_CORR_LOSS_MODE, selectionMode),
    		2, "Selection Mode");

    if( selectionMode == RSSPECAN_VAL_NOISE_LOSS_MODE_SPOT)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Inp", RSSPECAN_ATTR_NOISE_CORR_LOSS_SPOT, lossInputConstant),
        		3, "Loss Input Constant");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Loss Input Table
 * Purpose:  This function configures the Noise Loss Input Table.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossInputTable(
    ViSession   instrSession,
    ViInt32     numberOfValues,
    ViReal64    receiverFrequencyValues[],
    ViReal64    lossInputValues[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     i = 0;
    ViChar      buffer[4*RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *p2buffer = NULL;
    ViUInt32    retCnt          = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, numberOfValues, 1, 100),
    		2, "Number Of Values");

    p2buffer = buffer;

    p2buffer += sprintf (p2buffer, "SENS:CORR:LOSS:INP:TABL ");

    if (numberOfValues > 0)
    {
        for (i = 0; i < numberOfValues; i++)
        {
            p2buffer += sprintf (p2buffer, "%.12f,",receiverFrequencyValues[i]);
            p2buffer += sprintf (p2buffer, "%.12f,",lossInputValues[i]);
        }

        *p2buffer = '\0';
        *--p2buffer = '\n';

        checkErr(viWrite (instrSession, (ViBuf)buffer, (ViUInt32) (p2buffer - buffer), &retCnt));
    }

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function selects or deletes a loss input table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function. The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR operation/Selects the table operation.
/// HIPAR tableName/String containing the name of the table.
ViStatus _VI_FUNC rsspecan_NoiseLossInputTableOperations (ViSession instrSession,
                                                 ViInt32 operation,
                                                 ViString tableName)
{
	ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch(operation){
        case RSSPECAN_VAL_NOISE_TABLE_SELECT:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_LOSS_INPUT_TABLE_SELECT, tableName),
            		3, "Table Name");
        break;
        case RSSPECAN_VAL_NOISE_TABLE_DELETE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_LOSS_INPUT_TABLE_DELETE, tableName),
            		3, "Table Name");
        break;
		default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, operation), 2, "Operation");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Loss Output Settings
 * Purpose:  This function configures the Noise Loss Output Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossOutputSettings(
    ViSession   instrSession,
    ViInt32     selectionMode,
    ViReal64    lossOutputConstant
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, selectionMode, RSSPECAN_VAL_NOISE_LOSS_MODE_TABLE, RSSPECAN_VAL_NOISE_LOSS_MODE_SPOT),
    		2, "Selection Mode");
    viCheckParm(RsCore_InvalidViReal64Range(instrSession, lossOutputConstant, -999.99, 999.99), 3, "Loss Output Constant");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "Out", RSSPECAN_ATTR_NOISE_CORR_LOSS_MODE, selectionMode),
    		2, "Selection Mode");

    if( selectionMode == RSSPECAN_VAL_NOISE_LOSS_MODE_SPOT)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "Out", RSSPECAN_ATTR_NOISE_CORR_LOSS_SPOT, lossOutputConstant),
        		3, "Loss Output Constant");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Loss Output Table
 * Purpose:  This function configures the Noise Loss Output Table.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossOutputTable(
    ViSession   instrSession,
    ViInt32     numberOfValues,
    ViReal64    receiverFrequencyValues[],
    ViReal64    lossOutputValues[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     i = 0;
    ViChar      buffer[4*RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *p2buffer = NULL;
    ViUInt32    retCnt          = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, numberOfValues, 1, 100),
    		2, "Number Of Values");

    p2buffer = buffer;

    p2buffer += sprintf (p2buffer, "SENS:CORR:LOSS:OUTP:TABL ");

    if (numberOfValues > 0)
    {
        for (i = 0; i < numberOfValues; i++)
        {
            p2buffer += sprintf (p2buffer, "%le,%le,",receiverFrequencyValues[i], lossOutputValues[i]);
        }

        *p2buffer = '\0';
        *--p2buffer = '\n';

        checkErr(viWrite (instrSession, (ViBuf)buffer, (ViUInt32) (p2buffer - buffer), &retCnt));
    }

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function selects or deletes a loss output table.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR operation/Selects the table operation.
/// HIPAR tableName/String containing the name of the table.
ViStatus _VI_FUNC rsspecan_NoiseLossOutputTableOperations (ViSession instrSession,
                                                  ViInt32 operation,
                                                  ViString tableName)
{
	ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch(operation){
        case RSSPECAN_VAL_NOISE_TABLE_SELECT:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_LOSS_OUTPUT_TABLE_SELECT, tableName),
            		3, "Table Name");
        break;
        case RSSPECAN_VAL_NOISE_TABLE_DELETE:
            viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_CORR_LOSS_OUTPUT_TABLE_DELETE, tableName),
            		3, "Table Name");
        break;
		default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, operation), 2, "Operation");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Result Settings
 * Purpose:  This function configures the Noise Display Result Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseResultSettings(
    ViSession   instrSession,
    ViInt32     combinedTraceDisplay,
    ViBoolean   tableFormResults
)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, combinedTraceDisplay, RSSPECAN_VAL_DISP_FORMAT_SINGLE, RSSPECAN_VAL_DISP_FORMAT_SPLIT),
    		2, "Combined Trace Display");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_DISP_FORMAT, combinedTraceDisplay),
    		2, "Combined Trace Display");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_DISP_TABLE, tableFormResults),
    		3, "Table Form Results");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function selects a window in which the settings should be applied to.
/// HIFN
/// HIFN  Note(s):
/// HIFN
/// HIFN  (1) The functions that use this setting are:
/// HIFN
/// HIFN  rsspecan_ConfigureNoiseTraceSettings
/// HIFN  rsspecan_ConfigureNoiseGainTraceSettings
/// HIFN
/// HIFN  (2) This functions is intended to be used only with R&S FSW
/// HIFN  instrument.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR window/Selects the window. The values higher than 1 are applicable for R&S FSW instrument only.
ViStatus _VI_FUNC rsspecan_SelectWindow (ViSession instrSession, ViInt32 window)
{
	ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_WINDOW_SELECT, window),
    		2, "Window");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Trace Settings
 * Purpose:  This function configures the Noise Trace Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseTraceSettings(
    ViSession   instrSession,
    ViInt32     YAxis,
    ViBoolean   symbols,
    ViBoolean   automaticScaling,
    ViReal64    minYAxis,
    ViReal64    maxYAxis
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      repCap[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar      trace_no[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViInt32		window = 1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, YAxis, RSSPECAN_VAL_NOISE_DISP_DATA_OFF, RSSPECAN_VAL_NOISE_DISP_DATA_NFIGURE),
    		2, "Y-Axis");

	checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_WINDOW_SELECT, &window));

    sprintf(trace_no,"TR1");
	snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR1", window);

    switch (YAxis){
        case RSSPECAN_VAL_NOISE_DISP_DATA_OFF:
            viCheckParm(RsCore_InvalidViReal64Range(instrSession, maxYAxis, -75.0, 75.0), 6, "Max Y-Axis");
            viCheckParm(RsCore_InvalidViReal64Range(instrSession, minYAxis, -75.0, 75.0), 5, "Min Y-Axis");
            viCheckParm(rsspecan_SetAttributeViBoolean (instrSession, repCap,
                                                     RSSPECAN_ATTR_NOISE_DISP_TRAC_STAT,
                                                     VI_FALSE), 2, "Y-Axis")
        break;
        default:
            viCheckParm(RsCore_InvalidViReal64Range(instrSession, maxYAxis, -999990000.0, 999990000.0), 6, "Max Y-Axis");
            viCheckParm(RsCore_InvalidViReal64Range(instrSession, minYAxis, -999990000.0, 999990000.0), 5, "Min Y-Axis");
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_TRAC_STAT, VI_TRUE),
            		2, "Y-Axis");

            viCheckParm(rsspecan_SetAttributeViInt32(instrSession, trace_no, RSSPECAN_ATTR_NOISE_DISP_DATA_TRAC, YAxis),
            		2, "Y-Axis");

            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_SYMB, symbols),
            		3, "Symbols");

            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_AUTO_SCAL, automaticScaling),
            		4, "Automatic Scaling");

            if (automaticScaling == VI_FALSE)
            {
                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_TRAC_BOTTOM, minYAxis),
                		5, "Min Y-Axis");

                viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_TRAC_TOP, maxYAxis),
                		6, "Max Y-Axis");
            }
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Gain Trace Settings
 * Purpose:  This function configures the Gain Trace Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGainTraceSettings(
    ViSession   instrSession,
    ViBoolean   YAxis,
    ViBoolean   symbols,
    ViBoolean   automaticScaling,
    ViReal64    minYAxis,
    ViReal64    maxYAxis
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      repCap[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViInt32		window = 1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, maxYAxis, -75, 75), 6, "Max Y-Axis");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, minYAxis, -75, 75), 5, "Min Y-Axis");

    checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_WINDOW_SELECT, &window));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,TR2", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_TRAC_STAT, YAxis),
    		2, "Y-Axis");

    if (YAxis == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_SYMB, symbols),
        		3, "Symbols");

        viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_AUTO_SCAL, automaticScaling),
        		4, "Automatic Scaling");

        if (automaticScaling == VI_FALSE)
        {
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_TRAC_BOTTOM, minYAxis),
            		5, "Min Y-Axis");

            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_NOISE_DISP_TRAC_TOP, maxYAxis),
            		6, "Max Y-Axis");
        }
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Trace Memory Display
 * Purpose:  This function switches the display of trace memory <n> on or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseTraceMemoryDisplay (ViSession instrSession,
                                                             ViInt32 memory,
                                                             ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE]="";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, memory, 1, 3),
    		2, "Memory");

    sprintf (buffer, "MEM%ld", memory);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_NOISE_DISP_TRACE_MEM, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Noise Current Results Display
 * Purpose:  This function switches the display of the current measurement
 *           results on or off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureNoiseCurrentResultsDisplay (ViSession instrSession,
                                                                ViBoolean state)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_DISPL_CURR_RESULTS, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/*****************************************************************************
 * Function: Configure Noise X-Axis Frequency Display
 * Purpose:  This function selects the frequency to be displayed on the x-axis
 *           if the DUT is not an amplifier, i.e. in a frequency-converting
 *           measurement mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureXAxisFrequencyDisplay (ViSession instrSession,
                                                           ViInt32 measurementResults,
                                                           ViInt32 frequency)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE]="";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementResults, 1, 2),
    		2, "Measurement Results");

    sprintf (buffer, "TR%ld", measurementResults);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_NOISE_DISP_X_AXIS, frequency),
    		3, "Frequency");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Create Noise Limit Line
 * Purpose:  This function creates new limit or selects existing one and
 *           configures basic parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CreateNoiseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViString    name,
    ViString    comment
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");

    sprintf(lim_line_no,"L%d",limit);

    viCheckParm(rsspecan_SetAttributeViString(instrSession, lim_line_no, RSSPECAN_LIMIT_NAME, name),
    		3, "Limit Line Name");

    viCheckParm(rsspecan_SetAttributeViString(instrSession, lim_line_no, RSSPECAN_LIMIT_COMMENT, comment),
    		4, "Limit Line Comment");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Define Noise Limit Line
 * Purpose:  This function defines specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DefineNoiseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     resultType,
    ViInt32     type,
    ViInt32     count,
    ViReal64    xAxis[],
    ViReal64    amplitude[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViInt32     i = 0;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViChar      *pbuffer= NULL;
    ViChar      *p2buf;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, RSSPECAN_VAL_NOISE_CALC_LIM_LINE_NFIGURE, RSSPECAN_VAL_NOISE_CALC_LIM_LINE_GAIN),
    		3, "Result Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_LIMIT_LOWER, RSSPECAN_VAL_LIMIT_UPPER),
    		4, "Type");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, count, 1, INT_MAX),
    		5, "Count");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, xAxis), 6, "X Axis");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, amplitude), 7, "Amplitude");

    sprintf(lim_line_no,"L%d",limit);

    if (RsCore_IsInstrumentModel(instrSession, "FSW"))
    {
    	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, lim_line_no, RSSPECAN_ATTR_NOISE_CALC_LIM_TRAC, resultType),
    			3, "Result Type");
    }

    viCheckAlloc (pbuffer = (ViChar*) malloc (count * 20 + 200));

    p2buf = pbuffer + sprintf (pbuffer, "CALC:LIM%ld:CONT ", limit);

        for (i = 0; i < count; i++)
        p2buf += sprintf (p2buf, "%le,", xAxis[i]);

    *p2buf = '\0';
    *--p2buf = '\n';

    checkErr(viWrite (instrSession, (ViBuf) pbuffer, (ViUInt32) strlen (pbuffer), NULL));

    p2buf = pbuffer + sprintf (pbuffer, "CALC:LIM%ld:%s ", limit, measTypeArr [type]);

    for (i = 0; i < count; i++)
        p2buf += sprintf (p2buf, "%le,", amplitude[i]);

    *p2buf = '\0';
    *--p2buf = '\n';

    checkErr(viWrite (instrSession, (ViBuf) pbuffer, (ViUInt32) strlen (pbuffer), NULL));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer) free (pbuffer);

    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures a limit line for a particular result type.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR limit/This control selects limit.
/// HIPAR resultType/Select the result type.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLimitLineType (ViSession instrSession,
                                               ViInt32 limit, ViInt32 resultType)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      repCap [RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "L%ld",limit);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_NOISE_CALC_LIMIT_RESULT_TYPE, resultType),
    		4, "Result Type");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function assigns a limit line to a trace in the indicated measurement window.
/// HIFN
/// HIFN Note(s):
/// HIFN
/// HIFN (1) This function is available for R&S FSW instrument only.
/// HIFN
/// HIFN (2) Limit line is selected by its name. Use attribute
/// HIFN RSSPECAN_LIMIT_NAME or function rsspecan_CreateNoiseLimitLine.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR assignToTrace/Assigns a limit line to a trace in the indicated measurement window.
ViStatus _VI_FUNC rsspecan_AssignNoiseLimitLineToTrace (ViSession instrSession,
                                                        ViInt32 assignToTrace)
{
	ViStatus	error = VI_SUCCESS;
	ViChar      repCap [RS_MAX_MESSAGE_BUF_SIZE] = "";

	checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,L%ld", 1, 1);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_LIMIT_ASSIGN_TRACE, assignToTrace),
    		2, "Assign to Trace");

Error:
	(void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Enable Noise Limit Check
 * Purpose:  This function switches on or off the limit check and/or limit
 *           line for the selected limit line in the selected measurement
 *           window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_EnableNoiseLimitCheck(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     type,
    ViBoolean   limitEnabled,
    ViBoolean   checkEnabled
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViAttr      attr[] = {RSSPECAN_LIMIT_LOWER_STATE,
                          RSSPECAN_LIMIT_UPPER_STATE};
	ViInt32		window = 1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_LIMIT_LOWER, RSSPECAN_VAL_LIMIT_UPPER),
    		3, "Type");

    checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_WINDOW_SELECT, &window));

    sprintf(lim_line_no,"Win%ld,L%d", window, limit);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, lim_line_no, RSSPECAN_LIMIT_STATE, limitEnabled),
    		4, "Limit Enabled");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, lim_line_no, attr[type], checkEnabled),
    		5, "Check Enabled");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Move Noise Limit Line
 * Purpose:  This function moves specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MoveNoiseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     type,
    ViReal64    value
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViAttr      attr[] = {RSSPECAN_LIMIT_LOWER_SHIFT,
                          RSSPECAN_LIMIT_UPPER_SHIFT,
                          RSSPECAN_LIMIT_CONTROL_SHIFT};

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_LIMIT_LOWER, RSSPECAN_VAL_LIMIT_CONTROL),
    		3, "Type");

    if (type == RSSPECAN_VAL_LIMIT_CONTROL)
    {
        viCheckParm(RsCore_InvalidViReal64Range(instrSession, value, -300.0e9, 300.0e9), 4, "Value");
    }
    else
    {
        viCheckParm(RsCore_InvalidViReal64Range(instrSession, value, -200.0, 200.0), 4, "Value");
    }

    sprintf(lim_line_no,"L%d",limit);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, lim_line_no, attr[type], value),
    		4, "Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Copy Noise Limit Line
 * Purpose:  This function copies specified limit line into another one.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CopyNoiseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     copyTo
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");

    sprintf(lim_line_no,"L%d",limit);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, lim_line_no, RSSPECAN_LIMIT_COPY, copyTo),
    		3, "Copy To");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Clear Noise Limit Line Results
 * Purpose:  This function deletes the result of the current limit check.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ClearNoiseLimitLineResults(
    ViSession   instrSession,
    ViInt32     limit
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViInt32		window = 1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");

    checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_WINDOW_SELECT, &window));

    sprintf(lim_line_no,"Win%ld,L%d", window, limit);

    checkErr(rsspecan_SetAttributeViString(instrSession, lim_line_no, RSSPECAN_LIMIT_CLEAR, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Delete Noise Limit Line
 * Purpose:  This function removes specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeleteNoiseLimitLine(
    ViSession   instrSession,
    ViInt32     limit
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");

    sprintf(lim_line_no,"L%d",limit);

    checkErr(rsspecan_SetAttributeViString(instrSession, lim_line_no, RSSPECAN_LIMIT_DELETE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Noise Results
 * Purpose:  This function queries the noise measurement results.
 *
 *           Notes:
 *
 *           (1) This functions is for R&S FSW only.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryNoiseResults (ViSession instrSession,
                                              ViInt32 window, ViInt32 trace,
                                              ViInt32 resultType,
                                              ViInt32 arrayLength,
                                              ViInt32 *actualPoints,
                                              ViReal64 results[])
{
	ViStatus	error = VI_SUCCESS;
	ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar*     pbuffer = NULL;
    ViChar*     pstring_value;
    ViInt32     cnt = 0;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(RsCore_CheckInstrumentModel(instrSession, "FSW"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, RSSPECAN_VAL_NOISE_RESULT_TYPE_GAIN, RSSPECAN_VAL_NOISE_RESULT_TYPE_YFAC),
    		4, "Result Type");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "TRAC%ld:DATA? TRACe%ld,%s", window, trace, resultArr[resultType]);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &pbuffer)); // TODO: Check the response processing

    pstring_value = strtok (pbuffer, ",");

    while (pstring_value && cnt<arrayLength)
	{
        results[cnt++] = atof (pstring_value);
		pstring_value = strtok (NULL, ",");
    }

	if (actualPoints != NULL)
    	*actualPoints = cnt;

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
	(void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Noise Limit Check Result
 * Purpose:  This function queries the result of the limit check of the limit
 *           line indicated in the selected measurement window. It should be
 *           noted that a complete sweep must have been performed for obtaining
 *           a valid result. A synchronization with  *OPC, *OPC? or *WAI should
 *           therefore be provided.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetNoiseLimitCheckResult(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32*    state
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      lim_line_no[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViInt32		window = 1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, limit, 1, 6),
    		2, "Limit Line Number");

    checkErr(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_NOISE_WINDOW_SELECT, &window));

    sprintf(lim_line_no,"Win%ld,L%d", window, limit);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, lim_line_no, RSSPECAN_LIMIT_CHECK_RESULT, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Save Noise Trace Data To Memory
 * Purpose:  This function saves the current trace results (noise figure, noise
 *           temperature and noise gain traces) to trace memory <n>.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SaveNoiseTraceDataToMemory (ViSession instrSession,
                                                       ViInt32 memory)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE]="";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, memory, 1, 3),
    		2, "Memory");

    sprintf (buffer, "MEM%ld", memory);
    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_NOISE_ARRAY_MEMORY, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/// HIFN This function assigns the selected marker to the indicated trace in the specified window.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/This control selects marker.
/// HIPAR trace/Select trace the marker is assigned to.
ViStatus _VI_FUNC rsspecan_AssignMarkerToTrace (ViSession instrSession,
                                                ViInt32 window, ViInt32 marker,
                                                ViInt32 trace)
{
	ViStatus	error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE]="";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
			3, "Marker");

	sprintf (buffer, "C%ld,M%ld", window, marker);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_NOISE_ASSIGN_MARKER_TO_TRACE, trace),
    		4, "Trace");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function queries the position of a marker on the y-axis.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR window/This control selects the measurement window.
/// HIPAR marker/This control selects marker.
/// HIPAR markerAmplitude/Select the result type.
/// HIPAR resultType/Returns the amplitude value of the selected marker.
ViStatus _VI_FUNC rsspecan_QueryNoiseMarkerAmplitude (ViSession instrSession,
                                             ViInt32 window, ViInt32 marker,
											 ViInt32 resultType,
                                             ViReal64 *markerAmplitude)
{
	ViStatus	error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE]="";

	checkErr(RsCore_LockSession(instrSession));

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
			3, "Marker");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, resultType, RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_GAIN, RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_NUNC),
    		4, "Result Type");

	sprintf (buffer, "Win%ld,M%ld,%s", window, marker, markerResultArr[resultType]);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_NOISE_MARKER_AMPLITUDE, markerAmplitude),
    		4, "markerAmplitude");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function turns matching of the noise source characteristics used
/// HIFN  during calibration and measurement on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR state/This control turns matching of the noise source characteristics used
/// HIPAR state/during calibration and measurement on and off.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyCommonSource(
	ViSession	instrSession,
	ViBoolean	state
)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_COMMON_SOURCE, state),
    		2, "state");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function defines the output match of the noise source. Define the
/// HIFN  output match either as the VSWR or as the return loss (RL).
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR type/This control selects the type of output match.
/// HIPAR sourceOutput/This control defines the VSWR or return loss at the noise source
/// HIPAR sourceOutput/output.
/// HIPAR ernUncertainty/This control defines the uncertainty of the excess noise ratio of the
/// HIPAR ernUncertainty/noise source you are using.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintySourceCharacteristics(
	ViSession	instrSession,
	ViInt32	type,
	ViReal64	sourceOutput,
	ViReal64	ernUncertainty
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (type)
	{
        case RSSPECAN_VAL_NOISE_VSWR:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_SOURCE_VSWR, sourceOutput),
            		3, "Source Output");
        break;
        case RSSPECAN_VAL_NOISE_RETURN_LOSS:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_SOURCE_RETURN_LOSS, sourceOutput),
            		3, "Source Output");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 2, "Type");
    }

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_ERN, ernUncertainty),
			4, "ERN Uncertainty");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN  This function defines the output match of the noise source
/// HIFN  calibration. Define the output match either as the VSWR or as the
/// HIFN  return loss (RL).
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_SOURCE_VSWR
/// HIFN     RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_SOURCE_RETURN_LOSS
/// HIFN     RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_ERN
/// HIFN
/// HIFN     Remote-control command(s):
/// HIFN     CALCulate:UNCertainty:MATCh:SOURce:CALibration[:VSWR]
/// HIFN     CALCulate:UNCertainty:MATCh:SOURce:CALibration:RL
/// HIFN     CALCulate:UNCertainty:ENR:CALibration:UNCertainty
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR type/This control selects the type of output match.
/// HIPAR sourceOutput/This control defines the VSWR or return loss at the calibration noise
/// HIPAR sourceOutput/source output.
/// HIPAR ernUncertainty/This control defines the uncertainty of the excess noise ratio of the
/// HIPAR ernUncertainty/calibration noise source you are using.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintySourceCalibrationCharacteristics(
	ViSession	instrSession,
	ViInt32	type,
	ViReal64	sourceOutput,
	ViReal64	ernUncertainty
)
{
	ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    switch (type)
	{
        case RSSPECAN_VAL_NOISE_VSWR:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_SOURCE_VSWR, sourceOutput),
            		3, "Source Output");
        break;
        case RSSPECAN_VAL_NOISE_RETURN_LOSS:
            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_SOURCE_RETURN_LOSS, sourceOutput),
            		3, "Source Output");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 2, "Type");
    }

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_ERN, ernUncertainty),
			4, "ERN Uncertainty");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function defines the match at the DUT input and output. You can
/// HIFN  define the match either as the VSWR or as the return loss (RL). If you
/// HIFN  define the VSWR or the return loss, the application automatically
/// HIFN  calculates the other.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR direction/This control selects the match direction.
/// HIPAR type/This control selects the type of match.
/// HIPAR dutUncertainty/This control defines the match at the DUT input and output. You can
/// HIPAR dutUncertainty/define the match either as the VSWR or as the return loss (RL). If you
/// HIPAR dutUncertainty/define the VSWR or the return loss, the application automatically
/// HIPAR dutUncertainty/calculates the other.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyDUTCharacteristics(
	ViSession	instrSession,
	ViInt32	direction,
	ViInt32	type,
	ViReal64	dutUncertainty
)
{
    ViStatus	error = VI_SUCCESS;
    ViAttr	attr[2][2] = {{RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_INPUT_VSWR, RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_INPUT_RETURN_LOSS},
						  {RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_OUTPUT_VSWR, RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_OUTPUT_RETURN_LOSS}};

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, direction, RSSPECAN_VAL_NOISE_INPUT, RSSPECAN_VAL_NOISE_OUTPUT),
    		2, "Direction");
	viCheckParm(RsCore_InvalidViInt32Range(instrSession, type, RSSPECAN_VAL_NOISE_VSWR, RSSPECAN_VAL_NOISE_RETURN_LOSS),
			3, "Type");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", attr[direction][type], dutUncertainty),
    		4, "DUT Uncertainty");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function turns automatic determination of the DUT characteristics
/// HIFN  used for the uncertainty calculation on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR _auto/This control turns automatic determination of the DUT characteristics
/// HIPAR _auto/for the calculation of the uncertainty on and off.
/// HIPAR noiseFigure/This control defines the noise level of the DUT.
/// HIPAR gain/This control defines the gain of the DUT.
/// HIPAR frequency/This control defines the frequency for which the uncertainty should be
/// HIPAR frequency/calculated.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyMeasurementValues(
	ViSession	instrSession,
	ViBoolean	_auto,
	ViReal64	noiseFigure,
	ViReal64	gain,
	ViReal64	frequency
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_AUTO, _auto),
    		2, "Auto");
	if (_auto == VI_FALSE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_NOISE, noiseFigure),
				3, "Noise Figure");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_GAIN, gain),
				4, "Gain");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_FREQUENCY, frequency),
				5, "Frequency");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function configures the characteristics of the test setup when
/// HIFN  using an external preamplifier.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR preamplifierState/This control includes or excludes an external preamplifier from the
/// HIPAR preamplifierState/uncertainty calculation.
/// HIPAR noiseFigure/This control defines the noise level of an external preamplifier that
/// HIPAR noiseFigure/may be part of the test setup.
/// HIPAR gain/This control define the gain of an external preamplifier that may be
/// HIPAR gain/part of the test setup.
/// HIPAR type/This control selects the type of match.
/// HIPAR analyzerUncertainty/This control defines the VSWR or return loss at the input of the
/// HIPAR analyzerUncertainty/preamplifier.
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyAnalyzerCharacteristics(
	ViSession	instrSession,
	ViBoolean	preamplifierState,
	ViReal64	noiseFigure,
	ViReal64	gain,
	ViInt32	type,
	ViReal64	analyzerUncertainty
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_STATE, preamplifierState),
    		2, "Preamplifier State");
	if (preamplifierState == VI_TRUE)
	{
		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_NOISE, noiseFigure),
				3, "Noise Figure");

		viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_GAIN, gain),
				4, "Gain");

		switch (type)
		{
	        case RSSPECAN_VAL_NOISE_VSWR:
	            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_VSWR, analyzerUncertainty),
	            		6, "Analyzer Uncertainty");
	        break;
	        case RSSPECAN_VAL_NOISE_RETURN_LOSS:
	            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_RETURN_LOSS, analyzerUncertainty),
	            		6, "Analyzer Uncertainty");
	        break;
	        default:
	            viCheckParm(RsCore_InvalidViInt32Value(instrSession, type), 5, "Type");
	    }
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function queries the uncertainty of noise figure results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular
/// HIPAR instrSession/instrument session.
/// HIPAR uncertainty/This control queries the uncertainty of noise figure results.
ViStatus _VI_FUNC rsspecan_QueryNoiseUncertainty(
	ViSession	instrSession,
	ViReal64*	uncertainty
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_NOISE_UNCERTAINTY_RESULT, uncertainty),
    		2, "Uncertainty");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN The Uncertainty calculator takes several characteristics of the
/// HIFN spectrum analyser into account. Two of these characteristics are
/// HIFN the uncertainties on the spectrum analyser internal gain and
/// HIFN noise figure.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/he ViSession handle that you obtain from the rsspecan_init.
/// HIPAR spectrumAnalyzer/This control selects the spectrum analyzer uncertainty to be
/// HIPAR spectrumAnalyzer/queried.
/// HIPAR uncertainty/This control queries the uncertainty on the spectrum analyzer.
ViStatus _VI_FUNC rsspecan_QuerySpectrumAnalyzerUncertainty (ViSession instrSession,
                                                             ViInt32 spectrumAnalyzer,
                                                             ViReal64 *uncertainty)
{
	ViStatus	error = VI_SUCCESS;
	ViAttr		attr[] = {RSSPECAN_ATTR_SPEC_ANALYZER_UNCERTAINTY_GAIN,
						  RSSPECAN_ATTR_SPEC_ANALYZER_UNCERTAINTY_NOISE};

    checkErr(RsCore_LockSession(instrSession));

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

// TODO: ERROR!!! Missing Unlock

	viCheckParm(RsCore_InvalidViInt32Range(instrSession, spectrumAnalyzer, RSSPECAN_VAL_INT_GAIN, RSSPECAN_VAL_INT_NOISE),
			2, "Spectrum Analyzer");

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, "", attr[spectrumAnalyzer], uncertainty),
    		3, "Uncertainty");

Error:
	return error;
}

/// HIFN This function immediately initializes the external signal
/// HIFN generator. It also switches the external generator's RF output
/// HIFN on. If any of the external generator settings have changed,
/// HIFN initialization is required before a measurement using the
/// HIFN external generator control can be run.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/he ViSession handle that you obtain from the rsspecan_init.
ViStatus _VI_FUNC rsspecan_InitNoiseGenerator (ViSession instrSession)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_NOISE_INIT_GENERATOR, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Fetch Noise Array
 * Purpose:  This function returns the noise measurement results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchNoiseArray(
    ViSession   instrSession,
    ViInt32     modifier,
    ViInt32     arraySize,
    ViReal64    values[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViChar*     pbuffer = NULL;
    ViChar*     pstring_value;
    ViInt32     cnt     = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_NOISE_FETC_FIGURE, RSSPECAN_VAL_NOISE_FETC_GAIN),
    		2, "Modifier");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 1, 100),
    		3, "Array Size");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "FORM ASC;:FETC:ARR:NOIS:%s?", fetchTypeArr[modifier]);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &pbuffer)); // TODO: Check the response processing

    pstring_value = strtok(pbuffer, ",");
    while (pstring_value && cnt<arraySize)
    {
        values[cnt++] = atof (pstring_value);
        pstring_value = strtok(NULL, ",");
    }

    if (pbuffer) free(pbuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch Noise Scalar
 * Purpose:  This function returns the noise measurement results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchNoiseScalar(
    ViSession   instrSession,
    ViInt32     modifier,
    ViReal64*   value
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViChar*     pbuffer = NULL;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_NOISE_FETC_FIGURE, RSSPECAN_VAL_NOISE_FETC_GAIN),
    		2, "Modifier");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "FETC:SCAL:NOIS:%s?", fetchTypeArr[modifier]);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &pbuffer)); // TODO: Check the response processing

    sscanf(pbuffer,"%le",value);

    if (pbuffer) free(pbuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Fetch Noise Memory Array
 * Purpose:  This function returns the measurement results of the selected
 *           memory.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchNoiseMemoryArray (ViSession instrSession,
                                                  ViInt32 modifier,
                                                  ViInt32 memory,
                                                  ViInt32 arraySize,
                                                  ViReal64 measResult[])
{
    ViStatus    error   = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViChar*     pbuffer = NULL;
    ViChar*     pstring_value;
    ViInt32     cnt     = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, modifier, RSSPECAN_VAL_NOISE_FETC_FIGURE, RSSPECAN_VAL_NOISE_FETC_GAIN),
    		2, "Modifier");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, memory, 1, 3),
    		3, "Memory");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 1, 100),
    		4, "Array Size");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "FETC:ARR:MEM%ld:NOIS:%s?", memory, fetchTypeArr[modifier]);
    checkErr(RsCore_QueryViStringUnknownLength(instrSession, cmd, &pbuffer)); // TODO: Check the response processing

    pstring_value = strtok(pbuffer, ",");
    while (pstring_value && cnt<arraySize)
    {
        measResult[cnt++] = atof (pstring_value);
        pstring_value = strtok(NULL, ",");
    }

    if (pbuffer) free(pbuffer);

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- End Of RSSpecAn K30 - Noise Figure and Gain Measurement -----------------*
 *****************************************************************************/
