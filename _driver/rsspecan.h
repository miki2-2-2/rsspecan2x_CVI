/****************************************************************************
 *
 *  Rohde & Schwarz Spectrum Analyzer instrument driver include file
 *
 ****************************************************************************/

#ifndef __RSSPECAN_HEADER
#define __RSSPECAN_HEADER

#include "rscore.h"
#include "rsspecan_attributes.h"
#include "rsspecan_utility.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*****************************************************************************
 *- Useful Macros & Constants -----------------------------------------------*
 *****************************************************************************/
#define RSSPECAN_SIMULATION_ID_QUERY      "Rohde&Schwarz,FSQ-3,123456/789,1.23"  /* Default definition of ID Query Response for simulation */
#define RSSPECAN_VALID_ID_RESPONSE_STRING "Rohde&Schwarz"      /* Valid response for identification query */
#define RSSPECAN_SIMULATION_OPT_QUERY     ""      /* Simulated response for *OPT? command */

#define RSSPECAN_OPC_TIMEOUT              5000L   /* Maximum time to wait for OPC in milliseconds */

/****************************************************************************
 *----------------- Instrument Driver Revision Information -----------------*
 ****************************************************************************/

/* Class Driver Identification */

#define RSSPECAN_CLASS_DRIVER_DESCRIPTION           ""
#define RSSPECAN_CLASS_DRIVER_PREFIX                ""
#define RSSPECAN_CLASS_DRIVER_VENDOR                ""
#define RSSPECAN_CLASS_DRIVER_REVISION              ""
#define RSSPECAN_CLASS_SPEC_MAJOR_VERSION           2L    /* Class specification major version */
#define RSSPECAN_CLASS_SPEC_MINOR_VERSION           0L    /* Class specification minor version */

/* Driver Identification */

#define RSSPECAN_SPECIFIC_DRIVER_DESCRIPTION        ""
#define RSSPECAN_SPECIFIC_DRIVER_PREFIX             "RSSPECAN"
#define RSSPECAN_SPECIFIC_DRIVER_LOCATOR            ""
#define RSSPECAN_SPECIFIC_DRIVER_VENDOR             ""
#define RSSPECAN_MAJOR_VERSION                      2L    /* Instrument driver major version          */
#define RSSPECAN_MINOR_VERSION                      8L   /* Instrument driver minor version          */
#define RSSPECAN_MINOR_MINOR_VERSION                2L    /* Instrument driver minor minor version    */

/* Driver Capabilities */

#define RSSPECAN_SUPPORTED_INSTRUMENT_MODELS        ""    /* Instrument driver supported model(s)     */
#define RSSPECAN_GROUP_CAPABILITIES                 ""
#define RSSPECAN_FUNCTION_CAPABILITIES              ""

/****************************************************************************
 *---------------------------- Attribute Defines ---------------------------*
 ****************************************************************************/

    /*- Inherent Instrument Specific Attributes ----------------------------*/

        /* User Options */

#define RSSPECAN_ATTR_RANGE_CHECK                                           RS_ATTR_RANGE_CHECK                                     /* ViBoolean */
#define RSSPECAN_ATTR_QUERY_INSTRUMENT_STATUS                               RS_ATTR_QUERY_INSTRUMENT_STATUS                         /* ViBoolean */
#define RSSPECAN_ATTR_CACHE                                                 RS_ATTR_CACHE                                           /* ViBoolean */
#define RSSPECAN_ATTR_SIMULATE                                              RS_ATTR_SIMULATE                                        /* ViBoolean */
#define RSSPECAN_ATTR_RECORD_COERCIONS                                      RS_ATTR_RECORD_COERCIONS                                /* ViBoolean */
#define RSSPECAN_ATTR_INTERCHANGE_CHECK                                     RS_ATTR_INTERCHANGE_CHECK                               /* ViBoolean */
#define RSSPECAN_ATTR_SPY                                                   RS_ATTR_SPY                                             /* ViBoolean */
#define RSSPECAN_ATTR_USE_SPECIFIC_SIMULATION                               RS_ATTR_USE_SPECIFIC_SIMULATION                         /* ViBoolean */

        /* Class Driver Identification */

#define RSSPECAN_ATTR_CLASS_DRIVER_DESCRIPTION                              RS_ATTR_CLASS_DRIVER_DESCRIPTION                        /* ViString, read-only */
#define RSSPECAN_ATTR_CLASS_DRIVER_PREFIX                                   RS_ATTR_CLASS_DRIVER_PREFIX                             /* ViString, read-only */
#define RSSPECAN_ATTR_CLASS_DRIVER_VENDOR                                   RS_ATTR_CLASS_DRIVER_VENDOR                             /* ViString, read-only */
#define RSSPECAN_ATTR_CLASS_DRIVER_REVISION                                 RS_ATTR_CLASS_DRIVER_REVISION                           /* ViString, read-only */
#define RSSPECAN_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION                 RS_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION           /* ViInt32,  read-only */
#define RSSPECAN_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION                 RS_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION           /* ViInt32,  read-only */

        /* Driver Identification */

#define RSSPECAN_ATTR_SPECIFIC_DRIVER_DESCRIPTION                           RS_ATTR_SPECIFIC_DRIVER_DESCRIPTION                     /* ViString, read-only */
#define RSSPECAN_ATTR_SPECIFIC_DRIVER_PREFIX                                RS_ATTR_SPECIFIC_DRIVER_PREFIX                          /* ViString, read-only */
#define RSSPECAN_ATTR_SPECIFIC_DRIVER_LOCATOR                               RS_ATTR_SPECIFIC_DRIVER_LOCATOR                         /* ViString, read-only */
#define RSSPECAN_ATTR_SPECIFIC_DRIVER_VENDOR                                RS_ATTR_SPECIFIC_DRIVER_VENDOR                          /* ViString, read-only */
#define RSSPECAN_ATTR_SPECIFIC_DRIVER_REVISION                              RS_ATTR_SPECIFIC_DRIVER_REVISION                        /* ViString, read-only */
#define RSSPECAN_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION              RS_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION        /* ViInt32,  read-only */
#define RSSPECAN_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION              RS_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION        /* ViInt32,  read-only */

        /* Driver Capabilities */

#define RSSPECAN_ATTR_SUPPORTED_INSTRUMENT_MODELS                           RS_ATTR_SUPPORTED_INSTRUMENT_MODELS                     /* ViString, read-only */
#define RSSPECAN_ATTR_GROUP_CAPABILITIES                                    RS_ATTR_GROUP_CAPABILITIES                              /* ViString, read-only */
#define RSSPECAN_ATTR_FUNCTION_CAPABILITIES                                 RS_ATTR_FUNCTION_CAPABILITIES                           /* ViString, read-only */
#define RSSPECAN_ATTR_CHANNEL_COUNT                                         RS_ATTR_CHANNEL_COUNT                                   /* ViInt32,  read-only */

        /* Driver Setup */

#define RSSPECAN_ATTR_DRIVER_SETUP                                          RS_ATTR_DRIVER_SETUP                                    /* ViString            */

        /* Instrument Identification */

#define RSSPECAN_ATTR_INSTRUMENT_MANUFACTURER                               RS_ATTR_INSTRUMENT_MANUFACTURER                         /* ViString, read-only */
#define RSSPECAN_ATTR_INSTRUMENT_MODEL                                      RS_ATTR_INSTRUMENT_MODEL                                /* ViString, read-only */
#define RSSPECAN_ATTR_INSTRUMENT_FIRMWARE_REVISION                          RS_ATTR_INSTRUMENT_FIRMWARE_REVISION                    /* ViString, read-only */

        /* Advanced Session Information */

#define RSSPECAN_ATTR_IO_RESOURCE_DESCRIPTOR                                RS_ATTR_IO_RESOURCE_DESCRIPTOR                          /* ViString, read-only */
#define RSSPECAN_ATTR_LOGICAL_NAME                                          RS_ATTR_LOGICAL_NAME

/* ViString, read-only */

    /*- Instrument-Specific Attributes -------------------------------------*/


/* BEGIN GENERATE */
#define RSSPECAN_ATTR_AMPLITUDE_UNITS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1
#define RSSPECAN_ATTR_ATTENUATION                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2
#define RSSPECAN_ATTR_ATTENUATION_AUTO                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3
#define RSSPECAN_ATTR_ATTENUATION_MODE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 4
#define RSSPECAN_ATTR_FREQUENCY_START                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 6
#define RSSPECAN_ATTR_FREQUENCY_STOP                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 7
#define RSSPECAN_ATTR_FREQUENCY_OFFSET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 8
#define RSSPECAN_ATTR_FREQUENCY_CENTER                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 9
#define RSSPECAN_ATTR_FREQUENCY_CENTER_STEP                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 10
#define RSSPECAN_ATTR_FREQUENCY_STEP_AUTO                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 11
#define RSSPECAN_ATTR_FREQUENCY_CENTER_LINK                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 12
#define RSSPECAN_ATTR_FREQUENCY_CENTER_LINK_FACTOR                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 13
#define RSSPECAN_ATTR_FREQUENCY_SPAN                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 14
#define RSSPECAN_ATTR_FREQUENCY_SPAN_FULL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 15
#define RSSPECAN_ATTR_FREQUENCY_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 17
#define RSSPECAN_ATTR_INPUT_IMPEDANCE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 20
#define RSSPECAN_ATTR_NUMBER_OF_SWEEPS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 21
#define RSSPECAN_ATTR_REFERENCE_LEVEL                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 22
#define RSSPECAN_ATTR_REFERENCE_LEVEL_OFFSET                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 23
#define RSSPECAN_ATTR_RESOLUTION_BANDWIDTH                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 24
#define RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_AUTO                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 25
#define RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_RATIO                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 26
#define RSSPECAN_ATTR_SWEEP_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 27
#define RSSPECAN_ATTR_SWEEP_MODE_CONTINUOUS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 28
#define RSSPECAN_ATTR_SWEEP_TIME                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 29
#define RSSPECAN_ATTR_SWEEP_TIME_AUTO                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 30
#define RSSPECAN_ATTR_SWEEP_POINTS                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 31
#define RSSPECAN_ATTR_SWEEP_OPTIMIZATION                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2859
#define RSSPECAN_ATTR_SWEEP_TYPE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 32
#define RSSPECAN_ATTR_SWEEP_COUNT_CURRENT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 33
#define RSSPECAN_ATTR_VERTICAL_SCALE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 34
#define RSSPECAN_ATTR_VIDEO_BANDWIDTH                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 35
#define RSSPECAN_ATTR_VIDEO_BANDWIDTH_AUTO                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 36
#define RSSPECAN_ATTR_VIDEO_BANDWIDTH_RATIO                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 37
#define RSSPECAN_ATTR_FFT_FILTER_MODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 38
#define RSSPECAN_ATTR_PARAMETER_COUPLING                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 39
#define RSSPECAN_ATTR_CORR_YIG                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 40
#define RSSPECAN_ATTR_Y_RESULT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 41
#define RSSPECAN_ATTR_HORIZONTAL_SCALE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 42
#define RSSPECAN_ATTR_IF_OUTPUT_SOURCE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 43
#define RSSPECAN_ATTR_IF_OUTPUT_FREQUENCY                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2672
#define RSSPECAN_ATTR_NOISE_SOURCE_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2673
#define RSSPECAN_ATTR_SAN_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 44
#define RSSPECAN_ATTR_GET_INSTR_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 45
#define RSSPECAN_ATTR_PRESET_MEASUREMENT_CHANNEL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2860
#define RSSPECAN_ATTR_SELECT_MEASUREMENT_CHANNEL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2861
#define RSSPECAN_ATTR_DUPLICATE_MEASUREMENT_CHANNEL                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2997
#define RSSPECAN_ATTR_DELETE_MEASUREMENT_CHANNEL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2890
#define RSSPECAN_ATTR_MEAS_ABORT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 47
#define RSSPECAN_ATTR_INIT                                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 48
#define RSSPECAN_ATTR_INIT_WITHOUT_OPC                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 49
#define RSSPECAN_ATTR_INIT_CONMEAS                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 50
#define RSSPECAN_ATTR_INIT_SW_TRIGGER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 52
#define RSSPECAN_ATTR_INIT_SPURIOUS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 53
#define RSSPECAN_ATTR_INIT_SEM_MEAS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 56
#define RSSPECAN_ATTR_UPDATE_IQ_RESULTS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 57
#define RSSPECAN_ATTR_INIT_SEQUENCER                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2893
#define RSSPECAN_ATTR_ABORT_SEQUENCER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2894
#define RSSPECAN_ATTR_REFRESH_SEQUENCER                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2895
#define RSSPECAN_ATTR_SEQUENCER_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2896
#define RSSPECAN_ATTR_SEQUENCER_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2897
#define RSSPECAN_ATTR_AVG_STATE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 58
#define RSSPECAN_ATTR_AVG_TYPE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 59
#define RSSPECAN_ATTR_AVG_COUNT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 60
#define RSSPECAN_ATTR_RESOLUTION_BANDWIDTH_FILTER_TYPE                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 61
#define RSSPECAN_ATTR_VIDEO_BANDWIDTH_TYPE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 62
#define RSSPECAN_ATTR_PLL_BANDWIDTH                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 63
#define RSSPECAN_ATTR_TFAC_GEN_NAME                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 64
#define RSSPECAN_ATTR_TFAC_SEL_NAME                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 65
#define RSSPECAN_ATTR_TFAC_UNIT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 66
#define RSSPECAN_ATTR_TFAC_SCALING                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 67
#define RSSPECAN_ATTR_TFAC_COMMENT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 68
#define RSSPECAN_ATTR_TFAC_STATE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 69
#define RSSPECAN_ATTR_TFAC_DELETE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 70
#define RSSPECAN_ATTR_TFAC_DISPLAY                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 71
#define RSSPECAN_ATTR_TFAC_ADJ_STATE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 72
#define RSSPECAN_ATTR_TFAC_SET_COMMENT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 73
#define RSSPECAN_ATTR_TFAC_ACT_NAME                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 74
#define RSSPECAN_ATTR_TSET_SELECT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3098
#define RSSPECAN_ATTR_TSET_STATE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3099
#define RSSPECAN_ATTR_TSET_COMMENT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3100
#define RSSPECAN_ATTR_TSET_UNIT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3101
#define RSSPECAN_ATTR_TSET_BREAK                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3102
#define RSSPECAN_ATTR_TSET_DELETE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3103
#define RSSPECAN_ATTR_TSET_ACTIVE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3104
#define RSSPECAN_ATTR_CVL_NAME                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 75
#define RSSPECAN_ATTR_CVL_MIXER                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 76
#define RSSPECAN_ATTR_CVL_MIXER_SNUM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 77
#define RSSPECAN_ATTR_CVL_BAND                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 78
#define RSSPECAN_ATTR_CVL_HARM                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 79
#define RSSPECAN_ATTR_CVL_PORT_MIXER                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 80
#define RSSPECAN_ATTR_CVL_BIAS                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 81
#define RSSPECAN_ATTR_CVL_COMMENT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 82
#define RSSPECAN_ATTR_CVL_CLEAR                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 83
#define RSSPECAN_ATTR_TRACE_STATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 84
#define RSSPECAN_ATTR_TRACE_RESET_BEHAVIOR                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 85
#define RSSPECAN_ATTR_TRACE_TYPE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 86
#define RSSPECAN_ATTR_TRACE_MATH_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 88
#define RSSPECAN_ATTR_TRACE_MATH_POSITION                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 89
#define RSSPECAN_ATTR_TRACE_MATH_MODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 90
#define RSSPECAN_ATTR_DETECTOR_TYPE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 91
#define RSSPECAN_ATTR_DETECTOR_TYPE_AUTO                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 92
#define RSSPECAN_ATTR_DISPLAY_TRACE_CLEAR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2620
#define RSSPECAN_ATTR_TRACE_EXPORT_ALL                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3053
#define RSSPECAN_ATTR_CALIBRATION                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 95
#define RSSPECAN_ATTR_CALIBRATION_ABORT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 96
#define RSSPECAN_ATTR_CALIBRATION_RESULT_QUERY                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 97
#define RSSPECAN_ATTR_CALIBRATION_STATE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 98
#define RSSPECAN_ATTR_CAL_PROBE_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 99
#define RSSPECAN_ATTR_CAL_PROBE_DATA_SELECT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 100
#define RSSPECAN_ATTR_CAL_PROBE_DC_VOLTAGE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 101
#define RSSPECAN_ATTR_CAL_PROBE_PULSE_FREQ                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 102
#define RSSPECAN_ATTR_CAL_PROBE_ADJUST_MODE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 103
#define RSSPECAN_ATTR_CAL_PROBE_CHECK                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 104
#define RSSPECAN_ATTR_CAL_PROBE_DELETE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 105
#define RSSPECAN_ATTR_CAL_PROBE_MAN_COMPENSATION                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 106
#define RSSPECAN_ATTR_CAL_PROBE_FREQ_RESP_COMPENSATION                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 107
#define RSSPECAN_ATTR_CAL_PROBE_GAIN_ERROR_COMPENSATION                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 108
#define RSSPECAN_ATTR_CAL_PROBE_START                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 109
#define RSSPECAN_ATTR_PROBE_ACTIVATION                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2764
#define RSSPECAN_ATTR_PROBE_CONNECTOR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3072
#define RSSPECAN_ATTR_PROBE_MICRO_BUTTON_ACTION                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2765
#define RSSPECAN_ATTR_PROBE_CONNECTION_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2766
#define RSSPECAN_ATTR_PROBE_NAME                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2767
#define RSSPECAN_ATTR_PROBE_NUMBER                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2768
#define RSSPECAN_ATTR_PROBE_TYPE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3073
#define RSSPECAN_ATTR_INP_UPORT_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 110
#define RSSPECAN_ATTR_INP_UPORT_VALUE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 111
#define RSSPECAN_ATTR_OUT_UPORT_VALUE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 112
#define RSSPECAN_ATTR_AUTO_ADJUST_ALL                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 113
#define RSSPECAN_ATTR_AUTO_ADJUST_FREQ                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 114
#define RSSPECAN_ATTR_AUTO_ADJUST_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 115
#define RSSPECAN_ATTR_AUTO_ADJUST_LEVEL_MEAS_TIME                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 116
#define RSSPECAN_ATTR_AUTO_ADJUST_LEVEL_MEAS_TIME_MODE                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 117
#define RSSPECAN_ATTR_AUTO_ADJUST_HYSTERSIS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2769
#define RSSPECAN_ATTR_AUTO_ADJUST_TRIGGER_STATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2770
#define RSSPECAN_ATTR_AUTO_SCALE_SINGLE_SCREEN                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 118
#define RSSPECAN_ATTR_AUTO_SCALE_ALL_SCREENS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 119
#define RSSPECAN_ATTR_SPECTRUM_DISPLAY_CREATE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2358
#define RSSPECAN_ATTR_SPECTRUM_DISPLAY_SELECT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2359
#define RSSPECAN_ATTR_SPECTRUM_DISPLAY_DELETE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2360
#define RSSPECAN_ATTR_MARKER_AMPLITUDE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 120
#define RSSPECAN_ATTR_MARKER_ENABLED                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 121
#define RSSPECAN_ATTR_MARKER_POSITION                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 122
#define RSSPECAN_ATTR_MARKER_TRACE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 123
#define RSSPECAN_ATTR_ASSIGN_MARKER_TO_TRACE_RAISING_FALLING                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2898
#define RSSPECAN_ATTR_MARKER_AOFF                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 124
#define RSSPECAN_ATTR_MARKER_LOEX                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 125
#define RSSPECAN_ATTR_SIGNAL_TRACK_ENABLED                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 126
#define RSSPECAN_ATTR_SIGNAL_TRACK_BWID                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 127
#define RSSPECAN_ATTR_SIGNAL_TRACK_THRESHOLD                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 128
#define RSSPECAN_ATTR_SIGNAL_TRACK_TRACE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 129
#define RSSPECAN_ATTR_MARKER_STEP_SIZE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 130
#define RSSPECAN_ATTR_MARKER_PROBABILITY                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 131
#define RSSPECAN_ATTR_MARKER_SEARCH_MIN_RIGHT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 134
#define RSSPECAN_ATTR_MARKER_SEARCH_MIN_LEFT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 135
#define RSSPECAN_ATTR_MARKER_SEARCH_PEAK                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 136
#define RSSPECAN_ATTR_MARKER_SEARCH_PEAK_NEXT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 137
#define RSSPECAN_ATTR_MARKER_SEARCH_PEAK_RIGHT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 138
#define RSSPECAN_ATTR_MARKER_SEARCH_PEAK_LEFT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 139
#define RSSPECAN_ATTR_MARKER_SEARCH_MIN                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 140
#define RSSPECAN_ATTR_MARKER_SEARCH_MIN_NEXT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 141
#define RSSPECAN_ATTR_MARKER_SEARCH_PEAK_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 142
#define RSSPECAN_ATTR_MARKER_SEARCH_MIN_AUTO                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 143
#define RSSPECAN_ATTR_MARKER_THRESHOLD_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 144
#define RSSPECAN_ATTR_MARKER_THRESHOLD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 145
#define RSSPECAN_ATTR_PEAK_EXCURSION                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 146
#define RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_STATE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 147
#define RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_LEFT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 148
#define RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_RIGHT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 149
#define RSSPECAN_ATTR_MARKER_SEARCH_LIMITS_ZOOM                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 150
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_COUNT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 151
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_FOUND                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 152
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_SORT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 153
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_SIZE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2621
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_AUTO                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2622
#define RSSPECAN_ATTR_MARKER_ZOOM                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 154
#define RSSPECAN_ATTR_MARKER_DEMOD_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 159
#define RSSPECAN_ATTR_MARKER_DEMOD_TYPE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 160
#define RSSPECAN_ATTR_MARKER_DEMOD_HOLDOFF                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 161
#define RSSPECAN_ATTR_MARKER_DEMOD_CONT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 162
#define RSSPECAN_ATTR_MARKER_DEMOD_SQUELCH_STATE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 163
#define RSSPECAN_ATTR_MARKER_DEMOD_SQUELCH_LEVEL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 164
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_LABEL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2623
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_MARKER                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2624
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_ANNOTATION_STATE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2625
#define RSSPECAN_ATTR_MARKER_TO_CENTER                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 165
#define RSSPECAN_ATTR_MARKER_TO_STEP                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 166
#define RSSPECAN_ATTR_MARKER_TO_REFERENCE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 167
#define RSSPECAN_ATTR_MARKER_LINK                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 169
#define RSSPECAN_ATTR_MARKER_LINK_TO_MARKER                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2862
#define RSSPECAN_ATTR_MARKER_LINK_ALL_MARKERS                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2899
#define RSSPECAN_ATTR_MARKER_REF_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 170
#define RSSPECAN_ATTR_MARKER_REF_LEVEL                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 171
#define RSSPECAN_ATTR_MARKER_REF_LEVEL_OFFSET                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 172
#define RSSPECAN_ATTR_MARKER_REF_FREQ                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 173
#define RSSPECAN_ATTR_MARKER_REF_PEAK                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 174
#define RSSPECAN_ATTR_MARKER_STORE_PEAK_LIST                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 175
#define RSSPECAN_ATTR_MARKER_FILE_EXPORT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 176
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2579
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_PEAK_LABELS                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2580
#define RSSPECAN_ATTR_MARKER_PEAK_LIST_MAXIMUM_PEAKS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2581
#define RSSPECAN_ATTR_MARKER_BAND_POWER_STATE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2361
#define RSSPECAN_ATTR_MARKER_BAND_POWER_MODE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2362
#define RSSPECAN_ATTR_MARKER_BAND_POWER_SPAN                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2363
#define RSSPECAN_ATTR_QUERY_MARKER_BAND_POWER_RESULT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2364
#define RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_STATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2365
#define RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_MODE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2366
#define RSSPECAN_ATTR_DELTA_MARKER_BAND_POWER_SPAN                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2367
#define RSSPECAN_ATTR_QUERY_DELTA_MARKER_BAND_POWER_RESULT                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2368
#define RSSPECAN_ATTR_REFERENCE_MARKER_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 178
#define RSSPECAN_ATTR_REFERENCE_MARKER_MODE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 179
#define RSSPECAN_ATTR_REFERENCE_MARKER_POSITION                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 180
#define RSSPECAN_ATTR_REFERENCE_MARKER_REL_POSITION                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 181
#define RSSPECAN_ATTR_REFERENCE_MARKER_AMPLITUDE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 182
#define RSSPECAN_ATTR_REFERENCE_MARKER_TRACE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 183
#define RSSPECAN_ATTR_DELTA_MARKER_REFERENCE_MARKER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2369
#define RSSPECAN_ATTR_DELTA_MARKER_LINK_TO_MARKER                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2863
#define RSSPECAN_ATTR_REFERENCE_MARKER_AOFF                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 184
#define RSSPECAN_ATTR_REFERENCE_MARKER_PEAK                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 185
#define RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 186
#define RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_RIGHT                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 187
#define RSSPECAN_ATTR_REFERENCE_MARKER_PEAK_NEXT_LEFT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 188
#define RSSPECAN_ATTR_REFERENCE_MARKER_MIN                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 189
#define RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 190
#define RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_RIGHT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 191
#define RSSPECAN_ATTR_REFERENCE_MARKER_MIN_NEXT_LEFT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 192
#define RSSPECAN_ATTR_REFERENCE_MARKER_Y_TRIGGER                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2556
#define RSSPECAN_ATTR_SYST_ERR                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 193
#define RSSPECAN_ATTR_SYST_ERR_LIST                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 194
#define RSSPECAN_ATTR_SYST_ERR_CLEAR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 195
#define RSSPECAN_ATTR_SYST_PRESET                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 196
#define RSSPECAN_ATTR_PRESET_OPERATING_MODE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2900
#define RSSPECAN_ATTR_SYST_SPEAKER                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 197
#define RSSPECAN_ATTR_SYST_VERSION                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 203
#define RSSPECAN_ATTR_APPLICATIONS_VERSIONS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2706
#define RSSPECAN_ATTR_SYST_ID_FACTORY                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 204
#define RSSPECAN_ATTR_SYST_ID_FACTORY_QUERY                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 205
#define RSSPECAN_ATTR_APPL_SETUP_REC                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 206
#define RSSPECAN_ATTR_IF_SHIFT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 207
#define RSSPECAN_ATTR_IF_SHIFT_FREQ                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 208
#define RSSPECAN_ATTR_REMOTE_LOGGING_STATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2771
#define RSSPECAN_ATTR_SYSTEM_SHUTDOWN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2370
#define RSSPECAN_ATTR_LOCK_LOCAL_KEY                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2557
#define RSSPECAN_ATTR_FRONTEND_TEMPERATURE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2948
#define RSSPECAN_ATTR_ROSC_SOURCE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 211
#define RSSPECAN_ATTR_ROSC_SOURCE_EAUTO                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 212
#define RSSPECAN_ATTR_ROSC_FREQUENCY                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 213
#define RSSPECAN_ATTR_ROSC_PLL_BWID                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 214
#define RSSPECAN_ATTR_TRIGGER_SOURCE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 215
#define RSSPECAN_ATTR_TRIGGER_DELAY                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 216
#define RSSPECAN_ATTR_TRIGGER_SLOPE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 217
#define RSSPECAN_ATTR_TRIGGER_DROPOUT_TIME                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2674
#define RSSPECAN_ATTR_TRIGGER_MODE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2472
#define RSSPECAN_ATTR_TRIGGER_IQP_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2864
#define RSSPECAN_ATTR_TRIGGER_RFP_HOLDOFF                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2865
#define RSSPECAN_ATTR_TRIGGER_DIRECTION                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2675
#define RSSPECAN_ATTR_TRIGGER_OUTPUT_TYPE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2676
#define RSSPECAN_ATTR_TRIGGER_LEVEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2677
#define RSSPECAN_ATTR_TRIGGER_PULSE_LENGTH                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2678
#define RSSPECAN_ATTR_TRIGGER_PULSE_GENERATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2679
#define RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 218
#define RSSPECAN_ATTR_EXTERNAL_TRIGGER_LEVEL_PORT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2680
#define RSSPECAN_ATTR_EXTERNAL_TRIGGER_DELAY_COMPENSATION                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 219
#define RSSPECAN_ATTR_EXTERNAL_GATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 220
#define RSSPECAN_ATTR_EXTERNAL_GATE_HOLD                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 221
#define RSSPECAN_ATTR_EXTERNAL_GATE_TRIGGER_TYPE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 222
#define RSSPECAN_ATTR_EXTERNAL_GATE_POLARITY                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 223
#define RSSPECAN_ATTR_EXTERNAL_GATE_LENGTH                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 224
#define RSSPECAN_ATTR_EXTERNAL_GATE_SIGNAL_SOURCE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 225
#define RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_STATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 226
#define RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_START                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 227
#define RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_STOP                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 228
#define RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_PERIOD_LENGTH                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 229
#define RSSPECAN_ATTR_EXTERNAL_GATE_TRACE_COMMENT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 230
#define RSSPECAN_ATTR_VIDEO_TRIGGER_LEVEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 231
#define RSSPECAN_ATTR_TRIGGER_IFP_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 232
#define RSSPECAN_ATTR_TRIGGER_IFP_OFFSET                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 233
#define RSSPECAN_ATTR_TRIGGER_IFP_HYSTERESIS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 234
#define RSSPECAN_ATTR_TRIGGER_TIME_INTERVAL                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 235
#define RSSPECAN_ATTR_FREQUENCY_MASK_WINDOW                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2949
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_SOURCE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2950
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_NAME                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2951
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_COMMENT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2952
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_DIRECTORY                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2953
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_DELETE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2954
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGG_CONDITION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2955
#define RSSPECAN_ATTR_FREQUENCY_MASK_TRIGGER_MODE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2956
#define RSSPECAN_ATTR_FREQUENCY_MASK_UPP_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2957
#define RSSPECAN_ATTR_FREQUENCY_MASK_LOWER_STATE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2958
#define RSSPECAN_ATTR_FREQUENCY_MASK_UPPER_STATE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2959
#define RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_LOW_X                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2960
#define RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_LOW_Y                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2961
#define RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_UPP_X                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2962
#define RSSPECAN_ATTR_FREQUENCY_MASK_SHIFT_UPP_Y                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2963
#define RSSPECAN_ATTR_FREQUENCY_MASK_SPAN                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2964
#define RSSPECAN_ATTR_EXTERNAL_MIXER_SIGNAL_ID_MODE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 236
#define RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_LOW                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 237
#define RSSPECAN_ATTR_EXTERNAL_MIXER_LO_POWER                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 238
#define RSSPECAN_ATTR_EXTERNAL_MIXER_ENABLED                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 239
#define RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 240
#define RSSPECAN_ATTR_EXTERNAL_MIXER_NUMBER_OF_PORTS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 241
#define RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC_TYPE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 242
#define RSSPECAN_ATTR_EXTERNAL_MIXER_HARMONIC_BAND                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 243
#define RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_HIGH                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 244
#define RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_TABLE_HIGH             RS_SPECIFIC_PUBLIC_ATTR_BASE + 245
#define RSSPECAN_ATTR_EXTERNAL_MIXER_CONVERSION_LOSS_TABLE_LOW              RS_SPECIFIC_PUBLIC_ATTR_BASE + 246
#define RSSPECAN_ATTR_EXTERNAL_MIXER_BIAS_LOW                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 247
#define RSSPECAN_ATTR_EXTERNAL_MIXER_BIAS_HIGH                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 248
#define RSSPECAN_ATTR_EXTERNAL_MIXER_THRESHOLD                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 249
#define RSSPECAN_ATTR_GET_EXTERNAL_MIXER_RF_FREQUENCY_START                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 250
#define RSSPECAN_ATTR_GET_EXTERNAL_MIXER_RF_FREQUENCY_STOP                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 251
#define RSSPECAN_ATTR_EXTERNAL_MIXER_HANDOVER_FREQUENCY                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 252
#define RSSPECAN_ATTR_EXTERNAL_MIXER_RF_OVERRANGE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 253
#define RSSPECAN_ATTR_EXTERNAL_MIXER_PRESET_BAND                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 254
#define RSSPECAN_ATTR_EXTERNAL_MIXER_HIGH_HARMONIC_STATE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 255
#define RSSPECAN_ATTR_EXTERNAL_MIXER_HIGH_HARMONIC                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 256
#define RSSPECAN_ATTR_AMPL_RF_COUPLING                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 257
#define RSSPECAN_ATTR_AMPL_EATT_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 258
#define RSSPECAN_ATTR_AMPL_EATT_AUTO                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 259
#define RSSPECAN_ATTR_AMPL_EATT_VALUE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 260
#define RSSPECAN_ATTR_AMPL_MIXER_LEVEL                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 261
#define RSSPECAN_ATTR_AMPL_MIXER_AUTO                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 262
#define RSSPECAN_ATTR_NOISE_INP_PRES_STAT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1498
#define RSSPECAN_ATTR_AMPL_PREAMPLIFIER                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 263
#define RSSPECAN_ATTR_AMPL_PREAMPLIFIER_LEVEL                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2681
#define RSSPECAN_ATTR_AMPL_PROTECTION_RESET                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 264
#define RSSPECAN_ATTR_AMPL_YIG_FILTER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 265
#define RSSPECAN_ATTR_AMPL_HIGHPASS_FILTER                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2682
#define RSSPECAN_LIMIT_STATE                                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 266
#define RSSPECAN_LIMIT_CLEAR                                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 267
#define RSSPECAN_LIMIT_ASSIGN_TRACE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 268
#define RSSPECAN_LIMIT_UNITS                                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 269
#define RSSPECAN_LIMIT_COMMENT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 270
#define RSSPECAN_LIMIT_COPY                                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 271
#define RSSPECAN_LIMIT_NAME                                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 272
#define RSSPECAN_LIMIT_DELETE                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 273
#define RSSPECAN_ATTR_LIMIT_LINE_ACTIVE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2793
#define RSSPECAN_LIMIT_CONTROL_DOMAIN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 277
#define RSSPECAN_LIMIT_CONTROL_SPACING                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 278
#define RSSPECAN_LIMIT_CONTROL_SCALING                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 279
#define RSSPECAN_LIMIT_CONTROL_OFFSET                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 280
#define RSSPECAN_LIMIT_CONTROL_SHIFT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 281
#define RSSPECAN_LIMIT_LOWER_STATE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 282
#define RSSPECAN_LIMIT_LOWER_SCALING                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 283
#define RSSPECAN_LIMIT_LOWER_SPACING                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 284
#define RSSPECAN_LIMIT_LOWER_MARGIN                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 285
#define RSSPECAN_LIMIT_LOWER_THRESHOLD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 286
#define RSSPECAN_LIMIT_LOWER_OFFSET                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 287
#define RSSPECAN_LIMIT_LOWER_SHIFT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 288
#define RSSPECAN_LIMIT_UPPER_STATE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 289
#define RSSPECAN_LIMIT_UPPER_SCALING                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 290
#define RSSPECAN_LIMIT_UPPER_SPACING                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 291
#define RSSPECAN_LIMIT_UPPER_MARGIN                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 292
#define RSSPECAN_LIMIT_UPPER_THRESHOLD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 293
#define RSSPECAN_LIMIT_UPPER_OFFSET                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 294
#define RSSPECAN_LIMIT_UPPER_SHIFT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 295
#define RSSPECAN_ATTR_THRLINE_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 296
#define RSSPECAN_ATTR_THRLINE_POSITION                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 297
#define RSSPECAN_ATTR_DLINE_STATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 298
#define RSSPECAN_ATTR_DLINE_POSITION                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 299
#define RSSPECAN_ATTR_FLINE_STATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 300
#define RSSPECAN_ATTR_FLINE_POSITION                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 301
#define RSSPECAN_ATTR_TLINE_STATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 302
#define RSSPECAN_ATTR_TLINE_POSITION                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 303
#define RSSPECAN_ATTR_DISP_REF_VALUE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 304
#define RSSPECAN_ATTR_DISP_REF_POSITION                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 305
#define RSSPECAN_ATTR_DISP_FP_KEYS                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 306
#define RSSPECAN_ATTR_SELECT_WINDOW                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 307
#define RSSPECAN_ATTR_DISP_FREQ_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 308
#define RSSPECAN_ATTR_DISP_LOGO_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 309
#define RSSPECAN_ATTR_DISP_TIME_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 310
#define RSSPECAN_ATTR_DISP_COMMENT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 311
#define RSSPECAN_ATTR_DISP_COMMENT_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 312
#define RSSPECAN_ATTR_DISP_PWR_SAVE_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 313
#define RSSPECAN_ATTR_DISP_PWR_SAVE_HOLDOFF                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 314
#define RSSPECAN_ATTR_DISP_FORMAT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 315
#define RSSPECAN_ATTR_DISP_WINDOW_SIZE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 316
#define RSSPECAN_ATTR_DISP_COL_PRESET                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 317
#define RSSPECAN_ATTR_DISP_COL_PREDEFINED                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 320
#define RSSPECAN_ATTR_DISP_LOG_RANGE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 321
#define RSSPECAN_ATTR_DISP_AMPLITUDE_GRID_MODE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 322
#define RSSPECAN_ATTR_DISP_SINGLE_SWEEP                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 325
#define RSSPECAN_ATTR_DISP_UPDATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 326
#define RSSPECAN_ATTR_QUERY_ACTIVE_WINDOW                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 328
#define RSSPECAN_ATTR_QUERY_SELECT_SUBWINDOW                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2371
#define RSSPECAN_ATTR_DISPLAY_FOCUS_AREA                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2372
#define RSSPECAN_ATTR_UNIT_POWER                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 329
#define RSSPECAN_ATTR_DISP_REF_VALUE_AUTO                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 331
#define RSSPECAN_ATTR_DISP_TRACE_Y_AUTO                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 332
#define RSSPECAN_ATTR_DISP_Y_AXIS_SCALING                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 334
#define RSSPECAN_ATTR_DISP_STATUS_BAR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 335
#define RSSPECAN_ATTR_DISP_SOFTKEY_BAR                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 336
#define RSSPECAN_ATTR_DISP_TOOL_BAR                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 337
#define RSSPECAN_ATTR_DISP_TOUCHSCREEN_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 338
#define RSSPECAN_ATTR_DISP_SELECT_THEME                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 339
#define RSSPECAN_ATTR_DISP_MARKER_TABLE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 340
#define RSSPECAN_ATTR_DISPLAY_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 341
#define RSSPECAN_ATTR_DISPLAY_ZOOM_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 344
#define RSSPECAN_ATTR_DISPLAY_MULTIPLE_ZOOM_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2683
#define RSSPECAN_ATTR_DISP_MEAS_WINDOW_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2866
#define RSSPECAN_ATTR_LAYOUT_WINDOW_REMOVE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2684
#define RSSPECAN_ATTR_LAYOUT_WINDOW_LIST                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2685
#define RSSPECAN_ATTR_LAYOUT_QUERY_WINDOW_NAME                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2686
#define RSSPECAN_ATTR_LAYOUT_QUERY_WINDOW_TYPE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2965
#define RSSPECAN_ATTR_QUERY_ACTIVE_MEASUREMENT_WINDOW                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2867
#define RSSPECAN_ATTR_HCOPY_ABORT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 345
#define RSSPECAN_ATTR_HCOPY_COLOR_DEF                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 346
#define RSSPECAN_ATTR_HCOPY_COLOR                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 347
#define RSSPECAN_ATTR_HCOPY_PRINT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 348
#define RSSPECAN_ATTR_HCOPY_PRINT_NEXT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 349
#define RSSPECAN_ATTR_HCOPY_DEVICE_DESTINATION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 350
#define RSSPECAN_ATTR_HCOPY_COLOR_PREDEFINED                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 351
#define RSSPECAN_ATTR_HCOPY_DEVICE_LANG_OUT_FORM                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 352
#define RSSPECAN_ATTR_HCOPY_PRINT_SCREEN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 353
#define RSSPECAN_ATTR_HCOPY_PRINT_TAB                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 354
#define RSSPECAN_ATTR_HCOPY_COMM_SCR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 355
#define RSSPECAN_ATTR_HCOPY_PRINT_TRACE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 356
#define RSSPECAN_ATTR_HCOPY_DEVICE_ORIENTATION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 357
#define RSSPECAN_ATTR_HCOPY_FILE_NAME                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 358
#define RSSPECAN_ATTR_HCOPY_PRINTER                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 359
#define RSSPECAN_ATTR_HCOPY_PRINTER_NEXT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 360
#define RSSPECAN_ATTR_HCOPY_PRINTER_FIRST                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 361
#define RSSPECAN_ATTR_HCOPY_TIME_DATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 362
#define RSSPECAN_ATTR_FILE_RECALL                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 363
#define RSSPECAN_ATTR_FILE_STARTUP_RECALL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 364
#define RSSPECAN_ATTR_FILE_SAVE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 365
#define RSSPECAN_ATTR_FILE_SAVE_NEXT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 366
#define RSSPECAN_ATTR_FILE_DATA_CLEAR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 367
#define RSSPECAN_ATTR_FILE_DATA_CLEAR_ALL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 368
#define RSSPECAN_ATTR_FILE_EDIT_COMM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 369
#define RSSPECAN_ATTR_FILE_DEC_SEPARATOR                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 370
#define RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 371
#define RSSPECAN_ATTR_FILE_MANAGER_DELETE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 372
#define RSSPECAN_ATTR_FILE_MANAGER_FORMAT_DISK                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 373
#define RSSPECAN_ATTR_FILE_MANAGER_MAKE_DIR                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 374
#define RSSPECAN_ATTR_FILE_MANAGER_EDIT_PATH_DEVICE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 375
#define RSSPECAN_ATTR_FILE_MANAGER_DELETE_DIR                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 376
#define RSSPECAN_ATTR_FILE_MANAGER_DELETE_IMMEDIATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2373
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_SOURCE_CAL_DATA             RS_SPECIFIC_PUBLIC_ATTR_BASE + 377
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_TRAN                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 378
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_HWSETTINGS                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 379
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_TRAC                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 380
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SEL_LINE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 381
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_ALL                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 382
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_NONE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 383
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_DEFAULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 384
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_INCLUDE_IQ_DATA                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2662
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_NONE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2868
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_DEFAULT                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2869
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_ALL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2870
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_HWSETTINGS          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2871
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_TRAC                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2872
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_LINE                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2873
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SEL_TRAN                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2874
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_CHANNEL_SPEM                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2875
#define RSSPECAN_ATTR_DATA_SET_LOAD_MODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2876
#define RSSPECAN_ATTR_DATA_SET_STORE_MODE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2877
#define RSSPECAN_ATTR_NETWORK_DISCONNECT_DRIVE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 387
#define RSSPECAN_ATTR_UNUSED_NETWORK_DRIVES                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 388
#define RSSPECAN_ATTR_USED_NETWORK_DRIVES                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 389
#define RSSPECAN_ATTR_SHOW_NETWORK_FOLDER_INFO                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 390
#define RSSPECAN_ATTR_SERVICE_INPUT_SOURCE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 391
#define RSSPECAN_ATTR_SERVICE_INPUT_LEVEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 392
#define RSSPECAN_ATTR_SERVICE_INPUT_PULSED_STATE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 393
#define RSSPECAN_ATTR_SERVICE_INPUT_PULSED_RATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 394
#define RSSPECAN_ATTR_SERVICE_INPUT_RECTANGLE_STATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 395
#define RSSPECAN_ATTR_SERVICE_INPUT_RECTANGLE_RATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 396
#define RSSPECAN_ATTR_SERVICE_INPUT_COMB_FREQUENCY                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 397
#define RSSPECAN_ATTR_SERVICE_BASEBAND_SIGNAL_SOURCE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 398
#define RSSPECAN_ATTR_SERVICE_NOISE_SOURCE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 399
#define RSSPECAN_ATTR_SERVICE_DC_CAL_VOLTAGE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 400
#define RSSPECAN_ATTR_SERVICE_PULSE_CAL_DESTINATION                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 401
#define RSSPECAN_ATTR_SERVICE_PULSE_CAL_FREQ                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 402
#define RSSPECAN_ATTR_SERVICE_PULSE_CAL_FREQ_FSV                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 403
#define RSSPECAN_ATTR_SERVICE_HW_INFO                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 404
#define RSSPECAN_ATTR_SERVICE_STEST_RESULTS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 405
#define RSSPECAN_ATTR_SERVICE_STEST                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 406
#define RSSPECAN_ATTR_SERVICE_ROSC_INT_TUNE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 407
#define RSSPECAN_ATTR_SERVICE_ROSC_INT_TUNE_SAVE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 408
#define RSSPECAN_LIMIT_CHECK_RESULT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 409
#define RSSPECAN_ATTR_MARKER_NOISE_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 410
#define RSSPECAN_ATTR_MARKER_NOISE_RESULT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 411
#define RSSPECAN_ATTR_REFERENCE_MARKER_PNO_STATE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 412
#define RSSPECAN_ATTR_REFERENCE_MARKER_PNO_RESULT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 413
#define RSSPECAN_ATTR_REFERENCE_MARKER_PNO_AUTO                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 414
#define RSSPECAN_ATTR_MARKER_FREQUENCY_COUNTER_ENABLED                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 415
#define RSSPECAN_ATTR_MARKER_FREQUENCY_COUNTER_RESOLUTION                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 416
#define RSSPECAN_ATTR_MARKER_COUNT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 417
#define RSSPECAN_ATTR_MARKER_NDB_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 418
#define RSSPECAN_ATTR_MARKER_NDB_VAL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 419
#define RSSPECAN_ATTR_MARKER_NDB_RESULT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 420
#define RSSPECAN_ATTR_MARKER_NDB_Q_FACTOR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 421
#define RSSPECAN_ATTR_LIST_POW_TYPE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 422
#define RSSPECAN_ATTR_LIST_POW_STATE_OFF                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 423
#define RSSPECAN_ATTR_MEAS_TDOM_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 424
#define RSSPECAN_ATTR_MEAS_TDOM_PEAK                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 425
#define RSSPECAN_ATTR_MEAS_TDOM_RMS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 426
#define RSSPECAN_ATTR_MEAS_TDOM_MEAN                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 427
#define RSSPECAN_ATTR_MEAS_TDOM_SDEV                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 428
#define RSSPECAN_ATTR_MEAS_TDOM_AVG                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 429
#define RSSPECAN_ATTR_MEAS_TDOM_MAX_HOLD                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 430
#define RSSPECAN_ATTR_MEAS_TDOM_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 431
#define RSSPECAN_ATTR_MEAS_TDOM_AOFF                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 432
#define RSSPECAN_ATTR_MEAS_TDOM_SET_REFERENCE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 433
#define RSSPECAN_ATTR_MEAS_TDOM_PEAK_RESULT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 434
#define RSSPECAN_ATTR_MEAS_TDOM_RMS_RESULT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 435
#define RSSPECAN_ATTR_MEAS_TDOM_MEAN_RESULT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 436
#define RSSPECAN_ATTR_MEAS_TDOM_SDEV_RESULT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 437
#define RSSPECAN_ATTR_MEAS_TDOM_AVG_PEAK_RESULT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 438
#define RSSPECAN_ATTR_MEAS_TDOM_AVG_RMS_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 439
#define RSSPECAN_ATTR_MEAS_TDOM_AVG_MEAN_RESULT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 440
#define RSSPECAN_ATTR_MEAS_TDOM_AVG_SDEV_RESULT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 441
#define RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_RESULT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 442
#define RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_RMS_RESULT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 443
#define RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_MEAN_RESULT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 444
#define RSSPECAN_ATTR_MEAS_TDOM_PEAK_HOLD_SDEV_RESULT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 445
#define RSSPECAN_ATTR_MEAS_POW_SELECT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 446
#define RSSPECAN_ATTR_MEAS_POW_OFF                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 447
#define RSSPECAN_ATTR_MEAS_POW_ADJ_NUM                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 449
#define RSSPECAN_ATTR_MEAS_POW_ADJ_MODE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 450
#define RSSPECAN_ATTR_MEAS_POW_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 451
#define RSSPECAN_ATTR_MEAS_POW_RESULT_MODE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 453
#define RSSPECAN_ATTR_MEAS_POW_CHANNEL_SPACING                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 454
#define RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_SPACING                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 455
#define RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_SPACING                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 456
#define RSSPECAN_ATTR_MEAS_POW_CHANNEL_BANDWIDTH                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 457
#define RSSPECAN_ATTR_MEAS_POW_CHANNEL_NAME                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2374
#define RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_NAME                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2375
#define RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_NAME                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2376
#define RSSPECAN_ATTR_MEAS_POW_ADJ_CHANNEL_BANDWIDTH                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 459
#define RSSPECAN_ATTR_MEAS_POW_ALT_CHANNEL_BANDWIDTH                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 460
#define RSSPECAN_ATTR_MEAS_POW_NOISE_CORR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 463
#define RSSPECAN_ATTR_MEAS_POW_NOISE_CORR_AUTO                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2772
#define RSSPECAN_ATTR_MEAS_POW_TRACE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 464
#define RSSPECAN_ATTR_MEAS_POW_STANDARD                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 465
#define RSSPECAN_ATTR_MEAS_POW_REF_VALUE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 466
#define RSSPECAN_ATTR_MEAS_POW_ADJ_PRESET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 467
#define RSSPECAN_ATTR_ACP_USER_STD_SET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 469
#define RSSPECAN_ATTR_ACP_USER_STD_SAVE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 470
#define RSSPECAN_ATTR_ACP_USER_STD_DELETE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 471
#define RSSPECAN_ATTR_MEAS_ACP_HSP                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 472
#define RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 473
#define RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL_OFFSET                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 474
#define RSSPECAN_ATTR_MEAS_ACP_PRESET_REF_LEVEL_ADJ                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 475
#define RSSPECAN_ATTR_MEAS_ACP_LIMIT_STATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 476
#define RSSPECAN_ATTR_MEAS_ACP_LIMIT_REL_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 477
#define RSSPECAN_ATTR_MEAS_ACP_LIMIT_ABS_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 478
#define RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_REL_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 479
#define RSSPECAN_ATTR_MEAS_ACP_ALT_LIMIT_ABS_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 480
#define RSSPECAN_ATTR_MEAS_POW_CARR_SIG_NUM                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 481
#define RSSPECAN_ATTR_MEAS_POW_REF_CHAN_SEL_AUTO                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 482
#define RSSPECAN_ATTR_MEAS_POW_ADJ_REF_TXCHANNEL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 483
#define RSSPECAN_ATTR_MEAS_POW_BANDWIDTH                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 484
#define RSSPECAN_ATTR_MSR_NUMBER_OF_SUBBLOCKS                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3054
#define RSSPECAN_ATTR_MSR_SUBBLOCK_CENTER_FREQUENCY                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3055
#define RSSPECAN_ATTR_MSR_SUBBLOCK_RF_BANDWIDTH                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3056
#define RSSPECAN_ATTR_MSR_SUBBLOCK_TX_CHANNEL_COUNT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3057
#define RSSPECAN_ATTR_MSR_TX_CENTER_FREQUENCY                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3058
#define RSSPECAN_ATTR_MSR_TX_TECHNOLOGY                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3059
#define RSSPECAN_ATTR_MSR_TX_BANDWIDTH                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3060
#define RSSPECAN_ATTR_MSR_TX_WEIGHTING_FILTER_STATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3061
#define RSSPECAN_ATTR_MSR_TX_WEIGHTING_FILTER_ROLL_OFF_FACTOR               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3062
#define RSSPECAN_ATTR_MSR_TX_CHANNEL_NAME                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 3063
#define RSSPECAN_ATTR_MSR_GAP_CHANNEL_SPACING                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3064
#define RSSPECAN_ATTR_MSR_GAP_CHANNEL_BANDWIDTHS                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3065
#define RSSPECAN_ATTR_MSR_GAP_WEIGHTING_FILTER_STATE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3066
#define RSSPECAN_ATTR_MSR_GAP_WEIGHTING_FILTER_ROLL_OFF_FACTOR              RS_SPECIFIC_PUBLIC_ATTR_BASE + 3067
#define RSSPECAN_ATTR_MSR_GAP_LIMIT_RELATIVE_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3068
#define RSSPECAN_ATTR_MSR_GAP_LIMIT_ABSOLUTE_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3069
#define RSSPECAN_ATTR_MEAS_STAT_APD_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 486
#define RSSPECAN_ATTR_MEAS_STAT_CCDF_STATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 487
#define RSSPECAN_ATTR_MEAS_STAT_SAMPLES                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 488
#define RSSPECAN_ATTR_MEAS_STAT_X_REF                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 489
#define RSSPECAN_ATTR_MEAS_STAT_X_RANGE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 490
#define RSSPECAN_ATTR_MEAS_STAT_Y_UNIT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 491
#define RSSPECAN_ATTR_MEAS_STAT_Y_MIN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 492
#define RSSPECAN_ATTR_MEAS_STAT_Y_MAX                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 493
#define RSSPECAN_ATTR_MEAS_STAT_MEAN_POWER_POS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 494
#define RSSPECAN_ATTR_MEAS_STAT_ADJ                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 495
#define RSSPECAN_ATTR_MEAS_STAT_REF_LEVEL_ADJUST                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 496
#define RSSPECAN_ATTR_MEAS_STAT_PRESET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 497
#define RSSPECAN_ATTR_STAT_RESULT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 498
#define RSSPECAN_ATTR_MEAS_MDEPTH_STATE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 499
#define RSSPECAN_ATTR_MEAS_MDEPTH_RESULT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 500
#define RSSPECAN_ATTR_MEAS_MDEPTH_SEARCH                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 501
#define RSSPECAN_ATTR_MEAS_TOI_STATE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 502
#define RSSPECAN_ATTR_MEAS_TOI_RESULT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 503
#define RSSPECAN_ATTR_MEAS_TOI_SEARCH                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 504
#define RSSPECAN_ATTR_MEAS_TOI_POSITIONING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 505
#define RSSPECAN_ATTR_MEAS_HDIST_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 506
#define RSSPECAN_ATTR_MEAS_HDIST_NOOFHARM                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 507
#define RSSPECAN_ATTR_MEAS_HDIST_RBWAUTO                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 508
#define RSSPECAN_ATTR_MEAS_HDIST_PRESET                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 509
#define RSSPECAN_ATTR_MPOW_MIN                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 510
#define RSSPECAN_ATTR_MPOW_FTYPE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 511
#define RSSPECAN_ATTR_SE_PEAK_SEARCH_MARGIN                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 512
#define RSSPECAN_ATTR_SE_PEAK_SEARCH_PEAKS                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 513
#define RSSPECAN_ATTR_SE_LIST_RANG_START                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 514
#define RSSPECAN_ATTR_SE_LIST_RANG_STOP                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 515
#define RSSPECAN_ATTR_SE_LIST_RANG_FILT_TYPE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 516
#define RSSPECAN_ATTR_SE_LIST_RANG_BAND                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 517
#define RSSPECAN_ATTR_SE_LIST_RANG_BAND_VID                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 518
#define RSSPECAN_ATTR_SE_LIST_RANG_SWE_TIME_AUTO                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 519
#define RSSPECAN_ATTR_SE_LIST_RANG_SWE_TIME                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 520
#define RSSPECAN_ATTR_SE_LIST_RANG_DET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 521
#define RSSPECAN_ATTR_SE_LIST_RANG_RLEV                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 522
#define RSSPECAN_ATTR_SE_LIST_RANG_INP_ATT_AUTO                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 523
#define RSSPECAN_ATTR_SE_LIST_RANG_INP_ATT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 524
#define RSSPECAN_ATTR_SE_LIST_RANG_POINTS                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 525
#define RSSPECAN_ATTR_SE_LIST_RANG_BREAK                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 526
#define RSSPECAN_ATTR_SE_LIST_RANG_TRAN                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 527
#define RSSPECAN_ATTR_SE_LIST_RANG_INP_GAIN_STAT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 528
#define RSSPECAN_ATTR_SE_LIST_RANG_INP_GAIN_LEVEL                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2687
#define RSSPECAN_ATTR_SE_LIST_RANG_DEL                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 529
#define RSSPECAN_ATTR_SE_LIST_RANG_COUNT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 530
#define RSSPECAN_ATTR_SE_PEAK_SEARCH                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 531
#define RSSPECAN_ATTR_SE_STORE_FILE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 532
#define RSSPECAN_ATTR_SE_LIST_RANG_LIMIT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 533
#define RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_START                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 534
#define RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_STOP                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 535
#define RSSPECAN_ATTR_SE_LIST_RANG_LIMIT_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 536
#define RSSPECAN_ATTR_SE_LIST_EVAL_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 537
#define RSSPECAN_ATTR_SE_STANDARD_SAVE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2626
#define RSSPECAN_ATTR_SE_STANDARD_LOAD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2632
#define RSSPECAN_ATTR_SE_STANDARD_DELETE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2627
#define RSSPECAN_ATTR_IQ_DATA_ANALYZER_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2377
#define RSSPECAN_ATTR_IQ_DATA_FORMAT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 538
#define RSSPECAN_ATTR_IQ_SAMPLE_RATE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 539
#define RSSPECAN_ATTR_IQ_MAX_BANDWIDTH_EXTENSION                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2878
#define RSSPECAN_ATTR_IQ_RECORD_LENGTH                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2558
#define RSSPECAN_ATTR_IQ_EXT_BAND                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 540
#define RSSPECAN_ATTR_IQ_DATA_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 541
#define RSSPECAN_ATTR_IQ_START_SYNCHRONIZATION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 542
#define RSSPECAN_ATTR_IQ_DATA_WBAND_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 543
#define RSSPECAN_ATTR_IQ_DATA_AVER_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 544
#define RSSPECAN_ATTR_IQ_DATA_AVER_COUNT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 545
#define RSSPECAN_ATTR_IQ_DATA_GATE_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 546
#define RSSPECAN_ATTR_IQ_DATA_GATE_TYPE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 547
#define RSSPECAN_ATTR_IQ_DATA_GATE_LENGTH                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 548
#define RSSPECAN_ATTR_IQ_DATA_GATE_GAP                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 549
#define RSSPECAN_ATTR_IQ_DATA_GATE_NUMBER_OF_PERIODS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 550
#define RSSPECAN_ATTR_IQ_DATA_DISPLAY_TYPE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 551
#define RSSPECAN_ATTR_IQ_DATA_FILTER_BANDWIDTH                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 552
#define RSSPECAN_ATTR_LOAD_IQ_DATA                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2559
#define RSSPECAN_ATTR_SAVE_IQ_DATA                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2560
#define RSSPECAN_ATTR_IQ_DATA_EXPORT_FILE_DESCRIPTION                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2561
#define RSSPECAN_ATTR_QUERY_TPIS_TIME_OFFSET                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2707
#define RSSPECAN_ATTR_ESP_RESTORE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 553
#define RSSPECAN_ATTR_ESP_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 554
#define RSSPECAN_ATTR_ESP_PEAK_SEARCH                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 555
#define RSSPECAN_ATTR_ESP_MAN_VAL                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 556
#define RSSPECAN_ATTR_ESP_QUERY_X                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 557
#define RSSPECAN_ATTR_ESP_QUERY_Y                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 558
#define RSSPECAN_ATTR_ESP_TRAN                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 559
#define RSSPECAN_ATTR_LIM_STORE_FILE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 560
#define RSSPECAN_ATTR_ESP_ANALYSIS                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 561
#define RSSPECAN_ATTR_SEM_PEAK_SEARCH_MARGIN                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 562
#define RSSPECAN_ATTR_SEM_PEAK_SEARCH_PEAKS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 563
#define RSSPECAN_ATTR_SEM_LIST_EVAL_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 564
#define RSSPECAN_ATTR_SEM_MARK_ALL_PEAKS                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 565
#define RSSPECAN_ATTR_SEM_POWER_REF_TYPE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 566
#define RSSPECAN_ATTR_SEM_BANDWIDTH                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 567
#define RSSPECAN_ATTR_SEM_RRC_FILTER_STATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 568
#define RSSPECAN_ATTR_SEM_RRC_FILTER_ALPHA_VALUE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 569
#define RSSPECAN_ATTR_SEM_REF_RANGE_POSITION                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 570
#define RSSPECAN_ATTR_SEM_MSR_BAND_CATEGORY                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2901
#define RSSPECAN_ATTR_SEM_MSR_RF_BANDWIDTH                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2902
#define RSSPECAN_ATTR_SEM_MSR_GSM_PRESENT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2903
#define RSSPECAN_ATTR_SEM_MSR_LTE_PRESENT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2904
#define RSSPECAN_ATTR_SEM_MSR_APPLY                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2905
#define RSSPECAN_ATTR_SEM_FAST_SEM                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 571
#define RSSPECAN_ATTR_SEM_PRESET_STANDARD                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 572
#define RSSPECAN_ATTR_SEM_PRESET_STANDARD_SAVE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2628
#define RSSPECAN_ATTR_SEM_PRESET_STANDARD_DELETE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2629
#define RSSPECAN_ATTR_SEM_RESTORE_STANDARD                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 573
#define RSSPECAN_ATTR_SEM_STORE_PRESET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 574
#define RSSPECAN_ATTR_SEM_LIST_FILE_EXPORT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 575
#define RSSPECAN_ATTR_SEM_RANGE_BANDWIDTH                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 576
#define RSSPECAN_ATTR_SEM_RANGE_VBW                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 577
#define RSSPECAN_ATTR_SEM_RANGE_FREQ_START                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 578
#define RSSPECAN_ATTR_SEM_RANGE_FREQ_STOP                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 579
#define RSSPECAN_ATTR_SEM_RANGE_START_FREQ_ABS_LIMIT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 580
#define RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_ABS_LIMIT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 581
#define RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 582
#define RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 583
#define RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_FUNCTION               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2906
#define RSSPECAN_ATTR_SEM_RANGE_START_FREQ_REL_LIMIT_MAXIMUM                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2907
#define RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT_FUNCTION                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2908
#define RSSPECAN_ATTR_SEM_RANGE_STOP_FREQ_REL_LIMIT_MAXIMUM                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2909
#define RSSPECAN_ATTR_SEM_RANGE_LIMIT_CHECK_TYPE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 584
#define RSSPECAN_ATTR_SEM_RANGE_FILTER_TYPE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 585
#define RSSPECAN_ATTR_SEM_RANGE_INPUT_ATTENUATION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 586
#define RSSPECAN_ATTR_SEM_RANGE_INPUT_ATTENUATION_AUTO                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 587
#define RSSPECAN_ATTR_SEM_RANGE_PREAMPLIFIER                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 588
#define RSSPECAN_ATTR_SEM_RANGE_PREAMPLIFIER_LEVEL                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2688
#define RSSPECAN_ATTR_SEM_RANGE_REF_LEVEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 589
#define RSSPECAN_ATTR_SEM_RANGE_SWEEP_TIME                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 590
#define RSSPECAN_ATTR_SEM_RANGE_SWEEP_TIME_AUTO                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 591
#define RSSPECAN_ATTR_SEM_RANGE_TRANSDUCER                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 592
#define RSSPECAN_ATTR_SEM_RANGE_COUNT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 593
#define RSSPECAN_ATTR_SEM_INSERT_NEW_RANGE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 594
#define RSSPECAN_ATTR_SEM_DELETE_RANGE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 595
#define RSSPECAN_ATTR_SEM_POWER_CLASS_USED                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 596
#define RSSPECAN_ATTR_SEM_POWER_CLASS_LOWER_LEVEL_LIMIT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 597
#define RSSPECAN_ATTR_SEM_POWER_CLASS_UPPER_LEVEL_LIMIT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 598
#define RSSPECAN_ATTR_SEM_POWER_CLASS_LIMIT_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 599
#define RSSPECAN_ATTR_SEM_POWER_CLASS_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 600
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALL_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 601
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALL_ALPHA                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 602
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ACH_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 603
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ACH_ALPHA                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 604
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALT_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 605
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_ALT_ALPHA                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 606
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_TX_CHANNEL_STATE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 607
#define RSSPECAN_ATTR_SEM_WEIGHT_FILTER_TX_CHANNEL_ALPHA                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 608
#define RSSPECAN_ATTR_TRIGGER_RFP_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 956
#define RSSPECAN_ATTR_TRIGGER_SOURCE_TVRF                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 957
#define RSSPECAN_ATTR_TRIGGER_TV_VID_CONT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 958
#define RSSPECAN_ATTR_TRIGGER_TV_VERTICAL_SIGNAL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 959
#define RSSPECAN_ATTR_TRIGGER_TV_LINE_SYSTEM                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 960
#define RSSPECAN_ATTR_TRIGGER_TV_HORIZONTAL_SIGNAL                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 961
#define RSSPECAN_ATTR_TRIGGER_TV_VIDEO_POLARITY                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 962
#define RSSPECAN_ATTR_TRIGGER_POSITION_IN_SAMPLE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2562
#define RSSPECAN_ATTR_MSRA_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2910
#define RSSPECAN_ATTR_MSRA_REFRESH                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2911
#define RSSPECAN_ATTR_MSRA_CAPTURE_OFFSET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2912
#define RSSPECAN_ATTR_MSRA_SHOW_ANALYSIS_LINE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3000
#define RSSPECAN_ATTR_MSRA_ANALYSIS_LINE_POSITION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3001
#define RSSPECAN_ATTR_MSRA_DATA_EVALUATION                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3095
#define RSSPECAN_ATTR_TGEN_NORMALIZATION_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 963
#define RSSPECAN_ATTR_TGEN_METHOD                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 964
#define RSSPECAN_ATTR_TGEN_ACQUIRE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 965
#define RSSPECAN_ATTR_TGEN_RECALL                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 966
#define RSSPECAN_ATTR_TGEN_POW_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 967
#define RSSPECAN_ATTR_EXT_GEN_INTF_TYPE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 968
#define RSSPECAN_ATTR_TGEN_POW_START                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 969
#define RSSPECAN_ATTR_TGEN_POW_STOP                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 970
#define RSSPECAN_ATTR_EXT_GEN_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 971
#define RSSPECAN_ATTR_EXT_GEN_OUTPUT_FREQUENCY                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2563
#define RSSPECAN_ATTR_EXT_GEN_OUTPUT_FREQUENCY_COUPLING                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2564
#define RSSPECAN_ATTR_EXT_GEN_DENOMINATOR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 972
#define RSSPECAN_ATTR_EXT_GEN_NUMERATOR                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 973
#define RSSPECAN_ATTR_EXT_GEN_FREQ_OFFSET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 974
#define RSSPECAN_ATTR_EXT_GEN_FREQ_SWEEP_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 975
#define RSSPECAN_ATTR_EXT_GEN_OUTPUT_POWER                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 976
#define RSSPECAN_ATTR_EXT_GEN_ROSC_SOURCE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 977
#define RSSPECAN_ATTR_EXT_GEN_TYPE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 978
#define RSSPECAN_ATTR_EXT_GEN_IEEE_ADDR                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 979
#define RSSPECAN_ATTR_CORR_EGA_INP                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 980
#define RSSPECAN_ATTR_TGEN_DM_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 981
#define RSSPECAN_ATTR_TGEN_FM_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 982
#define RSSPECAN_ATTR_TGEN_FM_DEVIATION                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 983
#define RSSPECAN_ATTR_TGEN_FREQUENCY_OFFSET                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 984
#define RSSPECAN_ATTR_TGEN_OUTPUT_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 985
#define RSSPECAN_ATTR_TGEN_LEVEL_OFFSET                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 986
#define RSSPECAN_ATTR_TGEN_STATE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 987
#define RSSPECAN_ATTR_TGEN_AM_STATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 988
#define RSSPECAN_ATTR_FM_LPAS_MAN                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 990
#define RSSPECAN_ATTR_FM_SPAN_MAN                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 991
#define RSSPECAN_ATTR_FM_STATE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 992
#define RSSPECAN_ATTR_FM_LPAS_FILTER                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 993
#define RSSPECAN_ATTR_FM_HPAS_FILTER                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 994
#define RSSPECAN_ATTR_FM_SPAN                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 995
#define RSSPECAN_ATTR_BB_INPUT_SELECTION                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 996
#define RSSPECAN_ATTR_BB_SIGNAL_CALIBRATION_TYPE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3002
#define RSSPECAN_ATTR_BB_SIGNAL_CALIBRATION_OUTPUT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3003
#define RSSPECAN_ATTR_BB_INPUT_BALANCED                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 997
#define RSSPECAN_ATTR_BB_INPUT_IMPEDANCE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 998
#define RSSPECAN_ATTR_BB_INPUT_SIGNAL_PATH                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 999
#define RSSPECAN_ATTR_BB_INPUT_IQ_DITH                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1000
#define RSSPECAN_ATTR_BB_INPUT_IQ_LPAS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1001
#define RSSPECAN_ATTR_BB_INPUT_MEAS_RANGE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1002
#define RSSPECAN_ATTR_BB_INPUT_TRIGGER                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1003
#define RSSPECAN_ATTR_BB_INPUT_TRIGGER_I_LEVEL                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1004
#define RSSPECAN_ATTR_BB_INPUT_TRIGGER_Q_LEVEL                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1005
#define RSSPECAN_ATTR_BB_FULLSCALE_MODE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3074
#define RSSPECAN_ATTR_BB_FULLSCALE_LEVEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 3075
#define RSSPECAN_ATTR_BB_SWAP_IQ                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3076
#define RSSPECAN_ATTR_FFT_BAND_STEP_MODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1006
#define RSSPECAN_ATTR_FFT_FORM                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1007
#define RSSPECAN_ATTR_FFT_PLINE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1008
#define RSSPECAN_ATTR_FFT_PLINE_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1009
#define RSSPECAN_ATTR_FFT                                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1010
#define RSSPECAN_ATTR_FFT_CAPT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1011
#define RSSPECAN_ATTR_FFT_CAPT_AUTO                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1012
#define RSSPECAN_ATTR_FFT_CAPT_CALC                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1013
#define RSSPECAN_ATTR_FFT_PRESET                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1014
#define RSSPECAN_ATTR_FFT_WIN_TYPE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1015
#define RSSPECAN_ATTR_FFT_CAL_SIG                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1016
#define RSSPECAN_ATTR_FFT_CAL_DC                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1017
#define RSSPECAN_ATTR_FFT_CAL_FREQ                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1018
#define RSSPECAN_ATTR_FFT_CAL_SIG_SOUR                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1019
#define RSSPECAN_ATTR_FFT_OFFS_PHAS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1020
#define RSSPECAN_ATTR_FFT_UNIT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1021
#define RSSPECAN_ATTR_DIGITAL_INPUT_RANGE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1022
#define RSSPECAN_ATTR_DIGITAL_INPUT_RANGE_AUTO                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1023
#define RSSPECAN_ATTR_DIGITAL_INPUT_SRATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1024
#define RSSPECAN_ATTR_DIGITAL_INPUT_SAMPLE_RATE_AUTO                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1025
#define RSSPECAN_ATTR_QUERY_DIGITAL_INPUT_CONF                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1026
#define RSSPECAN_ATTR_QUERY_DIGITAL_OUTPUT_CONF                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1027
#define RSSPECAN_ATTR_DIGITAL_BASEBAND_OUTPUT_STATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1028
#define RSSPECAN_ATTR_DIGITAL_BASEBAND_FULL_SCALE_LEVEL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1029
#define RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_TRIGGER_SOURCE                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1030
#define RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_TRIGGER_LEVEL                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1031
#define RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_TRIGGER_HOLDOFF                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1032
#define RSSPECAN_ATTR_DIGITAL_BASEBAND_INPUT_UNIT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2378
#define RSSPECAN_ATTR_DIGITAL_INPUT_COUPLING                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2379
#define RSSPECAN_ATTR_GSM_MTYP                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1033
#define RSSPECAN_ATTR_GSM_REST                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1034
#define RSSPECAN_ATTR_GSM_PRAT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1035
#define RSSPECAN_ATTR_GSM_SLOT_MULT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1036
#define RSSPECAN_ATTR_GSM_CHAN_TSC                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1037
#define RSSPECAN_ATTR_GSM_TSC_USER                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1038
#define RSSPECAN_ATTR_GSM_SSE                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1039
#define RSSPECAN_ATTR_GSM_BSE                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1040
#define RSSPECAN_ATTR_GSM_BSTH                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1041
#define RSSPECAN_ATTR_GSM_BURS_PTEM_TMHR                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1042
#define RSSPECAN_ATTR_GSM_BURS_PTEM_FRZ                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1043
#define RSSPECAN_ATTR_GSM_BURS_PTEM_FILT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1044
#define RSSPECAN_ATTR_GSM_MCAR                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1045
#define RSSPECAN_ATTR_GSM_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1046
#define RSSPECAN_ATTR_GSM_BURS_PTEM_SEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1047
#define RSSPECAN_ATTR_GSM_SYNC_ADJ_EXT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1048
#define RSSPECAN_ATTR_GSM_SYNC_ADJ_IFP                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1049
#define RSSPECAN_ATTR_GSM_SYNC_ADJ_RFP                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1050
#define RSSPECAN_ATTR_GSM_SYNC_ADJ_IMM                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1051
#define RSSPECAN_ATTR_GSM_SYNC_IQPOWER                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1052
#define RSSPECAN_ATTR_GSM_BURS_PFER                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1053
#define RSSPECAN_ATTR_GSM_BURS_MACC                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1054
#define RSSPECAN_ATTR_GSM_BURS_POW                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1055
#define RSSPECAN_ATTR_GSM_BURS_PTEM                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1056
#define RSSPECAN_ATTR_GSM_SPEC_MOD                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1057
#define RSSPECAN_ATTR_GSM_SPEC_SWIT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1058
#define RSSPECAN_ATTR_GSM_SWIT_LIM                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1059
#define RSSPECAN_ATTR_GSM_SPUR                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1060
#define RSSPECAN_ATTR_GSM_MODULATION_SPECTRUM_LIST_AVER_TYPE                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1061
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_STATE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1062
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LONG_SLOT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1063
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_TRIG_REFERENCE                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1064
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_MIDAMBLE_REFERENCE                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1065
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_STANDARD                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1066
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_OFRAME                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1067
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_MODULATION                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1068
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_RLEV_MODE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1069
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_RLEV_VALUE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1070
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1071
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_STATE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1072
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_VALUE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1073
#define RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT_CLEV                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1074
#define RSSPECAN_ATTR_GSM_READ_BURST_PERR_RMS_AVER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1075
#define RSSPECAN_ATTR_GSM_READ_BURST_PERR_RMS_MAX                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1076
#define RSSPECAN_ATTR_GSM_READ_BURST_PERR_PEAK_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1077
#define RSSPECAN_ATTR_GSM_READ_BURST_PERR_PEAK_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1078
#define RSSPECAN_ATTR_GSM_READ_BURST_FERR_AVER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1079
#define RSSPECAN_ATTR_GSM_READ_BURST_FERR_MAX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1080
#define RSSPECAN_ATTR_GSM_READ_BURST_IQOF_AVER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1081
#define RSSPECAN_ATTR_GSM_READ_BURST_IQOF_MAX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1082
#define RSSPECAN_ATTR_GSM_READ_BURST_IQIM_AVER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1083
#define RSSPECAN_ATTR_GSM_READ_BURST_IQIM_MAX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1084
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_RMS_AVER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1085
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_RMS_MAX                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1086
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_PEAK_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1087
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_PEAK_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1088
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_OSUP_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1089
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_OSUP_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1090
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_PERC_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1091
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_PERC_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1092
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_FREQ_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1093
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_FREQ_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1094
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQOF_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1095
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQOF_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1096
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQIM_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1097
#define RSSPECAN_ATTR_GSM_READ_BURST_MACC_IQIM_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1098
#define RSSPECAN_ATTR_GSM_FETC_BURST_PERR_RMS_AVER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1099
#define RSSPECAN_ATTR_GSM_FETC_BURST_PERR_RMS_MAX                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1100
#define RSSPECAN_ATTR_GSM_FETC_BURST_PERR_PEAK_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1101
#define RSSPECAN_ATTR_GSM_FETC_BURST_PERR_PEAK_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1102
#define RSSPECAN_ATTR_GSM_FETC_BURST_FERR_AVER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1103
#define RSSPECAN_ATTR_GSM_FETC_BURST_FERR_MAX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1104
#define RSSPECAN_ATTR_GSM_FETC_BURST_IQOF_AVER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1105
#define RSSPECAN_ATTR_GSM_FETC_BURST_IQOF_MAX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1106
#define RSSPECAN_ATTR_GSM_FETC_BURST_IQIM_AVER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1107
#define RSSPECAN_ATTR_GSM_FETC_BURST_IQIM_MAX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1108
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_RMS_AVER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1109
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_RMS_MAX                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1110
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PEAK_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1111
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PEAK_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1112
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_OSUP_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1113
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_OSUP_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1114
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PERC_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1115
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_PERC_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1116
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_FREQ_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1117
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_FREQ_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1118
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQOF_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1119
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQOF_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1120
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQIM_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1121
#define RSSPECAN_ATTR_GSM_FETC_BURST_MACC_IQIM_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1122
#define RSSPECAN_ATTR_GSM_FETC_BURST_PTEM_TRGS_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1123
#define RSSPECAN_ATTR_GSM_FETC_BURST_PTEM_TRGS_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1124
#define RSSPECAN_ATTR_GSM_READ_BURST_PTEM_TRGS_AVER                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1125
#define RSSPECAN_ATTR_GSM_READ_BURST_PTEM_TRGS_MAX                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1126
#define RSSPECAN_ATTR_PULSE_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2800
#define RSSPECAN_ATTR_PULSE_SIGNAL_PERIOD                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2802
#define RSSPECAN_ATTR_PULSE_SIGNAL_DROOP_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2803
#define RSSPECAN_ATTR_PULSE_SIGNAL_MODULATION                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2804
#define RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_AUTO_MODE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2805
#define RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_WIDTH                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2806
#define RSSPECAN_ATTR_PULSE_SIGNAL_TIMING_OFF_TIME                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2807
#define RSSPECAN_ATTR_PULSE_SIGNAL_OFFSET_AUTO_MODE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2808
#define RSSPECAN_ATTR_PULSE_SIGNAL_OFFSET_VALUE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2809
#define RSSPECAN_ATTR_PULSE_SIGNAL_CHIRP_RATE_AUTO_MODE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2810
#define RSSPECAN_ATTR_PULSE_SIGNAL_CHIRP_RATE_VALUE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2811
#define RSSPECAN_ATTR_PULSE_INPUT_SIGNAL_SOURCE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2827
#define RSSPECAN_ATTR_PULSE_SELECT_IQ_DATA_FILE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3070
#define RSSPECAN_ATTR_PULSE_ACQUISITION_FILTER_TYPE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2828
#define RSSPECAN_ATTR_PULSE_ACQUISITION_BANDWIDTH                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2829
#define RSSPECAN_ATTR_PULSE_DETECTION_REF_SOURCE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2830
#define RSSPECAN_ATTR_PULSE_DETECTION_THRESHOLD                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2831
#define RSSPECAN_ATTR_PULSE_DETECTION_HYSTERESIS                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2832
#define RSSPECAN_ATTR_PULSE_DETECTION_LIMIT_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2833
#define RSSPECAN_ATTR_PULSE_DETECTION_PULSE_COUNT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2834
#define RSSPECAN_ATTR_PULSE_REFERENCE_POINT_TYPE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2835
#define RSSPECAN_ATTR_PULSE_REFERENCE_POINT_OFFSET                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2836
#define RSSPECAN_ATTR_PULSE_REFERENCE_POINT_AVERAGING_WINDOW                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2966
#define RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_TYPE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2837
#define RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_LENGTH                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2838
#define RSSPECAN_ATTR_PULSE_REFERENCE_RANGE_OFFSET                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2839
#define RSSPECAN_ATTR_PULSE_TOP_LEVEL_POSITION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2840
#define RSSPECAN_ATTR_PULSE_TOP_LEVEL_ALGORITHM                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2841
#define RSSPECAN_ATTR_PULSE_TOP_LEVEL_RIPPLE_PORTION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2842
#define RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_UNIT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2843
#define RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_THRESHOLD                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2844
#define RSSPECAN_ATTR_PULSE_REFERENCE_LEVEL_BOUNDARY                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2845
#define RSSPECAN_ATTR_PULSE_SELECTION                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2846
#define RSSPECAN_ATTR_PULSE_RESULT_AUTOMATIC_RANGE_SCALING_ONCE             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3107
#define RSSPECAN_ATTR_PULSE_RESULT_AUTOMATIC_RANGE_SCALING                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3108
#define RSSPECAN_ATTR_PULSE_RESULT_RANGE_REFERENCE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2847
#define RSSPECAN_ATTR_PULSE_RESULT_RANGE_OFFSET                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2848
#define RSSPECAN_ATTR_PULSE_RESULT_RANGE_ALIGNMENT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2849
#define RSSPECAN_ATTR_PULSE_RESULT_RANGE_LENGTH                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2850
#define RSSPECAN_ATTR_PULSE_RESULT_PHASE_UNIT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2851
#define RSSPECAN_ATTR_PULSE_PARAMETER_DISTRIBUTION_BINS                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3109
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_POWER_Y_AXIS             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3114
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_POWER_X_AXIS             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3115
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_TIMING_Y_AXIS            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3116
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_TIMING_X_AXIS            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3117
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_FREQUENCY_Y_AXIS         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3118
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_FREQUENCY_X_AXIS         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3119
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_PHASE_Y_AXIS             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3120
#define RSSPECAN_ATTR_PULSE_RESULT_PARAMETER_TREND_PHASE_X_AXIS             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3121
#define RSSPECAN_ATTR_PULSE_RESULT_TABLE_PHASE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2852
#define RSSPECAN_ATTR_PULSE_RESULT_TABLE_POWER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2853
#define RSSPECAN_ATTR_PULSE_AUTO_GRID_SCALING                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2854
#define RSSPECAN_ATTR_PULSE_ABSOLUTE_SCALING                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2855
#define RSSPECAN_ATTR_PULSE_REFERENCE_VALUE_MIN_MAX                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2856
#define RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_FREQUENCY                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 3136
#define RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_PHASE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3137
#define RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_POWER                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3138
#define RSSPECAN_ATTR_PULSE_PARAMETER_SPECTRUM_TIMING                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3139
#define RSSPECAN_ATTR_FMDEM_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1127
#define RSSPECAN_ATTR_FMDEM_TRIGGER_SOURCE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1128
#define RSSPECAN_ATTR_ADEM_TRIGGER_AM_LEVEL_ABS                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1129
#define RSSPECAN_ATTR_ADEM_TRIGGER_AM_LEVEL_REL                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1130
#define RSSPECAN_ATTR_ADEM_TRIGGER_FM_LEVEL                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1131
#define RSSPECAN_ATTR_ADEM_TRIGGER_PM_LEVEL                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1132
#define RSSPECAN_ATTR_ADEM_SIGNAL_SEARCH                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1133
#define RSSPECAN_ATTR_FMDEM_DISPLAY_PDIV                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1134
#define RSSPECAN_ATTR_FMDEM_STATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1135
#define RSSPECAN_ATTR_FMDEM_AF_COUP                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1136
#define RSSPECAN_ATTR_FMDEM_MTIM                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1137
#define RSSPECAN_ATTR_FMDEM_BAND_DEM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1138
#define RSSPECAN_ATTR_FMDEM_RLEN                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1139
#define RSSPECAN_ATTR_FMDEM_SRATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1140
#define RSSPECAN_ATTR_FMDEM_ZOOM                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1141
#define RSSPECAN_ATTR_FMDEM_ZOOM_START                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1142
#define RSSPECAN_ATTR_FMDEM_ZOOM_LENGTH_MODE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1143
#define RSSPECAN_ATTR_FMDEM_ZOOM_LENGTH                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1144
#define RSSPECAN_ATTR_FMDEM_SQUELCH                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2567
#define RSSPECAN_ATTR_FMDEM_SQUELCH_LEVEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2568
#define RSSPECAN_ATTR_ADEM_PHASE_WRAP                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1145
#define RSSPECAN_ATTR_ADEM_UNIT_ANGLE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1146
#define RSSPECAN_ATTR_ADEM_PM_RPO_X                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1147
#define RSSPECAN_ATTR_ANALOG_DEMOD_OUTPUT_STATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3004
#define RSSPECAN_ATTR_ANALOG_DEMOD_OUTPUT_SELECTION                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3005
#define RSSPECAN_ATTR_ANALOG_DEMOD_OUTPUT_CUTOFF_FREQUENCY                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3006
#define RSSPECAN_ATTR_ANALOG_DEMOD_PHONE_OUTPUT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3007
#define RSSPECAN_ATTR_ADEM_UNIT_THD                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1148
#define RSSPECAN_ATTR_ADEM_SELECT_TRACE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2967
#define RSSPECAN_ATTR_ADEM_PRESET_LOAD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2968
#define RSSPECAN_ATTR_ADEM_PRESET_STORE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2969
#define RSSPECAN_ATTR_ADEM_PRESET_RESTORE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2970
#define RSSPECAN_ATTR_FMDEM_AF_CENTER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1149
#define RSSPECAN_ATTR_FMDEM_AF_SPAN                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1150
#define RSSPECAN_ATTR_FMDEM_AF_START                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1151
#define RSSPECAN_ATTR_FMDEM_AF_STOP                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1152
#define RSSPECAN_ATTR_FMDEM_AF_FULL_SPAN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1153
#define RSSPECAN_ATTR_FMDEM_BAND_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1154
#define RSSPECAN_ATTR_FMDEM_SPEC_SPAN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1155
#define RSSPECAN_ATTR_FMDEM_SPEC_ZOOM                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1156
#define RSSPECAN_ATTR_FMDEM_FILT_HPAS_STAT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1157
#define RSSPECAN_ATTR_FMDEM_FILT_HPAS_FREQ                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1158
#define RSSPECAN_ATTR_FMDEM_FILT_LPAS_STAT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1159
#define RSSPECAN_ATTR_FMDEM_FILT_LPAS_FREQ                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1160
#define RSSPECAN_ATTR_ADEM_FILT_LPAS_FREQ_REL                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1161
#define RSSPECAN_ATTR_ADEM_FILT_FREQ_MANUAL                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3008
#define RSSPECAN_ATTR_FMDEM_FILT_DEMP_STAT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1162
#define RSSPECAN_ATTR_FMDEM_FILT_DEMP_TCON                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1163
#define RSSPECAN_ATTR_ADEM_FILT_CCITT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1164
#define RSSPECAN_ATTR_ADEM_FILT_CCIR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1165
#define RSSPECAN_ATTR_ANALOG_DEMOD_FILTER_A_WEIGHTED_ENABLED                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2380
#define RSSPECAN_ATTR_FMDEM_AFR_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1166
#define RSSPECAN_ATTR_FMDEM_FERR_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1167
#define RSSPECAN_ATTR_FMDEM_SIN_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1168
#define RSSPECAN_ATTR_FMDEM_THD_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1169
#define RSSPECAN_ATTR_FMDEM_CARR_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1170
#define RSSPECAN_ATTR_ADEM_SUMM_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1171
#define RSSPECAN_ATTR_ADEM_FM_OFFSET                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1172
#define RSSPECAN_ATTR_FMS_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1173
#define RSSPECAN_ATTR_FMS_STATE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1174
#define RSSPECAN_ATTR_FMS_TRIGGER_SOURCE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1175
#define RSSPECAN_ATTR_FMS_TRIGGER_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1176
#define RSSPECAN_ATTR_FMS_PHASE_NOISE_MARKER_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1177
#define RSSPECAN_ATTR_FMS_DIFFERENCE_FREQUENCY_DISTORTION_STATE             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1178
#define RSSPECAN_ATTR_FMS_DIFFERENCE_FREQUENCY_DISTORTION_UNIT              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1179
#define RSSPECAN_ATTR_FMS_INTERMODULATION_DISTORTION_STATE                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1180
#define RSSPECAN_ATTR_FMS_INTERMODULATION_DISTORTION_UNIT                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1181
#define RSSPECAN_ATTR_FMS_SQUELCH_LEVEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2566
#define RSSPECAN_ATTR_FMS_SQUELCH                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2565
#define RSSPECAN_ATTR_FMS_AF_AUTO_SCALE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1182
#define RSSPECAN_ATTR_FMS_DISPLAY_CONFIG                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1183
#define RSSPECAN_ATTR_FMS_REFERENCE_DEVIATION                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1184
#define RSSPECAN_ATTR_FMS_RESULT_SUMMARY_DETECTOR                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1185
#define RSSPECAN_ATTR_FMS_RESULT_SUMMARY_MODE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1186
#define RSSPECAN_ATTR_FMS_RESULT_SUMMARY_COUPLED                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1187
#define RSSPECAN_ATTR_FMS_MEAS_TO_REF                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1188
#define RSSPECAN_ATTR_FMS_AF_FILTER_HIGH_PASS_STATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1189
#define RSSPECAN_ATTR_FMS_AF_FILTER_HIGH_PASS_FREQUENCY                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1190
#define RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1191
#define RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_FREQUENCY_ABSOLUTE             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1192
#define RSSPECAN_ATTR_FMS_AF_FILTER_LOW_PASS_FREQUENCY_RELATIVE             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1193
#define RSSPECAN_ATTR_FMS_AF_FILTER_CCITT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1194
#define RSSPECAN_ATTR_FMS_AF_FILTER_CCIR_UNWEIGHTED                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1195
#define RSSPECAN_ATTR_FMS_AF_FILTER_CCIR_WEIGHTED                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1196
#define RSSPECAN_ATTR_FMS_AF_FILTER_A_WEIGHTED                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2569
#define RSSPECAN_ATTR_FMS_AF_FILTER_DEEMPHASIS_STATE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1197
#define RSSPECAN_ATTR_FMS_AF_FILTER_DEEMPHASIS_TIME_CONSTANT                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1198
#define RSSPECAN_ATTR_FMS_AF_FILTER_COUPLED                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1199
#define RSSPECAN_ATTR_FMS_ALL_AF_FILTERS_OFF                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1200
#define RSSPECAN_ATTR_FMS_AF_RANGE_DEV_UNIT_ABSOLUTE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1201
#define RSSPECAN_ATTR_FMS_AF_RANGE_DEV_UNIT_RELATIVE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1202
#define RSSPECAN_ATTR_FMS_DIFFERENCE_FREQUENCY_DISTORTION_SEARCH_SIGNAL     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1203
#define RSSPECAN_ATTR_FMS_INTERMODULATION_DISTORTION_SEARCH_SIGNAL          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1204
#define RSSPECAN_ATTR_QUERY_FMS_CARRIER_FREQUENCY                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1205
#define RSSPECAN_ATTR_QUERY_FMS_PHASE_NOISE_RESULT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1206
#define RSSPECAN_ATTR_QUERY_FMS_CHANNEL_TYPE_RESULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1207
#define RSSPECAN_ATTR_BTO_PTYPE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1208
#define RSSPECAN_ATTR_BTO_PRAT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1209
#define RSSPECAN_ATTR_BTO_PCL                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1210
#define RSSPECAN_ATTR_BTO_POW_AVER                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1211
#define RSSPECAN_ATTR_BTO_BWID_AUTO                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1212
#define RSSPECAN_ATTR_BTO_BWID                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1213
#define RSSPECAN_ATTR_BTO_VID_AUTO                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1214
#define RSSPECAN_ATTR_BTO_VID                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1215
#define RSSPECAN_ATTR_BTO_FILT_MEAS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1216
#define RSSPECAN_ATTR_BTO_TRAC_MOD                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1217
#define RSSPECAN_ATTR_BTO_TRAC                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1218
#define RSSPECAN_ATTR_BTO_TRAC_DET                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1219
#define RSSPECAN_ATTR_BTO_SWE                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1220
#define RSSPECAN_ATTR_BTO_SWE_TIME_AUTO                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1221
#define RSSPECAN_ATTR_BTO_SWE_TIME                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1222
#define RSSPECAN_ATTR_BTO_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1223
#define RSSPECAN_ATTR_BTO                                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1224
#define RSSPECAN_ATTR_BTO_LAP                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1225
#define RSSPECAN_ATTR_BTO_CHANNEL                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1226
#define RSSPECAN_ATTR_BTO_ACLR_ACP                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1227
#define RSSPECAN_ATTR_BTO_GEOG                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1228
#define RSSPECAN_ATTR_BTO_EGA_INP                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1229
#define RSSPECAN_ATTR_BTO_PULS_OFFS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1230
#define RSSPECAN_ATTR_BTO_SEARCH_PULS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1231
#define RSSPECAN_ATTR_BTO_SYNC_OFFS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1232
#define RSSPECAN_ATTR_BTO_SEARCH_SYNC                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1233
#define RSSPECAN_ATTR_BTO_SEARCH_TIME                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1234
#define RSSPECAN_ATTR_BTO_SEARCH_TIME_AUTO                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1235
#define RSSPECAN_ATTR_BTO_CFST_BLOCK_COUNT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1236
#define RSSPECAN_ATTR_BTO_IBS_ACP                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1237
#define RSSPECAN_ATTR_BTO_IBS_GATE_AUTO                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1238
#define RSSPECAN_ATTR_BTO_PBSC                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1239
#define RSSPECAN_ATTR_BTO_RTP_TIME                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1240
#define RSSPECAN_ATTR_BTO_PDIV                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1241
#define RSSPECAN_ATTR_BTO_ZOOM_STATE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1242
#define RSSPECAN_ATTR_BTO_ZOOM_START                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1243
#define RSSPECAN_ATTR_BTO_OPOW_PEAK                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1244
#define RSSPECAN_ATTR_BTO_OPOW_AVER                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1245
#define RSSPECAN_ATTR_BTO_STAT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1246
#define RSSPECAN_ATTR_BTO_ACLR_EXC                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1247
#define RSSPECAN_ATTR_BTO_MCH_DF_AVER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1248
#define RSSPECAN_ATTR_BTO_MCH_DF_MAX                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1249
#define RSSPECAN_ATTR_BTO_MCH_RAT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1250
#define RSSPECAN_ATTR_BTO_MCH_DF2_PERC                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1251
#define RSSPECAN_ATTR_BTO_ICFT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1252
#define RSSPECAN_ATTR_BTO_CFDR_MAX                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1253
#define RSSPECAN_ATTR_BTO_CFDR_RATE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1254
#define RSSPECAN_ATTR_BTO_PLEN                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1255
#define RSSPECAN_ATTR_BTO_PTYP                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1256
#define RSSPECAN_ATTR_BTO_CFST_BLOCK_COUNT_VALUE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1257
#define RSSPECAN_ATTR_BTO_CFST_DEVM_RMS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1258
#define RSSPECAN_ATTR_BTO_CFST_DEVM_PEAK                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1259
#define RSSPECAN_ATTR_BTO_CFST_DEVM_99PCT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1260
#define RSSPECAN_ATTR_BTO_CFST_FERR_TOTAL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1261
#define RSSPECAN_ATTR_BTO_CFST_FERR_BLOCK                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1262
#define RSSPECAN_ATTR_BTO_CFST_FERR_INITIAL                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1263
#define RSSPECAN_ATTR_BTO_DPEN_PACKETS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1264
#define RSSPECAN_ATTR_BTO_DPEN_PACKETS_PASSED                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1265
#define RSSPECAN_ATTR_BTO_DPEN_BER                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1266
#define RSSPECAN_ATTR_BTO_IBS_EXC                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1267
#define RSSPECAN_ATTR_BTO_IBS_HADJ                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1268
#define RSSPECAN_ATTR_BTO_IBS_TXCHANNEL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1269
#define RSSPECAN_ATTR_BTO_RTP                                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1270
#define RSSPECAN_ATTR_BTO_RTP_RATIO                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1271
#define RSSPECAN_ATTR_PMET_SELECT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2381
#define RSSPECAN_ATTR_PMET_REL                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1272
#define RSSPECAN_ATTR_PMET_REL_AUTO                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1273
#define RSSPECAN_ATTR_PWM_RELATIVE_OFFSET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1274
#define RSSPECAN_ATTR_PMET_FREQ                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1275
#define RSSPECAN_ATTR_PMET_MEAS_TIME                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1276
#define RSSPECAN_ATTR_PMET_COUPLING                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1277
#define RSSPECAN_ATTR_PMET_ZERO                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1278
#define RSSPECAN_ATTR_PMET_UNIT_ABS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1279
#define RSSPECAN_ATTR_PMET_UNIT_REL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1280
#define RSSPECAN_ATTR_PMET_STATE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1281
#define RSSPECAN_ATTR_PMET_AVERAGE_AUTO                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1282
#define RSSPECAN_ATTR_PMET_AVERAGE_COUNT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1283
#define RSSPECAN_ATTR_PMET_REF_LEVEL_OFFSET_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1284
#define RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_STATE                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1285
#define RSSPECAN_ATTR_PWR_EXTERNAL_POWER_TRIGGER_LEVEL                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1286
#define RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_HYSTERESIS                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2634
#define RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_DROPOUT_TIME               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2635
#define RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_HOLDOFF_TIME               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2636
#define RSSPECAN_ATTR_PWM_EXTERNAL_POWER_TRIGGER_SLOPE                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2637
#define RSSPECAN_ATTR_PMET_DUTY_CYCLE_STATE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1287
#define RSSPECAN_ATTR_PMET_DUTY_CYCLE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1288
#define RSSPECAN_ATTR_PMET_AUTOMATIC_ASSIGNMENT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2382
#define RSSPECAN_ATTR_PMET_NUMBER_OF_SENSORS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2383
#define RSSPECAN_ATTR_PMET_BARGRAPH_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1291
#define RSSPECAN_ATTR_PMET_RELATIVE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1292
#define RSSPECAN_ATTR_PMET_READ                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1294
#define RSSPECAN_ATTR_PMET_FETCH                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1295
#define RSSPECAN_ATTR_GSM_K10_MODE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1296
#define RSSPECAN_ATTR_GSM_K10_MODE_FSV                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1297
#define RSSPECAN_ATTR_GSM_K10_DUT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1298
#define RSSPECAN_ATTR_GSM_K10_SIG_CHAR_MODE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1299
#define RSSPECAN_ATTR_GSM_K10_SIG_CHAR_BAND                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1300
#define RSSPECAN_ATTR_GSM_K10_SIG_CHAR_BAND_ARFC                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1301
#define RSSPECAN_ATTR_GSM_K10_AUTO_PWR_LEV                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1302
#define RSSPECAN_ATTR_GSM_K10_AUTO_PWR_LEV_ONCE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1303
#define RSSPECAN_ATTR_GSM_K10_LEVEL_SETTINGS_PWR_CLASS                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1304
#define RSSPECAN_ATTR_GSM_K10_LEVEL_SETTINGS_POW_STAT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1305
#define RSSPECAN_ATTR_GSM_K10_STATISTIC_COUNT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2773
#define RSSPECAN_ATTR_GSM_K10_TRIGGER_TO_SYNC_ACQUISITIONS                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3078
#define RSSPECAN_ATTR_GSM_K10_CORRELATION_THRESHOLD                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2384
#define RSSPECAN_ATTR_GSM_K10_SYNCHRONIZATION                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1306
#define RSSPECAN_ATTR_GSM_K10_MEASURE_ONLY_ON_SYNC                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1307
#define RSSPECAN_ATTR_GSM_K10_CAP_SET_TRACK_TIME                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1308
#define RSSPECAN_ATTR_GSM_K10_IQ_SWAP                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2385
#define RSSPECAN_ATTR_GSM_K10_MARKER_ZOOM_FACTOR                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2386
#define RSSPECAN_ATTR_GSM_K10_RESULT_DISPLAY                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2387
#define RSSPECAN_ATTR_GSM_K10_AUTO_SET_LEVEL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1309
#define RSSPECAN_ATTR_GSM_K10_AUTO_SET_LEVEL_ONCE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1310
#define RSSPECAN_ATTR_GSM_K10_AUTO_SET_FRAME                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1311
#define RSSPECAN_ATTR_GSM_K10_AUTO_SET_FRAME_ONCE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1312
#define RSSPECAN_ATTR_GSM_K10_AUTO_SET_TRIGGER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1313
#define RSSPECAN_ATTR_GSM_K10_AUTO_SET_TRIGGER_ONCE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1314
#define RSSPECAN_ATTR_GSM_K10_DEMOD_SINGLE_SLOT_MEAS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1315
#define RSSPECAN_ATTR_GSM_K10_DEM_MULTI_SLOT_MEAS_SLOT_NUM                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1316
#define RSSPECAN_ATTR_GSM_K10_DEM_MULTI_SLOT_MEAS_FIRST_SLOT                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1317
#define RSSPECAN_ATTR_GSM_K10_DEMODULATION_EQUAL_TIMESLOT_LENGTH            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1318
#define RSSPECAN_ATTR_GSM_K10_BURST_SLOT_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1319
#define RSSPECAN_ATTR_GSM_K10_BURST_TYPE_BURST                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1320
#define RSSPECAN_ATTR_GSM_K10_BURST_PCL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1321
#define RSSPECAN_ATTR_GSM_K10_BURST_MOD                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1322
#define RSSPECAN_ATTR_GSM_K10_BURST_FILTER                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1323
#define RSSPECAN_ATTR_GSM_K10_BURST_TSC                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1324
#define RSSPECAN_ATTR_GSM_K10_BURST_TSC_USER_VALUE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2388
#define RSSPECAN_ATTR_GSM_K10_BURST_TIMING_ADVANCE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2582
#define RSSPECAN_ATTR_GSM_K10_BURST_SYNC                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2583
#define RSSPECAN_ATTR_GSM_K10_BURST_SCPIR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2584
#define RSSPECAN_ATTR_GSM_K10_BURST_SUBCH_TSC                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2585
#define RSSPECAN_ATTR_GSM_K10_BURST_SUBCH_TSC_USER_VALUE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2586
#define RSSPECAN_ATTR_GSM_K10_TRACE_TYPE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2587
#define RSSPECAN_ATTR_GSM_K10_MEAS_BURST_TYPE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1325
#define RSSPECAN_ATTR_GSM_K10_MEAS_SPECT_TYPE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1326
#define RSSPECAN_ATTR_GSM_K10_MEASUREMENT_WIDE_SPECTRUM                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2389
#define RSSPECAN_ATTR_GSM_K10_WIDE_SPECTRUM_UNIT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2638
#define RSSPECAN_ATTR_GSM_K10_MEASUREMENT_TRIGGER_TO_SYNC                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2639
#define RSSPECAN_ATTR_GSM_K10_TRIGGER_TO_SYNC_BINS_COUNT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2640
#define RSSPECAN_ATTR_GSM_K10_TRIGGER_TO_SYNC_ADAPTIVE_DATA_SIZE            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2641
#define RSSPECAN_ATTR_GSM_K10_PWR_VS_TIME_MEAS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1327
#define RSSPECAN_ATTR_GSM_K10_POWER_VS_TIME_FILTER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1328
#define RSSPECAN_ATTR_GSM_K10_POWER_VS_TIME_ALIGNMENT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2390
#define RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_ENABLE_LEFT_LIMIT RS_SPECIFIC_PUBLIC_ATTR_BASE + 1329
#define RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_ENABLE_RIGHT_LIMIT RS_SPECIFIC_PUBLIC_ATTR_BASE + 1330
#define RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_FILTER_TYPE     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2774
#define RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_REF_POWER       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2775
#define RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_HIGH_DYNAMIC    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2776
#define RSSPECAN_ATTR_GSM_K10_MODULATION_TRANSIENT_SPECTRUM_FREQUENCY_LIST  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2777
#define RSSPECAN_ATTR_GSM_K10_DEMODULATION_SYMBOL_DECISION                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2588
#define RSSPECAN_ATTR_GSM_K10_DEMODULATION_TAIL_TSC_BITS                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2589
#define RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2590
#define RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_POWER_VS_TIME                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2591
#define RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_DEMODULATION                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2592
#define RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_CONSTELLATION                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2593
#define RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_MODULATION_SPECTRUM                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2594
#define RSSPECAN_ATTR_GSM_K10_MULTI_MEAS_TRANSIENT_SPECTRUM                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2595
#define RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_BTS                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1331
#define RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_NUMBER_OF_ACTIVE_CHANNELS       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1332
#define RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_BTS_CLASS                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1333
#define RSSPECAN_ATTR_GSM_K10_MULTI_CARRIER_FILTER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1334
#define RSSPECAN_ATTR_GSM_K10_REFRESH_CAPTURED_DATA                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2778
#define RSSPECAN_ATTR_READ_GSM_K10_MEAS_RES                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1335
#define RSSPECAN_ATTR_READ_GSM_K10_MEAS_RES_95_PERC                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1336
#define RSSPECAN_ATTR_FETCH_GSM_K10_MEAS_RES                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1337
#define RSSPECAN_ATTR_FETCH_GSM_K10_MEAS_RES_95_PERC                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1338
#define RSSPECAN_ATTR_READ_GSM_K10_TRIGGER_TO_SYNC_RESULTS                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2642
#define RSSPECAN_ATTR_FETCH_GSM_K10_TRIGGER_TO_SYNC_RESULTS                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2643
#define RSSPECAN_ATTR_READ_GSM_K10_BURST_SLOT_RES                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1339
#define RSSPECAN_ATTR_FETCH_GSM_K10_BURST_SLOT_RES                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1340
#define RSSPECAN_ATTR_READ_GSM_K10_BURST_SLOT_DELTA_TO_SYNC                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2391
#define RSSPECAN_ATTR_FETCH_GSM_K10_BURST_SLOT_DELTA_TO_SYNC                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2392
#define RSSPECAN_ATTR_REALTIME_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2448
#define RSSPECAN_ATTR_SPEM_WINDOW                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2393
#define RSSPECAN_ATTR_REALTIME_MEASUREMENT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3159
#define RSSPECAN_ATTR_SPEM_STATE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1341
#define RSSPECAN_ATTR_SPEM_COLOR                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1342
#define RSSPECAN_ATTR_SPEM_SIZE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1343
#define RSSPECAN_ATTR_SPEM_SHAPE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2394
#define RSSPECAN_ATTR_SPEM_COLOR_STYLE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2395
#define RSSPECAN_ATTR_SPEM_COLOR_LOW                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2396
#define RSSPECAN_ATTR_SPEM_COLOR_UPPER                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2397
#define RSSPECAN_ATTR_SPEM_COLOR_DEFAULT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2398
#define RSSPECAN_ATTR_SPEM_HDEP                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1344
#define RSSPECAN_ATTR_SPEM_TIME_STAMP_STATE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1345
#define RSSPECAN_ATTR_SPEM_FRAME_COUNT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1346
#define RSSPECAN_ATTR_SPEM_FRAME_SELECT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1347
#define RSSPECAN_ATTR_SPEM_CONTINUOUS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1348
#define RSSPECAN_ATTR_SPEM_CLEAR                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1349
#define RSSPECAN_ATTR_SPEM_FFT_WINDOW                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2570
#define RSSPECAN_ATTR_FILE_ITEMS_SAVE_RECAL_SPEM                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1350
#define RSSPECAN_ATTR_SPEM_CONTINUOUS_TRIGGER                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2571
#define RSSPECAN_ATTR_SPEM_MARK_FRAME_NUMBER                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1351
#define RSSPECAN_ATTR_SPEM_MARKER_SEARCH_AREA                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1352
#define RSSPECAN_ATTR_SPEM_MARKER_SEARCH_XY                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1353
#define RSSPECAN_ATTR_SPEM_MARKER_SEARCH_Y                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1354
#define RSSPECAN_ATTR_SPEM_DELT_FRAME_NUMBER                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1355
#define RSSPECAN_ATTR_SPEM_DELT_SEARCH_AREA                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1356
#define RSSPECAN_ATTR_SPEM_DELT_SEARCH_XY                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1357
#define RSSPECAN_ATTR_SPEM_DELT_SEARCH_Y                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1358
#define RSSPECAN_ATTR_MARKER_SEARCH_Y_TRIGGER                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2572
#define RSSPECAN_ATTR_PSPEM_STATE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2399
#define RSSPECAN_ATTR_PSPEM_DURATION                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2400
#define RSSPECAN_ATTR_PSPEM_GRAN                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2401
#define RSSPECAN_ATTR_PSPEM_SHAPE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2402
#define RSSPECAN_ATTR_PSPEM_TRUN                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2403
#define RSSPECAN_ATTR_PSPEM_COLOR_STYLE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2404
#define RSSPECAN_ATTR_PSPEM_COLOR_DEFAULT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2405
#define RSSPECAN_ATTR_PSPEM_COLOR_LOWER                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2406
#define RSSPECAN_ATTR_PSPEM_COLOR_UPPER                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2407
#define RSSPECAN_ATTR_PSPEM_TRACE_STYLE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2408
#define RSSPECAN_ATTR_PSPEM_MAXH_TRACE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2409
#define RSSPECAN_ATTR_PSPEM_MAXH_INTENS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2410
#define RSSPECAN_ATTR_PSPEM_MAXH_RESET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2411
#define RSSPECAN_ATTR_PSPEM_MARKER_POSITION                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2596
#define RSSPECAN_ATTR_QUERY_PSPEM_MARKER_Z_AXIS                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2597
#define RSSPECAN_ATTR_PSPEM_DELTA_MARKER_POSITION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2598
#define RSSPECAN_ATTR_QUERY_PSPEM_DELTA_Z_AXIS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2412
#define RSSPECAN_ATTR_RESULT_DISPLAY                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2415
#define RSSPECAN_ATTR_SPEM_MASK_TRIGGER_SOURCE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2573
#define RSSPECAN_ATTR_SPEM_MASK_TRIGGER_NAME                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2416
#define RSSPECAN_ATTR_SPEM_MASK_TRIGGER_COMMENT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2417
#define RSSPECAN_ATTR_SPEM_MASK_TRIGGER_DIRECTORY                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2599
#define RSSPECAN_ATTR_SPEM_MASK_TRIGGER_DELETE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2418
#define RSSPECAN_ATTR_SPEM_MASK_TRIGG_CONDITION                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2419
#define RSSPECAN_ATTR_SPEM_MASK_TRIGGER_MODE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2420
#define RSSPECAN_ATTR_SPEM_MASK_UPP_AUTO                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2421
#define RSSPECAN_ATTR_SPEM_MASK_SHIFT_LOW_X                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2422
#define RSSPECAN_ATTR_SPEM_MASK_SHIFT_LOW_Y                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2423
#define RSSPECAN_ATTR_SPEM_MASK_SHIFT_UPP_X                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2424
#define RSSPECAN_ATTR_SPEM_MASK_SHIFT_UPP_Y                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2425
#define RSSPECAN_ATTR_SPEM_STORE_TO_FILE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2574
#define RSSPECAN_ATTR_SPEM_TIME_DOMAIN_TRIGGER                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2575
#define RSSPECAN_ATTR_SPEM_TIME_DOMAIN_TRIGGER_LEVEL                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2576
#define RSSPECAN_ATTR_AVI_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1359
#define RSSPECAN_ATTR_AVI_DEMOD_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1360
#define RSSPECAN_ATTR_AVI_DISTORTION_RESULT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1361
#define RSSPECAN_ATTR_AVI_DDM_UNIT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1362
#define RSSPECAN_ATTR_AVI_INPUT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1363
#define RSSPECAN_ATTR_AVI_DEMOD_BWID_AUTO                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1364
#define RSSPECAN_ATTR_AVI_DEMOD_BWID                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1365
#define RSSPECAN_ATTR_AVI_HARM_DIST_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1368
#define RSSPECAN_ATTR_AVI_HARM_DIST_FREQ                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1369
#define RSSPECAN_ATTR_AVI_THD_FUND_FREQ                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1370
#define RSSPECAN_ATTR_AVI_THD_MAX_FREQ                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1371
#define RSSPECAN_ATTR_AVI_RF_FREQ_RESULT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1372
#define RSSPECAN_ATTR_AVI_CARR_OFFS_RESULT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1373
#define RSSPECAN_ATTR_AVI_RF_LEVEL_RESULT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1374
#define RSSPECAN_ATTR_AVI_DDM_RESULT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1375
#define RSSPECAN_ATTR_AVI_FM_DEVIATION_RESULT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1376
#define RSSPECAN_ATTR_AVI_FM_FREQ_RESULT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1377
#define RSSPECAN_ATTR_AVI_PHASE_RESULT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1378
#define RSSPECAN_ATTR_AVI_INPUT_LEVEL_RESULT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1379
#define RSSPECAN_ATTR_AVI_SDM_RESULT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1380
#define RSSPECAN_ATTR_AVI_SHD_RESULT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1381
#define RSSPECAN_ATTR_MCGD_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2913
#define RSSPECAN_ATTR_MCGD_VERTICAL_SCALE_MAX_MIN                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2915
#define RSSPECAN_ATTR_MCGD_RESULT_PHASE_UNIT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2916
#define RSSPECAN_ATTR_MCGD_CARRIER_COUNT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2917
#define RSSPECAN_ATTR_MCGD_CARRIER_SPACING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2918
#define RSSPECAN_ATTR_MCGD_MEASUREMENT_TIME_AUTO                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2919
#define RSSPECAN_ATTR_MCGD_MEASUREMENT_TIME                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2920
#define RSSPECAN_ATTR_MCGD_MODE_ABS_REL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2921
#define RSSPECAN_ATTR_MCGD_CARRIER_OFFSET_MODE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2922
#define RSSPECAN_ATTR_MCGD_CARRIER_OFFSET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2923
#define RSSPECAN_ATTR_MCGD_INITIATE_CALIBRATION                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2924
#define RSSPECAN_ATTR_MCGD_CALIBRATION_STATUS                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2925
#define RSSPECAN_ATTR_MCGD_STORE_CALIBRATION_DATA                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2926
#define RSSPECAN_ATTR_MCGD_LOAD_CALIBRATION_DATA                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2927
#define RSSPECAN_ATTR_MCGD_STORE_MEASUREMENT_RESULTS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2928
#define RSSPECAN_ATTR_MCGD_TRACE_POINTS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2929
#define RSSPECAN_ATTR_MCGD_BANDWIDTH                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2930
#define RSSPECAN_ATTR_MCGD_RECORD_LENGTH                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2931
#define RSSPECAN_ATTR_MCGD_SAMPLE_RATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2932
#define RSSPECAN_ATTR_CATV_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1382
#define RSSPECAN_ATTR_CATV_Y_SCALE_AUTO                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1383
#define RSSPECAN_ATTR_CATV_EPAT_ZOOM_STATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1384
#define RSSPECAN_ATTR_CATV_EPAT_ZOOM_FACTOR                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1385
#define RSSPECAN_ATTR_CATV_MERR_ZOOM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1386
#define RSSPECAN_ATTR_CATV_OVER_ZOOM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1387
#define RSSPECAN_ATTR_CATV_QUAD_ZOOM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1388
#define RSSPECAN_ATTR_CATV_TRACE_MODE_FREEZE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1389
#define RSSPECAN_ATTR_CATV_CHANNEL                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1390
#define RSSPECAN_ATTR_CATV_RF_FREQUENCY                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1391
#define RSSPECAN_ATTR_CATV_RF_FREQUENCY_STEP_SIZE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1392
#define RSSPECAN_ATTR_CATV_SWEEP_SPACING                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1393
#define RSSPECAN_ATTR_CATV_SIDE_BAND                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1394
#define RSSPECAN_ATTR_CATV_MODUL_STANDARD                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1395
#define RSSPECAN_ATTR_CATV_MODUL_STANDARD_GDELAY                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1396
#define RSSPECAN_ATTR_CATV_MODUL_STANDARD_SIG_TYPE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1397
#define RSSPECAN_ATTR_CATV_ATV_MEAS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1398
#define RSSPECAN_ATTR_CATV_ATV_CARR_MEAS                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1399
#define RSSPECAN_ATTR_CATV_ATV_MEAS_MODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1400
#define RSSPECAN_ATTR_CATV_ATV_BWID                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1401
#define RSSPECAN_ATTR_CATV_ATV_FREQ_NEXT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1402
#define RSSPECAN_ATTR_CATV_ATV_NOISE_FLOOR_CORRECTION                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1403
#define RSSPECAN_ATTR_CATV_ATV_REF_CHANNEL_MANUAL                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1404
#define RSSPECAN_ATTR_CATV_ATV_REF_POWER_MANUAL                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1405
#define RSSPECAN_ATTR_CATV_ATV_REF_POWER_MODE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1406
#define RSSPECAN_ATTR_CATV_ATV_HUM_UNIT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1407
#define RSSPECAN_ATTR_CATV_ATV_VCP_UNIT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1408
#define RSSPECAN_ATTR_CATV_ATV_STANDARD                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1409
#define RSSPECAN_ATTR_CATV_ATV_SOUND_SYSTEM                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1410
#define RSSPECAN_ATTR_CATV_ATV_COLOR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1411
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_BAR_FIELD                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1412
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_BAR_LINE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1413
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_BAR_LINE_TYPE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1414
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_VIDEO_SCOPE_FIELD                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1415
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_VIDEO_SCOPE_LINE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1416
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_QLINE_FIELD                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1417
#define RSSPECAN_ATTR_CATV_ATV_TRIGGER_QUIET_LINE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1418
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_FREQ_OFFSET                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1419
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_REL_POWER                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1420
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCF                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1421
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCP                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1422
#define RSSPECAN_ATTR_CATV_ATV_LIM_CN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1423
#define RSSPECAN_ATTR_CATV_ATV_LIM_CSO                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1424
#define RSSPECAN_ATTR_CATV_ATV_LIM_CTB                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1425
#define RSSPECAN_ATTR_CATV_ATV_LIM_HUM_LOW                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1426
#define RSSPECAN_ATTR_CATV_ATV_LIM_HUM_UPP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1427
#define RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_MDEP                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1428
#define RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_RPC                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1429
#define RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_VCP                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1430
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_FREQ_OFFSET_RESULT                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1431
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_REL_POWER_RESULT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1432
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCF_RESULT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1433
#define RSSPECAN_ATTR_CATV_ATV_CARR_LIM_VCP_RESULT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1434
#define RSSPECAN_ATTR_CATV_ATV_LIM_CN_RESULT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1435
#define RSSPECAN_ATTR_CATV_ATV_LIM_CSO_RESULT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1436
#define RSSPECAN_ATTR_CATV_ATV_LIM_CTB_RESULT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1437
#define RSSPECAN_ATTR_CATV_ATV_LIM_HUM_RESULT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1438
#define RSSPECAN_ATTR_CATV_ATV_LIM_VMOD_MDEP_RESULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1439
#define RSSPECAN_ATTR_CATV_ATV_CARR_VCPA                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1440
#define RSSPECAN_ATTR_CATV_ATV_CARR_VCF_VALUE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1441
#define RSSPECAN_ATTR_CATV_ATV_CARR_REL_POWER_VALUE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1442
#define RSSPECAN_ATTR_CATV_ATV_CARR_FREQ_OFFSET_VALUE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1443
#define RSSPECAN_ATTR_CATV_ATV_VALUE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1444
#define RSSPECAN_ATTR_CATV_ATV_CN_CN_VALUE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1445
#define RSSPECAN_ATTR_CATV_ATV_CSO_CSO_VALUE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1446
#define RSSPECAN_ATTR_CATV_ATV_CTB_CTB_VALUE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1447
#define RSSPECAN_ATTR_CATV_ATV_HUM_VALUE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1448
#define RSSPECAN_ATTR_CATV_ATV_VMOD_RESULT_VALUE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1449
#define RSSPECAN_ATTR_CATV_DTV_MEAS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1450
#define RSSPECAN_ATTR_CATV_DTV_SATT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1451
#define RSSPECAN_ATTR_CATV_DTV_EPAT_UNIT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1452
#define RSSPECAN_ATTR_CATV_DTV_EPAT_RVEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1453
#define RSSPECAN_ATTR_CATV_DTV_ERROR_UNIT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1454
#define RSSPECAN_ATTR_CATV_DTV_EQUALIZER_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1455
#define RSSPECAN_ATTR_CATV_DTV_EQUALIZER_FREEZE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1456
#define RSSPECAN_ATTR_CATV_DTV_EQUALIZER_RESET                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1457
#define RSSPECAN_ATTR_CATV_DTV_FILTER_ALPHA                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1458
#define RSSPECAN_ATTR_CATV_DTV_SRATE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1459
#define RSSPECAN_ATTR_CATV_DTV_STANDARD                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1460
#define RSSPECAN_ATTR_CATV_DTV_FORMAT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1461
#define RSSPECAN_ATTR_CATV_DTV_CARR_LIM_FREQ_OFFSET                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1462
#define RSSPECAN_ATTR_CATV_DTV_LIM_CHANNEL_POWER                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1463
#define RSSPECAN_ATTR_CATV_DTV_LIM_SUPP                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1464
#define RSSPECAN_ATTR_CATV_DTV_LIM_ERR_LOW                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1465
#define RSSPECAN_ATTR_CATV_DTV_LIM_ERR_UPP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1466
#define RSSPECAN_ATTR_CATV_DTV_LIM_IMB                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1467
#define RSSPECAN_ATTR_CATV_DTV_LIM_PJIT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1468
#define RSSPECAN_ATTR_CATV_DTV_LIM_QERR                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1469
#define RSSPECAN_ATTR_CATV_DTV_LIM_SYMB_RATE_OFFSET                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1470
#define RSSPECAN_ATTR_CATV_DTV_CARR_LIM_FREQ_OFFSET_RESULT                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1471
#define RSSPECAN_ATTR_CATV_DTV_LIM_CHANNEL_POWER_RESULT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1472
#define RSSPECAN_ATTR_CATV_DTV_CARR_LIM_CARR_SUPP_RESULT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1473
#define RSSPECAN_ATTR_CATV_DTV_LIM_ERROR_RESULT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1474
#define RSSPECAN_ATTR_CATV_DTV_LIM_IMB_RESULT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1475
#define RSSPECAN_ATTR_CATV_DTV_LIM_PJIT_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1476
#define RSSPECAN_ATTR_CATV_DTV_LIM_QERR_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1477
#define RSSPECAN_ATTR_CATV_DTV_LIM_SYMB_RATE_OFFSET_RESULT                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1478
#define RSSPECAN_ATTR_CATV_DTV_SPEC                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1479
#define RSSPECAN_ATTR_CATV_DTV_ERROR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1480
#define RSSPECAN_ATTR_CATV_DTV_OVER_CARR_FREQ_OFFSET                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1481
#define RSSPECAN_ATTR_CATV_DTV_OVER_SRATE_OFFSET                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1482
#define RSSPECAN_ATTR_CATV_DTV_MERR_IMB                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1483
#define RSSPECAN_ATTR_CATV_DTV_MERR_QERR                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1484
#define RSSPECAN_ATTR_CATV_DTV_MERR_CSUP                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1485
#define RSSPECAN_ATTR_CATV_DTV_MERR_PJIT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1486
#define RSSPECAN_ATTR_CATV_DTV_CHANNEL_POWER_RESULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1487
#define RSSPECAN_ATTR_CATV_TV_MEAS                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1488
#define RSSPECAN_ATTR_CATV_TV_CHANNEL                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1489
#define RSSPECAN_ATTR_CATV_TV_MOD_STANDARD_TILT_ACTIVATE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1490
#define RSSPECAN_ATTR_CATV_TV_MOD_STANDARD_TILT_DEACTIVATE                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1491
#define RSSPECAN_ATTR_NOISE_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1492
#define RSSPECAN_ATTR_NOISE_CALC_LIM_TRAC                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1493
#define RSSPECAN_ATTR_NOISE_CONF_CORR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1494
#define RSSPECAN_ATTR_NOISE_CONF_LIST_CONT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1495
#define RSSPECAN_ATTR_NOISE_CONF_LIST_SING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1496
#define RSSPECAN_ATTR_NOISE_CONF_SING                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1497
#define RSSPECAN_ATTR_NOISE_CONF_FREQ_CONT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2426
#define RSSPECAN_ATTR_NOISE_CONF_FREQ_SING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2427
#define RSSPECAN_ATTR_NOISE_SINGLE_FREQUENCY                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 3079
#define RSSPECAN_ATTR_NOISE_SINGLE_FREQUENCY_COUPLING                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3080
#define RSSPECAN_ATTR_NOISE_INP_GAIN_STAT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1499
#define RSSPECAN_ATTR_NOISE_CORR_ENR_MODE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1500
#define RSSPECAN_ATTR_NOISE_CORR_ENR_SPOT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1501
#define RSSPECAN_ATTR_NOISE_CORR_TEMP                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1502
#define RSSPECAN_ATTR_NOISE_CORR_ENR_TABLE_SELECT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2879
#define RSSPECAN_ATTR_NOISE_CORR_ENR_TABLE_DELETE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2880
#define RSSPECAN_ATTR_NOISE_CORR_ENR_COMMON_MODE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2991
#define RSSPECAN_ATTR_NOISE_CORR_ENR_CALIBRATION_MODE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2992
#define RSSPECAN_ATTR_NOISE_CORR_ENR_CALIBRATION_SPOT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2993
#define RSSPECAN_ATTR_NOISE_CORR_ENR_CALIBRATION_TABLE_SELECT               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2994
#define RSSPECAN_ATTR_NOISE_CORR_LOSS_MODE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1503
#define RSSPECAN_ATTR_NOISE_CORR_LOSS_SPOT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1504
#define RSSPECAN_ATTR_NOISE_CORR_LOSS_INPUT_TABLE_SELECT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2881
#define RSSPECAN_ATTR_NOISE_CORR_LOSS_INPUT_TABLE_DELETE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2882
#define RSSPECAN_ATTR_NOISE_CORR_LOSS_OUTPUT_TABLE_SELECT                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2883
#define RSSPECAN_ATTR_NOISE_CORR_LOSS_OUTPUT_TABLE_DELETE                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2884
#define RSSPECAN_ATTR_NOISE_CORR_EREJ                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1505
#define RSSPECAN_ATTR_NOISE_CORR_STAT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1506
#define RSSPECAN_ATTR_NOISE_FREQ_STEP                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1507
#define RSSPECAN_ATTR_NOISE_FREQ_FIXED                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1508
#define RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1509
#define RSSPECAN_ATTR_NOISE_CONF_SYST_LOSC_FREQ                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1510
#define RSSPECAN_ATTR_NOISE_CONF_SYST_INT_FREQ                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1511
#define RSSPECAN_ATTR_NOISE_CONF_MODE_DUT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1512
#define RSSPECAN_ATTR_NOISE_EXT_FREQ_OFFS                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1513
#define RSSPECAN_ATTR_NOISE_EXT_FREQ_NUM                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1514
#define RSSPECAN_ATTR_NOISE_EXT_FREQ_DEN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1515
#define RSSPECAN_ATTR_NOISE_EXT_POW                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1516
#define RSSPECAN_ATTR_NOISE_GPIB_GEN_ADDR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1517
#define RSSPECAN_ATTR_NOISE_GEN_TYPE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1518
#define RSSPECAN_ATTR_NOISE_CONF_GEN_CONT_STAT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1519
#define RSSPECAN_ATTR_NOISE_CONF_GEN_INIT_AUTO                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1520
#define RSSPECAN_ATTR_NOISE_CONF_GEN_AUTO_SWITCH_OFF                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2600
#define RSSPECAN_ATTR_NOISE_CONF_DUT_GAIN                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1521
#define RSSPECAN_ATTR_NOISE_CONF_DUT_SETT_TIME                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1522
#define RSSPECAN_ATTR_NOISE_ARRAY_MEMORY                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1523
#define RSSPECAN_ATTR_NOISE_INIT_GENERATOR                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2601
#define RSSPECAN_ATTR_NOISE_ASSIGN_MARKER_TO_TRACE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2644
#define RSSPECAN_ATTR_NOISE_CALC_LIMIT_RESULT_TYPE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2885
#define RSSPECAN_ATTR_NOISE_MARKER_AMPLITUDE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2886
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_COMMON_SOURCE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3110
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_SOURCE_VSWR                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2971
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_SOURCE_RETURN_LOSS                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2972
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_ERN                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2973
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_SOURCE_VSWR             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3111
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_SOURCE_RETURN_LOSS      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3112
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_CALIBRATION_ERN                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3113
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_INPUT_VSWR                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2974
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_INPUT_RETURN_LOSS               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2975
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_OUTPUT_VSWR                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2976
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_DUT_OUTPUT_RETURN_LOSS              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2977
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_AUTO                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2978
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_NOISE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2979
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_GAIN                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2980
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_FREQUENCY                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2981
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_STATE                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2982
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_NOISE                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2983
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_GAIN                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2984
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_VSWR                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2985
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_PREAMPLIFIER_RETURN_LOSS            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2986
#define RSSPECAN_ATTR_NOISE_UNCERTAINTY_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2987
#define RSSPECAN_ATTR_SPEC_ANALYZER_UNCERTAINTY_GAIN                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2995
#define RSSPECAN_ATTR_SPEC_ANALYZER_UNCERTAINTY_NOISE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2996
#define RSSPECAN_ATTR_NOISE_WINDOW_SELECT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2933
#define RSSPECAN_ATTR_NOISE_DISP_DATA_TRAC                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1524
#define RSSPECAN_ATTR_NOISE_DISP_TRAC_STAT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1525
#define RSSPECAN_ATTR_NOISE_DISP_SYMB                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1526
#define RSSPECAN_ATTR_NOISE_DISP_AUTO_SCAL                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1527
#define RSSPECAN_ATTR_NOISE_DISP_TRAC_BOTTOM                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1528
#define RSSPECAN_ATTR_NOISE_DISP_TRAC_TOP                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1529
#define RSSPECAN_ATTR_NOISE_DISP_TRAC_REF_LEV_STAT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1530
#define RSSPECAN_ATTR_NOISE_DISP_TABLE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1531
#define RSSPECAN_ATTR_NOISE_DISP_TRACE_MEM                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1532
#define RSSPECAN_ATTR_NOISE_DISPL_CURR_RESULTS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1533
#define RSSPECAN_ATTR_NOISE_DISP_X_AXIS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1534
#define RSSPECAN_ATTR_PHASE_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1535
#define RSSPECAN_ATTR_PHASE_SWEEP_COUNT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1536
#define RSSPECAN_ATTR_PHASE_CONF_POW_AUTO                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1537
#define RSSPECAN_ATTR_PHASE_BAND_TYPE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1538
#define RSSPECAN_ATTR_PHASE_BAND_RAT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1539
#define RSSPECAN_ATTR_PHASE_CONF_POW_EXP_RF                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1540
#define RSSPECAN_ATTR_PHASE_SWEEP_MODE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1541
#define RSSPECAN_ATTR_PHASE_SWE_FORW                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1542
#define RSSPECAN_ATTR_PHASE_SIGNAL_VERIFICATION_FAILED                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2812
#define RSSPECAN_ATTR_PHASE_LIST_RANG_BAND                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1543
#define RSSPECAN_ATTR_PHASE_LIST_RANG_FILT_TYPE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1544
#define RSSPECAN_ATTR_PHASE_LIST_RANG_SWE_COUN                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1545
#define RSSPECAN_ATTR_PHASE_AM_REJECTION                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2934
#define RSSPECAN_ATTR_PHASE_TRACKING_BANDWIDTH                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2988
#define RSSPECAN_ATTR_PHASE_DIGITAL_PLL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3122
#define RSSPECAN_ATTR_PHASE_DECIMATION                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3123
#define RSSPECAN_ATTR_PHASE_FREQ_VER                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1546
#define RSSPECAN_ATTR_PHASE_FREQ_VER_TOL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1547
#define RSSPECAN_ATTR_PHASE_FREQ_VER_TOL_ABS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1548
#define RSSPECAN_ATTR_PHASE_FREQ_TRACK                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1549
#define RSSPECAN_ATTR_PHASE_POW_RLEV_VER                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1550
#define RSSPECAN_ATTR_PHASE_POW_RLEV_VER_TOL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1551
#define RSSPECAN_ATTR_PHASE_POW_TRACK                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1552
#define RSSPECAN_ATTR_PHASE_CALC_EVAL                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1553
#define RSSPECAN_ATTR_PHASE_CALC_EVAL_STAR                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1554
#define RSSPECAN_ATTR_PHASE_CALC_EVAL_STOP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1555
#define RSSPECAN_ATTR_PHASE_USER_TRACE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2813
#define RSSPECAN_ATTR_PHASE_USER_EVALUATE_FROM                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2814
#define RSSPECAN_ATTR_PHASE_USER_EVALUATE_TO                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2815
#define RSSPECAN_ATTR_PHASE_CALC_SNO_X                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1556
#define RSSPECAN_ATTR_PHASE_SPOT_NOISE_CALCULATION                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2816
#define RSSPECAN_ATTR_PHASE_CALC_SNO_Y                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1557
#define RSSPECAN_ATTR_PHASE_CALC_SNO_AOFF                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1558
#define RSSPECAN_ATTR_PHASE_CALC_SNO_STAT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1559
#define RSSPECAN_ATTR_PHASE_CONF_REFM                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1560
#define RSSPECAN_ATTR_PHASE_IQ_WINDOW_FUNCTION_PARTICULAR_DECADE            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2817
#define RSSPECAN_ATTR_PHASE_IQ_WINDOW_FUNCTION_ALL_DECADE                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2818
#define RSSPECAN_ATTR_PHASE_SPUR_SUPPRESSION                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2819
#define RSSPECAN_ATTR_PHASE_SPUR_THRESHOLD                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2820
#define RSSPECAN_ATTR_PHASE_NOISE_LIMIT_CORNER_FREQUENCY                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2794
#define RSSPECAN_ATTR_PHASE_NOISE_LIMIT_LEVEL                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2795
#define RSSPECAN_ATTR_PHASE_NOISE_LIMIT_SHAPE_TYPE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2797
#define RSSPECAN_ATTR_PHASE_NOISE_LIMIT_SLOPE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2798
#define RSSPECAN_ATTR_PHASE_COPY_TO_USER_LIMIT_LINE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3071
#define RSSPECAN_ATTR_PHASE_NOISE_LIMIT_CHECK_STATE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2799
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_Y                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1561
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1562
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_AUTO_ONCE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1563
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_RLEV                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1564
#define RSSPECAN_ATTR_PHASE_Y_AXIS_MANUAL_SCALING                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2821
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_BOTTOM                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2822
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_Y_RLEV_OFFS                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1565
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_SMO                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1566
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_SMO_APER                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1567
#define RSSPECAN_ATTR_PHASE_DISP_TRACE_SMO_TYPE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1568
#define RSSPECAN_ATTR_PHASE_X_AXIS_SCOPE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2823
#define RSSPECAN_ATTR_PHASE_X_AXIS_START_FREQUENCY                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2824
#define RSSPECAN_ATTR_PHASE_X_AXIS_STOP_FREQUENCY                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2825
#define RSSPECAN_ATTR_PHASE_X_AXIS_HALF_DECADE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2826
#define RSSPECAN_ATTR_PHASE_MEASURED_FREQUENCY                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2935
#define RSSPECAN_ATTR_PHASE_MEASURED_LEVEL                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2936
#define RSSPECAN_ATTR_PHASE_MEASURED_INTEGRATED_PHASE_NOISE                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3140
#define RSSPECAN_ATTR_PHASE_MEASURED_USER_INTEGRATED_PHASE_NOISE            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3141
#define RSSPECAN_ATTR_PHASE_MEASURED_DISCRETE_JITTER                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3142
#define RSSPECAN_ATTR_PHASE_MEASURED_RANDOM_JITTER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3143
#define RSSPECAN_ATTR_EMI_FMEAS_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2605
#define RSSPECAN_ATTR_EMI_FMEAS_MARKER_DETECTOR                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2606
#define RSSPECAN_ATTR_EMI_FMEAS_MARKER_PEAK_SEARCH                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2607
#define RSSPECAN_ATTR_EMI_FMEAS_DWELL_TIME                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2608
#define RSSPECAN_ATTR_EMI_FMEAS_MARKER_LIMIT_CONDITION                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2609
#define RSSPECAN_ATTR_EMI_FMEAS_MARKER_LIMIT_VERTICAL_DISTANCE              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2610
#define RSSPECAN_ATTR_EMI_FMEAS_MARKER_RESULT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2611
#define RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_DETECTOR                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2612
#define RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_PEAK_SEARCH                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2779
#define RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_LIMIT_CONDITION                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2613
#define RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_LIMIT_VERTICAL_DISTANCE        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2614
#define RSSPECAN_ATTR_EMI_FMEAS_DELTA_MARKER_RESULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2615
#define RSSPECAN_ATTR_EMI_LISN_V_NETWORK_TYPE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2616
#define RSSPECAN_ATTR_EMI_LISN_V_NETWORK_HIGH_PASS_FILTER                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2617
#define RSSPECAN_ATTR_EMI_LISN_V_NETWORK_PHASE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2618
#define RSSPECAN_ATTR_EMI_RESOLUTION_BANDWIDTH_FILTER_TYPE                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2619
#define RSSPECAN_ATTR_VSA_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1569
#define RSSPECAN_ATTR_VSA_FACTORY_DEFAULTS                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1570
#define RSSPECAN_ATTR_VSA_REF_LEVEL_ADJUST                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1571
#define RSSPECAN_ATTR_VSA_SCREEN_COUPLING                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1572
#define RSSPECAN_ATTR_VSA_MARKER_SEARCH                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1573
#define RSSPECAN_ATTR_VSA_MARKER_PEAK_SEARCH                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1574
#define RSSPECAN_ATTR_VSA_ADJUST_REFERENCE_LEVEL_TO_FULL_SCALE_LEVEL        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1575
#define RSSPECAN_ATTR_VSA_FULL_SCALE_LEVEL_UNIT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1576
#define RSSPECAN_ATTR_VSA_PRESET_SCREENS                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1577
#define RSSPECAN_ATTR_VSA_RESULT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1578
#define RSSPECAN_ATTR_VSA_RESULT_FORMAT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1579
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1580
#define RSSPECAN_ATTR_VSA_FREQUENCY_STEP_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1581
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_SAVE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1582
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_GROUP                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1583
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_COMMENT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1584
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_PREFIX                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1585
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_DELETE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1586
#define RSSPECAN_ATTR_VSA_DIGITAL_STANDARD_DEFAULTS                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1587
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_AUTO                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1588
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1589
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_MODE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1590
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_AUTO                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1591
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_MODE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1592
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_THRESHOLD_LEVEL                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1593
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_HYSTERESIS_LEVEL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1594
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_LENGTH_MIN                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1595
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_LENGTH_MAX                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1596
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_SKIP_RISE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1597
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_SKIP_FALL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1598
#define RSSPECAN_ATTR_VSA_BURST_SEARCH_GLEN                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1599
#define RSSPECAN_ATTR_VSA_SEARCH_BURST_AUTO_CONFIGURATION                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1600
#define RSSPECAN_ATTR_VSA_SEARCH_BURST_TOLERANCE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1601
#define RSSPECAN_ATTR_VSA_SYNC_SEARCH_AUTO                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1602
#define RSSPECAN_ATTR_VSA_SYNC_SEARCH_STATE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1603
#define RSSPECAN_ATTR_VSA_SYNC_SEARCH_MODE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1604
#define RSSPECAN_ATTR_VSA_SYNC_NAME                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1605
#define RSSPECAN_ATTR_VSA_SYNC_TEXT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1606
#define RSSPECAN_ATTR_VSA_SYNC_COMMENT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1607
#define RSSPECAN_ATTR_VSA_SYNC_NSTAT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1608
#define RSSPECAN_ATTR_VSA_SYNC_PATT_ADD                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1609
#define RSSPECAN_ATTR_VSA_SYNC_DELETE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1610
#define RSSPECAN_ATTR_VSA_SYNC_PATT_REMOVE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1611
#define RSSPECAN_ATTR_VSA_SYNC_PATT_REMOVE_ALL                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1612
#define RSSPECAN_ATTR_VSA_SYNC_COPY                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1613
#define RSSPECAN_ATTR_VSA_SYNC_SEARCH_PATTERN_AUTO_CONFIGURATION            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1614
#define RSSPECAN_ATTR_VSA_SYNC_SEARCH_IQ_CORRELATION_THRESHOLD              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1615
#define RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_MODE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2630
#define RSSPECAN_ATTR_VSA_USE_PATTERN_FOR_SYNC_STATE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2633
#define RSSPECAN_ATTR_VSA_FINE_SYNC_AUTO                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2780
#define RSSPECAN_ATTR_VSA_FINE_SYNC_MODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2781
#define RSSPECAN_ATTR_VSA_FINE_SYNC_SER                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2782
#define RSSPECAN_ATTR_VSA_FINE_SYNC_RESULT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2783
#define RSSPECAN_ATTR_VSA_KNOWN_DATA_STATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2784
#define RSSPECAN_ATTR_VSA_KNOWN_DATA_FILE_NAME                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2785
#define RSSPECAN_ATTR_VSA_EQUALIZER_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1616
#define RSSPECAN_ATTR_VSA_EQUALIZER_LEARNING                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1617
#define RSSPECAN_ATTR_VSA_EQUALIZER_SPEED_CONVERSION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1618
#define RSSPECAN_ATTR_VSA_EQUALIZER_LENGTH                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1619
#define RSSPECAN_ATTR_VSA_EQUALIZER_LOAD                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1620
#define RSSPECAN_ATTR_VSA_EQUALIZER_SAVE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1621
#define RSSPECAN_ATTR_VSA_EQUALIZER_DELETE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1622
#define RSSPECAN_ATTR_VSA_EQUALIZER_RESET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1623
#define RSSPECAN_ATTR_VSA_SIGNAL_TYPE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1624
#define RSSPECAN_ATTR_VSA_PATTERN_SIGNAL_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1625
#define RSSPECAN_ATTR_VSA_PATTERN_OFFSET_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1626
#define RSSPECAN_ATTR_VSA_MODULATION_SRATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1627
#define RSSPECAN_ATTR_VSA_MODULATION_FORMAT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1628
#define RSSPECAN_ATTR_VSA_MODULATION_QPSK_FORMAT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1629
#define RSSPECAN_ATTR_VSA_MODULATION_PSK_FORMAT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1630
#define RSSPECAN_ATTR_VSA_MODULATION_MSK_FORMAT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1631
#define RSSPECAN_ATTR_VSA_MODULATION_QAM_FORMAT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1632
#define RSSPECAN_ATTR_VSA_MODULATION_PSK_NSTAT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1633
#define RSSPECAN_ATTR_VSA_MODULATION_QAM_NSTAT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1634
#define RSSPECAN_ATTR_VSA_MODULATION_FSK_NSTAT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1635
#define RSSPECAN_ATTR_VSA_MODULATION_VSB_NSTAT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1636
#define RSSPECAN_ATTR_VSA_MODULATION_UQAM_NSTAT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1637
#define RSSPECAN_ATTR_VSA_MODULATION_ASK_MODE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2945
#define RSSPECAN_ATTR_VSA_MODULATION_APSK_MODE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2946
#define RSSPECAN_ATTR_VSA_MODULATION_MAPPING                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1638
#define RSSPECAN_ATTR_VSA_MODULATION_UQAM_MAPPING                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1639
#define RSSPECAN_ATTR_VSA_LOAD_USER_MODULATION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2947
#define RSSPECAN_ATTR_VSA_MODULATION_FILTER_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1640
#define RSSPECAN_ATTR_VSA_MODULATION_ALPHABT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1641
#define RSSPECAN_ATTR_VSA_MODULATION_PRATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1642
#define RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV_COMP                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1643
#define RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1644
#define RSSPECAN_ATTR_VSA_MODULATION_FSK_DEV_REL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2663
#define RSSPECAN_ATTR_VSA_MODULATION_EVM_CALCULATION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1645
#define RSSPECAN_ATTR_VSA_MODULATION_OPTIMIZATION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3124
#define RSSPECAN_ATTR_VSA_MODULATION_EVM_OFFSET                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1646
#define RSSPECAN_ATTR_VSA_TX_FILTER_TYPE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1647
#define RSSPECAN_ATTR_VSA_TX_FILTER_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1648
#define RSSPECAN_ATTR_VSA_TX_FILTER_ALPHA                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1649
#define RSSPECAN_ATTR_VSA_TX_FILTER_USER_FILE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2647
#define RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_AUTO                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1650
#define RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_SYM                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1651
#define RSSPECAN_ATTR_VSA_DEMODULATION_REC_LENGTH_S                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1652
#define RSSPECAN_ATTR_VSA_DEMODULATION_RESULT_LENGTH                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1653
#define RSSPECAN_ATTR_VSA_DEMODULATION_MULTIBURST_STATE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1654
#define RSSPECAN_ATTR_VSA_DEMODULATION_SIDE_BAND                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1655
#define RSSPECAN_ATTR_VSA_DEMODULATION_NORMALIZE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1656
#define RSSPECAN_ATTR_VSA_EVAL_LINE_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1657
#define RSSPECAN_ATTR_VSA_EVAL_LINE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1658
#define RSSPECAN_ATTR_VSA_DEMODULATION_WBAN_STATE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1659
#define RSSPECAN_ATTR_VSA_COMPENSATE_IQ_OFFSET                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1660
#define RSSPECAN_ATTR_VSA_COMPENSATE_IQ_IMBALANCE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1661
#define RSSPECAN_ATTR_VSA_COMPENSATE_AMPLITUDE_DROOP                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1662
#define RSSPECAN_ATTR_VSA_COMPENSATE_CARRIER_FREQUENCY_DRIFT                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2664
#define RSSPECAN_ATTR_VSA_COMPENSATE_FSK_DEVIATION                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2665
#define RSSPECAN_ATTR_VSA_COMPENSATE_CHANNEL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2989
#define RSSPECAN_ATTR_VSA_COMPENSATE_SYMBOL_RATE_ERROR                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2990
#define RSSPECAN_ATTR_VSA_AUTO_MEASUREMENT_FILTER                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1663
#define RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_TYPE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1664
#define RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_STATE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1665
#define RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_ALPHA                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1666
#define RSSPECAN_ATTR_VSA_MEASUREMENT_FILTER_USER_FILE                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2648
#define RSSPECAN_ATTR_VSA_AUTOMATIC_ESTIMATION_STATE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2666
#define RSSPECAN_ATTR_VSA_ESTIMATION_VALUE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2667
#define RSSPECAN_ATTR_VSA_UNIT_TIME                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1667
#define RSSPECAN_ATTR_VSA_UNIT_ANGLE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1668
#define RSSPECAN_ATTR_VSA_FIT_REF_POINT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1669
#define RSSPECAN_ATTR_VSA_FIT_ALIGN                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1670
#define RSSPECAN_ATTR_VSA_FIT_ALIGN_VALUE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1671
#define RSSPECAN_ATTR_VSA_FIT_ALIGN_OFFSET                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1672
#define RSSPECAN_ATTR_VSA_FIT_SYMBOL_NUMBER                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1673
#define RSSPECAN_ATTR_VSA_FIT_SYNC_OFFSET                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1674
#define RSSPECAN_ATTR_VSA_DISPLAY_TRACE_SYMBOL                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1675
#define RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_TRACE_SYMBOL_STATE              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2668
#define RSSPECAN_ATTR_VSA_TRACE_DISPLAY_SPACING                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1676
#define RSSPECAN_ATTR_VSA_SCALE_X_BAR_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1677
#define RSSPECAN_ATTR_VSA_SCALE_X_PDIV                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1678
#define RSSPECAN_ATTR_VSA_SCALE_X_RVAL                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1679
#define RSSPECAN_ATTR_VSA_SCALE_X_RPOS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1680
#define RSSPECAN_ATTR_VSA_SCALE_Y_PDIV                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1681
#define RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_POINTS_PER_SYMBOL_AUTO          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1682
#define RSSPECAN_ATTR_VSA_CONFIGURE_DISPLAY_POINTS_PER_SYMBOL               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1683
#define RSSPECAN_ATTR_VSA_CAPTURE_OVERSAMPLING                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1684
#define RSSPECAN_ATTR_VSA_TRACE_EVALUATION                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1685
#define RSSPECAN_ATTR_VSA_STATISTIC_COUNT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2669
#define RSSPECAN_ATTR_VSA_RESULT_RANGE_NUMBER                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2670
#define RSSPECAN_ATTR_VSA_CURRENT_STATISTICS_COUNTER                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2671
#define RSSPECAN_ATTR_VSA_MBURST_START                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1686
#define RSSPECAN_ATTR_VSA_MBURST_LENGTH                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1687
#define RSSPECAN_ATTR_VSA_MBURST_CAPTURE_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1688
#define RSSPECAN_ATTR_VSA_MBURST_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1689
#define RSSPECAN_ATTR_VSA_MBURST_SEARCH_FIRST                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1690
#define RSSPECAN_ATTR_VSA_MBURST_SEARCH_NEXT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1691
#define RSSPECAN_ATTR_VSA_MBURST_SEARCH_START                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1692
#define RSSPECAN_ATTR_VSA_FILE_EXP_HEADER                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1693
#define RSSPECAN_ATTR_VSA_FILE_DATA_MODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1694
#define RSSPECAN_ATTR_VSA_FILE_DATA_RAW_MODE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1695
#define RSSPECAN_ATTR_VSA_INCLUDE_IQ_DATA                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2577
#define RSSPECAN_ATTR_VSA_LIMIT_MODULATION_ACCURACY_DEFAULTS                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2649
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_STATE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2650
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_EVM                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2651
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_PHASE_ERROR                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2652
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_MAGNITUDE_ERROR                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2653
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_CARRIER_FREQUENCY_ERROR             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2654
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_RHO                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2655
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_IQ_OFFSET                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2656
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_FREQUENCY_ERROR                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2657
#define RSSPECAN_ATTR_VSA_LIMIT_MOD_ACC_FREQUENCY_DEVIATION_ERROR           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2658
#define RSSPECAN_ATTR_VSA_QUERY_MOD_ACC_LIMIT_CHECK_RESULT                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2659
#define RSSPECAN_ATTR_VSA_SPECTRUM_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1696
#define RSSPECAN_ATTR_VSA_CPOINT_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1697
#define RSSPECAN_ATTR_VSA_CPOINT_VALUE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1698
#define RSSPECAN_ATTR_VSA_CPOINT_PHASE_SHIFT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1699
#define RSSPECAN_ATTR_VSA_CPOINT_POWER                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1700
#define RSSPECAN_ATTR_VSA_X_FIRST_VALUE_QUERY                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1701
#define RSSPECAN_ATTR_VSA_X_MARKER_POSITION                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1702
#define RSSPECAN_ATTR_VSA_REFRESH_CAPTURED_DATA                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2660
#define RSSPECAN_ATTR_VSA_BER_BIT_ERROR_RATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2786
#define RSSPECAN_ATTR_VSA_BER_NUMBER_OF_ERRORS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2787
#define RSSPECAN_ATTR_VSA_BER_NUMBER_OF_BITS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2788
#define RSSPECAN_ATTR_BWCD_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1703
#define RSSPECAN_ATTR_3GBS_MEAS                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1704
#define RSSPECAN_ATTR_3GPP_TAER_CPICH_CODE_NUMBER                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2674
#define RSSPECAN_ATTR_3GPP_TAER_ANTENNA_PATTERN                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2675
#define RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2676
#define RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER_INSERT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2677
#define RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER_DELETE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2678
#define RSSPECAN_ATTR_3GPP_TAER_BTS_NUMBER_DELETE_ALL                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2679
#define RSSPECAN_ATTR_3GPP_TAER_FREQUENCY_OFFSET                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2680
#define RSSPECAN_ATTR_3GPP_TAER_SCRAMBLING_CODE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2681
#define RSSPECAN_ATTR_3GPP_TAER_TABLE_CATALOG                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2682
#define RSSPECAN_ATTR_3GPP_TAER_TABLE_DELETE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2683
#define RSSPECAN_ATTR_3GPP_TAER_TABLE_SAVE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2684
#define RSSPECAN_ATTR_3GPP_TAER_TABLE_PRESET                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2685
#define RSSPECAN_ATTR_3GBS_WCDP_HSDPAUPA_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1705
#define RSSPECAN_ATTR_3GBS_WCDP_SYNC_TYPE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1706
#define RSSPECAN_ATTR_3GBS_WCDP_AUTO_SCALE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1707
#define RSSPECAN_ATTR_3GBS_WCDP_MCACLR                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1708
#define RSSPECAN_ATTR_3GBS_MIMO_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2428
#define RSSPECAN_ATTR_3GBS_WCDP_NORMALIZE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1709
#define RSSPECAN_ATTR_3GBS_WCDP_COMPARE_MODE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1710
#define RSSPECAN_ATTR_3GBS_WCDP_INVERT_Q                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1711
#define RSSPECAN_ATTR_3GBS_WCDP_POW_REF                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1712
#define RSSPECAN_ATTR_3GBS_WCDP_SIDE_BAND                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1713
#define RSSPECAN_ATTR_3GBS_WCDP_INACT_CHAN_THR                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1714
#define RSSPECAN_ATTR_3GBS_WCDP_POW_DIFFERENCE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1715
#define RSSPECAN_ATTR_3GBS_WCDP_CONST_B                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1716
#define RSSPECAN_ATTR_3GBS_WCDP_MULTIFRAME_CAPTURE_LENGTH                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1717
#define RSSPECAN_ATTR_3GBS_WCDP_MULTIFRAME_FRAME_ANALYZE                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1718
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_LIMIT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1719
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_LIMIT_REL_STATE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1720
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_LIMIT_ABS_STATE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1721
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_EDIT_ACP_LIMIT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1722
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_EDIT_ACP_LIMIT_ABS                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1723
#define RSSPECAN_ATTR_3GBS_MCACLR_ACLR_ACP_CHAN_POW                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1724
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_ABS_REL                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1725
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_ADJUST_SETTINGS                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1726
#define RSSPECAN_ATTR_3GBS_MCACLR_ADJUST_REF_LEVEL                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1727
#define RSSPECAN_ATTR_3GBS_MCACLR_ACP_ACP_REF_SETTINGS                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1728
#define RSSPECAN_ATTR_3GBS_MCACLR_NOISE_CORRECTION                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1729
#define RSSPECAN_ATTR_3GPP_BS_STANDARD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2645
#define RSSPECAN_ATTR_3GBS_ANTENNA                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1730
#define RSSPECAN_ATTR_3GBS_MARKER_TO_CHANNEL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1731
#define RSSPECAN_ATTR_3GBS_REFERENCE_MARKER_TO_CHANNEL                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1732
#define RSSPECAN_ATTR_3GBS_SCODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1733
#define RSSPECAN_ATTR_3GBS_TIMING_OFFSET                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1734
#define RSSPECAN_ATTR_3GPP_PDIV                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1735
#define RSSPECAN_ATTR_3GPP_ASEQ                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1736
#define RSSPECAN_ATTR_3GPP_SCODE_SEARCH                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1737
#define RSSPECAN_ATTR_3GBS_WCDP_CTAB_HEAD_COMM                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1738
#define RSSPECAN_ATTR_3GBS_WCDP_CTAB_DEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1739
#define RSSPECAN_ATTR_3GBS_WCDP_CTAB_COPY                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1740
#define RSSPECAN_ATTR_3GBS_WCDP_CTAB_NAME                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1741
#define RSSPECAN_ATTR_3GBS_WCDP_CTAB                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1742
#define RSSPECAN_ATTR_3GBS_WCDP_CTAB_SEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1743
#define RSSPECAN_ATTR_3GBS_RESULTS_SFAC                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1744
#define RSSPECAN_ATTR_3GBS_RESULTS_SELECT_CHAN                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1745
#define RSSPECAN_ATTR_3GBS_RESULTS_SELECT_CPICH_SLOT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1746
#define RSSPECAN_ATTR_3GBS_RESULT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1747
#define RSSPECAN_ATTR_3GBS_RESULT_SUM_VIEW                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1748
#define RSSPECAN_ATTR_3GUE_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1749
#define RSSPECAN_ATTR_3GUE_RESULT                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1750
#define RSSPECAN_ATTR_3GUE_RRC_FILTER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1751
#define RSSPECAN_ATTR_3GUE_MEAS                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1752
#define RSSPECAN_ATTR_3GUE_CDP_BASE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1753
#define RSSPECAN_ATTR_3GUE_CDP_SIDEBAND                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1754
#define RSSPECAN_ATTR_3GUE_CDP_SEL_COMP                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1755
#define RSSPECAN_ATTR_3GUE_CDP_CHAN_CODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1756
#define RSSPECAN_ATTR_3GUE_CDP_INACT_CHAN_THR                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1757
#define RSSPECAN_ATTR_3GUE_CDP_SCR_CODE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1758
#define RSSPECAN_ATTR_3GUE_CDP_SCR_TYPE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1759
#define RSSPECAN_ATTR_3GUE_CDP_NORM                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1760
#define RSSPECAN_ATTR_3GUE_CDP_INVERT_Q                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1761
#define RSSPECAN_ATTR_3GUE_CDP_SFAC                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1762
#define RSSPECAN_ATTR_3GUE_CDP_SLOT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1763
#define RSSPECAN_ATTR_3GUE_CDP_ELIMINATE_TAIL_CHIPS                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1764
#define RSSPECAN_ATTR_3GUE_CDP_HSDPAUPA_STATE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 1765
#define RSSPECAN_ATTR_3GUE_CDP_MULTIFRAME_CAPTURE_LENGTH                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1766
#define RSSPECAN_ATTR_3GUE_CDP_MULTIFRAME_FRAME_ANALYZE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1767
#define RSSPECAN_ATTR_3GUE_CDP_EVM_MEAS_INTERVAL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1768
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB_HSDP                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1769
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB_COMMENT                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1770
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB_COPY                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1771
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB_DEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1772
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB_NAME                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1773
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB_SEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1774
#define RSSPECAN_ATTR_3GUE_WCDP_CTAB                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1775
#define RSSPECAN_ATTR_TDBS_MEAS                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1776
#define RSSPECAN_ATTR_TDBS_ADJ_RLEV_QUERY                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1781
#define RSSPECAN_ATTR_TDBS_CDP_LEV_ADJUST                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1782
#define RSSPECAN_ATTR_TDBS_STANDARD                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1783
#define RSSPECAN_ATTR_TDBS_SIDE_BAND                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1788
#define RSSPECAN_ATTR_TDBS_STS_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2429
#define RSSPECAN_ATTR_TDBS_CPICH_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1792
#define RSSPECAN_ATTR_TDBS_CPICH_CODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1793
#define RSSPECAN_ATTR_TDBS_CPICH_PATTERN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1794
#define RSSPECAN_ATTR_TDBS_RESULT_TYPE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1795
#define RSSPECAN_ATTR_TDBS_MEAS_PVT_HDYN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1810
#define RSSPECAN_ATTR_TDUE_ADAPT_SIGN_AUT_LEVEL_TIME                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1813
#define RSSPECAN_ATTR_TDUE_ADAPT_SIGN_SLOT_START                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1814
#define RSSPECAN_ATTR_TDUE_ADAPT_SIGN_SLOT_STOP                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1815
#define RSSPECAN_ATTR_TDUE_ADJ_RLEV_QUERY                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1816
#define RSSPECAN_ATTR_TDUE_CDP_LEV_ADJUST                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1817
#define RSSPECAN_ATTR_TDUE_SEM_LIMITS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2431
#define RSSPECAN_ATTR_TDUE_CAPTURE_LENGTH                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1818
#define RSSPECAN_ATTR_TDUE_SCODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1819
#define RSSPECAN_ATTR_TDUE_CELL_SHIFTS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1820
#define RSSPECAN_ATTR_TDUE_INACT_CHAN_THR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1821
#define RSSPECAN_ATTR_TDUE_SIDE_BAND                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1822
#define RSSPECAN_ATTR_TDUE_INVERT_Q                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1823
#define RSSPECAN_ATTR_TDUE_NORMALIZE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1824
#define RSSPECAN_ATTR_TDUE_SYNC_TO_SLOT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1825
#define RSSPECAN_ATTR_TDUE_CPICH_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1826
#define RSSPECAN_ATTR_TDUE_CPICH_CODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1827
#define RSSPECAN_ATTR_TDUE_CPICH_PATTERN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1828
#define RSSPECAN_ATTR_TDUE_RESULT_TYPE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1829
#define RSSPECAN_ATTR_TDUE_RESULT_SEL_CHAN                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1830
#define RSSPECAN_ATTR_TDUE_RESULT_SEL_SLOT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1831
#define RSSPECAN_ATTR_TDUE_RESULT_SET_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1832
#define RSSPECAN_ATTR_TDUE_RESULT_SET_VALUE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1833
#define RSSPECAN_ATTR_TDUE_CTAB_ORDER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1834
#define RSSPECAN_ATTR_TDUE_CTAB_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1835
#define RSSPECAN_ATTR_TDUE_CTAB_SEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1836
#define RSSPECAN_ATTR_TDUE_CTAB_NAME                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1837
#define RSSPECAN_ATTR_TDUE_CTAB_COMM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1838
#define RSSPECAN_ATTR_TDUE_CTAB_MSHIFT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1839
#define RSSPECAN_ATTR_TDUE_CTAB_COPY                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1840
#define RSSPECAN_ATTR_TDUE_CTAB_DEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1841
#define RSSPECAN_ATTR_TDUE_CTAB_MMAX                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2449
#define RSSPECAN_ATTR_TDUE_MEAS_PVT_SFRAMES                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1842
#define RSSPECAN_ATTR_TDUE_MEAS_PVT_HDYN                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1843
#define RSSPECAN_ATTR_TDBS_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1777
#define RSSPECAN_ATTR_TDUE_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1812
#define RSSPECAN_ATTR_TDUE_MEAS                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1811
#define RSSPECAN_ATTR_TDBS_INVERT_Q                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1789
#define RSSPECAN_ATTR_TDBS_RESULT_SET_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1798
#define RSSPECAN_ATTR_TDBS_RESULT_SET_VALUE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1799
#define RSSPECAN_ATTR_TDBS_CAPTURE_LENGTH                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1784
#define RSSPECAN_ATTR_TDBS_RESULT_SEL_CHAN                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1796
#define RSSPECAN_ATTR_TDBS_RESULT_SEL_SLOT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1797
#define RSSPECAN_ATTR_TDBS_ADAPT_SIGN_SLOT_START                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1779
#define RSSPECAN_ATTR_TDBS_ADAPT_SIGN_SLOT_STOP                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1780
#define RSSPECAN_ATTR_TDBS_ADAPT_SIGN_AUT_LEVEL_TIME                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1778
#define RSSPECAN_ATTR_TDBS_SCODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1785
#define RSSPECAN_ATTR_TDBS_CELL_SHIFTS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1786
#define RSSPECAN_ATTR_TDBS_SYNC_TO_SLOT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1791
#define RSSPECAN_ATTR_TDBS_SYNC_TO_SLOT_ROTATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2602
#define RSSPECAN_ATTR_TDUE_STS_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2432
#define RSSPECAN_ATTR_TDBS_INACT_CHAN_THR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1787
#define RSSPECAN_ATTR_TDBS_NORMALIZE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1790
#define RSSPECAN_ATTR_TDBS_POWER_DISPLAY                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2998
#define RSSPECAN_ATTR_TDBS_VERTICAL_SCALE_MAX_MIN                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2999
#define RSSPECAN_ATTR_TDBS_CTAB_ORDER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3044
#define RSSPECAN_ATTR_TDBS_CTAB_STATE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3045
#define RSSPECAN_ATTR_TDBS_CTAB_SEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3046
#define RSSPECAN_ATTR_TDBS_CTAB_NAME                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3047
#define RSSPECAN_ATTR_TDBS_CTAB_COMM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3048
#define RSSPECAN_ATTR_TDBS_CTAB_MSHIFT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3049
#define RSSPECAN_ATTR_TDBS_CTAB_COPY                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3050
#define RSSPECAN_ATTR_TDBS_CTAB_DEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3051
#define RSSPECAN_ATTR_TDBS_CTAB_MMAX                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3052
#define RSSPECAN_ATTR_TDBS_MEAS_PVT_SWITCH_POINT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1808
#define RSSPECAN_ATTR_TDBS_MEAS_PVT_SFRAMES                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1809
#define RSSPECAN_ATTR_C2K_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1844
#define RSSPECAN_ATTR_MC2K_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1845
#define RSSPECAN_ATTR_BDO_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1846
#define RSSPECAN_ATTR_MDO_MODE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1847
#define RSSPECAN_ATTR_C2K_CTABLE_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1848
#define RSSPECAN_ATTR_C2K_CTABLE_SELECT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1849
#define RSSPECAN_ATTR_C2K_CTABLE_NAME                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1850
#define RSSPECAN_ATTR_C2K_CTABLE_COMMENT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1851
#define RSSPECAN_ATTR_C2K_CTABLE_COPY                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1852
#define RSSPECAN_ATTR_C2K_CTABLE_DELETE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1853
#define RSSPECAN_ATTR_C2K_CTABLE_RESTORE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1854
#define RSSPECAN_ATTR_C2K_CDP_NORMALIZE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1855
#define RSSPECAN_ATTR_C2K_CDP_QINVERT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1856
#define RSSPECAN_ATTR_C2K_CDP_SLOT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1857
#define RSSPECAN_ATTR_C2K_CDP_SBAND                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1858
#define RSSPECAN_ATTR_C2K_CDP_ICTHRES                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1859
#define RSSPECAN_ATTR_C2K_CDP_LEVEL_ADJUST                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1860
#define RSSPECAN_ATTR_C2K_CDP_TPMEAS                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1861
#define RSSPECAN_ATTR_C2K_CDP_DISPLAY                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2887
#define RSSPECAN_ATTR_C2K_ACH_PRES                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1862
#define RSSPECAN_ATTR_C2K_ADJUST_REF_LEVEL                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1863
#define RSSPECAN_ATTR_C2K_PDIV                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1864
#define RSSPECAN_ATTR_C2K_ACH_REF_AUTO                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1865
#define RSSPECAN_ATTR_C2K_LIM_TRANS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1866
#define RSSPECAN_ATTR_C2K_SET_TO_ANALYZE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1867
#define RSSPECAN_ATTR_C2K_PERCENT_MARKER                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1868
#define RSSPECAN_ATTR_C2K_LIM_RESTORE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1869
#define RSSPECAN_ATTR_C2K_MEAS_SELECT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1870
#define RSSPECAN_ATTR_C2K_BCLASS                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1871
#define RSSPECAN_ATTR_C2K_SET_COUNT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1872
#define RSSPECAN_ATTR_C2K_RESULT_TYPE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1873
#define RSSPECAN_ATTR_C2K_LIM_VAL                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1874
#define RSSPECAN_ATTR_C2K_LIM_MODE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1875
#define RSSPECAN_ATTR_C2K_LIM_STORE_FILE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1876
#define RSSPECAN_ATTR_C2K_CDP_PNOFFSET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1877
#define RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1878
#define RSSPECAN_ATTR_C2K_MCARRIER_STATE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1879
#define RSSPECAN_ATTR_C2K_MCARRIER_ENH_ALG                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1880
#define RSSPECAN_ATTR_C2K_MCARRIER_FILTER_STATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1881
#define RSSPECAN_ATTR_C2K_MCARRIER_FILTER_TYPE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1882
#define RSSPECAN_ATTR_C2K_MCARRIER_ROLL_OFF                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1883
#define RSSPECAN_ATTR_C2K_MCARRIER_CUT_OFF_FREQ                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1884
#define RSSPECAN_ATTR_C2K_MARKER_TDPICH                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1885
#define RSSPECAN_ATTR_C2K_MARKER_PICH                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1886
#define RSSPECAN_ATTR_C2K_CDP_SFACTOR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1887
#define RSSPECAN_ATTR_C2K_CDP_PREFERENCE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1888
#define RSSPECAN_ATTR_C2K_CDP_ORDER                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1889
#define RSSPECAN_ATTR_C2K_CDP_IQLENGTH_FSQ_FSU                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1890
#define RSSPECAN_ATTR_C2K_CDP_CODE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1891
#define RSSPECAN_ATTR_C2K_CDP_ANTENNA                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1892
#define RSSPECAN_ATTR_C2K_FORCE_DV                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1893
#define RSSPECAN_ATTR_BDO_PVT_BURST_FIT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1894
#define RSSPECAN_ATTR_BDO_PVT_FAIL_RESTART                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1895
#define RSSPECAN_ATTR_BDO_LIM_PVT_REFERENCE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1896
#define RSSPECAN_ATTR_BDO_LIM_PVT_VALUE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1897
#define RSSPECAN_ATTR_BDO_LIM_PVT_LIST                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1898
#define RSSPECAN_ATTR_BDO_LIM_PVT_RESTORE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1899
#define RSSPECAN_ATTR_BDO_RFSLOT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 1900
#define RSSPECAN_ATTR_BDO_CDP_MMODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1901
#define RSSPECAN_ATTR_BDO_CDP_MAPPING                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1902
#define RSSPECAN_ATTR_BDO_CDP_CTYPE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1903
#define RSSPECAN_ATTR_BDO_CDP_OVERVIEW                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1904
#define RSSPECAN_ATTR_BDO_CDP_AVERAGE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1905
#define RSSPECAN_ATTR_BDO_CDP_IQLENGTH_FSQ_FSU                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1906
#define RSSPECAN_ATTR_BDO_CDP_CODE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1907
#define RSSPECAN_ATTR_BDO_REVISION                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1908
#define RSSPECAN_ATTR_MC2K_LIM_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1909
#define RSSPECAN_ATTR_MC2K_MARKER_PICH                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1910
#define RSSPECAN_ATTR_MC2K_CDP_MAPPING                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1911
#define RSSPECAN_ATTR_MC2K_CDP_PREFERENCE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1912
#define RSSPECAN_ATTR_MC2K_CDP_ORDER                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1913
#define RSSPECAN_ATTR_MC2K_CDP_OVERVIEW                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1914
#define RSSPECAN_ATTR_MC2K_CDP_LCODE_MASK                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 1915
#define RSSPECAN_ATTR_MC2K_CDP_LCODE_OFFSET                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1916
#define RSSPECAN_ATTR_MC2K_CDP_CODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1917
#define RSSPECAN_ATTR_MC2K_CDP_SFACTOR                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1918
#define RSSPECAN_ATTR_MC2K_CDP_IQLENGTH_FSQ_FSU                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1919
#define RSSPECAN_ATTR_MC2K_CDP_IQLENGTH_FSP                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1920
#define RSSPECAN_ATTR_MC2K_LCODE_MODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1921
#define RSSPECAN_ATTR_MDO_CDP_LCODE_I                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1922
#define RSSPECAN_ATTR_MDO_CDP_LCODE_Q                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1923
#define RSSPECAN_ATTR_MDO_CDP_CODE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1924
#define RSSPECAN_ATTR_MDO_CDP_IQLENGTH_FSQ_FSU                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1925
#define RSSPECAN_ATTR_MDO_CDP_IQLENGTH_FSP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 1926
#define RSSPECAN_ATTR_MDO_CDP_OPERATION                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1927
#define RSSPECAN_ATTR_MDO_CDP_AVERAGE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1928
#define RSSPECAN_ATTR_WLAN_PVT_RPOW                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1930
#define RSSPECAN_ATTR_WLAN_PVT_SEL                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1931
#define RSSPECAN_ATTR_WLAN_PVT                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1932
#define RSSPECAN_ATTR_WLAN_SPEC_FLAT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1933
#define RSSPECAN_ATTR_WLAN_BTYP                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1934
#define RSSPECAN_ATTR_WLAN_PVT_AVER                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1935
#define RSSPECAN_ATTR_WLAN_POW_EXP_IQ                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1936
#define RSSPECAN_ATTR_WLAN_POW_EXP_RF                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 1937
#define RSSPECAN_ATTR_WLAN_POW_AUTO                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1938
#define RSSPECAN_ATTR_WLAN_POW_AUTO_ONCE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1939
#define RSSPECAN_ATTR_WLAN_CHAN                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1940
#define RSSPECAN_ATTR_WLAN_EVM_ECAR                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1941
#define RSSPECAN_ATTR_WLAN_EVM_ESYM                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1942
#define RSSPECAN_ATTR_WLAN_SPEC_MASK_SEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 1944
#define RSSPECAN_ATTR_WLAN_SPEC_FFT                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1945
#define RSSPECAN_ATTR_WLAN_SPEC_SEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2686
#define RSSPECAN_ATTR_WLAN_SPEC_CSEL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2687
#define RSSPECAN_ATTR_WLAN_CONS_CSYM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1947
#define RSSPECAN_ATTR_WLAN_CONS_CCAR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1949
#define RSSPECAN_ATTR_WLAN_CONS_CARR_SEL_NR                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 1951
#define RSSPECAN_ATTR_WLAN_STAT_BSTR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1953
#define RSSPECAN_ATTR_WLAN_STAT_SFI                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1954
#define RSSPECAN_ATTR_WLAN_INIT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1955
#define RSSPECAN_ATTR_WLAN_INIT_CONT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1956
#define RSSPECAN_ATTR_WLAN_BURS_COUN                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1958
#define RSSPECAN_ATTR_WLAN_ACH_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1960
#define RSSPECAN_ATTR_WLAN_UNIT_PRE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1961
#define RSSPECAN_ATTR_WLAN_PVR_MODE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2631
#define RSSPECAN_ATTR_WLAN_PEAK_ERROR_VECTOR_MEAS_RANGE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2603
#define RSSPECAN_ATTR_WLAN_PRE                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1964
#define RSSPECAN_ATTR_WLAN_FLAT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1966
#define RSSPECAN_ATTR_WLAN_POW_AUTO_SWE_TIME                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 1967
#define RSSPECAN_ATTR_WLAN_IQ_LOAD                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1968
#define RSSPECAN_ATTR_WLAN_IQ_SAVE                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1969
#define RSSPECAN_ATTR_WLAN_CALC_BURS                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1970
#define RSSPECAN_ATTR_WLAN_PRE_SEL                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1971
#define RSSPECAN_ATTR_WLAN_NUMBER_OF_ANTENNAS                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2708
#define RSSPECAN_ATTR_WLAN_MIMO_DUT_CONFIGURATION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2709
#define RSSPECAN_ATTR_WLAN_MIMO_ANT_SIGNAL_CAPTURE_TYPE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2710
#define RSSPECAN_ATTR_WLAN_ANTENNA_SIMULTANEOUS_CAPTURE_STATE               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2789
#define RSSPECAN_ATTR_WLAN_MIMO_OSP_IP_ADDRESS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2711
#define RSSPECAN_ATTR_WLAN_MIMO_OSP_SWITCH_MODULE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2712
#define RSSPECAN_ATTR_WLAN_MIMO_PAYLOAD_LENGTH_SOURCE                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2892
#define RSSPECAN_ATTR_WLAN_DISP_AUTO_Y                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1974
#define RSSPECAN_ATTR_WLAN_Y_PDIV                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 1975
#define RSSPECAN_ATTR_WLAN_TRIS                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1976
#define RSSPECAN_ATTR_WLAN_TFAL                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1977
#define RSSPECAN_ATTR_WLAN_EVM                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 1981
#define RSSPECAN_ATTR_WLAN_TFAL_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1985
#define RSSPECAN_ATTR_WLAN_TRIS_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1986
#define RSSPECAN_ATTR_WLAN_EVM_RES                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1990
#define RSSPECAN_ATTR_WLAN_ACP_ADJ                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1995
#define RSSPECAN_ATTR_WLAN_ACP_ALT                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1997
#define RSSPECAN_ATTR_WLAN_SPEC_MASK_X                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1999
#define RSSPECAN_ATTR_WLAN_SPEC_MASK_Y                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2000
#define RSSPECAN_ATTR_WLAN_MARK_SYMB                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2001
#define RSSPECAN_ATTR_WLAN_MARK_CARR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2002
#define RSSPECAN_ATTR_WLAN_MARK_Y                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2003
#define RSSPECAN_ATTR_WLAN_FETC_TRIS                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2010
#define RSSPECAN_ATTR_WLAN_FETC_TFAL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2011
#define RSSPECAN_ATTR_WLAN_FETC_BERPILOT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2714
#define RSSPECAN_ATTR_WLAN_FETC_SYMB_COUNT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2022
#define RSSPECAN_ATTR_WLAN_FETC_BURST_LENGTH                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 3125
#define RSSPECAN_ATTR_WLAN_FETCH_BURST_PAYLOAD                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 3144
#define RSSPECAN_ATTR_WLAN_FETCH_BURST_PEAK                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3145
#define RSSPECAN_ATTR_WLAN_FETCH_BURST_PREAMBLE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3146
#define RSSPECAN_ATTR_WLAN_FETCH_BURST_START                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 3147
#define RSSPECAN_ATTR_WLAN_EGAT_HOLD_SAMP                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2023
#define RSSPECAN_ATTR_WLAN_EGAT_HOLD_TIME                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2024
#define RSSPECAN_ATTR_WLAN_EGAT_LENG_SAMP                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2025
#define RSSPECAN_ATTR_WLAN_EGAT_LENG_TIME                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2026
#define RSSPECAN_ATTR_WLAN_EGAT_LINK                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2027
#define RSSPECAN_ATTR_WLAN_EGAT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2028
#define RSSPECAN_ATTR_WLAN_TRIG_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2029
#define RSSPECAN_ATTR_WLAN_TRIG_POW_LEV                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2030
#define RSSPECAN_ATTR_WLAN_TRIG_POW_LEV_AUTO                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2031
#define RSSPECAN_ATTR_WLAN_ANALYZER_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2715
#define RSSPECAN_ATTR_WLAN_ANALYZER_ADDRESS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2716
#define RSSPECAN_ATTR_WLAN_ANTENNA_CONFIGURATION                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2717
#define RSSPECAN_ATTR_WLAN_SIGS                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2033
#define RSSPECAN_ATTR_WLAN_BAN                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2035
#define RSSPECAN_ATTR_WLAN_DBYT_EQU                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2038
#define RSSPECAN_ATTR_WLAN_DBYT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2039
#define RSSPECAN_ATTR_WLAN_DUR_EQU                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2040
#define RSSPECAN_ATTR_WLAN_DUR                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2041
#define RSSPECAN_ATTR_WLAN_EQUALIZER_FILTER_LENGTH                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2646
#define RSSPECAN_ATTR_WLAN_BURST_TYPE_TO_MEASURE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2718
#define RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEAS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2719
#define RSSPECAN_ATTR_WLAN_GUARD_INTERVAL_LEGTH                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2720
#define RSSPECAN_ATTR_WLAN_MIMO_SPATIAL_MAPPING_MODE                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2726
#define RSSPECAN_ATTR_WLAN_MIMO_POWER_NORMALISE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2727
#define RSSPECAN_ATTR_WLAN_MIMO_UDFSPATIAL_MAPPING_STREAM                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2728
#define RSSPECAN_ATTR_WLAN_MIMO_UDFSPATIAL_MAPPING_TIME_SHIFT               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2729
#define RSSPECAN_ATTR_WLAN_BURST_SELECT                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3148
#define RSSPECAN_ATTR_WLAN_BURST_STATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3149
#define RSSPECAN_ATTR_WLAN_IQ_COMPENSATION                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3150
#define RSSPECAN_ATTR_WLAN_BURST_UNIT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3151
#define RSSPECAN_ATTR_WLAN_SEM_TYPE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2045
#define RSSPECAN_ATTR_WLAN_SEM_CLASS                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2046
#define RSSPECAN_ATTR_WLAN_SEM_FILE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2047
#define RSSPECAN_ATTR_WLAN_MODE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1957
#define RSSPECAN_ATTR_WLAN_STAN                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3012
#define RSSPECAN_ATTR_WLAN_SRAT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3013
#define RSSPECAN_ATTR_WLAN_SWAP_IQ                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3014
#define RSSPECAN_ATTR_WLAN_RBW_FILTER                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3015
#define RSSPECAN_ATTR_WLAN_POWER_INTERVAL_SEARCH                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3009
#define RSSPECAN_ATTR_WLAN_FFT_START_OFFSET                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3016
#define RSSPECAN_ATTR_WLAN_CEST                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3017
#define RSSPECAN_ATTR_WLAN_TRAC_LEV                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3018
#define RSSPECAN_ATTR_WLAN_TRAC_PHAS                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3019
#define RSSPECAN_ATTR_WLAN_TRAC_TIME                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3020
#define RSSPECAN_ATTR_WLAN_TRAC_PILOTS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3021
#define RSSPECAN_ATTR_WLAN_BCON_AUTO                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3022
#define RSSPECAN_ATTR_WLAN_MCS_INDEX_TO_USE_FBUR                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3023
#define RSSPECAN_ATTR_WLAN_MCS_INDEX                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3024
#define RSSPECAN_ATTR_WLAN_STBC                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3025
#define RSSPECAN_ATTR_WLAN_GUARD_INTERVAL_LENGTH                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3027
#define RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE_802_11N                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 3029
#define RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEASURE_802_11N             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3030
#define RSSPECAN_ATTR_WLAN_EXT_SPATIAL_STREAMS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 3032
#define RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3010
#define RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEASURE                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3031
#define RSSPECAN_ATTR_WLAN_PSDU_MODULATION                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3011
#define RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE_802_11B_G                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3097
#define RSSPECAN_ATTR_WLAN_PSDU_MODULATION_802_11B_G                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3096
#define RSSPECAN_ATTR_WLAN_PPDU_FORMAT_TO_MEASURE_802_11AC                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3033
#define RSSPECAN_ATTR_WLAN_CHANNEL_BANDWIDTH_TO_MEASURE_802_11AC            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3034
#define RSSPECAN_ATTR_WLAN_NSTS_INDEX_MODE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 3035
#define RSSPECAN_ATTR_WLAN_NSTS_INDEX                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3036
#define RSSPECAN_ATTR_WLAN_BURS_COUN_STAT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 3037
#define RSSPECAN_ATTR_WLAN_CONS_CARR_SEL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 3038
#define RSSPECAN_ATTR_WLAN_SOURCE_OF_PAYLOAD_LENGTH                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3039
#define RSSPECAN_ATTR_WLAN_SYM_EQU                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2036
#define RSSPECAN_ATTR_WLAN_SYM                                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2037
#define RSSPECAN_ATTR_WLAN_UNIT_GIMB                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3040
#define RSSPECAN_ATTR_WLAN_UNIT_EVM                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1962
#define RSSPECAN_ATTR_WLAN_DISP_TABL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1973
#define RSSPECAN_ATTR_WLAN_FERR                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1978
#define RSSPECAN_ATTR_WLAN_SYMB                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1979
#define RSSPECAN_ATTR_WLAN_IQOF                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 1980
#define RSSPECAN_ATTR_WLAN_EVM_ALL                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1982
#define RSSPECAN_ATTR_WLAN_EVM_DATA                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1983
#define RSSPECAN_ATTR_WLAN_EVM_PIL                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 1984
#define RSSPECAN_ATTR_WLAN_SPEC_ACPR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1946
#define RSSPECAN_ATTR_WLAN_SPEC_MASK                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1943
#define RSSPECAN_ATTR_WLAN_SPEC_OCCUPIED_BANDWIDTH                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 3041
#define RSSPECAN_ATTR_WLAN_STAT_CCDF                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 1952
#define RSSPECAN_ATTR_WLAN_FETC_PRE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2005
#define RSSPECAN_ATTR_WLAN_FETC_PAYL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2006
#define RSSPECAN_ATTR_WLAN_FETC_RMS                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2007
#define RSSPECAN_ATTR_WLAN_FETC_PEAK                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2008
#define RSSPECAN_ATTR_WLAN_FETC_CRES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2009
#define RSSPECAN_ATTR_WLAN_FETC_FERR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2012
#define RSSPECAN_ATTR_WLAN_FETC_SYMB                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2013
#define RSSPECAN_ATTR_WLAN_FETC_IQOF                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2014
#define RSSPECAN_ATTR_WLAN_FETC_GIMB                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2015
#define RSSPECAN_ATTR_WLAN_FETC_QUAD                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2016
#define RSSPECAN_ATTR_WLAN_FETC_EVM_ALL                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2019
#define RSSPECAN_ATTR_WLAN_FETC_EVM_DATA                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2020
#define RSSPECAN_ATTR_WLAN_FETC_EVM_PILOT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2021
#define RSSPECAN_ATTR_WLAN_FETC_EVM_DIR                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2018
#define RSSPECAN_ATTR_WLAN_FETC_EVM                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2017
#define RSSPECAN_ATTR_WLAN_FETC_BURS_COUNT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2004
#define RSSPECAN_ATTR_WLAN_FETC_BURST_COUNT_ALL                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3042
#define RSSPECAN_ATTR_WLAN_FERR_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1987
#define RSSPECAN_ATTR_WLAN_SYMB_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1988
#define RSSPECAN_ATTR_WLAN_IQOF_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 1989
#define RSSPECAN_ATTR_WLAN_EVM_ALL_RES                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1992
#define RSSPECAN_ATTR_WLAN_EVM_DATA_RES                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1993
#define RSSPECAN_ATTR_WLAN_EVM_PIL_RES                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1994
#define RSSPECAN_ATTR_WLAN_ACP_ADJ_RES                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1996
#define RSSPECAN_ATTR_WLAN_ACP_ALT_RES                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 1998
#define RSSPECAN_ATTR_WLAN_LIM_FAIL_RES                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 1991
#define RSSPECAN_ATTR_WIMAX_STAN                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2048
#define RSSPECAN_ATTR_WIMAX_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2049
#define RSSPECAN_ATTR_WIMAX_BAND_CHAN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2050
#define RSSPECAN_ATTR_WIMAX_BURS_COUN_STAT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2051
#define RSSPECAN_ATTR_WIMAX_BURS_COUN                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2052
#define RSSPECAN_ATTR_WIMAX_FBAN                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2053
#define RSSPECAN_ATTR_WIMAX_CHAN                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2054
#define RSSPECAN_ATTR_WIMAX_IGR                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2055
#define RSSPECAN_ATTR_WIMAX_POW_AUTO                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2056
#define RSSPECAN_ATTR_WIMAX_POW_AUTO_ONCE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2057
#define RSSPECAN_ATTR_WIMAX_POW_AUTO_SWE_TIME                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2058
#define RSSPECAN_ATTR_WIMAX_POW_EXP_IQ                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2059
#define RSSPECAN_ATTR_WIMAX_POW_EXP_RF                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2060
#define RSSPECAN_ATTR_WIMAX_RLEV_IQ                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2061
#define RSSPECAN_ATTR_WIMAX_RLEV_RF                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2062
#define RSSPECAN_ATTR_WIMAX_SRAT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2063
#define RSSPECAN_ATTR_WIMAX_FLAT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2064
#define RSSPECAN_ATTR_WIMAX_IQ_LOAD                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2065
#define RSSPECAN_ATTR_WIMAX_IQ_SAVE                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2066
#define RSSPECAN_ATTR_WIMAX_SWAP_IQ                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2067
#define RSSPECAN_ATTR_WIMAX_TABLE_UNIT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2068
#define RSSPECAN_ATTR_WIMAX_RSSI_AVG_PARAM                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2069
#define RSSPECAN_ATTR_WIMAX_CALC_BURS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2070
#define RSSPECAN_ATTR_WIMAX_FORMAT_SELECT                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2071
#define RSSPECAN_ATTR_WIMAX_NFFT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2072
#define RSSPECAN_ATTR_WIMAX_YIG_AUTO                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2073
#define RSSPECAN_ATTR_WIMAX_FRAME_LOAD                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2074
#define RSSPECAN_ATTR_WIMAX_FRAME_SAVE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2075
#define RSSPECAN_ATTR_WIMAX_SMU_ADDRESS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2076
#define RSSPECAN_ATTR_WIMAX_TTC_FRAME                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2077
#define RSSPECAN_ATTR_WIMAX_TTC_START                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2078
#define RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER_TYPE               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2433
#define RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2434
#define RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER_OFFSET_TYPE        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2435
#define RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_REFERENCE_POWER_OFFSET             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2436
#define RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_AVERAGING                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2437
#define RSSPECAN_ATTR_WIMAX_TTC_SUBFRAME_LENGTH                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2438
#define RSSPECAN_ATTR_WIMAX_FSBW_RATIO_AUTO                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2079
#define RSSPECAN_ATTR_WIMAX_FSBW_SRATE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2080
#define RSSPECAN_ATTR_WIMAX_FSBW_CHANNEL_BW                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2081
#define RSSPECAN_ATTR_WIMAX_NUMBER_OF_ANTENNAS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2082
#define RSSPECAN_ATTR_WIMAX_STAT_BSTR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2083
#define RSSPECAN_ATTR_WIMAX_BSTR_SEL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2084
#define RSSPECAN_ATTR_WIMAX_BSTR_SEL_NR                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2085
#define RSSPECAN_ATTR_WIMAX_STAT_CCDF                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2086
#define RSSPECAN_ATTR_WIMAX_STAT_BSUM                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2087
#define RSSPECAN_ATTR_WIMAX_PVT                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2088
#define RSSPECAN_ATTR_WIMAX_BURS_PVT_BURS                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2089
#define RSSPECAN_ATTR_WIMAX_PVT_SEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2090
#define RSSPECAN_ATTR_WIMAX_CONS_SEL                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2091
#define RSSPECAN_ATTR_WIMAX_CONS_SEL_NR                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2092
#define RSSPECAN_ATTR_WIMAX_CONS_CCAR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2093
#define RSSPECAN_ATTR_WIMAX_CONS_CSYM                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2094
#define RSSPECAN_ATTR_WIMAX_CONSTELLATION_DIAGRAM                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2730
#define RSSPECAN_ATTR_WIMAX_PRE                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2095
#define RSSPECAN_ATTR_WIMAX_PRE_SEL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2096
#define RSSPECAN_ATTR_WIMAX_UNIT_PRE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2097
#define RSSPECAN_ATTR_WIMAX_EVM_ECAR                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2098
#define RSSPECAN_ATTR_WIMAX_EVM_ESYM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2099
#define RSSPECAN_ATTR_WIMAX_UNIT_EVM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2100
#define RSSPECAN_ATTR_WIMAX_SPEC_ACPR_SEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2101
#define RSSPECAN_ATTR_WIMAX_ACH_MODE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2102
#define RSSPECAN_ATTR_WIMAX_ACH_ACP                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2103
#define RSSPECAN_ATTR_WIMAX_ACH_BW_ACH                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2104
#define RSSPECAN_ATTR_WIMAX_ACH_BW_ALT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2105
#define RSSPECAN_ATTR_WIMAX_ACH_BW_CHAN                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2106
#define RSSPECAN_ATTR_WIMAX_ACH_SPAC_ACH                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2107
#define RSSPECAN_ATTR_WIMAX_ACH_SPAC_ALT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2108
#define RSSPECAN_ATTR_WIMAX_ACP_NOISE_CORR                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2109
#define RSSPECAN_ATTR_WIMAX_SPEC_ACPR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2110
#define RSSPECAN_ATTR_WIMAX_SPEC_FFT                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2111
#define RSSPECAN_ATTR_WIMAX_SPEC_FLAT                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2112
#define RSSPECAN_ATTR_WIMAX_SPEC_MASK                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2113
#define RSSPECAN_ATTR_WIMAX_SPEC_MASK_SEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2114
#define RSSPECAN_ATTR_WIMAX_SPEC_PREAMBLE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2115
#define RSSPECAN_ATTR_WIMAX_ACPR_TIME                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2116
#define RSSPECAN_ATTR_WIMAX_ACPR_TIME_AUTO                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2117
#define RSSPECAN_ATTR_WIMAX_SEM_TYPE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2118
#define RSSPECAN_ATTR_WIMAX_SEM_CLASS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2119
#define RSSPECAN_ATTR_WIMAX_SEM_FILE                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2120
#define RSSPECAN_ATTR_WIMAX_DISP_AUTO_Y                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2121
#define RSSPECAN_ATTR_WIMAX_DISP_TABL                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2122
#define RSSPECAN_ATTR_WIMAX_DISP_TABL_LIST                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2123
#define RSSPECAN_ATTR_WIMAX_Y_PDIV                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2124
#define RSSPECAN_ATTR_WIMAX_EVM_ALL                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2125
#define RSSPECAN_ATTR_WIMAX_EVM_DATA                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2126
#define RSSPECAN_ATTR_WIMAX_FERR                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2127
#define RSSPECAN_ATTR_WIMAX_SST                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2128
#define RSSPECAN_ATTR_WIMAX_SYMB                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2129
#define RSSPECAN_ATTR_WIMAX_IQOF                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2130
#define RSSPECAN_ATTR_WIMAX_BERP                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2131
#define RSSPECAN_ATTR_WIMAX_USERR                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2132
#define RSSPECAN_ATTR_WIMAX_EVM_ALL_RES                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2133
#define RSSPECAN_ATTR_WIMAX_EVM_DATA_RES                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2134
#define RSSPECAN_ATTR_WIMAX_FERR_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2135
#define RSSPECAN_ATTR_WIMAX_LIM_FAIL_RES                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2136
#define RSSPECAN_ATTR_WIMAX_SPEC_MASK_X                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2137
#define RSSPECAN_ATTR_WIMAX_SPEC_MASK_Y                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2138
#define RSSPECAN_ATTR_WIMAX_SST_RES                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2139
#define RSSPECAN_ATTR_WIMAX_SYMB_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2140
#define RSSPECAN_ATTR_WIMAX_IQOF_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2141
#define RSSPECAN_ATTR_WIMAX_BERP_RES                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2142
#define RSSPECAN_ATTR_WIMAX_USERR_RES                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2143
#define RSSPECAN_ATTR_WIMAX_MARK_BURST                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2144
#define RSSPECAN_ATTR_WIMAX_MARK_CARR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2145
#define RSSPECAN_ATTR_WIMAX_MARK_SYMB                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2146
#define RSSPECAN_ATTR_WIMAX_MARK_Y                                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2147
#define RSSPECAN_ATTR_WIMAX_FETC_BURS_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2148
#define RSSPECAN_ATTR_WIMAX_FETC_CRES                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2149
#define RSSPECAN_ATTR_WIMAX_FETC_EVM_ALL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2150
#define RSSPECAN_ATTR_WIMAX_FETC_EVM                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2151
#define RSSPECAN_ATTR_WIMAX_FETC_EVM_DATA                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2152
#define RSSPECAN_ATTR_WIMAX_FETC_EVM_PILOT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2153
#define RSSPECAN_ATTR_WIMAX_FETC_EVM_PREAMBLE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2154
#define RSSPECAN_ATTR_WIMAX_FETC_FERR                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2155
#define RSSPECAN_ATTR_WIMAX_FETC_GIMB                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2156
#define RSSPECAN_ATTR_WIMAX_FETC_IQOF                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2157
#define RSSPECAN_ATTR_WIMAX_FETC_QUAD                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2158
#define RSSPECAN_ATTR_WIMAX_FETC_RMS                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2159
#define RSSPECAN_ATTR_WIMAX_FETC_SST                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2160
#define RSSPECAN_ATTR_WIMAX_FETC_SYMB                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2161
#define RSSPECAN_ATTR_WIMAX_FETC_SYMB_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2162
#define RSSPECAN_ATTR_WIMAX_FETC_RMS_ALL                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2163
#define RSSPECAN_ATTR_WIMAX_FETC_RMS_PILOT                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2164
#define RSSPECAN_ATTR_WIMAX_FETC_RMS_DATA                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2165
#define RSSPECAN_ATTR_WIMAX_FETC_RMS_DLPR                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2166
#define RSSPECAN_ATTR_WIMAX_FETCH_NPIL_RMS                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2167
#define RSSPECAN_ATTR_WIMAX_FETC_ZONE_COUNT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2168
#define RSSPECAN_ATTR_WIMAX_FETC_TDOM_PREAMBLE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2169
#define RSSPECAN_ATTR_WIMAX_FETC_TDOM_SUBFRAME                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2170
#define RSSPECAN_ATTR_WIMAX_FETC_TDOM_ZONE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2171
#define RSSPECAN_ATTR_WIMAX_FETC_USERR                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2172
#define RSSPECAN_ATTR_WIMAX_FETC_ZONE_STAT_COUNT                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2173
#define RSSPECAN_ATTR_WIMAX_EGAT                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2174
#define RSSPECAN_ATTR_WIMAX_EGAT_HOLD_SAMP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2175
#define RSSPECAN_ATTR_WIMAX_EGAT_HOLD_TIME                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2176
#define RSSPECAN_ATTR_WIMAX_EGAT_LENG_SAMP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2177
#define RSSPECAN_ATTR_WIMAX_EGAT_LENG_TIME                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2178
#define RSSPECAN_ATTR_WIMAX_EGAT_LINK                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2179
#define RSSPECAN_ATTR_WIMAX_TRIG_MODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2180
#define RSSPECAN_ATTR_WIMAX_TRIG_POW_LEV                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2181
#define RSSPECAN_ATTR_WIMAX_TRIG_POW_LEV_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2182
#define RSSPECAN_ATTR_WIMAX_ANTENNA_AUTO_DETECTION                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2439
#define RSSPECAN_ATTR_WIMAX_ANTENNA_STATE                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2440
#define RSSPECAN_ATTR_WIMAX_ANTENNA_ADDRESS                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2441
#define RSSPECAN_ATTR_WIMAX_ANTENNA_CONFIGURATION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2442
#define RSSPECAN_ATTR_WIMAX_BAN                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2183
#define RSSPECAN_ATTR_WIMAX_BCON_AUTO                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2184
#define RSSPECAN_ATTR_WIMAX_BTYP                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2185
#define RSSPECAN_ATTR_WIMAX_CEST                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2186
#define RSSPECAN_ATTR_WIMAX_LMODE                                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2187
#define RSSPECAN_ATTR_WIMAX_SIGS                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2188
#define RSSPECAN_ATTR_WIMAX_SUBCHANNEL                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2189
#define RSSPECAN_ATTR_WIMAX_SUBCHANNEL_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2190
#define RSSPECAN_ATTR_WIMAX_SUBCHANNEL_ULPH                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2191
#define RSSPECAN_ATTR_WIMAX_SYM_EQU                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2192
#define RSSPECAN_ATTR_WIMAX_SYM                                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2193
#define RSSPECAN_ATTR_WIMAX_TRAC_LEV                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2194
#define RSSPECAN_ATTR_WIMAX_TRAC_PHAS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2195
#define RSSPECAN_ATTR_WIMAX_TRAC_TIME                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2196
#define RSSPECAN_ATTR_WIMAX_TDD_TTG                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2197
#define RSSPECAN_ATTR_WIMAX_FFT_OFFSET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2198
#define RSSPECAN_ATTR_WIMAX_CEST_OFDMA                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2199
#define RSSPECAN_ATTR_WIMAX_TRACK_PILOTS                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2200
#define RSSPECAN_ATTR_WIMAX_TRAC_CEST                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2201
#define RSSPECAN_ATTR_WIMAX_BURST_BOOSTING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2443
#define RSSPECAN_ATTR_WIMAX_AUTO_DEMODULATION_ZONE                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2444
#define RSSPECAN_ATTR_WIMAX_COMPENSATE_CROSSTALK                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2445
#define RSSPECAN_ATTR_WIMAX_DL_IDCELL                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2202
#define RSSPECAN_ATTR_WIMAX_DOWN_LINK_ID_CELL_AUTO                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2203
#define RSSPECAN_ATTR_WIMAX_FRAME_PREDEFINED_MAP                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2204
#define RSSPECAN_ATTR_WIMAX_DL_PREAMBLE_MODE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2205
#define RSSPECAN_ATTR_WIMAX_DL_PREAMBLE_INDEX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2206
#define RSSPECAN_ATTR_WIMAX_DL_SUBCHANNEL                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2207
#define RSSPECAN_ATTR_WIMAX_UL_FRAME                                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2208
#define RSSPECAN_ATTR_WIMAX_UL_SUBCHANNEL_BITMAP                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2209
#define RSSPECAN_ATTR_WIMAX_UL_FRAME_CRLEN                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2210
#define RSSPECAN_ATTR_WIMAX_ZONE_COUNT_QUERY                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2211
#define RSSPECAN_ATTR_WIMAX_ZONE_ANALYZE_STATE                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2212
#define RSSPECAN_ATTR_WIMAX_ZONE_TYPE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2213
#define RSSPECAN_ATTR_WIMAX_ZONE_SEGMENT_NUM                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2214
#define RSSPECAN_ATTR_WIMAX_ZONE_PERM_BASE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2215
#define RSSPECAN_ATTR_WIMAX_ZONE_PRBS                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2216
#define RSSPECAN_ATTR_WIMAX_ZONE_SPACE_TIME_CODING                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2217
#define RSSPECAN_ATTR_WIMAX_ZONE_MATRIX                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2218
#define RSSPECAN_ATTR_WIMAX_ZONE_DELETE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2219
#define RSSPECAN_ATTR_WIMAX_ZONE_RESET                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2220
#define RSSPECAN_ATTR_WIMAX_ZONE_COUNT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2221
#define RSSPECAN_ATTR_WIMAX_ZONE_COUNT_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2222
#define RSSPECAN_ATTR_WIMAX_UPLINK_ZONE_STC_MODE                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2446
#define RSSPECAN_ATTR_WIMAX_ZONE_SYMBOL_COUNT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2223
#define RSSPECAN_ATTR_WIMAX_ZONE_SYMBOL_OFFSET                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2224
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_COUNT_QUERY                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2225
#define RSSPECAN_ATTR_WIMAX_MIMO_BURST_STC_MODE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2447
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_FORMAT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2226
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_OFFSET_AUTO                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2227
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_SLOT_DURATION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2228
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_SUBC_COUNT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2229
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_SUBC_OFFSET                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2230
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_SYMBOL_COUNT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2231
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_SYMBOL_OFFSET                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2232
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_POWER                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2233
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_TYPE                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2234
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_MATRIX                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2235
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_DELETE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2236
#define RSSPECAN_ATTR_WIMAX_ZONE_BURST_RESET                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2237
#define RSSPECAN_ATTR_WIMAX_REFRESH                                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2238
#define RSSPECAN_ATTR_WIMAX_QUERY_TTC                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2239
#define RSSPECAN_ATTR_LTE_DOWNLINK_MODE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2240
#define RSSPECAN_ATTR_LTE_DOWNLINK_STORE_DEMODULATION_SETTINGS              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2241
#define RSSPECAN_ATTR_LTE_DOWNLINK_LOAD_DEMODULATION_SETTINGS               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2242
#define RSSPECAN_ATTR_LTE_DOWNLINK_MEASUREMENT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 3126
#define RSSPECAN_ATTR_LTE_DOWNLINK_DUPLEXING                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2243
#define RSSPECAN_ATTR_LTE_DOWNLINK_CHANNEL_BANDWIDTH                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2244
#define RSSPECAN_ATTR_LTE_DOWNLINK_NUMBER_OF_RESOURCE_BLOCKS                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2245
#define RSSPECAN_ATTR_LTE_DOWNLINK_CYCLIC_PREFIX                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2246
#define RSSPECAN_ATTR_LTE_DOWNLINK_AUTO_REFERENCE_LEVEL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2247
#define RSSPECAN_ATTR_LTE_DOWNLINK_REFERENCE_LEVEL                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2248
#define RSSPECAN_ATTR_LTE_DOWNLINK_REFERENCE_LEVEL_IN_BB_MODE               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2249
#define RSSPECAN_ATTR_LTE_DOWNLINK_MIMO_CONFIGURATION                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2250
#define RSSPECAN_ATTR_LTE_DOWNLINK_MIMO_ANTENNA                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2251
#define RSSPECAN_ATTR_LTE_DOWNLINK_IQ_SWAP                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2252
#define RSSPECAN_ATTR_LTE_DOWNLINK_INPUT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2253
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_STATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 3127
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_ANTENA_PORTS                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3128
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_CONFIGURATION_INDEX               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3129
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_OVERWRITE_PDSCH                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 3130
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_RELATIVE_POWER                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 3131
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_SUBFRAME_CONFIGURATION            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3132
#define RSSPECAN_ATTR_LTE_DOWNLINK_CSI_RS_FRAME_NUMBER_OFFSET               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3133
#define RSSPECAN_ATTR_LTE_DOWNLINK_CHANNEL_ESTIMATION                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2254
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_CALCULATION_METHOD                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2255
#define RSSPECAN_ATTR_LTE_DOWNLINK_SCRAMBLING_OF_CODED_BITS                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2256
#define RSSPECAN_ATTR_LTE_DOWNLINK_AUTO_DEMODULATION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2257
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_SUBFRAME_DETECT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2731
#define RSSPECAN_ATTR_LTE_DOWNLINK_BOOSTING_ESTIMATION                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2732
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_REFERENCE_DATA                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2733
#define RSSPECAN_ATTR_LTE_DOWNLINK_MULTICARRIER_FILTER                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2734
#define RSSPECAN_ATTR_LTE_DOWNLINK_PHASE_TRACKING                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2258
#define RSSPECAN_ATTR_LTE_DOWNLINK_TIMING                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2259
#define RSSPECAN_ATTR_LTE_DOWNLINK_TDD_UL_DL_ALLOCATIONS                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2260
#define RSSPECAN_ATTR_LTE_DOWNLINK_TDD_SPECIAL_SUBFRAME                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2937
#define RSSPECAN_ATTR_LTE_DOWNLINK_CID_GROUP_AUTO                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2261
#define RSSPECAN_ATTR_LTE_DOWNLINK_CELL_IDENTITY_GROUP                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2262
#define RSSPECAN_ATTR_LTE_DOWNLINK_CELL_IDENTITY_AUTO                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2263
#define RSSPECAN_ATTR_LTE_DOWNLINK_CELL_IDENTITY                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2264
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_CONFIGURABLE_SUBFRAMES             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2265
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_USED_ALLOCATIONS                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2266
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_MODULATION              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2267
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_RESOURCE_BLOCKS         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2268
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_RESOURCE_BLOCKS_OFFSET  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2269
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_POWER                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2270
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_ALLOCATION_START_OFFSET            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3134
#define RSSPECAN_ATTR_LTE_DOWNLINK_REFERENCE_POWER                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2271
#define RSSPECAN_ATTR_LTE_DOWNLINK_PRB_SYMBOL_OFFSET                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2604
#define RSSPECAN_ATTR_LTE_DOWNLINK_PSYNC_RELATIVE_POWER                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2272
#define RSSPECAN_ATTR_LTE_DOWNLINK_SSYNC_RELATIVE_POWER                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2273
#define RSSPECAN_ATTR_LTE_DOWNLINK_PBCH_STATE                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2274
#define RSSPECAN_ATTR_LTE_DOWNLINK_PBCH_RELATIVE_POWER                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2275
#define RSSPECAN_ATTR_LTE_DOWNLINK_PCFICH_STATE                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2276
#define RSSPECAN_ATTR_LTE_DOWNLINK_PCFICH_RELATIVE_POWER                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2277
#define RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_NUMBER_OF_GROUPS                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2278
#define RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_DURATION                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2279
#define RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_RELATIVE_POWER                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2280
#define RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_NG_PARAMETER                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3081
#define RSSPECAN_ATTR_LTE_DOWNLINK_PHICH_ENHANCED_TEST_MODELS               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3082
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDCCH_NUMBER_OF_PDCCHS                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2281
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDCCH_FORMAT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2282
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDCCH_RELATIVE_POWER                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2283
#define RSSPECAN_ATTR_LTE_DOWNLINK_PDSCH_POWER_RATIO                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3106
#define RSSPECAN_ATTR_LTE_DOWNLINK_MEASUREMENT_TYPE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2284
#define RSSPECAN_ATTR_LTE_DOWNLINK_SUBFRAME_SELECTION                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2285
#define RSSPECAN_ATTR_LTE_DOWNLINK_SUBFRAME_SELECTION_ALL                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2286
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_UNITS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2287
#define RSSPECAN_ATTR_LTE_DOWNLINK_BIT_STREAM_FORMAT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2288
#define RSSPECAN_ATTR_LTE_DOWNLINK_SEM_CHANNEL                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2289
#define RSSPECAN_ATTR_LTE_DOWNLINK_ACP_ASSUMED_ADJ_CHANNEL_CARRIER          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2290
#define RSSPECAN_ATTR_LTE_DOWNLINK_ACP_NOISE_CORRECTION                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2291
#define RSSPECAN_ATTR_LTE_DOWNLINK_CONSTELLATION_LOCATION                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2292
#define RSSPECAN_ATTR_LTE_DOWNLINK_Y_AXIS_SCALING_AUTO                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2938
#define RSSPECAN_ATTR_LTE_DOWNLINK_Y_AXIS_SCALING_PER_DIVISION              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2939
#define RSSPECAN_ATTR_LTE_DOWNLINK_Y_AXIS_SCALING_OFFSET                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2940
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PDSCH_QPSK_RESULT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2293
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PDSCH_16QAM_RESULT                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2294
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PDSCH_64QAM_RESULT                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2295
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_ALL_RESULT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2296
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PHYSICAL_CHANNEL_RESULT              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2297
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_PHYSICAL_SIGNAL_RESULT               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2298
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_SDSF_AVERAGE                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 3152
#define RSSPECAN_ATTR_LTE_DOWNLINK_FREQUENCY_ERROR_RESULT                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2299
#define RSSPECAN_ATTR_LTE_DOWNLINK_SAMPLING_ERROR_RESULT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2300
#define RSSPECAN_ATTR_LTE_DOWNLINK_IQ_OFFSET_RESULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2301
#define RSSPECAN_ATTR_LTE_DOWNLINK_IQ_GAIN_IMBALANCE_RESULT                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2302
#define RSSPECAN_ATTR_LTE_DOWNLINK_IQ_QUADRATURE_ERROR_RESULT               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2303
#define RSSPECAN_ATTR_LTE_DOWNLINK_FRAME_POWER_RESULT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2304
#define RSSPECAN_ATTR_LTE_DOWNLINK_CREST_FACTOR_RESULT                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2305
#define RSSPECAN_ATTR_LTE_DOWNLINK_TRIGGER_TO_FRAME_RESULT                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2306
#define RSSPECAN_ATTR_LTE_DOWNLINK_OSTP_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2307
#define RSSPECAN_ATTR_LTE_DOWNLINK_RSTP_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2735
#define RSSPECAN_ATTR_LTE_DOWNLINK_RSSI_RESULT                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 3153
#define RSSPECAN_ATTR_LTE_DOWNLINK_DETECTED_CELL_IDENTITY_GROUP             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2308
#define RSSPECAN_ATTR_LTE_DOWNLINK_DETECTED_CELL_IDENTITY                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2309
#define RSSPECAN_ATTR_LTE_DOWNLINK_DETECTED_CYCLIC_PREFIX                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2310
#define RSSPECAN_ATTR_LTE_UPLINK_MODE                                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2473
#define RSSPECAN_ATTR_LTE_LINK_DIRECTION                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2474
#define RSSPECAN_ATTR_LTE_UPLINK_STORE_DEMODULATION_SETTINGS                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2475
#define RSSPECAN_ATTR_LTE_UPLINK_LOAD_DEMODULATION_SETTINGS                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2476
#define RSSPECAN_ATTR_LTE_UPLINK_TRANSIENT_SLOTS                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3154
#define RSSPECAN_ATTR_LTE_UPLINK_DUPLEXING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2477
#define RSSPECAN_ATTR_LTE_UPLINK_CHANNEL_BANDWIDTH                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2478
#define RSSPECAN_ATTR_LTE_UPLINK_NUMBER_OF_RESOURCE_BLOCKS                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2479
#define RSSPECAN_ATTR_LTE_UPLINK_CYCLIC_PREFIX                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2480
#define RSSPECAN_ATTR_LTE_UPLINK_AUTO_REFERENCE_LEVEL                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2481
#define RSSPECAN_ATTR_LTE_UPLINK_REFERENCE_LEVEL                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2482
#define RSSPECAN_ATTR_LTE_UPLINK_REFERENCE_LEVEL_IN_BB_MODE                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2483
#define RSSPECAN_ATTR_LTE_UPLINK_TRIGGER_IF_POWER_LEVEL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2578
#define RSSPECAN_ATTR_LTE_UPLINK_TRIGGER_POWER_LEVEL_AUTO                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2736
#define RSSPECAN_ATTR_LTE_UPLINK_IQ_SWAP                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2484
#define RSSPECAN_ATTR_LTE_UPLINK_INPUT                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2485
#define RSSPECAN_ATTR_LTE_UPLINK_AUTO_LEVEL_TRACK_TIME                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2486
#define RSSPECAN_ATTR_LTE_UPLINK_RF_ATTENUATION                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2555
#define RSSPECAN_ATTR_LTE_UPLINK_CHANNEL_ESTIMATION                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2487
#define RSSPECAN_ATTR_LTE_UPLINK_COMPENSATE_DC_OFFSET                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2488
#define RSSPECAN_ATTR_LTE_UPLINK_AUTO_DEMODULATION                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2489
#define RSSPECAN_ATTR_LTE_UPLINK_SCRAMBLING_OF_CODED_BITS                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2791
#define RSSPECAN_ATTR_LTE_UPLINK_SUPPRESSED_INTERFERENCE_SYNC               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2737
#define RSSPECAN_ATTR_LTE_UPLINK_PHASE_TRACKING                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2490
#define RSSPECAN_ATTR_LTE_UPLINK_TIMING                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2491
#define RSSPECAN_ATTR_LTE_UPLINK_TDD_UL_DL_ALLOCATIONS                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2492
#define RSSPECAN_ATTR_LTE_UPLINK_TDD_SPECIAL_SUBFRAME                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2941
#define RSSPECAN_ATTR_LTE_UPLINK_CELL_IDENTITY_GROUP                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2493
#define RSSPECAN_ATTR_LTE_UPLINK_CELL_IDENTITY                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2494
#define RSSPECAN_ATTR_LTE_UPLINK_CONFIGURABLE_SUBFRAMES                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2495
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_ENABLE_PUCCH           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2496
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_MODULATION             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2497
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_RESOURCE_BLOCKS        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2498
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_RESOURCE_BLOCKS_OFFSET RS_SPECIFIC_PUBLIC_ATTR_BASE + 2499
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_ALLOCATION_POWER                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2500
#define RSSPECAN_ATTR_LTE_UPLINK_POWER_PUSCH                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2501
#define RSSPECAN_ATTR_LTE_UPLINK_POWER_PUCCH                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2502
#define RSSPECAN_ATTR_LTE_UPLINK_GROUP_HOPPING                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2503
#define RSSPECAN_ATTR_LTE_UPLINK_SEQUENCE_HOPPING                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2504
#define RSSPECAN_ATTR_LTE_UPLINK_DELTA_SEQUENCE_SHIFT                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2505
#define RSSPECAN_ATTR_LTE_UPLINK_N_DMRS                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2506
#define RSSPECAN_ATTR_LTE_UPLINK_N_PRS_STATE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2507
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_PRESENT          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2508
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_POWER            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2509
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_SYMBOL_OFFSET    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2510
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_SUBCARRIER_OFFSET RS_SPECIFIC_PUBLIC_ATTR_BASE + 2511
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_SUBCARRIERS      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2512
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_N_RRC            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2792
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_AN_TX            RS_SPECIFIC_PUBLIC_ATTR_BASE + 3135
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_ALPHA_PARAMETER  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2513
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_U_PARAMETER      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2514
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_MODE             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2515
#define RSSPECAN_ATTR_LTE_UPLINK_SOUNDING_REFERENCE_SIGNAL_Q_PARAMETER      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2516
#define RSSPECAN_ATTR_LTE_UPLINK_PUSCH_FREQUENCY_HOPPING_MODE               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2517
#define RSSPECAN_ATTR_LTE_UPLINK_PUSCH_SUBBANDS                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2518
#define RSSPECAN_ATTR_LTE_UPLINK_PUSCH_HOPPING_OFFSET                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3083
#define RSSPECAN_ATTR_LTE_UPLINK_PUSCH_INFO_IN_HOPPING_BITS                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3084
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_RESOURCE_BLOCKS                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2519
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_DELTA_SHIFT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2520
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_DELTA_OFFSET                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2521
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_CYCLIC_SHIFTS                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2522
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_BANDWIDTH                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2523
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_FORMAT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2524
#define RSSPECAN_ATTR_LTE_UPLINK_PUCCH_RESOURCE_INDEX                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 2525
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_CONFIGURATION                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 3085
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_RESTRICTED_SET                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3086
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_FREQUENCY_OFFSET                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 3087
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_NCS_CONFIGURATION                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 3088
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_LOGICAL_ROOT_SEQ_INDEX               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3089
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_SEQUENCE_INDEX                       RS_SPECIFIC_PUBLIC_ATTR_BASE + 3090
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_SEQUENCE_INDEX_VALUE                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3091
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_AUTOMATIC_PREAMBLE_MAPPING           RS_SPECIFIC_PUBLIC_ATTR_BASE + 3092
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_FREQUENCY_INDEX                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 3093
#define RSSPECAN_ATTR_LTE_UPLINK_PRACH_HALF_FRAME_INDICATOR                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3094
#define RSSPECAN_ATTR_LTE_UPLINK_MEASUREMENT_TYPE                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2526
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_SELECTION                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2527
#define RSSPECAN_ATTR_LTE_UPLINK_SUBFRAME_SELECTION_ALL                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2528
#define RSSPECAN_ATTR_LTE_UPLINK_SLOT_SELECTION                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2738
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_UNITS                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2529
#define RSSPECAN_ATTR_LTE_UPLINK_BIT_STREAM_FORMAT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2530
#define RSSPECAN_ATTR_LTE_UPLINK_SEM_CHANNEL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2531
#define RSSPECAN_ATTR_LTE_UPLINK_ACLR_ASSUMED_ADJ_CHANNEL_CARRIER           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2532
#define RSSPECAN_ATTR_LTE_UPLINK_ACLR_NOISE_CORRECTION                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2533
#define RSSPECAN_ATTR_LTE_UPLINK_Y_AXIS_SCALING_AUTO                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2942
#define RSSPECAN_ATTR_LTE_UPLINK_Y_AXIS_SCALING_PER_DIVISION                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2943
#define RSSPECAN_ATTR_LTE_UPLINK_Y_AXIS_SCALING_OFFSET                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2944
#define RSSPECAN_ATTR_LTE_UPLINK_MIMO_ANTENNA                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 3155
#define RSSPECAN_ATTR_LTE_UPLINK_MIMO_PUCCH                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3156
#define RSSPECAN_ATTR_LTE_UPLINK_MIMO_PUSCH                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 3157
#define RSSPECAN_ATTR_LTE_UPLINK_MIMO_SRS                                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 3158
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PDSCH_QPSK_RESULT                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2534
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PDSCH_16QAM_RESULT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2535
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PDSCH_64QAM_RESULT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2536
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PUSCH_QPSK_RESULT                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2537
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PUSCH_16QAM_RESULT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2538
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PUSCH_64QAM_RESULT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2539
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_DMRS_PUSCH_RESULT                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2739
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_DMRS_PUSCH_QPSK_RESULT                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2740
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_DMRS_PUSCH_16QAM_RESULT                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2741
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PUCCH_RESULT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2742
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PRACH_RESULT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2743
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_ALL_RESULT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2540
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PHYSICAL_CHANNEL_RESULT                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2541
#define RSSPECAN_ATTR_LTE_UPLINK_EVM_PHYSICAL_SIGNAL_RESULT                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2542
#define RSSPECAN_ATTR_LTE_UPLINK_FREQUENCY_ERROR_RESULT                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2543
#define RSSPECAN_ATTR_LTE_UPLINK_SAMPLING_ERROR_RESULT                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2544
#define RSSPECAN_ATTR_LTE_UPLINK_IQ_OFFSET_RESULT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2545
#define RSSPECAN_ATTR_LTE_UPLINK_IQ_GAIN_IMBALANCE_RESULT                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2546
#define RSSPECAN_ATTR_LTE_UPLINK_IQ_QUADRATURE_ERROR_RESULT                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2547
#define RSSPECAN_ATTR_LTE_UPLINK_FRAME_POWER_RESULT                         RS_SPECIFIC_PUBLIC_ATTR_BASE + 2548
#define RSSPECAN_ATTR_LTE_UPLINK_CREST_FACTOR_RESULT                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2549
#define RSSPECAN_ATTR_LTE_UPLINK_TRIGGER_TO_FRAME_RESULT                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2550
#define RSSPECAN_ATTR_LTE_UPLINK_DETECTED_CELL_IDENTITY_GROUP               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2552
#define RSSPECAN_ATTR_LTE_UPLINK_DETECTED_CELL_IDENTITY                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2553
#define RSSPECAN_ATTR_TETRA_MODE                                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2311
#define RSSPECAN_ATTR_TETRA_BURST_TYPE                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2312
#define RSSPECAN_ATTR_TETRA_CHANNEL_BANDWIDTH                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2313
#define RSSPECAN_ATTR_TETRA_REFERENCE_LEVEL_FOR_RF_MEASUREMENT              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2314
#define RSSPECAN_ATTR_TETRA_REFERENCE_LEVEL_FOR_IQ_MEASUREMENT              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2315
#define RSSPECAN_ATTR_TETRA_TEDS_SLOT_DURATION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2316
#define RSSPECAN_ATTR_TETRA_NUMBER_OF_BURST                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2317
#define RSSPECAN_ATTR_TETRA_OVERALL_BURST_COUNT                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2318
#define RSSPECAN_ATTR_TETRA_TRIGGER_MODE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2319
#define RSSPECAN_ATTR_TETRA_TRIGGER_POWER_LEVEL                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2320
#define RSSPECAN_ATTR_TETRA_TRIGGER_IQ_LEVEL                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2321
#define RSSPECAN_ATTR_TETRA_TRIGGER_TEDS_SLOT_DURATION                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2322
#define RSSPECAN_ATTR_TETRA_INPUT_SELECT                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2323
#define RSSPECAN_ATTR_TETRA_IQ_SETTING                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2324
#define RSSPECAN_ATTR_TETRA_IQ_CENTER_FREQUENCY                             RS_SPECIFIC_PUBLIC_ATTR_BASE + 2325
#define RSSPECAN_ATTR_TETRA_PAYLOAD_MODULATION                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2326
#define RSSPECAN_ATTR_TETRA_MAXIMUM_CARRIER_OFFSET                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2327
#define RSSPECAN_ATTR_TETRA_PILOT_TRACKING                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2328
#define RSSPECAN_ATTR_TETRA_COMPENSATE_AMPLITUDE_DROOP                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2329
#define RSSPECAN_ATTR_TETRA_COMPENSATE_IQ_OFFSET                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2330
#define RSSPECAN_ATTR_TETRA_MOVE_MARKER                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2331
#define RSSPECAN_ATTR_TETRA_MARKER_AMPLITUDE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2332
#define RSSPECAN_ATTR_TETRA_EVM_SUMMARY_LIMITS                              RS_SPECIFIC_PUBLIC_ATTR_BASE + 2333
#define RSSPECAN_ATTR_TETRA_ACP_LIMITS                                      RS_SPECIFIC_PUBLIC_ATTR_BASE + 2334
#define RSSPECAN_ATTR_TETRA_SUMMARY_TABLE_MEASUREMENT_RESULT                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2335
#define RSSPECAN_ATTR_TETRA_FETCH_ACP_MEASUREMENT                           RS_SPECIFIC_PUBLIC_ATTR_BASE + 2336
#define RSSPECAN_ATTR_TETRA_FETCH_ACP_MODULATION_RBW                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2337
#define RSSPECAN_ATTR_TETRA_PVT_MEASUREMENT                                 RS_SPECIFIC_PUBLIC_ATTR_BASE + 2338
#define RSSPECAN_ATTR_TETRA_PVT_TRIGGER_TO_SYNC_TIME                        RS_SPECIFIC_PUBLIC_ATTR_BASE + 2339
#define RSSPECAN_ATTR_TETRA_SPECTRUM_FFT_MEASUREMENT_RBW                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2340
#define RSSPECAN_ATTR_TETRA_TRACE_X_AXIS_MIN_MAX                            RS_SPECIFIC_PUBLIC_ATTR_BASE + 2341
#define RSSPECAN_ATTR_TETRA_IQ_SAMPLE_RATE                                  RS_SPECIFIC_PUBLIC_ATTR_BASE + 2342
#define RSSPECAN_ATTR_TETRA_RESULT_SUMMARY_LIMIT_CHECK_RESULT               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2343
#define RSSPECAN_ATTR_TETRA_ACP_LIMIT_CHECK_RESULT                          RS_SPECIFIC_PUBLIC_ATTR_BASE + 2344
#define RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2345
#define RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE_AUTO                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2346
#define RSSPECAN_ATTR_TETRA_MEASUREMENT_TYPE                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2347
#define RSSPECAN_ATTR_TETRA_Y_AXIS_TRACE_UNIT                               RS_SPECIFIC_PUBLIC_ATTR_BASE + 2348
#define RSSPECAN_ATTR_TETRA_CONSTELLATION_SYMBOL_SETTINGS                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2349
#define RSSPECAN_ATTR_TETRA_CONSTELLATION_CARRIER_NUMBER                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2350
#define RSSPECAN_ATTR_TETRA_EVM_RESULT_UNITS                                RS_SPECIFIC_PUBLIC_ATTR_BASE + 2351
#define RSSPECAN_ATTR_TETRA_GAIN_IMBALANCE_RESULT_UNITS                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2352
#define RSSPECAN_ATTR_TETRA_BURST_RECALC                                    RS_SPECIFIC_PUBLIC_ATTR_BASE + 2353
#define RSSPECAN_ATTR_ID_QUERY_RESPONSE                                     RS_SPECIFIC_PUBLIC_ATTR_BASE + 2354
#define RSSPECAN_ATTR_VSA_MODULATON_QUESTIONABLE_REGISTER                   RS_SPECIFIC_PUBLIC_ATTR_BASE + 2661

// rsspecan_rngUnits
#define RSSPECAN_VAL_UNIT_DBM                                               0
#define RSSPECAN_VAL_UNIT_DBPW                                              1
#define RSSPECAN_VAL_UNIT_WATT                                              2
#define RSSPECAN_VAL_UNIT_DBUV                                              3
#define RSSPECAN_VAL_UNIT_DBMV                                              4
#define RSSPECAN_VAL_UNIT_VOLT                                              5
#define RSSPECAN_VAL_UNIT_DBUA                                              6
#define RSSPECAN_VAL_UNIT_AMP                                               7
#define RSSPECAN_VAL_UNIT_DB                                                8
#define RSSPECAN_VAL_UNIT_DEG                                               9
#define RSSPECAN_VAL_UNIT_RAD                                               10
#define RSSPECAN_VAL_UNIT_S                                                 11
#define RSSPECAN_VAL_UNIT_HZ                                                12
#define RSSPECAN_VAL_UNIT_PCT                                               13
#define RSSPECAN_VAL_UNIT_UNITLESS                                          14
#define RSSPECAN_VAL_UNIT_DBPT                                              15
#define RSSPECAN_VAL_UNIT_DBMV_MHZ                                          16
#define RSSPECAN_VAL_UNIT_DBUV_MHZ                                          17
#define RSSPECAN_VAL_UNIT_DBUA_MHZ                                          18
#define RSSPECAN_VAL_UNIT_DBMV_M                                            19
#define RSSPECAN_VAL_UNIT_DBUA_M                                            20
#define RSSPECAN_VAL_UNIT_DBMV_MMHZ                                         21
#define RSSPECAN_VAL_UNIT_DBUA_MMHZ                                         22
#define RSSPECAN_VAL_UNIT_DBUV_M                                            23
#define RSSPECAN_VAL_UNIT_DBCHZ                                             24
#define RSSPECAN_VAL_UNIT_DBC                                               25

// rsspecan_rngPmetCoupling
#define RSSPECAN_VAL_PMET_COUP_OFF                                          0
#define RSSPECAN_VAL_PMET_COUP_CENT                                         1
#define RSSPECAN_VAL_PMET_COUP_MARK                                         2

// rsspecan_rngBtoGeog
#define RSSPECAN_BTO_GEOG_EUR                                               0
#define RSSPECAN_BTO_GEOG_USA                                               1
#define RSSPECAN_BTO_GEOG_FRAN                                              2

// rsspecan_rngPmetMeasTime
#define RSSPECAN_VAL_PMET_MEASTIME_NORM                                     0
#define RSSPECAN_VAL_PMET_MEASTIME_SHORT                                    1
#define RSSPECAN_VAL_PMET_MEASTIME_LONG                                     2

// rsspecan_rngPmetUnitAbs
#define RSSPECAN_VAL_ABSUNIT_DB                                             3
#define RSSPECAN_VAL_ABSUNIT_VSWR                                           4

// rsspecan_rngBtoPtype
#define RSSPECAN_VAL_BTO_PTYPE_AUTO                                         0
#define RSSPECAN_VAL_BTO_PTYPE_DH1                                          1
#define RSSPECAN_VAL_BTO_PTYPE_DH3                                          2
#define RSSPECAN_VAL_BTO_PTYPE_DH5                                          3
#define RSSPECAN_BTO_PTYPE_DM1                                              4
#define RSSPECAN_BTO_PTYPE_DM3                                              5
#define RSSPECAN_BTO_PTYPE_DM5                                              6
#define RSSPECAN_BTO_PTYPE_FHS                                              7
#define RSSPECAN_BTO_PTYPE_HV1                                              8
#define RSSPECAN_BTO_PTYPE_HV2                                              9
#define RSSPECAN_BTO_PTYPE_HV3                                              10
#define RSSPECAN_BTO_PTYPE_DV                                               11
#define RSSPECAN_BTO_PTYPE_NULL                                             12
#define RSSPECAN_BTO_PTYPE_POLL                                             13
#define RSSPECAN_BTO_PTYPE_AUX1                                             14
#define RSSPECAN_BTO_PTYPE_UNDEF                                            15

// rsspecan_rngBtoMeasFilt
#define RSSPECAN_BTO_MEASFILT_OFF                                           0
#define RSSPECAN_BTO_MEASFILT_BTO                                           1

// rsspecan_rngTraceMode
#define RSSPECAN_TRAC_MOD_WRITE                                             0
#define RSSPECAN_TRAC_MOD_VIEW                                              1
#define RSSPECAN_TRAC_MOD_AVER                                              2
#define RSSPECAN_TRAC_MOD_MAXH                                              3
#define RSSPECAN_TRAC_MOD_MINH                                              4
#define RSSPECAN_TRAC_MOD_RMS                                               5

// rsspecan_rngBtoTraceDetector
#define RSSPECAN_BTO_TRAC_DET_APEAK                                         0
#define RSSPECAN_BTO_TRAC_DET_NEG                                           1
#define RSSPECAN_BTO_TRAC_DET_POS                                           2
#define RSSPECAN_BTO_TRAC_DET_SAMPLE                                        3
#define RSSPECAN_BTO_TRAC_DET_RMS                                           4
#define RSSPECAN_BTO_TRAC_DET_AVER                                          5

// rsspecan_rngBtoMeasMode
#define RSSPECAN_BTO_MEAS_MODE_OPOW                                         0
#define RSSPECAN_BTO_MEAS_MODE_ACLR                                         1
#define RSSPECAN_BTO_MEAS_MODE_MCH                                          2
#define RSSPECAN_BTO_MEAS_MODE_ICFT                                         3
#define RSSPECAN_BTO_MEAS_MODE_CFDR                                         4
#define RSSPECAN_BTO_MEAS_MODE_RTP                                          5
#define RSSPECAN_BTO_MEAS_MODE_IBS                                          6
#define RSSPECAN_BTO_MEAS_MODE_DPEN                                         7
#define RSSPECAN_BTO_MEAS_MODE_CFST                                         8

// rsspecan_rngGsmMtyp
#define RSSPECAN_VAL_GSM_MTYP_GMSK                                          0
#define RSSPECAN_VAL_GSM_MTYP_EDGE                                          1

// rsspecan_rngGsmSlotMult
#define RSSPECAN_VAL_GSM_SLOT_11                                            0
#define RSSPECAN_VAL_GSM_SLOT_21                                            1
#define RSSPECAN_VAL_GSM_SLOT_22                                            2
#define RSSPECAN_VAL_GSM_SLOT_31                                            3
#define RSSPECAN_VAL_GSM_SLOT_32                                            4
#define RSSPECAN_VAL_GSM_SLOT_33                                            5
#define RSSPECAN_VAL_GSM_SLOT_41                                            6
#define RSSPECAN_VAL_GSM_SLOT_42                                            7
#define RSSPECAN_VAL_GSM_SLOT_43                                            8
#define RSSPECAN_VAL_GSM_SLOT_44                                            9
#define RSSPECAN_VAL_GSM_SLOT_81                                            10
#define RSSPECAN_VAL_GSM_SLOT_82                                            11
#define RSSPECAN_VAL_GSM_SLOT_83                                            12
#define RSSPECAN_VAL_GSM_SLOT_84                                            13
#define RSSPECAN_VAL_GSM_SLOT_85                                            14
#define RSSPECAN_VAL_GSM_SLOT_86                                            15
#define RSSPECAN_VAL_GSM_SLOT_87                                            16
#define RSSPECAN_VAL_GSM_SLOT_88                                            17

// rsspecan_rngGsmTsc
#define RSSPECAN_VAL_GSM_TSC_0                                              0
#define RSSPECAN_VAL_GSM_TSC_1                                              1
#define RSSPECAN_VAL_GSM_TSC_2                                              2
#define RSSPECAN_VAL_GSM_TSC_3                                              3
#define RSSPECAN_VAL_GSM_TSC_4                                              4
#define RSSPECAN_VAL_GSM_TSC_5                                              5
#define RSSPECAN_VAL_GSM_TSC_6                                              6
#define RSSPECAN_VAL_GSM_TSC_7                                              7
#define RSSPECAN_VAL_GSM_TSC_USER                                           8
#define RSSPECAN_VAL_GSM_TSC_AB0                                            9
#define RSSPECAN_VAL_GSM_TSC_AB1                                            10
#define RSSPECAN_VAL_GSM_TSC_AB2                                            11

// rsspecan_rngAmplitudeUnitsRangeTable
#define RSSPECAN_VAL_AMPLITUDE_UNITS_DBM                                    0
#define RSSPECAN_VAL_AMPLITUDE_UNITS_DBMV                                   1
#define RSSPECAN_VAL_AMPLITUDE_UNITS_DBUV                                   2
#define RSSPECAN_VAL_AMPLITUDE_UNITS_VOLT                                   3
#define RSSPECAN_VAL_AMPLITUDE_UNITS_WATT                                   4
#define RSSPECAN_VAL_AMPLITUDE_UNITS_DBPW                                   5
#define RSSPECAN_VAL_AMPLITUDE_UNITS_DBUA                                   6
#define RSSPECAN_VAL_AMPLITUDE_UNITS_A                                      7

// rsspecan_rngGsmPtemFilt
#define RSSPECAN_VAL_GSM_FILT_G500                                          0
#define RSSPECAN_VAL_GSM_FILT_B600                                          1

// rsspecan_rngGsmBurstSection
#define RSSPECAN_VAL_GSM_BURST_SEC_FULL                                     0
#define RSSPECAN_VAL_GSM_BURST_SEC_TOP                                      1
#define RSSPECAN_VAL_GSM_BURST_SEC_RIS                                      2
#define RSSPECAN_VAL_GSM_BURST_SEC_FALL                                     3
#define RSSPECAN_VAL_GSM_BURST_SEC_FRZ                                      4

// rsspecan_rngAbsRel
#define RSSPECAN_VAL_ABS                                                    0
#define RSSPECAN_VAL_REL                                                    1

// rsspecan_rngESPMode
#define RSSPECAN_VAL_ESP_MODE_AUTO                                          0
#define RSSPECAN_VAL_ESP_MODE_USER                                          1
#define RSSPECAN_VAL_ESP_MODE_MAN                                           2

// rsspecan_rngDetectorTypeRangeTable
#define RSSPECAN_VAL_DETECTOR_TYPE_AUTO_PEAK                                0
#define RSSPECAN_VAL_DETECTOR_TYPE_AVERAGE                                  1
#define RSSPECAN_VAL_DETECTOR_TYPE_MAX_PEAK                                 2
#define RSSPECAN_VAL_DETECTOR_TYPE_MIN_PEAK                                 3
#define RSSPECAN_VAL_DETECTOR_TYPE_SAMPLE                                   4
#define RSSPECAN_VAL_DETECTOR_TYPE_RMS                                      5
#define RSSPECAN_VAL_DETECTOR_TYPE_QPK                                      6

// rsspecan_rngC2KLimMode
#define RSSPECAN_VAL_C2K_LIM_MODE_AUTO                                      0
#define RSSPECAN_VAL_C2K_LIM_MODE_MAN                                       1
#define RSSPECAN_VAL_C2K_LIM_MODE_USER                                      2

// rsspecan_rngNoiseDispDataTrac
#define RSSPECAN_VAL_NOISE_DISP_DATA_TEFF                                   1
#define RSSPECAN_VAL_NOISE_DISP_DATA_NFIGURE                                2

// rsspecan_rngNoiseCalcLimLine
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_NFIGURE                            0
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TEFFECTIVE                         1
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_GAIN                               2

// rsspecan_rngNoiseERNModeSel
#define RSSPECAN_VAL_NOISE_ERN_MODE_SEL_TABLE                               0
#define RSSPECAN_VAL_NOISE_ERN_MODE_SEL_SPOT                                1

// rsspecan_rngNoiseLossMode
#define RSSPECAN_VAL_NOISE_LOSS_MODE_TABLE                                  0
#define RSSPECAN_VAL_NOISE_LOSS_MODE_SPOT                                   1

// rsspecan_rngNoiseConfSystLosc
#define RSSPECAN_VAL_NOISE_CONF_SYST_LOSC_FIXED                             0
#define RSSPECAN_VAL_NOISE_CONF_SYST_LOSC_VARIABLE                          1

// rsspecan_rngNoiseConfDUT
#define RSSPECAN_VAL_NOISE_CONF_DUT_AMPLIFIER                               0
#define RSSPECAN_VAL_NOISE_CONF_DUT_DOWNCONV                                1
#define RSSPECAN_VAL_NOISE_CONF_DUT_UPCONV                                  2

// rsspecan_rngFFTBandStepMode
#define RSSPECAN_VAL_BSTEP_MODE_LIN                                         0
#define RSSPECAN_VAL_BSTEP_MODE_L1235                                       1

// rsspecan_rngCoupling
#define RSSPECAN_VAL_DC                                                     0
#define RSSPECAN_VAL_AC                                                     1

// rsspecan_rngWlanPvtRPow
#define RSSPECAN_VAL_PVT_MEAN                                               0
#define RSSPECAN_VAL_PVT_MAX                                                1

// rsspecan_rngSweMode
#define RSSPECAN_VAL_SWE_MODE_FAST                                          0
#define RSSPECAN_VAL_SWE_MODE_NORM                                          1
#define RSSPECAN_VAL_SWE_MODE_AVER                                          2
#define RSSPECAN_VAL_SWE_MODE_MAN                                           3

// rsspecan_rngBandType
#define RSSPECAN_VAL_BAND_TYPE_NORM                                         0
#define RSSPECAN_VAL_BAND_TYPE_FFT                                          1
#define RSSPECAN_VAL_BAND_TYPE_IQFFT                                        2

// rsspecan_rngListRangDet
#define RSSPECAN_VAL_LIST_RANG_DET_APE                                      0
#define RSSPECAN_VAL_LIST_RANG_DET_NEG                                      1
#define RSSPECAN_VAL_LIST_RANG_DET_POS                                      2
#define RSSPECAN_VAL_LIST_RANG_DET_SAMP                                     3
#define RSSPECAN_VAL_LIST_RANG_DET_RMS                                      4
#define RSSPECAN_VAL_LIST_RANG_DET_AVER                                     5

// rsspecan_rngResults
#define RSSPECAN_VAL_RES_FAIL                                               0
#define RSSPECAN_VAL_RES_PASS                                               1

// rsspecan_rngVSABurstMode
#define RSSPECAN_VAL_VSA_BURS_MODE_MEAS                                     0
#define RSSPECAN_VAL_VSA_BURS_MODE_BURS                                     1

// rsspecan_rngTrigMode
#define RSSPECAN_VAL_TRIG_MODE_IMM                                          0
#define RSSPECAN_VAL_TRIG_MODE_EXT                                          1
#define RSSPECAN_VAL_TRIG_MODE_POW                                          2
#define RSSPECAN_VAL_TRIG_MODE_PSEN                                         3
#define RSSPECAN_VAL_TRIG_MODE_RFP                                          4
#define RSSPECAN_VAL_TRIG_MODE_EFAL                                         5
#define RSSPECAN_VAL_TRIG_MODE_ERIS                                         6

// rsspecan_rngBurstSel
#define RSSPECAN_VAL_SELECTION_ALL                                          0
#define RSSPECAN_VAL_SELECTION_PIL                                          1

// rsspecan_rngDISPConfPreDefColour
#define RSSPECAN_VAL_DISP_COL_BLAC                                          0
#define RSSPECAN_VAL_DISP_COL_BLUE                                          1
#define RSSPECAN_VAL_DISP_COL_BROW                                          2
#define RSSPECAN_VAL_DISP_COL_GRE                                           3
#define RSSPECAN_VAL_DISP_COL_CYAN                                          4
#define RSSPECAN_VAL_DISP_COL_RED                                           5
#define RSSPECAN_VAL_DISP_COL_MAG                                           6
#define RSSPECAN_VAL_DISP_COL_YELL                                          7
#define RSSPECAN_VAL_DISP_COL_WHIT                                          8
#define RSSPECAN_VAL_DISP_COL_DGRA                                          9
#define RSSPECAN_VAL_DISP_COL_LGRA                                          10
#define RSSPECAN_VAL_DISP_COL_LBLU                                          11
#define RSSPECAN_VAL_DISP_COL_LGRE                                          12
#define RSSPECAN_VAL_DISP_COL_LCY                                           13
#define RSSPECAN_VAL_DISP_COL_LRED                                          14
#define RSSPECAN_VAL_DISP_COL_LMAG                                          15

// rsspecan_rngWiMAXFlatSel
#define RSSPECAN_VAL_WIMAX_FLAT_FLAT                                        0
#define RSSPECAN_VAL_WIMAX_FLAT_GRD                                         1
#define RSSPECAN_VAL_WIMAX_FLAT_DIF                                         2

// rsspecan_rngWlanPvtSel
#define RSSPECAN_VAL_PVT_EDGE                                               0
#define RSSPECAN_VAL_PVT_FULL                                               1
#define RSSPECAN_VAL_PVT_RISE                                               2
#define RSSPECAN_VAL_PVT_FALL                                               3

// rsspecan_rngSpecMask
#define RSSPECAN_VAL_SPEC_IEEE                                              0
#define RSSPECAN_VAL_SPEC_ETSI                                              1
#define RSSPECAN_VAL_SPEC_ETSI5                                             2
#define RSSPECAN_VAL_SPEC_ETSI10                                            3

// rsspecan_rng3GUEFeed
#define RSSPECAN_VAL_3GUE_RES_CDP                                           0
#define RSSPECAN_VAL_3GUE_RES_CDPRAT                                        1
#define RSSPECAN_VAL_3GUE_RES_CDPOVER                                       2
#define RSSPECAN_VAL_3GUE_RES_CDEP                                          3
#define RSSPECAN_VAL_3GUE_RES_SUM                                           4
#define RSSPECAN_VAL_3GUE_RES_CTAB                                          5
#define RSSPECAN_VAL_3GUE_RES_PCD                                           6
#define RSSPECAN_VAL_3GUE_RES_MACC                                          7
#define RSSPECAN_VAL_3GUE_RES_PVSLA                                         8
#define RSSPECAN_VAL_3GUE_RES_PVSLR                                         9
#define RSSPECAN_VAL_3GUE_RES_BSTR                                          10
#define RSSPECAN_VAL_3GUE_RES_PVSY                                          11
#define RSSPECAN_VAL_3GUE_RES_SYMBCONST                                     12
#define RSSPECAN_VAL_3GUE_RES_SYMBEVM                                       13
#define RSSPECAN_VAL_3GUE_RES_FVSL                                          14
#define RSSPECAN_VAL_3GUE_RES_PSVSL                                         15
#define RSSPECAN_VAL_3GUE_RES_EVM_MAGN                                      16
#define RSSPECAN_VAL_3GUE_RES_EVM_PHAS                                      17
#define RSSPECAN_VAL_3GUE_RES_COMP_CONS                                     18
#define RSSPECAN_VAL_3GUE_RES_CHIP_EVM                                      19
#define RSSPECAN_VAL_3GUE_RES_CHIP_MAGN                                     20
#define RSSPECAN_VAL_3GUE_RES_CHIP_PHAS                                     21

// rsspecan_rng3GUECDPBase
#define RSSPECAN_VAL_3GUE_CDP_BASE_SLOT                                     0
#define RSSPECAN_VAL_3GUE_CDP_BASE_FRAME                                    1

// rsspecan_rngIQ
#define RSSPECAN_VAL_I                                                      0
#define RSSPECAN_VAL_Q                                                      1

// rsspecan_rng3GUECDPScrType
#define RSSPECAN_VAL_3GUE_CDP_SCR_LONG                                      0
#define RSSPECAN_VAL_3GUE_CDP_SCR_SHORT                                     1

// rsspecan_rngTDSMeas
#define RSSPECAN_VAL_TDS_MEAS_POW                                           0
#define RSSPECAN_VAL_TDS_MEAS_ACLR                                          1
#define RSSPECAN_VAL_TDS_MEAS_ESP                                           2
#define RSSPECAN_VAL_TDS_MEAS_OBAN                                          3
#define RSSPECAN_VAL_TDS_MEAS_PVT                                           4
#define RSSPECAN_VAL_TDS_MEAS_CDP                                           5
#define RSSPECAN_VAL_TDS_MEAS_CCDF                                          6
#define RSSPECAN_VAL_TDS_MEAS_MCAC                                          7

// rsspecan_rngTDSFeed
#define RSSPECAN_VAL_TDS_RES_CDP                                            0
#define RSSPECAN_VAL_TDS_RES_CDPRAT                                         1
#define RSSPECAN_VAL_TDS_RES_CDEP                                           2
#define RSSPECAN_VAL_TDS_RES_MACC                                           3
#define RSSPECAN_VAL_TDS_RES_PVSLA                                          4
#define RSSPECAN_VAL_TDS_RES_PVSLR                                          5
#define RSSPECAN_VAL_TDS_RES_PVSYM                                          6
#define RSSPECAN_VAL_TDS_RES_BSTR                                           7
#define RSSPECAN_VAL_TDS_RES_SUM                                            8
#define RSSPECAN_VAL_TDS_RES_CTAB                                           9
#define RSSPECAN_VAL_TDS_RES_PCD                                            10
#define RSSPECAN_VAL_TDS_RES_SYMBCONST                                      11
#define RSSPECAN_VAL_TDS_RES_SYMBEVM                                        12
#define RSSPECAN_VAL_TDS_RES_COMPCONST                                      13
#define RSSPECAN_VAL_TDS_RES_COMPEVM                                        14

// rsspecan_rngFFTWindow
#define RSSPECAN_VAL_FFT_WIND_FLAT                                          0
#define RSSPECAN_VAL_FFT_WIND_EXP                                           1
#define RSSPECAN_VAL_FFT_WIND_HAMM                                          2
#define RSSPECAN_VAL_FFT_WIND_HANN                                          3
#define RSSPECAN_VAL_FFT_WIND_RECT                                          4
#define RSSPECAN_VAL_FFT_WIND_CHEB                                          5

// rsspecan_rngVSAResultFormat
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_MAGN                                 0
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_PHAS                                 1
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_UPHAS                                2
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_RIM                                  3
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_FREQ                                 4
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_COMP                                 5
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_CONS                                 6
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_GDEL                                 7
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_IEYE                                 8
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_QEYE                                 9
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_CONV                                 10
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_FEYE                                 11
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_CONF                                 12
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_CONVF                                13
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_IQC                                  14
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_NONE                                 15
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_RCON                                 16
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_BER                                  18
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_RSUM                                 17
#define RSSPECAN_VAL_VSA_RESULT_FORMAT_MOV                                  19

// rsspecan_rngTDSCtabOrder
#define RSSPECAN_VAL_TDS_CTAB_ORDER_CODE                                    0
#define RSSPECAN_VAL_TDS_CTAB_ORDER_MID                                     1

// rsspecan_rngHcopyDeviceLang
#define RSSPECAN_VAL_HCOPY_DEVICE_LANG_GDI                                  0
#define RSSPECAN_VAL_HCOPY_DEVICE_LANG_WMF                                  1
#define RSSPECAN_VAL_HCOPY_DEVICE_LANG_EWMF                                 2
#define RSSPECAN_VAL_HCOPY_DEVICE_LANG_BMP                                  3

// rsspecan_rngHcopyDeviceOrient
#define RSSPECAN_VAL_HCOPY_DEVICE_ORIENT_LAND                               0
#define RSSPECAN_VAL_HCOPY_DEVICE_ORIENT_PORT                               1

// rsspecan_rngIQFilt
#define RSSPECAN_VAL_IQ_FILT_NORM                                           0
#define RSSPECAN_VAL_IQ_FILT_WIDE                                           1

// rsspecan_rngWlanDemod
#define RSSPECAN_VAL_WLAN_BPSK                                              0
#define RSSPECAN_VAL_WLAN_QPSK                                              1
#define RSSPECAN_VAL_WLAN_QAM16                                             2
#define RSSPECAN_VAL_WLAN_QAM64                                             3
#define RSSPECAN_VAL_WLAN_BPSK6                                             4
#define RSSPECAN_VAL_WLAN_BPSK9                                             5
#define RSSPECAN_VAL_WLAN_QPSK12                                            6
#define RSSPECAN_VAL_WLAN_QPSK18                                            7
#define RSSPECAN_VAL_WLAN_QAM1624                                           8
#define RSSPECAN_VAL_WLAN_QAM1636                                           9
#define RSSPECAN_VAL_WLAN_QAM6448                                           10
#define RSSPECAN_VAL_WLAN_QAM6454                                           11
#define RSSPECAN_VAL_WLAN_DBPSK                                             12
#define RSSPECAN_VAL_WLAN_DQPSK                                             13
#define RSSPECAN_VAL_WLAN_CCK55                                             14
#define RSSPECAN_VAL_WLAN_CCK11                                             15
#define RSSPECAN_VAL_WLAN_PBCC55                                            16
#define RSSPECAN_VAL_WLAN_PBCC11                                            17
#define RSSPECAN_VAL_WLAN_PBCC22                                            18
#define RSSPECAN_VAL_WLAN_BPSK3                                             19
#define RSSPECAN_VAL_WLAN_BPSK45                                            20
#define RSSPECAN_VAL_WLAN_QPSK6                                             21
#define RSSPECAN_VAL_WLAN_QPSK9                                             22
#define RSSPECAN_VAL_WLAN_QAM1612                                           23
#define RSSPECAN_VAL_WLAN_QAM1618                                           24
#define RSSPECAN_VAL_WLAN_QAM6424                                           25
#define RSSPECAN_VAL_WLAN_QAM6427                                           26

// rsspecan_rngTDBSStandard
#define RSSPECAN_VAL_TDBS_SETTINGS_STANDARD_GPP                             0
#define RSSPECAN_VAL_TDBS_SETTINGS_STANDARD_TSM                             1

// rsspecan_rngDISPFormat
#define RSSPECAN_VAL_DISP_FORMAT_SINGLE                                     0
#define RSSPECAN_VAL_DISP_FORMAT_SPLIT                                      1

// rsspecan_rngWlanBurstType
#define RSSPECAN_VAL_WLAN_BTYP_DIRECT                                       0
#define RSSPECAN_VAL_WLAN_BTYP_LOFDM                                        1
#define RSSPECAN_VAL_WLAN_BTYP_SOFDM                                        2
#define RSSPECAN_VAL_WLAN_BTYP_LONG                                         3
#define RSSPECAN_VAL_WLAN_BTYP_SHORT                                        4
#define RSSPECAN_VAL_WLAN_BTYP_MM20                                         5
#define RSSPECAN_VAL_WLAN_BTYP_GFM20                                        6
#define RSSPECAN_VAL_WLAN_BTYP_MM40                                         7
#define RSSPECAN_VAL_WLAN_BTYP_GFM40                                        8

// rsspecan_rngTDSSideBand
#define RSSPECAN_VAL_TDS_CDP_SBAND_NORM                                     0
#define RSSPECAN_VAL_TDS_CDP_SBAND_INV                                      1

// rsspecan_rng3GBSACLRMode
#define RSSPECAN_VAL_3GPP_BS_ACLR_MODE_ABS                                  0
#define RSSPECAN_VAL_3GPP_BS_ACLR_MODE_REL                                  1

// rsspecan_rng3GBSACLRACPRef
#define RSSPECAN_VAL_3GPP_BS_ACLR_CPACP_CONF_ACP_REF_SETTINGS_MIN           0
#define RSSPECAN_VAL_3GPP_BS_ACLR_CPACP_CONF_ACP_REF_SETTINGS_MAX           1
#define RSSPECAN_VAL_3GPP_BS_ACLR_CPACP_CONF_ACP_REF_SETTINGS_LHIG          2

// rsspecan_rngDomain
#define RSSPECAN_VAL_FREQ                                                   0
#define RSSPECAN_VAL_TIME                                                   1

// rsspecan_rngLinLog
#define RSSPECAN_VAL_LIN                                                    0
#define RSSPECAN_VAL_LOG                                                    1
#define RSSPECAN_VAL_MPOW                                                   2

// rsspecan_rng3GBSCompareMode
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_COMP_MODE_PILOT                       0
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_COMP_MODE_SLOT                        1

// rsspecan_rng3GPPSband
#define RSSPECAN_VAL_3GPP_SBAND_NORM                                        0
#define RSSPECAN_VAL_3GPP_SBAND_INV                                         1

// rsspecan_rng3GBSPowRef
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_POW_REF_TOT                           0
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_POW_REF_CPIC                          1

// rsspecan_rng3GBSSyncType
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_SYNC_TYPE_CPIC                        0
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_SYNC_TYPE_SCH                         1

// rsspecan_rng3GBSAntenna
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_ANTENNA_OFF                           0
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_ANTENNA_1                             1
#define RSSPECAN_VAL_3GPP_BS_SETTINGS_ANTENNA_2                             2

// rsspecan_rngPreamble
#define RSSPECAN_VAL_PRE_FREQ                                               0
#define RSSPECAN_VAL_PRE_PHAS                                               1

// rsspecan_rngWiMAXFreqBand
#define RSSPECAN_VAL_WIMAX_FBAN_UNSP                                        0
#define RSSPECAN_VAL_WIMAX_FBAN_ETSI                                        1
#define RSSPECAN_VAL_WIMAX_FBAN_MMDS                                        2
#define RSSPECAN_VAL_WIMAX_FBAN_WCS                                         3
#define RSSPECAN_VAL_WIMAX_FBAN_CEPT                                        4
#define RSSPECAN_VAL_WIMAX_FBAN_UNI                                         5
#define RSSPECAN_VAL_WIMAX_FBAN_ETSI2                                       6
#define RSSPECAN_VAL_WIMAX_FBAN_MMDS2                                       7
#define RSSPECAN_VAL_WIMAX_FBAN_WCS2                                        8
#define RSSPECAN_VAL_WIMAX_FBAN_CEPT2                                       9
#define RSSPECAN_VAL_WIMAX_FBAN_UNI2                                        10

// rsspecan_rngWiMAXLinkMode
#define RSSPECAN_VAL_WIMAX_LMOD_DL                                          0
#define RSSPECAN_VAL_WIMAX_LMOD_UL                                          1
#define RSSPECAN_VAL_WIMAX_LMOD_ANY                                         2

// rsspecan_rngServiceInput
#define RSSPECAN_VAL_INPUT_RF                                               0
#define RSSPECAN_VAL_INPUT_CAL                                              1
#define RSSPECAN_VAL_INPUT_TG                                               2
#define RSSPECAN_VAL_INPUT_AIQ                                              3
#define RSSPECAN_VAL_INPUT_WB_CAL                                           4

// rsspecan_rngAvgType
#define RSSPECAN_VAL_AVG_LIN                                                0
#define RSSPECAN_VAL_AVG_VID                                                1
#define RSSPECAN_VAL_AVG_POW                                                2

// rsspecan_rngDISPSizeWindow
#define RSSPECAN_VAL_DISP_SIZE_LARGE                                        0
#define RSSPECAN_VAL_DISP_SIZE_SMALL                                        1

// rsspecan_rngBBInputImpedance
#define RSSPECAN_VAL_BB_INPUT_IMP_LOW                                       0
#define RSSPECAN_VAL_BB_INPUT_IMP_HIGH                                      1

// rsspecan_rngBBInput
#define RSSPECAN_VAL_BB_INPUT_RF                                            0
#define RSSPECAN_VAL_BB_INPUT_AIQ                                           1
#define RSSPECAN_VAL_BB_INPUT_DIQ                                           2
#define RSSPECAN_VAL_BB_INPUT_FILE                                          3

// rsspecan_rngFFTFormat
#define RSSPECAN_VAL_FFT_MAGN                                               0
#define RSSPECAN_VAL_FFT_PHAS                                               1
#define RSSPECAN_VAL_FFT_UPH                                                2
#define RSSPECAN_VAL_FFT_RIM                                                3
#define RSSPECAN_VAL_FFT_MPH                                                4
#define RSSPECAN_VAL_FFT_VOLT                                               5

// rsspecan_rngFFTCalSignal
#define RSSPECAN_VAL_FFT_CAL_IHIG                                           0
#define RSSPECAN_VAL_FFT_CAL_ILOW                                           1
#define RSSPECAN_VAL_FFT_CAL_QHIG                                           2
#define RSSPECAN_VAL_FFT_CAL_QLOW                                           3

// rsspecan_rngFFTCalSigSour
#define RSSPECAN_VAL_FFT_SIG_SOUR_IQ                                        0
#define RSSPECAN_VAL_FFT_SIG_SOUR_GND                                       1
#define RSSPECAN_VAL_FFT_SIG_SOUR_CALD                                      2
#define RSSPECAN_VAL_FFT_SIG_SOUR_CALP                                      3

// rsspecan_rngBBSignalPath
#define RSSPECAN_VAL_BB_SIG_PATH_I                                          0
#define RSSPECAN_VAL_BB_SIG_PATH_Q                                          1
#define RSSPECAN_VAL_BB_SIG_PATH_IQ                                         2

// rsspecan_rngPhaseSmoothingType
#define RSSPECAN_VAL_PHASE_SMO_TYPE_LIN                                     0
#define RSSPECAN_VAL_PHASE_SMO_TYPE_LOG                                     1
#define RSSPECAN_VAL_PHASE_SMO_TYPE_MED                                     2

// rsspecan_rngWiMAXDemod
#define RSSPECAN_VAL_WIMAX_BPSK1_2                                          0
#define RSSPECAN_VAL_WIMAX_QPSK1_2                                          1
#define RSSPECAN_VAL_WIMAX_QPSK3_4                                          2
#define RSSPECAN_VAL_WIMAX_16QAM1_2                                         3
#define RSSPECAN_VAL_WIMAX_16QAM3_4                                         4
#define RSSPECAN_VAL_WIMAX_64QAM2_3                                         5
#define RSSPECAN_VAL_WIMAX_64QAM3_4                                         6

// rsspecan_rngWiMAXBurstType
#define RSSPECAN_VAL_WIMAX_OFDM                                             0
#define RSSPECAN_VAL_WIMAX_OFDMA                                            1

// rsspecan_rngMarkerPeakListSort
#define RSSPECAN_VAL_MARKER_SORT_X                                          0
#define RSSPECAN_VAL_MARKER_SORT_Y                                          1

// rsspecan_rngMarkerDemodType
#define RSSPECAN_VAL_MARKER_DEMOD_AM                                        0
#define RSSPECAN_VAL_MARKER_DEMOD_FM                                        1

// rsspecan_rngC2KFeed
#define RSSPECAN_VAL_C2K_FEED_CDP                                           0
#define RSSPECAN_VAL_C2K_FEED_CDP_RAT                                       1
#define RSSPECAN_VAL_C2K_FEED_CDEP                                          2
#define RSSPECAN_VAL_C2K_FEED_ERR_CTAB                                      3
#define RSSPECAN_VAL_C2K_FEED_MACC                                          4
#define RSSPECAN_VAL_C2K_FEED_PVSL                                          5
#define RSSPECAN_VAL_C2K_FEED_PVSY                                          6
#define RSSPECAN_VAL_C2K_FEED_BSTR                                          7
#define RSSPECAN_VAL_C2K_FEED_ERR_SUMM                                      8
#define RSSPECAN_VAL_C2K_FEED_ERR_PCD                                       9
#define RSSPECAN_VAL_C2K_FEED_SYMB_CONS                                     10
#define RSSPECAN_VAL_C2K_FEED_SYMB_EVM                                      11
#define RSSPECAN_VAL_C2K_FEED_COMP_CONS                                     12
#define RSSPECAN_VAL_C2K_FEED_PVCH                                          13
#define RSSPECAN_VAL_C2K_FEED_CPOW                                          14
#define RSSPECAN_VAL_C2K_FEED_CHAN_BSTR                                     15
#define RSSPECAN_VAL_C2K_FEED_CHAN_CONS                                     16
#define RSSPECAN_VAL_C2K_FEED_CHAN_SEVM                                     17

// rsspecan_rngC2KMeasSelect
#define RSSPECAN_VAL_C2K_MEAS_SEL_POWER                                     0
#define RSSPECAN_VAL_C2K_MEAS_SEL_ACLR                                      1
#define RSSPECAN_VAL_C2K_MEAS_SEL_MC_ACLR                                   2
#define RSSPECAN_VAL_C2K_MEAS_SEL_ESP                                       3
#define RSSPECAN_VAL_C2K_MEAS_SEL_OBW                                       4
#define RSSPECAN_VAL_C2K_MEAS_SEL_CDP                                       5
#define RSSPECAN_VAL_C2K_MEAS_SEL_CCDF                                      6
#define RSSPECAN_VAL_C2K_MEAS_SEL_PVTIME                                    7

// rsspecan_rngMeasPowerSelect
#define RSSPECAN_VAL_MEAS_POW_ACP                                           0
#define RSSPECAN_VAL_MEAS_POW_CPOW                                          1
#define RSSPECAN_VAL_MEAS_POW_MCAC                                          2
#define RSSPECAN_VAL_MEAS_POW_OBAN                                          3
#define RSSPECAN_VAL_MEAS_POW_CN                                            4
#define RSSPECAN_VAL_MEAS_POW_CN0                                           5
#define RSSPECAN_VAL_MEAS_POW_AOB                                           6
#define RSSPECAN_VAL_MEAS_POW_TDMA                                          7

// rsspecan_rngC2KSpreadingFactor
#define RSSPECAN_VAL_C2K_SFACTOR_64                                         0
#define RSSPECAN_VAL_C2K_SFACTOR_128                                        1

// rsspecan_rngC2KSideBand
#define RSSPECAN_VAL_C2K_SBAND_NORMAL                                       0
#define RSSPECAN_VAL_C2K_SBAND_INVERS                                       1

// rsspecan_rngMeasPowerStandard
#define RSSPECAN_VAL_MEAS_POW_STD_NONE                                      0
#define RSSPECAN_VAL_MEAS_POW_STD_NADC                                      1
#define RSSPECAN_VAL_MEAS_POW_STD_TETRA                                     2
#define RSSPECAN_VAL_MEAS_POW_STD_PDC                                       3
#define RSSPECAN_VAL_MEAS_POW_STD_PHS                                       4
#define RSSPECAN_VAL_MEAS_POW_STD_CDPD                                      5
#define RSSPECAN_VAL_MEAS_POW_STD_FIS95A                                    6
#define RSSPECAN_VAL_MEAS_POW_STD_RIS95A                                    7
#define RSSPECAN_VAL_MEAS_POW_STD_FIS95C0                                   8
#define RSSPECAN_VAL_MEAS_POW_STD_RIS95C0                                   9
#define RSSPECAN_VAL_MEAS_POW_STD_FJ008                                     10
#define RSSPECAN_VAL_MEAS_POW_STD_RJ008                                     11
#define RSSPECAN_VAL_MEAS_POW_STD_FIS95C1                                   12
#define RSSPECAN_VAL_MEAS_POW_STD_RIS95C1                                   13
#define RSSPECAN_VAL_MEAS_POW_STD_FWCD                                      14
#define RSSPECAN_VAL_MEAS_POW_STD_RWCD                                      15
#define RSSPECAN_VAL_MEAS_POW_STD_FW3G                                      16
#define RSSPECAN_VAL_MEAS_POW_STD_RW3G                                      17
#define RSSPECAN_VAL_MEAS_POW_STD_D2CD                                      18
#define RSSPECAN_VAL_MEAS_POW_STD_S2CD                                      19
#define RSSPECAN_VAL_MEAS_POW_STD_M2CD                                      20
#define RSSPECAN_VAL_MEAS_POW_STD_TCDM                                      21
#define RSSPECAN_VAL_MEAS_POW_STD_AWL                                       22
#define RSSPECAN_VAL_MEAS_POW_STD_BWL                                       23
#define RSSPECAN_VAL_MEAS_POW_STD_WIM                                       24
#define RSSPECAN_VAL_MEAS_POW_STD_WIBRO                                     25
#define RSSPECAN_VAL_MEAS_POW_STD_RTCDMA                                    26
#define RSSPECAN_VAL_MEAS_POW_STD_RFID                                      27
#define RSSPECAN_VAL_MEAS_POW_STD_GSM                                       28
#define RSSPECAN_VAL_MEAS_POW_STD_EURT                                      29
#define RSSPECAN_VAL_MEAS_POW_STD_REUT                                      30
#define RSSPECAN_VAL_MEAS_POW_STD_PAPC                                      31
#define RSSPECAN_VAL_MEAS_POW_STD_MSR                                       32

// rsspecan_rngC2KPowerReference
#define RSSPECAN_VAL_C2K_PREF_TOTAL                                         0
#define RSSPECAN_VAL_C2K_PREF_PICH                                          1

// rsspecan_rngMeasPowerMode
#define RSSPECAN_VAL_MEAS_POW_MODE_WRITE                                    0
#define RSSPECAN_VAL_MEAS_POW_MODE_MAXH                                     1

// rsspecan_rngC2KAntenna
#define RSSPECAN_VAL_C2K_ANTENNA_OFF                                        0
#define RSSPECAN_VAL_C2K_ANTENNA_1                                          1
#define RSSPECAN_VAL_C2K_ANTENNA_2                                          2

// rsspecan_rngC2KOrder
#define RSSPECAN_VAL_C2K_ORDER_HADAMARD                                     0
#define RSSPECAN_VAL_C2K_ORDER_BITREVERSE                                   1

// rsspecan_rngMeasStatUnit
#define RSSPECAN_VAL_PCT                                                    1

// rsspecan_rngBDOLimPVTReference
#define RSSPECAN_VAL_C2K_LIM_PVT_REF_AUTO                                   0
#define RSSPECAN_VAL_C2K_LIM_PVT_REF_MAN                                    1
#define RSSPECAN_VAL_C2K_LIM_PVT_REF_ONCE                                   2

// rsspecan_rngBDOCDPowerRFSlot
#define RSSPECAN_VAL_C2K_CDP_RFSLOT_FULL                                    0
#define RSSPECAN_VAL_C2K_CDP_RFSLOT_IDLE                                    1

// rsspecan_rngBDOCDPowerMMode
#define RSSPECAN_VAL_C2K_CDP_MMODE_AUTO                                     0
#define RSSPECAN_VAL_C2K_CDP_MMODE_IOQ                                      1
#define RSSPECAN_VAL_C2K_CDP_MMODE_COMPLEX                                  2

// rsspecan_rngBDOCDPowerMapping
#define RSSPECAN_VAL_C2K_CDP_MAPPING_I                                      0
#define RSSPECAN_VAL_C2K_CDP_MAPPING_Q                                      1

// rsspecan_rngBDOCDPowerCType
#define RSSPECAN_VAL_C2K_CDP_CTYPE_PILOT                                    0
#define RSSPECAN_VAL_C2K_CDP_CTYPE_MAC                                      1
#define RSSPECAN_VAL_C2K_CDP_CTYPE_PREAMBLE                                 2
#define RSSPECAN_VAL_C2K_CDP_CTYPE_DATA                                     3

// rsspecan_rngMC2KLimMode
#define RSSPECAN_VAL_MC2K_LIM_MODE_AUTO                                     0
#define RSSPECAN_VAL_MC2K_LIM_MODE_USER                                     1

// rsspecan_rngMC2KCDPowerMapping
#define RSSPECAN_VAL_MC2K_CDP_MAPPING_I                                     0
#define RSSPECAN_VAL_MC2K_CDP_MAPPING_Q                                     1

// rsspecan_rngMC2KSpreadingFactor
#define RSSPECAN_VAL_MC2K_SFACTOR_16                                        0
#define RSSPECAN_VAL_MC2K_SFACTOR_32                                        1
#define RSSPECAN_VAL_MC2K_SFACTOR_64                                        2

// rsspecan_rngMDOCDPowerOperation
#define RSSPECAN_VAL_MDO_CDP_OPER_ACCESS                                    0
#define RSSPECAN_VAL_MDO_CDP_OPER_TRAFFIC                                   1

// rsspecan_rngC2KAdjustSettings
#define RSSPECAN_VAL_C2K_ACH_PRES_ACP                                       0
#define RSSPECAN_VAL_C2K_ACH_PRES_CPOW                                      1
#define RSSPECAN_VAL_C2K_ACH_PRES_MCAC                                      2
#define RSSPECAN_VAL_C2K_ACH_PRES_OBW                                       3

// rsspecan_rngWiMAXBconAuto
#define RSSPECAN_VAL_WIMAX_BCON_NONE                                        0
#define RSSPECAN_VAL_WIMAX_BCON_FIRST                                       1
#define RSSPECAN_VAL_WIMAX_BCON_USER                                        2
#define RSSPECAN_VAL_WIMAX_BCON_ALL                                         3
#define RSSPECAN_VAL_WIMAX_BCON_SIGNAL                                      4
#define RSSPECAN_VAL_WIMAX_BCON_PREDEF                                      5
#define RSSPECAN_VAL_WIMAX_BCON_ULPHY                                       6
#define RSSPECAN_VAL_WIMAX_BCON_ULMAP                                       7
#define RSSPECAN_VAL_WIMAX_BCON_DLMAP                                       8
#define RSSPECAN_VAL_WIMAX_BCON_ALL64                                       9
#define RSSPECAN_VAL_WIMAX_BCON_ALL256                                      10

// rsspecan_rngServiceCalibrationFeed
#define RSSPECAN_VAL_SERVICE_FEED_IHIG                                      0
#define RSSPECAN_VAL_SERVICE_FEED_ILOW                                      1
#define RSSPECAN_VAL_SERVICE_FEED_QHIG                                      2
#define RSSPECAN_VAL_SERVICE_FEED_QLOW                                      3

// rsspecan_rngServiceBBSignalSource
#define RSSPECAN_VAL_SERVICE_BB_IQ                                          0
#define RSSPECAN_VAL_SERVICE_BB_GND                                         1
#define RSSPECAN_VAL_SERVICE_BB_CALDC                                       2
#define RSSPECAN_VAL_SERVICE_BB_CALPULSE                                    3

// rsspecan_rngVSASyncSearchMode
#define RSSPECAN_VAL_VSA_SYNC_MODE_MEAS                                     0
#define RSSPECAN_VAL_VSA_SYNC_MODE_SYNC                                     1

// rsspecan_rngFFTFilterMode
#define RSSPECAN_VAL_FFT_MODE_AUTO                                          0
#define RSSPECAN_VAL_FFT_MODE_WIDE                                          1
#define RSSPECAN_VAL_FFT_MODE_NARROW                                        2

// rsspecan_rngParameterCoupling
#define RSSPECAN_VAL_COUPLING_NONE                                          0
#define RSSPECAN_VAL_COUPLING_RLEV                                          1
#define RSSPECAN_VAL_COUPLING_CFB                                           2
#define RSSPECAN_VAL_COUPLING_CFA                                           3

// rsspecan_rngMC2KLCodeMode
#define RSSPECAN_VAL_MC2K_LCODE_STANDARD                                    0
#define RSSPECAN_VAL_MC2K_LCODE_ESG101                                      1

// rsspecan_rngBandFiltType
#define RSSPECAN_VAL_FILT_TYPE_NORM                                         0
#define RSSPECAN_VAL_FILT_TYPE_FFT                                          1
#define RSSPECAN_VAL_FILT_TYPE_CFIL                                         2
#define RSSPECAN_VAL_FILT_TYPE_RRC                                          3
#define RSSPECAN_VAL_FILT_TYPE_P5                                           4
#define RSSPECAN_VAL_FILT_TYPE_PULSE                                        5
#define RSSPECAN_VAL_FILT_TYPE_NOISE                                        6
#define RSSPECAN_VAL_FILT_TYPE_P5D                                          7
#define RSSPECAN_VAL_FILT_TYPE_CISP                                         8
#define RSSPECAN_VAL_FILT_TYPE_MIL                                          9

// rsspecan_rngVbwFiltType
#define RSSPECAN_VAL_VBW_FILT_LIN                                           0
#define RSSPECAN_VAL_VBW_FILT_LOG                                           1

// rsspecan_rngBandPLL
#define RSSPECAN_VAL_PLL_AUTO                                               0
#define RSSPECAN_VAL_PLL_LOW                                                1
#define RSSPECAN_VAL_PLL_MED                                                2
#define RSSPECAN_VAL_PLL_HIGH                                               3

// rsspecan_rngCorrMethod
#define RSSPECAN_VAL_CORR_TRAN                                              0
#define RSSPECAN_VAL_CORR_REFL                                              1

// rsspecan_rngCorrAcquire
#define RSSPECAN_VAL_CORR_ACQ_THR                                           0
#define RSSPECAN_VAL_CORR_ACQ_OPEN                                          1

// rsspecan_rngTFacUnit
#define RSSPECAN_VAL_TFAC_UNIT_DB                                           0
#define RSSPECAN_VAL_TFAC_UNIT_DBM                                          1
#define RSSPECAN_VAL_TFAC_UNIT_DBMV                                         2
#define RSSPECAN_VAL_TFAC_UNIT_DBUV                                         3
#define RSSPECAN_VAL_TFAC_UNIT_DBUVM                                        4
#define RSSPECAN_VAL_TFAC_UNIT_DBUA                                         5
#define RSSPECAN_VAL_TFAC_UNIT_DBUAM                                        6
#define RSSPECAN_VAL_TFAC_UNIT_DBPW                                         7
#define RSSPECAN_VAL_TFAC_UNIT_DBPT                                         8

// rsspecan_rngCvlBand
#define RSSPECAN_VAL_CVL_BAND_A                                             0
#define RSSPECAN_VAL_CVL_BAND_Q                                             1
#define RSSPECAN_VAL_CVL_BAND_U                                             2
#define RSSPECAN_VAL_CVL_BAND_V                                             3
#define RSSPECAN_VAL_CVL_BAND_E                                             4
#define RSSPECAN_VAL_CVL_BAND_W                                             5
#define RSSPECAN_VAL_CVL_BAND_F                                             6
#define RSSPECAN_VAL_CVL_BAND_D                                             7
#define RSSPECAN_VAL_CVL_BAND_G                                             8
#define RSSPECAN_VAL_CVL_BAND_Y                                             9
#define RSSPECAN_VAL_CVL_BAND_J                                             10
#define RSSPECAN_VAL_CVL_BAND_USER                                          11
#define RSSPECAN_VAL_CVL_BAND_K                                             12
#define RSSPECAN_VAL_CVL_BAND_KA                                            13

// rsspecan_rngFreqCentLink
#define RSSPECAN_VAL_CENT_FREQ_LINK_OFF                                     0
#define RSSPECAN_VAL_CENT_FREQ_LINK_SPAN                                    1
#define RSSPECAN_VAL_CENT_FREQ_LINK_RBW                                     2
#define RSSPECAN_VAL_CENT_FREQ_LINK_DIVT                                    3

// rsspecan_rngFreqMode
#define RSSPECAN_VAL_FREQ_MODE_CW                                           0
#define RSSPECAN_VAL_FREQ_MODE_FIX                                          1
#define RSSPECAN_VAL_FREQ_MODE_SWE                                          2
#define RSSPECAN_VAL_FREQ_MODE_SCAN                                         3

// rsspecan_rngListFiltType
#define RSSPECAN_VAL_LIST_FTYPE_NORM                                        0
#define RSSPECAN_VAL_LIST_FTYPE_CHAN                                        1
#define RSSPECAN_VAL_LIST_FTYPE_RRC                                         2
#define RSSPECAN_VAL_LIST_FTYPE_P5                                          3
#define RSSPECAN_VAL_LIST_FTYPE_PULS                                        4
#define RSSPECAN_VAL_LIST_FTYPE_NOIS                                        5
#define RSSPECAN_VAL_LIST_FTYPE_P5D                                         6

// rsspecan_rngOffOnAuto
#define RSSPECAN_VAL_STATE_OFF                                              0
#define RSSPECAN_VAL_STATE_ON                                               1
#define RSSPECAN_VAL_STATE_AUTO                                             2
#define RSSPECAN_VAL_STATE_ALL                                              3

// rsspecan_rngExtMixerHarmonicType
#define RSSPECAN_VAL_EXT_MIX_HARM_ODD                                       0
#define RSSPECAN_VAL_EXT_MIX_HARM_EVEN                                      1
#define RSSPECAN_VAL_EXT_MIX_HARM_EODD                                      2

// rsspecan_rngExtMixerHarmBand
#define RSSPECAN_VAL_EXT_MIX_BAND_A                                         0
#define RSSPECAN_VAL_EXT_MIX_BAND_Q                                         1
#define RSSPECAN_VAL_EXT_MIX_BAND_U                                         2
#define RSSPECAN_VAL_EXT_MIX_BAND_V                                         3
#define RSSPECAN_VAL_EXT_MIX_BAND_E                                         4
#define RSSPECAN_VAL_EXT_MIX_BAND_W                                         5
#define RSSPECAN_VAL_EXT_MIX_BAND_F                                         6
#define RSSPECAN_VAL_EXT_MIX_BAND_D                                         7
#define RSSPECAN_VAL_EXT_MIX_BAND_G                                         8
#define RSSPECAN_VAL_EXT_MIX_BAND_Y                                         9
#define RSSPECAN_VAL_EXT_MIX_BAND_J                                         10
#define RSSPECAN_VAL_EXT_MIX_BAND_USER                                      11
#define RSSPECAN_VAL_EXT_MIX_BAND_K                                         12
#define RSSPECAN_VAL_EXT_MIX_BAND_KA                                        13

// rsspecan_rngAdjChAutoSel
#define RSSPECAN_VAL_ADJ_RCHAN_AUTO_MIN                                     0
#define RSSPECAN_VAL_ADJ_RCHAN_AUTO_MAX                                     1
#define RSSPECAN_VAL_ADJ_RCHAN_AUTO_LHIG                                    2

// rsspecan_rngAdjPreset
#define RSSPECAN_VAL_ADJ_PRE_ACP                                            0
#define RSSPECAN_VAL_ADJ_PRE_CPOW                                           1
#define RSSPECAN_VAL_ADJ_PRE_MCAC                                           2
#define RSSPECAN_VAL_ADJ_PRE_OBAN                                           3
#define RSSPECAN_VAL_ADJ_PRE_CN                                             4
#define RSSPECAN_VAL_ADJ_PRE_CN0                                            5

// rsspecan_rngSourceIntExt
#define RSSPECAN_VAL_SOUR_INT                                               0
#define RSSPECAN_VAL_SOUR_EXT                                               1
#define RSSPECAN_VAL_SOUR_EAUT                                              2

// rsspecan_rngFmTriggerSource
#define RSSPECAN_VAL_TRG_IMM                                                0
#define RSSPECAN_VAL_TRG_EXT                                                1
#define RSSPECAN_VAL_TRG_IFP                                                2
#define RSSPECAN_VAL_TRG_FM                                                 3
#define RSSPECAN_VAL_TRG_AM                                                 4
#define RSSPECAN_VAL_TRG_PM                                                 5
#define RSSPECAN_VAL_TRG_AMR                                                6

// rsspecan_rngExtGateTrigType
#define RSSPECAN_VAL_EGAT_TRIG_LEV                                          0
#define RSSPECAN_VAL_EGAT_TRIG_EDGE                                         1

// rsspecan_rngHcopyDevice
#define RSSPECAN_VAL_HCOPY_DEVICE_MEM                                       0
#define RSSPECAN_VAL_HCOPY_DEVICE_PRN                                       1
#define RSSPECAN_VAL_HCOPY_DEVICE_CLP                                       2

// rsspecan_rngPolarity
#define RSSPECAN_VAL_NEG                                                    0
#define RSSPECAN_VAL_POS                                                    1

// rsspecan_rngExtGateSource
#define RSSPECAN_VAL_EGAT_SOUR_EXT                                          0
#define RSSPECAN_VAL_EGAT_SOUR_IFP                                          1
#define RSSPECAN_VAL_EGAT_SOUR_VID                                          2
#define RSSPECAN_VAL_EGAT_SOUR_RFP                                          3
#define RSSPECAN_VAL_EGAT_SOUR_PSE                                          4
#define RSSPECAN_VAL_EGAT_SOUR_EXT2                                         5
#define RSSPECAN_VAL_EGAT_SOUR_EXT3                                         6
#define RSSPECAN_VAL_EGAT_SOUR_IQP                                          7

// rsspecan_rngSweepContMode
#define RSSPECAN_VAL_SWE_MODE_AUTO                                          0
#define RSSPECAN_VAL_SWE_MODE_LIST                                          1
#define RSSPECAN_VAL_SWE_MODE_ESYNC                                         2
#define RSSPECAN_VAL_SWE_MODE_ESP                                           3

// rsspecan_rngTgenPowMode
#define RSSPECAN_VAL_TGEN_POW_FIX                                           0
#define RSSPECAN_VAL_TGEN_POW_SWE                                           1

// rsspecan_rngTraceFormat
#define RSSPECAN_VAL_TRACE_FORM_COMP                                        0
#define RSSPECAN_VAL_TRACE_FORM_IQBL                                        1
#define RSSPECAN_VAL_TRACE_FORM_IQP                                         2

// rsspecan_rngExtGenIntfType
#define RSSPECAN_VAL_EXT_GEN_INTF_GPIB                                      0
#define RSSPECAN_VAL_EXT_GEN_INTF_TTL                                       1

// rsspecan_rngTraceModeWithBlank
#define RSSPECAN_TRAC_MOD_BLANK                                             5

// rsspecan_rngTriggerSource
#define RSSPECAN_VAL_TRG_VID                                                3
#define RSSPECAN_VAL_TRG_TIME                                               4
#define RSSPECAN_VAL_TRG_RFP                                                5
#define RSSPECAN_VAL_TRG_PSEN                                               6
#define RSSPECAN_VAL_TRG_TDTR                                               7
#define RSSPECAN_VAL_TRG_BB_PWR                                             8
#define RSSPECAN_VAL_TRG_EXT2                                               9
#define RSSPECAN_VAL_TRG_EXT3                                               10
#define RSSPECAN_VAL_TRG_IQP                                                11
#define RSSPECAN_VAL_TRG_GP0                                                12
#define RSSPECAN_VAL_TRG_GP1                                                13
#define RSSPECAN_VAL_TRG_GP2                                                14
#define RSSPECAN_VAL_TRG_GP3                                                15
#define RSSPECAN_VAL_TRG_GP4                                                16
#define RSSPECAN_VAL_TRG_GP5                                                17
#define RSSPECAN_VAL_TRG_TUN                                                18

// rsspecan_rngFileDecSeparator
#define RSSPECAN_VAL_DEC_SEP_POIN                                           0
#define RSSPECAN_VAL_DEC_SEP_COMMA                                          1

// rsspecan_rngCalibration
#define RSSPECAN_VAL_CAL_ALL                                                0

// rsspecan_rng3GUEMeas
#define RSSPECAN_VAL_3GPPUE_MEAS_POW                                        0
#define RSSPECAN_VAL_3GPPUE_MEAS_ACLR                                       1
#define RSSPECAN_VAL_3GPPUE_MEAS_ESP                                        2
#define RSSPECAN_VAL_3GPPUE_MEAS_OBW                                        3
#define RSSPECAN_VAL_3GPPUE_MEAS_WCDP                                       4
#define RSSPECAN_VAL_3GPPUE_MEAS_CCDF                                       5

// rsspecan_rng3GBSMeas
#define RSSPECAN_VAL_3GPPBS_MEAS_POW                                        0
#define RSSPECAN_VAL_3GPPBS_MEAS_ACLR                                       1
#define RSSPECAN_VAL_3GPPBS_MEAS_ESP                                        2
#define RSSPECAN_VAL_3GPPBS_MEAS_OBW                                        3
#define RSSPECAN_VAL_3GPPBS_MEAS_WCDP                                       4
#define RSSPECAN_VAL_3GPPBS_MEAS_CCDF                                       5
#define RSSPECAN_VAL_3GPPBS_MEAS_MCAC                                       6
#define RSSPECAN_VAL_3GPPBS_MEAS_RFC                                        7
#define RSSPECAN_VAL_3GPPBS_MEAS_FDOM                                       8
#define RSSPECAN_VAL_3GPPBS_MEAS_TDOM                                       9
#define RSSPECAN_VAL_3GPPBS_MEAS_TAER                                       10

// rsspecan_rng3GBSTOffset
#define RSSPECAN_VAL_3GPP_TOFFSET_PRED                                      0
#define RSSPECAN_VAL_3GPP_TOFFSET_MEAS                                      1

// rsspecan_rng3GBSFeed
#define RSSPECAN_VAL_3GBS_RES_CDP                                           0
#define RSSPECAN_VAL_3GBS_RES_CDPRAT                                        1
#define RSSPECAN_VAL_3GBS_RES_CDEP                                          2
#define RSSPECAN_VAL_3GBS_RES_SUM                                           3
#define RSSPECAN_VAL_3GBS_RES_CTAB                                          4
#define RSSPECAN_VAL_3GBS_RES_PCD                                           5
#define RSSPECAN_VAL_3GBS_RES_MACC                                          6
#define RSSPECAN_VAL_3GBS_RES_PVSLA                                         7
#define RSSPECAN_VAL_3GBS_RES_PVSLR                                         8
#define RSSPECAN_VAL_3GBS_RES_BSTR                                          9
#define RSSPECAN_VAL_3GBS_RES_SYMBCONST                                     10
#define RSSPECAN_VAL_3GBS_RES_SYMBEVM                                       11
#define RSSPECAN_VAL_3GBS_RES_FVSL                                          12
#define RSSPECAN_VAL_3GBS_RES_PVSY                                          13
#define RSSPECAN_VAL_3GBS_RES_EVM_MAGN                                      14
#define RSSPECAN_VAL_3GBS_RES_EVM_PHAS                                      15
#define RSSPECAN_VAL_3GBS_RES_COMP_CONS                                     16
#define RSSPECAN_VAL_3GBS_RES_CHIP_EVM                                      17
#define RSSPECAN_VAL_3GBS_RES_CHIP_MAGN                                     18
#define RSSPECAN_VAL_3GBS_RES_CHIP_PHAS                                     19
#define RSSPECAN_VAL_3GBS_RES_PSVSL                                         20
#define RSSPECAN_VAL_3GBS_RES_CDPOVER                                       21

// rsspecan_rngC2KMCarrierFilterType
#define RSSPECAN_VAL_C2K_MCARRIER_FTYPE_LPASS                               0
#define RSSPECAN_VAL_C2K_MCARRIER_FTYPE_RRC                                 1

// rsspecan_rngVSAResult
#define RSSPECAN_VAL_RESULT_XTIM_DDEM_MEAS                                  0
#define RSSPECAN_VAL_RESULT_XTIM_DDEM_REF                                   1
#define RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_MPH                               2
#define RSSPECAN_VAL_RESULT_XTIM_DDEM_ERR_VECT                              3
#define RSSPECAN_VAL_RESULT_XTIM_DDEM_IMP                                   4
#define RSSPECAN_VAL_RESULT_XFR_DDEM_RAT                                    5
#define RSSPECAN_VAL_RESULT_XFR_DDEM_IRAT                                   6
#define RSSPECAN_VAL_RESULT_XTIM_DDEM_SYMB                                  7
#define RSSPECAN_VAL_RESULT_TCAP                                            8
#define RSSPECAN_VAL_RESULT_MACC                                            9

// rsspecan_rngVSAModulationFormat
#define RSSPECAN_VAL_VSA_MOD_FORMAT_QPSK                                    0
#define RSSPECAN_VAL_VSA_MOD_FORMAT_PSK                                     1
#define RSSPECAN_VAL_VSA_MOD_FORMAT_MSK                                     2
#define RSSPECAN_VAL_VSA_MOD_FORMAT_QAM                                     3
#define RSSPECAN_VAL_VSA_MOD_FORMAT_FSK                                     4
#define RSSPECAN_VAL_VSA_MOD_FORMAT_VSB                                     5
#define RSSPECAN_VAL_VSA_MOD_FORMAT_UQAM                                    6
#define RSSPECAN_VAL_VSA_MOD_FORMAT_ASK                                     7
#define RSSPECAN_VAL_VSA_MOD_FORMAT_APSK                                    8

// rsspecan_rngVSAModulQPSKFormat
#define RSSPECAN_VAL_MOD_QPSK_NORM                                          0
#define RSSPECAN_VAL_MOD_QPSK_DIFF                                          1
#define RSSPECAN_VAL_MOD_QPSK_OFFS                                          2
#define RSSPECAN_VAL_MOD_QPSK_DPI4                                          3
#define RSSPECAN_VAL_MOD_QPSK_NPI4                                          4
#define RSSPECAN_VAL_MOD_QPSK_N3PI4                                         5

// rsspecan_rngVSAModulPSKFormat
#define RSSPECAN_VAL_MOD_PSK_NORM                                           0
#define RSSPECAN_VAL_MOD_PSK_DIFF                                           1
#define RSSPECAN_VAL_MOD_PSK_N3PI8                                          2
#define RSSPECAN_VAL_MOD_PSK_PI8D8PSK                                       3

// rsspecan_rngVSAModulMSKFormat
#define RSSPECAN_VAL_MOD_MSK_NORM                                           0
#define RSSPECAN_VAL_MOD_MSK_DIFF                                           1

// rsspecan_rngVSAModulQAMFormat
#define RSSPECAN_VAL_MOD_QAM_NORM                                           0
#define RSSPECAN_VAL_MOD_QAM_DIFF                                           1
#define RSSPECAN_VAL_MOD_QAM_NPI4                                           2
#define RSSPECAN_VAL_MOD_QAM_MNPI4                                          3

// rsspecan_rngVSAModulationEVMCalc
#define RSSPECAN_VAL_MOD_EVM_SYMBOL                                         0
#define RSSPECAN_VAL_MOD_EVM_SIGNAL                                         1

// rsspecan_rngVSADemodSband
#define RSSPECAN_VAL_VSA_SBAND_NORM                                         0
#define RSSPECAN_VAL_VSA_SBAND_INV                                          1

// rsspecan_rngVSAConfDisplayUnitType
#define RSSPECAN_VAL_VSA_UNIT_TIME_S                                        0
#define RSSPECAN_VAL_VSA_UNIT_TIME_SYM                                      1

// rsspecan_rngVSAConfDisplayFitRefPoint
#define RSSPECAN_VAL_VSA_FIT_TRIGGER                                        0
#define RSSPECAN_VAL_VSA_FIT_BURST                                          1
#define RSSPECAN_VAL_VSA_FIT_PATTERN                                        2

// rsspecan_rngVSAConfDisplayFitAllign
#define RSSPECAN_VAL_FIT_ALIGN_LEFT                                         0
#define RSSPECAN_VAL_FIT_ALIGN_CENTER                                       1
#define RSSPECAN_VAL_FIT_ALIGN_RIGHT                                        2

// rsspecan_rngVSAConfDisplayTraceSymbol
#define RSSPECAN_VAL_VSA_TRACE_OFF                                          0
#define RSSPECAN_VAL_VSA_TRACE_DOTS                                         1
#define RSSPECAN_VAL_VSA_TRACE_BARS                                         2

// rsspecan_rngVSAConfDisplayTraceSpacing
#define RSSPECAN_VAL_TRACE_SPACE_LIN                                        0
#define RSSPECAN_VAL_TRACE_SPACE_LOG                                        1
#define RSSPECAN_VAL_TRACE_SPACE_PCT                                        2
#define RSSPECAN_VAL_TRACE_SPACE_LDB                                        3

// rsspecan_rngTriggerSourceB6
#define RSSPECAN_VAL_TRG_B6_RFP                                             0
#define RSSPECAN_VAL_TRG_B6_TV                                              1

// rsspecan_rngVSAExportDataMode
#define RSSPECAN_VAL_EXP_MODE_RAW                                           0
#define RSSPECAN_VAL_EXP_MODE_TRACE                                         1

// rsspecan_rngVSAFactoryDefaults
#define RSSPECAN_VAL_VSA_FACTORY_GST                                        0
#define RSSPECAN_VAL_VSA_FACTORY_STAN                                       1
#define RSSPECAN_VAL_VSA_FACTORY_MAPP                                       2
#define RSSPECAN_VAL_VSA_FACTORY_PATT                                       3
#define RSSPECAN_VAL_VSA_FACTORY_FILT                                       4
#define RSSPECAN_VAL_VSA_FACTORY_ALL                                        5

// rsspecan_rngBDORevision
#define RSSPECAN_VAL_BDO_REVISION_0                                         0
#define RSSPECAN_VAL_BDO_REVISION_A                                         1
#define RSSPECAN_VAL_BDO_REVISION_B                                         2

// rsspecan_rngWiMAXModulForm
#define RSSPECAN_VAL_WIMAX_MODUL_ALL                                        0
#define RSSPECAN_VAL_WIMAX_MODUL_QPSK                                       1
#define RSSPECAN_VAL_WIMAX_MODUL_QAM16                                      2
#define RSSPECAN_VAL_WIMAX_MODUL_QAM64                                      3
#define RSSPECAN_VAL_WIMAX_MODUL_BPSK                                       4

// rsspecan_rngWiMAXZoneType
#define RSSPECAN_VAL_WIMAX_ZONE_TYPE_DLFUSC                                 0
#define RSSPECAN_VAL_WIMAX_ZONE_TYPE_DLPUSC                                 1
#define RSSPECAN_VAL_WIMAX_ZONE_TYPE_ULPUSC                                 2

// rsspecan_rngWiMAXZoneBurstFormat
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QPSK_12                          0
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QPSK_34                          1
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QAM16_12                         2
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QAM16_34                         3
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QAM64_12                         4
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QAM64_23                         5
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QAM64_34                         6
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_QAM64_56                         7
#define RSSPECAN_VAL_WIMAX_ZONE_BURST_FORM_AUTO                             8

// rsspecan_rngWiMAXZoneBurstType
#define RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_FCH                               0
#define RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_DLMAP                             1
#define RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_ULMAP                             2
#define RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_DATA                              3
#define RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_HARQ                              4
#define RSSPECAN_VAL_WIMAX_ZONE_BURS_TYPE_FASTF                             5

// rsspecan_rngWiMAXCestType
#define RSSPECAN_VAL_WIMAX_CEST_PREAMONLY                                   0
#define RSSPECAN_VAL_WIMAX_CEST_PREAMPAYL                                   1
#define RSSPECAN_VAL_WIMAX_CEST_PAYLONLY                                    2

// rsspecan_rngWiMAXTracPilots
#define RSSPECAN_VAL_WIMAX_TRAC_PIL_PRED                                    0
#define RSSPECAN_VAL_WIMAX_TRAC_PIL_DET                                     1

// rsspecan_rngMarkerStepSize
#define RSSPECAN_VAL_MARK_STEP_SIZE_STAN                                    0
#define RSSPECAN_VAL_MARK_STEP_SIZE_POIN                                    1

// rsspecan_rngMpowFiltType
#define RSSPECAN_VAL_MPOW_FTYPE_NORM                                        0
#define RSSPECAN_VAL_MPOW_FTYPE_CFIL                                        1
#define RSSPECAN_VAL_MPOW_FTYPE_RRC                                         2

// rsspecan_rngTriggerTVVertSig
#define RSSPECAN_VAL_TRIG_TV_VFIEL_ALL                                      0
#define RSSPECAN_VAL_TRIG_TV_VFIEL_ODD                                      1
#define RSSPECAN_VAL_TRIG_TV_VFIEL_EVEN                                     2

// rsspecan_rngAdemPhaseWrap
#define RSSPECAN_VAL_ADEM_UPHAS                                             0
#define RSSPECAN_VAL_ADEM_PHAS                                              1

// rsspecan_rngUnitAngle
#define RSSPECAN_VAL_UNIT_ANGLE_DEG                                         0
#define RSSPECAN_VAL_UNIT_ANGLE_RAD                                         1

// rsspecan_rngSpemColor
#define RSSPECAN_VAL_SPEM_COL_COLOR                                         0
#define RSSPECAN_VAL_SPEM_COL_RADAR                                         1
#define RSSPECAN_VAL_SPEM_COL_GRAY                                          2

// rsspecan_rngSpemSearchArea
#define RSSPECAN_VAL_SPEM_SAR_MEMORY                                        1
#define RSSPECAN_VAL_SPEM_SAR_VISIBLE                                       0

// rsspecan_rngUnitSecMin
#define RSSPECAN_VAL_UNIT_SEC                                               0
#define RSSPECAN_VAL_UNIT_MIN                                               1

// rsspecan_rngCatvAtvCarrMeas
#define RSSPECAN_VAL_ATV_MEAS_CARR_CARR                                     0
#define RSSPECAN_VAL_ATV_MEAS_CARR_NOIS                                     1

// rsspecan_rngCatvAtvMeas
#define RSSPECAN_VAL_ATV_MEAS_ASP                                           0
#define RSSPECAN_VAL_ATV_MEAS_CARR                                          1
#define RSSPECAN_VAL_ATV_MEAS_CN                                            2
#define RSSPECAN_VAL_ATV_MEAS_CSO                                           3
#define RSSPECAN_VAL_ATV_MEAS_CTB                                           4
#define RSSPECAN_VAL_ATV_MEAS_HUM                                           5
#define RSSPECAN_VAL_ATV_MEAS_VMOD                                          6
#define RSSPECAN_VAL_ATV_MEAS_VSC                                           7

// rsspecan_rngCatvDtvMeas
#define RSSPECAN_VAL_DTV_MEAS_DSP                                           0
#define RSSPECAN_VAL_DTV_MEAS_CHP                                           1
#define RSSPECAN_VAL_DTV_MEAS_CONS                                          2
#define RSSPECAN_VAL_DTV_MEAS_MERR                                          3
#define RSSPECAN_VAL_DTV_MEAS_EPATT                                         4
#define RSSPECAN_VAL_DTV_MEAS_APD                                           5
#define RSSPECAN_VAL_DTV_MEAS_CCDF                                          6
#define RSSPECAN_VAL_DTV_MEAS_OVER                                          7

// rsspecan_rngCatvTvMeas
#define RSSPECAN_VAL_TV_MEAS_TILT                                           0

// rsspecan_rngCatvMerrZoom
#define RSSPECAN_VAL_CATV_MERR_ZOOM_NONE                                    0
#define RSSPECAN_VAL_CATV_MERR_ZOOM_MERR                                    1
#define RSSPECAN_VAL_CATV_MERR_ZOOM_MERP                                    2
#define RSSPECAN_VAL_CATV_MERR_ZOOM_EVMR                                    3
#define RSSPECAN_VAL_CATV_MERR_ZOOM_EVMP                                    4
#define RSSPECAN_VAL_CATV_MERR_ZOOM_IMB                                     5
#define RSSPECAN_VAL_CATV_MERR_ZOOM_QERR                                    6
#define RSSPECAN_VAL_CATV_MERR_ZOOM_SUP                                     7
#define RSSPECAN_VAL_CATV_MERR_ZOOM_PJIT                                    8

// rsspecan_rngCatvOverZoom
#define RSSPECAN_VAL_CATV_OVER_ZOOM_NONE                                    0
#define RSSPECAN_VAL_CATV_OVER_ZOOM_MERR                                    1
#define RSSPECAN_VAL_CATV_OVER_ZOOM_MERP                                    2
#define RSSPECAN_VAL_CATV_OVER_ZOOM_EVMR                                    3
#define RSSPECAN_VAL_CATV_OVER_ZOOM_EVMP                                    4
#define RSSPECAN_VAL_CATV_OVER_ZOOM_CFOF                                    5
#define RSSPECAN_VAL_CATV_OVER_ZOOM_SROF                                    6

// rsspecan_rngCatvConstDiagZoom
#define RSSPECAN_VAL_CATV_QUAD_ZOOM_NONE                                    0
#define RSSPECAN_VAL_CATV_QUAD_ZOOM_1                                       1
#define RSSPECAN_VAL_CATV_QUAD_ZOOM_2                                       2
#define RSSPECAN_VAL_CATV_QUAD_ZOOM_3                                       3
#define RSSPECAN_VAL_CATV_QUAD_ZOOM_4                                       4

// rsspecan_rngCatvAtvCarrMeasMode
#define RSSPECAN_VAL_ATV_MEAS_MODE_INSERVICE                                0
#define RSSPECAN_VAL_ATV_MEAS_MODE_OFFSERVICE                               1
#define RSSPECAN_VAL_ATV_MEAS_MODE_QUIETLINE                                2

// rsspecan_rngCatvAtvRefPowerMode
#define RSSPECAN_VAL_ATV_RPOW_MODE_RCH                                      0
#define RSSPECAN_VAL_ATV_RPOW_MODE_MCH                                      1
#define RSSPECAN_VAL_ATV_RPOW_MODE_MAN                                      2

// rsspecan_rngCatvDtvFilterAlpha
#define RSSPECAN_VAL_CATV_DTV_ALPHA_R012                                    0
#define RSSPECAN_VAL_CATV_DTV_ALPHA_R013                                    1
#define RSSPECAN_VAL_CATV_DTV_ALPHA_R015                                    2
#define RSSPECAN_VAL_CATV_DTV_ALPHA_R018                                    3

// rsspecan_rngCatvSBand
#define RSSPECAN_VAL_CATV_SBAND_NORM                                        0
#define RSSPECAN_VAL_CATV_SBAND_INV                                         1
#define RSSPECAN_VAL_CATV_SBAND_AUTO                                        2

// rsspecan_rngCatvModStandGroupDelay
#define RSSPECAN_VAL_CATV_GDELAY_GENERAL                                    0
#define RSSPECAN_VAL_CATV_GDELAY_GHALF                                      1
#define RSSPECAN_VAL_CATV_GDELAY_AUSTRALIA                                  2
#define RSSPECAN_VAL_CATV_GDELAY_DANMARK                                    3
#define RSSPECAN_VAL_CATV_GDELAY_NEWZELAND                                  4
#define RSSPECAN_VAL_CATV_GDELAY_NORWAY                                     5
#define RSSPECAN_VAL_CATV_GDELAY_SWEDENFULL                                 6
#define RSSPECAN_VAL_CATV_GDELAY_FLAT                                       7
#define RSSPECAN_VAL_CATV_GDELAY_OIRT                                       8
#define RSSPECAN_VAL_CATV_GDELAY_CHINA                                      9
#define RSSPECAN_VAL_CATV_GDELAY_CCIR                                       10
#define RSSPECAN_VAL_CATV_GDELAY_TDF                                        11
#define RSSPECAN_VAL_CATV_GDELAY_FFC                                        12

// rsspecan_rngCatvModStandSignalType
#define RSSPECAN_VAL_CATV_SIG_TYPE_ATV                                      0
#define RSSPECAN_VAL_CATV_SIG_TYPE_DTV                                      1

// rsspecan_rngCatvAtvStandard
#define RSSPECAN_VAL_ATV_STAN_BG                                            0
#define RSSPECAN_VAL_ATV_STAN_DK                                            1
#define RSSPECAN_VAL_ATV_STAN_I                                             2
#define RSSPECAN_VAL_ATV_STAN_K1                                            3
#define RSSPECAN_VAL_ATV_STAN_L                                             4
#define RSSPECAN_VAL_ATV_STAN_M                                             5
#define RSSPECAN_VAL_ATV_STAN_N                                             6

// rsspecan_rngCatvDtvStandard
#define RSSPECAN_VAL_DTV_STAN_J83A                                          0
#define RSSPECAN_VAL_DTV_STAN_J83B                                          1
#define RSSPECAN_VAL_DTV_STAN_J83C                                          2

// rsspecan_rngCatvAtvSoundSystem
#define RSSPECAN_VAL_ATV_AUDIO_FM55NICAM585                                 0
#define RSSPECAN_VAL_ATV_AUDIO_FM55FM5742                                   1
#define RSSPECAN_VAL_ATV_AUDIO_FM55MONO                                     2
#define RSSPECAN_VAL_ATV_AUDIO_FM65NICAM585                                 3
#define RSSPECAN_VAL_ATV_AUDIO_FM65FM6258                                   4
#define RSSPECAN_VAL_ATV_AUDIO_FM65FM6742                                   5
#define RSSPECAN_VAL_ATV_AUDIO_FM65MONO                                     6
#define RSSPECAN_VAL_ATV_AUDIO_FM60NICAM6552                                7
#define RSSPECAN_VAL_ATV_AUDIO_FM60MONO                                     8
#define RSSPECAN_VAL_ATV_AUDIO_AM65NICAM585                                 9
#define RSSPECAN_VAL_ATV_AUDIO_AM60MONO                                     10
#define RSSPECAN_VAL_ATV_AUDIO_FM45BTSC                                     11
#define RSSPECAN_VAL_ATV_AUDIO_FM45EIA_J                                    12
#define RSSPECAN_VAL_ATV_AUDIO_FM45FM4742                                   13
#define RSSPECAN_VAL_ATV_AUDIO_FM45MONO                                     14

// rsspecan_rngCatvAtvColorSystem
#define RSSPECAN_VAL_ATV_COLOR_SYSTEM_PAL                                   0
#define RSSPECAN_VAL_ATV_COLOR_SYSTEM_NTSC                                  1
#define RSSPECAN_VAL_ATV_COLOR_SYSTEM_SECAM                                 2

// rsspecan_rngCatvDtvFormat
#define RSSPECAN_VAL_DTV_FORMAT_QAM4                                        0
#define RSSPECAN_VAL_DTV_FORMAT_QAM16                                       1
#define RSSPECAN_VAL_DTV_FORMAT_QAM32                                       2
#define RSSPECAN_VAL_DTV_FORMAT_QAM64                                       3
#define RSSPECAN_VAL_DTV_FORMAT_QAM128                                      4
#define RSSPECAN_VAL_DTV_FORMAT_QAM256                                      5
#define RSSPECAN_VAL_DTV_FORMAT_QAM512                                      6
#define RSSPECAN_VAL_DTV_FORMAT_QAM1024                                     7

// rsspecan_rngCatvAtvTriggBarLineType
#define RSSPECAN_VAL_ATV_BLIN_TYPE_CCIR17                                   0
#define RSSPECAN_VAL_ATV_BLIN_TYPE_FCC                                      1
#define RSSPECAN_VAL_ATV_BLIN_TYPE_NTC7                                     2

// rsspecan_rngServiceCombFrequency
#define RSSPECAN_VAL_SERV_COMB_FREQ_COMB1                                   0
#define RSSPECAN_VAL_SERV_COMB_FREQ_COMB64                                  1
#define RSSPECAN_VAL_SERV_COMB_FREQ_COMB65                                  2

// rsspecan_rngNoiseXAxisFrequency
#define RSSPECAN_VAL_NOISE_XAXIS_IF                                         0
#define RSSPECAN_VAL_NOISE_XAXIS_RF                                         1

// rsspecan_rng3GPPResultSumView
#define RSSPECAN_VAL_3GPP_RES_SUM_NORMAL                                    0
#define RSSPECAN_VAL_3GPP_RES_SUM_EXTENDED                                  1

// rsspecan_rngWiMAXDLFramePreMode
#define RSSPECAN_VAL_WIMAX_PRE_AUTO                                         0
#define RSSPECAN_VAL_WIMAX_PRE_USER                                         1

// rsspecan_rngSpemAnalysis
#define RSSPECAN_VAL_SPEM_ANALYSIS_DL                                       0
#define RSSPECAN_VAL_SPEM_ANALYSIS_UL                                       1

// rsspecan_rngBBInputTrigger
#define RSSPECAN_VAL_BB_TRIG_IONLY                                          0
#define RSSPECAN_VAL_BB_TRIG_QONLY                                          1

// rsspecan_rngGsmExtSlotStandard
#define RSSPECAN_VAL_GSM_EXTSLOT_DYNAMIC                                    0
#define RSSPECAN_VAL_GSM_EXTSLOT_GSM900                                     1
#define RSSPECAN_VAL_GSM_EXTSLOT_GSM1800                                    2
#define RSSPECAN_VAL_GSM_EXTSLOT_GSM1900                                    3
#define RSSPECAN_VAL_GSM_EXTSLOT_GSM850                                     4

// rsspecan_rngGsmExtSlotModulation
#define RSSPECAN_VAL_EXT_SLOT_MODUL_OFF                                     0
#define RSSPECAN_VAL_EXT_SLOT_MODUL_GMSK                                    1
#define RSSPECAN_VAL_EXT_SLOT_MODUL_EDGE                                    2

// rsspecan_rng3GPPEVMMeasInterval
#define RSSPECAN_VAL_3GUE_CDP_EVM_INT_SLOT                                  0
#define RSSPECAN_VAL_3GUE_CDP_EVM_INT_MEAS                                  1
#define RSSPECAN_VAL_3GUE_CDP_EVM_INT_FHAL                                  2
#define RSSPECAN_VAL_3GUE_CDP_EVM_INT_SHAL                                  3

// rsspecan_rngVSAExportDataRawMode
#define RSSPECAN_VAL_EXP_RAW_MODE_ASC                                       0
#define RSSPECAN_VAL_EXP_RAW_MODE_WAV                                       1
#define RSSPECAN_VAL_EXP_RAW_MODE_BIN                                       2

// rsspecan_rngTableSEMType
#define RSSPECAN_VAL_SEM_IEEE                                               0
#define RSSPECAN_VAL_SEM_ETSI                                               1
#define RSSPECAN_VAL_SEM_USER                                               2
#define RSSPECAN_VAL_SEM_STANDARD                                           3
#define RSSPECAN_VAL_SEM_ETSI5                                              4
#define RSSPECAN_VAL_SEM_ETSI10                                             5

// rsspecan_rngWiMAXTracCest
#define RSSPECAN_VAL_WIMAX_TRAC_CEST_FULL                                   0
#define RSSPECAN_VAL_WIMAX_TRAC_CEST_USER                                   1

// rsspecan_rngCalProbeAdjustMode
#define RSSPECAN_VAL_PROBE_MODE_DC                                          0
#define RSSPECAN_VAL_PROBE_MODE_PULS                                        1
#define RSSPECAN_VAL_PROBE_MODE_COMP                                        2

// rsspecan_rngDetectorTypeESLRangeTable
#define RSSPECAN_VAL_DETECTOR_TYPE_CRMS                                     7
#define RSSPECAN_VAL_DETECTOR_TYPE_CAV                                      8

// rsspecan_rngSEMFilterType
#define RSSPECAN_VAL_SEM_FILTER_TYPE_NORM                                   0
#define RSSPECAN_VAL_SEM_FILTER_TYPE_CFIL                                   1
#define RSSPECAN_VAL_SEM_FILTER_TYPE_RRC                                    2
#define RSSPECAN_VAL_SEM_FILTER_TYPE_PULS                                   3
#define RSSPECAN_VAL_SEM_FILTER_TYPE_P5                                     4

// rsspecan_rngSEMInsertRange
#define RSSPECAN_VAL_SEM_INSERT_RANGE_AFTER                                 0
#define RSSPECAN_VAL_SEM_INSERT_RANGE_BEFORE                                1

// rsspecan_rngSEMRangeLimitCheckType
#define RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_ABS                                 0
#define RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_REL                                 1
#define RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_AND                                 2
#define RSSPECAN_VAL_SEM_LIM_CHECK_TYPE_OR                                  3

// rsspecan_rngSEMPowerRefType
#define RSSPECAN_VAL_SEM_REF_TYPE_PEAK                                      0
#define RSSPECAN_VAL_SEM_REF_TYPE_CPOW                                      1

// rsspecan_rngIFOutputSource
#define RSSPECAN_VAL_IF_OUTPUT_SOURCE_IF                                    0
#define RSSPECAN_VAL_IF_OUTPUT_SOURCE_VID                                   1
#define RSSPECAN_VAL_IF_OUTPUT_SOURCE_HVID                                  2
#define RSSPECAN_VAL_IF_OUTPUT_SOURCE_IF2                                   3

// rsspecan_rngAvionicsDemodMode
#define RSSPECAN_VAL_AVI_STAN_VOR                                           0
#define RSSPECAN_VAL_AVI_STAN_ILS                                           1

// rsspecan_rngAvionicsFeedType
#define RSSPECAN_VAL_AVI_FEED_SPEC                                          0
#define RSSPECAN_VAL_AVI_FEED_AMS                                           1

// rsspecan_rngAvionicsInput
#define RSSPECAN_VAL_BB_INPUT_AUD                                           3

// rsspecan_rngAvionicsFundFreq
#define RSSPECAN_VAL_AVI_THD_FUND_FREQ_90                                   0
#define RSSPECAN_VAL_AVI_THD_FUND_FREQ_150                                  1
#define RSSPECAN_VAL_AVI_THD_FUND_FREQ_90_150                               2
#define RSSPECAN_VAL_AVI_THD_FUND_FREQ_ID                                   3

// rsspecan_rngIFShift
#define RSSPECAN_VAL_IF_SHIFT_OFF                                           0
#define RSSPECAN_VAL_IF_SHIFT_A                                             1
#define RSSPECAN_VAL_IF_SHIFT_B                                             2
#define RSSPECAN_VAL_IF_SHIFT_AUTO                                          3

// rsspecan_rngTDSCPICHPattern
#define RSSPECAN_VAL_TDS_CPICH_PATT_OFF                                     0
#define RSSPECAN_VAL_TDS_CPICH_PATT_1                                       1
#define RSSPECAN_VAL_TDS_CPICH_PATT_2                                       2

// rsspecan_rngWiMAXNumberOfAntennas
#define RSSPECAN_VAL_WIMAX_DUTC_TX1                                         0
#define RSSPECAN_VAL_WIMAX_DUTC_TX2                                         1

// rsspecan_rngWiMAXSpaceTimeCoding
#define RSSPECAN_VAL_WIMAX_ZONE_STCC_NOST                                   0
#define RSSPECAN_VAL_WIMAX_ZONE_STCC_TX2                                    1
#define RSSPECAN_VAL_WIMAX_ZONE_STCC_TX1                                    2

// rsspecan_rngWiMAXZoneTypeFSV
#define RSSPECAN_VAL_WIMAX_ZONE_TYPE_DLAMC2X                                3
#define RSSPECAN_VAL_WIMAX_ZONE_TYPE_ULAMC2X                                4

// rsspecan_rngTouchscreenDisplay
#define RSSPECAN_VAL_TOUCH_OFF                                              0
#define RSSPECAN_VAL_TOUCH_ON                                               1
#define RSSPECAN_VAL_TOUCH_FRAM                                             2

// rsspecan_rngAutoMan
#define RSSPECAN_VAL_AUTO                                                   0
#define RSSPECAN_VAL_MAN                                                    1

// rsspecan_rngSweepType
#define RSSPECAN_VAL_SWEEP_TYPE_AUTO                                        0
#define RSSPECAN_VAL_SWEEP_TYPE_SWEEP                                       1
#define RSSPECAN_VAL_SWEEP_TYPE_FFT                                         2

// rsspecan_rngAmplAttenMode
#define RSSPECAN_VAL_ATT_MODE_NORM                                          0
#define RSSPECAN_VAL_ATT_MODE_LDIS                                          1

// rsspecan_rngHcopyDeviceLangFSL
#define RSSPECAN_VAL_HCOPY_DEVICE_LANG_JPG                                  4
#define RSSPECAN_VAL_HCOPY_DEVICE_LANG_PNG                                  5

// rsspecan_rngTOIPositioning
#define RSSPECAN_VAL_TOI_POS_CALC                                           0
#define RSSPECAN_VAL_TOI_POS_SEAR                                           1

// rsspecan_rngRoscPLLBwid
#define RSSPECAN_VAL_PLL_BWID_NORM                                          0
#define RSSPECAN_VAL_PLL_BWID_WIDE                                          1

// rsspecan_rngWiMAXTTCStart
#define RSSPECAN_VAL_WIMAX_TTC_FFT                                          0
#define RSSPECAN_VAL_WIMAX_TTC_FRAM                                         1

// rsspecan_rngWiMAXMatrix
#define RSSPECAN_VAL_WIMAX_MATRIX_BURS                                      0
#define RSSPECAN_VAL_WIMAX_MATRIX_A                                         1
#define RSSPECAN_VAL_WIMAX_MATRIX_B                                         2

// rsspecan_rngGSMK10Devices
#define RSSPECAN_VAL_GSM_K10_BTS_NORMAL                                     0
#define RSSPECAN_VAL_GSM_K10_BTS_MICRO                                      1
#define RSSPECAN_VAL_GSM_K10_BTS_PICO                                       2
#define RSSPECAN_VAL_GSM_K10_MS_NORMAL                                      3
#define RSSPECAN_VAL_GSM_K10_MS_SMALL                                       4

// rsspecan_rngGSMK10BandClass
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_PGSM                                0
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_EGSM                                1
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_DCS                                 2
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_PCS                                 3
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_TGSM                                4
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_RGSM                                5
#define RSSPECAN_VAL_GSM_K10_BAND_CLASS_GSM                                 6

// rsspecan_rngGSMK10PowClass
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_1                                    0
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_2                                    1
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_3                                    2
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_4                                    3
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_5                                    4
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_6                                    5
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_7                                    6
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_8                                    7
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_E1                                   8
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_E2                                   9
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_E3                                   10
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_M1                                   11
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_M2                                   12
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_M3                                   13
#define RSSPECAN_VAL_GSM_K10_POW_CLASS_P1                                   14

// rsspecan_rngGSMK10BurstType
#define RSSPECAN_VAL_GSM_K10_BURST_NORMAL                                   0
#define RSSPECAN_VAL_GSM_K10_BURST_HIGHER_RATE                              1
#define RSSPECAN_VAL_GSM_K10_BURST_ACCESS                                   2

// rsspecan_rngGSMK10Mod
#define RSSPECAN_VAL_GSM_K10_MOD_GMSK                                       0
#define RSSPECAN_VAL_GSM_K10_MOD_QPSK                                       1
#define RSSPECAN_VAL_GSM_K10_MOD_8PSK                                       2
#define RSSPECAN_VAL_GSM_K10_MOD_16QAM                                      3
#define RSSPECAN_VAL_GSM_K10_MOD_32QAM                                      4
#define RSSPECAN_VAL_GSM_K10_MOD_AQPSK                                      5

// rsspecan_rngGSMK10Filter
#define RSSPECAN_VAL_GSM_K10_FILTER_GMSK                                    0
#define RSSPECAN_VAL_GSM_K10_FILTER_LIN_GMSK                                1
#define RSSPECAN_VAL_GSM_K10_FILTER_NARROW                                  2
#define RSSPECAN_VAL_GSM_K10_FILTER_WIDE                                    3

// rsspecan_rngGSMK10TSC
#define RSSPECAN_VAL_TSC_SLOT_0                                             0
#define RSSPECAN_VAL_TSC_SLOT_1                                             1
#define RSSPECAN_VAL_TSC_SLOT_2                                             2
#define RSSPECAN_VAL_TSC_SLOT_3                                             3
#define RSSPECAN_VAL_TSC_SLOT_4                                             4
#define RSSPECAN_VAL_TSC_SLOT_5                                             5
#define RSSPECAN_VAL_TSC_SLOT_6                                             6
#define RSSPECAN_VAL_TSC_SLOT_7                                             7
#define RSSPECAN_VAL_TSC_SLOT_USER                                          -1
#define RSSPECAN_VAL_TSC_SLOT_0_1                                           8
#define RSSPECAN_VAL_TSC_SLOT_1_1                                           9
#define RSSPECAN_VAL_TSC_SLOT_2_1                                           10
#define RSSPECAN_VAL_TSC_SLOT_3_1                                           11
#define RSSPECAN_VAL_TSC_SLOT_4_1                                           12
#define RSSPECAN_VAL_TSC_SLOT_5_1                                           13
#define RSSPECAN_VAL_TSC_SLOT_6_1                                           14
#define RSSPECAN_VAL_TSC_SLOT_7_1                                           15
#define RSSPECAN_VAL_TSC_SLOT_0_2                                           16
#define RSSPECAN_VAL_TSC_SLOT_1_2                                           17
#define RSSPECAN_VAL_TSC_SLOT_2_2                                           18
#define RSSPECAN_VAL_TSC_SLOT_3_2                                           19
#define RSSPECAN_VAL_TSC_SLOT_4_2                                           20
#define RSSPECAN_VAL_TSC_SLOT_5_2                                           21
#define RSSPECAN_VAL_TSC_SLOT_6_2                                           22
#define RSSPECAN_VAL_TSC_SLOT_7_2                                           23
#define RSSPECAN_VAL_TSC_SLOT_TS0                                           24
#define RSSPECAN_VAL_TSC_SLOT_TS1                                           25
#define RSSPECAN_VAL_TSC_SLOT_TS2                                           26

// rsspecan_rngGSMK10BurstSel
#define RSSPECAN_VAL_GSM_K10_BURST_SEL_FULL                                 0
#define RSSPECAN_VAL_GSM_K10_BURST_SEL_TH_RES                               1
#define RSSPECAN_VAL_GSM_K10_BURST_SEL_RS                                   2

// rsspecan_rngLTEDuplexing
#define RSSPECAN_VAL_LTE_DUPLEXING_TDD                                      0
#define RSSPECAN_VAL_LTE_DUPLEXING_FDD                                      1

// rsspecan_rngLTELinkDirection
#define RSSPECAN_VAL_LTE_DOWNLINK                                           0
#define RSSPECAN_VAL_LTE_UPLINK                                             1

// rsspecan_rngLTEDownlinkMIMOConfiguration
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_TX1                                  0
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_TX2                                  1
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_TX4                                  2

// rsspecan_rngLTEDownlinkBandwidth
#define RSSPECAN_VAL_LTE_DOWNLINK_BW1_40                                    0
#define RSSPECAN_VAL_LTE_DOWNLINK_BW2_50                                    1
#define RSSPECAN_VAL_LTE_DOWNLINK_BW3_00                                    2
#define RSSPECAN_VAL_LTE_DOWNLINK_BW5_00                                    3
#define RSSPECAN_VAL_LTE_DOWNLINK_BW10_00                                   4
#define RSSPECAN_VAL_LTE_DOWNLINK_BW15_00                                   5
#define RSSPECAN_VAL_LTE_DOWNLINK_BW20_00                                   6

// rsspecan_rngLTEDownlinkCyclicPrefix
#define RSSPECAN_VAL_LTE_DOWNLINK_CPREFIX_NORMAL                            0
#define RSSPECAN_VAL_LTE_DOWNLINK_CPREFIX_EXTENDED                          1
#define RSSPECAN_VAL_LTE_DOWNLINK_CPREFIX_AUTO                              2

// rsspecan_rngLTEDownlinkMIMOAntenna
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_ANT_ALL                              0
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_ANT1                                 1
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_ANT2                                 2
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_ANT3                                 3
#define RSSPECAN_VAL_LTE_DOWNLINK_MIMO_ANT4                                 4

// rsspecan_rngLTEDownlinkChannelEstimation
#define RSSPECAN_VAL_LTE_DOWNLINK_CEST_TGPP                                 0
#define RSSPECAN_VAL_LTE_DOWNLINK_CEST_PILOT                                1
#define RSSPECAN_VAL_LTE_DOWNLINK_CEST_PILPAY                               2

// rsspecan_rngLTEDownlinkEVMCalculation
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_CALC_TGPP                            0
#define RSSPECAN_ATTR_LTE_DOWNLINK_EVM_CALC_OTP                             1

// rsspecan_rngLTEDownlinkPhaseTracking
#define RSSPECAN_VAL_LTE_DOWNLINK_PHASE_TRACKING_OFF                        0
#define RSSPECAN_VAL_LTE_DOWNLINK_PHASE_TRACKING_PILOT                      1
#define RSSPECAN_VAL_LTE_DOWNLINK_PHASE_TRACKING_PILPAY                     2

// rsspecan_rngLTEDownlinkModulation
#define RSSPECAN_VAL_LTE_DOWNLINK_MODULATION_QPSK                           0
#define RSSPECAN_VAL_LTE_DOWNLINK_MODULATION_16QAM                          1
#define RSSPECAN_VAL_LTE_DOWNLINK_MODULATION_64QAM                          2

// rsspecan_rngLTEDownlinkPHICHDuration
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_DURATION_NORMAL                     0
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_DURATION_EXTENDED                   1

// rsspecan_rngLTEDownlinkBitStreamFormat
#define RSSPECAN_VAL_LTE_DOWNLINK_STREAM_SYMBOLS                            0
#define RSSPECAN_VAL_LTE_DOWNLINK_STREAM_BITS                               1

// rsspecan_rngLTEDownlinkSEMChannel
#define RSSPECAN_VAL_LTE_DOWNLINK_CATEGORY_A                                0
#define RSSPECAN_VAL_LTE_DOWNLINK_CATEGORY_B                                1

// rsspecan_rngLTEDownlinkAssumedCarrier
#define RSSPECAN_VAL_LTE_DOWNLINK_ASSUMED_EUTRA                             0
#define RSSPECAN_VAL_LTE_DOWNLINK_ASSUMED_UTRA128                           1
#define RSSPECAN_VAL_LTE_DOWNLINK_ASSUMED_UTRA384                           2
#define RSSPECAN_VAL_LTE_DOWNLINK_ASSUMED_UTRA768                           3

// rsspecan_rngLTEDownlinkMeasurementType
#define RSSPECAN_VAL_LTE_DOWNLINK_CBUF                                      0
#define RSSPECAN_VAL_LTE_DOWNLINK_EVCA                                      1
#define RSSPECAN_VAL_LTE_DOWNLINK_EVSY                                      2
#define RSSPECAN_VAL_LTE_DOWNLINK_FEVS                                      3
#define RSSPECAN_VAL_LTE_DOWNLINK_EVSU                                      4
#define RSSPECAN_VAL_LTE_DOWNLINK_SEM                                       5
#define RSSPECAN_VAL_LTE_DOWNLINK_ACP                                       6
#define RSSPECAN_VAL_LTE_DOWNLINK_PSPE                                      7
#define RSSPECAN_VAL_LTE_DOWNLINK_PVRP                                      8
#define RSSPECAN_VAL_LTE_DOWNLINK_PVRR                                      9
#define RSSPECAN_VAL_LTE_DOWNLINK_FLAT                                      10
#define RSSPECAN_VAL_LTE_DOWNLINK_GDEL                                      11
#define RSSPECAN_VAL_LTE_DOWNLINK_FDIF                                      12
#define RSSPECAN_VAL_LTE_DOWNLINK_CONS                                      13
#define RSSPECAN_VAL_LTE_DOWNLINK_DFTC                                      14
#define RSSPECAN_VAL_LTE_DOWNLINK_CCDF                                      15
#define RSSPECAN_VAL_LTE_DOWNLINK_SFLO                                      16
#define RSSPECAN_VAL_LTE_DOWNLINK_ASUM                                      17
#define RSSPECAN_VAL_LTE_DOWNLINK_BSTR                                      18

// rsspecan_rngLTEDownlinkConstellationLocation
#define RSSPECAN_VAL_LTE_DOWNLINK_LOC_AMD                                   0
#define RSSPECAN_VAL_LTE_DOWNLINK_LOC_BMD                                   1

// rsspecan_rngIQDataGateType
#define RSSPECAN_VAL_GATE_TYPE_LEVEL                                        0
#define RSSPECAN_VAL_GATE_TYPE_EDGE                                         1

// rsspecan_rngIQDataDisplayType
#define RSSPECAN_VAL_IQ_DATA_TYPE_RIMAG                                     0
#define RSSPECAN_VAL_IQ_DATA_TYPE_MAGN                                      1
#define RSSPECAN_VAL_IQ_DATA_TYPE_FREQ                                      2
#define RSSPECAN_VAL_IQ_DATA_TYPE_VECT                                      3

// rsspecan_rngGSMK10Sync
#define RSSPECAN_VAL_GSM_K10_SYNC_ALL                                       0
#define RSSPECAN_VAL_GSM_K10_SYNC_TSC                                       1
#define RSSPECAN_VAL_GSM_K10_SYNC_BURS                                      2
#define RSSPECAN_VAL_GSM_K10_SYNC_NONE                                      3

// rsspecan_rngGSMK10PvsTFilter
#define RSSPECAN_VAL_GSM_K10_FILTER_1MHZ                                    0
#define RSSPECAN_VAL_GSM_K10_FILTER_500KHZ                                  1
#define RSSPECAN_VAL_GSM_K10_FILTER_600KHZ                                  2

// rsspecan_rngGSMK10MultiCarrierFilter
#define RSSPECAN_VAL_GSM_K10_MCAR_FILT_400                                  0
#define RSSPECAN_VAL_GSM_K10_MCAR_FILT_300                                  1

// rsspecan_rngWLANFFTStartOffset
#define RSSPECAN_VAL_WLAN_FFT_START_OFFSET_AUTO                             0
#define RSSPECAN_VAL_WLAN_FFT_START_OFFSET_GIC                              1
#define RSSPECAN_VAL_WLAN_FFT_START_OFFSET_PEAK                             2

// rsspecan_rngFMSTriggerSource
#define RSSPECAN_VAL_FMS_TRG_IMM                                            0
#define RSSPECAN_VAL_FMS_TRG_EXT                                            1
#define RSSPECAN_VAL_FMS_TRG_IFP                                            2
#define RSSPECAN_VAL_FMS_TRG_TIME                                           3
#define RSSPECAN_VAL_FMS_TRG_LEFT                                           4
#define RSSPECAN_VAL_FMS_TRG_RIGH                                           5
#define RSSPECAN_VAL_FMS_TRG_MPX                                            6
#define RSSPECAN_VAL_FMS_TRG_MONO                                           7
#define RSSPECAN_VAL_FMS_TRG_STER                                           8
#define RSSPECAN_VAL_FMS_TRG_RDS                                            9
#define RSSPECAN_VAL_FMS_TRG_PIL                                            10

// rsspecan_rngFMSDisplayConfig
#define RSSPECAN_VAL_FMS_DISP_SUMM                                          1
#define RSSPECAN_VAL_FMS_DISP_TD_RF                                         2
#define RSSPECAN_VAL_FMS_DISP_TD_LEFT                                       3
#define RSSPECAN_VAL_FMS_DISP_TD_RIGHT                                      4
#define RSSPECAN_VAL_FMS_DISP_TD_MPX                                        5
#define RSSPECAN_VAL_FMS_DISP_TD_MONO                                       6
#define RSSPECAN_VAL_FMS_DISP_TD_STER                                       7
#define RSSPECAN_VAL_FMS_DISP_TD_RDS                                        8
#define RSSPECAN_VAL_FMS_DISP_TD_PILOT                                      9
#define RSSPECAN_VAL_FMS_DISP_SPEC_RF                                       10
#define RSSPECAN_VAL_FMS_DISP_SPEC_LEFT                                     11
#define RSSPECAN_VAL_FMS_DISP_SPEC_RIGHT                                    12
#define RSSPECAN_VAL_FMS_DISP_SPEC_MPX                                      13
#define RSSPECAN_VAL_FMS_DISP_SPEC_MONO                                     14
#define RSSPECAN_VAL_FMS_DISP_SPEC_STER                                     15
#define RSSPECAN_VAL_FMS_DISP_SPEC_RDS                                      16
#define RSSPECAN_VAL_FMS_DISP_SPEC_PILOT                                    17

// rsspecan_rngFMSRSumDetector
#define RSSPECAN_VAL_FMS_RSUM_DETEC_RMS                                     0
#define RSSPECAN_VAL_FMS_RSUM_DETEC_SRMS                                    1
#define RSSPECAN_VAL_FMS_RSUM_DETEC_PPE                                     2
#define RSSPECAN_VAL_FMS_RSUM_DETEC_NPE                                     3
#define RSSPECAN_VAL_FMS_RSUM_DETEC_PAV                                     4
#define RSSPECAN_VAL_FMS_RSUM_DETEC_QPE                                     5
#define RSSPECAN_VAL_FMS_RSUM_DETEC_SQP                                     6

// rsspecan_rngFMSRSumMode
#define RSSPECAN_VAL_FMS_RSUM_MODE_WRIT                                     0
#define RSSPECAN_VAL_FMS_RSUM_MODE_AVER                                     1
#define RSSPECAN_VAL_FMS_RSUM_MODE_PHOL                                     2

// rsspecan_rngDBBTriggerSource
#define RSSPECAN_VAL_TRG_BBP                                                5

// rsspecan_rngVSAMarkerSearch
#define RSSPECAN_VAL_VSA_MARKER_SEARCH_REAL                                 0
#define RSSPECAN_VAL_VSA_MARKER_SEARCH_IMAG                                 1
#define RSSPECAN_VAL_VSA_MARKER_SEARCH_MAG                                  2

// rsspecan_rngVSASignalType
#define RSSPECAN_VAL_VSA_SIGNAL_TYPE_CONT                                   0
#define RSSPECAN_VAL_VSA_SIGNAL_TYPE_BURST                                  1

// rsspecan_rngVSATXFilterType
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_NONE                                0
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_RC                                  1
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_RRC                                 2
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_GAUSS                               3
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_GMSK                                4
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_LIN_GMSK                            5
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_APCO25                              6
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_CDMA2K_FWD                          7
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_CDMA2K_REV                          8
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_RECT                                9
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_HALF_SINE                           10
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_APCO25_HCPM                         11
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_APCO25_HDQPSK                       12
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_APCO25_HD8PSK_NARROW                13
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_APCO25_HD8PSK_WIDE                  14
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_EDGE_NARROW                         15
#define RSSPECAN_VAL_VSA_TX_FILTER_TYPE_EDGE_WIDE                           16
#define RSSPECAN_VAL_VSA_TX_FILTER_USER                                     17

// rsspecan_rngVSAMFilterType
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_NONE                              0
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_RRC                               1
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_EDGE                              2
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_EDGE_HSR_NARROW                   3
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_EDGE_HSR_WIDE                     4
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_GAUSS                             5
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_LOW_ISI                           6
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_LOW_PASS_NARROW                   7
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_LOW_PASS_WIDE                     8
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_RECTANGULAR                       9
#define RSSPECAN_VAL_VSA_MEAS_FILTER_TYPE_USER                              10

// rsspecan_rngVSACaptureOversampling
#define RSSPECAN_VAL_STAT_MODE_SONLY                                        0
#define RSSPECAN_VAL_STAT_MODE_INFINITE                                     1

// rsspecan_rngVSATraceEvaluation
#define RSSPECAN_VAL_TRAC_EVAL_MEAS                                         0
#define RSSPECAN_VAL_TRAC_EVAL_REF                                          1

// rsspecan_rngVSAModulationEVMCalcFSV
#define RSSPECAN_VAL_VSA_MOD_EVM_SYMBOL                                     0
#define RSSPECAN_VAL_VSA_MOD_EVM_SIGNAL                                     1
#define RSSPECAN_VAL_VSA_MOD_EVM_MACP                                       2
#define RSSPECAN_VAL_VSA_MOD_EVM_MECP                                       3

// rsspecan_rngTetraBurstType
#define RSSPECAN_VAL_TETRA_SLOT_TYPE_NDB                                    0
#define RSSPECAN_VAL_TETRA_SLOT_TYPE_NUB                                    1
#define RSSPECAN_VAL_TETRA_SLOT_TYPE_CUB                                    2
#define RSSPECAN_VAL_TETRA_SLOT_TYPE_RAB                                    3

// rsspecan_rngTetraChanBandwidth
#define RSSPECAN_VAL_TETRA_CHANN_BAND_25                                    0
#define RSSPECAN_VAL_TETRA_CHANN_BAND_50                                    1
#define RSSPECAN_VAL_TETRA_CHANN_BAND_100                                   2
#define RSSPECAN_VAL_TETRA_CHANN_BAND_150                                   3

// rsspecan_rngTetraModulation
#define RSSPECAN_VAL_TETRA_MOD_TYPE_QM4                                     0
#define RSSPECAN_VAL_TETRA_MOD_TYPE_QM16                                    1
#define RSSPECAN_VAL_TETRA_MOD_TYPE_QM64                                    2

// rsspecan_rngTetraMaxCarrOffset
#define RSSPECAN_VAL_TETRA_CARRIER_OFFSET_KH1                               0
#define RSSPECAN_VAL_TETRA_CARRIER_OFFSET_KH10                              1

// rsspecan_rngTetraSymbConstellation
#define RSSPECAN_VAL_TETRA_SYMBOLS_ALL                                      0
#define RSSPECAN_VAL_TETRA_SYMBOLS_DATA                                     1
#define RSSPECAN_VAL_TETRA_SYMBOLS_HEAD                                     2
#define RSSPECAN_VAL_TETRA_SYMBOLS_SYNC                                     3
#define RSSPECAN_VAL_TETRA_SYMBOLS_PIL                                      4

// rsspecan_rngGSMK10Alignment
#define RSSPECAN_VAL_GSM_K10_ALIGN_STM                                      0
#define RSSPECAN_VAL_GSM_K10_ALIGN_PSL                                      1

// rsspecan_rngGSMK10ResultDisplay
#define RSSPECAN_VAL_GSM_K10_DISPLAY_FREQ                                   0
#define RSSPECAN_VAL_GSM_K10_DISPLAY_LIST                                   1

// rsspecan_rngBandPowerMode
#define RSSPECAN_VAL_BPOWER_POWER                                           0
#define RSSPECAN_VAL_BPOWER_DENSITY                                         1

// rsspecan_rngInputUnit
#define RSSPECAN_VAL_DINPUT_UNIT_V                                          0
#define RSSPECAN_VAL_DINPUT_UNIT_DBM                                        1
#define RSSPECAN_VAL_DINPUT_UNIT_DBPV                                       2
#define RSSPECAN_VAL_DINPUT_UNIT_W                                          3
#define RSSPECAN_VAL_DINPUT_UNIT_DBMV                                       4
#define RSSPECAN_VAL_DINPUT_UNIT_DBUV                                       5
#define RSSPECAN_VAL_DINPUT_UNIT_A                                          6

// rsspecan_rngWIMAXSubframeRPT
#define RSSPECAN_VAL_WIMAX_SUBFRAME_RPTY_RMS                                0
#define RSSPECAN_VAL_WIMAX_SUBFRAME_RPTY_PEAK                               1
#define RSSPECAN_VAL_WIMAX_SUBFRAME_RPTY_USER                               2

// rsspecan_rngWIMAXSubframeRPOT
#define RSSPECAN_VAL_WIMAX_SUBFRAME_RPOT_PSN                                0
#define RSSPECAN_VAL_WIMAX_SUBFRAME_RPOT_USER                               1

// rsspecan_rngWIMAXBurstBoost
#define RSSPECAN_VAL_WIMAX_BURST_BOOST_PRED                                 0
#define RSSPECAN_VAL_WIMAX_BURST_BOOST_EST                                  1

// rsspecan_rngAntenna
#define RSSPECAN_VAL_WIMAX_ANTENNA0                                         0
#define RSSPECAN_VAL_WIMAX_ANTENNA1                                         1
#define RSSPECAN_VAL_WIMAX_ANTENNA2                                         2
#define RSSPECAN_VAL_WIMAX_ANTENNA3                                         3

// rsspecan_rngWIMAXSTCMode
#define RSSPECAN_VAL_WIMAX_STCMODE_STTD                                     0
#define RSSPECAN_VAL_WIMAX_STCMODE_1TX                                      1
#define RSSPECAN_VAL_WIMAX_STCMODE_2TX                                      2

// rsspecan_rngWIMAXUplinkZoneSTCM
#define RSSPECAN_VAL_WIMAX_STCMODE_MULTIPLEXING                             1
#define RSSPECAN_VAL_WIMAX_STCMODE_BURS                                     2

// rsspecan_rngTDBSMaxMode
#define RSSPECAN_VAL_TDS_MMAX_QPSK                                          0
#define RSSPECAN_VAL_TDS_MMAX_8PSK                                          1
#define RSSPECAN_VAL_TDS_MMAX_16QAM                                         2
#define RSSPECAN_VAL_TDS_MMAX_64QAM                                         3

// rsspecan_rngTDSSyncToSlotMode
#define RSSPECAN_VAL_TDS_STS_CODE                                           0
#define RSSPECAN_VAL_TDS_STS_MA                                             1

// rsspecan_rngTDSSEMLimits
#define RSSPECAN_VAL_TDUE_LIMIT_750                                         0
#define RSSPECAN_VAL_TDUE_LIMIT_760                                         1

// rsspecan_rngSpecColorStyle
#define RSSPECAN_VAL_SPEM_HOT                                               0
#define RSSPECAN_VAL_SPEM_COLD                                              1
#define RSSPECAN_VAL_SPEM_COL                                               2
#define RSSPECAN_VAL_SPEM_RAD                                               3
#define RSSPECAN_VAL_SPEM_GRAY                                              4

// rsspecan_rngPersisColorStyle
#define RSSPECAN_VAL_COL_HOT                                                0
#define RSSPECAN_VAL_COL_COLD                                               1
#define RSSPECAN_VAL_COL_RAD                                                2
#define RSSPECAN_VAL_COL_GRAY                                               3

// rsspecan_rngPersisTraceSymb
#define RSSPECAN_VAL_TRA_DOTS                                               0
#define RSSPECAN_VAL_TRA_VECT                                               1

// rsspecan_rngSpecMaskCondition
#define RSSPECAN_VAL_MASK_ENT                                               0
#define RSSPECAN_VAL_MASK_LEAV                                              1
#define RSSPECAN_VAL_MASK_INS                                               2
#define RSSPECAN_VAL_MASK_OUTS                                              3

// rsspecan_rngMaskMode
#define RSSPECAN_VAL_MASK_ABS                                               0
#define RSSPECAN_VAL_MASK_REL                                               1

// rsspecan_rngresultdispay
#define RSSPECAN_VAL_PSPEM                                                  0
#define RSSPECAN_VAL_SPEM                                                   1
#define RSSPECAN_VAL_REAL_SPEM                                              2
#define RSSPECAN_VAL_POWER_TIME                                             3
#define RSSPECAN_VAL_PVT_WATER                                              4

// rsspecan_rngCATMeasMode
#define RSSPECAN_VAL_CAT_MEAS_MODE_REFL                                     0
#define RSSPECAN_VAL_CAT_MEAS_MODE_DTF                                      1
#define RSSPECAN_VAL_CAT_MEAS_MODE_LOSS                                     2
#define RSSPECAN_VAL_CAT_MEAS_MODE_TRAN                                     3

// rsspecan_rngCATAmplUnits
#define RSSPECAN_VAL_CAT_AMPLITUDE_UNITS_DBM                                0
#define RSSPECAN_VAL_CAT_AMPLITUDE_UNITS_DBUV                               1
#define RSSPECAN_VAL_CAT_AMPLITUDE_UNITS_DBMV                               2
#define RSSPECAN_VAL_CAT_AMPLITUDE_UNITS_VOLT                               3
#define RSSPECAN_VAL_CAT_AMPLITUDE_UNITS_WATT                               4

// rsspecan_rngLTEUplinkBandwidth
#define RSSPECAN_VAL_LTE_UPLINK_BW1_40                                      0
#define RSSPECAN_VAL_LTE_UPLINK_BW2_50                                      1
#define RSSPECAN_VAL_LTE_UPLINK_BW3_00                                      2
#define RSSPECAN_VAL_LTE_UPLINK_BW5_00                                      3
#define RSSPECAN_VAL_LTE_UPLINK_BW10_00                                     4
#define RSSPECAN_VAL_LTE_UPLINK_BW15_00                                     5
#define RSSPECAN_VAL_LTE_UPLINK_BW20_00                                     6

// rsspecan_rngLTEUplinkCyclicPrefix
#define RSSPECAN_VAL_LTE_UPLINK_CPREFIX_NORMAL                              0
#define RSSPECAN_VAL_LTE_UPLINK_CPREFIX_EXTENDED                            1
#define RSSPECAN_VAL_LTE_UPLINK_CPREFIX_AUTO                                2

// rsspecan_rngLTEUplinkChannelEstimation
#define RSSPECAN_VAL_LTE_UPLINK_CEST_PILOT                                  0
#define RSSPECAN_VAL_LTE_UPLINK_CEST_PILPAY                                 1

// rsspecan_rngLTEUplinkPhaseTracking
#define RSSPECAN_VAL_LTE_UPLINK_PHASE_TRACKING_OFF                          0
#define RSSPECAN_VAL_LTE_UPLINK_PHASE_TRACKING_PILOT                        1
#define RSSPECAN_VAL_LTE_UPLINK_PHASE_TRACKING_PILPAY                       2

// rsspecan_rngLTEUplinkSubframeEnable
#define RSSPECAN_VAL_LTE_UPLINK_SUBFRAME_PUSCH                              0
#define RSSPECAN_VAL_LTE_UPLINK_SUBFRAME_PUCCH                              1

// rsspecan_rngLTEUplinkModulation
#define RSSPECAN_VAL_LTE_UPLINK_MODULATION_QPSK                             0
#define RSSPECAN_VAL_LTE_UPLINK_MODULATION_16QAM                            1
#define RSSPECAN_VAL_LTE_UPLINK_MODULATION_64QAM                            2

// rsspecan_rngLTEUplinkBitStreamFormat
#define RSSPECAN_VAL_LTE_UPLINK_STREAM_SYMBOLS                              0
#define RSSPECAN_VAL_LTE_UPLINK_STREAM_BITS                                 1

// rsspecan_rngLTEUplinkSEMChannel
#define RSSPECAN_VAL_LTE_UPLINK_CATEGORY_A                                  0
#define RSSPECAN_VAL_LTE_UPLINK_CATEGORY_B                                  1

// rsspecan_rngLTEUplinkAssumedCarrier
#define RSSPECAN_VAL_LTE_UPLINK_ASSUMED_EUTRA                               0
#define RSSPECAN_VAL_LTE_UPLINK_ASSUMED_UTRA128                             1
#define RSSPECAN_VAL_LTE_UPLINK_ASSUMED_UTRA384                             2
#define RSSPECAN_VAL_LTE_UPLINK_ASSUMED_UTRA768                             3

// rsspecan_rngLTEUplinkMeasurementType
#define RSSPECAN_VAL_LTE_UPLINK_CBUF                                        0
#define RSSPECAN_VAL_LTE_UPLINK_EVCA                                        1
#define RSSPECAN_VAL_LTE_UPLINK_EVSY                                        2
#define RSSPECAN_VAL_LTE_UPLINK_FEVS                                        3
#define RSSPECAN_VAL_LTE_UPLINK_EVSU                                        4
#define RSSPECAN_VAL_LTE_UPLINK_SEM                                         5
#define RSSPECAN_VAL_LTE_UPLINK_ACP                                         6
#define RSSPECAN_VAL_LTE_UPLINK_PSPE                                        7
#define RSSPECAN_VAL_LTE_UPLINK_FLAT                                        8
#define RSSPECAN_VAL_LTE_UPLINK_GDEL                                        9
#define RSSPECAN_VAL_LTE_UPLINK_FDIF                                        10
#define RSSPECAN_VAL_LTE_UPLINK_IE                                          11
#define RSSPECAN_VAL_LTE_UPLINK_CONS                                        12
#define RSSPECAN_VAL_LTE_UPLINK_DFTC                                        13
#define RSSPECAN_VAL_LTE_UPLINK_CCDF                                        14
#define RSSPECAN_VAL_LTE_UPLINK_ASUM                                        15
#define RSSPECAN_VAL_LTE_UPLINK_BSTR                                        16

// rsspecan_rngTriggerMode
#define RSSPECAN_VAL_TRIG_MODE_IMMEDIATE                                    0
#define RSSPECAN_VAL_TRIG_MODE_EXTERNAL                                     1
#define RSSPECAN_VAL_TRIG_MODE_IF_POWER                                     2
#define RSSPECAN_VAL_TRIG_MODE_PSENSOR                                      3
#define RSSPECAN_VAL_TRIG_MODE_RF_POWER                                     4
#define RSSPECAN_VAL_TRIG_MODE_POWER                                        5
#define RSSPECAN_VAL_TRIG_MODE_TUNIT                                        6

// rsspecan_rngLTEUplinkSRSMode
#define RSSPECAN_VAL_LTE_UPLINK_SRS_MODE_EXTENSION                          0
#define RSSPECAN_VAL_LTE_UPLINK_SRS_MODE_TRUNCATION                         1
#define RSSPECAN_VAL_LTE_UPLINK_SRS_MODE_AUTO                               2
#define RSSPECAN_VAL_LTE_UPLINK_SRS_MODE_DIRECT                             3

// rsspecan_rngLTEUplinkPUSCHFreqHoppingMode
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_FREQUENCY_HOPPING_OFF                 0
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_FREQUENCY_HOPPING_INTER               1
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_FREQUENCY_HOPPING_INTRA               2

// rsspecan_rngLTEUplinkPUCCHFormat
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1_NORMAL                      0
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1_SHORTENED                   1
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1A_NORMAL                     2
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1A_SHORTENED                  3
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1B_NORMAL                     4
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1B_SHORTENED                  5
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F2                             6
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F2A                            7
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F2B                            8
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F3                             9
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1                             10
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1A                            11
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_FORMAT_F1B                            12

// rsspecan_rngContinuousTrigger
#define RSSPECAN_VAL_TRIGGER_CONTINUOUS                                     0
#define RSSPECAN_VAL_TRIGGER_STOP                                           1

// rsspecan_rngFFTWinType
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_BLACKHARRIS                            0
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_FLATTOP                                1
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_GAUSSIAN                               2
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_HANNING                                3
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_KAISERBESSER                           4
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_RECTANGULAR                            5
#define RSSPECAN_VAL_SPEM_FFT_WINDOW_HAMMING                                6

// rsspecan_rngGSMK10BurstSync
#define RSSPECAN_VAL_SYNC_USER                                              -1
#define RSSPECAN_VAL_SYNC_0                                                 0
#define RSSPECAN_VAL_SYNC_1                                                 1
#define RSSPECAN_VAL_SYNC_2                                                 2

// rsspecan_rngGSMK10DemSymDec
#define RSSPECAN_VAL_GSM_K10_SYMBOL_DECISION_AUTO                           0
#define RSSPECAN_VAL_GSM_K10_SYMBOL_DECISION_LINEAR                         1
#define RSSPECAN_VAL_GSM_K10_SYMBOL_DECISION_SEQUENCE                       2

// rsspecan_rngMarkerPosition
#define RSSPECAN_VAL_MARKER_POSITION_MAXHOLD                                0
#define RSSPECAN_VAL_MARKER_POSITION_PERSISTENCE                            1

// rsspecan_rngGSMK10TailTSCBits
#define RSSPECAN_VAL_GSM_K10_STD_BITS_DETECTED                              0
#define RSSPECAN_VAL_GSM_K10_STD_BITS_STANDARD                              1

// rsspecan_rngVectMeasRange
#define RSSPECAN_VAL_WLAN_MEAS_RANGE_ALL                                    0
#define RSSPECAN_VAL_WLAN_MEAS_RANGE_PSDU                                   1

// rsspecan_rngGMSK10TraceType
#define RSSPECAN_TRAC_MOD_PDF                                               1

// rsspecan_rngEMIFmeasDetectorType
#define RSSPECAN_VAL_FMEAS_DETECTOR_TYPE_OFF                                0
#define RSSPECAN_VAL_FMEAS_DETECTOR_TYPE_AVER                               1
#define RSSPECAN_VAL_FMEAS_DETECTOR_TYPE_CAV                                2
#define RSSPECAN_VAL_FMEAS_DETECTOR_TYPE_CRMS                               3
#define RSSPECAN_VAL_FMEAS_DETECTOR_TYPE_POS                                4
#define RSSPECAN_VAL_FMEAS_DETECTOR_TYPE_QPE                                5

// rsspecan_rngEMILISNVnetworkType
#define RSSPECAN_VAL_EMI_VNET_TYPE_OFF                                      0
#define RSSPECAN_VAL_EMI_VNET_TYPE_ESH3Z5                                   1
#define RSSPECAN_VAL_EMI_VNET_TYPE_ESH2Z5                                   2
#define RSSPECAN_VAL_EMI_VNET_TYPE_4200                                     3
#define RSSPECAN_VAL_EMI_VNET_TYPE_216                                      4

// rsspecan_rngEMILISNVNetworkPhase
#define RSSPECAN_VAL_EMI_VNET_PHAS_N                                        0
#define RSSPECAN_VAL_EMI_VNET_PHAS_L1                                       1
#define RSSPECAN_VAL_EMI_VNET_PHAS_L2                                       2
#define RSSPECAN_VAL_EMI_VNET_PHAS_L3                                       3

// rsspecan_rngEMIBandwidthFilterType
#define RSSPECAN_VAL_EMI_FILT_TYPE_NORM                                     0
#define RSSPECAN_VAL_EMI_FILT_TYPE_FFT                                      1
#define RSSPECAN_VAL_EMI_FILT_TYPE_CFIL                                     2
#define RSSPECAN_VAL_EMI_FILT_TYPE_RRC                                      3
#define RSSPECAN_VAL_EMI_FILT_TYPE_P5                                       4
#define RSSPECAN_VAL_EMI_FILT_TYPE_CISPR                                    5
#define RSSPECAN_VAL_EMI_FILT_TYPE_MIL                                      6

// rsspecan_rngWlanPVRMode
#define RSSPECAN_VAL_WLAN_PVR_ALL                                           0
#define RSSPECAN_VAL_WLAN_PVR_PSDU                                          1

// rsspecan_rngNoiseMarkerAssign
#define RSSPECAN_VAL_TRACE_NOISE_CURRENT                                    1
#define RSSPECAN_VAL_TRACE_NOISE_MEM1                                       2
#define RSSPECAN_VAL_TRACE_NOISE_MEM2                                       3
#define RSSPECAN_VAL_TRACE_NOISE_MEM3                                       4
#define RSSPECAN_VAL_TRACE_GAIN_CURRENT                                     5
#define RSSPECAN_VAL_TRACE_GAIN_MEM1                                        6
#define RSSPECAN_VAL_TRACE_GAIN_MEM2                                        7
#define RSSPECAN_VAL_TRACE_GAIN_MEM3                                        8

// rsspecan_rng3GBSStandard
#define RSSPECAN_VAL_3GPP_BS_HOME                                           0
#define RSSPECAN_VAL_3GPP_BS_NORMAL                                         1

// rsspecan_rngVSALimitCheck
#define RSSPECAN_VAL_VSA_LIMIT_STATE_NONE                                   -1
#define RSSPECAN_VAL_VSA_LIMIT_STATE_PASS                                   0
#define RSSPECAN_VAL_VSA_LIMIT_STATE_FAIL                                   1
#define RSSPECAN_VAL_VSA_LIMIT_STATE_MARGIN                                 2

// rsspecan_rngAntennaWLAN
#define RSSPECAN_VAL_WLAN_ANTENNA1                                          1
#define RSSPECAN_VAL_WLAN_ANTENNA2                                          2
#define RSSPECAN_VAL_WLAN_ANTENNA3                                          3
#define RSSPECAN_VAL_WLAN_ANTENNA4                                          4

// rsspecan_rngAntennasNumberWLAN
#define RSSPECAN_VAL_WLAN_DUTC_TX1                                          1
#define RSSPECAN_VAL_WLAN_DUTC_TX2                                          2
#define RSSPECAN_VAL_WLAN_DUTC_TX3                                          3
#define RSSPECAN_VAL_WLAN_DUTC_TX4                                          4

// rsspecan_rngWLANBurstTypeToMeas
#define RSSPECAN_VAL_WLAN_BURST_TYPE_TO_MEAS_FBUR                           0
#define RSSPECAN_VAL_WLAN_BURST_TYPE_TO_MEAS_ALL                            1
#define RSSPECAN_VAL_WLAN_BURST_TYPE_TO_MEAS_MMIX                           2
#define RSSPECAN_VAL_WLAN_BURST_TYPE_TO_MEAS_MGRF                           3
#define RSSPECAN_VAL_WLAN_BURST_TYPE_TO_MEAS_DMIX                           4
#define RSSPECAN_VAL_WLAN_BURST_TYPE_TO_MEAS__DGRF                          5

// rsspecan_rngWLANChannelBandwidthToMeasure
#define RSSPECAN_VAL_WLAN_CHANNEL_BANDWIDTH_TO_MEAS_FBUR                    0
#define RSSPECAN_VAL_WLAN_CHANNEL_BANDWIDTH_TO_MEAS_ALL                     1
#define RSSPECAN_VAL_WLAN_CHANNEL_BANDWIDTH_TO_MEAS_MB20                    2
#define RSSPECAN_VAL_WLAN_CHANNEL_BANDWIDTH_TO_MEAS_MB40                    3
#define RSSPECAN_VAL_WLAN_CHANNEL_BANDWIDTH_TO_MEAS_DB20                    4
#define RSSPECAN_VAL_WLAN_CHANNEL_BANDWIDTH_TO_MEAS_DB40                    5

// rsspecan_rngGuardIntervalLength
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_FBUR                         0
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_ALL                          1
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_MN8                          2
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_ML16                         3
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_MN16                         4
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_ML32                         5
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_DN8                          6
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_DL16                         7
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_DN16                         8
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_LEGTH_DL32                         9

// rsspecan_rngSTBCField
#define RSSPECAN_VAL_WLAN_EXT_STBC_FBUR                                     0
#define RSSPECAN_VAL_WLAN_EXT_STBC_ALL                                      1
#define RSSPECAN_VAL_WLAN_EXT_STBC_M0                                       2
#define RSSPECAN_VAL_WLAN_EXT_STBC_M1                                       3
#define RSSPECAN_VAL_WLAN_EXT_STBC_M2                                       4
#define RSSPECAN_VAL_WLAN_EXT_STBC_D0                                       5
#define RSSPECAN_VAL_WLAN_EXT_STBC_D1                                       6
#define RSSPECAN_VAL_WLAN_EXT_STBC_D2                                       7

// rsspecan_rngWLANExtensionSpatialStreams
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_FBUR                          0
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_ALL                           1
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_M0                            2
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_M1                            3
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_M2                            4
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_M3                            5
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_D0                            6
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_D1                            7
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_D2                            8
#define RSSPECAN_VAL_WLAN_EXT_SPATIAL_STREAMS_D3                            9

// rsspecan_rngSourceOfPayloadLength
#define RSSPECAN_VAL_WLAN_SOURCE_OF_PAYLOAD_LENGTH_EST                      0
#define RSSPECAN_VAL_WLAN_SOURCE_OF_PAYLOAD_LENGTH_HTS                      1
#define RSSPECAN_VAL_WLAN_SOURCE_OF_PAYLOAD_LENGTH_LS                       2

// rsspecan_rngMCSIndexToUse
#define RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_FBUR                             0
#define RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_ALL                              1
#define RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_MEAS                             2
#define RSSPECAN_VAL_WLAN_MCS_INDEX_TO_USE_DEMOD                            3

// rsspecan_rngWLANMIMOSpatialMappingMode
#define RSSPECAN_VAL_WLAN_MIMO_SPATIAL_MAPPING_MODE_DIR                     0
#define RSSPECAN_VAL_WLAN_MIMO_SPATIAL_MAPPING_MODE_SEXP                    1
#define RSSPECAN_VAL_WLAN_MIMO_SPATIAL_MAPPING_MODE_USER                    2

// rsspecan_rngWLAN_SpectFlat
#define RSSPECAN_VAL_WLAN_SPEC_MODE_SEL_FLAT                                5
#define RSSPECAN_VAL_WLAN_SPEC_MODE_SEL_GRD                                 6

// rsspecan_rngWLANChannelSelect
#define RSSPECAN_VAL_WLAN_SPEC_MODE_CSEL_EFF                                0
#define RSSPECAN_VAL_WLAN_SPEC_MODE_CSEL_PHY                                1

// rsspecan_rngDL_PDSCHSubframeDetect
#define RSSPECAN_VAL_LTE_DOWNLINK_PDSCH_SUBFRAME_DETECT_OFF                 0
#define RSSPECAN_VAL_LTE_DOWNLINK_PDSCH_SUBFRAME_DETECT_PDCCH               1
#define RSSPECAN_VAL_LTE_DOWNLINK_PDSCH_SUBFRAME_DETECT_PHYDET              2

// rsspecan_rngDL_PDSCHReferenceData
#define RSSPECAN_VAL_LTE_DOWNLINK_PDSCH_REFERENCE_DATA_AUTO                 0
#define RSSPECAN_VAL_LTE_DOWNLINK_PDSCH_REFERENCE_DATA_ALL0                 1

// rsspecan_rngLTEUplinkSlotSelection
#define RSSPECAN_VAL_LTE_UPLINK_SLOT_SELECTION_ALL                          0
#define RSSPECAN_VAL_LTE_UPLINK_SLOT_SELECTION_S0                           1
#define RSSPECAN_VAL_LTE_UPLINK_SLOT_SELECTION_S1                           2

// rsspecan_rngWLAN_MIMO_DUTConfiguration
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_RX1                              1
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_RX2                              2
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_RX3                              3
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_RX4                              4

// rsspecan_rngWLAN_MIMOAntennaSignalCapture
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_TYPE_SIM                         0
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_TYPE_OSP                         1
#define RSSPECAN_VAL_WLAN_MIMO_ANT_CAPTURE_TYPE_MAN                         2

// rsspecan_rngWLAN_MIMO_OSPSwitchModule
#define RSSPECAN_VAL_WLAN_MIMO_OSP_SWITCH_MODULE_A11                        0
#define RSSPECAN_VAL_WLAN_MIMO_OSP_SWITCH_MODULE_A12                        1
#define RSSPECAN_VAL_WLAN_MIMO_OSP_SWITCH_MODULE_A13                        2

// rsspecan_rngAntennaPattern
#define RSSPECAN_VAL_3GPP_CPICH_PATTERN_NONE                                0
#define RSSPECAN_VAL_3GPP_CPICH_PATTERN_1                                   1
#define RSSPECAN_VAL_3GPP_CPICH_PATTERN_2                                   2

// rsspecan_rngTriggerDirection
#define RSSPECAN_VAL_TRIG_DIRECTION_INP                                     0
#define RSSPECAN_VAL_TRIG_DIRECTION_OUTP                                    1

// rsspecan_rngTriggerOType
#define RSSPECAN_VAL_TRIG_OTYPE_DEVICE                                      0
#define RSSPECAN_VAL_TRIG_OTYPE_TARMED                                      1
#define RSSPECAN_VAL_TRIG_OTYPE_UDEFINED                                    2

// rsspecan_rngTriggerLevel
#define RSSPECAN_VAL_TRIG_LEVEL_HIGH                                        0
#define RSSPECAN_VAL_TRIG_LEVEL_LOW                                         1

// rsspecan_rngPreampLevel
#define RSSPECAN_VAL_PREAMPLIFICATION_LEVEL_15DB                            0
#define RSSPECAN_VAL_PREAMPLIFICATION_LEVEL_30DB                            1

// rsspecan_rngWLANTrackingPilots
#define RSSPECAN_VAL_WLAN_TRACKING_PILOTS_STANDARD                          0
#define RSSPECAN_VAL_WLAN_TRACKING_PILOTS_DETECTED                          1

// rsspecan_rngVSAFineSynchronisationMode
#define RSSPECAN_VAL_VSA_FINE_SYNC_KDAT                                     0
#define RSSPECAN_VAL_VSA_FINE_SYNC_PATT                                     1
#define RSSPECAN_VAL_VSA_FINE_SYNC_DDAT                                     2

// rsspecan_rngCalibrationProbeMicroButton
#define RSSPECAN_VAL_PROBE_MICRO_BUTTON_RUN_SINGLE                          0
#define RSSPECAN_VAL_PROBE_MICRO_BUTTON_NO_ACTION                           1

// rsspecan_rngPowerNoiseCorrection
#define RSSPECAN_VAL_MEAS_POW_NOISE_CORR_OFF                                0
#define RSSPECAN_VAL_MEAS_POW_NOISE_CORR_ON                                 1
#define RSSPECAN_VAL_MEAS_POW_NOISE_CORR_AUTO                               2

// rsspecan_rngGSMK10FilterType
#define RSSPECAN_VAL_GSM_K10_FILTER_TYPE_NORMAL                             0
#define RSSPECAN_VAL_GSM_K10_FILTER_TYPE_P5                                 1

// rsspecan_rngGSMK10ReferencePower
#define RSSPECAN_VAL_GSM_K10_REF_POWER_RMS                                  0
#define RSSPECAN_VAL_GSM_K10_REF_POWER_PEAK                                 1

// rsspecan_rngGSMK10FrequencyList
#define RSSPECAN_VAL_GSM_K10_FREQUENCY_LIST_NARROW                          0
#define RSSPECAN_VAL_GSM_K10_FREQUENCY_LIST_WIDE                            1

// rsspecan_rngProbeConnectionState
#define RSSPECAN_VAL_PROBE_CONN_STATE_DET                                   1
#define RSSPECAN_VAL_PROBE_CONN_STATE_NDET                                  0

// rsspecan_rngPhaseUserTrace
#define RSSPECAN_VAL_PHASE_NONE                                             0
#define RSSPECAN_VAL_PHASE_TRACE1                                           1
#define RSSPECAN_VAL_PHASE_TRACE2                                           2
#define RSSPECAN_VAL_PHASE_TRACE3                                           3
#define RSSPECAN_VAL_PHASE_TRACE4                                           4
#define RSSPECAN_VAL_PHASE_TRACE5                                           5
#define RSSPECAN_VAL_PHASE_TRACE6                                           6

// rsspecan_rngPhaseXAxisScope
#define RSSPECAN_VAL_NOISE_X_SCOPE_MANUAL                                   0
#define RSSPECAN_VAL_NOISE_X_SCOPE_HDECADE                                  1
#define RSSPECAN_VAL_NOISE_X_SCOPE_MRANGE                                   2

// rsspecan_rngPhaseYAxisManualScaling
#define RSSPECAN_VAL_PHASE_Y_SCALE_OFF                                      0
#define RSSPECAN_VAL_PHASE_Y_SCALE_TOP_BOTTOM                               1
#define RSSPECAN_VAL_PHASE_Y_SCALE_TOP_RANGE                                2
#define RSSPECAN_VAL_PHASE_Y_SCALE_BOTTOM_RANGE                             3

// rsspecan_rngPhaseNoiseLimitShapeType
#define RSSPECAN_VAL_PHASE_LIMIT_NONE                                       0
#define RSSPECAN_VAL_PHASE_LIMIT_FC1                                        1
#define RSSPECAN_VAL_PHASE_LIMIT_FC2                                        2
#define RSSPECAN_VAL_PHASE_LIMIT_FC3                                        3
#define RSSPECAN_VAL_PHASE_LIMIT_FC4                                        4
#define RSSPECAN_VAL_PHASE_LIMIT_FC5                                        5

// rsspecan_rngPhaseWindowFunction
#define RSSPECAN_VAL_PHASE_WINDOW_CHEB                                      0
#define RSSPECAN_VAL_PHASE_WINDOW_GAUS                                      1
#define RSSPECAN_VAL_PHASE_WINDOW_RECT                                      2
#define RSSPECAN_VAL_PHASE_WINDOW_BHAR                                      3

// rsspecan_rngPulseSignalPeriod
#define RSSPECAN_VAL_PULSE_SIGNAL_PERIOD_HL                                 0
#define RSSPECAN_VAL_PULSE_SIGNAL_PERIOD_LH                                 1

// rsspecan_rngPulseSignalModulation
#define RSSPECAN_VAL_PULSE_SIGNAL_MODULATION_ARB                            0
#define RSSPECAN_VAL_PULSE_SIGNAL_MODULATION_CW                             1
#define RSSPECAN_VAL_PULSE_SIGNAL_MODULATION_LFM                            2

// rsspecan_rngPulseInputSignalSource
#define RSSPECAN_VAL_PULSE_INPUT_RF                                         0
#define RSSPECAN_VAL_PULSE_INPUT_FIQ                                        1

// rsspecan_rngPulseAcquisitionFilterType
#define RSSPECAN_VAL_PULSE_FILTER_TYPE_FLAT                                 0
#define RSSPECAN_VAL_PULSE_FILTER_TYPE_GAUSS                                1

// rsspecan_rngPulseDetectionReferenceSource
#define RSSPECAN_VAL_PULSE_DETECTION_REF_SOURCE_REFLEVEL                    0
#define RSSPECAN_VAL_PULSE_DETECTION_REF_SOURCE_PEAK                        1
#define RSSPECAN_VAL_PULSE_DETECTION_REF_SOURCE_NOISE                       2
#define RSSPECAN_VAL_PULSE_DETECTION_REF_SOURCE_ABSOLUTE                    3

// rsspecan_rngPulseReferencePointType
#define RSSPECAN_VAL_PULSE_REFERENCE_POINT_RISE                             0
#define RSSPECAN_VAL_PULSE_REFERENCE_POINT_CENTER                           1
#define RSSPECAN_VAL_PULSE_REFERENCE_POINT_FALL                             2

// rsspecan_rngPulseReferenceRangeType
#define RSSPECAN_VAL_PULSE_REFERENCE_RANGE_CENTER                           0
#define RSSPECAN_VAL_PULSE_REFERENCE_RANGE_EDGE                             1

// rsspecan_rngPulseTopLevelPosition
#define RSSPECAN_VAL_PULSE_TOP_LEVEL_POSITION_EDGE                          0
#define RSSPECAN_VAL_PULSE_TOP_LEVEL_POSITION_CENTER                        1

// rsspecan_rngPulseTopLevelAlgorithm
#define RSSPECAN_VAL_PULSE_TOP_LEVEL_ALGORITHM_MEAN                         0
#define RSSPECAN_VAL_PULSE_TOP_LEVEL_ALGORITHM_MEDIAN                       1
#define RSSPECAN_VAL_PULSE_TOP_LEVEL_ALGORITHM_HISTOGRAM                    2
#define RSSPECAN_VAL_PULSE_TOP_LEVEL_ALGORITHM_FIXED                        3

// rsspecan_rngPulseReferenceLevelUnit
#define RSSPECAN_VAL_PULSE_REFERENCE_LEVEL_UNIT_V                           0
#define RSSPECAN_VAL_PULSE_REFERENCE_LEVEL_UNIT_W                           1
#define RSSPECAN_VAL_PULSE_REFERENCE_LEVEL_UNIT_DBM                         2

// rsspecan_rngPulseResultRangeReference
#define RSSPECAN_VAL_PULSE_RESULT_RANGE_REFERENCE_RISE                      0
#define RSSPECAN_VAL_PULSE_RESULT_RANGE_REFERENCE_CENTER                    1
#define RSSPECAN_VAL_PULSE_RESULT_RANGE_REFERENCE_FALL                      2

// rsspecan_rngPulseResultRangeAlignment
#define RSSPECAN_VAL_PULSE_RESULT_RANGE_ALIGNMENT_LEFT                      0
#define RSSPECAN_VAL_PULSE_RESULT_RANGE_ALIGNMENT_CENTER                    1
#define RSSPECAN_VAL_PULSE_RESULT_RANGE_ALIGNMENT_RIGHT                     2

// rsspecan_rngPulseResultPhaseUnit
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_UNIT_DEG                            0
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_UNIT_RAD                            1

// rsspecan_rngSweepOptimization
#define RSSPECAN_VAL_SWEEP_OPTIMIZATION_AUTO                                0
#define RSSPECAN_VAL_SWEEP_OPTIMIZATION_DYNAMIC                             1
#define RSSPECAN_VAL_SWEEP_OPTIMIZATION_SPEED                               2

// rsspecan_rngNoiseCalcLimitResultType
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TYPE_GAIN                          0
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TYPE_NOISE                         1
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TYPE_PCOLD                         2
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TYPE_PHOT                          3
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TYPE_TEMP                          4
#define RSSPECAN_VAL_NOISE_CALC_LIM_LINE_TYPE_YFAC                          5

// rsspecan_rngPhaseSignalVerificationFailed
#define RSSPECAN_VAL_PHASE_SIGNAL_VERIFICATION_OFF                          0
#define RSSPECAN_VAL_PHASE_SIGNAL_VERIFICATION_ON                           1
#define RSSPECAN_VAL_PHASE_SIGNAL_VERIFICATION_AUTO                         2

// rsspecan_rngC2KPowerDisplay
#define RSSPECAN_VAL_C2K_CDP_DISPLAY_ABSOLUTE                               0
#define RSSPECAN_VAL_C2K_CDP_DISPLAY_RELATIVE                               1

// rsspecan_rngDataSetLoadMode
#define RSSPECAN_VAL_MEM_LOAD_MODE_NEW                                      0
#define RSSPECAN_VAL_MEM_LOAD_MODE_REPLACE                                  1

// rsspecan_rngDataSetStoreMode
#define RSSPECAN_VAL_MEM_STORE_MODE_INSTRUMENT                              0
#define RSSPECAN_VAL_MEM_STORE_MODE_CHANNEL                                 1

// rsspecan_rngWLANMIMOPayloadLengthSource
#define RSSPECAN_VAL_WLAN_MIMO_PAYLOAD_LENGTH_ESTIMATE                      0
#define RSSPECAN_VAL_WLAN_MIMO_PAYLOAD_LENGTH_HTSIGNAL                      1

// rsspecan_rngSequencerMode
#define RSSPECAN_VAL_SEQUENCER_MODE_SING                                    0
#define RSSPECAN_VAL_SEQUENCER_MODE_CONT                                    1
#define RSSPECAN_VAL_SEQUENCER_MODE_CDEF                                    2

// rsspecan_rngSpectrumEmissionMaskMSRBandCategory
#define RSSPECAN_VAL_SEM_MSR_BAND_CATEGORY_1                                1
#define RSSPECAN_VAL_SEM_MSR_BAND_CATEGORY_2                                2
#define RSSPECAN_VAL_SEM_MSR_BAND_CATEGORY_3                                3

// rsspecan_rngSpectrumEmissionMaskRangeStartFrequencyRelativeLimitFunction
#define RSSPECAN_VAL_SEM_REL_LIMIT_FUNC_OFF                                 0
#define RSSPECAN_VAL_SEM_REL_LIMIT_FUNC_MAX                                 1

// rsspecan_rngAssignMarkertoTraceRaisingFalling
#define RSSPECAN_VAL_MARKER_TRACE_OFF1                                      0
#define RSSPECAN_VAL_MARKER_TRACE_OFF2                                      1
#define RSSPECAN_VAL_MARKER_TRACE_FALLING1                                  2
#define RSSPECAN_VAL_MARKER_TRACE_FALLING2                                  3
#define RSSPECAN_VAL_MARKER_TRACE_RAISING1                                  4
#define RSSPECAN_VAL_MARKER_TRACE_RAISING2                                  5

// rsspecan_rngPresetOperatingMode
#define RSSPECAN_VAL_PRESET_MSRA                                            0
#define RSSPECAN_VAL_PRESET_SAN                                             1
#define RSSPECAN_VAL_PRESET_RTMS                                            2

// rsspecan_rngMCGDResultPhaseUnit
#define RSSPECAN_VAL_MCGD_RESULT_PHASE_UNIT_DEG                             0
#define RSSPECAN_VAL_MCGD_RESULT_PHASE_UNIT_RAD                             1

// rsspecan_rngMCGDCarrierOffsetMode
#define RSSPECAN_VAL_MCGD_CARRIER_OFFSET_ESTIMATED                          0
#define RSSPECAN_VAL_MCGD_CARRIER_OFFSET_FIXED                              1
#define RSSPECAN_VAL_MCGD_CARRIER_OFFSET_DCOM                               2

// rsspecan_rngLayoutQueryWindowType
#define RSSPECAN_VAL_LAYOUT_TYPE_DIAGRAM                                    0
#define RSSPECAN_VAL_LAYOUT_TYPE_MTABLE                                     1
#define RSSPECAN_VAL_LAYOUT_TYPE_PEAKLIST                                   2
#define RSSPECAN_VAL_LAYOUT_TYPE_RSUMMARY                                   3
#define RSSPECAN_VAL_LAYOUT_TYPE_SGRAM                                      4
#define RSSPECAN_VAL_LAYOUT_TYPE_MAGN                                       5
#define RSSPECAN_VAL_LAYOUT_TYPE_FREQ                                       6
#define RSSPECAN_VAL_LAYOUT_TYPE_VECT                                       7
#define RSSPECAN_VAL_LAYOUT_TYPE_RIMAG                                      8
#define RSSPECAN_VAL_LAYOUT_TYPE_MCAP                                       9
#define RSSPECAN_VAL_LAYOUT_TYPE_PRES                                       10
#define RSSPECAN_VAL_LAYOUT_TYPE_PST                                        11
#define RSSPECAN_VAL_LAYOUT_TYPE_PMAG                                       12
#define RSSPECAN_VAL_LAYOUT_TYPE_PPH                                        13
#define RSSPECAN_VAL_LAYOUT_TYPE_PFR                                        14
#define RSSPECAN_VAL_LAYOUT_TYPE_PTR                                        15
#define RSSPECAN_VAL_LAYOUT_TYPE_NOISE                                      16
#define RSSPECAN_VAL_LAYOUT_TYPE_GAIN                                       17
#define RSSPECAN_VAL_LAYOUT_TYPE_TEMP                                       18
#define RSSPECAN_VAL_LAYOUT_TYPE_YFAC                                       19
#define RSSPECAN_VAL_LAYOUT_TYPE_PHOT                                       20
#define RSSPECAN_VAL_LAYOUT_TYPE_PCOLD                                      21
#define RSSPECAN_VAL_LAYOUT_TYPE_RTAB                                       22
#define RSSPECAN_VAL_LAYOUT_TYPE_PNOISE                                     23
#define RSSPECAN_VAL_LAYOUT_TYPE_RNOISE                                     24
#define RSSPECAN_VAL_LAYOUT_TYPE_SNOISE                                     25
#define RSSPECAN_VAL_LAYOUT_TYPE_SPURS                                      26
#define RSSPECAN_VAL_LAYOUT_TYPE_GDEL                                       27
#define RSSPECAN_VAL_LAYOUT_TYPE_PHASE                                      28
#define RSSPECAN_VAL_LAYOUT_TYPE_REF_MAGN                                   29
#define RSSPECAN_VAL_LAYOUT_TYPE_REF_PHAS                                   30
#define RSSPECAN_VAL_LAYOUT_TYPE_SPECTRUM                                   31
#define RSSPECAN_VAL_LAYOUT_TYPE_BISTREAM                                   32
#define RSSPECAN_VAL_LAYOUT_TYPE_CMP_CONS                                   33
#define RSSPECAN_VAL_LAYOUT_TYPE_CDE_POW                                    34
#define RSSPECAN_VAL_LAYOUT_TYPE_CD_POWER                                   35
#define RSSPECAN_VAL_LAYOUT_TYPE_CMP_EVM                                    36
#define RSSPECAN_VAL_LAYOUT_TYPE_CH_TABLE                                   37
#define RSSPECAN_VAL_LAYOUT_TYPE_LST_EVAL                                   38
#define RSSPECAN_VAL_LAYOUT_TYPE_PCD_ERR                                    39
#define RSSPECAN_VAL_LAYOUT_TYPE_PWR_PCG                                    40
#define RSSPECAN_VAL_LAYOUT_TYPE_PWR_SYMB                                   41
#define RSSPECAN_VAL_LAYOUT_TYPE_SYM_CONS                                   42
#define RSSPECAN_VAL_LAYOUT_TYPE_SYM_EVM                                    43
#define RSSPECAN_VAL_LAYOUT_TYPE_CURRENT                                    44
#define RSSPECAN_VAL_LAYOUT_TYPE_PPWRAP                                     45
#define RSSPECAN_VAL_LAYOUT_TYPE_CBUFFER                                    46
#define RSSPECAN_VAL_LAYOUT_TYPE_MEAS                                       47
#define RSSPECAN_VAL_LAYOUT_TYPE_REFERENCE                                  48
#define RSSPECAN_VAL_LAYOUT_TYPE_EQUALIZER                                  49
#define RSSPECAN_VAL_LAYOUT_TYPE_EVECTOR                                    50
#define RSSPECAN_VAL_LAYOUT_TYPE_MACC                                       51
#define RSSPECAN_VAL_LAYOUT_TYPE_MERR                                       52
#define RSSPECAN_VAL_LAYOUT_TYPE_SYMBOLS                                    53
#define RSSPECAN_VAL_LAYOUT_TYPE_CMEMORY                                    54
#define RSSPECAN_VAL_LAYOUT_TYPE_CONSTEL                                    55
#define RSSPECAN_VAL_LAYOUT_TYPE_CVCARRIER                                  56
#define RSSPECAN_VAL_LAYOUT_TYPE_EVCARRIER                                  57
#define RSSPECAN_VAL_LAYOUT_TYPE_EVSYMBOL                                   58
#define RSSPECAN_VAL_LAYOUT_TYPE_FSPECTRUM                                  59
#define RSSPECAN_VAL_LAYOUT_TYPE_RSDETAIL                                   60
#define RSSPECAN_VAL_LAYOUT_TYPE_RSGLOBAL                                   61
#define RSSPECAN_VAL_LAYOUT_TYPE_SFIELD                                     62
#define RSSPECAN_VAL_LAYOUT_TYPE_SFLATNESS                                  63
#define RSSPECAN_VAL_LAYOUT_TYPE_PFPPDU                                     64
#define RSSPECAN_VAL_LAYOUT_TYPE_ASUM                                       65
#define RSSPECAN_VAL_LAYOUT_TYPE_BSTR                                       66
#define RSSPECAN_VAL_LAYOUT_TYPE_CCDF                                       67
#define RSSPECAN_VAL_LAYOUT_TYPE_FLAT                                       68
#define RSSPECAN_VAL_LAYOUT_TYPE_FSRS                                       69
#define RSSPECAN_VAL_LAYOUT_TYPE_EVSU                                       70
#define RSSPECAN_VAL_LAYOUT_TYPE_FEVS                                       71
#define RSSPECAN_VAL_LAYOUT_TYPE_IE                                         72
#define RSSPECAN_VAL_LAYOUT_TYPE_IEA                                        73
#define RSSPECAN_VAL_LAYOUT_TYPE_PSPE                                       74
#define RSSPECAN_VAL_LAYOUT_TYPE_EVRP                                       75
#define RSSPECAN_VAL_LAYOUT_TYPE_PVRP                                       76
#define RSSPECAN_VAL_LAYOUT_TYPE_PVRR                                       77
#define RSSPECAN_VAL_LAYOUT_TYPE_EVM_CHIP                                   78
#define RSSPECAN_VAL_LAYOUT_TYPE_FDRIFT                                     79
#define RSSPECAN_VAL_LAYOUT_TYPE_SRESULT                                    80
#define RSSPECAN_VAL_LAYOUT_TYPE_PDIS                                       81
#define RSSPECAN_VAL_LAYOUT_TYPE_PSP                                        82

// rsspecan_rngServiceBBSignalType
#define RSSPECAN_VAL_BB_SIGNAL_TYPE_OFF                                     0
#define RSSPECAN_VAL_BB_SIGNAL_TYPE_AC                                      1
#define RSSPECAN_VAL_BB_SIGNAL_TYPE_DC                                      2

// rsspecan_rngWLANPPDUFormat
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_FBUR                                  0
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_ALL                                   1
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_MNHT                                  2
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_DNHT                                  3

// rsspecan_rngWLANChannelBandwidth
#define RSSPECAN_VAL_WLAN_BANDWIDTH_FBUR                                    0
#define RSSPECAN_VAL_WLAN_BANDWIDTH_ALL                                     1
#define RSSPECAN_VAL_WLAN_BANDWIDTH_MB5                                     2
#define RSSPECAN_VAL_WLAN_BANDWIDTH_MB10                                    3
#define RSSPECAN_VAL_WLAN_BANDWIDTH_MB20                                    4
#define RSSPECAN_VAL_WLAN_BANDWIDTH_DB5                                     5
#define RSSPECAN_VAL_WLAN_BANDWIDTH_DB10                                    6
#define RSSPECAN_VAL_WLAN_BANDWIDTH_DB20                                    7

// rsspecan_rngWLANPSDUModulation
#define RSSPECAN_VAL_WLAN_MODULATION_BPSK6                                  0
#define RSSPECAN_VAL_WLAN_MODULATION_BPSK9                                  1
#define RSSPECAN_VAL_WLAN_MODULATION_QPSK12                                 2
#define RSSPECAN_VAL_WLAN_MODULATION_QPSK18                                 3
#define RSSPECAN_VAL_WLAN_MODULATION_QAM1624                                4
#define RSSPECAN_VAL_WLAN_MODULATION_QAM1636                                5
#define RSSPECAN_VAL_WLAN_MODULATION_QAM6448                                6
#define RSSPECAN_VAL_WLAN_MODULATION_QAM6454                                7

// rsspecan_rngWLANPSDUModulation802_11bg
#define RSSPECAN_VAL_WLAN_MODULATION_CCK11                                  0
#define RSSPECAN_VAL_WLAN_MODULATION_CCK55                                  1
#define RSSPECAN_VAL_WLAN_MODULATION_DBPSK1                                 2
#define RSSPECAN_VAL_WLAN_MODULATION_DQPSK2                                 3
#define RSSPECAN_VAL_WLAN_MODULATION_PBCC11                                 4
#define RSSPECAN_VAL_WLAN_MODULATION_PBCC22                                 5
#define RSSPECAN_VAL_WLAN_MODULATION_PBCC55                                 6

// rsspecan_rngWLANPPDUFormat802_11n
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_MMIX                                  2
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_MGRF                                  3
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_DMIX                                  4
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_DGRF                                  5

// rsspecan_rngWLANChannelBandwidth802_11n
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11N_FBUR                            0
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11N_ALL                             1
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11N_MB20                            2
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11N_MB40                            3
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11N_DB20                            4
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11N_DB40                            5

// rsspecan_rngWLANGuardIntervalLength
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_FBUR                               0
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_ALL                                1
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_MS                                 2
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_ML                                 3
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_DS                                 4
#define RSSPECAN_VAL_WLAN_GUARD_INTERVAL_DL                                 5

// rsspecan_rngWLANPPDUFormat802_11ac
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_MVHT                                  2
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_DVHT                                  3

// rsspecan_rngWLANPPDUFormat802_11bg
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_FAMA                                  0
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_FMMA                                  1
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_FAMM                                  2
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_FMMM                                  3
#define RSSPECAN_VAL_WLAN_PPDU_FORMAT_FMMD                                  4

// rsspecan_rngWLANChannelBandwidth802_11ac
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_FBUR                           0
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_ALL                            1
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_MB20                           2
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_MB40                           3
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_MB80                           4
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_MB160                          5
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_DB20                           6
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_DB40                           7
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_DB80                           8
#define RSSPECAN_VAL_WLAN_BANDWIDTH_802_11AC_DB160                          9

// rsspecan_rngWLANNSTSIndexMode
#define RSSPECAN_VAL_WLAN_NSTS_INDEX_FBUR                                   0
#define RSSPECAN_VAL_WLAN_NSTS_INDEX_ALL                                    1
#define RSSPECAN_VAL_WLAN_NSTS_INDEX_MEAS                                   2
#define RSSPECAN_VAL_WLAN_NSTS_INDEX_DEMOD                                  3

// rsspecan_rngTraceExportAll
#define RSSPECAN_TRAC_EXPORT_MODE_SINGLE                                    0
#define RSSPECAN_TRAC_EXPORT_MODE_ALL                                       1

// rsspecan_rngMSRTxTechnolgy
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_GSM                                  0
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_WCDMA                                1
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_LTE_1_40                             2
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_LTE_3_00                             3
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_LTE_5_00                             4
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_LTE_10_00                            5
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_LTE_15_00                            6
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_LTE_20_00                            7
#define RSSPECAN_VAL_MSR_TX_TECHNOLOGY_USER                                 8

// rsspecan_rngLTEDownlinkPHICHNgParameter
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_AUTO                             0
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_CUSTOM                           1
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_1_6                              2
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_1_2                              3
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_1                                4
#define RSSPECAN_VAL_LTE_DOWNLINK_PHICH_NG_2                                5

// rsspecan_rngLTEUplinkPRACHSequenceIndex
#define RSSPECAN_VAL_LTE_UPLINK_PRACH_SEQ_INDEX_AUTO                        1

// rsspecan_rngMSRADataEvaluation
#define RSSPECAN_VAL_MSRA_EVAL_TIME_DOM                                     0
#define RSSPECAN_VAL_MSRA_EVAL_FREQUENCY_DOM                                1
#define RSSPECAN_VAL_MSRA_EVAL_IQ_DATA                                      2

// rsspecan_rngTransducerSetUnit
#define RSSPECAN_VAL_TSET_UNIT_DB                                           0
#define RSSPECAN_VAL_TSET_UNIT_DBM                                          1
#define RSSPECAN_VAL_TSET_UNIT_DBUV                                         2
#define RSSPECAN_VAL_TSET_UNIT_DBUVM                                        3
#define RSSPECAN_VAL_TSET_UNIT_DBUA                                         4
#define RSSPECAN_VAL_TSET_UNIT_DBUAM                                        5
#define RSSPECAN_VAL_TSET_UNIT_DBPW                                         6
#define RSSPECAN_VAL_TSET_UNIT_DBPT                                         7

// rsspecan_rngPulseParameterPower
#define RSSPECAN_VAL_PULSE_RESULT_POWER_TOP                                 1
#define RSSPECAN_VAL_PULSE_RESULT_POWER_BASE                                2
#define RSSPECAN_VAL_PULSE_RESULT_POWER_AMPL                                3
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ON                                  4
#define RSSPECAN_VAL_PULSE_RESULT_POWER_AVG                                 5
#define RSSPECAN_VAL_PULSE_RESULT_POWER_MIN                                 6
#define RSSPECAN_VAL_PULSE_RESULT_POWER_MAX                                 7
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PON                                 8
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PAVG                                9
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PMIN                                10
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ADP                                 11
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ADDB                                12
#define RSSPECAN_VAL_PULSE_RESULT_POWER_RPER                                13
#define RSSPECAN_VAL_PULSE_RESULT_POWER_RDB                                 14
#define RSSPECAN_VAL_PULSE_RESULT_POWER_OPER                                15
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ODB                                 16
#define RSSPECAN_VAL_PULSE_RESULT_POWER_POIN                                17
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PPR                                 18

// rsspecan_rngPulseParameterTiming
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_TST                                1
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_SETT                               2
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_RISE                               3
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_FALL                               4
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_PWID                               5
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_OFF                                6
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_DRAT                               7
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_DCYC                               8
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_PRI                                9
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_PRF                                10

// rsspecan_rngPulseParameterFrequency
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_POIN                                 1
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_PPFR                                 2
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_RERR                                 3
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_PERR                                 4
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_DEV                                  5
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_CRAT                                 6

// rsspecan_rngPulseParameterPhase
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_POIN                                1
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_PPPH                                2
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_RERR                                3
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_PERR                                4
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_DEV                                 5

// rsspecan_rngVSAModulationOptimization
#define RSSPECAN_VAL_VSA_MOD_OPTIMIZATION_RMS                               0
#define RSSPECAN_VAL_VSA_MOD_OPTIMIZATION_EVM                               1

// rsspecan_rngLTEDownlinkMeasurement
#define RSSPECAN_VAL_LTE_DOWNLINK_MEAS_ACLR                                 0
#define RSSPECAN_VAL_LTE_DOWNLINK_MEAS_ESP                                  1
#define RSSPECAN_VAL_LTE_DOWNLINK_MEAS_EVM                                  2
#define RSSPECAN_VAL_LTE_DOWNLINK_MEAS_TAER                                 3

// rsspecan_rngLTEDownlinkCSIRSAntena
#define RSSPECAN_VAL_LTE_DOWNLINK_TX1                                       1
#define RSSPECAN_VAL_LTE_DOWNLINK_TX2                                       2
#define RSSPECAN_VAL_LTE_DOWNLINK_TX3                                       3
#define RSSPECAN_VAL_LTE_DOWNLINK_TX4                                       4

// rsspecan_rngWLANBurstUnit
#define RSSPECAN_VAL_BURST_UNIT_SYMBOL                                      0
#define RSSPECAN_VAL_BURST_UNIT_SAMPLE                                      1

// rsspecan_rngRealtimeMeasurement
#define RSSPECAN_VAL_REALTIME_HIGH_RESOLUTION                               0
#define RSSPECAN_VAL_REALTIME_MULTI_DOMAIN                                  1
/* END GENERATE */

/*****************************************************************************
 *- Hidden Attribute Declarations -------------------------------------------*
 *****************************************************************************/

#define RSSPECAN_ATTR_OPC_TIMEOUT               (RS_ATTR_OPC_TIMEOUT)           /* ViInt32 */
#define RSSPECAN_ATTR_IO_SESSION                (RS_ATTR_IO_SESSION)            /* ViSession */
#define RSSPECAN_ATTR_OPC_CALLBACK              (RS_ATTR_OPC_CALLBACK)          /* ViAddr */
#define RSSPECAN_ATTR_CHECK_STATUS_CALLBACK     (RS_ATTR_CHECK_STATUS_CALLBACK) /* ViAddr */

/****************************************************************************
 *------------------------ Attribute Value Defines -------------------------*
 ****************************************************************************/

    /*- Defined values for RSSPECAN_ATTR_TRACE_TYPE --------------------------*/

#define RSSPECAN_VAL_TRACE_TYPE_CLEAR_WRITE                         (1L)
#define RSSPECAN_VAL_TRACE_TYPE_MAX_HOLD                            (2L)
#define RSSPECAN_VAL_TRACE_TYPE_MIN_HOLD                            (3L)
#define RSSPECAN_VAL_TRACE_TYPE_VIDEO_AVERAGE                       (4L)
#define RSSPECAN_VAL_TRACE_TYPE_VIEW                                (5L)
#define RSSPECAN_VAL_TRACE_TYPE_STORE                               (6L)

#define RSSPECAN_VAL_TRACE_TYPE_CLASS_EXT_BASE                      (500L)
#define RSSPECAN_VAL_TRACE_TYPE_SPECIFIC_EXT_BASE                   (1000L)


    /*- Defined values for RSSPECAN_ATTR_VERTICAL_SCALE ----------------------*/

#define RSSPECAN_VAL_VERTICAL_SCALE_LINEAR                          (1L)
#define RSSPECAN_VAL_VERTICAL_SCALE_LOGARITHMIC                     (2L)

#define RSSPECAN_VAL_VERTICAL_SCALE_CLASS_EXT_BASE                  (500L)
#define RSSPECAN_VAL_VERTICAL_SCALE_SPECIFIC_EXT_BASE               (1000L)


    /*- Defined values for RSSPECAN_ATTR_MARKER_TYPE -------------------------*/

#define RSSPECAN_VAL_MARKER_TYPE_NORMAL                             (1L)
#define RSSPECAN_VAL_MARKER_TYPE_DELTA                              (2L)

#define RSSPECAN_VAL_MARKER_TYPE_CLASS_EXT_BASE                     (500L)
#define RSSPECAN_VAL_MARKER_TYPE_SPECIFIC_EXT_BASE                  (1000L)


    /*- Defined values for RSSPECAN_ATTR_TRIGGER_SOURCE ----------------------*/

#define RSSPECAN_VAL_TRIGGER_SOURCE_EXTERNAL                        (1L)
#define RSSPECAN_VAL_TRIGGER_SOURCE_IMMEDIATE                       (2L)
#define RSSPECAN_VAL_TRIGGER_SOURCE_SOFTWARE                        (3L)
#define RSSPECAN_VAL_TRIGGER_SOURCE_AC_LINE                         (4L)
#define RSSPECAN_VAL_TRIGGER_SOURCE_VIDEO                           (5L)

#define RSSPECAN_VAL_TRIGGER_SOURCE_CLASS_EXT_BASE                  (500L)
#define RSSPECAN_VAL_TRIGGER_SOURCE_SPECIFIC_EXT_BASE               (1000L)


    /*- Defined values for RSSPECAN_ATTR_EXTERNAL_TRIGGER_SLOPE --------------*/

#define RSSPECAN_VAL_EXTERNAL_TRIGGER_SLOPE_POSITIVE                (1L)
#define RSSPECAN_VAL_EXTERNAL_TRIGGER_SLOPE_NEGATIVE                (2L)

#define RSSPECAN_VAL_EXTERNAL_TRIGGER_SLOPE_CLASS_EXT_BASE          (500L)
#define RSSPECAN_VAL_EXTERNAL_TRIGGER_SLOPE_SPECIFIC_EXT_BASE       (1000L)

    /*- Defined values for RSSPECAN_ATTR_VIDEO_TRIGGER_SLOPE --------------*/

#define RSSPECAN_VAL_VIDEO_TRIGGER_SLOPE_POSITIVE                   (1L)
#define RSSPECAN_VAL_VIDEO_TRIGGER_SLOPE_NEGATIVE                   (2L)

#define RSSPECAN_VAL_VIDEO_TRIGGER_SLOPE_CLASS_EXT_BASE             (500L)
#define RSSPECAN_VAL_VIDEO_TRIGGER_SLOPE_SPECIFIC_EXT_BASE          (1000L)


    /*- Defined values for rsspecan_ReadXYTrace & rsspecan_ReadYTrace ----------*/

#define RSSPECAN_VAL_MAX_TIME_IMMEDIATE                             (0x0L)
#define RSSPECAN_VAL_MAX_TIME_INFINITE                              (0xFFFFFFFFUL)

    /*- Defined values for rsspecan_AcquisitionStatus ------------------------*/

#define RSSPECAN_VAL_ACQUISITION_STATUS_COMPLETE                    (1L)
#define RSSPECAN_VAL_ACQUISITION_STATUS_IN_PROGRESS                 (0L)
#define RSSPECAN_VAL_ACQUISITION_STATUS_UNKNOWN                     (-1L)

#define RSSPECAN_VAL_ACQUISITION_STATUS_CLASS_EXT_BASE              (500L)
#define RSSPECAN_VAL_ACQUISITION_STATUS_SPECIFIC_EXT_BASE           (1000L)


    /*- Defined values for rsspecan_MarkerSearch -----------------------------*/

#define RSSPECAN_VAL_MARKER_SEARCH_HIGHEST                          (1L)
#define RSSPECAN_VAL_MARKER_SEARCH_MINIMUM                          (2L)
#define RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK                        (3L)
#define RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_LEFT                   (4L)
#define RSSPECAN_VAL_MARKER_SEARCH_NEXT_PEAK_RIGHT                  (5L)
#define RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM                     (6L)
#define RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_LEFT                (7L)
#define RSSPECAN_VAL_MARKER_SEARCH_NEXT_MINIMUM_RIGHT               (8L)

#define RSSPECAN_VAL_MARKER_SEARCH_CLASS_EXT_BASE                   (500L)
#define RSSPECAN_VAL_MARKER_SEARCH_SPECIFIC_EXT_BASE                (1000L)


    /*- Defined values for rsspecan_SetInstrumentFromMarker -----------------------*/

#define RSSPECAN_VAL_INSTRUMENT_SETTING_FREQUENCY_CENTER            (1L)
#define RSSPECAN_VAL_INSTRUMENT_SETTING_FREQUENCY_STEP              (2L)
#define RSSPECAN_VAL_INSTRUMENT_SETTING_REFERENCE_LEVEL             (3L)

#define RSSPECAN_VAL_INSTRUMENT_SETTING_CLASS_EXT_BASE              (500L)
#define RSSPECAN_VAL_INSTRUMENT_SETTING_SPECIFIC_EXT_BASE           (1000L)


    /*- Instrument specific attribute value definitions --------------------*/


/****************************************************************************
 *---------------- Constant definition  ------------------------------------*
 ****************************************************************************/

#define RSSPECAN_VAL_COUPLING_RBW        0
#define RSSPECAN_VAL_COUPLING_VBW        1
#define RSSPECAN_VAL_COUPLING_SWEEP_TIME 2

#define RSSPECAN_VAL_COUPLING_RATIO_RBW 0
#define RSSPECAN_VAL_COUPLING_RATIO_VBW 1

#define RSSPECAN_VAL_LIMIT_LOWER   0
#define RSSPECAN_VAL_LIMIT_UPPER   1
#define RSSPECAN_VAL_LIMIT_CONTROL 2
#define RSSPECAN_VAL_LIMIT_MOVE_OFFSET 0
#define RSSPECAN_VAL_LIMIT_MOVE_SHIFT  1

#define RSSPECAN_VAL_MEASTYPE_NONE  0
#define RSSPECAN_VAL_MEASTYPE_MIN   1
#define RSSPECAN_VAL_MEASTYPE_MAX   2
#define RSSPECAN_VAL_MEASTYPE_PEAK  3
#define RSSPECAN_VAL_MEASTYPE_AVER  4


#define RSSPECAN_VAL_MEASTYPE_MAXMIN    0
#define RSSPECAN_VAL_MEASTYPE_MAXMAX    1
#define RSSPECAN_VAL_MEASTYPE_MAXAVER   2
#define RSSPECAN_VAL_MEASTYPE_AVERMIN   3
#define RSSPECAN_VAL_MEASTYPE_AVERMAX   4

#define RSSPECAN_VAL_POW_CONT_MODE_REL  0
#define RSSPECAN_VAL_POW_CONT_MODE_ABS  1

#define RSSPECAN_VAL_DATE       0

#define RSSPECAN_VAL_EATT_OFF   0
#define RSSPECAN_VAL_EATT_AUTO  1
#define RSSPECAN_VAL_EATT_MAN   2

#define RSSPECAN_VAL_LINE_THR           0
#define RSSPECAN_VAL_LINE_DISPLAY       1
#define RSSPECAN_VAL_LINE_FREQ          2
#define RSSPECAN_VAL_LINE_TIME          3

#define RSSPECAN_VAL_ACP_TX         0
#define RSSPECAN_VAL_ACP_ADJ        1
#define RSSPECAN_VAL_ACP_ALT        2

#define RSSPECAN_VAL_SSTAT_OFF      0
#define RSSPECAN_VAL_SSTAT_APD      1
#define RSSPECAN_VAL_SSTAT_CCDF     2

#define RSSPECAN_VAL_SSTAT_PROB_0_1         0
#define RSSPECAN_VAL_SSTAT_PROB_1           1
#define RSSPECAN_VAL_SSTAT_PROB_10          2

#define RSSPECAN_VAL_SSTAT_RESULT_MEAN      0
#define RSSPECAN_VAL_SSTAT_RESULT_PEAK      1
#define RSSPECAN_VAL_SSTAT_RESULT_CREST     2

#define RSSPECAN_VAL_TDOM_PEAK            0
#define RSSPECAN_VAL_TDOM_RMS             1
#define RSSPECAN_VAL_TDOM_MEAN            2
#define RSSPECAN_VAL_TDOM_SDEV            3

#define RSSPECAN_VAL_TDOM_RESULT              0
#define RSSPECAN_VAL_TDOM_AVG_RESULT          1
#define RSSPECAN_VAL_TDOM_MAX_HOLD_RESULT     2

#define RSSPECAN_VAL_BURS_MEAS_MEAN      0
#define RSSPECAN_VAL_BURS_MEAS_PEAK      1

#define RSSPECAN_VAL_FILE_ITEMS_NONE            0
#define RSSPECAN_VAL_FILE_ITEMS_DEFAULT         1
#define RSSPECAN_VAL_FILE_ITEMS_ALL             2
#define RSSPECAN_VAL_FILE_ITEMS_HWSETTINGS      3
#define RSSPECAN_VAL_FILE_ITEMS_ALLTRACES       4
#define RSSPECAN_VAL_FILE_ITEMS_ALLLINES        5
#define RSSPECAN_VAL_FILE_ITEMS_SOUR_CAL_DATA   6
#define RSSPECAN_VAL_FILE_ITEMS_ALLTRAN         7
#define RSSPECAN_VAL_FILE_ITEMS_SPECTROGRAM     8
#define RSSPECAN_VAL_FILE_ITEMS_CLIST           9
#define RSSPECAN_VAL_FILE_ITEMS_FINAL           10
#define RSSPECAN_VAL_FILE_ITEMS_IQ_DATA         11

#define RSSPECAN_VAL_FILE_NEW    0
#define RSSPECAN_VAL_FILE_COPY   1
#define RSSPECAN_VAL_FILE_RENAME 2
#define RSSPECAN_VAL_FILE_DELETE 3
#define RSSPECAN_VAL_FILE_RDIR   4
#define RSSPECAN_VAL_FILE_FORMAT 5
#define RSSPECAN_VAL_FILE_CDIR   6
#define RSSPECAN_VAL_FILE_CDISC  7
#define RSSPECAN_VAL_FILE_DELETE_IMM    8

#define RSSPECAN_VAL_MEM_SAVE    0
#define RSSPECAN_VAL_MEM_RECALL  1
#define RSSPECAN_VAL_MEM_DELETE  2
#define RSSPECAN_VAL_MEM_STARTUP 3

#define RSSPECAN_VAL_HCOPY_DEST_BMP  0
#define RSSPECAN_VAL_HCOPY_DEST_WMF  1
#define RSSPECAN_VAL_HCOPY_DEST_EMF  2
#define RSSPECAN_VAL_HCOPY_DEST_CLP  3
#define RSSPECAN_VAL_HCOPY_DEST_PRN  4
#define RSSPECAN_VAL_HCOPY_DEST_JPG  5
#define RSSPECAN_VAL_HCOPY_DEST_PNG  6

#define RSSPECAN_VAL_HCOP_ALL   0
#define RSSPECAN_VAL_HCOP_TABLE 1
#define RSSPECAN_VAL_HCOP_TRACE 2

#define RSSPECAN_VAL_FORMAT_ASC  0
#define RSSPECAN_VAL_FORMAT_REAL 1

#define RSSPECAN_VAL_TABLE_OFF          0
#define RSSPECAN_VAL_TABLE_SELECT       1
#define RSSPECAN_VAL_TABLE_NEW          2
#define RSSPECAN_VAL_TABLE_DELETE       3
#define RSSPECAN_VAL_TABLE_COPY         4

#define RSSPECAN_VAL_USER_PORT_INP      0
#define RSSPECAN_VAL_USER_PORT_OUT      1

#define RSSPECAN_VAL_X  0
#define RSSPECAN_VAL_Y  1

//TV trigger Sync
#define RSSPECAN_VAL_TRIG_TV_HORIZONTAL 3

/* B10 - External Generator */
#define RSSPECAN_VAL_TGEN_CAL_TRAN       0
#define RSSPECAN_VAL_TGEN_CAL_REFLO      1
#define RSSPECAN_VAL_TGEN_CAL_REFLS      2

#define RSSPECAN_VAL_TGEN_EXT_AM         0
#define RSSPECAN_VAL_TGEN_EXT_FM         1
#define RSSPECAN_VAL_TGEN_EXT_IQ         2


/* K5 - GSM/EDGE MS/BS Test - constants  */
#define RSSPECAN_VAL_GSM_MEAS_PFER      0
#define RSSPECAN_VAL_GSM_MEAS_MACC      1
#define RSSPECAN_VAL_GSM_MEAS_CPW       2
#define RSSPECAN_VAL_GSM_MEAS_PVT       3
#define RSSPECAN_VAL_GSM_MEAS_SPEC_MOD  4
#define RSSPECAN_VAL_GSM_MEAS_SPEC_TRA  5
#define RSSPECAN_VAL_GSM_MEAS_SPUR      6

#define RSSPECAN_VAL_GSM_TRIG_FRUN      0
#define RSSPECAN_VAL_GSM_TRIG_EXT       1
#define RSSPECAN_VAL_GSM_TRIG_IFP       2
#define RSSPECAN_VAL_GSM_TRIG_RFP       3


#define RSSPECAN_VAL_GSM_PERR_RMS       0
#define RSSPECAN_VAL_GSM_PERR_PEAK      1
#define RSSPECAN_VAL_GSM_FERR           2
#define RSSPECAN_VAL_GSM_IQOF           3
#define RSSPECAN_VAL_GSM_IQIM           4

#define RSSPECAN_VAL_GSM_MACC_RMS       0
#define RSSPECAN_VAL_GSM_MACC_PEAK      1
#define RSSPECAN_VAL_GSM_MACC_OSUP      2
#define RSSPECAN_VAL_GSM_MACC_PERC      3
#define RSSPECAN_VAL_GSM_MACC_FREQ      4
#define RSSPECAN_VAL_GSM_MACC_IQOF      5
#define RSSPECAN_VAL_GSM_MACC_IQIM      6

#define RSSPECAN_VAL_GSM_USER           8

#define RSSPECAN_VAL_GSM_PVT_TRGS      0


/* K7 - FM Demodulator - constants  */


#define RSSPECAN_VAL_FMDEM_AM         0
#define RSSPECAN_VAL_FMDEM_FM         1
#define RSSPECAN_VAL_FMDEM_PM         2
#define RSSPECAN_VAL_FMDEM_AFSAM      3
#define RSSPECAN_VAL_FMDEM_AFSFM      4
#define RSSPECAN_VAL_FMDEM_AFSPM      5
#define RSSPECAN_VAL_FMDEM_RF         6
#define RSSPECAN_VAL_FMDEM_AMREL      7
#define RSSPECAN_VAL_FMDEM_AFSAMREL   8

#define RSSPECAN_VAL_FMDEM_RESULT_OFF  0
#define RSSPECAN_VAL_FMDEM_RESULT_CURR 1
#define RSSPECAN_VAL_FMDEM_RESULT_AVG  2
#define RSSPECAN_VAL_FMDEM_RESULT_MAX  3
#define RSSPECAN_VAL_FMDEM_RESULT_MIN  4
#define RSSPECAN_VAL_FMDEM_RESULT_VIEW 5

#define RSSPECAN_VAL_FMDEM_FILTER_HP     0
#define RSSPECAN_VAL_FMDEM_FILTER_LP     1
#define RSSPECAN_VAL_FMDEM_FILTER_DEEMP  2
#define RSSPECAN_VAL_FMDEM_FILTER_LP_REL 3

#define RSSPECAN_VAL_FMDEM_COUP_DC 0
#define RSSPECAN_VAL_FMDEM_COUP_AC 1

#define RSSPECAN_VAL_FMDEM_RLEN  0
#define RSSPECAN_VAL_FMDEM_SRATE 1

#define RSSPECAN_VAL_FMDEM_IMM 0
#define RSSPECAN_VAL_FMDEM_AVG 1

#define RSSPECAN_VAL_FMDEM_DET_PPE 0
#define RSSPECAN_VAL_FMDEM_DET_MPE 1
#define RSSPECAN_VAL_FMDEM_DET_MID 2
#define RSSPECAN_VAL_FMDEM_DET_RMS 3

#define RSSPECAN_VAL_FMDEM_MEAS_AFR 0
#define RSSPECAN_VAL_FMDEM_MEAS_FER 1
#define RSSPECAN_VAL_FMDEM_MEAS_SIN 2
#define RSSPECAN_VAL_FMDEM_MEAS_THD 3
#define RSSPECAN_VAL_FMDEM_MEAS_CAR 4

#define RSSPECAN_VAL_FM_FEED_AM_REL         0
#define RSSPECAN_VAL_FM_FEED_AM             1
#define RSSPECAN_VAL_FM_FEED_SPEC           2
#define RSSPECAN_VAL_FM_FEED_FM             3
#define RSSPECAN_VAL_FM_FEED_PM             4
#define RSSPECAN_VAL_FM_FEED_AMS            5
#define RSSPECAN_VAL_FM_FEED_AMS_REL        6
#define RSSPECAN_VAL_FM_FEED_FMS            7
#define RSSPECAN_VAL_FM_FEED_PMS            8
#define RSSPECAN_VAL_FM_FEED_FM_AFSP        9
#define RSSPECAN_VAL_FM_FEED_PM_AFSP        10
#define RSSPECAN_VAL_FM_FEED_AM_REL_AFSP    11
#define RSSPECAN_VAL_FM_FEED_RFP_AFSP       12
#define RSSPECAN_VAL_FSM_FEED_SUMM          13

/* K9 Power Meter - constants */
#define RSSPECAN_VAL_PWMET_MEASTIME_NORM    0
#define RSSPECAN_VAL_PWMET_MEASTIME_SHORT   1
#define RSSPECAN_VAL_PWMET_MEASTIME_LONG    2
#define RSSPECAN_VAL_PWMET_MEASTIME_MANUAL 3

/* K30 - Noise Figure - constants  */
#define RSSPECAN_VAL_NOISE_DUT_MODE_DIRECT        0
#define RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_IF_PLUS 1
#define RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_IF_ABS  2
#define RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_LO_PLUS 3
#define RSSPECAN_VAL_NOISE_DUT_MODE_FIXED_LO_ABS  4

#define RSSPECAN_VAL_NOISE_SECOND_STAGE_CORR_MEAS  0
#define RSSPECAN_VAL_NOISE_CONT_FREQ_LIST_MEAS     1
#define RSSPECAN_VAL_NOISE_SINGLE_FREQ_LIST_MEAS   2
#define RSSPECAN_VAL_NOISE_SINGLE_FREQ_MEAS        3
#define RSSPECAN_VAL_NOISE_CONT_FREQ_SINGLE_MEAS   4
#define RSSPECAN_VAL_NOISE_SINGLE_FREQ_SINGLE_MEAS 5

#define RSSPECAN_VAL_DISP_FORMAT_SING             0
#define RSSPECAN_VAL_DISP_FORMAT_SPL              1

#define RSSPECAN_VAL_NOISE_DISP_DATA_OFF          0

#define RSSPECAN_VAL_NOISE_NOISE                1
#define RSSPECAN_VAL_NOISE_GAIN                 2

#define RSSPECAN_VAL_NOISE_RESULT_TYPE_GAIN  0
#define RSSPECAN_VAL_NOISE_RESULT_TYPE_NOISE 1
#define RSSPECAN_VAL_NOISE_RESULT_TYPE_PCOLD 2
#define RSSPECAN_VAL_NOISE_RESULT_TYPE_PHOT  3
#define RSSPECAN_VAL_NOISE_RESULT_TYPE_TEMP  4
#define RSSPECAN_VAL_NOISE_RESULT_TYPE_YFAC  5

/* K40 - Phase Noise - constants  */
#define RSSPECAN_VAL_PHASE_RFM 0
#define RSSPECAN_VAL_PHASE_RPM 1
#define RSSPECAN_VAL_PHASE_RMS 2

/* K90/91 - WLAN constants  */
#define RSSPECAN_VAL_WLAN_GATE_TIME      0
#define RSSPECAN_VAL_WLAN_GATE_SAMPLE    1

#define RSSPECAN_VAL_WLAN_EVM_MODE_SYMBOL    0
#define RSSPECAN_VAL_WLAN_EVM_MODE_CARRIER   1
#define RSSPECAN_VAL_WLAN_EVM_MODE_PHAS      2
#define RSSPECAN_VAL_WLAN_EVM_MODE_FREQ      3

#define RSSPECAN_VAL_WLAN_SPEC_MODE_IEEE     0
#define RSSPECAN_VAL_WLAN_SPEC_MODE_ETSI     1
#define RSSPECAN_VAL_WLAN_SPEC_MODE_FFT      2
#define RSSPECAN_VAL_WLAN_SPEC_MODE_ACPR     3
#define RSSPECAN_VAL_WLAN_SPEC_MODE_FLAT     4

#define RSSPECAN_VAL_WLAN_CONS_SYM       0
#define RSSPECAN_VAL_WLAN_CONS_CARR      1

#define RSSPECAN_VAL_WLAN_STAT_CCDF     0
#define RSSPECAN_VAL_WLAN_STAT_BSTR     1
#define RSSPECAN_VAL_WLAN_STAT_SFI      2

#define RSSPECAN_VAL_WLAN_POS_X          0
#define RSSPECAN_VAL_WLAN_POS_Y          1

#define RSSPECAN_VAL_WLAN_MARK_MIN       0
#define RSSPECAN_VAL_WLAN_MARK_MAX       1

#define RSSPECAN_VAL_WLAN_DEM_DUR_MS     0
#define RSSPECAN_VAL_WLAN_DEM_DUR_BYTE   1

#define RSSPECAN_VAL_WLAN_LIM_FERR      0
#define RSSPECAN_VAL_WLAN_LIM_SYMB      1
#define RSSPECAN_VAL_WLAN_LIM_IQOFF     2
#define RSSPECAN_VAL_WLAN_LIM_EVM_ALL   3
#define RSSPECAN_VAL_WLAN_LIM_EVM_DATA  4
#define RSSPECAN_VAL_WLAN_LIM_EVM_PIL   5
#define RSSPECAN_VAL_WLAN_LIM_RTIME     6
#define RSSPECAN_VAL_WLAN_LIM_FTIME     7
#define RSSPECAN_VAL_WLAN_LIM_EVM       8


#define RSSPECAN_VAL_WLAN_ACP_ADJ       0
#define RSSPECAN_VAL_WLAN_ACP_ALT       1

#define RSSPECAN_VAL_POW                1

#define RSSPECAN_VAL_ACP_CURR           0
#define RSSPECAN_VAL_ACP_MAX            1

#define RSSPECAN_VAL_WLAN_MEAS_PREA       0
#define RSSPECAN_VAL_WLAN_MEAS_PAYL       1
#define RSSPECAN_VAL_WLAN_MEAS_RMS_MAX    2
#define RSSPECAN_VAL_WLAN_MEAS_RMS_MIN    3
#define RSSPECAN_VAL_WLAN_MEAS_RMS_AVER   4
#define RSSPECAN_VAL_WLAN_MEAS_PEAK       5
#define RSSPECAN_VAL_WLAN_MEAS_CREST_MAX  6
#define RSSPECAN_VAL_WLAN_MEAS_CREST_MIN  7
#define RSSPECAN_VAL_WLAN_MEAS_CREST_AVER 8

#define RSSPECAN_VAL_WLAN_TIME_FALL     0
#define RSSPECAN_VAL_WLAN_TIME_RISE     1

#define RSSPECAN_VAL_WLAN_ERR_FREQ      0
#define RSSPECAN_VAL_WLAN_ERR_SYMB      1

#define RSSPECAN_VAL_WLAN_IQ_OFFS       0
#define RSSPECAN_VAL_WLAN_IQ_IMBA       1
#define RSSPECAN_VAL_WLAN_IQ_QUAD       2

#define RSSPECAN_VAL_WLAN_IQ_I          0
#define RSSPECAN_VAL_WLAN_IQ_Q          1
#define RSSPECAN_VAL_WLAN_IQ_IQ         2

#define RSSPECAN_VAL_WLAN_EVM_ALL       0
#define RSSPECAN_VAL_WLAN_EVM_DATA      1
#define RSSPECAN_VAL_WLAN_EVM_PILOT     2
#define RSSPECAN_VAL_WLAN_EVM           3
#define RSSPECAN_VAL_WLAN_EVM_PEAK      4

#define RSSPECAN_VAL_WLAN_BURS          0
#define RSSPECAN_VAL_WLAN_SYMB          1

#define RSSPECAN_VAL_SELECTION_NUM    2

/* K92 - WiMax constants */

#define RSSPECAN_VAL_WIMAX_MODE_IEEE       0
#define RSSPECAN_VAL_WIMAX_MODE_ETSI       1
#define RSSPECAN_VAL_WIMAX_MODE_FFT        2
#define RSSPECAN_VAL_WIMAX_MODE_FLAT       4
#define RSSPECAN_VAL_WIMAX_MODE_FLAT_GRDEL 5
#define RSSPECAN_VAL_WIMAX_MODE_FLAT_DIFF  6
#define RSSPECAN_VAL_WIMAX_MODE_ACPR_ABS   7
#define RSSPECAN_VAL_WIMAX_MODE_ACPR_REL   8
#define RSSPECAN_VAL_WIMAX_MODE_MASK       9
#define RSSPECAN_VAL_WIMAX_MODE_PREAMBLE   10
#define RSSPECAN_VAL_WIMAX_MODE_ETSI5      11
#define RSSPECAN_VAL_WIMAX_MODE_ETSI10     12

#define RSSPECAN_VAL_WIMAX_EVM_MODE_SYMBOL    0
#define RSSPECAN_VAL_WIMAX_EVM_MODE_CARRIER   1
#define RSSPECAN_VAL_WIMAX_EVM_MODE_PHAS  2
#define RSSPECAN_VAL_WIMAX_EVM_MODE_FREQ  3

#define RSSPECAN_VAL_WIMAX_STAT_CCDF    0
#define RSSPECAN_VAL_WIMAX_STAT_BSTR    1
#define RSSPECAN_VAL_WIMAX_STAT_BSUM    2

#define RSSPECAN_VAL_WIMAX_CONS_SYM     0
#define RSSPECAN_VAL_WIMAX_CONS_CARR    1
#define RSSPECAN_VAL_WIMAX_CONS_BURST   2

#define RSSPECAN_VAL_WIMAX_ERR_FREQ      0
#define RSSPECAN_VAL_WIMAX_ERR_SYMB      1

#define RSSPECAN_VAL_WIMAX_IQ_OFFS 0
#define RSSPECAN_VAL_WIMAX_IQ_IMBA 1
#define RSSPECAN_VAL_WIMAX_IQ_QUAD 2

#define RSSPECAN_VAL_WIMAX_EVM_ALL       0
#define RSSPECAN_VAL_WIMAX_EVM_DATA      1
#define RSSPECAN_VAL_WIMAX_EVM_PILOT     2
#define RSSPECAN_VAL_WIMAX_EVM           3
#define RSSPECAN_VAL_WIMAX_EVM_PREAM     4

#define RSSPECAN_VAL_WIMAX_LIM_FERR      0
#define RSSPECAN_VAL_WIMAX_LIM_SYMB      1
#define RSSPECAN_VAL_WIMAX_LIM_SST       2
#define RSSPECAN_VAL_WIMAX_LIM_EVM_ALL   3
#define RSSPECAN_VAL_WIMAX_LIM_EVM_DATA  4
#define RSSPECAN_VAL_WIMAX_LIM_IQOFF     5
#define RSSPECAN_VAL_WIMAX_LIM_BERP      6
#define RSSPECAN_VAL_WIMAX_LIM_USERR     7

#define RSSPECAN_VAL_WIMAX_BURS          0
#define RSSPECAN_VAL_WIMAX_SYMB          1
#define RSSPECAN_VAL_WIMAX_ZONE          2
#define RSSPECAN_VAL_WIMAX_STAT_ZONE     3

#define RSSPECAN_VAL_WIMAX_MEASUREMENT_RF   0
#define RSSPECAN_VAL_WIMAX_MEASUREMENT_IQ   1

#define RSSPECAN_VAL_WIMAX_MEAS_RMS       0
#define RSSPECAN_VAL_WIMAX_MEAS_CREST     1
#define RSSPECAN_VAL_WIMAX_MEAS_RMS_ALL   2
#define RSSPECAN_VAL_WIMAX_MEAS_RMS_PILOT 3
#define RSSPECAN_VAL_WIMAX_MEAS_RMS_DATA  4
#define RSSPECAN_VAL_WIMAX_MEAS_RMS_DLPR  5
#define RSSPECAN_VAL_WIMAX_MEAS_RMS_NPIL  6

#define RSSPECAN_VAL_WIMAX_GATE_TIME    0
#define RSSPECAN_VAL_WIMAX_GATE_SAMPLE  1

#define RSSPECAN_VAL_WIMAX_IQ_I         0
#define RSSPECAN_VAL_WIMAX_IQ_Q         1
#define RSSPECAN_VAL_WIMAX_IQ_IQ        2

#define RSSPECAN_VAL_WIMAX_PREA          0
#define RSSPECAN_VAL_WIMAX_SUBF          1

/* K8  Bluetooth constants */
#define RSSPECAN_VAL_BTO_CFS_FERR_INITIAL           0
#define RSSPECAN_VAL_BTO_CFS_FERR_BLOCK             1
#define RSSPECAN_VAL_BTO_CFS_FERR_TOTAL             2

#define RSSPECAN_VAL_BTO_CFS_DEVM_RMS               0
#define RSSPECAN_VAL_BTO_CFS_DEVM_99PCT             1
#define RSSPECAN_VAL_BTO_CFS_DEVM_PEAK              2

#define RSSPECAN_VAL_BTO_DPEN_TESTED                0
#define RSSPECAN_VAL_BTO_DPEN_BER                   1
#define RSSPECAN_VAL_BTO_DPEN_PASSED                2

#define RSSPECAN_VAL_BTO_SE_TX                  0
#define RSSPECAN_VAL_BTO_SE_HADJ_LOW            1
#define RSSPECAN_VAL_BTO_SE_HADJ_UPP            2

#define RSSPECAN_VAL_BTO_RTP_GFSK           0
#define RSSPECAN_VAL_BTO_RTP_DPSK           1
#define RSSPECAN_VAL_BTO_RTP_RATIO          2

/* K14 constants */
#define RSSPECAN_VAL_SPECM_MARK_MAX_PEAK     0
#define RSSPECAN_VAL_SPECM_MARK_MAX_NEXT     1
#define RSSPECAN_VAL_SPECM_MARK_MAX_LEFT     2
#define RSSPECAN_VAL_SPECM_MARK_MAX_RIGHT    3
#define RSSPECAN_VAL_SPECM_MARK_MIN_PEAK     4
#define RSSPECAN_VAL_SPECM_MARK_MIN_NEXT     5
#define RSSPECAN_VAL_SPECM_MARK_MIN_LEFT     6
#define RSSPECAN_VAL_SPECM_MARK_MIN_RIGHT    7
#define RSSPECAN_VAL_SPECM_MARK_Y_MAX_PEAK   8
#define RSSPECAN_VAL_SPECM_MARK_Y_MAX_NEXT   9
#define RSSPECAN_VAL_SPECM_MARK_Y_MAX_ABOVE 10
#define RSSPECAN_VAL_SPECM_MARK_Y_MAX_BELOW 11
#define RSSPECAN_VAL_SPECM_MARK_Y_MIN_PEAK  12
#define RSSPECAN_VAL_SPECM_MARK_Y_MIN_NEXT  13
#define RSSPECAN_VAL_SPECM_MARK_Y_MIN_ABOVE 14
#define RSSPECAN_VAL_SPECM_MARK_Y_MIN_BELOW 15
#define RSSPECAN_VAL_SPECM_MARK_XY_MAX      16
#define RSSPECAN_VAL_SPECM_MARK_XY_MIN      17
#define RSSPECAN_VAL_SPECM_MARK_Y_TRIGGER   18

#define RSSPECAN_VAL_SPECM_TSTAMP_CURR   0
#define RSSPECAN_VAL_SPECM_TSTAMP_ALL   1

#define RSSPECAN_VAL_LOWER               0
#define RSSPECAN_VAL_UPPER               1

#define RSSPECAN_VAL_SPECM_FRAME_CURRENT 0
#define RSSPECAN_VAL_SPECM_FRAME_ALL     1

/* K20 constants */
#define RSSPECAN_VAL_LIM_LOW 0
#define RSSPECAN_VAL_LIM_UPP 1

#define RSSPECAN_VAL_ATV_VCP         0
#define RSSPECAN_VAL_ATV_VCF         1
#define RSSPECAN_VAL_ATV_SC1PR       2
#define RSSPECAN_VAL_ATV_SC1IF       3
#define RSSPECAN_VAL_ATV_SC2PR       4
#define RSSPECAN_VAL_ATV_SC2IF       5

#define RSSPECAN_VAL_ATV_RPC         1
#define RSSPECAN_VAL_ATV_MDEP        2

#define RSSPECAN_VAL_DTV_MERR         0
#define RSSPECAN_VAL_DTV_MERP         1
#define RSSPECAN_VAL_DTV_EVMR         2
#define RSSPECAN_VAL_DTV_EVMP         3
#define RSSPECAN_VAL_DTV_CFOF         4
#define RSSPECAN_VAL_DTV_SROF         5

#define RSSPECAN_VAL_DTV_IMB          4
#define RSSPECAN_VAL_DTV_QERR         5
#define RSSPECAN_VAL_DTV_SUP          6
#define RSSPECAN_VAL_DTV_PJIT         7

#define RSSPECAN_VAL_STAT_DEFAULT       0
#define RSSPECAN_VAL_STAT_ADJ           1
#define RSSPECAN_VAL_STAT_XRLEV         2
#define RSSPECAN_VAL_STAT_XRANGE        3
#define RSSPECAN_VAL_STAT_YMAX          4
#define RSSPECAN_VAL_STAT_YMIN          5

#define RSSPECAN_VAL_ATV_RES_RPOW        0
#define RSSPECAN_VAL_ATV_RES_NFC         1
#define RSSPECAN_VAL_ATV_RES_MFR         2
#define RSSPECAN_VAL_ATV_RES_CR          3
#define RSSPECAN_VAL_ATV_RES_NRBW        4

#define RSSPECAN_VAL_DTV_SPEC_SAL       0
#define RSSPECAN_VAL_DTV_SPEC_SAUP      1

#define RSSPECAN_VAL_STAT_MEAN      0
#define RSSPECAN_VAL_STAT_PEAK      1
#define RSSPECAN_VAL_STAT_CFAC      2


/* End of K20 constants */
/* K30 constants */
#define RSSPECAN_VAL_NOISE_DISP_FORM_SPLIT      0
#define RSSPECAN_VAL_NOISE_DISP_FORM_SINGLE     1

/* End of K30 constants */

/* K70 constants */
#define RSSPECAN_VAL_SYNC_PATT_ADD          0
#define RSSPECAN_VAL_SYNC_PATT_SELECT       1
#define RSSPECAN_VAL_SYNC_PATT_DELETE       2
#define RSSPECAN_VAL_SYNC_PATT_REMOVE       3
#define RSSPECAN_VAL_SYNC_PATT_REMOVE_ALL   4
#define RSSPECAN_VAL_SYNC_PATT_COPY         5

#define RSSPECAN_VAL_VSA_PATTERN_ALL        0
#define RSSPECAN_VAL_VSA_PATTERN_CURRENT    1

#define RSSPECAN_VAL_RESULT_EQDISP_MAGN_LIN     0
#define RSSPECAN_VAL_RESULT_EQDISP_MAGN_LOG     1
#define RSSPECAN_VAL_RESULT_EQDISP_PHAS_WRAP    2
#define RSSPECAN_VAL_RESULT_EQDISP_PHAS_UWRAP   3
#define RSSPECAN_VAL_RESULT_EQDISP_RIM          4
#define RSSPECAN_VAL_RESULT_EQDISP_GDELAY       5
#define RSSPECAN_VAL_RESULT_EQDISP_PRESP_WRAP   6
#define RSSPECAN_VAL_RESULT_EQDISP_PRESP_UWRAP  7
#define RSSPECAN_VAL_RESULT_EQDISP_FRESP_LIN    8
#define RSSPECAN_VAL_RESULT_EQDISP_FRESP_LOG    9
#define RSSPECAN_VAL_RESULT_EQDISP_CHRESP_LIN   10
#define RSSPECAN_VAL_RESULT_EQDISP_CHRESP_LOG   11

#define RSSPECAN_VAL_VSA_EQUA_LOAD              0
#define RSSPECAN_VAL_VSA_EQUA_SAVE              1
#define RSSPECAN_VAL_VSA_EQUA_DELETE            2
#define RSSPECAN_VAL_VSA_EQUA_RESET             3

#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_QPSK         0
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_DQPSK        1
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_OQPSK        2
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_DPI4QPSK     3
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_BPSK         4
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_8PSK         5
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_D8PSK        6
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_N3PI8PSK     7
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_MSK          8
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_DMSK         9
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_16QAM        10
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_32QAM        11
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_64QAM        12
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_128QAM       13
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_256QAM       14
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_2FSK         15
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_4FSK         16
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_8FSK         24
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_8VSB         17
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_16DQAM       18
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_32DQAM       19
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_64DQAM       20
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_128DQAM      21
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_256DQAM      22
#define RSSPECAN_VAL_VSA_MODULATION_FORMAT_USERQAM      23

#define RSSPECAN_VAL_VSA_DEM_RLEN_AUTO_ON               0
#define RSSPECAN_VAL_VSA_DEM_RLEN_SYMBOL                1
#define RSSPECAN_VAL_VSA_DEM_RLEN_TIME                  2

#define RSSPECAN_VAL_VSA_MBURS_SEARCH_FIRST     0
#define RSSPECAN_VAL_VSA_MBURS_SEARCH_NEXT      1
#define RSSPECAN_VAL_VSA_MBURS_SEARCH_START     2

#define RSSPECAN_VAL_VSA_IMPEXP_STANDARDS   0
#define RSSPECAN_VAL_VSA_IMPEXP_MAPPINGS    1
#define RSSPECAN_VAL_VSA_IMPEXP_PATTERNS    2
#define RSSPECAN_VAL_VSA_IMPEXP_FILTERS     3
#define RSSPECAN_VAL_VSA_IMPEXP_EQUALIZERS  4

#define RSSPECAN_VAL_RESULT_DISP_FREQ_ABS   0
#define RSSPECAN_VAL_RESULT_DISP_FREQ_REL   1
#define RSSPECAN_VAL_RESULT_DISP_RIM        2
#define RSSPECAN_VAL_RESULT_DISP_IQ_CONST   3
#define RSSPECAN_VAL_RESULT_DISP_IQ_VECT    4
#define RSSPECAN_VAL_RESULT_DISP_MAGN_ABS   5
#define RSSPECAN_VAL_RESULT_DISP_MAGN_REL   6
#define RSSPECAN_VAL_RESULT_DISP_PHAS_WRAP  7
#define RSSPECAN_VAL_RESULT_DISP_PHAS_UWRAP 8
#define RSSPECAN_VAL_RESULT_DISP_EYE_I      9
#define RSSPECAN_VAL_RESULT_DISP_EYE_Q      10

#define RSSPECAN_VAL_RESULT_DISP_AMPM_CONV  7
#define RSSPECAN_VAL_RESULT_DISP_EVM        8

#define RSSPECAN_VAL_RESULT_DISP_MAGN_RAW   3

#define RSSPECAN_VAL_RESULT_DISP_MAGN_ERR   5
#define RSSPECAN_VAL_RESULT_DISP_PHAS_ERR   6

#define RSSPECAN_VAL_VSA_MPOW            0
#define RSSPECAN_VAL_VSA_DTTS            1
#define RSSPECAN_VAL_VSA_ADR             2
#define RSSPECAN_VAL_VSA_CFER            3
#define RSSPECAN_VAL_VSA_EVM             4
#define RSSPECAN_VAL_VSA_FSK_CDR         5
#define RSSPECAN_VAL_VSA_FSK_DERR        6
#define RSSPECAN_VAL_VSA_FSK_MDEV        7
#define RSSPECAN_VAL_VSA_IQIM            8
#define RSSPECAN_VAL_VSA_GIMB            9
#define RSSPECAN_VAL_VSA_MERR            10
#define RSSPECAN_VAL_VSA_OOFF            11
#define RSSPECAN_VAL_VSA_PERR            12
#define RSSPECAN_VAL_VSA_PLER            13
#define RSSPECAN_VAL_VSA_QERR            14
#define RSSPECAN_VAL_VSA_RHO             15
#define RSSPECAN_VAL_VSA_SNR             16
#define RSSPECAN_VAL_VSA_FDER            17
#define RSSPECAN_VAL_VSA_FSK_RDEV        18
#define RSSPECAN_VAL_VSA_SR_ERROR        19

#define RSSPECAN_VAL_VSA_MODIF_NONE     0
#define RSSPECAN_VAL_VSA_MODIF_RMS      1
#define RSSPECAN_VAL_VSA_MODIF_AVG      2
#define RSSPECAN_VAL_VSA_MODIF_SDEV     3
#define RSSPECAN_VAL_VSA_MODIF_TPE      4
#define RSSPECAN_VAL_VSA_MODIF_PEAK     5
#define RSSPECAN_VAL_VSA_MODIF_ASYM     6
#define RSSPECAN_VAL_VSA_MODIF_PCTL     7
#define RSSPECAN_VAL_VSA_MODIF_FSV_CURR 8
#define RSSPECAN_VAL_VSA_MODIF_FSV_AVG  9
#define RSSPECAN_VAL_VSA_MODIF_FSV_RPE  10
#define RSSPECAN_VAL_VSA_MODIF_FSV_SDEV 11
#define RSSPECAN_VAL_VSA_MODIF_FSV_PCTL 12
#define RSSPECAN_VAL_VSA_MODIF_FSV_PEAK 13
#define RSSPECAN_VAL_VSA_MODIF_FSV_PAVG 14
#define RSSPECAN_VAL_VSA_MODIF_FSV_TPE  15
#define RSSPECAN_VAL_VSA_MODIF_FSV_PSD  16
#define RSSPECAN_VAL_VSA_MODIF_FSV_PPCT 17

#define RSSPECAN_VAL_VSA_CPOINT_PSHIFT  0
#define RSSPECAN_VAL_VSA_CPOINT_POWER   1
#define RSSPECAN_VAL_VSA_CPOINT_DATA    2

#define RSSPECAN_VAL_VSA_MOD_ORDER_BPSK       0
#define RSSPECAN_VAL_VSA_MOD_ORDER_QPSK       1
#define RSSPECAN_VAL_VSA_MOD_ORDER_OQPSK      2
#define RSSPECAN_VAL_VSA_MOD_ORDER_DQPSK      3
#define RSSPECAN_VAL_VSA_MOD_ORDER_PI4QPSK    4
#define RSSPECAN_VAL_VSA_MOD_ORDER_DPI4QPSK   5
#define RSSPECAN_VAL_VSA_MOD_ORDER_3PI4QPSK   6
#define RSSPECAN_VAL_VSA_MOD_ORDER_8PSK       7
#define RSSPECAN_VAL_VSA_MOD_ORDER_D8PSK      8
#define RSSPECAN_VAL_VSA_MOD_ORDER_N3PI8PSK   9
#define RSSPECAN_VAL_VSA_MOD_ORDER_PI8D8PSK   10
#define RSSPECAN_VAL_VSA_MOD_ORDER_MSK        11
#define RSSPECAN_VAL_VSA_MOD_ORDER_DMSK       12
#define RSSPECAN_VAL_VSA_MOD_ORDER_16QAM      13
#define RSSPECAN_VAL_VSA_MOD_ORDER_PI4_16QAM  14
#define RSSPECAN_VAL_VSA_MOD_ORDER_32QAM      15
#define RSSPECAN_VAL_VSA_MOD_ORDER_NPI4_32QAM 16
#define RSSPECAN_VAL_VSA_MOD_ORDER_64QAM      17
#define RSSPECAN_VAL_VSA_MOD_ORDER_128QAM     18
#define RSSPECAN_VAL_VSA_MOD_ORDER_256QAM     19
#define RSSPECAN_VAL_VSA_MOD_ORDER_2FSK       20
#define RSSPECAN_VAL_VSA_MOD_ORDER_4FSK       21
#define RSSPECAN_VAL_VSA_MOD_ORDER_8FSK       22
#define RSSPECAN_VAL_VSA_MOD_ORDER_USER_2QAM  23
#define RSSPECAN_VAL_VSA_MOD_ORDER_USER_4QAM  24
#define RSSPECAN_VAL_VSA_MOD_ORDER_USER_16QAM 25
#define RSSPECAN_VAL_VSA_MOD_ORDER_USER_32QAM 26
#define RSSPECAN_VAL_VSA_MOD_ORDER_8VSB       27
#define RSSPECAN_VAL_VSA_MOD_ORDER_2ASK       28
#define RSSPECAN_VAL_VSA_MOD_ORDER_4ASK       29
#define RSSPECAN_VAL_VSA_MOD_ORDER_16APSK     30
#define RSSPECAN_VAL_VSA_MOD_ORDER_32APSK     31

#define RSSPECAN_VAL_VSA_REF_DEV_MODE_ABS 0
#define RSSPECAN_VAL_VSA_REF_DEV_MODE_REL 1

#define RSSPECAN_VAL_VSA_PATT_SYNC_OFF  0
#define RSSPECAN_VAL_VSA_PATT_SYNC_ON   1
#define RSSPECAN_VAL_VSA_PATT_SYNC_AUTO 2

#define RSSPECAN_VAL_VSA_AUTOMATIC_ESTIMATION  0
#define RSSPECAN_VAL_VSA_ESTIMATION_SYMBOL_1   1
#define RSSPECAN_VAL_VSA_ESTIMATION_SYMBOL_2   2
#define RSSPECAN_VAL_VSA_ESTIMATION_CAPTURE_OV 4

#define RSSPECAN_VAL_VSA_RESULT_DISP_MAG_ABS      0
#define RSSPECAN_VAL_VSA_RESULT_DISP_MAG_REL      1
#define RSSPECAN_VAL_VSA_RESULT_DISP_PHA_WRAP     2
#define RSSPECAN_VAL_VSA_RESULT_DISP_PHA_UNWRAP   3
#define RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ABS     4
#define RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_REL     5
#define RSSPECAN_VAL_VSA_RESULT_DISP_RI           6
#define RSSPECAN_VAL_VSA_RESULT_DISP_EYE_R        7
#define RSSPECAN_VAL_VSA_RESULT_DISP_EYE_I        8
#define RSSPECAN_VAL_VSA_RESULT_DISP_EYE_FREQ     9
#define RSSPECAN_VAL_VSA_RESULT_DISP_IQ_CONST     10
#define RSSPECAN_VAL_VSA_RESULT_DISP_IQ_VECT      11
#define RSSPECAN_VAL_VSA_RESULT_DISP_CONST_FREQ   12
#define RSSPECAN_VAL_VSA_RESULT_DISP_VECT_FREQ    13
#define RSSPECAN_VAL_VSA_RESULT_DISP_EVM          14
#define RSSPECAN_VAL_VSA_RESULT_DISP_MAG_ERR      15
#define RSSPECAN_VAL_VSA_RESULT_DISP_PHA_ERR      16
#define RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ERR_ABS 17
#define RSSPECAN_VAL_VSA_RESULT_DISP_FREQ_ERR_REL 18
#define RSSPECAN_VAL_VSA_RESULT_DISP_MOD_ACC      19
#define RSSPECAN_VAL_VSA_RESULT_DISP_IQ_CON_ROT   20
#define RSSPECAN_VAL_VSA_RESULT_DISP_MOV          21

#define RSSPECAN_VAL_VSA_TRANSFORMATION_NORMAL 0
#define RSSPECAN_VAL_VSA_TRANSFORMATION_SPEC   1
#define RSSPECAN_VAL_VSA_TRANSFORMATION_STAT   2

#define RSSPECAN_VAL_VSA_RESULT_DISP_BIN 0
#define RSSPECAN_VAL_VSA_RESULT_DISP_DEC 1
#define RSSPECAN_VAL_VSA_RESULT_DISP_OCT 2
#define RSSPECAN_VAL_VSA_RESULT_DISP_HEX 3

#define RSSPECAN_VAL_VSA_LIMIT_CURRENT      0
#define RSSPECAN_VAL_VSA_LIMIT_MEAN         1
#define RSSPECAN_VAL_VSA_LIMIT_PEAK         2
#define RSSPECAN_VAL_VSA_LIMIT_CURRENT_RMS  3
#define RSSPECAN_VAL_VSA_LIMIT_MEAN_RMS     4
#define RSSPECAN_VAL_VSA_LIMIT_PEAK_RMS     5
#define RSSPECAN_VAL_VSA_LIMIT_CURRENT_PEAK 6
#define RSSPECAN_VAL_VSA_LIMIT_MEAN_PEAK    7
#define RSSPECAN_VAL_VSA_LIMIT_PEAK_PEAK    8

#define RSSPECAN_VAL_VSA_RESULT_EVM              0
#define RSSPECAN_VAL_VSA_RESULT_PHASE_ERROR      1
#define RSSPECAN_VAL_VSA_RESULT_MAGNITUDE_ERROR  2
#define RSSPECAN_VAL_VSA_RESULT_CF_ERROR         3
#define RSSPECAN_VAL_VSA_RESULT_RHO              4
#define RSSPECAN_VAL_VSA_RESULT_IQ_OFFSET        5
#define RSSPECAN_VAL_VSA_RESULT_FREQ_ERROR       6
#define RSSPECAN_VAL_VSA_RESULT_FSK_DEV_ERROR    7
#define RSSPECAN_VAL_VSA_RESULT_FSK_MEAS_DEV     8
#define RSSPECAN_VAL_VSA_RESULT_FSK_REF_DEV      9
#define RSSPECAN_VAL_VSA_RESULT_POWER            10
#define RSSPECAN_VAL_VSA_RESULT_CFD              11
#define RSSPECAN_VAL_VSA_RESULT_MOD_ERROR        12
#define RSSPECAN_VAL_VSA_RESULT_IQ_IMBALANCE     13
#define RSSPECAN_VAL_VSA_RESULT_GAIN_IMBALANCE   14
#define RSSPECAN_VAL_VSA_RESULT_QUADRATURE_ERROR 15
#define RSSPECAN_VAL_VSA_RESULT_AMPLITUDE_DROOP  16

#define RSSPECAN_VAL_VSA_STAT_AVERAGE_SWEEP  0
#define RSSPECAN_VAL_VSA_STAT_AVERAGE        1
#define RSSPECAN_VAL_VSA_STAT_PEAK           2
#define RSSPECAN_VAL_VSA_STAT_SDEV           3
#define RSSPECAN_VAL_VSA_STAT_95_RMS         4
#define RSSPECAN_VAL_VSA_STAT_MAX_PEAK_SWEEP 5
#define RSSPECAN_VAL_VSA_STAT_MAX_AVG        6
#define RSSPECAN_VAL_VSA_STAT_MAX_PEAK       7
#define RSSPECAN_VAL_VSA_STAT_MAX_SDEV       8
#define RSSPECAN_VAL_VSA_STAT_MAX_95_RMS     9

#define RSSPECAN_VAL_VSA_LIMIT_MODULATION_ACCURACY 0

#define RSSPECAN_VAL_TRACE_1      1
#define RSSPECAN_VAL_TRACE_2      2
#define RSSPECAN_VAL_TRACE_3      3
#define RSSPECAN_VAL_TRACE_4      4
#define RSSPECAN_VAL_TRACE_5      5
#define RSSPECAN_VAL_TRACE_6      6
#define RSSPECAN_VAL_REAL_TRACE_1 7
#define RSSPECAN_VAL_REAL_TRACE_2 8
#define RSSPECAN_VAL_REAL_TRACE_3 9
#define RSSPECAN_VAL_IMAG_TRACE_1 10
#define RSSPECAN_VAL_IMAG_TRACE_2 11
#define RSSPECAN_VAL_IMAG_TRACE_3 12
#define RSSPECAN_VAL_IQC_TRACE_Y  13
#define RSSPECAN_VAL_IQC_TRACE_X  14

#define RSSPECAN_VAL_VSA_BER_RESULT_CURRENT       0
#define RSSPECAN_VAL_VSA_BER_RESULT_MINIMUM       1
#define RSSPECAN_VAL_VSA_BER_RESULT_MAXIMUM       2
#define RSSPECAN_VAL_VSA_BER_RESULT_ACCUMULATIVE  3
/* End of K70 constants */

/* K72/K74 constants */
#define RSSPECAN_VAL_CDP_TABLE_OFF          0
#define RSSPECAN_VAL_CDP_TABLE_SELECT       1
#define RSSPECAN_VAL_CDP_TABLE_NEW          2
#define RSSPECAN_VAL_CDP_TABLE_DELETE       3
#define RSSPECAN_VAL_CDP_TABLE_COPY         4

#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_DPCH     0
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_PICH     1
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_SCCPCH   2
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_HSSCCH   3
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_HSPDSCH  4
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_CHAN     5
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_CPRSD    10
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_CPRTPC   11
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_CPRSF    12
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_CPRSFTPC 13
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_EHICH    14
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_EAGCH    15
#define RSSPECAN_VAL_CDP_TABLE_CHANNEL_SCPICH   16

#define RSSPECAN_VAL_3GPP_MARKER_CPICH  0
#define RSSPECAN_VAL_3GPP_MARKER_PCCPCH 1

#define RSSPECAN_VAL_3GBS_TRAC_CDP           0
#define RSSPECAN_VAL_3GBS_TRAC_CDPRAT        1
#define RSSPECAN_VAL_3GBS_TRAC_CDEP          2
#define RSSPECAN_VAL_3GBS_TRAC_CTAB          3
#define RSSPECAN_VAL_3GBS_TRAC_CWCD          4
#define RSSPECAN_VAL_3GBS_TRAC_PWCD          5
#define RSSPECAN_VAL_3GBS_TRAC_PCD           6
#define RSSPECAN_VAL_3GBS_TRAC_MACC          7
#define RSSPECAN_VAL_3GBS_TRAC_PVSLA         8
#define RSSPECAN_VAL_3GBS_TRAC_PVSLR         9
#define RSSPECAN_VAL_3GBS_TRAC_BSTR          10
#define RSSPECAN_VAL_3GBS_TRAC_SYMBCONST     11
#define RSSPECAN_VAL_3GBS_TRAC_SYMBEVM       12
#define RSSPECAN_VAL_3GBS_TRAC_FVSL          13
#define RSSPECAN_VAL_3GBS_TRAC_PVSY          14
#define RSSPECAN_VAL_3GBS_TRAC_EVM_MAGN      15
#define RSSPECAN_VAL_3GBS_TRAC_EVM_PHAS      16
#define RSSPECAN_VAL_3GBS_TRAC_COMP_CONS     17
#define RSSPECAN_VAL_3GBS_TRAC_CHIP_EVM      18
#define RSSPECAN_VAL_3GBS_TRAC_CHIP_MAGN     19
#define RSSPECAN_VAL_3GBS_TRAC_CHIP_PHAS     20
#define RSSPECAN_VAL_3GBS_TRAC_ATRACE2       21

#define RSSPECAN_VAL_WCDP_RES_PTOT      0
#define RSSPECAN_VAL_WCDP_RES_FERR      1
#define RSSPECAN_VAL_WCDP_RES_TFR       2
#define RSSPECAN_VAL_WCDP_RES_MACC      3
#define RSSPECAN_VAL_WCDP_RES_PCD       4
#define RSSPECAN_VAL_WCDP_RES_EVMR      5
#define RSSPECAN_VAL_WCDP_RES_EVMP      6
#define RSSPECAN_VAL_WCDP_RES_CERR      7
#define RSSPECAN_VAL_WCDP_RES_CSL       8
#define RSSPECAN_VAL_WCDP_RES_SRAT      9
#define RSSPECAN_VAL_WCDP_RES_CHAN      10
#define RSSPECAN_VAL_WCDP_RES_CDPA      11
#define RSSPECAN_VAL_WCDP_RES_CDPR      12
#define RSSPECAN_VAL_WCDP_RES_IQOF      13
#define RSSPECAN_VAL_WCDP_RES_IQIM      14
#define RSSPECAN_VAL_WCDP_RES_MTYP      15
#define RSSPECAN_VAL_WCDP_RES_RHO       16
#define RSSPECAN_VAL_WCDP_RES_TOFF      17
#define RSSPECAN_VAL_WCDP_RES_PSYM      18
#define RSSPECAN_VAL_WCDP_RES_ACH       19
#define RSSPECAN_VAL_WCDP_RES_MPIC      20
#define RSSPECAN_VAL_WCDP_RES_RCDE      21
#define RSSPECAN_VAL_WCDP_RES_ARCD      22
#define RSSPECAN_VAL_WCDP_RES_RCDQ      23


#define RSSPECAN_VAL_POW_RES_ACP         0
#define RSSPECAN_VAL_POW_RES_CPOW        1
#define RSSPECAN_VAL_POW_RES_MCAC        2
#define RSSPECAN_VAL_POW_RES_OBAN        3
#define RSSPECAN_VAL_POW_RES_TDMA        4
#define RSSPECAN_VAL_POW_RES_ACLR        5
#define RSSPECAN_VAL_TAER_RES_TAER       0

/* End of K72/K74 constants*/

/* K73 constants*/
#define RSSPECAN_VAL_CDP_RES_PTOT       0
#define RSSPECAN_VAL_CDP_RES_FERR       1
#define RSSPECAN_VAL_CDP_RES_TFR        2
#define RSSPECAN_VAL_CDP_RES_MACC       3
#define RSSPECAN_VAL_CDP_RES_PCD        4
#define RSSPECAN_VAL_CDP_RES_EVMR       5
#define RSSPECAN_VAL_CDP_RES_EVMP       6
#define RSSPECAN_VAL_CDP_RES_CERR       7
#define RSSPECAN_VAL_CDP_RES_CSL        8
#define RSSPECAN_VAL_CDP_RES_SRAT       9
#define RSSPECAN_VAL_CDP_RES_CHAN       10
#define RSSPECAN_VAL_CDP_RES_CDPA       11
#define RSSPECAN_VAL_CDP_RES_CDPR       12
#define RSSPECAN_VAL_CDP_RES_IQOF       13
#define RSSPECAN_VAL_CDP_RES_IQIM       14
#define RSSPECAN_VAL_CDP_RES_CMAP       15
#define RSSPECAN_VAL_CDP_RES_PSYM       16
#define RSSPECAN_VAL_CDP_RES_RHO        17
#define RSSPECAN_VAL_CDP_RES_TOFF       18
#define RSSPECAN_VAL_CDP_RES_EVMB       19
#define RSSPECAN_VAL_CDP_RES_EVM        20
#define RSSPECAN_VAL_CDP_RES_MTYP       21
#define RSSPECAN_VAL_CDP_RES_ACH        22
#define RSSPECAN_VAL_CDP_RES_MPIC       23
#define RSSPECAN_VAL_CDP_RES_ARCD       24


#define RSSPECAN_VAL_3GUE_TRAC_CDP          0
#define RSSPECAN_VAL_3GUE_TRAC_CDPRAT       1
#define RSSPECAN_VAL_3GUE_TRAC_CDPOVER      2
#define RSSPECAN_VAL_3GUE_TRAC_CDEP         3
#define RSSPECAN_VAL_3GUE_TRAC_CTAB         4
#define RSSPECAN_VAL_3GUE_TRAC_PCD          5
#define RSSPECAN_VAL_3GUE_TRAC_MACC         6
#define RSSPECAN_VAL_3GUE_TRAC_PVSLA        7
#define RSSPECAN_VAL_3GUE_TRAC_PVSLR        8
#define RSSPECAN_VAL_3GUE_TRAC_BSTR         9
#define RSSPECAN_VAL_3GUE_TRAC_PVSY         10
#define RSSPECAN_VAL_3GUE_TRAC_SYMBCONST    11
#define RSSPECAN_VAL_3GUE_TRAC_SYMBEVM      12
#define RSSPECAN_VAL_3GUE_TRAC_FVSL         13
#define RSSPECAN_VAL_3GUE_TRAC_PSVSL        14
#define RSSPECAN_VAL_3GUE_TRAC_EVM_MAGN     15
#define RSSPECAN_VAL_3GUE_TRAC_EVM_PHAS     16
#define RSSPECAN_VAL_3GUE_TRAC_COMP_CONS    17
#define RSSPECAN_VAL_3GUE_TRAC_CHIP_EVM     18
#define RSSPECAN_VAL_3GUE_TRAC_CHIP_MAGN    19
#define RSSPECAN_VAL_3GUE_TRAC_CHIP_PHAS    20
#define RSSPECAN_VAL_3GUE_TRAC_EVM_RMS      21
/* End of K73 constants*/

/* K76/K77 constants */
#define RSSPECAN_VAL_TDS_TABLE_OFF      0
#define RSSPECAN_VAL_TDS_TABLE_SELECT   1
#define RSSPECAN_VAL_TDS_TABLE_NEW      2
#define RSSPECAN_VAL_TDS_TABLE_DELETE   3
#define RSSPECAN_VAL_TDS_TABLE_COPY     4

#define RSSPECAN_VAL_TDS_TABLE_CHANNEL_MID      1
#define RSSPECAN_VAL_TDS_TABLE_CHANNEL_DPCH     2
#define RSSPECAN_VAL_TDS_TABLE_CHANNEL_PCCPCH   3
#define RSSPECAN_VAL_TDS_TABLE_CHANNEL_SCCPCH   4
#define RSSPECAN_VAL_TDS_TABLE_CHANNEL_FPACH    5
#define RSSPECAN_VAL_TDS_TABLE_CHANNEL_PRACH    6

#define RSSPECAN_VAL_TDS_MOD_INVALID        0
#define RSSPECAN_VAL_TDS_MOD_QPSK           1
#define RSSPECAN_VAL_TDS_MOD_8PSK           2
#define RSSPECAN_VAL_TDS_MOD_16QAM          3

#define RSSPECAN_VAL_TDS_RESULT_SLOT            0
#define RSSPECAN_VAL_TDS_RESULT_PDAT            1
#define RSSPECAN_VAL_TDS_RESULT_PD1             2
#define RSSPECAN_VAL_TDS_RESULT_PD2             3
#define RSSPECAN_VAL_TDS_RESULT_PMID            4
#define RSSPECAN_VAL_TDS_RESULT_RHO             5
#define RSSPECAN_VAL_TDS_RESULT_MACC            6
#define RSSPECAN_VAL_TDS_RESULT_PCD             7
#define RSSPECAN_VAL_TDS_RESULT_FERR            8
#define RSSPECAN_VAL_TDS_RESULT_CERR            9
#define RSSPECAN_VAL_TDS_RESULT_TFR             10
#define RSSPECAN_VAL_TDS_RESULT_IQIM            11
#define RSSPECAN_VAL_TDS_RESULT_IQOF            12
#define RSSPECAN_VAL_TDS_RESULT_ACT             13
#define RSSPECAN_VAL_TDS_RESULT_SRAT            14
#define RSSPECAN_VAL_TDS_RESULT_CHAN            15
#define RSSPECAN_VAL_TDS_RESULT_SFAC            16
#define RSSPECAN_VAL_TDS_RESULT_CDPR            17
#define RSSPECAN_VAL_TDS_RESULT_CDP             18
#define RSSPECAN_VAL_TDS_RESULT_EVMR            19
#define RSSPECAN_VAL_TDS_RESULT_EVMP            20
#define RSSPECAN_VAL_TDS_RESULT_ARCDE           21
#define RSSPECAN_VAL_TDS_RESULT_CMAPPING        22
#define RSSPECAN_VAL_TDS_RESULT_CSLOT           23
#define RSSPECAN_VAL_TDS_RESULT_MPIC            24
#define RSSPECAN_VAL_TDS_RESULT_MTYPE           25
#define RSSPECAN_VAL_TDS_RESULT_PSYMBOL         26
#define RSSPECAN_VAL_TDS_RESULT_PTOTAL          27
#define RSSPECAN_VAL_TDS_RESULT_TOFFSET         28

/*End of K76 constants */

/* K8x constants*/
#define RSSPECAN_VAL_C2K_CTABLE         0

#define RSSPECAN_VAL_C2K_SLOT           0
#define RSSPECAN_VAL_C2K_PTOTAL         1
#define RSSPECAN_VAL_C2K_PPICH          2
#define RSSPECAN_VAL_C2K_RHO            3
#define RSSPECAN_VAL_C2K_MACCURACY      4
#define RSSPECAN_VAL_C2K_PCDERROR       5
#define RSSPECAN_VAL_C2K_IQIMBALANCE    6
#define RSSPECAN_VAL_C2K_IQOFFSET       7
#define RSSPECAN_VAL_C2K_FERROR         8
#define RSSPECAN_VAL_C2K_FERPPM         9
#define RSSPECAN_VAL_C2K_CERROR         10
#define RSSPECAN_VAL_C2K_TFRAME         11
#define RSSPECAN_VAL_C2K_ACTIVE         12
#define RSSPECAN_VAL_C2K_SRATE          13
#define RSSPECAN_VAL_C2K_CHANNEL        14
#define RSSPECAN_VAL_C2K_SFACTOR        15
#define RSSPECAN_VAL_C2K_TOFFSET        16
#define RSSPECAN_VAL_C2K_POFFSET        17
#define RSSPECAN_VAL_C2K_CDPRELATIVE    18
#define RSSPECAN_VAL_C2K_CDPABSOLUTE    19
#define RSSPECAN_VAL_C2K_EVMRMS         20
#define RSSPECAN_VAL_C2K_EVMPEAK        21
#define RSSPECAN_VAL_C2K_RHOPILOT       22
#define RSSPECAN_VAL_C2K_RHO1           23
#define RSSPECAN_VAL_C2K_RHO2           24
#define RSSPECAN_VAL_C2K_PPILOT         25
#define RSSPECAN_VAL_C2K_PMAC           26
#define RSSPECAN_VAL_C2K_PDATA          27
#define RSSPECAN_VAL_C2K_PPREAMBLE      28
#define RSSPECAN_VAL_C2K_DMTYPE         29
#define RSSPECAN_VAL_C2K_MACTIVE        30
#define RSSPECAN_VAL_C2K_DACTIVE        31
#define RSSPECAN_VAL_C2K_PLENGTH        32
#define RSSPECAN_VAL_C2K_PDMAX          33
#define RSSPECAN_VAL_C2K_PDMIN          34
#define RSSPECAN_VAL_C2K_IPMMAX         35
#define RSSPECAN_VAL_C2K_MTYPE          36
#define RSSPECAN_VAL_C2K_PRRI           37
#define RSSPECAN_VAL_C2K_DRPICH         38
#define RSSPECAN_VAL_C2K_RHOVERALL      39
#define RSSPECAN_VAL_C2K_RHODATA        40
#define RSSPECAN_VAL_C2K_RHOMAC         41

#define RSSPECAN_VAL_C2K_PNF            42
#define RSSPECAN_VAL_C2K_PPAV           43
#define RSSPECAN_VAL_C2K_TAU            44
#define RSSPECAN_VAL_C2K_TACT           45
#define RSSPECAN_VAL_C2K_EVMP           46

#define RSSPECAN_VAL_C2K_CDEPEAK        47
#define RSSPECAN_VAL_C2K_CDERMS         48
#define RSSPECAN_VAL_C2K_CODMULATION    49
#define RSSPECAN_VAL_C2K_CODPOWER       50


/* End of K8x constants*/

/* Receiver constants*/

#define RSSPECAN_VAL_VNET_MEAS_PRESCAN   0
#define RSSPECAN_VAL_VNET_MEAS_FINAL     1

/* End of Receiver constants*/

/* K10 constants*/
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_TGSM_380 0
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_TGSM_410 1
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_GSM_450  2
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_GSM_480  3
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_GSM_710  4
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_GSM_750  5
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_TGSM_810 6
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_GSM_850  7
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_PGSM_900 8
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_EGSM_900 9
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_RGSM_900 10
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_TGSM_900 11
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_DCS_1800 12
#define RSSPECAN_VAL_GSM_K10_FREQ_BAND_PCS_1900 13

#define RSSPECAN_VAL_GSM_K10_MACC  0    // - Modulation Accuracy
#define RSSPECAN_VAL_GSM_K10_PFER  1    // - Phase Error vs Time
#define RSSPECAN_VAL_GSM_K10_ETIM  2    // - EVM vs Time
#define RSSPECAN_VAL_GSM_K10_MERR  3    // - Magnitude Error
#define RSSPECAN_VAL_GSM_K10_CONST 4    // - Constellation
#define RSSPECAN_VAL_GSM_K10_PTEM  5    // - Power vs. Time
#define RSSPECAN_VAL_GSM_K10_MOD   6    // - Modulation Spectrum
#define RSSPECAN_VAL_GSM_K10_SWIT  7    // - Transient Spectrum
#define RSSPECAN_VAL_GSM_K10_WIDE  8    // - Wide Spectrum
#define RSSPECAN_VAL_GSM_K10_TS    9    // - Trigger to Sync

#define RSSPECAN_VAL_GSM_K10_MEAS_EVM_RMS         0
#define RSSPECAN_VAL_GSM_K10_MEAS_EVM_PEAK        1
#define RSSPECAN_VAL_GSM_K10_MEAS_MERR_RMS        2
#define RSSPECAN_VAL_GSM_K10_MEAS_MERR_PEAK       3
#define RSSPECAN_VAL_GSM_K10_MEAS_PERR_RMS        4
#define RSSPECAN_VAL_GSM_K10_MEAS_PERR_PEAK       5
#define RSSPECAN_VAL_GSM_K10_MEAS_OSUPP           6
#define RSSPECAN_VAL_GSM_K10_MEAS_IQOF            7
#define RSSPECAN_VAL_GSM_K10_MEAS_IQIM            8
#define RSSPECAN_VAL_GSM_K10_MEAS_FREQ            9
#define RSSPECAN_VAL_GSM_K10_MEAS_BPOW            10
#define RSSPECAN_VAL_GSM_K10_MEAS_ADR             11
#define RSSPECAN_VAL_GSM_K10_MEAS_PERC_EVM        12
#define RSSPECAN_VAL_GSM_K10_MEAS_PERC_MERR       13
#define RSSPECAN_VAL_GSM_K10_MEAS_PERC_PERR       14
#define RSSPECAN_VAL_GSM_K10_MEAS_TRIGGER_TO_SYNC 15

#define RSSPECAN_VAL_GSM_K10_MOD_AVG            0
#define RSSPECAN_VAL_GSM_K10_MOD_CUR            1
#define RSSPECAN_VAL_GSM_K10_MOD_MAX            2
#define RSSPECAN_VAL_GSM_K10_MOD_DEV            3

#define RSSPECAN_VAL_GSM_K10_BURST_ALL          0
#define RSSPECAN_VAL_GSM_K10_BURST_CURR         1

#define RSSPECAN_VAL_GSM_K10_BURST_MOD_AVG_PWR  0
#define RSSPECAN_VAL_GSM_K10_BURST_MOD_CREST    1
#define RSSPECAN_VAL_GSM_K10_BURST_MOD_MAX_PWR  2

#define RSSPECAN_VAL_GSM_K10_AUTO_OFF           0
#define RSSPECAN_VAL_GSM_K10_AUTO_ON            1
#define RSSPECAN_VAL_GSM_K10_AUTO_ONCE          2

#define RSSPECAN_VAL_GSM_K10_MOD_SPEC    0
#define RSSPECAN_VAL_GSM_K10_TRANS_SPEC  1
#define RSSPECAN_VAL_GSM_K10_WIDE_SPEC   2

/* End of K10 constants*/

/* K15 constants*/

#define RSSPECAN_VAR_AVI_DEMOD_SUMM_30     0
#define RSSPECAN_VAR_AVI_DEMOD_SUMM_996    1
#define RSSPECAN_VAR_AVI_DEMOD_SUMM_ID     2
#define RSSPECAN_VAR_AVI_DEMOD_SUMM_90     3
#define RSSPECAN_VAR_AVI_DEMOD_SUMM_150    4
#define RSSPECAN_VAR_AVI_DEMOD_SUMM_90_150 5

/* End of K15 constants*/
#define RSSPECAN_VAL_OFF                   0
#define RSSPECAN_VAL_ON                    1
#define RSSPECAN_VAL_ALL                   2

#define RSSPECAN_VAL_WFILTER_ALL           0
#define RSSPECAN_VAL_WFILTER_ADJ           1
#define RSSPECAN_VAL_WFILTER_ALT           2
#define RSSPECAN_VAL_WFILTER_TX            3

#define RSSPECAN_VAL_ADJ_ALL               0
#define RSSPECAN_VAL_ADJ_FREQ              1
#define RSSPECAN_VAL_ADJ_LEV               2

#define RSSPECAN_VAL_USTD_SET              0
#define RSSPECAN_VAL_USTD_SAVE             1
#define RSSPECAN_VAL_USTD_DEL              2

#define RSSPECAN_VAL_DIQ_IN                0
#define RSSPECAN_VAL_DIQ_OUT               1

/* Signal Source Analyzer constants*/

#define RSSPECAN_VAL_SSA_VCO_TYPE_PUSH     0
#define RSSPECAN_VAL_SSA_VCO_TYPE_PULL     1

#define RSSPECAN_VAL_SSA_PREMEAS_CONT      0
#define RSSPECAN_VAL_SSA_PREMEAS_SKIP      1
#define RSSPECAN_VAL_SSA_PREMEAS_FIN       2
#define RSSPECAN_VAL_SSA_PREMEAS_NEW       3

#define RSSPECAN_VAL_SSA_PHAS_NOIS_RES_RFM 0
#define RSSPECAN_VAL_SSA_PHAS_NOIS_RES_RPM 1
#define RSSPECAN_VAL_SSA_PHAS_NOIS_RES_RMS 2
#define RSSPECAN_VAL_SSA_PHAS_NOIS_RES_IPN 3

#define RSSPECAN_VAL_SSA_VCO_TCH_RES_TYPE_FREQ 0
#define RSSPECAN_VAL_SSA_VCO_TCH_RES_TYPE_POW  1
#define RSSPECAN_VAL_SSA_VCO_TCH_RES_TYPE_SENS 2
#define RSSPECAN_VAL_SSA_VCO_TCH_RES_TYPE_CURR 3

#define RSSPECAN_VAL_SSA_VCO_PUSH_PULL_RES_FDEV 0
#define RSSPECAN_VAL_SSA_VCO_PUSH_PULL_RES_PDEV 1
#define RSSPECAN_VAL_SSA_VCO_PUSH_PULL_RES_SENS 2

#define RSSPECAN_VAL_SSA_VCO_HARM_RES_FREQ 0
#define RSSPECAN_VAL_SSA_VCO_HARM_RES_APOW 1
#define RSSPECAN_VAL_SSA_VCO_HARM_RES_RPOW 2
#define RSSPECAN_VAL_SSA_VCO_HARM_RES_SENS 3

#define RSSPECAN_VAL_SSA_VCO_DCH_RES_FREQ  0
#define RSSPECAN_VAL_SSA_VCO_DCH_RES_POW   1

#define RSSPECAN_VAL_SSA_MOD_AM            0
#define RSSPECAN_VAL_SSA_MOD_FM            1
#define RSSPECAN_VAL_SSA_MOD_PM            2

#define RSSPECAN_VAL_SSA_DET_PPE           0
#define RSSPECAN_VAL_SSA_DET_MPE           1
#define RSSPECAN_VAL_SSA_DET_MID           2
#define RSSPECAN_VAL_SSA_DET_RMS           3
#define RSSPECAN_VAL_SSA_DET_FERR          4
#define RSSPECAN_VAL_SSA_DET_CARR          5

#define RSSPECAN_VAL_SSA_FREQ_BAND_1M10M   1
#define RSSPECAN_VAL_SSA_FREQ_BAND_10M50M  2
#define RSSPECAN_VAL_SSA_FREQ_BAND_50M8G   3
#define RSSPECAN_VAL_SSA_FREQ_BAND_8G26G   4
#define RSSPECAN_VAL_SSA_FREQ_BAND_26G50G  5

/* End of Signal Source Analyzer constants*/
#define RSSPECAN_VAL_TRACKING_GEN_TYPE_CLOSS 0
#define RSSPECAN_VAL_TRACKING_GEN_TYPE_MAGN  1
#define RSSPECAN_VAL_TRACKING_GEN_TYPE_PHAS  2
#define RSSPECAN_VAL_TRACKING_GEN_TYPE_REFL  3

/* */
#define RSSPECAN_VAL_AUDIO_MEAS_RES_PPEAK 0
#define RSSPECAN_VAL_AUDIO_MEAS_RES_RMS   1
#define RSSPECAN_VAL_AUDIO_MEAS_RES_THD   2
#define RSSPECAN_VAL_AUDIO_MEAS_RES_SINAD 3

#define RSSPECAN_VAL_LTE_DOWNLINK_QPSK      0
#define RSSPECAN_VAL_LTE_DOWNLINK_16QAM     1
#define RSSPECAN_VAL_LTE_DOWNLINK_64QAM     2

#define RSSPECAN_VAL_LTE_UPLINK_QPSK   0
#define RSSPECAN_VAL_LTE_UPLINK_16QAM  1
#define RSSPECAN_VAL_LTE_UPLINK_64QAM  2

#define RSSPECAN_VAL_MEASTYPE_VALUES  0
#define RSSPECAN_VAL_MEASTYPE_SUMMARY 1

#define RSSPECAN_VAL_MEASTYPE_PROBABILITY 0
#define RSSPECAN_VAL_MEASTYPE_POWER_STEPS 1

#define RSSPECAN_VAL_LTE_DOWNLINK_CHBW  0
#define RSSPECAN_VAL_LTE_DOWNLINK_NRB   1

#define RSSPECAN_VAL_FSM_CHAN_TYPE_LEFT        0
#define RSSPECAN_VAL_FSM_CHAN_TYPE_RIGHT       1
#define RSSPECAN_VAL_FSM_CHAN_TYPE_MPX         2
#define RSSPECAN_VAL_FSM_CHAN_TYPE_MONO        3
#define RSSPECAN_VAL_FSM_CHAN_TYPE_STER        4
#define RSSPECAN_VAL_FSM_CHAN_TYPE_RDS         5
#define RSSPECAN_VAL_FSM_CHAN_TYPE_PILOT       6
#define RSSPECAN_VAL_FSM_CHAN_TYPE_XTALK       7

#define RSSPECAN_VAL_FMS_HPAS_FILT_NONE        0
#define RSSPECAN_VAL_FMS_HPAS_FILT_20HZ        1
#define RSSPECAN_VAL_FMS_HPAS_FILT_50HZ        2
#define RSSPECAN_VAL_FMS_HPAS_FILT_300HZ       3

#define RSSPECAN_VAL_FMS_LPAS_FILT_NONE        0
#define RSSPECAN_VAL_FMS_LPAS_FILT_10PCT       1
#define RSSPECAN_VAL_FMS_LPAS_FILT_15KHZ       2
#define RSSPECAN_VAL_FMS_LPAS_FILT_150KHZ      3
#define RSSPECAN_VAL_FMS_LPAS_FILT_23KHZ       4
#define RSSPECAN_VAL_FMS_LPAS_FILT_25PCT       5
#define RSSPECAN_VAL_FMS_LPAS_FILT_3KHZ        6
#define RSSPECAN_VAL_FMS_LPAS_FILT_5PCT        7

#define RSSPECAN_VAL_FMS_WEIGHTING_NONE        0
#define RSSPECAN_VAL_FMS_WEIGHTING_CCIT        1
#define RSSPECAN_VAL_FMS_WEIGHTING_CCIRU       2
#define RSSPECAN_VAL_FMS_WEIGHTING_CCIRW       3
#define RSSPECAN_VAL_FMS_WEIGHTING_A           4

#define RSSPECAN_VAL_FMS_DEEMPH_NONE           0
#define RSSPECAN_VAL_FMS_DEEMPH_25             1
#define RSSPECAN_VAL_FMS_DEEMPH_50             2
#define RSSPECAN_VAL_FMS_DEEMPH_75             3
#define RSSPECAN_VAL_FMS_DEEMPH_750            4

#define RSSPECAN_VAL_FMS_AFSP                  0
#define RSSPECAN_VAL_FMS_TDOM                  1

#define RSSPECAN_VAL_FMS_MEAS_TYPE_ADEV        0
#define RSSPECAN_VAL_FMS_MEAS_TYPE_RDEV        1
#define RSSPECAN_VAL_FMS_MEAS_TYPE_SINAD       2
#define RSSPECAN_VAL_FMS_MEAS_TYPE_THD         3
#define RSSPECAN_VAL_FMS_MEAS_TYPE_AFR         4
#define RSSPECAN_VAL_FSM_MEAS_TYPE_XTALK       5

#define RSSPECAN_VAL_FMS_RESULT_OFF            0
#define RSSPECAN_VAL_FMS_RESULT_CURR           1
#define RSSPECAN_VAL_FMS_RESULT_AVG            2
#define RSSPECAN_VAL_FMS_RESULT_MAX            3
#define RSSPECAN_VAL_FMS_RESULT_MIN            4
#define RSSPECAN_VAL_FMS_RESULT_VIEW           5

#define RSSPECAN_VAL_MIXER_LOSS_AVER           0
#define RSSPECAN_VAL_MIXER_LOSS_TABL           1

#define RSSPECAN_VAL_FMS_DISP_OFF              0

#define RSSPECAN_VAL_AUTOSCALE_ALL             0
#define RSSPECAN_VAL_AUTOSCALE_A               1
#define RSSPECAN_VAL_AUTOSCALE_B               2
#define RSSPECAN_VAL_AUTOSCALE_C               3
#define RSSPECAN_VAL_AUTOSCALE_D               4

#define RSSPECAN_VAL_VSA_COMPENSATE_IQ_OFFSET   0
#define RSSPECAN_VAL_VSA_COMPENSATE_IQ_IMBAL    1
#define RSSPECAN_VAL_VSA_COMPENSATE_ADROOP      2
#define RSSPECAN_VAL_VSA_COMPENSATE_FSK_DEV_ERR 3
#define RSSPECAN_VAL_VSA_COMPENSATE_FSK_CFD     4
#define RSSPECAN_VAL_VSA_COMPENSATE_CHANNEL     5
#define RSSPECAN_VAL_VSA_COMPENSATE_SR_ERROR    6

#define RSSPECAN_VAL_WIMAX_PREDEF_MAP_CONFIG   0
#define RSSPECAN_VAL_WIMAX_PREDEF_MAP_IQDL     1
#define RSSPECAN_VAL_WIMAX_PREDEF_MAP_IQUL     2
#define RSSPECAN_VAL_WIMAX_PREDEF_MAP_SMU      3

#define RSSPECAN_VAL_TETRA_LIM_EVM_ALL         0
#define RSSPECAN_VAL_TETRA_LIM_EVM_DATA        1
#define RSSPECAN_VAL_TETRA_LIM_EVM_DHEA        2
#define RSSPECAN_VAL_TETRA_LIM_EVM_DSP         3
#define RSSPECAN_VAL_TETRA_LIM_EVM_SPIL        4
#define RSSPECAN_VAL_TETRA_LIM_IQOF            5
#define RSSPECAN_VAL_TETRA_LIM_FERR            6
#define RSSPECAN_VAL_TETRA_LIM_PHAS            7
#define RSSPECAN_VAL_TETRA_LIM_MAGN            8

#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_NEG_3 0
#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_NEG_2 1
#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_NEG_1 2
#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_TX    3
#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_POS_1 4
#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_POS_2 5
#define RSSPECAN_VAL_TETRA_ACP_CHANNEL_NUMBER_POS_3 6

#define RSSPECAN_VAL_TETRA_ACP_MODULATION       0
#define RSSPECAN_VAL_TETRA_ACP_TRANSIENT        1

#define RSSPECAN_VAL_TETRA_ACP_BW               0
#define RSSPECAN_VAL_TETRA_ACP_RCC              1
#define RSSPECAN_VAL_TETRA_ACP_OFFS             2
#define RSSPECAN_VAL_TETRA_ACP_ABS_POW          3
#define RSSPECAN_VAL_TETRA_ACP_REL_POW          4
#define RSSPECAN_VAL_TETRA_ACP_DLIMIT_POW       5
#define RSSPECAN_VAL_TETRA_ACP_RBW              6

#define RSSPECAN_VAL_TETRA_MEAS_NONE            0
#define RSSPECAN_VAL_TETRA_MEAS_MIN             1
#define RSSPECAN_VAL_TETRA_MEAS_MAX             2
#define RSSPECAN_VAL_TETRA_MEAS_AVER            3
#define RSSPECAN_VAL_TETRA_MEAS_CURR            4

#define RSSPECAN_VAL_CARRIER_ALL                0
#define RSSPECAN_VAL_CARRIER_NUM                1

#define RSSPECAN_VAL_TETRA_UNIT_SECONDS     0
#define RSSPECAN_VAL_TETRA_UNIT_SLOTS       1

#define RSSPECAN_VAL_MEAS_PVT  0
#define RSSPECAN_VAL_MEAS_EVMS 1
#define RSSPECAN_VAL_MEAS_EVMC 2
#define RSSPECAN_VAL_MEAS_MERR 3
#define RSSPECAN_VAL_MEAS_PERR 4
#define RSSPECAN_VAL_MEAS_SPEC 5
#define RSSPECAN_VAL_MEAS_ACPM 6
#define RSSPECAN_VAL_MEAS_ACPT 7
#define RSSPECAN_VAL_MEAS_CAPT 8

#define RSSPECAN_VAL_TETRA_PVT      0
#define RSSPECAN_VAL_TETRA_EVMS     1
#define RSSPECAN_VAL_TETRA_EVMC     2
#define RSSPECAN_VAL_TETRA_MAGN     3
#define RSSPECAN_VAL_TETRA_PHAS     4
#define RSSPECAN_VAL_TETRA_FFT      5
#define RSSPECAN_VAL_TETRA_ACPM     6
#define RSSPECAN_VAL_TETRA_ACPT_UPP 7
#define RSSPECAN_VAL_TETRA_ACPT_LOW 8
#define RSSPECAN_VAL_TETRA_CSYM     9
#define RSSPECAN_VAL_TETRA_CCAR     10
#define RSSPECAN_VAL_TETRA_BSTR     11
#define RSSPECAN_VAL_TETRA_SUMM     12

#define RSSPECAN_VAL_TETRA_TRACE_CURR 1
#define RSSPECAN_VAL_TETRA_TRACE_MIN  2
#define RSSPECAN_VAL_TETRA_TRACE_PEAK 3
#define RSSPECAN_VAL_TETRA_TRACE_AVER 4

#define RSSPECAN_VAL_TETRA_MEAS_EVM_ALL  0
#define RSSPECAN_VAL_TETRA_MEAS_EVM_DSP  1
#define RSSPECAN_VAL_TETRA_MEAS_EVM_DHEA 2
#define RSSPECAN_VAL_TETRA_MEAS_EVM_DATA 3
#define RSSPECAN_VAL_TETRA_MEAS_EVM_SPIL 4
#define RSSPECAN_VAL_TETRA_MEAS_EVM_IQOF 5
#define RSSPECAN_VAL_TETRA_MEAS_EVM_GIMB 6
#define RSSPECAN_VAL_TETRA_MEAS_EVM_QUAD 7
#define RSSPECAN_VAL_TETRA_MEAS_EVM_AMPD 8
#define RSSPECAN_VAL_TETRA_MEAS_EVM_FERR 9
#define RSSPECAN_VAL_TETRA_MEAS_EVM_PHAS 10
#define RSSPECAN_VAL_TETRA_MEAS_EVM_MAG  11
#define RSSPECAN_VAL_TETRA_MEAS_EVM_BPWR 12
#define RSSPECAN_VAL_TETRA_MEAS_EVM_CRES 13

#define RSSPECAN_VAL_TETRA_PVT_REF         0
#define RSSPECAN_VAL_TETRA_PVT_SLOT        1
#define RSSPECAN_VAL_TETRA_PVT_BEFB        2
#define RSSPECAN_VAL_TETRA_PVT_REL_BEFB    3
#define RSSPECAN_VAL_TETRA_PVT_AFTB        4
#define RSSPECAN_VAL_TETRA_PVT_REL_AFTB    5
#define RSSPECAN_VAL_TETRA_PVT_TRG_TO_SYNC 6


#define RSSPECAN_VAL_SPEM_GREY RSSPECAN_VAL_SPEM_GRAY

#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK  0
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM 1
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM 2

#define RSSPECAN_VAL_MEASTYPE_RESOURCE_BLOCK_INDEXES 0
#define RSSPECAN_VAL_MEASTYPE_INBAND_EMISSION        1
#define RSSPECAN_VAL_MEASTYPE_UPPER_LIMIT_LINE       2

/* Function: Configure Spectrum Display */
#define RSSPECAN_VAL_SAN_CREATE 0
#define RSSPECAN_VAL_SAN_SELECT 1
#define RSSPECAN_VAL_SAN_DELETE 2

#define RSSPECAN_VAL_VSA_EVAL_STATISTICS 0
#define RSSPECAN_VAL_VSA_EVAL_CAPTURE    1

/* Function:Query LTE Downlink Measurement EVM Channel & Query LTE Uplink Measurement EVM Channel*/
#define RSSPECAN_VAL_CHANNEL_PSYN    0
#define RSSPECAN_VAL_CHANNEL_SSYN    1
#define RSSPECAN_VAL_CHANNEL_PBCH    2
#define RSSPECAN_VAL_CHANNEL_PCFI    3

#define RSSPECAN_VAL_LAYOUT_DIRECTION_LEFT   0
#define RSSPECAN_VAL_LAYOUT_DIRECTION_RIGHT  1
#define RSSPECAN_VAL_LAYOUT_DIRECTION_ABOWE  2
#define RSSPECAN_VAL_LAYOUT_DIRECTION_BELOW  3

#define RSSPECAN_VAL_LTE_ANT1  1
#define RSSPECAN_VAL_LTE_ANT2  2
#define RSSPECAN_VAL_LTE_ANT3  3
#define RSSPECAN_VAL_LTE_ANT4  4

#define RSSPECAN_VAL_RESULT_EVM 0
#define RSSPECAN_VAL_RESULT_POW 1

/* Function: rsspecan_Set_3GPPTAERBTSOperation */
#define RSSPECAN_VAL_3GPP_BTS_NUMBER_INSERT 0
#define RSSPECAN_VAL_3GPP_BTS_NUMBER_DELETE 1
#define RSSPECAN_VAL_3GPP_BTS_NUMBER_DELETE_ALL 2

/* Function: rsspecan_Set_3GPPTAERTableOperation */
#define RSSPECAN_VAL_3GPP_TAER_TABLE_DELETE 0
#define RSSPECAN_VAL_3GPP_TAER_TABLE_SAVE   1

/* Function: rsspecan_QueryLTEUplinkMeasurementResultSummary */
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK_AVER       0
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM_AVER      1
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM_AVER      2
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_QPSK_AVER  3
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_16QAM_AVER 4
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_AVER       5
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_AVER            6
#define RSSPECAN_VAL_LTE_UPLINK_PRACH_AVER            7
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK_MAX        8
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM_MAX       9
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM_MAX      10
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_QPSK_MAX  11
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_16QAM_MAX 12
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_MAX       13
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_MAX            14
#define RSSPECAN_VAL_LTE_UPLINK_PRACH_MAX            15
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_QPSK_MIN       16
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_16QAM_MIN      17
#define RSSPECAN_VAL_LTE_UPLINK_PUSCH_64QAM_MIN      18
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_QPSK_MIN  19
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_16QAM_MIN 20
#define RSSPECAN_VAL_LTE_UPLINK_DMRS_PUSCH_MIN       21
#define RSSPECAN_VAL_LTE_UPLINK_PUCCH_MIN            22
#define RSSPECAN_VAL_LTE_UPLINK_PRACH_MIN            23

/* Function: rsspecan_ConfigurePhaseDisplayYAxisSettings */
#define RSSPECAN_VAL_PHASE_Y_SCALE_AUTO 4

/* Function: rsspecan_ConfigurePhaseGlobalIQWindowSettings */
#define RSSPECAN_VAL_PHASE_ALL_DECADE         0
#define RSSPECAN_VAL_PHASE_PARTICULAR_DECADE  1

/* Pulse Measurement */
#define RSSPECAN_VAL_PULSE_RESULT_X_AXIS_PNUMBER 0
#define RSSPECAN_VAL_PULSE_RESULT_X_AXIS_TSTAMP  1

#define RSSPECAN_VAL_PULSE_RESULT_POWER_ALL  0
#define RSSPECAN_VAL_PULSE_RESULT_POWER_TOP  1
#define RSSPECAN_VAL_PULSE_RESULT_POWER_BASE 2
#define RSSPECAN_VAL_PULSE_RESULT_POWER_AMPL 3
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ON   4
#define RSSPECAN_VAL_PULSE_RESULT_POWER_AVG  5
#define RSSPECAN_VAL_PULSE_RESULT_POWER_MIN  6
#define RSSPECAN_VAL_PULSE_RESULT_POWER_MAX  7
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PON  8
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PAVG 9
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PMIN 10
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ADP  11
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ADDB 12
#define RSSPECAN_VAL_PULSE_RESULT_POWER_RPER 13
#define RSSPECAN_VAL_PULSE_RESULT_POWER_RDB  14
#define RSSPECAN_VAL_PULSE_RESULT_POWER_OPER 15
#define RSSPECAN_VAL_PULSE_RESULT_POWER_ODB  16
#define RSSPECAN_VAL_PULSE_RESULT_POWER_POIN 17
#define RSSPECAN_VAL_PULSE_RESULT_POWER_PPR  18

#define RSSPECAN_VAL_PULSE_RESULT_TIMING_ALL  0
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_TST  1
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_SETT 2
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_RISE 3
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_FALL 4
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_PWID 5
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_OFF  6
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_DRAT 7
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_DCYC 8
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_PRI  9
#define RSSPECAN_VAL_PULSE_RESULT_TIMING_PRF  10

#define RSSPECAN_VAL_PULSE_RESULT_FREQ_ALL  0
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_POIN 1
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_PPFR 2
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_RERR 3
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_PERR 4
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_DEV  5
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_CRAT 6

#define RSSPECAN_VAL_PULSE_RESULT_PHASE_ALL  0
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_POIN 1
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_PPPH 2
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_RERR 3
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_PERR 4
#define RSSPECAN_VAL_PULSE_RESULT_PHASE_DEV  5

#define RSSPECAN_VAL_PULSE_MEASTYPE_CURR 0
#define RSSPECAN_VAL_PULSE_MEASTYPE_MIN  1
#define RSSPECAN_VAL_PULSE_MEASTYPE_MAX  2
#define RSSPECAN_VAL_PULSE_MEASTYPE_AVER 3
#define RSSPECAN_VAL_PULSE_MEASTYPE_SDEV 4

#define RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_HZ_NS  0
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_KHZ_US 1
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_MHZ_MS 2
#define RSSPECAN_VAL_PULSE_RESULT_FREQ_SCALING_GHZ_S  3

#define RSSPECAN_VAL_PULSE_QUERY_RANGE_SELECTED 0
#define RSSPECAN_VAL_PULSE_QUERY_RANGE_CURRENT  1
#define RSSPECAN_VAL_PULSE_QUERY_RANGE_ALL      2

#define RSSPECAN_VAL_CHANNEL_TYPE_SPECTRUM 0
#define RSSPECAN_VAL_CHANNEL_TYPE_IQ       1
#define RSSPECAN_VAL_CHANNEL_TYPE_PULSE    2
#define RSSPECAN_VAL_CHANNEL_TYPE_ADEM     3
#define RSSPECAN_VAL_CHANNEL_TYPE_MCGD     4
#define RSSPECAN_VAL_CHANNEL_TYPE_NOISE    5
#define RSSPECAN_VAL_CHANNEL_TYPE_PNOISE   6
#define RSSPECAN_VAL_CHANNEL_TYPE_VSA      7
#define RSSPECAN_VAL_CHANNEL_TYPE_3GPP_BTS 8
#define RSSPECAN_VAL_CHANNEL_TYPE_3GPP_UE  9
#define RSSPECAN_VAL_CHANNEL_TYPE_BC2K     10
#define RSSPECAN_VAL_CHANNEL_TYPE_MC2K     11
#define RSSPECAN_VAL_CHANNEL_TYPE_MDO      12
#define RSSPECAN_VAL_CHANNEL_TYPE_BDO      13
#define RSSPECAN_VAL_CHANNEL_TYPE_GSM      14
#define RSSPECAN_VAL_CHANNEL_TYPE_WLAN     15
#define RSSPECAN_VAL_CHANNEL_TYPE_LTE      16

#define RSSPECAN_VAL_NOISE_TABLE_SELECT 0
#define RSSPECAN_VAL_NOISE_TABLE_DELETE 1

#define RSSPECAN_VAL_PULSE_COLLUMNS_SELECTED 0
#define RSSPECAN_VAL_PULSE_COLLUMNS_ALL      1

#define RSSPECAN_VAL_ADEM_PRESET_LOAD    0
#define RSSPECAN_VAL_ADEM_PRESET_STORE   1
#define RSSPECAN_VAL_ADEM_PRESET_RESTORE 2

#define RSSPECAN_VAL_NOISE_VSWR        0
#define RSSPECAN_VAL_NOISE_RETURN_LOSS 1

#define RSSPECAN_VAL_NOISE_INPUT  0
#define RSSPECAN_VAL_NOISE_OUTPUT 1

#define RSSPECAN_VAL_WLAN_ITEM_TX_ALL                  0
#define RSSPECAN_VAL_WLAN_ITEM_TX_IQ_OFFSET            1
#define RSSPECAN_VAL_WLAN_ITEM_TX_QUAD_OFFSET          2
#define RSSPECAN_VAL_WLAN_ITEM_TX_CREST_FACTOR         3
#define RSSPECAN_VAL_WLAN_ITEM_TX_GAIN_IMBALANCE       4
#define RSSPECAN_VAL_WLAN_ITEM_TX_PPDU_POWER           5
#define RSSPECAN_VAL_WLAN_ITEM_RX_ALL                  6
#define RSSPECAN_VAL_WLAN_ITEM_RX_PPDU_POWER           7
#define RSSPECAN_VAL_WLAN_ITEM_RX_CREST_FACTOR         8
#define RSSPECAN_VAL_WLAN_ITEM_STREAM_ALL              9
#define RSSPECAN_VAL_WLAN_ITEM_STREAM_BER_PILOT        10
#define RSSPECAN_VAL_WLAN_ITEM_STREAM_EVM_DATA         11
#define RSSPECAN_VAL_WLAN_ITEM_STREAM_EVM_ALL          12
#define RSSPECAN_VAL_WLAN_ITEM_STREAM_EVM_PILOT        13
#define RSSPECAN_VAL_WLAN_ITEM_PILOT_BIT_ERROR         14
#define RSSPECAN_VAL_WLAN_ITEM_EVM_ALL                 15
#define RSSPECAN_VAL_WLAN_ITEM_EVM_DATA                16
#define RSSPECAN_VAL_WLAN_ITEM_EVM_PILOT               17
#define RSSPECAN_VAL_WLAN_ITEM_CENTER_FREQ_ERROR       18
#define RSSPECAN_VAL_WLAN_ITEM_SYMBOL_CLOCK_ERROR      19
#define RSSPECAN_VAL_WLAN_ITEM_PILOT_VECTOR_ERROR_DSSS 20
#define RSSPECAN_VAL_WLAN_ITEM_PPDU_EVM_DSSS           21
#define RSSPECAN_VAL_WLAN_ITEM_IQ_OFFSET_DSSS          22
#define RSSPECAN_VAL_WLAN_ITEM_GAIN_IMBALANCE_DSSS     23
#define RSSPECAN_VAL_WLAN_ITEM_QUADRATURE_ERROR_DSSS   24
#define RSSPECAN_VAL_WLAN_ITEM_CENTER_FREQ_ERROR_DSSS  25
#define RSSPECAN_VAL_WLAN_ITEM_CHIP_CLOCK_ERROR_DSSS   26
#define RSSPECAN_VAL_WLAN_ITEM_RISE_TIME_DSSS          27
#define RSSPECAN_VAL_WLAN_ITEM_FALL_TIME_DSSS          28
#define RSSPECAN_VAL_WLAN_ITEM_MEAN_POWER_DSSS         29
#define RSSPECAN_VAL_WLAN_ITEM_PEAK_POWER_DSSS         30
#define RSSPECAN_VAL_WLAN_ITEM_CREST_FACTOR_DSSS       31

#define RSSPECAN_VAL_INT_GAIN  0
#define RSSPECAN_VAL_INT_NOISE 1

#define RSSPECAN_VAL_MESSAGE_TYPE_ALL     0
#define RSSPECAN_VAL_MESSAGE_TYPE_INFO    1
#define RSSPECAN_VAL_MESSAGE_TYPE_WARNING 2
#define RSSPECAN_VAL_MESSAGE_TYPE_ERROR   3
#define RSSPECAN_VAL_MESSAGE_TYPE_FATAL   4
#define RSSPECAN_VAL_MESSAGE_TYPE_MESSAGE 5

#define RSSPECAN_VAL_SYNC_TO_PCCPCH   0
#define RSSPECAN_VAL_SYNC_TO_MIDAMBLE 1

#define RSSPECAN_VAL_WLAN_CURRENT 0
#define RSSPECAN_VAL_WLAN_ALL     1

#define RSSPECAN_VAL_WLAN_MEAS_ACPR 0
#define RSSPECAN_VAL_WLAN_MEAS_MASK 1
#define RSSPECAN_VAL_WLAN_MEAS_OBAN 2
#define RSSPECAN_VAL_WLAN_MEAS_CCDF 3

#define RSSPECAN_VAL_MEAS_POW_PPOW 6

#define RSSPECAN_VAL_WLAN_PSDU_FBUR  0
#define RSSPECAN_VAL_WLAN_PSDU_ALL   1
#define RSSPECAN_VAL_WLAN_PSDU_MEAS  2
#define RSSPECAN_VAL_WLAN_PSDU_DEMOD 3

#define RSSPECAN_VAL_LTE_UPLINK_PRACH_SEQ_INDEX_MANUAL 0

#define RSSPECAN_VAL_PULSE_RESULT_SCALE_OFF  0
#define RSSPECAN_VAL_PULSE_RESULT_SCALE_ON   1
#define RSSPECAN_VAL_PULSE_RESULT_SCALE_ONCE 2

#define RSSPECAN_VAL_WLAN_PPDU_LONG  0
#define RSSPECAN_VAL_WLAN_PPDU_SHORT 1

#define RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_COUNT     0
#define RSSPECAN_VAL_PULSE_RESULT_Y_AXIS_OCCURANCE 1

#define RSSPECAN_VAL_GSM_K10_SLOT_SCOPE   0
#define RSSPECAN_VAL_GSM_K10_SLOT_MEASURE 1

#define RSSPECAN_VAL_GROUP_X 0
#define RSSPECAN_VAL_GROUP_Y 1

#define RSSPECAN_VAL_VSA_DATA_SIGNAL_MEAS 0
#define RSSPECAN_VAL_VSA_DATA_SIGNAL_REF  1

#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_GAIN  0
#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_NOISE 1
#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_PCOLD 2
#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_PHOT  3
#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_TEMP  4
#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_YFAC  5
#define RSSPECAN_VAL_NOISE_LIM_LINE_TYPE_NUNC  6

#define RSSPECAN_VAL_NOISE_FETC_FIGURE      0
#define RSSPECAN_VAL_NOISE_FETC_TEMPERATURE 1
#define RSSPECAN_VAL_NOISE_FETC_GAIN        2

/* Fast Mode */
#define RSSPECAN_VAL_FAST_SWEEP_NORMAL 0
#define RSSPECAN_VAL_FAST_SWEEP_FAST   1
/****************************************************************************
 *------------------------ Error And Completion Codes ----------------------*
 ****************************************************************************/

#define RSSPECAN_WARN_MEASURE_UNCALIBRATED    (RS_CLASS_WARN_BASE + 0x0001L)
#define RSSPECAN_WARN_OVER_RANGE              (RS_CLASS_WARN_BASE + 0x0002L)

#define RSSPECAN_ERROR_MARKER_NOT_ENABLED     (RS_CLASS_ERROR_BASE + 0x0003L)
#define RSSPECAN_ERROR_NOT_DELTA_MARKER       (RS_CLASS_ERROR_BASE + 0x0002L)
#define RSSPECAN_ERROR_MAX_TIME_EXCEEDED      (RS_CLASS_ERROR_BASE + 0x0001L)

#define RSSPECAN_WARNMSG_MEASURE_UNCALIBRATED "Uncalibrated measurement"
#define RSSPECAN_WARNMSG_OVER_RANGE           "Measurement overrange"

#define RSSPECAN_ERRMSG_MARKER_NOT_ENABLED    "Marker Not Enabled"
#define RSSPECAN_ERRMSG_NOT_DELTA_MARKER      "Not Delta Marker"
#define RSSPECAN_ERRMSG_MAX_TIME_EXCEEDED     "Max Time Exceeded"

#define RSSPECAN_ERROR_CODES_AND_MSGS \
    {RSSPECAN_WARN_MEASURE_UNCALIBRATED,  RSSPECAN_WARNMSG_MEASURE_UNCALIBRATED},\
    {RSSPECAN_WARN_OVER_RANGE,            RSSPECAN_WARNMSG_OVER_RANGE},\
    {RSSPECAN_ERROR_MARKER_NOT_ENABLED,   RSSPECAN_ERRMSG_MARKER_NOT_ENABLED},\
    {RSSPECAN_ERROR_NOT_DELTA_MARKER,     RSSPECAN_ERRMSG_NOT_DELTA_MARKER},\
    {RSSPECAN_ERROR_MAX_TIME_EXCEEDED,    RSSPECAN_ERRMSG_MAX_TIME_EXCEEDED}

/****************************************************************************
 *---------------- Instrument Driver Function Declarations -----------------*
 ****************************************************************************/
ViStatus _VI_FUNC rsspecan_init (ViRsrc resourceName, ViBoolean IDQuery,
                                 ViBoolean resetDevice,
                                 ViSession* instrumentHandle);
ViStatus _VI_FUNC rsspecan_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
                                            ViBoolean resetDevice,
                                            ViString optionString,
                                            ViSession* instrumentHandle);
ViStatus _VI_FUNC rsspecan_SetSANMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrumDisplay (ViSession instrumentHandle,
                                                     ViInt32 operation,
                                                     ViInt32 spectrumDisplay);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStartStop (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViReal64 startFrequency,
                                                        ViReal64 stopFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCenter (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViReal64 centerFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCenterSpan (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViReal64 centerFrequency,
                                                         ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStepSize (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViReal64 stepSize);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyStepSizeAuto
             (ViSession instrumentHandle, ViInt32 window, ViBoolean stepSizeAuto);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyCouplingFactor
             (ViSession instrumentHandle, ViInt32 window, ViInt32 coupling,
              ViInt32 couplingFactor);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencySpanFull (ViSession instrumentHandle,
                                                       ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyZeroSpan (ViSession instrumentHandle,
                                                       ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyOffset (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViReal64 frequencyOffset);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMode (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 frequencyMode);
ViStatus _VI_FUNC rsspecan_ConfigureSweepTime (ViSession instrumentHandle,
                                               ViInt32 window,
                                               ViBoolean sweepTimeAuto,
                                               ViReal64 sweepTime);
ViStatus _VI_FUNC rsspecan_ConfigureSweepCouplingAuto (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 sweepCoupling);
ViStatus _VI_FUNC rsspecan_ConfigureSweepCoupling (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 sweepCoupling,
                                                   ViReal64 couplingValue);
ViStatus _VI_FUNC rsspecan_ConfigureSweepCouplingAdvanced
             (ViSession instrumentHandle, ViInt32 window, ViInt32 ratioSelection,
              ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureMainPLLBwid (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 PLL);
ViStatus _VI_FUNC rsspecan_ConfigureFilterType (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 filterType);
ViStatus _VI_FUNC rsspecan_ConfigureVBWMode (ViSession instrumentHandle,
                                             ViInt32 window,
                                             ViInt32 videoFilterType);
ViStatus _VI_FUNC rsspecan_ConfigureFFTFilterMode (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 FFTFilterMode);
ViStatus _VI_FUNC rsspecan_ConfigureAveraging (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 trace,
                                               ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureAveragingType (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 averagingType);
ViStatus _VI_FUNC rsspecan_ConfigureAveragingCount (ViSession instrumentHandle,
                                                    ViInt32 window, ViInt32 count);
ViStatus _VI_FUNC rsspecan_ConfigureAcquisition (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViBoolean sweepModeContinuous,
                                                 ViInt32 numberOfSweeps);
ViStatus _VI_FUNC rsspecan_ConfigureVerticalScale (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 verticalScale);
ViStatus _VI_FUNC rsspecan_ConfigureHorizontalScale (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 horizontalScale);
ViStatus _VI_FUNC rsspecan_ConfigureSweepPoints (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 sweepPoints);
ViStatus _VI_FUNC rsspecan_QuerySweepPoints (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32* sweepPoints);
ViStatus _VI_FUNC rsspecan_ConfigureSweepOptimization (ViSession instrumentHandle,
                                                       ViInt32 sweepOptimization);
ViStatus _VI_FUNC rsspecan_ConfigureSweepType (ViSession instrumentHandle,
                                               ViInt32 sweepType);
ViStatus _VI_FUNC rsspecan_ConfigureAttenuation (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViBoolean attenuationAuto,
                                                 ViReal64 attenuation);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevel (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViReal64 referenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevelOffset (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViReal64 referenceLevelOffset);
ViStatus _VI_FUNC rsspecan_ConfigureLevel (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32 amplitudeUnits,
                                           ViReal64 inputImpedance,
                                           ViReal64 referenceLevel,
                                           ViReal64 referenceLevelOffset,
                                           ViBoolean attenuationAuto,
                                           ViReal64 attenuation);
ViStatus _VI_FUNC rsspecan_ConfigureAttenuationMode (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureParameterCoupling (ViSession instrumentHandle,
                                                       ViInt32 parameterCoupling);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceLevelCoupling
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean coupledReferenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureIFOutputSource (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 outputSource);
ViStatus _VI_FUNC rsspecan_ConfigureIFOutFrequency (ViSession instrumentHandle,
                                                    ViReal64 IFOutFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseSource (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViBoolean noiseSource);
ViStatus _VI_FUNC rsspecan_ConfigureUnitPower (ViSession instrumentHandle,
                                               ViInt32 unitPower, ViInt32 window);
ViStatus _VI_FUNC rsspecan_AddMeasurementChannel (ViSession instrumentHandle,
                                                  ViInt32 channelType,
                                                  ViString channelName);
ViStatus _VI_FUNC rsspecan_SelectMeasurementChannel (ViSession instrumentHandle,
                                                     ViString channelName);
ViStatus _VI_FUNC rsspecan_DuplicateMeasurementChannel (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ReplaceMeasurementChannel (ViSession instrumentHandle,
                                                      ViString oldChannelName,
                                                      ViInt32 newChannelType,
                                                      ViString newChannelName);
ViStatus _VI_FUNC rsspecan_RenameMeasurementChannel (ViSession instrumentHandle,
                                                     ViString oldChannelName,
                                                     ViString newChannelName);
ViStatus _VI_FUNC rsspecan_DeleteMeasurementChannel (ViSession instrumentHandle,
                                                     ViString channelName);
ViStatus _VI_FUNC rsspecan_PresetMeasurementChannel (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_QueryAllMeasurementChannels (ViSession instrumentHandle,
                                                        ViInt32 arraySize,
                                                        ViInt32 _VI_FAR channelType[],
                                                        ViChar _VI_FAR channelName[],
                                                        ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ConfigureTrace (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32 trace,
                                           ViInt32 traceType);
ViStatus _VI_FUNC rsspecan_ConfigureTraceDetector (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 trace,
                                                   ViBoolean detectorTypeAuto,
                                                   ViInt32 detectorType);
ViStatus _VI_FUNC rsspecan_ConfigureTraceResetBehavior (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 trace,
                                                        ViBoolean resetAtChange);
ViStatus _VI_FUNC rsspecan_StoreTraceToFile (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32 trace,
                                             ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureTraceExport (ViSession instrumentHandle,
                                                 ViInt32 exportMode,
                                                 ViBoolean exportHeader);
ViStatus _VI_FUNC rsspecan_TraceClear (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_TraceIQSet (ViSession instrumentHandle, ViInt32 window,
                                       ViReal64 samplingRate, ViInt32 triggerMode,
                                       ViInt32 triggerSlope, ViReal64 bandwidth,
                                       ViInt32 pretriggerSamples,
                                       ViInt32 noOfSamples);
ViStatus _VI_FUNC rsspecan_TraceIQSamplingRate (ViSession instrumentHandle,
                                                ViInt32 window,
                                                ViReal64 samplingRate);
ViStatus _VI_FUNC rsspecan_TraceIQMaximumBandwidthExtension
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_TraceIQRecordLength (ViSession instrumentHandle,
                                                ViInt32 recordLength);
ViStatus _VI_FUNC rsspecan_TraceIQAveraging (ViSession instrumentHandle,
                                             ViInt32 window, ViBoolean averageState,
                                             ViInt32 averageCount);
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataAnalyzerEnabled
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean IQDataAnalyzerEnabled);
ViStatus _VI_FUNC rsspecan_TraceIQExtendedBandwidth (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 filter);
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataAcquisition
             (ViSession instrumentHandle, ViInt32 window, ViBoolean traceState);
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQStartSynchronization
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean startSynchronization);
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQBWExtension (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean BWExtensionState);
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQGate (ViSession instrumentHandle,
                                                 ViBoolean gate, ViInt32 type,
                                                 ViInt32 gateLength,
                                                 ViInt32 gateGap,
                                                 ViInt32 numberOfGatePeriods);
ViStatus _VI_FUNC rsspecan_ConfigureTraceIQDataDisplayType
             (ViSession instrumentHandle, ViInt32 displayType);
ViStatus _VI_FUNC rsspecan_QueryTraceIQFilterBandwidth (ViSession instrumentHandle,
                                                        ViReal64* filterBandwidth);
ViStatus _VI_FUNC rsspecan_LoadIQData (ViSession instrumentHandle,
                                       ViString fileName);
ViStatus _VI_FUNC rsspecan_StoreIQData (ViSession instrumentHandle,
                                        ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureIQDataExportFileDescription
             (ViSession instrumentHandle, ViString description);
ViStatus _VI_FUNC rsspecan_QueryTPISTimeOffset (ViSession instrumentHandle,
                                                ViInt32 trace, ViInt32* timeOffset);
ViStatus _VI_FUNC rsspecan_CopyTrace (ViSession instrumentHandle, ViInt32 window,
                                      ViInt32 destinationTrace,
                                      ViInt32 sourceTrace);
ViStatus _VI_FUNC rsspecan_ConfigureSubtractTraces (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViReal64 position, ViInt32 mode,
                                                    ViBoolean traceMathState);
ViStatus _VI_FUNC rsspecan_SubtractTraces (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32 trace2);
ViStatus _VI_FUNC rsspecan_ConfigureMarker (ViSession instrumentHandle,
                                            ViInt32 window, ViInt32 marker,
                                            ViBoolean markerEnabled, ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceMarker (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 marker,
                                                     ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureSignalTrack (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViBoolean signalTrackEnabled,
                                                 ViReal64 signalTrackBandwidth,
                                                 ViReal64 signalTrackThreshold,
                                                 ViInt32 trace);
ViStatus _VI_FUNC rsspecan_DisableAllMarkers (ViSession instrumentHandle,
                                              ViInt32 window);
ViStatus _VI_FUNC rsspecan_MarkerSearch (ViSession instrumentHandle, ViInt32 window,
                                         ViInt32 marker, ViInt32 markerSearch);
ViStatus _VI_FUNC rsspecan_MarkerPeakAuto (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32 markerSearch,
                                           ViBoolean autoPeak);
ViStatus _VI_FUNC rsspecan_MoveMarker (ViSession instrumentHandle, ViInt32 window,
                                       ViInt32 marker, ViReal64 markerPosition);
ViStatus _VI_FUNC rsspecan_SetInstrumentFromMarker (ViSession instrumentHandle,
                                                    ViInt32 window, ViInt32 marker,
                                                    ViInt32 instrumentSetting);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerZoom (ViSession instrumentHandle,
                                                ViInt32 window, ViReal64 markerZoom,
                                                ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_LinkMarkerAndDeltamarker (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 marker,
                                                     ViBoolean state);
ViStatus _VI_FUNC rsspecan_LinkMarkerToAnotherMarker (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 firstMarker,
                                                      ViInt32 secondMarker,
                                                      ViBoolean state);
ViStatus _VI_FUNC rsspecan_LinkAllMarkers (ViSession instrumentHandle,
                                           ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryMarker (ViSession instrumentHandle, ViInt32 window,
                                        ViInt32 marker, ViReal64* markerPosition,
                                        ViReal64* markerAmplitude);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerStepSize (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 stepSize);
ViStatus _VI_FUNC rsspecan_ProbabilityMarkerPosition (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 marker,
                                                      ViReal64 probability);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerBandPower (ViSession instrumentHandle,
                                                     ViBoolean bandPowerEnabled,
                                                     ViInt32 displayMode,
                                                     ViReal64 span);
ViStatus _VI_FUNC rsspecan_QueryMarkerBandPower (ViSession instrumentHandle,
                                                 ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchThreshold
             (ViSession instrumentHandle, ViInt32 window, ViBoolean thresholdState,
              ViReal64 markerThreshold);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchPeakExcursion
             (ViSession instrumentHandle, ViInt32 window, ViReal64 peakExcursion);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLocalOscillator
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean localOscillatorSupression);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLimits (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean searchLimits,
                                                        ViReal64 searchLimitLeft,
                                                        ViReal64 searchLimitRight);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerSearchLimitsZoom
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean searchLimitsZoom);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerDemodulation (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 marker,
                                                        ViBoolean state,
                                                        ViInt32 demodulationType,
                                                        ViReal64 markerStopTime,
                                                        ViBoolean continuousDemodulation,
                                                        ViBoolean squelchState,
                                                        ViReal64 squelchLevel);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListAnnotation
             (ViSession instrumentHandle, ViInt32 window, ViBoolean label,
              ViBoolean marker);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListState (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakList (ViSession instrumentHandle,
                                                    ViInt32 window, ViInt32 marker,
                                                    ViInt32 peakListCount,
                                                    ViInt32 peakListSort,
                                                    ViInt32 peakListSize,
                                                    ViBoolean automaticPeaksSearch);
ViStatus _VI_FUNC rsspecan_ActivateMarkerPeakSearch (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 marker,
                                                     ViBoolean peakSearch);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPeakListSettings
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViBoolean peakLabels, ViInt32 maximumPeaks);
ViStatus _VI_FUNC rsspecan_StoreMarkerPeakList (ViSession instrumentHandle,
                                                ViString path);
ViStatus _VI_FUNC rsspecan_QueryMarkerPeakListFound (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 marker,
                                                     ViInt32* peakListFound);
ViStatus _VI_FUNC rsspecan_QueryMarkerPeakList (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 marker,
                                                ViInt32 arraySize,
                                                ViInt32 peakListSelection,
                                                ViReal64 _VI_FAR peakList[]);
ViStatus _VI_FUNC rsspecan_MarkerSaveToFile (ViSession instrumentHandle,
                                             ViString path);
ViStatus _VI_FUNC rsspecan_ReadMarkerPeakList (ViSession instrumentHandle,
                                               ViInt32 noOfValues,
                                               ViReal64 _VI_FAR frequency[],
                                               ViReal64 _VI_FAR level[],
                                               ViReal64 _VI_FAR deltaLimit[],
                                               ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarker (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 deltaMarker,
                                                 ViBoolean state, ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarkerPosition (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 deltaMarker,
                                                         ViInt32 mode,
                                                         ViReal64 position);
ViStatus _VI_FUNC rsspecan_DisableAllDeltaMarkers (ViSession instrumentHandle,
                                                   ViInt32 window);
ViStatus _VI_FUNC rsspecan_DeltaMarkerSearch (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 deltaMarker,
                                              ViInt32 markerSearch);
ViStatus _VI_FUNC rsspecan_LinkDeltaMarkerToMarker (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 firstMarker,
                                                    ViInt32 secondMarker,
                                                    ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryDeltaMarker (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32 deltaMarker,
                                             ViInt32 mode, ViReal64* position,
                                             ViReal64* amplitude);
ViStatus _VI_FUNC rsspecan_ConfigureDeltaMarkerBandPower (ViSession instrumentHandle,
                                                          ViInt32 displayMode,
                                                          ViBoolean bandPowerEnabled,
                                                          ViReal64 span);
ViStatus _VI_FUNC rsspecan_QueryDeltaMarkerBandPower (ViSession instrumentHandle,
                                                      ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceFixed (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViBoolean fixedReference);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceFixedPoint (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViReal64 refPointLevel,
                                                         ViReal64 refPointLevelOffset,
                                                         ViReal64 refPointFrequencyTime);
ViStatus _VI_FUNC rsspecan_ReferenceFixedPeakSearch (ViSession instrumentHandle,
                                                     ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceOscillator (ViSession instrumentHandle,
                                                         ViInt32 source,
                                                         ViReal64 externalFrequency);
ViStatus _VI_FUNC rsspecan_QuerySelectedReferenceOscillatorSource
             (ViSession instrumentHandle, ViInt32* source);
ViStatus _VI_FUNC rsspecan_ConfigureReferenceOscillatorExternalPLLBandwidth
             (ViSession instrumentHandle, ViInt32 bandwidth);
ViStatus _VI_FUNC rsspecan_SpeakerVolume (ViSession instrumentHandle,
                                          ViReal64 speakerVolume);
ViStatus _VI_FUNC rsspecan_ApplicationSetupRecovery (ViSession instrumentHandle,
                                                     ViBoolean applicationSetupRecovery);
ViStatus _VI_FUNC rsspecan_ConfigureIFShift (ViSession instrumentHandle,
                                             ViInt32 IFShift,
                                             ViReal64 IFShiftFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureRemoteLoggingState (ViSession instrumentHandle,
                                                        ViBoolean state);
ViStatus _VI_FUNC rsspecan_SystemShutdown (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_QueryFrontendTemperature (ViSession instrumentHandle,
                                                     ViReal64* frontendTemperature);
ViStatus _VI_FUNC rsspecan_ConfigureTriggerSource (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 triggerSource);
ViStatus _VI_FUNC rsspecan_ConfigureTVRFPTriggerSource (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 triggerSource);
ViStatus _VI_FUNC rsspecan_ConfigureTrigger (ViSession instrumentHandle,
                                             ViInt32 window, ViReal64 triggerDelay,
                                             ViInt32 triggerPolarity);
ViStatus _VI_FUNC rsspecan_ConfigureTriggerDropoutTime (ViSession instrumentHandle,
                                                        ViReal64 dropoutTime);
ViStatus _VI_FUNC rsspecan_ConfigureExternalTrigger (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViReal64 externalTriggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureExternalTriggerPort (ViSession instrumentHandle,
                                                         ViInt32 port,
                                                         ViReal64 externalTriggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureDelayCompensation (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViBoolean delayCompensation);
ViStatus _VI_FUNC rsspecan_ConfigureVideoTrigger (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViReal64 videoTriggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureIFPowerTrigger (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViReal64 triggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureIFPowerTriggerParameters
             (ViSession instrumentHandle, ViInt32 window, ViReal64 triggerLevel,
              ViReal64 holdoff, ViReal64 hysteresis);
ViStatus _VI_FUNC rsspecan_ConfigureIQPowerTrigger (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViReal64 triggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureRFPowerTrigger (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViReal64 triggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureRFPowerTriggerHoldoff
             (ViSession instrumentHandle, ViInt32 window, ViReal64 holdoff);
ViStatus _VI_FUNC rsspecan_ConfigureTimeTrigger (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViReal64 timeInterval);
ViStatus _VI_FUNC rsspecan_ConfigureTVTrigger (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 lineSystem,
                                               ViInt32 synchronization,
                                               ViInt32 horizontalSyncLine,
                                               ViInt32 polarity,
                                               ViBoolean TVFreeRunState);
ViStatus _VI_FUNC rsspecan_QueryTriggerPositionInSample (ViSession instrumentHandle,
                                                         ViInt32* triggerPositionInSample);
ViStatus _VI_FUNC rsspecan_ConfigureTriggerInput (ViSession instrumentHandle,
                                                  ViInt32 port);
ViStatus _VI_FUNC rsspecan_ConfigureTriggerOutput (ViSession instrumentHandle,
                                                   ViInt32 port, ViInt32 outputType,
                                                   ViInt32 level, ViReal64 length);
ViStatus _VI_FUNC rsspecan_GenerateTriggerPulse (ViSession instrumentHandle,
                                                 ViInt32 port);
ViStatus _VI_FUNC rsspecan_FrequencyMaskWindow (ViSession instrumentHandle,
                                                ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskTriggerSource
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMask (ViSession instrumentHandle,
                                                   ViString name, ViString comment);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskDirectory
             (ViSession instrumentHandle, ViString directory);
ViStatus _VI_FUNC rsspecan_DeleteFrequencyMask (ViSession instrumentHandle,
                                                ViString name);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskCondition
             (ViSession instrumentHandle, ViInt32 condition);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskShape (ViSession instrumentHandle,
                                                        ViInt32 arraySize,
                                                        ViInt32 mode,
                                                        ViReal64 _VI_FAR lower[],
                                                        ViReal64 _VI_FAR upper[]);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskAuto (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskShift (ViSession instrumentHandle,
                                                        ViInt32 type,
                                                        ViReal64 frequency,
                                                        ViReal64 amplitude);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskState (ViSession instrumentHandle,
                                                        ViInt32 type,
                                                        ViBoolean frequencyMask);
ViStatus _VI_FUNC rsspecan_ConfigureFrequencyMaskSpan (ViSession instrumentHandle,
                                                       ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGate (ViSession instrumentHandle,
                                                  ViInt32 window, ViBoolean gating,
                                                  ViInt32 gateSource, ViInt32 mode,
                                                  ViInt32 polarity, ViReal64 delay,
                                                  ViReal64 length);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGateTrace (ViSession instrumentHandle,
                                                       ViInt32 trace, ViInt32 range,
                                                       ViBoolean traceState,
                                                       ViString comment,
                                                       ViReal64 traceStart,
                                                       ViReal64 traceStop,
                                                       ViReal64 periodLength);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerEnabled (ViSession instrumentHandle,
                                                          ViBoolean externalMixingEnabled);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerNumberOfPorts
             (ViSession instrumentHandle, ViInt32 numberOfPorts);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixer (ViSession instrumentHandle,
                                                   ViInt32 band, ViInt32 harmonic,
                                                   ViInt32 harmonicType);
ViStatus _VI_FUNC rsspecan_QueryExternalMixerRFStartStop (ViSession instrumentHandle,
                                                          ViReal64* RFStart,
                                                          ViReal64* RFStop);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerHandoverFrequency
             (ViSession instrumentHandle, ViReal64 handoverFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerRFOverrange
             (ViSession instrumentHandle, ViBoolean RFOverrange);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerPresetBand
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerHighHarmonic
             (ViSession instrumentHandle, ViBoolean highHarmonic,
              ViInt32 harmonicOrder);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerSignalIDMode
             (ViSession instrumentHandle, ViInt32 mode, ViReal64 autoIDThreshold);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerBias (ViSession instrumentHandle,
                                                       ViInt32 range,
                                                       ViReal64 bias);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerConversionLoss
             (ViSession instrumentHandle, ViInt32 range, ViInt32 conversionLossMode,
              ViReal64 conversionLossAverage, ViString conversionLossTable);
ViStatus _VI_FUNC rsspecan_ConfigureExternalMixerLowPower
             (ViSession instrumentHandle, ViReal64 lowPower);
ViStatus _VI_FUNC rsspecan_CreateExternalMixerConversionLossTable
             (ViSession instrumentHandle, ViInt32 window, ViString tableName,
              ViString mixerType, ViInt32 band, ViString serialNumber,
              ViInt32 harmonic, ViInt32 ports, ViReal64 bias, ViString comment,
              ViInt32 numberOfValues, ViReal64 _VI_FAR frequencyValues[],
              ViReal64 _VI_FAR conversionLossValues[]);
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableSelect
             (ViSession instrumentHandle, ViInt32 window, ViString tableName);
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableDelete
             (ViSession instrumentHandle, ViInt32 window, ViString tableName);
ViStatus _VI_FUNC rsspecan_ExternalMixerConversionLossTableCatalog
             (ViSession instrumentHandle, ViInt32* numberOfCVL, ViInt32 bufferSize,
              ViChar _VI_FAR CVLNamesList[]);
ViStatus _VI_FUNC rsspecan_ConfigureRFInputCoupling (ViSession instrumentHandle,
                                                     ViInt32 RFInputCoupling);
ViStatus _VI_FUNC rsspecan_ConfigureElectronicAttenuator (ViSession instrumentHandle,
                                                          ViInt32 input,
                                                          ViInt32 mode,
                                                          ViInt32 manualValue);
ViStatus _VI_FUNC rsspecan_ConfigureInputMixerLevel (ViSession instrumentHandle,
                                                     ViInt32 input,
                                                     ViBoolean automaticSetup,
                                                     ViReal64 power);
ViStatus _VI_FUNC rsspecan_ConfigurePreamplifierState (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigurePreamplifierLevel (ViSession instrumentHandle,
                                                       ViInt32 preamplifierLevel);
ViStatus _VI_FUNC rsspecan_ConfigureYIGFilter (ViSession instrumentHandle,
                                               ViInt32 window, ViBoolean YIGFilter);
ViStatus _VI_FUNC rsspecan_ConfigureHighpassFilter (ViSession instrumentHandle,
                                                    ViBoolean highpassFilter);
ViStatus _VI_FUNC rsspecan_InputProtectionReset (ViSession instrumentHandle,
                                                 ViInt32 window);
ViStatus _VI_FUNC rsspecan_CreateLimitLine (ViSession instrumentHandle,
                                            ViInt32 window, ViInt32 limit,
                                            ViString name, ViInt32 domain,
                                            ViString comment, ViInt32 assignToTrace,
                                            ViBoolean deleteExistingLimitLine);
ViStatus _VI_FUNC rsspecan_ConfigureLimitLine (ViSession instrumentHandle,
                                               ViInt32 limit, ViInt32 type,
                                               ViInt32 units,
                                               ViInt32 xAxisInterpolation,
                                               ViInt32 yAxisInterpolation,
                                               ViInt32 xAxisScaling,
                                               ViReal64 margin, ViReal64 threshold);
ViStatus _VI_FUNC rsspecan_DefineLimitLine (ViSession instrumentHandle,
                                            ViInt32 limit, ViInt32 type,
                                            ViInt32 count, ViReal64 _VI_FAR xAxis[],
                                            ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_SelectLimitLine (ViSession instrumentHandle,
                                            ViInt32 window, ViInt32 limit,
                                            ViString name, ViBoolean limitEnabled);
ViStatus _VI_FUNC rsspecan_EnableLimitCheck (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32 limit,
                                             ViInt32 type, ViBoolean limitEnabled,
                                             ViBoolean checkEnabled);
ViStatus _VI_FUNC rsspecan_MoveLimitLine (ViSession instrumentHandle, ViInt32 limit,
                                          ViInt32 type, ViInt32 method,
                                          ViReal64 value);
ViStatus _VI_FUNC rsspecan_CopyLimitLine (ViSession instrumentHandle, ViInt32 limit,
                                          ViInt32 copyTo);
ViStatus _VI_FUNC rsspecan_DeleteLimitLine (ViSession instrumentHandle,
                                            ViInt32 limit);
ViStatus _VI_FUNC rsspecan_ClearLimitLineResults (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 limit);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLineState (ViSession instrumentHandle,
                                                      ViInt32 window, ViInt32 line,
                                                      ViInt32 type,
                                                      ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLinePosition (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 line, ViInt32 type,
                                                         ViReal64 position);
ViStatus _VI_FUNC rsspecan_GetActiveLimitLines (ViSession instrumentHandle,
                                                ViInt32 stringSize,
                                                ViChar _VI_FAR activeLimitLines[],
                                                ViInt32* returnedStringLength);
ViStatus _VI_FUNC rsspecan_GetLimitLineCatalog (ViSession instrumentHandle,
                                                ViInt32* numberOfLimitLines,
                                                ViInt32 bufferSize,
                                                ViChar _VI_FAR limitLinesList[]);
ViStatus _VI_FUNC rsspecan_DisplaySetActiveWindow (ViSession instrumentHandle,
                                                   ViInt32 window);
ViStatus _VI_FUNC rsspecan_DisplayGetActiveWindow (ViSession instrumentHandle,
                                                   ViInt32* window);
ViStatus _VI_FUNC rsspecan_GetSelectedSubwindow (ViSession instrumentHandle,
                                                 ViInt32* subwindow);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayFocusedArea (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 subwindow);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayControl (ViSession instrumentHandle,
                                                    ViBoolean frequency,
                                                    ViBoolean logo, ViBoolean time);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayComment (ViSession instrumentHandle,
                                                    ViInt32 window, ViBoolean state,
                                                    ViString comment);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayPowerSave (ViSession instrumentHandle,
                                                      ViBoolean state,
                                                      ViInt32 holdoff);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayFormat (ViSession instrumentHandle,
                                                   ViInt32 format);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayWindowSize (ViSession instrumentHandle,
                                                       ViInt32 windowSize);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayLogRange (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViReal64 range);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayAmplitudeGridMode
             (ViSession instrumentHandle, ViInt32 window, ViInt32 yAxisGridMode);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayReferenceValue
             (ViSession instrumentHandle, ViInt32 window, ViReal64 referenceValue);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayReferencePosition
             (ViSession instrumentHandle, ViInt32 window,
              ViReal64 referencePosition);
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySingleSweep (ViSession instrumentHandle,
                                                        ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayUpdate (ViSession instrumentHandle,
                                                   ViBoolean displayInRemote);
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySoftFrontpanel
             (ViSession instrumentHandle, ViBoolean softFrontpanel);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayAutoscaleOnce
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayYAxisScaling (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViReal64 scaling);
ViStatus _VI_FUNC rsspecan_ConfigureDisplaySettings (ViSession instrumentHandle,
                                                     ViBoolean statusBarState,
                                                     ViBoolean softkeyBarState,
                                                     ViBoolean toolBarState,
                                                     ViInt32 touchscreenState);
ViStatus _VI_FUNC rsspecan_GetDisplayThemeCatalog (ViSession instrumentHandle,
                                                   ViInt32* numberOfThemes,
                                                   ViInt32 bufferSize,
                                                   ViChar _VI_FAR themeList[]);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayThemeSelect (ViSession instrumentHandle,
                                                        ViString theme);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMarkerTable (ViSession instrumentHandle,
                                                        ViInt32 markerTable);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayZoom (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViBoolean zoomState, ViReal64 x1,
                                                 ViReal64 y1, ViReal64 x2,
                                                 ViReal64 y2);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMultipleZoom (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 zoomWindow,
                                                         ViBoolean enableZoom,
                                                         ViReal64 x1, ViReal64 y1,
                                                         ViReal64 x2, ViReal64 y2);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayMeasurementWindowState
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryActiveMeasurementWindow (ViSession instrumentHandle,
                                                         ViInt32* window);
ViStatus _VI_FUNC rsspecan_DisplayColorDefault (ViSession instrumentHandle,
                                                ViInt32 defaultSetting);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayColor (ViSession instrumentHandle,
                                                  ViInt32 colorMap, ViReal64 TINT,
                                                  ViReal64 SATURATION,
                                                  ViReal64 BRIGHTNESS);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayColorByPredefined
             (ViSession instrumentHandle, ViInt32 colorMap,
              ViInt32 predefinedColors);
ViStatus _VI_FUNC rsspecan_AddWindow (ViSession instrumentHandle,
                                      ViInt32 stringSize, ViString windowName,
                                      ViInt32 direction, ViInt32 windowType,
                                      ViChar _VI_FAR returnedWindowName[]);
ViStatus _VI_FUNC rsspecan_RemoveWindow (ViSession instrumentHandle,
                                         ViString windowName);
ViStatus _VI_FUNC rsspecan_ReplaceWindow (ViSession instrumentHandle,
                                          ViString windowName, ViInt32 windowType);
ViStatus _VI_FUNC rsspecan_QueryWindowsList (ViSession instrumentHandle,
                                             ViInt32 bufferSize,
                                             ViChar _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryWindowIndex (ViSession instrumentHandle,
                                             ViString windowName, ViInt32* index);
ViStatus _VI_FUNC rsspecan_QueryWindowType (ViSession instrumentHandle,
                                            ViInt32 index, ViInt32* windowType);
ViStatus _VI_FUNC rsspecan_QueryWindowName (ViSession instrumentHandle,
                                            ViInt32 stringSize, ViInt32 index,
                                            ViChar _VI_FAR windowName[]);
ViStatus _VI_FUNC rsspecan_ConfigureSplitterPosition (ViSession instrumentHandle,
                                                      ViString firstWindowName,
                                                      ViString secondWindowName,
                                                      ViInt32 position);
ViStatus _VI_FUNC rsspecan_SelectTransducerFactor (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViBoolean transducerState,
                                                   ViString transducerName);
ViStatus _VI_FUNC rsspecan_ConfigureTransducerFactor (ViSession instrumentHandle,
                                                      ViInt32 window, ViString name,
                                                      ViInt32 unit,
                                                      ViInt32 interpolation,
                                                      ViString comment,
                                                      ViInt32 noOfTestPoints,
                                                      ViReal64 _VI_FAR frequencyData[],
                                                      ViReal64 _VI_FAR levelData[]);
ViStatus _VI_FUNC rsspecan_ConfigureTransducerFactorRefLevAdj
             (ViSession instrumentHandle, ViInt32 window, ViBoolean refLevAdj);
ViStatus _VI_FUNC rsspecan_ConfigureDisplayTransducerState
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_GetTransducerFactorCatalog (ViSession instrumentHandle,
                                                       ViInt32* numberOfTransducerFactors,
                                                       ViInt32 bufferSize,
                                                       ViChar _VI_FAR transducerFactorsList[]);
ViStatus _VI_FUNC rsspecan_DeleteTransducerFactor (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViString transducerName);
ViStatus _VI_FUNC rsspecan_GetTransducerActive (ViSession instrumentHandle,
                                                ViChar _VI_FAR activeTransducer[]);
ViStatus _VI_FUNC rsspecan_SelectTransducerSet (ViSession instrumentHandle,
                                                ViString transducerSetName,
                                                ViBoolean transducerSetState);
ViStatus _VI_FUNC rsspecan_ConfigureTransducerSet (ViSession instrumentHandle,
                                                   ViString transducerSetName,
                                                   ViString comment, ViInt32 unit,
                                                   ViBoolean transducerBreak,
                                                   ViInt32 range,
                                                   ViReal64 startFrequency,
                                                   ViReal64 stopFrequency,
                                                   ViString factors);
ViStatus _VI_FUNC rsspecan_QueryTransducerSetCatalog (ViSession instrumentHandle,
                                                      ViInt32* numberOfTransducerSets,
                                                      ViInt32 bufferSize,
                                                      ViChar _VI_FAR transducerSetsList[]);
ViStatus _VI_FUNC rsspecan_DeleteTransducerSet (ViSession instrumentHandle,
                                                ViString transducerSetName);
ViStatus _VI_FUNC rsspecan_QueryActiveTransducerSet (ViSession instrumentHandle,
                                                     ViChar _VI_FAR activeTransducerSet[]);
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyDevice (ViSession instrumentHandle,
                                                    ViInt32 device,
                                                    ViInt32 destination,
                                                    ViInt32 pageOrientation);
ViStatus _VI_FUNC rsspecan_HardcopyAbort (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_HardcopyPrint (ViSession instrumentHandle,
                                          ViInt32 device, ViInt32 items);
ViStatus _VI_FUNC rsspecan_HardcopyPrintNext (ViSession instrumentHandle,
                                              ViInt32 device);
ViStatus _VI_FUNC rsspecan_HardcopySetFileName (ViSession instrumentHandle,
                                                ViString name);
ViStatus _VI_FUNC rsspecan_HardcopyGetPrinterList (ViSession instrumentHandle,
                                                   ViInt32 bufferSize,
                                                   ViChar _VI_FAR printerList[]);
ViStatus _VI_FUNC rsspecan_HardcopySetPrinter (ViSession instrumentHandle,
                                               ViInt32 device,
                                               ViString printerName);
ViStatus _VI_FUNC rsspecan_HardcopyComment (ViSession instrumentHandle,
                                            ViString comment);
ViStatus _VI_FUNC rsspecan_HardcopyTimeDate (ViSession instrumentHandle,
                                             ViInt32 device,
                                             ViBoolean timeAndDateStamp);
ViStatus _VI_FUNC rsspecan_HardcopyColor (ViSession instrumentHandle,
                                          ViBoolean color, ViInt32 defaultSetting);
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyColor (ViSession instrumentHandle,
                                                   ViInt32 colorMap, ViReal64 TINT,
                                                   ViReal64 SATURATION,
                                                   ViReal64 BRIGHTNESS);
ViStatus _VI_FUNC rsspecan_ConfigureHardcopyColorByPredefined
             (ViSession instrumentHandle, ViInt32 colorMap,
              ViInt32 predefinedColors);
ViStatus _VI_FUNC rsspecan_Calibration (ViSession instrumentHandle, ViBoolean sync,
                                        ViUInt32 timeoutms, ViInt32* result);
ViStatus _VI_FUNC rsspecan_CalibrationAbort (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_CalibrationResult (ViSession instrumentHandle,
                                              ViInt32 arraySize,
                                              ViChar _VI_FAR result[]);
ViStatus _VI_FUNC rsspecan_CalibrationState (ViSession instrumentHandle,
                                             ViBoolean state);
ViStatus _VI_FUNC rsspecan_YIGCorrection (ViSession instrumentHandle,
                                          ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_GetCalibrationProbeCatalog (ViSession instrumentHandle,
                                                       ViInt32* numberOfDataSets,
                                                       ViInt32 bufferSize,
                                                       ViChar _VI_FAR dataSetList[]);
ViStatus _VI_FUNC rsspecan_CalibrationProbeState (ViSession instrumentHandle,
                                                  ViString probeDataSetName,
                                                  ViBoolean state);
ViStatus _VI_FUNC rsspecan_CalibrationProbeSettings (ViSession instrumentHandle,
                                                     ViInt32 probeMode,
                                                     ViReal64 value);
ViStatus _VI_FUNC rsspecan_CalibrationProbeManualCompensation
             (ViSession instrumentHandle, ViBoolean manualProbeAdjustment);
ViStatus _VI_FUNC rsspecan_CalibrationProbeCompensation (ViSession instrumentHandle,
                                                         ViBoolean frequencyResponseComp,
                                                         ViBoolean gainErrorCompensation);
ViStatus _VI_FUNC rsspecan_StartProbeCalibration (ViSession instrumentHandle,
                                                  ViString probeDataSetName);
ViStatus _VI_FUNC rsspecan_GetProbeCalibrationStatus (ViSession instrumentHandle,
                                                      ViUInt32 timeout,
                                                      ViBoolean* probeCalibrationStatusI,
                                                      ViBoolean* probeCalibrationStatusQ);
ViStatus _VI_FUNC rsspecan_CalibrationProbeCheck (ViSession instrumentHandle,
                                                  ViString probeDataSetName,
                                                  ViInt32* probeCheck);
ViStatus _VI_FUNC rsspecan_CalibrationProbeResult (ViSession instrumentHandle,
                                                   ViString probeDataSetName,
                                                   ViInt32 arraySize,
                                                   ViChar _VI_FAR result[]);
ViStatus _VI_FUNC rsspecan_CalibrationProbeRename (ViSession instrumentHandle,
                                                   ViString probeDataSetSource,
                                                   ViString probeDataSetDestination);
ViStatus _VI_FUNC rsspecan_CalibrationProbeDelete (ViSession instrumentHandle,
                                                   ViString probeDataSetName);
ViStatus _VI_FUNC rsspecan_SelectProbeConnector (ViSession instrumentHandle,
                                                 ViInt32 probeConnector);
ViStatus _VI_FUNC rsspecan_ProbeActivation (ViSession instrumentHandle,
                                            ViBoolean state);
ViStatus _VI_FUNC rsspecan_ProbeMicroButtonAction (ViSession instrumentHandle,
                                                   ViInt32 microButtonAction);
ViStatus _VI_FUNC rsspecan_GetProbeInformation (ViSession instrumentHandle,
                                                ViInt32 bufferSize, ViInt32* state,
                                                ViChar _VI_FAR name[],
                                                ViChar _VI_FAR serialNumber[],
                                                ViChar _VI_FAR partNumber[]);
ViStatus _VI_FUNC rsspecan_GetProbeType (ViSession instrumentHandle,
                                         ViInt32 probeConnector, ViInt32 bufferSize,
                                         ViChar _VI_FAR probeType[]);
ViStatus _VI_FUNC rsspecan_ConfigureControlLinesState (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureOutputControlLines (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViString userPorts);
ViStatus _VI_FUNC rsspecan_GetControlLines (ViSession instrumentHandle,
                                            ViInt32 window, ViInt32 lines,
                                            ViChar _VI_FAR userPorts[]);
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjust (ViSession instrumentHandle,
                                                ViInt32 adjustSettings,
                                                ViInt32 measurementTimeMode,
                                                ViReal64 measurementTime);
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjustHystersis (ViSession instrumentHandle,
                                                         ViReal64 lowerHystersis,
                                                         ViReal64 upperHystersis);
ViStatus _VI_FUNC rsspecan_ConfigureAutoAdjustTriggerState
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_YAxisAutoScale (ViSession instrumentHandle,
                                           ViInt32 autoScaleScreen);
ViStatus _VI_FUNC rsspecan_SetAttributeViInt32 (ViSession instrumentHandle,
                                                ViString repeatedCapabilityName,
                                                ViUInt32 attributeID,
                                                ViInt32 attributeValue);
ViStatus _VI_FUNC rsspecan_SetAttributeViReal64 (ViSession instrumentHandle,
                                                 ViString repeatedCapabilityName,
                                                 ViUInt32 attributeID,
                                                 ViReal64 attributeValue);
ViStatus _VI_FUNC rsspecan_SetAttributeViString (ViSession instrumentHandle,
                                                 ViString repeatedCapabilityName,
                                                 ViUInt32 attributeID,
                                                 ViString attributeValue);
ViStatus _VI_FUNC rsspecan_SetAttributeViBoolean (ViSession instrumentHandle,
                                                  ViString repeatedCapabilityName,
                                                  ViUInt32 attributeID,
                                                  ViBoolean attributeValue);
ViStatus _VI_FUNC rsspecan_SetAttributeViSession (ViSession instrumentHandle,
                                                  ViString repeatedCapabilityName,
                                                  ViUInt32 attributeID,
                                                  ViSession attributeValue);
ViStatus _VI_FUNC rsspecan_GetAttributeViInt32 (ViSession instrumentHandle,
                                                ViString repeatedCapabilityName,
                                                ViUInt32 attributeID,
                                                ViInt32* attributeValue);
ViStatus _VI_FUNC rsspecan_GetAttributeViReal64 (ViSession instrumentHandle,
                                                 ViString repeatedCapabilityName,
                                                 ViUInt32 attributeID,
                                                 ViReal64* attributeValue);
ViStatus _VI_FUNC rsspecan_GetAttributeViString (ViSession instrumentHandle,
                                                 ViString repeatedCapabilityName,
                                                 ViUInt32 attributeID,
                                                 ViInt32 bufferSize,
                                                 ViChar _VI_FAR attributeValue[]);
ViStatus _VI_FUNC rsspecan_GetAttributeViBoolean (ViSession instrumentHandle,
                                                  ViString repeatedCapabilityName,
                                                  ViUInt32 attributeID,
                                                  ViBoolean* attributeValue);
ViStatus _VI_FUNC rsspecan_GetAttributeViSession (ViSession instrumentHandle,
                                                  ViString repeatedCapabilityName,
                                                  ViUInt32 attributeID,
                                                  ViSession* attributeValue);
ViStatus _VI_FUNC rsspecan_CheckAttributeViInt32 (ViSession instrumentHandle,
                                                  ViString repeatedCapabilityName,
                                                  ViUInt32 attributeID,
                                                  ViInt32 attributeValue);
ViStatus _VI_FUNC rsspecan_CheckAttributeViReal64 (ViSession instrumentHandle,
                                                   ViString repeatedCapabilityName,
                                                   ViUInt32 attributeID,
                                                   ViReal64 attributeValue);
ViStatus _VI_FUNC rsspecan_CheckAttributeViString (ViSession instrumentHandle,
                                                   ViString repeatedCapabilityName,
                                                   ViUInt32 attributeID,
                                                   ViString attributeValue);
ViStatus _VI_FUNC rsspecan_CheckAttributeViBoolean (ViSession instrumentHandle,
                                                    ViString repeatedCapabilityName,
                                                    ViUInt32 attributeID,
                                                    ViBoolean attributeValue);
ViStatus _VI_FUNC rsspecan_CheckAttributeViSession (ViSession instrumentHandle,
                                                    ViString repeatedCapabilityName,
                                                    ViUInt32 attributeID,
                                                    ViSession attributeValue);
ViStatus _VI_FUNC rsspecan_GetAttributeRepeatedCapabilityIds
             (ViSession instrumentHandle, ViUInt32 attributeID, ViInt32 bufferSize,
              ViChar _VI_FAR repeatedCapabilityId_s[]);
ViStatus _VI_FUNC rsspecan_GetAttributeRepeatedCapabilityIdNames
             (ViSession instrumentHandle, ViUInt32 attributeID,
              ViString repeatedCapabilityId, ViInt32 bufferSize,
              ViChar _VI_FAR repeatedCapabilityIdName_s[]);
ViStatus _VI_FUNC rsspecan_ReadYTrace (ViSession instrumentHandle, ViInt32 window,
                                       ViInt32 trace, ViUInt32 maximumTime_ms,
                                       ViInt32 arrayLength, ViInt32* actualPoints,
                                       ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_FetchYTrace (ViSession instrumentHandle, ViInt32 window,
                                        ViInt32 trace, ViInt32 arrayLength,
                                        ViInt32* actualPoints,
                                        ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_ReadYTracePrevious (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 trace,
                                               ViInt32 offsetSweepPoint,
                                               ViInt32 numberOfSweeps,
                                               ViInt32 arrayLength,
                                               ViInt32* actualPoints,
                                               ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_FetchXTrace (ViSession instrumentHandle, ViInt32 trace,
                                        ViInt32 arrayLength, ViInt32* actualPoints,
                                        ViReal64 _VI_FAR x[]);
ViStatus _VI_FUNC rsspecan_GetCurrentResults (ViSession instrumentHandle,
                                              ViInt32 window, ViReal64* xValue,
                                              ViReal64* yValue);
ViStatus _VI_FUNC rsspecan_GetCurrentYResult (ViSession instrumentHandle,
                                              ViInt32 window, ViReal64* level);
ViStatus _VI_FUNC rsspecan_ReadTraceIQData (ViSession instrumentHandle,
                                            ViInt32 window, ViUInt32 timeout,
                                            ViInt32 bufferSize, ViInt32* noOfPoints,
                                            ViReal64 _VI_FAR realParts_I[],
                                            ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_FetchTraceIQData (ViSession instrumentHandle,
                                             ViInt32 offsetSamples,
                                             ViInt32 noOfSamples,
                                             ViInt32 bufferSize,
                                             ViInt32* noOfPoints,
                                             ViReal64 _VI_FAR realParts_I[],
                                             ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_ReadMemoryIQLargeData (ViSession instrumentHandle,
                                                  ViInt32 window, ViUInt32 timeout,
                                                  ViInt32 format, ViInt32 dataOrder,
                                                  ViInt32* noOfPoints);
ViStatus _VI_FUNC rsspecan_ReadMemoryIQLargeDataBlock (ViSession instrumentHandle,
                                                       ViInt32 bufferSize,
                                                       ViChar _VI_FAR returnedData[],
                                                       ViInt32* returnedCounts);
ViStatus _VI_FUNC rsspecan_GetLimitCheckResult (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 limit,
                                                ViInt32* state);
ViStatus _VI_FUNC rsspecan_Initiate (ViSession instrumentHandle, ViInt32 window,
                                     ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_InitiateSpurious (ViSession instrumentHandle,
                                             ViInt32 window, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_InitiateSEMMeasurement (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_Abort (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Continue (ViSession instrumentHandle, ViInt32 window,
                                     ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_SendSoftwareTrigger (ViSession instrumentHandle,
                                                ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_InitiateSequencer (ViSession instrumentHandle,
                                              ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_AbortSequencer (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_RefreshSequencer (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureSequencerState (ViSession instrumentHandle,
                                                    ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureSequencerMode (ViSession instrumentHandle,
                                                   ViInt32 sequencerMode);
ViStatus _VI_FUNC rsspecan_GetSweepNumber (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32* numberOfSweeps);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerNoiseMeasurement
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryMarkerNoiseMeasurementResult
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerPhaseNoiseMeasurement
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_PhaseNoisePeakSearch (ViSession instrumentHandle,
                                                 ViInt32 window);
ViStatus _VI_FUNC rsspecan_PhaseNoisePeakSearchAuto (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryPhaseNoiseResult (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViReal64* phaseNoiseResult);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerFrequencyCounter
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViBoolean markerFrequencyCounter,
              ViReal64 frequencyCounterResolution);
ViStatus _VI_FUNC rsspecan_QueryMarkerFrequencyCounterResult
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViReal64* counterFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureNdBPoints (ViSession instrumentHandle,
                                               ViInt32 window, ViBoolean nDBState,
                                               ViReal64 nDBLevel);
ViStatus _VI_FUNC rsspecan_QueryNdBResult (ViSession instrumentHandle,
                                           ViInt32 window, ViReal64* nDBResult);
ViStatus _VI_FUNC rsspecan_QueryNdBFrequencies (ViSession instrumentHandle,
                                                ViInt32 window,
                                                ViReal64* nDBFrequencyLower,
                                                ViReal64* nDBFrequencyHigher);
ViStatus _VI_FUNC rsspecan_QueryNdBTimes (ViSession instrumentHandle,
                                          ViInt32 window, ViReal64* nDBTimeLower,
                                          ViReal64* nDBTimeHigher);
ViStatus _VI_FUNC rsspecan_QueryNdBQFactor (ViSession instrumentHandle,
                                            ViInt32 window, ViReal64* nDBQFactor);
ViStatus _VI_FUNC rsspecan_ConfigureListPowerAverageType (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 type);
ViStatus _VI_FUNC rsspecan_ConfigureListPowerSequence (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 noOfListItems,
                                                       ViReal64 _VI_FAR analyzerFrequency[],
                                                       ViReal64 _VI_FAR referenceLevel[],
                                                       ViReal64 _VI_FAR RFInputAttenuation[],
                                                       ViReal64 _VI_FAR RFInputElectronicAttn[],
                                                       ViInt32 _VI_FAR filterType[],
                                                       ViReal64 _VI_FAR resolutionBandwidth[],
                                                       ViReal64 _VI_FAR videoBandwidth[],
                                                       ViReal64 _VI_FAR measTime[],
                                                       ViReal64 _VI_FAR triggerLevel[]);
ViStatus _VI_FUNC rsspecan_ConfigureListPowerSet (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViBoolean peakMeas,
                                                  ViBoolean RMSMeas,
                                                  ViBoolean AVGMeas,
                                                  ViInt32 triggerMode,
                                                  ViInt32 triggerSlope,
                                                  ViReal64 triggerOffset,
                                                  ViReal64 gateLength);
ViStatus _VI_FUNC rsspecan_ListPowerMeasurementOff (ViSession instrumentHandle,
                                                    ViInt32 window);
ViStatus _VI_FUNC rsspecan_QueryListPowerSequence (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 noOfListItems,
                                                   ViReal64 _VI_FAR analyzerFrequency[],
                                                   ViReal64 _VI_FAR referenceLevel[],
                                                   ViReal64 _VI_FAR RFInputAttenuation[],
                                                   ViReal64 _VI_FAR RFInputElectronicAttn[],
                                                   ViInt32 _VI_FAR filterType[],
                                                   ViReal64 _VI_FAR resolutionBandwidth[],
                                                   ViReal64 _VI_FAR videoBandwidth[],
                                                   ViReal64 _VI_FAR measTime[],
                                                   ViReal64 _VI_FAR triggerLevel[],
                                                   ViReal64 _VI_FAR listPowerResults[]);
ViStatus _VI_FUNC rsspecan_QueryListPowerResult (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 noOfResults,
                                                 ViReal64 _VI_FAR listPowerResults[],
                                                 ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ConfigureTimeDomainPowerMeasurementState
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureTimeDomainPowerMeasurement
             (ViSession instrumentHandle, ViInt32 window, ViBoolean peak,
              ViBoolean RMS, ViBoolean mean, ViBoolean standardDeviation,
              ViBoolean average, ViBoolean maxHold, ViInt32 power);
ViStatus _VI_FUNC rsspecan_TimeDomainPowerMeasurementAllOff
             (ViSession instrumentHandle, ViInt32 window);
ViStatus _VI_FUNC rsspecan_TimeDomainPowerMeasurementSetReference
             (ViSession instrumentHandle, ViInt32 window);
ViStatus _VI_FUNC rsspecan_QueryTDomPowerResult (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 measurement,
                                                 ViInt32 resultType,
                                                 ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryTDomPowerPulseSequenceResult
             (ViSession instrumentHandle, ViInt32 window,
              ViReal64 timeOffsetOfFirstPulse, ViReal64 measurementTime,
              ViReal64 period, ViInt32 numberOfPulses, ViReal64 _VI_FAR result[]);
ViStatus _VI_FUNC rsspecan_SelectPowerMeasurement (ViSession instrumentHandle,
                                                   ViInt32 window, ViBoolean state,
                                                   ViInt32 powerMeasurement);
ViStatus _VI_FUNC rsspecan_ConfigurePowerMeasurement (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 noOfAdjChannels,
                                                      ViInt32 channelMode,
                                                      ViInt32 powerMode,
                                                      ViBoolean resultMode);
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelSpacing (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 channel,
                                                         ViInt32 channelNumber,
                                                         ViReal64 spacing);
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelBandwidth
             (ViSession instrumentHandle, ViInt32 window, ViInt32 channel,
              ViInt32 channelNumber, ViReal64 bandwidth);
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelName (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 channel,
                                                      ViInt32 channelNumber,
                                                      ViString name);
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerNoiseCorrection
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean noiseCorrectionState);
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerNoiseCorrectionAuto
             (ViSession instrumentHandle, ViInt32 window, ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerTrace (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 traceNumber);
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerStandard (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 ACPStandard);
ViStatus _VI_FUNC rsspecan_QueryChannelPowerStandardCatalog
             (ViSession instrumentHandle, ViInt32 bufferSize,
              ViChar _VI_FAR powerStandardCatalog[]);
ViStatus _VI_FUNC rsspecan_ConfigureChannelPowerUserStandard
             (ViSession instrumentHandle, ViInt32 window, ViInt32 operation,
              ViString userStandardName);
ViStatus _VI_FUNC rsspecan_PowerAdjustReferenceLevel (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_PowerPresetMeasurement (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 channelPowerType,
                                                   ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_QueryPowerResults (ViSession instrumentHandle,
                                              ViInt32 window,
                                              ViInt32 powerMeasurement,
                                              ViInt32 arraySize,
                                              ViReal64 _VI_FAR results[],
                                              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ConfigurePowerChannelWeightingFilters
             (ViSession instrumentHandle, ViInt32 filterType, ViBoolean filterState,
              ViReal64 filterValue);
ViStatus _VI_FUNC rsspecan_ConfigureACPFastACPState (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViBoolean fastACPState);
ViStatus _VI_FUNC rsspecan_ACPChannelPowerAutoAdjust (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_ACPChannelPowerAdjustRefLevelOffset
             (ViSession instrumentHandle, ViInt32 window,
              ViReal64 adjustRefLevelOffset);
ViStatus _VI_FUNC rsspecan_ACPAdjustReferenceLevelToChannelPower
             (ViSession instrumentHandle, ViInt32 window, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_ConfigureACPLimitState (ViSession instrumentHandle,
                                                   ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureACPAdjacentChannelLimit
             (ViSession instrumentHandle, ViInt32 window, ViBoolean relativeState,
              ViReal64 relativeValue, ViBoolean absoluteState,
              ViReal64 absoluteValue);
ViStatus _VI_FUNC rsspecan_ConfigureACPAlternateChannelLimit
             (ViSession instrumentHandle, ViInt32 window,
              ViInt32 alternateChannelNumber, ViBoolean relativeState,
              ViReal64 relativeValue, ViBoolean absoluteState,
              ViReal64 absoluteValue);
ViStatus _VI_FUNC rsspecan_QueryACPAdjacentChannelLimitCheckResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32* lowerResult,
              ViInt32* upperResult);
ViStatus _VI_FUNC rsspecan_QueryACPAlternateChannelLimitCheckResults
             (ViSession instrumentHandle, ViInt32 window,
              ViInt32 alternateChannelNumber, ViInt32* lowerResult,
              ViInt32* upperResult);
ViStatus _VI_FUNC rsspecan_ConfigureMulticarrierACPPower (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 noOfAdjChannels,
                                                          ViInt32 noOfTXChannels,
                                                          ViInt32 channelMode,
                                                          ViInt32 powerMode,
                                                          ViBoolean resultMode);
ViStatus _VI_FUNC rsspecan_ConfigureACPReferenceChannelAuto
             (ViSession instrumentHandle, ViInt32 window, ViInt32 reference);
ViStatus _VI_FUNC rsspecan_ConfigureACPReferenceChannelMan
             (ViSession instrumentHandle, ViInt32 window, ViInt32 channel);
ViStatus _VI_FUNC rsspecan_ConfigureMSRNumberOfSubblocks (ViSession instrumentHandle,
                                                          ViInt32 numberOfSubblocks);
ViStatus _VI_FUNC rsspecan_ConfigureMSRSubblockDefinition
             (ViSession instrumentHandle, ViInt32 subblock,
              ViReal64 centerFrequency, ViReal64 RFBandwidth,
              ViInt32 txChannelCount);
ViStatus _VI_FUNC rsspecan_ConfigureMSRTxChannelDefinition
             (ViSession instrumentHandle, ViInt32 subblock, ViInt32 txChannel,
              ViReal64 txCenterFrequency, ViInt32 technology, ViReal64 txBandwidth,
              ViBoolean weightingFilterState, ViReal64 rollOffFactor);
ViStatus _VI_FUNC rsspecan_QueryMSRTxName (ViSession instrumentHandle,
                                           ViInt32 subblock, ViInt32 txChannel,
                                           ViInt32 bufferSize,
                                           ViChar _VI_FAR name[]);
ViStatus _VI_FUNC rsspecan_ConfigureMSRGapChannel (ViSession instrumentHandle,
                                                   ViInt32 gap,
                                                   ViReal64 channelSpacing,
                                                   ViReal64 channelBandwidths,
                                                   ViBoolean weightingFilterState,
                                                   ViReal64 rollOffFactor);
ViStatus _VI_FUNC rsspecan_ConfigureMSRGapLimitChecking (ViSession instrumentHandle,
                                                         ViInt32 gap,
                                                         ViBoolean relativeState,
                                                         ViReal64 relativeValue,
                                                         ViBoolean absoluteState,
                                                         ViReal64 absoluteValue);
ViStatus _VI_FUNC rsspecan_ConfigureOccupiedBandwidth (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViReal64 powerBandwidth,
                                                       ViReal64 channelBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurement
             (ViSession instrumentHandle, ViInt32 measurement,
              ViInt32 numberOfSamples, ViReal64 resolutionBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurementXAxis
             (ViSession instrumentHandle, ViReal64 xAxisRefLevel,
              ViReal64 xAxisRange);
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeasurementYAxis
             (ViSession instrumentHandle, ViInt32 yUnit, ViReal64 yAxisMaxValue,
              ViReal64 yAxisMinValue);
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticSweep (ViSession instrumentHandle,
                                                          ViBoolean sweepModeContinuous);
ViStatus _VI_FUNC rsspecan_ConfigureSignalStatisticMeanPowerPosition
             (ViSession instrumentHandle, ViReal64 meanPowerPosition);
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementAdjustSettings
             (ViSession instrumentHandle, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementRefLevelAdjust
             (ViSession instrumentHandle, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_SignalStatisticMeasurementPresetScaling
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_QuerySignalStatisticCCDFLevel (ViSession instrumentHandle,
                                                          ViInt32 trace,
                                                          ViInt32 probability,
                                                          ViReal64* result);
ViStatus _VI_FUNC rsspecan_QuerySignalStatisticResult (ViSession instrumentHandle,
                                                       ViInt32 trace,
                                                       ViInt32 resultType,
                                                       ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureModDepthMeasurement (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryModulationDepth (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViReal64* modulationDepth);
ViStatus _VI_FUNC rsspecan_ModulationDepthSignalSearch (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 marker);
ViStatus _VI_FUNC rsspecan_ConfigureTOIMeasurement (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryTOI (ViSession instrumentHandle, ViInt32 window,
                                     ViReal64* TOIMeasurement);
ViStatus _VI_FUNC rsspecan_TOISignalSearch (ViSession instrumentHandle,
                                            ViInt32 window, ViInt32 marker);
ViStatus _VI_FUNC rsspecan_ConfigureTOIPositioning (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 interceptMarkersPositioning);
ViStatus _VI_FUNC rsspecan_ConfigureHDistMeasurement (ViSession instrumentHandle,
                                                      ViBoolean state,
                                                      ViInt32 noOfHarmonics,
                                                      ViBoolean harmonicRBWAuto);
ViStatus _VI_FUNC rsspecan_AdjustHDistSettings (ViSession instrumentHandle,
                                                ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_QueryHDist (ViSession instrumentHandle,
                                       ViReal64* harmonicDistortionInPercent,
                                       ViReal64* harmonicDistortionInDB);
ViStatus _VI_FUNC rsspecan_QueryHDistHarmonicsList (ViSession instrumentHandle,
                                                    ViReal64 _VI_FAR harmonicsList[]);
ViStatus _VI_FUNC rsspecan_QueryHDistBandwidthList (ViSession instrumentHandle,
                                                    ViInt32 arraySize,
                                                    ViReal64 _VI_FAR resultValues[],
                                                    ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_BurstPowerSequence (ViSession instrumentHandle,
                                               ViInt32 window,
                                               ViReal64 analyzerFrequencyHz,
                                               ViReal64 resolutionBandwidthHz,
                                               ViReal64 measTimes,
                                               ViInt32 triggerSource,
                                               ViReal64 triggerLevel,
                                               ViReal64 triggerOffsets,
                                               ViInt32 typeOfMeas,
                                               ViInt32 noOfMeasurements);
ViStatus _VI_FUNC rsspecan_BurstPowerFilterType (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 filterType);
ViStatus _VI_FUNC rsspecan_GetBurstPowerSequence (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViReal64 analyzerFrequencyHz,
                                                  ViReal64 resolutionBandwidthHz,
                                                  ViReal64 measTimes,
                                                  ViInt32 triggerSource,
                                                  ViReal64 triggerLevel,
                                                  ViReal64 triggerOffsets,
                                                  ViInt32 typeOfMeas,
                                                  ViInt32 noOfMeasurements,
                                                  ViUInt32 timeout,
                                                  ViReal64 _VI_FAR burstPowerResults[]);
ViStatus _VI_FUNC rsspecan_GetBurstPowerResults (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 noOfResults,
                                                 ViReal64 _VI_FAR burstPowerResults[]);
ViStatus _VI_FUNC rsspecan_GetBurstPwrResultMin (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViReal64* powerResultMin);
ViStatus _VI_FUNC rsspecan_ConfigureSEState (ViSession instrumentHandle,
                                             ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureSEPeakSearch (ViSession instrumentHandle,
                                                  ViInt32 window, ViReal64 margin,
                                                  ViInt32 peaksPerRange);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListStartStop (ViSession instrumentHandle,
                                                          ViInt32 range,
                                                          ViReal64 startFrequency,
                                                          ViReal64 stopFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListFilterType
             (ViSession instrumentHandle, ViInt32 range, ViInt32 filter);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListBandwidth (ViSession instrumentHandle,
                                                          ViInt32 range,
                                                          ViReal64 resolutionBandwidth,
                                                          ViReal64 videoBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepSweepListTime (ViSession instrumentHandle,
                                                          ViInt32 range,
                                                          ViBoolean sweepTimeAuto,
                                                          ViReal64 sweepTime);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListDetector (ViSession instrumentHandle,
                                                         ViInt32 range,
                                                         ViInt32 detector);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListReferenceLevel
             (ViSession instrumentHandle, ViInt32 range, ViReal64 refLevelValue);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListRFAttenuation
             (ViSession instrumentHandle, ViInt32 range, ViBoolean attenuatorAuto,
              ViReal64 manualAttenuatorValuedB);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListPreamplifier
             (ViSession instrumentHandle, ViInt32 range, ViBoolean preamplifier);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListPreamplifierLevel
             (ViSession instrumentHandle, ViInt32 range,
              ViInt32 preamplificationLevel);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListSweep (ViSession instrumentHandle,
                                                      ViInt32 range,
                                                      ViInt32 sweepPoints,
                                                      ViBoolean stopAfterSweep);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListTransducerFactor
             (ViSession instrumentHandle, ViInt32 range, ViString transducerFactor);
ViStatus _VI_FUNC rsspecan_ConfigureSESweepListLimits (ViSession instrumentHandle,
                                                       ViInt32 range,
                                                       ViBoolean limitChecking,
                                                       ViReal64 limit,
                                                       ViReal64 absoluteLimitStart,
                                                       ViReal64 absoluteLimitStop);
ViStatus _VI_FUNC rsspecan_SEStandardCatalog (ViSession instrumentHandle,
                                              ViInt32 arraySize,
                                              ViInt32* numberOfStandards,
                                              ViChar _VI_FAR catalog[]);
ViStatus _VI_FUNC rsspecan_SEStandardSave (ViSession instrumentHandle,
                                           ViString standardSave);
ViStatus _VI_FUNC rsspecan_SEStandardLoad (ViSession instrumentHandle,
                                           ViString standardLoad);
ViStatus _VI_FUNC rsspecan_SEStandardDelete (ViSession instrumentHandle,
                                             ViString standardDelete);
ViStatus _VI_FUNC rsspecan_SEDeleteRange (ViSession instrumentHandle,
                                          ViInt32 range);
ViStatus _VI_FUNC rsspecan_GetSENumberOfListRanges (ViSession instrumentHandle,
                                                    ViInt32* numberOfRanges);
ViStatus _VI_FUNC rsspecan_SESearchPeak (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_StoreSEToFile (ViSession instrumentHandle,
                                          ViString fileName);
ViStatus _VI_FUNC rsspecan_ReadSEPeakList (ViSession instrumentHandle,
                                           ViInt32 noOfValues,
                                           ViReal64 _VI_FAR frequency[],
                                           ViReal64 _VI_FAR level[],
                                           ViReal64 _VI_FAR deltaLimit[],
                                           ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_SEListEvaluation (ViSession instrumentHandle,
                                             ViBoolean state);
ViStatus _VI_FUNC rsspecan_ReadSEListEvaluation (ViSession instrumentHandle,
                                                 ViInt32 noOfValues,
                                                 ViReal64 _VI_FAR rangeNumber[],
                                                 ViReal64 _VI_FAR startFrequency[],
                                                 ViReal64 _VI_FAR stopFrequency[],
                                                 ViReal64 _VI_FAR resolutionBandwidth[],
                                                 ViReal64 _VI_FAR frequency[],
                                                 ViReal64 _VI_FAR levelAbs[],
                                                 ViReal64 _VI_FAR levelRel[],
                                                 ViReal64 _VI_FAR delta[],
                                                 ViReal64 _VI_FAR limitCheck[],
                                                 ViReal64 _VI_FAR reserved1[],
                                                 ViReal64 _VI_FAR reserved2[],
                                                 ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ConfigureSEMState (ViSession instrumentHandle,
                                              ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureSEM (ViSession instrumentHandle,
                                         ViInt32 limitLineType,
                                         ViReal64 limitLineValue,
                                         ViReal64 _30kHz1MHzTransition);
ViStatus _VI_FUNC rsspecan_ConfigureSEMPeakSearch (ViSession instrumentHandle,
                                                   ViReal64 margin,
                                                   ViInt32 peaksPerRange);
ViStatus _VI_FUNC rsspecan_RestoreSEMStandardLines (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SEMSearchPeak (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_StoreSEMToFile (ViSession instrumentHandle,
                                           ViString fileName);
ViStatus _VI_FUNC rsspecan_ReadSEMPeakList (ViSession instrumentHandle,
                                            ViInt32 window,
                                            ViInt32 finalMeasurement,
                                            ViInt32 noOfValues,
                                            ViReal64 _VI_FAR frequency[],
                                            ViReal64 _VI_FAR level[],
                                            ViReal64 _VI_FAR deltaLevel[],
                                            ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetSEMValue (ViSession instrumentHandle,
                                        ViInt32 selection, ViReal64* value);
ViStatus _VI_FUNC rsspecan_SEMListEvaluation (ViSession instrumentHandle,
                                              ViBoolean state);
ViStatus _VI_FUNC rsspecan_ReadSEMListEvaluationResults (ViSession instrumentHandle,
                                                         ViInt32 noOfValues,
                                                         ViReal64 _VI_FAR rangeNumber[],
                                                         ViReal64 _VI_FAR startFrequency[],
                                                         ViReal64 _VI_FAR stopFrequency[],
                                                         ViReal64 _VI_FAR resolutionBandwidth[],
                                                         ViReal64 _VI_FAR frequency[],
                                                         ViReal64 _VI_FAR powerAbs[],
                                                         ViReal64 _VI_FAR powerRel[],
                                                         ViReal64 _VI_FAR delta[],
                                                         ViReal64 _VI_FAR limitCheck[],
                                                         ViReal64 _VI_FAR reserved1[],
                                                         ViReal64 _VI_FAR reserved2[],
                                                         ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_SEMMarkAllPeaks (ViSession instrumentHandle,
                                            ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureSEMReferenceRange (ViSession instrumentHandle,
                                                       ViInt32 powerReferenceType,
                                                       ViReal64 txBandwidth,
                                                       ViBoolean RRCFilterState,
                                                       ViReal64 RRCFilterAlpha);
ViStatus _VI_FUNC rsspecan_QuerySEMReferenceRangePosition
             (ViSession instrumentHandle, ViInt32* referenceRangePosition);
ViStatus _VI_FUNC rsspecan_ConfigureSEMMSRSettings (ViSession instrumentHandle,
                                                    ViInt32 bandCategory,
                                                    ViReal64 baseStationRFBandwidth,
                                                    ViBoolean GSMEDGEPresent,
                                                    ViBoolean LTEFDD14MHz3MHzPresent);
ViStatus _VI_FUNC rsspecan_ApplySEMMSRSettings (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureFastSEM (ViSession instrumentHandle,
                                             ViBoolean fastSEM);
ViStatus _VI_FUNC rsspecan_ConfigureSEMPresetStandard (ViSession instrumentHandle,
                                                       ViString presetStandard);
ViStatus _VI_FUNC rsspecan_SEMStandardSave (ViSession instrumentHandle,
                                            ViString saveStandard);
ViStatus _VI_FUNC rsspecan_SEMStandardDelete (ViSession instrumentHandle,
                                              ViString deleteStandard);
ViStatus _VI_FUNC rsspecan_RestoreSEMStandardFiles (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_StoreSEMPresetSettings (ViSession instrumentHandle,
                                                   ViString settingsFileName);
ViStatus _VI_FUNC rsspecan_SEMSaveListToFile (ViSession instrumentHandle,
                                              ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeStartStop (ViSession instrumentHandle,
                                                       ViInt32 range,
                                                       ViReal64 rangeStart,
                                                       ViReal64 rangeStop);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeFilterType (ViSession instrumentHandle,
                                                        ViInt32 range,
                                                        ViInt32 filterType);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeBandwidth (ViSession instrumentHandle,
                                                       ViInt32 range,
                                                       ViReal64 resolutionBandwidth,
                                                       ViReal64 videoBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeSweepTime (ViSession instrumentHandle,
                                                       ViInt32 range,
                                                       ViBoolean sweepTimeMode,
                                                       ViReal64 sweepTime);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeReferenceLevel
             (ViSession instrumentHandle, ViInt32 range, ViReal64 referenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRFAttenuation
             (ViSession instrumentHandle, ViInt32 range,
              ViBoolean RFAttenuationMode, ViReal64 RFAttenuator,
              ViBoolean preamplifiier);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRFPreamplifierLevel
             (ViSession instrumentHandle, ViInt32 range,
              ViInt32 preamplificationLevel);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeTransducerFactor
             (ViSession instrumentHandle, ViInt32 range, ViString transducerFactor);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeLimits (ViSession instrumentHandle,
                                                    ViInt32 range,
                                                    ViInt32 limitCheckType,
                                                    ViReal64 absoluteLimitStart,
                                                    ViReal64 absoluteLimitStop,
                                                    ViReal64 relativeLimitStart,
                                                    ViReal64 relativeLimitStop);
ViStatus _VI_FUNC rsspecan_ConfigureSEMRangeRelativeLimitsFunctions
             (ViSession instrumentHandle, ViInt32 range, ViInt32 powerClass,
              ViInt32 startFunction, ViReal64 absoluteLimitStart,
              ViInt32 stopFunction, ViReal64 absoluteLimitStop);
ViStatus _VI_FUNC rsspecan_GetSEMRangeCount (ViSession instrumentHandle,
                                             ViInt32* rangeCount);
ViStatus _VI_FUNC rsspecan_SEMInsertNewRange (ViSession instrumentHandle,
                                              ViInt32 range, ViInt32 insertRange);
ViStatus _VI_FUNC rsspecan_SEMDeleteRange (ViSession instrumentHandle,
                                           ViInt32 range);
ViStatus _VI_FUNC rsspecan_ConfigureSEMUsedPowerClasses (ViSession instrumentHandle,
                                                         ViInt32 powerClassNumber,
                                                         ViInt32 usedPowerClass);
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassCount (ViSession instrumentHandle,
                                                        ViInt32 count);
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassLimits (ViSession instrumentHandle,
                                                         ViInt32 powerClassNumber,
                                                         ViInt32 limitState,
                                                         ViReal64 lowerLevelLimit,
                                                         ViReal64 upperLevelLimit);
ViStatus _VI_FUNC rsspecan_ConfigureSEMPowerClassAllLimits
             (ViSession instrumentHandle, ViInt32 numberOfLimits,
              ViReal64 _VI_FAR limits[]);
ViStatus _VI_FUNC rsspecan_QuerySEMPowerClassAllLimits (ViSession instrumentHandle,
                                                        ViReal64 _VI_FAR limits[]);
ViStatus _VI_FUNC rsspecan_MSRAMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_QueryMSRAAnalysisInterval (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViReal64* intervalStart,
                                                      ViReal64* intervalStop);
ViStatus _VI_FUNC rsspecan_QueryMSRAAllAnalysisInterval (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 arraySize,
                                                         ViReal64 _VI_FAR intervalStart[],
                                                         ViReal64 _VI_FAR intervalStop[]);
ViStatus _VI_FUNC rsspecan_MSRARefresh (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureMSRACaptureOffset (ViSession instrumentHandle,
                                                       ViInt32 offset);
ViStatus _VI_FUNC rsspecan_ConfigureMSRAAnalysisLine (ViSession instrumentHandle,
                                                      ViBoolean showLine,
                                                      ViReal64 position);
ViStatus _VI_FUNC rsspecan_ConfigureMSRADataEvaluation (ViSession instrumentHandle,
                                                        ViInt32 dataEvaluation);
ViStatus _VI_FUNC rsspecan_ConfigureTrackingGenerator (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViReal64 sourcePower,
                                                       ViReal64 powerOffset,
                                                       ViReal64 frequencyOffset);
ViStatus _VI_FUNC rsspecan_SetTrackingGeneratorState (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureTrackingGeneratorExternalModulation
             (ViSession instrumentHandle, ViInt32 window, ViInt32 modulation,
              ViBoolean modState, ViReal64 FMDeviation);
ViStatus _VI_FUNC rsspecan_ConfigureTrackingGeneratorPowerSweep
             (ViSession instrumentHandle, ViInt32 window, ViInt32 powerSweep,
              ViReal64 powerStart, ViReal64 powerStop);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorPower
             (ViSession instrumentHandle, ViInt32 window, ViInt32 generator,
              ViReal64 power);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorSource
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorFrequency
             (ViSession instrumentHandle, ViInt32 window, ViInt32 generator,
              ViReal64 frequency, ViBoolean coupling);
ViStatus _VI_FUNC rsspecan_SelectExternalGenerator (ViSession instrumentHandle,
                                                    ViInt32 generator,
                                                    ViInt32 generatorAddress,
                                                    ViInt32 generatorLink,
                                                    ViString generatorType);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorFrequencySweep
             (ViSession instrumentHandle, ViInt32 window, ViInt32 generator,
              ViBoolean frequencySweep, ViReal64 frequencyOffset,
              ViInt32 denominator, ViInt32 numerator);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGeneratorRefOscillator
             (ViSession instrumentHandle, ViInt32 window, ViInt32 reference);
ViStatus _VI_FUNC rsspecan_ConfigureExternalGain (ViSession instrumentHandle,
                                                  ViInt32 window, ViReal64 gain);
ViStatus _VI_FUNC rsspecan_SendCommandToExternalGenerator
             (ViSession instrumentHandle, ViInt32 deviceAddress,
              ViString commandString);
ViStatus _VI_FUNC rsspecan_ExternalGeneratorNormalization
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ExternalGeneratorCalibration (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 method,
                                                         ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_ExternalGeneratorRecall (ViSession instrumentHandle,
                                                    ViInt32 window);
ViStatus _VI_FUNC rsspecan_ExternalGeneratorSaveAsTrdFactor
             (ViSession instrumentHandle, ViInt32 window, ViString name);
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogState (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogLowpassFilter
             (ViSession instrumentHandle, ViInt32 window, ViBoolean AFLowpass,
              ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogHighpassFilter
             (ViSession instrumentHandle, ViInt32 window, ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ConfigureFMDemodAnalogRange (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean spanAutoManual,
                                                        ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureFFTDiagnostic (ViSession instrumentHandle,
                                                   ViInt32 signalPath,
                                                   ViInt32 signalSource,
                                                   ViReal64 voltage,
                                                   ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandInput (ViSession instrumentHandle,
                                                         ViInt32 basebandInputs);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalCalibration
             (ViSession instrumentHandle, ViInt32 signalType,
              ViBoolean signalOutput);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandInputMeasurementRange
             (ViSession instrumentHandle, ViReal64 measurementRange);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandTrigger
             (ViSession instrumentHandle, ViInt32 trigger);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandTriggerLevel
             (ViSession instrumentHandle, ViInt32 trigger, ViReal64 triggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandParameters
             (ViSession instrumentHandle, ViBoolean balanced, ViInt32 impedance);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalPath
             (ViSession instrumentHandle, ViInt32 path);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalSourceLowPass
             (ViSession instrumentHandle, ViBoolean lowPass);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandFullscaleLevel
             (ViSession instrumentHandle, ViBoolean mode, ViReal64 level);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSignalSourceDither
             (ViSession instrumentHandle, ViBoolean IQDither);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogBasebandSwapIQ (ViSession instrumentHandle,
                                                          ViBoolean swapIQ);
ViStatus _VI_FUNC rsspecan_FFTSetFFTMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_FFTPreset (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_FFTRecalc (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureFFTReferenceLevel (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 amplitudeUnits,
                                                       ViReal64 referenceLevel,
                                                       ViReal64 referenceLevelOffset);
ViStatus _VI_FUNC rsspecan_ConfigureFFTFrequencyDomain (ViSession instrumentHandle,
                                                        ViInt32 type,
                                                        ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureFFTTimeDomain (ViSession instrumentHandle,
                                                   ViInt32 type);
ViStatus _VI_FUNC rsspecan_ConfigureFFTPhaseDiagram (ViSession instrumentHandle,
                                                     ViInt32 phaseWrap,
                                                     ViReal64 phaseOffset,
                                                     ViInt32 phaseUnit);
ViStatus _VI_FUNC rsspecan_ConfigureFFTPhaseLine (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 line,
                                                  ViBoolean phaseLineState,
                                                  ViReal64 phaseLinePosition);
ViStatus _VI_FUNC rsspecan_ConfigureFFTSignalSource (ViSession instrumentHandle,
                                                     ViInt32 path, ViInt32 IQInput,
                                                     ViBoolean IQBalanced,
                                                     ViBoolean lowPass,
                                                     ViBoolean IQDither);
ViStatus _VI_FUNC rsspecan_ConfigureFFTSweep (ViSession instrumentHandle,
                                              ViBoolean sweepModeContinuous,
                                              ViInt32 sweepCount,
                                              ViInt32 sweepPoints,
                                              ViBoolean capureBothDomain,
                                              ViBoolean recalculateAuto);
ViStatus _VI_FUNC rsspecan_ConfigureFFTBandwidth (ViSession instrumentHandle,
                                                  ViBoolean resolutionBandwidthAuto,
                                                  ViReal64 resolutionBandwidth,
                                                  ViReal64 resolutionBandwidthRatio,
                                                  ViInt32 resolutionBW1235);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInput (ViSession instrumentHandle,
                                                          ViInt32 basebandInputs);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputCoupling
             (ViSession instrumentHandle, ViBoolean basebandInputCoupling);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputParametersAuto
             (ViSession instrumentHandle, ViBoolean autoRange,
              ViBoolean autoSampleRate);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputParameters
             (ViSession instrumentHandle, ViReal64 range, ViReal64 sampleRate);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandOutput
             (ViSession instrumentHandle, ViBoolean output);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandFullScaleLevel
             (ViSession instrumentHandle, ViReal64 fullScaleLevel);
ViStatus _VI_FUNC rsspecan_QueryDigitalBasebandInputOutputConfiguration
             (ViSession instrumentHandle, ViInt32 digitalBasebandSelect,
              ViInt32 arraySize, ViChar _VI_FAR configuration[]);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputTriggerSource
             (ViSession instrumentHandle, ViInt32 triggerSource);
ViStatus _VI_FUNC rsspecan_ConfigureDigitalBasebandInputTriggerParameters
             (ViSession instrumentHandle, ViReal64 triggerLevel, ViReal64 holdoff);
ViStatus _VI_FUNC rsspecan_GsmMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureGsmTrigger (ViSession instrumentHandle,
                                                ViInt32 triggerSource,
                                                ViReal64 triggerValue);
ViStatus _VI_FUNC rsspecan_ConfigureGsmDemodulation (ViSession instrumentHandle,
                                                     ViInt32 modulationType,
                                                     ViInt32 multiSlot,
                                                     ViInt32 samplesPerSymbols,
                                                     ViBoolean syncSearch,
                                                     ViBoolean burstSearch,
                                                     ViReal64 burstSearchThreshold,
                                                     ViInt32 midamble,
                                                     ViString userSequence);
ViStatus _VI_FUNC rsspecan_ConfigureGsmMultiCarrierMode (ViSession instrumentHandle,
                                                         ViBoolean multiCarrierMode);
ViStatus _VI_FUNC rsspecan_ConfigureGsmTransSpecLimitType
             (ViSession instrumentHandle, ViInt32 limitType);
ViStatus _VI_FUNC rsspecan_ConfigureGsmIFRFTriggerAsIQPower
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureGsmModulationSpectrumListAverage
             (ViSession instrumentHandle, ViInt32 listAverage);
ViStatus _VI_FUNC rsspecan_RestoreGsmLimitLines (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotState (ViSession instrumentHandle,
                                                          ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlot (ViSession instrumentHandle,
                                                     ViInt32 standard,
                                                     ViInt32 triggerReference,
                                                     ViInt32 referenceMidamble,
                                                     ViBoolean absoluteLevelState,
                                                     ViReal64 limitBase1,
                                                     ViReal64 limitBase2);
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotCommonSettings
             (ViSession instrumentHandle, ViBoolean longSlots, ViInt32 arraySize,
              ViInt32 _VI_FAR longSlotsValues[], ViBoolean onlyOneFrame);
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotParameters
             (ViSession instrumentHandle, ViInt32 slotNumber, ViInt32 modulation,
              ViInt32 referenceLevelMode, ViReal64 referenceLevelValue,
              ViInt32 limitLineCtrlLevel, ViString limitLineLower,
              ViString limitLineUpper);
ViStatus _VI_FUNC rsspecan_ConfigureGsmMeasurement (ViSession instrumentHandle,
                                                    ViInt32 measurementMode);
ViStatus _VI_FUNC rsspecan_ConfigureGsmPVTMeasurement (ViSession instrumentHandle,
                                                       ViInt32 burstSection,
                                                       ViBoolean highResolutionMode,
                                                       ViInt32 filterBandwidth,
                                                       ViInt32 transitionNo);
ViStatus _VI_FUNC rsspecan_ReadGsmLevelTime (ViSession instrumentHandle,
                                             ViUInt32 timeout, ViBoolean* status,
                                             ViReal64* signalPowerdBm,
                                             ViReal64* triggerTimes,
                                             ViReal64* triggerLeveldBmV,
                                             ViReal64* reserved);
ViStatus _VI_FUNC rsspecan_ReadGsmPhaseFreqErrorValues (ViSession instrumentHandle,
                                                        ViUInt32 timeout,
                                                        ViInt32 measurement,
                                                        ViInt32 modifier,
                                                        ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchGsmPhaseFreqErrorValues (ViSession instrumentHandle,
                                                         ViInt32 measurement,
                                                         ViInt32 modifier,
                                                         ViReal64* value);
ViStatus _VI_FUNC rsspecan_ReadGsmModulAccValues (ViSession instrumentHandle,
                                                  ViUInt32 timeout,
                                                  ViInt32 measurement,
                                                  ViInt32 modifier,
                                                  ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchGsmModulAccValues (ViSession instrumentHandle,
                                                   ViInt32 measurement,
                                                   ViInt32 modifier,
                                                   ViReal64* value);
ViStatus _VI_FUNC rsspecan_ReadGsmPtempRef (ViSession instrumentHandle,
                                            ViUInt32 timeout,
                                            ViReal64 _VI_FAR returnedValues[]);
ViStatus _VI_FUNC rsspecan_ReadGsmPowVsTimeValues (ViSession instrumentHandle,
                                                   ViUInt32 timeout,
                                                   ViInt32 measurement,
                                                   ViInt32 modifier,
                                                   ViReal64* value);
ViStatus _VI_FUNC rsspecan_ReadGsmExtendedSlotPtempRef (ViSession instrumentHandle,
                                                        ViUInt32 timeout,
                                                        ViInt32 arraySize,
                                                        ViInt32 _VI_FAR slotNumber[],
                                                        ViReal64 _VI_FAR measuredLevel[],
                                                        ViReal64 _VI_FAR correctedLevel[],
                                                        ViReal64 _VI_FAR resolutionBandwidth[]);
ViStatus _VI_FUNC rsspecan_FetchGsmPtempRef (ViSession instrumentHandle,
                                             ViReal64 _VI_FAR returnedValues[]);
ViStatus _VI_FUNC rsspecan_FetchGsmPowVsTimeValues (ViSession instrumentHandle,
                                                    ViInt32 measurement,
                                                    ViInt32 modifier,
                                                    ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchGsmExtendedSlotPtempRef (ViSession instrumentHandle,
                                                         ViInt32 arraySize,
                                                         ViInt32 _VI_FAR slotNumber[],
                                                         ViReal64 _VI_FAR measuredLevel[],
                                                         ViReal64 _VI_FAR correctedLevel[],
                                                         ViReal64 _VI_FAR resolutionBandwidth[]);
ViStatus _VI_FUNC rsspecan_ReadGsmSpecModRef (ViSession instrumentHandle,
                                              ViUInt32 timeout,
                                              ViReal64 _VI_FAR returnedValues[]);
ViStatus _VI_FUNC rsspecan_ReadGsmModSpectrum (ViSession instrumentHandle,
                                               ViInt32 arraySize, ViUInt32 timeout,
                                               ViInt32 _VI_FAR index[],
                                               ViReal64 _VI_FAR startFrequencies[],
                                               ViReal64 _VI_FAR stopFrequencies[],
                                               ViReal64 _VI_FAR levels[],
                                               ViReal64 _VI_FAR limits[],
                                               ViInt32 _VI_FAR absRel[],
                                               ViInt32 _VI_FAR status[],
                                               ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_FetchGsmSpecModRef (ViSession instrumentHandle,
                                               ViReal64 _VI_FAR returnedValues[]);
ViStatus _VI_FUNC rsspecan_FetchGsmModSpectrum (ViSession instrumentHandle,
                                                ViInt32 arraySize,
                                                ViInt32 _VI_FAR index[],
                                                ViReal64 _VI_FAR startFrequencies[],
                                                ViReal64 _VI_FAR stopFrequencies[],
                                                ViReal64 _VI_FAR levels[],
                                                ViReal64 _VI_FAR limits[],
                                                ViInt32 _VI_FAR absRel[],
                                                ViInt32 _VI_FAR status[],
                                                ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_ReadGsmSpecTransRef (ViSession instrumentHandle,
                                                ViUInt32 timeout,
                                                ViReal64 _VI_FAR returnedValues[]);
ViStatus _VI_FUNC rsspecan_ReadGsmTransSpectrum (ViSession instrumentHandle,
                                                 ViInt32 arraySize,
                                                 ViUInt32 timeout,
                                                 ViInt32 _VI_FAR index[],
                                                 ViReal64 _VI_FAR startFrequencies[],
                                                 ViReal64 _VI_FAR stopFrequencies[],
                                                 ViReal64 _VI_FAR levels[],
                                                 ViReal64 _VI_FAR limits[],
                                                 ViInt32 _VI_FAR absRel[],
                                                 ViInt32 _VI_FAR status[],
                                                 ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_FetchGsmTransSpectrum (ViSession instrumentHandle,
                                                  ViInt32 arraySize,
                                                  ViInt32 _VI_FAR index[],
                                                  ViReal64 _VI_FAR startFrequencies[],
                                                  ViReal64 _VI_FAR stopFrequencies[],
                                                  ViReal64 _VI_FAR levels[],
                                                  ViReal64 _VI_FAR limits[],
                                                  ViInt32 _VI_FAR absRel[],
                                                  ViInt32 _VI_FAR status[],
                                                  ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_PulseMeasurementMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalModel (ViSession instrumentHandle,
                                                      ViInt32 pulsePeriod,
                                                      ViBoolean pulseHasDroop,
                                                      ViInt32 pulseModulation);
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalTimingAuto
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalTimingManual
             (ViSession instrumentHandle, ViReal64 minPulseWidth,
              ViReal64 maxPulseWidth, ViReal64 minPulseOffTime);
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalFrequencyOffset
             (ViSession instrumentHandle, ViBoolean autoMode, ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigurePulseSignalChirpRate (ViSession instrumentHandle,
                                                          ViBoolean autoMode,
                                                          ViReal64 chirpRate);
ViStatus _VI_FUNC rsspecan_ConfigurePulseInputSignalSource
             (ViSession instrumentHandle, ViInt32 source);
ViStatus _VI_FUNC rsspecan_PulseSelectIQDataFile (ViSession instrumentHandle,
                                                  ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigurePulseRFInput (ViSession instrumentHandle,
                                                  ViInt32 inputCoupling,
                                                  ViReal64 impedance,
                                                  ViBoolean highpassFilter,
                                                  ViBoolean YIGPreselector);
ViStatus _VI_FUNC rsspecan_ConfigurePulseFrequencyStepSize
             (ViSession instrumentHandle, ViBoolean stepSizeAuto,
              ViReal64 stepSize);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMechanicalAttenuator
             (ViSession instrumentHandle, ViBoolean autoMode, ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigurePulseElectronicAttenuator
             (ViSession instrumentHandle, ViBoolean state,
              ViBoolean electronicAttenuatorAuto, ViInt32 manualValuedB);
ViStatus _VI_FUNC rsspecan_ConfigurePulsePreamplifier (ViSession instrumentHandle,
                                                       ViBoolean state,
                                                       ViInt32 preamplifierLevel);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementAcquisition
             (ViSession instrumentHandle, ViInt32 filterType,
              ViReal64 measurementBandwidth, ViReal64 measurementTime);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementDetection
             (ViSession instrumentHandle, ViInt32 referenceSource,
              ViReal64 threshold, ViReal64 hysteresis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementDetectionLimit
             (ViSession instrumentHandle, ViBoolean detectionLimit,
              ViInt32 maximumPulseCount);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementPoint
             (ViSession instrumentHandle, ViInt32 reference, ViReal64 offset);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementPointAveragingWindow
             (ViSession instrumentHandle, ViReal64 averagingWindow);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMeasurementRange
             (ViSession instrumentHandle, ViInt32 reference, ViReal64 length,
              ViReal64 riseOffset, ViReal64 fallOffset);
ViStatus _VI_FUNC rsspecan_ConfigurePulseTopLevel (ViSession instrumentHandle,
                                                   ViInt32 position,
                                                   ViInt32 measurementAlgorithm,
                                                   ViReal64 ripplePortion);
ViStatus _VI_FUNC rsspecan_ConfigurePulseReferenceLevel (ViSession instrumentHandle,
                                                         ViInt32 unit,
                                                         ViReal64 highThreshold,
                                                         ViReal64 middleThreshold,
                                                         ViReal64 lowerThreshold,
                                                         ViReal64 boundary);
ViStatus _VI_FUNC rsspecan_ConfigurePulseSelection (ViSession instrumentHandle,
                                                    ViInt32 pulseNumber);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultAutomaticRangeScaling
             (ViSession instrumentHandle, ViInt32 automaticRangeScaling);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultReferencePoint
             (ViSession instrumentHandle, ViInt32 reference, ViReal64 offset);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultRange (ViSession instrumentHandle,
                                                      ViInt32 alignment,
                                                      ViReal64 length);
ViStatus _VI_FUNC rsspecan_ConfigurePulsePhaseUnit (ViSession instrumentHandle,
                                                    ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendPower
             (ViSession instrumentHandle, ViInt32 window, ViInt32 group,
              ViInt32 axis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendTiming
             (ViSession instrumentHandle, ViInt32 window, ViInt32 group,
              ViInt32 axis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendFrequency
             (ViSession instrumentHandle, ViInt32 window, ViInt32 yAxis,
              ViInt32 xAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterTrendPhase
             (ViSession instrumentHandle, ViInt32 window, ViInt32 yAxis,
              ViInt32 xAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionBins
             (ViSession instrumentHandle, ViInt32 window, ViInt32 numberOfBins);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionPower
             (ViSession instrumentHandle, ViInt32 window, ViInt32 xAxis,
              ViInt32 yAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionTiming
             (ViSession instrumentHandle, ViInt32 window, ViInt32 xAxis,
              ViInt32 yAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionFrequency
             (ViSession instrumentHandle, ViInt32 window, ViInt32 xAxis,
              ViInt32 yAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterDistributionPhase
             (ViSession instrumentHandle, ViInt32 window, ViInt32 xAxis,
              ViInt32 yAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumFrequency
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumPhase
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumPower
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultParameterSpectrumTiming
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTablePower
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter,
              ViBoolean visibility);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTableTiming
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter,
              ViBoolean visibility, ViInt32 scaling);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTableFrequency
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter,
              ViBoolean visibility, ViInt32 scaling);
ViStatus _VI_FUNC rsspecan_ConfigurePulseResultTablePhase
             (ViSession instrumentHandle, ViInt32 window, ViInt32 parameter,
              ViBoolean visibility);
ViStatus _VI_FUNC rsspecan_ExportPulseResultTable (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 exportCollumns,
                                                   ViString filename);
ViStatus _VI_FUNC rsspecan_ConfigurePulseAutomaticGridScaling
             (ViSession instrumentHandle, ViInt32 window, ViInt32 trace,
              ViBoolean autoScale);
ViStatus _VI_FUNC rsspecan_ConfigurePulseAbsoluteScaling (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViReal64 minimum,
                                                          ViReal64 maximum);
ViStatus _VI_FUNC rsspecan_ConfigurePulseRelativeScaling (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 trace,
                                                          ViReal64 perDivison,
                                                          ViReal64 referencePosition,
                                                          ViReal64 referenceValue);
ViStatus _VI_FUNC rsspecan_ConfigurePulseReferenceValues (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 trace,
                                                          ViReal64 minimum,
                                                          ViReal64 maximum);
ViStatus _VI_FUNC rsspecan_ConfigurePulseMarker (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViInt32 markerNumber,
                                                 ViBoolean markerState,
                                                 ViReal64 stimulus);
ViStatus _VI_FUNC rsspecan_ConfigurePulseDeltaMarker (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 deltaMarker,
                                                      ViBoolean deltaMarkerState,
                                                      ViReal64 stimulus);
ViStatus _VI_FUNC rsspecan_QueryPulseIDs (ViSession instrumentHandle,
                                          ViInt32 queryRange, ViInt32 arraySize,
                                          ViInt32 _VI_FAR pulseIDs[],
                                          ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_QueryPulseNumbers (ViSession instrumentHandle,
                                              ViInt32 queryRange, ViInt32 arraySize,
                                              ViInt32 _VI_FAR pulseNumbers[],
                                              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_QueryPulseResultPower (ViSession instrumentHandle,
                                                  ViInt32 parameter,
                                                  ViInt32 resultType,
                                                  ViInt32 queryRange,
                                                  ViInt32 arraySize,
                                                  ViReal64 _VI_FAR results[],
                                                  ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_QueryPulseResultTiming (ViSession instrumentHandle,
                                                   ViInt32 parameter,
                                                   ViInt32 resultType,
                                                   ViInt32 queryRange,
                                                   ViInt32 arraySize,
                                                   ViReal64 _VI_FAR results[],
                                                   ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_QueryPulseResultFrequency (ViSession instrumentHandle,
                                                      ViInt32 parameter,
                                                      ViInt32 resultType,
                                                      ViInt32 queryRange,
                                                      ViInt32 arraySize,
                                                      ViReal64 _VI_FAR results[],
                                                      ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_QueryPulseResultPhase (ViSession instrumentHandle,
                                                  ViInt32 parameter,
                                                  ViInt32 resultType,
                                                  ViInt32 queryRange,
                                                  ViInt32 arraySize,
                                                  ViReal64 _VI_FAR results[],
                                                  ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ADemodMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SetADemodState (ViSession instrumentHandle,
                                           ViBoolean state);
ViStatus _VI_FUNC rsspecan_SelectADemodResult (ViSession instrumentHandle,
                                               ViInt32 operatingMode,
                                               ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureADemodTrigger (ViSession instrumentHandle,
                                                   ViInt32 triggerSource,
                                                   ViReal64 triggerOffset,
                                                   ViInt32 triggerPolarity,
                                                   ViReal64 triggerValue);
ViStatus _VI_FUNC rsspecan_ConfigureADemodSet (ViSession instrumentHandle,
                                               ViReal64 sampleRate,
                                               ViInt32 recordLength,
                                               ViInt32 triggerSource,
                                               ViInt32 triggerSlope,
                                               ViInt32 pretriggerSamples,
                                               ViInt32 numberOfMeas);
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilter (ViSession instrumentHandle,
                                                  ViInt32 filterType,
                                                  ViBoolean filterState,
                                                  ViReal64 filterFrequency,
                                                  ViReal64 AMFMDeemphasisValue);
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilterManual (ViSession instrumentHandle,
                                                        ViInt32 filterType,
                                                        ViReal64 filterFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilterWeighting
             (ViSession instrumentHandle, ViBoolean CCITT, ViBoolean CCIR);
ViStatus _VI_FUNC rsspecan_ConfigureADemodFilterAWeighting
             (ViSession instrumentHandle, ViBoolean aWeightingFilter);
ViStatus _VI_FUNC rsspecan_ConfigureADemodZoom (ViSession instrumentHandle,
                                                ViBoolean zoomState,
                                                ViReal64 startTime);
ViStatus _VI_FUNC rsspecan_ConfigureADemodZoomLength (ViSession instrumentHandle,
                                                      ViInt32 zoomLengthMode,
                                                      ViReal64 zoomLength);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodSquelch (ViSession instrumentHandle,
                                                        ViBoolean squelchState,
                                                        ViReal64 squelchLevel);
ViStatus _VI_FUNC rsspecan_ConfigureADemodTHDUnit (ViSession instrumentHandle,
                                                   ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ADemAutoTune (ViSession instrumentHandle,
                                         ViUInt32 timeoutms);
ViStatus _VI_FUNC rsspecan_SelectAnalogDemodTrace (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPredefinedStandard
             (ViSession instrumentHandle, ViInt32 operation, ViString standard);
ViStatus _VI_FUNC rsspecan_ConfigureADemodOnlineOutput (ViSession instrumentHandle,
                                                        ViBoolean outputState,
                                                        ViString outputSelection,
                                                        ViReal64 ACCutoffFrequency,
                                                        ViBoolean phonesOutput);
ViStatus _VI_FUNC rsspecan_ConfigureADemodBandwidth (ViSession instrumentHandle,
                                                     ViReal64 demodulationBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFCoupling (ViSession instrumentHandle,
                                                      ViInt32 AFCoupling);
ViStatus _VI_FUNC rsspecan_ConfigureADemodMeasuringTime (ViSession instrumentHandle,
                                                         ViReal64 measuringTime);
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFFrequencyStartStop
             (ViSession instrumentHandle, ViReal64 startFrequency,
              ViReal64 stopFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFFrequencyCenterSpan
             (ViSession instrumentHandle, ViReal64 centerFrequency, ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFFullSpan (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureADemodAFParam (ViSession instrumentHandle,
                                                   ViReal64 resolutionBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureADemodRFParam (ViSession instrumentHandle,
                                                   ViReal64 resolutionBandwidth,
                                                   ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureADemodRFZoom (ViSession instrumentHandle,
                                                  ViReal64 zoom);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPMPhaseWrap
             (ViSession instrumentHandle, ViInt32 phaseWrap);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPMPhaseUnits
             (ViSession instrumentHandle, ViInt32 PMUnits);
ViStatus _VI_FUNC rsspecan_ConfigureAnalogDemodPMZeroPhaseReferencePoint
             (ViSession instrumentHandle, ViReal64 referencePoint);
ViStatus _VI_FUNC rsspecan_ConfigureADemodResultType (ViSession instrumentHandle,
                                                      ViInt32 demodulationType,
                                                      ViInt32 _VI_FAR resultTypes[]);
ViStatus _VI_FUNC rsspecan_GetADemodCurrentlySetValue (ViSession instrumentHandle,
                                                       ViInt32 value,
                                                       ViReal64* result);
ViStatus _VI_FUNC rsspecan_GetADemodResultValues (ViSession instrumentHandle,
                                                  ViInt32 demodulationType,
                                                  ViInt32 resultType,
                                                  ViUInt32 timeout,
                                                  ViInt32 arraySize,
                                                  ViReal64 _VI_FAR resultValues[],
                                                  ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetADemodOffset (ViSession instrumentHandle,
                                            ViInt32 resultType, ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetADemodMarkerModulationValue
             (ViSession instrumentHandle, ViInt32 markerNumber,
              ViInt32 demodulationType, ViInt32 detectorType, ViInt32 trace,
              ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetADemodMarkerMeasValue (ViSession instrumentHandle,
                                                     ViInt32 markerNumber,
                                                     ViInt32 measurement,
                                                     ViInt32 trace,
                                                     ViReal64* value);
ViStatus _VI_FUNC rsspecan_FMStereoMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SetFMStereoState (ViSession instrumentHandle,
                                             ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTrigger (ViSession instrumentHandle,
                                                     ViInt32 triggerSource,
                                                     ViReal64 triggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoPhaseNoiseMarker
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoDifferenceFrequencyDistortion
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state,
              ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoIntermodulationDistortion
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state,
              ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFAutoScale (ViSession instrumentHandle,
                                                         ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoDisplayConfig
             (ViSession instrumentHandle, ViInt32 window,
              ViInt32 displayConfiguration);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoReferenceDeviation
             (ViSession instrumentHandle, ViReal64 referenceDeviation);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoResultSummarySetup
             (ViSession instrumentHandle, ViInt32 channelType, ViInt32 detector,
              ViInt32 mode, ViBoolean coupled);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoMeasToRef (ViSession instrumentHandle,
                                                       ViInt32 channelType);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoMeasurementTime
             (ViSession instrumentHandle, ViReal64 measuringTime);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFFilter (ViSession instrumentHandle,
                                                      ViInt32 channelType,
                                                      ViInt32 highPass,
                                                      ViInt32 lowPass,
                                                      ViInt32 weighting,
                                                      ViInt32 deemphasis,
                                                      ViBoolean coupled);
ViStatus _VI_FUNC rsspecan_AllFMStereoAFFiltersOff (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFRange (ViSession instrumentHandle,
                                                     ViInt32 deviation,
                                                     ViReal64 devPerDivision);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFRangeTHDUnit
             (ViSession instrumentHandle, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoAFRangeDevUnit
             (ViSession instrumentHandle, ViInt32 absoluteDevUnit,
              ViInt32 relativeDevUnit);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTimeDomainZoom
             (ViSession instrumentHandle, ViBoolean zoomState, ViReal64 startTime);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTimeDomainZoomLength
             (ViSession instrumentHandle, ViInt32 zoomLengthMode,
              ViReal64 zoomLength);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoSquelch (ViSession instrumentHandle,
                                                     ViBoolean squelchState,
                                                     ViReal64 squelchLevel);
ViStatus _VI_FUNC rsspecan_QueryFMStereoCarrierFrequency (ViSession instrumentHandle,
                                                          ViReal64* carrierFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureFMStereoTraceModes (ViSession instrumentHandle,
                                                        ViInt32 channelType,
                                                        ViInt32 measurementType,
                                                        ViInt32 _VI_FAR traceModes[]);
ViStatus _VI_FUNC rsspecan_FMStereoDifferenceFrequencyDistortionSearchSignal
             (ViSession instrumentHandle, ViInt32 window, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_FMStereoIntermodulationDistortionSearchSignal
             (ViSession instrumentHandle, ViInt32 window, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_QueryFMStereoPhaseNoiseResult (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 marker,
                                                          ViReal64* phaseNoiseResult);
ViStatus _VI_FUNC rsspecan_QueryFMStereoDifferenceFrequencyDistortionResult
             (ViSession instrumentHandle, ViInt32 window,
              ViReal64 _VI_FAR differenceFrequencyDistortion[]);
ViStatus _VI_FUNC rsspecan_QueryFMStereoIntermodulationDistortionResult
             (ViSession instrumentHandle, ViInt32 window,
              ViReal64 _VI_FAR intermodulationDistortion[]);
ViStatus _VI_FUNC rsspecan_QueryFMStereoMeasurementResults
             (ViSession instrumentHandle, ViReal64 _VI_FAR absoluteDeviation[],
              ViReal64 _VI_FAR relativeDeviation[], ViReal64 _VI_FAR SINAD[],
              ViReal64 _VI_FAR THD[], ViReal64 _VI_FAR modulationFrequency[]);
ViStatus _VI_FUNC rsspecan_QueryFMStereoChannelTypeResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 channelType,
              ViInt32 measurementType, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryFMStereoChannelTypeAllResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 channelType,
              ViReal64 _VI_FAR result[]);
ViStatus _VI_FUNC rsspecan_QueryFMStereoResultValues (ViSession instrumentHandle,
                                                      ViInt32 channelType,
                                                      ViInt32 measurementType,
                                                      ViInt32 traceMode,
                                                      ViUInt32 timeout,
                                                      ViInt32 arraySize,
                                                      ViReal64 _VI_FAR resultValues[],
                                                      ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_BTOMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureBTOMeasurement (ViSession instrumentHandle,
                                                    ViInt32 geographicalRegion,
                                                    ViInt32 channel,
                                                    ViInt32 outputPowerClass,
                                                    ViInt32 samplesPerSymbols,
                                                    ViInt32 packetType,
                                                    ViReal64 antennaGain);
ViStatus _VI_FUNC rsspecan_ConfigureBTOAveragingMethod (ViSession instrumentHandle,
                                                        ViReal64 startPosition,
                                                        ViReal64 stopPosition);
ViStatus _VI_FUNC rsspecan_ConfigureBTOSynchronization (ViSession instrumentHandle,
                                                        ViBoolean findSync,
                                                        ViInt32 syncOffset,
                                                        ViBoolean findBurst,
                                                        ViReal64 burstOffset,
                                                        ViBoolean searchLengthAuto,
                                                        ViReal64 searchLength);
ViStatus _VI_FUNC rsspecan_ConfigureBTOLAP (ViSession instrumentHandle,
                                            ViInt32 lowest24BitsOfDevAddress);
ViStatus _VI_FUNC rsspecan_ConfigureBTOMeasurementMode (ViSession instrumentHandle,
                                                        ViInt32 measurementMode);
ViStatus _VI_FUNC rsspecan_ConfigureBTONumberOfAdjacentChannels
             (ViSession instrumentHandle, ViInt32 numberOfAdjacentChannels);
ViStatus _VI_FUNC rsspecan_ConfigureBTOReceiveFilter (ViSession instrumentHandle,
                                                      ViInt32 filter);
ViStatus _VI_FUNC rsspecan_ConfigureBTOSweepCouplingAuto (ViSession instrumentHandle,
                                                          ViInt32 sweepCoupling);
ViStatus _VI_FUNC rsspecan_ConfigureBTOSweepCoupling (ViSession instrumentHandle,
                                                      ViInt32 sweepCoupling,
                                                      ViReal64 couplingValue);
ViStatus _VI_FUNC rsspecan_ConfigureBTOAcquisition (ViSession instrumentHandle,
                                                    ViBoolean sweepModeContinuous,
                                                    ViInt32 numberOfSweeps);
ViStatus _VI_FUNC rsspecan_SelectBTOTrace (ViSession instrumentHandle,
                                           ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureBTOTrace (ViSession instrumentHandle,
                                              ViInt32 trace, ViInt32 traceMode,
                                              ViInt32 traceDetector);
ViStatus _VI_FUNC rsspecan_ConfigureBTOPacketBytesSCO (ViSession instrumentHandle,
                                                       ViInt32 packetBytesSCO);
ViStatus _VI_FUNC rsspecan_ConfigureBTODisplayYAxisScaling
             (ViSession instrumentHandle, ViReal64 scaling);
ViStatus _VI_FUNC rsspecan_ConfigureBTOAnalogDemodulationZoom
             (ViSession instrumentHandle, ViBoolean zoomState, ViReal64 startTime);
ViStatus _VI_FUNC rsspecan_GetBTOMesurementStatus (ViSession instrumentHandle,
                                                   ViInt32* status);
ViStatus _VI_FUNC rsspecan_GetBTOPacketInfo (ViSession instrumentHandle,
                                             ViInt32* type, ViInt32* length);
ViStatus _VI_FUNC rsspecan_GetBTOOutputPower (ViSession instrumentHandle,
                                              ViInt32 type, ViReal64* power);
ViStatus _VI_FUNC rsspecan_GetBTONumberOfExceptions (ViSession instrumentHandle,
                                                     ViInt32* numberOfExceptions);
ViStatus _VI_FUNC rsspecan_GetBTOPowerOfChannels (ViSession instrumentHandle,
                                                  ViInt32 arraySize,
                                                  ViReal64 _VI_FAR power[],
                                                  ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetBTOFrequencyDeviation (ViSession instrumentHandle,
                                                     ViInt32 frequencyDeviation,
                                                     ViInt32 type, ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTORatioAverageFrequencyDeviation
             (ViSession instrumentHandle, ViInt32 type, ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTOPercentageDeviation (ViSession instrumentHandle,
                                                      ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTOPacketDataBits (ViSession instrumentHandle,
                                                 ViUInt32 maximumTime_ms,
                                                 ViInt32 arrayLength,
                                                 ViInt32* actualPoints,
                                                 ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_ReadBTOFMTrace (ViSession instrumentHandle,
                                           ViUInt32 maximumTime_ms,
                                           ViInt32 arrayLength,
                                           ViInt32* actualPoints,
                                           ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_GetBTOInitCarrFreqTol (ViSession instrumentHandle,
                                                  ViInt32 type, ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTOCarrFreqDrift (ViSession instrumentHandle,
                                                ViInt32 type, ViReal64* value);
ViStatus _VI_FUNC rsspecan_ConfigureBTOSENumAdjacentChannels
             (ViSession instrumentHandle, ViInt32 number);
ViStatus _VI_FUNC rsspecan_AdjustBTOSEGateSetting (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_GetBTOChannelPowerOfSEMeasurement
             (ViSession instrumentHandle, ViInt32 parameter, ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTOSEPowerOfAdjacentChannels
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR power[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetBTOSENumberOfExceptions (ViSession instrumentHandle,
                                                       ViInt32* exceptions);
ViStatus _VI_FUNC rsspecan_ConfigureBTOCFSBlockCount (ViSession instrumentHandle,
                                                      ViInt32 blockCount);
ViStatus _VI_FUNC rsspecan_GetBTOCFSDeviation (ViSession instrumentHandle,
                                               ViInt32 parameter, ViInt32 type,
                                               ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTOCFSModulAccuracy (ViSession instrumentHandle,
                                                   ViInt32 parameter, ViInt32 type,
                                                   ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetBTODifferentialPhaseEncodingValues
             (ViSession instrumentHandle, ViInt32 parameter, ViInt32* value);
ViStatus _VI_FUNC rsspecan_ConfigureBTORTPMeasurement (ViSession instrumentHandle,
                                                       ViInt32 packetSection,
                                                       ViReal64 start,
                                                       ViReal64 stop);
ViStatus _VI_FUNC rsspecan_GetBTORelativeTransmissionPower
             (ViSession instrumentHandle, ViInt32 parameter, ViInt32 modificator,
              ViReal64* value);
ViStatus _VI_FUNC rsspecan_PWMMode (ViSession instrumentHandle, ViInt32 window,
                                    ViBoolean state);
ViStatus _VI_FUNC rsspecan_PWMSelect (ViSession instrumentHandle, ViInt32 sensor);
ViStatus _VI_FUNC rsspecan_ConfigurePWMMeasurement (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 measurementTime,
                                                    ViInt32 coupling,
                                                    ViReal64 frequency,
                                                    ViInt32 averageCount);
ViStatus _VI_FUNC rsspecan_ConfigurePWMRelative (ViSession instrumentHandle,
                                                 ViInt32 window,
                                                 ViReal64 referenceValue,
                                                 ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigurePWMAbsolute (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigurePWMReferenceLevelOffsetState
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigurePWMExternalPowerTrigger
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state,
              ViReal64 level);
ViStatus _VI_FUNC rsspecan_ConfigurePWMExternalPowerTriggerAdvanced
             (ViSession instrumentHandle, ViInt32 window, ViInt32 hysteresis,
              ViReal64 dropoutTime, ViReal64 holdoffTime, ViInt32 slope);
ViStatus _VI_FUNC rsspecan_ConfigurePWMBarGraphView (ViSession instrumentHandle,
                                                     ViBoolean barGraphView);
ViStatus _VI_FUNC rsspecan_ConfigurePWMDutyCycle (ViSession instrumentHandle,
                                                  ViBoolean dutyCycle,
                                                  ViReal64 dutyCycleValue);
ViStatus _VI_FUNC rsspecan_ConfigurePowerSensorAssignment
             (ViSession instrumentHandle, ViBoolean autoAssignment,
              ViString placeholder, ViString type, ViString intf,
              ViString serialNumber);
ViStatus _VI_FUNC rsspecan_QueryPowerSensorCount (ViSession instrumentHandle,
                                                  ViInt32* powerSensorCount);
ViStatus _VI_FUNC rsspecan_ConfigurePWMMeasToRef (ViSession instrumentHandle,
                                                  ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigurePWMZero (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePowerSensorReferenceLevelOffset
             (ViSession instrumentHandle, ViReal64 offset);
ViStatus _VI_FUNC rsspecan_ReadPWMResult (ViSession instrumentHandle,
                                          ViInt32 window, ViUInt32 timeout,
                                          ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchPWMResult (ViSession instrumentHandle,
                                           ViInt32 window, ViReal64* result);
ViStatus _VI_FUNC rsspecan_GSMK10Mode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Trace (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 trace,
                                                 ViInt32 traceType);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DUT (ViSession instrumentHandle,
                                               ViInt32 deviceUnderTest);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10SignalCharacteristics
             (ViSession instrumentHandle, ViInt32 bandClass, ViInt32 ARFCN);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10LevelSettings (ViSession instrumentHandle,
                                                         ViInt32 automaticReferenceLevel,
                                                         ViReal64 referenceLevel,
                                                         ViReal64 externalAttenuation,
                                                         ViInt32 powerClass,
                                                         ViInt32 staticPCL);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10CaptureSettings
             (ViSession instrumentHandle, ViInt32 signalSource,
              ViReal64 captureTime, ViInt32 triggerMode, ViReal64 triggerOffset,
              ViReal64 triggerLevel, ViInt32 statisticCount);
ViStatus _VI_FUNC rsspecan_QueryGSMK10StatisticCount (ViSession instrumentHandle,
                                                      ViInt32* statisticCount);
ViStatus _VI_FUNC rsspecan_QueryGSMK10TriggerToSyncNumberOfAcquisitions
             (ViSession instrumentHandle, ViInt32* numberOfAcquisitions);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10CorrelationThreshold
             (ViSession instrumentHandle, ViReal64 correlationThreshold);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Synchronization
             (ViSession instrumentHandle, ViInt32 synchronization,
              ViBoolean measureOnlyOnSync);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AutoTrackTime (ViSession instrumentHandle,
                                                         ViReal64 autoTrackTime);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10SwapIQ (ViSession instrumentHandle,
                                                  ViBoolean swapIQ);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10MarkerZoom (ViSession instrumentHandle,
                                                      ViInt32 zoomFactor);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10ResultDisplay (ViSession instrumentHandle,
                                                         ViInt32 resultDisplay);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AutoSettings (ViSession instrumentHandle,
                                                        ViInt32 level,
                                                        ViInt32 frameConfiguration,
                                                        ViInt32 trigger);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DemodulationSingleSlotMeasurements
             (ViSession instrumentHandle, ViInt32 slotToMeasure);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DemodulationMultiSlotMeasurements
             (ViSession instrumentHandle, ViInt32 numberOfSlotsToMeasure,
              ViInt32 firstSlotToMeasure);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10DemodulationEqualTimeslotLength
             (ViSession instrumentHandle, ViBoolean equalTimeslotLength);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Burst (ViSession instrumentHandle,
                                                 ViInt32 slot, ViBoolean active,
                                                 ViInt32 burstType, ViInt32 PCL,
                                                 ViInt32 modulation, ViInt32 filter,
                                                 ViInt32 TSC);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10UserTSC (ViSession instrumentHandle,
                                                   ViInt32 slot, ViString userTSC);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AccessBurst (ViSession instrumentHandle,
                                                       ViInt32 slot,
                                                       ViBoolean active,
                                                       ViInt32 timingAdvance,
                                                       ViInt32 sync,
                                                       ViString userTSC);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10AQPSKBurst (ViSession instrumentHandle,
                                                      ViInt32 slot,
                                                      ViBoolean active,
                                                      ViReal64 SCPIR,
                                                      ViInt32 subchannel1TSC,
                                                      ViString subchannel1UserTSC,
                                                      ViInt32 subchannel2TSC,
                                                      ViString subchannel2UserTSC);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10TriggerToSync (ViSession instrumentHandle,
                                                         ViInt32 numberOfBins,
                                                         ViInt32 adaptiveDataSize);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10WideSpectrumResultsUnit
             (ViSession instrumentHandle, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Measurement (ViSession instrumentHandle,
                                                       ViInt32 measurementMode);
ViStatus _VI_FUNC rsspecan_ConfigureGsmK10PVTMeasurement (ViSession instrumentHandle,
                                                          ViInt32 burstSection);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10PVTFilter (ViSession instrumentHandle,
                                                     ViInt32 powerVsTimeFilter);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10PVTAlignment (ViSession instrumentHandle,
                                                        ViInt32 alignment);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10ModulationTransientSpectrum
             (ViSession instrumentHandle, ViBoolean enableLeftLimit,
              ViBoolean enableRightLimit);
ViStatus
    _VI_FUNC rsspecan_ConfigureGSMK10ModulationTransientSpectrumAdditional
        (ViSession instrumentHandle, ViInt32 filterType,
         ViInt32 transientReferencePower, ViInt32 offsetFrequency,
         ViInt32 modulationRBWAt1800kHz, ViBoolean highDynamic,
         ViInt32 modulationFrequencyList);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10Demodulation (ViSession instrumentHandle,
                                                        ViInt32 symbolDecision,
                                                        ViInt32 tailTSCBits);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10MultiMeasurement
             (ViSession instrumentHandle, ViBoolean multipleMeasurementMode,
              ViBoolean powerVsTime, ViBoolean demodulation,
              ViBoolean constellation, ViBoolean modulationSpectrum,
              ViBoolean transientSpectrum);
ViStatus _VI_FUNC rsspecan_ConfigureGSMK10MultiCarrier (ViSession instrumentHandle,
                                                        ViBoolean multiCarrierBTS,
                                                        ViInt32 numberOfActiveCarriers,
                                                        ViInt32 BTSClass,
                                                        ViInt32 pvTFilter);
ViStatus _VI_FUNC rsspecan_GSMK10RefreshCapturedData (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ReadGSMK10MeasurementResults (ViSession instrumentHandle,
                                                         ViUInt32 timeout,
                                                         ViInt32 measurement,
                                                         ViInt32 modifier,
                                                         ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchGSMK10MeasurementResults (ViSession instrumentHandle,
                                                          ViInt32 measurement,
                                                          ViInt32 modifier,
                                                          ViReal64* result);
ViStatus _VI_FUNC rsspecan_ReadGSMK10ModulationAccuracyResults
             (ViSession instrumentHandle, ViUInt32 timeout, ViInt32 arraySize,
              ViReal64 _VI_FAR result[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchGSMK10ModulationAccuracyResults
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR result[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ReadGSMK10BurstSlotResults (ViSession instrumentHandle,
                                                       ViUInt32 timeout,
                                                       ViInt32 slot,
                                                       ViInt32 burstSelection,
                                                       ViInt32 modifier,
                                                       ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchGSMK10BurstSlotResults (ViSession instrumentHandle,
                                                        ViInt32 slot,
                                                        ViInt32 burstSelection,
                                                        ViInt32 modifier,
                                                        ViReal64* result);
ViStatus _VI_FUNC rsspecan_ReadGSMK10BurstSlotDeltaToSync
             (ViSession instrumentHandle, ViUInt32 timeout, ViInt32 slot,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchGSMK10BurstSlotDeltaToSync
             (ViSession instrumentHandle, ViInt32 slot, ViReal64* result);
ViStatus _VI_FUNC rsspecan_ReadGSMK10SpectrumReferencePower
             (ViSession instrumentHandle, ViUInt32 timeout, ViInt32 spectrumType,
              ViReal64* level1, ViReal64* level2, ViReal64* resolutionBW);
ViStatus _VI_FUNC rsspecan_FetchGSMK10SpectrumReferencePower
             (ViSession instrumentHandle, ViInt32 spectrumType, ViReal64* level1,
              ViReal64* level2, ViReal64* resolutionBW);
ViStatus _VI_FUNC rsspecan_ReadGSMK10ModulationSpectrumResults
             (ViSession instrumentHandle, ViInt32 arraySize, ViUInt32 timeout,
              ViInt32 _VI_FAR index[], ViReal64 _VI_FAR startFrequencies[],
              ViReal64 _VI_FAR stopFrequencies[], ViReal64 _VI_FAR levels[],
              ViReal64 _VI_FAR limits[], ViInt32 _VI_FAR absRel[],
              ViInt32 _VI_FAR status[], ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_FetchGSMK10ModulationSpectrumResults
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR index[], ViReal64 _VI_FAR startFrequencies[],
              ViReal64 _VI_FAR stopFrequencies[], ViReal64 _VI_FAR levels[],
              ViReal64 _VI_FAR limits[], ViInt32 _VI_FAR absRel[],
              ViInt32 _VI_FAR status[], ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_ReadGSMK10TransientSpectrumResults
             (ViSession instrumentHandle, ViInt32 arraySize, ViUInt32 timeout,
              ViInt32 _VI_FAR index[], ViReal64 _VI_FAR startFrequencies[],
              ViReal64 _VI_FAR stopFrequencies[], ViReal64 _VI_FAR levels[],
              ViReal64 _VI_FAR limits[], ViInt32 _VI_FAR absRel[],
              ViInt32 _VI_FAR status[], ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_FetchGSMK10TransientSpectrumResults
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR index[], ViReal64 _VI_FAR startFrequencies[],
              ViReal64 _VI_FAR stopFrequencies[], ViReal64 _VI_FAR levels[],
              ViReal64 _VI_FAR limits[], ViInt32 _VI_FAR absRel[],
              ViInt32 _VI_FAR status[], ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_ReadGSMK10WideSpectrumResults (ViSession instrumentHandle,
                                                          ViInt32 arraySize,
                                                          ViUInt32 timeout,
                                                          ViInt32 _VI_FAR index[],
                                                          ViReal64 _VI_FAR absoluteOffsetFrequencies1[],
                                                          ViReal64 _VI_FAR absoluteOffsetFrequencies2[],
                                                          ViReal64 _VI_FAR levels[],
                                                          ViReal64 _VI_FAR limits[],
                                                          ViInt32 _VI_FAR absRel[],
                                                          ViInt32 _VI_FAR status[],
                                                          ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_FetchGSMK10WideSpectrumResults
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR index[],
              ViReal64 _VI_FAR absoluteOffsetFrequencies1[],
              ViReal64 _VI_FAR absoluteOffsetFrequencies2[],
              ViReal64 _VI_FAR levels[], ViReal64 _VI_FAR limits[],
              ViInt32 _VI_FAR absRel[], ViInt32 _VI_FAR status[],
              ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_ReadGSMK10WideSpectrumGating (ViSession instrumentHandle,
                                                         ViUInt32 timeout,
                                                         ViReal64* triggerOffset,
                                                         ViReal64* gateLength);
ViStatus _VI_FUNC rsspecan_FetchGSMK10MagnitudeCaptureResults
             (ViSession instrumentHandle, ViInt32 arraySize, ViInt32 resultType,
              ViReal64 _VI_FAR position[], ViReal64 _VI_FAR length[],
              ViInt32* numOfResults);
ViStatus _VI_FUNC rsspecan_SpectrogramMode (ViSession instrumentHandle,
                                            ViBoolean state);
ViStatus _VI_FUNC rsspecan_RealtimeSpectrumAnalysisMode
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SpectrogramWindow (ViSession instrumentHandle,
                                              ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramSweep (ViSession instrumentHandle,
                                                      ViBoolean continuous);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDisplayParameters
             (ViSession instrumentHandle, ViInt32 color, ViInt32 size);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramColors (ViSession instrumentHandle,
                                                       ViReal64 shape,
                                                       ViInt32 colorStyle,
                                                       ViReal64 colorLower,
                                                       ViReal64 colorUpper);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDefaultColors
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMeasurementParameters
             (ViSession instrumentHandle, ViInt32 historyDepth, ViBoolean timeStamp,
              ViInt32 frameCount, ViBoolean continueFrame);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramSelectedFrame
             (ViSession instrumentHandle, ViInt32 frameNumber);
ViStatus _VI_FUNC rsspecan_SpectrogramClear (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramFFTWindow (ViSession instrumentHandle,
                                                          ViInt32 FFTWindowType);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMarkerFrameNumber
             (ViSession instrumentHandle, ViInt32 markerNumber,
              ViInt32 frameNumber);
ViStatus _VI_FUNC rsspecan_GetSpectrogramMarkerFrameNumber
             (ViSession instrumentHandle, ViInt32 markerNumber,
              ViInt32* frameNumber);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMarkerSearchArea
             (ViSession instrumentHandle, ViInt32 markerNumber, ViInt32 searchArea);
ViStatus _VI_FUNC rsspecan_SpectrogramMarkerSearch (ViSession instrumentHandle,
                                                    ViInt32 markerNumber,
                                                    ViInt32 searchMode);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDeltaMarkerFrameNumber
             (ViSession instrumentHandle, ViInt32 deltaMarkerNumber,
              ViInt32 frameNumber);
ViStatus _VI_FUNC rsspecan_GetSpectrogramDeltaMarkerFrameNumber
             (ViSession instrumentHandle, ViInt32 deltaMarker,
              ViInt32* frameNumber);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramDeltaMarkerSearchArea
             (ViSession instrumentHandle, ViInt32 deltaMarker, ViInt32 searchArea);
ViStatus _VI_FUNC rsspecan_SpectrogramDeltaMarkerSearch (ViSession instrumentHandle,
                                                         ViInt32 deltaMarker,
                                                         ViInt32 searchMode);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumParameters
             (ViSession instrumentHandle, ViInt32 trace,
              ViBoolean enablePersistence, ViReal64 duration, ViReal64 granularity);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumDisplayParameters
             (ViSession instrumentHandle, ViReal64 shape, ViInt32 style,
              ViReal64 colorLower, ViReal64 colorUpper);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumDefaultColors
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumTruncate
             (ViSession instrumentHandle, ViBoolean truncate);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumTraceStyle
             (ViSession instrumentHandle, ViInt32 trace, ViInt32 symbol);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumMaxholdIntensity
             (ViSession instrumentHandle, ViInt32 trace, ViBoolean enableMaxhold,
              ViInt32 intensity);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumMaxholdReset
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumMarkerPosition
             (ViSession instrumentHandle, ViInt32 marker, ViInt32 markerPostion);
ViStatus _VI_FUNC rsspecan_QueryPersistenceMarkerZAxis (ViSession instrumentHandle,
                                                        ViInt32 marker,
                                                        ViReal64* zAxis);
ViStatus _VI_FUNC rsspecan_ConfigurePersistenceSpectrumDeltaMarkerPosition
             (ViSession instrumentHandle, ViInt32 marker,
              ViInt32 deltaMarkerPostion);
ViStatus _VI_FUNC rsspecan_QueryPersistenceDeltaMarkerZAxis
             (ViSession instrumentHandle, ViInt32 deltaMarker, ViReal64* zAxis);
ViStatus _VI_FUNC rsspecan_SelectRealtimeSpectrumMeasurement
             (ViSession instrumentHandle, ViInt32 realtimeMeasurement);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramResultDisplayMode
             (ViSession instrumentHandle, ViInt32 resultDisplay);
ViStatus _VI_FUNC rsspecan_GetSpectrogramTimeStamp (ViSession instrumentHandle,
                                                    ViInt32 frame,
                                                    ViInt32 arraySize,
                                                    ViInt32 _VI_FAR part1[],
                                                    ViInt32 _VI_FAR part2[],
                                                    ViInt32 _VI_FAR part3[],
                                                    ViInt32 _VI_FAR part4[],
                                                    ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ReadSpectrogramTraceData (ViSession instrumentHandle,
                                                     ViInt32 arraySize,
                                                     ViReal64 _VI_FAR results[],
                                                     ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ReadPersistenceSpectrumTraceData
             (ViSession instrumentHandle, ViUInt32 timeout, ViInt32 arraySize,
              ViReal64 _VI_FAR results[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ReadMaxholdTraceData (ViSession instrumentHandle,
                                                 ViUInt32 timeout,
                                                 ViInt32 arraySize,
                                                 ViReal64 _VI_FAR results[],
                                                 ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetSpectrogramXAxisScaling (ViSession instrumentHandle,
                                                       ViInt32 frame,
                                                       ViInt32 arraySize,
                                                       ViReal64 _VI_FAR xAxis[],
                                                       ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_StoreSpectrogramToFile (ViSession instrumentHandle,
                                                   ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramContinuousTrigger
             (ViSession instrumentHandle, ViInt32 trigger);
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramTimeDomainTrigger
             (ViSession instrumentHandle, ViReal64 timeDomainTriggerLevel);
ViStatus _VI_FUNC rsspecan_AvionicsMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDemodulationMode
             (ViSession instrumentHandle, ViInt32 demodulationMode);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDistortionResult
             (ViSession instrumentHandle, ViInt32 type);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsTHDUnit (ViSession instrumentHandle,
                                                     ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDDMUnit (ViSession instrumentHandle,
                                                     ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsInput (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 input);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDemodulationBandwidth
             (ViSession instrumentHandle, ViInt32 window, ViBoolean autoBandwidth,
              ViReal64 demodulationBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFFrequencyStartStop
             (ViSession instrumentHandle, ViReal64 startFrequency,
              ViReal64 stopFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFFrequencyCenterSpan
             (ViSession instrumentHandle, ViReal64 centerFrequency, ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFFullSpan (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsAFParam (ViSession instrumentHandle,
                                                     ViReal64 resolutionBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureAvionicsDistortionMeasurement
             (ViSession instrumentHandle, ViBoolean SHDState,
              ViReal64 SHDHarmonicFrequency, ViInt32 THDFundamentalFrequency,
              ViReal64 THDMaxFrequency);
ViStatus _VI_FUNC rsspecan_QueryAvionicsRFFrequency (ViSession instrumentHandle,
                                                     ViReal64* RFFrequencyResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsCarrierOffset (ViSession instrumentHandle,
                                                       ViReal64* carrierOffsetResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsRFLevel (ViSession instrumentHandle,
                                                 ViReal64* RFLevelResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsAM (ViSession instrumentHandle,
                                            ViInt32 AMDepth,
                                            ViReal64* AMDepthResult,
                                            ViInt32 AMFrequency,
                                            ViReal64* AMFrequencyResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsTHD (ViSession instrumentHandle,
                                             ViInt32 THD, ViReal64* THDResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsDifferenceInDepth
             (ViSession instrumentHandle, ViReal64* DDMResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsFM (ViSession instrumentHandle,
                                            ViReal64* FMDeviationResult,
                                            ViReal64* FMFrequencyResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsPhase (ViSession instrumentHandle,
                                               ViReal64* phaseResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsInputLevel (ViSession instrumentHandle,
                                                    ViReal64* inputLevelResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsSDM (ViSession instrumentHandle,
                                             ViReal64* SDMResult);
ViStatus _VI_FUNC rsspecan_QueryAvionicsSHD (ViSession instrumentHandle,
                                             ViReal64* SHDResult);
ViStatus _VI_FUNC rsspecan_MultiCarrierGroupDelayMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayInput
             (ViSession instrumentHandle, ViInt32 inputCoupling, ViInt32 impedance,
              ViBoolean highpassFilter, ViBoolean YIGPreselector);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayReferenceLevel
             (ViSession instrumentHandle, ViInt32 window, ViReal64 referenceLevel,
              ViReal64 referenceLevelOffset);
ViStatus
    _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayMechanicalAttenuator
        (ViSession instrumentHandle, ViBoolean autoMode, ViReal64 value);
ViStatus
    _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayElectronicAttenuator
        (ViSession instrumentHandle, ViBoolean state,
         ViBoolean electronicAttenuatorAuto, ViInt32 manualValuedB);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayPreamplifier
             (ViSession instrumentHandle, ViBoolean state,
              ViInt32 preamplifierLevel);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayYAxisScaling
             (ViSession instrumentHandle, ViInt32 window, ViReal64 range,
              ViReal64 referencePosition, ViInt32 verticalScale,
              ViInt32 yAxisGridMode);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayYAxisScalingMaxMin
             (ViSession instrumentHandle, ViInt32 window, ViReal64 maximumValue,
              ViReal64 miniumumValue);
ViStatus
    _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayYAxisScalingReferencePerDivision
        (ViSession instrumentHandle, ViInt32 window, ViReal64 scaling,
         ViReal64 referencePosition, ViReal64 referenceValue);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayPhaseUnit
             (ViSession instrumentHandle, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelaySignalDescription
             (ViSession instrumentHandle, ViInt32 numberOfCarriers,
              ViReal64 carrierSpacing);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelayMeasurementTime
             (ViSession instrumentHandle, ViBoolean mode, ViReal64 measurementTime);
ViStatus _VI_FUNC rsspecan_ConfigureMultiCarrierGroupDelaySettings
             (ViSession instrumentHandle, ViInt32 sweepCount, ViInt32 groupDelay,
              ViInt32 carrierOffset);
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayCarrierOffset
             (ViSession instrumentHandle, ViReal64* carrierOffset);
ViStatus _VI_FUNC rsspecan_InitiateMultiCarrierGroupDelayCalibration
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayCalibrationStatus
             (ViSession instrumentHandle, ViInt32* calibrationStatus);
ViStatus _VI_FUNC rsspecan_StoreMultiCarrierGroupDelayCalibrationData
             (ViSession instrumentHandle, ViString fileName);
ViStatus _VI_FUNC rsspecan_LoadMultiCarrierGroupDelayCalibrationData
             (ViSession instrumentHandle, ViString fileName);
ViStatus _VI_FUNC rsspecan_StoreMultiCarrierGroupDelayMeasurementResults
             (ViSession instrumentHandle, ViString fileName);
ViStatus _VI_FUNC rsspecan_StoreMultiCarrierGroupDelayTraceToFile
             (ViSession instrumentHandle, ViInt32 window, ViInt32 trace,
              ViString fileName);
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayNumberOfTracePoints
             (ViSession instrumentHandle, ViInt32 window, ViInt32 trace,
              ViInt32* numberOfTracePoints);
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayMeasurementBandwidth
             (ViSession instrumentHandle, ViReal64* bandwidth);
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelayRecordLength
             (ViSession instrumentHandle, ViInt32* recordLength);
ViStatus _VI_FUNC rsspecan_QueryMultiCarrierGroupDelaySampleRate
             (ViSession instrumentHandle, ViReal64* sampleRate);
ViStatus _VI_FUNC rsspecan_CATVMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureCATVChannelSetup (ViSession instrumentHandle,
                                                      ViString channelTableName,
                                                      ViInt32 measurementChannel);
ViStatus _VI_FUNC rsspecan_ConfigureCATVChannelFrequency (ViSession instrumentHandle,
                                                          ViReal64 RFFrequency,
                                                          ViInt32 axisLabeling);
ViStatus _VI_FUNC rsspecan_ConfigureCATVChannelFrequencyStepSize
             (ViSession instrumentHandle, ViReal64 stepSize);
ViStatus _VI_FUNC rsspecan_ConfigureCATVModulationStandard
             (ViSession instrumentHandle, ViString modulationStandard,
              ViInt32 signalType, ViInt32 sidebandPosition);
ViStatus _VI_FUNC rsspecan_CATVChannelAdjustAtt (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_CATVAutoscale (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureCATVMarkerState (ViSession instrumentHandle,
                                                     ViInt32 markerNumber,
                                                     ViBoolean markerState);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDeltaMarkerState (ViSession instrumentHandle,
                                                          ViInt32 deltaMarkerNumber,
                                                          ViBoolean deltaMarkerState);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAMeasurement (ViSession instrumentHandle,
                                                      ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAModulStandard (ViSession instrumentHandle,
                                                        ViInt32 TVStandard,
                                                        ViInt32 soundSystem,
                                                        ViInt32 groupDelay,
                                                        ViInt32 colorSystem,
                                                        ViInt32 barField,
                                                        ViInt32 barLine,
                                                        ViInt32 barLineType,
                                                        ViInt32 quietField,
                                                        ViInt32 quietLine);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVideoScopeLine (ViSession instrumentHandle,
                                                         ViInt32 line);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVideoScopeField (ViSession instrumentHandle,
                                                          ViInt32 activeField);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNMeasurement (ViSession instrumentHandle,
                                                        ViInt32 measCarrier,
                                                        ViInt32 measurementMethod,
                                                        ViReal64 noiseReferenceBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNReferencePower
             (ViSession instrumentHandle, ViInt32 referencePower,
              ViInt32 referenceChannel, ViReal64 powerManual);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNMeasurementFrequencies
             (ViSession instrumentHandle, ViInt32 tableRow, ViBoolean state,
              ViReal64 centerFrequency, ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACNNoiseFloorCorrection
             (ViSession instrumentHandle, ViBoolean noiseFloorCorrection,
              ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_CATVACNNextMeasFrequency (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSOMeasurement (ViSession instrumentHandle,
                                                         ViInt32 measCarrier,
                                                         ViInt32 measurementMethod);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSOReferencePower
             (ViSession instrumentHandle, ViInt32 referencePower,
              ViInt32 referenceChannel, ViReal64 powerManual);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSOMeasurementFrequencies
             (ViSession instrumentHandle, ViInt32 tableRow, ViBoolean state,
              ViReal64 centerFrequency, ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACSONoiseFloorCorrection
             (ViSession instrumentHandle, ViBoolean noiseFloorCorrection,
              ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_CATVACSONextMeasFrequency (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBMeasurement (ViSession instrumentHandle,
                                                         ViInt32 measCarrier);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBReferencePower
             (ViSession instrumentHandle, ViInt32 referencePower,
              ViInt32 referenceChannel, ViReal64 powerManual);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBMeasurementFrequencies
             (ViSession instrumentHandle, ViInt32 tableRow, ViBoolean state,
              ViReal64 centerFrequency, ViReal64 span);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACTBNoiseFloorCorrection
             (ViSession instrumentHandle, ViBoolean noiseFloorCorrection,
              ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_CATVACTBNextMeasFrequency (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACarrierLimit (ViSession instrumentHandle,
                                                       ViInt32 measurement,
                                                       ViInt32 limitType,
                                                       ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVACarrierRatioLimit
             (ViSession instrumentHandle, ViInt32 measurement, ViInt32 limitType,
              ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAHumLimit (ViSession instrumentHandle,
                                                   ViInt32 limitType,
                                                   ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVisionModLimit (ViSession instrumentHandle,
                                                         ViInt32 limit,
                                                         ViInt32 limitType,
                                                         ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAVisionModulationCarrierPowerUnit
             (ViSession instrumentHandle, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureCATVAHumUnit (ViSession instrumentHandle,
                                                  ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDMeasurement (ViSession instrumentHandle,
                                                      ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDModulStandard (ViSession instrumentHandle,
                                                        ViInt32 TVStandard,
                                                        ViInt32 rollOff,
                                                        ViInt32 constellationParameter,
                                                        ViReal64 symbolRate);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDShoulderAttenuation
             (ViSession instrumentHandle, ViBoolean shoulderAttenuation);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDEchoPattern (ViSession instrumentHandle,
                                                      ViBoolean zoomState,
                                                      ViInt32 zoomFactor,
                                                      ViReal64 velocityFactor);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDModulErrorZoom (ViSession instrumentHandle,
                                                         ViInt32 zoom);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDOverviewZoom (ViSession instrumentHandle,
                                                       ViInt32 zoom);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDConstellationDiagramZoom
             (ViSession instrumentHandle, ViInt32 zoomDiagram);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDEqualizer (ViSession instrumentHandle,
                                                    ViBoolean state,
                                                    ViBoolean freeze);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDSignalStatisticsNoSamples
             (ViSession instrumentHandle, ViInt32 noOfSamples);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDSignalStatisticsScaling
             (ViSession instrumentHandle, ViInt32 scalingMode,
              ViReal64 scalingValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDCCDFPercentMark (ViSession instrumentHandle,
                                                          ViInt32 markerNumber,
                                                          ViReal64 positionValue);
ViStatus _VI_FUNC rsspecan_CATVDResetEqualizer (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDOverLimit (ViSession instrumentHandle,
                                                    ViInt32 measurement,
                                                    ViInt32 limitType,
                                                    ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDChanPowLimit (ViSession instrumentHandle,
                                                       ViInt32 limitType,
                                                       ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDModErrLimit (ViSession instrumentHandle,
                                                      ViInt32 measurement,
                                                      ViInt32 limitType,
                                                      ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDEchoPatternUnit (ViSession instrumentHandle,
                                                          ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureCATVDUnit (ViSession instrumentHandle,
                                               ViInt32 measurement, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureCATVTVMeasurement (ViSession instrumentHandle,
                                                       ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureCATVTVStandardForTitlMeasurement
             (ViSession instrumentHandle, ViString modulationStandard,
              ViBoolean state);
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierResult (ViSession instrumentHandle,
                                                    ViInt32 measurement,
                                                    ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierAllResults (ViSession instrumentHandle,
                                                        ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierRatioResult (ViSession instrumentHandle,
                                                         ViInt32 measurement,
                                                         ViInt32 resultType,
                                                         ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierAllRatioResults
             (ViSession instrumentHandle, ViInt32 measurement,
              ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryCATVAHumResult (ViSession instrumentHandle,
                                                ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVAVisionModulationResult
             (ViSession instrumentHandle, ViInt32 measurement, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVAVisionAllModulationResults
             (ViSession instrumentHandle, ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierLimitResult (ViSession instrumentHandle,
                                                         ViInt32 measurement,
                                                         ViInt32 limitType,
                                                         ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryCATVACarrierRatioLimitResult
             (ViSession instrumentHandle, ViInt32 measurement, ViInt32 limitType,
              ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryCATVAHumLimitResult (ViSession instrumentHandle,
                                                     ViInt32 limitType,
                                                     ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryCATVAVisionModulationLimitResult
             (ViSession instrumentHandle, ViInt32 measurement, ViInt32 limitType,
              ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDSpectrumResult (ViSession instrumentHandle,
                                                     ViInt32 measurement,
                                                     ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDChanelPowerResult (ViSession instrumentHandle,
                                                        ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDOverviewResult (ViSession instrumentHandle,
                                                     ViInt32 measurement,
                                                     ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDOverviewAllResults (ViSession instrumentHandle,
                                                         ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryCATVDModulationErrorsResult
             (ViSession instrumentHandle, ViInt32 measurement, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDModulationErrorsAllResults
             (ViSession instrumentHandle, ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryCATVDSignalStatisticsResult
             (ViSession instrumentHandle, ViInt32 traceNumber, ViInt32 resultType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDSignalStatisticsAllResults
             (ViSession instrumentHandle, ViInt32 traceNumber,
              ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_QueryCATVDOverviewLimitResult (ViSession instrumentHandle,
                                                          ViInt32 measurement,
                                                          ViInt32 limitType,
                                                          ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDChannelPowerLimitResult
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryCATVDModulationErrorsLimitResult
             (ViSession instrumentHandle, ViInt32 measurement, ViInt32 limitType,
              ViInt32* result);
ViStatus _VI_FUNC rsspecan_NoiseMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseFrequencySettings
             (ViSession instrumentHandle, ViReal64 startFrequency,
              ViReal64 stopFrequency, ViReal64 stepFrequency, ViInt32 mode,
              ViReal64 fixedLO, ViReal64 fixedIF, ViReal64 imageRejection,
              ViReal64 fixedFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseFrequencyTable (ViSession instrumentHandle,
                                                         ViInt32 numberOfValues,
                                                         ViReal64 _VI_FAR receiverFrequencyValues[],
                                                         ViReal64 _VI_FAR localOscilatorFrequencyVal[],
                                                         ViReal64 _VI_FAR intermediateFrequencyValues[]);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseMeasurementMode (ViSession instrumentHandle,
                                                          ViInt32 measurementMode);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseCalibration (ViSession instrumentHandle,
                                                      ViBoolean _2ndStageCorrection);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseAnalyzerSettings
             (ViSession instrumentHandle, ViReal64 resolutionBandwidth,
              ViReal64 sweepTime, ViReal64 settlingTime, ViInt32 average,
              ViReal64 RFAttenuation, ViReal64 range, ViBoolean automaticRefLevel,
              ViReal64 refLevel, ViBoolean preselector,
              ViBoolean preamplifier_B2B25);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGeneratorSettings
             (ViSession instrumentHandle, ViBoolean automaticControl,
              ViReal64 level, ViString generatorSelect, ViInt32 GPIBAddress,
              ViBoolean initBeforeMeasurement);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGeneratorAutoSwitchOff
             (ViSession instrumentHandle, ViBoolean automaticSwitchOff);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGeneratorFrequency
             (ViSession instrumentHandle, ViReal64 offsetNr1, ViReal64 offsetNr2,
              ViInt32 numerator, ViInt32 denominator);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseSingleFrequency (ViSession instrumentHandle,
                                                          ViReal64 singleFrequency,
                                                          ViBoolean coupledToSweepList);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRSettings (ViSession instrumentHandle,
                                                      ViInt32 selectionMode,
                                                      ViReal64 ENRConstant,
                                                      ViReal64 roomTemperature);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRTable (ViSession instrumentHandle,
                                                   ViInt32 numberOfValues,
                                                   ViReal64 _VI_FAR receiverFrequencyValues[],
                                                   ViReal64 _VI_FAR ENRValues[]);
ViStatus _VI_FUNC rsspecan_NoiseENRTableOperations (ViSession instrumentHandle,
                                                    ViInt32 operation,
                                                    ViString tableName);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRCommonMode (ViSession instrumentHandle,
                                                        ViBoolean commonMode);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseENRCalibrationSettings
             (ViSession instrumentHandle, ViInt32 selectionMode,
              ViReal64 ENRConstant);
ViStatus _VI_FUNC rsspecan_SelectNoiseENRCalibrationTable
             (ViSession instrumentHandle, ViString tableName);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossInputSettings
             (ViSession instrumentHandle, ViInt32 selectionMode,
              ViReal64 lossInputConstant);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossInputTable (ViSession instrumentHandle,
                                                         ViInt32 numberOfValues,
                                                         ViReal64 _VI_FAR receiverFrequencyValues[],
                                                         ViReal64 _VI_FAR lossInputValues[]);
ViStatus _VI_FUNC rsspecan_NoiseLossInputTableOperations (ViSession instrumentHandle,
                                                          ViInt32 operation,
                                                          ViString tableName);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossOutputSettings
             (ViSession instrumentHandle, ViInt32 selectionMode,
              ViReal64 lossOutputConstant);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLossOutputTable (ViSession instrumentHandle,
                                                          ViInt32 numberOfValues,
                                                          ViReal64 _VI_FAR receiverFrequencyValues[],
                                                          ViReal64 _VI_FAR lossOutputValues[]);
ViStatus _VI_FUNC rsspecan_NoiseLossOutputTableOperations
             (ViSession instrumentHandle, ViInt32 operation, ViString tableName);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseResultSettings (ViSession instrumentHandle,
                                                         ViInt32 combinedTraceDisplay,
                                                         ViBoolean tableFormResults);
ViStatus _VI_FUNC rsspecan_SelectWindow (ViSession instrumentHandle,
                                         ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseTraceSettings (ViSession instrumentHandle,
                                                        ViInt32 yAxis,
                                                        ViBoolean symbols,
                                                        ViBoolean automaticScaling,
                                                        ViReal64 minYAxis,
                                                        ViReal64 maxYAxis);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseGainTraceSettings
             (ViSession instrumentHandle, ViBoolean yAxis, ViBoolean symbols,
              ViBoolean automaticScaling, ViReal64 minYAxis, ViReal64 maxYAxis);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseTraceMemoryDisplay
             (ViSession instrumentHandle, ViInt32 memory, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseCurrentResultsDisplay
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureXAxisFrequencyDisplay
             (ViSession instrumentHandle, ViInt32 measurementResults,
              ViInt32 frequency);
ViStatus _VI_FUNC rsspecan_CreateNoiseLimitLine (ViSession instrumentHandle,
                                                 ViInt32 limit, ViString name,
                                                 ViString comment);
ViStatus _VI_FUNC rsspecan_DefineNoiseLimitLine (ViSession instrumentHandle,
                                                 ViInt32 limit, ViInt32 resultType,
                                                 ViInt32 type, ViInt32 count,
                                                 ViReal64 _VI_FAR xAxis[],
                                                 ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseLimitLineType (ViSession instrumentHandle,
                                                        ViInt32 limit,
                                                        ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_AssignNoiseLimitLineToTrace (ViSession instrumentHandle,
                                                        ViInt32 assignToTrace);
ViStatus _VI_FUNC rsspecan_EnableNoiseLimitCheck (ViSession instrumentHandle,
                                                  ViInt32 limit, ViInt32 type,
                                                  ViBoolean limitEnabled,
                                                  ViBoolean checkEnabled);
ViStatus _VI_FUNC rsspecan_MoveNoiseLimitLine (ViSession instrumentHandle,
                                               ViInt32 limit, ViInt32 type,
                                               ViReal64 value);
ViStatus _VI_FUNC rsspecan_CopyNoiseLimitLine (ViSession instrumentHandle,
                                               ViInt32 limit, ViInt32 copyTo);
ViStatus _VI_FUNC rsspecan_ClearNoiseLimitLineResults (ViSession instrumentHandle,
                                                       ViInt32 limit);
ViStatus _VI_FUNC rsspecan_DeleteNoiseLimitLine (ViSession instrumentHandle,
                                                 ViInt32 limit);
ViStatus _VI_FUNC rsspecan_SaveNoiseTraceDataToMemory (ViSession instrumentHandle,
                                                       ViInt32 memory);
ViStatus _VI_FUNC rsspecan_AssignMarkerToTrace (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 marker,
                                                ViInt32 trace);
ViStatus _VI_FUNC rsspecan_QueryNoiseMarkerAmplitude (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 marker,
                                                      ViInt32 resultType,
                                                      ViReal64* markerAmplitude);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyCommonSource
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintySourceCharacteristics
             (ViSession instrumentHandle, ViInt32 type, ViReal64 sourceOutput,
              ViReal64 ERNUncertainty);
ViStatus
    _VI_FUNC rsspecan_ConfigureNoiseUncertaintySourceCalibrationCharacteristics
        (ViSession instrumentHandle, ViInt32 type, ViReal64 sourceOutput,
         ViReal64 ERNUncertainty);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyDUTCharacteristics
             (ViSession instrumentHandle, ViInt32 direction, ViInt32 type,
              ViReal64 DUTUncertainty);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyMeasurementValues
             (ViSession instrumentHandle, ViBoolean autoo, ViReal64 noiseFigure,
              ViReal64 gain, ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ConfigureNoiseUncertaintyAnalyzerCharacteristics
             (ViSession instrumentHandle, ViBoolean preamplifierState,
              ViReal64 noiseFigure, ViReal64 gain, ViInt32 type,
              ViReal64 analyzerUncertainty);
ViStatus _VI_FUNC rsspecan_QueryNoiseUncertainty (ViSession instrumentHandle,
                                                  ViReal64* uncertainty);
ViStatus _VI_FUNC rsspecan_QuerySpectrumAnalyzerUncertainty
             (ViSession instrumentHandle, ViInt32 spectrumAnalyzer,
              ViReal64* uncertainty);
ViStatus _VI_FUNC rsspecan_InitNoiseGenerator (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_FetchNoiseArray (ViSession instrumentHandle,
                                            ViInt32 modifier, ViInt32 arraySize,
                                            ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_FetchNoiseScalar (ViSession instrumentHandle,
                                             ViInt32 modifier, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchNoiseMemoryArray (ViSession instrumentHandle,
                                                  ViInt32 modifier, ViInt32 memory,
                                                  ViInt32 arraySize,
                                                  ViReal64 _VI_FAR measResult[]);
ViStatus _VI_FUNC rsspecan_QueryNoiseResults (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 trace,
                                              ViInt32 resultType,
                                              ViInt32 arrayLength,
                                              ViInt32* actualPoints,
                                              ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_GetNoiseLimitCheckResult (ViSession instrumentHandle,
                                                     ViInt32 limit, ViInt32* state);
ViStatus _VI_FUNC rsspecan_PhaseMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSignalSettings (ViSession instrumentHandle,
                                                         ViReal64 frequency,
                                                         ViReal64 level,
                                                         ViBoolean sweepForward,
                                                         ViBoolean verifyFrequencyLevel,
                                                         ViReal64 frequencyToleranceRelative,
                                                         ViReal64 levelTolerance);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseFrequencyTolerance
             (ViSession instrumentHandle, ViReal64 absoluteFrequencyTolerance);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseAFCSettings (ViSession instrumentHandle,
                                                      ViBoolean trackFrequency,
                                                      ViBoolean trackLevel);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseAMRejection (ViSession instrumentHandle,
                                                      ViBoolean AMRejection);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseTrackingBW (ViSession instrumentHandle,
                                                     ViReal64 trackingBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDigitalPLL (ViSession instrumentHandle,
                                                     ViBoolean digitalPLL);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDecimation (ViSession instrumentHandle,
                                                     ViBoolean decimation);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDisplaySettings (ViSession instrumentHandle,
                                                          ViReal64 xAxisStart,
                                                          ViReal64 xAxisStop,
                                                          ViBoolean autoscaleOnce,
                                                          ViReal64 yAxisTop,
                                                          ViReal64 yAxisRange);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDisplayYAxisSettings
             (ViSession instrumentHandle, ViInt32 yAxisScaling, ViReal64 yAxisTop,
              ViReal64 yAxisRange, ViReal64 yAxisBottom);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseDisplayXAxisSettings
             (ViSession instrumentHandle, ViInt32 xAxisScope, ViReal64 xAxisStart,
              ViReal64 xAxisStop, ViReal64 halfDecade);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseTraceSettings (ViSession instrumentHandle,
                                                        ViReal64 traceOffset,
                                                        ViInt32 traceSmoothing,
                                                        ViInt32 smoothingType);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseResidualCalculations
             (ViSession instrumentHandle, ViBoolean useMeasurementSettings,
              ViReal64 evaluateFrom, ViReal64 evaluateTo);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseTraceCustomRange
             (ViSession instrumentHandle, ViInt32 window, ViInt32 userRange,
              ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseUserResidualCalculations
             (ViSession instrumentHandle, ViInt32 window, ViInt32 userRange,
              ViReal64 evaluateFrom, ViReal64 evaluateTo);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpotNoiseSettings
             (ViSession instrumentHandle, ViBoolean enable,
              ViReal64 offsetFrequency1, ViReal64 offsetFrequency2,
              ViReal64 offsetFrequency3, ViReal64 offsetFrequency4);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseAnalyzerSettings
             (ViSession instrumentHandle, ViBoolean preselector,
              ViBoolean preamplifier_B2B25B23);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpotNoiseCalculation
             (ViSession instrumentHandle, ViBoolean spotNoiseCalculation);
ViStatus _VI_FUNC rsspecan_SetPhaseAllSpotNoiseMarkersOff
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpurSettings (ViSession instrumentHandle,
                                                       ViBoolean spurSuppression,
                                                       ViReal64 spurThreshold);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSweepSettings (ViSession instrumentHandle,
                                                        ViInt32 sweepMode);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSignalVerificationFailed
             (ViSession instrumentHandle, ViInt32 stopOnVerifyFailed);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseSpanSettings (ViSession instrumentHandle,
                                                       ViReal64 startOffset,
                                                       ViReal64 stopOffset);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseCarrierFrequencyOffsetTable
             (ViSession instrumentHandle, ViInt32 subband, ViReal64 RBW,
              ViInt32 average, ViInt32 FFT);
ViStatus _VI_FUNC rsspecan_ConfigurePhasePresetSettings (ViSession instrumentHandle,
                                                         ViReal64 RBW,
                                                         ViInt32 average,
                                                         ViInt32 FFT);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseGlobalIQWindowSettings
             (ViSession instrumentHandle, ViInt32 decade, ViInt32 subband,
              ViInt32 globalIQWindow);
ViStatus _VI_FUNC rsspecan_CreatePhaseLimitLine (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 limit,
                                                 ViString name, ViString comment,
                                                 ViInt32 assignToTrace);
ViStatus _VI_FUNC rsspecan_DefinePhaseLimitLine (ViSession instrumentHandle,
                                                 ViInt32 limit, ViInt32 type,
                                                 ViInt32 count,
                                                 ViReal64 _VI_FAR xAxis[],
                                                 ViReal64 _VI_FAR amplitude[]);
ViStatus _VI_FUNC rsspecan_EnablePhaseLimitCheck (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 limit,
                                                  ViInt32 type,
                                                  ViBoolean limitEnabled,
                                                  ViBoolean checkEnabled);
ViStatus _VI_FUNC rsspecan_MovePhaseLimitLine (ViSession instrumentHandle,
                                               ViInt32 limit, ViInt32 type,
                                               ViReal64 value);
ViStatus _VI_FUNC rsspecan_CopyPhaseLimitLine (ViSession instrumentHandle,
                                               ViInt32 limit, ViInt32 copyTo);
ViStatus _VI_FUNC rsspecan_ClearPhaseLimitLineResults (ViSession instrumentHandle,
                                                       ViInt32 limit);
ViStatus _VI_FUNC rsspecan_DeletePhaseLimitLine (ViSession instrumentHandle,
                                                 ViInt32 limit);
ViStatus _VI_FUNC rsspecan_GetActivePhaseLimitLine (ViSession instrumentHandle,
                                                    ViInt32 bufferSize,
                                                    ViChar _VI_FAR activeLimitLines[]);
ViStatus _VI_FUNC rsspecan_ConfigurePhaseNoiseLimitSettings
             (ViSession instrumentHandle, ViInt32 shapeType,
              ViString selectedTraces, ViReal64 noiseFloor, ViInt32 segment,
              ViReal64 cornerFrequency, ViReal64 slope);
ViStatus _VI_FUNC rsspecan_PhaseCopyToUserLimitLine (ViSession instrumentHandle,
                                                     ViInt32 limitLine);
ViStatus _VI_FUNC rsspecan_GetPhaseNoiseLimitCheckState (ViSession instrumentHandle,
                                                         ViInt32* limitCheck);
ViStatus _VI_FUNC rsspecan_TracePhaseSettings (ViSession instrumentHandle,
                                               ViInt32 traceNumber,
                                               ViBoolean traceState,
                                               ViInt32 traceMode,
                                               ViBoolean smoothing);
ViStatus _VI_FUNC rsspecan_ReferencePhaseMeasurement (ViSession instrumentHandle,
                                                      ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_GetPhaseEstimatedMeasurementTime
             (ViSession instrumentHandle, ViReal64* estimatedMeasurementTime);
ViStatus _VI_FUNC rsspecan_ReadPhaseTraceData (ViSession instrumentHandle,
                                               ViInt32 sourceTrace,
                                               ViInt32* noOfPoints,
                                               ViReal64 _VI_FAR traceDataX[],
                                               ViReal64 _VI_FAR traceDataY[]);
ViStatus _VI_FUNC rsspecan_FetchPhaseResidualResults (ViSession instrumentHandle,
                                                      ViInt32 modifier,
                                                      ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchPhaseResidualUserResults (ViSession instrumentHandle,
                                                          ViInt32 userRange,
                                                          ViInt32 modifier,
                                                          ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchPhaseSpurs (ViSession instrumentHandle,
                                            ViInt32* noOfPoints,
                                            ViReal64 _VI_FAR frequencyValues[],
                                            ViReal64 _VI_FAR levelValues[]);
ViStatus _VI_FUNC rsspecan_FetchPhaseSpursDiscrete (ViSession instrumentHandle,
                                                    ViReal64* discrete);
ViStatus _VI_FUNC rsspecan_FetchPhaseSpursRandom (ViSession instrumentHandle,
                                                  ViReal64* random);
ViStatus _VI_FUNC rsspecan_FetchPhaseFrequencyLevel (ViSession instrumentHandle,
                                                     ViReal64* frequency,
                                                     ViReal64* level);
ViStatus _VI_FUNC rsspecan_FetchPhaseIntegratedPhaseNoise
             (ViSession instrumentHandle, ViReal64* IPN);
ViStatus _VI_FUNC rsspecan_FetchPhaseUserIntegratedPhaseNoise
             (ViSession instrumentHandle, ViInt32 user, ViReal64* IPN);
ViStatus _VI_FUNC rsspecan_GetPhaseSpotNoiseYPosition (ViSession instrumentHandle,
                                                       ViInt32 spotNoiseMarkerNumber,
                                                       ViReal64* spotNoiseMarkerResult);
ViStatus _VI_FUNC rsspecan_GetPhaseDecadeSpotNoiseResult (ViSession instrumentHandle,
                                                          ViInt32 arraySize,
                                                          ViReal64 _VI_FAR verticalPosition[],
                                                          ViReal64 _VI_FAR horizontalPosition[],
                                                          ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetPhaseLimitCheckResult (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 limit,
                                                     ViInt32* state);
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementMarkerState
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementMarkerDetector
             (ViSession instrumentHandle, ViInt32 marker, ViInt32 detector);
ViStatus _VI_FUNC rsspecan_EMIFinalMeasurementMarkerPeakSearch
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementMarkerDwellTime
             (ViSession instrumentHandle, ViReal64 dwellTime);
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementMarkerLimitLine
             (ViSession instrumentHandle, ViInt32 marker, ViInt32 limit,
              ViInt32* markerCondition, ViReal64* markerVerticalDistance);
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementMarkerResult
             (ViSession instrumentHandle, ViInt32 marker, ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureEMIFinalMeasurementDeltaMarkerDetector
             (ViSession instrumentHandle, ViInt32 deltaMarker, ViInt32 detector);
ViStatus _VI_FUNC rsspecan_EMIFinalMeasurementDeltaMarkerPeakSearch
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementDeltaMarkerLimitLine
             (ViSession instrumentHandle, ViInt32 deltaMarker, ViInt32 limit,
              ViInt32* deltaMarkerCondition, ViReal64* deltaMarkerVerticalDistance);
ViStatus _VI_FUNC rsspecan_QueryEMIFinalMeasurementDeltaMarkerResult
             (ViSession instrumentHandle, ViInt32 deltaMarker, ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureEMILISNVnetworkType (ViSession instrumentHandle,
                                                         ViInt32 vNetworkType);
ViStatus _VI_FUNC rsspecan_ConfigureEMILISNVnetworkHighPassFilter
             (ViSession instrumentHandle, ViBoolean highPassFilter);
ViStatus _VI_FUNC rsspecan_ConfigureEMILISNVnetworkPhase (ViSession instrumentHandle,
                                                          ViInt32 phase);
ViStatus _VI_FUNC rsspecan_ConfigureEMIBandwithFilterType
             (ViSession instrumentHandle, ViInt32 filterType);
ViStatus _VI_FUNC rsspecan_ConfigureVSADigitalStandard (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViString standard,
                                                        ViBoolean stepWidthAuto,
                                                        ViReal64 stepSize);
ViStatus _VI_FUNC rsspecan_GetVSADigitalStandardCatalog (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32* numberOfDigitalStandards,
                                                         ViInt32 bufferSize,
                                                         ViChar _VI_FAR digitalStandardsList[]);
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchHysteresis
             (ViSession instrumentHandle, ViInt32 window, ViReal64 hysteresisLevel);
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchExpertSettings
             (ViSession instrumentHandle, ViInt32 window, ViInt32 usefulLength,
              ViInt32 maxLength, ViInt32 runIn, ViInt32 runOut, ViInt32 minGaplen);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEqualizer (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViReal64 speedOfConversion,
                                                  ViInt32 length);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEqualizerMode (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViBoolean state,
                                                      ViBoolean training);
ViStatus _VI_FUNC rsspecan_VSAEqualizerOperations (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 operation,
                                                   ViString equalizerName);
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulation (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViReal64 symbolRate,
                                                   ViInt32 mode,
                                                   ViString mappingName,
                                                   ViBoolean signalFiltering,
                                                   ViInt32 pointsSymbol);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFSKModulation (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViBoolean deviationCompensation,
                                                      ViReal64 FSKDeviation);
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationFilters (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViString TXFilter,
                                                          ViString receiveFilter,
                                                          ViString measFilter);
ViStatus _VI_FUNC rsspecan_GetVSAModulationFilterCatalog (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32* numberOfFilters,
                                                          ViInt32 bufferSize,
                                                          ViChar _VI_FAR modulationFiltersList[]);
ViStatus _VI_FUNC rsspecan_ConfigureVSADemodulation (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 recordingLength,
                                                     ViReal64 recordLength,
                                                     ViBoolean multiburstSearch,
                                                     ViInt32 sideBand,
                                                     ViBoolean normalize,
                                                     ViInt32 pointsSymbol);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEvaluationLinesState
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean evaluationLinesState);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEvaluationLines (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 evaluationLine,
                                                        ViInt32 lineValue);
ViStatus _VI_FUNC rsspecan_ConfigureVSAForceWBPath (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitPower (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViInt32 unitPower);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayFitTrace (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 trace, ViInt32 fit,
                                                        ViInt32 symbolNumber,
                                                        ViInt32 patternPosition);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayTrace (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 trace,
                                                     ViInt32 symbol,
                                                     ViInt32 spacing);
ViStatus _VI_FUNC rsspecan_ConfigureVSATraceEvaluation (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 traceEvaluation);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMultipleEvaluation
             (ViSession instrumentHandle, ViInt32 window, ViBoolean captureAuto,
              ViBoolean multiburstState);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMultipleEvaluationZoom
             (ViSession instrumentHandle, ViInt32 window, ViInt32 zoomStart,
              ViInt32 zoomLength);
ViStatus _VI_FUNC rsspecan_VSASearchBurstInCaptureRam (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 action);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMeasurementResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 measurementSignal,
              ViInt32 resultDisplay, ViBoolean spectrum, ViBoolean statistics);
ViStatus _VI_FUNC rsspecan_ConfigureVSARawSignal (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViInt32 resultDisplay,
                                                  ViBoolean spectrum,
                                                  ViBoolean statistics);
ViStatus _VI_FUNC rsspecan_ConfigureVSAErrorResults (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 resultDisplay,
                                                     ViBoolean spectrum,
                                                     ViBoolean statistics);
ViStatus _VI_FUNC rsspecan_ConfigureVSASymbolTableResults
             (ViSession instrumentHandle, ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEqualizerResults (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 resultDisplay);
ViStatus _VI_FUNC rsspecan_GetVSAXMarkerPosition (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 marker,
                                                  ViReal64* position);
ViStatus _VI_FUNC rsspecan_ConfigureVSACompressionPointMeasurement
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state,
              ViReal64 value);
ViStatus _VI_FUNC rsspecan_GetVSACompressionPointResults (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 type,
                                                          ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_SetVSAMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SetVSAFactoryDefaults (ViSession instrumentHandle,
                                                  ViInt32 defaults);
ViStatus _VI_FUNC rsspecan_PresetVSAScreens (ViSession instrumentHandle,
                                             ViInt32 window);
ViStatus _VI_FUNC rsspecan_VSARefLevelAdjust (ViSession instrumentHandle,
                                              ViInt32 window, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_VSADigitalStandardLoad (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViString digitalStandard);
ViStatus _VI_FUNC rsspecan_VSADigitalStandardSave (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViString userDefinedStandard,
                                                   ViString group, ViString comment,
                                                   ViString prefix);
ViStatus _VI_FUNC rsspecan_VSADigitalStandardDelete (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViString digitalStandard);
ViStatus _VI_FUNC rsspecan_VSADigitalStandardDefaults (ViSession instrumentHandle,
                                                       ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationSettings
             (ViSession instrumentHandle, ViInt32 window, ViInt32 modulation,
              ViString mappingName, ViReal64 symbolRate);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFSKRefDeviation (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViReal64 referenceDeviation,
                                                        ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureVSATXFilter (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 type,
                                                 ViReal64 alphaBT);
ViStatus _VI_FUNC rsspecan_ConfigureVSAUserDefinedTXFilter
             (ViSession instrumentHandle, ViInt32 window, ViString filterFileName);
ViStatus _VI_FUNC rsspecan_GetVSAModulationMappingCatalog
             (ViSession instrumentHandle, ViInt32 window, ViInt32* numberOfMappings,
              ViInt32 bufferSize, ViChar _VI_FAR mappingsList[]);
ViStatus _VI_FUNC rsspecan_GetVSAModulationUQAMLevel (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32* specificLevel);
ViStatus _VI_FUNC rsspecan_VSALoadUserModulation (ViSession instrumentHandle,
                                                  ViString userModulation);
ViStatus _VI_FUNC rsspecan_ConfigureVSASignalType (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32 signalType);
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSignal (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 usefulLength,
                                                    ViInt32 maxLength,
                                                    ViInt32 runIn, ViInt32 runOut);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSignalState
             (ViSession instrumentHandle, ViInt32 window, ViBoolean patternSignal);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternOffset (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViBoolean patternOffsetState,
                                                      ViInt32 patternOffset);
ViStatus _VI_FUNC rsspecan_ConfigureVSADataCaptureSettings
             (ViSession instrumentHandle, ViInt32 window, ViInt32 mode,
              ViReal64 captureLength, ViInt32 captureOversampling,
              ViBoolean swapIQ);
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchAuto (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 burstSearch);
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearch (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViBoolean searchState,
                                                    ViInt32 burstSearchMode,
                                                    ViBoolean thresholdAuto,
                                                    ViInt32 thresholdMode,
                                                    ViReal64 thresholdLevel);
ViStatus _VI_FUNC rsspecan_ConfigureVSABurstSearchAutoConfiguration
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean autoConfiguration, ViInt32 tolerance, ViInt32 minGaplen);
ViStatus _VI_FUNC rsspecan_GetVSABurstFound (ViSession instrumentHandle,
                                             ViBoolean* found);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSearchAuto (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 patternSearch);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSearch (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViBoolean searchState,
                                                      ViInt32 patternSearchMode,
                                                      ViString pattern);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSearchAutoConfiguration
             (ViSession instrumentHandle, ViInt32 window,
              ViBoolean autoConfiguration, ViReal64 IQCorrelationThreshold);
ViStatus _VI_FUNC rsspecan_GetVSAPatternFound (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 bufferSize,
                                               ViChar _VI_FAR found[]);
ViStatus _VI_FUNC rsspecan_GetVSAPatternCatalog (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 selection,
                                                 ViInt32* numberOfPatterns,
                                                 ViInt32 bufferSize,
                                                 ViChar _VI_FAR patternsList[]);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternOperations (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 operation,
                                                          ViString patternName);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternSettings (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViString patternName,
                                                        ViString description,
                                                        ViString comment,
                                                        ViInt32 degreeOfModulation,
                                                        ViString patternData);
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultLength (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 resultLength);
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultRangeReference
             (ViSession instrumentHandle, ViInt32 window, ViInt32 trace,
              ViInt32 reference);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayAlignment (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 trace,
                                                         ViInt32 alignment,
                                                         ViReal64 alignValue,
                                                         ViInt32 alignOffset);
ViStatus _VI_FUNC rsspecan_ConfigureVSASymbolNoAtReference
             (ViSession instrumentHandle, ViInt32 window, ViInt32 trace,
              ViInt32 value);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEvaluationRange (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean entireResultRange,
                                                        ViInt32 start,
                                                        ViInt32 stop);
ViStatus _VI_FUNC rsspecan_ConfigureVSAStatisticCount (ViSession instrumentHandle,
                                                       ViInt32 statisticCount);
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultRangeNumber (ViSession instrumentHandle,
                                                          ViInt32 resultRangeNumber);
ViStatus _VI_FUNC rsspecan_GetVSACurrentStatisticsCounter
             (ViSession instrumentHandle, ViInt32 evaluationType,
              ViInt32* currentCounter);
ViStatus _VI_FUNC rsspecan_ConfigureVSACompensation (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 compensateFor,
                                                     ViBoolean compensationState);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEVMModulation (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 calculation);
ViStatus _VI_FUNC rsspecan_ConfigureVSAOptimization (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 optimization);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEVMOffsetState (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViBoolean EVMOffset);
ViStatus _VI_FUNC rsspecan_ConfigureVSAPatternForSync (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 patternForSync);
ViStatus _VI_FUNC rsspecan_ConfigureVSAKnownData (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViBoolean knownDataState,
                                                  ViString dataFileName);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFineSynchronization
             (ViSession instrumentHandle, ViInt32 window, ViBoolean autoSet,
              ViInt32 mode, ViReal64 SER);
ViStatus _VI_FUNC rsspecan_QueryVSAFineSynchronizationResult
             (ViSession instrumentHandle, ViInt32 window, ViInt32* result);
ViStatus _VI_FUNC rsspecan_ConfigureVSAEstimationPointsSymbol
             (ViSession instrumentHandle, ViInt32 window,
              ViInt32 estimationPointsSym);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMeasurementFilter (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViBoolean autoMeasurementFilter,
                                                          ViInt32 type,
                                                          ViReal64 alphaBT);
ViStatus _VI_FUNC rsspecan_ConfigureVSAUserDefinedMeasurementFilter
             (ViSession instrumentHandle, ViInt32 window, ViString filterFileName);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFilterCharacteristic
             (ViSession instrumentHandle, ViInt32 window, ViReal64 alphaBT);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMarkerState (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 markerNumber,
                                                    ViBoolean markerState);
ViStatus _VI_FUNC rsspecan_ConfigureVSAScreenCoupling (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 markerNumber,
                                                       ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMarkerSearch (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 search);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMarkerPeakSearch (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 markerNumber);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFullScaleLevelUnit
             (ViSession instrumentHandle, ViInt32 fullScaleLevelUnit);
ViStatus _VI_FUNC rsspecan_VSAAdjustReferenceLevelToFullScaleLevel
             (ViSession instrumentHandle, ViBoolean adjustReferenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayYAxis (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViReal64 scaling,
                                                     ViReal64 referenceValue);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayYAxisStatisticMeasurement
             (ViSession instrumentHandle, ViInt32 yUnit, ViReal64 yAxisMaxValue,
              ViReal64 yAxisMinValue);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayYAxisRefPosition
             (ViSession instrumentHandle, ViInt32 window,
              ViReal64 referencePosition);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayXAxis (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 trace,
                                                     ViInt32 quantize,
                                                     ViInt32 scaling);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayXAxisReference
             (ViSession instrumentHandle, ViInt32 window, ViInt32 trace,
              ViInt32 referenceValue, ViInt32 referencePosition);
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitTime (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitYAxis (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 trace,
                                                  ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureVSAUnitAngle (ViSession instrumentHandle,
                                                  ViInt32 window,
                                                  ViInt32 unitAngle);
ViStatus _VI_FUNC rsspecan_VSAImport (ViSession instrumentHandle, ViInt32 type,
                                      ViString name, ViString path);
ViStatus _VI_FUNC rsspecan_VSAExport (ViSession instrumentHandle, ViInt32 type,
                                      ViString name, ViString path);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFileImport (ViSession instrumentHandle,
                                                   ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureVSAFileExport (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 trace,
                                                   ViInt32 decimalSeparator,
                                                   ViBoolean header,
                                                   ViInt32 dataMode,
                                                   ViInt32 dataRaw,
                                                   ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsDefault (ViSession instrumentHandle,
                                                      ViInt32 window, ViInt32 type);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsEVM (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 limitType,
                                                  ViReal64 EVMRMS,
                                                  ViBoolean checkRMS,
                                                  ViReal64 EVMPeak,
                                                  ViBoolean checkPeak);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsPhaseError (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 limitType,
                                                         ViReal64 phaseErrorRMS,
                                                         ViBoolean checkRMS,
                                                         ViReal64 phaseErrorPeak,
                                                         ViBoolean checkPeak);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsMagnitudeError
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limitType,
              ViReal64 magnitudeErrorRMS, ViBoolean checkRMS,
              ViReal64 magnitudeErrorPeak, ViBoolean checkPeak);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsCarrierFrequencyError
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limitType,
              ViReal64 carrierFrequencyError, ViBoolean check);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsRHO (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 limitType,
                                                  ViReal64 RHO, ViBoolean check);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsIQOffset (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 limitType,
                                                       ViReal64 IQOffset,
                                                       ViBoolean check);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsFrequencyError
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limitType,
              ViReal64 frequencyErrorRMS, ViBoolean checkRMS,
              ViReal64 frequencyErrorPeak, ViBoolean checkPeak);
ViStatus _VI_FUNC rsspecan_ConfigureVSALimitsFrequencyDeviationError
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limitType,
              ViReal64 freqDevError, ViBoolean check);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayWindowState
             (ViSession instrumentHandle, ViInt32 window, ViBoolean windowState);
ViStatus _VI_FUNC rsspecan_ConfigureVSAHighlightSymbols (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 trace,
                                                         ViBoolean highlightSymbols);
ViStatus _VI_FUNC rsspecan_ConfigureVSACaptureBuffer (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureVSAMeasurementSignal (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureVSATraceEval (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 trace,
                                                  ViInt32 traceEvaluation);
ViStatus _VI_FUNC rsspecan_ConfigureVSASymbols (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureVSAErrorVector (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationErrors (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationAccuracy
             (ViSession instrumentHandle, ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureVSAModulationAccuracyAdditional
             (ViSession instrumentHandle, ViInt32 window, ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureVSAResultTypeTransformation
             (ViSession instrumentHandle, ViInt32 window, ViInt32 transformation);
ViStatus _VI_FUNC rsspecan_ConfigureVSACaptureOversampling
             (ViSession instrumentHandle, ViInt32 window, ViInt32 oversampling,
              ViInt32 statisticalMode);
ViStatus _VI_FUNC rsspecan_ConfigureVSADisplayPointPerSymbol
             (ViSession instrumentHandle, ViInt32 window, ViInt32 pointsPerSymbol,
              ViInt32 numberOfPointsPerSymbol);
ViStatus _VI_FUNC rsspecan_VSARefreshCapturedData (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_GetVSAXAxisFirstValue (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 trace,
                                                  ViReal64* result);
ViStatus _VI_FUNC rsspecan_GetVSAResult (ViSession instrumentHandle, ViInt32 window,
                                         ViInt32 type, ViInt32 modifier,
                                         ViReal64* result);
ViStatus _VI_FUNC rsspecan_GetVSAAllResults (ViSession instrumentHandle,
                                             ViInt32 window,
                                             ViReal64 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_ReadVSATraceData (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32 sourceTrace,
                                             ViInt32 arraySize,
                                             ViReal64 _VI_FAR traceData[],
                                             ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_QueryVSABERResults (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 resultType,
                                               ViReal64* bitErrorRate,
                                               ViInt32* numberOfErrors,
                                               ViInt32* numberOfBits);
ViStatus _VI_FUNC rsspecan_QueryVSAModulationAccuracyLimitsCheckResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 measurementType,
              ViInt32 limitType, ViInt32* checkResults);
ViStatus _VI_FUNC rsspecan_QueryVSAModulationAccuracyStatisticResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViInt32 measurementType, ViInt32 statistic, ViReal64* value);
ViStatus _VI_FUNC rsspecan_Set3GPPBSMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMode (ViSession instrumentHandle,
                                                ViInt32 baseStationMode);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMeasurement (ViSession instrumentHandle,
                                                       ViInt32 scramblingCode,
                                                       ViInt32 powerReference,
                                                       ViInt32 compareMode,
                                                       ViInt32 syncType,
                                                       ViInt32 sideBand,
                                                       ViBoolean normalize,
                                                       ViBoolean HSDPAUPA,
                                                       ViReal64 inactiveChannelTreshold,
                                                       ViBoolean invertQ,
                                                       ViInt32 antenna,
                                                       ViBoolean powerDifference,
                                                       ViInt32 constellationParameterB,
                                                       ViInt32 timingOffset);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMultiframe (ViSession instrumentHandle,
                                                      ViInt32 captureLength,
                                                      ViInt32 frameToAnalyze);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSMulticarrier (ViSession instrumentHandle,
                                                        ViBoolean multicarrierState,
                                                        ViBoolean autoscale);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSSingleAntennaMIMOMode
             (ViSession instrumentHandle, ViBoolean mode);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSResults (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViInt32 CPICHSlot,
                                                   ViInt32 spreadingFactor);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSResultType (ViSession instrumentHandle,
                                                      ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSResultSummaryView
             (ViSession instrumentHandle, ViInt32 view);
ViStatus _VI_FUNC rsspecan_Configure3GPPBSYScaleDivision (ViSession instrumentHandle,
                                                          ViReal64 yScaleDivision);
ViStatus _VI_FUNC rsspecan_Set3GPPBSASequence (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Search3GPPBSScramblingCode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Configure3GPPTAERBTSCarrier (ViSession instrumentHandle,
                                                        ViInt32 carrier,
                                                        ViInt32 antenna,
                                                        ViInt32 CPICHCodeNumber,
                                                        ViInt32 CPICHPattern,
                                                        ViInt32 frequencyOffset,
                                                        ViInt32 scramblingCode);
ViStatus _VI_FUNC rsspecan_Query3GPPTAERBTSNumber (ViSession instrumentHandle,
                                                   ViInt32* numberOfBTS);
ViStatus _VI_FUNC rsspecan_Set_3GPPTAERBTSOperation (ViSession instrumentHandle,
                                                     ViInt32 carrier,
                                                     ViInt32 operation);
ViStatus _VI_FUNC rsspecan_Query3GPPTAERTableCatalog (ViSession instrumentHandle,
                                                      ViInt32 bufferSize,
                                                      ViChar _VI_FAR catalogTablesList[]);
ViStatus _VI_FUNC rsspecan_Set_3GPPTAERTableOperation (ViSession instrumentHandle,
                                                       ViInt32 operation,
                                                       ViString tableName);
ViStatus _VI_FUNC rsspecan_Set_3GPPTAERTablePreset (ViSession instrumentHandle,
                                                    ViString tableName);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDBSChannelTableOperations
             (ViSession instrumentHandle, ViInt32 operation, ViString channelTable);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDBSChannelTableData
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR codeClass[], ViInt32 _VI_FAR codeNumber[],
              ViBoolean _VI_FAR TFCI[], ViInt32 _VI_FAR timingOffset[],
              ViInt32 _VI_FAR pilotLength[], ViInt32 _VI_FAR channelType[],
              ViBoolean _VI_FAR status[], ViReal64 _VI_FAR CDPRelative[]);
ViStatus _VI_FUNC rsspecan_Set3GPPBSMarkerToChannel (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViInt32 channel);
ViStatus _VI_FUNC rsspecan_Set3GPPBSDeltaMarkerToChannel (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 channel);
ViStatus _VI_FUNC rsspecan_Set3GPPBSMeasurement (ViSession instrumentHandle,
                                                 ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDBSChannelTableComment
             (ViSession instrumentHandle, ViString channelTableComment);
ViStatus _VI_FUNC rsspecan_Query3GPPFDDBSChannelTableCatalog
             (ViSession instrumentHandle, ViInt32* numberOfChannelTables,
              ViInt32 bufferSize, ViChar _VI_FAR channelTablesList[]);
ViStatus _VI_FUNC rsspecan_Get3GPPBSTrace (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32 trace,
                                           ViInt32 arrayLength,
                                           ViInt32* actualPoints,
                                           ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_Get3GPPBSResult (ViSession instrumentHandle,
                                            ViInt32 type, ViReal64* result);
ViStatus _VI_FUNC rsspecan_Get3GPPBSCDPResult (ViSession instrumentHandle,
                                               ViInt32 type, ViInt32 arraySize,
                                               ViReal64 _VI_FAR results[],
                                               ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_Get3GPPBSCDPScramblingCodeResult
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR scrambingCode[], ViReal64 _VI_FAR power[],
              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_Get3GPPBSCDPListEvaluationResult
             (ViSession instrumentHandle, ViInt32 noOfValues,
              ViReal64 _VI_FAR rangeNumber[], ViReal64 _VI_FAR startFrequency[],
              ViReal64 _VI_FAR stopFrequency[],
              ViReal64 _VI_FAR resolutionBandwidth[], ViReal64 _VI_FAR frequency[],
              ViReal64 _VI_FAR powerAbs[], ViReal64 _VI_FAR powerRel[],
              ViReal64 _VI_FAR delta[], ViInt32 _VI_FAR limitCheck[],
              ViReal64 _VI_FAR reserved1[], ViReal64 _VI_FAR reserved2[],
              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_Get3GPPBSTimeAlignmentErrorResult
             (ViSession instrumentHandle, ViInt32 type, ViReal64* result);
ViStatus _VI_FUNC rsspecan_Set3GPPUEMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Configure3GPPUEMeasurement (ViSession instrumentHandle,
                                                       ViInt32 scramblingCode,
                                                       ViInt32 scramblingType,
                                                       ViInt32 measurementBase,
                                                       ViInt32 slot,
                                                       ViInt32 sideBand,
                                                       ViBoolean normalize,
                                                       ViBoolean RRCFilter,
                                                       ViBoolean HSDPAUPA,
                                                       ViBoolean eliminateTailChips,
                                                       ViReal64 inactiveChannelTreshold,
                                                       ViBoolean invertQ,
                                                       ViInt32 EVMMeasInterval);
ViStatus _VI_FUNC rsspecan_Configure3GPPUEMultiframe (ViSession instrumentHandle,
                                                      ViInt32 captureLength,
                                                      ViInt32 frameToAnalyze);
ViStatus _VI_FUNC rsspecan_Configure3GPPUEResults (ViSession instrumentHandle,
                                                   ViInt32 selectIQ,
                                                   ViInt32 channel,
                                                   ViInt32 spreadingFactor);
ViStatus _VI_FUNC rsspecan_Configure3GPPUEResultType (ViSession instrumentHandle,
                                                      ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableOperations
             (ViSession instrumentHandle, ViInt32 operation, ViString channelTable);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableData
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR codeClass[], ViInt32 _VI_FAR codeNumber[],
              ViInt32 _VI_FAR channelMapping[], ViInt32 _VI_FAR pilotLength[],
              ViBoolean _VI_FAR status[], ViReal64 _VI_FAR CDPRelative[]);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableHighSpeedChannel
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_Set3GPPUEMeasurement (ViSession instrumentHandle,
                                                 ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_Configure3GPPFDDUEChannelTableComment
             (ViSession instrumentHandle, ViString channelTableComment);
ViStatus _VI_FUNC rsspecan_Query3GPPFDDUEChannelTableCatalog
             (ViSession instrumentHandle, ViInt32* numberOfChannelTables,
              ViInt32 bufferSize, ViChar _VI_FAR channelTablesList[]);
ViStatus _VI_FUNC rsspecan_Get3GPPUETrace (ViSession instrumentHandle,
                                           ViInt32 window, ViInt32 trace,
                                           ViInt32 arrayLength,
                                           ViInt32* actualPoints,
                                           ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_Get3GPPUEResult (ViSession instrumentHandle,
                                            ViInt32 type, ViReal64* result);
ViStatus _VI_FUNC rsspecan_SetTDSBSMeasurement (ViSession instrumentHandle,
                                                ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSCapture (ViSession instrumentHandle,
                                                  ViInt32 setCount,
                                                  ViInt32 setValue,
                                                  ViInt32 captureLength);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSCDACapture (ViSession instrumentHandle,
                                                     ViInt32 setCount,
                                                     ViInt32 setValue,
                                                     ViInt32 captureLength,
                                                     ViInt32 channel, ViInt32 slot);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSResults (ViSession instrumentHandle,
                                                  ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_TDSBSAdaptSignalAutoLevelTime (ViSession instrumentHandle,
                                                          ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_TDSBSCDPLevelAdjust (ViSession instrumentHandle,
                                                ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSMeasurement (ViSession instrumentHandle,
                                                      ViInt32 standard,
                                                      ViInt32 scramblingCode,
                                                      ViInt32 midambleShiftCells,
                                                      ViInt32 sideBand,
                                                      ViBoolean normalize,
                                                      ViReal64 inactiveChannelTreshold,
                                                      ViBoolean invertQ);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSSyncToSlot (ViSession instrumentHandle,
                                                     ViBoolean syncToSlot);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSSyncToSlotMode (ViSession instrumentHandle,
                                                         ViInt32 syncToSlotMode);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSSyncToSlotRotateCodeChannels
             (ViSession instrumentHandle, ViBoolean rotateCodeChannels);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSUCPICH (ViSession instrumentHandle,
                                                 ViBoolean userDefinedCPICH,
                                                 ViInt32 CPICHCode,
                                                 ViInt32 CPICHPattern);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSPVTMeasurement (ViSession instrumentHandle,
                                                         ViInt32 switchingPoint,
                                                         ViInt32 subframes,
                                                         ViBoolean highDynamicState);
ViStatus _VI_FUNC rsspecan_GetTDSBSReferenceLevelAdjustmentResult
             (ViSession instrumentHandle, ViUInt32 timeout, ViInt32* result);
ViStatus _VI_FUNC rsspecan_SetTDSUEMeasurement (ViSession instrumentHandle,
                                                ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEAdaptToSignal (ViSession instrumentHandle,
                                                        ViInt32 startSlot,
                                                        ViInt32 stopSlot);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUECapture (ViSession instrumentHandle,
                                                  ViInt32 setCount,
                                                  ViInt32 setValue,
                                                  ViInt32 captureLength,
                                                  ViInt32 channel, ViInt32 slot);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEResults (ViSession instrumentHandle,
                                                  ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_TDSUEAdaptSignalAutoLevelTime
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_TDSUECDPLevelAdjust (ViSession instrumentHandle,
                                                ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUESEMLimits (ViSession instrumentHandle,
                                                    ViInt32 release);
ViStatus _VI_FUNC rsspecan_TDSUEChannelTableOperations (ViSession instrumentHandle,
                                                        ViInt32 operation,
                                                        ViString channelTable);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableComment
             (ViSession instrumentHandle, ViString channelTableComment);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableData
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR channelType[], ViInt32 _VI_FAR codeClass[],
              ViInt32 _VI_FAR codeNumber[], ViInt32 _VI_FAR modulationType[],
              ViInt32 _VI_FAR midambleShift[], ViBoolean _VI_FAR status[],
              ViInt32 _VI_FAR reserved1[], ViInt32 _VI_FAR reserved2[]);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableOrder
             (ViSession instrumentHandle, ViInt32 order);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableMidambleShift
             (ViSession instrumentHandle, ViInt32 maximumOfMidambleShifts);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEChannelTableMaxModulation
             (ViSession instrumentHandle, ViInt32 highestModulation);
ViStatus _VI_FUNC rsspecan_GetTDSUEChannelTableCatalog (ViSession instrumentHandle,
                                                        ViInt32* numberOfChannelTables,
                                                        ViInt32 bufferSize,
                                                        ViChar _VI_FAR channelTablesList[]);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEMeasurement (ViSession instrumentHandle,
                                                      ViInt32 scramblingCode,
                                                      ViInt32 midambleShiftCells,
                                                      ViInt32 sideBand,
                                                      ViBoolean normalize,
                                                      ViReal64 inactiveChannelTreshold,
                                                      ViBoolean invertQ);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUESyncToSlot (ViSession instrumentHandle,
                                                     ViBoolean syncToSlot);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEUCPICH (ViSession instrumentHandle,
                                                 ViBoolean userDefinedCPICH,
                                                 ViInt32 CPICHCode,
                                                 ViInt32 CPICHPattern);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUEPVTMeasurement (ViSession instrumentHandle,
                                                         ViInt32 subframes,
                                                         ViBoolean highDynamicState);
ViStatus _VI_FUNC rsspecan_GetTDSUEReferenceLevelAdjustmentResult
             (ViSession instrumentHandle, ViUInt32 timeout, ViInt32* result);
ViStatus _VI_FUNC rsspecan_SetTDSBSMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SetTDSUEMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_SelectTDSMeasurement (ViSession instrumentHandle,
                                                 ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureTDSSignalCapture (ViSession instrumentHandle,
                                                      ViBoolean invertQ,
                                                      ViInt32 setCount,
                                                      ViInt32 setValue,
                                                      ViInt32 captureLength);
ViStatus _VI_FUNC rsspecan_ConfigureTDSEvaluationRange (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViInt32 slot);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSAdaptToSignal (ViSession instrumentHandle,
                                                        ViInt32 startSlot,
                                                        ViInt32 stopSlot);
ViStatus _VI_FUNC rsspecan_TDSAdaptSignalAutoLevelTime (ViSession instrumentHandle,
                                                        ViInt32 timeout);
ViStatus _VI_FUNC rsspecan_ConfigureTDSSynchronization (ViSession instrumentHandle,
                                                        ViInt32 scramblingCode,
                                                        ViInt32 midambleShiftCells,
                                                        ViInt32 syncTo,
                                                        ViBoolean rotateCodeChannels);
ViStatus _VI_FUNC rsspecan_ConfigureTDSUESyncToSlotMode (ViSession instrumentHandle,
                                                         ViInt32 syncToSlotMode);
ViStatus _VI_FUNC rsspecan_ConfigureTDSInactiveChannelThreshold
             (ViSession instrumentHandle, ViReal64 inactiveChannelTreshold);
ViStatus _VI_FUNC rsspecan_ConfigureTDSCodeDomainSettings
             (ViSession instrumentHandle, ViBoolean normalize,
              ViInt32 codePowerDisplay);
ViStatus _VI_FUNC rsspecan_ConfigureTDSInput (ViSession instrumentHandle,
                                              ViInt32 inputCoupling,
                                              ViInt32 impedance,
                                              ViBoolean highpassFilter,
                                              ViBoolean YIGPreselector);
ViStatus _VI_FUNC rsspecan_ConfigureTDSReferenceLevel (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViReal64 referenceLevel,
                                                       ViReal64 referenceLevelOffset);
ViStatus _VI_FUNC rsspecan_ConfigureTDSMechanicalAttenuator
             (ViSession instrumentHandle, ViBoolean autoMode, ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureTDSElectronicAttenuator
             (ViSession instrumentHandle, ViBoolean state,
              ViBoolean electronicAttenuatorAuto, ViInt32 manualValuedB);
ViStatus _VI_FUNC rsspecan_ConfigureTDSPreamplifier (ViSession instrumentHandle,
                                                     ViBoolean state,
                                                     ViInt32 preamplifierLevel);
ViStatus _VI_FUNC rsspecan_ConfigureTDSYAxisScalingMaxMin
             (ViSession instrumentHandle, ViReal64 maximumValue,
              ViReal64 minimumValue);
ViStatus _VI_FUNC rsspecan_TDSBSChannelTableOperations (ViSession instrumentHandle,
                                                        ViInt32 operation,
                                                        ViString channelTable);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableComment
             (ViSession instrumentHandle, ViString channelTableComment);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableData
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR channelType[], ViInt32 _VI_FAR codeClass[],
              ViInt32 _VI_FAR codeNumber[], ViInt32 _VI_FAR modulationType[],
              ViInt32 _VI_FAR midambleShift[], ViBoolean _VI_FAR status[],
              ViInt32 _VI_FAR reserved1[], ViInt32 _VI_FAR reserved2[]);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableOrder
             (ViSession instrumentHandle, ViInt32 order);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableMidambleShift
             (ViSession instrumentHandle, ViInt32 maximumOfMidambleShifts);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSChannelTableMaxModulation
             (ViSession instrumentHandle, ViInt32 highestModulation);
ViStatus _VI_FUNC rsspecan_GetTDSBSChannelTableCatalog (ViSession instrumentHandle,
                                                        ViInt32* numberOfChannelTables,
                                                        ViInt32 bufferSize,
                                                        ViChar _VI_FAR channelTablesList[]);
ViStatus _VI_FUNC rsspecan_ConfigureTDSBSPowerVsTime (ViSession instrumentHandle,
                                                      ViInt32 switchingPoint,
                                                      ViInt32 subframes);
ViStatus _VI_FUNC rsspecan_GetTDSBSPowerVsTimeResult (ViSession instrumentHandle,
                                                      ViInt32 timeout,
                                                      ViReal64* results);
ViStatus _VI_FUNC rsspecan_GetTDSBSResult (ViSession instrumentHandle, ViInt32 type,
                                           ViInt32 timeout, ViReal64* result);
ViStatus _VI_FUNC rsspecan_GetTDSUEResult (ViSession instrumentHandle, ViInt32 type,
                                           ViUInt32 timeout, ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchTDSBSTrace (ViSession instrumentHandle,
                                            ViInt32 trace, ViInt32 arrayLength,
                                            ViInt32* actualPoints,
                                            ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_FetchTDSUETrace (ViSession instrumentHandle,
                                            ViInt32 trace, ViInt32 arrayLength,
                                            ViInt32* actualPoints,
                                            ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_AdjustC2KSettings (ViSession instrumentHandle,
                                              ViInt32 window,
                                              ViInt32 adjustSettings,
                                              ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_AdjustC2KReferenceLevel (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_RestoreC2KSEMStandardLines (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureC2KSignalStatisticMeasurement
             (ViSession instrumentHandle, ViInt32 measurement,
              ViInt32 numberOfSamples);
ViStatus _VI_FUNC rsspecan_ConfigureC2KSignalStatisticsPercentMarker
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViReal64 percentMarker);
ViStatus _VI_FUNC rsspecan_ConfigureC2KChannelTableOperations
             (ViSession instrumentHandle, ViInt32 operation, ViString channelTable);
ViStatus _VI_FUNC rsspecan_RestoreC2KChannelTable (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureC2KBasicSettings (ViSession instrumentHandle,
                                                      ViInt32 bandClass,
                                                      ViBoolean timePhase,
                                                      ViReal64 inactChannelThreshold,
                                                      ViBoolean invertQ,
                                                      ViInt32 sideBand,
                                                      ViBoolean normalize);
ViStatus _VI_FUNC rsspecan_ConfigureC2KPowerDisplay (ViSession instrumentHandle,
                                                     ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureC2KAmplitudeYScaleDivision
             (ViSession instrumentHandle, ViReal64 yScaleDiv);
ViStatus _VI_FUNC rsspecan_AdjustC2KAmplitudeReferenceLevel
             (ViSession instrumentHandle, ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_SelectC2KMeasurement (ViSession instrumentHandle,
                                                 ViInt32 measurementSelect);
ViStatus _VI_FUNC rsspecan_ConfigureC2KPCGSettings (ViSession instrumentHandle,
                                                    ViInt32 setCount,
                                                    ViInt32 setToAnalyze);
ViStatus _VI_FUNC rsspecan_ConfigureC2KChannelTableComment
             (ViSession instrumentHandle, ViString channelTableComment);
ViStatus _VI_FUNC rsspecan_QueryC2KChannelTableCatalog (ViSession instrumentHandle,
                                                        ViInt32* numberOfChannelTables,
                                                        ViInt32 bufferSize,
                                                        ViChar _VI_FAR channelTablesList[]);
ViStatus _VI_FUNC rsspecan_ReadC2KTraceData (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32 sourceTrace,
                                             ViInt32 arrayLength,
                                             ViInt32* actualPoints,
                                             ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_ReadC2KCDPTraceData (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 sourceTrace,
                                                ViInt32 arrayLength,
                                                ViInt32* actualPoints,
                                                ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_BC2KMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KChannelTableData (ViSession instrumentHandle,
                                                          ViInt32 _VI_FAR channelType[],
                                                          ViInt32 _VI_FAR codeClass[],
                                                          ViInt32 _VI_FAR codeNumber[],
                                                          ViInt32 _VI_FAR radioConfiguration[],
                                                          ViInt32 _VI_FAR reserved1[],
                                                          ViInt32 _VI_FAR reserved2[],
                                                          ViBoolean _VI_FAR status[],
                                                          ViReal64 _VI_FAR CDPRelative[],
                                                          ViInt32 numOfElements);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KAdvancedSettings (ViSession instrumentHandle,
                                                          ViInt32 PNOffset,
                                                          ViInt32 baseSpreadingFactor,
                                                          ViInt32 powerReference,
                                                          ViInt32 order);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KForce1xEVDV (ViSession instrumentHandle,
                                                     ViBoolean _1xEVDV);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KAntennaSettings (ViSession instrumentHandle,
                                                         ViBoolean antennaDiversity,
                                                         ViInt32 antennaNumber);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KMultiCarrier (ViSession instrumentHandle,
                                                      ViBoolean multiCarrier,
                                                      ViBoolean enhancedAlgorithm,
                                                      ViBoolean MCFilter,
                                                      ViInt32 filterType,
                                                      ViReal64 rollOff,
                                                      ViReal64 cutOffFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KPichMarkerSettings
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KTDPichMarkerSettings
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KCodeDomainAnalyzerResults
             (ViSession instrumentHandle, ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KCodeDomainAnalyzerSettings
             (ViSession instrumentHandle, ViInt32 captureLength,
              ViInt32 selectChannel, ViInt32 selectPCG);
ViStatus _VI_FUNC rsspecan_QueryBC2KCodeDomainAnalyzerResultSummary
             (ViSession instrumentHandle, ViInt32 window, ViInt32 resultType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureBC2KSEM (ViSession instrumentHandle,
                                             ViInt32 limitLineType,
                                             ViReal64 limitLineValue,
                                             ViReal64 _30kHz1MHzTransition);
ViStatus _VI_FUNC rsspecan_StoreBC2KSEMToFile (ViSession instrumentHandle,
                                               ViString fileName);
ViStatus _VI_FUNC rsspecan_ReadBC2KSEMPeakList (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 noOfValues,
                                                ViReal64 _VI_FAR frequency[],
                                                ViReal64 _VI_FAR level[],
                                                ViReal64 _VI_FAR deltaLevel[],
                                                ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_MC2KMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KChannelTableData (ViSession instrumentHandle,
                                                          ViInt32 _VI_FAR channelType[],
                                                          ViInt32 _VI_FAR codeClass[],
                                                          ViInt32 _VI_FAR codeNumber[],
                                                          ViInt32 _VI_FAR mapping[],
                                                          ViInt32 _VI_FAR reserved1[],
                                                          ViInt32 _VI_FAR reserved2[],
                                                          ViBoolean _VI_FAR status[],
                                                          ViReal64 _VI_FAR CDPRelative[],
                                                          ViInt32 numOfElements);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KAdvancedSettings (ViSession instrumentHandle,
                                                          ViInt32 baseSpreadingFactor,
                                                          ViInt32 powerReference,
                                                          ViInt32 order,
                                                          ViInt32 selectIQ,
                                                          ViInt32 longCodeMode,
                                                          ViString longCodeMask,
                                                          ViString longCodeOffset);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCDPOverview (ViSession instrumentHandle,
                                                     ViBoolean codeDomainOverview);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KPichMarkerSettings
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCodeDomainAnalyzerResults
             (ViSession instrumentHandle, ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KCodeDomainAnalyzerSettings
             (ViSession instrumentHandle, ViInt32 captureLength,
              ViInt32 selectChannel, ViInt32 selectPCG, ViInt32 selectMapping);
ViStatus _VI_FUNC rsspecan_QueryMC2KCodeDomainAnalyzerResultSummary
             (ViSession instrumentHandle, ViInt32 window, ViInt32 resultType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureMC2KSEM (ViSession instrumentHandle,
                                             ViInt32 limitLineType,
                                             ViReal64 _30kHz1MHzTransition);
ViStatus _VI_FUNC rsspecan_BDOMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureBDOPowerVsTime (ViSession instrumentHandle,
                                                    ViInt32 numberOfHalfslots,
                                                    ViInt32 referenceSlot,
                                                    ViBoolean burstFit,
                                                    ViInt32 referenceMeanPower,
                                                    ViReal64 referenceValue,
                                                    ViBoolean restartOnFail);
ViStatus _VI_FUNC rsspecan_ConfigureBDOPowerVsTimeListEvaluation
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_RestoreBDOPowerVsTimeStandardLines
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureBDOChannelTableData (ViSession instrumentHandle,
                                                         ViInt32 _VI_FAR channelType[],
                                                         ViInt32 _VI_FAR codeClass[],
                                                         ViInt32 _VI_FAR codeNumber[],
                                                         ViInt32 _VI_FAR modulation[],
                                                         ViInt32 _VI_FAR reserved1[],
                                                         ViInt32 _VI_FAR reserved2[],
                                                         ViBoolean _VI_FAR status[],
                                                         ViReal64 _VI_FAR CDPRelative[],
                                                         ViInt32 numOfElements);
ViStatus _VI_FUNC rsspecan_ConfigureBDOAdvancedSettings (ViSession instrumentHandle,
                                                         ViInt32 PNOffset,
                                                         ViBoolean CDPAverage,
                                                         ViInt32 revision);
ViStatus _VI_FUNC rsspecan_ConfigureBDOMultiCarrier (ViSession instrumentHandle,
                                                     ViBoolean multiCarrier,
                                                     ViBoolean enhancedAlgorithm,
                                                     ViBoolean MCFilter,
                                                     ViInt32 filterType,
                                                     ViReal64 rollOff,
                                                     ViReal64 cutOffFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureBDOCodeDomainAnalyzerResults
             (ViSession instrumentHandle, ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureBDOCodeDomainAnalyzerSettings
             (ViSession instrumentHandle, ViInt32 captureLength, ViInt32 selectCode,
              ViInt32 selectSlot);
ViStatus _VI_FUNC rsspecan_ConfigureBDOCDPChannelType (ViSession instrumentHandle,
                                                       ViInt32 channelType,
                                                       ViBoolean codeDomainOverview,
                                                       ViInt32 mapping,
                                                       ViInt32 select);
ViStatus _VI_FUNC rsspecan_ConfigureBDOChannelType (ViSession instrumentHandle,
                                                    ViInt32 channelType,
                                                    ViInt32 mapping,
                                                    ViInt32 select);
ViStatus _VI_FUNC rsspecan_ConfigureEVDOBSTPowerReference
             (ViSession instrumentHandle, ViInt32 powerReference);
ViStatus _VI_FUNC rsspecan_QueryBDOCodeDomainAnalyzerGeneralResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 resultType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryBDOCodeDomainAnalyzerChannelResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 resultType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureBDOSEM (ViSession instrumentHandle,
                                            ViInt32 limitLineType,
                                            ViReal64 limitLineValue,
                                            ViReal64 _30kHz1MHzTransition);
ViStatus _VI_FUNC rsspecan_StoreBDOSEMToFile (ViSession instrumentHandle,
                                              ViString fileName);
ViStatus _VI_FUNC rsspecan_ReadBDOSEMPeakList (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 noOfValues,
                                               ViReal64 _VI_FAR frequency[],
                                               ViReal64 _VI_FAR level[],
                                               ViReal64 _VI_FAR deltaLevel[],
                                               ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ReadBDOPowerVsTimeListEvaluation
             (ViSession instrumentHandle, ViInt32 noOfValues,
              ViReal64 _VI_FAR rangeNumber[], ViReal64 _VI_FAR startTime[],
              ViReal64 _VI_FAR stopTime[], ViReal64 _VI_FAR avgPowerAbs[],
              ViReal64 _VI_FAR avgPowerRel[], ViReal64 _VI_FAR maxPowerAbs[],
              ViReal64 _VI_FAR maxPowerRel[], ViReal64 _VI_FAR minPowerAbs[],
              ViReal64 _VI_FAR minPowerRel[], ViReal64 _VI_FAR limitCheck[],
              ViReal64 _VI_FAR reserved1[], ViReal64 _VI_FAR reserved2[],
              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_MDOMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureMDOChannelTableData (ViSession instrumentHandle,
                                                         ViInt32 _VI_FAR channelType[],
                                                         ViInt32 _VI_FAR codeClass[],
                                                         ViInt32 _VI_FAR codeNumber[],
                                                         ViInt32 _VI_FAR mapping[],
                                                         ViInt32 _VI_FAR activity[],
                                                         ViInt32 _VI_FAR reserved1[],
                                                         ViBoolean _VI_FAR status[],
                                                         ViInt32 _VI_FAR reserved2[],
                                                         ViInt32 numOfElements);
ViStatus _VI_FUNC rsspecan_ConfigureMDOAdvancedSettings (ViSession instrumentHandle,
                                                         ViInt32 powerReference,
                                                         ViInt32 order,
                                                         ViInt32 selectIQ,
                                                         ViBoolean CDPAverage,
                                                         ViInt32 operation,
                                                         ViString longCodeI,
                                                         ViString longCodeQ);
ViStatus _VI_FUNC rsspecan_ConfigureMDOCDPOverview (ViSession instrumentHandle,
                                                    ViBoolean codeDomainOverview);
ViStatus _VI_FUNC rsspecan_ConfigureMDOPichMarkerSettings
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureMDOCodeDomainAnalyzerResults
             (ViSession instrumentHandle, ViInt32 resultType);
ViStatus _VI_FUNC rsspecan_ConfigureMDOCodeDomainAnalyzerSettings
             (ViSession instrumentHandle, ViInt32 captureLength,
              ViInt32 selectChannel, ViInt32 selectHalfslot, ViInt32 select);
ViStatus _VI_FUNC rsspecan_QueryMDOCodeDomainAnalyzerResultSummary
             (ViSession instrumentHandle, ViInt32 window, ViInt32 resultType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_ConfigureMDOSEM (ViSession instrumentHandle,
                                            ViInt32 limitLineType,
                                            ViReal64 _30kHz1MHzTransition);
ViStatus _VI_FUNC rsspecan_ConfigureWlanStandard (ViSession instrumentHandle,
                                                  ViInt32 standard);
ViStatus _VI_FUNC rsspecan_ConfigureWLANYIGFilter (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViBoolean YIGFilter);
ViStatus _VI_FUNC rsspecan_ConfigureWlanTrigger (ViSession instrumentHandle,
                                                 ViInt32 mode, ViReal64 offset,
                                                 ViBoolean autoTriggerLevel,
                                                 ViReal64 powerTriggerLevel,
                                                 ViReal64 extTriggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureWlanGate (ViSession instrumentHandle,
                                              ViBoolean gating, ViInt32 mode,
                                              ViReal64 delay, ViReal64 length,
                                              ViBoolean linkGateAndMarker);
ViStatus _VI_FUNC rsspecan_ConfigureWlanACPMode (ViSession instrumentHandle,
                                                 ViInt32 ACPMode);
ViStatus _VI_FUNC rsspecan_ConfigureWLANSweep (ViSession instrumentHandle,
                                               ViBoolean sweepModeContinuous);
ViStatus _VI_FUNC rsspecan_ConfigureWLANPeakVectorError (ViSession instrumentHandle,
                                                         ViInt32 peakVectorError);
ViStatus _VI_FUNC rsspecan_ConfigureWlanMeasRange (ViSession instrumentHandle,
                                                   ViInt32 measRange);
ViStatus _VI_FUNC rsspecan_ConfigureWLANNumberOfAntennas (ViSession instrumentHandle,
                                                          ViInt32 numberOfAntennas);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMODUTConfiguration
             (ViSession instrumentHandle, ViInt32 DUTConfiguration);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMOAntennaSignalCapture
             (ViSession instrumentHandle, ViInt32 MIMOAntennaSignalCapture);
ViStatus _VI_FUNC rsspecan_ConfigureWLANSTCMIMO (ViSession instrumentHandle,
                                                 ViInt32 rxAnalyzer,
                                                 ViBoolean antennaState,
                                                 ViString analyzerIPAddress,
                                                 ViInt32 configuration);
ViStatus _VI_FUNC rsspecan_ConfigureWLANSimultaneousCaptureState
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMOSequentialCaptureSetup
             (ViSession instrumentHandle, ViString OSPIPAddress,
              ViInt32 OSPSwitchModule);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMIMOPayloadLengthSource
             (ViSession instrumentHandle, ViInt32 source);
ViStatus _VI_FUNC rsspecan_ConfigureWLANRBWFilter (ViSession instrumentHandle,
                                                   ViBoolean RBWFilter);
ViStatus _VI_FUNC rsspecan_ConfigureWlanAutoLevelOnce (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureWlanBasebandSignalLevel
             (ViSession instrumentHandle, ViBoolean autoLevel,
              ViReal64 autoLevelTime, ViReal64 signalLevel, ViReal64 extAtt);
ViStatus _VI_FUNC rsspecan_ConfigureWlanRFChannelFrequency
             (ViSession instrumentHandle, ViInt32 channelNo, ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ConfigureWlanRFSignalLevel (ViSession instrumentHandle,
                                                       ViBoolean autoLevel,
                                                       ViReal64 autoLevelTime,
                                                       ViReal64 signalLevel,
                                                       ViReal64 RFAtt,
                                                       ViReal64 extAtt);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMarkerEnabled (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 marker,
                                                       ViBoolean markerEnabled,
                                                       ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkPosition (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 marker,
                                                      ViInt32 position,
                                                      ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkerPositionConstellation
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViInt32 type, ViInt32 position);
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkerZoom (ViSession instrumentHandle,
                                                    ViInt32 window, ViInt32 marker,
                                                    ViReal64 zoomRatio);
ViStatus _VI_FUNC rsspecan_ConfigureWlanMarkerBurstSymbol
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViInt32 burstNumber, ViInt32 symbolNumber);
ViStatus _VI_FUNC rsspecan_SetWlanMarkerPeak (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 marker,
                                              ViInt32 position);
ViStatus _VI_FUNC rsspecan_WlanDisableAllMarkers (ViSession instrumentHandle,
                                                  ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureWlanDemodulation (ViSession instrumentHandle,
                                                      ViBoolean useSignalFieldContent,
                                                      ViInt32 burstType,
                                                      ViBoolean autoDemodulation,
                                                      ViInt32 demodulator,
                                                      ViBoolean channelEstimation,
                                                      ViBoolean equalBurstLength,
                                                      ViInt32 minNumberOfDataSymbols,
                                                      ViInt32 maxNumberOfDataSymbols);
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPDemodulation (ViSession instrumentHandle,
                                                          ViBoolean usePLCPHeaderContent,
                                                          ViInt32 burstType,
                                                          ViBoolean autoDemodulation,
                                                          ViInt32 demodulator,
                                                          ViInt32 lengthType,
                                                          ViBoolean equalBurstLength,
                                                          ViInt32 minPayloadLength,
                                                          ViInt32 maxPayloadLength);
ViStatus _VI_FUNC rsspecan_ConfigureWlanSelectFilter (ViSession instrumentHandle,
                                                      ViString TXFilterName,
                                                      ViString RXFilterName);
ViStatus _VI_FUNC rsspecan_ConfigureWLANFFTEqualizerFilterLength
             (ViSession instrumentHandle, ViInt32 filterLength);
ViStatus _VI_FUNC rsspecan_GetWlanFilterCatalog (ViSession instrumentHandle,
                                                 ViInt32 bufferSize,
                                                 ViChar _VI_FAR filterList[],
                                                 ViInt32* returnedLength);
ViStatus _VI_FUNC rsspecan_ConfigureWLANBurstsToAnalyzeSettings
             (ViSession instrumentHandle, ViInt32 burstTypeToMeasure,
              ViInt32 channelBandwidthToMeasure, ViInt32 guardIntervalLength,
              ViInt32 STBCField, ViInt32 extensionSpatialStreams,
              ViInt32 sourceOfPayloadLength);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMCS (ViSession instrumentHandle,
                                             ViInt32 MCSIndexToUse,
                                             ViInt32 MCSIndex);
ViStatus _VI_FUNC rsspecan_ConfigureWLANFFTStartOffset (ViSession instrumentHandle,
                                                        ViInt32 FFTStartOffset);
ViStatus _VI_FUNC rsspecan_ConfigureWLAN_MIMOSpatialMappingConfiguration
             (ViSession instrumentHandle, ViInt32 spatialMappingMode,
              ViBoolean powerNormalize);
ViStatus _VI_FUNC rsspecan_ConfigureWLAN_MIMO_UDFSpatialMappingAntenna
             (ViSession instrumentHandle, ViInt32 TX, ViReal64 _VI_FAR realValues[],
              ViReal64 _VI_FAR imaginaryValues[], ViReal64 timeShift);
ViStatus _VI_FUNC rsspecan_ConfigureWLAN_MIMO_UDFSpatialMappingStream
             (ViSession instrumentHandle, ViInt32 TX, ViInt32 stream,
              ViReal64 realValue, ViReal64 imaginaryValue, ViReal64 timeShift);
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPLimit (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 limit,
                                                   ViInt32 limitType, ViInt32 mode,
                                                   ViReal64 value);
ViStatus _VI_FUNC rsspecan_GetWlanACPChannelLimit (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 limit,
                                                   ViInt32 channelType,
                                                   ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetWlanSpectrumMaxOverstepping
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limit,
              ViInt32 channelType, ViReal64* value);
ViStatus _VI_FUNC rsspecan_ConfigureWlanMeas (ViSession instrumentHandle,
                                              ViBoolean overallBurstCount,
                                              ViInt32 noOfBurstToAnalyze,
                                              ViReal64 captureTime,
                                              ViBoolean swapIQ, ViInt32 sweepCount);
ViStatus _VI_FUNC rsspecan_ConfigureWlanPVTMeas (ViSession instrumentHandle,
                                                 ViInt32 risingFalling);
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPPVT (ViSession instrumentHandle,
                                                 ViInt32 referencePower,
                                                 ViInt32 ramp,
                                                 ViInt32 averageLength);
ViStatus _VI_FUNC rsspecan_ConfigureWlanEVMMeasurement (ViSession instrumentHandle,
                                                        ViInt32 mode,
                                                        ViBoolean autoScaling,
                                                        ViReal64 perDivision,
                                                        ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureWlanSpectrumMeasurement
             (ViSession instrumentHandle, ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureWLANChannelSelect (ViSession instrumentHandle,
                                                       ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureWlanConstellationMeasurement
             (ViSession instrumentHandle, ViInt32 constellationMode,
              ViInt32 carrierSelection, ViInt32 carrierNumber);
ViStatus _VI_FUNC rsspecan_ConfigureWlanPLCPConstellation
             (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureWlanStatisticsMeasurement
             (ViSession instrumentHandle, ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureWlanIQMeasurementParameters
             (ViSession instrumentHandle, ViReal64 sampleRate, ViInt32 filter);
ViStatus _VI_FUNC rsspecan_ConfigureWlanSEMMeasurement (ViSession instrumentHandle,
                                                        ViInt32 type,
                                                        ViString fileName,
                                                        ViInt32 powerClass);
ViStatus _VI_FUNC rsspecan_ConfigureWlanBurst (ViSession instrumentHandle,
                                               ViInt32 burst, ViBoolean state);
ViStatus _VI_FUNC rsspecan_GetWlanACPMeasurement (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 marker,
                                                  ViInt32 mode,
                                                  ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_LoadWlanIQData (ViSession instrumentHandle,
                                           ViString fileName);
ViStatus _VI_FUNC rsspecan_StoreWlanIQData (ViSession instrumentHandle,
                                            ViString fileName);
ViStatus _VI_FUNC rsspecan_ConfigureWlanIQCompensation (ViSession instrumentHandle,
                                                        ViBoolean IQCompensation);
ViStatus _VI_FUNC rsspecan_WlanBurstImm (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureWlanBurstUnit (ViSession instrumentHandle,
                                                   ViInt32 unit);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstTime (ViSession instrumentHandle,
                                               ViInt32 time, ViInt32 mode,
                                               ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstErrorRateForPilots
             (ViSession instrumentHandle, ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanCount (ViSession instrumentHandle, ViInt32 type,
                                           ViInt32* count);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPayload (ViSession instrumentHandle,
                                                  ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPeak (ViSession instrumentHandle,
                                               ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPreamble (ViSession instrumentHandle,
                                                   ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstStart (ViSession instrumentHandle,
                                                ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetWlanPLCPLimitCheckResult (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 limit,
                                                        ViInt32 limitType,
                                                        ViInt32 mode,
                                                        ViInt32* result);
ViStatus _VI_FUNC rsspecan_GetWlanACPChannLimitCheckResult
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limit,
              ViInt32 channelType, ViInt32* result);
ViStatus _VI_FUNC rsspecan_WlanSetWlanMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureWLANSignal (ViSession instrumentHandle,
                                                ViInt32 standard,
                                                ViReal64 centerFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureWLANInputSource (ViSession instrumentHandle,
                                                     ViInt32 inputCoupling,
                                                     ViInt32 impedance,
                                                     ViBoolean highpassFilter,
                                                     ViBoolean YIGPreselector);
ViStatus _VI_FUNC rsspecan_ConfigureWLANReferenceLevel (ViSession instrumentHandle,
                                                        ViBoolean autoLevel,
                                                        ViReal64 referenceLevel,
                                                        ViReal64 referenceLevelOffset);
ViStatus _VI_FUNC rsspecan_ConfigureWLANMechanicalAttenuation
             (ViSession instrumentHandle, ViBoolean autoMode, ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureWlanElectronicAtt (ViSession instrumentHandle,
                                                       ViBoolean state,
                                                       ViBoolean electronicAttenuatorAuto,
                                                       ViInt32 manualValuedB);
ViStatus _VI_FUNC rsspecan_ConfigureWLANPreamplifier (ViSession instrumentHandle,
                                                      ViBoolean state,
                                                      ViInt32 preamplifierLevel);
ViStatus _VI_FUNC rsspecan_ConfigureWLANSignalCapture (ViSession instrumentHandle,
                                                       ViReal64 sampleRate,
                                                       ViReal64 captureTime,
                                                       ViBoolean swapIQ,
                                                       ViBoolean filterOutAdjacentChannels);
ViStatus _VI_FUNC rsspecan_ConfigureWLANSynchronization (ViSession instrumentHandle,
                                                         ViBoolean powerIntervalSearch,
                                                         ViInt32 FFTStartOffset);
ViStatus _VI_FUNC rsspecan_ConfigureWLANChannelEstimation
             (ViSession instrumentHandle, ViBoolean channelEstimationRange);
ViStatus _VI_FUNC rsspecan_ConfigureWlanTracking (ViSession instrumentHandle,
                                                  ViBoolean phase, ViBoolean timing,
                                                  ViBoolean level);
ViStatus _VI_FUNC rsspecan_ConfigureWLANTrackingPilots (ViSession instrumentHandle,
                                                        ViInt32 pilots);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11a
             (ViSession instrumentHandle, ViBoolean PPDUAnalysisMode,
              ViInt32 PPDUFormatToMeasure, ViInt32 channelBandwidthToMeasure,
              ViInt32 PSDUModulationToUse, ViInt32 PSDUModulation);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11n
             (ViSession instrumentHandle, ViBoolean PPDUAnalysisMode,
              ViInt32 PPDUFormatToMeasure, ViInt32 channelBandwidthToMeasure);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nMCSIndex
             (ViSession instrumentHandle, ViInt32 MCSIndexMode, ViInt32 MCSIndex);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nSTBCField
             (ViSession instrumentHandle, ViInt32 STBCField);
ViStatus
    _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nGuardIntervalLength
        (ViSession instrumentHandle, ViInt32 guardIntervalLength);
ViStatus
    _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11nExtensionSpatialStreams
        (ViSession instrumentHandle, ViInt32 extensionSpatialStreams);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11b_g_DSSS
             (ViSession instrumentHandle, ViInt32 PPDUFormatPSDUModulation,
              ViInt32 PPDUFormat, ViInt32 PSDUModulation);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11ac
             (ViSession instrumentHandle, ViBoolean PPDUAnalysisMode,
              ViInt32 PPDUFormatToMeasure, ViInt32 channelBandwidthToMeasure);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acMCSIndex
             (ViSession instrumentHandle, ViInt32 MCSIndexMode, ViInt32 MCSIndex);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acSTBCField
             (ViSession instrumentHandle, ViInt32 STBCField);
ViStatus
    _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acGuardIntervalLength
        (ViSession instrumentHandle, ViInt32 guardIntervalLength);
ViStatus _VI_FUNC rsspecan_ConfigureWLANDemodulation802_11acNsts
             (ViSession instrumentHandle, ViInt32 nstsIndexMode, ViInt32 nstsIndex);
ViStatus _VI_FUNC rsspecan_ConfigureWLANEvaluationRangeStatistics
             (ViSession instrumentHandle, ViBoolean PPDUStatisticCount,
              ViInt32 noOfPPDUsToAnalyze);
ViStatus _VI_FUNC rsspecan_ConfigureWLANEvaluationRangeTimeDomain
             (ViSession instrumentHandle, ViInt32 sourceOfPayloadLength,
              ViBoolean equalPPDULength, ViInt32 minNumberOfDataSymbols,
              ViInt32 maxNumberOfDataSymbols);
ViStatus _VI_FUNC rsspecan_ConfigureWlanGainImbalanceUnit
             (ViSession instrumentHandle, ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureWlanEVMUnit (ViSession instrumentHandle,
                                                 ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureWlanDisplay (ViSession instrumentHandle,
                                                 ViBoolean displayTable);
ViStatus _VI_FUNC rsspecan_ConfigureWlanResultSummaryDisplay
             (ViSession instrumentHandle, ViInt32 window, ViInt32 item,
              ViBoolean displayItem);
ViStatus _VI_FUNC rsspecan_ConfigureWlanLimit (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 limit,
                                               ViInt32 limitType, ViInt32 mode,
                                               ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureWLANAllLimits (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 limit,
                                                   ViInt32 arraySize,
                                                   ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_GetWlanAllLimits (ViSession instrumentHandle,
                                             ViInt32 window, ViInt32 limit,
                                             ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_SelectWlanRFMeasurement (ViSession instrumentHandle,
                                                    ViInt32 RFMeasurement);
ViStatus _VI_FUNC rsspecan_ReadWlanTraceData (ViSession instrumentHandle,
                                              ViInt32 trace, ViInt32 arraySize,
                                              ViReal64 _VI_FAR traceData[],
                                              ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadWlanMemoryIQData (ViSession instrumentHandle,
                                                 ViInt32 offsetSamples,
                                                 ViInt32 noOfSamples,
                                                 ViInt32 bufferSize,
                                                 ViInt32* noOfPoints,
                                                 ViReal64 _VI_FAR realParts_I[],
                                                 ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_ReadWlanSEMResults (ViSession instrumentHandle,
                                               ViInt32 noOfValues,
                                               ViReal64 _VI_FAR index[],
                                               ViReal64 _VI_FAR startFrequencyBand[],
                                               ViReal64 _VI_FAR stopFrequencyBand[],
                                               ViReal64 _VI_FAR resolutionBandwidth[],
                                               ViReal64 _VI_FAR limitFailFrequency[],
                                               ViReal64 _VI_FAR powerAbs[],
                                               ViReal64 _VI_FAR powerRel[],
                                               ViReal64 _VI_FAR limitDistance[],
                                               ViReal64 _VI_FAR failureFlag[],
                                               ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstAll (ViSession instrumentHandle,
                                              ViReal64 _VI_FAR results[],
                                              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstPower (ViSession instrumentHandle,
                                                ViInt32 powerMeasurement,
                                                ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstError (ViSession instrumentHandle,
                                                ViInt32 errorType, ViInt32 mode,
                                                ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanIQImp (ViSession instrumentHandle,
                                           ViInt32 IQImpairment, ViInt32 mode,
                                           ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanEVM (ViSession instrumentHandle, ViInt32 EVM,
                                         ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstCount (ViSession instrumentHandle,
                                                ViInt32 type, ViInt32* count);
ViStatus _VI_FUNC rsspecan_FetchWlanSymbolCount (ViSession instrumentHandle,
                                                 ViInt32 arraySize,
                                                 ViInt32 _VI_FAR symbolCount[],
                                                 ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_FetchWlanBurstLength (ViSession instrumentHandle,
                                                 ViInt32* length);
ViStatus _VI_FUNC rsspecan_GetWlanPowerResults (ViSession instrumentHandle,
                                                ViInt32 window,
                                                ViInt32 powerMeasurement,
                                                ViInt32 arraySize,
                                                ViReal64 _VI_FAR results[],
                                                ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetWlanLimitCheckResult (ViSession instrumentHandle,
                                                    ViInt32 window, ViInt32 limit,
                                                    ViInt32 limitType, ViInt32 mode,
                                                    ViInt32* result);
ViStatus _VI_FUNC rsspecan_GetWlanACPChannLimitCheckResults
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limit,
              ViInt32 channelType, ViInt32 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_GetWlanAllLimitCheckResults (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 limit,
                                                        ViInt32 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_GetWlanLimitCheck (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 limit,
                                              ViInt32* result);
ViStatus _VI_FUNC rsspecan_SetWiMAXMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXStandard (ViSession instrumentHandle,
                                                   ViInt32 standard);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXElectronicAtt (ViSession instrumentHandle,
                                                        ViBoolean state,
                                                        ViBoolean electronicAttenuatorAuto,
                                                        ViInt32 manualValuedB);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXYIGFilter (ViSession instrumentHandle,
                                                    ViInt32 window,
                                                    ViBoolean YIGFilter);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXYIGFilterAuto (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean YIGFilterAuto);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTrigger (ViSession instrumentHandle,
                                                  ViInt32 mode, ViReal64 offset,
                                                  ViBoolean autoTriggerLevel,
                                                  ViReal64 powerTriggerLevel,
                                                  ViReal64 extTriggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSTCMIMO (ViSession instrumentHandle,
                                                  ViBoolean autoAssignment,
                                                  ViInt32 rxAnalyzer,
                                                  ViBoolean antennaState,
                                                  ViString analyzerIPAddress,
                                                  ViInt32 configuration);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXGate (ViSession instrumentHandle,
                                               ViBoolean gating, ViInt32 mode,
                                               ViReal64 delay, ViReal64 length,
                                               ViBoolean linkGateAndMarker);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXACP (ViSession instrumentHandle,
                                              ViInt32 ACPMode,
                                              ViInt32 numberOfChannels,
                                              ViBoolean noiseCorrection);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXACPChannelBandwidth
             (ViSession instrumentHandle, ViInt32 channel, ViInt32 channelNumber,
              ViReal64 bandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXACPChannelSpacing
             (ViSession instrumentHandle, ViInt32 channel, ViInt32 channelNumber,
              ViReal64 spacing);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXLevel (ViSession instrumentHandle,
                                                ViInt32 measurement,
                                                ViBoolean autoLevel,
                                                ViReal64 referenceLevel,
                                                ViReal64 externalAttenuation);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSEMMode (ViSession instrumentHandle,
                                                  ViInt32 linkMode);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFSBW (ViSession instrumentHandle,
                                               ViBoolean FSBWAuto,
                                               ViReal64 samplingRateFs,
                                               ViReal64 channelBandwidthBW);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXNumberOfAntennas
             (ViSession instrumentHandle, ViInt32 numberOfAntennas);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXAutoLevelOnce (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXBasebandSignalLevel
             (ViSession instrumentHandle, ViBoolean autoLevel,
              ViReal64 autoLevelTime, ViReal64 signalLevel, ViReal64 extAtt);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXRFChannelFrequency
             (ViSession instrumentHandle, ViInt32 channelNo, ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXRFSignalLevel (ViSession instrumentHandle,
                                                        ViBoolean autoLevel,
                                                        ViReal64 autoLevelTime,
                                                        ViReal64 signalLevel,
                                                        ViReal64 RFAtt,
                                                        ViReal64 extAtt);
ViStatus _VI_FUNC rsspecan_GetWiMAXReferenceLevel (ViSession instrumentHandle,
                                                   ViReal64* referenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkerEnabled (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViInt32 marker,
                                                        ViBoolean markerEnabled,
                                                        ViInt32 trace);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkPosition (ViSession instrumentHandle,
                                                       ViInt32 window,
                                                       ViInt32 marker,
                                                       ViInt32 position,
                                                       ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkerPositionConstellation
             (ViSession instrumentHandle, ViInt32 window, ViInt32 marker,
              ViInt32 type, ViInt32 position);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMarkerZoom (ViSession instrumentHandle,
                                                     ViInt32 window,
                                                     ViReal64 zoomRatio);
ViStatus _VI_FUNC rsspecan_SetWiMAXMarkerPeak (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 marker,
                                               ViInt32 position);
ViStatus _VI_FUNC rsspecan_WiMAXDisableAllMarkers (ViSession instrumentHandle,
                                                   ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDemodulation (ViSession instrumentHandle,
                                                       ViBoolean useSignalFieldContent,
                                                       ViInt32 modulationDetectionMode,
                                                       ViInt32 demodulator,
                                                       ViInt32 linkMode,
                                                       ViBoolean channelEstimation,
                                                       ViBoolean equalBurstLength,
                                                       ViInt32 minNumberOfDataSymbols,
                                                       ViInt32 maxNumberOfDataSymbols);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXAutoDemodulation
             (ViSession instrumentHandle, ViInt32 autoDemodulationZone);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDemodulationOFDMA
             (ViSession instrumentHandle, ViInt32 channelEstimation,
              ViReal64 FFTOffset);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSubchannelization
             (ViSession instrumentHandle, ViBoolean state, ViInt32 subchannel,
              ViInt32 upLinkPhysicalModifier);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTracking (ViSession instrumentHandle,
                                                   ViInt32 basedOn, ViBoolean phase,
                                                   ViBoolean timing,
                                                   ViBoolean level);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXOFDMATracking (ViSession instrumentHandle,
                                                        ViInt32 basedOn,
                                                        ViBoolean phase,
                                                        ViBoolean timing,
                                                        ViInt32 trackingPilot);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSTCMIMODemodulation
             (ViSession instrumentHandle, ViBoolean compansateCrosstalk);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXBurstBoosting (ViSession instrumentHandle,
                                                        ViInt32 burstBoosting);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSelectFilter (ViSession instrumentHandle,
                                                       ViString TXFilterName,
                                                       ViString RXFilterName);
ViStatus _VI_FUNC rsspecan_GetWiMAXFilterCatalog (ViSession instrumentHandle,
                                                  ViInt32 bufferSize,
                                                  ViChar _VI_FAR filterList[],
                                                  ViInt32* returnedLength);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDisplay (ViSession instrumentHandle,
                                                  ViBoolean displayTable);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDisplayResultTable
             (ViSession instrumentHandle, ViInt32 page);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXLimit (ViSession instrumentHandle,
                                                ViInt32 window, ViInt32 limit,
                                                ViInt32 limitType, ViInt32 mode,
                                                ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXAllLimits (ViSession instrumentHandle,
                                                    ViInt32 window, ViInt32 limit,
                                                    ViInt32 arraySize,
                                                    ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_GetWiMAXSpectrumMaxOverstepping
             (ViSession instrumentHandle, ViInt32 window, ViInt32 limit,
              ViInt32 channelType, ViReal64* value);
ViStatus _VI_FUNC rsspecan_GetWiMAXAllLimits (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 limit,
                                              ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrame (ViSession instrumentHandle,
                                                ViInt32 IDCell,
                                                ViInt32 ULSubframeFrameNumber,
                                                ViReal64 TDDFrame,
                                                ViInt32 ULControlRegionLength);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameIDCellAuto (ViSession instrumentHandle,
                                                          ViBoolean IDCellAuto);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFramePredefinedMap
             (ViSession instrumentHandle, ViInt32 predefinedMap,
              ViBoolean predefinedMapState);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameOFDMAMAP (ViSession instrumentHandle,
                                                        ViInt32 useForAnalysis);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXDLFramePreambleIndex
             (ViSession instrumentHandle, ViInt32 mode, ViInt32 preamblePattern);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameDLUsedSubchannel
             (ViSession instrumentHandle, ViInt32 segment, ViInt32 subchannelUsage);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXFrameULAllocatedSubchannel
             (ViSession instrumentHandle, ViInt32 segment, ViInt32 bitmap,
              ViInt32 logicalSubchannel);
ViStatus _VI_FUNC rsspecan_LoadWiMAXFrameData (ViSession instrumentHandle,
                                               ViString fileName);
ViStatus _VI_FUNC rsspecan_StoreWiMAXFrameData (ViSession instrumentHandle,
                                                ViString fileName);
ViStatus _VI_FUNC rsspecan_CreateWiMAXNewZone (ViSession instrumentHandle,
                                               ViInt32 zone, ViBoolean analyze,
                                               ViInt32 type, ViInt32 segment,
                                               ViInt32 length, ViInt32 offset,
                                               ViInt32 permbase, ViInt32 PRBS_ID);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZone (ViSession instrumentHandle,
                                               ViInt32 zone, ViBoolean analyze,
                                               ViInt32 type, ViInt32 segment,
                                               ViInt32 length, ViInt32 offset,
                                               ViInt32 permbase, ViInt32 PRBS_ID);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneSTCMode (ViSession instrumentHandle,
                                                      ViInt32 zone,
                                                      ViInt32 STCMode);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneSpaceTimeCoding
             (ViSession instrumentHandle, ViInt32 zone, ViInt32 spaceTimeCoding);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneMatrix (ViSession instrumentHandle,
                                                     ViInt32 zone,
                                                     ViInt32 matrixType);
ViStatus _VI_FUNC rsspecan_CreateWiMAXNewZoneBurst (ViSession instrumentHandle,
                                                    ViInt32 zone, ViInt32 burst,
                                                    ViInt32 modulation,
                                                    ViInt32 subchannels,
                                                    ViInt32 symbols,
                                                    ViInt32 slotDuration,
                                                    ViInt32 subchannelOffset,
                                                    ViInt32 symbolOffset,
                                                    ViReal64 burstPower,
                                                    ViInt32 burstType);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurst (ViSession instrumentHandle,
                                                    ViInt32 zone, ViInt32 burst,
                                                    ViInt32 modulation,
                                                    ViInt32 subchannels,
                                                    ViInt32 symbols,
                                                    ViInt32 slotDuration,
                                                    ViReal64 burstPower,
                                                    ViInt32 burstType,
                                                    ViBoolean offsetAuto,
                                                    ViInt32 subchannelOffset,
                                                    ViInt32 symbolOffset);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurstMatrix (ViSession instrumentHandle,
                                                          ViInt32 zone,
                                                          ViInt32 burst,
                                                          ViInt32 matrixType);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXZoneBurstSTCMode
             (ViSession instrumentHandle, ViInt32 zone, ViInt32 burst,
              ViInt32 STCMode);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXExtFrameZoneGeneratorAddress
             (ViSession instrumentHandle, ViString TCPIPAddress);
ViStatus _VI_FUNC rsspecan_GetWiMAXZoneCount (ViSession instrumentHandle,
                                              ViInt32* count);
ViStatus _VI_FUNC rsspecan_GetWiMAXZoneBurstCount (ViSession instrumentHandle,
                                                   ViInt32 zone, ViInt32* count);
ViStatus _VI_FUNC rsspecan_WiMAXZoneDelete (ViSession instrumentHandle,
                                            ViInt32 zone);
ViStatus _VI_FUNC rsspecan_WiMAXZoneBurstDelete (ViSession instrumentHandle,
                                                 ViInt32 zone, ViInt32 burst);
ViStatus _VI_FUNC rsspecan_WiMAXZoneReset (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_WiMAXZoneBurstReset (ViSession instrumentHandle,
                                                ViInt32 zone);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMeas (ViSession instrumentHandle,
                                               ViBoolean overallBurstCount,
                                               ViInt32 noOfBurstToAnalyze,
                                               ViReal64 captureTime,
                                               ViBoolean swapIQ, ViInt32 sweepCount,
                                               ViBoolean sweepTimeAuto_MaskACP,
                                               ViReal64 sweepTime_MaskACP,
                                               ViInt32 frequencyBand,
                                               ViReal64 channelBandwidth,
                                               ViInt32 guardRatio);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMeasOFDMA (ViSession instrumentHandle,
                                                    ViReal64 captureTime,
                                                    ViBoolean swapIQ,
                                                    ViInt32 sweepCount,
                                                    ViBoolean sweepTimeAuto_MaskACP,
                                                    ViReal64 sweepTime_MaskACP,
                                                    ViInt32 frequencyBand,
                                                    ViReal64 channelBandwidth,
                                                    ViInt32 guardRatio,
                                                    ViInt32 FFTSize,
                                                    ViBoolean captureCountState,
                                                    ViInt32 numberOfSubframesToAnalyze);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXMeasWiBro (ViSession instrumentHandle,
                                                    ViReal64 captureTime,
                                                    ViBoolean swapIQ,
                                                    ViInt32 sweepCount,
                                                    ViBoolean sweepTimeAuto_MaskACP,
                                                    ViReal64 sweepTime_MaskACP,
                                                    ViReal64 channelBandwidth,
                                                    ViBoolean captureCountState,
                                                    ViInt32 numberOfSubframesToAnalyze);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXPVTMeas (ViSession instrumentHandle,
                                                  ViInt32 risingFalling,
                                                  ViInt32 burstSelection);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXPVTMeasOFDMA (ViSession instrumentHandle,
                                                       ViInt32 risingFalling);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXEVMMeasurement (ViSession instrumentHandle,
                                                         ViInt32 mode,
                                                         ViBoolean autoScaling,
                                                         ViReal64 perDivision,
                                                         ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSpectrumMeasurement
             (ViSession instrumentHandle, ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXConstellationMeasurement
             (ViSession instrumentHandle, ViInt32 constellationMode,
              ViInt32 carrierSelection, ViInt32 carrierNumber);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXConstellationSelection
             (ViSession instrumentHandle, ViInt32 modulation,
              ViInt32 burstSelection, ViInt32 burstNumber, ViInt32 carrierSelection,
              ViInt32 carrierNumber, ViInt32 symbolSelection, ViInt32 symbolNumber);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXConstellationDiagram
             (ViSession instrumentHandle, ViBoolean normalization);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXBitstreamSelection
             (ViSession instrumentHandle, ViInt32 modulation,
              ViInt32 burstSelection, ViInt32 burstNumber, ViInt32 symbolSelection,
              ViInt32 symbolNumber);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXStatisticsMeasurement
             (ViSession instrumentHandle, ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXIQMeasurementParameters
             (ViSession instrumentHandle, ViReal64 sampleRate, ViInt32 filter);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXSEMMeasurement (ViSession instrumentHandle,
                                                         ViInt32 type,
                                                         ViString fileName,
                                                         ViInt32 powerClass);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTTCFrame (ViSession instrumentHandle,
                                                   ViInt32 frame);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTTCSubFrameLengthMeasure
             (ViSession instrumentHandle, ViInt32 referencePowerType,
              ViReal64 userRefPower, ViInt32 referencePowerOffset,
              ViReal64 userRefPowerOffset, ViInt32 averaging);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXTTCStart (ViSession instrumentHandle,
                                                   ViInt32 start);
ViStatus _VI_FUNC rsspecan_GetWiMAXTTCSubFrameLength (ViSession instrumentHandle,
                                                      ViInt32* subframeLength);
ViStatus _VI_FUNC rsspecan_GetWiMAXACPMeasurement (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 marker,
                                                   ViInt32 mode,
                                                   ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_LoadWiMAXIQData (ViSession instrumentHandle,
                                            ViString fileName);
ViStatus _VI_FUNC rsspecan_StoreWiMAXIQData (ViSession instrumentHandle,
                                             ViString fileName);
ViStatus _VI_FUNC rsspecan_WiMAXBurstImm (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_WiMAXRefreshIQMeasurement (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_WiMAXGetMarkerTTC (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 markerNumber,
                                              ViReal64* time);
ViStatus _VI_FUNC rsspecan_ReadWiMAXTraceData (ViSession instrumentHandle,
                                               ViInt32 trace, ViInt32 arraySize,
                                               ViReal64 _VI_FAR traceData[],
                                               ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadWiMAXMemoryIQData (ViSession instrumentHandle,
                                                  ViInt32 offsetSamples,
                                                  ViInt32 noOfSamples,
                                                  ViInt32 bufferSize,
                                                  ViInt32* noOfPoints,
                                                  ViReal64 _VI_FAR realParts_I[],
                                                  ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_ReadWiMAXSEMResults (ViSession instrumentHandle,
                                                ViInt32 noOfValues,
                                                ViReal64 _VI_FAR index[],
                                                ViReal64 _VI_FAR startFrequencyBand[],
                                                ViReal64 _VI_FAR stopFrequencyBand[],
                                                ViReal64 _VI_FAR resolutionBandwidth[],
                                                ViReal64 _VI_FAR limitFailFrequency[],
                                                ViReal64 _VI_FAR powerAbs[],
                                                ViReal64 _VI_FAR powerRel[],
                                                ViReal64 _VI_FAR limitDistance[],
                                                ViReal64 _VI_FAR failureFlag[],
                                                ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchWiMAXBurstAll (ViSession instrumentHandle,
                                               ViReal64 _VI_FAR results[],
                                               ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchWiMAXBurstPower (ViSession instrumentHandle,
                                                 ViInt32 powerMeasurement,
                                                 ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWiMAXBurstError (ViSession instrumentHandle,
                                                 ViInt32 errorType, ViInt32 mode,
                                                 ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWiMAXIQImp (ViSession instrumentHandle,
                                            ViInt32 IQImpairment, ViInt32 mode,
                                            ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWiMAXEVM (ViSession instrumentHandle, ViInt32 EVM,
                                          ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWiMAXSubscriberStationTime
             (ViSession instrumentHandle, ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_FetchWiMAXCINRValues (ViSession instrumentHandle,
                                                 ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_FetchWiMAXRSSIValues (ViSession instrumentHandle,
                                                 ViReal64 _VI_FAR values[]);
ViStatus _VI_FUNC rsspecan_FetchWiMAXCount (ViSession instrumentHandle,
                                            ViInt32 type, ViInt32* count);
ViStatus _VI_FUNC rsspecan_FetchWiMAXTimeDomainPreamble (ViSession instrumentHandle,
                                                         ViInt32 result,
                                                         ViReal64* minimum,
                                                         ViReal64* average,
                                                         ViReal64* maximum);
ViStatus _VI_FUNC rsspecan_FetchWiMAXUnmodulatedSubcarrierError
             (ViSession instrumentHandle, ViInt32 mode, ViReal64* value);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXListResultUnit (ViSession instrumentHandle,
                                                         ViInt32 unit);
ViStatus _VI_FUNC rsspecan_ConfigureWiMAXRSSICINRAvg (ViSession instrumentHandle,
                                                      ViReal64 averagingParameter);
ViStatus _VI_FUNC rsspecan_GetWiMAXLimitCheckResult (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 limit,
                                                     ViInt32 limitType,
                                                     ViInt32 mode, ViInt32* result);
ViStatus _VI_FUNC rsspecan_GetWiMAXAllLimitCheckResults (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32 limit,
                                                         ViInt32 _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_GetWiMAXLimitCheck (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 limit,
                                               ViInt32* result);
ViStatus _VI_FUNC rsspecan_SetLTEDownlinkMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSignalCharacteristics
             (ViSession instrumentHandle, ViInt32 duplexing, ViReal64 frequency,
              ViInt32 bandwidthSettingType, ViInt32 channelBandwidth,
              ViInt32 numberOfResourceBlocks, ViInt32 cyclicPrefix);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkLevel (ViSession instrumentHandle,
                                                      ViReal64 referenceLevel,
                                                      ViBoolean autoReferenceLevel,
                                                      ViReal64 externalAttenuation);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkLevelInBBMode
             (ViSession instrumentHandle, ViReal64 referenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkMIMO (ViSession instrumentHandle,
                                                     ViInt32 configuration,
                                                     ViInt32 antennaSelection);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkMeasurement
             (ViSession instrumentHandle, ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkIQ (ViSession instrumentHandle,
                                                   ViBoolean swapIQ);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkInput (ViSession instrumentHandle,
                                                      ViInt32 source);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkCSIRSState
             (ViSession instrumentHandle, ViBoolean state);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkCSIRS (ViSession instrumentHandle,
                                                      ViInt32 antenaPorts,
                                                      ViInt32 configurationIndex,
                                                      ViBoolean overwritePDSCH,
                                                      ViReal64 relativePower,
                                                      ViInt32 subframeConfiguration,
                                                      ViInt32 frameNumberOffset);
ViStatus _VI_FUNC rsspecan_LoadLTEDownlinkDemodulationSettings
             (ViSession instrumentHandle, ViString filePath);
ViStatus _VI_FUNC rsspecan_StoreLTEDownlinkDemodulationSettings
             (ViSession instrumentHandle, ViString filePath);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkDataAnalysis
             (ViSession instrumentHandle, ViInt32 channelEstimation,
              ViInt32 EVMCalculationMethod, ViBoolean scramblingOfCodedBits,
              ViBoolean autoDemodulation);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkDataAnalysisSettings
             (ViSession instrumentHandle, ViInt32 PDSCHSubframeDetect,
              ViBoolean boostingEstimation, ViInt32 PDSCHReferenceData,
              ViBoolean multicarrierFilter);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkTracking (ViSession instrumentHandle,
                                                         ViInt32 phaseTracking,
                                                         ViBoolean timing);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkTDDAllocation
             (ViSession instrumentHandle, ViInt32 TDDULDLAllocations);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkTDDSpecialSubframe
             (ViSession instrumentHandle, ViInt32 specialSubframe);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPhysicalLayerCellIdentity
             (ViSession instrumentHandle, ViBoolean autoCellIdentityGroup,
              ViInt32 cellIdentityGroup, ViInt32 cellIdentity,
              ViBoolean autoCellIdentity);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHConfigurableSubframes
             (ViSession instrumentHandle, ViInt32 configurableSubframes);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHUsedAllocations
             (ViSession instrumentHandle, ViInt32 subframe,
              ViInt32 usedAllocations);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHUsedAllocation
             (ViSession instrumentHandle, ViInt32 subframe, ViInt32 usedAllocation,
              ViInt32 codeword, ViInt32 modulation, ViInt32 resourceBlocks,
              ViInt32 resourceBlocksOffset, ViReal64 power);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHStartOffset
             (ViSession instrumentHandle, ViInt32 subframe, ViInt32 usedAllocation,
              ViInt32 resourceBlocksOffset);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPRBSymbolOffset
             (ViSession instrumentHandle, ViInt32 PRBSymbolOffset);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkReferenceSignal
             (ViSession instrumentHandle, ViReal64 referencePower);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSynchronisationSignal
             (ViSession instrumentHandle, ViReal64 pSYNCRelativePower,
              ViReal64 sSYNCRelativePower);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPBCH (ViSession instrumentHandle,
                                                     ViBoolean PBCHPresent,
                                                     ViReal64 PBCHRelativePower);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPCFICH (ViSession instrumentHandle,
                                                       ViBoolean PCFICHPresent,
                                                       ViReal64 PCFICHRelativePower);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPHICH (ViSession instrumentHandle,
                                                      ViInt32 PHICHNumberOfGroups,
                                                      ViInt32 PHICHDuration,
                                                      ViReal64 PHICHRelativePower);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPHICHNgParameter
             (ViSession instrumentHandle, ViInt32 PHICHNgParameter,
              ViInt32 PHICHNumberOfGroups);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPHICHEnhancedTestModels
             (ViSession instrumentHandle, ViBoolean PHICHTDDM_i1);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDCCH (ViSession instrumentHandle,
                                                      ViInt32 numberOfPDCCHs,
                                                      ViInt32 PDCCHFormat,
                                                      ViReal64 PDCCHRelativePower);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkPDSCHPowerRatio
             (ViSession instrumentHandle, ViInt32 PDSCHPowerRatio);
ViStatus _VI_FUNC rsspecan_UpdateIQResults (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkMeasurementType
             (ViSession instrumentHandle, ViInt32 window, ViInt32 measurementType);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSubframeSelection
             (ViSession instrumentHandle, ViBoolean allSubframes,
              ViInt32 singleSubframeSelection);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkEVMUnits (ViSession instrumentHandle,
                                                         ViInt32 EVMUnits);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkBitStream (ViSession instrumentHandle,
                                                          ViInt32 bitStreamFormat);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkSEMSettings
             (ViSession instrumentHandle, ViInt32 SEMChannelCategory);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkACPSettings
             (ViSession instrumentHandle, ViInt32 assumedAdjChannelCarrier,
              ViBoolean noiseCorrection);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkConstellationLocation
             (ViSession instrumentHandle, ViInt32 constellationLocation);
ViStatus _VI_FUNC rsspecan_ConfigureLTEDownlinkYAxisScaling
             (ViSession instrumentHandle, ViBoolean autoScaling,
              ViReal64 perDivision, ViReal64 offset);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementResultSummary
             (ViSession instrumentHandle, ViInt32 frameResult, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMAll
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMPhysicalChannel
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMPhysicalSignal
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementEVMSDSFAverage
             (ViSession instrumentHandle, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementFrequencyError
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementSamplingError
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementIQOffset
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementIQGainImbalance
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementIQQuadratureError
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementFramePower
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementCrestFactor
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementTriggerToFrame
             (ViSession instrumentHandle, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementOSTP
             (ViSession instrumentHandle, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementRSTP
             (ViSession instrumentHandle, ViInt32 mode, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementRSSI
             (ViSession instrumentHandle, ViInt32 mode, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkMeasurementACPResult
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR result[]);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkDetectedCellIdentityGroup
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkDetectedCellIdentity
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryLTEDownlinkDetectedCyclicPrefix
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkEVMVersusCarrierTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkEVMVersusSymbolTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkFrequencyErrorVersusSymbolTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkEVMVersusSubframeTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkSEMTrace (ViSession instrumentHandle,
                                                    ViInt32 measurementType,
                                                    ViInt32 arraySize,
                                                    ViReal64 _VI_FAR traceData[],
                                                    ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkACPTrace (ViSession instrumentHandle,
                                                    ViInt32 arraySize,
                                                    ViReal64 _VI_FAR traceData[],
                                                    ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkPowerSpectrumTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkPowerVersusRBReferenceSignalTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkPowerVersusRBPDSCHTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkChannelFlatnessTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkChannelGroupDelayTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkChannelFlatnessDifferenceTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkConstellationDiagram
             (ViSession instrumentHandle, ViInt32 bufferSize, ViInt32* noOfPoints,
              ViReal64 _VI_FAR realParts_I[], ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkCCDF (ViSession instrumentHandle,
                                                ViInt32 measurementType,
                                                ViInt32 arraySize,
                                                ViReal64 _VI_FAR traceData[],
                                                ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkAllocationSummary
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR subframeNumber[], ViInt32 _VI_FAR allocationID[],
              ViInt32 _VI_FAR numberOfRB[], ViInt32 _VI_FAR offsetRB[],
              ViInt32 _VI_FAR modulation[], ViReal64 _VI_FAR power[],
              ViReal64 _VI_FAR EVM[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEDownlinkBitstream (ViSession instrumentHandle,
                                                     ViInt32 arraySize,
                                                     ViInt32 bitStreamSize,
                                                     ViInt32 _VI_FAR subframeNumber[],
                                                     ViInt32 _VI_FAR allocationID[],
                                                     ViInt32 _VI_FAR codeword[],
                                                     ViInt32 _VI_FAR modulation[],
                                                     ViInt32 _VI_FAR numberOfSymbols[],
                                                     ViChar _VI_FAR bitStreams[],
                                                     ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_SetLTEUplinkMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSignalCharacteristics
             (ViSession instrumentHandle, ViInt32 duplexing, ViReal64 frequency,
              ViInt32 bandwidthSettingType, ViInt32 channelBandwidth,
              ViInt32 numberOfResourceBlocks, ViInt32 cyclicPrefix);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkLevel (ViSession instrumentHandle,
                                                    ViBoolean autoReferenceLevel,
                                                    ViReal64 referenceLevel,
                                                    ViReal64 externalAttenuation);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkLevelInBBMode
             (ViSession instrumentHandle, ViReal64 referenceLevel);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTrigger (ViSession instrumentHandle,
                                                      ViInt32 mode, ViReal64 offset,
                                                      ViReal64 holdoff,
                                                      ViReal64 hysteresis,
                                                      ViReal64 triggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkIQ (ViSession instrumentHandle,
                                                 ViBoolean swapIQ);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkInput (ViSession instrumentHandle,
                                                    ViInt32 source);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkAutoLevelTrackTime
             (ViSession instrumentHandle, ViReal64 autoLevelTrackTime);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkRFAttenuation
             (ViSession instrumentHandle, ViInt32 attenuation);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkDigitalBaseband
             (ViSession instrumentHandle, ViReal64 digitalInputDataRate,
              ViReal64 fullScaleLevel);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkMIMO (ViSession instrumentHandle,
                                                   ViInt32 antenna, ViInt32 PUCCH,
                                                   ViInt32 PUSCH, ViInt32 SRS);
ViStatus _VI_FUNC rsspecan_LoadLTEUplinkDemodulationSettings
             (ViSession instrumentHandle, ViString filePath);
ViStatus _VI_FUNC rsspecan_StoreLTEUplinkDemodulationSettings
             (ViSession instrumentHandle, ViString filePath);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkDataAnalysis
             (ViSession instrumentHandle, ViInt32 channelEstimation,
              ViBoolean compensateDCOffset, ViBoolean autoDemodulation);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkCodeBitsScrambling
             (ViSession instrumentHandle, ViBoolean scramblingOfCodedBits);
ViStatus
    _VI_FUNC rsspecan_ConfigureLTEUplinkSuppressedInterferenceSynchronization
        (ViSession instrumentHandle, ViBoolean suppressedInterferenceSync);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTracking (ViSession instrumentHandle,
                                                       ViInt32 phaseTracking,
                                                       ViBoolean timing);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTransientSlots
             (ViSession instrumentHandle, ViBoolean transientSlots);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTDDAllocation
             (ViSession instrumentHandle, ViInt32 TDDULDLAllocations);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkTDDSpecialSubframe
             (ViSession instrumentHandle, ViInt32 specialSubframe);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPhysicalLayerCellIdentity
             (ViSession instrumentHandle, ViInt32 cellIdentityGroup,
              ViInt32 identity);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkConfigurableSubframes
             (ViSession instrumentHandle, ViInt32 configurableSubframes);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSubframeTable
             (ViSession instrumentHandle, ViInt32 subframe, ViBoolean enablePUCCH,
              ViInt32 modulation, ViInt32 resourceBlocks,
              ViInt32 resourceBlocksOffset, ViReal64 power);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkReferenceSignal
             (ViSession instrumentHandle, ViReal64 relativePowerPUSCH,
              ViReal64 relativePowerPUCCH, ViBoolean groupHopping,
              ViBoolean sequenceHopping, ViInt32 deltaSequenceShift, ViInt32 n_DMRS,
              ViBoolean enableN_PRS);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignal
             (ViSession instrumentHandle, ViBoolean present, ViReal64 relativePower,
              ViInt32 symbolOffset, ViInt32 subcarrierOffset,
              ViInt32 numberOfSubcarriers);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignalNRRC
             (ViSession instrumentHandle, ViInt32 frequencyDomainPosition);
ViStatus
    _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignalANSimultaneousTX
        (ViSession instrumentHandle, ViBoolean ANSimultaneousTX);
ViStatus
    _VI_FUNC rsspecan_ConfigureLTEUplinkSoundingReferenceSignalModulation
        (ViSession instrumentHandle, ViReal64 parameterAlpha, ViInt32 parameterU,
         ViInt32 mode, ViInt32 parameterQ);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPUSCHStructure
             (ViSession instrumentHandle, ViInt32 frequencyHoppingMode,
              ViInt32 numberOfSubbands);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPUSCHHopping
             (ViSession instrumentHandle, ViInt32 hoppingOffset,
              ViInt32 infoInHoppingBits);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPUCCHStructure
             (ViSession instrumentHandle, ViInt32 numberOfRBsForPUCCH,
              ViInt32 deltaShift, ViInt32 deltaOffset, ViInt32 n1_cs, ViInt32 n2_RB,
              ViInt32 format, ViInt32 n_PUCCH);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPRACHStructure
             (ViSession instrumentHandle, ViInt32 PRACHConfiguration,
              ViBoolean restrictedSet, ViInt32 frequencyOffset,
              ViInt32 ncsConfiguration, ViInt32 logicalRootSequenceIndex,
              ViInt32 sequenceIndex, ViInt32 sequenceIndexValue);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkPRACHPreambleMapping
             (ViSession instrumentHandle, ViBoolean automaticPreambleMapping,
              ViInt32 frequencyIndex, ViInt32 halfFrameIndicator);
ViStatus _VI_FUNC rsspecan_ConfigureMarkerToTrace (ViSession instrumentHandle,
                                                   ViInt32 window, ViInt32 marker,
                                                   ViInt32 trace);
ViStatus _VI_FUNC rsspecan_LTEUplinkUpdateIQResults (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkMeasurementType
             (ViSession instrumentHandle, ViInt32 window, ViInt32 measurementType);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSubframeSelection
             (ViSession instrumentHandle, ViBoolean allSubframes,
              ViInt32 singleSubframeSelection);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSlotSelection
             (ViSession instrumentHandle, ViInt32 slotSelection);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkEVMUnits (ViSession instrumentHandle,
                                                       ViInt32 EVMUnits);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkBitStream (ViSession instrumentHandle,
                                                        ViInt32 bitStreamFormat);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkSEMSettings (ViSession instrumentHandle,
                                                          ViInt32 SEMChannelCategory);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkACLRSettings
             (ViSession instrumentHandle, ViInt32 assumedAdjChannelCarrier,
              ViBoolean noiseCorrection);
ViStatus _VI_FUNC rsspecan_ConfigureLTEUplinkYAxisScaling
             (ViSession instrumentHandle, ViBoolean autoScaling,
              ViReal64 perDivision, ViReal64 offset);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementResultSummary
             (ViSession instrumentHandle, ViInt32 frameResult, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementEVMAll
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementEVMPhysicalChannel
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementEVMPhysicalSignal
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementFrequencyError
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementSamplingError
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementIQOffset
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementIQGainImbalance
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementIQQuadratureError
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementFramePower
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementCrestFactor
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementTriggerToFrame
             (ViSession instrumentHandle, ViReal64* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkMeasurementACLRResult
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR result[]);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkDetectedCellIdentityGroup
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkDetectedCellIdentity
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_QueryLTEUplinkDetectedCyclicPrefix
             (ViSession instrumentHandle, ViInt32* result);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkEVMVersusCarrierTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkEVMVersusSymbolTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkEVMVersusSubframeTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkSEMTrace (ViSession instrumentHandle,
                                                  ViInt32 measurementType,
                                                  ViInt32 arraySize,
                                                  ViReal64 _VI_FAR traceData[],
                                                  ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkACLRTrace (ViSession instrumentHandle,
                                                   ViInt32 arraySize,
                                                   ViReal64 _VI_FAR traceData[],
                                                   ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkInbandEmissionTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkPowerSpectrumTrace
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkChannelFlatnessTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkChannelGroupDelayTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkChannelFlatnessDifferenceTrace
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR traceData[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkConstellationDiagram
             (ViSession instrumentHandle, ViInt32 bufferSize, ViInt32* noOfPoints,
              ViReal64 _VI_FAR realParts_I[], ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkDFTPrecodedConstellation
             (ViSession instrumentHandle, ViInt32 bufferSize, ViInt32* noOfPoints,
              ViReal64 _VI_FAR realParts_I[], ViReal64 _VI_FAR imaginaryParts_Q[]);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkCCDF (ViSession instrumentHandle,
                                              ViInt32 measurementType,
                                              ViInt32 arraySize,
                                              ViReal64 _VI_FAR traceData[],
                                              ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkAllocationSummary
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR subframeNumber[], ViInt32 _VI_FAR allocationID[],
              ViInt32 _VI_FAR numberOfRB[], ViInt32 _VI_FAR offsetRB[],
              ViInt32 _VI_FAR modulation[], ViReal64 _VI_FAR power[],
              ViReal64 _VI_FAR EVM[], ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadLTEUplinkBitstream (ViSession instrumentHandle,
                                                   ViInt32 arraySize,
                                                   ViInt32 bitStreamSize,
                                                   ViInt32 _VI_FAR subframeNumber[],
                                                   ViInt32 _VI_FAR modulation[],
                                                   ViInt32 _VI_FAR numberOfSymbols[],
                                                   ViChar _VI_FAR bitStreams[],
                                                   ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_SetTETRAMode (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureTETRASignalCharacteristics
             (ViSession instrumentHandle, ViReal64 frequency, ViInt32 slotType,
              ViInt32 channelBandwidth);
ViStatus _VI_FUNC rsspecan_ConfigureTETRALevelSettings (ViSession instrumentHandle,
                                                        ViBoolean autoLevel,
                                                        ViReal64 level,
                                                        ViReal64 externalAttenuation);
ViStatus _VI_FUNC rsspecan_ConfigureTETRADataCaptureSettings
             (ViSession instrumentHandle, ViReal64 captureTime,
              ViInt32 captureTimeUnit, ViBoolean useNumberOfSlotsToAnalyzer,
              ViInt32 numberOfSlotsToAnalyze);
ViStatus _VI_FUNC rsspecan_ConfigureTETRATriggerSettings (ViSession instrumentHandle,
                                                          ViInt32 mode,
                                                          ViReal64 offset,
                                                          ViInt32 offsetUnit,
                                                          ViReal64 extTriggerLevel,
                                                          ViBoolean autoTriggerLevel,
                                                          ViReal64 powerTriggerLevel);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAIQSettings (ViSession instrumentHandle,
                                                     ViBoolean swapIQ);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAInputSettings (ViSession instrumentHandle,
                                                        ViInt32 input);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAAdvancedBasebandSettings
             (ViSession instrumentHandle, ViInt32 impedance, ViInt32 path,
              ViBoolean balanced, ViBoolean lowPass, ViBoolean IQDither);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAAdvancedIQDigital
             (ViSession instrumentHandle, ViBoolean autoLevel, ViReal64 level,
              ViBoolean autoSamplingRate, ViReal64 samplingRate);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAIQCenterFrequency
             (ViSession instrumentHandle, ViReal64 IQCenterFrequency);
ViStatus _VI_FUNC rsspecan_ConfigureTETRADemodulationSettings
             (ViSession instrumentHandle, ViInt32 slotType,
              ViInt32 payloadModulation, ViReal64 demodRRCRolloff,
              ViInt32 maxCarrierOffset);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAEVMSettings (ViSession instrumentHandle,
                                                      ViBoolean pilotTracking,
                                                      ViBoolean compensateAmplitudeDroop,
                                                      ViBoolean compensateIQOffset);
ViStatus _VI_FUNC rsspecan_ConfigureTETRATraceSettings (ViSession instrumentHandle,
                                                        ViInt32 trace,
                                                        ViBoolean traceState);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAMarker (ViSession instrumentHandle,
                                                 ViInt32 window, ViInt32 marker,
                                                 ViBoolean markerEnabled,
                                                 ViInt32 trace);
ViStatus _VI_FUNC rsspecan_TETRAMarkerSearch (ViSession instrumentHandle,
                                              ViInt32 window, ViInt32 marker,
                                              ViInt32 markerSearch);
ViStatus _VI_FUNC rsspecan_TETRAMoveMarker (ViSession instrumentHandle,
                                            ViInt32 window, ViInt32 marker,
                                            ViInt32 measurement,
                                            ViReal64 markerPosition);
ViStatus _VI_FUNC rsspecan_QueryTETRAMarkerAmplitude (ViSession instrumentHandle,
                                                      ViInt32 window,
                                                      ViInt32 marker,
                                                      ViReal64* markerAmplitude);
ViStatus _VI_FUNC rsspecan_ConfigureTETRADisplayFormat (ViSession instrumentHandle,
                                                        ViInt32 format);
ViStatus _VI_FUNC rsspecan_ConfigureTETRADisplayActiveWindow
             (ViSession instrumentHandle, ViInt32 window);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAResultSummaryLimits
             (ViSession instrumentHandle, ViInt32 limitType, ViInt32 mode,
              ViReal64 value);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAResultSummaryLimitsAll
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR limitValues[]);
ViStatus _VI_FUNC rsspecan_GetTETRAResultSummaryLimitsAll
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR limitValues[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAACPLimits (ViSession instrumentHandle,
                                                    ViInt32 ACP,
                                                    ViInt32 channelNumber,
                                                    ViInt32 channelBandwidth,
                                                    ViReal64 limitValue);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAMeasurementType (ViSession instrumentHandle,
                                                          ViInt32 measurement);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAYAxis (ViSession instrumentHandle,
                                                ViInt32 measurement,
                                                ViBoolean autoScaling, ViInt32 unit,
                                                ViReal64 yCenter, ViReal64 ySpan);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAContinuousMeasurement
             (ViSession instrumentHandle, ViBoolean continuous);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAConstellationSettings
             (ViSession instrumentHandle, ViInt32 symbolsToPlotSelection,
              ViInt32 carrierSelection, ViInt32 carrierNumber);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAEVMResultUnits (ViSession instrumentHandle,
                                                         ViInt32 EVMUnits);
ViStatus _VI_FUNC rsspecan_ConfigureTETRAGainImbalanceResultUnits
             (ViSession instrumentHandle, ViInt32 gainImbalanceUnits);
ViStatus _VI_FUNC rsspecan_TETRABurstRecalc (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ReadTETRATraceData (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 trace,
                                               ViInt32 arraySize,
                                               ViReal64 _VI_FAR traceData[],
                                               ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_ReadTETRABitstream (ViSession instrumentHandle,
                                               ViInt32 window, ViInt32 trace,
                                               ViInt32 arraySize,
                                               ViReal64 _VI_FAR traceData[],
                                               ViInt32* noOfValues);
ViStatus _VI_FUNC rsspecan_FetchTETRANumberOfBursts (ViSession instrumentHandle,
                                                     ViInt32* numberOfBurst);
ViStatus _VI_FUNC rsspecan_FetchTETRAMeasuremenResults (ViSession instrumentHandle,
                                                        ViInt32 measurement,
                                                        ViInt32 measurementType,
                                                        ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchTETRAMeasurementResultsAll
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR result[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchTETRASubcarrierMeanPower (ViSession instrumentHandle,
                                                          ViInt32 measurementType,
                                                          ViInt32 arraySize,
                                                          ViReal64 _VI_FAR result[],
                                                          ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchTETRASubcarrierReferencePower
             (ViSession instrumentHandle, ViInt32 measurementType,
              ViInt32 arraySize, ViReal64 _VI_FAR result[],
              ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchTETRAPVTMeasurement (ViSession instrumentHandle,
                                                     ViInt32 PVT,
                                                     ViInt32 measurementType,
                                                     ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchTETRAPVTMeasurementAll (ViSession instrumentHandle,
                                                        ViReal64 _VI_FAR results[],
                                                        ViInt32 arraySize,
                                                        ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_FetchTETRASpectrumFFTMeasurementRBW
             (ViSession instrumentHandle, ViReal64* result);
ViStatus _VI_FUNC rsspecan_FetchTETRAACP (ViSession instrumentHandle, ViInt32 ACP,
                                          ViInt32 ACPMeasurement,
                                          ViInt32 channelNumber,
                                          ViInt32 measurementType,
                                          ViReal64* result);
ViStatus _VI_FUNC rsspecan_GetTETRATraceXAxisMinMax (ViSession instrumentHandle,
                                                     ViInt32 window, ViInt32 trace,
                                                     ViInt32 valueType,
                                                     ViReal64* xAxisValue);
ViStatus _VI_FUNC rsspecan_GetTETRAIQDataSamplingRate (ViSession instrumentHandle,
                                                       ViReal64* IQDataSamplingRate);
ViStatus _VI_FUNC rsspecan_GetTETRAResultSummaryLimitCheckResult
             (ViSession instrumentHandle, ViInt32 limitType, ViInt32 mode,
              ViInt32* result);
ViStatus _VI_FUNC rsspecan_GetTETRAResultSummaryLimitCheckResultsAll
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViInt32 _VI_FAR results[], ViInt32* returnedValues);
ViStatus _VI_FUNC rsspecan_GetTETRAACPLimitCheckResult (ViSession instrumentHandle,
                                                        ViInt32 ACP,
                                                        ViInt32 channelNumber,
                                                        ViInt32 measurementType,
                                                        ViInt32* result);
ViStatus _VI_FUNC rsspecan_ServiceConfigureInput (ViSession instrumentHandle,
                                                  ViInt32 window, ViInt32 input,
                                                  ViReal64 level);
ViStatus _VI_FUNC rsspecan_ServiceConfigurePulsedInput (ViSession instrumentHandle,
                                                        ViInt32 window,
                                                        ViBoolean state,
                                                        ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ServiceConfigureRectangleInput
             (ViSession instrumentHandle, ViInt32 window, ViBoolean state,
              ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ServiceConfigureNoise (ViSession instrumentHandle,
                                                  ViBoolean noise);
ViStatus _VI_FUNC rsspecan_ServiceConfigureBasebandSource
             (ViSession instrumentHandle, ViInt32 source);
ViStatus _VI_FUNC rsspecan_ServiceConfigurePulsedIQCalibrationSignal
             (ViSession instrumentHandle, ViInt32 destination, ViReal64 frequency);
ViStatus _VI_FUNC rsspecan_ServiceConfigureDCIQCalibrationSignal
             (ViSession instrumentHandle, ViReal64 voltage);
ViStatus _VI_FUNC rsspecan_ServiceConfigureCombGenerator (ViSession instrumentHandle,
                                                          ViInt32 window,
                                                          ViInt32 combFrequency);
ViStatus _VI_FUNC rsspecan_ServiceHWInfo (ViSession instrumentHandle,
                                          ViInt32 arraySize,
                                          ViChar _VI_FAR HWInfo[]);
ViStatus _VI_FUNC rsspecan_ServiceVersionInfo (ViSession instrumentHandle,
                                               ViInt32 arraySize,
                                               ViChar _VI_FAR versionInfo[]);
ViStatus _VI_FUNC rsspecan_FileDecimalSeparator (ViSession instrumentHandle,
                                                 ViInt32 decimalSeparator);
ViStatus _VI_FUNC rsspecan_FileManagerOperations (ViSession instrumentHandle,
                                                  ViInt32 operation,
                                                  ViString source,
                                                  ViString destination);
ViStatus _VI_FUNC rsspecan_FileDirectory (ViSession instrumentHandle,
                                          ViString directory, ViInt32 bufferSize,
                                          ViChar _VI_FAR output[]);
ViStatus _VI_FUNC rsspecan_FileDirectoryPath (ViSession instrumentHandle,
                                              ViString directory,
                                              ViInt32 bufferSize,
                                              ViChar _VI_FAR output[]);
ViStatus _VI_FUNC rsspecan_DataSetFileOperations (ViSession instrumentHandle,
                                                  ViInt32 operation, ViString path);
ViStatus _VI_FUNC rsspecan_DataSetFileSaveNext (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_DataSetFileComment (ViSession instrumentHandle,
                                               ViString comment);
ViStatus _VI_FUNC rsspecan_DataSetFileClearAll (ViSession instrumentHandle,
                                                ViString directory);
ViStatus _VI_FUNC rsspecan_DataSetFileSelectItems (ViSession instrumentHandle,
                                                   ViInt32 itemSelector,
                                                   ViBoolean itemState);
ViStatus _VI_FUNC rsspecan_DataSetFileSelectItemsFromChannel
             (ViSession instrumentHandle, ViInt32 itemSelector,
              ViBoolean itemState);
ViStatus _VI_FUNC rsspecan_DataSetLoadMode (ViSession instrumentHandle,
                                            ViInt32 mode);
ViStatus _VI_FUNC rsspecan_DataSetStoreMode (ViSession instrumentHandle,
                                             ViInt32 mode);
ViStatus _VI_FUNC rsspecan_ConfigureUserSettingsSoftkey (ViSession instrumentHandle,
                                                         ViInt32 softkeyNumber,
                                                         ViString filename,
                                                         ViString label);
ViStatus _VI_FUNC rsspecan_NetworkMapDrive (ViSession instrumentHandle,
                                            ViString driveLetter,
                                            ViString hostNameIPAddress,
                                            ViBoolean useOptionalParameters,
                                            ViString userName, ViString password,
                                            ViBoolean reconnectAtLogon);
ViStatus _VI_FUNC rsspecan_NetworkDisconnectDrive (ViSession instrumentHandle,
                                                   ViString driveLetter);
ViStatus _VI_FUNC rsspecan_NetworkUnusedDrivesList (ViSession instrumentHandle,
                                                    ViInt32 arraySize,
                                                    ViChar _VI_FAR unusedDrives[]);
ViStatus _VI_FUNC rsspecan_NetworkUsedDrivesList (ViSession instrumentHandle,
                                                  ViInt32 arraySize,
                                                  ViChar _VI_FAR usedDrives[]);
ViStatus _VI_FUNC rsspecan_NetworkShowFolderInfo (ViSession instrumentHandle,
                                                  ViBoolean showFolderInfo);
ViStatus _VI_FUNC rsspecan_reset (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Disable (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigureErrorChecking (ViSession instrumentHandle,
                                                   ViBoolean optionChecking,
                                                   ViBoolean rangeChecking,
                                                   ViBoolean statusChecking);
ViStatus _VI_FUNC rsspecan_self_test (ViSession instrumentHandle,
                                      ViInt16* selfTestResult,
                                      ViChar _VI_FAR selfTestMessage[]);
ViStatus _VI_FUNC rsspecan_SelfTestResults (ViSession instrumentHandle,
                                            ViInt32 bufferSize,
                                            ViChar _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_revision_query (ViSession instrumentHandle,
                                           ViChar _VI_FAR instrumentDriverRevision[],
                                           ViChar _VI_FAR firmwareRevision[]);
ViStatus _VI_FUNC rsspecan_SystemVersion (ViSession instrumentHandle,
                                          ViInt32 length,
                                          ViChar _VI_FAR systemVersion[]);
ViStatus _VI_FUNC rsspecan_ApplicationsVersions (ViSession instrumentHandle,
                                                 ViInt32 bufferSize,
                                                 ViChar _VI_FAR results[]);
ViStatus _VI_FUNC rsspecan_MemorySize (ViSession instrumentHandle,
                                       ViInt32* memorySize);
ViStatus _VI_FUNC rsspecan_error_query (ViSession instrumentHandle,
                                        ViInt32* errorCode,
                                        ViChar _VI_FAR errorMessage[]);
ViStatus _VI_FUNC rsspecan_ErrorList (ViSession instrumentHandle,
                                      ViInt32 bufferSize, ViChar _VI_FAR errors[]);
ViStatus _VI_FUNC rsspecan_ErrorListSpecificType (ViSession instrumentHandle,
                                                  ViInt32 messageType,
                                                  ViString channelName,
                                                  ViInt32 bufferSize,
                                                  ViChar _VI_FAR messages[]);
ViStatus _VI_FUNC rsspecan_error_message (ViSession instrumentHandle,
                                          ViStatus errorCode,
                                          ViChar _VI_FAR errorMessage[]);
ViStatus _VI_FUNC rsspecan_ClearInstrumentErrors (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_Preset (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ConfigurePresetOperatingMode (ViSession instrumentHandle,
                                                         ViInt32 presetOperatingMode);
ViStatus _VI_FUNC rsspecan_IDStringFactory (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_GetIDStringFactory (ViSession instrumentHandle,
                                               ViInt32* factoryDefault);
ViStatus _VI_FUNC rsspecan_SetTimeout (ViSession instrumentHandle,
                                       ViUInt32 timeout);
ViStatus _VI_FUNC rsspecan_GetTimeout (ViSession instrumentHandle,
                                       ViUInt32* timeout);
ViStatus _VI_FUNC rsspecan_LockLocalKey (ViSession instrumentHandle,
                                         ViBoolean lockLocalKey);
ViStatus _VI_FUNC rsspecan_setCheckStatus (ViSession instrumentHandle,
                                           ViBoolean state);
ViStatus _VI_FUNC rsspecan_setCheckOption (ViSession instrumentHandle,
                                           ViBoolean optionChecking);
ViStatus _VI_FUNC rsspecan_GetError (ViSession instrumentHandle, ViStatus* code,
                                     ViInt32 bufferSize,
                                     ViChar _VI_FAR description[]);
ViStatus _VI_FUNC rsspecan_ClearError (ViSession instrumentHandle);
ViStatus _VI_FUNC rsspecan_ReadToFileFromInstrument (ViSession instrumentHandle,
                                                     ViString source,
                                                     ViString destination);
ViStatus _VI_FUNC rsspecan_WriteFromFileToInstrument (ViSession instrumentHandle,
                                                      ViString source,
                                                      ViString destination);
ViStatus _VI_FUNC rsspecan_WriteInstrData (ViSession instrumentHandle,
                                           ViString writeBuffer);
ViStatus _VI_FUNC rsspecan_ReadInstrData (ViSession instrumentHandle,
                                          ViInt32 numberOfBytesToRead,
                                          ViChar _VI_FAR readBuffer[],
                                          ViUInt32* numBytesRead);
ViStatus _VI_FUNC rsspecan_QueryViBoolean (ViSession instrumentHandle,
                                           ViString command, ViBoolean* value);
ViStatus _VI_FUNC rsspecan_QueryViInt32 (ViSession instrumentHandle,
                                         ViString command, ViInt32* value);
ViStatus _VI_FUNC rsspecan_QueryViReal64 (ViSession instrumentHandle,
                                          ViString command, ViReal64* value);
ViStatus _VI_FUNC rsspecan_QueryViString (ViSession instrumentHandle,
                                          ViString command, ViInt32 bufferSize,
                                          ViChar _VI_FAR value[]);
ViStatus _VI_FUNC rsspecan_setStatusRegister (ViSession instrumentHandle,
                                              ViInt32 registerOperation,
                                              ViInt32 questionableRegister,
                                              ViInt32 enable, ViInt32 PTransition,
                                              ViInt32 NTransition);
ViStatus _VI_FUNC rsspecan_getStatusRegister (ViSession instrumentHandle,
                                              ViInt32 statusRegistersQuery,
                                              ViInt32* registerValue);
ViStatus _VI_FUNC rsspecan_setVSAModulationStatusRegister
             (ViSession instrumentHandle, ViInt32 window, ViInt32 registerOperation,
              ViInt32 questionableRegister, ViInt32 enable, ViInt32 PTransition,
              ViInt32 NTransition);
ViStatus _VI_FUNC rsspecan_getVSAModulationStatusRegister
             (ViSession instrumentHandle, ViInt32 window,
              ViInt32 questionableRegister, ViInt32 statusRegistersQuery,
              ViInt32* registerValue);
ViStatus _VI_FUNC rsspecan_ConditionFrequencyRegister (ViSession instrumentHandle,
                                                       ViInt32* statusRegisterValue);
ViStatus _VI_FUNC rsspecan_ConditionLimitRegister (ViSession instrumentHandle,
                                                   ViInt32 window,
                                                   ViInt32* statusRegisterValue);
ViStatus _VI_FUNC rsspecan_ConditionLimitMarginRegister (ViSession instrumentHandle,
                                                         ViInt32 window,
                                                         ViInt32* statusRegisterValue);
ViStatus _VI_FUNC rsspecan_ConditionACPLimitRegister (ViSession instrumentHandle,
                                                      ViInt32* statusRegisterValue);
ViStatus _VI_FUNC rsspecan_ConditionPowerRegister (ViSession instrumentHandle,
                                                   ViInt32* statusRegisterValue);
ViStatus _VI_FUNC rsspecan_ConditionSyncRegister (ViSession instrumentHandle,
                                                  ViInt32* statusRegisterValue);
ViStatus _VI_FUNC rsspecan_close (ViSession instrumentHandle);

ViStatus _VI_FUNC rsspecan_QueryOPC(ViSession instrSession, ViInt32* opc);
ViStatus _VI_FUNC rsspecan_IDQueryResponse (ViSession Instrument_Handle, ViUInt32 Buffer_Size, ViChar _VI_FAR ID_Query_Response[]);
ViStatus _VI_FUNC rsspecan_ProcessAllPreviousCommands (ViSession Instrument_Handle);
ViStatus _VI_FUNC rsspecan_ClearStatus (ViSession Instrument_Handle);
ViStatus _VI_FUNC rsspecan_ConfigureAutoSystemErrQuery(ViSession instrSession, ViBoolean autoSystErrQuery);
ViStatus _VI_FUNC rsspecan_GetVISATimeout(ViSession instrSession, ViUInt32* VISATimeout);
ViStatus _VI_FUNC rsspecan_SetVISATimeout(ViSession instrSession, ViUInt32 VISATimeout);
/****************************************************************************
 *---------------------------- End Include File ----------------------------*
 ****************************************************************************/

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __RSSPECAN_HEADER */


