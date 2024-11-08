#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"
#include <sys/mman.h>
#include <atomic>
#include <semaphore.h>
#include <thread>


ProcessManagement::ProcessManagement(){

    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);
    
    if(itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED){
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    // shm_open is used to create or open shared memory with the name /my_queue.
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    if (shmFd == -1){
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    
    //sets the size of the shared memory to the size of the SharedMemory struct.
    ftruncate(shmFd, sizeof(SharedMemory));

    //whatever memory is opened through shmFd , using that make a pointer of the SharedMem using mmap files
    sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

ProcessManagement::~ProcessManagement(){

    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);

    sem_close(itemsSemaphore);
    sem_close(emptySlotsSemaphore);
    sem_unlink("/items_semaphore");
    sem_unlink("/empty_slots_semaphore");
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task){

    sem_wait(emptySlotsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    if (sharedMem->size.load() >= 1000) {
        return false;
    }

    // it copies the serialized task string into sharedMem->tasks at the rear index.
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());

    // it then moves rear to the next position (wrapping around using % 1000 to make it circular) and increments size.
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);

    lock.unlock();
    sem_post(itemsSemaphore);

    std::thread thread_1(&ProcessManagement::executeTask, this);
    thread_1.detach();
    
    return true;
}

//consuming tasks from the queue and then making child process refer the diag.
// The child copies the task string from the front index of sharedMem->tasks, moves front to the next position, and decrements size.
void ProcessManagement::executeTask(){

    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queueLock);

    char taskStr[256];

    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    sem_post(emptySlotsSemaphore);
    executeCryption(taskStr);

    lock.unlock();
}

