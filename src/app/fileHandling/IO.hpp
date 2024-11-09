//it means it will only compile once whenever it will be called
#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <string>

class IO {

    public:
        IO(const std::string &file_path);
        ~IO();
        std::fstream getFileStream();

    private:
        std::fstream file_stream;
};

#endif

