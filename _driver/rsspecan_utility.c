/*****************************************************************************
 *------------------------ Utility  -----------------------------------------*
 *****************************************************************************/
#include <float.h>
#include <math.h>
#include "rsspecan.h"

/*****************************************************************************
 *- Callback Declarations ---------------------------------------------------*
 *****************************************************************************/

ViStatus rsspecan_CheckStatusCallback (ViSession io);

/*****************************************************************************
 *- Common Callback Declarations --------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_InitAttributes
 * Purpose:  This function inits attributes to the desired values if needed.
 *****************************************************************************/
ViStatus rsspecan_InitAttributes (ViSession instrSession)
{
    ViStatus    error           = VI_SUCCESS;
    ViInt32     p2value         = 0;
    void        *p2void;
    ViChar      buffer[RS_MAX_MESSAGE_BUF_SIZE],
                revision[RS_MAX_MESSAGE_BUF_SIZE],
                manufacturer[RS_MAX_MESSAGE_BUF_SIZE],
                model[RS_MAX_MESSAGE_BUF_SIZE],
                tmp_model[RS_MAX_MESSAGE_BUF_SIZE],
                idQ[RS_MAX_MESSAGE_BUF_SIZE],
                options_list[RS_MAX_MESSAGE_BUF_SIZE];

    buffer[RS_MAX_MESSAGE_BUF_SIZE -1] = 0;

    /*- Misc -*/

    if (!Rs_Simulating(instrSession))
        {
        /* Instrument identification */
        viCheckErr (viQueryf (instrSession, "*IDN?\n", "%[^\r\n]", idQ));
        idQ[RS_MAX_MESSAGE_BUF_SIZE - 1] = 0;
        checkErr (Rs_SetAttribute (instrSession, "", RSSPECAN_ATTR_ID_QUERY_RESPONSE, 0, idQ));
        
        /* Option(s) string */
        viCheckErr (viQueryf (instrSession, "*OPT?\n", "%[^\r\n]", options_list));
        options_list[RS_MAX_MESSAGE_BUF_SIZE - 1] = 0;
        }
    else /* Simulating */
        {
        /* Instrument identification */
        strncpy (idQ, RSSPECAN_SIMULATION_ID_QUERY, RS_MAX_MESSAGE_BUF_SIZE - 1);
        checkErr (Rs_SetAttribute (instrSession, "", RSSPECAN_ATTR_ID_QUERY_RESPONSE, 0, idQ));
        
        /* Option(s) string */
        strncpy (options_list, RSSPECAN_SIMULATION_OPT_QUERY, RS_MAX_MESSAGE_BUF_SIZE - 1);
        }

    /*- Inherent Instrument Specific Attributes ----------------------------*/

    /*- User Options -*/

    /*
    RS_ATTR_RANGE_CHECK             ... Rs_SpecificDriverNew
    RS_ATTR_QUERY_INSTRUMENT_STATUS ... Rs_SpecificDriverNew
    RS_ATTR_CACHE                   ... Rs_SpecificDriverNew
    RS_ATTR_SIMULATE                ... Rs_SpecificDriverNew
    RS_ATTR_RECORD_COERCIONS        ... not supported
    RS_ATTR_INTERCHANGE_CHECK       ... not supported
    RS_ATTR_SPY                     ... not supported
    RS_ATTR_USE_SPECIFIC_SIMULATION ... not supported
    */

    /*- Class Driver Identification -*/

    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_DESCRIPTION, 0, RSSPECAN_CLASS_DRIVER_DESCRIPTION));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_PREFIX, 0, RSSPECAN_CLASS_DRIVER_PREFIX));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_VENDOR, 0, RSSPECAN_CLASS_DRIVER_VENDOR));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_REVISION, 0, RSSPECAN_CLASS_DRIVER_REVISION));
    p2value = RSSPECAN_CLASS_SPEC_MAJOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_CLASS_SPEC_MAJOR_VERSION, 0, &p2value));
    p2value = RSSPECAN_CLASS_SPEC_MINOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_CLASS_SPEC_MINOR_VERSION, 0, &p2value));

    /*- Driver Identification -*/
                                                        
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_DESCRIPTION, 0, RSSPECAN_SPECIFIC_DRIVER_DESCRIPTION));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_PREFIX, 0, RSSPECAN_SPECIFIC_DRIVER_PREFIX));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_LOCATOR, 0, RSSPECAN_SPECIFIC_DRIVER_LOCATOR));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_VENDOR, 0, RSSPECAN_SPECIFIC_DRIVER_VENDOR));

    sprintf (buffer, 
             "Driver: %s %d.%d.%d, Compiler: %s %3.2f, "
             "Components: RSEngine %.2f, VISA-Spec %.2f",
             RSSPECAN_SPECIFIC_DRIVER_PREFIX,
             RSSPECAN_MAJOR_VERSION, RSSPECAN_MINOR_VERSION, RSSPECAN_MINOR_MINOR_VERSION,
             RS_COMPILER_NAME, RS_COMPILER_VER_NUM, 
             RS_ENGINE_MAJOR_VERSION + RS_ENGINE_MINOR_VERSION/10.0, 
             Rs_ConvertVISAVer(VI_SPEC_VERSION));

    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_REVISION, 0, buffer));
    p2value = RSSPECAN_CLASS_SPEC_MAJOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION, 0, &p2value));
    p2value = RSSPECAN_CLASS_SPEC_MINOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION, 0, &p2value));

    /*- Driver Capabilities -*/
                                   
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SUPPORTED_INSTRUMENT_MODELS, 0, RSSPECAN_SUPPORTED_INSTRUMENT_MODELS));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_GROUP_CAPABILITIES, 0, RSSPECAN_GROUP_CAPABILITIES));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_FUNCTION_CAPABILITIES, 0, RSSPECAN_FUNCTION_CAPABILITIES));
    /* RS_ATTR_CHANNEL_COUNT        ... Rs_BuildRepCapTable */

    /*- Driver Setup -*/

    /* RS_ATTR_DRIVER_SETUP         ... Rs_SpecificDriverNew */

    /*- Instrument Identification -*/

    checkErr (Rs_GetAttribute (instrSession, "", RSSPECAN_ATTR_ID_QUERY_RESPONSE, 0, RSSPECAN_IO_BUFFER_SIZE, idQ));
    sscanf (idQ, "%[^,], %[^,],%*[^,], %256[^\n]", manufacturer, tmp_model, revision); // added 2 spaces in the parsing string
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_INSTRUMENT_MANUFACTURER, 0, manufacturer));
    sscanf (tmp_model,"%[^-0-9]",model); // added number elimination
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, 0, model));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_INSTRUMENT_FIRMWARE_REVISION, 0, revision));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_OPTIONS_LIST, 0, options_list));

    /*- Advanced Session Information -*/
                                  
    /*  
    RS_ATTR_IO_RESOURCE_DESCRIPTOR  ... rsspecan_InitWithOptions
    RS_ATTR_LOGICAL_NAME            ... not supported
    */

    /*- Inherent Vendor Specific Attributes --------------------------------*/

    /*- Error Info -*/

    /*
    RS_ATTR_PRIMARY_ERROR           ... Rs_SetErrorInfo
    RS_ATTR_SECONDARY_ERROR         ... Rs_SetErrorInfo
    RS_ATTR_ERROR_ELABORATION       ... Rs_SetErrorInfo
    */

    /*- Session I/O -*/

    /*
    RS_ATTR_VISA_RM_SESSION         ... rsspecan_RsInit
    RS_ATTR_IO_SESSION              ... Rs_UpdateIOSession
    RS_ATTR_IO_SESSION_TYPE         ... not supported
    */

    /*- Version Info -*/                     

    p2value = RSSPECAN_MAJOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION, 0, &p2value));
    p2value = RSSPECAN_MINOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_MINOR_VERSION, 0, &p2value));
    p2value = RSSPECAN_MINOR_MINOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_SPECIFIC_DRIVER_MINOR_MINOR_VERSION, 0, &p2value));
    p2value = RSSPECAN_CLASS_SPEC_MAJOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_MAJOR_VERSION, 0, &p2value));
    p2value = RSSPECAN_CLASS_SPEC_MINOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CLASS_DRIVER_MINOR_VERSION, 0, &p2value));
    p2value = RS_ENGINE_MAJOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_ENGINE_MAJOR_VERSION, 0, &p2value));
    p2value = RS_ENGINE_MINOR_VERSION;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_ENGINE_MINOR_VERSION, 0, &p2value));
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_ENGINE_REVISION, 0, RS_ENGINE_REVISION));

    /*- Session Callbacks -*/                

