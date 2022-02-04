#ifndef OSAPI_MORTAL_THREAD_H
#define OSAPI_MORTAL_THREAD_H

class MortalThread : public Thread
{
	private:
		sig_atomic_t killSignal = 0;
		
		/** Implementation of the job method */
		virtual void job()
		{
			killSignal = 0;
			begin();
			while ( killSignal == 0 )
			{
				loop();
			}
			end();
		}
	
	public:
    MortalThread(int priority, unsigned int stackSize, const char* name = "unnamed") : Thread(priority, stackSize, JOINABLE, name)
	{
		killSignal = 0;
	}

    virtual ~MortalThread() {}

    /** Sends termination signal to the thread. */
    void kill()
	{
		killSignal = 1;
    }

	protected:
		virtual void begin(void) = 0;
		virtual void loop(void) = 0;
		virtual void end(void) = 0;

};

#endif // OSAPI_MORTAL_THREAD_H
