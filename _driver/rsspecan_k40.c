/*****************************************************************************
 *- RSSpecAn K40 - Phase Noise ----------------------------------------------*
 *****************************************************************************/

#include "rsspecan.h"
#include "rsspecan_utility.h"

//#define DATA_BUFFER_SIZE    126000L                      /* Data buffer size */

static ViString fetchPhaseArr[] = {"RFM","RPM","RMS", VI_NULL};
static ViString limTypeArr[] = {"LOW", "UPP", VI_NULL};

//void rsspecan_SWAP_32(unsigned char *data, long points);

/*****************************************************************************
 * Function: Phase Mode
 * Purpose:  This function selects the Phase Noise mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_PhaseMode(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr( rsspecan_SetAttributeViString (instrSession, "",
                                               RSSPECAN_ATTR_PHASE_MODE, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Signal Settings
 * Purpose:  This function configures the Phase Signal Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSignalSettings(
    ViSession   instrSession,
    ViReal64    frequency,
    ViReal64    level,
    ViBoolean   sweepForward,
    ViBoolean   verifyFrequencyLevel,
    ViReal64    frequencyTolerance,
    ViReal64    levelTolerance
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((frequency<0.0)||(frequency>7.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Frequency");
        
    if ((level<-100.0)||(level>30.0))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Level");
        
    if ( sweepForward == VI_TRUE)
    {
        if ((frequencyTolerance<1.0)||(frequencyTolerance>1.0e+6))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 6, "Frequency Tolerance");
    }
    else
    {
        if ((frequencyTolerance<1.0)||(frequencyTolerance>100.0))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 6, "Frequency Tolerance");
    }
    
    if ((levelTolerance<3.0)||(levelTolerance>20.0))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 7, "Level Tolerance");    

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_CENTER,
                                                frequency), 2, "Frequency"); 
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_CONF_POW_EXP_RF,
                                                level), 3, "Level");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_PHASE_SWE_FORW,
                                                 sweepForward), 4, "Sweep Forward");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_PHASE_FREQ_VER,
                                                 verifyFrequencyLevel), 5, "Verify Frequency Level");
    
    if(verifyFrequencyLevel == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                    RSSPECAN_ATTR_PHASE_FREQ_VER_TOL,
                                                    frequencyTolerance), 6, "Frequency Tolerance");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                    RSSPECAN_ATTR_PHASE_POW_RLEV_VER_TOL,
                                                    levelTolerance), 7, "Level Tolerance");
    }
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Configure Phase Frequency Tolerance
/* Purpose:  This function sets the absolute frequency tolerance. This 
/*           functions automatically sets the Verify Frequency to ON. This is 
/*           required for the Frequency Tolerance to be enabled and set.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseFrequencyTolerance(
	ViSession	instrSession,
	ViReal64	absoluteFrequencyTolerance
)
{
    ViStatus    error = VI_SUCCESS;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
	viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                     RSSPECAN_ATTR_PHASE_FREQ_VER,
                                                     VI_TRUE), 2, "");
    viCheckParm( rsspecan_SetAttributeViReal64 (instrSession, "", 
                                                RSSPECAN_ATTR_PHASE_FREQ_VER_TOL_ABS, absoluteFrequencyTolerance),
                                                2, "Absolute Frequency Tolerance");                                                 
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}
	
	
/*****************************************************************************
 * Function: Configure Phase AFC Settings
 * Purpose:  This function configures the Phase AFC Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseAFCSettings(
    ViSession   instrSession,
    ViBoolean   trackFrequency,
    ViBoolean   trackLevel
)
{
    ViStatus    error   = VI_SUCCESS;

    ViBoolean   state   = VI_FALSE;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    viCheckErr( rsspecan_GetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_PHASE_FREQ_VER,
                                                 &state));
                                                 
    if (state == VI_TRUE)                                                
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                     RSSPECAN_ATTR_PHASE_FREQ_TRACK,
                                                     trackFrequency), 2, "Track Frequency");

        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_PHASE_POW_TRACK,
                                                     trackLevel), 3, "Track Level");
    }
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN  This function turns the suppression of AM noise on and off.
/// HIFN     
/// HIFN     Note(s):
/// HIFN     
/// HIFN     (1) AM rejection is available for the I/Q sweep mode.
/// HIFN     
/// HIFN     (2) This function is available for R&S FSW only.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR amRejection/Turns the suppression of AM noise on and off.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseAMRejection(
	ViSession	instrSession,
	ViBoolean	amRejection
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                 RSSPECAN_ATTR_PHASE_AM_REJECTION,
                                                 amRejection), 2, "AM Rejection");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function defines the maximum tracking bandwidth (sample rate) for 
/// HIFN  all half decades.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR trackingBandwidth/Defines the maximum tracking bandwidth (sample rate) for all half 
/// HIPAR trackingBandwidth/decades.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseTrackingBW(
	ViSession	instrSession,
	ViReal64	trackingBandwidth
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_TRACKING_BANDWIDTH,
                                                trackingBandwidth), 2, "Tracking Bandwidth");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function turns the digital PLL on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR digitalPLL/Turns the digital PLL on and off.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDigitalPLL(
	ViSession	instrSession,
	ViBoolean	digitalPLL
)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_DIGITAL_PLL,
                                                digitalPLL), 2, "Digital PLL");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;   
}


/// HIFN  This function turns decimation of results on and off.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR decimation/Turns decimation of results on and off.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDecimation(
	ViSession	instrSession,
	ViBoolean	decimation
)
{
	ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_DECIMATION,
                                                decimation), 2, "Decimation"); 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error; 	    
}

/*****************************************************************************
 * Function: Configure Phase Display Settings
 * Purpose:  This function configures the Phase Display Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDisplaySettings(
    ViSession   instrSession,
    ViReal64    xAxisStart,
    ViReal64    xAxisStop,
    ViBoolean   autoscaleOnce,
    ViReal64    yAxisTop,
    ViReal64    yAxisRange
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((xAxisStart<1.0)||(xAxisStart>300.0e+6))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "X Axis Start");
        
    if ((xAxisStop<3.0)||(xAxisStop>1.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "X Axis Stop");
        
    if (xAxisStart>xAxisStop)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "X Axis Start>X Axis Stop");    
    
    if ((yAxisTop<-200.0)||(yAxisTop>0.0))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 5, "Y Axis Top");
        
    if ((yAxisRange<1.0)||(yAxisRange>200.0))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 6, "Y Axis Range");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_START,
                                                xAxisStart), 2, "X Axis Start");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_STOP,
                                                xAxisStop), 3, "X Axis Stop");
    
    if(autoscaleOnce == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViString (instrSession, "",
                                                     RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_AUTO_ONCE,
                                                     ""), 4, "Autoscale Once");
    }
    else
    {
    
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                     RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_AUTO,
                                                     autoscaleOnce), 4, "Autoscale Once");
        
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                    RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_RLEV,
                                                    yAxisTop), 5, "Y Axis Top");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                    RSSPECAN_ATTR_PHASE_DISP_TRACE_Y, 
                                                    yAxisRange), 6, "Y Axis Range");
    }
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the Phase Display Settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR yAxisScaling/Selects the way the application scales the horizontal axis.
/// HIPAR yAxisTop/Specifies the max Phase Noise level in y-axis for the trace results.
/// HIPAR yAxisRange/Specifies the distance from the top to the origin in y-axis.
/// HIPAR yAxisBottom/Defines the reference value or upper border of the diagram area.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDisplayYAxisSettings (ViSession instrSession,
                                                      ViInt32 yAxisScaling,
                                                      ViReal64 yAxisTop,
                                                      ViReal64 yAxisRange,
                                                      ViReal64 yAxisBottom)
{
	ViStatus    error   = VI_SUCCESS; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	switch (yAxisScaling)
    {
        case RSSPECAN_VAL_PHASE_Y_SCALE_OFF:
			viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_Y_AXIS_MANUAL_SCALING,
                                                yAxisScaling), 2, "Y Axis Scaling");
            break;
        case RSSPECAN_VAL_PHASE_Y_SCALE_TOP_BOTTOM:
			viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_Y_AXIS_MANUAL_SCALING,
                                                yAxisScaling), 2, "Y Axis Scaling");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_RLEV,
                                                yAxisTop), 3, "Y Axis Top");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_BOTTOM,
                                                yAxisBottom), 4, "Y Axis Bottom");
            break;
    	case RSSPECAN_VAL_PHASE_Y_SCALE_TOP_RANGE:
			viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_Y_AXIS_MANUAL_SCALING,
                                                yAxisScaling), 2, "Y Axis Scaling");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_RLEV,
                                                yAxisTop), 3, "Y Axis Top");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_DISP_TRACE_Y,
                                                yAxisRange), 4, "Y Axis Range");
            break;
        case RSSPECAN_VAL_PHASE_Y_SCALE_BOTTOM_RANGE:
			viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_Y_AXIS_MANUAL_SCALING,
                                                yAxisScaling), 2, "Y Axis Scaling");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_BOTTOM,
                                                yAxisBottom), 4, "Y Axis Bottom");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_DISP_TRACE_Y,
                                                yAxisRange), 4, "Y Axis Range");
            break;
        case RSSPECAN_VAL_PHASE_Y_SCALE_AUTO:
			viCheckErr (rsspecan_SetAttributeViInt32 (instrSession, "", 
					 							RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_AUTO,
                                                VI_TRUE));
			
            break;
		default:
			viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Y Axis Scaling");
			break;
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the phase display X axis settings.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR xAxisStart/Selects the way the application scales the horizontal axis.
/// HIPAR xAxisStop/Defines the start frequency of the horizontal axis.
/// HIPAR xAxisScope/Defines the stop frequency of the horizontal axis.
/// HIPAR halfDecade/Selects the half decade that is displayed.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDisplayXAxisSettings (ViSession instrSession,
                                                      ViInt32 xAxisScope,
													  ViReal64 xAxisStart,
                                                      ViReal64 xAxisStop,
                                                      ViReal64 halfDecade)
{
	ViStatus    error   = VI_SUCCESS; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_X_AXIS_SCOPE,
                                                xAxisScope), 2, "X Axis Scope");

	switch (xAxisScope)
    {
        case RSSPECAN_VAL_NOISE_X_SCOPE_MANUAL:
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_X_AXIS_START_FREQUENCY,
                                                xAxisStart), 3, "X Axis Start");
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_X_AXIS_STOP_FREQUENCY,
                                                xAxisStop), 4, "X Axis Stop");
            break;
        case RSSPECAN_VAL_NOISE_X_SCOPE_HDECADE:
			viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_X_AXIS_HALF_DECADE,
                                                halfDecade), 5, "Half Decade");
            break;
    }
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Trace Settings
 * Purpose:  This function configures the Phase Trace Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseTraceSettings(
    ViSession   instrSession,
    ViReal64    traceOffset,
    ViInt32     traceSmoothing,
    ViInt32     smoothingType
) 
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((traceOffset<-200.0)||(traceOffset>200.0e+6))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Trace Offset");
        
    if ((traceSmoothing<1)||(traceSmoothing>20))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Trace Smoothing");
        
    if ((smoothingType<RSSPECAN_VAL_PHASE_SMO_TYPE_LIN)||
        (smoothingType>RSSPECAN_VAL_PHASE_SMO_TYPE_MED))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Smoothing Type");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_RLEV_OFFS,
                                                traceOffset), 2, "Trace Offset");
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_PHASE_DISP_TRACE_SMO_APER,
                                               traceSmoothing), 3, "Trace Smoothing");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "",
                                               RSSPECAN_ATTR_PHASE_DISP_TRACE_SMO_TYPE,
                                               smoothingType), 4, "Smoothing Type");
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Residual Calculations
 * Purpose:  This function configures the Phase Residual Calculations.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseResidualCalculations(
    ViSession   instrSession,
    ViBoolean   useMeasurementSettings,
    ViReal64    evaluateFrom,
    ViReal64    evaluateTo
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((evaluateFrom<1.0e+3)||(evaluateFrom>1.0e+6))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Evaluate From");
        
    if ((evaluateTo<1.0e+3)||(evaluateTo>1.0e+6))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Evaluate To");
        
    if (evaluateFrom>evaluateTo)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Evaluate From>Evaluate To");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "C1", RSSPECAN_ATTR_PHASE_CALC_EVAL,
                                                 !useMeasurementSettings), 2, "Use Measurement Settings");
    
    if(useMeasurementSettings == VI_FALSE)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "C1",
                                                    RSSPECAN_ATTR_PHASE_CALC_EVAL_STAR,
                                                    evaluateFrom), 3, "Evaluate From");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "C1",
                                                    RSSPECAN_ATTR_PHASE_CALC_EVAL_STOP,
                                                    evaluateTo), 4, "Evaluate To");
    }
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the trace for a custom residual noise calculation range.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR userRange/Selects the user range.
/// HIPAR trace/Selects the trace for a custom residual noise calculation range.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseTraceCustomRange (ViSession instrSession,
												  ViInt32 window,
                                                  ViInt32 userRange,
                                                  ViInt32 trace)
{
	ViStatus    error   = VI_SUCCESS;
	ViChar      rep_cap [50] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (userRange, 1, 3) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "User Range");  
    }
	
	sprintf (rep_cap, "C%ld,U%ld", window, userRange);
        
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap, 
											   RSSPECAN_ATTR_PHASE_USER_TRACE,
                                               trace), 3, "trace");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the Phase Residual User Calculations.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR userRange/Selects the user range.
/// HIPAR evaluateFrom/This control defines the start point of a custom residual noise calculation range.
/// HIPAR evaluateTo/This control defines the end point of a custom residual noise calculation range.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseUserResidualCalculations (ViSession instrSession,
                                                          ViInt32 window,
														  ViInt32 userRange,
                                                          ViReal64 evaluateFrom,
                                                          ViReal64 evaluateTo)
{
	ViStatus    error   = VI_SUCCESS;
	ViChar      rep_cap [50] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (userRange, 1, 3) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "User Range");  
    }
	
	sprintf (rep_cap, "C%ld,U%ld", window, userRange);
        
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap, 
											   RSSPECAN_ATTR_PHASE_USER_EVALUATE_FROM,
                                               evaluateFrom), 3, "Evaluate From");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, rep_cap, 
											   RSSPECAN_ATTR_PHASE_USER_EVALUATE_TO,
                                               evaluateTo), 4, "Evaluate To");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Spot Noise Settings
 * Purpose:  This function configures the Phase Spot Noise Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpotNoiseSettings(
    ViSession   instrSession,
    ViBoolean   enable,
    ViReal64    offsetFrequency1,
    ViReal64    offsetFrequency2,
    ViReal64    offsetFrequency3,
    ViReal64    offsetFrequency4
)
{
    ViStatus    error   = VI_SUCCESS;
	ViChar		buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((offsetFrequency1<1.0)||(offsetFrequency1>1.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Offset Frequency 1");
        
    if ((offsetFrequency2<1.0)||(offsetFrequency2>1.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Offset Frequency 2");
    
    if ((offsetFrequency3<1.0)||(offsetFrequency3>1.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 5, "Offset Frequency 3");
        
    if ((offsetFrequency4<1.0)||(offsetFrequency4>1.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 6, "Offset Frequency 4");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "SN1",
                                                 RSSPECAN_ATTR_PHASE_CALC_SNO_STAT, 
                                                 enable), 2, "Enable");
	
	checkErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (strstr (buffer, "FSW") != VI_NULL)
	{
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "SN2",
                                                 RSSPECAN_ATTR_PHASE_CALC_SNO_STAT, 
                                                 enable), 2, "Enable");
	
		viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "SN3",
                                                 RSSPECAN_ATTR_PHASE_CALC_SNO_STAT, 
                                                 enable), 2, "Enable");
	
		viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "SN4",
                                                 RSSPECAN_ATTR_PHASE_CALC_SNO_STAT, 
                                                 enable), 2, "Enable");
	}
	
                                                 
    if(enable == VI_TRUE)
    {
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "SN1",
                                                    RSSPECAN_ATTR_PHASE_CALC_SNO_X,
                                                    offsetFrequency1), 3, "Offset Frequency 1");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "SN2",
                                                    RSSPECAN_ATTR_PHASE_CALC_SNO_X,
                                                    offsetFrequency2), 4, "Offset Frequency 2");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "SN3",
                                                    RSSPECAN_ATTR_PHASE_CALC_SNO_X,
                                                    offsetFrequency3), 5, "Offset Frequency 3");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "SN4",
                                                    RSSPECAN_ATTR_PHASE_CALC_SNO_X,
                                                    offsetFrequency4), 6, "Offset Frequency 4");
    
    }                                            


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Analyzer Settings
 * Purpose:  This function configures the Phase Analyzer Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseAnalyzerSettings(
    ViSession   instrSession,
    ViBoolean   preselector,
    ViBoolean   preamplifier
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (strstr (buffer, "B2,") != VI_NULL)
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                     RSSPECAN_ATTR_NOISE_INP_PRES_STAT,
                                                     preselector), 2, "Pre-selector");
    }
    
    if ((strstr (buffer, "B23") != VI_NULL) && ((strstr (buffer, "B25") != VI_NULL) || (strstr (buffer, "B2,") != VI_NULL)))
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "",
                                                     RSSPECAN_ATTR_NOISE_INP_GAIN_STAT,
                                                     preamplifier), 3, "Pre-amplifier");
    } 

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the spot noise calculation for each decade.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR spotNoiseCalculation/This control turns the spot noise calculation on every 
/// HIPAR spotNoiseCalculation/10x offset frequency on and off.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpotNoiseCalculation (ViSession instrSession,
                                                      ViBoolean spotNoiseCalculation)
{
	ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_PHASE_SPOT_NOISE_CALCULATION,
                                              spotNoiseCalculation), 2, "Spot Noise Calculation");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/*****************************************************************************
 * Function: Set Phase Spot Noise All Off
 * Purpose:  This function sets all the phase spot noise markers off.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SetPhaseAllSpotNoiseMarkersOff(
    ViSession   instrSession
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_CALC_SNO_AOFF,
                                                VI_NULL)); 


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the spur.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR spurSuppression/This control turns spur suppression on and off.
/// HIPAR spurThreshold/This control defines the level threshold for spur removal.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpurSettings (ViSession instrSession,
                                              ViBoolean spurSuppression,
                                              ViReal64 spurThreshold)
{
	ViStatus    error   = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_PHASE_SPUR_SUPPRESSION,
                                              spurSuppression), 2, "Spur Suppression");
	
	if (spurSuppression == VI_TRUE)
	{
		viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", RSSPECAN_ATTR_PHASE_SPUR_THRESHOLD,
	                                              spurThreshold), 3, "Spur Threshold");
	}
	
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Sweep Settings
 * Purpose:  This function configures the Phase Sweep Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSweepSettings(
    ViSession   instrSession,
    ViInt32     sweepMode
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((sweepMode<RSSPECAN_VAL_SWE_MODE_FAST)||(sweepMode>RSSPECAN_VAL_SWE_MODE_MAN))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Sweep Mode");
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_PHASE_SWEEP_MODE,
                                              sweepMode), 2, "Sweep Mode");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures repeated tries to start the measurement 
/// HIFN if signal verification fails on and off.
/// HIFN You may use multiple lines for documentation.
/// HIRET Document return value here.
/// HIRET You may use multiple lines for documentation.
/// HIPAR instrSession/Document parameter here.
/// HIPAR stopOnVerifyFailed/Turns repeated tries to start the measurement 
/// HIPAR stopOnVerifyFailed/if signal verification fails on and off.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSignalVerificationFailed (ViSession instrSession,
                                                          ViInt32 stopOnVerifyFailed)
{
	ViStatus    error   = VI_SUCCESS;  

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
											   RSSPECAN_ATTR_PHASE_SIGNAL_VERIFICATION_FAILED,
                                               stopOnVerifyFailed), 2, "Stop On Verify Failed");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Span Settings
 * Purpose:  This function configures the Phase Span Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpanSettings(
    ViSession   instrSession,
    ViReal64    startOffset,
    ViReal64    stopOffset
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((startOffset<1.0)||(startOffset>300.0e+6))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Start Offset");
        
    if ((stopOffset<3.0)||(stopOffset>1.0e+9))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Stop Offset");
    
    if (startOffset>stopOffset)
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Start Offset>Stop Offset");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_START,
                                                startOffset), 2, "Start Offset");
    
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_FREQUENCY_STOP,
                                                stopOffset), 3, "Stop Offset");
    


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Carrier Frequency Offset Table
 * Purpose:  This function configures the Carrier Frequency Offset Table.
 *           
 *           Notes:
 *           
 *           (1) The Carrier Frequency Offset Table can be used only when the
 *           Configure Sweep Settings is set to Manual value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhaseCarrierFrequencyOffsetTable(
    ViSession   instrSession,
    ViInt32     subband,
    ViReal64    rbw,
    ViInt32     average,
    ViInt32     fft
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      range[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     sweepMode;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr( rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_PHASE_SWEEP_MODE,
                                  &sweepMode));
    
    if (sweepMode == RSSPECAN_VAL_SWE_MODE_MAN)
    {
        if ((subband<1)||(subband>18))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Sub-band");
    
        if ((rbw<1.0)||(rbw>300.0e+6))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "RBW");
        
        if ((average<1)||(average>10000))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Average");
    
        if ((fft<RSSPECAN_VAL_BAND_TYPE_NORM)||(fft>RSSPECAN_VAL_BAND_TYPE_IQFFT))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 5, "FFT");
    
        sprintf(range,"R%d",subband);
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, range,
                                                    RSSPECAN_ATTR_PHASE_LIST_RANG_BAND,
                                                    rbw), 3, "RBW");
    
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, range,
                                                   RSSPECAN_ATTR_PHASE_LIST_RANG_SWE_COUN,
                                                   average), 4, "Average");
    
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, range,
                                                   RSSPECAN_ATTR_PHASE_LIST_RANG_FILT_TYPE,
                                                   fft), 5, "FFT");
    } 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Phase Preset Settings
 * Purpose:  This function configures the Phase Preset Settings.
 *           
 *           Notes:
 *           
 *           (1) The Preset Settings can be used only when the Configure Sweep
 *           Settings is set to Manual value.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePhasePresetSettings(
    ViSession   instrSession,
    ViReal64    rbw,
    ViInt32     average,
    ViInt32     fft
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     sweepMode;

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    checkErr( rsspecan_GetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_PHASE_SWEEP_MODE,
                                  &sweepMode));
    
    if (sweepMode == RSSPECAN_VAL_SWE_MODE_MAN)
    {
        if ((rbw<1.0)||(rbw>300.0e+6))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "RBW");
        
        if ((average<1)||(average>10000))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Average");
    
        if ((fft<RSSPECAN_VAL_BAND_TYPE_NORM)||(fft>RSSPECAN_VAL_BAND_TYPE_IQFFT))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "FFT");
    
        viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "Win1", RSSPECAN_ATTR_PHASE_BAND_RAT,
                                                    rbw), 2, "RBW");
    
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Win1", RSSPECAN_ATTR_PHASE_SWEEP_COUNT,
                                                   average), 3, "Average");
    
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "Win1", RSSPECAN_ATTR_PHASE_BAND_TYPE,
                                                   fft), 4, "FFT");
    }
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the window function for all or particular half decade.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR decade/This control selects the decades.
/// HIPAR subband/Specifies the sub-band for the IQ Window type.
/// HIPAR globalIQWindow/This control selects the window function for all or particular half decade.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseGlobalIQWindowSettings (ViSession instrSession,
                                                        ViInt32 decade,
														ViInt32 subband,
                                                        ViInt32 globalIQWindow)
{
	ViStatus    error   = VI_SUCCESS;
	ViChar      rep_cap [50] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (decade, RSSPECAN_VAL_PHASE_ALL_DECADE, 
									RSSPECAN_VAL_PHASE_PARTICULAR_DECADE) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Decade");  
    }
	
	if (rsspecan_invalidViInt32Range (subband, 1, 20) == VI_TRUE)
	{
		viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Subband");  
    }
	
	sprintf (rep_cap, "R%ld", subband);
        
    if (decade == RSSPECAN_VAL_PHASE_PARTICULAR_DECADE)
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, rep_cap, 
											   RSSPECAN_ATTR_PHASE_IQ_WINDOW_FUNCTION_PARTICULAR_DECADE,
                                               globalIQWindow), 4, "Global IQ Window");
	}
	else 
	{
		viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
											   RSSPECAN_ATTR_PHASE_IQ_WINDOW_FUNCTION_ALL_DECADE,
                                               globalIQWindow), 5, "Global IQ Window");
	}
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Create Phase Limit Line
 * Purpose:  This function creates new limit or selects existing one and 
 *           configures basic parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CreatePhaseLimitLine(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     limit,
    ViString    name,
    ViString    comment,
    ViInt32     assigntoTrace
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    }
    
    if (strlen (name) > 8)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Name");
    }

    if (strlen (comment) > 40)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Comment");
    }

    sprintf (buffer, "L%ld", limit);
    
    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_NAME, name), 4, "Name");
    viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_COMMENT,comment), 5, "Comment");

    sprintf (buffer, "Win%ld,L%ld", window, limit);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_ASSIGN_TRACE, assigntoTrace), 6, "Assign to Trace");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Define Phase Limit Line
 * Purpose:  This function defines specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DefinePhaseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     type,
    ViInt32     count,
    ViReal64    xAxis[],
    ViReal64    amplitude[]
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      *pbuffer= VI_NULL;
    ViChar      *p2buf;
    ViInt32     i;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }
    if (rsspecan_invalidViInt32Range (type, 0, 1))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Type");
    }
    if (count < 1)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 4, "Count");
    }
    if (!xAxis)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "X Axis");
    }
    if (!amplitude)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 6, "Amplitude");
    }

    viCheckAlloc (pbuffer = (ViChar*) malloc (count * 20 + 200));
    
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

    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Enable Phase Limit Check
 * Purpose:  This function switches on or off the limit check and/or limit 
 *           line for the selected limit line in the selected measurement 
 *           window.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_EnablePhaseLimitCheck(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     limit,
    ViInt32     type,
    ViBoolean   limitEnabled,
    ViBoolean   checkEnabled
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     attribute = 0;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
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
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, buffer, attribute, checkEnabled), 6, "Check Enabled");
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Move Phase Limit Line
 * Purpose:  This function moves specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_MovePhaseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     type,
    ViReal64    value
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViInt32     attributes[] = {RSSPECAN_LIMIT_LOWER_SHIFT,
                                RSSPECAN_LIMIT_UPPER_SHIFT,
                                RSSPECAN_LIMIT_CONTROL_SHIFT};


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }

    if (rsspecan_invalidViInt32Range (type, 0, 2))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Type");
    }

    sprintf (buffer, "L%ld", limit);

    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, attributes[type], value), 4, "Value");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Copy Phase Limit Line
 * Purpose:  This function copies specified limit line into another one.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_CopyPhaseLimitLine(
    ViSession   instrSession,
    ViInt32     limit,
    ViInt32     copyTo
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }

    sprintf (buffer, "L%ld", limit);

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_COPY, copyTo), 3, "Copy To");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Clear Phase Limit Line Results
 * Purpose:  This function deletes the result of the current limit check.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ClearPhaseLimitLineResults(
    ViSession   instrSession,
    ViInt32     limit
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      lim_line_no[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }
    
    sprintf(lim_line_no,"Win1,L%d",limit);

    viCheckErr(rsspecan_SetAttributeViString (instrSession, lim_line_no,
                                               RSSPECAN_LIMIT_CLEAR, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Delete Phase Limit Line
 * Purpose:  This function removes specified limit line.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_DeletePhaseLimitLine(
    ViSession   instrSession,
    ViInt32     limit
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit");
    }

    sprintf (buffer, "L%ld", limit);

    viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_LIMIT_DELETE, VI_NULL));


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the names of all active limit lines.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR bufferSize/Pass the number of bytes in the string you specify for the Active Limit Lines parameter.
/// HIPAR activeLimitLines/This control returns the names of all active limit lines in  alphabetical order.
ViStatus _VI_FUNC rsspecan_GetActivePhaseLimitLine (ViSession instrSession,
                                           ViInt32 bufferSize,
                                           ViChar activeLimitLines[])
{
	ViStatus    error   = VI_SUCCESS;
  
    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViString (instrSession, "", 
						                RSSPECAN_ATTR_LIMIT_LINE_ACTIVE, bufferSize, 
										activeLimitLines), 3, "Active Limit Lines"); 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function configures the phase noise limit parameters.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR shapeType/This control selects the shape of a phase noise limit line.
/// HIPAR selectedTraces/Selects the trace(s) to assign a phase noise limit line to.
/// HIPAR noiseFloor/This control  defines the noise floor level of the DUT.
/// HIPAR segment/Selects the limit line segment.
/// HIPAR cornerFrequency/Sets the start frequency of a segment of a phase noise limit line.
/// HIPAR slope/This control defines the slope for a phase noise limit line segment.
ViStatus _VI_FUNC rsspecan_ConfigurePhaseNoiseLimitSettings (ViSession instrSession,
                                                    ViInt32 shapeType,                                                
                                                    ViString selectedTraces,
                                                    ViReal64 noiseFloor,
                                                    ViInt32 segment,
													ViReal64 cornerFrequency,
                                                    ViReal64 slope)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[25] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (segment, 1, 5))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 5, "Segment");
    }

    viCheckParm (rsspecan_GetAttributeRepCapName (instrSession,
                                                 RSSPECAN_ATTR_PHASE_NOISE_LIMIT_SLOPE,
                                                 "pnlSegment",
                                                 segment - 1,
                                                 25,
                                                 buffer), 5, "Segment");

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_NOISE_LIMIT_SHAPE_TYPE,
												shapeType), 2, "Shape Type");
    
	viCheckErr (viPrintf (instrSession, "CALC:PNL:TRAC %s", selectedTraces));

	checkErr (rsspecan_CheckStatus (instrSession));
		
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_NOISE_LIMIT_LEVEL,
												noiseFloor), 4, "Noise Floor");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, 
												RSSPECAN_ATTR_PHASE_NOISE_LIMIT_CORNER_FREQUENCY,
												cornerFrequency), 6, "Corner Frequency");
	
	viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, 
												RSSPECAN_ATTR_PHASE_NOISE_LIMIT_SLOPE,
												slope), 7, "Slope");											

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function creates a new user limit line from the data of a phase 
/// HIFN  noise limit line.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR limitLine/Number of the limit line the phase noise limit line is copied to. An 
/// HIPAR limitLine/existing limit line in that slot is overwritten.
ViStatus _VI_FUNC rsspecan_PhaseCopyToUserLimitLine (ViSession instrSession,
                                                     ViInt32 limitLine)
{
	ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limitLine, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Limit Line");
    }

    sprintf (buffer, "LL%ld",  limitLine);

    viCheckErr (rsspecan_SetAttributeViString (instrSession, buffer, 
							RSSPECAN_ATTR_PHASE_COPY_TO_USER_LIMIT_LINE, VI_NULL)); 
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN This function queries the limit check results for phase noise limit lines.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR limitCheck/This control returns the limit check results for phase noise limit lines.
ViStatus _VI_FUNC rsspecan_GetPhaseNoiseLimitCheckState (ViSession instrSession,
                                                ViInt32 *limitCheck)
{
	ViStatus    error   = VI_SUCCESS;
   
    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, "", 
												RSSPECAN_ATTR_PHASE_NOISE_LIMIT_CHECK_STATE,
												limitCheck), 2, "Limit Check");
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Phase Limit Check Result
 * Purpose:  This function queries the result of the limit check of the limit 
 *           line indicated in the selected measurement window. It should be 
 *           noted that a complete sweep must have been performed for obtaining 
 *           a valid result. A synchronization with  *OPC, *OPC? or *WAI should 
 *           therefore be provided.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetPhaseLimitCheckResult(
    ViSession   instrSession,
    ViInt32     window,
    ViInt32     limit,
    ViInt32*    state
)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (limit, 1, 8))
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 3, "Limit");
    }

    sprintf (buffer, "Win%ld,L%ld", window, limit);

    viCheckParm (rsspecan_GetAttributeViInt32 (instrSession, buffer, RSSPECAN_LIMIT_CHECK_RESULT, state), 4, "State");
    

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/*****************************************************************************
 * Function: Trace Phase Settings
 * Purpose:  This function configures the Phase Trace Settings.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_TracePhaseSettings(
    ViSession   instrSession,
    ViInt32     traceNumber,
    ViBoolean   traceState,
    ViInt32     traceMode,
    ViBoolean   smoothing
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      traceNR[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((traceNumber<1)||(traceNumber>3))
            viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Trace Number");
        
    if ((traceMode<RSSPECAN_TRAC_MOD_WRITE)||(traceMode>RSSPECAN_TRAC_MOD_MINH))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 4, "Trace Mode");

    sprintf(traceNR, "TR%ld", traceNumber);
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, traceNR,
                                                 RSSPECAN_ATTR_PHASE_DISP_TRACE_SMO,
                                                 smoothing), 5, "Smoothing");

    sprintf(traceNR, "Win1,TR%ld", traceNumber);
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, traceNR,
                                                 RSSPECAN_ATTR_TRACE_STATE, traceState), 3, "Trace State");
    
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, traceNR,
                                               RSSPECAN_ATTR_TRACE_TYPE, 
                                               traceMode), 4, "Trace Mode");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Reference Phase Measurement
 * Purpose:  This function configures and initiates a reference measurement.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReferencePhaseMeasurement(
    ViSession   instrSession,
    ViUInt32    timeout
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     old_timeout = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViUInt32Range (timeout, 0, 4294967295UL) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Timeout");
        
    checkErr (rsspecan_GetOPCTimeout (instrSession, &old_timeout));
    checkErr (rsspecan_SetOPCTimeout (instrSession, timeout));
    viCheckErr( rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_PHASE_CONF_REFM,
                                               VI_NULL));

    checkErr (rsspecan_SetOPCTimeout (instrSession, old_timeout));   


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/*****************************************************************************
 * Function: Get Phase Estimated Measurement Time
 * Purpose:  This function returns the Phase Sweep Estimated Measurement Time.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetPhaseEstimatedMeasurementTime(
    ViSession   instrSession,
    ViReal64*   estimatedMeasurementTime
)
{
    ViStatus    error   = VI_SUCCESS;


    checkErr( Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "Win1",
                                                RSSPECAN_ATTR_SWEEP_TIME,
                                                estimatedMeasurementTime),
                                                2, "Estimated Measurement Time");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*===========================================================================*/