#pragma warning( push )
#pragma warning( disable : 4152)
    p2void = rsspecan_WaitForOPCCallback;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_OPC_CALLBACK, 0, &p2void));
    p2void = rsspecan_CheckStatusCallback;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_CHECK_STATUS_CALLBACK, 0, &p2void));
    p2value = RSSPECAN_OPC_TIMEOUT;
    checkErr (Rs_SetAttribute (instrSession, "", RS_ATTR_OPC_TIMEOUT, 0, &p2value));
#pragma warning( pop )

Error:
    return error;
}

/*****************************************************************************
 * Function: rsspecan_DefaultInstrSetup
 * Purpose:  This function sends a default setup to the instrument. The
 *           rsspecan_reset function calls this function. The
 *           rsspecan_RsInit function calls this function when the
 *           user passes VI_FALSE for the reset parameter. This function is
 *           useful for configuring settings that other instrument driver
 *           functions require.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
ViStatus rsspecan_DefaultInstrSetup (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    /* Set all the attributes to the default state. Do not update inherent attributes! */
    checkErr (Rs_ApplyAttributeDefaults (instrSession, VI_FALSE));

    /* Init attributes */
    checkErr( rsspecan_InitAttributes (instrSession));

    if (!Rs_Simulating(instrSession))
    {
        viCheckErr( viPrintf (instrSession, "*CLS;*ESE %hu;*SRE 0\n", IEEE_488_2_ESE_MASK));
    }

