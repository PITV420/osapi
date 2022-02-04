#ifndef OSAPI_MUTEX_WINDOWS_H
#define OSAPI_MUTEX_WINDOWS_H


class Mutex : public MutexInterface
{
	private:
		HANDLE mutex;

	public:
		Mutex()
		{
			mutex = CreateSemaphore(NULL, 1, 1, NULL);
		}
		
		virtual ~Mutex()
		{
			if (mutex != nullptr) CloseHandle(mutex);
		}

		virtual bool lock(unsigned int timeout)
		{
			if(mutex != nullptr)
			{
				return WaitForSingleObject(mutex, timeout) == WAIT_OBJECT_0 ? true : false;
			}
			return false;
		}
		
		virtual void unlock()
		{
			if(mutex != nullptr)
			{
				ReleaseSemaphore(mutex, 1, NULL);
			}
		}

};

#endif // OSAPI_MUTEX_WINDOWS_H
