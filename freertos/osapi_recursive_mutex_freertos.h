#ifndef OSAPI_RECURSIVE_MUTEX_FREERTOS_H
#define OSAPI_RECURSIVE_MUTEX_FREERTOS_H

#include "osapi.h"

class RecursiveMutex : public MutexInterface
{
	private:
		SemaphoreHandle_t xSemaphore;

	public:
		RecursiveMutex()
		{
			xSemaphore = xSemaphoreCreateRecursiveMutex();
		}

		virtual bool lock(unsigned int timeout)
		{
			if ( xSemaphore != NULL )
			{
				if( xSemaphoreTakeRecursive( xSemaphore, ( TickType_t ) timeout ) == pdTRUE )
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
			xSemaphoreGiveRecursive( xSemaphore );
		}
};

#endif