Error:
    return error;
}

/*****************************************************************************
 * Function: rsspecan_CheckStatus
 * Purpose:  This function checks the status of the instrument to detect
 *           whether the instrument has encountered an error. This function
 *           is called at the end of most exported driver functions. If the
 *           instrument reports an error, this function returns
 *           RS_ERROR_INSTRUMENT_SPECIFIC. The user can set the
 *           rsspecan_ATTR_QUERY_INSTRUMENT_STATUS attribute to VI_FALSE to disable this
 *           check and increase execution speed.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
ViStatus rsspecan_CheckStatus (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    if (Rs_QueryInstrStatus (instrSession) && !Rs_Simulating (instrSession))
        {
        checkErr( rsspecan_CheckStatusCallback (instrSession));
        }

Error:
    return error;
}

/*****************************************************************************
 * Function: rsspecan_WaitForOPC
 * Purpose:  This function waits for the instrument to complete the
 *           execution of all pending operations. This function is a
 *           wrapper for the WaitForOPCCallback. It can be called from
 *           other instrument driver functions.
 *
 *           The maxTime parameter specifies the maximum time to wait for
 *           operation complete in milliseconds.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
ViStatus rsspecan_WaitForOPC (ViSession instrSession, ViUInt32 maxTime)
{
    ViStatus    error = VI_SUCCESS;
    
    if (!Rs_Simulating(instrSession))
        {
        ViInt32  oldOPCTimeout;

        checkErr( Rs_GetAttribute (instrSession, VI_NULL, RSSPECAN_ATTR_OPC_TIMEOUT, 0,
                                   (unsigned int) sizeof (oldOPCTimeout), &oldOPCTimeout));

        Rs_SetAttribute (instrSession, VI_NULL, RSSPECAN_ATTR_OPC_TIMEOUT, 0, &maxTime);

        error = rsspecan_WaitForOPCCallback (instrSession);

        Rs_SetAttribute (instrSession, VI_NULL, RSSPECAN_ATTR_OPC_TIMEOUT, 0, &oldOPCTimeout);

        viCheckErr( error);
        }
Error:
    return error;
}
 
/*===========================================================================*/
/* Function: String Index                                                    */
/* Purpose:  Returns the index of string in string array                     */
/*===========================================================================*/
ViInt32 rsspecan_StringIndex(ViString array[], ViChar buff[])
{
    ViInt32 index = 0;

    while (array[index]){
        if (!strcmp(buff, array[index]))
            break; 
        index++;    
    }        
    /* If found string is not found returns -1. */
    if (!array[index]) return -1;

    return index;
}

/*===========================================================================*/
/* Function: Read Binary Data Block                                          */
/* Purpose:  Reads the binary data from instrument. Allocates memory for data*/
/* WARNING:  You must deallocate buffer, if any pointer has been returned.   */
/*           This function does not work correctly, when you read bin data   */
/*           via serial intfc                                            */
/*===========================================================================*/
ViStatus rsspecan_readDataBlock (ViSession instrSession,
                                  ViChar **outputBuffer, ViUInt32 *byteCount)
{
    ViStatus    error   = VI_SUCCESS;
    ViUInt16    term_en;
    ViUInt32    ret_cnt;
    ViInt32     len;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE];
    
    *outputBuffer = VI_NULL;
    
    if (byteCount == NULL)
        return VI_ERROR_PARAMETER3;
    
    /* Disable termination character */
    checkErr (viGetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, &term_en));
    checkErr (viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));

    /* Parse header */
    checkErr (viRead (instrSession, (ViBuf) buffer, 1, &ret_cnt)); 
    if (buffer[0]=='#')
    {   /* Binary data */
        /* Disable termination character */
        checkErr (viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));
    
        /* Parse header */
        checkErr (viRead (instrSession, (ViBuf) buffer, 1, &ret_cnt));

        buffer[1] = '\0';
        if (buffer[0] == '(')
        {
            checkErr (viScanf (instrSession, "%ld)", &len));
        }
        else
            sscanf (buffer, "%ld", &len);
        
        if (len > 0)
        {
            checkErr (viRead (instrSession, (ViBuf) buffer, len, &ret_cnt));
            buffer[len] = '\0';
            sscanf (buffer, "%ld", &len);
        
            checkAlloc (*outputBuffer = (ViChar*) malloc (len));
            /* Read useful data */
            checkErr (viRead (instrSession, (ViBuf) *outputBuffer, len, &ret_cnt));
    
            if (len != (ViInt32) ret_cnt)
                viCheckErr (RS_ERROR_UNEXPECTED_RESPONSE);

            /* Flush garbage */
            viScanf (instrSession,"%*[^\r\n]");
            *byteCount = len;
            if (error > 0) error = VI_SUCCESS;
        }
        else
        {
            /* Unlimited size of data */
            checkErr (Rs_ReadDataUnknownLength (instrSession, outputBuffer, byteCount));
        }
    }
    else
    {   /* ASCII data */
        /* read rest of first value */
        ViChar* extBuf = NULL;

        /* read all values */
        viCheckErr (Rs_ReadDataUnknownLength(instrSession, &extBuf, byteCount));
        checkAlloc (*outputBuffer = (ViChar *)calloc (*byteCount + 2, sizeof (ViChar)));
        **outputBuffer = buffer[0];
        memcpy (*outputBuffer + 1, extBuf, *byteCount);
        *byteCount = *byteCount + 1;
        error = 1;
        
        if (extBuf) free (extBuf);
    }

