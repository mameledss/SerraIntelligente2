#include "../include/ImpiantoDesertico.h"
#include <sstream>
#include <iomanip>
#include <iostream>

ImpiantoDesertico::ImpiantoDesertico(int id, const std::string& nome)
    : Impianto(id, nome, 0.3f) { // Consumo di 0.3 litri al minuto
}

bool ImpiantoDesertico::impostaTimer(const Orario& inizio, const Orario& fine) {
    timerIntervallo = TimerIntervallo(inizio, fine);
    std::cout << inizio.format() << " Timer impostato per \"" << nome << "\"" << std::endl;
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

        // Se attraversiamo l'orario di fine
        if (orarioPrecedente < timerIntervallo->fine && orarioCorrente >= timerIntervallo->fine) {
            spegni(timerIntervallo->fine);
        }
    }
    // Se l'impianto è attivo, aggiorna il consumo
    else if (attivo && orarioPrecedente != orarioCorrente) {
        int minutiAttivi = orarioCorrente.diffInMinuti(orarioPrecedente);
        aggiornaConsumo(minutiAttivi);
    }
}

std::string ImpiantoDesertico::getInfo() const {
    std::ostringstream oss;
    oss << "Tipo: Desertico" << std::endl;
    return oss.str();
}

bool ImpiantoDesertico::dovrebbeEssereAttivo(const Orario& orario) const {
    if (!timerIntervallo.has_value()) {
        return false;
    }
    
    return orario.isDentroIntervallo(timerIntervallo->inizio, timerIntervallo->fine);
}