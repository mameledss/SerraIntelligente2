#include <iostream>
#include <algorithm>
#include <iomanip>
#include "Serra.h"
#include "CommandParser.h"
using namespace std;

Serra::Serra() : orarioCorrente(0, 0), prossimoIdImpianto(1) {} //costruttore classe Serra: inizializza l'orario a 00:00 e l'id degli impianti a 1

Serra::~Serra() { //distruttore: libera la memoria degli impianti creati dinamicamente
    for (Impianto* impianto : impianti) { //per ogni impianto in impianti
        delete impianto; //dealloca la memoria relativa all'impianto
    }
    impianti.clear(); //svuota il vettore
}

bool Serra::aggiungiImpianto(const string& tipo, const string& nome) { //aggiunge un impianto alla serra
    if (trovaImpianto(nome) != nullptr) { //se esiste già un impianto con quel nome
        logMessage(orarioCorrente, "Errore: Esiste gia' un impianto con il nome \"" + nome + "\"", 1);
        return false;
    }
    Impianto* nuovoImpianto = Impianto::creaImpianto(prossimoIdImpianto, tipo, nome); //crea un nuovo impianto con ID, tipo e nome specificati
    if (!nuovoImpianto) {
        logMessage(orarioCorrente, "Errore: Tipo di impianto \"" + tipo + "\" non riconosciuto", 1);
        return false;
    }
    impianti.push_back(nuovoImpianto); //aggiunge l’impianto al vettore
    prossimoIdImpianto++; //incrementa l’ID per il prossimo
    logMessage(orarioCorrente, "Impianto \"" + nome + "\" aggiunto con successo", 0);
    return true;
}

bool Serra::rimuoviImpianto(const string& nome) { //rimuove un impianto cercandolo per nome
    for (int i = 0; i < impianti.size(); ++i) { //per ogni elemento in impianto
        if (impianti[i]->getNome() == nome) { //se il nome dell'impianto ad i corrisponde al nome passato come parametro
            delete impianti[i]; //libera la memoria dell'impianto
            impianti.erase(impianti.begin() + i); //rimuove l'elemento dal vettore
            logMessage(orarioCorrente, "Impianto \"" + nome + "\" rimosso con successo", 0);
            return true;
        }
    }
    logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
    return false;
}

bool Serra::accendiImpianto(const string& nome) { //accende un impianto specifico
    Impianto* impianto = trovaImpianto(nome); //si salva l'impianto cercandolo per nome
    if (!impianto) { //se l'impianto non esiste
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->isAttivo()) { //se l'impianto è già acceso
        logMessage(orarioCorrente, "Impianto \"" + nome + "\" e' già acceso", 1);
        return true;
    }
    return impianto->accendi(orarioCorrente);
}

bool Serra::accendiImpiantoOn(const string& nome) { //accende un impianto (per comando on)
    Impianto* impianto = trovaImpianto(nome); //si salva l'impianto cercandolo per nome
    if (!impianto) { //se l'impianto non esiste
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->impostaTimer2(orarioCorrente)) { //se riesce ad impostare il timer correttamente
        if (!impianto->isAttivo()) { //se non è attivo
                impianto->accendi(orarioCorrente); //lo accende
        }
        return true;
    }
    return false;
}