Error:
    viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE);
    return error;
}

/*===========================================================================*/
/* Function: Long Signed Integer Value Out Of Range - ViInt32                */
/* Purpose:  This function checks a long signed integer value to see if it   */
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsspecan_invalidViInt32Range (ViInt32 val, ViInt32 min, ViInt32 max)
{
    return (ViBoolean)((val < min || val > max) ? VI_TRUE : VI_FALSE);
}
/*===========================================================================*/
/* Function: Long Unsigned Integer Value Out Of Range - ViUInt32             */
/* Purpose:  This function checks a long unsigned integer value to see if it */
/*           lies between a minimum and maximum value.  If the value is out  */
/*           of range, the return value is VI_TRUE, otherwise the return     */
/*           value is VI_FALSE.                                              */
/*===========================================================================*/
ViBoolean rsspecan_invalidViUInt32Range (ViUInt32 val, ViUInt32 min, ViUInt32 max)
{
    return (ViBoolean)((val < min || val > max) ? VI_TRUE : VI_FALSE);
}
/*===========================================================================*/
/* Function: Real (Double) Value Out Of Range - ViReal64                     */
/* Purpose:  This function checks a real (double) value to see if it lies    */
/*           between a minimum and maximum value.  If the value is out of    */
/*           range, the return value is VI_TRUE, otherwise the return value  */
/*           is VI_FALSE.                                                    */
/*===========================================================================*/
ViBoolean rsspecan_invalidViReal64Range (ViReal64 val, ViReal64 min, ViReal64 max)
{
    return (ViBoolean)((val < min || val > max) ? VI_TRUE : VI_FALSE);
}
/*===========================================================================*/
/* Function: Boolean Value Out Of Range - ViBoolean                          */
/* Purpose:  This function checks a Boolean to see if it is equal to VI_TRUE */
/*           or VI_FALSE. If the value is out of range, the return value is  */
/*           VI_TRUE, otherwise the return value is VI_FALSE.                */
/*===========================================================================*/
ViBoolean rsspecan_invalidViBooleanRange (ViBoolean val)
{
    return (ViBoolean)((val != VI_FALSE && val != VI_TRUE) ? VI_TRUE : VI_FALSE);
}
/*===========================================================================*/
/* Function: Swap 32bit Integer                                              */
/* Purpose:  Byte order for binary transfers depends on MOTOROLA or INTEL    */
/*           formats.                                                        */
/*===========================================================================*/
void rsspecan_SWAP_32(unsigned char *data, long points)
{
    /* MOTOROLA (byte order 0123) or INTEL (byte order 3210). */
    unsigned int    word = 0x1122;
    unsigned char   *p2word = (unsigned char*)&word;

    if (*p2word == 0x11) /* MOTOROLA format */
    {
       unsigned char    src[4];
       long             *p2data = (long*)data;
       int              idx;

       for (idx = 0; idx < points; idx++, p2data++)
          {
              *((long *)src) = *(p2data);
              ((unsigned char *)(p2data))[0] =  ((unsigned char*)(src))[3];
              ((unsigned char *)(p2data))[1] =  ((unsigned char*)(src))[2];
              ((unsigned char *)(p2data))[2] =  ((unsigned char*)(src))[1];
              ((unsigned char *)(p2data))[3] =  ((unsigned char*)(src))[0];
          }
    }

   return;
}

