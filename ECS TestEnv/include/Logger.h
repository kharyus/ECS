#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

// Define logging mode, or if not on debug replace LOG to nothing.

// TODO: Remove the STDLOG_MODE and HTMLLOG_MODE and make different classes derived from Logger, and the logger singleton assume one of their forms.
#ifdef STDLOG_MODE

#define LOG(x) std::cerr << x;
#define LOG_LINE(x) std::cerr << x << std::endl;
#define NEW_LINE std::endl;

#elif defined( HTMLLOG_MODE )

#define LOG(x) std::cerr << x;
#define LOG_LINE(x) std::cerr << "<p>" << x << "</p>" << std::endl;
#define NEW_LINE std::cerr << "<br/>" << std::endl;

#elif !defined(DEBUG)

#define LOG(x) ;
#define LOG_LINE(x) ;
#define NEW_LINE ;

#endif //DEBUG



// Only create logger if on DEBUG mode.

#ifdef DEBUG

/** Utility Class for logging.
  * If built on release mode write and writeline do nothing. */
class Logger
{
    public:

    protected:

    private:
        Logger();
        virtual ~Logger();

        static Logger logger;
};

#endif // DEBUG

#endif // LOGGER_H
