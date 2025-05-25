#include "logger.h"
#include <iostream>

std::ofstream Logger::logFile;
bool Logger::fileInitialized = false;

void Logger::initializeFile() {
    if (!fileInitialized) {
        logFile.open("serra.log", std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Errore: Impossibile aprire il file di log serra.log" << std::endl;
        } else {
            std::cout << "File di log serra.log creato/aperto con successo" << std::endl;
        }
        fileInitialized = true;
    }
}

void Logger::logMessage(const Orario& time, const std::string& message, int errorLevel) {
    initializeFile();

    std::string formattedMessage = "[" + time.format() + "] " + message;

    // Log su console - sempre su cout per uniformità
    std::cout << formattedMessage << std::endl;

    // Log su file solo se il file è aperto correttamente
    if (logFile.is_open()) {
        logFile << formattedMessage << std::endl;
        logFile.flush(); // Forza la scrittura immediata
    }
}

void Logger::closeLog() {
    if (logFile.is_open()) {
        logFile.close();
        fileInitialized = false;
    }
}

Logger::~Logger() {
    closeLog();
}