#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../include/CommandParser.h"
#include "../include/Serra.h"
#include "../include/Orario.h"

using namespace std;

CommandParser::CommandParser(Serra& serra) : serra(serra) {}

void CommandParser::logMessage(const Orario &time, const std::string &message, const int &errorLevel) {
    if (errorLevel == 0)
        std::cout << "[" << time.format() << "] " << message << std::endl;
    else if (errorLevel == 1)
        std::cerr << "[" << time.format() << "] " << message << std::endl;
}

std::vector<std::string> CommandParser::parseCommand(const std::string &command) {
    std::vector<std::string> tokens;
    std::istringstream ss(command);
    std::string token;

    bool isDeviceName = false;
    std::string deviceName;

    while (ss >> token) {
        if (isDeviceName) {
            if (token == "on" || token == "off" || token.find(':') != std::string::npos) {
                if (!deviceName.empty()) {
                    tokens.push_back(deviceName);
                    deviceName.clear();
                }
                isDeviceName = false;
            }
        }

        if (token == "set" || token == "rm" || token == "show") {
            tokens.push_back(token);
            isDeviceName = true;
            continue;
        }

        if (isDeviceName) {
            if (!deviceName.empty()) {
                deviceName += " ";
            }
            deviceName += token;
        } else {
            tokens.push_back(token);
        }
    }

    if (!deviceName.empty()) {
        tokens.push_back(deviceName);
    }

    return tokens;
}

bool CommandParser::commandParser(const std::string &command) {
    Orario time = serra.getOrarioCorrente();
   //logMessage(time, "Comando ricevuto: " + command, 0);

    try {
        std::vector<std::string> tokens = parseCommand(command);

        if (tokens.empty()) {
            throw std::invalid_argument("Errore: comando vuoto.");
        }

        const std::string &action = tokens[0];

        if (action == "set") {
    if (tokens.size() < 2) {
        throw std::invalid_argument("Errore: comando 'set' incompleto.");
    }

    const std::string &deviceName = tokens[1];

    if (deviceName == "time") {
        if (tokens.size() != 3) {
            throw std::invalid_argument("Errore: formato per 'set time' non valido. Usa: set time HH:MM");
        }

        try {
            Orario nuovoOrario(tokens[2]);
            serra.setOrario(nuovoOrario);
            logMessage(nuovoOrario, "Orario impostato a: " + nuovoOrario.format(), 0);
            return true;
        }
        catch (const std::exception& e) {
            throw std::invalid_argument("Errore: formato orario non valido. Usa HH:MM");
        }
    } else {
        // Gestione comandi per impianti
        if (tokens.size() < 3) {
            throw std::invalid_argument("Errore: comando 'set' incompleto per dispositivo.");
        }

        const std::string &operation = tokens[2];

        if (operation == "on") {
            return serra.accendiImpianto(deviceName);
        } else if (operation == "off") {
            return serra.spegniImpianto(deviceName);
            // SOSTITUISCI QUESTA PARTE nel tuo codice originale:
            // Dalla riga che inizia con "} else {" dopo il controllo "if (deviceName == "time")"
            // Fino alla fine del blocco "if (action == "set")"

        } else {
            // Gestione comandi per impianti
            if (tokens.size() < 3) {
                throw std::invalid_argument("Errore: comando 'set' incompleto per dispositivo.");
            }

            const std::string &operation = tokens[2];

            if (operation == "on") {
                return serra.accendiImpianto(deviceName);
            } else if (operation == "off") {
                return serra.spegniImpianto(deviceName);
            } else {
                // Gestisci comando "set PLANTNAME HH:MM"
                try {
                    Orario inizio(operation);

                    if (tokens.size() == 3) {
                        // Timer con durata predefinita di 1 ora
                        int oreSpegnimento = (inizio.getOre() + 1) % 24;
                        int minutiSpegnimento = inizio.getMinuti();
                        Orario orarioSpegnimento(oreSpegnimento, minutiSpegnimento);

                        return serra.impostaTimer(deviceName, inizio, orarioSpegnimento);
                    } else if (tokens.size() == 4) {
                        // Timer con orario di fine specificato
                        Orario orarioFine(tokens[3]);
                        return serra.impostaTimer(deviceName, inizio, orarioFine);
                    } else {
                        throw std::invalid_argument("Errore: troppi parametri per il comando set.");
                    }
                } catch (const std::exception& e) {
                    throw std::invalid_argument("Errore: formato orario non valido per il timer. Usa HH:MM");
                }
            }
        }
    }
} else if (action == "rm") {
            if (tokens.size() != 2) {
                throw std::invalid_argument("Errore: comando 'rm' non valido. Usa: rm " + tokens[1]);
            }
            return serra.rimuoviTimer(tokens[1]);
        } else if (action == "show") {
            if (tokens.size() == 1) {
                serra.mostraStato();
                return true;
            } else if (tokens.size() == 2) {
                const std::string &showType = tokens[1];

                if (showType == "tipi") {
                    // Mostra tutti i tipi di impianti disponibili
                    logMessage(time, "Tipi di impianti disponibili:", 0);
                    cout << "  - Tropicale" << endl;
                    cout << "  - Desertico" << endl;
                    cout << "  - Carnivoro" << endl;
                    cout << "  - Alpino" << endl;
                    cout << "  - Mediterraneo" << endl;
                    return true;
                } else {

                    serra.mostraImpianto(showType);
                    return true;
                }
            } else {
                throw std::invalid_argument("Errore: comando 'show' non valido. Usa: show oppure show ${DEVICENAME}");
            }

        } else if (action == "reset") {
            if (tokens.size() != 2) {
                throw std::invalid_argument("Errore: comando 'reset' non valido.");
            }

            const std::string &resetType = tokens[1];
            if (resetType == "time") {
                serra.resetOrario();
                return true;
            } else if (resetType == "timers") {
                serra.resetTimers();
                return true;
            } else if (resetType == "all") {
                serra.resetAll();
                return true;
            } else {
                throw std::invalid_argument(
                        "Errore: opzione 'reset' non valida. Usa: reset time | reset timers | reset all");
            }
        } else if (action == "add") {
            if (tokens.size() != 3) {

                throw std::invalid_argument("Errore: comando 'add' non valido. Usa: add [tipo] [nome_impianto]");
            }

            const std::string &tipo = tokens[1];
            const std::string &nomeImpianto = tokens[2];


            if (tipo != "Tropicale" && tipo != "Desertico" && tipo != "Carnivoro" &&
                tipo != "Alpino" && tipo != "Mediterraneo") {
                throw std::invalid_argument("Errore: tipo impianto non supportato. Tipi disponibili: Tropicale, Desertico, Carnivoro, Alpino, Mediterraneo");
                }

            bool risultato = serra.aggiungiImpianto(tipo, nomeImpianto);
            if (risultato) {
                logMessage(time, "Impianto '" + nomeImpianto + "' di tipo '" + tipo + "' aggiunto con successo", 0);
            }
            return risultato;

        } else if (action == "remove") {
            if (tokens.size() != 2) {
                throw std::invalid_argument("Errore: comando 'remove' non valido. Usa: remove [nome_impianto]");
            }

            const std::string &nomeImpianto = tokens[1];
            bool risultato = serra.rimuoviImpianto(nomeImpianto);
            if (risultato) {
                logMessage(time, "Impianto '" + nomeImpianto + "' rimosso con successo", 0);
            }
            return risultato;

        }else {
            throw std::invalid_argument("Errore: comando '" + action + "' non riconosciuto.");
        }
    } catch (const std::exception& e) {
        logMessage(time, e.what(), 1);
        return false;
    }

    return false;
}