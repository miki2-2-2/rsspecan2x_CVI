#include "TestFile_Utilities.h"
#include "rsspecan.h"
#include "math.h"

#if defined (_CVI_)
#include <utility.h>
#endif

ViChar static gTestStepErrorEntries[10000];

// Clearing error entries
void ClearTestErrors(void)
{
	strncpy(gTestStepErrorEntries, "", 10000);
}

// add the message to log file
ViStatus AddToFileLog(ViString message)
{
	size_t written = 0;
	FILE* targetFile = NULL;

	if ((targetFile = fopen("c:\\temp\\_cvi_test_log_file.txt", "a")) == NULL)
		return RS_ERROR_CANNOT_OPEN_FILE;

	written = fwrite(message, sizeof(ViChar), strlen(message), targetFile);
	written = fclose(targetFile);

	return VI_SUCCESS;
}

// Entering new non-fatal error
void AddTestFailEntry(ViInt32 line, ViString failEntry)
{
	ViBoolean empty = VI_TRUE;
	ViChar entryString[RS_MAX_MESSAGE_BUF_SIZE];

	if (strlen(gTestStepErrorEntries) > 0)
	{
		empty = VI_FALSE;
	}

	if (!empty)
	{
		RsCore_StrcatMaxLen(gTestStepErrorEntries, 10000, "\n", NULL, NULL);
	}

	snprintf(entryString, RS_MAX_MESSAGE_BUF_SIZE, "%d: FAILED - %s", line, failEntry);
	RsCore_StrcatMaxLen(gTestStepErrorEntries, 10000, entryString, NULL, NULL);
	TestMessage(entryString);
	TestMessage("\n");
}

// Entering new pass message
void AddTestPassEntry(ViInt32 line, ViString passEntry)
{
	ViChar entryString[RS_MAX_MESSAGE_BUF_SIZE];

	snprintf(entryString, RS_MAX_MESSAGE_BUF_SIZE, "%d: Passed - %s", line, passEntry);
	TestMessage(entryString);
	TestMessage("\n");
}

/******************************************************************************************************/

// Displaying fatal error result
ViStatus DisplayResult(ViSession io, ViStatus error)
{
	ViChar errorMessage[4096];
	ViChar testMessage[4096];
	ViStatus storedError;

	if (error < VI_SUCCESS)
	{
		rsspecan_GetError(io, &storedError, TESTS_STRING_LENGTH, errorMessage);
		if (storedError != VI_SUCCESS)
		{
			snprintf(testMessage, 4096, "\n\nERROR OCCURED:\n%s", errorMessage);
			TestMessage(testMessage);
		}
		else
		{
			rsspecan_error_message(io, error, errorMessage);
			snprintf(testMessage, 4096, "\n\nERROR OCCURED (not stored by SetError):\n%s", errorMessage);
			TestMessage(testMessage);
		}
	}
	else
	{
		printf("Finished sucessfully");
	}

	if (strlen(gTestStepErrorEntries) > 0)
	{
		printf("\n\nErrors in the test results queue:\n%s", gTestStepErrorEntries);
	}

	return error;
}

/******************************************************************************************************/

ViBoolean AddAssertionFailureErrorEntry(ViSession io, ViInt32 line, ViStatus expError, ViStatus error,
                                        ViString elabPattern)
{
	ViBoolean result = VI_TRUE;
	ViStatus primary;
	ViChar elaboration[RS_MAX_MESSAGE_BUF_SIZE];
	ViChar errorMessage[RS_MAX_MESSAGE_BUF_SIZE];

	rsspecan_GetError(io, &primary, RS_MAX_MESSAGE_BUF_SIZE, elaboration);

	if (error != primary)
	{
		snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE, "Generated error code 0x%x is not equal GetError code 0x%x",
		         error, primary);
		AddTestFailEntry(line, errorMessage);
		result = VI_FALSE;
	}
	if (error != expError)
	{
		snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE, "Generated error code 0x%x != expected error 0x%x", error,
		         expError);
		AddTestFailEntry(line, errorMessage);
		result = VI_FALSE;
	}

	if (elabPattern != NULL && strlen(elabPattern) > 0)
	{
		ViChar token[RS_MAX_MESSAGE_BUF_SIZE];
		if (strstr(elabPattern, "@") != NULL)
		{
			int elementsCount = RsCore_GetElementsCount(elabPattern, '@');
			for (int i = 0; i < elementsCount; i++)
			{
				RsCore_GetTokenAtIndex(elabPattern, '@', i, token, RS_MAX_MESSAGE_BUF_SIZE, NULL);
				if (strstr(elaboration, token) == NULL)
				{
					snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE,
					         "Generated Elaboration does not correspond the required pattern. Required:\n%s\nvs actual:\n%s",
					         token, elaboration);
					AddTestFailEntry(line, errorMessage);
					result = VI_FALSE;
					break;
				}
			}
		}
		else
		{
			if (strstr(elaboration, elabPattern) == NULL)
			{
				snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE,
				         "Generated Elaboration does not correspond the required pattern. Required:\n%s\nvs actual:\n%s",
				         elabPattern, elaboration);
				AddTestFailEntry(line, errorMessage);
				result = VI_FALSE;
			}
		}
	}

	if (result == VI_TRUE)
	{
		snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE, "Error assertion code 0x%x and message", expError);
		AddTestPassEntry(line, errorMessage);
	}

	return result;
}

