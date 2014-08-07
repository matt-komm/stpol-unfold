#ifndef __LOGGING_H__
#define __LOGGING_H__

enum LOGLEVEL
{
    DEBUG=0,
    INFO=1,
    WARNING=2,
    ERROR=3,
    CRITICAL=4,
    NONE=5
};

std::string toStr(LOGLEVEL level)
{
    static std::string debug("DEBUG");
    static std::string info("INFO");
    static std::string warning("WARNING");
    static std::string error("ERROR");
    static std::string critical("CRITICAL");
    switch (level)
    {
        case DEBUG:
            return debug;
        case INFO:
            return info;
        case WARNING:
            return warning;
        case ERROR:
            return error;
        case CRITICAL:
            return critical;
        case NONE:
            return "";
    }
    return "";
    
}

static LOGLEVEL LogLevel = WARNING;

template<class...ARGS> void log(LOGLEVEL level, const std::string& str, ARGS... args)
{
    if (level>=LogLevel)
    {
        if (str.substr(0,5) ==" ... ")
        {
            printf(str.c_str(),args...);
        }
        else
        {
            printf((toStr(level)+": "+str).c_str(),args...);
        }
    }
}
void log(LOGLEVEL level, const std::string& str)
{
    if (level>=LogLevel)
    {
        if (str.substr(0,4) ==" ... ")
        {
            printf("%s",str.c_str());
        }
        else
        {
            printf("%s",(toStr(level)+": "+str).c_str());
        }
    }
}

#endif

