#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <memory>
#include <mutex>
#include <atomic>
#include <semaphore.h>

class ProcessManagement{

    sem_t* itemsSemaphore;
    sem_t* emptySlotsSemaphore;

public:

    ProcessManagement();
    ~ProcessManagement();
    bool submitToQueue(std::unique_ptr<Task> task);
    void executeTask();

private:

    // size is an atomic variable, which is crucial for safe access and modification in a concurrent, multi-threaded, or multi-process environment.
    //1000 files can be stored each containing 256 bytes
    struct SharedMemory {

        std::atomic<int> size;
        char tasks[1000][256];
        int front;
        int rear;
    };

    SharedMemory* sharedMem;
    int shmFd;
    const char* SHM_NAME = "/my_queue";
    std::mutex queueLock;
};

#endif

// In a queue, size could represent the current number of tasks in tasks. As different processes or threads enqueue (add to rear) or dequeue (remove from front), size would increment or decrement. Using std::atomic<int> here ensures that size always reflects the accurate number of tasks, even if multiple threads or processes access it simultaneously.

