/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver - K14 Real Time Spectrum
 *  VXIpnp Style Instrument Driver
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

/*****************************************************************************
 *- RSSpecAn K14 - Spectrogram Measurement ----------------------------------------*
 *****************************************************************************/

//static ViReal64 rateArr[] = {15.625E3, 31.25E3, 62.5E3, 125E3, 250E3, 500E3, 1E6, 2E6, 4E6, 8E6, 16E6, 32E6};

/*****************************************************************************
 * Function: Spectrogram Mode
 * Purpose:  This function activates or deactivates the Spectrogram
 *           Measurement option.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpectrogramMode (ViSession instrSession,
                                            ViBoolean state)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_SPEM_STATE, state),
    		2, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function activates realtime mode.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
ViStatus _VI_FUNC rsspecan_RealtimeSpectrumAnalysisMode (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_REALTIME_MODE, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Spectrogram Window
 * Purpose:  This function selects the current window for the realtime
 *           spectrum.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpectrogramWindow (ViSession instrSession,
                                              ViInt32 window)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SPEM_WINDOW, window));

Error:
     (void)RsCore_UnlockSession(instrSession);
     return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Sweep
 * Purpose:  This function starts a continuous sweep measurement or stops it
 *           and switches to single sweep mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramSweep (ViSession instrSession,
                                                      ViBoolean continuous)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS, continuous),
    		2, "Continuous");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Display Parameters
 * Purpose:  This function configures Spectogram display parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDisplayParameters (ViSession instrSession,
                                                                  ViInt32 color,
                                                                  ViInt32 size)
{
    ViInt32     win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      repCap[RS_REPCAP_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    snprintf (repCap, RS_REPCAP_BUF_SIZE, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SPEM_COLOR, color),
    		2, "Color");

	if (RsCore_IsInstrumentModel(instrSession, "FSL"))
	{
		viCheckParm(rsspecan_SetAttributeViInt32(instrSession, repCap, RSSPECAN_ATTR_SPEM_SIZE, size),
				3, "Size");
	}

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Colors
 * Purpose:  This function configures Spectogram color parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramColors (ViSession instrSession,
                                                       ViReal64 shape,
                                                       ViInt32 colorStyle,
                                                       ViReal64 colorLower,
                                                       ViReal64 colorUpper)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_SPEM_SHAPE, shape),
    		2, "Shape");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_COLOR_STYLE, colorStyle),
    		3, "Color Style");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_SPEM_COLOR_LOW, colorLower),
    		4, "Color Lower");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_SPEM_COLOR_UPPER, colorUpper),
    		5, "Color Upper");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Default Colors
 * Purpose:  This function sets the color settings for the spectrogram result
             display to its default state.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDefaultColors (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SPEM_COLOR_DEFAULT, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Measurement Parameters
 * Purpose:  This function configures Spectogram measurement parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMeasurementParameters (ViSession instrSession,
                                                                      ViInt32 historyDepth,
                                                                      ViBoolean timeStamp,
                                                                      ViInt32 frameCount,
                                                                      ViBoolean continueFrame)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_HDEP, historyDepth),
    		2, "History Depth");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_SPEM_TIME_STAMP_STATE, timeStamp),
    		3, "Time Stamp");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_FRAME_COUNT, frameCount),
    		4, "Frame Count");

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_SPEM_CONTINUOUS, continueFrame),
    		5, "Continue Frame");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Selected Frame
 * Purpose:  This function selects the frame if no measurement is running.
             During a measurement, the value is fixed to 0 and this command
             is available only if no measurement is running or after a single
             sweep has ended.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramSelectedFrame (ViSession instrSession,
                                                              ViInt32 frame_number)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_FRAME_SELECT, frame_number),
    		2, "Frame number");

Error:
     (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Spectrogram Clear
 * Purpose:  This function deletes the spectrogram result display and the history buffer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpectrogramClear (ViSession instrSession)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_CLEAR, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram FFT Window
 * Purpose:  This function selects the type of FFT window that you want to
 *           use in realtime mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramFFTWindow (ViSession instrSession,
                                                          ViInt32 FFTWindowType)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL,
        RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld", win);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_FFT_WINDOW, FFTWindowType),
    		2, "FFT Window Type");

Error:
     (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Marker Frame Number
 * Purpose:  This function defines the frame number of the specified marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMarkerFrameNumber (ViSession instrSession,
                                                                  ViInt32 markerNumber,
                                                                  ViInt32 frameNumber)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,M%ld",win, markerNumber);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARK_FRAME_NUMBER, frameNumber),
    		3, "Frame Number");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Spectrogram Marker Frame Number
 * Purpose:  This function returns the frame number of the specified marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSpectrogramMarkerFrameNumber (ViSession instrSession,
                                                            ViInt32 markerNumber,
                                                            ViInt32 *frameNumber)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,M%ld",win, markerNumber);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARK_FRAME_NUMBER, frameNumber),
    		3, "Frame Number");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Marker Search Area
 * Purpose:  This function defines the search area of the specified marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMarkerSearchArea (ViSession instrSession,
                                                                 ViInt32 markerNumber,
                                                                 ViInt32 searchArea)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,M%ld",win, markerNumber);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_AREA, searchArea),
    		3, "Search Area");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Marker Search
 * Purpose:  This function positions the specified marker on the specified place.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpectrogramMarkerSearch (ViSession instrSession,
                                                    ViInt32 markerNumber,
                                                    ViInt32 searchMode)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, markerNumber, 1, 4),
    		2, "Marker Number");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, searchMode, 0, 18),
    		3, "Search Mode");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,M%ld", win, markerNumber);

    switch (searchMode){
        case RSSPECAN_VAL_SPECM_MARK_MAX_PEAK:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MAX_NEXT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_NEXT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MAX_LEFT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_LEFT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MAX_RIGHT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_PEAK_RIGHT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_PEAK:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_NEXT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN_NEXT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_LEFT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN_LEFT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_RIGHT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_MIN_RIGHT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_PEAK:
            sprintf (buffer, "Win%ld,M%ld,Max,Peak", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_NEXT:
            sprintf (buffer, "Win%ld,M%ld,Max,Next", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_ABOVE:
            sprintf (buffer, "Win%ld,M%ld,Max,Above", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_BELOW:
            sprintf (buffer, "Win%ld,M%ld,Max,Below", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_PEAK:
            sprintf (buffer, "Win%ld,M%ld,Min,Peak",win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_NEXT:
            sprintf (buffer, "Win%ld,M%ld,Min,Next", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_ABOVE:
            sprintf (buffer, "Win%ld,M%ld,Min,Above", win,markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_BELOW:
            sprintf (buffer, "Win%ld,M%ld,Min,Below", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_XY_MAX:
            sprintf (buffer, "Win%ld,M%ld,Max", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_XY, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_XY_MIN:
            sprintf (buffer, "Win%ld,M%ld,Min", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_MARKER_SEARCH_XY, NULL));
        case RSSPECAN_VAL_SPECM_MARK_Y_TRIGGER:
            sprintf (buffer, "Win%ld,DM%ld", win, markerNumber);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_MARKER_SEARCH_Y_TRIGGER, NULL));
        break;
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Delta Marker Frame Number
 * Purpose:  This function defines the frame number of the specified delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDeltaMarkerFrameNumber (ViSession instrSession,
                                                                       ViInt32 deltaMarkerNumber,
                                                                       ViInt32 frameNumber)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarkerNumber, 1, 4),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,DM%ld",win, deltaMarkerNumber);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_FRAME_NUMBER, frameNumber),
    		3, "Frame Number");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}
/*****************************************************************************
 * Function: Get Spectrogram Delta Marker Frame Number
 * Purpose:  This function returns the frame number of the specified delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSpectrogramDeltaMarkerFrameNumber (ViSession instrSession,
                                                                 ViInt32 deltaMarker,
                                                                 ViInt32 *frameNumber)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 4),
    		2, "Delta Marker");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,DM%ld",win, deltaMarker);

    viCheckParm(rsspecan_GetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_FRAME_NUMBER, frameNumber),
    		3, "Frame Number");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Delta Marker Search Area
 * Purpose:  This function defines the search area of the specified delta marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDeltaMarkerSearchArea (ViSession instrSession,
                                                                      ViInt32 deltaMarker,
                                                                      ViInt32 searchArea)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 4),
    		2, "Delta Marker");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,DM%ld",win, deltaMarker);
    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_AREA, searchArea),
    		3, "Search Area");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Persistence Spectrum Parameters
 * Purpose:  This function sets the persistence spectrum parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumParameters (ViSession instrSession,
                                                                   ViInt32 trace,
                                                                   ViBoolean enablePersistence,
                                                                   ViReal64 duration,
                                                                   ViReal64 granularity)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,TR%ld",win,trace);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_PSPEM_STATE, enablePersistence),
    		2, "Enable Persistence");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_PSPEM_DURATION, duration),
    		3, "Duration");

     viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_PSPEM_GRAN, granularity),
     		4, "Granularity");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Persistence Spectrum Display Parameters
 * Purpose:  This function sets the persistence spectrum display parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumDisplayParameters (ViSession instrSession,
                                                                          ViReal64 shape,
                                                                          ViInt32 style,
                                                                          ViReal64 colorLower,
                                                                          ViReal64 colorUpper)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld",win);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_PSPEM_SHAPE, shape),
    		3, "Shape");

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_PSPEM_COLOR_STYLE, style),
    		4, "Style");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_PSPEM_COLOR_LOWER, colorLower),
    		5, "Color Lower");

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_PSPEM_COLOR_UPPER, colorUpper),
    		5, "Color Upper");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function sets the color settings for the persistence
/// HIFN spectrum result display to its default state.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumDefaultColors (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_PSPEM_COLOR_DEFAULT, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/*****************************************************************************
 * Function: Configure Persistence Spectrum Truncate
 * Purpose:  This function reduces the range of the color map of the
             persistence spectrum if there are no hits at the start or end of
             the value range.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumTruncate (ViSession instrSession,
                                                                 ViBoolean truncate)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, "", RSSPECAN_ATTR_PSPEM_TRUN, truncate),
    		5, "Truncate");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Persistence Spectrum Trace Style
 * Purpose:  This function sets the persistence spectrum trace style.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumTraceStyle (ViSession instrSession,
                                                                   ViInt32 trace,
                                                                   ViInt32 symbol)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,TR%ld",win,trace);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_PSPEM_TRACE_STYLE, symbol),
    		2, "Symbol");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Persistence Spectrum Maxhold Intensity
 * Purpose:  This function sets the persistence spectrum maxhold trace
 *           parameters.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumMaxholdIntensity (ViSession instrSession,
                                                                     ViInt32 trace,
                                                                     ViBoolean enableMaxhold,
                                                                     ViInt32 intensity)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,TR%ld",win,trace);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_PSPEM_MAXH_TRACE, enableMaxhold),
    		3, "Enable Maxhold");

    if (enableMaxhold == VI_TRUE)
    {
        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_PSPEM_MAXH_INTENS, intensity),
        		4, "Intensity");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN This function resets the maxhold trace in the persistence
/// HIFN spectrum result display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/The ViSession handle that you obtain from the rsspecan_init or rsspecan_InitWithOptions function.
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumMaxholdReset (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_PSPEM_MAXH_RESET, NULL));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/// HIFN This function defines if the delta marker is positioned on the
/// HIFN persistence trace or the maxhold trace. .
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the
/// HIPAR instrSession/Initialize function to select the desired instrument driver session.
/// HIPAR marker/This control selects delta marker.
/// HIPAR markerPostion/This control defines if the marker is positioned on the
/// HIPAR markerPostion/persistence trace or the maxhold trace.
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumMarkerPosition (ViSession instrSession,
                                                                       ViInt32 marker,
                                                                       ViInt32 markerPostion)
{
    ViInt32     win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,M%ld", win, marker);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_PSPEM_MARKER_POSITION, markerPostion),
    		3, "Marker Postion");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/// HIFN This function queries the z-axis of the indicated marker in the
/// HIFN persistence spectrum result display.
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the
/// HIPAR instrSession/Initialize function to select the desired instrument driver session.
/// HIPAR marker/This control selects marker.
/// HIPAR zAxis/queries the z-axis of the indicated marker in the persistence
/// HIPAR zAxis/spectrum result display.
ViStatus _VI_FUNC rsspecan_QueryPersistenceMarkerZAxis (ViSession instrSession,
                                                        ViInt32 marker,
                                                        ViReal64 *zAxis)
{
    ViInt32     win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,M%ld", win, marker);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_QUERY_PSPEM_MARKER_Z_AXIS, zAxis),
    		3, "Z-Axis");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}



/// HIFN This function defines if the delta marker is positioned on the
/// HIFN persistence trace or the maxhold trace. .
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the
/// HIPAR instrSession/Initialize function to select the desired instrument driver session.
/// HIPAR marker/This control selects delta marker.
/// HIPAR deltaMarkerPostion/This control defines if the delta marker is positioned on the
/// HIPAR deltaMarkerPostion/persistence trace or the maxhold trace.
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumDeltaMarkerPosition (ViSession instrSession,
                                                                            ViInt32 marker,
                                                                            ViInt32 deltaMarkerPostion)
{
    ViInt32     win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, marker, 1, 16),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,DM%ld", win, marker);

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_PSPEM_DELTA_MARKER_POSITION, deltaMarkerPostion),
    		3, "Delta Marker Postion");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Query Persistence Delta Marker Z-Axis
 * Purpose:  This function queries the z-axis of the indicated delta marker
 *           in the persistence spectrum result display.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_QueryPersistenceDeltaMarkerZAxis (ViSession instrSession,
                                                             ViInt32 deltaMarker,
                                                             ViReal64 *zAxis)
{
    ViInt32     win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 16),
    		2, "Marker Number");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,DM%ld", win, deltaMarker);

    viCheckParm(rsspecan_GetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_QUERY_PSPEM_DELTA_Z_AXIS, zAxis),
    		3, "Z-Axis");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Delta Marker Search
 * Purpose:  This function positions the specified delta marker on the specified place.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpectrogramDeltaMarkerSearch (ViSession instrSession,
                                                         ViInt32 deltaMarker,
                                                         ViInt32 searchMode)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, deltaMarker, 1, 4),
    		2, "Delta Marker");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, searchMode, 0, 18),
    		3, "Search Mode");
    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld,DM%ld", win, deltaMarker);

    switch (searchMode){
        case RSSPECAN_VAL_SPECM_MARK_MAX_PEAK:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MAX_NEXT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MAX_LEFT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_LEFT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MAX_RIGHT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_RIGHT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_PEAK:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_MIN, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_NEXT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_LEFT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_LEFT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_MIN_RIGHT:
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_RIGHT, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_PEAK:
            sprintf (buffer, "Win%ld,DM%ld,Max,Peak", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_NEXT:
            sprintf (buffer, "Win%ld,DM%ld,Max,Next", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_ABOVE:
            sprintf (buffer, "Win%ld,DM%ld,Max,Above", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MAX_BELOW:
            sprintf (buffer, "Win%ld,DM%ld,Max,Below", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_PEAK:
            sprintf (buffer, "Win%ld,DM%ld,Min,Peak", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_NEXT:
            sprintf (buffer, "Win%ld,DM%ld,Min,Next", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_ABOVE:
            sprintf (buffer, "Win%ld,DM%ld,Min,Above", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_Y_MIN_BELOW:
            sprintf (buffer, "Win%ld,DM%ld,Min,Below", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_XY_MAX:
            sprintf (buffer, "Win%ld,DM%ld,Max", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_XY, NULL));
        break;
        case RSSPECAN_VAL_SPECM_MARK_XY_MIN:
            sprintf (buffer, "Win%ld,DM%ld,Min", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_SPEM_DELT_SEARCH_XY, NULL));
			break;
        case RSSPECAN_VAL_SPECM_MARK_Y_TRIGGER:
            sprintf (buffer, "Win%ld,DM%ld", win, deltaMarker);
            checkErr(rsspecan_SetAttributeViString(instrSession, buffer, RSSPECAN_ATTR_REFERENCE_MARKER_Y_TRIGGER, NULL));
    	break;
    }
    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Spectrogram Time Stamp
 * Purpose:  This function queries the time stamp of the frames.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSpectrogramTimeStamp (ViSession instrSession,
                                              ViInt32 frame, ViInt32 arraySize,
                                              ViInt32 part1[], ViInt32 part2[],
                                              ViInt32 part3[], ViInt32 part4[],
                                              ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32*     data = NULL;
    ViInt32     i, dataSize, j = 0;

    checkErr(RsCore_LockSession(instrSession));

    if (!RsCore_IsInstrumentModel(instrSession, "FSL") && !rsspecan_IsFSV (instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K14"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K160R"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, frame, 0, 1),
    		2, "Frame");

	snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, ":CALC:SPEC:TST:DATA? %s", (frame == 0) ? "CURR" : "ALL");
	checkErr(RsCore_QueryAsciiViInt32Array(instrSession, cmd, &data, &dataSize));
	checkErr(rsspecan_CheckStatus(instrSession));

	dataSize /= 4;

	if (returnedValues)
		*returnedValues = dataSize;

	if (dataSize > arraySize)
		dataSize = arraySize;

	for (i = 0; i < dataSize; i++)
	{
		part1[i] = data[j++];
		part2[i] = data[j++];
		part3[i] = data[j++];
		part4[i] = data[j++];
	}

Error:
	if (data) free(data);
    (void)RsCore_UnlockSession(instrSession);
    return error;
}


/*****************************************************************************
 * Function: Read Spectrogram Trace Data
 * Purpose:  This function ReadSpectrogramTraceData
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadSpectrogramTraceData (ViSession instrSession,
                                           ViInt32 arraySize, ViReal64 results[],
                                           ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE]="";

    checkErr(RsCore_LockSession(instrSession));

    if (!RsCore_IsInstrumentModel(instrSession, "FSL") && !rsspecan_IsFSV (instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K14"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K160R"));

    sprintf (buffer, "SPEC");
    checkErr(rsspecan_dataReadTrace (instrSession, 0, buffer, arraySize,
                    results, returnedValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Read Persistence Spectrum Trace Data
 * Purpose:  This function returns the results of the persistence spectrum
 *           result display. The function returns 801*600 percentages, one
 *           for each pixel in the histogram.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadPersistenceSpectrumTraceData (ViSession instrSession,
                                                             ViUInt32 timeout,
                                                             ViInt32 arraySize,
                                                             ViReal64 results[],
                                                             ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;
    ViChar      trace[RS_MAX_MESSAGE_BUF_SIZE]="";

    checkErr(RsCore_LockSession(instrSession));

    if (!RsCore_IsInstrumentModel(instrSession, "FSL") && !rsspecan_IsFSV (instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K14"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K160R"));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

    checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "TRAC? PSP", arraySize, results, returnedValues));
    checkErr(rsspecan_CheckStatus(instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Read Maxhold Trace Data
 * Purpose:  This function returns the results of the maxhold trace in the
 *           persistence spectrum result display. The function returns
 *           801*600 percentages, one for each pixel in the maxhold trace.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ReadMaxholdTraceData (ViSession instrSession,
                                                 ViUInt32 timeout,
                                                 ViInt32 arraySize,
                                                 ViReal64 results[],
                                                 ViInt32 *returnedValues)
{
    ViStatus    error   = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

	if (!RsCore_IsInstrumentModel(instrSession, "FSL") && !rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K14"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K160R"));

    viCheckParm(RsCore_InvalidViUInt32Range(instrSession, timeout, 0, 4294967295UL), 2, "Timeout");

	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, "TRAC? HMAX", arraySize, results, returnedValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Get Spectrogram X-Axis Scaling
 * Purpose:  This function queries the scaling of the x-axis in the
             spectrogram.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSpectrogramXAxisScaling (ViSession instrSession,
                                                       ViInt32 frame, ViInt32 arraySize,
                                                       ViReal64 xAxis[], ViInt32 *returnedValues)
{
    ViInt32     win = 1;
    ViStatus    error = VI_SUCCESS;
	ViChar      cmd[RS_MAX_MESSAGE_BUF_SIZE];

    checkErr(RsCore_LockSession(instrSession));

	if (!rsspecan_IsFSV(instrSession))
	{
		checkErr(RS_ERROR_INSTRUMENT_MODEL);
	}

	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K14"));
	checkErr(RsCore_CheckInstrumentOptions(instrSession, "K160R"));

    viCheckParm(RsCore_InvalidViInt32Range(instrSession, frame, RSSPECAN_VAL_SPECM_FRAME_CURRENT, RSSPECAN_VAL_SPECM_FRAME_ALL),
    		2, "Frame");
    viCheckParm(RsCore_InvalidViInt32Range(instrSession, arraySize, 1, INT_MAX),
    		3, "Array Size");
    viCheckParm(RsCore_InvalidNullPointer(instrSession, xAxis), 4, "X-Axis");

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    snprintf(cmd, RS_MAX_MESSAGE_BUF_SIZE, "CALC%ld:SPEC:X:DATA? %s", win, (frame == RSSPECAN_VAL_SPECM_FRAME_CURRENT) ? "CURR" : "ALL");
	checkErr(RsCore_QueryFloatArrayToUserBuffer(instrSession, cmd, arraySize, xAxis, returnedValues));

    checkErr(rsspecan_CheckStatus (instrSession));

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Store Spectrogram To File
 * Purpose:  This function stores the spectrogram data in ASCII format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_StoreSpectrogramToFile (ViSession instrSession,
                                                   ViString fileName)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SPEM_STORE_TO_FILE, fileName),
    		2, "File Name");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Store Spectrogram To File
 * Purpose:  This function stores the spectrogram data in ASCII format to a file.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramContinuousTrigger (ViSession instrSession,
                                                                  ViInt32 trigger)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_SPEM_CONTINUOUS_TRIGGER, trigger),
    		2, "Trigger");

Error:
     (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Time Domain Trigger
 * Purpose:  This function configures time domain trigger as source and level.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramTimeDomainTrigger (ViSession instrSession,
                                                                  ViReal64 timeDomainTriggerLevel)
{
    ViStatus    error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_SetAttributeViString(instrSession, "", RSSPECAN_ATTR_SPEM_TIME_DOMAIN_TRIGGER, NULL));

     viCheckParm(rsspecan_SetAttributeViReal64(instrSession, "", RSSPECAN_ATTR_SPEM_TIME_DOMAIN_TRIGGER_LEVEL, timeDomainTriggerLevel),
     		2, "Time Domain Trigger Level");

Error:
     (void)RsCore_UnlockSession(instrSession);
    return error;
}

/// HIFN  This function Selects a realtime measurement optimized either for high
/// HIFN  frequency resolution or providing additional evaluation in the time
/// HIFN  domain.
/// HIFN
/// HIFN     Note(s):
/// HIFN
/// HIFN     (1) This function is available only with R&S FSW.
/// HIFN
/// HIFN     Attribute(s):
/// HIFN     RSSPECAN_ATTR_REALTIME_MEASUREMENT
/// HIFN
/// HIFN     Remote-control command(s)
/// HIFN     CONFigure:REALtime:MEASurement
/// HIRET Returns the status code of this operation.
/// HIPAR instrSession/This control accepts the Instrument Handle returned by the Initialize
/// HIPAR instrSession/function to select the desired instrument driver session.
/// HIPAR realtimeMeasurement/This control selects a realtime measurement optimized either for high
/// HIPAR realtimeMeasurement/frequency resolution or providing additional evaluation in the time
/// HIPAR realtimeMeasurement/domain.
ViStatus _VI_FUNC rsspecan_SelectRealtimeSpectrumMeasurement(
	ViSession	instrSession,
	ViInt32	realtimeMeasurement
)
{
    ViStatus	error = VI_SUCCESS;

    checkErr(RsCore_LockSession(instrSession));

    viCheckParm(rsspecan_SetAttributeViInt32(instrSession, "", RSSPECAN_ATTR_REALTIME_MEASUREMENT, realtimeMeasurement),
    		2, "Realtime Measurement");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Result Display Mode
 * Purpose:  This function selects the result display in realtime mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramResultDisplayMode (ViSession instrSession,
                                                                  ViInt32 resultDisplay)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    checkErr(rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));

    sprintf (buffer, "Win%ld",win);
    switch (resultDisplay)
    {
        case RSSPECAN_VAL_PSPEM:
        case RSSPECAN_VAL_SPEM:
        case RSSPECAN_VAL_REAL_SPEM:
        case RSSPECAN_VAL_POWER_TIME:
        case RSSPECAN_VAL_PVT_WATER:

        viCheckParm(rsspecan_SetAttributeViInt32(instrSession, buffer, RSSPECAN_ATTR_RESULT_DISPLAY, resultDisplay),
        		2, "Result Display");
        break;
        default:
            viCheckParm(RsCore_InvalidViInt32Value(instrSession, resultDisplay), 2, "Result Display");
    }

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- Callback Definition   ---------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *- End Of RSSpecAn K14 - Spectrogram Measurement ---------------------------*
 *****************************************************************************/
