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
    CommandParser(Serra& serra);
    
    // Metodo per elaborare un comando
    bool elaboraComando(const std::string& comandoCompleto);
    
private:
    // Metodi per gestire i diversi tipi di comandi
    bool gestisciComandoSet(const std::vector<std::string>& tokens);
    bool gestisciComandoRm(const std::vector<std::string>& tokens);
    bool gestisciComandoShow(const std::vector<std::string>& tokens);
    bool gestisciComandoReset(const std::vector<std::string>& tokens);
    
    // Metodi ausiliari
    std::vector<std::string> tokenizzaComando(const std::string& comandoCompleto);
};

#endif // COMMAND_PARSER_H