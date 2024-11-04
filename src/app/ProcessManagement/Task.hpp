#ifndef TASK_HPP
#define TASK_HPP

#include "../fileHandling/IO.hpp"
#include <fstream>
#include <string>
#include <sstream>

// this whole file is a type of datatype that we are creating for storing things and then pushing it into the queue
enum class Action {
    ENCRYPT,
    DECRYPT
};

struct Task {

    std::string filePath;
    std::fstream f_stream;
    Action action;

    //&& -> copy will not be created instead a temp will be created which will then be moved, type of temp buffer
    Task(std::fstream&& stream, Action act, std::string filePath) : f_stream(std::move(stream)), action(act), filePath(filePath) {}

    //we are doing this to pass the task from process management to child process using serialization and de-serialization
    std::string toString() const {

        //output stringstream
        std::ostringstream oss;
        oss << filePath << "," << (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");

        //test.txt -> "test.txt,ENCRYPT"
       //just making the string of file path and the action
       //This serialization makes it easier to pass Task objects between processes
        return oss.str();
    }

    //de-serialize
    static Task fromString(const std::string& taskData) {

        std::istringstream iss(taskData);
        std::string filePath;
        std::string actionStr;

        if (std::getline(iss, filePath, ',') && std::getline(iss, actionStr)) {

            Action action = (actionStr == "ENCRYPT") ? Action::ENCRYPT : Action::DECRYPT;

            //move -> don't make the copy but copy the ownership to the above f_stream var;
            IO io(filePath);
            std::fstream f_stream = std::move(io.getFileStream());

            if (f_stream.is_open()) {
                return Task(std::move(f_stream), action, filePath);
            } 
            
            else {
                throw std::runtime_error("Failed to open file: " + filePath);
            }
        } 
        
        else {
            throw std::runtime_error("Invalid task data format");
        }
    }
};

#endif

