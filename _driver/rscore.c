
/*	Rohde&Schwarz Instrument Driver Core
	Non-exported instrument driver core functions implementation.
	Original Release: October 2005
	By: Martin Koutny

	e-mail: CustomerSupport@rohde-schwarz.com

Modification History:

3.00, (build 32) 2018-11-05 Miloslav Macko
	- Reviewed and reworked to reflect the features of LabVIEW and IVI.NET core
	- Incompatible with rsidr_core 2.xx, therefore the file name and function prefixes change to be able to co-exist with rsidr_core 2.xx :
	- File names changed to rscore.h and rscore.c
	- Exported function names prefix changed to RsCore_
	- Internal function names changed to RsCoreInt_
	- Instrument drivers using this core no longer includes visa.h, only visatype.h
	- viCheckErr made rscore.c private

2.57, 2018-04-20
	- Rs_ClearBeforeRead - added sending *CLS before *ESE? in case of STB byte 0x20 is set

2.56, 2017-10-24
	- Replaced VISA specific null with NULL or 0

2.55, 2017-03-29
	- Fixed Rs_ReadDataUnknownLength

2.54, 2017-03-20
	- Fixed Rs_ClearBeforeRead

2.53, 2017-02-13
	- Added new option string allowing to apply delays (in ms) before each viWrite and viRead

2.52, 2016-11-08
	- Rs_GetAttribute - VI_SUCCESS_TERM_CHAR cleared out from return value

2.51, 2016-10-04
	- Rs_ClearBeforeRead - changed implementation, viReadSTB instead of *STB? if FastSweep=1 in option string
	- Rs_SetAttribute - clear of ESR registry to clear OPC bit
	- Rs_GetAttribute - clear of ESR registry to clear OPC bit
	- Rs_WriteCallback - removed prep-ending *CLS to command with *OPC

2.50, 2016-09-19
	- Rs_ClearBeforeRead - fixed hiding of local declarations

2.49, 2016-07-28
	- Modified Rs_ClearBeforeRead not to get stuck in infinite loop

2.48, 2016-07-22
	- Rs_GetAttribute - VI_SUCCESS_MAX_CNT cleared out from return value

2.47, 2016-06-08
	- Workaround for viStatusDesc bug in 64-bit

2.46, 2016-05-31
	- Rs_SpecificDriverNew - initializing inherent attribute RS_ATTR_RANGE_CHECK to VI_TRUE

2.45, 2016-04-08
	- Rs_CheckModelOpt - parsing of Model() without option fixed

2.44, 2016-03-21
	- Added Rs_WriteToFile2 and Rs_ReadFromFile2 to improve handling of large files

2.43, 2016-02-02
	- renumbered overlapping error codes

2.42, 2015-07-07
	- added ViStatus Rs_SpreadsheetStringToDblArray

2.41, 2015-02-26
	- new flag RS_VAL_WAIT_TO_CONTINUE added

2.40, 2014-08-21
	- Improved robustness of code
	- Removed coercing for boolean data type

2.39, 2014-08-19
	- Modified Rs_ClearBeforeRead to support serial interfaces

2.38, 2014-07-07
	- Modifications for Int64 data type

2.37, 2014-06-26
	- viCheckParm macro modified to work with both ways of its use

2.36, 2013-12-11
	- Rs_ClearBeforeRead - removed local shadowing problem

2.35, 2013-09-30
	- CVI 2013 warnings fixed

2.34, 2013-05-15
- ViInt64 data type added
	
2.33, 2013-04-05
	- some includes moved from header file to source file

2.32, 2013-03-13
	- Rs_ReadCallback : parsing quoted string formatting adapted to single and double quotes

2.31, 2013-01-22
	- Rs_ReadCallback : parsing quoted string formatting fixed

2.30, 2012-10-24
	- changed macro viCheckParm : now it overwrites old errors. This macro is used in HL
		functions and there it is required to do the overwrite

2.29, 2012-10-05
	- Rs_ClearBeforeRead : HiSLIP problem (see 2.27) solved.
	- Rs_SetAttribute : reverted back to previous version (without ESR query)

2.27, 2012-09-24
	- Rs_SetAttribute : added query of ESR register for attributes waiting for OPC to reset ESR register.
		It is necessary for synchronization of control and data queue if connected via HiSLIP,
		where *CLS at the beginning of command string is not fast enough and consequent call into viRedSTB
		reads previous value of STB register

2.26, 2012-09-04
	- Rs_ReadCallback : changed parsing of the string response to eliminate
		callbacks for quoted strings

2.25, 2012-07-13
	- Rs_ReadToFile, Rs_WriteFromFile - buffer allocation changed to dynamic

2.24, 2012-07-11
	- Changed RS_RANGE_TABLE_LAST_ENTRY to remove warnings, introduced RS_RANGE_TABLE_END_VALUE in unused cmdValue item
	- Fixed Rs_CoerceCallback - could have livelock for user values larger than largest coerced range value
	- Fixed missing termination character for optionString local copy in Rs_SpecificDriverNew

2.23, 2012-07-09
	- Rs_RepCapCount, Rs_AddRepCap - buffer size changed from RS_IO_BUFFER_SIZE to RS_MAX_MESSAGE_LEN
	- RS_IO_BUFFER_SIZE changed value to 1Mi (1048576)

2.22, 2012-05-17
- Added RS_WARN_UNKNOWN_INSTRUMENT_STATUS
	
2.21, 2012-02-06
	- Rs_GetErrorInfo : modified error elaboration
	- Changed some error codes' values

2.20, 2011-12-20
	- Rs_Dispose : fixed memory leaks

2.19, 2011-12-01
	- Rs_GetErrorInfo : modified error elaboration for RS_ERROR_INSTRUMENT_STATUS

2.18, 2011-07-18
	- added function that returns the count of values in repeated capability:
		Rs_RepCapCount

2.17, 2011-07-03
	- removed <userint.h> and ProcessSystemEvents

2.16, 2011-06-12
	- added dynamic repeated capability
	- repeated capabilities are deep copied to session instead of shallow copy
	- added Rs_AddRepCap, Rs_RemoveRepCap, Rs_ResetRepCap, Rs_ResetRepCapTable

2.15, 2010-09-07
	- Modification: if repeated capability is NULL or empty string, attributes
		does not fail with error, but takes default (zeroth) repeated capability name

2.14, 2010-06-29
	- Modifications for 64bit compatibility

2.13, 2010-06-09
	- Rs_BuildCommandString, Rs_GetRepCapCmdValue - repCapNames size increased 3 times
	- Rs_GetAttributeRepCapName - strncpy is now 3 times bigger (relates to above fix)

2.12, 2010-06-07
	- fixed Rs_ReadCallback: incorrect ViBoolean reading

2.11, 2010-05-13
	- fixed rep_cap comparing method in:
		Rs_ValidateAttrForRepCapNameId
		Rs_GetAttributeRepCapName
		Rs_GetRepCapCmdValue

2.10, 2010-03-29
	- Rs_Delay - Sleep redesigned for Windows and CVI platforms only

2.9, 2010-03-10
	- Rs SetAttribute - Check status - attribute specified callback added

2.8, 2010-03-09
	- %.12Le changed to %.lG

2.7, 2009-06-18
	- RS_STATUS_CODE_BASE: value changed

2.6, 2009-04-02
	- fixed Rs_SetErrorInfo. When no overwrite selected, attributes have not been filled

2.5, 2009-02-24
	- fixed Rs_BuildCommandString - if there was nothing between repCaps ({repCap1}{repCap2}), function failed
	- larger repeated capability buffer (RS_MAX_MESSAGE_BUF_SIZE)

2.4, 2008-10-01
	- RS_STATUS_CODE_BASE changed from 0x3FFA0000L to 0x3FFF0000L (customer request)

2.3, 2008-09-08
	- fixed Rs_SpecificDriverNew - incorrect status query attribute used

2.2, 2008-02-27
	- added option checking switching, added to structure RsCoreSession,
		changes in functions: Rs_SpecificDriverNew, Rs_CheckModelOpt

2.1, 2007-11-15
	- fixed Rs_BuildCommandString for Visual Studio compilation

2.0, 2007-09-25
	- redesigned to support two drivers in one application
	- Rs_BuildCommandString function added
	- Rs_ClearBeforeRead function added
	- *OPC checking modified

1.13, 2007-07-16
	- RsCoreCompareAttrCallback - repCaps, numberOfRestRepCaps, restricted repCaps removed from RsCoreAttribute definition
	- Rs_ReadCallback, Rs_WriteCallback - repCap replace changed
	- Rs_ValidateAttrForRepCapNameId - repeated capability working changed
	- restricted capability checking removed, no restricted capability supported

1.12, 2006-12-11
	- Rs_ApplyAttributeDefaults - data_size calculation modified for RS_VAL_STRING, the length of default string
		is not always RS_VAL_STRING_SIZE.
	- Rs_WriteCallback - sprintf format string modified for RS_VAL_REAL64

1.11, 2006-09-04
	- removed vi session from API of all functions
	- implemented simple resource management; modified Rs_ViSession to avoid too many calls into viGetAttribute
	- fixed Rs_WriteCallback, string attribute write fixed

1.10, 2006-08-24
	- Rs_CheckModelOpt - modified, strtok removed

1.9, 2006-07-26
	- Rs_GetAttribute - wait for OPC before read support added
	- Rs_GetIndexOfToken - search algorithm modified to distinguish CM10 and CM1
		in string CM1, CM2, CM3, ... CM10
	- Rs_WriteCallback - wait for OPC after read support added

1.8, 2006-07-21
	- RS_ATTR_... constant values changed in rsidr_core.h

1.7, 2006-06-06
	- Code optimized according to LINT recommendations

1.6, 2006-03-20
	- Rs_Delay - fixed for non-Windows environment. Added Sleep (0) to
		prevent live lock.

1.5, 2005-12-14
	- Rs_ReadCallback - #ifdef section (RS_LONG_RESPONSE) added for non-standard instrument response
		in long form. If RS_LONG_RESPONSE defined, the values in list has to be sorted since longer to shorter
		eg. {UNSP,ETSI2,MMDS2,ETSI,MMDS}

1.4, 2005-11-03
	- removed unnecessary warnings
	- Rs_CheckModelOpt - various fixes, incorrect recognition of supported models, failed if no option is selected
	- fixed Rs_ParamPositionError - incorrect condition for parameter > 8
	- fixed Rs_GetErrorInfo - memcpy replaced by memset

1.3, 2005-10-13
	- Rs_GetRepCapCmdValue - incorrect operation with restricted capabilities fixed
	- Rs_CheckModelOpt - if no model listed, option checking was skipped, fixed

1.2, 2005-10-07 by Martin Koutny
	- Rs_CheckModelOpt: added missing implementation of option checking,
		functions Rs_SetAttribute, Rs_GetAttribute and
		Rs_CheckAttribute are also modified.
	- Range checking (range tables handling) added. Functions
		Rs_SetAttribute, Rs_GetAttribute and
		Rs_CheckAttribute are also modified.
	- Added missing implementation for following functions:
		Rs_CoerceCallback
		Rs_CheckCallback
	- Updated macros:   Added parameter instrSession instead of vi
		viCheckErrElab
		viCheckParm
		viCheckWarn
	- Changed functions: exported vi parameter is replaced by session
		Rs_SetErrorInfo
		Rs_GetErrorInfo
		Rs_ClearErrorInfo
	- Reimplemented functions:  Rs_WriteCallback
		Rs_ReadCallback
	- code debugging and fixing

1.1, 2005-10-05 by Jiri Kominek
	- Rs_WriteCallback, Rs_SetAttribute: added RS_VAL_EVENT to switch structure to support
	attributes with no argument.

1.0, First Release
	- Rs_GetAttribute - bug fixed in copy returned string to return value for caching. No MAX_BUFFER_SIZE can be copied.
		returned value is allocated by user and its size might not be MAX_BUFFER_SIZE
	- Rs_CheckCallback, Rs_CoerceCallback - user defined Range Table callback moved from "if (rangeTable)" case */

// #pragma warning (disable:4100 4055)
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <locale.h>

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define ANSI
#endif

#ifndef ANSI                /* UNIX compatible */
#include <sys/timeb.h>
#include <sys/types.h>
#include <unistd.h>
#define Sleep sleep
#else                       /* ANSI compatible */
#include <windows.h>
#endif

#include "rscore.h"
#include <visa.h>

#if defined (__linux__)
#include <pthread.h>
#include <ctype.h>
#endif

// Core version info
#define RSCORE_ENGINE_REVISION                      "Rev 3.00, 10/2018"
#define RSCORE_ENGINE_MAJOR_VERSION                 3L
#define RSCORE_ENGINE_MINOR_VERSION                 00

/*****************************************************************************************************/
/*  Useful Constants
******************************************************************************************************/

// Status Byte masks
#define IEEE_488_2_STB_ESB_MASK                 0x20 // Status Byte ESB bit mask
#define IEEE_488_2_STB_ERR_QUEUE_MASK           0x04 // Status Byte ErrorQueueNotEmpty bit mask
#define IEEE_488_2_STB_MAV_MASK                 0x10 // Status Byte Message available mask

// Max number of repCaps for one attribute command
#define RSCORE_MAX_ATTR_RCAPS                   10 // Max number of repeated capability names per attribute

#define RS_VAL_INT_MAX 2147483647
#define RS_VAL_INT_MIN (-2147483647 - 1)
#define RS_VAL_LLONG_MAX 9223372036854775807
#define RS_VAL_LLONG_MIN (-9223372036854775807 - 1)

/*****************************************************************************************************/
/*  Driver data types and string pairs
******************************************************************************************************/
const RsCoreDataType gDataTypes[] = {
	RS_VAL_UNKNOWN_TYPE, RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64,
	RS_VAL_STRING, RS_VAL_ADDR, RS_VAL_SESSION, RS_VAL_BOOLEAN, RS_VAL_EVENT, 0
};

const ViString gDataTypeStrings[] = {
	"RS_VAL_UNKNOWN_TYPE", "RS_VAL_INT32", "RS_VAL_INT64", "RS_VAL_REAL64",
	"RS_VAL_STRING", "RS_VAL_ADDR", "RS_VAL_SESSION", "RS_VAL_BOOLEAN", "RS_VAL_EVENT"
};

/*****************************************************************************************************/
/*  Non Exported DataTypes
******************************************************************************************************/
#if defined (__linux__)
#define MUTEX pthread_mutex_t*
#elif defined(ANSI)
#define MUTEX HANDLE
#endif
/*****************************************************************************************************/
/*  Non-exported functions
******************************************************************************************************/

MUTEX RsCoreInt_MutexLock(MUTEX mutex);
void RsCoreInt_MutexUnlock(MUTEX mutex);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Helper functions -----------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViBoolean RsCoreInt_StrcpyMaxLen(ViChar* target, ViInt32 bufferSize, ViConstString source);
ViChar* RsCoreInt_ToLowerCase(ViChar* string);
ViChar* RsCoreInt_GetDataTypeString(RsCoreDataType dataType);
ViBoolean RsCoreInt_SplitTokenToNameAndValue(ViConstString token, ViConstString valueDelimiter, ViChar* tokenName, ViChar* tokenValue,
                                             ViInt32 buffersSize);
void RsCoreInt_FormatBufferForStringMessage(ViByte* data, ViInt32 dataSize, ViInt32 truncLen);
ViInt32 RsCoreInt_AppendDiscreteRangeTableList(RsCoreRangeTablePtr rangeTable, RsCoreDataType dataType, ViInt32 bufferSize, ViChar* buffer);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Global sessions list functions ---------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViInt32 RsCoreInt_SessionsListGetCount(void);
ViStatus RsCoreInt_SessionsListAddSession(ViRsrc resourceName, RsCoreSessionPtr* rsSessionNew);
ViStatus RsCoreInt_SessionsListDeleteSession(ViSession instrSession);
ViStatus RsCoreInt_SessionsListExists(ViRsrc resourceName, RsCoreSessionPtr* rsSession);
ViStatus RsCoreInt_SessionsListGetUniqueHandle(ViPSession instrSession);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Global error info ----------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_SetGlobalErrorInfo(ViStatus primaryError, ViStatus secondaryError, ViConstString errorElab);
ViStatus RsCoreInt_GetGlobalErrorInfo(ViStatus* primaryError, ViStatus* secondaryError, ViChar* errorElab);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Session error info ---------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_SetErrorInfoAttr(ViSession instrSession,
                                    RsCoreAttributePtr attr,
                                    ViBoolean overWrite,
                                    ViStatus primaryError,
                                    ViStatus secondaryError,
                                    ViConstString errorElab);

ViStatus RsCoreInt_CheckErrorElaboration(ViSession instrSession, ViConstString pattern, ViBoolean* patternFound);

ViStatus RsCoreInt_StatusDescription(ViStatus statusCode,
                                     ViInt32 bufferSize,
                                     ViChar* message);

ViStatus RsCoreInt_CheckStatusAfterOpcSync(ViSession instrSession, ViStatus errorBefore);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL New Session creation -------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_ParseOptionsBefore(ViConstString optionsString, ViPBoolean simulate, ViPBoolean reuseSession);
ViStatus RsCoreInt_InitOrReuseSession(ViRsrc resourceName, RsCoreAttributePtr* attrList, ViBoolean simulate, ViBoolean reuseSession,
                                      ViPSession instrSessionNew);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Attribute-related functions ------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViChar* RsCoreInt_GetAttrInfoString(RsCoreAttributePtr attr, ViInt32 bufferSize, ViChar* attributeInfo);
ViStatus RsCoreInt_CastDataType(ViSession instrSession, RsCoreDataType sourceType, void* sourceValue, RsCoreDataType targetType, void* targetValue);
ViStatus RsCoreInt_GetAttributePtr(ViSession instrSession, ViAttr attributeId, RsCoreAttributePtr* attrOut);
ViStatus RsCoreInt_GetAttrP2Value(RsCoreAttributePtr attr, void* value, ViInt32 bufSize);
ViStatus RsCoreInt_GetAttrViStringLen(RsCoreAttributePtr attr, ViInt32* stringLength, ViInt32* allocatedBufferSize);
ViStatus RsCoreInt_SetAttribute(ViSession instrSession,
                                ViConstString repCapName,
                                ViAttr attributeId,
                                ViInt32 optionFlags,
								RsCoreDataType valueDataType,
                                void*valueIn);

ViStatus RsCoreInt_GetAttribute(ViSession instrSession,
                                ViConstString repCapName,
                                ViAttr attributeId,
                                ViInt32 optionFlags,
								RsCoreDataType valueDataType,
                                ViInt32 bufSize,
                                void* valueOut);

ViStatus RsCoreInt_CheckAttr(ViSession instrSession,
                             ViConstString repCapName,
                             ViAttr attributeId,
                             ViInt32 optionFlags,
                             void* value);

ViStatus RsCoreInt_CheckInstrumentModel(ViSession instrSession, ViConstString modelsList, ViBoolean *pass, ViBoolean generateError);

ViStatus RsCoreInt_CheckAttrInstrModelAndOptions(ViSession instrSession,
                                                 RsCoreAttributePtr attr);

ViStatus RsCoreInt_QueryAttrCommandUni(ViSession instrSession, RsCoreAttributePtr attr,
                                       ViConstString query, ViBoolean unlimitedResponse, ViChar** response, ViInt32* byteCount);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL RepCaps --------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_DisposeRepCapTable(ViSession instrSession);
ViStatus RsCoreInt_FindRepCap(ViSession instrSession, RsCoreRepCapPtr customRepCapTable, ViConstString repCapNameId, RsCoreRepCapPtr* repCap);
ViStatus RsCoreInt_GetRepCapCount(ViSession instrSession, RsCoreRepCapPtr repCap, ViInt32* count);
ViStatus RsCoreInt_FindRepCapName(ViSession instrSession, RsCoreRepCapPtr repCap,
                                  ViString repCapName, ViBoolean errorIfNotFound, ViInt32* foundIdx,
                                  ViInt32* tokenOffset);
ViStatus RsCoreInt_GetRepCapNameAtIndex(ViSession instrSession, RsCoreRepCapPtr repCap, ViInt32 idx, ViInt32 bufferSize, ViChar* repCapName,
                                        ViInt32* tokenOffset);
ViStatus RsCoreInt_GetRepCapCmdAtIndex(ViSession instrSession, RsCoreRepCapPtr repCap, ViInt32 idx, ViInt32 bufferSize, ViChar* repCapCmd,
                                       ViInt32* tokenOffset);
ViStatus RsCoreInt_ValidateAttrForRepCap(ViSession instrSession,
                                         RsCoreRepCapPtr repCap,
                                         RsCoreAttributePtr attr);
ViStatus RsCoreInt_GetAttrRepCapNameIds(ViSession instrSession,
                                        RsCoreAttributePtr attr,
                                        ViInt32 bufferSize,
                                        ViChar* repCapNameIds);


/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Range Table Functions ------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_FindDiscreteRangeTableCmdString(ViSession instrSession,
                                                   RsCoreRangeTablePtr rangeTable,
                                                   RsCoreDataType dataType,
                                                   ViReal64 numericValue,
                                                   ViReal64 precision,
                                                   ViChar** p2cmdString);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL VISA IO Helpers ------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
void RsCoreInt_PollingDelay(ViReal64 elapsedTimeMs, ViInt32 opcWaitMode);
ViStatus RsCoreInt_GetVisaInfo(ViSession instrSession, ViInt32 bufferSize, ViChar* visaInfo);
ViStatus RsCoreInt_GetSessionType(ViSession instrSession, RsCoreSessionType* sessionType);
ViStatus RsCoreInt_SpecifyVisaError(ViSession instrSession, ViStatus error, ViConstString context, ViConstString sentCmd);
ViStatus RsCoreInt_ClearBeforeRead(ViSession instrSession);
ViStatus RsCoreInt_QueryClearEsr(ViSession instrSession);
ViStatus RsCoreInt_ResolveOpcTimeout(ViSession instrSession, ViInt32* opcTimeoutMs);
ViStatus RsCoreInt_ParseOptionsString(ViSession instrSession, ViChar* instrumentOptionsString, ViInt32 optionsParsingMode);
ViStatus RsCoreInt_GetStatusByte(ViSession instrSession, ViInt32* stb, ViBoolean blockTmoSettings);
ViStatus RsCoreInt_WaitForOpcSTBpoll(ViSession instrSession, ViInt32 timeoutMs);
ViStatus RsCoreInt_WaitForOpcSRQ(ViSession instrSession, ViInt32 timeoutMs);
ViStatus RsCoreInt_FlushData(ViSession instrSession, ViBoolean totalFlush);
ViStatus RsCoreInt_ParseBinaryDataHeader(ViSession instrSession, ViBoolean errorIfNotBinDataBlock, ViBoolean* isBinDataBlock, ViInt64* length,
                                         ViChar* dataPartRead);
ViStatus RsCoreInt_SendBinDataHeader(ViSession instrSession, ViConstString command, ViInt64 dataSize);
ViStatus RsCoreInt_GenerateOpcTimeoutError(ViSession instrSession, ViStatus error, ViInt32 opcTimeoutMs);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL VISA IO R/W ----------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_Read(ViSession instrSession,
                        ViInt32 bufferSize,
                        ViByte* readBuffer,
                        ViInt32* bytesRead);
ViStatus RsCoreInt_ReadDataKnownLength(ViSession instrSession, ViInt64 length, ViByte* buffer);
ViStatus RsCoreInt_ReadDataKnownLengthToFile(ViSession instrSession, ViInt64 length, FILE* file, ViInt64* bytesWritten);
ViStatus RsCoreInt_ReadBinaryDataBlock(ViSession instrSession,
                                       ViBoolean errorIfNotBinDataBlock,
                                       ViBoolean* isBinDataBlock,
                                       ViByte** outputBuffer,
                                       ViInt64* byteCount);

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL VISA IO R/W with OPC sync --------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/
ViStatus RsCoreInt_WriteWithOpc(ViSession instrSession, ViConstString command,
                                ViBoolean allowCheckStatus, ViInt32 opcTimeoutMs);

ViStatus RsCoreInt_QueryViStringWithOpc(ViSession instrSession, ViConstString query, ViBoolean allowCheckStatus,
                                        ViInt32 opcTimeoutMs, ViInt32 bufferSize, ViChar* responseString);

ViStatus RsCoreInt_QueryUnknownLengthWithOpc(ViSession instrSession, ViConstString query, ViBoolean allowCheckStatus,
                                             ViInt32 opcTimeoutMs, ViByte** response, ViInt64* byteCount);

ViStatus RsCoreInt_QueryBinaryDataBlockWithOpc(ViSession instrSession, ViConstString query,
                                               ViBoolean errorIfNotBinDataBlock, ViBoolean* isBinDataBlock,
                                               ViBoolean allowCheckStatus,
                                               ViInt32 opcTimeoutMs, ViByte** outBinDataBlock, ViInt64* blockSize);

/*****************************************************************************************************/
/*  Resource managing
******************************************************************************************************/
#define CACHE_USED_ATTRS
#define MAX_NUMBER_OF_SESSIONS 50

static RsCoreSessionPtr gSessionsList[MAX_NUMBER_OF_SESSIONS];
static ViInt32 gSessionsCount = -1; // Tracks the number of active sessions. -1 means not initialized yet

// The last global error that occured. Used in RsCore_GetError in case the instrSession is invalid
static ViStatus gPrimaryError;
static ViStatus gSecondaryError;
static ViChar* gErrorElaboration = NULL;
static MUTEX gMutex = NULL;

/*****************************************************************************************************/
/*  Internal MACROS
******************************************************************************************************/

#define attrHasFlag(flag) ((attr->flags & (flag)) > 0)

#define attrHasNotFlag(flag) ((attr->flags & (flag)) == 0)

// viCheckErr(status)
// Assign status to the error variable.
// If status is negative, pass it to Rs_SetErrorInfo
// and jump to the Error label.
#ifndef viCheckErr
#define viCheckErr(fCall)	 error = (fCall); \
								 if (error < 0) { (void)RsCore_SetErrorInfo(instrSession, VI_TRUE, error, 0, ""); goto Error;}
#endif

/*****************************************************************************************************/
/*  Multi-thread locks
*******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_MutexLock
    Initializes and Locks the mutex
	If the entered MUTEX is NULL, the function initializes a new mutex
	and gives its ownership to the current thread
******************************************************************************************************/
MUTEX RsCoreInt_MutexLock(MUTEX mutex)
{
#if defined(ANSI)
	//static double msDivider = (double)CLOCKS_PER_SEC / (double)1000.0;
	//clock_t start, elapsed_time_ms;
	DWORD dwWaitResult;

	if (mutex == NULL)
	{
		// Create and own the mutex
		mutex = CreateMutex(NULL, TRUE, NULL);
		// printf("\nThread %lu created mutex, obtained access ...", GetCurrentThreadId());
		if (mutex == NULL)
		{
			//printf("CreateMutex error: %lu\n", GetLastError());
			return NULL;
		}
	}
	else
	{
		//printf("\nThread %d obtaining access ...", GetCurrentThreadId());
		//start = clock();
		dwWaitResult = WaitForSingleObject(mutex, INFINITE);
		if (dwWaitResult == WAIT_ABANDONED_0)
		{
			//printf("\nThread %lu ERROR obtaining access, MUTEX was abandoned, creating new MUTEX", GetCurrentThreadId());
			mutex = CreateMutex(NULL, TRUE, NULL);
		}
		else if (dwWaitResult != WAIT_OBJECT_0)
		{
			//printf("\nThread %lu ERROR obtaining access ... ", GetCurrentThreadId());
			return mutex;
		}
		else
		{
			//elapsed_time_ms = (double)(clock() - start) / ms_divider;
			//printf("Thread %d got access after %d ms ...", GetCurrentThreadId(), elapsed_time_ms);
		}
	}
	return mutex;
#endif
#if defined(__linux__)
	pthread_mutex_lock(gMutex);
	return gMutex;
#endif
}

/*****************************************************************************************************/
/*  RsCoreInt_MutexUnlock
	Releases the ownership of the entered mutex
******************************************************************************************************/
void RsCoreInt_MutexUnlock(MUTEX mutex)
{
#if defined(ANSI)
	//printf("\nThread %d releasing access ... ", GetCurrentThreadId());
	if (ReleaseMutex(mutex) == 0)
	{
		//printf(" Thread %lu ERROR releasing access ...", GetCurrentThreadId());
	}
	else
	{
		//printf(" Thread %d released mutex successfully", GetCurrentThreadId());
	}

#endif

#if defined(__linux__)
    pthread_mutex_unlock(gMutex);
#endif
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Helper functions -----------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_StrcpyMaxLen
    Copies source string to a target string with maxBufferSize limitation.
    Works like strcpy if the source length is smaller than maxBufferSize-1.
    In this case, the the return value is FALSE
    If the source is longer than maxBufferSize-1, the function only copies maxBufferSize-1 bytes,
    terminates the target string with null-termination and returns TRUE
******************************************************************************************************/
ViBoolean RsCoreInt_StrcpyMaxLen(ViChar* target, ViInt32 bufferSize, ViConstString source)
{
	ViInt32 len = (ViInt32)strlen(source);

	if (bufferSize <= 0)
		return VI_TRUE;

	bufferSize--;
	if (len > bufferSize)
	{
		memcpy(target, source, (size_t)bufferSize);
		*(target + bufferSize) = 0;
		return VI_TRUE;
	}

	memcpy(target, source, (size_t)(len + 1));
	return VI_FALSE;
}

/*****************************************************************************************************/
/*  RsCoreInt_ToLowerCase
    Converts string in-place to lower case
******************************************************************************************************/
ViChar* RsCoreInt_ToLowerCase(ViChar* string)
{
	ViChar* p;

	for (p = string; *p; ++p)
	{
		if (*p > 'Z')
			continue;
		else if (*p < 'A')
			continue;
		else *p = (ViChar) (*p | 0x60);
	}

	return string;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetDataTypeString
	Returns pointer to a string-representation of the entered data type
	If the data type is not recognised, the return value points to string "RS_VAL_UNKNOWN_TYPE"
******************************************************************************************************/
ViChar* RsCoreInt_GetDataTypeString(RsCoreDataType dataType)
{
	ViInt32 i = 0, found = 0;

	while (gDataTypes[i] != 0)
	{
		if (dataType == gDataTypes[i])
		{
			found = i;
			break;
		}

		i++;
	}

	return gDataTypeStrings[found];
}

/*****************************************************************************************************/
/*  RsCoreInt_SplitTokenToNameAndValue
    Splits and trims a token to name and value and:
    - Trims both strings for white characters and paired quotation marks
    - If the valueDelimiter was not found, the function returns VI_FALSE,
        the tokenName contains the whole token, and the tokenValue is empty string.
    - If the valueDelimiter was found, the tokenName and tokenValue are returned.
    - The result VI_TRUE requires the delimiter to be found and the tokenName to be non-empty.
******************************************************************************************************/
ViBoolean RsCoreInt_SplitTokenToNameAndValue(ViConstString token, ViConstString valueDelimiter, ViChar* tokenName, ViChar* tokenValue,
                                             ViInt32 buffersSize)
{
	ViChar* index;
	ViInt32 len;

	if (valueDelimiter == NULL || token == NULL || tokenName == NULL || tokenValue == NULL || buffersSize < 1)
		return VI_FALSE;

	RsCoreInt_StrcpyMaxLen(tokenName, buffersSize, "");
	RsCoreInt_StrcpyMaxLen(tokenValue, buffersSize, "");

	index = (ViChar*)strstr(token, valueDelimiter);
	if (!index)
	{
		RsCoreInt_StrcpyMaxLen(tokenName, buffersSize, token);
		RsCore_TrimString(tokenName, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		return VI_FALSE;
	}

	len = (ViInt32)(index - token) + 1;
	if (len >= buffersSize)
		len = buffersSize;

	RsCoreInt_StrcpyMaxLen(tokenName, len, token);
	RsCore_TrimString(tokenName, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
	RsCoreInt_StrcpyMaxLen(tokenValue, buffersSize, index + strlen(valueDelimiter));
	RsCore_TrimString(tokenValue, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);

	return strlen(tokenName) > 0;
}

/*****************************************************************************************************/
/*  RsCoreInt_FormatBufferForStringMessage
    Formats any data into a message with only printable characters
    Non-printable characters are replaced by '*' and LF is replaced by '.'
    If the dataSize is bigger than truncLen, the data is shortened to truncLen and added '...' at the end
******************************************************************************************************/
void RsCoreInt_FormatBufferForStringMessage(ViByte* data, ViInt32 dataSize, ViInt32 truncLen)
{
	ViInt32 i;
	ViInt32 dataLen;

	if (dataSize <= 0)
		return;

	dataLen = dataSize - 1;
	data[dataLen] = 0;

	if (truncLen < dataLen)
	{
		// Replace the last 3 bytes of the string, but only if the entire length is bigger than 10 bytes
		if (truncLen >= 10)
		{
			strncpy((ViChar*)data + truncLen - 3, "...", 4);
		}

		dataLen = truncLen;
	}

	data[dataLen] = 0;

	for (i = 0; i < dataLen; i++)
	{
		if (data[i] == '\n')
		{
			data[i] = '.';
			continue;
		}

		if (!isprint(data[i]))
		{
			data[i] = '*';
		}
	}
}

/*****************************************************************************************************/
/*  RsCoreInt_AppendDiscreteRangeTableList
    Appends range table list of command values + strings
    For data type RS_VAL_INT32 and RS_VAL_REAL64, the format is e.g.:
    0.0: [ INT ]
    1.0: [ EXT ]
    For RS_VAL_STRING the format is e.g.: ' INT, EXT '
    The function returns number of range table elements
******************************************************************************************************/
ViInt32 RsCoreInt_AppendDiscreteRangeTableList(RsCoreRangeTablePtr rangeTable, RsCoreDataType dataType, ViInt32 bufferSize, ViChar* buffer)
{
	// Create a csv-list of discrete values
	ViInt32 i = 0;
	ViChar element[RS_MAX_MESSAGE_LEN];

	while (rangeTable->rangeValues[i].cmdValue != RS_RANGE_TABLE_END_VALUE && i < 99999)
	{
		if (dataType == RS_VAL_INT32)
		{
			snprintf(element, RS_MAX_MESSAGE_LEN, _PERCD": [ %s ]",
			         (ViInt32)rangeTable->rangeValues[i].discreteOrMinValue,
			         rangeTable->rangeValues[i].cmdString);
			RsCore_AppendToCsvString(buffer, (ViChar*)"\n", bufferSize, element);
		}
		else if (dataType == RS_VAL_REAL64)
		{
			snprintf(element, RS_MAX_MESSAGE_LEN, "%.12lG: [ %s ]",
			         rangeTable->rangeValues[i].discreteOrMinValue,
			         rangeTable->rangeValues[i].cmdString);
			RsCore_AppendToCsvString(buffer, (ViChar*)"\n", bufferSize, element);
		}
		else if (dataType == RS_VAL_STRING)
		{
			if (i == 0)
				// opening quotation mark
				RsCore_StrcatMaxLen(buffer, bufferSize, "' ", rangeTable->rangeValues[i].cmdString, NULL);
			else
				RsCore_StrcatMaxLen(buffer, bufferSize, ", ", rangeTable->rangeValues[i].cmdString, NULL);
		}

		i++;
	}

	if (dataType == RS_VAL_STRING)
	{
		//Append closing quotation mark
		RsCore_StrcatMaxLen(buffer, bufferSize, " '", NULL, NULL);
	}

	return i;
}


/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Global sessions list functions ---------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_SessionsListGetCount
    Returns current number of sessions in the sessions list
******************************************************************************************************/
ViInt32 RsCoreInt_SessionsListGetCount(void)
{
	return gSessionsCount;
}

/*****************************************************************************************************/
/*  RsCoreInt_SessionsListAddSession
    Internal function that initializes new RsCoreSession structure
    and adds it into the gSessionsList
******************************************************************************************************/
ViStatus RsCoreInt_SessionsListAddSession(ViRsrc resourceName, RsCoreSessionPtr* rsSessionNew)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 idx;
	ViInt32 count;
	ViInt32 emptySlotIdx = -1;
	RsCoreSessionPtr rsSession = NULL;
	ViSession instrSession = 0;

	*rsSessionNew = NULL;

	// First, check if the gSessionsList has already been initialized
	if (gSessionsCount == -1)
	{
		memset(gSessionsList, 0, MAX_NUMBER_OF_SESSIONS * sizeof(RsCoreSessionPtr));
		gSessionsCount = 0;
	}

	// Update the number of current valid sessions (count)
	// Find the first empty spot to occupy (emptySlotIdx). The empty spots can be scattered.
	// For simulation purposes create a unique instrSession number
	count = 0;
	for (idx = 0; idx < MAX_NUMBER_OF_SESSIONS; idx++)
	{
		rsSession = gSessionsList[idx];
		if (rsSession != NULL)
		{
			count++;
		}
		else
		{
			// remember first empty slot index
			if (emptySlotIdx == -1)
				emptySlotIdx = idx;
		}
	}

	gSessionsCount = count;
	if (gSessionsCount >= MAX_NUMBER_OF_SESSIONS)
	{
		viCheckErrElab(RS_ERROR_MAX_SESSION_COUNT_REACHED,
			"Maximum number of sessions (50) reached. "
			"Make sure you are closing the opened sessions. "
			"Alternatively, use the InitWithOptions optionString parameter "
			"to enter 'ReuseSession = 1'");
	}

	if ((rsSession = (RsCoreSessionPtr)calloc(1, sizeof(RsCoreSession))) == NULL)
	{
		checkErr(RS_ERROR_ALLOC);
	}

	gSessionsList[emptySlotIdx] = rsSession;
	rsSession->sessionsListIndex = emptySlotIdx;
	gSessionsCount++;

	*rsSessionNew = rsSession;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_SessionsListDeleteSession
    Removes and deallocates a session from the gSessionsList
******************************************************************************************************/
ViStatus RsCoreInt_SessionsListDeleteSession(ViSession instrSession)
{
	ViInt32 idx;

	for (idx = 0; idx < MAX_NUMBER_OF_SESSIONS; idx++)
	{
		if (gSessionsList[idx] != NULL && gSessionsList[idx]->io == instrSession)
		{
			free(gSessionsList[idx]);
			gSessionsList[idx] = NULL;

			if (gSessionsCount > 0)
				gSessionsCount--;

			break;
		}
	}

	return VI_SUCCESS;
}

/*****************************************************************************************************/
/*  RsCoreInt_SessionsListExists
    Internal function that returns pointer to RsCoreSessionPtr with the entered resourceName
    If such session is not found in the global sessions list, the rsSession is NULL
******************************************************************************************************/
ViStatus RsCoreInt_SessionsListExists(ViRsrc resourceName, RsCoreSessionPtr* rsSession)
{
	ViInt32 idx;

	if (gSessionsCount == -1)
	{
		*rsSession = NULL;
		goto Error;
	}

	for (idx = 0; idx < MAX_NUMBER_OF_SESSIONS; idx++)
	{
		if (gSessionsList[idx] != NULL)
		{
			// Valid session
			if (strcmp(resourceName, gSessionsList[idx]->resourceName) == 0)
			{
				*rsSession = gSessionsList[idx];
				goto Error;
			}
		}
	}

	*rsSession = NULL;

Error:
	return VI_SUCCESS;
}

/*****************************************************************************************************/
/*  RsCoreInt_SessionsListGetUniqueHandle
    Internal function that returns a unique instrSession for simulation purposes
******************************************************************************************************/
ViStatus RsCoreInt_SessionsListGetUniqueHandle(ViPSession instrSession)
{
	ViInt32 idx;
	ViSession highestInstrSession = 0;

	// First, check if the gSessionsList has already been initialized
	if (gSessionsCount == -1)
	{
		*instrSession = 1 | 0xF0000000;
	}

	for (idx = 0; idx < MAX_NUMBER_OF_SESSIONS; idx++)
		if (gSessionsList[idx] != NULL) // valid session
			if (highestInstrSession < gSessionsList[idx]->io) highestInstrSession = gSessionsList[idx]->io;

	*instrSession = (highestInstrSession + 1) | 0xF0000000;

	return VI_SUCCESS;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Global error info ----------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_SetGlobalErrorInfo
    Sets the global error information
******************************************************************************************************/
ViStatus RsCoreInt_SetGlobalErrorInfo(ViStatus primaryError, ViStatus secondaryError, ViConstString errorElab)
{
	ViStatus error = VI_SUCCESS;

	gPrimaryError = primaryError;
	gSecondaryError = secondaryError;

	if (gErrorElaboration == NULL)
	checkAlloc(gErrorElaboration = (ViChar*)calloc(RS_MAX_MESSAGE_BUF_SIZE, 1));

	RsCoreInt_StrcpyMaxLen(gErrorElaboration, RS_MAX_MESSAGE_BUF_SIZE, errorElab);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetGlobalErrorInfo
    Gets the global error information. errorElab buffer needs maximum of RS_MAX_MESSAGE_BUF_SIZE
******************************************************************************************************/
ViStatus RsCoreInt_GetGlobalErrorInfo(ViStatus* primaryError, ViStatus* secondaryError, ViChar* errorElab)
{
	ViStatus error = VI_SUCCESS;

	*primaryError = gPrimaryError;
	*secondaryError = gSecondaryError;

	if (gErrorElaboration != NULL)
		RsCoreInt_StrcpyMaxLen(errorElab, RS_MAX_MESSAGE_BUF_SIZE, gErrorElaboration);
	else
		RsCoreInt_StrcpyMaxLen(errorElab, RS_MAX_MESSAGE_BUF_SIZE, "");

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Session error info ---------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_SetErrorInfoAttr
    Same as RsCore_SetErrorInfo, but adds the attribute info at the beginning
******************************************************************************************************/
ViStatus RsCoreInt_SetErrorInfoAttr(ViSession instrSession,
                                    RsCoreAttributePtr attr,
                                    ViBoolean overWrite,
                                    ViStatus primaryError,
                                    ViStatus secondaryError,
                                    ViConstString errorElab)
{
	ViStatus error = VI_SUCCESS;
	ViChar message[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* ptr;

	ptr = RsCoreInt_GetAttrInfoString(attr, RS_MAX_MESSAGE_BUF_SIZE, message);
	snprintf(ptr, RS_MAX_MESSAGE_BUF_SIZE - (ptr - message) - 1, " - %s", errorElab);
	error = RsCore_SetErrorInfo(instrSession, overWrite, primaryError, secondaryError, message);

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_CheckErrorElaboration
    Check the current error elaboration for the entered pattern
    The function is optimized for speed by directly the session's errorElaboration field
******************************************************************************************************/
ViStatus RsCoreInt_CheckErrorElaboration(ViSession instrSession, ViConstString pattern, ViBoolean* patternFound)
{
	RsCoreSessionPtr rsSession;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	*patternFound = (strstr(rsSession->errorElaboration, pattern) != NULL) ? VI_TRUE : VI_FALSE;

	return VI_SUCCESS;
}

/*****************************************************************************************************/
/*  RsCoreInt_StatusDescription
    Converts a error code to a human-readable string.
    The function first tries to find the error code in the RsCore errors database.
    If that does not succeed, it calls VISA's viStatusDesc()
    If even that does not succeed, it reports VI_WARN_UNKNOWN_STATUS and ""Unknown status value" in the message.
******************************************************************************************************/
ViStatus RsCoreInt_StatusDescription(ViStatus statusCode, ViInt32 bufferSize, ViChar* message)
{
	ViStatus error = VI_SUCCESS;
	ViInt16 idx;

	static RsCoreStringValueEntry statusDescArray[] = {

		{VI_SUCCESS, "The call was successful."},
		{RS_ERROR_CANNOT_RECOVER, "Unrecoverable failure."},
		{RS_ERROR_INSTRUMENT_STATUS, "Instrument error detected."},
		{RS_ERROR_CANNOT_OPEN_FILE, "File could not be opened."},
		{RS_ERROR_READING_FILE, "File is being read."},
		{RS_ERROR_WRITING_FILE, "File is being modified."},

		{RS_ERROR_DRIVER_MODULE_NOT_FOUND, "Driver module file not found."},
		{RS_ERROR_CANNOT_OPEN_DRIVER_MODULE, "Cannot open driver module file for reading."},
		{RS_ERROR_INVALID_DRIVER_MODULE, "Driver module has invalid file format or contains invalid data."},
		{RS_ERROR_UNDEFINED_REFERENCES, "Driver module contains undefined references."},
		{RS_ERROR_FUNCTION_NOT_FOUND, "Cannot find function in driver module."},
		{RS_ERROR_LOADING_DRIVER_MODULE, "Failure loading driver module."},

		{RS_ERROR_INVALID_PATHNAME, "The path name is invalid."},
		{RS_ERROR_INVALID_ATTRIBUTE, "Attribute ID not recognized."},
		{RS_ERROR_RS_ATTR_NOT_WRITABLE, "Attribute is read-only."},
		{RS_ERROR_RS_ATTR_NOT_READABLE, "Attribute is write-only."},
		{RS_ERROR_INVALID_PARAMETER, "Invalid parameter."},
		{RS_ERROR_INVALID_VALUE, "Invalid value for parameter or property."},
		{RS_ERROR_FUNCTION_NOT_SUPPORTED, "Function or method not supported."},
		{RS_ERROR_ATTRIBUTE_NOT_SUPPORTED, "Attribute or property not supported."},
		{RS_ERROR_VALUE_NOT_SUPPORTED, "The enumeration value for the parameter is not supported."},
		{RS_ERROR_SESSION_TYPE_NOT_SUPPORTED, "The selected session type is not supported."},
		{RS_ERROR_INVALID_TYPE, "Invalid Type."},
		{RS_ERROR_TYPES_DO_NOT_MATCH, "The attribute and function parameter types do not match."},
		{RS_ERROR_MULTIPLE_DEFERRED_SETTING, "The specified attribute already has a value waiting to be updated."},
		{RS_ERROR_ITEM_ALREADY_EXISTS, "The specified item already exists."},
		{RS_ERROR_INVALID_CONFIGURATION, "Not a valid configuration."},
		{RS_ERROR_VALUE_NOT_AVAILABLE, "The requested item or value does not exist or is not available."},
		{RS_ERROR_ATTRIBUTE_VALUE_NOT_KNOWN, "The requested attribute value not known and cannot be determined."},
		{RS_ERROR_NO_RANGE_TABLE, "There is no range table for this attribute."},
		{RS_ERROR_INVALID_RANGE_TABLE, "The range table is invalid."},
		{RS_ERROR_NOT_INITIALIZED, "A connection to the instrument has not been initialized."},
		{RS_ERROR_NON_INTERCHANGEABLE_BEHAVIOR, "The class instrument driver has encountered underspecified instrument configurations that limit interchangeability."},
		{RS_ERROR_NO_CHANNEL_TABLE, "No channel table has been built for the session. The instrument driver must call RsCore_BuildChannelTable in its RsInit function."},
		{RS_ERROR_UNKNOWN_CHANNEL_NAME, "Channel or repeated capability name specified is not valid for the instrument."},
		{RS_ERROR_SYS_RSRC_ALLOC, "Unable to allocate system resource."},
		{RS_ERROR_ACCESS_DENIED, "Permission to access file was denied."},
		{RS_ERROR_TOO_MANY_OPEN_FILES, "Too many files opened."},
		{RS_ERROR_UNABLE_TO_CREATE_TEMP_FILE, "Unable to create temporary file in target directory."},
		{RS_ERROR_NO_UNUSED_TEMP_FILENAMES, "All temporary file-names already used in target directory."},
		{RS_ERROR_DISK_FULL, "Disk is full."},
		{RS_ERROR_CONFIG_FILE_NOT_FOUND, "Configuration file was not found on disk."},
		{RS_ERROR_CANNOT_OPEN_CONFIG_FILE, "Cannot open configuration file."},
		{RS_ERROR_ERROR_READING_CONFIG_FILE, "Error reading configuration file."},
		{RS_ERROR_BAD_INTEGER_IN_CONFIG_FILE, "Invalid ViInt32 value in configuration file."},
		{RS_ERROR_BAD_DOUBLE_IN_CONFIG_FILE, "Invalid ViReal64 value in configuration file."},
		{RS_ERROR_BAD_BOOLEAN_IN_CONFIG_FILE, "Invalid ViBoolean value in configuration file."},
		{RS_ERROR_CONFIG_ENTRY_NOT_FOUND, "Entry missing from configuration file."},
		{RS_ERROR_DRIVER_DLL_INIT_FAILED, "Initialization failed in driver DLL."},
		{RS_ERROR_DRIVER_UNRESOLVED_SYMBOL, "Driver module could not be loaded because of an unresolved external reference."},
		{RS_ERROR_CANNOT_FIND_CVI_RTE, "Cannot find CVI Run-Time Engine."},
		{RS_ERROR_CANNOT_OPEN_CVI_RTE, "Cannot open CVI Run-Time Engine."},
		{RS_ERROR_CVI_RTE_INVALID_FORMAT, "CVI Run-Time Engine has invalid format."},
		{RS_ERROR_CVI_RTE_MISSING_FUNCTION, "CVI Run-Time Engine is missing one or more required functions."},
		{RS_ERROR_CVI_RTE_INIT_FAILED, "CVI Run-Time Engine initialization failed, probably because of insufficient memory."},
		{RS_ERROR_CVI_RTE_UNRESOLVED_SYMBOL, "CVI Run-Time Engine could not be loaded because of an unresolved external reference."},
		{RS_ERROR_LOADING_CVI_RTE, "Failure loading CVI Run-Time Engine."},
		{RS_ERROR_CANNOT_OPEN_DLL_FOR_EXPORTS, "Cannot open DLL to read exports."},
		{RS_ERROR_DLL_CORRUPTED, "DLL file is corrupt."},
		{RS_ERROR_NO_DLL_EXPORT_TABLE, "No export table in DLL."},
		{RS_ERROR_UNKNOWN_DEFAULT_SETUP_ATTR, "Unknown attribute name for initial setting in configuration file."},
		{RS_ERROR_INVALID_DEFAULT_SETUP_VAL, "Invalid attribute value for initial setting in configuration file."},
		{RS_ERROR_UNKNOWN_MEMORY_PTR, "Memory pointer specified is not known."},
		{RS_ERROR_EMPTY_CHANNEL_LIST, "Unable to find any channel or repeated capability strings."},
		{RS_ERROR_DUPLICATE_CHANNEL_STRING, "The channel or repeated capability list contains two instances of the same name."},
		{RS_ERROR_DUPLICATE_VIRT_CHAN_NAME, "The VirtualChannelNames item in the configuration file contains a duplicate virtual channel name."},
		{RS_ERROR_MISSING_VIRT_CHAN_NAME, "The VirtualChannelNames item in the configuration file contains an entry without a virtual channel name (nothing before the '=')."},
		{RS_ERROR_BAD_VIRT_CHAN_NAME, "The VirtualChannelNames item in the configuration file contains an invalid virtual channel name. Channel names must contain only alphanumerics, underscores, or an exclamation point."},
		{RS_ERROR_UNASSIGNED_VIRT_CHAN_NAME, "The VirtualChannelNames item in the configuration file contains a virtual channel name without an assigned channel string (i.e., nothing after '=')."},
		{RS_ERROR_BAD_VIRT_CHAN_ASSIGNMENT, "The VirtualChannelNames item in the configuration file contains a virtual channel name that is assigned to an unknown or invalid channel string."},
		{RS_ERROR_CHANNEL_NAME_REQUIRED, "Channel or repeated capability name required."},
		{RS_ERROR_CHANNEL_NAME_NOT_ALLOWED, "The channel or repeated capability name is not allowed."},
		{RS_ERROR_ATTR_NOT_VALID_FOR_CHANNEL, "The attribute is not valid for the specified channel or repeated capability."},
		{RS_ERROR_ATTR_MUST_BE_CHANNEL_BASED, "This operation requires a channel or repeated capability based attribute. The specified attribute is not channel or repeated capability based."},
		{RS_ERROR_CHANNEL_ALREADY_EXCLUDED, "The channel has already been excluded for the specified attribute and cannot be re-included."},
		{RS_ERROR_MISSING_OPTION_NAME, "The option string parameter contains an entry without a name."},
		{RS_ERROR_MISSING_OPTION_VALUE, "The option string parameter contains an entry without a value."},
		{RS_ERROR_BAD_OPTION_NAME, "The option string parameter contains an entry with an unknown option name."},
		{RS_ERROR_BAD_OPTION_VALUE, "The option string parameter contains an entry with an unknown option value."},
		{RS_ERROR_NOT_CREATED_BY_CLASS, "This operation is valid only on a session created by a class driver."},
		{RS_ERROR_RS_INI_IS_RESERVED, "You cannot create a configuration file named \"rs.ini\". That filename is reserved."},
		{RS_ERROR_DUP_RUNTIME_CONFIG_ENTRY, "There already is an entry of the same name in the run-time configuration."},
		{RS_ERROR_INDEX_IS_ONE_BASED, "The index parameter is one-based. You must pass a number greater than or equal to 1."},
		{RS_ERROR_INDEX_IS_TOO_HIGH, "The index exceeds the number of items available."},
		{RS_ERROR_ATTR_NOT_CACHEABLE, "You cannot set the cache for an attribute that has the RS_VAL_NEVER_CACHE flag."},
		{RS_ERROR_ADDR_ATTRS_MUST_BE_HIDDEN, "An instrument driver cannot export a ViAddr attribute to the end-user. Use the RS_VAL_HIDDEN flag macro to make it a private attribute."},
		{RS_ERROR_BAD_CHANNEL_NAME, "Channel or repeated capability strings must contain only alphanumerics, underscores, or an exclamation point."},
		{RS_ERROR_BAD_PREFIX_IN_CONFIG_FILE, "The Prefix item in the configuration file does not match the specific driver's prefix."},
		{RS_ERROR_OUT_OF_MEMORY, "The necessary memory could not be allocated."},
		{RS_ERROR_OPERATION_PENDING, "Operation in progress."},
		{RS_ERROR_NULL_POINTER, "NULL pointer passed for parameter or property."},
		{RS_ERROR_UNEXPECTED_RESPONSE, "Unexpected response from the instrument."},
		{RS_ERROR_FILE_NOT_FOUND, "File not found."},
		{RS_ERROR_INVALID_FILE_FORMAT, "The file format is invalid."},
		{RS_ERROR_STATUS_NOT_AVAILABLE, "The instrument status is not available."},
		{RS_ERROR_ID_QUERY_FAILED, "Instrument ID Query failed."},
		{RS_ERROR_RESET_FAILED, "Instrument reset failed."},
		{RS_ERROR_RESOURCE_UNKNOWN, "Insufficient location information or resource not present in the system."},
		{RS_ERROR_ALREADY_INITIALIZED, "The driver is already initialized."},
		{RS_ERROR_CANNOT_CHANGE_SIMULATION_STATE, "The simulation state cannot be changed."},
		{RS_ERROR_INVALID_NUMBER_OF_LEVELS_IN_SELECTOR, "Invalid number of levels in selector."},
		{RS_ERROR_INVALID_RANGE_IN_SELECTOR, "Invalid range in selector."},
		{RS_ERROR_UNKOWN_NAME_IN_SELECTOR, "Unknown name in selector."},
		{RS_ERROR_BADLY_FORMED_SELECTOR, "Badly-formed selector."},
		{RS_ERROR_UNKNOWN_PHYSICAL_IDENTIFIER, "Unknown physical selector."},

		{RS_ERROR_INVALID_SESSION_HANDLE, "The session handle is not valid."},
		{RS_ERROR_CANNOT_CREATE_LOCK, "The session handle is not valid."},
		{RS_ERROR_CANNOT_CREATE_THREAD_LOCAL, "Could not create thread local."},

		{RS_CONFIG_ERROR_DESERIALIZE_FAILED, "The specified configuration store file could not be deserialized."},
		{RS_CONFIG_ERROR_ALREADY_DESERIALIZED, "A deserialize was attempted after a previous deserialize had already succeeded."},
		{RS_CONFIG_ERROR_SERIALIZE_FAILED, "The specified configuration store file could not be serialized."},
		{RS_CONFIG_ERROR_SESSION_NOT_FOUND, "The session name or logical name could not be resolved to a session or driver session."},
		{RS_CONFIG_ERROR_NOT_IN_GLOBAL, "The item does not exist in the global collection."},
		{RS_CONFIG_ERROR_ALREADY_EXIST, "An entry with name already exists in the collection."},
		{RS_CONFIG_ERROR_MASTER_NOT_FOUND, "The registry entry for the master configuration store does not exist or the file could not be found."},
		{RS_CONFIG_ERROR_NOT_EXIST, "The item does not exist in the collection."},
		{RS_CONFIG_ERROR_INVALID_DATA_COMPONENT, "The data component is not a valid data component."},
		{RS_CONFIG_ERROR_LOCAL_REFERENCE_EXIST,	"The element cannot be removed from the global collection when it is referenced in the local collections."},
		{RS_CONFIG_ERROR_INVALID_HANDLE, "The specified handle is invalid or of an incorrect type."},
		{RS_CONFIG_ERROR_INVALID_PROPERTY_ID, "The specified property ID is not valid for this function."},

		{RS_ERROR_CANNOT_MODIFY_REPEATED_CAPABILITY_TABLE, "Repeated Capability lists cannot be modified after attributes have been added to them."},
		{RS_ERROR_CANNOT_RESTRICT_ATTRIBUTE_TWICE, "An attribute can only be restricted to a subset of a repeated capability once."},
		{RS_ERROR_REPEATED_CAPABILITY_ALREADY_EXISTS, "The repeated capability table cannot be built because it already exists."},
		{RS_ERROR_REPEATED_CAPABILITY_NOT_DEFINED, "The repeated capability has not been defined yet."},
		{RS_ERROR_INVALID_REPEATED_CAPABILITY_NAME, "The repeated capability name cannot be an empty or NULL string."},
		{RS_ERROR_CONFIG_SERVER_NOT_PRESENT, "The Config Server module is not present on the system."},
		{RS_WARN_UNKNOWN_INSTRUMENT_STATUS, "Instrument status not checked."},
		
		{VI_WARN_NSUP_ID_QUERY, "Identification query not supported."},
		{VI_WARN_NSUP_RESET, "Reset operation not supported."},
		{VI_WARN_NSUP_SELF_TEST, "Self test operation not supported."},
		{VI_WARN_NSUP_ERROR_QUERY, "Error query operation not supported."},
		{VI_WARN_NSUP_REV_QUERY, "Revision query not supported."},

		/* Common Instrument Driver Errors and Warnings */

		{VI_ERROR_PARAMETER1, "Parameter 1 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER2, "Parameter 2 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER3, "Parameter 3 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER4, "Parameter 4 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER5, "Parameter 5 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER6, "Parameter 6 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER7, "Parameter 7 out of range, or error trying to set it."},
		{VI_ERROR_PARAMETER8, "Parameter 8 out of range, or error trying to set it."},
		{VI_ERROR_FAIL_ID_QUERY, "Instrument failed the ID Query."},
		{VI_ERROR_INV_RESPONSE, "Invalid response from instrument."},

		{RS_ERROR_INSTRUMENT_OPTION, "Required instrument option(s) not present."},
		{RS_ERROR_INSTRUMENT_MODEL, "Required instrument model(s) not present."},
		{RS_ERROR_STRING_TOO_LONG, "The length of the string exceeded the maximum of RS_MAX_MESSAGE_BUF_SIZE (4096 bytes)"},
		{RS_ERROR_DATA_TOO_LONG, "The length of the data exceeded the allowed maximum"},
		{RS_ERROR_MAX_SESSION_COUNT_REACHED, "The maximum allowed number of active sessions (50) have been reached"},
		{RS_ERROR_ALLOC, "Allocation or re-allocation of memory resource failed"},

		{0, NULL}
	};

	// First search the internal code database

	message[0] = 0;
	for (idx = 0; statusDescArray[idx].string; idx++)
	{
		if (statusDescArray[idx].value == statusCode)
		{
			RsCoreInt_StrcpyMaxLen(message, bufferSize, statusDescArray[idx].string);
			return error;
		}
	}

	RsCoreInt_StrcpyMaxLen(message, bufferSize, "");
	// Call VISA code->message converter

	error = viStatusDesc(0, statusCode, message);
	if (strstr(message, "VI_ERROR_") == message)
	{
		ViChar* ptrMessageStart = strstr(message, ": ");
		// Remove e.g. VI_ERROR_xxx: from the beginning, it might, or might not be there
		if (ptrMessageStart)
		{
			memmove(message, ptrMessageStart + 2, strlen(ptrMessageStart + 3));
		}
	}

	if (error != VI_SUCCESS || (strstr(message, "Unknown status value") != NULL))
	{
		snprintf(message, bufferSize, "The status code passed to the operation could not be interpreted.");
		return VI_WARN_UNKNOWN_STATUS;
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_CheckStatusAfterOpcSync
    Enhancement for RsCore_CheckStatus for cases where RsCore_CheckStatus would
    overwrite the OPC timeout error
******************************************************************************************************/
ViStatus RsCoreInt_CheckStatusAfterOpcSync(ViSession instrSession, ViStatus errorBefore)
{
	ViStatus error = VI_SUCCESS;
	ViBoolean patternFound;

	(void)RsCoreInt_CheckErrorElaboration(instrSession, "Waiting for OPC resulted in timeout", &patternFound);
	if (patternFound != VI_TRUE)
	{
		checkErr(RsCore_CheckStatus(instrSession, errorBefore));
	}
	else
	{
		return errorBefore;
	}

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL New Session creation -------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_ParseOptionsBefore
    Parses the option string only for specific tokens - the ones needed before opening a session
    - Simulate
    - ReuseSession
******************************************************************************************************/
ViStatus RsCoreInt_ParseOptionsBefore(ViConstString optionsString, ViPBoolean simulate, ViPBoolean reuseSession)
{
	ViStatus error = VI_SUCCESS;
	ViChar token[RS_MAX_MESSAGE_LEN];
	ViChar tokenName[RS_MAX_MESSAGE_LEN];
	ViChar tokenValue[RS_MAX_MESSAGE_LEN];
	ViInt32 elementsCount;
	ViInt32 idx;

	*simulate = VI_FALSE;
	*reuseSession = VI_FALSE;

	if (isNullOrEmpty(optionsString))
		return error;

	elementsCount = RsCore_GetElementsCount(optionsString, ',');
	for (idx = 0; idx < elementsCount; idx++)
	{
		RsCore_GetTokenAtIndex(optionsString, ',', idx, token, RS_MAX_MESSAGE_LEN, NULL);
		if (!RsCoreInt_SplitTokenToNameAndValue(token, "=", tokenName, tokenValue, RS_MAX_MESSAGE_LEN))
			continue;

		RsCoreInt_ToLowerCase(tokenName);
		if (strcmp(tokenName, "simulate") == 0)
		{
			*simulate = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "reusesession") == 0)
		{
			*reuseSession = RsCore_Convert_String_To_Boolean(tokenValue);
		}
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_InitOrReuseSession
    Initializes a new VISA session, or if reuseSession is TRUE, it potentially uses the existing one with the same resourceName
    In case of a successful reuse, the function just returns the existing instrSession.
    The reused session type overrides the entered parameter 'simulate'.
******************************************************************************************************/
ViStatus RsCoreInt_InitOrReuseSession(ViRsrc resourceName, RsCoreAttributePtr* attrList, ViBoolean simulate, ViBoolean reuseSession,
                                      ViPSession instrSessionNew)
{
	ViStatus error = VI_SUCCESS;
	ViChar errorElab[RS_MAX_MESSAGE_LEN];
	RsCoreAttributePtr attr;
	ViSession instrSession, rmSession;
	ViInt32 attributesCount;
	ViInt32 dataSize;
	ViInt32 idx;
	RsCoreSessionPtr rsSession = NULL;

	gMutex = RsCoreInt_MutexLock(gMutex);

	if (reuseSession)
	{
		checkErr(RsCoreInt_SessionsListExists(resourceName, &rsSession));
		if (rsSession != NULL)
		{
			// reuse the existing session
			*instrSessionNew = rsSession->io;
			goto Error;
		}
	}

	if (!simulate)
	{
		instrSession = 0;
		error = viOpenDefaultRM(&rmSession);
		if (error < VI_SUCCESS)
		{
			snprintf(errorElab, RS_MAX_MESSAGE_LEN,
			         "Opening VISA Resource Manager: Entered resource name: '%s'"
			         " Make sure your VISA installation is correct.", resourceName);
			(void)RsCore_SetErrorInfo(instrSession, VI_TRUE, error, VI_SUCCESS, errorElab);
			goto Error;
		}

		error = viOpen(rmSession, resourceName, 0, 0, &instrSession);
		if (error == VI_ERROR_RSRC_NFOUND || error == VI_ERROR_INV_RSRC_NAME)
		{
			// Special handling of the most-common error from the viOpen
			// Unify those two errors into one, because of the differences between RS VISA and NI VISA
			error = VI_ERROR_RSRC_NFOUND;
			snprintf(errorElab, RS_MAX_MESSAGE_LEN,
			         "VISA Open error: The entered resource name '%s' is invalid or not found."
			         " Make sure you enter a valid resource name.", resourceName);
			(void)RsCore_SetErrorInfo(instrSession, VI_TRUE, error, VI_SUCCESS, errorElab);
			goto Error;
		}
		
		if (error < VI_SUCCESS)
			goto Error;
	}
	else
	{
		checkErr(RsCoreInt_SessionsListGetUniqueHandle(&instrSession));
		rmSession = instrSession;
	}

	checkErr(RsCoreInt_SessionsListAddSession(resourceName, &rsSession));

	*instrSessionNew = instrSession;
	rsSession->io = instrSession;
	rsSession->rmSession = rmSession;
	RsCoreInt_StrcpyMaxLen(rsSession->resourceName, RS_MAX_SHORT_MESSAGE_BUF_SIZE, resourceName);

	// Clear the cached attributes table
	rsSession->usedAttributesCache.allocatedSize = 0;
	rsSession->usedAttributesCache.cachedCount = 0;
	rsSession->usedAttributesCache.attrs = NULL;
	rsSession->usedAttributesCache.IDs = NULL;

	// Determine number of attributes and allocate the memory
	for (attributesCount = 0; attrList[attributesCount]; attributesCount++);

	rsSession->attributesCount = attributesCount;

	viCheckAllocElab(rsSession->attributes = (RsCoreAttribute *)calloc(attributesCount, sizeof(RsCoreAttribute)),
		"Allocation of memory for Attributes list");

	// Allocate memory for current (cached) value of each attribute and copy statically
	// declared attribute properties into session-based allocated memory space.
	for (idx = 0; idx < (ViInt32)attributesCount; idx++)
	{
		attr = &rsSession->attributes[idx];
		memcpy(attr, attrList[idx], sizeof(RsCoreAttribute));
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			dataSize = sizeof(ViInt32);
			break;
		case RS_VAL_INT64:
			dataSize = sizeof(ViInt64);
			break;
		case RS_VAL_REAL64:
			dataSize = sizeof(ViReal64);
			break;
		case RS_VAL_STRING:
			// For string attribute, allocate minimum of one byte and maximum of its default value + 1
			dataSize = (attr->stringDefault != NULL) ? (ViInt32)strlen(attr->stringDefault) + 1 : 1;
			attr->integerDefault = dataSize;
			break;
		case RS_VAL_ADDR:
			dataSize = sizeof(ViAddr);
			break;
		case RS_VAL_SESSION:
			dataSize = sizeof(ViSession);
			break;
		case RS_VAL_BOOLEAN:
			dataSize = sizeof(ViInt32); // alloc 4 bytes instead of Boolean 2 bytes
			break;
		case RS_VAL_UNKNOWN_TYPE:
		default:
			dataSize = sizeof(void *);
		}

		attr->p2value = calloc(1, dataSize);
		if (attr->p2value == NULL)
		{
			viCheckErrElab(RS_ERROR_ALLOC, "Allocation of memory for Attribute values");
		}
	}

Error:
	RsCoreInt_MutexUnlock(gMutex);
	if (rsSession == NULL)
	{
		*instrSessionNew = 0;
		rmSession = 0;
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetAttributePtr
    Returns pointer to the required attribute.
******************************************************************************************************/
ViStatus RsCoreInt_GetAttributePtr(ViSession instrSession, ViAttr attributeId, RsCoreAttributePtr* attrOut)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 idx;
	ViInt32 foundIdx = -1;
	ViInt32 constant = (ViInt32)attributeId;
	RsCoreUsedAttributesCachePtr cache;
	const ViInt32 cacheSizeIncrease = 64;
	RsCoreAttributePtr attr = NULL;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	cache = &rsSession->usedAttributesCache;
#ifdef CACHE_USED_ATTRS
	// First, try the table with cached attributes
	for (idx = 0; idx < cache->cachedCount; idx++)
	{
		if (rsSession->usedAttributesCache.IDs[idx] == attributeId)
		{
			foundIdx = idx;
			attr = rsSession->usedAttributesCache.attrs[foundIdx];
			break;
		}
	}
#endif

	if (foundIdx < 0)
	{
		// Not found in cached attributes, search in the whole attribute table
		for (idx = 0; idx < rsSession->attributesCount; idx++)
		{
			if (rsSession->attributes[idx].constant == constant)
			{
				foundIdx = idx;
				attr = &(rsSession->attributes[idx]);
#ifdef CACHE_USED_ATTRS
				// Found, add to the cache
				if (cache->allocatedSize == 0)
				{
					// Allocate new cache
					viCheckAlloc(cache->attrs = (RsCoreAttributePtr*)calloc(cacheSizeIncrease, sizeof(RsCoreAttributePtr)));
					viCheckAlloc(cache->IDs = (ViAttr*)calloc(cacheSizeIncrease, sizeof(ViAttr)));
					cache->allocatedSize = cacheSizeIncrease;
				}

				if (cache->cachedCount >= cache->allocatedSize)
				{
					// Reallocate cache size
					cache->allocatedSize = cache->allocatedSize + cacheSizeIncrease;
					viCheckReallocElab(cache->attrs,
						(RsCoreAttributePtr*)realloc(cache->attrs, cache->allocatedSize * sizeof(RsCoreAttributePtr)),
						"Cache for used attributes cache->attrs");

					viCheckReallocElab(cache->IDs,
						(ViAttr*)realloc(cache->IDs, cache->allocatedSize * sizeof(ViAttr)),
						"Cache for used attributes cache->IDs");
				}

				cache->attrs[cache->cachedCount] = &(rsSession->attributes[foundIdx]);
				cache->IDs[cache->cachedCount] = (ViAttr)constant;
				cache->cachedCount++;
#endif
				break;
			}
		}
	}

	if (foundIdx < 0)
	{
		viCheckErrElab(RS_ERROR_INVALID_ATTRIBUTE, "Attribute not found in the attributes table");
	}

	if (attrHasFlag(RS_VAL_NOT_SUPPORTED))
	{
		viCheckErrElab(RS_ERROR_ATTRIBUTE_NOT_SUPPORTED, "Attribute is not supported - the flag RS_VAL_NOT_SUPPORTED is set to TRUE");
	}

	*attrOut = attr;

Error:
	return error;
}

#pragma warning ( disable : 4789 )
/*****************************************************************************************************/
/*  RsCoreInt_GetAttrP2Value
    Returns value of the entered attribute
******************************************************************************************************/
ViStatus RsCoreInt_GetAttrP2Value(RsCoreAttributePtr attr, void* value, ViInt32 bufSize)
{
	ViStatus error = VI_SUCCESS;

	if (!value)
		return error;

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
		*(ViInt32 *)value = *(ViInt32 *)attr->p2value;
		break;
	case RS_VAL_INT64:
		*(ViInt64 *)value = *(ViInt64 *)attr->p2value;
		break;
	case RS_VAL_REAL64:
		*(ViReal64 *)value = *(ViReal64 *)attr->p2value;
		break;
	case RS_VAL_STRING:
		RsCoreInt_StrcpyMaxLen((ViString)value, bufSize, (ViString)attr->p2value);
		break;
	case RS_VAL_ADDR:
		*(ViAddr *)value = *(ViAddr *)attr->p2value;
		break;
	case RS_VAL_SESSION:
		*(ViSession *)value = *(ViSession *)attr->p2value;
		break;
	case RS_VAL_BOOLEAN:
		*(ViBoolean *)value = *(ViBoolean *)attr->p2value;
		break;
	case RS_VAL_UNKNOWN_TYPE:
	default:
		return RS_ERROR_INVALID_TYPE;
	}

	return error;
}
#pragma warning ( default : 4789 )

/*****************************************************************************************************/
/*  RsCoreInt_GetAttrViStringLen
    Returns length of a current attribute value and optionally size of the currently allocated buffer for the string value
******************************************************************************************************/
ViStatus RsCoreInt_GetAttrViStringLen(RsCoreAttributePtr attr, ViInt32* stringLength, ViInt32* allocatedBufferSize)
{
	ViStatus error = VI_SUCCESS;

	switch (attr->dataType)
	{
	case RS_VAL_STRING:
		*stringLength = (ViInt32)strlen((ViChar*)attr->p2value);
		if (allocatedBufferSize != NULL)
			*allocatedBufferSize = attr->integerDefault;
		break;
	default:
		checkErr(RS_ERROR_INVALID_TYPE);
	}

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Attribute-related functions ------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_GetAttrInfoString
    Returns string identifying the attribute ID - used in error messages
    Output value points to the end of the composed string
******************************************************************************************************/
ViChar* RsCoreInt_GetAttrInfoString(RsCoreAttributePtr attr, ViInt32 bufferSize, ViChar* attributeInfo)
{
	ViInt32 attrId;
	ViChar scpiCommand[RS_MAX_MESSAGE_LEN] = "";

	if (attr == NULL)
		return attributeInfo;

	if (!isNullOrEmpty(attr->command))
		snprintf(scpiCommand, RS_MAX_MESSAGE_LEN, ", SCPI cmd '%s'", attr->command);

	attrId = attr->constant;

	if (attrId > RS_CLASS_PUBLIC_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, (ViChar*)"Attribute ID RS_CLASS_PUBLIC_ATTR_BASE + " _PERCD "%s", attrId - RS_CLASS_PUBLIC_ATTR_BASE, scpiCommand);
	}
	else if (attrId > RS_CLASS_PRIVATE_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, "Attribute ID RS_CLASS_PRIVATE_ATTR_BASE +" _PERCD "%s", attrId - RS_CLASS_PRIVATE_ATTR_BASE, scpiCommand);
	}
	else if (attrId > RS_SPECIFIC_PUBLIC_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, "Attribute ID RS_SPECIFIC_PUBLIC_ATTR_BASE +" _PERCD "%s", attrId - RS_SPECIFIC_PUBLIC_ATTR_BASE, scpiCommand);
	}
	else if (attrId > RS_SPECIFIC_PRIVATE_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, "Attribute ID RS_SPECIFIC_PRIVATE_ATTR_BASE + " _PERCD "%s", attrId - RS_SPECIFIC_PRIVATE_ATTR_BASE, scpiCommand);
	}
	else if (attrId > RS_ENGINE_PUBLIC_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, "Attribute ID RS_ENGINE_PUBLIC_ATTR_BASE + " _PERCD "%s", attrId - RS_ENGINE_PUBLIC_ATTR_BASE, scpiCommand);
	}
	else if (attrId > RS_ENGINE_PRIVATE_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, "Attribute ID RS_ENGINE_PRIVATE_ATTR_BASE + " _PERCD "%s", attrId - RS_ENGINE_PRIVATE_ATTR_BASE, scpiCommand);
	}
	else if (attrId > RS_ATTR_BASE)
	{
		snprintf(attributeInfo, bufferSize, "Attribute ID RS_ATTR_BASE + " _PERCD "%s", attrId - RS_ATTR_BASE, scpiCommand);
	}

	return attributeInfo + strlen(attributeInfo);
}

/*****************************************************************************************************/
/*  RsCoreInt_CastDataType
	Casts the entered value data type to the target data type
	Supported casts:
	- RS_VAL_INT32 -> RS_VAL_REAL64, RS_VAL_BOOLEAN, RS_VAL_INT64, RS_VAL_SESSION
	- RS_VAL_REAL64 -> RS_VAL_INT32, RS_VAL_BOOLEAN, RS_VAL_INT64
	- RS_VAL_BOOLEAN -> RS_VAL_INT32, RS_VAL_REAL64, RS_VAL_INT64
	- RS_VAL_INT64 -> RS_VAL_INT32, RS_VAL_REAL64, RS_VAL_BOOLEAN
	- RS_VAL_SESSION -> RS_VAL_INT32

	You have to provide the buffer of at least sizeof(targetType)
	If the targetType == sourceType, the function ends with VI_SUCCESS and copy of the sourceValue in the targetValue buffer
	If the sourceType can not be casted to the targetType, the function generates RS_ERROR_INVALID_TYPE
	If the sourceType can be casted to the targetType, the function casts: targetValue = (targetType)sourceValue
	and ends with VI_SUCCESS
******************************************************************************************************/
ViStatus RsCoreInt_CastDataType(ViSession instrSession, RsCoreDataType sourceType, void* sourceValue, RsCoreDataType targetType, void* targetValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViBoolean cannotCast = VI_FALSE;

	ViInt32 sourceViInt32 = 0, targetViInt32 = 0;
	ViReal64 sourceViReal64 = 0.0, targetViReal64 = 0.0;
	ViBoolean sourceViBoolean = VI_FALSE, targetViBoolean = VI_FALSE;
	ViInt64 sourceViInt64 = 0, targetViInt64 = 0;
	ViSession sourceViSession = 0, targetViSession = 0;

	if (targetType == sourceType)
	{
		switch (sourceType)
		{
		case RS_VAL_INT32:		*((ViInt32*)targetValue) = *((ViInt32*)sourceValue); break;
		case RS_VAL_REAL64:		*((ViReal64*)targetValue) = *((ViReal64*)sourceValue); break;
		case RS_VAL_BOOLEAN:	*((ViBoolean*)targetValue) = *((ViBoolean*)sourceValue); break;
		case RS_VAL_INT64:		*((ViInt64*)targetValue) = *((ViInt64*)sourceValue); break;
		case RS_VAL_SESSION:	*((ViSession*)targetValue) = *((ViSession*)sourceValue); break;
		case RS_VAL_STRING:		*((ViChar*)targetValue) = *((ViChar*)sourceValue); break;
		case RS_VAL_ADDR:		*((ViAddr*)targetValue) = *((ViAddr*)sourceValue); break;
		case RS_VAL_EVENT:	break;
		default: goto Error;
		}

		goto Error;
	}

	switch (sourceType)
	{
	case RS_VAL_INT32:		sourceViInt32 = (*((ViInt32*)sourceValue)); break;
	case RS_VAL_REAL64:		sourceViReal64 = (*((ViReal64*)sourceValue)); break;
	case RS_VAL_BOOLEAN:	sourceViBoolean = (*((ViBoolean*)sourceValue)); break;
	case RS_VAL_INT64:		sourceViInt64 = (*((ViInt64*)sourceValue)); break;
	case RS_VAL_SESSION:	sourceViSession = (*((ViSession*)sourceValue)); break;
	default: cannotCast = VI_TRUE; goto Error;
	}

	if (targetType == RS_VAL_INT32)
	{
		switch (sourceType)
		{
		case RS_VAL_REAL64:
			if (sourceViReal64 > RS_VAL_INT_MAX) targetViInt32 = RS_VAL_INT_MAX;
			else if (sourceViReal64 < RS_VAL_INT_MIN) targetViInt32 = RS_VAL_INT_MIN;
			else targetViInt32 = (ViInt32)sourceViReal64;
			break;
		case RS_VAL_BOOLEAN:	targetViInt32 = (ViInt32)sourceViBoolean; break;
		case RS_VAL_INT64:
			if (sourceViInt64 > RS_VAL_INT_MAX) targetViInt32 = RS_VAL_INT_MAX;
			else if (sourceViInt64 < RS_VAL_INT_MIN) targetViInt32 = RS_VAL_INT_MIN;
			else targetViInt32 = (ViInt32)sourceViInt64;
			break;
		case RS_VAL_SESSION:	targetViInt32 = (ViInt32)sourceViSession; break;
		default: cannotCast = VI_TRUE; goto Error;
		}
		memcpy(targetValue, &targetViInt32, sizeof(ViInt32));
	}
	else if (targetType == RS_VAL_REAL64)
	{
		switch (sourceType)
		{
		case RS_VAL_INT32:		targetViReal64 = (ViReal64)sourceViInt32; break;
		case RS_VAL_BOOLEAN:	targetViReal64 = (ViReal64)sourceViBoolean; break;
		case RS_VAL_INT64:		targetViReal64 = (ViReal64)sourceViInt64; break;
		default: cannotCast = VI_TRUE; goto Error;
		}
		memcpy(targetValue, &targetViReal64, sizeof(ViReal64));
	}
	else if (targetType == RS_VAL_BOOLEAN)
	{
		switch (sourceType)
		{
		case RS_VAL_INT32:		targetViBoolean = sourceViInt32 != 0 ? VI_TRUE : VI_FALSE; break;
		case RS_VAL_REAL64:		targetViBoolean = fabs(sourceViReal64) > DBL_MIN ? VI_TRUE : VI_FALSE; break;
		case RS_VAL_INT64:		targetViBoolean = sourceViInt64 != 0 ? VI_TRUE : VI_FALSE; break;
		default: cannotCast = VI_TRUE; goto Error;
		}
		memcpy(targetValue, &targetViBoolean, sizeof(ViBoolean));
	}
	else if (targetType == RS_VAL_INT64)
	{
		switch (sourceType)
		{
		case RS_VAL_INT32:		targetViInt64 = (ViInt64)sourceViInt32; break;
		case RS_VAL_REAL64:
			if (sourceViReal64 > RS_VAL_LLONG_MAX) targetViInt64 = RS_VAL_LLONG_MAX;
			else if (sourceViReal64 < RS_VAL_LLONG_MIN) targetViInt64 = RS_VAL_LLONG_MIN;
			else targetViInt64 = (ViInt64)sourceViReal64; break;
		case RS_VAL_BOOLEAN:	targetViInt64 = (ViInt64)sourceViBoolean; break;
		default: cannotCast = VI_TRUE; goto Error;
		}
		memcpy(targetValue, &targetViInt64, sizeof(ViInt64));
	}
	else if (targetType == RS_VAL_SESSION)
	{
		switch (sourceType)
		{
		case RS_VAL_INT32:		targetViSession = (ViSession)sourceViInt32; break;
		default: cannotCast = VI_TRUE; goto Error;
		}
		memcpy(targetValue, &targetViSession, sizeof(ViSession));
	}
	else
	{
		cannotCast = VI_TRUE;
	}

Error:
	if (cannotCast == VI_TRUE)
	{
		error = RS_ERROR_INVALID_TYPE;
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Cannot data type cast '%s' -> '%s'",
			RsCoreInt_GetDataTypeString(sourceType), RsCoreInt_GetDataTypeString(targetType));
		(void)RsCore_SetErrorInfo(instrSession, VI_TRUE, error, VI_SUCCESS, errElab);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_SetAttribute
    Sets ViInt32, ViReal64, ViBoolean and ViString attribute you specify to a new value.
    The function takes into account the Attribute properties and can use following callbacks,
    if the attribute specifies it:
    - UserCallback
    - CheckCallback (default RsCore_CheckCallback) if the attribute has range table defined
    - WriteCallback (default RsCore_WriteCallback)
    - CheckStatusCallback (default RsCore_CheckStatusCallback)

    The main communication with the instrument is performed by WriteCallback.
    At the end the function writes the set value to the attribute's cache memory
******************************************************************************************************/
ViStatus RsCoreInt_SetAttribute(ViSession instrSession,
                                ViConstString repCapName,
                                ViAttr attributeId,
                                ViInt32 optionFlags,
								RsCoreDataType valueDataType,
                                void* valueIn)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViStatus primaryError, secondaryError;
	ViBoolean simulation = RsCore_Simulating(instrSession);
	ViBoolean queryInstrumentStatus = RsCore_QueryInstrStatus(instrSession);
	ViBoolean rangeChecking = RsCore_RangeChecking(instrSession);
	void* p2CastedValue = NULL;
	void* p2Value = valueIn;
	RsCoreAttributePtr attr = NULL;
	ViBoolean directUserCall = ((optionFlags & RS_VAL_DIRECT_USER_CALL) != 0);
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));

	if (attr->access == RS_VAL_READ_ONLY)
	{
		viCheckErr(RS_ERROR_RS_ATTR_NOT_WRITABLE);
	}

	if (directUserCall && attrHasFlag(RS_VAL_NOT_USER_WRITABLE))
	{
		viCheckErr(RS_ERROR_ATTRIBUTE_NOT_SUPPORTED);
	}

	// Data type check and casting
	if (valueDataType != attr->dataType && attr->dataType != RS_VAL_EVENT)
	{
		if (rsSession->reportAttrDataTypeMismatch == VI_TRUE)
		{
			viCheckErr(RS_ERROR_INVALID_TYPE);
		}

		// Allocate a buffer for the casted value, maximum castable variable size is ViReal64
		viCheckAllocElab(p2CastedValue = calloc(1, sizeof(ViReal64)), "RsCoreInt_SetAttribute: Allocation for casted input value");
		checkErr(RsCoreInt_CastDataType(instrSession, valueDataType, valueIn, attr->dataType, p2CastedValue));
		p2Value = p2CastedValue;
	}

	// User callback completely skips to the cache update
	if (attr->userCallback)
	{
		checkErr((attr->userCallback)(instrSession, repCapName, attr, 0, p2Value));
		goto UpdateCache;
	}

	if (directUserCall)
	{
		checkErr(RsCoreInt_CheckAttrInstrModelAndOptions(instrSession, attr));
	}

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
	case RS_VAL_REAL64:
		if (rangeChecking == VI_TRUE)
		{
			// Range checking callback
			checkErr(RsCore_CheckCallback(instrSession, repCapName, attr, p2Value));
		}
		break;
	default:
		break;
	}

	if (optionFlags & RS_VAL_SET_CACHE_ONLY)
		goto UpdateCache;

	if (attr->writeCallback)
	{
		// Custom WriteCallback in simulation is called only if the flag RS_VAL_USE_CALLBACKS_FOR_SIMULATION is TRUE
		if (simulation == VI_FALSE || (simulation == VI_TRUE && attrHasFlag(RS_VAL_USE_CALLBACKS_FOR_SIMULATION)))
		{
			// Attribute-specific Write Callback
			checkStatusOnErr(attr->writeCallback(instrSession, repCapName, attr, p2Value));
		}
	}
	else
	{
		// Standard Write callback is called always
		checkStatusOnErr(RsCore_WriteCallback(instrSession, repCapName, attr, p2Value));
	}

CheckStatus:
	// CheckStatus
	if (queryInstrumentStatus == VI_FALSE || attrHasFlag(RS_VAL_DONT_CHECK_STATUS) || !directUserCall)
		goto ClearEsr;

	if (attr->checkStatusCallback)
	{
		// Custom CheckStatusCallback in simulation is not called
		if (simulation == VI_FALSE)
		{
			// Attribute-specific check status callback
			checkErr((attr->checkStatusCallback)(instrSession, error));
		}
	}
	else
	{
		// Default-defined check status callback
		if (attrHasFlag(RS_VAL_WAIT_FOR_OPC_AFTER_WRITES))
		{
			checkErr(RsCoreInt_CheckStatusAfterOpcSync(instrSession, error));
		}
		else
		{
			checkErr(RsCore_CheckStatus(instrSession, error));
		}
	}

ClearEsr:
	// Clear ESR register after OPC-sync command
	if (error == VI_SUCCESS && attrHasFlag(RS_VAL_WAIT_FOR_OPC_AFTER_WRITES))
	{
		if (rsSession->opcWaitMode != RS_VAL_OPCWAIT_OPCQUERY)
		{
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}
	}

UpdateCache:
	// Set the attribute value to p2value
	checkErr(RsCore_SetAttributeP2Value(instrSession, attr, p2Value));

Error:
	if (p2CastedValue)
		free(p2CastedValue);
	
	if (error != VI_SUCCESS)
	{
		(void)RsCore_GetErrorInfo(instrSession, &primaryError, &secondaryError, errElab);
		if (error == RS_ERROR_INVALID_TYPE)
		{
			// Special case for invalid data type, rewrite the message
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "SetAttribute: The entered data value type '%s' does not match the attribute data type '%s'",
				RsCoreInt_GetDataTypeString(valueDataType),
				RsCoreInt_GetDataTypeString(attr->dataType));
		}
		
		// Add the attribute info to the error message
		(void)RsCoreInt_SetErrorInfoAttr(instrSession, attr, VI_TRUE, primaryError, secondaryError, errElab);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetAttribute
    Reads ViInt32, ViReal64, ViBoolean and ViString attribute you specify.
    The function takes into account the Attribute properties and can use following callbacks,
    if the attribute specifies it:
    - UserCallback
    - ReadCallback (default RsCore_ReadCallback)
    - CheckStatusCallback (default RsCore_CheckStatusCallback)

    The main communication with the instrument is performed by ReadCallback.
    To optimize memory allocations, the ReadCallback must also write the retrieved value to the attribute's cache memory
******************************************************************************************************/
ViStatus RsCoreInt_GetAttribute(ViSession instrSession,
                                ViConstString repCapName,
                                ViAttr attributeId,
                                ViInt32 optionFlags,
								RsCoreDataType valueDataType,
                                ViInt32 bufSize,
                                void* valueOut)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViStatus primaryError, secondaryError;
	ViBoolean simulation = RsCore_Simulating(instrSession);
	ViBoolean queryInstrumentStatus = RsCore_QueryInstrStatus(instrSession);
	RsCoreAttributePtr attr = NULL;
	ViBoolean directUserCall = (optionFlags & RS_VAL_DIRECT_USER_CALL) != 0;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));

	if (attr->access == RS_VAL_WRITE_ONLY)
	{
		viCheckErr(RS_ERROR_RS_ATTR_NOT_READABLE);
	}

	if (directUserCall && attrHasFlag(RS_VAL_NOT_USER_READABLE))
	{
		viCheckErr(RS_ERROR_ATTRIBUTE_NOT_SUPPORTED);
	}

	// Data type check. Potential casting to the output value happens at the SetUserBuffer
	if (valueDataType != attr->dataType && rsSession->reportAttrDataTypeMismatch == VI_TRUE)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "The required output data value type '%s' does not match the attribute data type '%s'",
			RsCoreInt_GetDataTypeString(valueDataType), RsCoreInt_GetDataTypeString(attr->dataType));
		viCheckErrElab(RS_ERROR_INVALID_TYPE, errElab);
	}

	if (attr->userCallback)
	{
		checkErr((attr->userCallback)(instrSession, repCapName, attr, bufSize, valueOut));
		goto SetUserBuffer;
	}

	if (directUserCall)
	{
		checkErr(RsCoreInt_CheckAttrInstrModelAndOptions(instrSession, attr));
	}
	
	if (attr->readCallback)
	{
		// Custom ReadCallback in simulation is called only if the flag RS_VAL_USE_CALLBACKS_FOR_SIMULATION is TRUE
		if (simulation == VI_FALSE || (simulation == VI_TRUE && attrHasFlag(RS_VAL_USE_CALLBACKS_FOR_SIMULATION)))
		{
			checkStatusOnErr((attr->readCallback)(instrSession, repCapName, attr));
		}
	}
	else
	{
		// Standard read callback in simulation
		checkStatusOnErr(RsCore_ReadCallback(instrSession, repCapName, attr));
	}

CheckStatus:
	// CheckStatus
	if (queryInstrumentStatus == VI_FALSE || attrHasFlag(RS_VAL_DONT_CHECK_STATUS) || !directUserCall)
		goto ClearEsr;

	if (attr->checkStatusCallback)
	{
		// Custom CheckStatusCallback in simulation is not called
		if (simulation == VI_FALSE)
		{
			checkErr((attr->checkStatusCallback)(instrSession, error)); // User defined check status callback
		}
	}
	else
	{
		// Default-defined check status callback
		if (attrHasFlag(RS_VAL_WAIT_FOR_OPC_AFTER_WRITES))
		{
			checkErr(RsCoreInt_CheckStatusAfterOpcSync(instrSession, error));
		}
		else
		{
			checkErr(RsCore_CheckStatus(instrSession, error));
		}
	}

ClearEsr:
	// Clear ESR register after OPC-sync command
	if (error == VI_SUCCESS && attrHasFlag(RS_VAL_WAIT_FOR_OPC_AFTER_WRITES))
	{
		if (rsSession->vxiCapable == VI_TRUE)
		{
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}
	}

SetUserBuffer:
	if (valueDataType == attr->dataType)
	{
		checkErr(RsCoreInt_GetAttrP2Value(attr, valueOut, bufSize));
	}
	else
	{
		// Coercing to the user data type
		checkErr(RsCoreInt_CastDataType(instrSession, attr->dataType, attr->p2value, valueDataType, valueOut));
	}

Error:
	if (error != VI_SUCCESS)
	{
		(void)RsCore_GetErrorInfo(instrSession, &primaryError, &secondaryError, errElab);
		if (error == RS_ERROR_INVALID_TYPE)
		{
			// Special case for invalid data type, rewrite the message
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "GetAttribute: The return data value type '%s' does not match the attribute data type '%s'",
				RsCoreInt_GetDataTypeString(valueDataType),
				RsCoreInt_GetDataTypeString(attr->dataType));
		}

		// Add the attribute info to the error message
		(void)RsCoreInt_SetErrorInfoAttr(instrSession, attr, VI_TRUE, primaryError, secondaryError, errElab);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_CheckAttr
    Checks ViInt32, ViReal64, ViBoolean and ViString attribute you specify.
    The function takes into account the Attribute properties and can use following callbacks,
    if the attribute specifies it:
    - UserCallback
    - CheckCallback (default RsCore_CheckCallback) if the attribute has a range defined
    It also checks for instrument model and options.
******************************************************************************************************/
ViStatus RsCoreInt_CheckAttr(ViSession instrSession,
                             ViConstString repCapName,
                             ViAttr attributeId,
                             ViInt32 optionFlags,
                             void* value)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttributePtr attr;
	ViBoolean directUserCall = ((optionFlags & RS_VAL_DIRECT_USER_CALL) != 0);

	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));

	if (attr->access == RS_VAL_READ_ONLY)
	{
		viCheckErr(RS_ERROR_RS_ATTR_NOT_WRITABLE);
	}

	if (directUserCall && attrHasFlag(RS_VAL_NOT_USER_WRITABLE))
	{
		viCheckErr(RS_ERROR_ATTRIBUTE_NOT_SUPPORTED);
	}

	if (attr->userCallback)
	{
		checkErr((attr->userCallback)(instrSession, repCapName, attr, 0, value));
		goto Error;
	}

	if (directUserCall)
	{
		checkErr(RsCoreInt_CheckAttrInstrModelAndOptions(instrSession, attr));
	}

	if (attr->checkCallback)
	{
		// Custom CheckCallback in simulation is called only if the flag RS_VAL_USE_CALLBACKS_FOR_SIMULATION is TRUE
		checkErr((attr->checkCallback)(instrSession, repCapName, attr, value));
	}
	else
	{
		checkErr(RsCore_CheckCallback(instrSession, repCapName, attr, value));
	}

Error:
	if (error != VI_SUCCESS)
	{
		// Add the attribute info to the error message
		ViChar errorElaboration[RS_MAX_MESSAGE_BUF_SIZE];
		ViStatus primaryError, secondaryError;
		(void)RsCore_GetErrorInfo(instrSession, &primaryError, &secondaryError, errorElaboration);
		(void)RsCoreInt_SetErrorInfoAttr(instrSession, attr, VI_TRUE, primaryError, secondaryError, errorElaboration);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_CheckInstrumentModel
	Returns 'pass' value if the current instrument fulfills the modelsList
	If generateError is TRUE, the function generates RS_ERROR_INSTRUMENT_MODEL error if pass is FALSE
	The function does not take the rsCoreSession->optionChecking into account
	- Example of a positive requirement: modelsList = 'FSW|FSWP': for pass to be TRUE, the current instrument must be either FSW or FSWP
	- Example of a negative requirement: modelsList = '!FSW|FSWP': for pass to be TRUE, the current instrument must be neither FSW nor FSWP
******************************************************************************************************/
ViStatus RsCoreInt_CheckInstrumentModel(ViSession instrSession, ViConstString modelsList, ViBoolean* pass, ViBoolean generateError)
{
	ViStatus error = VI_SUCCESS;
	ViChar* instrModel = NULL;
	ViChar token[RS_MAX_MESSAGE_LEN];
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 offset = 0;
	ViInt32 tokensCount, idx;
	ViBoolean negated = VI_FALSE;
	ViInt32 len;
	ViBoolean flag = VI_FALSE;
	RsCoreSessionPtr rsSession = NULL;

	if (isNullOrEmpty(modelsList))
	{
		flag = VI_TRUE;
		goto Error;
	}
	
	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	instrModel = (ViChar *)rsSession->attrInstrModelPtr->p2value;
	tokensCount = RsCore_GetElementsCount(modelsList, '|');
	if (modelsList[0] == '!')
	{
		offset = 1;
		flag = VI_TRUE;
		negated = VI_TRUE;
	}

	for (idx = 0; idx < tokensCount; idx++)
	{
		checkErr(RsCore_GetTokenAtIndex(modelsList + offset, '|', idx, token, RS_MAX_MESSAGE_LEN, NULL));
		len = RsCore_TrimString(token, RS_VAL_TRIM_WHITESPACES);
		if (len == 0)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "RsCoreInt_CheckInstrumentModel: Empty token in the modelsList expression: '%s'", modelsList);
			viCheckErrElab(RS_ERROR_INVALID_VALUE, errElab);
		}

		if (negated == VI_TRUE)
		{
			// Break at any match with flag = VI_FALSE;
			if (strncmp(instrModel, token, len) == 0)
			{
				flag = VI_FALSE;
				break;
			}
		}
		else
		{
			// Break at any match with flag = VI_TRUE;
			if (strncmp(instrModel, token, len) == 0)
			{
				flag = VI_TRUE;
				break;
			}
		}
	}

	if (negated == VI_TRUE)
	{
		if (flag == VI_FALSE && generateError == VI_TRUE)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
				"Current instrument model '%s' is in the list of not-supported models: '%s'",
				instrModel, modelsList);
			viCheckErrElab(RS_ERROR_INSTRUMENT_MODEL, errElab);
		}
	}
	else
	{
		if (flag == VI_FALSE && generateError == VI_TRUE)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
				"Current instrument model '%s' is not in the list of supported models: '%s'",
				instrModel, modelsList);
			viCheckErrElab(RS_ERROR_INSTRUMENT_MODEL, errElab);
		}
	}

Error:
	if (pass) *pass = flag;
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_CheckInstrumentOptions
	Returns 'pass' value if the current instrument has the required option(s) installed.
	The optionsExpression is the same one as used in attributes.
	If generateError is TRUE, the function generates RS_ERROR_INSTRUMENT_OPTION error if pass is FALSE
	The function does not take the rsCoreSession->optionChecking into account
******************************************************************************************************/
ViStatus RsCoreInt_CheckInstrumentOptions(ViSession instrSession, ViConstString optionsExpression, ViBoolean* pass, ViBoolean generateError)
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	ViBoolean optionFound = VI_FALSE;
	ViChar optionsString[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar operation;
	ViChar* p2CurrentInstrModel = NULL;
	ViChar* p2instrOptionsList = NULL;
	ViChar *ptrStart, *ptrLeftBracket, *ptrRightBracket, *ptrOperator;
	ViInt32 len = 0;
	ViInt32 optionIdx;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	if (isNullOrEmpty(optionsExpression))
	{
		optionFound = VI_TRUE;
		goto Error;
	}

	p2CurrentInstrModel = (ViChar *)rsSession->attrInstrModelPtr->p2value;
	p2instrOptionsList = (ViChar *)rsSession->attrOptionsListPtr->p2value;

	RsCoreInt_StrcpyMaxLen(buffer, RS_MAX_MESSAGE_BUF_SIZE, optionsExpression);

	if (isspace(buffer[0]))
	{
		if (RsCore_TrimString(buffer, RS_VAL_TRIM_WHITESPACES) == 0)
			goto Error;
	}

	// Verify if the instrument model & option are available. User option checking syntax:
	// InstrModel(option1&option2|option3)|InstrModel(option1&option2|option3)...
	// or
	// option1&option2|option3...
	// No operator priorities (behaves as stack automat). */

	// Loop through the optionsExpression, until you find the current-instrument-related token
	// e.g. optionsExpression = "FSW(K3&K9|K160)|FPS(K8|K9|K30)", current instrument = "FSW"
	// We need the part within the round brackets "K3&K9|K160"
	ptrStart = buffer;
	while (ptrStart)
	{
		ptrLeftBracket = strchr(ptrStart, '(');
		if (ptrLeftBracket == ptrStart)
		{
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE,
				"RsCoreInt_CheckInstrumentOptions: Found left bracket without preceeding instrument model string: '%s'", optionsExpression);
			viCheckErrElab(RS_ERROR_INVALID_VALUE, buffer);
		}

		if (ptrLeftBracket == NULL)
		{
			// No model-specific expression, the optionsExpression is valid for all models
			RsCoreInt_StrcpyMaxLen(optionsString, RS_MAX_MESSAGE_BUF_SIZE, optionsExpression);
			break;
		}

		ptrRightBracket = strchr(ptrLeftBracket, ')');
		if (ptrRightBracket == NULL)
		{
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE,
				"RsCoreInt_CheckInstrumentOptions: Unpaired brackets in the optionsExpression - found left bracket, missing right bracket: '%s'",
				optionsExpression);
			viCheckErrElab(RS_ERROR_INVALID_VALUE, buffer);
		}

		if (strncmp(ptrStart, p2CurrentInstrModel, ptrLeftBracket - ptrStart) == 0)
		{
			// Instrument model equals string preceeding the '('
			// As the option string take the string between the brackets '(' and ')'

			len = (ViInt32)(ptrRightBracket - ptrLeftBracket);
			if (len > RS_MAX_MESSAGE_BUF_SIZE)
				len = RS_MAX_MESSAGE_BUF_SIZE;

			RsCoreInt_StrcpyMaxLen(optionsString, len, ptrLeftBracket + 1);
			break;
		}

		ptrStart = strchr(ptrRightBracket, '|');
		if (ptrStart == NULL)
		{
			// We went through the tk1|tk2| tokens and no instrument fits the current instrument model
			// Evaluate it as: No option needed for the current instrument and end without an error
			optionFound = VI_TRUE;
			goto Error;
		}

		ptrStart++;
		if (ptrStart[0] == 0)
		{
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE,
				"RsCoreInt_CheckInstrumentOptions: Semicolon at the end of the optionsExpression: '%s'", optionsExpression);
			viCheckErrElab(RS_ERROR_INVALID_VALUE, buffer);
		}
	}

	// If the optionsString is empty, the expression could have been e.g. SMVB(K40)|SMW()
	// Evaluate it as: No option needed for the current instrument and end without an error
	if (strlen(optionsString) == 0)
	{
		optionFound = VI_TRUE;
		goto Error;
	}

	// Parse option string
	ptrStart = optionsString;
	ptrOperator = strpbrk(optionsString, "&|");

	if (ptrOperator == NULL) // optionsString is a single option, e.g. "K3", no &| operators
	{
		optionFound = (RsCore_GetIndexOfToken(p2instrOptionsList, ',', ptrStart, &optionIdx, NULL) == VI_SUCCESS);
	}
	else // Expression, evaluate
	{
		operation = *ptrOperator;
		*ptrOperator = '\0';
		optionFound = (RsCore_GetIndexOfToken(p2instrOptionsList, ',', ptrStart, &optionIdx, NULL) == VI_SUCCESS);
		do
		{
			ViChar operation2 = '&';

			ptrStart = ptrOperator + 1;
			ptrOperator = strpbrk(ptrStart, "&|");
			if (ptrOperator)
			{
				operation2 = *ptrOperator;
				*ptrOperator = '\0';
			}

			switch (operation)
			{
			case '|':
				optionFound |= (RsCore_GetIndexOfToken(p2instrOptionsList, ',', ptrStart, &optionIdx, NULL) == VI_SUCCESS);
				break;
			case '&':
				optionFound &= (RsCore_GetIndexOfToken(p2instrOptionsList, ',', ptrStart, &optionIdx, NULL) == VI_SUCCESS);
				break;
			default:
				break;
			}
			operation = operation2;
		} while (ptrOperator);
	}

	if (optionFound == VI_FALSE && generateError == VI_TRUE)
	{
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE,
			"Rs_CheckInstrumentOptions: Instrument option not installed. "
			"Required option: '%s', current instrument model '%s', installed options '%s'",
			optionsExpression, p2CurrentInstrModel, p2instrOptionsList);
		viCheckErrElab(RS_ERROR_INSTRUMENT_OPTION, buffer);
	}

Error:
	if (pass) *pass = optionFound;
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_CheckAttrInstrModelAndOptions
    Check if the attribute is supported for the currently used instrument model (and options included)
******************************************************************************************************/
ViStatus RsCoreInt_CheckAttrInstrModelAndOptions(ViSession instrSession, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViChar* instrOptionsListDyn = NULL;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	
	// Check if at all this needs to be performed
	if (rsSession->optionChecking == VI_FALSE)
		return error;

	if (isNullOrEmpty(attr->instrumentOptions) && isNullOrEmpty(attr->supportedModels))
		return error;

	// Check instrument model
	checkErr(RsCoreInt_CheckInstrumentModel(instrSession, attr->supportedModels, NULL, VI_TRUE));
	checkErr(RsCoreInt_CheckInstrumentOptions(instrSession, attr->instrumentOptions, NULL, VI_TRUE));

Error:
	if (instrOptionsListDyn)
		free(instrOptionsListDyn);

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_QueryAttrCommandUni
    Queries an attribute command, takes into account RS_VAL_WAIT_FOR_OPC_AFTER_WRITES
    Command question-mark "?" is added to the end, if not already present
    The response is a null-terminated string trimmed for LF
    If 'unlimitedResponse' is FALSE, provide an allocated buffer in 'response' with the minimum size of RS_MAX_MESSAGE_BUF_SIZE
    If 'unlimitedResponse' is TRUE, the function allocates and returns the 'response' buffer
    WARNING!!! In case of unlimitedResponse==TRUE, make sure you free the 'response' buffer after use.
******************************************************************************************************/
ViStatus RsCoreInt_QueryAttrCommandUni(ViSession instrSession, RsCoreAttributePtr attr,
                                       ViConstString query, ViBoolean unlimitedResponse, ViChar** response, ViInt32* byteCount)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 cmdLen;
	ViInt32 bufferSize;
	ViChar* dynQuery = NULL;
	ViInt64 byteCountIntern = 0;

	if (query == NULL)
		return VI_ERROR_INV_PARAMETER;

	cmdLen = (ViInt32)strlen(query);
	if (cmdLen == 0)
		return VI_ERROR_INV_PARAMETER;

	checkErr(RsCore_StrCatDynamicString(instrSession, &dynQuery, query, "          ", NULL));
	bufferSize = cmdLen + 10;
	RsCore_TrimString(dynQuery, RS_VAL_TRIM_WHITESPACES);

	if ((memchr(dynQuery, '?', (size_t)cmdLen) == NULL))
	{
		// The query has no question-mark, add it at the end
		RsCore_StrcatMaxLen(dynQuery, bufferSize, "?", NULL, NULL);
	}

	// Use the command as it is
	if (attrHasFlag(RS_VAL_WAIT_FOR_OPC_AFTER_WRITES))
	{
		// OPC - synchronized queries
		if (unlimitedResponse)
		{
			checkErr(RsCoreInt_QueryUnknownLengthWithOpc(instrSession, dynQuery, VI_FALSE, 0, (ViByte **)response, &byteCountIntern));
			RsCore_TrimString(*response, RS_VAL_TRIM_WHITESPACES);
		}
		else
		{
			checkErr(RsCoreInt_QueryViStringWithOpc(instrSession, dynQuery, VI_FALSE, 0, RS_MAX_MESSAGE_BUF_SIZE, *response));
		}
	}
	else
	{
		// Normal queries
		if (unlimitedResponse)
		{
			checkErr(RsCore_QueryViStringUnknownLength(instrSession, dynQuery, response));
		}
		else
		{
			checkErr(RsCore_QueryViString(instrSession, dynQuery, *response));
		}
	}

	if (byteCount)
		*byteCount = (ViInt32)byteCountIntern;

Error:
	if (dynQuery)
		free(dynQuery);

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL RepCaps --------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_DisposeRepCapTable
    Frees the resources of a repeated capability table for the entered session
******************************************************************************************************/
ViStatus RsCoreInt_DisposeRepCapTable(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->repCapTables)
	{
		for (i = 0; i < rsSession->repCapsCount; i++)
		{
			if (rsSession->repCapTables[i].repCapNameId)
			{
				free(rsSession->repCapTables[i].repCapNameId);
				rsSession->repCapTables[i].repCapNameId = NULL;
			}
			if (rsSession->repCapTables[i].cmdValues)
			{
				free(rsSession->repCapTables[i].cmdValues);
				rsSession->repCapTables[i].cmdValues = NULL;
			}
			if (rsSession->repCapTables[i].repCapNames)
			{
				free(rsSession->repCapTables[i].repCapNames);
				rsSession->repCapTables[i].repCapNames = NULL;
			}
		}

		free(rsSession->repCapTables);
		rsSession->repCapTables = NULL;
	}
Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_FindRepCap
    Returns pointer to the required RepCap table. You can provide a custom RepCapTable.
    If you set it to NULL, the function takes the session's RepCapTable.
    If not found, the function generates error and sets the repCap to NULL;
******************************************************************************************************/
ViStatus RsCoreInt_FindRepCap(ViSession instrSession, RsCoreRepCapPtr customRepCapTable, ViConstString repCapNameId, RsCoreRepCapPtr* repCap)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	
	if (isNullOrEmpty(repCapNameId))
	{
		viCheckErrElab(RS_ERROR_NULL_POINTER, "FindRepCap: repCapNameId is NULL or empty");
	}

	if (customRepCapTable == NULL)
	{
		customRepCapTable = rsSession->repCapTables;
	}

	*repCap = NULL;
	for (i = 0; i < rsSession->repCapsCount; i++)
	{
		if ((strcmp(customRepCapTable[i].repCapNameId, repCapNameId) == 0))
		{
			*repCap = &(customRepCapTable[i]);
			goto Error;
		}
	}

	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "FindRepCap: repCapNameId '%s' not found", repCapNameId);
	viCheckErrElab(RS_ERROR_UNKNOWN_REPEATED_CAPABILITY_NAME, errElab);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetRepCapCount
    Returns the number of values in the repCap
******************************************************************************************************/
ViStatus RsCoreInt_GetRepCapCount(ViSession instrSession, RsCoreRepCapPtr repCap, ViInt32* count)
{
	*count = RsCore_GetElementsCount(repCap->repCapNames, ',');
	return VI_SUCCESS;
}

/*****************************************************************************
* Function: RsCoreInt_FindRepCapName
* Purpose:  If the repCapName exists in the entered repCapNameId table,
            the function returns its index in the repCapNames csv-string
            If the repCapName is not found, foundIdx = -1
            If you provide *tokenOffset, the function returns start offset of the repCapName in the repCapNames string
*****************************************************************************/
ViStatus RsCoreInt_FindRepCapName(ViSession instrSession, RsCoreRepCapPtr repCap,
                                  ViString repCapName, ViBoolean errorIfNotFound, ViInt32* foundIdx,
                                  ViInt32* tokenOffset)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	RsCore_GetIndexOfToken(repCap->repCapNames, ',', repCapName, foundIdx, tokenOffset);

	if (*foundIdx < 0 && errorIfNotFound == VI_TRUE)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
		         "FindRepCapName: Cannot find repCapName '%s' in repCapNameId '%s'. List of available RepCapNames: '%s'",
		         repCapName, repCap->repCapNameId, repCap->repCapNames);
		viCheckErrElab(RS_ERROR_UNKNOWN_REPEATED_CAPABILITY_NAME, errElab);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetRepCapNameAtIndex
    Returns the repCapName on the index idx from the provided repCap table. tokenOffset can be set to NULL
******************************************************************************************************/
ViStatus RsCoreInt_GetRepCapNameAtIndex(ViSession instrSession, RsCoreRepCapPtr repCap, ViInt32 idx, ViInt32 bufferSize, ViChar* repCapName,
                                        ViInt32* tokenOffset)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 repCapsCount;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	error = RsCore_GetTokenAtIndex(repCap->repCapNames, ',', idx, repCapName, bufferSize, tokenOffset);
	if (error == RS_ERROR_INDEX_IS_TOO_HIGH)
	{
		repCapsCount = RsCore_GetElementsCount(repCap->repCapNames, ',');
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
		         "GetRepCapNameAtIndex: 0-based index " _PERCD " is out of range."
		         " Valid range: 0.." _PERCD " (RepCapName count is " _PERCD ")", idx, repCapsCount - 1, repCapsCount);
		viCheckErrElab(RS_ERROR_INDEX_IS_TOO_HIGH, errElab);
	}

Error:
	return error;
}

/****************************************************************************/
/*  RsCoreInt_GetRepCapCmdAtIndex
    Returns the repCapCmd on the index idx from the provided repCap table
    tokenOffset can be set to NULL
*****************************************************************************/
ViStatus RsCoreInt_GetRepCapCmdAtIndex(ViSession instrSession, RsCoreRepCapPtr repCap, ViInt32 idx, ViInt32 bufferSize, ViChar* repCapCmd,
                                       ViInt32* tokenOffset)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 repCapsCount;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	error = RsCore_GetTokenAtIndex(repCap->cmdValues, ',', idx, repCapCmd, bufferSize, tokenOffset);
	if (error == RS_ERROR_INDEX_IS_TOO_HIGH)
	{
		repCapsCount = RsCore_GetElementsCount(repCap->cmdValues, ',');
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
		         "GetRepCapCmdAtIndex: 0-based index " _PERCD " is out of range."
		         " Valid range: 0.." _PERCD " (RepCapCmds count is " _PERCD ")", idx, repCapsCount - 1, repCapsCount);
		viCheckErrElab(RS_ERROR_INDEX_IS_TOO_HIGH, errElab);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_ValidateAttrForRepCap
    Checks whether you can use an attribute on a particular repeated capability instance.
    It generates an error if:
    - Attribute has repCaps defined (usercaps is TRUE), but none are found in the command
    - Attribute does not use the entered repCap
******************************************************************************************************/
ViStatus RsCoreInt_ValidateAttrForRepCap(ViSession instrSession,
                                         RsCoreRepCapPtr repCap,
                                         RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViChar rCaps[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE] = "";

	// Check if the attribute is enabled to use repeated capabilities
	if (attr->usercaps == VI_FALSE)
	{
		viCheckErrElab(RS_ERROR_ATTR_MUST_BE_REPEATED_CAPABILITY_BASED, "Attribute has no repCaps allowed (property usercaps is FALSE)");
	}

	// Check if the attribute has defined repeated capability names and
	// if the define repeated capability name id is for that attribute valid
	checkErr(RsCoreInt_GetAttrRepCapNameIds(instrSession, attr, RS_MAX_MESSAGE_BUF_SIZE, rCaps));
	if (strlen(rCaps) == 0)
	{
		snprintf(errElab, RS_MAX_MESSAGE_LEN, "Attribute has empty RepCaps list, required repCapNameId: '%s'", repCap->repCapNameId);
		viCheckErrElab(RS_ERROR_EMPTY_REPEATED_CAPABILITY_LIST, errElab);
	}

	if (!strstr(rCaps, repCap->repCapNameId))
	{
		snprintf(errElab, RS_MAX_MESSAGE_LEN, "Required repCapNameId '%s' is not in the attribute's list of repCapIDs '%s'",
		         repCap->repCapNameId, rCaps);
		viCheckErrElab(RS_ERROR_REPEATED_CAPABILITY_NAME_NOT_ALLOWED, errElab);
	}

Error:

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetAttrRepCapNameIds
	Returns the repeated capability IDs that belongs to the attribute you specify.
	More then one id is listed in comma separated string.
	Example: Attr command: 'SENS{Channel}:TGAT{Timegate}:TIME'
	Returned repCapNameIds:  'Channel,Timegate'
******************************************************************************************************/
ViStatus RsCoreInt_GetAttrRepCapNameIds(ViSession instrSession,
                                        RsCoreAttributePtr attr,
                                        ViInt32 bufferSize,
                                        ViChar* repCapNameIds)
{
	ViStatus error = VI_SUCCESS;
	ViChar *p2cmd, *p2cmdStart, *p2cmdStop;
	ViInt32 len;
	ViInt32 offset;
	ViInt32 tokenSize;

	if (repCapNameIds == NULL)
	{
		viCheckErr(RS_ERROR_NULL_POINTER);
	}

	if (attr->usercaps == VI_FALSE)
	{
		viCheckErr(RS_ERROR_ATTR_MUST_BE_REPEATED_CAPABILITY_BASED);
	}

	p2cmd = attr->command;
	len = (ViInt32)strlen(attr->command);

	repCapNameIds[0] = 0;
	offset = 0;
	while ((p2cmdStart = (ViChar*)memchr(p2cmd, '{', len)) != NULL)
	{
		len -= (ViInt32)(p2cmdStart - p2cmd);
		if ((p2cmdStop = (ViChar*)memchr(p2cmdStart, '}', len)) == NULL) break;
		len -= (ViInt32)(p2cmdStop - p2cmdStart);
		tokenSize = (ViInt32)(p2cmdStop - p2cmdStart) - 1;
		if (bufferSize < (offset + tokenSize + 1))
			break;

		memcpy(repCapNameIds + offset, p2cmdStart + 1, tokenSize);
		offset += tokenSize;
		repCapNameIds[offset] = ',';
		p2cmd = p2cmdStop;
		offset++;
	}
	if (offset > 0)
		repCapNameIds[offset - 1] = 0;
Error:

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL Range Table Functions ------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_FindDiscreteRangeTableCmdString
	Finds the numericValue in the range table and returns the corresponding command string
	If the value is not found, the function returns RS_ERROR_INVALID_VALUE
	Precision for dataType RS_VAL_INT32 is coerced to DBL_MIN
	Warning!!! The function returns the pointer to the actual cmd value in the range table,
	do not modify the value
******************************************************************************************************/
ViStatus RsCoreInt_FindDiscreteRangeTableCmdString(ViSession instrSession,
                                                   RsCoreRangeTablePtr rangeTable,
                                                   RsCoreDataType dataType,
                                                   ViReal64 numericValue,
                                                   ViReal64 precision,
                                                   ViChar** p2cmdString)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0;
	ViInt32 int32Value = 0;
	ViReal64 p2discrete;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	switch (dataType)
	{
	case RS_VAL_INT32:
		int32Value = (ViInt32)numericValue;
		break;
	case RS_VAL_REAL64:
		break;
	default:
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, dataType,
			"RsCoreInt_FindDiscreteRangeTableCmdString only accepts type RS_VAL_INT32 and RS_VAL_REAL64"));
	}

	// Go over range table values and find appropriate command string for passed attribute value.
	// For floating point values use predefined attribute precision.
	do
	{
		p2discrete = rangeTable->rangeValues[i].discreteOrMinValue;
		*p2cmdString = (rangeTable->rangeValues[i].cmdString);
		if (rangeTable->rangeValues[i].cmdValue == RS_RANGE_TABLE_END_VALUE)
		{
			// Value not found, report RS_ERROR_INVALID_VALUE
			if (dataType == RS_VAL_INT32)
			{
				snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
				         "Entered integer value " _PERCD " is not in the list of the recognized discrete integer values:", int32Value);
			}
			else if (dataType == RS_VAL_REAL64)
			{
				snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
				         "Entered double value %.12lG is not in the list of the recognized discrete float values:", numericValue);
			}

			RsCoreInt_AppendDiscreteRangeTableList(rangeTable, dataType, RS_MAX_MESSAGE_BUF_SIZE, errElab);
			viCheckErrElab(RS_ERROR_INVALID_VALUE, errElab);
		}

		if (dataType == RS_VAL_INT32)
		{
			if (fabs(p2discrete - int32Value) <= DBL_MIN)
				break;
		}
		else
		{
			if (fabs(p2discrete - numericValue) <= precision)
				break;
		}

		i++;
	} while (i < 999999); // End of searching for range table enum, safe-measures limit
	
	if (i >= 999999)
	{
		viCheckErrElab(RS_ERROR_INVALID_RANGE_TABLE, "RsCoreInt_FindDiscreteRangeTableCmd: Range table has improper ending");
	}

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL VISA IO Helpers ------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_PollingDelay
    Generates progressive OPC polling delay depending on the selected mode
******************************************************************************************************/
void RsCoreInt_PollingDelay(ViReal64 elapsedTimeMs, ViInt32 opcWaitMode)
{
	switch (opcWaitMode)
	{
	case RS_VAL_OPCWAIT_STBPOLLINGSLOW:
		if (elapsedTimeMs < 10.0)
			Sleep(1);
		else if (elapsedTimeMs < 1000.0)
			Sleep(20);
		else if (elapsedTimeMs < 5000.0)
			Sleep(100);
		else if (elapsedTimeMs < 10000.0)
			Sleep(200);
		else if (elapsedTimeMs < 20000.0)
			Sleep(500);
		else
			Sleep(1000);
		break;

	case RS_VAL_OPCWAIT_STBPOLLINGSUPERSLOW:
		if (elapsedTimeMs < 1000.0)
			Sleep(100);
		else if (elapsedTimeMs < 10000.0)
			Sleep(500);
		else if (elapsedTimeMs < 20000.0)
			Sleep(1000);
		else
			Sleep(2000);
		break;

	default: // RS_VAL_OPCWAIT_STBPOLLING
		if (elapsedTimeMs < 10.0);
		else if (elapsedTimeMs < 100.0)
			Sleep(5);
		else if (elapsedTimeMs < 1000.0)
			Sleep(20);
		else if (elapsedTimeMs < 5000.0)
			Sleep(50);
		else if (elapsedTimeMs < 10000.0)
			Sleep(100);
		else if (elapsedTimeMs < 50000.0)
			Sleep(500);
		else
			Sleep(1000);
		break;
	}
}

/*****************************************************************************************************/
/*  RsCoreInt_GetVisaInfo
    Returns used VISA info string
******************************************************************************************************/
ViStatus RsCoreInt_GetVisaInfo(ViSession instrSession, ViInt32 bufferSize, ViChar* visaInfo)
{
	ViStatus error = VI_SUCCESS;
	ViChar manufacturer[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 version;

	if (RsCore_Simulating(instrSession) == VI_TRUE)
	{
		RsCoreInt_StrcpyMaxLen(visaInfo, bufferSize, "Rohde & Schwarz Visa 5.8.5");
		return error;
	}

	checkErr(viGetAttribute(instrSession, VI_ATTR_RSRC_MANF_NAME, manufacturer));
	checkErr(viGetAttribute(instrSession, VI_ATTR_RSRC_IMPL_VERSION, &version));
	snprintf(visaInfo, bufferSize, "%s VISA %0.2f", manufacturer,
	         (ViReal64)(version >> 20) * 100 +
	         (ViReal64)((version >> 8) & 0xFFF) * 10 +
	         (ViReal64)(version & 0xFF) / 100.0);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GetSessionType
    Returns the session type based on which the core makes different operations
******************************************************************************************************/
ViStatus RsCoreInt_GetSessionType(ViSession instrSession, RsCoreSessionType* sessionType)
{
	ViStatus error = VI_SUCCESS;
	ViUInt16 intf;
	ViChar rsrcClass[256] = "";

	checkErr(viGetAttribute(instrSession, VI_ATTR_INTF_TYPE, &intf));
	switch (intf)
	{
	case VI_INTF_TCPIP:
		checkErr(viGetAttribute(instrSession, VI_ATTR_RSRC_CLASS, rsrcClass));
		if (strcmp(rsrcClass, "SOCKET") == 0)
			*sessionType = RS_INTF_SOCKET;
		else
			*sessionType = RS_INTF_TCPIP;

		break;

	case VI_INTF_GPIB:
	case VI_INTF_GPIB_VXI:
		*sessionType = RS_INTF_GPIB;
		break;

	case VI_INTF_ASRL:
		*sessionType = RS_INTF_ASRL;
		break;

	case VI_INTF_USB:
		*sessionType = RS_INTF_USB;
		break;
	default:
		*sessionType = RS_INTF_UNKNOWN;
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_SpecifyVisaError
    Describes VISA error in more details
******************************************************************************************************/
ViStatus RsCoreInt_SpecifyVisaError(ViSession instrSession, ViStatus error, ViConstString context, ViConstString sentCmd)
{
	ViUInt32 visaTimeoutMs = 10000;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar cmdString[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViBoolean patternFound;

	if (error == VI_SUCCESS)
		return error;

	if (error == VI_ERROR_TMO)
	{
		// VISA timeout error, specify it more
		// But only if it has not been more specified before
		(void)RsCoreInt_CheckErrorElaboration(instrSession, "VISA Read resulted in timeout", &patternFound);
		if (patternFound)
			return error;

		(void)RsCoreInt_CheckErrorElaboration(instrSession, "Waiting for OPC resulted in timeout", &patternFound);
		if (patternFound)
			return error;

		(void)RsCore_GetVisaTimeout(instrSession, &visaTimeoutMs);
		if (!isNullOrEmpty(sentCmd))
		{
			RsCoreInt_StrcpyMaxLen(cmdString, RS_MAX_MESSAGE_BUF_SIZE, sentCmd);
			RsCore_TrimString(cmdString, RS_VAL_TRIM_WHITESPACES);
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, " Sent command: \"%s\". ", cmdString);
			RsCoreInt_StrcpyMaxLen(cmdString, RS_MAX_MESSAGE_BUF_SIZE, errElab);
		}

		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
		         "%s VISA Read resulted in timeout.%s"
		         " This usually means the instrument is not responding to a query. "
		         "Current VISA timeout is set to " _PERCU " ms. You can change it with the "
		         "PREFIX_SetVisaTimeout.", isNullOrEmpty(context) ? "" : context, cmdString, visaTimeoutMs);
		(void)RsCore_SetErrorInfo(instrSession, VI_TRUE, VI_ERROR_TMO, VI_SUCCESS, errElab);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_ClearBeforeRead
    Puts the instrument's status subsystem and its IO buffer to a defined status
******************************************************************************************************/
ViStatus RsCoreInt_ClearBeforeRead(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 stb = 0, previous_stb;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	// STB read is determined by the fastSweepInstrument
	if (rsSession->vxiCapable == VI_TRUE)
	{
		if (rsSession->fastSweepInstrument > 0)
		{
			checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));
		}
		else
		{
			checkErr(RsCore_QueryViInt32(instrSession, "*STB?", &stb));
		}
	}
	else
	{
		// Non-VXI session must use *CLS in any case
		checkErr(RsCore_Write(instrSession, "*CLS"));
		checkErr(RsCore_QueryViInt32(instrSession, "*OPC?", NULL));
		checkErr(RsCore_QueryViInt32(instrSession, "*STB?", &stb));
	}

	// ErrorQueueNotEmpty or EventStatusByte or MessageAvailable
	while (stb & 0x34)
	{
		if ((stb & IEEE_488_2_STB_ERR_QUEUE_MASK) > 0)
		{
			// ErrorQueueNotEmpty
			checkErr(RsCore_Write(instrSession, "*CLS"));
			checkErr(RsCore_ErrorQueryAll(instrSession, NULL, RS_MAX_MESSAGE_BUF_SIZE, buffer));
		}

		if ((stb & IEEE_488_2_STB_ESB_MASK) > 0)
		{
			// EventStatusByte
			checkErr(RsCore_Write(instrSession, "*CLS"));
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}

		if ((stb & IEEE_488_2_STB_MAV_MASK) > 0)
		{
			// Message available
			checkErr(RsCoreInt_FlushData(instrSession, VI_TRUE));
		}

		previous_stb = stb;
		// STB read is determined by the fastSweepInstrument
		if (rsSession->fastSweepInstrument > 0 && rsSession->vxiCapable > 0)
		{
			checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));
		}
		else
		{
			checkErr(RsCore_QueryViInt32(instrSession, "*STB?", &stb));
		}

		if (stb == previous_stb)
		{
			// Cannot clear the StatusRegister
			snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, "Cannot clear the instrument status. Last STB value: " _PERCD, stb);
			viCheckErrElab(RS_ERROR_CANNOT_RECOVER, buffer);
			break;
		}
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  Queries and Clears the ESR register
******************************************************************************************************/
ViStatus RsCoreInt_QueryClearEsr(ViSession instrSession)
{
	return RsCore_QueryViStringShort(instrSession, "*ESR?", NULL);
}

/*****************************************************************************************************/
/*  If the input opcTimeoutMs is > 0, it is not changed
    If the input opcTimeoutMs is <= 0, it is changed to the current value of RS_ATTR_OPC_TIMEOUT
******************************************************************************************************/
ViStatus RsCoreInt_ResolveOpcTimeout(ViSession instrSession, ViInt32* opcTimeoutMs)
{
	if (*opcTimeoutMs > 0)
	{
		return VI_SUCCESS;
	}

	return RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, opcTimeoutMs);
}

/*****************************************************************************************************/
/*  Return Status byte values.
    For VXI11-capable sessions it uses viReadSTB(), for others it uses *STB? query
******************************************************************************************************/
ViStatus RsCoreInt_GetStatusByte(ViSession instrSession, ViInt32* stb, ViBoolean blockTmoSettings)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 oldVisaTout = 0;
	ViUInt16 stbInt16;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->vxiCapable)
	{
		if (rsSession->viReadStbVisaTimeoutMs > 0 && blockTmoSettings == VI_FALSE)
		{
			checkErr(viGetAttribute(instrSession, VI_ATTR_TMO_VALUE, &oldVisaTout));
			checkErr(viSetAttribute(instrSession, VI_ATTR_TMO_VALUE, rsSession->viReadStbVisaTimeoutMs));
		}
		error = viReadSTB(instrSession, &stbInt16);
		*stb = (ViInt32)stbInt16;
	}
	else
	{
		*stb = 0;
		checkErr(RsCore_QueryViStringShort(instrSession, "*STB?", response));
		*stb = (ViInt32)atoi(response);
	}

Error:
	if (oldVisaTout > 0)
	{
		checkErr(viSetAttribute(instrSession, VI_ATTR_TMO_VALUE, oldVisaTout));
	}

	return error;
}

/*****************************************************************************************************/
/*  Rebuilds the entered instrument options string with the following steps:
    - based on the optionsParsingMode it keeps parts before or after dash
    - trims all elements for white spaces and paired quotation marks
    - removes duplicates
    - sorts the options in the ascending order - starting with K0, up to K9999 and then B0 up to B9999
******************************************************************************************************/
ViStatus RsCoreInt_ParseOptionsString(ViSession instrSession, ViChar* instrumentOptionsString, ViInt32 optionsParsingMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar* element;
	ViInt32 len;
	ViInt32 elementsCount, allocatedCount = 0;
	ViChar** optionsArray = NULL;
	ViInt32* optionsNumbersArray = NULL;
	ViInt32 idx, tokenIdx;
	ViInt32 i;
	ViInt32 minIdx;
	ViInt32 optionNumber;
	ViBoolean result;
	ViBoolean duplicate;
	ViChar auxString[RS_MAX_MESSAGE_LEN];
	ViChar before[RS_MAX_MESSAGE_LEN];
	ViChar after[RS_MAX_MESSAGE_LEN];
	ViChar token[RS_MAX_MESSAGE_LEN];

	if (instrumentOptionsString == NULL)
	{
		checkErr(RS_ERROR_NULL_POINTER);
	}

	RsCore_TrimString(instrumentOptionsString, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
	len = (ViInt32)strlen(instrumentOptionsString);
	if (len == 0)
		goto Error;

	elementsCount = RsCore_GetElementsCount(instrumentOptionsString, ',');

	// Allocate optionsArray[] of ViChar* pointers;
	checkAlloc(optionsArray = (ViChar **)calloc(elementsCount, sizeof(ViChar*)));
	allocatedCount = elementsCount;

	// loop through the elements, fill the optionsArray[], remove duplicates
	idx = 0;
	for (tokenIdx = 0; tokenIdx < elementsCount; tokenIdx++)
	{
		checkErr(RsCore_GetTokenAtIndex(instrumentOptionsString, ',', tokenIdx, token, RS_MAX_MESSAGE_LEN, NULL));
		element = token;
		if (optionsParsingMode == RS_VAL_OPTIONS_PARSING_KEEP_ORIGINAL)
		{
			RsCoreInt_SplitTokenToNameAndValue(token, ":", auxString, after, RS_MAX_MESSAGE_LEN);
			element = auxString;
		}
		else if (optionsParsingMode == RS_VAL_OPTIONS_PARSING_KEEP_BEFORE_DASH)
		{
			RsCoreInt_SplitTokenToNameAndValue(token, "-", before, after, RS_MAX_MESSAGE_LEN); // take before in any case

			// Remove the parts after ':'
			RsCoreInt_SplitTokenToNameAndValue(before, ":", auxString, after, RS_MAX_MESSAGE_LEN);
			element = auxString;
		}
		else if (optionsParsingMode == RS_VAL_OPTIONS_PARSING_KEEP_AFTER_DASH)
		{
			result = RsCoreInt_SplitTokenToNameAndValue(token, "-", before, after, RS_MAX_MESSAGE_LEN);
			if (result) // Dash found, take the part after
			{
				// Remove the parts after ':'
				RsCoreInt_SplitTokenToNameAndValue(after, ":", auxString, before, RS_MAX_MESSAGE_LEN);
				element = auxString;
			}
			else // Dash not found, take the whole string stored in before
			{
				// Remove the parts after ':'
				RsCoreInt_SplitTokenToNameAndValue(before, ":", auxString, after, RS_MAX_MESSAGE_LEN);
				element = auxString;
			}
		}
		else if (optionsParsingMode == RS_VAL_OPTIONS_PARSING_AUTO)
		{
			ViInt32 beforeScanned, afterScanned;
			
			// Remove the parts after ':'
			result = RsCoreInt_SplitTokenToNameAndValue(token, ":", auxString, after, RS_MAX_MESSAGE_LEN);
			if (result) // ':' found, take the part before
			{
				element = auxString;
			}
			else
			{
				element = token;
			}
			result = RsCoreInt_SplitTokenToNameAndValue(element, "-", before, after, RS_MAX_MESSAGE_LEN);
			if (result) // Dash found, pick either before or after
			{
				beforeScanned = sscanf(before, "%[KB]" _PERCD, auxString, &optionNumber);
				afterScanned = sscanf(after, "%[KB]" _PERCD, auxString, &optionNumber);
				
				// Prefer element after
				element = after;
				if (beforeScanned > afterScanned)
				{
					element = before;
				}
			}
		}

		RsCore_TrimString(element, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		if (strlen(element) == 0)
			continue;

		// Detect duplicates, only add to the optionsArray if not already there
		duplicate = VI_FALSE;
		for (i = 0; i < idx; i++)
		{
			if (strcmp(element, optionsArray[i]) == 0)
			{
				duplicate = VI_TRUE;
				break;
			}
		}

		if (!duplicate)
		{
			checkAlloc(optionsArray[idx] = (ViChar*)calloc(strlen(element) + 1, sizeof(ViChar)));
			RsCoreInt_StrcpyMaxLen(optionsArray[idx], (ViInt32)strlen(element) + 1, element);
			idx++;
		}
	}

	// New elements count after removing duplicates
	elementsCount = idx;

	// Allocate optionsNumbersArray[] of ViInt32 values used for sorting
	checkAlloc(optionsNumbersArray = (ViInt32*)calloc(elementsCount, sizeof(ViInt32)));
	for (idx = 0; idx < elementsCount; idx++)
	{
		optionNumber = 0;
		i = sscanf(optionsArray[idx], "%[KB]" _PERCD, auxString, &optionNumber);
		if (i == 0) // Scanned incorrectly
			// Put to the very end
			optionNumber += 2000000;
		else if (i > 0) // K/B scanned correctly
			// Offset to put B-options behind K-options
			if (strcmp(auxString, "B") == 0) optionNumber += 1000000;

		optionsNumbersArray[idx] = optionNumber;
	}

	// Pick one by one starting from the smallest optionNumber, rebuild the instrumentOptionsString
	memset(instrumentOptionsString, 0, len + 1);
	for (idx = 0; idx < elementsCount; idx++)
	{
		// Find the minimum
		optionNumber = 10000000;
		minIdx = 1;
		for (i = 0; i < elementsCount; i++)
		{
			if (optionNumber > optionsNumbersArray[i])
			{
				optionNumber = optionsNumbersArray[i];
				minIdx = i;
			}
		}

		optionsNumbersArray[minIdx] = 10000000; // Invalidate the value
		RsCore_AppendToCsvString(instrumentOptionsString, (ViChar*)",", len + 1, optionsArray[minIdx]);
	}

Error:
	// Free allocated resources
	if (optionsArray)
		for (idx = 0; idx < allocatedCount; idx++)
			if (optionsArray[idx])
				free(optionsArray[idx]);

	if (optionsArray)
		free(optionsArray);

	if (optionsNumbersArray)
		free(optionsNumbersArray);

	return error;
}

/*****************************************************************************************************/
/*  Waits for OPC by STB polling
******************************************************************************************************/
ViStatus RsCoreInt_WaitForOpcSTBpoll(ViSession instrSession, ViInt32 timeoutMs)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 stb = 0;
	ViReal64 opcTimeoutMs;
	ViInt32 oldVisaTout = 0;
	ViReal64 elapsedTimeMs;
	ViReal64 ms_divider = (ViReal64)CLOCKS_PER_SEC / (ViReal64)1000.0;
	clock_t start_time = clock();
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession) == VI_TRUE)
		goto Error;

	opcTimeoutMs = (ViReal64)timeoutMs;

	if (rsSession->vxiCapable && rsSession->viReadStbVisaTimeoutMs > 0)
	{
		// Will use the viReadSTB, set the VISA timeout here
		checkErr(viGetAttribute(instrSession, VI_ATTR_TMO_VALUE, &oldVisaTout));
		checkErr(viSetAttribute(instrSession, VI_ATTR_TMO_VALUE, rsSession->viReadStbVisaTimeoutMs));
	}

	do
	{
		checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_TRUE));
		if ((stb & IEEE_488_2_STB_ESB_MASK) != 0)
			break;

		if ((stb & IEEE_488_2_STB_ERR_QUEUE_MASK) > 0)
		{
			error = RS_ERROR_INSTRUMENT_STATUS;
			goto Error;
		}

		elapsedTimeMs = (ViReal64)(clock() - start_time) / ms_divider;
		if (elapsedTimeMs > opcTimeoutMs)
		{
			error = VI_ERROR_TMO;
			goto Error;
		}

		RsCoreInt_PollingDelay(elapsedTimeMs, rsSession->opcWaitMode);
	}
	while ((stb & IEEE_488_2_STB_ESB_MASK) == 0);

Error:
	if (oldVisaTout > 0)
	{
		(void)(RsCore_SetVisaTimeout(instrSession, oldVisaTout));
	}

	return error;
}

/*****************************************************************************************************/
/*  Waiting for OPC with Wait for Service Request
******************************************************************************************************/
ViStatus RsCoreInt_WaitForOpcSRQ(ViSession instrSession, ViInt32 timeoutMs)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 stb = 0;

	if (RsCore_Simulating(instrSession) == VI_TRUE)
		return error;

	error = viWaitOnEvent(instrSession, VI_EVENT_SERVICE_REQ, timeoutMs, VI_NULL, VI_NULL);
	if (error == VI_SUCCESS)
	{
		checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));
	}
	else if (error == VI_ERROR_TMO)
	{
		checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));
		error = VI_ERROR_TMO;
	}

Error:
	(void)viDisableEvent(instrSession, VI_EVENT_SERVICE_REQ, VI_QUEUE); // No error handling
	if ((stb & IEEE_488_2_STB_ERR_QUEUE_MASK) > 0)
	{
		error = RS_ERROR_INSTRUMENT_STATUS;
	}

	return error;
}

/*****************************************************************************************************/
/*  Reads and discards all the data from the instrument input buffer.
    If totalFlush is TRUE, the method tries to completely flush the data.
    If totalFlush is FALSE, after several reads returns an error.
******************************************************************************************************/
ViStatus RsCoreInt_FlushData(ViSession instrSession, ViBoolean totalFlush)
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	ViUInt32 retCnt, len;
	ViBoolean moreDataAvailable;
	ViInt32 i; RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->readDelay)
		Sleep(rsSession->readDelay);

	len = 8;
	i = 0;
	moreDataAvailable = VI_TRUE;
	while (moreDataAvailable == VI_TRUE)
	{
		checkErr(viRead(instrSession, (ViBuf)buffer, len, &retCnt));
		len *= 2;
		if (len > RS_MAX_MESSAGE_BUF_SIZE)
			len = RS_MAX_MESSAGE_BUF_SIZE;

		moreDataAvailable = (error == VI_SUCCESS_MAX_CNT);
		if (!moreDataAvailable && retCnt == len)
		{
			if (buffer[len - 1] != '\n')
				moreDataAvailable = VI_TRUE;
		}

		i++;
		if (totalFlush == VI_FALSE && i > 100)
			break;
	}

	if (moreDataAvailable == VI_TRUE)
	{
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, "FlushJunkData: data length is too big to be completely read from the instrument.");
		viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, buffer);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  Parses binary data block header and returns length of the following binary data block.
    If you provide dataPartRead buffer, in case of non-binary data
    the function returns the null-terminated sting already read out, so you can concatenate
    it for further reading. The dataPartRead buffer must be at least 30 bytes long
******************************************************************************************************/
ViStatus RsCoreInt_ParseBinaryDataHeader(ViSession instrSession, ViBoolean errorIfNotBinDataBlock,
                                         ViBoolean* isBinDataBlock, ViInt64* length, ViChar* dataPartRead)
{
	ViStatus error = VI_SUCCESS;
	ViChar header[100];
	ViUInt32 retCnt;
	ViBoolean binDataBlockDetected = VI_FALSE;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	// Only apply the read delay once at the beginning
	if (rsSession->readDelay)
		Sleep(rsSession->readDelay);

	checkErr(viRead(instrSession, (ViBuf)header, 1, &retCnt));
	if (header[0] == '#')
	{
		if (dataPartRead)
			dataPartRead[0] = 0;

		// Binary data confirmed
		binDataBlockDetected = VI_TRUE;
		checkErr(viRead(instrSession, (ViBuf)header, 1, &retCnt));
		if (header[0] == '(')
		{
			// format for > 1E9 bytes: '#(1234567890123)<data>'
			ViInt32 count;
			checkErr(RsCore_ReadUpToCharacter(instrSession, ')', 100, (ViBuf)header, &count));
			header[count] = 0;
			sscanf(header,  _PERCLD, length);
		}
		else if (header[0] == '0')
		{
			*length = -1; // long response, unknown length
		}
		else
		{
			// classic format for < 1E9 bytes: '#9123456789...'
			ViInt32 lenOfLen;
			header[1] = '\0';
			lenOfLen = (ViInt32)atoi(header);
			checkErr(viRead(instrSession, (ViBuf)header, lenOfLen, &retCnt));
			header[retCnt] = '\0';
			sscanf(header,  _PERCLD, length);
		}
	}
	else
	{
		// Non-binary data
		binDataBlockDetected = VI_FALSE;
		*length = -2; //unrecognized binary header, treat it like an ASCII transfer
		if (errorIfNotBinDataBlock)
		{
			// Read more bytes to compose an error message
			checkErr(viRead(instrSession, (ViBuf)(header + 1), 24, &retCnt));
			if (dataPartRead)
			{
				header[25] = 0;
				RsCoreInt_StrcpyMaxLen(dataPartRead, 25, header);
			}

			RsCore_TrimString(header, RS_VAL_TRIM_WHITESPACES);
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Expected Binary data header starting with #(hash),"
			         " received data '%s'", header);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
		else
		{
			// return null-terminated string of the already read data
			RsCoreInt_StrcpyMaxLen(dataPartRead, 2, header);
		}
	}

	if (isBinDataBlock)
		*isBinDataBlock = binDataBlockDetected;

Error:
	return error;
}

/*****************************************************************************************************/
/*  Sends binary data header based on the entered dataSize. The function expects that you continue
    sending the corresponding binary data block, therefore it also sets VI_ATTR_SEND_END_EN to FALSE
    !!!Important!!! - send the VI_ATTR_SEND_END_EN back to TRUE after sending the binary data block
******************************************************************************************************/
ViStatus RsCoreInt_SendBinDataHeader(ViSession instrSession, ViConstString command, ViInt64 dataSize)
{
	ViStatus error = VI_SUCCESS;
	ViChar header[RS_MAX_MESSAGE_LEN];
	ViInt32 lenOfLen;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));


	lenOfLen = snprintf(header, RS_MAX_MESSAGE_LEN,  _PERCLD, dataSize);
	snprintf(header, RS_MAX_MESSAGE_LEN, "%s" _PERCLD, command, dataSize);
	if (lenOfLen < 10)
	{
		// #512345
		snprintf(header, RS_MAX_MESSAGE_LEN, "%s#" _PERCU _PERCLD, command, lenOfLen, dataSize);
	}
	else
	{
		// #(3000000000)
		snprintf(header, RS_MAX_MESSAGE_LEN, "%s#(" _PERCLD ")", command, dataSize);
	}

	// send header
	if (rsSession->writeDelay)
		Sleep(rsSession->writeDelay);

	(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_FALSE);
	checkErr(viWrite(instrSession, (ViBuf)header, (ViInt32)strlen(header), NULL));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_GenerateOpcTimeoutError
    Internal function that generates OPC timeout error if the error == VI_ERROR_TMO
******************************************************************************************************/
ViStatus RsCoreInt_GenerateOpcTimeoutError(ViSession instrSession, ViStatus error, ViInt32 opcTimeoutMs)
{
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession;
	ViUInt32 visaTimeout = 10000;
	ViInt32 sessionOpcTimeoutMs;
	ViInt32 len;

	if (error == VI_ERROR_TMO)
	{
		(void)RsCore_GetRsSession(instrSession, &rsSession);
		(void)RsCore_GetVisaTimeout(instrSession, &visaTimeout);
		(void)RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, &sessionOpcTimeoutMs);

		(void)RsCoreInt_ResolveOpcTimeout(instrSession, &opcTimeoutMs);
		if (opcTimeoutMs == sessionOpcTimeoutMs)
		{
			// Entered OPC timeout is equal to the session's OPC timeout
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Waiting for OPC resulted in timeout. OPC timeout is set to " _PERCD " ms. "
			         "Change it with %s_SetTimeout / %s_SetOpcTimeout. "
			         "Note, that the OPC Timeout is not the same as VISA Timeout.",
			         opcTimeoutMs, rsSession->specificPrefix, rsSession->specificPrefix);
		}
		else
		{
			// Custom OPC timeout
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Waiting for OPC resulted in timeout. Entered timeout: " _PERCD " ms. "
			         "Note, that the OPC Timeout is not the same as VISA Timeout.", opcTimeoutMs);
		}

		if (rsSession->vxiCapable == VI_TRUE && rsSession->opcWaitMode != RS_VAL_OPCWAIT_OPCQUERY)
		{
			// Add VISA timeout value info
			len = (ViInt32)strlen(errElab);
			snprintf(errElab + len, RS_MAX_MESSAGE_BUF_SIZE - len, " VISA timeout current value: " _PERCD " ms.", visaTimeout);
		}

		(void)RsCore_SetErrorInfo(instrSession, VI_TRUE, VI_ERROR_TMO, VI_FALSE, errElab);
	}

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- INTERNAL VISA IO R/W ----------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/******************************************************************************************************
* Function: RsCoreInt_Read
* Purpose:  Reads data from the instrument using VISA viRead()
******************************************************************************************************/
ViStatus RsCoreInt_Read(ViSession instrSession, ViInt32 bufferSize, ViByte* readBuffer, ViInt32* bytesRead)
{
	ViStatus error = VI_SUCCESS;
	ViUInt32 retCnt = 0;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*bytesRead = 0;
		if (bufferSize > 0)
			readBuffer[0] = 0;

		return error;
	}

	if (rsSession->readDelay)
		Sleep(rsSession->readDelay);

	error = viRead(instrSession, (ViBuf)readBuffer, bufferSize, &retCnt);

	if (error == VI_SUCCESS_TERM_CHAR)
		error = VI_SUCCESS;

	if (error == VI_ERROR_TMO)
		(void)RsCoreInt_SpecifyVisaError(instrSession, error, "RsCoreInt_Read", NULL);

	if (bytesRead)
		*bytesRead = retCnt;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_ReadDataKnownLength
    Reads data of the entered length to the provided allocated buffer
    Reading is done in chunks with maximum of rsSession->ioSegmentSize
    This function is used to read binary data blocks
******************************************************************************************************/
ViStatus RsCoreInt_ReadDataKnownLength(ViSession instrSession, ViInt64 length, ViByte* buffer)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 chunkSize;
	ViByte* p2buffer;
	ViUInt32 retCnt;
	ViInt32 stb;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*buffer = 0;
		return error;
	}

	chunkSize = rsSession->ioSegmentSize;

	// If data size is 0
	if (length == 0)
	{
		// Just flushing of the data
		if (rsSession->vxiCapable)
		{
			checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));
			if ((stb & IEEE_488_2_STB_MAV_MASK) > 0)
			{
				checkErr(RsCoreInt_FlushData(instrSession, VI_FALSE));
			}
		}
		else
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE));
			checkErr(RsCoreInt_FlushData(instrSession, VI_FALSE));
		}
	}
	else if (length < chunkSize)
	{
		// All data can be read in one segment
		checkErr(viRead(instrSession, (ViPBuf)buffer, (ViUInt32)length, &retCnt));
		if (length != retCnt)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected number of bytes read. Expected " _PERCLD " bytes, read " _PERCU " bytes", length, retCnt);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}
	else
	{
		// Segmented reading loop
		ViInt64 remaining = length;
		p2buffer = buffer;
		do
		{
			if (remaining > chunkSize)
			{
				checkErr(viRead(instrSession, (ViPBuf)p2buffer, chunkSize, &retCnt));
				remaining -= retCnt;
				p2buffer += retCnt;
			}
			else
			{
				checkErr(viRead(instrSession, (ViPBuf)p2buffer, (ViUInt32)remaining, &retCnt));
				remaining -= retCnt;
				break;
			}
		} while (1);
		if (remaining != 0)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected number of bytes read. Expected " _PERCLD " bytes, read " _PERCLD " bytes", length, length - remaining);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}

	if (error == VI_SUCCESS_MAX_CNT) // Flush remaining data
	{
		error = VI_SUCCESS;
		if (!rsSession->vxiCapable)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE));
		}

		checkErr(RsCoreInt_FlushData(instrSession, VI_TRUE));
	}

Error:
	if (error == VI_SUCCESS_TERM_CHAR)
		error = VI_SUCCESS;

	if (error == VI_ERROR_TMO)
		RsCoreInt_SpecifyVisaError(instrSession, error, "RsCoreInt_ReadDataKnownLength", NULL);

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_ReadDataUnknownLength
    Reads data from the instrument and returns the allocated buffer with the read data
    The outputBuffer is always at least +1 byte bigger than the returned byteCount, and the empty place is padded with nulls.
    Therefore, you can treat the buffer as a null-terminated string.
    WARNING!!! - Deallocate the outputBuffer after use.
    Note! Do not use for reading binary data, for that purpose use the RsCore_ReadBinaryDataBlock... functions
******************************************************************************************************/
ViStatus RsCoreInt_ReadDataUnknownLength(ViSession instrSession, ViByte** outputBuffer, ViInt64* byteCount)
{
	ViStatus error = VI_SUCCESS;
	ViUInt32 retCnt = 0;
	ViUInt32 chunkSize;
	ViByte *buffer = NULL, *p2buffer;
	ViInt64 actualSize;
	ViBoolean moreDataAvailable;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*byteCount = 0;
		viCheckAllocElab(buffer = (ViByte *)calloc((size_t)(*byteCount + 1), sizeof(ViByte)),
			"RsCore_ReadViStringUnknownLength simulation buffer allocation");
		buffer[0] = '\0';
		*outputBuffer = buffer;
		return error;
	}

	*outputBuffer = NULL;

	if (byteCount)
		*byteCount = 0;

	// Start reading with a small chunk (1024 bytes), then increase it up to rsSession->ioSegmentSize
    chunkSize = rsSession->ioSegmentSize < 1024 ? rsSession->ioSegmentSize < 1024 : 1024;
    chunkSize = chunkSize < RS_MAX_MESSAGE_BUF_SIZE ? chunkSize : RS_MAX_MESSAGE_BUF_SIZE;

	// Allocate Buffer
	viCheckAllocElab(buffer = (ViByte *)calloc(chunkSize, sizeof(ViByte)), "RsCore_ReadViStringUnknownLength buffer allocation");

	// Only apply the read delay once at the beginning
	if (rsSession->readDelay)
		Sleep(rsSession->readDelay);

	actualSize = 0;
	p2buffer = buffer;
	do
	{
		moreDataAvailable = VI_FALSE;
		checkErr(viRead(instrSession, (ViPBuf)p2buffer, chunkSize, &retCnt));
		actualSize += retCnt;
		if (error == VI_SUCCESS_MAX_CNT)
		{
			// VISA reports more data is available, read further
			moreDataAvailable = VI_TRUE;
		}
		else
		{
			// VISA reports no more data available. To be sure, check the returned count
			if (retCnt == (ViUInt32)chunkSize)
			{
				// Check the last read character. If it is not LF, more data is available
				if (p2buffer[retCnt - 1] != '\n')
					moreDataAvailable = VI_TRUE;
			}
		}

		if (!moreDataAvailable)
			break; // stop the reading, the data was all read

		// Increase the chunk size gradually
		chunkSize *= 4;
		if (chunkSize > (ViUInt32)rsSession->ioSegmentSize)
            chunkSize = rsSession->ioSegmentSize;

		// Reallocate the buffer, the old content stays untouched. + 1 byte to have space for a null-terminator
		viCheckAllocElab(buffer = (ViByte *)realloc(buffer, (size_t)(actualSize + chunkSize + 1)),
			"RsCore_ReadViStringUnknownLength buffer re-allocation");
		p2buffer = buffer + actualSize; /* Set pointer to end of data in reallocated buffer */
		memset(p2buffer, 0, chunkSize + 1); // fill the added part with nulls
	} while (1);

	*outputBuffer = buffer;
	if (byteCount != NULL)
		*byteCount = actualSize;

Error:
	if (error < 0)
	{
		if (buffer)
			free(buffer);

		*outputBuffer = NULL;
	}

	if (error == VI_SUCCESS_TERM_CHAR)
		error = VI_SUCCESS;

	if (error == VI_ERROR_TMO)
		RsCoreInt_SpecifyVisaError(instrSession, error, "RsCore_ReadViStringUnknownLength", NULL);

	return error;
}

/*****************************************************************************************************/
/*  Reads data of the entered length into the provided file
    Reading is done in chunks with maximum of rsSession->ioSegmentSize
    If you do not need the return value bytesWritten, set it to NULL
    This function is used to read binary data blocks
******************************************************************************************************/
ViStatus RsCoreInt_ReadDataKnownLengthToFile(ViSession instrSession, ViInt64 length, FILE* file, ViInt64* bytesWritten)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 chunkSize;
	ViChar* buffer = NULL;
	ViUInt32 retCnt;
	ViInt64 written;
	ViInt32 stb;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		if (bytesWritten)
			*bytesWritten = length;

		return error;
	}

	chunkSize = rsSession->ioSegmentSize;

	// If data size is 0
	if (length == 0)
	{
		// Just flushing of the data
		if (rsSession->vxiCapable)
		{
			checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));
			if ((stb & IEEE_488_2_STB_MAV_MASK) > 0)
			{
				checkErr(RsCoreInt_FlushData(instrSession, VI_FALSE));
			}
		}
		else
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE));
			checkErr(RsCoreInt_FlushData(instrSession, VI_FALSE));
		}
	}
	else if (length < chunkSize)
	{
		// All data can be read in one segment
		viCheckAllocElab(buffer = (ViChar *)malloc((ViInt32)length), "ReadDataKnownLengthToFile: Allocation of a buffer for reading to file");
		checkErr(viRead(instrSession, (ViPBuf)buffer, (ViUInt32)length, &retCnt));

		if (length != retCnt)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected number of bytes read. Expected " _PERCLD " bytes, read " _PERCU " bytes", length, retCnt);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}

		written = (ViInt32)fwrite(buffer, sizeof(ViByte), (size_t)retCnt, file);
		if (length != written)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected number of bytes written to file. Expected " _PERCLD " bytes, written " _PERCLD " bytes", length, written);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}

		if (bytesWritten)
			*bytesWritten = length;
	}
	else
	{
		// Segmented reading loop
		viCheckAllocElab(buffer = (ViChar *)malloc(chunkSize), "ReadDataKnownLengthToFile: Allocation of a chunk-size buffer for reading to file");
		ViInt64 remaining = length;
		do
		{
			if (remaining > chunkSize)
			{
				checkErr(viRead(instrSession, (ViPBuf)buffer, chunkSize, &retCnt));
				written = (ViInt32)fwrite(buffer, sizeof(ViByte), (size_t)retCnt, file);
				remaining -= written;
			}
			else
			{
				checkErr(viRead(instrSession, (ViPBuf)buffer, (ViUInt32)remaining, &retCnt));
				written = (ViInt32)fwrite(buffer, sizeof(ViByte), (size_t)retCnt, file);
				remaining -= written;
				break;
			}
		} while (1);

		if (bytesWritten)
			*bytesWritten = length - remaining;

		if (remaining != 0)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected number of bytes written to file. Expected " _PERCLD " bytes, written " _PERCLD " bytes", length, length - remaining);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}

	if (error == VI_SUCCESS_MAX_CNT) // flush remaining data
	{
		error = VI_SUCCESS;
		if (!rsSession->vxiCapable)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE));
		}
		checkErr(RsCoreInt_FlushData(instrSession, VI_FALSE));
	}

Error:
	if (buffer != NULL)
		free(buffer);

	if (error == VI_SUCCESS_TERM_CHAR)
		error = VI_SUCCESS;

	if (error == VI_ERROR_TMO)
		RsCoreInt_SpecifyVisaError(instrSession, error, "Rs_ReadDataUnknownLength", NULL);

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_ReadBinaryDataBlock
    Reads binary data of unknown size from the instrument.
    If errorIfNotBinDataBlock == TRUE, the function generates error in case the
    data is not binary block (not starting with '#')
    If errorIfNotBinDataBlock == FALSE, the function returns read data and indicates isBinDataBlock = FALSE.
    If you do not need the isBinDataBlock info, set the pointer to NULL
    WARNING!!! Deallocate the outputBuffer after use
******************************************************************************************************/
ViStatus RsCoreInt_ReadBinaryDataBlock(ViSession instrSession,
                                       ViBoolean errorIfNotBinDataBlock, ViBoolean* isBinDataBlock,
                                       ViByte** outputBuffer, ViInt64* byteCount)
{
	ViStatus error = VI_SUCCESS;
	ViInt64 length = 0;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar *stringBuffer = NULL, *completeStringBuffer = NULL;
	ViBoolean binDataBlockDetected = VI_FALSE;
	RsCoreSessionPtr rsSession = NULL;
	
	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	*outputBuffer = NULL;
	*byteCount = 0;
	checkErr(RsCoreInt_ParseBinaryDataHeader(instrSession, errorIfNotBinDataBlock, &binDataBlockDetected, &length, buffer));
	if (length > 2147483646)
	{
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE,
		         "ReadBinaryDataBlock: parsed length " _PERCLD " is bigger than maximum supported length of Int32 Max 2147483646 bytes", length);
		viCheckErrElab(RS_ERROR_DATA_TOO_LONG, buffer);
	}

	if (binDataBlockDetected == VI_FALSE)
	{
		// Unrecognized binary header, treat it like an ASCII transfer
		checkErr(RsCore_ReadViStringUnknownLength(instrSession, &stringBuffer));
		checkErr(RsCore_StrCatDynamicString(instrSession, &completeStringBuffer, buffer, stringBuffer, NULL));
		*outputBuffer = (ViByte*)completeStringBuffer;
		*byteCount = (ViInt64)strlen(completeStringBuffer);
		goto Error;
	}

	if (rsSession->vxiCapable != VI_TRUE)
	{
		// Confirmed binary transfer, switch the read termination character OFF
		checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));
	}

	if (length == 0) // Indicated null data size, read and discard all that is available
	{
		viCheckAllocElab(*outputBuffer = (ViByte*)malloc(1), "ReadBinaryDataBlock: Allocation of buffer for empty binary data.");
		*byteCount = 1;
		checkErr(RsCoreInt_ReadDataKnownLength(instrSession, 1, *outputBuffer));
	}
	else if (length > 0) // Binary known length
	{
		viCheckAllocElab(*outputBuffer = (ViByte*)malloc((size_t)(length + 1)), "ReadBinaryDataBlock: Allocation of buffer for binary data of known size.");
		checkErr(RsCoreInt_ReadDataKnownLength(instrSession, length, *outputBuffer));
		*byteCount = length;
	}
	else if (length == -1) // Long data of unknown size, treat it like data unknown size
	{
		if (rsSession->vxiCapable == VI_FALSE)
		{
			viCheckErrElab(RS_ERROR_SESSION_TYPE_NOT_SUPPORTED,
				"ReadBinaryDataBlock: Binary data block with unknown length is not supported on non-VXI11 interfaces");
		}

		checkErr(RsCoreInt_ReadDataUnknownLength(instrSession, outputBuffer, byteCount));
	}

Error:
	if (stringBuffer)
		free(stringBuffer);

	if (isBinDataBlock)
		*isBinDataBlock = binDataBlockDetected;

	if (rsSession->vxiCapable != VI_TRUE)
	{
		viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE);
	}

	return error;
}

/*****************************************************************************************************/
/*---------------------------------------------------------------------------------------------------*/
/*---- INTERNAL VISA IO R/W with OPC sync -----------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*****************************************************************************************************/

/*****************************************************************************************************/
/*  RsCoreInt_WriteWithOpc
    Internal function to write a command with OPC. The command length is unlimited
    Set the parameter 'timeoutMs' to 0 in order to use the current session's OPC timeout.
    You can not use this function to send a query for non-VXI11 sessions or WaitOpcMode=OpcQuery
******************************************************************************************************/
ViStatus RsCoreInt_WriteWithOpc(ViSession instrSession, ViConstString command,
                                ViBoolean allowCheckStatus, ViInt32 opcTimeoutMs)
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* dynBuffer = NULL;
	ViChar* p2SendBuffer;
	ViBoolean hasLF, tooLong;
	ViInt32 cmdLen, bufferSize;
	RsCoreOpcAttrCallback p2opcFunction;
	void* value;
	ViUInt32 oldVisaTimeoutMs = 0;
	ViBoolean clearEsr = VI_TRUE;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (command == NULL)
	{
		viCheckErrElab(RS_ERROR_INVALID_VALUE, "RsCoreInt_WriteWithOpc: input parameter command is NULL");
	}

	cmdLen = (ViInt32)strlen(command);
	if (cmdLen == 0)
	{
		viCheckErrElab(RS_ERROR_INVALID_VALUE, "RsCoreInt_WriteWithOpc: input parameter command string is empty");
	}

	checkErr(RsCoreInt_ResolveOpcTimeout(instrSession, &opcTimeoutMs));
	hasLF = (command[cmdLen - 1] == '\n');

	// We need a longer buffer to append '*OPC'
	bufferSize = cmdLen + 10;

	tooLong = (bufferSize > RS_MAX_MESSAGE_BUF_SIZE);
	if (rsSession->opcWaitMode != RS_VAL_OPCWAIT_OPCQUERY)
	{
		// For commands (not queries), all OPC modes except the OPCquery can use the same procedure
		if (tooLong)
		{
			// Command is too long, we need to allocate the buffer dynamically
			viCheckAllocElab(dynBuffer = (ViChar *)malloc(bufferSize), "RsCoreInt_WriteWithOpc: Allocation of a dynamic buffer for long command");
			p2SendBuffer = dynBuffer;
		}
		else
		{
			p2SendBuffer = buffer;
			bufferSize = RS_MAX_MESSAGE_BUF_SIZE;
		}

		RsCoreInt_StrcpyMaxLen(p2SendBuffer, bufferSize, command);
		if (hasLF)
		{
			RsCore_TrimString(p2SendBuffer, RS_VAL_TRIM_WHITESPACES);
		}

		if (rsSession->assureWriteWithLF == VI_TRUE)
			RsCore_StrcatMaxLen(p2SendBuffer, bufferSize, ";*OPC\n", NULL, NULL);
		else
			RsCore_StrcatMaxLen(p2SendBuffer, bufferSize, ";*OPC", NULL, NULL);

		checkErr(RsCoreInt_ClearBeforeRead(instrSession));

		// For Service request, clear and allow the SRQ event
		if (rsSession->opcWaitMode == RS_VAL_OPCWAIT_SERVICEREQUEST)
		{
			checkErr(viDiscardEvents(instrSession, VI_EVENT_SERVICE_REQ, VI_QUEUE | VI_SUSPEND_HNDLR));
			checkErr(viEnableEvent(instrSession, VI_EVENT_SERVICE_REQ, VI_QUEUE, VI_NULL));
		}

		checkErr(RsCore_Write(instrSession, p2SendBuffer));
		checkErr(RsCoreInt_GetAttrP2Value(rsSession->attrOpcCallbackPtr, &value, 0));
		if (value == NULL)
		{
			checkStatusOnErr(RsCore_WaitForOpcCallback(instrSession, opcTimeoutMs));
		}
		else
		{
			p2opcFunction = (RsCoreOpcAttrCallback)(ViAddr *)value;
			checkStatusOnErr(p2opcFunction(instrSession, opcTimeoutMs));
		}
	}
	else
	{
		// OPC query mode
		clearEsr = VI_FALSE;
		checkErr(RsCoreInt_ClearBeforeRead(instrSession));
		checkErr(RsCore_Write(instrSession, command));
		checkErr(RsCore_GetVisaTimeout(instrSession, &oldVisaTimeoutMs));
		checkErr(RsCore_SetVisaTimeout(instrSession, opcTimeoutMs));

		error = RsCore_QueryViStringShort(instrSession, "*OPC?", NULL);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		checkErr(RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs));
		oldVisaTimeoutMs = 0;
	}

CheckStatus:
	// Check Status if allowed
	if (allowCheckStatus == VI_FALSE)
		goto Error;

	if (RsCore_QueryInstrStatus(instrSession) == VI_TRUE)
	{
		checkErr(RsCoreInt_CheckStatusAfterOpcSync(instrSession, error));
	}

	if (clearEsr)
	{
		if (error == VI_ERROR_TMO)
		{
			(void)RsCoreInt_QueryClearEsr(instrSession);
		}
		else
		{
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}
	}

Error:
	if (dynBuffer)
		free(dynBuffer);

	if (oldVisaTimeoutMs > 0)
	{
		(void)RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_QueryViStringWithOpc
    Queries instrument for a string response with OPC. The response length is limited by the bufferSize
    The response is trimmed for LF and terminated with '\0'
    Set the parameter 'timeoutMs' to 0 in order to use the current session's OPC timeout.
******************************************************************************************************/
ViStatus RsCoreInt_QueryViStringWithOpc(ViSession instrSession, ViConstString query, ViBoolean allowCheckStatus,
                                        ViInt32 opcTimeoutMs, ViInt32 bufferSize, ViChar* responseString)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 returnedCount;
	ViUInt32 oldVisaTimeoutMs = 0;
	ViBoolean clearEsr = VI_TRUE;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	checkErr(RsCoreInt_ResolveOpcTimeout(instrSession, &opcTimeoutMs));

	if (rsSession->vxiCapable && rsSession->opcWaitMode != RS_VAL_OPCWAIT_OPCQUERY)
	{
		// For VXI-capable session, use the viReadSTB poll or SRQ wait and then read the response
		checkStatusOnErr(RsCoreInt_WriteWithOpc(instrSession, query, VI_FALSE, opcTimeoutMs));
		checkStatusOnErr(RsCoreInt_Read(instrSession, bufferSize, (ViByte*)responseString, &returnedCount));
	}
	else
	{
		// For Non-VXI-capable sessions or OpcWaitMode OpcQuery we use standard query with VISA timeout = OPC timeout
		clearEsr = VI_FALSE;
		checkErr(RsCoreInt_ClearBeforeRead(instrSession));
		checkErr(RsCore_Write(instrSession, query));
		checkErr(RsCore_GetVisaTimeout(instrSession, &oldVisaTimeoutMs));
		checkErr(RsCore_SetVisaTimeout(instrSession, opcTimeoutMs));

		error = RsCoreInt_Read(instrSession, bufferSize, (ViByte*)responseString, &returnedCount);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		error = RsCore_QueryViStringShort(instrSession, "*OPC?", NULL);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		checkErr(RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs));
		oldVisaTimeoutMs = 0;
	}

	responseString[returnedCount - 1] = 0;
	RsCore_TrimString(responseString, RS_VAL_TRIM_WHITESPACES);

CheckStatus:
	// Check Status if allowed
	if (allowCheckStatus == VI_FALSE)
		goto Error;

	if (RsCore_QueryInstrStatus(instrSession) == VI_TRUE)
	{
		checkErr(RsCoreInt_CheckStatusAfterOpcSync(instrSession, error));
	}

	if (clearEsr)
	{
		if (error == VI_ERROR_TMO)
		{
			(void)RsCoreInt_QueryClearEsr(instrSession);
		}
		else
		{
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}
	}

Error:
	if (oldVisaTimeoutMs > 0)
	{
		(void)RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs);
	}

	if (error < 0)
		RsCoreInt_SpecifyVisaError(instrSession, error, "RsCoreInt_QueryViStringWithOpc", query);

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_QueryUnknownLengthWithOpc
    Query instrument with OPC sync for a general response. The response length is unlimited.
    Returned allocated 'response' buffer is always at least 1 byte bigger than 'byteCount',
    and the response can be treated as properly null-terminated string.
    However, the end is not trimmed for LF. For reading the response, the function uses RsCore_ReadUnknownLength.
    Set the parameter 'timeoutMs' to 0 in order to use the current session's OPC timeout.
    If you do not need the returned 'byteCount', set it to NULL.
    If 'allowCheckStatus' is FALSE, CheckStatus call and ESR clear are skipped.
    Important!!! - Free the response buffer after use !!!
******************************************************************************************************/
ViStatus RsCoreInt_QueryUnknownLengthWithOpc(ViSession instrSession, ViConstString query, ViBoolean allowCheckStatus,
                                             ViInt32 opcTimeoutMs, ViByte** response, ViInt64* byteCount)
{
	ViStatus error = VI_SUCCESS;
	ViInt64 returnedCount;
	ViUInt32 oldVisaTimeoutMs = 0;
	ViBoolean clearEsr = VI_TRUE;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	checkErr(RsCoreInt_ResolveOpcTimeout(instrSession, &opcTimeoutMs));

	if (RsCore_Simulating(instrSession))
	{
		if (byteCount)
			*byteCount = 1;

		*response = (ViByte*)malloc(1);
		return error;
	}

	if (rsSession->vxiCapable && rsSession->opcWaitMode != RS_VAL_OPCWAIT_OPCQUERY)
	{
		// For VXI-capable session, use the viReadSTB poll or SRQ wait and then read the response
		checkStatusOnErr(RsCoreInt_WriteWithOpc(instrSession, query, VI_FALSE, opcTimeoutMs));
		checkStatusOnErr(RsCoreInt_ReadDataUnknownLength(instrSession, response, &returnedCount));
	}
	else
	{
		// For Non-VXI-capable sessions or OpcWaitMode OpcQuery we use standard query with VISA timeout = OPC timeout
		clearEsr = VI_FALSE;
		checkErr(RsCoreInt_ClearBeforeRead(instrSession));
		checkErr(RsCore_Write(instrSession, query));
		checkErr(RsCore_GetVisaTimeout(instrSession, &oldVisaTimeoutMs));
		checkErr(RsCore_SetVisaTimeout(instrSession, opcTimeoutMs));

		error = RsCoreInt_ReadDataUnknownLength(instrSession, response, &returnedCount);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		error = RsCore_QueryViStringShort(instrSession, "*OPC?", NULL);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		(void)RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs);
		oldVisaTimeoutMs = 0;
	}

	// Read the response unknown length

	if (byteCount)
		*byteCount = returnedCount;

CheckStatus:
	// Check Status if allowed
	if (allowCheckStatus == VI_FALSE)
		goto Error;

	if (RsCore_QueryInstrStatus(instrSession) == VI_TRUE)
	{
		checkErr(RsCoreInt_CheckStatusAfterOpcSync(instrSession, error));
	}

	if (clearEsr)
	{
		if (error == VI_ERROR_TMO)
		{
			(void)RsCoreInt_QueryClearEsr(instrSession);
		}
		else
		{
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}
	}

Error:
	if (oldVisaTimeoutMs > 0)
	{
		(void)RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs);
	}

	if (error < 0)
		(void)RsCoreInt_SpecifyVisaError(instrSession, error, "RsCoreInt_QueryUnknownLengthWithOpc", query);

	return error;
}

/*****************************************************************************************************/
/*  RsCoreInt_QueryBinaryDataBlockWithOpc
    Queries binary data of unknown size from the instrument.
    If errorIfNotBinDataBlock == TRUE, the function generates error in case the data is not a binary block (not starting with '#')
    If errorIfNotBinDataBlock == FALSE, the function returns read data and indicates isBinDataBlock = FALSE.
    If you do not need the isBinDataBlock info, set the pointer to NULL
    Set the parameter 'timeoutMs' to 0 in order to use the current session's OPC timeout.
    WARNING!!! - Free the outBinDataBlock after use
******************************************************************************************************/
ViStatus RsCoreInt_QueryBinaryDataBlockWithOpc(ViSession instrSession, ViConstString query,
                                               ViBoolean errorIfNotBinDataBlock, ViBoolean* isBinDataBlock,
                                               ViBoolean allowCheckStatus,
                                               ViInt32 opcTimeoutMs, ViByte** outBinDataBlock, ViInt64* blockSize)
{
	ViStatus error = VI_SUCCESS;
	ViInt64 returnedCount = 0;
	ViUInt32 oldVisaTimeoutMs = 0;
	ViBoolean clearEsr = VI_TRUE;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	checkErr(RsCoreInt_ResolveOpcTimeout(instrSession, &opcTimeoutMs));

	if (RsCore_Simulating(instrSession))
	{
		returnedCount = 1;
		*outBinDataBlock = (ViByte*)malloc(1);
		goto Error;
	}

	if (rsSession->vxiCapable && rsSession->opcWaitMode != RS_VAL_OPCWAIT_OPCQUERY)
	{
		// For VXI-capable session, use the viReadSTB poll or SRQ wait and then read the response
		checkStatusOnErr(RsCoreInt_WriteWithOpc(instrSession, query, VI_FALSE, opcTimeoutMs));
		checkStatusOnErr(RsCoreInt_ReadBinaryDataBlock(instrSession, errorIfNotBinDataBlock, isBinDataBlock, outBinDataBlock, &returnedCount));
	}
	else
	{
		// For Non-VXI-capable sessions or OpcWaitMode OpcQuery we use standard query with VISA timeout = OPC timeout
		clearEsr = VI_FALSE;
		checkErr(RsCoreInt_ClearBeforeRead(instrSession));
		checkErr(RsCore_Write(instrSession, query));
		checkErr(RsCore_GetVisaTimeout(instrSession, &oldVisaTimeoutMs));
		checkErr(RsCore_SetVisaTimeout(instrSession, opcTimeoutMs));

		error = RsCoreInt_ReadBinaryDataBlock(instrSession, errorIfNotBinDataBlock, isBinDataBlock, outBinDataBlock, &returnedCount);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		error = RsCore_QueryViStringShort(instrSession, "*OPC?", NULL);
		checkStatusOnErr(RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs));

		(void)RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs);
		oldVisaTimeoutMs = 0;
	}

CheckStatus:
	// Check Status if allowed
	if (allowCheckStatus == VI_FALSE)
		goto Error;

	if (RsCore_QueryInstrStatus(instrSession) == VI_TRUE)
	{
		checkErr(RsCoreInt_CheckStatusAfterOpcSync(instrSession, error));
	}

	if (clearEsr)
	{
		if (error == VI_ERROR_TMO)
		{
			(void)RsCoreInt_QueryClearEsr(instrSession);
		}
		else
		{
			checkErr(RsCoreInt_QueryClearEsr(instrSession));
		}
	}

Error:
	if (blockSize)
		*blockSize = returnedCount;

	if (oldVisaTimeoutMs > 0)
	{
		(void)RsCore_SetVisaTimeout(instrSession, oldVisaTimeoutMs);
	}

	if (error < 0)
		(void)RsCoreInt_SpecifyVisaError(instrSession, error, "RsCoreInt_QueryUnknownLengthWithOpc", query);

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- EXPORTED FUNCTIONS ------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Session - related functions ---------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_NewSpecificDriver
    Initiates/reuses VISA IO session, + adds the session structure to the global sessions list.
    This is the first function that the instrument driver calls.
******************************************************************************************************/
ViStatus RsCore_NewSpecificDriver(
	ViRsrc resourceName,
	ViConstString specificPrefix,
	ViConstString optionsString,
	RsCoreAttributePtr* attrList,
	ViInt32 writeDelay,
	ViInt32 readDelay,
	ViInt32 ioSegmentSize,
	ViInt32 opcWaitMode,
	ViInt32 opcTimeout,
	ViInt32 visaTimeout,
	ViInt32 selfTestTimeout,
	ViInt32 binaryFloatNumbersFormat,
	ViInt32 binaryIntegerNumbersFormat,
	ViPSession newSession)
{
	ViStatus error = VI_SUCCESS;
	ViChar errorElab[RS_MAX_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession;
	ViSession instrSession;
	ViInt32 flag = 0;
	ViBoolean simulate = VI_TRUE;
	ViBoolean reuseSession = VI_FALSE;

	/* --- Set "C" locale --- */
	{
		char* locale;
		locale = setlocale(LC_ALL, "C");
	}

	checkErr(RsCoreInt_ParseOptionsBefore(optionsString, &simulate, &reuseSession));
	checkErr(RsCoreInt_InitOrReuseSession(resourceName, attrList, simulate, reuseSession, &instrSession));
	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	*newSession = instrSession;

	// Default values for the rsSession
	RsCoreInt_StrcpyMaxLen(rsSession->specificPrefix, RS_MAX_SHORT_MESSAGE_BUF_SIZE, specificPrefix);
	rsSession->reuseSession = reuseSession;
	rsSession->mutex = NULL;
	rsSession->multiThreadLocking = VI_FALSE;
	rsSession->sessionType = RS_INTF_TCPIP;
	rsSession->allowViClear = VI_TRUE;
	rsSession->optionChecking = VI_TRUE; // OptionCheck has no attribute, only the rsSession field
	rsSession->fastSweepInstrument = 0;
	rsSession->writeDelay = writeDelay;
	rsSession->readDelay = readDelay;
	rsSession->ioSegmentSize = ioSegmentSize;
	rsSession->opcWaitMode = opcWaitMode;
	rsSession->selfTestTimeout = selfTestTimeout;
	rsSession->viReadStbVisaTimeoutMs = 0;
	rsSession->autoSystErrQuery = VI_FALSE;
	rsSession->sendOpcInCheckStatus = VI_FALSE;
	rsSession->recogniseLongResponses = VI_FALSE;
	rsSession->reportAttrDataTypeMismatch = VI_FALSE;
	rsSession->binaryFloatNumbersFormat = binaryFloatNumbersFormat;
	rsSession->binaryIntegerNumbersFormat = binaryIntegerNumbersFormat;
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_OPTIONS_LIST, &rsSession->attrOptionsListPtr));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_INSTRUMENT_MODEL, &rsSession->attrInstrModelPtr));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_OPC_CALLBACK, &rsSession->attrOpcCallbackPtr));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_QUERY_INSTRUMENT_STATUS, &rsSession->attrQueryInstrStatusPtr));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_RANGE_CHECK, &rsSession->attrRangeCheckPtr));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_CHECK_STATUS_CALLBACK, &rsSession->attrCheckStatusCallbackPtr));
	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_SIMULATE, &rsSession->attrSimulatePtr));
	checkErr(RsCore_SetAttributeViBoolean(instrSession, NULL, RS_ATTR_SIMULATE, 0, simulate));
	checkErr(RsCore_ClearErrorInfo(instrSession)); // Clears primaryError, secondaryError, errorElaboration


	checkErr(RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_ENGINE_MAJOR_VERSION, 0, RSCORE_ENGINE_MAJOR_VERSION));
	checkErr(RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_ENGINE_MINOR_VERSION, 0, RSCORE_ENGINE_MINOR_VERSION));
	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_ENGINE_REVISION, 0, RSCORE_ENGINE_REVISION));

	// InterfaceType determination
	if (!simulate)
	{
		checkErr(viSetAttribute(instrSession, VI_ATTR_USER_DATA, (ViAttrState)rsSession));
		checkErr(viSetAttribute(instrSession, VI_ATTR_TMO_VALUE, visaTimeout));
		checkErr(RsCoreInt_GetSessionType(instrSession, &rsSession->sessionType));

		/* Configure VISA Formatted I/O */
		checkErr(viSetBuf(instrSession, (ViUInt16)(VI_READ_BUF | VI_WRITE_BUF), 4000));
		checkErr(viSetAttribute(instrSession, VI_ATTR_WR_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));
		checkErr(viSetAttribute(instrSession, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS));

		// Settings for different interfaces
		if (rsSession->sessionType == RS_INTF_ASRL)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR, 10));
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE));
			checkErr(viSetAttribute(instrSession, VI_ATTR_ASRL_END_IN, 0));
			// In viRead the read end is controlled by ReadTermCharacterEnabled property
			checkErr(viSetAttribute(instrSession, VI_ATTR_ASRL_END_OUT, 0)); // In viWrite nothing is added to the sent data
			rsSession->vxiCapable = VI_FALSE;
			rsSession->addTermCharToWriteBinBlock = VI_TRUE;
			rsSession->assureWriteWithLF = VI_TRUE;
		}
		else if (rsSession->sessionType == RS_INTF_SOCKET)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR, 10));
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_TRUE));
			rsSession->vxiCapable = VI_FALSE;
			rsSession->addTermCharToWriteBinBlock = VI_TRUE;
			rsSession->assureWriteWithLF = VI_TRUE;
		}
		else if (rsSession->sessionType == RS_INTF_GPIB)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));
			rsSession->vxiCapable = VI_TRUE;
			rsSession->addTermCharToWriteBinBlock = VI_FALSE;
			rsSession->assureWriteWithLF = VI_FALSE;
		}
		else if (rsSession->sessionType == RS_INTF_TCPIP)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));
			rsSession->vxiCapable = VI_TRUE;
			rsSession->addTermCharToWriteBinBlock = VI_FALSE;
			rsSession->assureWriteWithLF = VI_FALSE;
		}
		else if (rsSession->sessionType == RS_INTF_USB)
		{
			checkErr(viSetAttribute(instrSession, VI_ATTR_TERMCHAR_EN, VI_FALSE));
			rsSession->vxiCapable = VI_TRUE;
			rsSession->addTermCharToWriteBinBlock = VI_FALSE;
			rsSession->assureWriteWithLF = VI_FALSE;
		}
		else
		{
			rsSession->sessionType = RS_INTF_UNKNOWN;
			snprintf(errorElab, RS_MAX_MESSAGE_BUF_SIZE, "Unrecognized interface type no. " _PERCD, rsSession->sessionType);
			viCheckErrElab(RS_ERROR_INVALID_TYPE, errorElab);
		}

		// Coerce the opcWaitMode for non-VXI session
		if ((rsSession->vxiCapable == VI_FALSE) && (rsSession->opcWaitMode == RS_VAL_OPCWAIT_SERVICEREQUEST))
		{
			rsSession->opcWaitMode = RS_VAL_OPCWAIT_STBPOLLINGSLOW;
		}
	}

	// Pass value to RS_ATTR_SPECIFIC_DRIVER_PREFIX
	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_SPECIFIC_DRIVER_PREFIX, 0, specificPrefix));

	// Set RangeCheck and InstrumentStatusCheck to true.
	checkErr(RsCore_SetAttributeViBoolean(instrSession, NULL, RS_ATTR_RANGE_CHECK, 0, VI_TRUE));
	checkErr(RsCore_SetAttributeViBoolean(instrSession, NULL, RS_ATTR_QUERY_INSTRUMENT_STATUS, 0, VI_TRUE));


	// Additional attributes setting
	checkErr(RsCore_SetAttributeViSession(instrSession, NULL, RS_ATTR_VISA_RM_SESSION, 0, rsSession->rmSession));
	checkErr(RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, opcTimeout));

	checkErr(RsCore_GetAttributeFlags(instrSession, RS_ATTR_IO_RESOURCE_DESCRIPTOR, &flag));
	checkErr(RsCore_SetAttributeFlags(instrSession, RS_ATTR_IO_RESOURCE_DESCRIPTOR, flag & 0xfffb | 0x0010));
	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_IO_RESOURCE_DESCRIPTOR, 0, resourceName));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ApplyOptionString
    Parses the init_with_options option string and sets the session properties and attributes
******************************************************************************************************/
ViStatus RsCore_ApplyOptionString(ViSession instrSession, ViConstString optionsString)
{
	ViStatus error = VI_SUCCESS;
	ViChar* driverSetup = NULL;
	ViChar token[RS_MAX_MESSAGE_LEN];
	ViChar tokenName[RS_MAX_MESSAGE_LEN];
	ViChar tokenValue[RS_MAX_MESSAGE_LEN];
	ViChar elab[RS_MAX_MESSAGE_LEN];
	ViInt32 elementsCount, idx;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	ViInt32 bufferSize = (ViInt32)strlen(optionsString) + 1;

	if (!optionsString || bufferSize <= 1)
		goto Error;

	viCheckAllocElab(driverSetup = (ViChar*)calloc(1, bufferSize), "Driver setup string in parsing rsxxx_InitWithOptions option string");

	elementsCount = RsCore_GetElementsCount(optionsString, ',');
	for (idx = 0; idx < elementsCount; idx++)
	{
		RsCore_GetTokenAtIndex(optionsString, ',', idx, token, RS_MAX_MESSAGE_LEN, NULL);
		if (!RsCoreInt_SplitTokenToNameAndValue(token, "=", tokenName, tokenValue, RS_MAX_MESSAGE_LEN))
			goto Unresolved;

		RsCoreInt_ToLowerCase(tokenName);
		if (strcmp(tokenName, "simulate") == 0)
		{
			// already parsed and set in the RsCore_SpecificDriverNew, here only mentioned not to be considered unresolved
		}
		else if (strcmp(tokenName, "reusesession") == 0)
		{
			// Already parsed and set in the RsCore_SpecificDriverNew, here only mentioned not to be considered unresolved
		}
		else if (strcmp(tokenName, "multithreadlocking") == 0)
		{
			rsSession->multiThreadLocking = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "rangecheck") == 0)
		{
			checkErr(RsCore_SetAttributeViBoolean(instrSession, NULL, RS_ATTR_RANGE_CHECK, 0, RsCore_Convert_String_To_Boolean(tokenValue)));
		}
		else if (strcmp(tokenName, "queryinstrstatus") == 0)
		{
			checkErr(RsCore_SetAttributeViBoolean(instrSession, NULL, RS_ATTR_QUERY_INSTRUMENT_STATUS, 0, RsCore_Convert_String_To_Boolean(tokenValue))
			);
		}
		else if (strcmp(tokenName, "fastsweep") == 0)
		{
			rsSession->fastSweepInstrument = atoi(tokenValue);
		}
		else if (strcmp(tokenName, "readdelay") == 0)
		{
			rsSession->readDelay = (ViInt32)atof(tokenValue);
		}
		else if (strcmp(tokenName, "writedelay") == 0)
		{
			rsSession->writeDelay = (ViInt32)atof(tokenValue);
		}
		else if (strcmp(tokenName, "cache") == 0)
		{
			checkErr(RsCore_SetAttributeViBoolean(instrSession, NULL, RS_ATTR_CACHE, 0, RsCore_Convert_String_To_Boolean(tokenValue)));
		}
		else if (strcmp(tokenName, "visatimeout") == 0)
		{
			if (!RsCore_Simulating(instrSession))
			{
				checkErr(viSetAttribute(instrSession, VI_ATTR_TMO_VALUE, atol(tokenValue)));
			}
		}
		else if (strcmp(tokenName, "opctimeout") == 0)
		{
			checkErr(RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_OPC_TIMEOUT, 0, (ViInt32)atof(tokenValue)));
		}
		else if (strcmp(tokenName, "sendopcqueryincheckstatus") == 0)
		{
			rsSession->sendOpcInCheckStatus = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "recogniselongresponses") == 0)
		{
			rsSession->recogniseLongResponses = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "reportattrdatatypemismatch") == 0)
		{
			rsSession->reportAttrDataTypeMismatch = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "vireadstbvisatimeout") == 0)
		{
			rsSession->viReadStbVisaTimeoutMs = (ViInt32)atof(tokenValue);
		}
		else if (strcmp(tokenName, "iosegmentsize") == 0)
		{
			rsSession->ioSegmentSize = (ViInt32)atof(tokenValue);
		}
		else if (strcmp(tokenName, "assurewritewithlf") == 0)
		{
			rsSession->assureWriteWithLF = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "addtermchartowritebinblock") == 0)
		{
			rsSession->addTermCharToWriteBinBlock = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "autosystemerrorquery") == 0)
		{
			rsSession->autoSystErrQuery = RsCore_Convert_String_To_Boolean(tokenValue);
		}
		else if (strcmp(tokenName, "selftesttimeout") == 0)
		{
			rsSession->selfTestTimeout = (ViInt32)atof(tokenValue);
		}
		else if (strcmp(tokenName, "opcwaitmode") == 0)
		{
			RsCoreInt_ToLowerCase(tokenValue);
			if (strcmp(tokenValue, "stbpolling") == 0) rsSession->opcWaitMode = RS_VAL_OPCWAIT_STBPOLLING;
			else if (strcmp(tokenValue, "stbpollingslow") == 0) rsSession->opcWaitMode = RS_VAL_OPCWAIT_STBPOLLINGSLOW;
			else if (strcmp(tokenValue, "stbpollingsuperslow") == 0) rsSession->opcWaitMode = RS_VAL_OPCWAIT_STBPOLLINGSUPERSLOW;
			else if (strcmp(tokenValue, "servicerequest") == 0) rsSession->opcWaitMode = RS_VAL_OPCWAIT_SERVICEREQUEST;
			else if (strcmp(tokenValue, "opcquery") == 0) rsSession->opcWaitMode = RS_VAL_OPCWAIT_OPCQUERY;
			else
			{
				snprintf(elab, RS_MAX_MESSAGE_LEN,
				         "Option string token 'OpcWaitMode', value '%s' is unrecognised."
				         "Defined values: 'StbPolling / StbPollingSlow / StbPollingSuperSlow / ServiceRequest'", tokenValue);
				viCheckErrElab(RS_ERROR_INVALID_VALUE, elab);
			}
		}
		else
		{
		Unresolved:
			// Store unresolved option string for RS_ATTR_DRIVER_SETUP
			RsCore_TrimString(token, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
			RsCore_AppendToCsvString(driverSetup, ",", bufferSize, token);
		}
	}

	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_DRIVER_SETUP, 0, driverSetup));

Error:
	if (driverSetup)
		free(driverSetup);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetRsSession
    Returns core session properties related to the VISA instrSession
******************************************************************************************************/
ViStatus RsCore_GetRsSession(ViSession instrSession, RsCoreSessionPtr* rsSession)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 idx;

	*rsSession = NULL;

	if (instrSession == 0)
	{
		checkErr(RS_ERROR_INVALID_SESSION_HANDLE);
	}

	gMutex = RsCoreInt_MutexLock(gMutex);

	for (idx = 0; idx < MAX_NUMBER_OF_SESSIONS; idx++)
		if (gSessionsList[idx] != NULL && gSessionsList[idx]->io == instrSession)
		{
			*rsSession = gSessionsList[idx];
			goto Error;
		}

	checkErr(RS_ERROR_INVALID_SESSION_HANDLE);

Error:
	RsCoreInt_MutexUnlock(gMutex);
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ViClose
    Closes the VISA I/O session
    After calling RsCore_ViClose, call the RsCore_Dispose to close all the session resources used by the driver.
******************************************************************************************************/
ViStatus RsCore_ViClose(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	if (rsSession == NULL)
	{
		// no other info is available, close the session without reporting an error
		(void)viClose(instrSession);
		return error;
	}

	if (RsCore_Simulating(instrSession) == VI_TRUE)
		return error;

	(void)viClose(instrSession);
	(void)viClose(rsSession->rmSession);
	rsSession->rmSession = 0;

	return error;
}

/*****************************************************************************************************/
/*  RsCore_Dispose
    Destroys the RS session, all of its attributes, and the memory resources it uses.
    Close the VISA I/O session before by calling RsCore_ViClose()
******************************************************************************************************/
ViStatus RsCore_Dispose(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 idx = 0;
	void* p2value;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);
	
	gMutex = RsCoreInt_MutexLock(gMutex);
	if (rsSession != NULL)
	{
		(void)RsCoreInt_DisposeRepCapTable(instrSession);

		/* Deallocate memory for attribute properties list */
		if (rsSession->attributes != NULL)
		{
			for (idx = 0; idx < rsSession->attributesCount; idx++)
			{
				p2value = rsSession->attributes[idx].p2value;
				if (p2value != NULL)
				{
					free(p2value);
					p2value = NULL;
				}
			}
			free(rsSession->attributes);
			rsSession->attributes = NULL;
		}

		if (rsSession->usedAttributesCache.attrs)
		{
			free(rsSession->usedAttributesCache.attrs);
			rsSession->usedAttributesCache.attrs = NULL;
		}

		if (rsSession->usedAttributesCache.IDs)
		{
			free(rsSession->usedAttributesCache.IDs);
			rsSession->usedAttributesCache.IDs = NULL;
		}

		(void)RsCoreInt_SessionsListDeleteSession(instrSession);
	}
	RsCoreInt_MutexUnlock(gMutex);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_SetSpecificDriverRevision
    Sets summary information string usually to the RS_ATTR_SPECIFIC_DRIVER_REVISION
    Use it in the driver's PREFIX_InitAttributes function
******************************************************************************************************/
ViStatus RsCore_SetSpecificDriverRevision(ViSession instrSession, ViAttr specificDriverRevisionAttrId)
{
	ViStatus error = VI_SUCCESS;
	ViChar prefix[RS_MAX_MESSAGE_LEN];
	ViChar buffer[RS_MAX_MESSAGE_LEN];
	ViChar revision[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 majorVer, minorVer, minorminorVer;

	checkErr(RsCore_GetAttributeViString(instrSession, NULL, RS_ATTR_SPECIFIC_DRIVER_PREFIX, 0, RS_MAX_MESSAGE_LEN, prefix));
	checkErr(RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION, 0, &majorVer));
	checkErr(RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_SPECIFIC_DRIVER_MINOR_VERSION, 0, &minorVer));
	checkErr(RsCore_GetAttributeViInt32(instrSession, NULL, RS_ATTR_SPECIFIC_DRIVER_MINOR_MINOR_VERSION, 0, &minorminorVer));

	checkErr(RsCoreInt_GetVisaInfo(instrSession, RS_MAX_MESSAGE_BUF_SIZE, revision));

	sprintf(buffer,
	        "Driver: %s " _PERCD "." _PERCD "." _PERCD ", Compiler: %s %3.2f, "
	        "Components: RsCore Component %.2f, VISA-Spec %s",
	        prefix, majorVer, minorVer, minorminorVer,
	        RS_COMPILER_NAME, RS_COMPILER_VER_NUM,
	        RSCORE_ENGINE_MAJOR_VERSION + RSCORE_ENGINE_MINOR_VERSION / 10.0,
	        revision);

	checkErr(RsCore_SetAttributeViString(instrSession, NULL, specificDriverRevisionAttrId, 0, buffer));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ViClear
    Sends VISA viClear command to the instrument
******************************************************************************************************/
ViStatus RsCore_ViClear(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession) == VI_TRUE)
		goto Error;

	if (rsSession->allowViClear == VI_FALSE)
		goto Error;

	checkErr(viClear(instrSession));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ViClear
    Performs standard instrument ClearStatus procedure
******************************************************************************************************/
ViStatus RsCore_ClearStatus(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_ViClear(instrSession));
	checkErr(RsCoreInt_ClearBeforeRead(instrSession));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_LockSession
    Obtains a lock on the specified resource.
    Obtains a multi-thread lock on the instrument session. Before it does so, it waits until all other execution threads
    have released their locks on the instrument session.
******************************************************************************************************/
ViStatus RsCore_LockSession(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->multiThreadLocking == VI_TRUE)
	{
		rsSession->mutex = RsCoreInt_MutexLock(rsSession->mutex);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_UnlockSession
    Releases a lock that you acquired on an instrument session using RsCore_LockSession.
******************************************************************************************************/
ViStatus RsCore_UnlockSession(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->multiThreadLocking == VI_TRUE)
	{
		RsCoreInt_MutexUnlock(rsSession->mutex);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_Simulating
    Returns the current value of the RS_ATTR_SIMULATE attribute
******************************************************************************************************/
ViBoolean RsCore_Simulating(ViSession instrSession)
{
	ViBoolean value = VI_FALSE;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	if (rsSession == 0)
		return value;

	(void)(RsCoreInt_GetAttrP2Value(rsSession->attrSimulatePtr, &value, 0));

	return value;
}

/*****************************************************************************************************/
/*  RsCore_RangeChecking
    Returns the current value of the RS_ATTR_RANGE_CHECK attribute
******************************************************************************************************/
ViBoolean RsCore_RangeChecking(ViSession instrSession)
{
	ViBoolean value = VI_TRUE;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	if (rsSession == 0)
		return value;

	(void)(RsCoreInt_GetAttrP2Value(rsSession->attrRangeCheckPtr, &value, 0));

	return value;
}

/*****************************************************************************************************/
/*   RsCore_QueryInstrStatus
     Returns the current value of the RS_ATTR_QUERY_INSTR_STATUS attribute
******************************************************************************************************/
ViBoolean RsCore_QueryInstrStatus(ViSession instrSession)
{
	ViBoolean value = VI_TRUE;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	if (rsSession == 0)
		return value;

	(void)(RsCoreInt_GetAttrP2Value(rsSession->attrQueryInstrStatusPtr, &value, 0));

	return value;
}

/*****************************************************************************************************/
/*  RsCore_SetVisaTimeout
    Sets VISA timeout in milliseconds
******************************************************************************************************/
ViStatus RsCore_SetVisaTimeout(ViSession instrSession, ViUInt32 timeoutMs)
{
	if (RsCore_Simulating(instrSession) == VI_TRUE)
		return VI_SUCCESS;

	return viSetAttribute(instrSession, VI_ATTR_TMO_VALUE, timeoutMs);
}

/*****************************************************************************************************/
/*  Returns VISA timeout in milliseconds
******************************************************************************************************/
ViStatus RsCore_GetVisaTimeout(ViSession instrSession, ViPUInt32 timeoutMs)
{
	if (RsCore_Simulating(instrSession) == VI_TRUE)
		return VI_SUCCESS;

	return viGetAttribute(instrSession, VI_ATTR_TMO_VALUE, timeoutMs);
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Set / Get / Clear Error info --------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_SetErrorInfo
    Sets error info for the session. The viCheckErr, viCheckErrElab, viCheckParm, viCheckAlloc,
    and viCheckWarn macros use RsCore_SetErrorInfo
******************************************************************************************************/
ViStatus RsCore_SetErrorInfo(ViSession instrSession,
                             ViBoolean overWrite,
                             ViStatus primaryError,
                             ViStatus secondaryError,
                             ViConstString errorElab)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	if (rsSession == NULL)
	{
		if (overWrite == VI_TRUE || ((gPrimaryError >= VI_SUCCESS) && (primaryError < VI_SUCCESS)))
		{
			RsCoreInt_SetGlobalErrorInfo(primaryError, secondaryError, errorElab);
			return primaryError;
		}

		return VI_ERROR_INV_OBJECT;
	}

	if (overWrite == VI_TRUE || (((rsSession->primaryError >= VI_SUCCESS) && (primaryError < VI_SUCCESS))))
	{
		// Set the values to the rsSession structure
		rsSession->primaryError = primaryError;
		rsSession->secondaryError = secondaryError;
		RsCoreInt_StrcpyMaxLen(rsSession->errorElaboration, RS_MAX_MESSAGE_BUF_SIZE, errorElab);

		// Set the values to the attributes
		(void)RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_PRIMARY_ERROR, 0, primaryError);
		(void)RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_SECONDARY_ERROR, 0, secondaryError);
		(void)RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_ERROR_ELABORATION, 0, errorElab);

		(void)RsCoreInt_SetGlobalErrorInfo(primaryError, secondaryError, errorElab);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetErrorInfo
    Retrieves and then clears the error information for the entered session.
    Used attributes:
    - RS_ATTR_PRIMARY_ERROR or PREFIX_ATTR_PRIMARY_ERROR
    - RS_ATTR_SECONDARY_ERROR or PREFIX_ATTR_SECONDARY_ERROR
    - RS_ATTR_ERROR_ELABORATION or PREFIX_ATTR_ERROR_ELABORATION
        For errorElaboration string you need to pass the buffer with the size at least RS_MAX_MESSAGE_BUF_SIZE
******************************************************************************************************/
ViStatus RsCore_GetErrorInfo(ViSession instrSession, ViStatus* primaryError, ViStatus* secondaryError, ViChar* errorElaboration)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	if (rsSession == NULL)
	{
		// in this case return the global last error entry
		RsCoreInt_GetGlobalErrorInfo(primaryError, secondaryError, errorElaboration);
		RsCoreInt_SetGlobalErrorInfo(VI_SUCCESS, VI_SUCCESS, "");
	}
	else
	{
		*primaryError = rsSession->primaryError;
		*secondaryError = rsSession->secondaryError;
		RsCoreInt_StrcpyMaxLen(errorElaboration, RS_MAX_MESSAGE_BUF_SIZE, rsSession->errorElaboration);
		(void)RsCore_ClearErrorInfo(instrSession);
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCore_AppendSecondaryError
    If primaryError is < 0, the function adds the secondary error and errorElab
    If the entered primaryError does not equal the existing primaryError, the entered primaryError takes priority.
    The function does not overwrite the existing error elaboration, rather it appends the new one to the end.
    For special secondary error codes e.g. RS_ERROR_INVALID_PARAMETER, VI_ERROR_PARAMETER1 .. VI_ERROR_PARAMETER8
    the new error elaboration is added in a special way to make the elaboration more comprehensive
******************************************************************************************************/
ViStatus RsCore_AppendSecondaryError(ViSession instrSession,
                                     ViStatus primaryError,
                                     ViStatus secondaryError,
                                     ViConstString errorElab)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 parameterIdx = 1;
	ViChar existingErrElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar composedErrElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViStatus existingPrimaryErr, existingSecondaryErr;

	if (primaryError == VI_SUCCESS)
		return error;

	switch (secondaryError)
	{
	case RS_ERROR_INVALID_PARAMETER: parameterIdx = 0;
		break;
	case VI_ERROR_PARAMETER1: parameterIdx = 1;
		break;
	case VI_ERROR_PARAMETER2: parameterIdx = 2;
		break;
	case VI_ERROR_PARAMETER3: parameterIdx = 3;
		break;
	case VI_ERROR_PARAMETER4: parameterIdx = 4;
		break;
	case VI_ERROR_PARAMETER5: parameterIdx = 5;
		break;
	case VI_ERROR_PARAMETER6: parameterIdx = 6;
		break;
	case VI_ERROR_PARAMETER7: parameterIdx = 7;
		break;
	case VI_ERROR_PARAMETER8: parameterIdx = 8;
		break;
	default: parameterIdx = -1;
		break;
	}

	(void)RsCore_GetErrorInfo(instrSession, &existingPrimaryErr, &existingSecondaryErr, existingErrElab);

	if (parameterIdx == -1)
	{
		// Error code other than Parameter error
		snprintf(composedErrElab, RS_MAX_MESSAGE_BUF_SIZE, "%s\n%s", existingErrElab, errorElab);
	}
	else
	{
		if (isNullOrEmpty(errorElab))
		{
			if (parameterIdx == 0)
			{
				// Parameter error, unknown Idx, no name provided
				snprintf(composedErrElab, RS_MAX_MESSAGE_BUF_SIZE,
				         "Error related to unknown function parameter.\n%s", existingErrElab);
			}
			else
			{
				// Parameter error, known Idx, no name provided
				snprintf(composedErrElab, RS_MAX_MESSAGE_BUF_SIZE,
				         "Error related to function parameter no. " _PERCD ".\n%s", parameterIdx, existingErrElab);
			}
		}
		else
		{
			if (parameterIdx == 0)
			{
				// Parameter error, unknown Idx, name is provided
				snprintf(composedErrElab, RS_MAX_MESSAGE_BUF_SIZE,
				         "Error related to function parameter '%s'.\n%s", errorElab, existingErrElab);
			}
			else
			{
				// Parameter error, known Idx, name is provided
				snprintf(composedErrElab, RS_MAX_MESSAGE_BUF_SIZE,
				         "Error related to function parameter no. " _PERCD " - '%s'.\n%s", parameterIdx, errorElab, existingErrElab);
			}
		}
	}

	RsCore_TrimString(composedErrElab, RS_VAL_TRIM_WHITESPACES);
	RsCore_SetErrorInfo(instrSession, VI_TRUE, primaryError, secondaryError, composedErrElab);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_ClearErrorInfo
    Clears error information for the current session and the global error info.
    Affected attributes:
    - RS_ATTR_PRIMARY_ERROR or PREFIX_ATTR_PRIMARY_ERROR
    - RS_ATTR_SECONDARY_ERROR or PREFIX_ATTR_SECONDARY_ERROR
    - RS_ATTR_ERROR_ELABORATION or PREFIX_ATTR_ERROR_ELABORATION
******************************************************************************************************/
ViStatus RsCore_ClearErrorInfo(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	(void)RsCore_GetRsSession(instrSession, &rsSession);

	RsCoreInt_SetGlobalErrorInfo(VI_SUCCESS, VI_SUCCESS, "");

	if (rsSession == NULL)
		return error;

	rsSession->primaryError = VI_SUCCESS;
	rsSession->secondaryError = VI_SUCCESS;
	memset(rsSession->errorElaboration, 0, RS_MAX_MESSAGE_BUF_SIZE);

	(void)RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_PRIMARY_ERROR, 0, VI_SUCCESS);
	(void)RsCore_SetAttributeViInt32(instrSession, NULL, RS_ATTR_SECONDARY_ERROR, 0, VI_SUCCESS);
	(void)RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_ERROR_ELABORATION, 0, "");

	return error;
}

/*****************************************************************************************************/
/*  RsCore_ParamPositionError
    Returns one of the VI_ERROR_PARAMETER1 ... VI_ERROR_PARAMETER8, RS_ERROR_INVALID_PARAMETER.
******************************************************************************************************/
ViStatus RsCore_ParamPositionError(ViInt32 parameterPosition)
{
	ViInt32 error[] = {
		RS_ERROR_INVALID_PARAMETER,
		VI_ERROR_PARAMETER1, VI_ERROR_PARAMETER2,
		VI_ERROR_PARAMETER3, VI_ERROR_PARAMETER4,
		VI_ERROR_PARAMETER5, VI_ERROR_PARAMETER6,
		VI_ERROR_PARAMETER7, VI_ERROR_PARAMETER8
	};

	return ((0 < parameterPosition) && (parameterPosition < 9)) ? error[parameterPosition] : RS_ERROR_INVALID_PARAMETER;
}

/*****************************************************************************************************/
/*  RsCore_GetErrorCompleteDescription
    Calls RsCore_GetErrorInfo and composes a comprehensive string description of the current error.
    For errorMessageFnc pass the pointer to the driver's PREFIX_error_message() function
******************************************************************************************************/
ViStatus RsCore_GetErrorCompleteDescription(ViSession instrSession, RsCoreErrorMessage errorMessageFnc,
                                            ViStatus* errorCode, ViInt32 bufferSize, ViChar* description)
{
	ViStatus error = VI_SUCCESS;
	ViStatus primaryError = VI_SUCCESS;
	ViStatus secondaryError = VI_SUCCESS;
	ViChar elaboration[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar elabNew[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar errorMessage[RS_MAX_MESSAGE_BUF_SIZE + 1];
	ViInt32 offset = 0;

	checkErr(RsCore_GetErrorInfo(instrSession, &primaryError, &secondaryError, elaboration));
	RsCoreInt_StrcpyMaxLen(elabNew, RS_MAX_MESSAGE_BUF_SIZE, "");
	if (primaryError != VI_SUCCESS)
	{
		checkErr((*errorMessageFnc)(instrSession, primaryError, errorMessage));
		offset += snprintf(elabNew + offset, RS_MAX_MESSAGE_BUF_SIZE - offset, "Primary Error: (Hex 0x%08X) %s\n", (unsigned int)primaryError,
		                   errorMessage);
	}

	if (secondaryError != VI_SUCCESS)
	{
		checkErr((*errorMessageFnc)(instrSession, secondaryError, errorMessage));
		offset += snprintf(elabNew + offset, RS_MAX_MESSAGE_BUF_SIZE - offset, "Secondary Error: (Hex 0x%08X) %s\n", (unsigned int)secondaryError,
		                   errorMessage);
	}

	if (strlen(elaboration) > 0)
		snprintf(elabNew + offset, RS_MAX_MESSAGE_BUF_SIZE - offset, "Elaboration: %s", elaboration);

	if (errorCode)
		*errorCode = primaryError;

	if (bufferSize > 0)
		RsCoreInt_StrcpyMaxLen(description, bufferSize, elabNew);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetSpecificDriverStatusDesc
    Same as RsCoreInt_GetErrorMessage, but includes additional error codes in the additionalTableToSearch
    Use this function to implement the PREFIX_error_message function in the instrument driver.
    The function works even with invalid instrSession
******************************************************************************************************/
ViStatus RsCore_GetSpecificDriverStatusDesc(ViSession instrSession,
                                            ViStatus statusCode,
                                            ViChar* messageBuf, ViInt32 bufferSize,
                                            RsCoreStringValueTable additionalTableToSearch)
{
	ViStatus error = VI_SUCCESS;
	ViInt16 cnt;

	error = RsCoreInt_StatusDescription(statusCode, bufferSize, messageBuf);

	if (error == VI_WARN_UNKNOWN_STATUS)
	{
		for (cnt = 0; additionalTableToSearch[cnt].string; cnt++)
		{
			if (additionalTableToSearch[cnt].value == statusCode)
			{
				snprintf(messageBuf, bufferSize, "%s", additionalTableToSearch[cnt].string);
				return error;
			}
		}

		RsCoreInt_StrcpyMaxLen(messageBuf, bufferSize, "The status code passed to the operation could not be interpreted.");
		return VI_WARN_UNKNOWN_STATUS;
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCore_GenerateInvalidDataTypeError
	Generates error RS_ERROR_INVALID_TYPE with comprehensive message
	context can be set to NULL
	context is added to the end of the message as a new sentence
******************************************************************************************************/
ViStatus RsCore_GenerateInvalidDataTypeError(ViSession instrSession, RsCoreDataType dataType, ViConstString context)
{
	ViStatus error = RS_ERROR_INVALID_TYPE;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (isNullOrEmpty(context))
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Invalid data type - %s", RsCoreInt_GetDataTypeString(dataType));
	}
	else
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Invalid data type - %s. %s", RsCoreInt_GetDataTypeString(dataType), context);
	}

	(void)RsCore_SetErrorInfo(instrSession, VI_TRUE, error, VI_SUCCESS, errElab);

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Instrument Model and Instrument Options ---------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  Queries and parses the instrument *IDN? string
    In simulation, it uses the provided simulationIDNstring as a source for parsing
    Sets the attributes:
    - PREFIX_ATTR_ID_QUERY_RESPONSE
    - RS_ATTR_INSTRUMENT_MANUFACTURER
    - RS_ATTR_INSTRUMENT_MODEL - based on the idnModelFullName true: "FSW-26", false: "FSW"
    - RS_ATTR_INSTRUMENT_FIRMWARE_REVISION
******************************************************************************************************/
ViStatus RsCore_QueryAndParseIDNstring(ViSession instrSession, ViAttr idnStringAttributeId, ViString simulationIDNstring,
                                       ViBoolean idnModelFullName, ViString customScanString)
{
	ViStatus error = VI_SUCCESS;
	ViChar revision[RS_MAX_MESSAGE_LEN];
	ViChar manufacturer[RS_MAX_MESSAGE_LEN];
	ViChar modelFull[RS_MAX_MESSAGE_LEN];
	ViChar model[RS_MAX_MESSAGE_LEN];
	ViChar idnString[RS_MAX_MESSAGE_BUF_SIZE];

	RsCoreInt_StrcpyMaxLen(manufacturer, RS_MAX_MESSAGE_LEN, "Rohde&Schwarz");
	RsCoreInt_StrcpyMaxLen(modelFull, RS_MAX_MESSAGE_LEN, "DummyModel-Full");
	RsCoreInt_StrcpyMaxLen(revision, RS_MAX_MESSAGE_LEN, "1.0.0");

	// Query the *IDN?, parse it and store it into the PREFIX_ATTR_ID_QUERY_RESPONSE
	if (!RsCore_Simulating(instrSession))
	{
		// Read the real *IDN? response
		checkErr(RsCore_QueryViString(instrSession, "*IDN?", idnString));
	}
	else
	{
		// use the simulationIDNstring value
		RsCoreInt_StrcpyMaxLen(idnString, RS_MAX_MESSAGE_LEN, simulationIDNstring);
	}

	if (customScanString != NULL && strlen(customScanString) > 0)
	{
		sscanf(idnString, customScanString, manufacturer, modelFull, revision);
	}
	else
	{
		sscanf(idnString, "%[^,],%[^,],%*[^,],%256[^\n]", manufacturer, modelFull, revision);
	}

	checkErr(RsCore_SetAttributeViString(instrSession, NULL, idnStringAttributeId, 0, idnString));
	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_INSTRUMENT_MANUFACTURER, 0, manufacturer));
	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_INSTRUMENT_FIRMWARE_REVISION, 0, revision));

	// For model, decide between the model and modelFull - this value is used for option checking
	RsCoreInt_StrcpyMaxLen(model, RS_MAX_MESSAGE_LEN, "");
	sscanf(modelFull, "%[^-0-9]", model);
	if (strlen(model) == 0)
		idnModelFullName = VI_TRUE;

	if (idnModelFullName)
	{
		checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_INSTRUMENT_MODEL, 0, modelFull));
	}
	else
	{
		checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_INSTRUMENT_MODEL, 0, model));
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  Evaluates if the IDN string matches case insensitive the expected pattern 'requiredIdnPattern1' or 'requiredIdnPattern2'
    Returns VI_ERROR_FAIL_ID_QUERY if neither of 'requiredIdnPattern1' or 'requiredIdnPattern2' is found in the IDN string
    requiredIdnPattern2 can be optionally set to NULL
******************************************************************************************************/
ViStatus RsCore_FitsIDNpattern(ViSession instrSession, ViAttr idnStringAttributeId, ViConstString requiredIdnPattern1,
                               ViConstString requiredIdnPattern2)
{
	ViStatus error = VI_SUCCESS;
	ViChar idnString[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar validPatternLowCase[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCore_GetAttributeViString(instrSession, NULL, idnStringAttributeId, 0, RS_MAX_MESSAGE_LEN, idnString));
	RsCoreInt_ToLowerCase(idnString);

	RsCoreInt_StrcpyMaxLen(validPatternLowCase, RS_MAX_MESSAGE_LEN, requiredIdnPattern1);
	RsCoreInt_ToLowerCase(validPatternLowCase);
	if (strstr(idnString, validPatternLowCase) != NULL)
		goto Error;

	if (requiredIdnPattern2 != NULL)
	{
		RsCoreInt_StrcpyMaxLen(validPatternLowCase, RS_MAX_MESSAGE_LEN, requiredIdnPattern2);
		if (strstr(idnString, RsCoreInt_ToLowerCase(validPatternLowCase)) != NULL)
			goto Error;

		checkErr(RsCore_GetAttributeViString(instrSession, NULL, idnStringAttributeId, 0, RS_MAX_MESSAGE_LEN, idnString));
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "*IDN? string '%s' does not fit either of the required patterns '%s' or '%s'",
		         idnString, requiredIdnPattern1, requiredIdnPattern2);
		viCheckErrElab(VI_ERROR_FAIL_ID_QUERY, errElab);
	}

	checkErr(RsCore_GetAttributeViString(instrSession, NULL, idnStringAttributeId, 0, RS_MAX_MESSAGE_LEN, idnString));
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "*IDN? string '%s' does not fit the required pattern '%s'", idnString, requiredIdnPattern1);
	viCheckErrElab(VI_ERROR_FAIL_ID_QUERY, errElab);

Error:
	return error;
}

/*****************************************************************************************************/
/*  Queries and parses the instrument options *OPT?
    In simulation, it uses the provided simulationOPTstring as a source for parsing
    Sets the attribute RS_ATTR_OPTIONS_LIST
******************************************************************************************************/
ViStatus RsCore_QueryAndParseOPTstring(ViSession instrSession, ViString simulationOPTstring, ViInt32 optionsParseMode)
{
	ViStatus error = VI_SUCCESS;
	ViChar* optionsList;

	// Query the *OPT?, parse it and store it into the RS_ATTR_OPTIONS_LIST
	if (!RsCore_Simulating(instrSession))
	{
		// Read the real *OPT? response
		checkErr(RsCore_QueryViStringUnknownLength(instrSession, "*OPT?", &optionsList));
	}
	else
	{
		// Use the simulationOPTstring value
		checkAlloc(optionsList = (ViChar *)calloc(strlen(simulationOPTstring) + 1, 1));
		RsCoreInt_StrcpyMaxLen(optionsList, (ViInt32)strlen(simulationOPTstring) + 1, simulationOPTstring);
	}

	checkErr(RsCoreInt_ParseOptionsString(instrSession, optionsList, optionsParseMode));
	checkErr(RsCore_SetAttributeViString(instrSession, NULL, RS_ATTR_OPTIONS_LIST, 0, optionsList));

Error:
	if (optionsList)
		free(optionsList);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_CheckInstrumentModel
	Generates RS_ERROR_INSTRUMENT_MODEL error, if the current instrument does not fulfill the modelsList requirement.
	If session's optionChecking is OFF, the function always returns VI_SUCCESS
	- Example of a positive requirement: modelsList = 'FSW|FSWP': for VI_SUCCESS, the current instrument must be either FSW or FSWP
	- Example of a negative requirement: modelsList = '!FSW|FSWP': for VI_SUCCESS, the current instrument must be neither FSW nor FSWP
******************************************************************************************************/
ViStatus RsCore_CheckInstrumentModel(ViSession instrSession, ViConstString modelsList)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	if (rsSession->optionChecking == VI_TRUE)
	{
		checkErr(RsCoreInt_CheckInstrumentModel(instrSession, modelsList, NULL, VI_TRUE));
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_IsInstrumentModel
	Returns TRUE, if the current model fulfills the modelsList requirement.
	The session's optionChecking status has no effect on this function
	- Example of a positive requirement: modelsList = 'FSW|FSWP': for result TRUE, the current instrument must be either FSW or FSWP
	- Example of a negative requirement: modelsList = '!FSW|FSWP': for result TRUE, the current instrument must be neither FSW nor FSWP
******************************************************************************************************/
ViBoolean RsCore_IsInstrumentModel(ViSession instrSession, ViConstString modelsList)
{
	ViBoolean pass;
	
	(void)RsCoreInt_CheckInstrumentModel(instrSession, modelsList, &pass, VI_FALSE);

	return pass;
}

/*****************************************************************************************************/
/*  RsCore_CheckInstrumentOptions
	Generates RS_ERROR_INSTRUMENT_OPTION error, if the current instrument does not have the required option(s) installed.
	The optionsExpression is the same one as used in attributes.
	If session's optionChecking is OFF, the function always returns VI_SUCCESS
******************************************************************************************************/
ViStatus RsCore_CheckInstrumentOptions(ViSession instrSession, ViConstString optionsExpression)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	if (rsSession->optionChecking == VI_TRUE)
	{
		checkErr(RsCoreInt_CheckInstrumentOptions(instrSession, optionsExpression, NULL, VI_TRUE));
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_HasInstrumentOptions
	Returns TRUE, if the current instrument has the required option(s) installed.
	The session's optionChecking status has no effect on this function
	The optionsExpression is the same one as used in attributes.
******************************************************************************************************/
ViBoolean RsCore_HasInstrumentOptions(ViSession instrSession, ViConstString optionsExpression)
{
	ViBoolean pass;

	(void)RsCoreInt_CheckInstrumentOptions(instrSession, optionsExpression, &pass, VI_FALSE);

	return pass;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Attributes - related functions ------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_SetAttributeViReal64
    Exported type-safe function for setting an attribute ViReal64 value
******************************************************************************************************/
ViStatus RsCore_SetAttributeViReal64(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViReal64 value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_REAL64, &value);
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeViInt32
    Exported type-safe function for setting an attribute ViInt32 value
******************************************************************************************************/
ViStatus RsCore_SetAttributeViInt32(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViInt32 value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_INT32, &value);
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeViInt64
    Exported type-safe function for setting an attribute ViInt64 value
******************************************************************************************************/
ViStatus RsCore_SetAttributeViInt64(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViInt64 value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_INT64, &value);
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeViString
    Exported type-safe function for setting an attribute ViString value
******************************************************************************************************/
#pragma warning( disable : 4090 )
ViStatus RsCore_SetAttributeViString(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViConstString value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_STRING, (void*)value);
}
#pragma warning( default : 4090 )

/*****************************************************************************************************/
/*  RsCore_SetAttributeViBoolean
    Exported type-safe function for setting an attribute ViBoolean value
******************************************************************************************************/
ViStatus RsCore_SetAttributeViBoolean(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViBoolean value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_BOOLEAN, &value);
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeViSession
    Exported type-safe function for setting an attribute ViSession value
******************************************************************************************************/
ViStatus RsCore_SetAttributeViSession(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViSession value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_SESSION, &value);
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeViAddress
    Exported type-safe function for setting an attribute ViAddr value
******************************************************************************************************/
ViStatus RsCore_SetAttributeViAddress(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViAddr value)
{
	return RsCoreInt_SetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_ADDR, &value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViReal64
    Exported type-safe function for reading an attribute ViReal64 value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViReal64(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViReal64* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_REAL64, (ViInt32)sizeof(ViReal64), value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViInt32
    Exported type-safe function for reading an attribute ViInt32 value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViInt32(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViInt32* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_INT32, (ViInt32)sizeof(ViInt32), value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViInt64
    Exported type-safe function for reading an attribute ViInt64 value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViInt64(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViInt64* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_INT64, (ViInt32)sizeof(ViInt32), value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViInt64
    Exported type-safe function for reading an attribute ViInt64 value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViString(ViSession instrSession, ViString repCap,
                                     ViUInt32 attributeId, ViInt32 optionFlag, ViInt32 bufferSize, ViChar* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_STRING, bufferSize, value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViBoolean
    Exported type-safe function for reading an attribute ViBoolean value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViBoolean(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViBoolean* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_BOOLEAN, (ViInt32)sizeof(ViBoolean), value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViSession
    Exported type-safe function for reading an attribute ViSession value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViSession(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViSession* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_SESSION, (ViInt32)sizeof(ViSession), value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeViAddress
    Exported type-safe function for reading an attribute ViAddr value
******************************************************************************************************/
ViStatus RsCore_GetAttributeViAddress(ViSession instrSession, ViString repCap, ViUInt32 attributeId, ViInt32 optionFlag, ViAddr* value)
{
	return RsCoreInt_GetAttribute(instrSession, repCap, attributeId, optionFlag, RS_VAL_ADDR, (ViInt32)sizeof(ViAddr), value);
}

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViReal64
    Exported type-safe function for checking a ViReal64 attribute
******************************************************************************************************/
ViStatus RsCore_CheckAttributeViReal64(ViSession instrSession,
                                       ViString repCap,
                                       ViUInt32 attributeId,
                                       ViInt32 optionFlag,
                                       ViReal64 value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, &value);
}

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViInt32
    Exported type-safe function for checking a ViInt32 attribute
******************************************************************************************************/
ViStatus RsCore_CheckAttributeViInt32(ViSession instrSession,
                                      ViString repCap,
                                      ViUInt32 attributeId,
                                      ViInt32 optionFlag,
                                      ViInt32 value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, &value);
}

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViInt64
    Exported type-safe function for checking a ViInt64 attribute
******************************************************************************************************/
ViStatus RsCore_CheckAttributeViInt64(ViSession instrSession,
                                      ViString repCap,
                                      ViUInt32 attributeId,
                                      ViInt32 optionFlag,
                                      ViInt64 value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, &value);
}

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViString
    Exported type-safe function for checking a ViString attribute
******************************************************************************************************/
#pragma warning( disable : 4090 )
ViStatus RsCore_CheckAttributeViString(ViSession instrSession,
                                       ViString repCap,
                                       ViUInt32 attributeId,
                                       ViInt32 optionFlag,
                                       ViConstString value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, (void*)value);
}
#pragma warning( default : 4090 )

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViBoolean
    Exported type-safe function for checking a ViBoolean attribute
******************************************************************************************************/
ViStatus RsCore_CheckAttributeViBoolean(ViSession instrSession,
                                        ViString repCap,
                                        ViUInt32 attributeId,
                                        ViInt32 optionFlag,
                                        ViBoolean value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, &value);
}

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViSession
    Exported type-safe function for checking a ViSession attribute
******************************************************************************************************/
ViStatus RsCore_CheckAttributeViSession(ViSession instrSession,
                                        ViString repCap,
                                        ViUInt32 attributeId,
                                        ViInt32 optionFlag,
                                        ViSession value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, &value);
}

/*****************************************************************************************************/
/*  RsCore_CheckAttributeViAddress
    Exported type-safe function for checking a ViAddress attribute
******************************************************************************************************/
ViStatus RsCore_CheckAttributeViAddress(ViSession instrSession,
                                        ViString repCap,
                                        ViUInt32 attributeId,
                                        ViInt32 optionFlag,
                                        ViAddr value)
{
	return RsCoreInt_CheckAttr(instrSession, repCap, attributeId, optionFlag, &value);
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeFlags
    Obtains the current values of the flags for the attribute you specify.
******************************************************************************************************/
ViStatus RsCore_GetAttributeFlags(ViSession instrSession,
                                  ViAttr attributeId,
                                  RsCoreAttrFlags* flags)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttributePtr attr;

	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));

	*flags = attr->flags;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeFlags
    Sets the flags of an attribute to new values. It always sets all of the flags.
    If you want to change one flag, use RsCore_GetAttributeFlags to obtain the current values of all
    the flags, modify the bit for the flag you want to change, and then call RsCore_SetAttributeFlags.
    You cannot modify the value of the RS_VAL_MULTI_CHANNEL flag.
******************************************************************************************************/
ViStatus RsCore_SetAttributeFlags(ViSession instrSession,
                                  ViAttr attributeId,
                                  RsCoreAttrFlags flags)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttributePtr attr;

	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));

	attr->flags &= RS_VAL_MULTI_CHANNEL;
	attr->flags |= (flags & (~RS_VAL_MULTI_CHANNEL));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_SetAttributeP2Value
    Internal function that sets the cache value of the entered attribute
******************************************************************************************************/
ViStatus RsCore_SetAttributeP2Value(ViSession instrSession, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
		*(ViInt32 *)attr->p2value = (value) ? *(ViInt32 *)value : 0;
		break;
	case RS_VAL_INT64:
		*(ViInt64 *)attr->p2value = (value) ? *(ViInt64 *)value : 0;
		break;
	case RS_VAL_REAL64:
		*(ViReal64 *)attr->p2value = (value) ? *(ViReal64 *)value : 0;
		break;
	case RS_VAL_STRING:
		{
			ViChar* newString = (ViString)value;
			ViInt32 reqBuffer = (ViInt32)strlen(newString) + 1;
			// If the currently allocated buffer is too small, reallocate it
			if (reqBuffer > attr->integerDefault)
			{
				ViChar* oldP2value = (ViChar *)attr->p2value;
				if ((attr->p2value = realloc(attr->p2value, reqBuffer)) == NULL)
				{
					free(oldP2value);
					return RS_ERROR_ALLOC;
				}

				attr->integerDefault = reqBuffer;
			}

			RsCoreInt_StrcpyMaxLen((ViChar *)attr->p2value, attr->integerDefault, newString);
			break;
		}
	case RS_VAL_ADDR:
		*(ViAddr *)attr->p2value = (value) ? *(ViAddr *)value : 0;
		break;
	case RS_VAL_SESSION:
		*(ViSession *)attr->p2value = (value) ? *(ViSession *)value : 0;
		break;
	case RS_VAL_BOOLEAN:
		*(ViBoolean *)attr->p2value = (value) ? *(ViBoolean *)value : 0;
		break;
	case RS_VAL_EVENT:
		break;
	case RS_VAL_UNKNOWN_TYPE:
	default:
		error = RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SetAttributeP2Value");
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCore_ApplyAttributeDefaults
    Applies default attribute values. Optionally exclude inherent attributes from update.
******************************************************************************************************/
ViStatus RsCore_ApplyAttributeDefaults(ViSession instrSession, ViBoolean updateInherentAttr)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 idx = 0;
	size_t offset;
	size_t dataSize = 0;
	RsCoreAttributePtr attr;
	void *p2value, *p2default;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	for (idx = 0; idx < rsSession->attributesCount; idx++)
	{
		attr = &rsSession->attributes[idx];

		/* Update inherent attributes? */
		if ((updateInherentAttr == VI_FALSE) && (attr->constant < RS_SPECIFIC_PRIVATE_ATTR_BASE))
		{
			/* Inherent attributes are not updated */
		}
		else
		{
			offset = 0;
			p2value = attr->p2value;
			switch (attr->dataType)
			{
			case RS_VAL_INT32:
				dataSize = sizeof(ViInt32);
				p2default = &attr->integerDefault;
				break;
			case RS_VAL_INT64:
				dataSize = sizeof(ViInt32);
				p2default = &attr->integerDefault;
				offset = sizeof(ViInt64) - dataSize;
				break;
			case RS_VAL_REAL64:
				dataSize = sizeof(ViReal64);
				p2default = &attr->doubleDefault;
				break;
			case RS_VAL_STRING:
				if (attr->stringDefault)
					dataSize = strlen(attr->stringDefault);
				else
					dataSize = 0;
				p2default = attr->stringDefault;
				break;
			case RS_VAL_ADDR:
				dataSize = sizeof(ViAddr);
				p2default = &attr->integerDefault;
				break;
			case RS_VAL_SESSION:
				dataSize = sizeof(ViSession);
				p2default = &attr->sessionDefault;
				break;
			case RS_VAL_BOOLEAN:
				dataSize = sizeof(ViBoolean);
				p2default = &attr->booleanDefault;
				break;
			default:
				dataSize = sizeof(void *);
				p2default = &attr->p2value;
			}

			if (attr->dataType == RS_VAL_STRING)
			{
				if (p2default)
					RsCoreInt_StrcpyMaxLen((ViChar*)p2value, attr->integerDefault, (ViConstString)p2default);
				else
					RsCoreInt_StrcpyMaxLen((ViChar*)p2value, attr->integerDefault, "");
			}
			else
			{
				memset(p2value, 0, dataSize + offset);
				memcpy(((char*)p2value) + offset, p2default, dataSize);
			}
		}
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_BuildAttrCommandString
    Build the attribute's command - replaces its RepCapID parts with the corresponding command values
	The cmd is always at least RS_MAX_MESSAGE_BUF_SIZE big
    WARNING!!!: cmd is allocated dynamically in the function. Deallocate it afterwards!
******************************************************************************************************/
ViStatus RsCore_BuildAttrCommandString(ViSession instrSession, RsCoreAttributePtr attr, ViConstString repCapsInputString, ViChar** cmd)
{
	ViStatus error = VI_SUCCESS;
	ViChar repCapCmd[RS_MAX_MESSAGE_LEN];
	ViChar singleInputRepCap[RS_MAX_MESSAGE_LEN];
	ViChar singleRepCapNameId[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViInt32 index, i, inputRepCapsCount;
	ViInt32 foundIdx = 0;
	ViInt32 cmdActualLen = (ViInt32)strlen(attr->command);
	ViInt32 cmdBufferSize;
	ViInt32 restLen, repCapIdLen, cmdValueLen, startOffset, resizeOffset;
	ViChar *p2SearchStart, *p2LeftBracket, *p2RightBracket, *target, *source;
	RsCoreRepCapPtr repCap;

	if (isNullOrEmpty(repCapsInputString))
	{
		inputRepCapsCount = 0;
	}
	else
	{
		inputRepCapsCount = RsCore_GetElementsCount(repCapsInputString, ',');
	}

	cmdBufferSize = cmdActualLen + 1;
	if (cmdBufferSize < RS_MAX_SHORT_MESSAGE_BUF_SIZE)
		cmdBufferSize = RS_MAX_SHORT_MESSAGE_BUF_SIZE;

	viCheckAllocElab(*cmd = (ViChar*)malloc(cmdBufferSize), "Rs_BuildAttrCommandString command malloc");

	RsCoreInt_StrcpyMaxLen(*cmd, cmdBufferSize, attr->command);
	cmdActualLen = (ViInt32)strlen(*cmd);
	if (attr->usercaps == VI_TRUE)
	{
		/* Repeated Capabilities */
		for (index = 0; index < RSCORE_MAX_ATTR_RCAPS; index++)
		{
			// Scan repCap from command e.g. 'CAL{Channel}:ZERO{Sensor}:AUTO',
			// we need the 'Channel' part, in the second iteration then 'Sensor' part
			p2SearchStart = *cmd;
			restLen = cmdActualLen;
			if ((p2LeftBracket = (ViChar *)memchr(p2SearchStart, '{', restLen)) == NULL) break;
			startOffset = (ViInt32)(p2LeftBracket - p2SearchStart);
			restLen -= startOffset;
			if ((p2RightBracket = (ViChar *)memchr(p2LeftBracket, '}', restLen)) == NULL) break;
			repCapIdLen = (ViInt32)(p2RightBracket - p2LeftBracket) - 1;
			restLen -= repCapIdLen + 2;
			memcpy(singleRepCapNameId, p2LeftBracket + 1, repCapIdLen);
			singleRepCapNameId[repCapIdLen] = 0;

			// singleRepCapNameId now contains 'Channel', repCapNames is a string 'CH0,CH1,CH2,CH3,CH4'
			checkErr(RsCoreInt_FindRepCap(instrSession, NULL, singleRepCapNameId, &repCap));

			// Go through the repCapNames string items and search them in the entered repCapsInputString
			if (inputRepCapsCount == 0)
			{
				// If the repCapsInputString is empty, take the 1st one
				checkErr(RsCoreInt_GetRepCapCmdAtIndex(instrSession, repCap, 0, RS_MAX_MESSAGE_LEN, repCapCmd, NULL));
			}
			else
			{
				// inputRepCapsCount are not empty, go through its tokens
				for (i = 0; i < inputRepCapsCount; i++)
				{
					checkErr(RsCore_GetTokenAtIndex(repCapsInputString, ',', i, singleInputRepCap, RS_MAX_MESSAGE_LEN, NULL));
					checkErr(RsCoreInt_FindRepCapName(instrSession, repCap, singleInputRepCap, VI_FALSE, &foundIdx, NULL));
					if (foundIdx >= 0)
					{
						break;
					}
				}

				if (foundIdx < 0)
					foundIdx = 0; // If none of the repCapsInputString tokens contains the singleInputRepCap, take the 1st one

				checkErr(RsCoreInt_GetRepCapCmdAtIndex(instrSession, repCap, foundIdx, RS_MAX_MESSAGE_LEN, repCapCmd, NULL));
			}

			// Here, we have the valid command value in repCapCmd
			cmdValueLen = (ViInt32)strlen(repCapCmd);
			resizeOffset = cmdValueLen - (repCapIdLen + 2); // negative resizeOffset means the *cmd length shrinks

			if (resizeOffset > 0)
			{
				if ((cmdActualLen + resizeOffset) >= cmdBufferSize)
				{
					// need to reallocate the *cmd to a bigger buffer
					cmdBufferSize += resizeOffset + 10;
					viCheckReallocElab(*cmd, (ViChar*)realloc(*cmd, cmdBufferSize), "Memory reallocation for attribute command");
				}
			}

			if (restLen > 0)
			{
				// Part after the right bracket is non-empty, move it
				target = p2LeftBracket + cmdValueLen;
				source = p2RightBracket + 1;
				memmove(target, source, restLen + 1); // including '\0'
				memcpy(p2LeftBracket, repCapCmd, cmdValueLen);
			}
			else
			{
				// No other characters in the command
				// Copy the command value, terminate the string and break
				memcpy(p2LeftBracket, repCapCmd, cmdValueLen + 1); // including '\0'
				break;
			}

			cmdActualLen = (ViInt32)strlen(*cmd);
		} /* EOF RepCaps for loop */
	}
	else
	{
		RsCoreInt_StrcpyMaxLen(*cmd, cmdActualLen + 1, attr->command);
	}

Error:
	return error;
}

/****************************************************************************/
/*  RsCore_WriteAttributeCommand
    Writes attribute command to instrument. Takes into account the following attribute flags:
    RS_VAL_WAIT_FOR_OPC_AFTER_WRITES
    RS_VAL_WAIT_TO_CONTINUE
  *****************************************************************************/
ViStatus RsCore_WriteAttributeCommand(ViSession instrSession, RsCoreAttributePtr attr, ViConstString command)
{
	ViStatus error = VI_SUCCESS;

	if (attrHasFlag(RS_VAL_WAIT_FOR_OPC_AFTER_WRITES))
	{
		return RsCoreInt_WriteWithOpc(instrSession, command, VI_FALSE, 0);
	}
	else if (attrHasFlag(RS_VAL_WAIT_TO_CONTINUE))
	{
		checkErr(RsCore_Write(instrSession, command));
		checkErr(RsCore_Write(instrSession, "*WAI"));
	}
	else
	{
		// Standard write
		return RsCore_Write(instrSession, command);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryAttributeCommand
    Queries attribute command from the instrument.
    The response length is limited to RS_MAX_MESSAGE_BUF_SIZE
    The response is trimmed for the trailing LF
    The function takes into account RS_VAL_WAIT_FOR_OPC_AFTER_WRITES and adds the question-mark "?" to the query
    if not already present. For unlimited response lengths use the RsCore_QueryAttributeCommandUnknownLength()
 *****************************************************************************/
ViStatus RsCore_QueryAttributeCommand(ViSession instrSession, RsCoreAttributePtr attr, ViConstString query, ViChar* response)
{
	return RsCoreInt_QueryAttrCommandUni(instrSession, attr, query, VI_FALSE, &response, NULL);
}

/*****************************************************************************************************/
/*  RsCore_QueryAttributeCommandUnknownLength
    Queries attribute command from the instrument.
    The response length is unlimited.
    The response is trimmed for the trailing LF
    The function takes into account RS_VAL_WAIT_FOR_OPC_AFTER_WRITES and adds the question-mark "?" to the query
    if not already present.
    WARNING!!! De-allocated the response buffer afterwards
******************************************************************************************************/

ViStatus RsCore_QueryAttributeCommandUnknownLength(ViSession instrSession, RsCoreAttributePtr attr,
                                                   ViConstString query, ViChar** response, ViInt32* byteCount)
{
	return RsCoreInt_QueryAttrCommandUni(instrSession, attr, query, VI_TRUE, response, byteCount);
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Repeated Capabilities ---------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/


/*****************************************************************************************************/
/*  RsCore_BuildRepCapTable
    Creates a unique repeated capability table for the session. A repeated capability table
    consists of a name that identifies the repeated capability, the identifier strings and command
    values that are valid for the instrument session.Calling  this function more than once
    for the same repeated capability will return the RS_ERROR_REPEATED_CAPABILITY_ALREADY_EXISTS error.
    The RS Library maintains the repeated capability table for the session.
******************************************************************************************************/
ViStatus RsCore_BuildRepCapTable(ViSession instrSession, RsCoreRepCapPtr repCapTable)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 numberOfRepCaps = 0;
	size_t table_size = 0;
	ViInt32 i = 0;
	ViInt32 bufferSize;
	RsCoreAttributePtr attr;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->repCapTables)
	{
		/* If the session is not being reused (new session created regardless if the same one already exists),
		the rsSession->repCapTable field shall not be initialized */
		if (rsSession->reuseSession == VI_FALSE)
		{
			viCheckErr(RS_ERROR_REPEATED_CAPABILITY_ALREADY_EXISTS);
		}

		/* If the session is not being duplicated (already existing session is being reused)
		the rsSession->repCapTable field may already be initialized
		In that case, dispose of it in order to reset the RepCap Table */
		checkErr(RsCoreInt_DisposeRepCapTable(instrSession));
	}

	/* Determine number of repeated capabilities */
	for (numberOfRepCaps = 0; repCapTable[numberOfRepCaps].repCapNameId; numberOfRepCaps++);

	checkErr(RsCoreInt_GetAttributePtr(instrSession, RS_ATTR_CHANNEL_COUNT, &attr));
	rsSession->repCapsCount = numberOfRepCaps;
	*(ViInt32*)attr->p2value = numberOfRepCaps;

	/* Allocate memory for session based repeated capabilities */
	table_size = (size_t)(numberOfRepCaps * sizeof(repCapTable[0]));
	checkAlloc(rsSession->repCapTables = (RsCoreRepCapPtr)malloc(table_size));
	for (i = 0; i < numberOfRepCaps; i++)
	{
		bufferSize = (ViInt32)strlen(repCapTable[i].repCapNameId) + 1;
		checkAlloc(rsSession->repCapTables[i].repCapNameId = (ViChar*)malloc(bufferSize));
		RsCoreInt_StrcpyMaxLen(rsSession->repCapTables[i].repCapNameId, bufferSize, repCapTable[i].repCapNameId);

		bufferSize = (ViInt32)strlen(repCapTable[i].repCapNames) + 1;
		checkAlloc(rsSession->repCapTables[i].repCapNames = (ViChar*)malloc(bufferSize));
		RsCoreInt_StrcpyMaxLen(rsSession->repCapTables[i].repCapNames, bufferSize, repCapTable[i].repCapNames);

		bufferSize = (ViInt32)strlen(repCapTable[i].cmdValues) + 1;
		checkAlloc(rsSession->repCapTables[i].cmdValues = (ViChar*)malloc(bufferSize));
		RsCoreInt_StrcpyMaxLen(rsSession->repCapTables[i].cmdValues, bufferSize, repCapTable[i].cmdValues);
	}

Error:
	return error;
}

/******************************************************************************************************
* Function: RsCore_ResetRepCapTable
* Purpose:  This function removes all existing repeated capabilities and
*           creates the table again with default values
*
******************************************************************************************************/
ViStatus RsCore_ResetRepCapTable(ViSession instrSession, RsCoreRepCapPtr repCapTable)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCoreInt_DisposeRepCapTable(instrSession));
	checkErr(RsCore_BuildRepCapTable(instrSession, repCapTable));

Error:

	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeRepCapName
    Function used by Hi-level functions to obtain a repCap name from:
    - attributeID
    - repCapNameID
    - 0-based index in the repCapNames
******************************************************************************************************/
ViStatus RsCore_GetAttributeRepCapName(ViSession instrSession,
                                       ViUInt32 attributeId,
                                       ViString repCapNameId,
                                       ViInt32 index,
                                       ViInt32 bufferSize,
                                       ViChar* repCapName)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttributePtr attr;
	RsCoreRepCapPtr repCap;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));
	checkErr(RsCoreInt_FindRepCap(instrSession, NULL, repCapNameId, &repCap));
	checkErr(RsCoreInt_ValidateAttrForRepCap(instrSession, repCap, attr));
	checkErr(RsCoreInt_GetRepCapNameAtIndex(instrSession, repCap, index, bufferSize, repCapName, NULL));

Error:
	switch (error)
	{
	case RS_ERROR_INVALID_ATTRIBUTE:
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Attribute ID " _PERCU " is invalid", attributeId);
		(void)RsCore_SetErrorInfo(instrSession, VI_FALSE, error, VI_FALSE, errElab);
		break;
	case RS_ERROR_ATTRIBUTE_NOT_SUPPORTED:
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Attribute ID " _PERCU " is not supported", attributeId);
		(void)RsCore_SetErrorInfo(instrSession, VI_FALSE, error, VI_FALSE, errElab);
		break;
	default:
		break;
	}

	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeRepCapNameIds
	Returns the repeated capability IDs that belongs to the attribute you specify.
	More then one id is listed in comma separated string.
	Example: Attr command: 'SENS{Channel}:TGAT{Timegate}:TIME'
	Returned repCapNameIds:  'Channel,Timegate'
******************************************************************************************************/
ViStatus RsCore_GetAttributeRepCapNameIds(ViSession instrSession, ViUInt32 attributeId, ViInt32 bufferSize, ViChar* repCapNameIds)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttributePtr attr = NULL;

	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));
	checkErr(RsCoreInt_GetAttrRepCapNameIds(instrSession, attr, bufferSize, repCapNameIds));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetAttributeRepCapNamesAll
	Returns all the repCapNames defined for the entered repCapNameId
	More then one name is listed in comma separated string.
	Example: Attr command: 'SENS{Channel}:TGAT{Timegate}:TIME'
	Entered repCapNameId = 'Channel'
	Returned repCapNames:  'CH0,CH1,CH2,CH3,CH4'
******************************************************************************************************/
ViStatus RsCore_GetAttributeRepCapNamesAll(ViSession instrSession, ViUInt32 attributeId, ViConstString repCapNameId, ViInt32 bufferSize,
                                           ViChar* repCapNames)
{
	ViStatus error = VI_SUCCESS;
	RsCoreAttributePtr attr = NULL;
	RsCoreRepCapPtr repCap;

	checkErr(RsCoreInt_GetAttributePtr(instrSession, attributeId, &attr));
	checkErr(RsCoreInt_FindRepCap(instrSession, NULL, repCapNameId, &repCap));
	checkErr(RsCoreInt_ValidateAttrForRepCap(instrSession, repCap, attr));
	RsCoreInt_StrcpyMaxLen(repCapNames, bufferSize, repCap->repCapNames);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_AddRepCap
    Adds a new dynamic repCapName to the specified repCapNameId.
    If the repCapName already exists, the function ends with no changes and no error
******************************************************************************************************/
ViStatus RsCore_AddRepCap(ViSession instrSession,
                          ViString repCapNameId,
                          ViString repCapName,
                          ViString cmdValue)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 namesBufferSize, cmdsBufferSize;
	RsCoreRepCapPtr repCap;
	ViInt32 foundIdx;

	if (isNullOrEmpty(repCapName))
	{
		viCheckErrElab(RS_ERROR_NULL_POINTER, "Rs_AddRepCap: repCapName is NULL or empty");
	}

	if (cmdValue == NULL)
	{
		viCheckErrElab(RS_ERROR_NULL_POINTER, "Rs_AddRepCap: cmdValue is NULL");
	}

	checkErr(RsCoreInt_FindRepCap(instrSession, NULL, repCapNameId, &repCap));
	checkErr(RsCoreInt_FindRepCapName(instrSession, repCap, repCapName, VI_FALSE, &foundIdx, NULL));

	if (foundIdx >= 0)
	{
		// RepCapName already exists, end the function
		goto Error;
	}

	// RepCapNames expansion
	namesBufferSize = (ViInt32)strlen(repCap->repCapNames) + (ViInt32)strlen(repCapName) + 2;
	viCheckReallocElab(repCap->repCapNames, (ViChar*)realloc(repCap->repCapNames, namesBufferSize), "Rs_AddRepCap: allocated space for repCapName");

	// RepCapCmdValues expansion
	cmdsBufferSize = (ViInt32)strlen(repCap->cmdValues) + (ViInt32)strlen(cmdValue) + 2;
	viCheckReallocElab(repCap->cmdValues, (ViChar*)realloc(repCap->cmdValues, cmdsBufferSize), "Rs_AddRepCap: allocated space for cmdValues");

	RsCore_AppendToCsvString(repCap->repCapNames, ",", namesBufferSize, repCapName);
	RsCore_AppendToCsvString(repCap->cmdValues, ",", cmdsBufferSize, cmdValue);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_RemoveRepCap
    Removes existing dynamic repeated capability.
******************************************************************************************************/
ViStatus RsCore_RemoveRepCap(ViSession instrSession,
                             ViString repCapNameId,
                             ViString repCapName)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 repCapNameLen, repCapCmdLen;
	ViInt32 foundIdx;
	ViInt32 startOffsetName, startOffsetCmd;
	ViChar *target = NULL, *source = NULL;
	ViInt32 rcCount;
	ViChar cmdValue[RS_MAX_MESSAGE_LEN] = "";
	RsCoreRepCapPtr repCap;

	if (isNullOrEmpty(repCapName))
	{
		viCheckErrElab(RS_ERROR_NULL_POINTER, "RemoveRepCap: repCapName is NULL or empty");
	}

	checkErr(RsCoreInt_FindRepCap(instrSession, NULL, repCapNameId, &repCap));
	checkErr(RsCoreInt_FindRepCapName(instrSession, repCap, repCapName, VI_TRUE, &foundIdx, &startOffsetName));
	checkErr(RsCoreInt_GetRepCapCmdAtIndex(instrSession, repCap, foundIdx, RS_MAX_MESSAGE_LEN, cmdValue, &startOffsetCmd));
	checkErr(RsCoreInt_GetRepCapCount(instrSession, repCap, &rcCount));

	repCapNameLen = (ViInt32)strlen(repCapName);
	repCapCmdLen = (ViInt32)strlen(cmdValue);

	if (rcCount == 1)
	{
		// Remove the last remaining repCap
		repCap->repCapNames[0] = 0;
		repCap->cmdValues[0] = 0;
	}
	else if (foundIdx == (ViInt32)rcCount - 1)
	{
		// RepCapName is at the last position, fill startOffset with zeroes
		memset(repCap->repCapNames + startOffsetName - 1, 0, (size_t)(repCapNameLen + 2));
		memset(repCap->cmdValues + startOffsetCmd - 1, 0, (size_t)(repCapCmdLen + 2));
	}
	else
	{
		// Somewhere 0..n-1, but not the last one. Move the rest after it to its place.
		target = repCap->repCapNames + startOffsetName;
		source = target + repCapNameLen + 1;
		memmove(target, source, strlen(source) + 1);

		target = repCap->cmdValues + startOffsetCmd;
		source = target + repCapCmdLen + 1;
		memmove(target, source, strlen(source) + 1);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ResetRepCapID
    Sets selected repeated capability to it default value
    You have to provide the complete list of the default Repeated Capabilities Table
******************************************************************************************************/
ViStatus RsCore_ResetRepCapID(ViSession instrSession,
                              ViString repCapNameId, RsCoreRepCapPtr repCapTable)
{
	ViStatus error = VI_SUCCESS;
	RsCoreRepCapPtr defaultRepCap = NULL;
	RsCoreRepCapPtr repCap = NULL;
	ViInt32 bufferSize;

	checkErr(RsCoreInt_FindRepCap(instrSession, repCapTable, repCapNameId, &defaultRepCap));
	checkErr(RsCoreInt_FindRepCap(instrSession, NULL, repCapNameId, &repCap));

	if (repCap->repCapNames)
		free(repCap->repCapNames);

	if (repCap->cmdValues)
		free(repCap->cmdValues);

	bufferSize = (ViInt32)strlen(defaultRepCap->repCapNames) + 1;
	viCheckAllocElab(repCap->repCapNames = (ViChar*)malloc(bufferSize), "Allocation of buffer for default RepCapNames");
	RsCoreInt_StrcpyMaxLen(repCap->repCapNames, bufferSize, defaultRepCap->repCapNames);

	bufferSize = (ViInt32)strlen(defaultRepCap->cmdValues) + 1;
	viCheckAllocElab(repCap->cmdValues = (ViChar*)malloc(bufferSize), "Allocation of buffer for default RepCapCmdValues");
	RsCoreInt_StrcpyMaxLen(repCap->cmdValues, bufferSize, defaultRepCap->cmdValues);

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Range Tables ------------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_FindDiscreteRangeTableNumericValue
    Finds the entered cmdString in the discrete range table and returns the corresponding numericValue
    If the cmdString is not found, the function returns RS_ERROR_INVALID_VALUE
    Based on if rsSession->recogniseLongResponses:
    If FALSE, the cmdString must be exactly equal to the rangeTable value, e.g. cmdString = 'ENABLED', rangeTable.cmdString = 'ENABLED'
    If TRUE, the cmdString can be a longer version of the rangeTable value, e.g. cmdString = 'ENABLED', rangeTable.cmdString = 'ENAB'
******************************************************************************************************/
ViStatus RsCore_FindDiscreteRangeTableNumericValue(ViSession instrSession, RsCoreRangeTablePtr rangeTable,
                                                   ViConstString cmdString, ViReal64* numericValue)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 i = 0;
	ViInt32 foundIdx = -1;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	// Go over range table values and check if the command string is present
	while (rangeTable->rangeValues[i].cmdValue != RS_RANGE_TABLE_END_VALUE && i < 99999)
	{
		if (rsSession->recogniseLongResponses)
		{
			if (strncmp(rangeTable->rangeValues[i].cmdString, cmdString, strlen(rangeTable->rangeValues[i].cmdString)) == 0)
			{
				foundIdx = i;
				break;
			}
		}
		else
		{
			if (strcmp(rangeTable->rangeValues[i].cmdString, cmdString) == 0)
			{
				foundIdx = i;
				break;
			}
		}

		i++;
	}
	if (i >= 99999)
	{
		viCheckErrElab(RS_ERROR_INVALID_RANGE_TABLE, "RsCore_FindDiscreteRangeTableNumericValue: The Range table has improper ending");
	}

	if (foundIdx == -1)
	{
		*numericValue = INFINITY;
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Cannot find the instrument response '%s' in the list of recognized values: ", cmdString);
		// Compose list of the range table cmd Values
		RsCoreInt_AppendDiscreteRangeTableList(rangeTable, RS_VAL_STRING, RS_MAX_MESSAGE_BUF_SIZE, errElab);
		viCheckErrElab(RS_ERROR_INVALID_VALUE, errElab);
	}
	else
	{
		*numericValue = rangeTable->rangeValues[foundIdx].discreteOrMinValue;
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_FindRangeTableCmdStringViReal64
	Finds the numericValue in the range table and returns the corresponding command string
	If the value is not found, the function returns RS_ERROR_INVALID_VALUE
	Precision for dataType RS_VAL_INT32 is coerced to DBL_MIN
	Warning!!! The function returns the pointer to the actual cmd value in the range table,
	do not modify the value
******************************************************************************************************/
ViStatus RsCore_FindRangeTableCmdStringViReal64(ViSession instrSession, RsCoreRangeTablePtr rangeTable, ViReal64 rangeTableValue,
                                                ViReal64 precision, ViChar** p2cmdString)
{
	return RsCoreInt_FindDiscreteRangeTableCmdString(instrSession, rangeTable, RS_VAL_REAL64, rangeTableValue, precision, p2cmdString);
}

/*****************************************************************************************************/
/*  RsCore_FindRangeTableCmdString
	Finds the numericValue in the range table and returns the corresponding command string
	If the value is not found, the function returns RS_ERROR_INVALID_VALUE
	Warning!!! The function returns the pointer to the actual cmd value in the range table,
	do not modify the value
******************************************************************************************************/
ViStatus RsCore_FindRangeTableCmdString(ViSession instrSession, RsCoreRangeTablePtr rangeTable, ViInt32 rangeTableValue, ViChar** p2cmdString)
{
	return RsCoreInt_FindDiscreteRangeTableCmdString(instrSession, rangeTable, RS_VAL_INT32, rangeTableValue, DBL_MIN, p2cmdString);
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Instrument ASCII IO -----------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_Write
    Writes command string to the instrument. The command length is unlimited.
    For non-VXI-11 sessions, it assures that the command is terminated with LF (rsSession->assureWriteWithLF)
    Note! Do not use for writing binary data blocks. For that purpose use RsCore_WriteBinaryDataBlock... functions
******************************************************************************************************/
ViStatus RsCore_Write(ViSession instrSession, ViConstString writeBuffer)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 cmdLen;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	const ViInt32 cmdMaxLen = RS_MAX_SHORT_MESSAGE_BUF_SIZE - 4;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
		goto Error;

	if (writeBuffer == NULL)
		return RS_ERROR_INVALID_VALUE;

	cmdLen = (ViInt32)strlen(writeBuffer);
	if (cmdLen == 0)
		return RS_ERROR_INVALID_VALUE;

	if (rsSession->writeDelay)
		Sleep(rsSession->writeDelay);

	if (rsSession->assureWriteWithLF && writeBuffer[cmdLen - 1] != '\n')
	{
		// For commands, up to 4092 bytes - RS_MAX_SHORT_MESSAGE_BUF_SIZE
		if (cmdLen < cmdMaxLen)
		{
			RsCoreInt_StrcpyMaxLen(buffer, cmdMaxLen, writeBuffer);
			buffer[cmdLen] = '\n';
			buffer[cmdLen + 1] = 0;
			return viWrite(instrSession, (ViBuf)buffer, cmdLen + 1, NULL);
		}
		else
		{
			// Long commands, send the LF in the 2nd write
			checkErr(viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_FALSE));
			checkErr(viWrite(instrSession, (ViBuf)writeBuffer, cmdLen, NULL));
			checkErr(viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE));
			checkErr(viWrite(instrSession, (ViBuf)"\n", 1, NULL));
		}
	}
	else
	{
		// Write the string as is
		return viWrite(instrSession, (ViBuf)writeBuffer, cmdLen, NULL);
	}
Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_WriteWithOpc
    Writes an OPC-synchronized command to the instrument.
    Set the parameter timeoutMs to 0 in order to use the session's OPC timeout.
    Because of the order of commands, this function also calls the driver's CheckStatus callback.
    If you use this function, do not call the CheckStatus function anymore
    Note! Do not use for writing binary data blocks. For that purpose use RsCore_WriteBinaryDataBlock...WithOpc functions
******************************************************************************************************/
ViStatus RsCore_WriteWithOpc(ViSession instrSession, ViConstString writeBuffer, ViInt32 timeoutMs)
{
	return RsCoreInt_WriteWithOpc(instrSession, writeBuffer, VI_TRUE, timeoutMs);
}

/*****************************************************************************************************/
/*  RsCore_WriteAsciiViInt32Array
    Writes to the instrument command string followed by ViInt32 array formatted as comma-separated values
******************************************************************************************************/
ViStatus RsCore_WriteAsciiViInt32Array(ViSession instrSession, ViConstString command, ViInt32* intArray, ViInt32 arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* completeCmd = NULL;

	checkErr(RsCore_Convert_ViInt32array_To_String(instrSession, intArray, arraySize, command, &completeCmd));
	checkErr(RsCore_Write(instrSession, completeCmd));

Error:
	if (completeCmd)
		free(completeCmd);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_WriteAsciiViReal64Array
    Writes to the instrument command string followed by ViReal64 array formatted as comma-separated values
    Used formatting string: %.12lG
******************************************************************************************************/
ViStatus RsCore_WriteAsciiViReal64Array(ViSession instrSession, ViConstString command, ViReal64* dblArray, ViInt32 arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* completeCmd = NULL;

	checkErr(RsCore_Convert_ViReal64array_To_String(instrSession, dblArray, arraySize, command, &completeCmd));
	checkErr(RsCore_Write(instrSession, completeCmd));

Error:
	if (completeCmd)
		free(completeCmd);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_WriteAsciiViBooleanArray
    Writes to the instrument command string followed by ViBoolean array formatted as comma-separated values
    Boolean representation to string: VI_TRUE = 'ON', VI_FALSE = 'OFF'
******************************************************************************************************/
ViStatus RsCore_WriteAsciiViBooleanArray(ViSession instrSession, ViConstString command, ViBoolean* boolArray, ViInt32 arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* completeCmd = NULL;

	checkErr(RsCore_Convert_ViBooleanArray_To_String(instrSession, boolArray, arraySize, command, &completeCmd));
	checkErr(RsCore_Write(instrSession, completeCmd));

Error:
	if (completeCmd)
		free(completeCmd);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_ReadViStringUnknownLength
    Reads data from the instrument and returns the allocated buffer with the read data
    The response is trimmed for trailing LF-characters and always null-terminated.
    WARNING!!! - Deallocate the outputBuffer after use.
    Note! Do not use for reading binary data, for that purpose use the RsCore_ReadBinaryDataBlock... functions
******************************************************************************************************/
ViStatus RsCore_ReadViStringUnknownLength(ViSession instrSession, ViChar** responseString)
{
	ViStatus error = VI_SUCCESS;
	ViInt64 byteCount;

	checkErr(RsCoreInt_ReadDataUnknownLength(instrSession, (ViByte**)responseString, &byteCount));
	// Cut one LF at the end if present
	if (byteCount > 0 && (*responseString)[byteCount - 1] == '\n')
	{
		(*responseString)[byteCount - 1] = 0;
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ReadUpToCharacter
	Reads data byte-by-byte until it encounters the stopChar, or the bufferSize limit, or the MSG_END
	If the required stop character is not successfully read,  the function generates RS_ERROR_UNEXPECTED_RESPONSE
	Returns the buffer including the stopChar, retCnt includes the stopChar. The rest of the buffer is filled with zeroes
******************************************************************************************************/
ViStatus RsCore_ReadUpToCharacter(ViSession instrSession, ViChar stopChar, ViInt32 bufferSize, ViBuf buffer, ViInt32* retCnt)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;
	ViInt32 i;
	ViBoolean success = VI_FALSE;
	ViUInt32 readCount;

	*retCnt = 0;
	buffer[0] = 0;
	
	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	// Only apply the read delay once at the beginning
	if (rsSession->readDelay)
		Sleep(rsSession->readDelay);

	for (i = 0; i < bufferSize; i++)
	{
		checkErr(viRead(instrSession, buffer + i, 1, &readCount));
		if (buffer[i] == stopChar)
		{
			error = VI_SUCCESS;
			success = VI_TRUE;
			break;
		}
		if (error != VI_SUCCESS_MAX_CNT)
			break;
	}

	if (!success)
	{
		viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, "RsCore_ReadUpToCharacter: The required stop character was not read");
	}

	if (i < bufferSize - 1)
	{
		// Fill up with nulls
		memset(buffer + i + 1, 0, bufferSize - i - 1);
	}
	
	*retCnt = i + 1;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViString
    Queries string from the instrument. The response length is limited.
    The response is trimmed for trailing LF-characters and always null-terminated.
    You have to provide a response buffer with minimum size of RS_MAX_MESSAGE_BUF_SIZE (4096 bytes)
    You can set the responseString to NULL if you do not need the response
    Note! Do not use for reading binary data blocks. For that purpose use RsCore_QueryBinaryDataBlock... functions
******************************************************************************************************/
ViStatus RsCore_QueryViString(ViSession instrSession, ViConstString query, ViChar* responseString)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 returnedCount = 0;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar auxResponse[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar* p2response = (responseString == NULL ? auxResponse : responseString);

	if (RsCore_Simulating(instrSession))
	{
		if (responseString != VI_NULL)
			responseString[0] = 0;

		goto Error;
	}

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCoreInt_Read(instrSession, RS_MAX_MESSAGE_BUF_SIZE, (ViByte *)p2response, &returnedCount));
	if (returnedCount >= RS_MAX_MESSAGE_BUF_SIZE)
	{
		// Truncate the message, it is too long
		p2response[RS_MAX_MESSAGE_BUF_SIZE - 1] = '\0';
		if (p2response[RS_MAX_MESSAGE_BUF_SIZE - 2] != '\n')
		{
			// Report an error, the response is not complete
			p2response[64] = 0;
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected response on query '%s', longer than maximum %d bytes: '%64s...' "
			         "The response probably comes from a previous not completely read-out response",
			         query, RS_MAX_MESSAGE_BUF_SIZE, p2response);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}
	else
	{
		p2response[returnedCount] = '\0';
	}

	RsCore_TrimString(p2response, RS_VAL_TRIM_WHITESPACES);

Error:
	if (error < 0)
		RsCoreInt_SpecifyVisaError(instrSession, error, "RsCore_QueryViString", query);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViStringShort
    Queries string from the instrument. The response length is limited to RS_MAX_SHORT_MESSAGE_BUF_SIZE (64 bytes)
    The response is trimmed for trailing LF-characters and always null-terminated.
    You have to provide a response buffer with minimum size of RS_MAX_SHORT_MESSAGE_BUF_SIZE
    You can set the response buffer to NULL if you do not need the response
    The function returns
    Note! Use this function instead of RsCore_QueryViString when you expect a short response e.g. an integer number
******************************************************************************************************/
ViStatus RsCore_QueryViStringShort(ViSession instrSession, ViConstString query, ViChar* responseString)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 returnedCount = 0;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar auxResponse[RS_MAX_SHORT_MESSAGE_BUF_SIZE];
	ViChar* p2response = (responseString == NULL ? auxResponse : responseString);

	if (RsCore_Simulating(instrSession))
	{
		if (responseString != VI_NULL)
			responseString[0] = 0;

		goto Error;
	}

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCoreInt_Read(instrSession, RS_MAX_SHORT_MESSAGE_BUF_SIZE, (ViByte *)p2response, &returnedCount));
	if (returnedCount >= RS_MAX_SHORT_MESSAGE_BUF_SIZE)
	{
		// Truncate the message, it is too long
		p2response[RS_MAX_SHORT_MESSAGE_BUF_SIZE - 1] = '\0';
		if (p2response[RS_MAX_SHORT_MESSAGE_BUF_SIZE - 2] != '\n')
		{
			// Report an error, the response is not complete
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Unexpected response on query '%s', longer than maximum %d bytes: '%32s...' "
			         "The response probably comes from a previous not completely read-out response",
			         query, RS_MAX_SHORT_MESSAGE_BUF_SIZE, p2response);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}
	else
	{
		p2response[returnedCount] = '\0';
	}

	RsCore_TrimString(p2response, RS_VAL_TRIM_WHITESPACES);

Error:
	if (error < 0)
		RsCoreInt_SpecifyVisaError(instrSession, error, "RsCore_QueryViStringShort", query);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViStringUnknownLength
    Queries string response from the instrument. The response length is unlimited.
    The response is trimmed for trailing LF-characters and always null-terminated.
    WARNING!!! - Deallocate the outputBuffer after use.
    Note! Do not use for querying binary data, for that purpose use the RsCore_QueryBinaryDataBlock... functions
******************************************************************************************************/
ViStatus RsCore_QueryViStringUnknownLength(ViSession instrSession, ViConstString query, ViChar** responseString)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCore_ReadViStringUnknownLength(instrSession, responseString));

Error:

	if (error == VI_ERROR_TMO)
		RsCoreInt_SpecifyVisaError(instrSession, error, "RsCore_QueryViStringUnknownLength", query);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViStringWithOpc
    Sends an OPC-synchronized query to the instrument. The response length is limited.
    The response is trimmed for any leading LF characters and terminated with null-character
    Set the parameter timeoutMs to 0 in order to use the session OPC timeout.
    Because of the order of commands, this function also calls the driver's CheckStatus callback.
    If you use this function, do not call the CheckStatus anymore
    Note! Do not use for querying binary data, for that purpose use the RsCore_QueryBinaryDataBlock...withOpc functions
******************************************************************************************************/
ViStatus RsCore_QueryViStringWithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs, ViInt32 bufferSize, ViChar* responseString)
{
	return RsCoreInt_QueryViStringWithOpc(instrSession, query, VI_TRUE, timeoutMs, bufferSize, responseString);
}

/*****************************************************************************************************/
/*  RsCore_QueryViStringUnknownLengthWithOpc
    Sends an OPC-synchronized query to the instrument. The response length is unlimited.
    The returned string is trimmed for LF and allocated buffer always at least one byte bigger than the returned byteCount, and has always
    minimum of one null at the end. This allows you to use the outputBuffer as a properly null-terminated string.
    Set the parameter timeoutMs to 0 in order to use the session OPC timeout.
    Because of the order of commands, this function also calls the driver's CheckStatus callback.
    If you use this function, do not call the CheckStatus anymore
    WARNING!!! - Deallocate the responseString after use.
    Note! Do not use for querying binary data, for that purpose use the RsCore_QueryBinaryDataBlock...WithOpc functions
******************************************************************************************************/
ViStatus RsCore_QueryViStringUnknownLengthWithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs, ViChar** responseString)
{
	ViStatus error = VI_SUCCESS;
	ViInt64 byteCount;

	checkErr(RsCoreInt_QueryUnknownLengthWithOpc(instrSession, query, VI_TRUE, timeoutMs, (ViByte **)responseString, &byteCount));
	RsCore_TrimString(*responseString, RS_VAL_TRIM_WHITESPACES);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViInt32
    Uses RsCore_QueryInstrShortString to query the instrument data and returns it as ViInt32 value
    If you do not need the response, set the value to NULL
******************************************************************************************************/
ViStatus RsCore_QueryViInt32(ViSession instrSession, ViConstString query, ViPInt32 value)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE] = "0";

	if (RsCore_Simulating(instrSession))
	{
		if (value)
			*value = 1;

		goto Error;
	}

	checkErr(RsCore_QueryViStringShort(instrSession, query, response));

	if (value)
		*value = RsCore_Convert_String_To_ViInt32(response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViInt32WithOpc
	Sends an OPC-synchronized query to the instrument and returns it as ViInt32 value
	The response read is limited to RS_MAX_SHORT_MESSAGE_BUF_SIZE (64 bytes)
	Set the parameter timeoutMs to 0 in order to use the session OPC timeout.
	Because of the order of commands, this function also calls the driver's CheckStatus callback.
	If you use this function, do not call the CheckStatus anymore
	If you do not need the response, set the value to NULL
******************************************************************************************************/
ViStatus RsCore_QueryViInt32WithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs, ViPInt32 value)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE] = "0";

	if (RsCore_Simulating(instrSession))
	{
		if (value)
			*value = 1;

		goto Error;
	}

	checkErr(RsCoreInt_QueryViStringWithOpc(instrSession, query, VI_TRUE, timeoutMs, RS_MAX_SHORT_MESSAGE_BUF_SIZE, response));

	if (value)
		*value = RsCore_Convert_String_To_ViInt32(response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViReal64
    Uses RsCore_QueryInstrShortString to query the instrument data and returns it as ViReal64 value
    If you do not need the response, set the value to NULL
******************************************************************************************************/
ViStatus RsCore_QueryViReal64(ViSession instrSession, ViConstString query, ViPReal64 value)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE] = "0.0";

	if (RsCore_Simulating(instrSession))
	{
		if (value)
			*value = 1.0;

		goto Error;
	}

	checkErr(RsCore_QueryViStringShort(instrSession, query, response));

	if (value)
		*value = RsCore_Convert_String_To_ViReal64(response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViReal64WithOpc
	Sends an OPC-synchronized query to the instrument and returns it as ViReal64 value
	The response read is limited to RS_MAX_SHORT_MESSAGE_BUF_SIZE (64 bytes)
	Set the parameter timeoutMs to 0 in order to use the session OPC timeout.
	Because of the order of commands, this function also calls the driver's CheckStatus callback.
	If you use this function, do not call the CheckStatus anymore
	If you do not need the response, set the value to NULL
******************************************************************************************************/
ViStatus RsCore_QueryViReal64WithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs, ViPReal64 value)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE] = "0.0";

	if (RsCore_Simulating(instrSession))
	{
		if (value)
			*value = 1.0;

		goto Error;
	}

	checkErr(RsCoreInt_QueryViStringWithOpc(instrSession, query, VI_TRUE, timeoutMs, RS_MAX_SHORT_MESSAGE_BUF_SIZE, response));

	if (value)
		*value = RsCore_Convert_String_To_ViReal64(response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViBoolean
    Uses RsCore_QueryInstrShortString to query the instrument data and returns it as ViBoolean value
    If you do not need the response, set the value to NULL
******************************************************************************************************/
ViStatus RsCore_QueryViBoolean(ViSession instrSession, ViConstString query, ViPBoolean value)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE] = "OFF";

	if (RsCore_Simulating(instrSession))
	{
		if (value)
			*value = VI_FALSE;

		goto Error;
	}

	checkErr(RsCore_QueryViStringShort(instrSession, query, response));

	if (value)
		*value = RsCore_Convert_String_To_Boolean(response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryViBooleanWithOpc
	Sends an OPC-synchronized query to the instrument and returns it as ViBoolean value
	The response read is limited to RS_MAX_SHORT_MESSAGE_BUF_SIZE (64 bytes)
	Set the parameter timeoutMs to 0 in order to use the session OPC timeout.
	Because of the order of commands, this function also calls the driver's CheckStatus callback.
	If you use this function, do not call the CheckStatus anymore
	If you do not need the response, set the value to NULL
******************************************************************************************************/
ViStatus RsCore_QueryViBooleanWithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs, ViPBoolean value)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_SHORT_MESSAGE_BUF_SIZE] = "OFF";

	if (RsCore_Simulating(instrSession))
	{
		if (value)
			*value = VI_FALSE;

		goto Error;
	}

	checkErr(RsCoreInt_QueryViStringWithOpc(instrSession, query, VI_TRUE, timeoutMs, RS_MAX_SHORT_MESSAGE_BUF_SIZE, response));

	if (value)
		*value = RsCore_Convert_String_To_Boolean(response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryStringIndex
	Queries string response from the instrument, trims it for LF, and searches for it in the provided patternsArray.
	The last element of the patternsArray must be NULL
	Returned patternIdx is the index of the response in the patternsArray
	If the response is not found, the patternIdx = -1
	The response length is limited to the maximum of RS_MAX_MESSAGE_BUF_SIZE (4096 bytes)
******************************************************************************************************/
ViStatus RsCore_QueryStringIndex(ViSession instrSession, ViConstString query, ViString patternsArray[], ViInt32* patternIdx)
{
	ViStatus error = VI_SUCCESS;
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	*patternIdx = -1;

	if (RsCore_Simulating(instrSession))
	{
		*patternIdx = 0;
		goto Error;
	}

	checkErr(RsCore_QueryViString(instrSession, query, response));
	*patternIdx = RsCore_FindStringIndex(patternsArray, response);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryAsciiViReal64Array
    Queries the instrument for an ASCII-format comma-separated float values
    and returns them as ViReal64 array
    WARNING!!! - Deallocate the dblArray after use.
******************************************************************************************************/
ViStatus RsCore_QueryAsciiViReal64Array(ViSession instrSession, ViConstString query, ViReal64** dblArray, ViInt32* arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* auxString = NULL;

	if (RsCore_Simulating(instrSession))
	{
		*arraySize = 1;
		checkAlloc(*dblArray = (ViReal64 *)calloc(1, sizeof(ViReal64)));
		(*dblArray)[0] = 0.0;

		goto Error;
	}

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, query, &auxString));
	checkErr(RsCore_Convert_String_To_ViReal64Array(instrSession, auxString, dblArray, arraySize));

Error:
	if (auxString)
		free(auxString);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryAsciiViInt32Array
    Queries the instrument for an ASCII-format comma-separated integer values
    and returns them as ViInt32 array
    WARNING!!! - Deallocate the intArray after use.
******************************************************************************************************/
ViStatus RsCore_QueryAsciiViInt32Array(ViSession instrSession, ViConstString query, ViInt32** intArray, ViInt32* arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* auxString = NULL;

	if (RsCore_Simulating(instrSession))
	{
		*arraySize = 1;
		checkAlloc(*intArray = (ViInt32 *)calloc(1, sizeof(ViInt32)));
		(*intArray)[0] = 0;

		goto Error;
	}

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, query, &auxString));
	checkErr(RsCore_Convert_String_To_ViInt32Array(instrSession, auxString, intArray, arraySize));

Error:
	if (auxString)
		free(auxString);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryAsciiViBooleanArray
    Queries the instrument for an ASCII-format comma-separated boolean values
    and returns them as ViBoolean array
    WARNING!!! - Deallocate the boolArray after use.
******************************************************************************************************/
ViStatus RsCore_QueryAsciiViBooleanArray(ViSession instrSession, ViConstString query, ViBoolean** boolArray, ViInt32* arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* auxString = NULL;

	if (RsCore_Simulating(instrSession))
	{
		*arraySize = 1;
		checkAlloc(*boolArray = (ViBoolean *)calloc(1, sizeof(ViBoolean)));
		(*boolArray)[0] = VI_FALSE;

		goto Error;
	}

	checkErr(RsCore_QueryViStringUnknownLength(instrSession, query, &auxString));
	checkErr(RsCore_Convert_String_To_ViBooleanArray(instrSession, auxString, boolArray, arraySize));

Error:
	if (auxString)
		free(auxString);

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Instrument Binary Data IO -----------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_WriteBinaryDataBlock
    Writes binary data block to the instrument including binary data header.
    You just need to provide the command, payload data and the payload length
******************************************************************************************************/
ViStatus RsCore_WriteBinaryDataBlock(ViSession instrSession, ViConstString command, ViByte* data, ViInt64 dataSize)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViByte* p2data = data;
	ViInt64 remainingBytes;
	ViInt32 chunkSize;
	ViUInt32 retCnt;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	chunkSize = rsSession->ioSegmentSize;
	if (dataSize < chunkSize)
	{
		// Can be written in one segment
		checkErr(RsCoreInt_SendBinDataHeader(instrSession, command, dataSize));
		if (rsSession->addTermCharToWriteBinBlock)
		{
			checkErr(viWrite(instrSession, data, (ViUInt32)dataSize, &retCnt));
			errElab[0] = '\n';
			(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
			checkErr(viWrite(instrSession, (ViBuf)errElab, 1, NULL));
		}
		else
		{
			(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
			checkErr(viWrite(instrSession, data, (ViUInt32)dataSize, &retCnt));
		}

		if (dataSize != (ViInt64)retCnt)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "Rs_WriteBinaryData: Written data length " _PERCU " bytes is different than the indicated dataSize size " _PERCLD " bytes",
			         retCnt, dataSize);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}
	else
	{
		// Write in more segments
		remainingBytes = dataSize;
		checkErr(RsCoreInt_SendBinDataHeader(instrSession, command, dataSize));
		while (remainingBytes > 0)
		{
			if (remainingBytes > chunkSize)
			{
				checkErr(viWrite(instrSession, p2data, chunkSize, &retCnt));
				remainingBytes -= retCnt;
				p2data += retCnt;

				if (chunkSize != (ViInt32)retCnt)
				{
					snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
					         "Rs_WriteBinaryData: Written data chunk " _PERCU " bytes is different than the indicated written data size " _PERCU " bytes",
					         chunkSize, retCnt);
					viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
				}
			}
			else
			{
				// Last chunk write
				if (rsSession->addTermCharToWriteBinBlock)
				{
					checkErr(viWrite(instrSession, p2data, (ViUInt32)remainingBytes, &retCnt));
					errElab[0] = '\n';
					(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
					checkErr(viWrite(instrSession, (ViBuf)errElab, 1, NULL));
				}
				else
				{
					(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
					checkErr(viWrite(instrSession, p2data, (ViUInt32)remainingBytes, &retCnt));
				}

				if (remainingBytes != retCnt)
				{
					snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
					         "Rs_WriteBinaryData: Written data last chunk " _PERCLD " bytes is different than the indicated written data size " _PERCU
					         " bytes",
					         remainingBytes, retCnt);
					viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
				}
				remainingBytes -= retCnt;
				break;
			}
		}

		if (remainingBytes != 0)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "RsCore_WriteBinaryDataBlock: Written data length " _PERCLD " bytes is different than the indicated data size " _PERCLD " bytes",
			         dataSize - remainingBytes, dataSize);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}

Error:
	(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryBinaryDataBlock
    Queries binary data of from the instrument.
    If errorIfNotBinDataBlock == TRUE, the function generates error in case the
    data is not binary block (not starting with '#')
    If errorIfNotBinDataBlock == FALSE, the function returns read data and indicates isBinDataBlock = FALSE.
    If you do not need the isBinDataBlock info, set the pointer to NULL
    WARNING!!! Deallocate the buffer after use
******************************************************************************************************/
ViStatus RsCore_QueryBinaryDataBlock(ViSession instrSession,
                                     ViConstString query,
                                     ViBoolean errorIfNotBinDataBlock,
                                     ViBoolean* isBinDataBlock,
                                     ViByte** outputBuffer,
                                     ViInt64* byteCount)
{
	ViStatus error = VI_SUCCESS;

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCoreInt_ReadBinaryDataBlock(instrSession, errorIfNotBinDataBlock, isBinDataBlock, outputBuffer, byteCount));

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_QueryBinaryDataBlockWithOpc
    Queries binary data from the instrument with OPC-synchronization
    If errorIfNotBinDataBlock == TRUE, the function generates error in case the
    data is not binary block (not starting with '#')
    If errorIfNotBinDataBlock == FALSE, the function returns read data and indicates isBinDataBlock = FALSE.
    If you do not need the isBinDataBlock info, set the pointer to NULL
    Because of the order of commands, this function also calls the driver's CheckStatus callback.
    If you use this function, do not call the CheckStatus anymore
    WARNING!!! Deallocate the buffer after use
******************************************************************************************************/
ViStatus RsCore_QueryBinaryDataBlockWithOpc(ViSession instrSession,
                                            ViConstString query,
                                            ViBoolean errorIfNotBinDataBlock,
                                            ViInt32 timeoutMs,
                                            ViBoolean* isBinDataBlock,
                                            ViByte** outputBuffer,
                                            ViInt64* byteCount)
{
	return RsCoreInt_QueryBinaryDataBlockWithOpc(instrSession, query, errorIfNotBinDataBlock, isBinDataBlock, VI_TRUE, timeoutMs, outputBuffer,
	                                             byteCount);
}

/*****************************************************************************************************/
/*  RsCore_QueryBinaryDataBlockToFile
    This function queries instrument for binary data and writes the response to the provided file.
    The file is opened in binary mode.
******************************************************************************************************/
ViStatus RsCore_QueryBinaryDataBlockToFile(ViSession instrSession,
                                           ViConstString query,
                                           ViConstString filename,
                                           ViInt32 fileAction)
{
	ViStatus error = VI_SUCCESS;
	FILE* file = NULL;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt64 length = 0;

	if ((file = fopen(filename, (fileAction == RS_VAL_APPEND) ? "ab" : "wb")) == NULL)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Rs_QueryBinaryDataBlockToFile: File '%s' cannot be opened for writing", filename);
		viCheckErrElab(RS_ERROR_CANNOT_OPEN_FILE, errElab);
	}

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCoreInt_ParseBinaryDataHeader(instrSession, VI_TRUE, NULL, &length, NULL));
	checkErr(RsCoreInt_ReadDataKnownLengthToFile(instrSession, length, file, NULL));

Error:
	if (file)
		(void)fclose(file);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_WriteBinaryDataFromFile
Writes binary data from a file you specify and writes it to instrument using VISA I/O.
This function composes and sends the required binary data header.
******************************************************************************************************/
ViStatus RsCore_WriteBinaryDataFromFile(ViSession instrSession, ViConstString command, ViConstString sourceFile)
{
	ViStatus error = VI_SUCCESS;
	ViByte* buffer = NULL;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	FILE* file = NULL;
	ViInt64 fileSize = 0, remainingBytes;
	ViInt32 chunkSize;
	ViInt64 bytesRead;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if ((file = fopen(sourceFile, "rb")) == NULL)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "RsCore_WriteBinaryDataFromFile: File '%s' cannot be opened for reading", sourceFile);
		viCheckErrElab(RS_ERROR_CANNOT_OPEN_FILE, errElab);
	}

#if defined (_CVI_)

    ViUInt32 fileSize32;
    (void)fseek(file, 0, SEEK_END);
    fileSize32 = ftell(file);
    (void)fseek(file, 0, SEEK_SET);
    fileSize = (ViInt64)fileSize32;

#elif defined(_MSC_VER)

	(void)fseek(file, 0, SEEK_END);
	fileSize = (ViInt64)_ftelli64(file);
	(void)fseek(file, 0, SEEK_SET);

#else

	(void)fseek(file, 0, SEEK_END);
	fileSize = (ViInt64)ftell(file);
	(void)fseek(file, 0, SEEK_SET);

#endif

	if (fileSize < 1)
		fileSize = 1;

	chunkSize = rsSession->ioSegmentSize;
	if (fileSize < chunkSize)
	{
		// Can be written in one segment
		viCheckAllocElab(buffer = (ViByte*)calloc(sizeof(ViByte*), (size_t)fileSize),
			"RsCore_WriteBinaryDataFromFile: Allocation of the fileSize memory");
		bytesRead = (ViInt64)fread(buffer, sizeof(ViByte), (size_t)fileSize, file);
		checkErr(RsCoreInt_SendBinDataHeader(instrSession, command, fileSize));
		if (rsSession->addTermCharToWriteBinBlock)
		{
			checkErr(viWrite(instrSession, buffer, (ViInt32)fileSize, NULL));
			buffer[0] = '\n';
			(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
			checkErr(viWrite(instrSession, buffer, 1, NULL));
		}
		else
		{
			(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
			checkErr(viWrite(instrSession, buffer, (ViUInt32)fileSize, NULL));
		}

		if (bytesRead != fileSize)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "RsCore_WriteBinaryDataFromFile: Length of data read from the file " _PERCLD " bytes does not match the file length " _PERCLD " bytes",
			         bytesRead, fileSize);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}
	else
	{
		// Write in more segments
		remainingBytes = fileSize;
		viCheckAllocElab(buffer = (ViByte*)calloc(sizeof(ViByte*), chunkSize), "RsCore_WriteBinaryDataFromFile: Allocation of the chunkSize memory");
		checkErr(RsCoreInt_SendBinDataHeader(instrSession, command, fileSize));
		while (feof(file) == 0)
		{
			if (remainingBytes > chunkSize)
			{
				bytesRead = (ViUInt32)fread(buffer, sizeof(ViByte), chunkSize, file);
				checkErr(viWrite(instrSession, buffer, (ViUInt32)bytesRead, NULL));
				remainingBytes -= bytesRead;

				if (bytesRead != chunkSize)
				{
					snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
					         "RsCore_WriteBinaryDataFromFile: Length of data read from the file " _PERCLD " bytes does not match the chunk size " _PERCU
					         " bytes",
					         bytesRead, chunkSize);
					viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
				}
			}
			else
			{
				// Last chunk write
				bytesRead = (ViInt64)fread(buffer, sizeof(ViByte), (ViUInt32)remainingBytes, file);
				if (rsSession->addTermCharToWriteBinBlock)
				{
					checkErr(viWrite(instrSession, buffer, (ViUInt32)bytesRead, NULL));
					buffer[0] = '\n';
					(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
					checkErr(viWrite(instrSession, buffer, 1, NULL));
				}
				else
				{
					(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);
					checkErr(viWrite(instrSession, buffer, (ViUInt32)bytesRead, NULL));
				}
				remainingBytes -= bytesRead;
				break;
			}
		}

		if (remainingBytes != 0)
		{
			snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
			         "RsCore_WriteBinaryDataFromFile: Written data length " _PERCLD " bytes is smaller than the indicated file size " _PERCLD " bytes",
			         fileSize - remainingBytes, fileSize);
			viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
		}
	}

Error:
	(void)viSetAttribute(instrSession, VI_ATTR_SEND_END_EN, VI_TRUE);

	if (file)
		(void)fclose(file);

	if (buffer)
		free(buffer);

	return error;
}

/******************************************************************************************************/
/*  QueryBinaryOrAsciiFloatArray
    Queries an array of floating-point numbers that can be returned in ASCII format or in binary format.
    The array is always returned as the most-universal ViReal64 array.
    - For ASCII format, the array numbers are decoded as comma-separated values.
    - For Binary Format, the numbers are decoded based on the property binaryFloatNumbersFormat, usually float 32-bit (FORM REAL,32)
    WARNING!!! - Deallocate the outDblArray after use. */
/******************************************************************************************************/
ViStatus RsCore_QueryBinaryOrAsciiFloatArray(ViSession instrSession, ViConstString query, ViReal64** outDblArray, ViInt32* outArraySize)
{
	ViStatus error = VI_SUCCESS;
	ViByte* data = NULL;
	ViBoolean isBinDataBlock;
	ViInt64 byteCount64;
	ViInt32 byteCount;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*outArraySize = 1;
		checkAlloc(*outDblArray = (ViReal64 *)calloc(1, sizeof(ViReal64)));
		(*outDblArray)[0] = 0.0;

		goto Error;
	}

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCoreInt_ReadBinaryDataBlock(instrSession, VI_FALSE, &isBinDataBlock, &data, &byteCount64));
	byteCount = (ViInt32)byteCount64;
	if (isBinDataBlock)
	{
		switch (rsSession->binaryFloatNumbersFormat)
		{
		case RS_VAL_BIN_FLOAT_FORMAT_SINGLE_4BYTES:
			checkErr(RsCore_Convert_BinaryDataBlock_To_Float32Array(instrSession, data, byteCount, outDblArray, outArraySize, VI_FALSE));
			break;
		case RS_VAL_BIN_FLOAT_FORMAT_SINGLE_4BYTES_SWAPPED:
			checkErr(RsCore_Convert_BinaryDataBlock_To_Float32Array(instrSession, data, byteCount, outDblArray, outArraySize, VI_TRUE));
			break;
		case RS_VAL_BIN_FLOAT_FORMAT_DOUBLE_8BYTES:
			*outArraySize = byteCount / 8;
			*outDblArray = (ViReal64 *)data; // Same data, do not free the data buffer
			break;
		case RS_VAL_BIN_FLOAT_FORMAT_DOUBLE_8BYTES_SWAPPED:
			*outArraySize = byteCount / 8;
			RsCore_SwapEndianness(data, *outArraySize, 8);
			*outDblArray = (ViReal64 *)data; // Same data, do not free the data buffer
			break;
		default:
			viCheckErrElab(VI_ERROR_NSUP_MODE,
				"RsCore_QueryBinaryOrAsciiFloatArray: Unsupported mode of conversion BinDataBlock -> FloatArray[]");
			break;
		}
	}
	else
	{
		RsCore_TrimString((ViChar*)data, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		checkErr(RsCore_Convert_String_To_ViReal64Array(instrSession, (ViChar*)data, outDblArray, outArraySize));
	}

Error:
	// Same data buffer, do not free it, it goes to the user
	if (data && (ViReal64 *)data != *outDblArray)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryBinaryOrAsciiFloatArrayWithOpc
    Queries with OPC an array of floating-point numbers that can be returned in ASCII format or in binary format.
    Set the parameter timeoutMs to 0 in order to use the session's OPC timeout.
    Because of the order of commands, this function also calls the driver's CheckStatus callback.
    If you use this function, do not call the CheckStatus anymore
    The array is always returned as the most-universal ViReal64 array.
    - For ASCII format, the array numbers are decoded as comma-separated values.
    - For Binary Format, the numbers are decoded based on the property binaryFloatNumbersFormat, usually float 32-bit (FORM REAL,32)
    WARNING!!! - Deallocate the outDblArray after use. */
/******************************************************************************************************/
ViStatus RsCore_QueryBinaryOrAsciiFloatArrayWithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs,
                                                    ViReal64** outDblArray, ViInt32* outArraySize)
{
	ViStatus error = VI_SUCCESS;
	ViByte* data = NULL;
	ViBoolean isBinDataBlock;
	ViInt32 byteCount;
	ViInt64 byteCount64;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*outArraySize = 1;
		checkAlloc(*outDblArray = (ViReal64 *)calloc(1, sizeof(ViReal64)));
		(*outDblArray)[0] = 0.0;

		goto Error;
	}

	checkErr(RsCoreInt_QueryBinaryDataBlockWithOpc(instrSession, query, VI_FALSE, &isBinDataBlock, VI_TRUE, timeoutMs, &data, &byteCount64));
	byteCount = (ViInt32)byteCount64;
	if (isBinDataBlock)
	{
		switch (rsSession->binaryFloatNumbersFormat)
		{
		case RS_VAL_BIN_FLOAT_FORMAT_SINGLE_4BYTES:
			checkErr(RsCore_Convert_BinaryDataBlock_To_Float32Array(instrSession, data, byteCount, outDblArray, outArraySize, VI_FALSE));
			break;
		case RS_VAL_BIN_FLOAT_FORMAT_SINGLE_4BYTES_SWAPPED:
			checkErr(RsCore_Convert_BinaryDataBlock_To_Float32Array(instrSession, data, byteCount, outDblArray, outArraySize, VI_TRUE));
			break;
		case RS_VAL_BIN_FLOAT_FORMAT_DOUBLE_8BYTES:
			*outArraySize = byteCount / 8;
			*outDblArray = (ViReal64 *)data; // Same data, do not free the data buffer
			break;
		case RS_VAL_BIN_FLOAT_FORMAT_DOUBLE_8BYTES_SWAPPED:
			*outArraySize = byteCount / 8;
			RsCore_SwapEndianness(data, *outArraySize, 8);
			*outDblArray = (ViReal64 *)data; // Same data, do not free the data buffer
			break;
		default:
			viCheckErrElab(VI_ERROR_NSUP_MODE,
				"RsCore_QueryBinaryOrAsciiFloatArrayWithOpc: Unsupported mode of conversion BinDataBlock -> FloatArray[]");
			break;
		}
	}
	else
	{
		RsCore_TrimString((ViChar*)data, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		checkErr(RsCore_Convert_String_To_ViReal64Array(instrSession, (ViChar*)data, outDblArray, outArraySize));
	}

Error:
	// Same data buffer, do not free it, it goes to the user
	if (data && (ViReal64 *)data != *outDblArray)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryBinaryOrAsciiIntegerArray
    Queries an array of integer numbers that can be returned in ASCII format or in binary format.
    The array is always returned as the most-universal ViInt32 array.
    - For ASCII format, the array numbers are decoded as comma-separated values.
    - For Binary Format, the numbers are decoded based on the property binaryIntegerNumbersFormat, usually integer 32-bit (FORM INT,32)
    WARNING!!! - Deallocate the outInt32Array after use. */
/******************************************************************************************************/
ViStatus RsCore_QueryBinaryOrAsciiIntegerArray(ViSession instrSession, ViConstString query, ViInt32** outInt32Array, ViInt32* outArraySize)
{
	ViStatus error = VI_SUCCESS;
	ViByte* data = NULL;
	ViBoolean isBinDataBlock = VI_FALSE;
	ViInt32 byteCount;
	ViInt64 byteCount64;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*outArraySize = 1;
		checkAlloc(*outInt32Array = (ViInt32 *)calloc(1, sizeof(ViInt32)));
		(*outInt32Array)[0] = 0;

		goto Error;
	}

	checkErr(RsCore_Write(instrSession, query));
	checkErr(RsCoreInt_ReadBinaryDataBlock(instrSession, VI_FALSE, &isBinDataBlock, &data, &byteCount64));
	byteCount = (ViInt32)byteCount64;
	if (isBinDataBlock)
	{
		switch (rsSession->binaryIntegerNumbersFormat)
		{
		case RS_VAL_BIN_INTEGER_FORMAT_INT32_4BYTES:
			*outArraySize = byteCount / 4;
			*outInt32Array = (ViInt32 *)data; // Same data, do not free the data buffer
			break;
		case RS_VAL_BIN_INTEGER_FORMAT_INT32_4BYTES_SWAPPED:
			*outArraySize = byteCount / 4;
			RsCore_SwapEndianness(data, *outArraySize, 4);
			*outInt32Array = (ViInt32 *)data; // Same data, do not free the data buffer
			break;
		default:
			viCheckErrElab(VI_ERROR_NSUP_MODE,
				"RsCore_QueryBinaryOrAsciiIntegerArray: Unsupported mode of conversion BinDataBlock -> Integer32Array[]");
			break;
		}
	}
	else
	{
		RsCore_TrimString((ViChar*)data, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		checkErr(RsCore_Convert_String_To_ViInt32Array(instrSession, (ViChar*)data, outInt32Array, outArraySize));
	}

Error:
	// Same data buffer, do not free it, it goes to the user
	if (data && (ViInt32 *)data != *outInt32Array)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryBinaryOrAsciiIntegerArrayWithOpc
    Queries with OPC an array of integer numbers that can be returned in ASCII format or in binary format.
    Set the parameter timeoutMs to 0 in order to use the session's OPC timeout.
    Because of the order of commands, this function also calls the driver's CheckStatus callback.
    If you use this function, do not call the CheckStatus anymore
    The array is always returned as the most-universal ViInt32 array.
    - For ASCII format, the array numbers are decoded as comma-separated values.
    - For Binary Format, the numbers are decoded based on the property binaryIntegerNumbersFormat, usually integer 32-bit (FORM INT,32)
    WARNING!!! - Deallocate the outInt32Array after use. */
/******************************************************************************************************/
ViStatus RsCore_QueryBinaryOrAsciiIntegerArrayWithOpc(ViSession instrSession, ViConstString query, ViInt32 timeoutMs,
                                                      ViInt32** outInt32Array, ViInt32* outArraySize)
{
	ViStatus error = VI_SUCCESS;
	ViByte* data = NULL;
	ViBoolean isBinDataBlock = VI_FALSE;
	ViInt32 byteCount;
	ViInt64 byteCount64;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession))
	{
		*outArraySize = 1;
		checkAlloc(*outInt32Array = (ViInt32 *)calloc(1, sizeof(ViInt32)));
		(*outInt32Array)[0] = 0;

		goto Error;
	}

	checkErr(RsCoreInt_QueryBinaryDataBlockWithOpc(instrSession, query, VI_FALSE, &isBinDataBlock, VI_TRUE, timeoutMs, &data, &byteCount64));
	byteCount = (ViInt32)byteCount64;
	if (isBinDataBlock)
	{
		switch (rsSession->binaryIntegerNumbersFormat)
		{
		case RS_VAL_BIN_INTEGER_FORMAT_INT32_4BYTES:
			*outArraySize = byteCount / 4;
			*outInt32Array = (ViInt32 *)data; // Same data, do not free the data buffer
			break;
		case RS_VAL_BIN_INTEGER_FORMAT_INT32_4BYTES_SWAPPED:
			*outArraySize = byteCount / 4;
			RsCore_SwapEndianness(data, *outArraySize, 4);
			*outInt32Array = (ViInt32 *)data; // Same data, do not free the data buffer
			break;
		default:
			viCheckErrElab(VI_ERROR_NSUP_MODE,
				"RsCore_QueryBinaryOrAsciiIntegerArray: Unsupported mode of conversion BinDataBlock -> Integer32Array[]");
			break;
		}
	}
	else
	{
		RsCore_TrimString((ViChar*)data, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		checkErr(RsCore_Convert_String_To_ViInt32Array(instrSession, (ViChar*)data, outInt32Array, outArraySize));
	}

Error:
	// Same data buffer, do not free it, it goes to the user
	if (data && (ViInt32 *)data != *outInt32Array)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryFloatArrayToUserBuffer
	The function sends the query to the instrument, reads the array response and copies it to the provided user buffer
	Before sending the command, it sends the bin format setting: ':FORM REAL,32'
	Even if the instrument returns the data in ASCII format, the function parses it properly
	(it uses RsCore_QueryBinaryOrAsciiFloatArray)
	If the userBufferLength is smaller than read-out dataCount, the function only copies
	the maximum provided count of the data and returns positive error number that equals the dataCount.
	actualPointsCount is allowed to be NULL
/******************************************************************************************************/
ViStatus RsCore_QueryFloatArrayToUserBuffer(ViSession instrSession,
										ViConstString query,
										ViInt32 userBufferLength,
										ViReal64 *userBuffer,
										ViInt32* actualPointsCount)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataCount = 0;

	checkErr(RsCore_Write(instrSession, ":FORM REAL,32"));
	checkErr(RsCore_QueryBinaryOrAsciiFloatArray(instrSession, query, &data, &dataCount));
	
	if (dataCount <= userBufferLength)
	{
		// Copy the whole data
		memcpy(userBuffer, data, dataCount * sizeof(ViReal64));
	}
	else
	{
		// Copy the userBufferLength data
		memcpy(userBuffer, data, userBufferLength * sizeof(ViReal64));
		error = dataCount;
	}

	if (actualPointsCount)
		*actualPointsCount = dataCount;

Error:
	if (data)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryFloatArrayToUserBufferWithOpc
	Same as RsCore_QueryBinaryOrAsciiFloatArrayWithOpc, but the query is sent with OPC-sync
	Set the parameter timeoutMs to 0 in order to use the session's OPC timeout
/******************************************************************************************************/
ViStatus RsCore_QueryFloatArrayToUserBufferWithOpc(ViSession instrSession,
										ViConstString query,
										ViInt32 timeoutMs,
										ViInt32 userBufferLength,
										ViReal64 *userBuffer,
										ViInt32* actualPointsCount)
{
	ViStatus error = VI_SUCCESS;
	ViReal64* data = NULL;
	ViInt32 dataCount = 0;

	checkErr(RsCore_Write(instrSession, ":FORM REAL,32"));
	checkErr(RsCore_QueryBinaryOrAsciiFloatArrayWithOpc(instrSession, query, timeoutMs, &data, &dataCount));

	if (dataCount <= userBufferLength)
	{
		// Copy the whole data
		memcpy(userBuffer, data, dataCount * sizeof(ViReal64));
	}
	else
	{
		// Copy the userBufferLength data
		memcpy(userBuffer, data, userBufferLength * sizeof(ViReal64));
		error = dataCount;
	}

	if (actualPointsCount)
		*actualPointsCount = dataCount;

Error:
	if (data)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryIntegerArrayToUserBuffer
	The function uses RsCore_QueryBinaryOrAsciiIntegerArray to send the query to the instrument,
	read the array response and copies it to the provided user buffer.
	The function parses correctly ASCII or BINARY response (it uses RsCore_QueryBinaryOrAsciiIntegerArray)
	If the userBufferLength is smaller than read-out dataCount, the function only copies
	the maximum provided count of the data and returns positive error number that equals the dataCount.
	actualPointsCount is allowed to be NULL
/******************************************************************************************************/
ViStatus RsCore_QueryIntegerArrayToUserBuffer(ViSession instrSession,
	ViConstString query,
	ViInt32 userBufferLength,
	ViInt32 *userBuffer,
	ViInt32* actualPointsCount)
{
	ViStatus error = VI_SUCCESS;
	ViInt32* data = NULL;
	ViInt32 dataCount = 0;

	checkErr(RsCore_QueryBinaryOrAsciiIntegerArray(instrSession, query, &data, &dataCount));

	if (dataCount <= userBufferLength)
	{
		// Copy the whole data
		memcpy(userBuffer, data, dataCount * sizeof(ViInt32));
	}
	else
	{
		// Copy the userBufferLength data
		memcpy(userBuffer, data, userBufferLength * sizeof(ViInt32));
		error = dataCount;
	}

	if (actualPointsCount)
		*actualPointsCount = dataCount;

Error:
	if (data)
		free(data);

	return error;
}

/******************************************************************************************************/
/*  RsCore_QueryIntegerArrayToUserBufferWithOpc
	Same as RsCore_QueryIntegerArrayToUserBuffer, but the query is sent with OPC-sync
	Set the parameter timeoutMs to 0 in order to use the session's OPC timeout
/******************************************************************************************************/
ViStatus RsCore_QueryIntegerArrayToUserBufferWithOpc(ViSession instrSession,
	ViConstString query,
	ViInt32 timeoutMs,
	ViInt32 userBufferLength,
	ViReal64 *userBuffer,
	ViInt32* actualPointsCount)
{
	ViStatus error = VI_SUCCESS;
	ViInt32* data = NULL;
	ViInt32 dataCount = 0;

	checkErr(RsCore_QueryBinaryOrAsciiIntegerArrayWithOpc(instrSession, query, timeoutMs, &data, &dataCount));

	if (dataCount <= userBufferLength)
	{
		// Copy the whole data
		memcpy(userBuffer, data, dataCount * sizeof(ViInt32));
	}
	else
	{
		// Copy the userBufferLength data
		memcpy(userBuffer, data, userBufferLength * sizeof(ViInt32));
		error = dataCount;
	}

	if (actualPointsCount)
		*actualPointsCount = dataCount;

Error:
	if (data)
		free(data);

	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Instrument Helper IO Functions ------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_ResetRegistersEseSre
    Correctly sets the ESE and SRE registers for the current session OPC wait mode
    Also coerces the session's OPC mode if necessary
******************************************************************************************************/
ViStatus RsCore_ResetRegistersEseSre(ViSession instrSession)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (rsSession->vxiCapable == VI_FALSE && rsSession->opcWaitMode == RS_VAL_OPCWAIT_SERVICEREQUEST)
	{
		// fall back to the StbPolling in case of non-VXI session
		rsSession->opcWaitMode = RS_VAL_OPCWAIT_STBPOLLINGSLOW;
	}

	// Set the SRE and ESE registers accordingly
	if (rsSession->opcWaitMode == RS_VAL_OPCWAIT_SERVICEREQUEST)
	{
		checkErr(RsCore_Write(instrSession, "*ESE 1"));
		checkErr(RsCore_Write(instrSession, "*SRE 32"));
	}
	else
	{
		checkErr(RsCore_Write(instrSession, "*ESE 1"));
		checkErr(RsCore_Write(instrSession, "*SRE 0"));
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ErrorQuery
    Queries ONE error from the instrument error queue
    You have to provide the errMessage with at least 256 bytes buffer
    The variable 'errorFlag' is set to TRUE, if an error was read out.
    In case of 'No error', errorFlag is FALSE
******************************************************************************************************/
ViStatus RsCore_ErrorQuery(ViSession instrSession, ViInt32* errCode, ViChar* errMessage, ViBoolean* errorFlag)
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 code;
	ViChar* p2message = NULL;

	*errorFlag = VI_TRUE;
	if (RsCore_Simulating(instrSession))
	{
		RsCoreInt_StrcpyMaxLen(buffer, RS_MAX_MESSAGE_BUF_SIZE, "0,\"No error.\"");
	}
	else
	{
		checkErr(RsCore_QueryViString(instrSession, "SYST:ERR?", buffer));
	}

	//Find and split code from the message
	p2message = strstr(buffer, ",\"");
	if (p2message == NULL)
	{
		code = -1;
		p2message = buffer;
	}
	else
	{
		code = atoi(buffer);
		p2message[0] = 0;
		p2message ++;
	}

	RsCore_TrimString(p2message, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
	RsCoreInt_StrcpyMaxLen(errMessage, 255, p2message);

	RsCoreInt_ToLowerCase(p2message);
	if (code == 0 || (strstr(p2message, "no error") == p2message))
	{
		*errorFlag = VI_FALSE;
	}

	if (errCode)
		*errCode = code;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_ErrorQueryAll
    Queries ALL errors from the instrument error queue. Returned errCode is from the last error.
    Returned errors variable contains LF-separated list of error starting with the last one
******************************************************************************************************/
ViStatus RsCore_ErrorQueryAll(ViSession instrSession, ViInt32* errCode, ViInt32 bufferSize, ViChar* errors)
{
	ViStatus error = VI_SUCCESS;
	ViBoolean errorFlag;
	ViInt32 code;
	ViChar oneEntry[256] = "";
	ViInt32 i = 0;

	if (errCode)
		*errCode = 0;

	if (bufferSize > 0)
		RsCoreInt_StrcpyMaxLen(errors, bufferSize, "");

	for (i = 0; i < 50; i++) // 50 is a safety limit
	{
		checkErr(RsCore_ErrorQuery(instrSession, &code, oneEntry, &errorFlag));
		if (errorFlag == VI_FALSE)
		{
			break;
		}

		// Error code from the last error
		if (errCode && *errCode == 0)
			*errCode = code;

		RsCore_AppendToCsvString(errors, "\n", bufferSize, oneEntry);
	}

	RsCore_TrimString(errors, RS_VAL_TRIM_WHITESPACES);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_SelfTest
    Performs standard self-test routine using OPC-synchronized query '*TST?'
    - selfTestMessage is set to 'Self-Test Passed' if the selfTestResult was 0
    - selfTestMessage is set to 'Self-Test Failed' if the selfTestResult was not equal 0
******************************************************************************************************/
ViStatus RsCore_SelfTest(ViSession instrSession, ViInt16* selfTestResult, ViInt32 bufferSize, ViChar* selfTestMessage)
{
	ViStatus error = VI_SUCCESS;
	ViChar buffer[RS_MAX_SHORT_MESSAGE_BUF_SIZE];
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession) == VI_TRUE)
	{
		*selfTestResult = 0;
	}
	else
	{
		checkErr(RsCore_QueryViStringWithOpc(instrSession, "*TST?", rsSession->selfTestTimeout, RS_MAX_SHORT_MESSAGE_BUF_SIZE, buffer));
		sscanf(buffer, "%hd", selfTestResult);
	}

	if (*selfTestResult == 0)
		RsCoreInt_StrcpyMaxLen(selfTestMessage, bufferSize, "Self-Test Passed.");
	else
		RsCoreInt_StrcpyMaxLen(selfTestMessage, bufferSize, "Self-Test Failed.");

Error:
	(void)RsCore_UnlockSession(instrSession);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_CheckStatus
    Default function to use in the instrument driver to call the session's default CheckStatusCallback
    If the attribute RS_ATTR_CHECK_STATUS_CALLBACK value is NULL, the core's default RsCore_CheckStatusCallback is used
******************************************************************************************************/
ViStatus RsCore_CheckStatus(ViSession instrSession, ViStatus errorBefore)
{
	ViStatus error = VI_SUCCESS;
	RsCoreCheckStatusCallback p2checkStatusFunction;
	void* value;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	checkErr(RsCoreInt_GetAttrP2Value(rsSession->attrCheckStatusCallbackPtr, &value, 0));
	p2checkStatusFunction = (RsCoreCheckStatusCallback)((ViAddr *)value);
	if (p2checkStatusFunction == NULL)
	{
		checkErr(RsCore_CheckStatusCallback(instrSession, errorBefore));
	}
	else
	{
		checkErr(p2checkStatusFunction(instrSession, errorBefore));
	}

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Default Callback Routines -----------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_ReadCallback
    Queries command from the instrument.
    This is the default ReadCallback for ViInt32, ViReal64, ViBoolean, ViString and ViSession attribute.
    Attributes ViAddr and ViSession have 'attr->command' set to NULL, so the RsCore_ReadCallback is skipped.
    Called by the RsCoreInt_GetAttr if the attribute has no special ReadCallback defined.
    Compared to RsCore_WriteCallback, this function updates 'attr->p2value' to avoid the unnecessary data copying.
    The caller RsCoreInt_GetAttr then takes the 'attr->p2value' and copies it to the user-provided buffer.
******************************************************************************************************/
ViStatus RsCore_ReadCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViChar responseFixed[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* responseUnknownLen = NULL; // For string attribute, use dynamic buffer
	ViInt32 responseSize = 0;
	ViReal64 rangeTableNumValue;
	ViChar* p2response = NULL;

	/* If no command available, exit the function */
	if (isNullOrEmpty(attr->command))
		goto Error;

	// Command string building from repeated capabilities
	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	if (attr->rangeTableCallback)
	{
		// Range Table Callback - attribute's range table is set dynamically by this callback
		checkErr(((attr->rangeTableCallback)(instrSession, repCapName, attr, &(attr->rangeTable))));
	}

	if (attr->dataType == RS_VAL_STRING)
	{
		// Query the instrument, get the unlimited size response
		checkErr(RsCore_QueryAttributeCommandUnknownLength(instrSession, attr, cmd, &responseUnknownLen, &responseSize));
		// For string-type attributes remove trailing Whitespaces and pair quotations
		RsCore_TrimString(responseUnknownLen, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		p2response = responseUnknownLen;
	}
	else
	{
		// Query the instrument, get the limited size response - RS_MAX_MESSAGE_BUF_SIZE
		checkErr(RsCore_QueryAttributeCommand(instrSession, attr, cmd, responseFixed));
		p2response = responseFixed;
	}

	// In simulation, end here. Do not touch the cache value.
	// It is read by the GetAttribute and returned to the caller.
	if (RsCore_Simulating(instrSession))
		goto Error;

	if (attr->rangeTable && attr->rangeTable->type == RS_VAL_DISCRETE)
	{
		// Value has to converted to the range table value
		checkErr(RsCore_FindDiscreteRangeTableNumericValue(instrSession, attr->rangeTable, p2response, &rangeTableNumValue));
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			*(ViInt32 *)attr->p2value = (ViInt32)rangeTableNumValue;
			break;
		case RS_VAL_REAL64:
			*(ViReal64 *)attr->p2value = rangeTableNumValue;
			break;
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_ReadCallback: Discrete Range table data type must be either RS_VAL_INT32 or RS_VAL_REAL64"));

		}
	}
	else
	{
		// No range table, covert the response directly to the appropriate type
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			*((ViInt32*)attr->p2value) = (ViInt32)RsCore_Convert_String_To_ViInt32(p2response);
			break;
		case RS_VAL_INT64:
			*((ViInt64*)attr->p2value) = (ViInt64)RsCore_Convert_String_To_ViReal64(p2response);
			break;
		case RS_VAL_REAL64:
			*((ViReal64*)attr->p2value) = RsCore_Convert_String_To_ViReal64(p2response);
			break;
		case RS_VAL_STRING:
			checkErr(RsCore_SetAttributeP2Value(instrSession, attr, p2response));
			break;
		case RS_VAL_BOOLEAN:
			*((ViBoolean*)attr->p2value) = RsCore_Convert_String_To_Boolean(p2response);
			break;
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
				"RsCore_ReadCallback: Allowed data types: RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64, RS_VAL_STRING, RS_VAL_BOOLEAN"));
		}
	}

Error:
	if (cmd)
		free(cmd);

	if (responseUnknownLen)
		free(responseUnknownLen);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_WriteCallback
    This is the default callback function for the ViInt32, ViReal64, ViBoolean and ViString attribute.
    Writes a command to instrument.
    Called by RsCoreInt_SetAttr if the attribute has no special WriteCallback defined.
    Updating of the current attribute value 'attr->p2value' is done in the caller
    RsCoreInt_SetAttr with the function RsCore_SetAttributeP2Value
******************************************************************************************************/
ViStatus RsCore_WriteCallback(ViSession instrSession,
                              ViConstString repCapName,
                              RsCoreAttributePtr attr,
                              void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;

	// If no command is available, exit the function
	if (isNullOrEmpty(attr->command))
		goto Error;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	if (attr->rangeTableCallback)
	{
		checkErr((attr->rangeTableCallback)(instrSession, repCapName, attr, &(attr->rangeTable)));
	}

	if ((attr->rangeTable) && (attr->rangeTable->type == RS_VAL_DISCRETE))
	{
		ViChar* p2cmdString = NULL;
		ViReal64 precision = attr->comparePrecision;

		// Allowed data types are only ViInt32 or ViReal64
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			checkErr(RsCore_FindRangeTableCmdString(instrSession, attr->rangeTable, *(ViInt32 *)value, &p2cmdString));
			break;
		case RS_VAL_REAL64:
			precision = (precision < DBL_MIN) ? DBL_MIN : precision;
			checkErr(RsCore_FindRangeTableCmdStringViReal64(instrSession, attr->rangeTable, *(ViReal64 *)value, precision, &p2cmdString));
			break;
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
				"RsCore_WriteCallback: Discrete Range Table data types can only be RS_VAL_INT32, RS_VAL_REAL64"));
		}

		checkErr(RsCore_StrCatDynamicString(instrSession, &cmd, " ", p2cmdString, NULL));
	}
	else
	{
		// No range table, cmdString is composed based on the data type
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			checkErr(RsCore_StrCatDynamicViInt32(instrSession, &cmd, " ", *(ViInt32 *)value, NULL));
			break;
		case RS_VAL_INT64:
			checkErr(RsCore_StrCatDynamicViInt64(instrSession, &cmd, " ", *(ViInt64 *)value, NULL));
			break;
		case RS_VAL_REAL64:
			checkErr(RsCore_StrCatDynamicViReal64(instrSession, &cmd, " ", *(ViReal64 *)value, NULL));
			break;
		case RS_VAL_STRING:
			checkErr(RsCore_StrCatDynamicString(instrSession, &cmd, " \'", (ViString)value, "\'"));
			break;
		case RS_VAL_BOOLEAN:
			checkErr(RsCore_StrCatDynamicViBoolean(instrSession, &cmd, " ", *(ViBoolean *)value, NULL));
			break;
		case RS_VAL_EVENT:
			break;
		case RS_VAL_ADDR:
		case RS_VAL_SESSION:
		case RS_VAL_UNKNOWN_TYPE:
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, NULL));
		}
	}

	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_CheckCallback
    Performs the default actions for checking the validity of a ViInt32 or ViReal64 attribute value.
    This callback is called by RsCoreInt_SetAttribute() the attribute has a range table defined,
    to check whether the value fulfills the required range.
******************************************************************************************************/
ViStatus RsCore_CheckCallback(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	void* coercedValue = NULL;
	ViInt32 intValue;
	ViInt64 longIntValue;
	ViReal64 doubleValue;

	/* Default range table callback? */
	if (attr->rangeTableCallback)
	{
		checkErr((attr->rangeTableCallback)(instrSession, repCapName, attr, &(attr->rangeTable)));
	}

	/* Proceed only when the range table exists */
	if (attr->rangeTable)
	{
		checkErr(RsCore_RangeTableCallback(instrSession, repCapName, attr, attr->rangeTable));

		// Resolve range tables according the its type
		switch (attr->rangeTable->type)
		{
		case RS_VAL_RANGED:
			{
				// Only first RsCoreRangeTableEntry from the range table plays the role
				ViReal64* p2min = &(attr->rangeTable->rangeValues[0].discreteOrMinValue);
				ViReal64* p2max = &(attr->rangeTable->rangeValues[0].maxValue);

				if (!p2min || !p2max)
					return RS_ERROR_INVALID_RANGE_TABLE;

				switch (attr->dataType)
				{
				case RS_VAL_INT32:
					intValue = *(ViInt32*)value;
					if (intValue < (ViInt32)(*p2min) || intValue > (ViInt32)(*p2max))
					{
						snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
						         "Entered value " _PERCD " is out of allowed range " _PERCD ".." _PERCD, intValue, (ViInt32)(*p2min), (ViInt32)(*p2max));
						viCheckErrElab(RS_ERROR_INVALID_VALUE, errElab);
					}
					else
					{
						goto Error;
					}

				case RS_VAL_INT64:
					longIntValue = *(ViInt64*)value;
					if (longIntValue < (ViInt64)(*p2min) || longIntValue > (ViInt64)(*p2max))
					{
						snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
						         "Entered value " _PERCLD " is out of allowed range " _PERCLD ".." _PERCLD, longIntValue, (ViInt64)(*p2min), (ViInt64)(*p2max));
						viCheckErrElab(RS_ERROR_INVALID_VALUE, errElab);
					}
					else
					{
						goto Error;
					}

				case RS_VAL_REAL64:
					doubleValue = *(ViReal64*)value;
					if (doubleValue < *p2min || doubleValue > *p2max)
					{
						snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
						         "Entered value %.12lG is out of allowed range %.12lG..%.12lG", doubleValue, *p2min, *p2max);
						viCheckErrElab(RS_ERROR_INVALID_VALUE, errElab);
					}
					else
					{
						goto Error;
					}
				default:
					/* No action */
					break;
				}
			}
			break;

		case RS_VAL_DISCRETE:
			{
				ViChar* temp;

				if (attr->dataType == RS_VAL_INT32)
				{
					RsCore_FindRangeTableCmdString(instrSession, attr->rangeTable, *(ViInt32 *)value, &temp);
				}
				else if (attr->dataType == RS_VAL_REAL64)
				{
					RsCore_FindRangeTableCmdStringViReal64(instrSession, attr->rangeTable, attr->comparePrecision, *(ViReal64 *)value, &temp);
				}
			}
			break;

		case RS_VAL_COERCED:
			if (attrHasNotFlag(RS_VAL_COERCEABLE_ONLY_BY_INSTR))
			{
				checkAlloc(coercedValue = malloc(sizeof(ViReal64)));
				checkErr(RsCore_CoerceCallback(instrSession, repCapName, attr, value));
			}
			break;

		default:
			viCheckErr(RS_ERROR_INVALID_RANGE_TABLE);
			break;
		}
	}

Error:
	if (coercedValue)
		free(coercedValue);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_CoerceCallback
    Performs default actions for coercing a value for the ViReal64 or ViInt32 attribute.
    The function is called by RsCore_CheckCallback if the attribute has a coerce-type range table defined
******************************************************************************************************/
ViStatus RsCore_CoerceCallback(ViSession instrSession,
                               ViConstString repCapName,
                               RsCoreAttributePtr attr,
                               void* value)
{
	ViStatus error = VI_SUCCESS;

	/* Proceed only when the range table exists */
	if (attr->rangeTable)
	{
		/* Default range table callback? */
		if (attr->rangeTableCallback)
		{
			checkErr((attr->rangeTableCallback)(instrSession, repCapName, attr, &(attr->rangeTable)));
		}
		else
		{
			checkErr(RsCore_RangeTableCallback(instrSession, repCapName, attr, attr->rangeTable));
		}

		/* Default coerce callback? */
		if (attr->coerceCallback)
		{
			checkErr((attr->coerceCallback)(instrSession, repCapName, attr, value));
		}
		else
		{
			// Coerce the value if the table type is RS_VAL_COERCED
			if (attr->rangeTable->type == RS_VAL_COERCED)
			{
				// Coerce entered value to the nearest value from the range table. Data must be in ascending order! */
				ViInt32 i, minDifIdx;
				ViReal64 viReal64value = 0.0;
				ViReal64 minDiff = 0.0, aux;
				ViReal64 minValue, maxValue;

				switch (attr->dataType)
				{
				case RS_VAL_INT32:
					viReal64value = (ViReal64)(*(ViInt32 *)value);
					break;
				case RS_VAL_INT64:
					viReal64value = (ViReal64)(*(ViInt64 *)value);
					break;
				case RS_VAL_REAL64:
					viReal64value = *(ViReal64 *)value;
					break;
				default:
					checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
						"RsCore_CoerceCallback: Coerced Range Table data types can only be RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64"));
				}

				// Coerce the value
				i = 0;
				minDifIdx = -1;
				minDiff = INFINITY;
				minValue = INFINITY;
				maxValue = -INFINITY;
				while (attr->rangeTable->rangeValues[i].cmdValue != RS_RANGE_TABLE_END_VALUE)
				{
					if (minValue > attr->rangeTable->rangeValues[i].coercedValue) minValue = attr->rangeTable->rangeValues[i].coercedValue;
                    if (maxValue < attr->rangeTable->rangeValues[i].coercedValue) maxValue = attr->rangeTable->rangeValues[i].coercedValue;
					aux = fabs(viReal64value - attr->rangeTable->rangeValues[i].coercedValue);

					if (aux < minDiff)
					{
						minDifIdx = i;
						minDiff = aux;
					}
					i++;
				}

				if (viReal64value < minValue) // Lower than Lowest correction
					aux = minValue;
				else if (viReal64value > maxValue) // Higher than Highest correction
					aux = maxValue;
				else
					aux = attr->rangeTable->rangeValues[minDifIdx].coercedValue;

				switch (attr->dataType)
				{
				case RS_VAL_INT32:
					*(ViInt32 *)value = (ViInt32)aux;
					break;
				case RS_VAL_INT64:
					*(ViInt64 *)value = (ViInt64)aux;
					break;
				case RS_VAL_REAL64:
					*(ViReal64 *)value = aux;
					break;
				default:
					checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
						"RsCore_CoerceCallback: Coerced Range Table data types can only be RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64"));
				}
			} /* EOF Coerce the value if the table type is COERCED */
		} /* EOF NOT Default coerce callback? */
	} /* EOF If range table exists */

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_RangeTableCallback
    The Range Table callback is called by the RsCore_CheckCallback and RsCore_CoerceCallback
    if the attribute has a range table defined.
    This function as a default range table callback only returns pointer to the attribute's attr->rangeTable
    If the attribute has a custom Range Table Callback defined, the functionality can be different.
    This mechanism allows for having dynamic range tables for one attribute based on different conditions.
******************************************************************************************************/
ViStatus RsCore_RangeTableCallback(ViSession instrSession,
                                   /*@unused@*/ViConstString repCapName,
                                   RsCoreAttributePtr attr,
                                   RsCoreRangeTablePtr rangeTablePtr)
{
	ViStatus error = VI_SUCCESS;

	/* Update range table ptr only when available */
	if (rangeTablePtr)
	{
		attr->rangeTable = rangeTablePtr;
	}
	else
	{
		checkErr(RS_ERROR_NO_RANGE_TABLE);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_WaitForOpcCallback
    Default callback for Waiting for OPC mechanism
******************************************************************************************************/
ViStatus RsCore_WaitForOpcCallback(ViSession instrSession, ViInt32 opcTimeoutMs)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (RsCore_Simulating(instrSession) == VI_TRUE)
		goto Error;

	switch (rsSession->opcWaitMode)
	{
	case RS_VAL_OPCWAIT_STBPOLLING:
	case RS_VAL_OPCWAIT_STBPOLLINGSLOW:
	case RS_VAL_OPCWAIT_STBPOLLINGSUPERSLOW:
		checkErr(RsCoreInt_WaitForOpcSTBpoll(instrSession, opcTimeoutMs));
		break;
	case RS_VAL_OPCWAIT_SERVICEREQUEST:
		checkErr(RsCoreInt_WaitForOpcSRQ(instrSession, opcTimeoutMs));
		break;
	default:
		error = RS_ERROR_INVALID_VALUE;
		break;
	}

Error:
	error = RsCoreInt_GenerateOpcTimeoutError(instrSession, error, opcTimeoutMs);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_CheckStatusCallback
    Default CheckStatusCallback querying *STB? eventually also SYST:ERR? and *OPC?
******************************************************************************************************/
ViStatus RsCore_CheckStatusCallback(ViSession instrSession, ViStatus errorBefore)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar auxString[RS_MAX_MESSAGE_BUF_SIZE];
	ViInt32 stb = 0;
	ViInt64 responseLen;
	ViStatus primary, secondary;
	ViByte* unexpResponse = NULL;
	RsCoreSessionPtr rsSession = NULL;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));

	if (errorBefore == VI_SUCCESS && rsSession->sendOpcInCheckStatus == VI_TRUE)
	{
		checkErr(RsCore_QueryViInt32(instrSession, "*OPC?", NULL));
	}

	// Special handling for VI_ERROR_TMO and RS_ERROR_INSTRUMENT_STATUS
	if (errorBefore == VI_ERROR_TMO)
	{
		// VISA Timeout occured. Get the error info, and add the potential secondary instrument status error
		responseLen = 0;
		if (rsSession->vxiCapable)
		{
			checkErr(RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE));

			if (stb & IEEE_488_2_STB_MAV_MASK)
			{
				// Message available in the output buffer, set the secondary error to RS_ERROR_UNEXPECTED_RESPONSE
				checkErr(RsCoreInt_ReadDataUnknownLength(instrSession, &unexpResponse, &responseLen));
			}
		}
		else
		{
			// Non-VXI session
			ViUInt32 visaTout;
			(void)RsCore_GetVisaTimeout(instrSession, &visaTout);
			(void)RsCore_SetVisaTimeout(instrSession, 500);
			(void)RsCoreInt_ReadDataUnknownLength(instrSession, &unexpResponse, &responseLen);
			error = RsCoreInt_GetStatusByte(instrSession, &stb, VI_FALSE);
			(void)RsCore_SetVisaTimeout(instrSession, visaTout);

			if (error < VI_SUCCESS)
			{
				goto Error;
			}
		}

		if (responseLen > 0)
		{
			RsCoreInt_FormatBufferForStringMessage(unexpResponse, (ViInt32)responseLen + 1, 60);

			checkErr(RsCore_GetErrorInfo(instrSession, &primary, &secondary, errElab));
			snprintf(auxString, RS_MAX_MESSAGE_BUF_SIZE, "\nAfterwards:\nUnread " _PERCLD " bytes long response in the instrument's output buffer: '%s'",
			         responseLen, (ViChar*)unexpResponse);
			RsCore_StrcatMaxLen(errElab, RS_MAX_MESSAGE_BUF_SIZE, auxString, NULL, NULL);
			checkErr(RsCore_SetErrorInfo(instrSession, VI_TRUE, VI_ERROR_TMO, RS_ERROR_UNEXPECTED_RESPONSE, errElab));
		}

		if (stb & IEEE_488_2_STB_ERR_QUEUE_MASK)
		{
			// Error in the error queue, set the secondary error to RS_ERROR_INSTRUMENT_STATUS
			checkErr(RsCore_GetErrorInfo(instrSession, &primary, &secondary, errElab));
			if (rsSession->autoSystErrQuery)
			{
				checkErr(RsCore_ErrorQueryAll(instrSession, NULL, RS_MAX_MESSAGE_BUF_SIZE, auxString));
				RsCore_StrcatMaxLen(errElab, RS_MAX_MESSAGE_BUF_SIZE, "\nAdditionally, the instrument reports error(s) in the error queue:\n",
				                    auxString, NULL);
				checkErr(RsCore_SetErrorInfo(instrSession, VI_TRUE, VI_ERROR_TMO, RS_ERROR_INSTRUMENT_STATUS, errElab));
			}
			else
			{
				checkErr(RsCore_SetErrorInfo(instrSession, VI_TRUE, VI_ERROR_TMO, RS_ERROR_INSTRUMENT_STATUS,
					"\nAdditionally, the instrument reports error(s) in the error queue. Call the function PREFIX_error_query() to read them out."
					" Alternatively, use the PREFIX_ConfigureAutoSystemErrQuery(io, VI_TRUE) to read them automatically."
					" To read a comprehensive information about the last error that occured, call the PREFIX_GetError()"));
			}
		}
	}
	else
	{
		// All other errors including RS_ERROR_INSTRUMENT_STATUS
		checkErr(RsCore_QueryViInt32(instrSession, "*STB?", &stb));

		if (stb & IEEE_488_2_STB_ERR_QUEUE_MASK)
		{
			// Error Queue not empty, change the error code to RS_ERROR_INSTRUMENT_STATUS
			errorBefore = RS_ERROR_INSTRUMENT_STATUS;
			if (rsSession->autoSystErrQuery)
			{
				checkErr(RsCore_ErrorQueryAll(instrSession, NULL, RS_MAX_MESSAGE_BUF_SIZE, auxString));
				snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Instrument reports error(s) in the error queue:\n%s", auxString);
				checkErr(RsCore_SetErrorInfo(instrSession, VI_TRUE, errorBefore, 0, errElab));
			}
			else
			{
				checkErr(RsCore_SetErrorInfo(instrSession, VI_TRUE, errorBefore, 0,
					"Instrument reports error(s) in the error queue. To query these errors, call the function PREFIX_error_query()."
					" Alternatively, use the PREFIX_ConfigureAutoSystemErrQuery(io, VI_TRUE) to read them automatically."
					" To read a comprehensive information about the last error that occured, call the PREFIX_GetError()"));
			}
		}
	}

Error:
	if (unexpResponse)
		free(unexpResponse);

	if (errorBefore >= VI_SUCCESS)
		errorBefore = error;

	return errorBefore;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Range Checking ----------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_InvalidViInt32Range
	Checks for the ViInt32 val to be within min..max range
	If not, the function generates RS_ERROR_INVALID_PARAMETER
	The function only generates the RS_ERROR_INVALID_PARAMETER if the session's range checking is ON
******************************************************************************************************/
ViStatus RsCore_InvalidViInt32Range(ViSession instrSession, ViInt32 val, ViInt32 min, ViInt32 max)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_LEN];

	if (RsCore_RangeChecking(instrSession) == VI_FALSE)
		goto Error;

	if (val < min || val > max)
	{
		snprintf(errElab, RS_MAX_MESSAGE_LEN, "InvalidViInt32Range: value " _PERCD " is out of allowed range " _PERCD ".." _PERCD, val, min, max);
		viCheckErrElab(RS_ERROR_INVALID_PARAMETER, errElab);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_InvalidViInt32Value
	Reports entered value as invalid - generates RS_ERROR_INVALID_PARAMETER
	The function ignores range checking
******************************************************************************************************/
ViStatus RsCore_InvalidViInt32Value(ViSession instrSession, ViInt32 value)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_LEN];

	snprintf(errElab, RS_MAX_MESSAGE_LEN, "InvalidViInt32Value: value " _PERCD " is invalid", value);
	viCheckErrElab(RS_ERROR_INVALID_PARAMETER, errElab);

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_InvalidViUInt32Range
	Checks for the ViUInt32 val to be within min..max range
	If not, the function generates RS_ERROR_INVALID_PARAMETER
	The function only generates the RS_ERROR_INVALID_PARAMETER if the session's range checking is ON
******************************************************************************************************/
ViStatus RsCore_InvalidViUInt32Range(ViSession instrSession, ViUInt32 val, ViUInt32 min, ViUInt32 max)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_LEN];

	if (RsCore_RangeChecking(instrSession) == VI_FALSE)
		goto Error;

	if (val < min || val > max)
	{
		snprintf(errElab, RS_MAX_MESSAGE_LEN, "InvalidViUInt32Range: value " _PERCU " is out of allowed range " _PERCU ".." _PERCU, val, min, max);
		viCheckErrElab(RS_ERROR_INVALID_PARAMETER, errElab);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_InvalidViReal64Range
	Checks for the ViReal64 val to be within min..max range
	If not, the function generates RS_ERROR_INVALID_PARAMETER
	The function only generates the RS_ERROR_INVALID_PARAMETER if the session's range checking is ON
******************************************************************************************************/
ViStatus RsCore_InvalidViReal64Range(ViSession instrSession, ViReal64 val, ViReal64 min, ViReal64 max)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_LEN];

	if (RsCore_RangeChecking(instrSession) == VI_FALSE)
		goto Error;

	if (val < min || val > max)
	{
		snprintf(errElab, RS_MAX_MESSAGE_LEN, "InvalidViReal64Range: value %.12lG is out of allowed range %.12lG..%.12lG", val, min, max);
		viCheckErrElab(RS_ERROR_INVALID_PARAMETER, errElab);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_InvalidViBooleanRange
	Checks for the ViReal64 val to be either VI_TRUE or VI_FALSE
	If not, the function generates RS_ERROR_INVALID_PARAMETER
	The function only generates the RS_ERROR_INVALID_PARAMETER if the session's range checking is ON
******************************************************************************************************/
ViStatus RsCore_InvalidViBooleanRange(ViSession instrSession, ViBoolean val)
{
	ViStatus error = VI_SUCCESS;
	ViChar errElab[RS_MAX_MESSAGE_LEN];

	if (RsCore_RangeChecking(instrSession) == VI_FALSE)
		goto Error;

	if (val != VI_TRUE && val != VI_FALSE)
	{
		snprintf(errElab, RS_MAX_MESSAGE_LEN, "InvalidViBooleanRange: value " _PERCD " is out of allowed range VI_TRUE(1) / VI_FALSE(0)", (ViInt32)val);
		viCheckErrElab(RS_ERROR_INVALID_PARAMETER, errElab);
	}

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_InvalidNullPointer
	Checks whether the entered parameter is equal NULL
	If yes, the function generates RS_ERROR_INVALID_PARAMETER
	Session's Range checking is not taken into consideration
******************************************************************************************************/
ViStatus RsCore_InvalidNullPointer(ViSession instrSession, void* parameter)
{
	ViStatus error = VI_SUCCESS;

	if (parameter == NULL)
	{
		viCheckErrElab(RS_ERROR_INVALID_PARAMETER, "InvalidNullPointer: parameter pointer is NULL");
	}

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Conversions -------------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_Convert_String_To_Boolean
    Converts string to ViBoolean
******************************************************************************************************/
ViBoolean RsCore_Convert_String_To_Boolean(ViConstString string)
{
	if (strncmp(string, "1", 1) == 0) return VI_TRUE;
	else if (strncmp(string, "0", 1) == 0) return VI_FALSE;

	else if (strncmp(string, "ON", 2) == 0) return VI_TRUE;
	else if (strncmp(string, "OFF", 3) == 0) return VI_FALSE;

	else if (strncmp(string, "On", 2) == 0) return VI_TRUE;
	else if (strncmp(string, "Off", 3) == 0) return VI_FALSE;

	else if (strncmp(string, "TRUE", 4) == 0) return VI_TRUE;
	else if (strncmp(string, "FALSE", 5) == 0) return VI_FALSE;

	else if (strncmp(string, "True", 4) == 0) return VI_TRUE;
	else if (strncmp(string, "False", 5) == 0) return VI_FALSE;

	else if (strncmp(string, "true", 4) == 0) return VI_TRUE;
	else if (strncmp(string, "false", 5) == 0) return VI_FALSE;

	else if (strncmp(string, "VI_TRUE", 7) == 0) return VI_TRUE;
	else if (strncmp(string, "VI_FALSE", 8) == 0) return VI_FALSE;

	ViInt32 value = 0;
	sscanf(string, _PERCD, &value);

	return value > 0;
}

/*****************************************************************************************************/
/*	RsCore_Convert_String_To_ViInt32
	Converts a string to integer number.
	Also recognizes NotANumber values - in this case returns RS_VAL_NAN_VI_INT32
******************************************************************************************************/
ViInt32 RsCore_Convert_String_To_ViInt32(ViConstString value)
{
	static ViString states[] = { "NAN","INV", "NAV", "NCAP", "DTX", "nan", NULL };

	if (RsCore_FindStringIndex(states, value) == -1)
		return (ViInt32)atof(value);
	else
		return RS_VAL_NAN_VI_INT32;
}

/*****************************************************************************************************/
/*	RsCore_Convert_String_To_ViReal64
	Converts a string to float number.
	Also recognizes NotANumber values - in this case returns RS_VAL_NAN
******************************************************************************************************/
ViReal64 RsCore_Convert_String_To_ViReal64(ViConstString value)
{
	static ViString states[] = { "NAN","INV", "NAV", "NCAP", "DTX", "nan", NULL };

	if (RsCore_FindStringIndex(states, value) == -1)
		return atof(value);
	else
		return RS_VAL_NAN_VI_REAL64;
}

/*****************************************************************************************************/
/*  RsCore_Convert_ViReal64_To_String
    Converts ViReal64 value to string. Recognizes special values like NAN, +INF or -INF
    Returns length of the result string
******************************************************************************************************/
ViInt32 RsCore_Convert_ViReal64_To_String(ViReal64 value, ViInt32 bufferSize, ViChar* buffer)
{
	ViInt32 elementLen;

#if defined (_CVI_)
    elementLen = snprintf(buffer, bufferSize, "%.12lG", value);
#else
	if (isnan(value))
	{
		elementLen = snprintf(buffer, bufferSize, "NAN");
	}
	else
	{
		elementLen = snprintf(buffer, bufferSize, "%.12lG", value);
	}
#endif

	return elementLen;
}

/*****************************************************************************************************/
/*  RsCore_Convert_HexaPattern_To_BinPattern
    Converts Hexa string to binary string e.g. 'F201' -> '1111001000000001'
    Returns VI_TRUE, if the conversion was successfull.
    Returns VI_FALSE, if the hexaPattern contained other character then 0-9 A-F a-f
******************************************************************************************************/
ViBoolean RsCore_Convert_HexaPattern_To_BinPattern(ViConstString hexaPattern, ViInt32 bufferSize, ViChar* binPattern)
{
	ViInt32 i, idx;
	ViChar* ptr;
	ViBoolean success = VI_TRUE;
	const ViChar* hexasLC = "0123456789abcdef";
	const ViChar* hexasUC = "0123456789ABCDEF";
	const ViChar* bins = "0000,0001,0010,0011,0100,0101,0110,0111,1000,1001,1010,1011,1100,1101,1110,1111";

	for (i = 0; i < (ViInt32)strlen(hexaPattern); i++)
	{
		if ((ptr = (ViChar*)strchr(hexasLC, hexaPattern[i])) != NULL)
		{
			idx = (ViInt32)(ptr - hexasLC);
		}
		else if ((ptr = (ViChar*)strchr(hexasUC, hexaPattern[i])) != NULL)
		{
			idx = (ViInt32)(ptr - hexasUC);
		}
		else
		{
			success = VI_FALSE;
			idx = 0;
		}

		RsCore_GetTokenAtIndex(bins, ',', idx, binPattern + i * 4, bufferSize - i * 4, NULL);
	}

	return success;
}

/*****************************************************************************************************/
/*  RsCore_Convert_BinaryString_To_ViInt64
	Converts binary string to decimal integer64 number e.g. '00111' -> 7
	Stops at the first invalid character
	The function modifies the binPattern string
******************************************************************************************************/
ViInt64 RsCore_Convert_BinaryString_To_ViInt64(ViConstString binPattern)
{
	int j = 0;
	ViInt64 bin = 0;
	ViChar single;

	while (binPattern[j])
	{
		single = binPattern[j];
		if (single == 32)
		{
			// skip blanks
			j++;
			continue;
		}

		single -= 48; // from char to int 0 or 1
		if (single >= 2 || single < 0)
			// invalid character, done
			break;

		bin *= 2;
		bin += single;
		j++;
	}
	return bin;
}

/*****************************************************************************************************/
/*  RsCore_Convert_String_To_ViReal64Array
    Converts comma-separated string values to double array and returns an allocated ViReal64 array
    WARNING!!! - free the dblArray afterwards
******************************************************************************************************/
ViStatus RsCore_Convert_String_To_ViReal64Array(ViSession instrSession, ViConstString inputString, ViReal64** dblArray, ViInt32* arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* inputStringCopy = NULL;
	ViChar* ptr = NULL;
	ViInt32 idx;
	ViInt32 elementsCount;
	ViInt32 inputStringLen;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (inputString == NULL || dblArray == NULL)
	{
		RsCoreInt_StrcpyMaxLen(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Rs_ConvertCsvStringToViReal64Array: NULL inputString or dblArray pointer");
		viCheckErrElab(RS_ERROR_NULL_POINTER, errElab);
	}

	inputStringLen = (ViInt32)strlen(inputString);
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "Rs_ConvertCsvStringToViReal64Array: Allocation of the inputString copy, length " _PERCU " bytes", inputStringLen);
	viCheckAllocElab(inputStringCopy = (ViChar*)malloc(inputStringLen + 1), errElab);
	memcpy(inputStringCopy, inputString, inputStringLen + 1);

	RsCore_TrimString(inputStringCopy, RS_VAL_TRIM_WHITESPACES);
	inputStringLen = (ViInt32)strlen(inputStringCopy);

	if (inputStringLen == 0)
	{
		// For empty string allocate one element with 0.0 value and return array size 0
		*arraySize = 0;
		viCheckAllocElab(*dblArray = (ViReal64*)malloc(sizeof(ViReal64)), errElab);
		(*dblArray)[0] = 0.0;
		goto Error;
	}

	elementsCount = RsCore_GetElementsCount(inputStringCopy, ',');

	*arraySize = elementsCount;
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "Rs_ConvertCsvStringToViReal64Array: Allocation of the result ViReal64 array " _PERCD " elements", elementsCount);
	viCheckAllocElab(*dblArray = (ViReal64*)malloc(elementsCount * sizeof(ViReal64)), errElab);

	ptr = strtok(inputStringCopy, ",");
	if (ptr == NULL)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Rs_ConvertCsvStringToViReal64Array: Unexpected input string '%s'", inputStringCopy);
		viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
	}

	idx = 0;
	do
	{
		(*dblArray)[idx++] = RsCore_Convert_String_To_ViReal64(ptr);
		ptr = strtok(NULL, ",");
	} while (ptr);

Error:
	if (inputStringCopy)
		free(inputStringCopy);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_ViReal64array_To_String
    Converts array of ViReal64 numbers to a comma-separated string, allocates the required memory
    and returns it as resultString. The minimum allocated size is RS_MAX_SHORT_MESSAGE_BUF_SIZE (4096 bytes)
    The beforeString is prepended at the start of the resultString.
    You can set the beforeString to empty or NULL.
    WARNING!!! - free the resultString after use
******************************************************************************************************/
ViStatus RsCore_Convert_ViReal64array_To_String(ViSession instrSession, ViReal64* dblArray, ViInt32 arraySize, ViConstString beforeString,
                                                ViChar** resultString)
{
	ViStatus error = VI_SUCCESS;
	ViChar *buffer = NULL, *ptr = NULL;
	ViInt32 allocSize = arraySize * 20;
	ViInt32 beforeStringLen = 0;
	ViInt32 idx;
	ViInt32 elementLen;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (!isNullOrEmpty(beforeString))
	{
		beforeStringLen = (ViInt32)strlen(beforeString);
		allocSize += beforeStringLen;
	}

	if (allocSize < RS_MAX_MESSAGE_BUF_SIZE)
	    allocSize = RS_MAX_MESSAGE_BUF_SIZE;

	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "Rs_Convert_ViReal64array_To_String: Allocation of the resultString buffer for " _PERCU " ViReal64 elements, size " _PERCU " bytes",
	         arraySize, allocSize);
	viCheckAllocElab(buffer = (ViChar*)malloc(allocSize), errElab);

	ptr = buffer;
	if (!isNullOrEmpty(beforeString))
	{
		memcpy(buffer, beforeString, beforeStringLen + 1);
		ptr += beforeStringLen;
		allocSize -= beforeStringLen;
	}

	for (idx = 0; idx < arraySize; idx++)
	{
		elementLen = RsCore_Convert_ViReal64_To_String(dblArray[idx], allocSize, ptr);
		ptr += elementLen;
		ptr[0] = ',';
		ptr++;
		allocSize -= elementLen - 1;
	}

	*--ptr = '\0';

	*resultString = buffer;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_String_To_ViInt32Array
    Converts comma-separated string values to integer array and returns an allocated ViInt32 array
    WARNING!!! - free the intArray afterwards
******************************************************************************************************/
ViStatus RsCore_Convert_String_To_ViInt32Array(ViSession instrSession, ViConstString inputString, ViInt32** intArray, ViInt32* arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* inputStringCopy = NULL;
	ViChar* ptr = NULL;
	ViInt32 idx;
	ViInt32 elementsCount;
	ViInt32 inputStringLen;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (inputString == NULL || intArray == NULL)
	{
		RsCoreInt_StrcpyMaxLen(errElab, RS_MAX_MESSAGE_BUF_SIZE, "Rs_ConvertCsvStringToViInt32Array: NULL inputString or intArray pointer");
		viCheckErrElab(RS_ERROR_NULL_POINTER, errElab);
	}

	inputStringLen = (ViInt32)strlen(inputString);
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "Rs_ConvertCsvStringToViInt32Array: Allocation of the inputString copy, length " _PERCU " bytes", inputStringLen);
	viCheckAllocElab(inputStringCopy = (ViChar*)malloc(inputStringLen + 1), errElab);
	memcpy(inputStringCopy, inputString, inputStringLen + 1);

	RsCore_TrimString(inputStringCopy, RS_VAL_TRIM_WHITESPACES);
	inputStringLen = (ViInt32)strlen(inputStringCopy);

	if (inputStringLen == 0)
	{
		// For empty string allocate one element with 0 value and return array size 0
		*arraySize = 0;
		viCheckAllocElab(*intArray = (ViInt32*)malloc(sizeof(ViInt32)), errElab);
		(*intArray)[0] = 0;
		goto Error;
	}

	elementsCount = RsCore_GetElementsCount(inputStringCopy, ',');

	*arraySize = elementsCount;
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "RsCore_Convert_String_To_ViInt32Array: Allocation of the result RsCore_Convert_String_To_ViInt32Array array " _PERCD " elements",
	         elementsCount);
	viCheckAllocElab(*intArray = (ViInt32*)malloc(elementsCount * sizeof(ViInt32)), errElab);

	ptr = strtok(inputStringCopy, ",");
	if (ptr == NULL)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "RsCore_Convert_String_To_ViInt32Array: Unexpected input string '%s'", inputStringCopy);
		viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
	}

	idx = 0;
	do
	{
		(*intArray)[idx++] = RsCore_Convert_String_To_ViInt32(ptr);
		ptr = strtok(NULL, ",");
	}
	while (ptr);

Error:
	if (inputStringCopy)
		free(inputStringCopy);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_ViInt32array_To_String
    Converts array of ViInt32 numbers to a comma-separated string, allocates the required memory
    and returns it as resultString. The minimum allocated size is RS_MAX_SHORT_MESSAGE_BUF_SIZE (4096 bytes)
    The beforeString is prepended at the start of the resultString.
    You can set the beforeString to empty or NULL.
    WARNING!!! - free the resultString after use
******************************************************************************************************/
ViStatus RsCore_Convert_ViInt32array_To_String(ViSession instrSession, ViInt32* intArray, ViInt32 arraySize, ViConstString beforeString,
                                               ViChar** resultString)
{
	ViStatus error = VI_SUCCESS;
	ViChar *buffer = NULL, *ptr = NULL;
	ViInt32 allocSize = arraySize * 15;
	ViInt32 beforeStringLen = 0;
	ViInt32 idx;
	ViInt32 elementLen;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (!isNullOrEmpty(beforeString))
	{
		beforeStringLen = (ViInt32)strlen(beforeString);
		allocSize += beforeStringLen;
	}

    if (allocSize < RS_MAX_MESSAGE_BUF_SIZE)
        allocSize = RS_MAX_MESSAGE_BUF_SIZE;

	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "RsCore_Convert_ViInt32array_To_String: Allocation of the resultString buffer for " _PERCU " ViInt32 elements, size " _PERCU " bytes",
	         arraySize, allocSize);
	viCheckAllocElab(buffer = (ViChar*)malloc(allocSize), errElab);

	ptr = buffer;
	if (!isNullOrEmpty(beforeString))
	{
		memcpy(buffer, beforeString, beforeStringLen + 1);
		ptr += beforeStringLen;
		allocSize -= beforeStringLen;
	}

	for (idx = 0; idx < arraySize; idx++)
	{
		elementLen = snprintf(ptr, allocSize, _PERCD",", intArray[idx]);
		ptr += elementLen;
		allocSize -= elementLen;
	}

	*--ptr = '\0';

	*resultString = buffer;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_String_To_ViBooleanArray
    Converts comma-separated string values to boolean array using RsCore_Convert_String_To_Boolean() on each element
    THe function allocates and returns the booleanArray buffer
    WARNING!!! - free the booleanArray afterwards
******************************************************************************************************/
ViStatus RsCore_Convert_String_To_ViBooleanArray(ViSession instrSession, ViConstString inputString, ViBoolean** booleanArray, ViInt32* arraySize)
{
	ViStatus error = VI_SUCCESS;
	ViChar* inputStringCopy = NULL;
	ViChar* ptr = NULL;
	ViInt32 idx;
	ViInt32 elementsCount;
	ViInt32 inputStringLen;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (inputString == NULL || booleanArray == NULL)
	{
		RsCoreInt_StrcpyMaxLen(errElab, RS_MAX_MESSAGE_BUF_SIZE, "RsCore_Convert_String_To_ViBooleanArray: NULL inputString or booleanArray pointer");
		viCheckErrElab(RS_ERROR_NULL_POINTER, errElab);
	}

	inputStringLen = (ViInt32)strlen(inputString);
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "RsCore_Convert_String_To_ViBooleanArray: Allocation of the inputString copy, length " _PERCU " bytes", inputStringLen);
	viCheckAllocElab(inputStringCopy = (ViChar*)malloc(inputStringLen + 1), errElab);
	memcpy(inputStringCopy, inputString, inputStringLen + 1);

	RsCore_TrimString(inputStringCopy, RS_VAL_TRIM_WHITESPACES);
	inputStringLen = (ViInt32)strlen(inputStringCopy);

	if (inputStringLen == 0)
	{
		// For empty string allocate one element with VI_FALSE value and return array size 0
		*arraySize = 0;
		viCheckAllocElab(*booleanArray = (ViBoolean*)malloc(sizeof(ViBoolean)), errElab);
		(*booleanArray)[0] = VI_FALSE;
		goto Error;
	}

	elementsCount = RsCore_GetElementsCount(inputStringCopy, ',');

	*arraySize = elementsCount;
	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "RsCore_Convert_String_To_ViBooleanArray: Allocation of the result ViBoolean array " _PERCD " elements", elementsCount);
	viCheckAllocElab(*booleanArray = (ViBoolean*)malloc(elementsCount * sizeof(ViBoolean)), errElab);

	ptr = strtok(inputStringCopy, ",");
	if (ptr == NULL)
	{
		snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE, "RsCore_Convert_String_To_ViBooleanArray: Unexpected input string '%s'", inputStringCopy);
		viCheckErrElab(RS_ERROR_UNEXPECTED_RESPONSE, errElab);
	}

	idx = 0;
	do
	{
		(*booleanArray)[idx++] = RsCore_Convert_String_To_Boolean(ptr);
		ptr = strtok(NULL, ",");
	}
	while (ptr);

Error:
	if (inputStringCopy)
		free(inputStringCopy);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_ViBooleanArray_To_String
    Converts array of ViBoolean numbers to a comma-separated string. VI_TRUE = 'ON', VI_FALSE = 'OFF'
    The function allocates the required memory and returns it as resultString.
    The minimum allocated size is RS_MAX_SHORT_MESSAGE_BUF_SIZE (4096 bytes)
    The beforeString is prepended at the start of the resultString.
    You can set the beforeString to empty or NULL.
    WARNING!!! - free the resultString after use
******************************************************************************************************/
ViStatus RsCore_Convert_ViBooleanArray_To_String(ViSession instrSession, ViBoolean* boolArray, ViInt32 arraySize, ViConstString beforeString,
                                                 ViChar** resultString)
{
	ViStatus error = VI_SUCCESS;
	ViChar *buffer = NULL, *ptr = NULL;
	ViInt32 allocSize = arraySize * 5;
	ViInt32 beforeStringLen = 0;
	ViInt32 idx;
	ViInt32 elementLen;
	ViChar errElab[RS_MAX_MESSAGE_BUF_SIZE];

	if (!isNullOrEmpty(beforeString))
	{
		beforeStringLen = (ViInt32)strlen(beforeString);
		allocSize += beforeStringLen;
	}

    if (allocSize < RS_MAX_MESSAGE_BUF_SIZE)
        allocSize = RS_MAX_MESSAGE_BUF_SIZE;

	snprintf(errElab, RS_MAX_MESSAGE_BUF_SIZE,
	         "RsCore_Convert_ViBooleanArray_To_String: Allocation of the resultString buffer for " _PERCU " ViBoolean elements, size " _PERCU " bytes",
	         arraySize, allocSize);
	viCheckAllocElab(buffer = (ViChar*)malloc(allocSize), errElab);

	ptr = buffer;
	if (!isNullOrEmpty(beforeString))
	{
		memcpy(buffer, beforeString, beforeStringLen + 1);
		ptr += beforeStringLen;
		allocSize -= beforeStringLen;
	}

	for (idx = 0; idx < arraySize; idx++)
	{
		if (boolArray[idx] == VI_TRUE)
			elementLen = snprintf(ptr, allocSize, "ON,");
		else
			elementLen = snprintf(ptr, allocSize, "OFF,");

		ptr += elementLen;
		allocSize -= elementLen;
	}

	*--ptr = '\0';

	*resultString = buffer;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_BinaryDataBlock_To_Float32Array
    Interprets inputBinDataBlock as float 4-bytes/number and returns them as the most universal ViReal64 array
    outputArraySize can be set to NULL
    If swapEndianness is TRUE, the inputBinDataBlock buffer is modified !!!
    WARNING!!! - free the outputArray after use
******************************************************************************************************/
ViStatus RsCore_Convert_BinaryDataBlock_To_Float32Array(ViSession instrSession, ViByte* inputBinDataBlock, ViInt32 inputBinDataSize,
                                                        ViReal64** outputArray, ViInt32* outputArraySize, ViBoolean swapEndianness)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 count;
	ViInt32 idx;
	ViReal32* aux = NULL;

	count = (ViInt32)(inputBinDataSize / 4);
	viCheckAllocElab(*outputArray = (ViReal64*)malloc(count * sizeof(ViReal64)),
		"Allocation of ViReal64 buffer for conversion BinDataBlock -> Float[]");

	if (swapEndianness == VI_TRUE)
	{
		RsCore_SwapEndianness(inputBinDataBlock, count, 4);
	}

	aux = (ViReal32 *)inputBinDataBlock;
	for (idx = 0; idx < count; idx++)
	{
		(*outputArray)[idx] = (ViReal64)aux[idx];
	}

	if (outputArraySize)
		*outputArraySize = count;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_Convert_Float32Array_To_BinaryDataBlock
    Converts ViReal64 array into float32 array 4 bytes/number and return them as Binary Data Block
    inputArray buffer is not modified.
    WARNING!!! - free the outputBinData after use
******************************************************************************************************/
ViStatus RsCore_Convert_Float32Array_To_BinaryDataBlock(ViSession instrSession, ViReal64* inputArray, ViInt32 inputArraySize,
                                                        ViByte** outputBinData, ViInt32* outputDataSize, ViBoolean swapEndianness)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 idx;
	ViByte* ptr;
	ViReal32 value;

	*outputDataSize = inputArraySize * sizeof(ViReal32);
	viCheckAllocElab(*outputBinData = (ViByte*)malloc(*outputDataSize), "Allocation of ViByte buffer for conversion Float[] -> BinDataBlock");
	ptr = *outputBinData;

	for (idx = 0; idx < inputArraySize; idx++)
	{
		value = (ViReal32)inputArray[idx];
		memcpy(ptr, &value, sizeof(ViReal32));
		ptr += sizeof(ViReal32);
	}

	if (swapEndianness == VI_TRUE)
	{
		checkErr(RsCore_SwapEndianness(*outputBinData, inputArraySize, 4));
	}

Error:
	return error;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Utility functions -------------------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  Trims string in-place for white spaces and optionally quotation marks.
    Returns the result string length.
    Quotation marks are only trimmed if they are symmetrical and they the string does not contain them
    in the middle od the text. For example, the following string will not be trimmed: 'Item1','Item2'
    The function is optimized for high-performance
    Mode:
    - RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES: Remove all white spaces and all types of paired quotes
    - RS_VAL_TRIM_WCH_AND_SINGLE_QUOTES: Remove all white spaces and all paired single quotes
    - RS_VAL_TRIM_WCH_AND_DOUBLE_QUOTES: Remove all white spaces and all paired double quotes
    - RS_VAL_TRIM_WCH: Remove all white spaces only
******************************************************************************************************/
ViInt32 RsCore_TrimString(ViString text, ViInt32 mode)
{
	ViChar* p2char;
	ViInt32 finalCutIx, startIx, endIx;
	ViInt32 trimmedFlag;
	ViInt32 firstSingleQuotesIx = -1;
	ViInt32 firstDoubleQuotesIx = -1;
	ViInt32 removeSingleQuotes = (mode == RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES || mode == RS_VAL_TRIM_WHITESPACES_AND_SINGLE_QUOTES);
	ViInt32 removeDoubleQuotes = (mode == RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES || mode == RS_VAL_TRIM_WHITESPACES_AND_DOUBLE_QUOTES);
	ViInt32 len = (ViInt32)strlen(text);

	if (len == 0)
		return 0;

	// trim possible end linefeeds
	p2char = text + len - 1;
	while (p2char >= text && isspace((unsigned char)*p2char))
		p2char--;

	p2char++;
	*(p2char) = 0;

	// if empty string
	if (p2char <= text)
		return 0;

	// trim possible start linefeeds
	p2char = text;
	while (isspace((unsigned char)*p2char))
		p2char++;

	if (p2char > text)
	{
		len = (ViInt32)strlen(p2char);
		// White spaces at the beginning, shift the buffer left by p2char-text
		memmove(text, p2char, len);
		text[len] = 0;
	}

	startIx = 0;
	len = (ViInt32)strlen(text);
	endIx = len - 1;

	if (removeSingleQuotes || removeDoubleQuotes)
	{
		if (endIx - 2 < startIx)
		{
			// string has 2 or less characters
			if (removeSingleQuotes && strcmp(text, "''") == 0)
				text[0] = 0;

			if (removeDoubleQuotes && strcmp(text, "\"\"") == 0)
				text[0] = 0;

			return 0;
		}

		// Loop to cut the outer paired quotation marks
		do
		{
			trimmedFlag = 0;
			if (removeSingleQuotes && text[startIx] == '\'' && text[endIx] == '\'')
			{
				if (firstSingleQuotesIx < 0)
					firstSingleQuotesIx = startIx;

				startIx++;
				endIx--;
				trimmedFlag = 1;
			}

			if (endIx - 2 < startIx) break;

			if (removeDoubleQuotes && text[startIx] == '"' && text[endIx] == '"')
			{
				if (firstDoubleQuotesIx < 0)
					firstDoubleQuotesIx = startIx;

				startIx++;
				endIx--;
				trimmedFlag = 1;
			}

			if (endIx - 2 < startIx) break;
		} while (trimmedFlag > 0);

		len = (ViInt32)strlen(text);

		if (startIx == 0)
			return len;

		finalCutIx = startIx;
		if (firstSingleQuotesIx >= 0 && memchr(text + startIx, '\'', len - 2 * startIx) != NULL)
		{
			finalCutIx = firstSingleQuotesIx;
		}

		if (firstDoubleQuotesIx >= 0 && memchr(text + startIx, '"', len - 2 * startIx) != NULL)
		{
			finalCutIx = finalCutIx < firstDoubleQuotesIx ? finalCutIx : firstDoubleQuotesIx;
		}

		if (finalCutIx > 0)
		{
			len = (ViInt32)strlen(text) - 2 * finalCutIx;
			memmove(text, text + finalCutIx, len);
			text[len] = 0;
		}
	}

	return len;
}

/*****************************************************************************************************/
/*  Parses catalogIn as csv-string and removes potential paired quotes (single or double) from each individual element
	Returns catalogOut as a csv-string in the same order
	Example: 'ElementA','ValueB','ItemC',"MemberD","FollowerE" -> ElementA,ValueB,ItemC,MemberD,FollowerE
******************************************************************************************************/
ViStatus RsCore_ParseCatalog(ViConstString catalogIn, ViInt32 bufferSize, ViChar* catalogOut, ViInt32* elementsCount)
{
	ViStatus error = VI_SUCCESS;
	ViChar* element = NULL;
	ViInt32 idx, count = 0;
	ViInt32 size = (ViInt32)strlen(catalogIn) + 1;

	element = (ViChar*)malloc(size);
	checkAlloc(element);

	RsCoreInt_StrcpyMaxLen(catalogOut, bufferSize, "");
	RsCoreInt_StrcpyMaxLen(element, size, catalogIn);
	RsCore_TrimString(element, RS_VAL_TRIM_WHITESPACES);
	if (isNullOrEmpty(element))
		goto Error;

	count = RsCore_GetElementsCount(element, ',');

	for (idx = 0; idx < count; idx++)
	{
		RsCore_GetTokenAtIndex(catalogIn, ',', idx, element, size, NULL);
		RsCore_TrimString(element, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		RsCore_AppendToCsvString(catalogOut, ",", bufferSize, element);
	}

Error:
	if (element)
		free(element);

	if (elementsCount)
		*elementsCount = count;

	return error;
}

/*****************************************************************************************************/
/*  Replaces one or all occurences of a searchString within the string with the replaceString
	The length is always kept within the bufferSize bounds, if the buffer would overflow, the replacement stops
	If replaceAll is TRUE, all occurences of the searchString are replaced without searching already replaced parts
	Returns number of replacements
******************************************************************************************************/
ViInt32 RsCore_ReplaceString(ViChar* string, ViInt32 bufferSize, ViChar* searchString, ViChar* replaceString, ViBoolean replaceAll)
{
	ViInt32 count = 0;
	ViInt32 offset = 0;
	ViInt32 searchLen = (ViInt32)strlen(searchString);
	ViInt32 replaceLen = (ViInt32)strlen(replaceString);
	ViInt32 growLen = replaceLen - searchLen;

	ViChar* ptr;
	while ((ptr = strstr(string + offset, searchString)) != NULL)
	{
		if (growLen < 0)
		{
			// Replace pattern is smaller
			// Put the new string in place
			memcpy(ptr, replaceString, replaceLen);
			// Copy the part after the searchString including termination
			memcpy(ptr + replaceLen, ptr + searchLen, (ViInt32)strlen(ptr + searchLen) + 1);
		}
		else
		{
			// Replace pattern is bigger
			// Buffer overflow check
			if ((ViInt32)(strlen(string)) + 1 > (bufferSize - growLen))
			{
				// Cannot replace anymore, the buffer would overflow, finish
				break;
			}

			// Copy the part after the searchString including termination
			memcpy(ptr + replaceLen, ptr + searchLen, (ViInt32)strlen(ptr + searchLen) + 1);
			// Put the new string in place
			memcpy(ptr, replaceString, replaceLen);
		}

		count++;

		// Put the offset after the replace string to avoid repeated matching
		offset = (ViInt32)(ptr - string) + replaceLen;

		if (replaceAll == VI_FALSE && count > 0)
			break;
	}

	return count;
}

/*****************************************************************************************************/
/*  RsCore_FindStringIndex
	Searches for the stringToFind in the provided patternsArray.
	The last element of the patternsArray must be NULL!!!
	Returned value is the index of the response in the patternsArray
	If the response is not found, the function returns = -1
******************************************************************************************************/
ViInt32 RsCore_FindStringIndex(ViString patternsArray[], ViConstString stringToFind)
{
	ViInt32 idx = 0;

	while (patternsArray[idx])
	{
		if (strcmp(stringToFind, patternsArray[idx]) == 0)
			return idx;

		idx++;
	}

	return -1;
}

/*****************************************************************************************************/
/*  RsCore_GetElementsCount
    Returns number of elements in the string separated by the provided delimiter.
    The minimum returned count is 1, for empty strings or string where no delimiter is found.
******************************************************************************************************/
ViInt32 RsCore_GetElementsCount(ViConstString string, ViChar delimiter)
{
	ViInt32 elementsCount = 1;
	ViInt32 idx = 0;
	do
	{
		if (string[idx] == 0)
			break;

		if (string[idx++] == delimiter)
			elementsCount++;
	} while (1);

	return elementsCount;
}

/*****************************************************************************************************/
/*  RsCore_GetTokenAtIndex
    Returns string token at the entered index. The inputString is not modified.
    tokenOffset can be left NULL if not needed. Otherwise it returns the offset of the token
    relative to beginning of the string.
******************************************************************************************************/
ViStatus RsCore_GetTokenAtIndex(ViConstString inputString,
                                ViChar delimiter,
                                ViInt32 idx,
                                ViChar* token,
                                ViInt32 bufferSize,
                                ViInt32* tokenOffset)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 cnt = 0;
	ViInt32 tokenLen;
	ViInt32 len = (ViInt32)strlen(inputString);
	ViInt32 searchLen;
	ViChar* ptr;
	ViInt32 offset = 0;

	if (bufferSize <= 0)
		return RS_ERROR_INVALID_PARAMETER;

	if ((token == NULL) || (inputString == NULL))
		return RS_ERROR_NULL_POINTER;

	do
	{
		searchLen = len - offset;
		ptr = (ViChar*)memchr(inputString + offset, delimiter, searchLen);

		if (cnt == idx)
		{
			if (ptr)
			{
				// delimiter found
				tokenLen = (ViInt32)(ptr - inputString - offset);
			}
			else
			{
				// delimiter not found, take the string to the end
				tokenLen = searchLen;
			}

			if (tokenLen > bufferSize - 1)
				tokenLen = bufferSize - 1;

			memcpy(token, inputString + offset, tokenLen);
			token[tokenLen] = 0;

			if (tokenOffset)
				*tokenOffset = offset;

			break;
		}

		if (!ptr)
			return RS_ERROR_INDEX_IS_TOO_HIGH;

		offset = (ViInt32)(ptr - inputString) + 1;
		cnt++;
	} while (1);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_GetIndexOfToken
    Assuming the inputString is an array of elements separated by the provided delimiter,
    the function returns index of the tokenToFind element in the inputString.
    If the tokenToFind does not exist, the function returns RS_ERROR_INVALID_VALUE and foundIdx is -1
    tokenOffset can be left NULL if not needed. Otherwise it returns the offset of the token
    relative to beginning of the string.
******************************************************************************************************/
ViStatus RsCore_GetIndexOfToken(ViConstString inputString,
                                ViChar delimiter,
                                ViConstString tokenToFind,
                                ViInt32* foundIdx,
                                ViInt32* tokenOffset)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 cnt = 0;
	ViChar* ptr;
	ViInt32 tokenToFindLen = (ViInt32)strlen(tokenToFind);
	ViInt32 offset = 0;
	ViInt32 inputStringLen;

	*foundIdx = -1;

	if (!inputString)
		return RS_ERROR_NULL_POINTER;

	inputStringLen = (ViInt32)strlen(inputString);

	if (tokenToFindLen == 0)
		return RS_ERROR_INVALID_VALUE;

	do
	{
		ptr = (ViChar *)memchr(inputString + offset, delimiter, inputStringLen - offset);
		if (ptr == NULL)
		{
			if (tokenToFindLen == inputStringLen - offset && strncmp(tokenToFind, inputString + offset, tokenToFindLen) == 0)
			{
				*foundIdx = cnt;

				if (tokenOffset)
					*tokenOffset = offset;

				return error;
			}

			break;
		}

		if (tokenToFindLen == ptr - inputString - offset && strncmp(tokenToFind, inputString + offset, tokenToFindLen) == 0)
		{
			*foundIdx = cnt;

			if (tokenOffset)
				*tokenOffset = offset;

			return error;
		}

		offset = (ViInt32)(ptr - inputString) + 1;
		cnt++;
	} while (1);

	return RS_ERROR_INVALID_VALUE;
}

/*****************************************************************************************************/
/*  RsCore_StrcatMaxLen
    Works like strcat, but keeps the target string within the bufferSize - 1
    If the result string would overflow the buffer, it is copied only
    up to the maximum length. The target string is always properly null-terminated
    Either of append1 or append2 or append3 can be set to NULL
******************************************************************************************************/
ViBoolean RsCore_StrcatMaxLen(ViChar* target, ViInt32 bufferSize, ViConstString append1, ViConstString append2, ViConstString append3)
{
	ViInt32 targetLen = (ViInt32)strlen(target);
	ViInt32 len;
	ViConstString strings[3] = {append1, append2, append3};
	ViInt32 i;

	if (bufferSize <= 0)
		return VI_TRUE;

	bufferSize--;
	if (targetLen >= bufferSize)
		// The target is already longer than the bufferSize
		return VI_TRUE;

	for (i = 0; i < 3; i++)
	{
		if (strings[i] != NULL)
		{
			len = (ViInt32)strlen(strings[i]);
			if (targetLen + len > bufferSize)
			{
				// The result target would be longer than the bufferSize
				memcpy(target + targetLen, strings[i], bufferSize - targetLen);
				*(target + bufferSize) = 0;
				return VI_TRUE;
			}

			// The result target is less than the bufferSize
			memcpy(target + targetLen, strings[i], len + 1);
			targetLen += len;
		}
	}

	return VI_FALSE;
}

/*****************************************************************************************************/
/*  RsCore_StrCatDynamicString
    If the target buffer is NULL, it is newly allocated to accomodate all strings append1, append2, append3
    If the target buffer is not NULL, it is reallocated to accomodate the whole concatenated content
    Either of append1 or append2 or append3 can be set to NULL
    WARNING!!!: 'target' must be allocated dynamically. You have to deallocate it afterwards.
******************************************************************************************************/
ViStatus RsCore_StrCatDynamicString(ViSession instrSession, ViChar** target, ViConstString append1, ViConstString append2, ViConstString append3)
{
	ViStatus error = VI_SUCCESS;
	size_t targetLen = 0, len1 = 0, len2 = 0, len3 = 0;
	ViChar* ptr;

	if (append1 != NULL)
		len1 = strlen(append1);

	if (append2 != NULL)
		len2 = strlen(append2);

	if (append3 != NULL)
		len3 = strlen(append3);

	if (*target == NULL)
	{
		viCheckAllocElab(*target = (ViChar *)malloc(len1 + len2 + len3 + 1), "RsCore_StrCatDynamicString: Allocation of a new buffer buffer");
		ptr = *target;
	}
	else
	{
		targetLen = strlen(*target);
		viCheckReallocElab(*target, (ViChar *)realloc(*target, targetLen + len1 + len2 + len3 + 1), "RsCore_StrCatDynamicString: Reallocation of the buffer");
		ptr = *target + targetLen;
	}

	if (append1 != NULL)
	{
		memcpy(ptr, append1, len1);
		ptr += len1;
	}

	if (append2 != NULL)
	{
		memcpy(ptr, append2, len2);
		ptr += len2;
	}

	if (append3 != NULL)
	{
		memcpy(ptr, append3, len3);
		ptr += len3;
	}

	ptr[0] = 0;

Error:
	return error;
}

/*****************************************************************************************************/
/*  RsCore_StrCatDynamicViInt32
    If the target buffer is NULL, it is newly allocated to accomodate all append1 + valueToAppend + append2
    If the target buffer is not NULL, it is reallocated to accomodate the target + append1 + valueToAppend + append2
    append1 or append2 can be set to NULL
    WARNING!!!: 'target' must be allocated dynamically. You have to deallocate it afterwards.
******************************************************************************************************/
ViStatus RsCore_StrCatDynamicViInt32(ViSession instrSession, ViChar** target, ViConstString append1, ViInt32 valueToAppend, ViConstString append2)
{
	ViChar number[64];

	snprintf(number, 64, _PERCD, valueToAppend);
	return RsCore_StrCatDynamicString(instrSession, target, append1, number, append2);
}

/*****************************************************************************************************/
/*  RsCore_StrCatDynamicViInt64
    If the target buffer is NULL, it is newly allocated to accomodate all append1 + valueToAppend + append2
    If the target buffer is not NULL, it is reallocated to accomodate the target + append1 + valueToAppend + append2
    append1 or append2 can be set to NULL
    WARNING!!!: 'target' must be allocated dynamically. You have to deallocate it afterwards.
******************************************************************************************************/
ViStatus RsCore_StrCatDynamicViInt64(ViSession instrSession, ViChar** target, ViConstString append1, ViInt64 valueToAppend, ViConstString append2)
{
	ViChar number[64];

	snprintf(number, 64,  _PERCLD, valueToAppend);
	return RsCore_StrCatDynamicString(instrSession, target, append1, number, append2);
}

/*****************************************************************************************************/
/*  RsCore_StrCatDynamicViReal64
    If the target buffer is NULL, it is newly allocated to accomodate all append1 + valueToAppend + append2
    If the target buffer is not NULL, it is reallocated to accomodate the target + append1 + valueToAppend + append2
    append1 or append2 can be set to NULL
    WARNING!!!: 'target' must be allocated dynamically. You have to deallocate it afterwards.
******************************************************************************************************/
ViStatus RsCore_StrCatDynamicViReal64(ViSession instrSession, ViChar** target, ViConstString append1, ViReal64 valueToAppend, ViConstString append2)
{
	ViChar number[64];

	RsCore_Convert_ViReal64_To_String(valueToAppend, 64, number);
	return RsCore_StrCatDynamicString(instrSession, target, append1, number, append2);
}

/*****************************************************************************************************/
/*  RsCore_StrCatDynamicViBoolean
    If the target buffer is NULL, it is newly allocated to accomodate all append1 + valueToAppend + append2 append1 + ViBoolean value  + append2
    ViBoolean value is converted to string: VI_TRUE -> 'ON', VI_FALSE -> 'OFF'
    If the target buffer is not NULL, it is reallocated to accomodate the target + append1 + valueToAppend + append2
    append1 or append2 can be set to NULL
    WARNING!!!: 'target' must be allocated dynamically. You have to deallocate it afterwards.
******************************************************************************************************/
ViStatus RsCore_StrCatDynamicViBoolean(ViSession instrSession, ViChar** target, ViConstString append1, ViBoolean valueToAppend, ViConstString append2)
{
	return RsCore_StrCatDynamicString(instrSession, target, append1, (valueToAppend == VI_TRUE) ? "ON" : "OFF", append2);
}

/*****************************************************************************************************/
/*  RsCore_AddToCsvString
    If the target is empty, the function appends the element. e.g.: "" -> "item1"
    If the target is non-empty, the function appends the delimiter + element. e.g.: "item1" -> "item1,item2"
    Hence, it is suitable for composing csv-strings
    Returns pointer to the end of the result target string
    The function  makes sure, that the target string length is within the targetBufferSize - 1
    If the result string would overflow the buffer, it is copied only up to the maximum length.
    The target string is always properly null-terminated
******************************************************************************************************/
ViChar* RsCore_AppendToCsvString(ViChar* target, ViChar* delimiter, ViInt32 targetBufferSize, ViConstString element)
{
	if (target[0] != 0)
	{
		RsCore_StrcatMaxLen(target, targetBufferSize, delimiter, element, NULL);
	}
	else
	{
		RsCore_StrcatMaxLen(target, targetBufferSize, element, NULL, NULL);
	}

	return target + strlen(target);
}

/*****************************************************************************************************/
/*  RsCore_Delay
    Suspends the current thread for the number of seconds specified.
    RsCore_Delay is accurate to a millisecond. The function is platform-independent.
******************************************************************************************************/
void RsCore_Delay(ViReal64 numberOfSeconds)
{
	clock_t StartTime = clock();

	while ((((double)(clock() - StartTime)) / (double)CLOCKS_PER_SEC) < (numberOfSeconds)) Sleep(0);

	return;
}

/*****************************************************************************************************/
/*  RsCore_CopyViReal64ArrayToUserBuffer
Copies ViReal64 array used by the driver to the user-provided buffer that is space-limited.
- If the user buffer is bigger or equal to the dataSize, the function copies the entire data
- If user buffer is smaller than the dataSize, only the userBufferSize is copied,
and the function returns positive number (warning) that equals the dataSize
WARNING!!! - Use this function as the last one, otherwise the potential warning
can be overwritten by the checkErr or viCheckErr macros
******************************************************************************************************/
ViStatus RsCore_CopyToUserBufferViReal64Array(ViSession instrSession, ViReal64* userBuffer, ViInt32 userBufferSize, ViReal64* data, ViInt64 dataSize)
{
	if ((size_t)userBufferSize >= (size_t)dataSize)
	{
		memcpy(userBuffer, data, (size_t)(dataSize * sizeof(ViReal64)));
		return VI_SUCCESS;
	}

	memcpy(userBuffer, data, (userBufferSize * sizeof(ViReal64)));

	return (ViStatus)dataSize;
}

/*****************************************************************************************************/
/*  RsCore_CopyToUserBufferAsciiData
    Copies ASCII DATA(null-terminated string) used by the driver to the user-provided buffer that is space-limited.
    - If the userBufferSize is at least one byte bigger than the sourceString size, the function copies the entire string.
    - If the userBufferSize is equal or smaller than the sourceString size, only the userBufferSize-1 characters are copied,
        the user buffer is null-terminated and the function returns positive number (warning) that equals sourceString size + 1
    WARNING!!! - Use this function as the last one, otherwise the potential warning
    can be overwritten by the checkErr or viCheckErr macros
******************************************************************************************************/
ViStatus RsCore_CopyToUserBufferAsciiData(ViSession instrSession, ViChar* userBuffer, ViInt32 userBufferSize, ViChar* sourceString)
{
	if (RsCoreInt_StrcpyMaxLen(userBuffer, userBufferSize, sourceString) == VI_TRUE)
		return (ViStatus)strlen(sourceString) + 1;

	return VI_SUCCESS;
}

/*****************************************************************************************************/
/*  RsCore_SwapEndianness
    Swaps endianness in-place for 2,4,8-byte values
    elementSize can be either 2,4 or 8 bytes
    arrayElementsCount * elementSize must equal to the block size.
******************************************************************************************************/
ViStatus RsCore_SwapEndianness(void* block, ViInt32 arrayElementsCount, ViInt32 elementSize)
{
	ViInt32 idx;
	ViByte* ptr = (ViByte *)block;
	ViByte carry;

	switch (elementSize)
	{
	case 2:
		for (idx = 0; idx < arrayElementsCount; idx++)
		{
			carry = ptr[0];
			ptr[0] = ptr[1];
			ptr[1] = carry;

			ptr += 2;
		}

		break;
	case 4:
		for (idx = 0; idx < arrayElementsCount; idx++)
		{
			carry = ptr[0];
			ptr[0] = ptr[3];
			ptr[3] = carry;

			carry = ptr[1];
			ptr[1] = ptr[2];
			ptr[2] = carry;

			ptr += 4;
		}

		break;
	case 8:
		for (idx = 0; idx < arrayElementsCount; idx++)
		{
			carry = ptr[0];
			ptr[0] = ptr[7];
			ptr[7] = carry;

			carry = ptr[1];
			ptr[1] = ptr[6];
			ptr[6] = carry;

			carry = ptr[2];
			ptr[2] = ptr[5];
			ptr[5] = carry;

			carry = ptr[3];
			ptr[3] = ptr[4];
			ptr[4] = carry;

			ptr += 8;
		}

		break;
	default:
		return VI_ERROR_NSUP_OPER;
	}

	return VI_SUCCESS;
}

/******************************************************************************************************
------------------------------------------------------------------------------------------------------*
---- Special Write/Read callbacks --------------------------------------------------------------------*
------------------------------------------------------------------------------------------------------*
******************************************************************************************************/

/*****************************************************************************************************/
/*  RsCore_SpecialCallback_SurroundParamWrite
    Supported data types: RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64, RS_VAL_STRING, RS_VAL_BOOLEAN
    Writes command with parameter enclosed with strings beforeParam and afterParam.
	WARNING!!! for beforeParam you must also include the space between the command and the parameter!!!
	Example: SCPI command: "SOUR:OUT"
	beforeParam: " <" , afterParam: ">"
	value: RS_VAL_STRING "ON"
	Complete command: "SOUR:OUT <ON>"
******************************************************************************************************/
ViStatus RsCore_SpecialCallback_SurroundParamWrite(ViSession instrSession,
                                                        ViConstString repCapName,
                                                        RsCoreAttributePtr attr, 
														ViConstString beforeParam,
														void* value,
														ViConstString afterParam)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;

	// If no command is available, exit the function
	if (isNullOrEmpty(attr->command))
		goto Error;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	if (attr->rangeTableCallback)
	{
		checkErr((attr->rangeTableCallback)(instrSession, repCapName, attr, &(attr->rangeTable)));
	}

	if ((attr->rangeTable) && (attr->rangeTable->type == RS_VAL_DISCRETE))
	{
		ViChar* p2cmdString = NULL;
		ViReal64 precision = attr->comparePrecision;

		// Allowed data types are only ViInt32 or ViReal64
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			checkErr(RsCore_FindRangeTableCmdString(instrSession, attr->rangeTable, *(ViInt32 *)value, &p2cmdString));
			break;
		case RS_VAL_REAL64:
			precision = (precision < DBL_MIN) ? DBL_MIN : precision;
			checkErr(RsCore_FindRangeTableCmdStringViReal64(instrSession, attr->rangeTable, *(ViReal64 *)value, precision, &p2cmdString));
			break;
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
				"RsCore_SpecialCallback_SurroundParamWrite: Discrete Range Table data types can only be RS_VAL_INT32, RS_VAL_REAL64"));
		}

		checkErr(RsCore_StrCatDynamicString(instrSession, &cmd, beforeParam, p2cmdString, afterParam));
	}
	else
	{
		// No range table, cmdString is composed based on the data type
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			checkErr(RsCore_StrCatDynamicViInt32(instrSession, &cmd, beforeParam, *(ViInt32 *)value, afterParam));
			break;
		case RS_VAL_INT64:
			checkErr(RsCore_StrCatDynamicViInt64(instrSession, &cmd, beforeParam, *(ViInt64 *)value, afterParam));
			break;
		case RS_VAL_REAL64:
			checkErr(RsCore_StrCatDynamicViReal64(instrSession, &cmd, beforeParam, *(ViReal64 *)value, afterParam));
			break;
		case RS_VAL_STRING:
			checkErr(RsCore_StrCatDynamicString(instrSession, &cmd, beforeParam, (ViString)value, afterParam));
			break;
		case RS_VAL_BOOLEAN:
			checkErr(RsCore_StrCatDynamicViBoolean(instrSession, &cmd, beforeParam, *(ViBoolean *)value, afterParam));
			break;
		case RS_VAL_EVENT:
			break;
		case RS_VAL_ADDR:
		case RS_VAL_SESSION:
		case RS_VAL_UNKNOWN_TYPE:
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, NULL));
		}
	}

	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/*****************************************************************************************************/
/*  RsCore_SpecialCallback_AnyQuotedParamRead
	Supported data types: RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64, RS_VAL_STRING, RS_VAL_BOOLEAN
	For all responses and all data types, it first removes all the paired quotes (single or double)
	Effectively, for RS_VAL_STRING the callback works exacly like the standard RsCore_ReadCallback()
******************************************************************************************************/
ViStatus RsCore_SpecialCallback_AnyQuotedParamRead(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViChar responseFixed[RS_MAX_MESSAGE_BUF_SIZE] = "";
	ViChar* responseUnknownLen = NULL; // For string attribute, use dynamic buffer
	ViInt32 responseSize = 0;
	ViReal64 rangeTableNumValue;
	ViChar* p2response = NULL;

	/* If no command available, exit the function */
	if (isNullOrEmpty(attr->command))
		goto Error;

	// Command string building from repeated capabilities
	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	if (attr->rangeTableCallback)
	{
		// Range Table Callback - attribute's range table is set dynamically by this callback
		checkErr(((attr->rangeTableCallback)(instrSession, repCapName, attr, &(attr->rangeTable))));
	}

	if (attr->dataType == RS_VAL_STRING)
	{
		// Query the instrument, get the unlimited size response
		checkErr(RsCore_QueryAttributeCommandUnknownLength(instrSession, attr, cmd, &responseUnknownLen, &responseSize));
		// For string-type attributes remove trailing Whitespaces and pair quotations
		RsCore_TrimString(responseUnknownLen, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		p2response = responseUnknownLen;
	}
	else
	{
		// Query the instrument, get the limited size response - RS_MAX_MESSAGE_BUF_SIZE
		checkErr(RsCore_QueryAttributeCommand(instrSession, attr, cmd, responseFixed));
		RsCore_TrimString(responseFixed, RS_VAL_TRIM_WHITESPACES_AND_ALL_QUOTES);
		p2response = responseFixed;
	}

	// In simulation, end here. Do not touch the cache value.
	// It is read by the GetAttribute and returned to the caller.
	if (RsCore_Simulating(instrSession))
		goto Error;

	if (attr->rangeTable && attr->rangeTable->type == RS_VAL_DISCRETE)
	{
		// Value has to converted to the range table value
		checkErr(RsCore_FindDiscreteRangeTableNumericValue(instrSession, attr->rangeTable, p2response, &rangeTableNumValue));
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			*(ViInt32 *)attr->p2value = (ViInt32)rangeTableNumValue;
			break;
		case RS_VAL_REAL64:
			*(ViReal64 *)attr->p2value = rangeTableNumValue;
			break;
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
				"RsCore_SpecialCallback_QuotedParamRead: Discrete Range table data type must be either RS_VAL_INT32 or RS_VAL_REAL64"));
		}
	}
	else
	{
		// No range table, covert the response directly to the appropriate type
		switch (attr->dataType)
		{
		case RS_VAL_INT32:
			*((ViInt32*)attr->p2value) = RsCore_Convert_String_To_ViInt32(p2response);
			break;
		case RS_VAL_INT64:
			*((ViInt64*)attr->p2value) = (ViInt64)RsCore_Convert_String_To_ViReal64(p2response);
			break;
		case RS_VAL_REAL64:
			*((ViReal64*)attr->p2value) = RsCore_Convert_String_To_ViReal64(p2response);
			break;
		case RS_VAL_STRING:
			checkErr(RsCore_SetAttributeP2Value(instrSession, attr, p2response));
			break;
		case RS_VAL_BOOLEAN:
			*((ViBoolean*)attr->p2value) = RsCore_Convert_String_To_Boolean(p2response);
			break;
		default:
			checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType,
				"RsCore_SpecialCallback_QuotedParamRead: Allowed data types: RS_VAL_INT32, RS_VAL_INT64, RS_VAL_REAL64, RS_VAL_STRING, RS_VAL_BOOLEAN"));
		}
	}

Error:
	if (cmd)
		free(cmd);

	if (responseUnknownLen)
		free(responseUnknownLen);

	return error;
}


/******************************************************************************************************
    RsCore_SpecialCallback_HexPatternWrite
    Supported data types: RS_VAL_STRING
    Writes string parameter in the following format: "#H<string_value>,<bitLen>" where bitLen = 4x size of the string
    Example: value='F0F0F0F0F0F0', composed command: SOUR:BB:SIR:DATA:PATT #HF0F0F0F0F0F0,48
*****************************************************************************************************/
ViStatus RsCore_SpecialCallback_HexPatternWrite(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViInt32 patternLength;

	// If no command is available, exit the function
	if (isNullOrEmpty(attr->command))
		goto Error;

	if (attr->dataType != RS_VAL_STRING)
	{
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SpecialCallback_HexPatternWrite only accepts RS_VAL_STRING data types"));
	}

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	patternLength = 4 * ((ViInt32)strlen((ViString)value));

	RsCore_StrCatDynamicString(instrSession, &cmd, " #H", (ViString)value, NULL);
	RsCore_StrCatDynamicViInt32(instrSession, &cmd, ",", patternLength, NULL);
	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/******************************************************************************************************
    RsCore_SpecialCallback_HexPatternRead
    Supported data types: RS_VAL_STRING
    Reads string response expected to be in the following format: "#H<string_value>,<bitLen>"
    Returns the <string_value> portion.
    Example: string response = '#HF0F0F0F0F0F0,48', returned value ='F0F0F0F0F0F0'
*****************************************************************************************************/
ViStatus RsCore_SpecialCallback_HexPatternRead(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	// If no command is available, exit the function
	if (isNullOrEmpty(attr->command))
		goto Error;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	if (attr->dataType != RS_VAL_STRING)
	{
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SpecialCallback_HexPatternRead only accepts type RS_VAL_STRING"));
	}

	checkErr(RsCore_QueryAttributeCommand(instrSession, attr, cmd, response));
	sscanf(response, "#H%[^,]", buffer);
	checkErr(RsCore_SetAttributeP2Value(instrSession, attr, buffer));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/******************************************************************************************************
    RsCore_SpecialCallback_BinPatternRead
    Supported data types: RS_VAL_STRING
    Reads string response expected to be in the following formats:
    - "#B<string_value>,<bitLen>" - returns the <string_value> portion.
        Example: string response = '#B1110001110000110,16', returned value ='1110001110000110'
    - "#H<string_value>,<bitLen>" - returns the recalculated binary pattern string
        Example: string response = '#HF2,8', returned value ='11110010'
*****************************************************************************************************/
ViStatus RsCore_SpecialCallback_BinPatternRead(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 i = 0, patternLen, responseLen, diff;
	ViChar* cmd = NULL;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];

	// If no command available, exit the function
	if (attr->command == NULL)
		return VI_SUCCESS;

	if (attr->dataType != RS_VAL_STRING)
	{
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SpecialCallback_BinPatternRead only accepts type RS_VAL_STRING"));
	}

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));
	checkErr(RsCore_QueryAttributeCommand(instrSession, attr, cmd, response));
	i = sscanf(response, "#H%[^,]", buffer);
	if (i == 1)
	{
		i = sscanf(response, "#H%[^,]," _PERCD, buffer, &patternLen);
		// #H format detected, get every character as hexa and expand it to 4 bin characters
		(void)RsCore_Convert_HexaPattern_To_BinPattern(buffer, RS_MAX_MESSAGE_BUF_SIZE, response);
		if (i == 2)
		{
			if (patternLen > RS_MAX_MESSAGE_BUF_SIZE - 1)
			{
				viCheckErrElab(RS_ERROR_STRING_TOO_LONG, "RsCore_SpecialCallback_BinPatternRead: patternLen is longer than 4095 bytes");
			}

			// If the hex patternLen indicate other size than the response size, prepend / trim the response
			// to have responseLen = patternLen
			responseLen = (ViInt32)strlen(response);
			diff = patternLen - responseLen;
			if (diff == 0)
			{
				checkErr(RsCore_SetAttributeP2Value(instrSession, attr, response));
			}
			else if (diff > 0)
			{
				// Prepend nulls
				memset(buffer, '0', diff);
				RsCoreInt_StrcpyMaxLen(buffer + diff, RS_MAX_MESSAGE_BUF_SIZE - diff, response);
				checkErr(RsCore_SetAttributeP2Value(instrSession, attr, buffer));
			}
			else if (diff < 0)
			{
				// Trim the beginning
				checkErr(RsCore_SetAttributeP2Value(instrSession, attr, response - diff));
			}

			goto Error;
		}
		checkErr(RsCore_SetAttributeP2Value(instrSession, attr, response));
		goto Error;
	}

	i = sscanf(response, "#B%[^,]", buffer);
	if (i == 1)
	{
		// #B format detected, result equals the buffer
		checkErr(RsCore_SetAttributeP2Value(instrSession, attr, buffer));
	}

Error:
	if (cmd)
		free(cmd);

	return error;
}

/******************************************************************************************************
    RsCore_SpecialCallback_BinPatternWrite
    Supported data types: RS_VAL_STRING
    Writes string parameter in the following format: "#B<string_value>,<bitLen>" where bitLen = 4x size of the string
    Example: value='00110010', composed command: SOUR1:BB:WLNN:TRIG:OUTP1:PATT #B11001010,8
    The function can properly handle the following input values:
    '#B00110011,20' -> '#B00110011,8' - corrects the pattern length and does not add the #B prefix if not necessary
    '#HF0F0,40' or '#HF0F0' -> '#B1111000011110000,16' - converts the hexa pattern and properly adds the bin length
    '001100' -> '#B001100,6' - adds the #B prefix and the proper length
*****************************************************************************************************/
ViStatus RsCore_SpecialCallback_BinPatternWrite(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViInt32 patternLength;
	ViChar result[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar pattern[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar *ptr, *comma;

	// If no command available, exit the function
	if (attr->command == NULL)
		return VI_SUCCESS;

	if (attr->dataType != RS_VAL_STRING)
	{
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SpecialCallback_BinPatternWrite only accepts type RS_VAL_STRING"));
	}

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	ptr = (ViString)value;
	comma = strchr(ptr, ',');
	if (comma)
	{
		// Comma detected, cut the string there
		RsCoreInt_StrcpyMaxLen(pattern, (ViInt32)(comma - ptr) + 1, ptr);
		ptr = pattern;
	}

	if (strstr(ptr, "#B") == ptr)
	{
		// Already has #B at the beginning
		RsCore_StrCatDynamicString(instrSession, &cmd, " ", ptr, NULL);
		patternLength = (ViInt32)strlen(ptr) - 2;
	}
	else if (strstr(ptr, "#H") == ptr)
	{
		// #H at the beginning, convert the data to bin
		RsCore_Convert_HexaPattern_To_BinPattern(ptr + 2, RS_MAX_MESSAGE_BUF_SIZE, result);
		RsCore_StrCatDynamicString(instrSession, &cmd, " #B", result, NULL);
		patternLength = (ViInt32)strlen(result);
	}
	else
	{
		// Add #B at the beginning
		RsCore_StrCatDynamicString(instrSession, &cmd, " #B", ptr, NULL);
		patternLength = (ViInt32)strlen(ptr);
	}

	RsCore_StrCatDynamicViInt32(instrSession, &cmd, ",", patternLength, NULL);
	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}

/******************************************************************************************************
    RsCore_SpecialCallback_HexNumberRead
    Supported data types: RS_VAL_INT32, RS_VAL_INT64
    Reads the value returned from the instrument in hexa format and returns is as decimal number
    Trims the patternLen after the comma
    Example: return value = '#H400,12' -> 1024
*****************************************************************************************************/
ViStatus RsCore_SpecialCallback_HexNumberRead(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr)
{
	ViStatus error = VI_SUCCESS;
	ViInt32 int32Value;
	ViInt64 int64value;
	ViChar *cmd = NULL, *ptr = NULL;
	ViChar response[RS_MAX_MESSAGE_BUF_SIZE];


	// If no command available, exit the function
	if (attr->command == NULL)
		return VI_SUCCESS;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));
	checkErr(RsCore_QueryAttributeCommand(instrSession, attr, cmd, response));
	ptr = strchr(response, ',');
	if (ptr)
		ptr[0] = 0;

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
		sscanf(response, "#H" _PERCX, &int32Value);
		checkErr(RsCore_SetAttributeP2Value(instrSession, attr, &int32Value));
		break;
	case RS_VAL_INT64:
		sscanf(response, "#H" _PERCLX, &int64value);
		checkErr(RsCore_SetAttributeP2Value(instrSession, attr, &int64value));
		break;
	default:
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SpecialCallback_HexNumberRead only accepts type RS_VAL_INT32 and RS_VAL_INT64"));
	}

Error:
	if (cmd)
		free(cmd);

	return error;
}

/******************************************************************************************************
    RsCore_SpecialCallback_HexNumberWrite
    Supported data types: RS_VAL_INT32, RS_VAL_INT64
    Writes the input decimal value to instrument in hexa format and appends bites length after comma:
    Example: value = 1024 -> '#H400,12'
*****************************************************************************************************/
ViStatus RsCore_SpecialCallback_HexNumberWrite(ViSession instrSession, ViConstString repCapName, RsCoreAttributePtr attr, void* value)
{
	ViStatus error = VI_SUCCESS;
	ViChar* cmd = NULL;
	ViChar buffer[RS_MAX_MESSAGE_BUF_SIZE];

	// If no command available, exit the function
	if (attr->command == NULL)
		return VI_SUCCESS;

	checkErr(RsCore_BuildAttrCommandString(instrSession, attr, repCapName, &cmd));

	switch (attr->dataType)
	{
	case RS_VAL_INT32:
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, " #H" _PERCX ",", *(ViUInt32*)value);
		break;
	case RS_VAL_INT64:
		snprintf(buffer, RS_MAX_MESSAGE_BUF_SIZE, " #H" _PERCLX ",", *(ViUInt64*)value);
		break;
	default:
		checkErr(RsCore_GenerateInvalidDataTypeError(instrSession, attr->dataType, "RsCore_SpecialCallback_HexNumberWrite only accepts type RS_VAL_INT32 and RS_VAL_INT64"));
	}

	checkErr(RsCore_StrCatDynamicViInt32(instrSession, &cmd, buffer, (ViInt32)(strlen(buffer)-4) * 4, NULL));
	checkErr(RsCore_WriteAttributeCommand(instrSession, attr, cmd));

Error:
	if (cmd)
		free(cmd);

	return error;
}