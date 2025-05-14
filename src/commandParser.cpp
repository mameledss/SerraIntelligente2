#include "../include/CommandParser.h"
#include "../include/Orario.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
CommandParser::CommandParser(Serra& serra) : serra(serra) {}

bool CommandParser::elaboraComando(const std::string& comandoCompleto) {
    // Tokenizza il comando
    std::vector<std::string> tokens = tokenizzaComando(comandoCompleto);
    
    if (tokens.empty()) {
        return false;
    }
    
    // Identifica il tipo di comando
    std::string tipoComando = tokens[0];
    
    if (tipoComando == "set") {
        return gestisciComandoSet(tokens);
    } else if (tipoComando == "rm") {
        return gestisciComandoRm(tokens);
    } else if (tipoComando == "show") {
        return gestisciComandoShow(tokens);
    } else if (tipoComando == "reset") {
        return gestisciComandoReset(tokens);
    } else {
        std::cout << "Comando non riconosciuto" << std::endl;
        return false;
    }
}

std::vector<std::string> CommandParser::tokenizzaComando(const std::string& comandoCompleto) {
    std::vector<std::string> tokens;
    std::istringstream iss(comandoCompleto);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool CommandParser::gestisciComandoSet(const std::vector<std::string>& tokens) {
    if (tokens.size() >= 3) {
        if (tokens[1] == "time" && tokens.size() == 3) {
            // Gestisci comando "set time HH:MM"
            try {
                Orario nuovoOrario(tokens[2]);
                serra.setOrario(nuovoOrario);
                return true;
            } catch (const std::exception& e) {
                std::cout << "Formato orario non valido. Usa HH:MM" << std::endl;
                return false;
            }
        } else if (tokens.size() == 3) {
            // Gestisci comando "set PLANTNAME on/off"
            std::string nomeImpianto = tokens[1];
            std::string azione = tokens[2];
            
            if (azione == "on") {
                return serra.accendiImpianto(nomeImpianto);
            } else if (azione == "off") {
                return serra.spegniImpianto(nomeImpianto);
            } else {
                std::cout << "Azione non riconosciuta. Usa 'on' o 'off'" << std::endl;
                return false;
            }
        } else if (tokens.size() == 4) {
            // Gestisci comando "set PLANTNAME START STOP"
            std::string nomeImpianto = tokens[1];
            try {
                Orario orarioInizio(tokens[2]);
                Orario orarioFine(tokens[3]);
                
                return serra.impostaTimer(nomeImpianto, orarioInizio, orarioFine);
            } catch (const std::exception& e) {
                std::cout << "Formato orario non valido. Usa HH:MM" << std::endl;
                return false;
            }
        }
    }
    
    std::cout << "Formato comando 'set' non valido" << std::endl;
    return false;
}

bool CommandParser::gestisciComandoRm(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        // Gestisci comando "rm PLANTNAME"
        std::string nomeImpianto = tokens[1];
        return serra.rimuoviTimer(nomeImpianto);
    }
    
    std::cout << "Formato comando 'rm' non valido" << std::endl;
    return false;
}

bool CommandParser::gestisciComandoShow(const std::vector<std::string>& tokens) {
    if (tokens.size() == 1) {
        // Gestisci comando "show"
        serra.mostraStato();
        return true;
    } else if (tokens.size() == 2) {
        // Gestisci comando "show PLANTNAME"
        std::string nomeImpianto = tokens[1];
        serra.mostraImpianto(nomeImpianto);
        return true;
    }
    
    std::cout << "Formato comando 'show' non valido" << std::endl;
    return false;
}

bool CommandParser::gestisciComandoReset(const std::vector<std::string>& tokens) {
    if (tokens.size() == 2) {
        // Gestisci comando "reset time/timers/all"
        std::string tipoReset = tokens[1];
        
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
    
    std::cout << "Formato comando 'reset' non valido" << std::endl;
    return false;
}