/*===========================================================================*/
/* Function: Parse Catalog List                                              */
/* Purpose:  Parse Catalog List returned from instrument and removes from it */
/*           all unneeded parameters from this list.                         */
/*===========================================================================*/
ViStatus rsspecan_ParseCatalog(ViChar *buffer, ViInt32 size, ViChar *catalogList, ViInt32 *nbValues)
{
    ViStatus    error = VI_SUCCESS; 
    ViChar*     pbuf;
    ViInt32     num;
    
    if (size != 0)
    {
        memcpy(catalogList, buffer, ( size > (ViInt32)strlen(buffer)) ? strlen(buffer) : size);
    }
    pbuf = strtok(buffer, ",");
    num=0;
    while (pbuf) 
    {
        num++;
        pbuf = strtok(NULL, ",");        
    } 
    if (nbValues) *nbValues = num;
    
    return error;
}

/*===========================================================================*/
/* Function: Parse Limit Line Catalog                                        */
/* Purpose:  Parse Limit Line Catalog returned from instrument and removes   */
/*           from it all unneeded parameters from this list.                 */
/*===========================================================================*/
ViStatus rsspecan_ParseLimitLineCatalog(ViChar *buffer, ViInt32 size, ViChar *catalogList, ViInt32 *nbValues)
{
    ViStatus    rsspecan_status = VI_SUCCESS;
    ViChar*     plist_of_files = 0;
    ViChar*     ptag = NULL;
    ViChar*     pend = NULL;
    ViInt32     i = 0;
    
    if (*buffer != '\0')
    {
        sscanf(buffer, "%*ld,%*ld,%[^\r\n]", buffer);
        plist_of_files = (ViChar*)malloc(strlen (buffer)+1);
        pend = plist_of_files;
        *pend = '\0';
        ptag = strtok(buffer, ",");
        while (ptag != NULL && rsspecan_status == VI_SUCCESS)
        {
            i++;
            pend += sprintf(pend, "%s,", ptag+1);
            ptag = strtok(NULL, ",");
            if (ptag == NULL)
                rsspecan_status = RS_ERROR_INVALID_VALUE;
            else
                ptag = strtok(NULL, ",");
        }
    }
    
    *--pend = '\0';

    if (nbValues) *nbValues = i;

    strncpy(catalogList, plist_of_files, size);
    
    free(plist_of_files);

    return rsspecan_status;
}

/*===========================================================================*/
/* Function: Read Trace Data                                                 */
/* Purpose:  This function reads out trace data from the instrument.         */
/*           It checks if data are ASCII or binary                           */
/*===========================================================================*/
ViStatus rsspecan_dataReadTrace (ViSession instrSession,
                                 ViInt32 window,
                                 ViString trace, 
                                 ViInt32 arrayLength, 
                                 /*@out@*/ViReal64 _VI_FAR traceData[],
                                 ViPInt32 noofPoints)
{
    ViChar      *extBuf=VI_NULL;
    ViReal32    *temp;
    ViUInt32    i = 0;
    ViStatus    error = VI_SUCCESS;
    ViUInt16    intfc = VI_INTF_GPIB;
    ViUInt32    count=0;
    ViChar      *p2buf=VI_NULL;
    ViUInt32    ret_cnt;
    
    checkErr (viGetAttribute (instrSession, VI_ATTR_INTF_TYPE, &intfc));
/*Don't put parameter range checking here! 
All parameters has to be checked in high level function */      
    if (window==0)    
    {
        viCheckErr (viPrintf (instrSession, ":FORM REAL,32;*CLS;:TRAC? %s\n",
                trace));
    }
    else
        {
        viCheckErr (viPrintf (instrSession, ":FORM REAL,32;*CLS;:TRAC%ld? %s\n",
                window, trace));                
        }
    viCheckErr (rsspecan_readDataBlock(instrSession, &extBuf, &ret_cnt));
    
    if (error == VI_SUCCESS)
    {   /* binary data, apply conversion */
        /* Convert binary data into array of ViReal64 types. */
        count = (ViInt32)(ret_cnt/4);
        /* Binary data byte order (MOTOROLA or INTEL). */
        rsspecan_SWAP_32((ViPBuf)extBuf, count);
        /* Pointer to data */
        temp = (ViReal32 *)extBuf;
        if (noofPoints) *noofPoints=count;
    
        count=(count<(ViUInt32)arrayLength)?count:(ViUInt32)arrayLength;
    
        for (i = 0; i < count; i++)
            if (i<(ViUInt32)arrayLength)
            *traceData++ = (ViReal64)*temp++;
    }
    else
        
    {   /* ASCII data */
        //read rest of first value

        p2buf = strtok(extBuf, ",");
        i = 0;
        while (p2buf)
        {
            if (i < (ViUInt32)arrayLength)
            {
                traceData[i] = atof (p2buf);
            }   
            p2buf = strtok(NULL, ",");
            i++;
        }
    
        if (noofPoints) *noofPoints=i; 
    
    }

Error:
    viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE);
    switch (intfc)
    {
        case VI_INTF_ASRL:
            viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, VI_ASRL_END_TERMCHAR);
        break;    
    }    
    if (extBuf) free(extBuf);
    return error;
}

