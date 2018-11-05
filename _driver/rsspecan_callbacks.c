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

ViStatus rsspecan_doubleQuotedString_ReadCallback (ViSession io, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value);
ViStatus rsspecan_doubleQuotedString_WriteCallback (ViSession io, ViConstString repCapName, ViAttr attributeId, void *value);


/***/

/*****************************************************************************
 *- Read Callbacks ----------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 * Function: rsspecan_empty_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Returns value
 *           from cache
 *****************************************************************************/
ViStatus rsspecan_empty_ReadCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    return VI_SUCCESS;
}   

/*===========================================================================*/
/* Function: Calibration User Callback                                       */
/* Purpose:  Overrides standard callback                                     */
/*===========================================================================*/
ViStatus rsspecan_Calibration_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViUInt32    max_cmd_size    = 0,
                n               = 0;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    max_cmd_size = (ViUInt32) strlen (sessionProperties -> attr[idx].command);

    if (repCapName)
        max_cmd_size += (ViUInt32) strlen (repCapName);

    viCheckAlloc (cmd = (ViChar*) malloc (++max_cmd_size));

    cmd[0] = 0;

    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        /* Repeated Capabilities */
        ViChar      single_repCapName[RS_MAX_MESSAGE_BUF_SIZE],
                    single_repCapNameId[RS_MAX_MESSAGE_BUF_SIZE],
                    command_value[RS_MAX_MESSAGE_BUF_SIZE],
                    tmp[RS_MAX_MESSAGE_BUF_SIZE],
                    *p2cmd = sessionProperties -> attr[idx].command;
        //          repCapNameId[RS_MAX_MESSAGE_BUF_SIZE],
        ViUInt32    index = 0;

            for (index = 0; index < RS_MAX_ATTR_RCAPS; index++)  
            {
                if ((error = Rs_GetTokenAtIndex ((ViChar *)repCapName, ",", index, single_repCapName, RS_MAX_MESSAGE_BUF_SIZE)) != VI_SUCCESS)
                {
                    error = VI_SUCCESS;
                    break;
                }
                /* Scan repCap from command*/
            
                n = (ViUInt32) sscanf (p2cmd, "%[^{]{%[^}]}", tmp, single_repCapNameId);
                p2cmd = strchr(p2cmd, '}') + 1;
               
                viCheckErr (Rs_GetRepCapCmdValue (instrSession, attributeId, single_repCapNameId, single_repCapName, command_value));
                max_cmd_size += (ViUInt32) strlen (command_value);
                viCheckAlloc (cmd = (ViChar*) realloc (cmd, max_cmd_size));

                /* Build the command */
                strcat (cmd, tmp);
                strcat (cmd, command_value);
            }

        strcat (cmd, p2cmd);
        }
    else
        strcpy (cmd, sessionProperties -> attr[idx].command);

   /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s?;*OPC\n", cmd));
            viCheckErr (rsspecan_WaitForOPCCallback(instrSession));
            viCheckErr (viScanf(instrSession, "%ld%*[^\n]", value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }
Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_stringToBin_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToBin_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;


    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

   /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_STRING:
            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viScanf(instrSession, "#B%s", value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_stringToHex_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToHex_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;


    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
   viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

   /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_STRING:
            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viScanf(instrSession, "#H%s", value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_hex_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_hex_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

   /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viScanf(instrSession, "#H%X", value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
};

/*****************************************************************************
 * Function: rsspecan_displayComment_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Reads string 
 *           returned with apostrophes
 *****************************************************************************/
ViStatus rsspecan_displayComment_ReadCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    
    if ((strstr (model, "FSL")) || (strstr (model, "FSV")))
    {
        viCheckErr (Rs_ReadCallback (instrSession, repCapName, attributeId, bufSize, value));       
    }
    else
    {
        viCheckErr (rsspecan_quotedString_ReadCallback (instrSession, repCapName, attributeId, bufSize, value));
    }
    
Error:

    return error;       
}
/*****************************************************************************
 * Function: rsspecan_quotedString_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Reads string 
 *           returned with apostrophes
 *****************************************************************************/
 ViStatus rsspecan_quotedString_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error           = VI_SUCCESS;
    ViUInt32    count           = bufSize;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    /* --- If the range table for the attribute exists, revert 'command string'
       back to the 'discreteOrMinValue' and return it as attribute value. --- */
    
    /* Range Table Callback */
    if (sessionProperties -> attr[idx].rangeTableCallback)
    {                                                          
        viCheckErr ((sessionProperties -> attr[idx].rangeTableCallback)(instrSession, repCapName, attributeId, &(sessionProperties -> attr[idx].rangeTable)));
    }
    
    if (sessionProperties -> attr[idx].rangeTable)
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViInt32     found;

            viCheckAlloc (buffer = (ViChar*) malloc (RS_MAX_MESSAGE_BUF_SIZE));
            count = RS_MAX_MESSAGE_BUF_SIZE;

            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viScanf(instrSession, "'%#[^']'", &count, buffer));

            /* Go over range table values and check if the command string is present */
            do
                {
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    return RS_ERROR_INVALID_VALUE;
#ifdef RS_LONG_RESPONSE            
                if ((strlen (p2string) <= 3) || (strlen (buffer) > 4))
                    found = strcmp (buffer,p2string);
                else
                    found = strncmp (buffer, p2string, strlen (p2string));
#else
                found = strcmp (buffer,p2string);
#endif
                if (found == 0)
                    break;

                i++;

                } while (i);

            /* Return value from range table */
            switch (sessionProperties -> attr[idx].dataType)
                {
                case RS_VAL_INT32:
                    *(ViInt32 *)value = (ViInt32) sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                    break;
                case RS_VAL_REAL64:
                    *(ViReal64 *)value = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_EVENT:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }
            
            goto Error;
            }

        /* --- Attribute data type is used for VISA I/O --- */
        switch (sessionProperties -> attr[idx].dataType)
            {
            case RS_VAL_STRING:
                if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
                viCheckErr (viScanf(instrSession, "'%#[^']'", &count, value));
                break;
            case RS_VAL_INT32:
            case RS_VAL_REAL64:                
            case RS_VAL_BOOLEAN:
            case RS_VAL_ADDR:
            case RS_VAL_SESSION:
            case RS_VAL_EVENT:
            case RS_VAL_UNKNOWN_TYPE:
            default:
                viCheckErr (RS_ERROR_INVALID_TYPE);
            }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
}

 
 /*****************************************************************************
 * Function: rsspecan_doubleQuotedString_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Reads string 
 *           returned with apostrophes
 *****************************************************************************/
 ViStatus rsspecan_doubleQuotedString_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error           = VI_SUCCESS;
    ViUInt32    count           = bufSize;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    /* --- If the range table for the attribute exists, revert 'command string'
       back to the 'discreteOrMinValue' and return it as attribute value. --- */
    
    /* Range Table Callback */
    if (sessionProperties -> attr[idx].rangeTableCallback)
    {                                                          
        viCheckErr ((sessionProperties -> attr[idx].rangeTableCallback)(instrSession, repCapName, attributeId, &(sessionProperties -> attr[idx].rangeTable)));
    }
    
    if (sessionProperties -> attr[idx].rangeTable)
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViInt32     found;

            viCheckAlloc (buffer = (ViChar*) malloc (RS_MAX_MESSAGE_BUF_SIZE));
            count = RS_MAX_MESSAGE_BUF_SIZE;

            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viScanf(instrSession, "\"%#[^\"]\"", &count, buffer));

            /* Go over range table values and check if the command string is present */
            do
                {
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    return RS_ERROR_INVALID_VALUE;
#ifdef RS_LONG_RESPONSE            
                if ((strlen (p2string) <= 3) || (strlen (buffer) > 4))
                    found = strcmp (buffer,p2string);
                else
                    found = strncmp (buffer, p2string, strlen (p2string));
#else
                found = strcmp (buffer,p2string);
#endif
                if (found == 0)
                    break;

                i++;

                } while (i);

            /* Return value from range table */
            switch (sessionProperties -> attr[idx].dataType)
                {
                case RS_VAL_INT32:
                    *(ViInt32 *)value = (ViInt32) sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                    break;
                case RS_VAL_REAL64:
                    *(ViReal64 *)value = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_EVENT:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }
            
            goto Error;
            }

        /* --- Attribute data type is used for VISA I/O --- */
        switch (sessionProperties -> attr[idx].dataType)
            {
            case RS_VAL_STRING:
                if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
                viCheckErr (viScanf(instrSession, "\"%#[^\"]\"", &count, value));
                break;
            case RS_VAL_INT32:
            case RS_VAL_REAL64:                
            case RS_VAL_BOOLEAN:
            case RS_VAL_ADDR:
            case RS_VAL_SESSION:
            case RS_VAL_EVENT:
            case RS_VAL_UNKNOWN_TYPE:
            default:
                viCheckErr (RS_ERROR_INVALID_TYPE);
            }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
}

 /*****************************************************************************
 * Function: rsspecan_Item1Item2Itemx_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_Item1Item2Itemx_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error           = VI_SUCCESS;
    ViUInt32    count           = bufSize;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  
  
    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            viCheckErr (viScanf(instrSession, "%ld", value));
            break;
        case RS_VAL_REAL64:
            viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            viCheckErr (viScanf(instrSession, "%le", value));
            break;
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            viCheckErr (viScanf(instrSession, "%#[^\r\n]", &count, value));
            break;
        case RS_VAL_BOOLEAN:
            {
                ViChar  buf[10];
                ViInt32 l_count = 3;
                
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
                viCheckErr (viScanf(instrSession, "%#[^\r\n]", &l_count, buf));
                if (!strncmp (buf, "ON", 2))
                    *(ViInt32*)value = 1;
                else
                {
                    if (!strncmp (buf, "OFF", 3))
                        *(ViInt32*)value = 0;
                    else
                        sscanf(buf, "%ld", (long*)value);
                }
//                viCheckErr (viScanf(instrSession, "%ld", value)); /* OFF = 0, ON = 1 */
            }
            break;
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_EVENT:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
    
} 

 /*****************************************************************************
 * Function: rsspecan_quotedInt_ReadCallback
 * Purpose:  This function overrides standard calback funtion. Reads int 
 *           returned with apostrophes
 *****************************************************************************/
 ViStatus rsspecan_quotedInt_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    return rsspecan_quotedString_ReadCallback(instrSession, repCapName, attributeId, bufSize, value);
}

