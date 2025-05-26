//AUTHOR: BRUNELLO
#include "CommandParser.h"
#include "ImpiantoDesertico.h"

ImpiantoDesertico::ImpiantoDesertico(int id, const string& nome) //costruttore: inizializza l’impianto con consumo di 0.3 litri/minuto
    : Impianto(id, nome, 0.3f) {
}

bool ImpiantoDesertico::impostaTimer(const Orario& inizio, const Orario& fine) { // Imposta il timer con orario di inizio e fine
    timerIntervallo = TimerIntervallo(inizio, fine); //salva l’intervallo di accensione nel timer
    logMessage(inizio, " Timer impostato per \"" + nome + "\"", 0);
    return true;
}

bool ImpiantoDesertico::impostaTimer2(const Orario& inizio) { //imposta un timer "infinito", cioè da un orario fino alle 23:59
    timerIntervallo = TimerIntervallo(inizio, Orario(23, 59)); //durata illimitata impostando fine a 23:59
    logMessage(inizio, " Timer impostato per \"" + nome + "\"", 0);
    return true;
}

bool ImpiantoDesertico::rimuoviTimer() { //rimuove il timer se presente
    if (timerIntervallo.has_value()) { //se il timer è stato impostato
        timerIntervallo.reset(); //lo rimuove
        return true;
    }
    return false;
}

void ImpiantoDesertico::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) { //aggiorna lo stato dell’impianto in base all’orario
    if (!timerIntervallo.has_value()) { //se non ha un timer
        return; //ritorna
    }
    bool dovrebbeEssereAttivoOra = dovrebbeEssereAttivo(orarioCorrente); //controlla se ora dovrebbe essere attivo
    bool dovrebbeEssereStatoAttivo = dovrebbeEssereAttivo(orarioPrecedente); //ccontrolla se prima era attivo

    if (dovrebbeEssereAttivoOra != dovrebbeEssereStatoAttivo) { //se lo stato è cambiato
        if (orarioPrecedente < timerIntervallo->inizio && orarioCorrente >= timerIntervallo->inizio) { //se si attraversa l'inizio del timer
            accendi(timerIntervallo->inizio); //lo accende
        }
        if (timerIntervallo->fine.getOre() != 23 || timerIntervallo->fine.getMinuti() != 59) { //se si raggiunge la fine del timer (tranne "infinito" 23:59)
            if (orarioCorrente >= timerIntervallo->fine) {
                spegni(timerIntervallo->fine);  //lo spegne
            }
        }
    }
    else if (attivo && orarioPrecedente != orarioCorrente) { //se resta acceso
        int minutiAttivi = orarioCorrente.diffInMinuti(orarioPrecedente); //calcola durata attivazione
        aggiornaConsumo(minutiAttivi); //aggiorna il consumo
    }
}

string ImpiantoDesertico::getTipo() const { //restituisce una stringa con tipo impianto
    return "Desertico";
}

bool ImpiantoDesertico::dovrebbeEssereAttivo(const Orario& orario) const { //controlla se l’impianto dovrebbe essere acceso all’orario indicato
    if (!timerIntervallo.has_value()) { //se non ha nessun timer
        return false; //non attivo
    }
    if (timerIntervallo->fine.getOre() == 23 && timerIntervallo->fine.getMinuti() == 59) { //se l'orario di fine è 23:59 (infinito)
        return orario >= timerIntervallo->inizio; //è attivo da inizio in poi
    }
    return orario.isDentroIntervallo(timerIntervallo->inizio, timerIntervallo->fine); //altrimenti, è attivo solo se siamo nell’intervallo
}