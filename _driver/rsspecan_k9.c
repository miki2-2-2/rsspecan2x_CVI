
/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  K9 - Power Meter
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
/* Function: PWM Mode
/* Purpose:  This function switches measurements with a power sensor on or
/*           off.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_PWMMode(ViSession  instrSession,
                                         ViInt32    window,
                                         ViBoolean  state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];
    ViInt32 pmet;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_STATE, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function selects current power meter
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR sensor/Selects the sensor.

ViStatus _VI_FUNC rsspecan_PWMSelect (ViSession instrSession, ViInt32 sensor)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_PMET_SELECT, sensor),
    		2, "Sensor");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Measurement
/* Purpose:  This function configures the power meter measurement
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMMeasurement(ViSession   instrSession,
                                                    ViInt32     window,
                                                    ViInt32     measurementTime,
                                                    ViInt32     coupling,
                                                    ViReal64    frequency,
                                                    ViInt32     averageCount)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, measurementTime, RSSPECAN_VAL_PWMET_MEASTIME_NORM, RSSPECAN_VAL_PWMET_MEASTIME_MANUAL),
    		3, "Measurement Time");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);
    switch (measurementTime)
    {
        case RSSPECAN_VAL_PWMET_MEASTIME_MANUAL:
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_AVERAGE_AUTO, VI_TRUE));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PMET_AVERAGE_COUNT, averageCount));
        break;
        default:
            checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_AVERAGE_AUTO, VI_FALSE));
            checkErr(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PMET_MEAS_TIME, measurementTime));
    }
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PMET_COUPLING, coupling),
    		4, "Coupling");
    if (coupling == RSSPECAN_VAL_PMET_COUP_OFF)
    {
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PMET_FREQ, frequency),
        		5, "Frequency");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Relative
/* Purpose:  This function configures the relative measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMRelative(ViSession   instrSession,
                                                ViInt32     window,
                                                ViReal64    referenceValue,
                                                ViInt32     unit)
{
    ViStatus error = VI_SUCCESS;
    ViInt32  pmet = 1;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,PM%ld", window, pmet);

    checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_RELATIVE, VI_TRUE));

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PMET_REL, referenceValue),
    		3, "Reference Value");

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PMET_UNIT_REL, unit),
    		4, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Absolute
/* Purpose:  This function configures the relative measurement.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMAbsolute(ViSession   instrSession,
                                                ViInt32 window,
                                                ViInt32 unit)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,PM%ld", window, pmet);
    checkErr(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_RELATIVE, VI_FALSE));
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PMET_UNIT_ABS, unit),
    		3, "Unit");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*===========================================================================*/
