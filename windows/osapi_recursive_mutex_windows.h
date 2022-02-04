#ifndef OSAPI_RECURSIVE_MUTEX_WINDOWS_H
#define OSAPI_RECURSIVE_MUTEX_WINDOWS_H

class RecursiveMutex : public MutexInterface
{
	private:
		HANDLE mutex;

	public:
		RecursiveMutex()
		{
			mutex = CreateMutex(NULL, TRUE, NULL);
		}

		virtual ~RecursiveMutex()
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
				ReleaseMutex(mutex);
			}
		}

};

#endif // OSAPI_RECURSIVE_MUTEX_WINDOWS_H
