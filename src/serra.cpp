#include "Serra.h"
#include "CommandParser.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
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
        cout << "Errore: Esiste già un impianto con il nome \"" << nome << "\"" << endl;
        return false;
    }

    Impianto* nuovoImpianto = Impianto::creaImpianto(prossimoIdImpianto, tipo, nome);
    if (!nuovoImpianto) {
        cout << "Errore: Tipo di impianto \"" << tipo << "\" non riconosciuto" << endl;
        return false;
    }

    impianti.push_back(nuovoImpianto);
    prossimoIdImpianto++;

    cout << "Impianto \"" << nome << "\" aggiunto con successo" << endl;
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
        cout << "Impianto \"" << nome << "\" rimosso con successo" << endl;
        return true;
    }

    cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
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
        cout << "Impianto \"" << nome << "\" rimosso con successo" << endl;
        return true;
    }

    cout << "Errore: Impianto con ID " << id << " non trovato" << endl;
    return false;
}

bool Serra::accendiImpianto(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
        return false;
    }

    if (impianto->isAttivo()) {
        cout << "Impianto \"" << nome << "\" è già acceso" << endl;
        return true;
    }

    return impianto->accendi(orarioCorrente);
}

bool Serra::accendiImpiantoOn(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
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
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
        return false;
    }

    if (!impianto->isAttivo()) {
        cout << "Impianto \"" << nome << "\" è già spento" << endl;
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
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
        return false;
    }

    string info = impianto->getInfo();
    bool isTropicale = info.find("Tropicale") != string::npos;
    bool isAlpino = info.find("Alpino") != string::npos;
    bool isCarnivoro = info.find("Carnivoro") != string::npos;

    if (isTropicale || isCarnivoro || isAlpino) {
        cout << "Errore: Gli impianti automatici non supportano timer con intervallo specifico" << endl;
        return false;
    }

    if (impianto->impostaTimer(inizio, fine)) {
        return true;
    } else {
        cout << "Errore: Impossibile impostare il timer per l'impianto \"" << nome << "\"" << endl;
        return false;
    }
}

bool Serra::impostaTimer2(const string& nome, const Orario& inizio) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
        return false;
    }

    if (impianto->impostaTimer2(inizio)) {
        return true;
    } else {
        cout << "Errore: Impossibile impostare il timer per l'impianto \"" << nome << "\"" << endl;
        return false;
    }
}

bool Serra::rimuoviTimer(const string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
        return false;
    }

    if (impianto->rimuoviTimer()) {
        cout << "[" << orarioCorrente.format() << "] Timer rimosso per \"" << nome << "\"" << endl;
        return true;
    } else {
        cout << "Errore: Nessun timer impostato per l'impianto \"" << nome << "\"" << endl;
        return false;
    }
}

void Serra::setOrario(const Orario& nuovoOrario) {
    if (nuovoOrario < orarioCorrente) {
        cout << "Errore: Non è possibile impostare un orario precedente a quello attuale" << endl;
        return;
    }

    aggiornaStatiImpianti(nuovoOrario);
    orarioCorrente = nuovoOrario;
    cout << orarioCorrente.format() << " L'orario attuale è " << orarioCorrente.format() << endl;
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
    cout << orarioCorrente.format() << " Orario reimpostato a " << orarioCorrente.format() << endl;
}

void Serra::resetTimers() {
    for (auto impianto : impianti) {
        impianto->rimuoviTimer();
    }

    cout << orarioCorrente.format() << " Tutti i timer sono stati rimossi" << endl;
}

void Serra::resetAll() {
    for (auto impianto : impianti) {
        if (impianto->isAttivo()) {
            impianto->spegni(orarioCorrente);
        }
        impianto->rimuoviTimer();
    }

    orarioCorrente = Orario(0, 0);
    cout << orarioCorrente.format() << " Sistema reimpostato completamente" << endl;
}

void Serra::mostraStato() const {
    if (impianti.empty()) {
        cout << "Nessun impianto presente nella serra" << endl;
        return;
    }

    cout << "Stato attuale degli impianti (orario: " << orarioCorrente.format() << "):" << endl;
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
        cout << "Errore: Impianto \"" << nome << "\" non trovato" << endl;
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
