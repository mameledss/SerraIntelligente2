#include <iostream>
#include <algorithm>
#include <iomanip>
#include "Serra.h"
#include "CommandParser.h"
using namespace std;

Serra::Serra() : orarioCorrente(0, 0), prossimoIdImpianto(1) {}

Serra::~Serra() {
    for (auto impianto : impianti) {
        delete impianto;
    }
    impianti.clear();
}

bool Serra::aggiungiImpianto(const string& tipo, const string& nome) {
    if (trovaImpianto(nome) != nullptr) {
        logMessage(orarioCorrente, "Errore: Esiste gia' un impianto con il nome \"" + nome + "\"", 1);
        return false;
    }

    Impianto* nuovoImpianto = Impianto::creaImpianto(prossimoIdImpianto, tipo, nome);
    if (!nuovoImpianto) {
        logMessage(orarioCorrente, "Errore: Tipo di impianto \"" + tipo + "\" non riconosciuto", 1);
        return false;
    }

    impianti.push_back(nuovoImpianto);
    prossimoIdImpianto++;

    logMessage(orarioCorrente, "Impianto \"" + nome + "\" aggiunto con successo", 0);
    return true;
}

bool Serra::rimuoviImpianto(const string& nome) {
    auto it = find_if(impianti.begin(), impianti.end(),
                      [&nome](const Impianto* imp) {
                          return imp->getNome() == nome;
                      });

    if (it != impianti.end()) {
        Impianto* impiantoDaRimuovere = *it;
        impianti.erase(it);
        delete impiantoDaRimuovere;
        logMessage(orarioCorrente, "Impianto \"" + nome + "\" rimosso con successo", 0);
        return true;
    }

    logMessage(orarioCorrente, "Impianto \"" + nome + "\" rimosso con successo", 0);
    return false;
}

bool Serra::rimuoviImpianto(int id) {
    auto it = find_if(impianti.begin(), impianti.end(),
                      [id](const Impianto* imp) {
                          return imp->getId() == id;
                      });

    if (it != impianti.end()) {
        string nome = (*it)->getNome();
        Impianto* impiantoDaRimuovere = *it;
        impianti.erase(it);
        delete impiantoDaRimuovere;
        logMessage(orarioCorrente, "Impianto \"" + nome + "\" rimosso con successo", 0);
        return true;
    }
    logMessage(orarioCorrente,"Errore: Impianto con ID " + to_string(id) + " non trovato" , 1);
    return false;
}

bool Serra::accendiImpianto(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->isAttivo()) {
        logMessage(orarioCorrente, "Impianto \"" + nome + "\" è già acceso", 1);
        return true;
    }

    return impianto->accendi(orarioCorrente);
}

bool Serra::accendiImpiantoOn(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }

    string info = impianto->getInfo();
    bool isTropicale = info.find("Tropicale") != string::npos;
    bool isAlpino = info.find("Alpino") != string::npos;
    bool isCarnivoro = info.find("Carnivoro") != string::npos;

    if (isTropicale || isAlpino || isCarnivoro) {
        if (impianto->impostaTimer2(orarioCorrente)) {
            if (!impianto->isAttivo()) {
                impianto->accendi(orarioCorrente);
            }
            return true;
        }
    } else {
        if (impianto->impostaTimer2(orarioCorrente)) {
            if (!impianto->isAttivo()) {
                impianto->accendi(orarioCorrente);
            }
            return true;
        }
    }
    return false;
}

bool Serra::spegniImpianto(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }

    if (!impianto->isAttivo()) {
        logMessage(orarioCorrente, "Impianto \"" + nome + "\" è già spento", 1);
        return true;
    }

    if (impianto->spegni(orarioCorrente)) {
        impianto->rimuoviTimer();
        return true;
    }

    return false;
}

bool Serra::impostaTimer(const string& nome, const Orario& inizio, const Orario& fine) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }

    string info = impianto->getInfo();
    bool isTropicale = info.find("Tropicale") != string::npos;
    bool isAlpino = info.find("Alpino") != string::npos;
    bool isCarnivoro = info.find("Carnivoro") != string::npos;

    if (isTropicale || isCarnivoro || isAlpino) {
        logMessage(orarioCorrente, "Errore: Gli impianti automatici non supportano timer con intervallo specifico", 1);
        return false;
    }
    if (impianto->impostaTimer(inizio, fine)) {
        return true;
    } else {
        logMessage(orarioCorrente, "Errore: Impossibile impostare il timer per l'impianto \"" + nome + "\"", 1);
        return false;
    }
}

bool Serra::impostaTimer2(const string& nome, const Orario& inizio) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->impostaTimer2(inizio)) {
        return true;
    } else {
        logMessage(orarioCorrente, "Errore: Impossibile impostare il timer per l'impianto \"" + nome + "\"", 1);
        return false;
    }
}

bool Serra::rimuoviTimer(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->rimuoviTimer()) {
        logMessage(orarioCorrente, "Timer rimosso per \"" + nome + "\"", 0);
        return true;
    } else {
        logMessage(orarioCorrente, "Errore: Nessun timer impostato per l'impianto \"" + nome + "\"", 1);
        return false;
    }
}