/* Function: Read Phase Trace Data                                           */
/* Purpose:  This function reads out trace data from the instrument.         */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ReadPhaseTraceData(
    ViSession instrSession,
    ViInt32 sourceTrace, 
	ViPInt32 noofPoints,
    ViReal64 _VI_FAR traceDataX[],
    ViReal64 _VI_FAR traceDataY[]
)
{
    ViStatus    error   = VI_SUCCESS;

    ViInt32     retCnt  = 0;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViInt32     cnt     = 0;
	ViReal64*	data = NULL;
	ViInt32		i = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if (rsspecan_invalidViInt32Range (sourceTrace, 0, 2))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Source Trace");
	
	sprintf (buffer, "TRACE%ld", sourceTrace + 1);
    
    if (*noofPoints != VI_NULL)
    {
		data = (ViReal64*) malloc (*noofPoints * sizeof (ViReal64) * 2);
		
    	viCheckErr (rsspecan_dataReadTrace (instrSession, 0, buffer, *noofPoints * 2,  data, &retCnt));
		
		while (cnt < (*noofPoints))
	    {
	        traceDataX[cnt] = data[i++];
	        traceDataY[cnt++] = data[i++];
	    }
	}
	else
	{
		data = (ViReal64*) malloc (RSSPECAN_DATA_BUFFER_SIZE * sizeof (ViReal64) * 2);
		
		viCheckErr (rsspecan_dataReadTrace (instrSession, 0, buffer, RSSPECAN_DATA_BUFFER_SIZE,  data, &retCnt));
		
		while (cnt < (retCnt / 2))
	    {
	        traceDataX[cnt] = data[i++];
	        traceDataY[cnt++] = data[i++];
	    }
	}
    
    *noofPoints = cnt;
    
    if (pbuffer) free(pbuffer);
    
    if ((error = rsspecan_CheckStatus (instrSession)) < 0)
        return error;

    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Fetch Phase Results
 * Purpose:  This function returns the phase measurement results.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_FetchPhaseResidualResults(
    ViSession   instrSession,
    ViInt32     modifier,
    ViReal64*   value
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar*     pbuffer = VI_NULL; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    if ((modifier<0)||(modifier>2))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Modifier");
    
    viCheckErr (viPrintf (instrSession, "FETC:PNO:%s?\n", fetchPhaseArr[modifier]));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, VI_NULL)); 

    sscanf(pbuffer,"%le",value);

    if (pbuffer) free(pbuffer);
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN This function returns the phase measurement results.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR userRange/Selects the user range.
/// HIPAR modifier/Sets the modifier of returned value.
/// HIPAR value/Returns the value.
ViStatus _VI_FUNC rsspecan_FetchPhaseResidualUserResults (ViSession instrSession,
                                                 ViInt32 userRange,
                                                 ViInt32 modifier,
                                                 ViReal64 *value)
{
	ViStatus    error   = VI_SUCCESS;

    ViChar*     pbuffer = VI_NULL; 

    checkErr( Rs_LockSession (instrSession, VI_NULL));
	
	if ((userRange<1)||(userRange>3))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "User Range");
    
    if ((modifier<0)||(modifier>2))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 3, "Modifier");
    
    viCheckErr (viPrintf (instrSession, "FETC:PNO:USER%ld:%s?\n", userRange, fetchPhaseArr[modifier]));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, VI_NULL)); 

    sscanf(pbuffer,"%le",value);

    if (pbuffer) free(pbuffer);
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 