/*===========================================================================*/
/* Function: Read ASCII Trace Data                                           */
/* Purpose:  This function reads out trace data in ascii form from           */
/*           the instrument.                                                 */
/*===========================================================================*/
ViStatus rsspecan_dataAsciiReadTrace (ViSession instrSession,
                                 ViInt32 window,
                                 ViString trace, 
                                 ViInt32 arrayLength, 
                                 /*@out@*/ViReal64 _VI_FAR traceData[],
                                 ViPInt32 noofPoints)
{
    ViUInt32 retCnt = 0;
    ViChar *extBuf=VI_NULL;
    ViChar *pbuffer;
    ViInt32  i = 0;
    ViStatus error = VI_SUCCESS;
    
    
/*Don't put parameter range checking here! 
All parameters has to be checked in high level function */      
    if (window==0)    
    {
        viCheckErr (viPrintf (instrSession, "*CLS;:TRAC? %s\n",
                trace));
    }
    else
    {
        viCheckErr (viPrintf (instrSession, "*CLS;:TRAC%ld? %s\n",
                window, trace));                
    }
    viCheckErr (Rs_ReadDataUnknownLength(instrSession, &extBuf, &retCnt)); 

    pbuffer = strtok(extBuf, ",");
    i=0;
    while (pbuffer) 
    {
        if (i < arrayLength)
        {
            traceData[i++] = atof (pbuffer);
        }   
        pbuffer = strtok(NULL, ",");        
    }
    
    if (noofPoints) *noofPoints=i;    
    
    
Error:
   
    if (extBuf) free(extBuf);
    return error;
}

/*===========================================================================*/
/* Function: Read Trace I/Q Data                                             */
/* Purpose:  This function will start a measurement and read I/Q trace data  */
/*           results.                                                        */
/*===========================================================================*/
ViStatus _VI_FUNC rsspecan_dataReadTraceIQ (ViSession   instrSession,
                                            ViInt32     bufferSize,
                                            ViPInt32    noofPoints,
                                            ViReal64 _VI_FAR realPartsI[],
                                            ViReal64 _VI_FAR imaginaryPartsQ[])
{
    ViStatus error = VI_SUCCESS;  
    ViUInt32    retCnt = 0;
    ViChar      *extBuf = 0;
    ViInt32     count = 0;
    ViInt32     i = 0, j = 0;
    ViUInt16    intfc = VI_INTF_GPIB;

    checkErr (viGetAttribute (instrSession, VI_ATTR_INTF_TYPE, &intfc));

    /* intfc specific operations */
    switch (intfc)
    {
        case VI_INTF_ASRL: /* Serial intfc. */
            viCheckErr( viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, VI_ASRL_END_NONE));
            viCheckErr( viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));
        break;
    }
    viCheckErr (rsspecan_readDataBlock(instrSession, &extBuf, &retCnt));

    //if (retCnt != count+4)
    //    viCheckErr (VI_ERROR_INV_RESPONSE);
   
    count = retCnt / (ViInt32)sizeof (float); /* number of I and Q samples */
    /* Binary data byte order (MOTOROLA or INTEL). */
    if (bufferSize > 0)
        rsspecan_SWAP_32((ViPBuf)extBuf, count);

    if (bufferSize > 0)
    {
        ViInt32 loopCount = count,
                remainder = 0,
                blockSize = 524288; /* I or Q block size (number of values) */

        ViReal64 *p2I = realPartsI,
                 *p2Q = imaginaryPartsQ;
        ViReal32 *tp2I,
                 *tp2Q;
        ViChar   *p2exBuf = extBuf;

        /*
            If number of I/Q samples is > 512k (524288 samples) block tranfer mode applies
                "<I-val 1><I-val 2>..<I-val 524288><Q-val 1><Q-val 2>..<Q-val 524288>..<..>"
            else returned data are splited in half for I and rest for Q
                "<I-val 1><I-val 2>..<I-val count/2><Q-val 1><Q-val 2>..<Q-val count/2>.."
        */

        loopCount = ((bufferSize * 2) < loopCount) ? (bufferSize * 2) : loopCount;

        /* number of I or Q samples */
        if (noofPoints)
            *noofPoints = loopCount / 2;

        /* Number of I/Q data blocks */
        remainder = loopCount % (blockSize * 2);
        loopCount /= (blockSize * 2);

        /* Make sure that loop counter is set to zero! */
        i = 0;

        /* If loopCount is not zero, retrieve all data */
        if (loopCount)
            {
            /* Number of I and Q data blocks */
            for (i = 0; i < loopCount; i++)
                {
                /* Set pointers */
                tp2I = (ViReal32*)(p2exBuf + (blockSize * 4) * (i*2));
                tp2Q = (ViReal32*)(p2exBuf + (blockSize * 4) * (i*2 + 1));
                /* I and Q values */
                for (j = 0; j < blockSize; j++)
                    {
                    *p2I++ = (ViReal64)*tp2I++;
                    *p2Q++ = (ViReal64)*tp2Q++;
                    }
                }
            }

        /* If reminder is not zero, retrieve remaining data */
        if (remainder)
            {
            /* Set pointers */
            if (i == 0)
                {
                tp2I = (ViReal32*)(p2exBuf);
                tp2Q = (ViReal32*)(p2exBuf + (remainder * 2));
                }
            else
                {
                tp2I = (ViReal32*)(p2exBuf + (blockSize * 4) * (2*i));
                tp2Q = (ViReal32*)(p2exBuf + (blockSize * 4) * (2*i) + (remainder*2));
                }
            /* I and Q values */
            for (j = 0; j < remainder; j += 2)
                {
                *p2I++ = (ViReal64)*tp2I++;
                *p2Q++ = (ViReal64)*tp2Q++;
                }
            }
    }
    checkErr( rsspecan_CheckStatus (instrSession));  