/*****************************************************************************
 * Function: rsspecan_WimaxZoneToUse_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_WimaxZoneToUse_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;
    ViChar      response [RS_MAX_MESSAGE_BUF_SIZE] = "";

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

   /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viRead (instrSession, (ViPBuf) response, RS_MAX_MESSAGE_BUF_SIZE, NULL));
            if (strstr (response, "LAST") != NULL)
                *(ViInt32*)value = 0;
            else
                sscanf(response, "Z%ld", (long*)value);
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_DeltaMarkerReferenceMarker_ReadCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_DeltaMarkerReferenceMarker_ReadCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, ViInt32 bufSize, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx             = 0;
    ViChar      *cmd            = VI_NULL,
                *buffer         = VI_NULL;
    ViChar      response [RS_MAX_MESSAGE_BUF_SIZE] = "";

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

   /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            if ((sessionProperties -> attr[idx].access==RS_VAL_READ_ONLY)) 
            {
                viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s?\n", cmd));
            }
            viCheckErr (viRead (instrSession, (ViPBuf) response, RS_MAX_MESSAGE_BUF_SIZE, NULL));
            if (strstr (response, "FIX") != NULL)
                *(ViInt32*)value = 0;
            else
                sscanf(response, "%ld", (long*)value);
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }

Error:

    if (buffer)
        free (buffer);

    if (cmd)
        free (cmd);

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
ViStatus rsspecan_empty_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    return VI_SUCCESS;
}
/*****************************************************************************
 * Function: rsspecan_stringToBin_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_stringToBin_WriteCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
 {
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;
    
    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s #B%s\n", cmd, (ViString) value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }
Error:

    if (cmd)
        free (cmd);

    return error;
 }
/*****************************************************************************
 * Function: rsspecan_stringToHex_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_stringToHex_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;
    
    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s \'#H%s\'\n", cmd, (ViString) value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }
Error:

    if (cmd)
        free (cmd);

    return error;

} 
 
/*****************************************************************************
 * Function: rsspecan_hex_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_hex_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;
    
    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s #H%X\n", cmd, *(ViInt32 *)value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }
Error:

    if (cmd)
        free (cmd);

    return error;

};

/*****************************************************************************
 * Function: rsspecan_WimaxZoneToUse_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_WimaxZoneToUse_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;
    
    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            if (*(ViInt32 *)value != 0)
            {
                viCheckErr (viPrintf(instrSession, "%s Z%ld\n", cmd, *(ViInt32 *)value));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s LASTd\n", cmd));
            }
                
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }
Error:

    if (cmd)
        free (cmd);

    return error;

};

/*****************************************************************************
 * Function: rsspecan_InstrumentCreate_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_InstrumentCreate_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;
    
    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    switch (sessionProperties -> attr[idx].dataType)
    {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s \'Spectrum %ld\'\n", cmd, *(ViInt32 *)value));
            break;
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
    }
Error:

    if (cmd)
        free (cmd);

    return error;

};

/*****************************************************************************
 * Function: rsspecan_quotedString_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Writes string 
 *           with apostrophes
 *****************************************************************************/
