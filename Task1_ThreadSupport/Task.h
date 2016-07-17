#include <stdlib.h>


// Abstract class declaration: thread, scoped into the task namespace
// Usage would be | class derived : public task::thread

namespace task
{
	class thread
	{
	public: 
		virtual ~thread() {};
		
		// Param1: Pointer to the function to be threaded and function has a single argument
		// Param2: Pointer to argument of function
		virtual void run(void *(*routine)(void*), void * arg) = 0;
		
		// Checks if the thread can be joined
		virtual bool joinable() = 0;
		
		// Joins the thread if possible
		virtual void join() = 0;
	}; //class declaration

}// namespace delcaration