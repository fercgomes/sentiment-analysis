#include "Logger.h"
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

Logger::Logger()
{
    path = GetFileName();
}

Logger::~Logger()
{}

void Logger::AddLog(std::string str)
{
    std::ofstream fp;
    fp.open(path, std::ofstream::out | std::ofstream::app);

    /* get current time and date */
    time_t t = time(NULL);
    tm* tPtr = localtime(&t);

    /* builds the output file name */
    std::stringstream ss;
    ss << tPtr->tm_hour << ":" << tPtr->tm_min << ":" << tPtr->tm_sec;

    fp << ss.str() << " - " << str << std::endl;

    fp.close();
}
std::string Logger::GetFileName()
{
    /* get current time and date */
    time_t t = time(NULL);
    tm* tPtr = localtime(&t);

    /* builds the output file name */
    std::stringstream ss;
    ss << std::setfill('0')
        << std::setw(2) << tPtr->tm_hour
        << std::setw(2) << tPtr->tm_min
        << std::setw(2) << tPtr->tm_sec
        << "-"
        << std::setw(2) << tPtr->tm_mday
        << std::setw(2) << tPtr->tm_mon + 1
        << std::setw(4) << tPtr->tm_year + 1900
        << "_log";
    
    return ss.str();
}