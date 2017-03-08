#include "Logger.h"

#ifdef DEBUG

#include <ctime>
#include <cstdio>
#include <iostream>
#include <algorithm>

// Initialize static class
Logger Logger::logger = Logger();


Logger::Logger()
{
    auto time = std::time(NULL);
    std::string date = std::asctime(std::localtime(&time));
    date.pop_back();

    #ifdef STDLOG_MODE
    std::string extension = ".log";
    #elif defined( HTMLLOG_MODE )
    std::string extension = ".html";
    #endif

    std::string path = std::string("logs/") + date + extension;
    // Change date format to be able to work with the Windows dir system.
    std::replace(path.begin(), path.end(), ' ', '_');
    std::replace(path.begin(), path.end(), ':', '-');

    freopen(path.c_str(), "w", stderr); // change to log

    std::cerr << "<h1>Date: " << date << "</h1>" << std::endl;
    std::cerr << "<h1>Path: " << path << "</h1>" << std::endl;
    NEW_LINE
}

Logger::~Logger()
{
    fclose(stderr);
}

#endif // DEBUG