Error:
    switch (intfc)
    {
        case VI_INTF_ASRL:
            viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, VI_ASRL_END_TERMCHAR);
            viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE);
        break;    
    }    
    if (extBuf) free(extBuf);
    return error;
}

/*===========================================================================*/
/* Function: Read Trace Data                                                 */
/* Purpose:  This function only reads out trace data from the instrument.    */
/*           It checks if data are ASCII or binary.                           */
/*===========================================================================*/
ViStatus rsspecan_dataReadTraceOnly (ViSession instrSession,
                                 ViInt32 arrayLength, 
                                 /*@out@*/ViReal64 _VI_FAR traceData[],
                                 ViPInt32 noofPoints)
{
    ViChar      *extBuf=VI_NULL;
    ViReal32    *temp;
    ViUInt32     i = 0;
    ViStatus    error = VI_SUCCESS;
    ViUInt16    intfc = VI_INTF_GPIB;
    ViUInt32    count=0;
    ViChar      *p2buf=VI_NULL;
    ViUInt32    ret_cnt;
    
    viCheckErr (rsspecan_readDataBlock(instrSession, &extBuf, &ret_cnt));
    
    if (error == VI_SUCCESS)
    {   /* binary data, apply conversion */
        /* Convert binary data into array of ViReal64 types. */
        count = (ViInt32)(ret_cnt/4);
        /* Binary data byte order (MOTOROLA or INTEL). */
        rsspecan_SWAP_32((ViPBuf)extBuf, count);
        /* Pointer to data */
        temp = (ViReal32 *)extBuf;
        if (noofPoints) *noofPoints=count;
    
        count=(count<(ViUInt32)arrayLength)?count:(ViUInt32)arrayLength;
    
        for (i = 0; i < count; i++)
            if (i<(ViUInt32)arrayLength)
            *traceData++ = (ViReal64)*temp++;
    }
    else
        
    {   /* ASCII data */
        //read rest of first value

        p2buf = strtok(extBuf, ",");
        i = 0;
        while (p2buf)
        {
            if (i < (ViUInt32)arrayLength)
            {
                traceData[i] = atof (p2buf);
            }   
            p2buf = strtok(NULL, ",");
            i++;
        }
    
        if (noofPoints) *noofPoints=i; 
    
    }

Error:
    viSetAttribute (instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE);
    switch (intfc)
    {
        case VI_INTF_ASRL:
            viSetAttribute (instrSession, VI_ATTR_ASRL_END_IN, VI_ASRL_END_TERMCHAR);
        break;    
    }    
    if (extBuf) free(extBuf);
    return error;
}

/*===========================================================================*/
/* Function: Set OPC Timeout                                                 */
/* Purpose:  Sets new OPC timeout                                            */
/*===========================================================================*/
ViStatus rsspecan_SetOPCTimeout(ViSession   instrSession, ViInt32 timeout)
{
    ViStatus    error = VI_SUCCESS; 
    
    checkErr( Rs_SetAttribute (instrSession, VI_NULL, RS_ATTR_OPC_TIMEOUT, 0, &timeout));  
    
Error:    
    return error;
}

/*===========================================================================*/
/* Function: Get OPC Timeout                                                 */
/* Purpose:  Gets new OPC timeout                                            */
/*===========================================================================*/
ViStatus rsspecan_GetOPCTimeout(ViSession   instrSession, ViInt32 *timeout)
{
    ViStatus    error = VI_SUCCESS; 
    
    checkErr( Rs_GetAttribute (instrSession, VI_NULL, RS_ATTR_OPC_TIMEOUT, 0, (unsigned int) sizeof(timeout), timeout));  
    
Error:    
    return error;
}

