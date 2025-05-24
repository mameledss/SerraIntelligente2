#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>
#include "Serra.h"
using namespace std;

class CommandParser {
private:
    Serra& serra;
    
public:
    CommandParser(Serra& serra);
    bool elaboraComando(const string& comandoCompleto);
    
private:
    bool gestisciComandoSet(const vector<string>& tokens);
    bool gestisciComandoRm(const vector<string>& tokens);
    bool gestisciComandoShow(const vector<string>& tokens);
    bool gestisciComandoReset(const vector<string>& tokens);
    vector<string> tokenizzaComando(const string& comandoCompleto);
};
#endif // COMMAND_PARSER_H