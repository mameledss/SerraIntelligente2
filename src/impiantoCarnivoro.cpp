//AUTHOR: GITARIC
#include "CommandParser.h"
#include "ImpiantoCarnivoro.h"

ImpiantoCarnivoro::ImpiantoCarnivoro(int id, const string& nome) //costruttore: inizializza l’impianto con consumo di 0.5 litri/minuto
    : Impianto(id, nome, 0.5f) {
}

bool ImpiantoCarnivoro::impostaTimer(const Orario& inizio, const Orario& fine) {
    return false; //non esiste il comando con due orari
}

bool ImpiantoCarnivoro::impostaTimer2(const Orario& inizio) { //imposta il timer con un solo orario
    orarioTimer = inizio; //salva orario di inizio
    logMessage(inizio, " Timer impostato per \"" + nome + "\"", 0);
    return true;
}

bool ImpiantoCarnivoro::rimuoviTimer() { //rimuove il timer se presente
    if (orarioTimer.has_value()) { //se il timer è stato impostato
        orarioTimer.reset(); //lo rimuove
        return true;
    }
    return false;
}

void ImpiantoCarnivoro::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) { //aggiorna lo stato dell’impianto in base all’orario
    if (!orarioTimer.has_value()) { //se non ha un timer
        return; //ritorna
    }
    bool dovrebbeEssereAttivoOra = dovrebbeEssereAttivo(orarioCorrente); //controlla se ora dovrebbe essere attivo
    bool dovrebbeEssereStatoAttivo = dovrebbeEssereAttivo(orarioPrecedente); //ccontrolla se prima era attivo

    if (dovrebbeEssereAttivoOra != dovrebbeEssereStatoAttivo) { //se lo stato è cambiato
        if (dovrebbeEssereAttivoOra) {
            accendi(orarioTimer.value()); //accende l’impianto
        } else {
            Orario orarioSpegnimento = calcolaOrarioSpegnimento(); //calcola ora di spegnimento
            if (orarioSpegnimento >= orarioPrecedente && orarioSpegnimento <= orarioCorrente) {
                spegni(orarioSpegnimento); //spegne se siamo nel range di tempo corretto
            }
        }
    }
    else if (attivo && orarioPrecedente != orarioCorrente) { //se resta attivo, aggiorna consumo
        int minutiAttivi = orarioCorrente.diffInMinuti(orarioPrecedente); //calcola tempo trascorso
        aggiornaConsumo(minutiAttivi); //aggiorna consumo in base ai minuti attivi
    }
}

string ImpiantoCarnivoro::getTipo() const { //restituisce una stringa con tipo impianto
    return "Carnivoro";
}

bool ImpiantoCarnivoro::dovrebbeEssereAttivo(const Orario& orario) const { //controlla se l’impianto dovrebbe essere acceso all’orario indicato
    if (!orarioTimer.has_value()) { //se non ha nessun timer
        return false; //non attivo
    }
    Orario orarioSpegnimento = calcolaOrarioSpegnimento(); //calcola fine irrigazione
    return orario.isDentroIntervallo(orarioTimer.value(), orarioSpegnimento); //verifica se siamo nel range
}

Orario ImpiantoCarnivoro::calcolaOrarioSpegnimento() const { //calcola orario di spegnimento: 2h e 30 dopo attivazione
    if (!orarioTimer.has_value()) { //se non ha timer
        return Orario(); //restituisce orario di default
    }
    int durataIrrigazione = 150; //durata 2h30m = 150 minuti
    int oreAggiuntive = durataIrrigazione / 60; //2 ore
    int minutiAggiuntivi = durataIrrigazione % 60; //30 minuti
    int nuoveOre = (orarioTimer.value().getOre() + oreAggiuntive) % 24;
    int nuoviMinuti = (orarioTimer.value().getMinuti() + minutiAggiuntivi) % 60;

    if (orarioTimer.value().getMinuti() + minutiAggiuntivi >= 60) { //se i minuti attuali + quelli aggiuntivi superano i 60
        nuoveOre = (nuoveOre + 1) % 24; //incrementa le ore
    }
    return Orario(nuoveOre, nuoviMinuti); //restituisce orario di spegnimento
}