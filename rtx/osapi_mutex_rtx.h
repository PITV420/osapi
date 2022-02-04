#ifndef OSAPI_MUTEX_RTX_H
#define OSAPI_MUTEX_RTX_H

#include "osapi.h"

class Mutex : public MutexInterface
{
private:
	osMutexAttr_t Thread_Mutex_attr;
	osMutexId_t mutex_id;
	osStatus_t status;

public:
	Mutex()
	{
			Thread_Mutex_attr.attr_bits = osMutexRobust;
	}
	
	virtual ~Mutex()
	{
		if (mutex_id) osMutexDelete(mutex_id);
	}
	
	virtual bool lock(unsigned int timeout)
	{
		int32_t state = osKernelLock();
		if (mutex_id == nullptr)
		{
			mutex_id = osMutexNew(&Thread_Mutex_attr);
		}
		osKernelRestoreLock( state );
		status = osMutexAcquire ( mutex_id, timeout);
		if ( status == osOK )
		{
			return true;
		}
		return false;
	}
	
	virtual void unlock()
	{
		if (mutex_id != nullptr) status = osMutexRelease( mutex_id );
	}

};

#endif // OSAPI_MUTEX_RTX_H
