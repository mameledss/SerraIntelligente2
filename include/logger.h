#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include "Orario.h"

class Logger {
private:
    static std::ofstream logFile;
    static bool fileInitialized;
    
    static void initializeFile();
    
public:
    static void logMessage(const Orario& time, const std::string& message, int errorLevel = 0);
    static void closeLog();
    ~Logger();
};

// Funzione globale per compatibilità con il codice esistente
inline void logMessage(const Orario& time, const std::string& message, int errorLevel = 0) {
    Logger::logMessage(time, message, errorLevel);
}

#endif // LOGGER_H