//FILE AUTHOR: DALLA SANTA MANUEL
#include <iostream>
#include <sstream>
#include <fstream>
#include "CommandParser.h"
#include "Orario.h"

CommandParser::CommandParser(Serra& serra) : serra(serra) {} //costruttore che inizializza il riferimento alla serra

void logMessage(const Orario &time, const string &message, const int &errorLevel) {
    static bool firstCall = true; // variabile statica che mantiene il valore tra le chiamate

    if (errorLevel == 0)
        cout << "[" << time.toString() << "] " << message << endl;
    else if (errorLevel == 1) {
        cout.flush();
        cerr << "[" << time.toString() << "] " << message << endl;
    }

    ofstream logFile("log.txt", ios::app);
    if (logFile.is_open()) {
        if (firstCall) { // solo alla prima chiamata
            logFile << "\n========NUOVA SIMULAZIONE========" << endl;
            firstCall = false; // imposta a false per le chiamate successive
        }

        string levelString = (errorLevel == 0) ? "INFO" : "ERROR";
        logFile << "[" << time.toString() << "] [" << levelString << "] " << message << endl;
        logFile.close();
    }
}


bool CommandParser::elaboraComando(const string& comandoCompleto) { //elabora un comando ricevuto in formato stringa
    vector<string> tokens = tokenizzaComando(comandoCompleto); //divide il comando in parole (token)
    if (tokens.empty()) //se non ci sono token
        return false; //ritorna falso, comando non valido

    string tipoComando = tokens[0]; //il primo token rappresenta il tipo di comando

    if (tipoComando == "set") //se il comando è "set"
        return gestisciComandoSet(tokens); //chiama il gestore del comando "set"
    else if (tipoComando == "add") //se il comando è "add"
        return gestisciComandoAdd(tokens); //chiama il gestore del comando "add"
    else if (tipoComando == "rm") //se è "rm"
        return gestisciComandoRm(tokens); //chiama gestore "rm"
    else if (tipoComando == "show") //se è "show"
        return gestisciComandoShow(tokens); //chiama gestore "show"
    else if (tipoComando == "reset") //se è "reset"
        return gestisciComandoReset(tokens); //chiama gestore "reset"
    else { //altrimenti
        cerr << "Comando non riconosciuto" << endl; //se il comando non è riconosciuto, stampa errore
        return false;
    }
}

vector<string> CommandParser::tokenizzaComando(const string& comandoCompleto) { //divide la stringa comando in parole
    vector<string> tokens; //lista di parole che compongono il comando
    istringstream iss(comandoCompleto); //crea input string stream dalla stringa per trattarla come fosse un input da cui leggere parola per parola
    string token; //conterrà una parola alla volta durante il while
    while (iss >> token) { //cicla su tutte le parole separate da spazi
        tokens.push_back(token); //aggiunge ogni parola al vettore
    }
    return tokens; //restituisce il vettore dei token
}

