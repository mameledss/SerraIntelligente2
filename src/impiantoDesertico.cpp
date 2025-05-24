#include <sstream>
#include <iomanip>
#include <iostream>
#include "CommandParser.h"
#include "ImpiantoDesertico.h"

ImpiantoDesertico::ImpiantoDesertico(int id, const string& nome)
    : Impianto(id, nome, 0.3f) { // Consumo di 0.3 litri al minuto
}

bool ImpiantoDesertico::impostaTimer(const Orario& inizio, const Orario& fine) {
    timerIntervallo = TimerIntervallo(inizio, fine);
    cout << inizio.format() << " Timer impostato per \"" << nome << "\"" << endl;
    return true;
}

bool ImpiantoDesertico::impostaTimer2(const Orario& inizio) {
    // Per l'impianto desertico, timer semplice significa accensione all'infinito
    // Impostiamo un orario di fine molto lontano (23:59) per simulare l'infinito
    timerIntervallo = TimerIntervallo(inizio, Orario(23, 59));
    logMessage(inizio, " Timer impostato per \"" + nome + "\"", 0);
    return true;
}

bool ImpiantoDesertico::rimuoviTimer() {
    if (timerIntervallo.has_value()) {
        timerIntervallo.reset();
        return true;
    }
    return false;
}

void ImpiantoDesertico::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) {
    if (!timerIntervallo.has_value()) {
        return; // Nessun timer impostato
    }
    // Verifica lo stato all'orario corrente
    bool dovrebbeEssereAttivoOra = dovrebbeEssereAttivo(orarioCorrente);

    // Verifica lo stato all'orario precedente
    bool dovrebbeEssereStatoAttivo = dovrebbeEssereAttivo(orarioPrecedente);

    // Se è cambiato lo stato desiderato
    if (dovrebbeEssereAttivoOra != dovrebbeEssereStatoAttivo) {
        // Se attraversiamo l'orario di inizio
        if (orarioPrecedente < timerIntervallo->inizio && orarioCorrente >= timerIntervallo->inizio) {
            accendi(timerIntervallo->inizio);
        }

        // Se attraversiamo l'orario di fine (solo se non è 23:59 che rappresenta infinito)
        if (timerIntervallo->fine.getOre() != 23 || timerIntervallo->fine.getMinuti() != 59) {
            if (orarioCorrente >= timerIntervallo->fine) {
                spegni(timerIntervallo->fine);
            }
        }
    }
    // Se l'impianto è attivo, aggiorna il consumo
    else if (attivo && orarioPrecedente != orarioCorrente) {
        int minutiAttivi = orarioCorrente.diffInMinuti(orarioPrecedente);
        aggiornaConsumo(minutiAttivi);
    }
}

string ImpiantoDesertico::getInfo() const {
    ostringstream oss;
    oss << "Impianto Desertico \"" << nome << "\" (ID: " << id << ")" << endl;
    oss << "  Stato: " << (attivo ? "Attivo" : "Disattivo") << endl;
    oss << "  Ultima attivazione: " << (attivo ? ultimaAttivazione.toString() : "Mai") << endl;
    oss << "  Consumo totale: " << fixed << setprecision(2) << consumoTotale << " litri" << endl;
    return oss.str();
}

bool ImpiantoDesertico::dovrebbeEssereAttivo(const Orario& orario) const {
    if (!timerIntervallo.has_value()) {
        return false;
    }
    // Se l'orario di fine è 23:59, consideriamo che l'impianto rimane acceso fino alla fine del giorno
    if (timerIntervallo->fine.getOre() == 23 && timerIntervallo->fine.getMinuti() == 59) {
        return orario >= timerIntervallo->inizio;
    }
    return orario.isDentroIntervallo(timerIntervallo->inizio, timerIntervallo->fine);
}