/// HIFN  This function queries the location and level of all spurs that have 
/// HIFN  been detected.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR noofPoints/Returns the number of trace data points.
/// HIPAR frequencyValues/Returns frequency values for each each spur that has been detected.
/// HIPAR levelValues/Returns level values for each each spur that has been detected.
ViStatus _VI_FUNC rsspecan_FetchPhaseSpurs(
	ViSession	instrSession,
	ViInt32*	noofPoints,
	ViReal64	frequencyValues[],
	ViReal64	levelValues[]
)
{
    ViStatus	error = VI_SUCCESS;
	ViChar		buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViUInt32    retCnt  = 0;
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViInt32     cnt     = 0;
	
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K40")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
    if (*noofPoints != VI_NULL)
    {
		viCheckErr (viPrintf (instrSession, "FETC:PNO:SPUR?\n"));
	    viCheckErr( Rs_ReadDataUnknownLength(instrSession, &pbuffer, VI_NULL));

		pstring_value = strtok (pbuffer, ",");
		
		while (pstring_value != NULL && cnt < (*noofPoints))
	    {
	        frequencyValues[cnt] = atof (pstring_value);
			pstring_value = strtok (NULL, ",");
	        levelValues[cnt++] = atof (pstring_value);
			pstring_value = strtok (NULL, ",");
	    }
	}
	else
	{
		viCheckErr (viPrintf (instrSession, "FETC:PNO:SPUR?\n"));
	    viCheckErr( Rs_ReadDataUnknownLength(instrSession, &pbuffer, &retCnt));

		pstring_value = strtok (pbuffer, ",");
		
		while (pstring_value != NULL && cnt < ((ViInt32)retCnt / 2))
	    {
	        frequencyValues[cnt] = atof (pstring_value);
			pstring_value = strtok (NULL, ",");
	        levelValues[cnt++] = atof (pstring_value);
			pstring_value = strtok (NULL, ",");
	    }
	}
    
    *noofPoints = cnt;
    
    checkErr( rsspecan_CheckStatus (instrSession));

Error:
    if (pbuffer) free(pbuffer);
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/// HIFN  This function queries the Discrete Jitter result.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PHASE_MEASURED_DISCRETE_JITTER
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:PNOise:SPURs:DISCrete?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR discrete/Queries the Discrete Jitter result.
ViStatus _VI_FUNC rsspecan_FetchPhaseSpursDiscrete(
	ViSession	instrSession,
	ViReal64*	discrete
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_MEASURED_DISCRETE_JITTER,
                                                discrete), 2, "Discrete");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function queries the Random Jitter result.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PHASE_MEASURED_RANDOM_JITTER
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:PNOise:SPURs:RANDom?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR random/Queries the Random Jitter result.
ViStatus _VI_FUNC rsspecan_FetchPhaseSpursRandom(
	ViSession	instrSession,
	ViReal64*	random
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_MEASURED_RANDOM_JITTER,
                                                random), 2, "Random");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function queries the carrier frequency and the level of the DUT 