void Serra::setOrario(const Orario& nuovoOrario) {
    if (nuovoOrario < orarioCorrente) {
        logMessage(orarioCorrente, "Errore: Non e' possibile impostare un orario precedente a quello attuale", 1);
        return;
    }
    aggiornaStatiImpianti(nuovoOrario);
    orarioCorrente = nuovoOrario;
    logMessage(orarioCorrente, " L'orario attuale e' " + orarioCorrente.format(), 0);
}

Orario Serra::getOrarioCorrente() const {
    return orarioCorrente;
}

void Serra::resetOrario() {
    for (auto impianto : impianti) {
        if (impianto->isAttivo()) {
            impianto->spegni(orarioCorrente);
        }
        impianto->rimuoviTimer();
    }

    orarioCorrente = Orario(0, 0);
    logMessage(orarioCorrente, " Orario reimpostato a " + orarioCorrente.format(), 0);
}

void Serra::resetTimers() {
    for (auto impianto : impianti) {
        impianto->rimuoviTimer();
    }
    logMessage(orarioCorrente, " Tutti i timer sono stati rimossi", 0);
}

void Serra::resetAll() {
    for (auto impianto : impianti) {
        if (impianto->isAttivo()) {
            impianto->spegni(orarioCorrente);
        }
        impianto->rimuoviTimer();
    }
    orarioCorrente = Orario(0, 0);
    logMessage(orarioCorrente, " Sistema reimpostato completamente", 0);
}

void Serra::mostraStato() const {
    if (impianti.empty()) {
        logMessage(orarioCorrente, "Nessun impianto presente nella serra", 1);
        return;
    }
    logMessage(orarioCorrente, "Stato attuale degli impianti", 0);
    cout << "--------------------------------------------------------" << endl;
    cout << left << setw(5) << "ID"
         << setw(15) << "Nome"
         << setw(15) << "Tipo"
         << setw(10) << "Stato"
         << setw(20) << "Consumo (litri)" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (const auto impianto : impianti) {
        string tipo = impianto->getInfo();
        size_t posTipo = tipo.find(": ");
        if (posTipo != string::npos) {
            tipo = tipo.substr(posTipo + 2);
        }

        cout << left << setw(5) << impianto->getId()
             << setw(15) << impianto->getNome()
             << setw(15) << tipo
             << setw(10) << (impianto->isAttivo() ? "Acceso" : "Spento")
             << setw(20) << fixed << setprecision(2) << impianto->getConsumoTotale() << endl;
    }
    cout << "--------------------------------------------------------" << endl;
}

void Serra::mostraImpianto(const string& nome) const {
    auto it = find_if(impianti.begin(), impianti.end(),
                      [&nome](const Impianto* imp) {
                          return imp->getNome() == nome;
                      });
    if (it == impianti.end()) {
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return;
    }
    const auto impianto = *it;
    cout << "Dettagli impianto:" << endl;
    cout << "ID: " << impianto->getId() << endl;
    cout << "Nome: " << impianto->getNome() << endl;
    cout << "Stato: " << (impianto->isAttivo() ? "Acceso" : "Spento") << endl;

    Orario ultimaAttivazione = impianto->getUltimaAttivazione();
    cout << "Ultima attivazione: ";
    if (ultimaAttivazione.getOre() != 0 || ultimaAttivazione.getMinuti() != 0) {
        cout << ultimaAttivazione.format() << endl;
    } else {
        cout << "Mai" << endl;
    }
    cout << "Consumo totale: " << fixed << setprecision(2) << impianto->getConsumoTotale() << " litri" << endl;
}

Impianto* Serra::trovaImpianto(const string& nome) {
    auto it = find_if(impianti.begin(), impianti.end(),
                      [&nome](const Impianto* imp) {
                          return imp->getNome() == nome;
                      });

    return (it != impianti.end()) ? *it : nullptr;
}

Impianto* Serra::trovaImpianto(int id) {
    auto it = find_if(impianti.begin(), impianti.end(),
                      [id](const Impianto* imp) {
                          return imp->getId() == id;
                      });

    return (it != impianti.end()) ? *it : nullptr;
}

void Serra::aggiornaStatiImpianti(const Orario& nuovoOrario) {
    Orario orarioSimulato = orarioCorrente;

    while (orarioSimulato < nuovoOrario) {
        Orario orarioPrecedente = orarioSimulato;

        int nuoveOre = orarioSimulato.getOre();
        int nuoviMinuti = orarioSimulato.getMinuti() + 1;

        if (nuoviMinuti >= 60) {
            nuoviMinuti = 0;
            nuoveOre = (nuoveOre + 1) % 24;
        }
        orarioSimulato = Orario(nuoveOre, nuoviMinuti);

        for (auto impianto : impianti) {
            impianto->aggiornaStato(orarioPrecedente, orarioSimulato);
        }
    }
}