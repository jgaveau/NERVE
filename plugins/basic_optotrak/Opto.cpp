// Opto.cpp : Defines the entry point for the console application.
//

#include "ndtypes.h"
#include "ndpack.h"
#include "ndhost.h"
#include "ndopto.h"
#include "windows.h"
#include "stdio.h"

void printError(int result){if(result){char buffer[1024] = {0};OptotrakGetErrorString(buffer,1024);printf("Error %i: %s\n",result,buffer);}}
void testOptotrak()
{
	printf("Optotrak test\n");
	
	int result;
	result = TransputerDetermineSystemCfg("logfile\0");
	printError(result);
	result = TransputerLoadSystem("system");
	printError(result);
	printf("TransputerLoadSystem completed\n");
	Sleep(1000);
	result = TransputerInitializeSystem(OPTO_LOG_ERRORS_FLAG);
	printError(result);
	//result = OptotrakLoadCameraParameters("standard.cam");
	//printError(result);

	printf("TransputerInitializeSystem completed\n");
	int nNumSensors;
	int nNumOdaus;
	int nMarkers;
	result = OptotrakGetStatus(&nNumSensors,&nNumOdaus,0,&nMarkers,0,0,0,0,0,0,0,0,0,0);
	printError(result);
	printf("Num Sensors: %i\n",nNumSensors);
	printf("Num Odaus  : %i\n",nNumOdaus);
	printf("Num Markers: %i\n",nMarkers);
	result = OptotrakSetupCollection(1,100.f,2500.f,30,160,1,.35f,7.f,20.f,0.f,0);
	printError(result);
	Sleep(1000);
	result = OptotrakActivateMarkers();
	printError(result);
}