bool CommandParser::gestisciComandoSet(const vector<string>& tokens) { //gestisce i comandi "set"
    if (tokens.size() >= 3) { //se il comando è di almeno 3 token
        if (tokens[1] == "time" && tokens.size() == 3) { //se la seconda parola è "time" e il comando ha dimensione 3 -> comando "set time HH:MM"
            try {
                Orario nuovoOrario(tokens[2]); //prova a creare un nuovo orario dal terzo token
                serra.setOrario(nuovoOrario); //imposta l'orario nella serra
                return true;
            } catch (const exception& e) { //se c'è un errore di formato
                cerr << "Formato orario non valido. Usa HH:MM" << endl; //ritorna errore
                return false;
            }
        } else if (tokens.size() == 3) { //altrimenti il comando è "set NOME on/off/HH:MM"
            string nomeImpianto = tokens[1]; //nome dell'impianto
            string azione = tokens[2]; //azione da eseguire
            if (azione == "on") { //se l'azione è "on"
                return serra.accendiImpiantoOn(nomeImpianto); //accende l'impianto
            } else if (azione == "off") { //se è off
                return serra.spegniImpianto(nomeImpianto); //lo spegne
            } else { //altrimenti è "set NOME HH:MM"
                try {
                    Orario inizio(tokens[2]); //tenta di creare un orario di inizio
                    return serra.impostaTimer2(nomeImpianto, inizio); //imposta il timer con orario di accensione
                } catch (const exception& e) { //altrimenti genera un'eccezione
                    cerr << "Formato non valido. Usa 'on', 'off' o un orario in formato HH:MM" << endl;
                    return false;
                }
            }
        } else if (tokens.size() == 4) { //comando "set NOME ORA_INIZIO ORA_FINE"
            string nomeImpianto = tokens[1]; //nome dell'impianto
            try {
                Orario orarioInizio(tokens[2]); //crea oggetto Orario per l'inizio
                Orario orarioFine(tokens[3]); //e per la fine
                return serra.impostaTimer(nomeImpianto, orarioInizio, orarioFine); //imposta il timer
            } catch (const exception& e) { //altrimenti genera eccezione
                cerr << "Formato orario non valido. Usa HH:MM" << endl;
                return false;
            }
        }
    }
    cerr << "Formato comando 'set' non valido" << endl;
    return false;
}

bool CommandParser::gestisciComandoAdd(const vector<string>& tokens) { //gestisce il comando "add"
    if (tokens.size() == 3) { //se il comando è di 3 token: "add TipoImpianto NomeImpianto"
        string tipoImpianto = tokens[1]; //tipo dell'impianto
        string nomeImpianto = tokens[2]; //nome dell'impianto
        return serra.aggiungiImpianto(tipoImpianto, nomeImpianto); //aggiunge l'impianto alla serra
    }
    cerr << "Formato comando 'add' non valido. Usa: add [TipoImpianto] [NomeImpianto]" << endl;
    return false;
}

bool CommandParser::gestisciComandoRm(const vector<string>& tokens) { //gestisce il comando "rm"
    if (tokens.size() == 2) { //se il comando è di 2 token: "rm NomeImpianto" (rimuove timer)
        string nomeImpianto = tokens[1]; //nome dell'impianto
        return serra.rimuoviTimer(nomeImpianto); //rimuove il timer associato
    } else if (tokens.size() == 3 && tokens[1] == "-i") { //se il comando è di 3 token e ha -i: "rm -i NomeImpianto" (rimuove impianto)
        string nomeImpianto = tokens[2]; //nome dell'impianto
        return serra.rimuoviImpianto(nomeImpianto); //rimuove l'impianto dalla serra
    }
    cerr << "Formato comando 'rm' non valido. Usa: rm [nome_impianto] o rm -i [nome_impianto]" << endl;
    return false;
}

bool CommandParser::gestisciComandoShow(const vector<string>& tokens) { //gestisce il comando "show"
    if (tokens.size() == 1) { //se il comando è di 1 token
        serra.mostraStato(); //mostra lo stato di tutti gli impianti
        return true;
    } else if (tokens.size() == 2) { //altrimenti
        string nomeImpianto = tokens[1]; //nome dell'impianto
        serra.mostraImpianto(nomeImpianto); //mostra info impianto specifico
        return true;
    }
    cerr << "Formato comando 'show' non valido" << endl;
    return false;
}

bool CommandParser::gestisciComandoReset(const vector<string>& tokens) { //gestisce il comando "reset"
    if (tokens.size() == 2) { //se il comando è di 2 token
        string tipoReset = tokens[1]; //tipo di reset richiesto

        if (tipoReset == "time") { //se il reset è di tipo "time"
            serra.resetOrario(); //reimposta orario a 00:00
            return true;
        } else if (tipoReset == "timers") { //se il reset è di tipo "timers"
            serra.resetTimers(); //rimuove tutti i timer dagli impianti
            return true;
        } else if (tipoReset == "all") { //se il reset è di tipo "all"
            serra.resetAll(); //reset completo: orario + impianti
            return true;
        }
    }
    cerr << "Formato comando 'reset' non valido" << endl;
    return false;
}
