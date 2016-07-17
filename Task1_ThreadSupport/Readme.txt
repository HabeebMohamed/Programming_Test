IMPORTANT NOTE:

The task given has been implemented without the use of mutex or semaphores. 
The objective was to demonstrate the understanding of threading and their concepts.
This has been demonstrated in the implementation. Mutex and semaphores can be used
when concurrent access to resources are required in a safe manner. The usage program
has been kept simple enough to join worker threads to the main thread once execution 
completes.

The current program prints to console output when the thread is running with a simple
message: I am running. Also when a thread is deemed joinable to the main thread, a message
is logged saying it is joinable and joined to the main thread. Thread IDs have not been used
for this purpose though.

The library, Task.h, can be included in the source files to eliminate the need for redeclaration. This also requires the file of concern to be compiled with appropriate compiler options gcc {..} 
-include Task.h etc...
