#include "../include/Serra.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

Serra::Serra() : orarioCorrente(0, 0), prossimoIdImpianto(1) {}

Serra::~Serra() {
    // Elimina tutti gli impianti
    for (auto impianto : impianti) {
        delete impianto;
    }
    impianti.clear();
}

bool Serra::aggiungiImpianto(const std::string& tipo, const std::string& nome) {
    // Verifica se esiste già un impianto con lo stesso nome
    if (trovaImpianto(nome) != nullptr) {
        std::cout << "Errore: Esiste già un impianto con il nome \"" << nome << "\"" << std::endl;
        return false;
    }

    // Crea un nuovo impianto
    Impianto* nuovoImpianto = Impianto::creaImpianto(prossimoIdImpianto, tipo, nome);
    if (!nuovoImpianto) {
        std::cout << "Errore: Tipo di impianto \"" << tipo << "\" non riconosciuto" << std::endl;
        return false;
    }

    // Aggiungi l'impianto alla collezione e incrementa l'ID
    impianti.push_back(nuovoImpianto);
    prossimoIdImpianto++;

    std::cout << "Impianto \"" << nome << "\" aggiunto con successo" << std::endl;
    return true;
}

bool Serra::rimuoviImpianto(const std::string& nome) {
    auto it = std::find_if(impianti.begin(), impianti.end(),
                          [&nome](const Impianto* imp) {
                              return imp->getNome() == nome;
                          });

    if (it != impianti.end()) {
        Impianto* impiantoDaRimuovere = *it;
        impianti.erase(it);
        delete impiantoDaRimuovere;  // Libera la memoria
        std::cout << "Impianto \"" << nome << "\" rimosso con successo" << std::endl;
        return true;
    }

    std::cout << "Errore: Impianto \"" << nome << "\" non trovato" << std::endl;
    return false;
}

bool Serra::rimuoviImpianto(int id) {
    auto it = std::find_if(impianti.begin(), impianti.end(),
                          [id](const Impianto* imp) {
                              return imp->getId() == id;
                          });

    if (it != impianti.end()) {
        std::string nome = (*it)->getNome();
        Impianto* impiantoDaRimuovere = *it;
        impianti.erase(it);
        delete impiantoDaRimuovere;  // Libera la memoria
        std::cout << "Impianto \"" << nome << "\" rimosso con successo" << std::endl;
        return true;
    }

    std::cout << "Errore: Impianto con ID " << id << " non trovato" << std::endl;
    return false;
}

bool Serra::accendiImpianto(const std::string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        std::cout << "Errore: Impianto \"" << nome << "\" non trovato" << std::endl;
        return false;
    }

    if (impianto->isAttivo()) {
        std::cout << "Impianto \"" << nome << "\" è già acceso" << std::endl;
        return true;
    }

    if (impianto->accendi(orarioCorrente)) {
        return true;
    }

    return false;
}

bool Serra::spegniImpianto(const std::string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        std::cout << "Errore: Impianto \"" << nome << "\" non trovato" << std::endl;
        return false;
    }

    if (!impianto->isAttivo()) {
        std::cout << "Impianto \"" << nome << "\" è già spento" << std::endl;
        return true;
    }

    if (impianto->spegni(orarioCorrente)) {
        return true;
    }

    return false;
}

bool Serra::impostaTimer(const std::string& nome, const Orario& inizio, const Orario& fine) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        std::cout << "Errore: Impianto \"" << nome << "\" non trovato" << std::endl;
        return false;
    }

    if (impianto->impostaTimer(inizio, fine)) {
        return true;
    } else {
        std::cout << "Errore: Impossibile impostare il timer per l'impianto \"" << nome << "\"" << std::endl;
        return false;
    }
}

bool Serra::rimuoviTimer(const std::string& nome) {
    Impianto* impianto = trovaImpianto(nome);
    if (!impianto) {
        std::cout << "Errore: Impianto \"" << nome << "\" non trovato" << std::endl;
        return false;
    }

    if (impianto->rimuoviTimer()) {
        std::cout << "[" << orarioCorrente.format() << "] Timer rimosso per \"" << nome << "\"" << std::endl;
        return true;
    } else {
        std::cout << "Errore: Nessun timer impostato per l'impianto \"" << nome << "\"" << std::endl;
        return false;
    }
}

void Serra::setOrario(const Orario& nuovoOrario) {
    if (nuovoOrario < orarioCorrente) {
        std::cout << "Errore: Non è possibile impostare un orario precedente a quello attuale" << std::endl;
        return;
    }

    // Aggiorna lo stato degli impianti in base al nuovo orario
    aggiornaStatiImpianti(nuovoOrario);

    // Aggiorna l'orario corrente
    orarioCorrente = nuovoOrario;
    std::cout << orarioCorrente.format() << " L'orario attuale è " << orarioCorrente.format() << std::endl;
}

Orario Serra::getOrarioCorrente() const {
    return orarioCorrente;
}

