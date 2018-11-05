/*****************************************************************************
 *  Rohde&Schwarz Spectrum Analyzer instrument driver
 *  B27 - FM Demodulation Analog
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
/* Function: Configure FM Demodulation Analog  State
/* Purpose:  This function switches FM demodulation on or off.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogState(ViSession  instrSession,
                                                       ViInt32     window,
                                                       ViBoolean   state)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FM_STATE, state),
    		3, "State");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure FM Demodulation Analog  Lowpass Filter
/* Purpose:  This function configures the AF lowpass filters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogLowpassFilter(ViSession  instrSession,
                                                               ViInt32     window,
                                                               ViBoolean   AFLowpass,
                                                               ViReal64    frequency)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FM_LPAS_MAN, AFLowpass),
    		3, "Lowpass Auto");
    if (AFLowpass)
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FM_LPAS_FILTER, frequency),
        		4, "Frequency");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure FM Demodulation Analog  Highpass Filter
/* Purpose:  This function configures the AF highpass filters.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogHighpassFilter(ViSession instrSession,
                                                                ViInt32   window,
                                                                ViReal64  frequency)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FM_HPAS_FILTER, frequency),
    		3, "Frequency");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*===========================================================================*/
/* Function: Configure FM Demodulation Analog  Range
/* Purpose:  This function configures the AF demodulation range.
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogRange(ViSession  instrSession,
                                                       ViInt32     window,
                                                       ViBoolean   spanAutoMan,
                                                       ViReal64    span)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";

    checkErr(RsCore_LockSession(instrSession));

    sprintf (buffer, "Win%ld", window);

    viCheckParm(rsspecan_SetAttributeViBoolean(instrSession, buffer, RSSPECAN_ATTR_FM_SPAN_MAN, spanAutoMan),
    		3, "Span Auto/Manual");
    if (spanAutoMan)
        viCheckParm(rsspecan_SetAttributeViReal64(instrSession, buffer, RSSPECAN_ATTR_FM_SPAN, span),
        		4, "Span");

Error:
    (void)RsCore_UnlockSession(instrSession);
    return error;
}

/*****************************************************************************
 *- User defined callbacks --------------------------------------------------*
 *****************************************************************************/
