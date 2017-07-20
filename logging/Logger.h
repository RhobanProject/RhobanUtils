#ifndef _RHOBAN_LOGGER_H
#define _RHOBAN_LOGGER_H

#include <stdint.h>
#include <string>

typedef enum {
    LoggerNone,
    LoggerError,
    LoggerWarning,
    LoggerMessage,
    LoggerSuccess,
    LoggerDebug
} LoggerLevel;

namespace Rhoban
{
    class Logger
    {
        public:
            Logger(std::string module, LoggerLevel level=LoggerSuccess);

            void colorize(LoggerLevel lvl);
            void resetColor();
            void prefix();
    
            static void getTime(uint8_t &hour, uint8_t &min, uint8_t &sec);

            void error(const char *fmt, ...);
            void warning(const char *fmt, ...);
            void log(const char *fmt, ...);
            void success(const char *fmt, ...);
            void debug(const char *fmt, ...);

            bool colors;

        protected:
            std::string module;
            LoggerLevel level;
    };
}

#endif
