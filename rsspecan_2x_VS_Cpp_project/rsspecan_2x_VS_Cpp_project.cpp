//==============================================================================
// Include files

#include "rsspecan.h"
#include "TestFile_DriverFunctions.h"
#include <time.h>

//==============================================================================
// Constants
//==============================================================================

#define IDN_STRING_LEN 34

int main(void)
{
	ViStatus error = VI_SUCCESS;
	ViSession io = 0;

	//ViChar *optionString = (ViChar *)"Simulate =   0,QueryInstrStatus = True, ReadDelay=20, WriteDelay=,Janik='aaa',\"=sd";
	ViChar *optionString1 = (ViChar *)"Simulate =   1,QueryInstrStatus = True, ReadDelay=0, WriteDelay=,Janik='aaa',\"=sd, SendOpcQueryInCheckStatus=VI_FALSE";
	ViChar *optionString2 = (ViChar *)"Simulate =   0, ReuseSession=0, ReadDelay=0, WriteDelay=,Janik='aaa',\"=sd, SendOpcQueryInCheckStatus=VI_FALSE";
	ViChar *optionString3 = (ViChar *)"Simulate =   0, ReuseSession=0, OPCwaitMode = ServiceRequest";
	ViChar *optionString4 = (ViChar *)"Simulate =   0, ReuseSession=0, OPCwaitMode = OPCquery";
	ViChar *optionString5 = (ViChar *)"Simulate =   0, ReuseSession=0, OPCwaitMode = ServiceRequest, IOsegmentSize=499, AutoSystemErrorQuery = 1";

	ViChar  *resourceName = (ViChar *)"TCPIP::localhost::INSTR";

	error = Test_DriverFunctions(resourceName, optionString2);
	//error = Test_MultithreadSafety(resourceName, optionString1);
	//error = Test_MeasureInLoop(resourceName, optionString4);
	//error = Test_Attributes(resourceName, optionString2);
	//error = Test_RangeTables(resourceName, optionString1);

	printf("\n\n\nPress ENTER to continue...");
	(void)getchar();

}