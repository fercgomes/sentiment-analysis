#include <iostream>
#include <fstream>

class Logger
{

public:
    Logger();
    ~Logger();

    void AddLog(std::string str);

private:
    std::string path;
    std::string GetFileName();

};