ViStatus rsspecan_quotedString_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    /* --- If the range table for the attribute exists, pass 'command string' or
       'discreteOrMinValue' instead of attribute value. --- */
    /* Range Table Callback */
    if (sessionProperties -> attr[idx].rangeTableCallback)
    {                                                          
        viCheckErr ((sessionProperties -> attr[idx].rangeTableCallback)(instrSession, repCapName, attributeId, &(sessionProperties -> attr[idx].rangeTable)));
    }
    
    if (sessionProperties -> attr[idx].rangeTable)
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViReal64    *p2discrete;
            ViInt32     data_type = sessionProperties -> attr[idx].dataType;
            ViReal64    precision = sessionProperties -> attr[idx].comparePrecision;

            precision = (precision < DBL_MIN) ? DBL_MIN : precision;

            /* Allowed data types are only ViInt32 or ViReal64 */
            switch (data_type)
                {
                case RS_VAL_INT32:
                case RS_VAL_REAL64:
                    /* Valid data types */
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_EVENT:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }

            /* Go over range table values and find appropriate command string for passed
               attribute value. For floating point values use predefined attribute precision. */
            do
                {
                p2discrete = &sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    return RS_ERROR_INVALID_VALUE;

                if (data_type == RS_VAL_INT32)
                    if (fabs(*p2discrete - *(ViInt32 *)value) <= DBL_MIN)
                        {
                        viCheckErr (viPrintf(instrSession, "%s \'%s\'\n", cmd, p2string));
                        break;
                        }

                if (data_type == RS_VAL_REAL64)
                    if (fabs(*p2discrete - *(ViReal64 *)value) <= precision)
                        {
                        viCheckErr (viPrintf(instrSession, "%s \'%s\'\n", cmd, p2string));
                        break;
                        }

                i++;

                } while (i);

            goto Error; /* Exit the function */
            }

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s %ld\n", cmd, *(ViInt32 *)value));
            break;
        case RS_VAL_REAL64:
            viCheckErr (viPrintf(instrSession, "%s %.12f\n", cmd, *(ViReal64 *)value));
            break;
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s \'%s\'\n", cmd, value));
            break;
        case RS_VAL_BOOLEAN:
            viCheckErr (viPrintf(instrSession, "%s %s\n", cmd, (*(ViBoolean *)value == VI_FALSE) ? "OFF" : "ON"));
            break;
        case RS_VAL_EVENT:
            viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            break;
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;

}