/// HIFN  that has been actually measured.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR frequency/Returns the carrier frequency that has been actually measured.
/// HIPAR level/Returns the level of the DUT that has been actually measured.
ViStatus _VI_FUNC rsspecan_FetchPhaseFrequencyLevel(
	ViSession	instrSession,
	ViReal64*	frequency,
	ViReal64*	level
)
{
    ViStatus	error = VI_SUCCESS;
    
    checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_MEASURED_FREQUENCY,
                                                frequency), 2, "Frequency");
    
	viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_MEASURED_LEVEL,
                                                level), 3, "Level");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}

/// HIFN  This function queries the Integrated Phase Noise for the first trace.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PHASE_MEASURED_INTEGRATED_PHASE_NOISE
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:PNOise:IPN?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR ipn/Queries the Integrated Phase Noise for the first trace.
ViStatus _VI_FUNC rsspecan_FetchPhaseIntegratedPhaseNoise(
	ViSession	instrSession,
	ViReal64*	ipn
)
{
    ViStatus	error = VI_SUCCESS;
    
	checkErr (Rs_LockSession (instrSession, VI_NULL));

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, "",
                                                RSSPECAN_ATTR_PHASE_MEASURED_INTEGRATED_PHASE_NOISE,
                                                ipn), 2, "IPN");

Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/// HIFN  This function queries the Integrated Phase Noise for a particular user 
/// HIFN  range.
/// HIFN     
/// HIFN     The trace that is queried depends on 
/// HIFN  CALCulate<n>:EVALuation:USER<range>:TRACe.
/// HIFN     
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_PHASE_MEASURED_USER_INTEGRATED_PHASE_NOISE
/// HIFN     
/// HIFN     Remote-control command(s):
/// HIFN     FETCh:PNOise:USER<User>:IPN?
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR user/Sets the user number.
/// HIPAR ipn/Queries the Integrated Phase Noise for a particular user range.
ViStatus _VI_FUNC rsspecan_FetchPhaseUserIntegratedPhaseNoise(
	ViSession	instrSession,
	ViInt32	user,
	ViReal64*	ipn
)
{
    ViStatus	error = VI_SUCCESS;
    ViChar	rep_cap[5] = "";

    checkErr (Rs_LockSession (instrSession, VI_NULL));

    if (rsspecan_invalidViInt32Range (user, 1, 3) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "User");
    }

    sprintf (rep_cap, "U%ld", user);  

    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, rep_cap,
                                                RSSPECAN_ATTR_PHASE_MEASURED_USER_INTEGRATED_PHASE_NOISE,
                                                ipn), 3, "IPN");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);    
    return error;
}


