#include "../include/ImpiantoTropicale.h"
#include <sstream>
#include <iomanip>

ImpiantoTropicale::ImpiantoTropicale(int id, const std::string& nome)
    : Impianto(id, nome, 0.5f) { // Consumo di 0.5 litri al minuto
}

bool ImpiantoTropicale::impostaTimer(const Orario& inizio, const Orario& fine) {
    // Per l'impianto tropicale, ignoriamo l'orario di fine poiché è automatico
    orarioTimer = inizio;
    std::cout << inizio.format() << " Timer impostato per \"" << nome << "\"" << std::endl;
    return true;
}

bool ImpiantoTropicale::rimuoviTimer() {
    if (orarioTimer.has_value()) {
        orarioTimer.reset();
        return true;
    }
    return false;
}

void ImpiantoTropicale::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) {
    if (!orarioTimer.has_value()) {
        return; // Nessun timer impostato
    }
    
    // Verifica se l'impianto dovrebbe essere acceso all'orario corrente
    bool dovrebbeEssereAttivoOra = dovrebbeEssereAttivo(orarioCorrente);
    
    // Verifica se l'impianto dovrebbe essere stato acceso all'orario precedente
    bool dovrebbeEssereStatoAttivo = dovrebbeEssereAttivo(orarioPrecedente);
    
    // Se c'è stato un cambiamento nello stato desiderato
    if (dovrebbeEssereAttivoOra != dovrebbeEssereStatoAttivo) {
        // Calcola l'orario esatto di attivazione o disattivazione
        if (dovrebbeEssereAttivoOra) {
            // Dobbiamo accendere l'impianto - calcola l'orario di attivazione
            accendi(orarioTimer.value());
        } else {
            // Dobbiamo spegnere l'impianto - calcola l'orario di spegnimento
            Orario orarioSpegnimento = calcolaOrarioSpegnimento();
            if (orarioSpegnimento >= orarioPrecedente && orarioSpegnimento <= orarioCorrente) {
                spegni(orarioSpegnimento);
            }
        }
    }
    // Se l'impianto è attivo, aggiorna il consumo
    else if (attivo && orarioPrecedente != orarioCorrente) {
        int minutiAttivi = orarioCorrente.diffInMinuti(orarioPrecedente);
        aggiornaConsumo(minutiAttivi);
    }
}

std::string ImpiantoTropicale::getInfo() const {
    std::ostringstream oss;
    oss << "Impianto Tropicale \"" << nome << "\" (ID: " << id << ")" << std::endl;
    oss << "  Stato: " << (attivo ? "Attivo" : "Disattivo") << std::endl;
    oss << "  Ultima attivazione: " << (attivo ? ultimaAttivazione.toString() : "N/A") << std::endl;
    oss << "  Consumo totale: " << std::fixed << std::setprecision(2) << consumoTotale << " litri" << std::endl;
    oss << "  Timer: " << (orarioTimer.has_value() ? orarioTimer.value().toString() : "Non impostato") << std::endl;
    return oss.str();
}

bool ImpiantoTropicale::dovrebbeEssereAttivo(const Orario& orario) const {
    if (!orarioTimer.has_value()) {
        return false;
    }
    
    // Calcola l'orario di spegnimento
    Orario orarioSpegnimento = calcolaOrarioSpegnimento();
    
    // Verifica se l'orario corrente è compreso tra l'orario di attivazione e quello di spegnimento
    return orario.isDentroIntervallo(orarioTimer.value(), orarioSpegnimento);
}

Orario ImpiantoTropicale::calcolaOrarioSpegnimento() const {
    if (!orarioTimer.has_value()) {
        return Orario();
    }
    
    // Calcola l'orario di spegnimento (orario timer + durata irrigazione)
    int oreAggiuntive = durataIrrigazione / 60;
    int minutiAggiuntivi = durataIrrigazione % 60;
    
    int nuoveOre = (orarioTimer.value().getOre() + oreAggiuntive) % 24;
    int nuoviMinuti = (orarioTimer.value().getMinuti() + minutiAggiuntivi) % 60;
    
    // Se i minuti eccedono 60, aggiustiamo le ore
    if (orarioTimer.value().getMinuti() + minutiAggiuntivi >= 60) {
        nuoveOre = (nuoveOre + 1) % 24;
    }
    
    return Orario(nuoveOre, nuoviMinuti);
}