/* Function: Configure PWM Reference Level Offset State
/* Purpose:  This function defines whether the reference level offset set for the analyzer
is taken into account for the measured power or not.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMReferenceLevelOffsetState (ViSession instrSession,
                                                                  ViInt32 window,
                                                                  ViBoolean state)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_REF_LEVEL_OFFSET_STATE, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM External Power Trigger
/* Purpose:  This function accepts the current result as a reference value
/*           for relative measurements.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMExternalPowerTrigger (ViSession instrSession,
                                                             ViInt32 window,
                                                             ViBoolean state,
                                                             ViReal64 level)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_STATE, state),
    		3, "State");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PWR_EXTERNAL_POWER_TRIGGER_LEVEL, level),
    		4, "Level");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function configures the external power trigger advanced settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init.
/// HIPAR window/This control is reserved for future use.
/// HIPAR hysteresis/Sets the value for the trigger hysteresis of an external power trigger
/// HIPAR dropoutTime/Sets the time the input signal must stay below the IF power
/// HIPAR dropoutTime/trigger level in order to allow a trigger to start the measurement.
/// HIPAR holdoffTime/Sets the holdoff value in s, which is the time which must pass
/// HIPAR holdoffTime/before triggering, in case another trigger event happens.
/// HIPAR slope/Defines whether the sweep starts after a positive or negative
/// HIPAR slope/edge of the trigger signal.
ViStatus _VI_FUNC rsspecan_ConfigurePWMExternalPowerTriggerAdvanced (ViSession instrSession,
                                                                     ViInt32 window,
                                                                     ViInt32 hysteresis,
                                                                     ViReal64 dropoutTime,
                                                                     ViReal64 holdoffTime,
                                                                     ViInt32 slope)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];
	ViInt32     pmet = 1;

    checkErr(RsCore_LockSession(instrSession));

	checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_HYSTERESIS, hysteresis),
    		3, "Hysteresis");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_DROPOUT_TIME, dropoutTime),
    		4, "Dropout Time");

	viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_HOLDOFF_TIME, holdoffTime),
			4, "Holdoff Time");

	viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_SLOPE, slope),
			3, "Slope");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Bar Graph View
/* Purpose:  This function accepts the current result as a reference value
/*           for relative measurements.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMBarGraphView (ViSession instrSession,
                                                     ViBoolean barGraphView)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win0,PM%ld", pmet);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_BARGRAPH_STATE, barGraphView),
    		2, "Bar Graph View");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function assign the connected power sensors to the configuration sets.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR autoAssignment/Defines whether the selected power sensor index is automatically assigned to a subsequently connected power sensor.
/// HIPAR placeholder/This control is reserved for future use.
/// HIPAR type/The power sensor type.
/// HIPAR intfc/This control is reserved for future use.
/// HIPAR serialNumber/The power sensor serial number.

ViStatus _VI_FUNC rsspecan_ConfigurePowerSensorAssignment (ViSession instrSession,
                                                           ViBoolean autoAssignment,
                                                           ViString placeholder,
                                                           ViString type,
                                                           ViString intfc,
                                                           ViString serialNumber)
{
    ViStatus    error = VI_SUCCESS;
    ViChar cmd[RS_MAX_MESSAGE_BUF_SIZE];
    ViInt32     pmet = 1;
    ViChar      repCap[RS_REPCAP_BUF_SIZE];
    intfc;
    placeholder;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));
    snprintf(repCap, RS_REPCAP_BUF_SIZE, "PM%ld", pmet);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_AUTOMATIC_ASSIGNMENT, autoAssignment),
    		2, "Auto Assignment");

    if (autoAssignment == VI_FALSE) // if not AutoAssign only
    {
        snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "SYST:COMM:RDEV:PMET%ld:DEF '','%s','USB','%s'", pmet, type, serialNumber);
        checkErr(RsCore_Write(instrSession, cmd));
    }

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  Queries the number of power sensors currently connected to the instrument.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize function to select the desired instrument driver session.
/// HIPAR powerSensorCount/Queries the number of power sensors currently connected to the instrument.

ViStatus _VI_FUNC rsspecan_QueryPowerSensorCount (ViSession instrSession,
                                                  ViInt32 *powerSensorCount)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_PMET_NUMBER_OF_SENSORS, powerSensorCount),
    		2, "Power Sensor Count");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Duty Cycle
/* Purpose:  This function configures the duty cycle correction.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMDutyCycle (ViSession instrSession,
                                                  ViBoolean dutyCycle,
                                                  ViReal64 dutyCycleValue)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "PM%ld", pmet);
    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, repCap, RSSPECAN_ATTR_PMET_DUTY_CYCLE_STATE, dutyCycle),
    		2, "Duty Cycle");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PMET_DUTY_CYCLE, dutyCycleValue),
    		3, "Duty Cycle Value");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Measure To Reference
/* Purpose:  This function accepts the current result as a reference value
/*           for relative measurements.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMMeasToRef(ViSession   instrSession,
                                        ViInt32 window)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "C%ld,PM%ld", window, pmet);
    checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_PMET_REL_AUTO, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure PWM Zero
/* Purpose:  This function zeroes the power sensor.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePWMZero(ViSession   instrSession)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "PM%ld", pmet);
    checkErr(rsspecan_SetAttributeViString(instrSession, repCap, RSSPECAN_ATTR_PMET_ZERO, ""));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure Power Sensor Reference Level Offset
/* Purpose:  This function sets the offset of the reference level for
/*           relative measurements.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePowerSensorReferenceLevelOffset (ViSession instrSession,
                                                                     ViReal64 offset)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "PM%ld", pmet);
    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PWM_RELATIVE_OFFSET, offset),
    		2, "Offset");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Read PWM Result
/* Purpose:  This function triggers a measurement with the power sensor and
/*           then outputs the result.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadPWMResult(ViSession  instrSession,
                                        ViInt32     window,
                                        ViUInt32    timeout,
                                        ViReal64*   result)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];
    ViInt32  old_timeout = -1;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 3, "Timeout");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);
    checkErr(rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr(rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PMET_READ, result),
    		3, "Result");

Error:
    if (old_timeout >= 0)
        rsspecan_SetOPCTimeout (instrSession, old_timeout);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Fetch PWM Result
/* Purpose:  This function outputs the result of the power sensor.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_FetchPWMResult(ViSession  instrSession,
                                        ViInt32     window,
                                        ViReal64*   result)
{
    ViInt32  pmet = 1;
    ViStatus error = VI_SUCCESS;
    ViChar   repCap[RS_REPCAP_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_PMET_SELECT, &pmet));

    snprintf(repCap, RS_REPCAP_BUF_SIZE, "Win%ld,PM%ld", window, pmet);
    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, repCap, RSSPECAN_ATTR_PMET_FETCH, result),
    		3, "Result");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- End Of RSSpecAn K9 - Power Meter ----------------------------------------*
 *****************************************************************************/
