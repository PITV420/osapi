#ifndef OSAPI_THREAD_WINDOWS_H
#define OSAPI_THREAD_WINDOWS_H

#include "osapi.h"

/** Thread interface implementation for Windows. */
class Thread : public ThreadInterface
{
    private:
        HANDLE threadHandler;
        bool joinablE;
        const char* namE;
        unsigned int stackSizE;
        int prioritY;
        bool running;

    public:
        /** Thread constructor.
         *  @param[in] priority thread priority
         *  @param[in] stackSize thread stack size in bytes
         *  @param[in] isJoinable decides if the thread supports join operation or not
         *  @param[in] name optional thread name
         */
        Thread(int priority, unsigned int stackSize, Joinable isJoinable, const char* name = "unnamed")
        {
			namE = name;
			stackSizE = stackSize;
			prioritY = priority;
			running = false;

			joinablE = ( isJoinable == JOINABLE ) ? true : false;
			threadHandler = nullptr;
        }
        
        /** Virtual destructor required to properly destroy derived class objects. */
        virtual ~Thread()
        { 
        	CloseHandle(threadHandler);
        }
        
        /** Runs the thread.
         *  @retval true if the thread was started successfully, 
         *  @retval false if the thread was not started successfully, or the thread was already running
         */
        virtual bool run()
        { 
        	if (!running)
        	{
				threadHandler = CreateThread(NULL, stackSizE, threadFunction, (LPVOID)this, 0, NULL);
				if (threadHandler)
				{
					SetThreadPriority(threadHandler, prioritY);
					running = true;
					return true;
				}
        	}
            return false;
        }
        
        /** Checks if the thread is running.
         *  @retval true if the thread is running
         *  @retval false if the thread is not running
         */
        virtual bool isRunning()
        { 
        	return (threadHandler != nullptr) ? running : false;
        }   

        /** Waits for the thread to finish executing, with a given timeout.
         *  @param timeout[in] number of milliseconds to wait for the thread to finish executing
         *  @retval true if the thread was successfully joined in the given time
         *  @retval false if the thread was not joined within the given time or the thread is not joinable at all
         */
        virtual bool join(unsigned int timeout)
        {
        	if (joinablE)
        	{
        		if( WaitForSingleObject(threadHandler, timeout) == WAIT_OBJECT_0)
				{
        			running = false;
        			return true;
				}
        		return false;
        	}
        	return false;
        }

        /** Checks, if the thread is joinable.
         *  @retval true if the thread is joinable
         *  @retval false if the thread is not joinable
         */
        virtual bool isJoinable()
        {
        	return joinablE;
        }

        /** Suspends thread execution.
         *  @retval true if the thread was suspended successfully
         *  @retval false if the thread was not suspended for some reason
         */
        virtual bool suspend()
        {
        	if (running)
        	{
        		running = false;
        		return SuspendThread(threadHandler) != -1 ? true : false;
        	}
        	return false;
        }

        /** Resumes thread execution.
         *  @retval true if the thread was resumed successfully
         *  @retval false if the thread was not resumed for some reason
         */
        virtual bool resume()
        {
        	if (!running)
        	{
        		running = true;
        		return ResumeThread(threadHandler) != -1 ? true : false;
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
        	return SetThreadPriority(threadHandler, prioritY) != 0 ? true : false;
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
        static DWORD WINAPI threadFunction(LPVOID argument)
        {
        	Thread* osapiThreadObject = reinterpret_cast<Thread*>(argument);
        	if (osapiThreadObject)
        	{
        		osapiThreadObject->job();
        	}
        }

        /** Delays thread execution for a given time.
         *  @param time[in] number of milliseconds to delay thread execution
         */
        virtual void sleep(unsigned int time)
        {
        	Sleep(time);
        }

};

#endif /* OSAPI_THREAD_WINDOWS_H */