/*****************************************************************************
 * Function: Get Phase Spot Noise Y Position
 * Purpose:  This function returns the Phase Spot Noise Y Position.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetPhaseSpotNoiseYPosition(
    ViSession   instrSession,
    ViInt32     spotNoiseMarkerNumber,
    ViReal64*   spotNoiseMarkerResult
)
{
    ViStatus    error   = VI_SUCCESS;

    ViChar      snoiseNR[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL));

    if ((spotNoiseMarkerNumber<1)||(spotNoiseMarkerNumber>5))
        viCheckParm( RS_ERROR_INVALID_PARAMETER, 2, "Spot Noise Marker Number");
    
    sprintf(snoiseNR, "SN%ld", spotNoiseMarkerNumber);
    
    viCheckParm (rsspecan_GetAttributeViReal64 (instrSession, snoiseNR, RSSPECAN_ATTR_PHASE_CALC_SNO_Y,
                                                spotNoiseMarkerResult), 3, "Spot Noise Marker Result");


Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
} 

/// HIFN This function returns the horizontal and vertical poistion of the 10x offset frequency spot noise markers.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or 
/// HIPAR instrSession/rsspecan_InitWithOptions function.  The handle identifies a particular 
/// HIPAR instrSession/instrument session.
/// HIPAR arraySize/This parameter determines the size of array result values. 
/// HIPAR verticalPosition/Returns the list of level values, one for each 10x spot noise marker.
/// HIPAR horizontalPosition/Returns the list of offset frequencies, one for each 10x spot noise marker.
/// HIPAR returnedValues/Returns the number of returned values.
ViStatus _VI_FUNC rsspecan_GetPhaseDecadeSpotNoiseResult (ViSession instrSession,
                                                 ViInt32 arraySize,
                                                 ViReal64 verticalPosition[],
                                                 ViReal64 horizontalPosition[],
                                                 ViInt32 *returnedValues)
{
	ViStatus    error = VI_SUCCESS;
	ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    ViChar*     pbuffer = VI_NULL; 
    ViChar*     pstring_value;
    ViUInt32    ret_cnt;
    ViInt32     cnt = 0;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, 
														RSSPECAN_IO_BUFFER_SIZE, buffer));

    if (!(strstr (buffer, "K40")))
        viCheckErr (RS_ERROR_INSTRUMENT_OPTION);
	
    viCheckErr (viPrintf (instrSession, "CALC:SNO:DEC:X?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 
	
	checkErr (rsspecan_CheckStatus (instrSession));
    
	pstring_value = strtok (pbuffer, ",");  
    while (pstring_value && cnt<arraySize)
	{
    	sscanf (pstring_value, "%le", &horizontalPosition[cnt++]);
        pstring_value = strtok (NULL, ","); 
    }
    
	if (pbuffer) free (pbuffer);
	cnt = 0;
	
	viCheckErr (viPrintf (instrSession, "CALC:SNO:DEC:Y?\n"));
    viCheckErr (Rs_ReadDataUnknownLength (instrSession, &pbuffer, &ret_cnt)); 

    pstring_value = strtok (pbuffer, ",");  
    while (pstring_value && cnt<arraySize)
	{
        sscanf (pstring_value, "%le", &verticalPosition[cnt++]);
        pstring_value = strtok (NULL, ","); 
    }
    if (pbuffer) free (pbuffer);
    
    *returnedValues = cnt;
	
    checkErr (rsspecan_CheckStatus (instrSession));
	
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}