/******************************************************************************************************/

ViBoolean AddAssertionFailureErrorCode(ViSession io, ViInt32 line, ViStatus expError, ViStatus error,
                                       ViString elabPattern)
{
	ViBoolean result = VI_TRUE;
	ViChar errorMessage[RS_MAX_MESSAGE_BUF_SIZE];

	if (error != expError)
	{
		snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE, "Generated error code 0x%x != expected error 0x%x", error,
		         expError);
		AddTestFailEntry(line, errorMessage);
		result = VI_FALSE;
	}

	if (result == VI_TRUE)
	{
		snprintf(errorMessage, RS_MAX_MESSAGE_BUF_SIZE, "Error assertion code 0x%x", expError);
		AddTestPassEntry(line, errorMessage);
	}

	return result;
}

void TestMessage(ViString message)
{
	printf(message);
	AddToFileLog(message);
}

void InfoMessage(ViString message)
{
	ViChar fullMessage[RS_MAX_MESSAGE_BUF_SIZE];

	snprintf(fullMessage, RS_MAX_MESSAGE_BUF_SIZE, "\n         ------ %s\n", message);
	TestMessage(fullMessage);
}

/******************************************************************************************************/

/*  Mutex init
/******************************************************************************************************/

int MutexInit(MUTEX* mutex)
{
	*mutex = CreateMutex(0, FALSE, 0);
	return (*mutex == 0);
}

/******************************************************************************************************/

/*  Mutex locking
/******************************************************************************************************/

int MutexLock(MUTEX* mutex)
{
	return (WaitForSingleObject(*mutex, INFINITE) == WAIT_FAILED ? 1 : 0);
}

/******************************************************************************************************/

/*  Mutex unlocking
/******************************************************************************************************/

int MutexUnlock(MUTEX* mutex)
{
	return (ReleaseMutex(*mutex) == 0);
}

/******************************************************************************************************/
/*  Compares two numbers with a tolerance
	Returns 0 if they are equal
/******************************************************************************************************/
int CompareTwoViReal64Numbers(ViReal64 number1, ViReal64 number2, ViReal64 tolerance)
{
	if (fabs(number1 - number2) > tolerance)
		return 1;
	else
		return 0;
}

/******************************************************************************************************/
/*  Compares two arrays - precondition is the same sizes. Returns 0 if they are equal
    Returns number of different elements 
/******************************************************************************************************/
int CompareTwoViReal64Arrays(ViReal64* array1, ViReal64* array2, ViUInt32 size, ViReal64 tolerance)
{
	ViUInt32 idx;
	ViUInt32 wrongCounter = 0;

	for (idx = 0; idx < size; idx++)
	{
		if (fabs(array1[idx] - array2[idx]) > tolerance)
		{
			wrongCounter++;
		}
	}

	return wrongCounter;
}

/******************************************************************************************************/
/*  Compares two arrays - precondition is the same sizes. Returns 0 if they are equal
    Returns number of different elements
/******************************************************************************************************/
int CompareTwoViBooleanArrays(ViBoolean* array1, ViBoolean* array2, ViUInt32 size)
{
	ViUInt32 idx;
	ViUInt32 wrongCounter = 0;

	for (idx = 0; idx < size; idx++)
	{
		if (array1[idx] != array2[idx])
		{
			wrongCounter++;
		}
	}

	return wrongCounter;
}

/******************************************************************************************************/
/*  Compares two arrays - precondition is the same sizes. Returns 0 if they are equal
    Returns number of different elements
/******************************************************************************************************/
int CompareTwoViInt32Arrays(ViInt32* array1, ViInt32* array2, ViUInt32 size)
{
	ViUInt32 idx;
	ViUInt32 wrongCounter = 0;

	for (idx = 0; idx < size; idx++)
	{
		if (array1[idx] != array2[idx])
		{
			wrongCounter++;
		}
	}

	return wrongCounter;
}


/******************************************************************************************************/

/*  Compare two files - precondition is the same sizes. Returns 0 if they are equal
Return number of different elements
/******************************************************************************************************/