/*******************************************************************************
 * Function: rsspecan_outputSelectionCallback_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Writes string 
 *           with apostrophes, when string is FOC, writes it without apostrophes
 *******************************************************************************/
ViStatus rsspecan_outputSelectionCallback_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    
    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_STRING:
            if (strcmp (value, "FOC") == 0)
			{
				viCheckErr (viPrintf(instrSession, "%s %s\n", cmd, value));
			}
			else
			{
				viCheckErr (viPrintf(instrSession, "%s \'%s\'\n", cmd, value));
			}
            break;
		case RS_VAL_INT32:
        case RS_VAL_REAL64:
        case RS_VAL_BOOLEAN:
        case RS_VAL_EVENT:
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;

}

/*****************************************************************************
 * Function: rsspecan_doubleQuotedString_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Writes string 
 *           with apostrophes
 *****************************************************************************/
ViStatus rsspecan_doubleQuotedString_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    /* --- If the range table for the attribute exists, pass 'command string' or
       'discreteOrMinValue' instead of attribute value. --- */
    /* Range Table Callback */
    if (sessionProperties -> attr[idx].rangeTableCallback)
    {                                                          
        viCheckErr ((sessionProperties -> attr[idx].rangeTableCallback)(instrSession, repCapName, attributeId, &(sessionProperties -> attr[idx].rangeTable)));
    }
    
    if (sessionProperties -> attr[idx].rangeTable)
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViReal64    *p2discrete;
            ViInt32     data_type = sessionProperties -> attr[idx].dataType;
            ViReal64    precision = sessionProperties -> attr[idx].comparePrecision;

            precision = (precision < DBL_MIN) ? DBL_MIN : precision;

            /* Allowed data types are only ViInt32 or ViReal64 */
            switch (data_type)
                {
                case RS_VAL_INT32:
                case RS_VAL_REAL64:
                    /* Valid data types */
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_EVENT:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }

            /* Go over range table values and find appropriate command string for passed
               attribute value. For floating point values use predefined attribute precision. */
            do
                {
                p2discrete = &sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    return RS_ERROR_INVALID_VALUE;

                if (data_type == RS_VAL_INT32)
                    if (fabs(*p2discrete - *(ViInt32 *)value) <= DBL_MIN)
                        {
                        viCheckErr (viPrintf(instrSession, "%s \"%s\"\n", cmd, p2string));
                        break;
                        }

                if (data_type == RS_VAL_REAL64)
                    if (fabs(*p2discrete - *(ViReal64 *)value) <= precision)
                        {
                        viCheckErr (viPrintf(instrSession, "%s \"%s\"\n", cmd, p2string));
                        break;
                        }

                i++;

                } while (i);

            goto Error; /* Exit the function */
            }

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s %ld\n", cmd, *(ViInt32 *)value));
            break;
        case RS_VAL_REAL64:
            viCheckErr (viPrintf(instrSession, "%s %.12f\n", cmd, *(ViReal64 *)value));
            break;
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s \"%s\"\n", cmd, value));
            break;
        case RS_VAL_BOOLEAN:
            viCheckErr (viPrintf(instrSession, "%s %s\n", cmd, (*(ViBoolean *)value == VI_FALSE) ? "OFF" : "ON"));
            break;
        case RS_VAL_EVENT:
            viCheckErr (viPrintf(instrSession, "%s\n", cmd));
            break;
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;

}
/*****************************************************************************
 * Function: rsspecan_FileStateSpecialFormat_WriteCallback
 * Purpose:  This function formats 1, before the argument string
 *****************************************************************************/
ViStatus rsspecan_FileStateSpecialFormat_WriteCallback(ViSession instrSession, 
                                ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s 1,\'%s\'\n", cmd, value));
            break;
        case RS_VAL_INT32: 
        case RS_VAL_REAL64:
        case RS_VAL_BOOLEAN: 
        case RS_VAL_EVENT:
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;
}

