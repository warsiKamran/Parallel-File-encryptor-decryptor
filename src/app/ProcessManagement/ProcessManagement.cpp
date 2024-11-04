#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    taskQueue.push(std::move(task));
    return true;
}

//consuming tasks from the queue refer the diag.
void ProcessManagement::executeTasks() {

    while (!taskQueue.empty()){

        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();

        std::cout << "Executing task: " << taskToExecute->toString() << std::endl;

        //takes serialize input and then deserializes it
        executeCryption(taskToExecute->toString());
    }
}

