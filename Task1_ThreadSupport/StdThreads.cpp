// Author: Habeeb Mohamed
// Task 1: Thread Support using std::thread

#include <stdio.h>
#include <vector>
#include <string.h>
#include <thread>
#include <iostream>
#include <chrono>

/* The namespace task, can be defined in a separate header file and be included here
 * with the appropriate compiler options e.g. gcc {..} -include task.h.
 * It has been redeclared here for simplicity sake of compiling only this file
 * and creating an executable or binary out of it
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
class StdThreads : public task::thread
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
            // Create a list of threads to run and store it in a vector
            for(int i=0; i<MAX_NUMBER_OF_THREADS; i++)
            {
                threadList.push_back(std::thread(routine, arg));
                //Delay some time
                std::this_thread::sleep_for(std::chrono::seconds(5));
            }
        };
        
        // @brief      This API checks if the current thread is joinable i.e. 
        //             its task is finished and it's not in a detachable state
        // @paramIn    None
        // @paramOut   Boolean
        // @returns    True if thread is joinable, false otherwise          
        bool joinable()
        {               
            return threadList[currThreadCount].joinable();
        };
        
        // @brief      This API joins the thread with the main thread
        // @paramIn    None
        // @paramOut   Worker thread is joined with the main thread
        // @returns    None        
        void join()
        {
            threadList[currThreadCount].join();
            std::cout << "Thread " << currThreadCount << " joined" << std::endl;
        };
        
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
        int currThreadCount  = 0;
        // Container to store the thread references
        std::vector<std::thread> threadList;
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

// @brief      This API is the entry point for the (std::thread) threads implementation
//             It will instantiate a StdThreads object, call the run() API which
//             will spawn the worker threads. These worker threads will be joined
//             to the main thread once they are done executing the tasks and are
//             not detached
// @paramIn    None
// @paramOut   Thread output
// @returns    Safe exit of program
int main()
{    
    StdThreads thread;
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
