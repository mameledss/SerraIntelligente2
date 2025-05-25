#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>
#include "Serra.h"
using namespace std;
//funzione per mostrare messaggio con livello di errore e orario
void logMessage(const Orario &time, const string &message, const int &errorLevel);

class CommandParser { //definizione classe CommandParser
private:
    Serra& serra; //riferimento a oggetto Serra
    bool gestisciComandoSet(const vector<string>& tokens); //metodo per gestire comando "set",
    bool gestisciComandoAdd(const vector<string>& tokens); //comando "Add"
    bool gestisciComandoRm(const vector<string>& tokens); //comando "rm",
    bool gestisciComandoShow(const vector<string>& tokens); //comando "show"
    bool gestisciComandoReset(const vector<string>& tokens); //e comando "reset"
    vector<string> tokenizzaComando(const string& comandoCompleto); //metodo per dividere comando in sottostringhe (token)
public:
    CommandParser(Serra& serra); //costruttore che riceve un oggetto Serra da associare al parser
    bool elaboraComando(const string& comandoCompleto); //metodo per elaborare comando ricevuto come stringa
};
#endif // COMMAND_PARSER_H