ViStatus rsspecan_HcopyColorDef_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  
    /* --- If the range table for the attribute exists, pass 'command string' or
       'discreteOrMinValue' instead of attribute value. --- */

    if (sessionProperties -> attr[idx].rangeTable)
    {
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViReal64    *p2discrete;
            ViInt32     data_type = sessionProperties -> attr[idx].dataType;
            ViReal64    precision = sessionProperties -> attr[idx].comparePrecision;

            precision = (precision < DBL_MIN) ? DBL_MIN : precision;

            /* Allowed data types are only ViInt32 or ViReal64 */
            switch (data_type)
                {
                case RS_VAL_INT32:
                case RS_VAL_REAL64:
                    /* Valid data types */
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_EVENT:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }

            /* Go over range table values and find appropriate command string for passed
               attribute value. For floating point values use predefined attribute precision. */
            do
                {
                p2discrete = &sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    viCheckErr (RS_ERROR_INVALID_VALUE);

                if (data_type == RS_VAL_INT32)
                    if (fabs(*p2discrete - *(ViInt32 *)value) <= DBL_MIN)
                        {
                        //viCheckErr (viPrintf(io, "%s %s\n", cmd, p2string));
                        break;
                        }

                if (data_type == RS_VAL_REAL64)
                    if (fabs(*p2discrete - *(ViReal64 *)value) <= precision)
                        {
                        //viCheckErr (viPrintf(io, "%s %s\n", cmd, p2string));
                        break;
                        }

                i++;

                } while (i != 0);
            /* Check Wait for OPC after writes flag */
            if ((sessionProperties -> attr[idx].flags & RS_VAL_WAIT_FOR_OPC_AFTER_WRITES) != 0)
            {
                viCheckErr (viPrintf(instrSession, "%s%s;*OPC\n", cmd, p2string));
            }   
            else
                viCheckErr (viPrintf(instrSession, "%s%s\n", cmd, p2string));
                
            goto Error; /* Exit the function */
            }
    }

    /* --- Attribute data type is used for VISA I/O --- */
    /* Check Wait for OPC after writes flag */
    if ((sessionProperties -> attr[idx].flags & RS_VAL_WAIT_FOR_OPC_AFTER_WRITES) != 0)
    {               
        switch (sessionProperties -> attr[idx].dataType)
            {
            case RS_VAL_INT32:
                viCheckErr (viPrintf(instrSession, "%s%ld;*OPC\n", cmd, *(ViInt32 *)value));
                break;
            case RS_VAL_REAL64:
            case RS_VAL_STRING:
            case RS_VAL_BOOLEAN:
            case RS_VAL_EVENT:
            case RS_VAL_ADDR:
            case RS_VAL_SESSION:
            case RS_VAL_UNKNOWN_TYPE:
            default:
                viCheckErr (RS_ERROR_INVALID_TYPE);
            }
     }
     else
     {
        switch (sessionProperties -> attr[idx].dataType)
            {
            case RS_VAL_INT32:
                viCheckErr (viPrintf(instrSession, "%s%ld\n", cmd, *(ViInt32 *)value));
                break;
            case RS_VAL_REAL64:
            case RS_VAL_STRING:
            case RS_VAL_BOOLEAN:
            case RS_VAL_EVENT:
            case RS_VAL_ADDR:
            case RS_VAL_SESSION:
            case RS_VAL_UNKNOWN_TYPE:
            default:
                viCheckErr (RS_ERROR_INVALID_TYPE);
            }
     }
       
Error:

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_quotedInt_WriteCallback
 * Purpose:  This function overrides standard calback funtion. Writes int 
 *           with apostrophes
 *****************************************************************************/
ViStatus rsspecan_quotedInt_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    return rsspecan_quotedString_WriteCallback(instrSession, repCapName, attributeId, value);
}

ViStatus rsspecan_FFTPhaseLinePosition_WriteCallback (ViSession instrSession,
                           ViConstString repCapName,
                           ViAttr attributeId,
                           void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViChar      buffer [RS_MAX_MESSAGE_BUF_SIZE] = "";
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));
    
    /* --- If the range table for the attribute exists, pass 'command string' or
       'discreteOrMinValue' instead of attribute value. --- */

    if (sessionProperties -> attr[idx].rangeTable)
    {
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViReal64    *p2discrete;
            ViInt32     data_type = sessionProperties -> attr[idx].dataType;
            ViReal64    precision = sessionProperties -> attr[idx].comparePrecision;

            precision = (precision < DBL_MIN) ? DBL_MIN : precision;

            /* Allowed data types are only ViInt32 or ViReal64 */
            switch (data_type)
                {
                case RS_VAL_INT32:
                case RS_VAL_REAL64:
                    /* Valid data types */
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_EVENT:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }

            /* Go over range table values and find appropriate command string for passed
               attribute value. For floating point values use predefined attribute precision. */
            do
                {
                p2discrete = &sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    viCheckErr(RS_ERROR_INVALID_VALUE);

                if (data_type == RS_VAL_INT32)
                    if (fabs(*p2discrete - *(ViInt32 *)value) <= DBL_MIN)
                        {
                        break;
                        }

                if (data_type == RS_VAL_REAL64)
                    if (fabs(*p2discrete - *(ViReal64 *)value) <= precision)
                        {
                        break;
                        }

                i++;

                } while (i != 0);
    /* Check Wait for OPC after writes flag */
            sprintf(buffer, "%s %s", cmd, p2string);
            if (sessionProperties -> attr[idx].flags & RS_VAL_WAIT_FOR_OPC_AFTER_WRITES)
                strcat (buffer, "DEG;*OPC\n");
            else
                strcat (buffer, "DEG\n");
            
            viCheckErr (viWrite (instrSession, (ViBuf) buffer, (ViUInt32) strlen (buffer), NULL));
            goto Error; /* Exit the function */
            }
    }            
    /* --- Attribute data type is used for VISA I/O --- */
    /* Check Wait for OPC after writes flag */
        switch (sessionProperties -> attr[idx].dataType)
            {
            case RS_VAL_INT32:
            sprintf(buffer, "%s %ld", cmd, *(ViInt32 *)value);
                break;
            case RS_VAL_REAL64:
              //sprintf(buffer, "%s %.12lf", cmd, *(ViReal64 *)value);
              //Modified to fix CVI bug formating 1.0e+11 to 0:00000.0000
              sprintf(buffer, "%s %.12Le", cmd, *(ViReal64 *)value);
                break;
            case RS_VAL_STRING:
              sprintf(buffer, "%s \'%s\'", cmd, (ViString) value);
                break;
            case RS_VAL_BOOLEAN:
              sprintf(buffer, "%s %s", cmd, (*(ViBoolean *)value == VI_FALSE) ? "OFF" : "ON");
                break;
            case RS_VAL_EVENT:
              sprintf(buffer, "%s", cmd);
                break;
            case RS_VAL_ADDR:
            case RS_VAL_SESSION:
            case RS_VAL_UNKNOWN_TYPE:
            default:
                viCheckErr (RS_ERROR_INVALID_TYPE);
            }
    if (sessionProperties -> attr[idx].flags & RS_VAL_WAIT_FOR_OPC_AFTER_WRITES)
        strcat (buffer,"DEG;*OPC\n");
     else
        strcat (buffer,"DEG\n");
    
     viCheckErr (viWrite (instrSession, (ViBuf) buffer, (ViUInt32) strlen (buffer), NULL));
    