void Serra::resetOrario() {
    // Spegni tutti gli impianti
    for (auto impianto : impianti) {
        if (impianto->isAttivo()) {
            impianto->spegni(orarioCorrente);
        }
        impianto->rimuoviTimer();
    }

    // Reimposta l'orario a 00:00
    orarioCorrente = Orario(0, 0);
    std::cout << orarioCorrente.format() << " Orario reimpostato a " << orarioCorrente.format() << std::endl;
}

void Serra::resetTimers() {
    // Rimuovi tutti i timer ma mantieni lo stato degli impianti
    for (auto impianto : impianti) {
        impianto->rimuoviTimer();
    }

    std::cout << orarioCorrente.format() << " Tutti i timer sono stati rimossi" << std::endl;
}

void Serra::resetAll() {
    // Spegni tutti gli impianti, rimuovi tutti i timer e reimposta l'orario
    for (auto impianto : impianti) {
        if (impianto->isAttivo()) {
            impianto->spegni(orarioCorrente);
        }
        impianto->rimuoviTimer();
    }

    orarioCorrente = Orario(0, 0);
    std::cout << orarioCorrente.format() << " Sistema reimpostato completamente" << std::endl;
}

void Serra::mostraStato() const {
    if (impianti.empty()) {
        std::cout << "Nessun impianto presente nella serra" << std::endl;
        return;
    }

    std::cout << "Stato attuale degli impianti (orario: " << orarioCorrente.format() << "):" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(15) << "Nome"
              << std::setw(15) << "Tipo"
              << std::setw(10) << "Stato"
              << std::setw(20) << "Consumo (litri)" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    for (const auto impianto : impianti) {
        std::string tipo = impianto->getInfo();
        // Estrai solo il tipo dall'informazione completa
        size_t posTipo = tipo.find(": ");
        if (posTipo != std::string::npos) {
            tipo = tipo.substr(posTipo + 2);
        }

        std::cout << std::left << std::setw(5) << impianto->getId()
                  << std::setw(15) << impianto->getNome()
                  << std::setw(15) << tipo
                  << std::setw(10) << (impianto->isAttivo() ? "Acceso" : "Spento")
                  << std::setw(20) << std::fixed << std::setprecision(2) << impianto->getConsumoTotale() << std::endl;
    }
    std::cout << "--------------------------------------------------------" << std::endl;
}

void Serra::mostraImpianto(const std::string& nome) const {
    auto it = std::find_if(impianti.begin(), impianti.end(),
                          [&nome](const Impianto* imp) {
                              return imp->getNome() == nome;
                          });

    if (it == impianti.end()) {
        std::cout << "Errore: Impianto \"" << nome << "\" non trovato" << std::endl;
        return;
    }

    const auto impianto = *it;
    std::cout << "Dettagli impianto:" << std::endl;
    std::cout << "ID: " << impianto->getId() << std::endl;
    std::cout << "Nome: " << impianto->getNome() << std::endl;
    std::cout << "Stato: " << (impianto->isAttivo() ? "Acceso" : "Spento") << std::endl;

    Orario ultimaAttivazione = impianto->getUltimaAttivazione();
    std::cout << "Ultima attivazione: ";
    if (ultimaAttivazione.getOre() != 0 || ultimaAttivazione.getMinuti() != 0) {
        std::cout << ultimaAttivazione.format() << std::endl;
    } else {
        std::cout << "Mai" << std::endl;
    }

    std::cout << "Consumo totale: " << std::fixed << std::setprecision(2) << impianto->getConsumoTotale() << " litri" << std::endl;
}

Impianto* Serra::trovaImpianto(const std::string& nome) {
    auto it = std::find_if(impianti.begin(), impianti.end(),
                          [&nome](const Impianto* imp) {
                              return imp->getNome() == nome;
                          });

    return (it != impianti.end()) ? *it : nullptr;
}

Impianto* Serra::trovaImpianto(int id) {
    auto it = std::find_if(impianti.begin(), impianti.end(),
                          [id](const Impianto* imp) {
                              return imp->getId() == id;
                          });

    return (it != impianti.end()) ? *it : nullptr;
}

void Serra::aggiornaStatiImpianti(const Orario& nuovoOrario) {
    // Per simulare il passaggio di tempo minuto per minuto
    Orario orarioSimulato = orarioCorrente;

    // Iteriamo per ogni minuto tra l'orario corrente e il nuovo orario
    while (orarioSimulato < nuovoOrario) {
        // Salva l'orario precedente per passarlo ai metodi di aggiornamento
        Orario orarioPrecedente = orarioSimulato;

        // Avanza di un minuto
        int nuoveOre = orarioSimulato.getOre();
        int nuoviMinuti = orarioSimulato.getMinuti() + 1;

        if (nuoviMinuti >= 60) {
            nuoviMinuti = 0;
            nuoveOre = (nuoveOre + 1) % 24;
        }

        orarioSimulato = Orario(nuoveOre, nuoviMinuti);

        // Controlla ogni impianto
        for (auto impianto : impianti) {
            // Memorizza lo stato precedente per rilevare cambiamenti
            bool eraAttivo = impianto->isAttivo();
            Orario vecchiaUltimaAttivazione = impianto->getUltimaAttivazione();

            // Aggiorna lo stato dell'impianto
            impianto->aggiornaStato(orarioPrecedente, orarioSimulato);
        }
    }
}