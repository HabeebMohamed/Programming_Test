// Author: Habeeb Mohamed
// Task 1: Thread Support using POSIX threads

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <thread>
#include <chrono>

/* The namespace task, can be defined in a separate header file and be included here
 * with the appropriate compiler options e.g. gcc {..} -include task.h.
 * It has been redeclared here for simplicity sake of compiling only this file
 * and creating a binary out of it
 * #include "Task.h"
 */

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

// Global variable to declare the number of threads to be spawned
const int MAX_NUMBER_OF_THREADS = 3;


/* @brief   This class implements the "task" interface
 *          It implements the pure derived virtual methods: 
 *          run, joinable and join
 */
class POSIXThreads : public task::thread
{
    // Methods declaration
	public:
        // @brief      This API creates a number of threads defined by the constant NUMBER_OF_THREADS
        //             and stores the references in an internal container, threadList attribute
        // @paramIn    Routine: The task to be threaded
        // @paramIn    Arg: The argument to pass to the threaded task
        // @paramOut   ThreadList: List of POSIX threads created and references stored in the container
        // @returns    None
		void run(void *(*routine) (void *), void * arg)
		{
            // Spawn a bunch of threads with joinable attributes
            for(int i=0; i<MAX_NUMBER_OF_THREADS; i++)
            {
            	pthread_t thread;
            	pthread_attr_t attr;
            	pthread_attr_init(&attr);
            	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
			
            	int iRet = pthread_create(&thread, &attr, routine, arg);
            	if(!iRet)
            	{
                	threadList.push_back(thread);                    
            	}
            }
		};
		
        // @brief      This API checks if the current thread is joinable i.e. 
        //             its task is finished and it's not in a detachable state
        // @paramIn    None
        // @paramOut   Boolean
        // @returns    True if thread is joinable, false otherwise        
		bool joinable()
		{
            int resultCode = pthread_detach(getCurrentThread());            
            return (resultCode == 0) ? true : false;
		};
		
        // @brief      This API joins the thread with the main thread
        // @paramIn    None
        // @paramOut   Worker thread is joined with the main thread
        // @returns    None
		void join()
		{
            pthread_join(getCurrentThread(), NULL);
            std::cout << "Thread " << currThreadCount << " joined" << std::endl;
		};
        
        // @brief      This API returns the reference to the current thread executing/finished indicated by the attribute, currThreadCount 
        // @paramIn    None
        // @paramOut   Pthread: Reference to the current thread
        // @returns    Reference to the current thread
        pthread_t getCurrentThread()
        {
            return threadList[currThreadCount];
        }
        
        // @brief      This API sets the attribute, currThreadCount
        // @paramIn    pCount: Thread count
        // @paramOut   currThreadCount: Assigned with the value of pCount
        // @returns    None
        void setCurrentThreadCount(int pCount)
        {
            currThreadCount = pCount;
        }
	
	// Variables Declaration        
    public:
        // To keep track of current thread count (this makes accessing operations 0(1))
        int currThreadCount = 0;
        // Container to store the thread references
        std::vector<pthread_t> threadList;
};


// @brief      This API will be threaded and it will simply print a message
// @paramIn    ptr: Pointer to the message to be printed
// @paramOut   None
// @returns    None
void* printMessage(void *ptr)
{
    char *message;
    message = (char*) ptr;
    std::cout << "Printing message in running thread: " << message << std::endl;
}

// @brief      This API is the entry point for the POSIX threads implementation
//             It will instantiate a POSIXThread object, call the run() API which
//             will spawn the worker threads. These worker threads will be joined
//             to the main thread once they are done executing the tasks and are
//             not detached
// @paramIn    None
// @paramOut   Thread output
// @returns    Safe exit of program
int main()
{    
    POSIXThreads thread;
    std::string message = "I am running";
    thread.run(&printMessage, (void*)message.c_str());
    //Join all threads
    for(int i=0; i<MAX_NUMBER_OF_THREADS; i++)
    {
        thread.setCurrentThreadCount(i);
        if(thread.joinable())
        {
            std::cout << "Thread " << i << " joinable " << std::endl;
            thread.join();
        }
        else
        {
            std::cout << "Thread " << i << " not joinable " << std::endl;
        }
    }
    return 0;
}