bool Serra::spegniImpianto(const string& nome) { //spegne un impianto
    Impianto* impianto = trovaImpianto(nome); //salva l'impianto
    if (!impianto) { //se l'impianto non esiste
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (!impianto->isAttivo()) { //se l'impianto è spento
        logMessage(orarioCorrente, "Impianto \"" + nome + "\" e' già spento", 1);
        return true;
    }
    if (impianto->spegni(orarioCorrente)) { //se l'impianto si spegne correttamente
        impianto->rimuoviTimer(); //rimuove il timer
        return true;
    }
    return false;
}

bool Serra::impostaTimer(const string& nome, const Orario& inizio, const Orario& fine) { //imposta un timer con inizio e fine
    Impianto* impianto = trovaImpianto(nome); //trova l'impianto in base al nome
    if (!impianto) { //se l'impianto non esiste
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    string tipo = impianto->getTipo(); //ottiene il tipo
    if (tipo=="Tropicale" || tipo=="Carnivoro" || tipo=="Alpino") { //se impianti automatici
        logMessage(orarioCorrente, "Errore: Gli impianti automatici non supportano timer con intervallo specifico", 1);
        return false;
    }
    if (impianto->impostaTimer(inizio, fine)) //imposta il timer se possibile
        return true;
    else {
        logMessage(orarioCorrente, "Errore: Impossibile impostare il timer per l'impianto \"" + nome + "\"", 1);
        return false;
    }
}

bool Serra::impostaTimer2(const string& nome, const Orario& inizio) { //imposta un timer data l'ora di inizio
    Impianto* impianto = trovaImpianto(nome); //trova l'impianto
    if (!impianto) { //se l'impianto non esiste
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->impostaTimer2(inizio)) { //imposta il timer se possibile
        return true;
    } else {
        logMessage(orarioCorrente, "Errore: Impossibile impostare il timer per l'impianto \"" + nome + "\"", 1);
        return false;
    }
}

bool Serra::rimuoviTimer(const string& nome) { //rimuove il timer da un impianto
    Impianto* impianto = trovaImpianto(nome); //si salva l'impianto
    if (!impianto) { //se non trova l'impianto
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return false;
    }
    if (impianto->rimuoviTimer()) { //rimuova il timer se possibile
        logMessage(orarioCorrente, "Timer rimosso per \"" + nome + "\"", 0);
        return true;
    } else {
        logMessage(orarioCorrente, "Errore: Nessun timer impostato per l'impianto \"" + nome + "\"", 1);
        return false;
    }
}

void Serra::setOrario(const Orario& nuovoOrario) { //imposta un nuovo orario e aggiorna lo stato degli impianti
    if (nuovoOrario < orarioCorrente) {
        logMessage(orarioCorrente, "Errore: Non e' possibile impostare un orario precedente a quello attuale", 1);
        return;
    }
    aggiornaStatiImpianti(nuovoOrario);
    orarioCorrente = nuovoOrario;
    logMessage(orarioCorrente, " L'orario attuale e' " + orarioCorrente.format(), 0);
}

Orario Serra::getOrarioCorrente() const { //ritorna l’orario corrente
    return orarioCorrente;
}

void Serra::resetOrario() { //reimposta l’orario e spegne tutti gli impianti
    for (Impianto* impianto : impianti) { //per ogni impianto in impianti
        if (impianto->isAttivo()) { //se è acceso
            impianto->spegni(orarioCorrente); //lo spegne
        }
        impianto->rimuoviTimer(); //rimuove il timer associato
    }
    orarioCorrente = Orario(0, 0); //riporta l'orario corrente a mezzanotte
    logMessage(orarioCorrente, " Orario reimpostato a " + orarioCorrente.format(), 0);
}

void Serra::resetTimers() { //rimuove tutti i timer dagli impianti
    for (Impianto* impianto : impianti) { //per ogni impianto in impianti
        impianto->rimuoviTimer(); //rimuove il timer associato ad ogni impianto
    }
    logMessage(orarioCorrente, " Tutti i timer sono stati rimossi", 0);
}

void Serra::resetAll() { //reimposta tutto: impianti, orario e timer
    for (Impianto* impianto : impianti) { //per ogni impianto in impianti
        if (impianto->isAttivo()) { //se l'impianto è attivo
            impianto->spegni(orarioCorrente); //lo spegne
        }
        impianto->rimuoviTimer(); //rimuove il timer
    }
    orarioCorrente = Orario(0, 0); //imposta l'ora a mezzanotte
    logMessage(orarioCorrente, " Sistema reimpostato completamente", 0);
}

void Serra::mostraStato() const { //mostra lo stato attuale di tutti gli impianti
    if (impianti.empty()) { //se impianti è vuoto
        logMessage(orarioCorrente, "Nessun impianto presente nella serra", 1);
        return;
    }
    logMessage(orarioCorrente, "Stato attuale degli impianti",0);
    cout << "--------------------------------------------------------" << endl;
    cout << left << setw(5) << "ID"
         << setw(15) << "Nome"
         << setw(15) << "Tipo"
         << setw(10) << "Stato"
         << setw(20) << "Consumo (litri)" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (const Impianto* impianto : impianti) {
        string tipo = impianto->getTipo();

        cout << left << setw(5) << impianto->getId()
             << setw(15) << impianto->getNome()
             << setw(15) << tipo
             << setw(10) << (impianto->isAttivo() ? "Acceso" : "Spento")
             << setw(20) << fixed << setprecision(2) << impianto->getConsumoTotale() << endl;
    }
    cout << "--------------------------------------------------------" << endl;
}

void Serra::mostraImpianto(const string& nome) const { //mostra informazioni dettagliate di un impianto
    Impianto* impianto = nullptr; //inizializza l'impianto come null

    for (Impianto* imp : impianti) { //per ogni impianto in impianti
        if (imp->getNome() == nome) { //se il nome dell'impianto corrisponde a quello passato come parametro
            impianto = imp; //viene salvato in impianto
            break;
        }
    }
    if (!impianto) { //se l'impianto non esiste
        logMessage(orarioCorrente, "Errore: Impianto \"" + nome + "\" non trovato", 1);
        return;
    }
    string tipo = impianto->getTipo(); //viene ottenuto il tipo

    logMessage(orarioCorrente, "Dettagli impianto \"" + nome + "\"", 0);
    cout << "--------------------------------------------------------" << endl;
    cout << left << setw(20) << "ID:" << impianto->getId() << endl;
    cout << left << setw(20) << "Nome:" << impianto->getNome() << endl;
    cout << left << setw(20) << "Tipo:" << tipo << endl;
    cout << left << setw(20) << "Stato:" << (impianto->isAttivo() ? "Acceso" : "Spento") << endl;

    Orario ultimaAttivazione = impianto->getUltimaAttivazione();
    cout << left << setw(20) << "Ultima attivazione:";
    if (ultimaAttivazione.getOre() != 0 || ultimaAttivazione.getMinuti() != 0)
        cout << ultimaAttivazione.format() << endl;
    else
        cout << "Mai" << endl;

    cout << left << setw(20) << "Consumo totale:" << fixed << setprecision(2)
         << impianto->getConsumoTotale() << " litri" << endl;
    cout << "--------------------------------------------------------" << endl;
}

Impianto* Serra::trovaImpianto(const string& nome) { //trova un impianto per nome
    for (Impianto* impianto : impianti) { //per ogni impianto in impianti
        if (impianto->getNome() == nome) { //se il nome dell'impianto corrisponde a quello passato come parametro
            return impianto; //ritorna l'impianto
        }
    }
    return nullptr;
}

Impianto* Serra::trovaImpianto(int id) { //trova un impianto per ID
    for (Impianto* impianto : impianti) { //per ogni impianto in impianti
        if (impianto->getId() == id) { //se l'ID dell'impianto corrisponde a quello passato come parametro
            return impianto; //ritorna l'impianto
        }
    }
    return nullptr;
}

void Serra::aggiornaStatiImpianti(const Orario& nuovoOrario) { //avanza l'orario minuto per minuto e aggiorna gli impianti
    Orario orarioSim = orarioCorrente; //copia dell'orario corrente della serra

    while (orarioSim < nuovoOrario) { //finché non raggiunge il nuovo orario passato come parametro
        Orario orarioPrecedente = orarioSim; //salva l'orario attuale prima di incrementarlo (per aggiornaStato)
        int nuoveOre = orarioSim.getOre(); //ottiene le ore dell'orario simulato corrente
        int nuoviMinuti = orarioSim.getMinuti() + 1; //incrementa i minuti di 1
        if (nuoviMinuti >= 60) { //se i minuti hanno superato 59
            nuoviMinuti = 0; //resetta i minuti a 0
            nuoveOre = (nuoveOre + 1) % 24; //incrementa le ore
        }
        orarioSim = Orario(nuoveOre, nuoviMinuti); //crea il nuovo orario simulato con ore e minuti aggiornati
        if (orarioSim.getMinuti() == 0) //se si raggiunge una nuova ora (minuti = 0)
            logMessage(orarioSim, "Ora attuale: " + orarioSim.format(), 0);

        for (Impianto* impianto : impianti) //per ogni impianto in impianti
            impianto->aggiornaStato(orarioPrecedente, orarioSim); //aggiorna stato di ogni impianto passando orario precedente e attuale
    }
}