Error:
    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_VsaDemodRecLengthSym_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_VsaDemodRecLengthSym_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));   

    /* --- If the range table for the attribute exists, pass 'command string' or
       'discreteOrMinValue' instead of attribute value. --- */

    if (sessionProperties -> attr[idx].rangeTable)
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViReal64    *p2discrete;
            ViInt32     data_type = sessionProperties -> attr[idx].dataType;
            ViReal64    precision = sessionProperties -> attr[idx].comparePrecision;

            precision = (precision < DBL_MIN) ? DBL_MIN : precision;

            /* Allowed data types are only ViInt32 or ViReal64 */
            switch (data_type)
                {
                case RS_VAL_INT32:
                case RS_VAL_REAL64:
                    /* Valid data types */
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_EVENT:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }

            /* Go over range table values and find appropriate command string for passed
               attribute value. For floating point values use predefined attribute precision. */
            do
                {
                p2discrete = &sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    viCheckErr(RS_ERROR_INVALID_VALUE)

                if (data_type == RS_VAL_INT32)
                    if (fabs(*p2discrete - *(ViInt32 *)value) <= DBL_MIN)
                        {
                        viCheckErr (viPrintf(instrSession, "%s %sSYM\n", cmd, p2string));
                        break;
                        }

                if (data_type == RS_VAL_REAL64)
                    if (fabs(*p2discrete - *(ViReal64 *)value) <= precision)
                        {
                        viCheckErr (viPrintf(instrSession, "%s %sSYM\n", cmd, p2string));
                        break;
                        }

                i++;

                } while (i != 0);

            goto Error; /* Exit the function */
            }

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s %ldSYM\n", cmd, *(ViInt32 *)value));
            break;
        case RS_VAL_REAL64:
            viCheckErr (viPrintf(instrSession, "%s %.12fSYM\n", cmd, *(ViReal64 *)value));
            break;
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s \'%s\'SYM\n", cmd, value));
            break;
        case RS_VAL_BOOLEAN:
            viCheckErr (viPrintf(instrSession, "%s %sSYM\n", cmd, (*(ViBoolean *)value == VI_FALSE) ? "OFF" : "ON"));
            break;
        case RS_VAL_EVENT:
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_VsaDemodRecLengthSec_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_VsaDemodRecLengthSec_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));   

    /* --- If the range table for the attribute exists, pass 'command string' or
       'discreteOrMinValue' instead of attribute value. --- */

    if (sessionProperties -> attr[idx].rangeTable)
        if (sessionProperties -> attr[idx].rangeTable -> type == RS_VAL_DISCRETE)
            {
            ViUInt16    i = 0;
            ViChar      *p2string;
            ViReal64    *p2discrete;
            ViInt32     data_type = sessionProperties -> attr[idx].dataType;
            ViReal64    precision = sessionProperties -> attr[idx].comparePrecision;

            precision = (precision < DBL_MIN) ? DBL_MIN : precision;

            /* Allowed data types are only ViInt32 or ViReal64 */
            switch (data_type)
                {
                case RS_VAL_INT32:
                case RS_VAL_REAL64:
                    /* Valid data types */
                    break;
                case RS_VAL_STRING:
                case RS_VAL_BOOLEAN:
                case RS_VAL_EVENT:
                case RS_VAL_ADDR:
                case RS_VAL_SESSION:
                case RS_VAL_UNKNOWN_TYPE:
                default:
                    viCheckErr (RS_ERROR_INVALID_TYPE);
                }

            /* Go over range table values and find appropriate command string for passed
               attribute value. For floating point values use predefined attribute precision. */
            do
                {
                p2discrete = &sessionProperties -> attr[idx].rangeTable -> rangeValues[i].discreteOrMinValue;
                p2string = sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString;

                if (sessionProperties -> attr[idx].rangeTable -> rangeValues[i].cmdString == RS_RANGE_TABLE_END_STRING)
                    viCheckErr(RS_ERROR_INVALID_VALUE)

                if (data_type == RS_VAL_INT32)
                    if (fabs(*p2discrete - *(ViInt32 *)value) <= DBL_MIN)
                        {
                        viCheckErr (viPrintf(instrSession, "%s %sS\n", cmd, p2string));
                        break;
                        }

                if (data_type == RS_VAL_REAL64)
                    if (fabs(*p2discrete - *(ViReal64 *)value) <= precision)
                        {
                        viCheckErr (viPrintf(instrSession, "%s %sS\n", cmd, p2string));
                        break;
                        }

                i++;

                } while (i != 0);

            goto Error; /* Exit the function */
            }

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_INT32:
            viCheckErr (viPrintf(instrSession, "%s %ldS\n", cmd, *(ViInt32 *)value));
            break;
        case RS_VAL_REAL64:
            viCheckErr (viPrintf(instrSession, "%s %.12fS\n", cmd, *(ViReal64 *)value));
            break;
        case RS_VAL_STRING:
            viCheckErr (viPrintf(instrSession, "%s \'%s\'S\n", cmd, value));
            break;
        case RS_VAL_BOOLEAN:
            viCheckErr (viPrintf(instrSession, "%s %sS\n", cmd, (*(ViBoolean *)value == VI_FALSE) ? "OFF" : "ON"));
            break;
        case RS_VAL_EVENT:
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;
}

