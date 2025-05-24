#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>
#include "Serra.h"

class CommandParser {
private:
    Serra& serra;

public:
    // Costruttore
    explicit CommandParser(Serra& serra);

    // Metodo per elaborare un comando
    bool commandParser(const std::string &comandoCompleto);

private:
    // Metodi per gestire i diversi tipi di comandi
    void logMessage(const Orario &time, const std::string &message, const int &errorLevel);
    std::vector<std::string> parseCommand(const std::string &comandoCompleto);



};

#endif // COMMAND_PARSER_H