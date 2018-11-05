/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  B10 - External Generator Control
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

/*===========================================================================*/
/* Function: Configure External Generator Power
/* Purpose:  This function configures the selected external generator.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorPower(ViSession     instrSession,
                                                           ViInt32       window,
                                                           ViInt32       generator,
                                                           ViReal64      power)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, generator, 1, 2),
    		3, "Generator");

    sprintf (buffer, "Win%ld,G%ld", window, generator);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_OUTPUT_POWER, power),
    		4, "Power");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure External Generator Source
/* Purpose:  This function configures the external generator source.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorSource(ViSession   instrSession,
                                                            ViInt32     window,
                                                            ViBoolean   state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_STATE, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure External Generator Frequency
/* Purpose:  This function configures the selected external generator.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorFrequency (ViSession instrSession,
                                                                ViInt32 window,
                                                                ViInt32 generator,
                                                                ViReal64 frequency,
                                                                ViBoolean coupling)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, generator, 1, 2),
    		3, "Generator");

    sprintf (buffer, "Win%ld,G%ld", window, generator);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_OUTPUT_FREQUENCY, frequency),
    		4, "Frequency");

	viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_OUTPUT_FREQUENCY_COUPLING, coupling),
			5, "Coupling");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Select External Generator
/* Purpose:  This function selects the external generator.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SelectExternalGenerator(ViSession    instrSession,
                                                    ViInt32     generator,
                                                    ViInt32     generatorAddress,
                                                    ViInt32     generatorLink,
                                                    ViString    generatorType)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, generator, 1, 2),
    		2, "Generator");

    sprintf (buffer, "G%ld", generator);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_IEEE_ADDR, generatorAddress),
    		3, "Generator Address");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_INTF_TYPE, generatorLink),
    		4, "Generator Link");

    viCheckParm(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_TYPE, generatorType),
    		5, "Generator Type");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure External Generator Frequency Sweep
/* Purpose:  This function is used for setting the generator multiplier and
/*           the offset used to derive the generator frequency from the
/*           analyzer frequency.
/*
/*           Notes:
/*
/*           (1) If no external generator is selected, if the IECBUS address
/*           is not correct or the generator is not ready for operation, an
/*           execution error will be generated.
/*
/*           (2) This function is only valid in combination with option Ext.
/*           Generator Control FSQ-B10.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorFrequencySweep(ViSession   instrSession,
                                                                    ViInt32     window,
                                                                    ViInt32     generator,
                                                                    ViBoolean   frequencySweep,
                                                                    ViReal64    frequencyOffset,
                                                                    ViInt32    denominator,
                                                                    ViInt32    numerator)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, generator, 1, 2),
    		3, "Generator");
    sprintf (buffer, "Win%ld,G%ld", window, generator);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_FREQ_SWEEP_STATE, frequencySweep),
    		4, "Frequency Sweep");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_FREQ_OFFSET, frequencyOffset),
    		5, "Frequency Offset");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_DENOMINATOR, denominator),
    		6, "Denominator");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_NUMERATOR, numerator),
    		7, "Numerator");


    checkErr(RsCore_LockSession(instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure External Generator Ref Oscillator
/* Purpose:  This function switches between external and internal reference
/*           oscillator for the frequency processing of external generator 1
/*           and 2.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorRefOscillator(ViSession    instrSession,
                                                                   ViInt32      window,
                                                                   ViInt32      reference)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_EXT_GEN_ROSC_SOURCE, reference),
    		3, "Reference");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure External Gain
/* Purpose:  This function makes an external gain known to the analyzer,
/*           which will take it into account during the display of measurement
/*           results.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureExternalGain (ViSession instrSession,
                                                  ViInt32 window,
                                                  ViReal64 gain)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_CORR_EGA_INP, gain),
    		3, "Gain");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Send Command To External Generator
/* Purpose:  This function sends a command or query string to the device using
/*           the GPIB interface FSP-B10. Usually an external generator is
/*           connected to this interface.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SendCommandToExternalGenerator (ViSession instrSession,
                                                           ViInt32 deviceAddress,
                                                           ViString commandString)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deviceAddress, 0, 30),
    		2, "Device Address");

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":SYST:COMM:GPIB:RDEV:COMM %ld,%s", deviceAddress, commandString);
    checkErr(RsCore_Write(instrSession, cmd));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: External Generator Normalization
/* Purpose:  This function configures the tracking generator normalization.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ExternalGeneratorNormalization(ViSession instrSession,
                                                            ViInt32 window,
                                                            ViBoolean   state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TGEN_NORMALIZATION_STATE, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: External Generator Calibration
/* Purpose:  This function activates the selected tracking generator
/*           calibration.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ExternalGeneratorCalibration(ViSession   instrSession,
                                                        ViInt32     window,
                                                        ViInt32     method,
                                                        ViUInt32    timeout)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViInt32     old_timeout = 0;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 4, "Timeout");

    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));

    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));

    sprintf (buffer, "Win%ld", window);
    switch(method){
        case RSSPECAN_VAL_TGEN_CAL_TRAN:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TGEN_METHOD, RSSPECAN_VAL_CORR_TRAN));
        break;
        case RSSPECAN_VAL_TGEN_CAL_REFLO:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TGEN_METHOD, RSSPECAN_VAL_CORR_REFL));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TGEN_ACQUIRE, RSSPECAN_VAL_CORR_ACQ_OPEN));
        break;
        case RSSPECAN_VAL_TGEN_CAL_REFLS:
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TGEN_METHOD, RSSPECAN_VAL_CORR_REFL));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TGEN_ACQUIRE, RSSPECAN_VAL_CORR_ACQ_THR));
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, method), 3, "Method");
    }

    checkErr(rsspecan_SetOPCTimeout (instrSession, old_timeout));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: External Generator Recall
/* Purpose:  This function restores the instrument setting that was valid
/*           for the measurement of the reference data, provided that the
/*           tracking generator is active.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ExternalGeneratorRecall(ViSession    instrSession,
                                                   ViInt32  window)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_TGEN_RECALL, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: External Generator Save As Transducer Factor
/* Purpose:  This function uses the normalized measurement data to
/*           generate a transducer factor with up to 625 points.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ExternalGeneratorSaveAsTrdFactor(ViSession   instrSession,
                                                            ViInt32     window,
                                                            ViString    name)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_TFAC_GEN_NAME, name));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure Tracking Generator
/* Purpose:  This function configures the tracking generator.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTrackingGenerator(ViSession instrSession,
                                                      ViInt32   window,
                                                      ViReal64  sourcePower,
                                                      ViReal64  powerOffset,
                                                      ViReal64  frequencyOffset)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TGEN_OUTPUT_LEVEL, sourcePower),
    		3, "Source Power");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TGEN_LEVEL_OFFSET, powerOffset),
    		4, "Power Offset");
    if (strstr (model, "FSQ") != NULL)
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TGEN_FREQUENCY_OFFSET, frequencyOffset),
        		5, "Frequency Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Set Tracking Generator State
/* Purpose:  This function sets the tracking generator state.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_SetTrackingGeneratorState(ViSession instrSession,
                                                      ViInt32   window,
                                                      ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TGEN_STATE, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure Tracking Generator External Modulation
/* Purpose:  This function switches on or off the external modulations of
/*           the tracking generator.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTrackingGeneratorExternalModulation(ViSession   instrSession,
                                                                    ViInt32         window,
                                                                    ViInt32         modulation,
                                                                    ViBoolean       modState,
                                                                    ViReal64        fmDeviation)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    switch(modulation){
        case RSSPECAN_VAL_TGEN_EXT_AM:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TGEN_AM_STATE, modState),
            		4, "State");
        break;
        case RSSPECAN_VAL_TGEN_EXT_FM:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TGEN_FM_STATE, modState),
            		4, "State");

            viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TGEN_FM_DEVIATION, fmDeviation),
            		5, "FM Deviation");
        break;
        case RSSPECAN_VAL_TGEN_EXT_IQ:
            viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_TGEN_DM_STATE, modState),
            		4, "State");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, modulation), 3, "Modulation");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure Tracking Generator Power Sweep
/* Purpose:  This function configures the power sweep of the tracking
/*           generator.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureTrackingGeneratorPowerSweep(ViSession   instrSession,
                                                                ViInt32     window,
                                                                ViInt32     powerSweep,
                                                                ViReal64    powerStart,
                                                                ViReal64    powerStop)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_TGEN_POW_MODE, powerSweep),
    		3, "Power Sweep");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TGEN_POW_START, powerStart),
    		4, "Power Sweep");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_TGEN_POW_STOP, powerStop),
    		5, "Power Sweep");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- User defined callbacks --------------------------------------------------*
 *****************************************************************************/
