#ifndef OSAPI_THREAD_RTX_H
#define OSAPI_THREAD_RTX_H

#include "osapi.h"
/** Thread interface implementation for RTX. */

class Thread : public ThreadInterface
{
	private:
			int prioritY;
			const char* namE;
			unsigned int stackSizE;
			Joinable joinablE;
			osThreadId_t thread1_id;
			osThreadAttr_t threadAttr_thread1;
			osStatus_t status;
			osThreadState_t state;
			osSemaphoreId_t sid_Semaphore;

  public:
      /** Thread constructor.
        *  @param[in] priority thread priority
        *  @param[in] stackSize thread stack size in bytes
        *  @param[in] isJoinable decides if the thread supports join operation or not
        *  @param[in] name optional thread name
        */
      Thread(int priority, unsigned int stackSize, Joinable isJoinable, const char* name = "unnamed")
      {
        prioritY = priority;
        namE = name;
        stackSizE = stackSize;
        joinablE = isJoinable;
        sid_Semaphore = osSemaphoreNew(1U, 0U, NULL);
      }

      /** Virtual destructor required to properly destroy derived class objects. */
      virtual ~Thread()
      { 
        osSemaphoreDelete(sid_Semaphore);
      }
      
      /** Runs the thread.
      *  @retval true if the thread was started successfully, 
      *  @retval false if the thread was not started successfully, or the thread was already running
      */
      virtual bool run()
      {
        threadAttr_thread1 = {
          .name = namE,
          .stack_size = stackSizE,
          .priority = (osPriority_t) prioritY
        };

        if (joinablE == JOINABLE)
        {
          threadAttr_thread1.attr_bits = osThreadJoinable;
        }
        else
        {
          threadAttr_thread1.attr_bits = osThreadDetached;
        }

        thread1_id = osThreadNew(threadFunction, this, &threadAttr_thread1);
        
        return thread1_id ? true : false;
      }

      /** Checks if the thread is running.
      *  @retval true if the thread is running
      *  @retval false if the thread is not running
      */
      virtual bool isRunning()
      {
        state = osThreadGetState(thread1_id);
        if ( state == osThreadReady  || state == osThreadRunning || state == osThreadBlocked )
        {
          return true;
        }
        return false;
      }
      
      /** Waits for the thread to finish executing, with a given timeout.
        *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
        *  @retval true if the thread was successfully joined in the given time
        *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
        */
      virtual bool join(unsigned int timeout)
      {
        if (joinablE == JOINABLE)
        {
          return osOK == osSemaphoreAcquire(sid_Semaphore, timeout) ? true : false;
        }
        return false;
      }

      /** Checks, if the thread is joinable.
        *  @retval true if the thread is joinable
        *  @retval false if the thread is not joinable
        */
      virtual bool isJoinable()
      {
        return joinablE == JOINABLE ? true : false;
      }

      /** Suspends thread execution.
        *  @retval true if the thread was suspended successfully
        *  @retval false if the thread was not suspended for some reason
        */
      virtual bool suspend()
      {
        status = osThreadSuspend(	thread1_id );
        if ( status == osOK )
        {
          return true;
        }
        return false;
      }           

      /** Resumes thread execution.
        *  @retval true if the thread was resumed successfully
        *  @retval false if the thread was not resumed for some reason
        */
      virtual bool resume()
      {
        status = osThreadResume (	thread1_id	);
        if ( status == osOK )
        {
          return true;
        }
        return false;
      }           

      /** Sets thread priority
        *  @param[in] priority new thread priority
        *  @retval true if the priority for the thread was set successfully
        *  @retval false if the priority for the thread was not set successfully for some reason
        */
      virtual bool setPriority(int priority)
      {
        prioritY = priority;
        status = osThreadSetPriority(thread1_id, (osPriority_t) priority);
        return osOK == status ? true : false;
      }

      /** Gets the thread priority
        *  @return current thread priority
        */
      virtual int getPriority()
      {
        return prioritY;
      }
      
      /** Gets thread name
        *  @return name of the thread
        */
      virtual const char* getName()
      {
        return namE;
      }           
  
  protected:
      /** Delays thread execution for a given time.
        *  @param time[in] number of milliseconds to delay thread execution
        */
      void static threadFunction(void* argument) 
      {
        Thread* osapiThreadObject = reinterpret_cast<Thread*>(argument);
        if (osapiThreadObject) 
        {	
          osapiThreadObject->job();
          if ( osapiThreadObject->joinablE == JOINABLE )
          {
            osSemaphoreRelease(osapiThreadObject->sid_Semaphore);
          }
        }				
        osThreadExit();
      }
  
      virtual void sleep(unsigned int time) {
        osDelay(time);
      }

};

#endif // OSAPI_THREAD_RTX_H