/*****************************************************************************
 * Function: rsspecan_CheckBeforeRead
 * Purpose:  This function waits for the instrument to complete the
 *           execution of all pending operations and returns data. 
 *
 *           The maxTime parameter specifies the maximum time to wait for
 *           operation complete in milliseconds.
 *
 *           Note:  Call this function only when the session is locked.
 *****************************************************************************/
ViStatus rsspecan_CheckBeforeRead (ViSession instrSession, ViUInt32 maxTime)
{
    ViStatus    error = VI_SUCCESS;
    ViUInt16    response    = 0;
    ViUInt32     counter     = 0;
    ViUInt16    intfc   = VI_INTF_GPIB;
 
    viCheckErr( viGetAttribute (instrSession, VI_ATTR_INTF_TYPE, &intfc));

    /* intfc specific operations */
    switch (intfc)
        {
        case VI_INTF_ASRL: /* Serial intfc */

            /* Not supported */

        break;
        default: /* Default case */

            /* Check STB for errors. */
            do
            {
                error = viReadSTB (instrSession, &response);

                Rs_Delay (0.01);
                counter++;

                if (counter >= maxTime)
                    return VI_ERROR_TMO;

                /* Wait while STB is non zero value. Only Error/Event Queue
                   and ESB bits are taken into consideration. */
            
            } while ((response & 0x14) == 0);

            /* (check Error(Event) Queue bit in STB) */
            /* sometimes instrument sets both bits, but error queue is empty */
            return (((response & 0x4) != 0) && ((response & 0x10) == 0)) ? RS_ERROR_INSTRUMENT_STATUS : error;
        }

Error:
    return error;
}

/*===========================================================================*/
/* Function: Clear STB Before Read                                           */
/* Purpose:  This is a utility function which clears error and message queue */
/*           before starting a query                                         */
/*===========================================================================*/
ViStatus rsspecan_ClearBeforeRead (ViSession instrSession)
{
    ViStatus error = VI_SUCCESS;
    ViUInt16 response = 0;
    ViChar  garbage[10];

    viCheckErr (viReadSTB (instrSession, &response));
    
    while (response & 0x14)
    {
        if (response & 0x4)
        {
            ViInt32 i_error;
            viCheckErr(viQueryf(instrSession, "*ESR?\n", "%ld", &i_error));
            viCheckErr(viQueryf(instrSession, ":SYST:ERR?\n", "%*[^\r\n]"));
        }
        
        if (response & 0x10)
            viRead (instrSession, (ViBuf) garbage, 10, VI_NULL);

        viCheckErr (viReadSTB (instrSession, &response));
    
    }

Error:
    return error;
}

/// HIFN This function returns repeated capabilty name according to attribute and repeated capability id
/// HIRET Returns regular status code
/// HIPAR instrSession/Session.
/// HIPAR attributeID/Attribute ID
/// HIPAR repCapNameId/Repeated capability ID. For instance: Channel
/// HIPAR value/Numeric value of repeated capability (index in array of names)
/// HIPAR bufferSize/Size of repCapName array
/// HIPAR repCapName/String representing repeated capability name
ViStatus rsspecan_GetAttributeRepCapName (ViSession instrSession,
                                ViUInt32 attributeID,
                                ViString repCapNameId,
                                ViInt32 value,
                                ViInt32 bufferSize,
                                ViChar _VI_FAR repCapName[])
{
    ViStatus error = VI_SUCCESS;
    ViChar   *p2buf = NULL;
    ViChar   names [3*RS_MAX_MESSAGE_BUF_SIZE] = "";
    int      i = 0;
    
    checkErr (rsspecan_GetAttributeRepeatedCapabilityIdNames (instrSession,
                                                             attributeID,
                                                             repCapNameId,
                                                             RS_MAX_MESSAGE_BUF_SIZE,
                                                             names));
    
    p2buf = strtok (names, ",");
    while (p2buf)
    {
        if (value == i++)
        {
            strncpy (repCapName, p2buf, bufferSize);
            break;
        }
        p2buf = strtok (NULL, ",");
    }
    if (p2buf == NULL)
        error = RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME;
Error:
    return error;
}

/// HIFN Returns true for all FSV compatible instruments
/// HIRET Returns true for all FSV compatible instruments
/// HIPAR instrSession/Session.
ViBoolean rsspecan_IsFSV (ViSession instrSession)
{
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = "FSV";

    rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model);
    return ((strstr (model, "FSV") != NULL) || (strstr (model, "FSW") != NULL) || (strstr (model, "FSVR") != NULL));

}
