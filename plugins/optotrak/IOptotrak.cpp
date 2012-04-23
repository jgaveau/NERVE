#include "IOptotrak.h"
#include "OptotrakWrapper.h"
#include <OpenThreads/Thread>
#include "nrvThread/NerveThread.h"
#include "nrvThread/NerveModule.h"
#include "nrvToolbox/TriBuf.h"
#include "ndtypes.h"
#include "ndpack.h"
#include "ndopto.h"

#define NUM_MARKERS 1

class RealtimeOptotrakModule : public NerveModule
{
public:
	RealtimeOptotrakModule():isInitialized(false){}
	void getData(RealtimeData& data)
	{
		data = buffer.getData();
	}
protected:
	void moduleOperation(NerveModuleUser*)
	{
		if(DataGetLatest3D(&uFrameNumber,&uElements,&uFlags,&pos3d) ==0)
		{
			
			current.x = pos3d.x;
			current.y = pos3d.y;
			current.z = pos3d.z;
			buffer.setData(current);
		}
		OpenThreads::Thread::microSleep(1000);

	}
	unsigned int uFrameNumber;
	unsigned int uElements;
	unsigned int uFlags;
	Position3d pos3d;
	RealtimeData current;
	TriBuf<RealtimeData> buffer;
	RealtimeData ref;
	bool isInitialized;
};
class IOptotrak_private
{
public:
	IOptotrak_private():realtime(0),initialized(false){}
	bool initOptotrak(const OptotrakParams& p)
	{
		printf("initOptotrak\n");
		int nNumSensors, nNumOdaus, nMarkers;
		char szNDErrorString[MAX_ERROR_STRING_LENGTH + 1];

		if(TransputerDetermineSystemCfg("logfile\0"))
		{
			printf("Failed in OptotrakModule::initOptotrak - TransputerDetermineSystemCfg\n");
			return false;
		}
		if(TransputerLoadSystem("system"))
		{
			printf("Failed in OptotrakModule::initOptotrak - TransputerLoadSystem\n");
			return false;
		}
		OpenThreads::Thread::microSleep(500000); //wait half a second for the system to load
		if(TransputerInitializeSystem(OPTO_LOG_ERRORS_FLAG))
		{
			printf("Failed in OptotrakModule::initOptotrak - TransputerInitialize\n");
			return false;
		}
		if(OptotrakLoadCameraParameters(const_cast<char*>( p.cameraParamFile.c_str() )))
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakLoadCameraParameters\n");
			return false;
		}

		//Request and receive the OPTOTRAK status.
		if(OptotrakGetStatus(
				&nNumSensors,    /* Number of sensors in the OPTOTRAK system. */
				&nNumOdaus,      /* Number of ODAUs in the OPTOTRAK system. */
				NULL,            /* Number of rigid bodies being tracked by the O/T. */
				&nMarkers,       /* Number of markers in the collection. */
				NULL,            /* Frequency that data frames are being collected. */
				NULL,            /* Marker firing frequency. */
				NULL,            /* Dynamic or Static Threshold value being used. */
				NULL,            /* Minimum gain code amplification being used. */
				NULL,            /* Stream mode indication for the data buffers */
				NULL,            /* Marker Duty Cycle being used. */
				NULL,            /* Voltage being used when turning on markers. */
				NULL,            /* Number of seconds data is being collected. */
				NULL,            /* Number of seconds data is being pre-triggered. */
				NULL ))         /* Configuration flags. */
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakGetStatus\n");
		}
		
	    
		if(OptotrakSetupCollection(
				NUM_MARKERS,    /* Number of markers in the collection. */
				(float)100.0,   /* Frequency to collect data frames at. */
				(float)2500.0,  /* Marker frequency for marker maximum on-time. */
				30,             /* Dynamic or Static Threshold value to use. */
				 160,            /* Minimum gain code amplification to use. */
				1,              /* Stream mode for the data buffers. */
				(float)0.35,    /* Marker Duty Cycle to use. */
				(float)7.0,     /* Voltage to use when turning on markers. */
				(float)20.0,     /* Number of seconds of data to collect. */
				(float)0.0,     /* Number of seconds to pre-trigger data by. */
				0))/*OPTOTRAK_BUFFER_RAW_FLAG | OPTOTRAK_GET_NEXT_FRAME_FLAG*/ 
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakSetupCollection\n");
			return false;
		}
				
		OpenThreads::Thread::microSleep(500000);
		
		if(OptotrakActivateMarkers())
		{
			printf("Failed in OptotrakModule::initOptotrak - OptotrakActivateMarkers\n");
			return false;
		}
		initialized = true;
		return true;
	}
	void initRealtime()
	{
		if(initialized==true && !thr.isRunning())
		{
			realtime = new RealtimeOptotrakModule();
			realtime->setOperateAction(NerveModule::DONT_REMOVE_MODULE);
			realtime->setRemoveAction(NerveModule::DELETE_MODULE);
			thr.addModule(*realtime);
			thr.start();
		}
	}
	void getRealtimeData(RealtimeData& data)
	{
		if(realtime == 0) return;
		realtime->getData(data);
	}
	~IOptotrak_private()
	{
		thr.cancel();
		while(thr.isRunning()) OpenThreads::Thread::microSleep(10);

		if(OptotrakDeActivateMarkers())
		{
			printf("Failed in IOptotrak::dtor - OptotrakDeActivateMarkers\n");
		}
	}
private:
	NerveThread thr;
	bool initialized;
	RealtimeOptotrakModule* realtime;
};
IOptotrak::IOptotrak(){d_=new IOptotrak_private();}
IOptotrak::~IOptotrak(){delete d_;}
bool IOptotrak::initOptotrak(const OptotrakParams &p){return d_->initOptotrak(p);}
void IOptotrak::initRealtime(){return d_->initRealtime();}
void IOptotrak::getRealtimeData(RealtimeData& r){return d_->getRealtimeData(r);}