int CompareTwoFiles(ViString file1, ViString file2)
{
	FILE *fp1 = NULL, *fp2 = NULL;
	int result = 1;
	long long size1, size2;
	ViInt64 bytesRead1, bytesRead2, chunkSize, remaining, bytesToRead;
	ViByte *buffer1 = NULL, *buffer2 = NULL;
	chunkSize = 1000000;

	buffer1 = calloc(1, chunkSize);
	if (!buffer1)
		goto End;

	buffer2 = calloc(1, chunkSize);
	if (!buffer2)
		goto End;

	fp1 = fopen(file1, "rb");
	fp2 = fopen(file2, "rb");


	if (fp1 == NULL)
	{
		printf("Cannot open '%s' for reading ", file1);
		goto End;
	}
	else if (fp2 == NULL)
	{
		printf("Cannot open '%s' for reading ", file2);
		goto End;
	}
	else
	{
#if defined (_CVI_)
    long temp;
    
    (void)fseek(fp1, 0, SEEK_END);
    temp = ftell(fp1);
    (void)fseek(fp1, 0, SEEK_SET);
    size1 = (long long)temp;
    
    (void)fseek(fp2, 0, SEEK_END);
    temp = ftell(fp2);
    (void)fseek(fp2, 0, SEEK_SET);
    size2 = (long long)temp;
        
    

#elif defined(_MSC_VER)

		(void)fseek(fp1, 0, SEEK_END);
		size1 = (ViUInt64)_ftelli64(fp1);
		(void)fseek(fp1, 0, SEEK_SET);

		(void)fseek(fp2, 0, SEEK_END);
		size2 = (ViUInt64)_ftelli64(fp2);
		(void)fseek(fp2, 0, SEEK_SET);
#endif

		if (size1 != size2)
			goto End;

		remaining = size1;
		while (remaining > 0)
		{
			bytesToRead = chunkSize;
			if (remaining < chunkSize)
			{
				bytesToRead = remaining;
			}


			bytesRead1 = (ViInt64)fread(buffer1, sizeof(ViByte), (ViInt32)bytesToRead, fp1);
			bytesRead2 = (ViInt64)fread(buffer2, sizeof(ViByte), (ViInt32)bytesToRead, fp2);

			if (bytesRead1 != bytesRead2)
			{
				goto End;
			}

			if (memcmp(buffer1, buffer2, bytesRead1) != 0)
				goto End;

			remaining -= bytesRead1;
		}
		result = 0;
	}
End:
	if (fp1)
		fclose(fp1);

	if (fp2)
		fclose(fp2);

	if (buffer1)
		free(buffer1);

	if (buffer2)
		free(buffer2);

	return result;
}

/******************************************************************************************************

* Function: CoreTests_GetAttributeProperties
* Purpose:  Internal function to get the sessionProperties attribute index from attributeId
******************************************************************************************************/

ViStatus CoreTests_GetAttributeProperties(ViSession instrSession, ViAttr attributeId, RsCoreAttributePtr* attr)
{
	ViStatus error = VI_SUCCESS;
	RsCoreSessionPtr rsSession = NULL;
	ViInt32 idx;

	checkErr(RsCore_GetRsSession(instrSession, &rsSession));
	for (idx = 0; idx < (ViInt32)rsSession->attributesCount; idx++)
		if (rsSession->attributes[idx].constant == (ViInt32)attributeId)
			break;

	if (idx >= (ViInt32)rsSession->attributesCount)
		return RS_ERROR_INVALID_ATTRIBUTE;

	*attr = &(rsSession->attributes[idx]);

Error:
	return error;
}

void GenerateRandomString(ViChar* string, ViUInt32 bufferLen)
{
	ViUInt32 x, i;

	for (i = 0; i < bufferLen - 1; i++)
	{
		/* Note: islower returns only a b c..x y z, isdigit 0..9 and isprint only printable characters */
		do
		{
			x = rand() % 128 + 0; /* ASCII 0 to 127 */
		}
		while (!islower(x) && !isdigit(x));

		string[i] = (char)x;
	}

	string[bufferLen - 1] = '\0';
}

void GenerateRandomViReal64Array(ViReal64* inputArray, ViReal64 min, ViReal64 max, ViUInt32 length)
{
	ViUInt32 i;

	ViReal64 range = (max - min);
	ViReal64 div = RAND_MAX / range;

	for (i = 0; i < length; i++)
		inputArray[i] = min + (rand() / div);
}

void GenerateRandomViReal32Array(ViReal32* inputArray, ViReal32 min, ViReal32 max, ViUInt32 length)
{
	ViUInt32 i;

	ViReal32 range = (max - min);
	ViReal32 div = RAND_MAX / range;

	for (i = 0; i < length; i++)
		inputArray[i] = min + (rand() / div);
}

void GenerateRandomViInt32array(ViInt32* inputArray, ViInt32 min, ViInt32 max, ViUInt32 length)
{
	ViUInt32 i;

	ViReal64 range = (max - min) + 0.1;
	ViReal64 div = RAND_MAX / range;

	for (i = 0; i < length; i++)
		inputArray[i] = (ViInt32)(min + (rand() / div));
}
