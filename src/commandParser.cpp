#include <iostream>
#include <sstream>
#include "CommandParser.h"
#include "Orario.h"

CommandParser::CommandParser(Serra& serra) : serra(serra) {}

bool CommandParser::elaboraComando(const string& comandoCompleto) {
    vector<string> tokens = tokenizzaComando(comandoCompleto); //suddivide comando in token
    if (tokens.empty()) {
        return false;
    }
    string tipoComando = tokens[0]; // Identifica il tipo di comando
    
    if (tipoComando == "set") {
        return gestisciComandoSet(tokens);
    } else if (tipoComando == "rm") {
        return gestisciComandoRm(tokens);
    } else if (tipoComando == "show") {
        return gestisciComandoShow(tokens);
    } else if (tipoComando == "reset") {
        return gestisciComandoReset(tokens);
    } else {
        cout << "Comando non riconosciuto" << endl;
        return false;
    }
}

vector<string> CommandParser::tokenizzaComando(const string& comandoCompleto) {
    vector<string> tokens;
    istringstream iss(comandoCompleto);
    string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

bool CommandParser::gestisciComandoSet(const vector<string>& tokens) {
    if (tokens.size() >= 3) {
        if (tokens[1] == "time" && tokens.size() == 3) {
            // Gestisci comando "set time HH:MM"
            try {
                Orario nuovoOrario(tokens[2]);
                serra.setOrario(nuovoOrario);
                return true;
            } catch (const exception& e) {
                cout << "Formato orario non valido. Usa HH:MM" << endl;
                return false;
            }
        } else if (tokens.size() == 3) {
            // Gestisci comando "set PLANTNAME on/off" o "set PLANTNAME HH:MM"
            string nomeImpianto = tokens[1];
            string azione = tokens[2];

            if (azione == "on") {
                // Gestisci comando "set PLANTNAME on"
                return serra.accendiImpiantoOn(nomeImpianto);
            } else if (azione == "off") {
                // Gestisci comando "set PLANTNAME off"
                return serra.spegniImpianto(nomeImpianto);
            } else {
                try {
                    // Gestisci comando "set PLANTNAME HH:MM"
                    Orario inizio(tokens[2]);
                    return serra.impostaTimer2(nomeImpianto, inizio);
                } catch (const exception& e) {
                    cout << "Formato non valido. Usa 'on', 'off' o un orario in formato HH:MM" << endl;
                    return false;
                }
            }
        } else if (tokens.size() == 4) {
            // Gestisci comando "set PLANTNAME START STOP" (solo per desertico)
            string nomeImpianto = tokens[1];
            try {
                Orario orarioInizio(tokens[2]);
                Orario orarioFine(tokens[3]);
                
                return serra.impostaTimer(nomeImpianto, orarioInizio, orarioFine);
            } catch (const exception& e) {
                cout << "Formato orario non valido. Usa HH:MM" << endl;
                return false;
            }
        }
    }
    cout << "Formato comando 'set' non valido" << endl;
    return false;
}

bool CommandParser::gestisciComandoRm(const vector<string>& tokens) {
    if (tokens.size() == 2) {
        // Gestisci comando "rm PLANTNAME"
        string nomeImpianto = tokens[1];
        return serra.rimuoviTimer(nomeImpianto);
    }
    
    cout << "Formato comando 'rm' non valido" << endl;
    return false;
}

bool CommandParser::gestisciComandoShow(const vector<string>& tokens) {
    if (tokens.size() == 1) {
        // Gestisci comando "show"
        serra.mostraStato();
        return true;
    } else if (tokens.size() == 2) {
        // Gestisci comando "show PLANTNAME"
        string nomeImpianto = tokens[1];
        serra.mostraImpianto(nomeImpianto);
        return true;
    }
    cout << "Formato comando 'show' non valido" << endl;
    return false;
}

bool CommandParser::gestisciComandoReset(const vector<string>& tokens) {
    if (tokens.size() == 2) {
        // Gestisci comando "reset time/timers/all"
        string tipoReset = tokens[1];
        
        if (tipoReset == "time") {
            serra.resetOrario();
            return true;
        } else if (tipoReset == "timers") {
            serra.resetTimers();
            return true;
        } else if (tipoReset == "all") {
            serra.resetAll();
            return true;
        }
    }
    
    cout << "Formato comando 'reset' non valido" << endl;
    return false;
}