/*****************************************************************************
 * Function: rsspecan_DeltaMarkerReferenceMarker_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_DeltaMarkerReferenceMarker_WriteCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));  

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        
        case RS_VAL_INT32:
            if (*(ViInt32 *)value == 0)
            {
                viCheckErr (viPrintf(instrSession, "%s FIX\n", cmd));
            }
            else
            {
                viCheckErr (viPrintf(instrSession, "%s %ld\n", cmd, *(ViInt32 *)value));
            }
            break;
        case RS_VAL_STRING:
        case RS_VAL_REAL64:
        case RS_VAL_BOOLEAN: 
        case RS_VAL_EVENT:
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

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
 ViStatus rsspecan_EVMCalculation_RangeTableCallback  (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
 {
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSV")) || (strstr (model, "FSW")))
    {
        *rangeTablePtr=&rsspecan_rngVSAModulationEVMCalcFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngVSAModulationEVMCalc;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
 }

 /*****************************************************************************
 * Function: rsspecan_SpectrogramHistoryDepth_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_SpectrogramHistoryDepth_RangeTableCallback  (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
 {
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSVR"))
    {
        *rangeTablePtr=&rsspecan_rngSpemHistoryBuffDepthFSVR;
    }
    else if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngSpemHistoryBuffDepthFSV;   
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngSpemHistoryBuffDepth;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
 }

/*****************************************************************************
 * Function: rsspecan_XAxisQuantize_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_XAxisQuantize_RangeTableCallback  (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
 {
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngVSAConfDisplayXQuantizeFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngVSAConfDisplayXQuantize;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
 }

 /*****************************************************************************
 * Function: rsspecan_TraceRange_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_TraceRange_RangeTableCallback(ViSession instrSession, ViConstString repCapName, 
                    ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSL")) || (strstr (model, "FSV")))
    {
        *rangeTablePtr=&rsspecan_rngFslTrace;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngTrace;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
};

 /*****************************************************************************
 * Function: rsspecan_AmpUnit_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_AmpUnit_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
 {
   ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL"))
    {
        *rangeTablePtr=&rsspecan_rngFslAmplitudeUnitsRangeTable;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngAmplitudeUnitsRangeTable;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;    
 }

/*****************************************************************************
 * Function: rsspecan_PowStandard_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_PowStandard_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSL"))
    {
        *rangeTablePtr=&rsspecan_rngFslMeasPowerStandard;
    }
    else if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngFsvMeasPowerStandard;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngMeasPowerStandard;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_ExtMixerHarmonic_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_ExtMixerHarmonic_RangeTableCallback  (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
 {
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngExtMixerHarmonicFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngExtMixerHarmonic;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
 }

 /*****************************************************************************
 * Function: rsspecan_BtoChannel_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_BtoChannel_RangeTableCallback(ViSession instrSession, ViConstString repCapName, 
                    ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViInt32     region;
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViInt32(instrSession,"", RSSPECAN_ATTR_BTO_GEOG, &region));
    switch (region){
        case RSSPECAN_BTO_GEOG_EUR:
        case RSSPECAN_BTO_GEOG_USA:
            *rangeTablePtr=&rsspecan_rngBtoChannel;    
        break;
        case RSSPECAN_BTO_GEOG_FRAN:
            *rangeTablePtr=&rsspecan_rngBtoChannelFranc;
        break;
    }    
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
};

ViStatus rsspecan_C2kSetCount_RangeTableCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{ 
    ViStatus    error = VI_SUCCESS;
    ViChar     mode[256]="";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString(instrSession,"", RSSPECAN_ATTR_GET_INSTR_MODE, 256, mode));
    if (!(strcmp(mode,"BC2K")&&strcmp(mode,"M2CK")))
        *rangeTablePtr=&rsspecan_rngC2KSetCount;        
    else
        *rangeTablePtr=&rsspecan_rngBDOSetCount;    
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_IQSrate_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_IQSrate_RangeTableCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (!(strstr (buffer, "B72")))
        *rangeTablePtr=&rsspecan_rngIQSRate;    
    else
        *rangeTablePtr=&rsspecan_rngIQSRateB72;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
} 

/*****************************************************************************
 * Function: rsspecan_AvgType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_AvgType_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSV")) || (strstr (model, "FSL")))
    {
        *rangeTablePtr=&rsspecan_rngAvgTypeFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngAvgType;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}
                                             
/*****************************************************************************
 * Function: rsspecan_InpAmpEattMan_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_InpAmpEattMan_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngInpAmptEattManFSV;
    }
	else if (strstr (model, "FSW"))
    {
        *rangeTablePtr=&rsspecan_rngInpAmptEattManFSW;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngInpAmptEattMan;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_AdemSpecBW_FSV_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_AdemSpecBW_FSV_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngAdemBandResFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngAdemBandRes;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_ZoneType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_ZoneType_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngWiMAXZoneTypeFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngWiMAXZoneType;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_SEListRangeFilterType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_SEListRangeFilterType_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSV"))
    {
        *rangeTablePtr=&rsspecan_rngListFiltTypeFSV;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngListFiltType;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_HCopyLanguage_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_HCopyLanguage_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if ((strstr (model, "FSV")) || (strstr (model, "FSL")))
    {
        *rangeTablePtr=&rsspecan_rngHcopyDeviceLangFSL;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngHcopyDeviceLang;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_IQSrateFMU_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_IQSrateFMU_RangeTableCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    ViChar      opt[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_OPTIONS_LIST, RSSPECAN_IO_BUFFER_SIZE, opt));
    
    if (!(strstr (buffer, "FMU")) && !(strstr (buffer, "FSQ")))
    {
        *rangeTablePtr=&rsspecan_rngIQSRate;    
    
        if ((strstr (opt, "B72")) || ((strstr (buffer, "FSV")) && (strstr (opt, "B70"))))
            *rangeTablePtr=&rsspecan_rngIQSRateB72;
    }
    else
        *rangeTablePtr=&rsspecan_rngIQSRateFMU;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_DetectorType_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_DetectorType_RangeTableCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (((strstr (buffer, "FMU")) != NULL) || ((strstr (buffer, "FSW")) != NULL))
        *rangeTablePtr=&rsspecan_rngDetectorTypeFMURangeTable;
    else if (strstr (buffer, "FSV") != NULL)
        *rangeTablePtr=&rsspecan_rngDetectorTypeESLRangeTable;
    else
        *rangeTablePtr=&rsspecan_rngDetectorTypeRangeTable;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_BBInput_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_BBInput_RangeTableCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (!(strstr (buffer, "FMU")))
        *rangeTablePtr=&rsspecan_rngBBInput;    
    else
        *rangeTablePtr=&rsspecan_rngBBInputFMU;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
} 

/*****************************************************************************
 * Function: rsspecan_TriggerRFLevel_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_TriggerRFLevel_RangeTableCallback(ViSession instrSession, ViConstString repCapName, ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    checkErr (rsspecan_GetAttributeViString (instrSession, "", RS_ATTR_INSTRUMENT_MODEL, RSSPECAN_IO_BUFFER_SIZE, buffer));
    
    if (!(strstr (buffer, "FSV")))
        *rangeTablePtr=&rsspecan_rngTrigRFPowerLevel;    
    else
        *rangeTablePtr=&rsspecan_rngTrigRFPowerLevelFSV;
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_TraceRange_RangeTableCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
 ViStatus rsspecan_NoiseMarkerAssign_RangeTableCallback(ViSession instrSession, ViConstString repCapName, 
                    ViAttr attributeId, RsRangeTablePtr *rangeTablePtr)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      model [RSSPECAN_IO_BUFFER_SIZE] = ""; 
    
    checkErr( Rs_LockSession (instrSession, VI_NULL)); 
    
    viCheckErr (rsspecan_GetAttributeViString (instrSession, "",
                                               RS_ATTR_INSTRUMENT_MODEL,
                                               RSSPECAN_IO_BUFFER_SIZE, model));
    if (strstr (model, "FSW"))
    {
        *rangeTablePtr=&rsspecan_rngNoiseMarkerAssignFSW;
    }
    else
    {
        *rangeTablePtr=&rsspecan_rngNoiseMarkerAssign;   
    }
   
Error:
    Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: rsspecan_Attenuation_WriteCallback
 * Purpose:  This function overrides standard calback funtion.
 *****************************************************************************/
