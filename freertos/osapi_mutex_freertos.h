#ifndef OSAPI_MUTEX_FREERTOS_H
#define OSAPI_MUTEX_FREERTOS_H

#include "osapi.h"

class Mutex : public MutexInterface
{
	private:
		SemaphoreHandle_t xSemaphore;

	public:
		Mutex()
		{
			xSemaphore = xSemaphoreCreateMutex();
		}
		
		virtual bool lock(unsigned int timeout)
		{
			if ( xSemaphore != NULL )
			{
				if( xSemaphoreTake( xSemaphore, ( TickType_t ) timeout ) == pdTRUE )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			return false;
		}
		
		virtual void unlock()
		{
			xSemaphoreGive( xSemaphore );
		}

};

#endif