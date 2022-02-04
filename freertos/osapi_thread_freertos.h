#ifndef OSAPI_THREAD_FREERTOS_H
#define OSAPI_THREAD_FREERTOS_H

#include "osapi.h"
// #include <stdio.h>

/** Thread interface implementation for FreeRTOS. */
class Thread : public ThreadInterface
{
	private:
    int prioritY;
    unsigned int stackSizE;
    Joinable joinChecK;
    const char* namE;
    TaskHandle_t pxCreatedTask;
    SemaphoreHandle_t xSemaphore;

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
      joinChecK = isJoinable;
      namE = name;
      stackSizE = stackSize;
      pxCreatedTask = NULL;
      xSemaphore = xSemaphoreCreateBinary();
    }
    
    /** Virtual destructor required to properly destroy derived class objects. */
    virtual ~Thread()
    { 
      vSemaphoreDelete( xSemaphore );
    }
    
    /** Runs the thread.
    *  @retval true if the thread was started successfully, 
    *  @retval false if the thread was not started successfully, or the thread was already running
    */				
    virtual bool run() 
    {
      return xTaskCreate(threadFunction, namE, stackSizE, this, prioritY, &pxCreatedTask); == pdPASS ? true : false;
    }
    
    /** Checks if the thread is running.
    *  @retval true if the thread is running
    *  @retval false if the thread is not running
    */
    virtual bool isRunning()
    {
      if (pxCreatedTask)
      {	
        eTaskState state = eTaskGetState(pxCreatedTask);
        if (state == eReady || state == eRunning || state == eBlocked)
        {
          return true;
        }
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
      if ( joinChecK == JOINABLE )
      {
        return xSemaphoreTake( xSemaphore, pdMS_TO_TICKS( timeout )) == pdTRUE ? true : false;
      }
      return false;
    }

    /** Checks, if the thread is joinable.
     *  @retval true if the thread is joinable
     *  @retval false if the thread is not joinable
     */
    virtual bool isJoinable()
    {
      if ( joinChecK == JOINABLE )
      {
        return true;
      }
      return false;           
    }

    /** Suspends thread execution.
     *  @retval true if the thread was suspended successfully
     *  @retval false if the thread was not suspended for some reason
     */
    virtual bool suspend()
    {
      vTaskSuspend( pxCreatedTask );
      eTaskState state = eTaskGetState(pxCreatedTask);
      return state ==  eSuspended ? true : false;
    }           

    /** Resumes thread execution.
     *  @retval true if the thread was resumed successfully
     *  @retval false if the thread was not resumed for some reason
     */
    virtual bool resume()
    {
      vTaskResume( pxCreatedTask );
      eTaskState state = eTaskGetState(pxCreatedTask);
      if ( state == eReady || state == eRunning || state == eBlocked )
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
      vTaskPrioritySet( pxCreatedTask, prioritY );
      if ( prioritY == priority )
      {
        return true;
      }
      return false;        
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
      return name_;
    }
  
  protected:
    static void threadFunction(void* argument)
    {
      Thread* osapiThreadObject = reinterpret_cast<Thread*>(argument);
      if (osapiThreadObject) {
        if ( osapiThreadObject->joinChecK == JOINABLE){
          xSemaphoreTake( osapiThreadObject->xSemaphore, ( TickType_t ) 0 );
        }

        osapiThreadObject->job();

        if ( osapiThreadObject->joinChecK == JOINABLE) {
          xSemaphoreGive( osapiThreadObject->xSemaphore );
        }
      }

      vTaskDelete(NULL); // this is required in FreeRTOS to make sure that the thread does not just simply return
    }

    /** Delays thread execution for a given time.
     *  @param time[in] number of milliseconds to delay thread execution
     */
    virtual void sleep(unsigned int time)
    {
      vTaskDelay( (TickType_t)time );
    }
    
};


#endif // OSAPI_THREAD_FREERTOS_H