ViStatus rsspecan_Attenuation_WriteCallback (ViSession instrSession, ViConstString repCapName, ViAttr attributeId, void *value)
{
    RsSessionPropertiesPtr  sessionProperties = Rs_ViSession (instrSession);
    ViStatus    error               = VI_SUCCESS;
    ViInt32     idx                 = 0;
    ViChar      *cmd                = VI_NULL;

    viCheckErr (Rs_GetAttributeIndex (instrSession, attributeId, &idx));

    /* If no command available, exit the function */
    if (sessionProperties -> attr[idx].command == VI_NULL)
        return VI_SUCCESS;

    /* Check if attribute uses repeated capabilities */
    if (sessionProperties -> attr[idx].usercaps == VI_TRUE)
        {
        if (!repCapName)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);

        if (strlen (repCapName) == 0)
            viCheckErr (RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME);
        }

    /* Build the command string */
    viCheckErr (Rs_BuildCommandString (instrSession, attributeId, idx, repCapName, &cmd));   

    /* --- Attribute data type is used for VISA I/O --- */
    switch (sessionProperties -> attr[idx].dataType)
        {
        case RS_VAL_REAL64:
            viCheckErr (viPrintf(instrSession, "%s %.0f\n", cmd, *(ViReal64 *)value));
            break;
        case RS_VAL_INT32:
        case RS_VAL_STRING:
        case RS_VAL_BOOLEAN:
        case RS_VAL_EVENT:
        case RS_VAL_ADDR:
        case RS_VAL_SESSION:
        case RS_VAL_UNKNOWN_TYPE:
        default:
            viCheckErr (RS_ERROR_INVALID_TYPE);
        }

Error:

    if (cmd)
        free (cmd);

    return error;
}
