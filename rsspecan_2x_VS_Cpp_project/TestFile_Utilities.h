#pragma once

#ifndef __RS_TESTFILE_UTILITIES_HEADER
#define __RS_TESTFILE_UTILITIES_HEADER

#include <visatype.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif


#include <Windows.h>
#include <rscore.h>

#define MUTEX HANDLE
//==============================================================================
// Constants
//==============================================================================
#define TESTS_STRING_LENGTH 4096
#define TESTS_ERROR_NOT_EQUAL -123456

//==============================================================================
// Macros
//==============================================================================
#ifndef checkErr
#define checkErr(fCall) error = (fCall); if (error < 0) { goto Error; }
#endif

#ifndef checkTestErr
#define checkTestErr(condition, message) if(!(condition)) { AddTestFailEntry(__LINE__, message); } else { AddTestPassEntry(__LINE__, message); }
#endif

#ifndef assertTestErr
#define assertTestErr(fCall, expError, elabPattern) error = (fCall); AddAssertionFailureErrorEntry(io, __LINE__, expError, error, elabPattern);
#endif

#ifndef assertErrCode
#define assertErrCode(fCall, expError, elabPattern) error = (fCall); AddAssertionFailureErrorCode(io, __LINE__, expError, error, elabPattern);
#endif
//==============================================================================
// Functions
//==============================================================================
int MutexInit(MUTEX* mutex);
int MutexLock(MUTEX* mutex);
int MutexUnlock(MUTEX* mutex);

ViStatus DisplayResult(ViSession io, ViStatus error);
void ClearTestErrors(void);
void AddTestFailEntry(ViInt32 line, ViString failEntry);
void AddTestPassEntry(ViInt32 line, ViString passEntry);
ViBoolean AddAssertionFailureErrorEntry(ViSession io, ViInt32 line, ViStatus expError, ViStatus error,
                                        ViString elabPattern);
ViBoolean AddAssertionFailureErrorCode(ViSession io, ViInt32 line, ViStatus expError, ViStatus error,
                                       ViString elabPattern);
void TestMessage(ViString message);
void InfoMessage(ViString message);
int CompareTwoViReal64Numbers(ViReal64 number1, ViReal64 number2, ViReal64 tolerance);
int CompareTwoViReal64Arrays(ViReal64* array1, ViReal64* array2, ViUInt32 size, ViReal64 tolerance);
int CompareTwoViBooleanArrays(ViBoolean* array1, ViBoolean* array2, ViUInt32 size);
int CompareTwoViInt32Arrays(ViInt32* array1, ViInt32* array2, ViUInt32 size);
int CompareTwoFiles(ViString file1, ViString file2);
void GenerateRandomString(ViChar* string, ViUInt32 bufferLen);
void GenerateRandomViReal64Array(ViReal64* inputArray, ViReal64 min, ViReal64 max, ViUInt32 length);
void GenerateRandomViReal32Array(ViReal32* inputArray, ViReal32 min, ViReal32 max, ViUInt32 length);
void GenerateRandomViInt32array(ViInt32* inputArray, ViInt32 min, ViInt32 max, ViUInt32 length);

ViStatus CoreTests_GetAttributeProperties(ViSession instrSession, ViAttr attributeId, RsCoreAttributePtr* attr);
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // __RS_TESTFILE_UTILITIES_HEADER
