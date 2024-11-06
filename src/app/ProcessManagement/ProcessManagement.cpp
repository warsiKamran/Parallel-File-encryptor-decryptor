#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"
#include<sys/mman.h>
#include<atomic>
#include<fcntl.h>

ProcessManagement::ProcessManagement(){

    // shm_open is used to create or open shared memory with the name /my_queue.
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); 

    //sets the size of the shared memory to the size of the SharedMemory struct.
    ftruncate(shmFd, sizeof(SharedMemory));

    //whatever memory is opened through shmFd , using that make a pointer of the SharedMem using mmap files
    sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMem->front = 0;
    sharedMem->rear = 0; 
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {

    if(sharedMem->size.load() >= 1000){
        return false;
    }

    // it copies the serialized task string into sharedMem->tasks at the rear index.
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());

    // it then moves rear to the next position (wrapping around using % 1000 to make it circular) and increments size.
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);

    int pid = fork();

    if(pid < 0){
        return false;
    }

    else if(pid > 0){
        std::cout<<"Entering parent process"<<std::endl;
    }

    else{
        std::cout<<"Entering child process"<<std::endl;
        executeTasks();
        std::cout<<"Exiting the child process"<<std::endl;
    }

    return true;
}

//consuming tasks from the queue and then making child process refer the diag.

// The child copies the task string from the front index of sharedMem->tasks, moves front to the next position, and decrements size.
void ProcessManagement::executeTasks() {

    char taskStr[256];

    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);

    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    //takes serialize input and then deserializes it
    executeCryption(taskStr);
    
}

