#include <sstream>
#include <iomanip>
#include "CommandParser.h"
#include "ImpiantoCarnivoro.h"
#include "logger.h"
ImpiantoCarnivoro::ImpiantoCarnivoro(int id, const string& nome)
    : Impianto(id, nome, 0.5f) { // Consumo di 0.5 litri al minuto
}

bool ImpiantoCarnivoro::impostaTimer(const Orario& inizio, const Orario& fine) {
    // Per l'impianto tropicale, non esiste il comando con due orari
    // Quindi questo metodo non dovrebbe essere chiamato, ma lo manteniamo per compatibilità
    return false;
}

bool ImpiantoCarnivoro::impostaTimer2(const Orario& inizio) {
    orarioTimer = inizio;
    cout << "[" << inizio.format() << "] Timer impostato per \"" << nome << "\"" << std::endl;
    return true;
}

bool ImpiantoCarnivoro::rimuoviTimer() {
    if (orarioTimer.has_value()) {
        orarioTimer.reset();
        return true;
    }
    return false;
}

void ImpiantoCarnivoro::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) {
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

string ImpiantoCarnivoro::getInfo() const {
    ostringstream oss;
    oss << "Impianto Carnivoro \"" << nome << "\" (ID: " << id << ")" << endl;
    oss << "  Stato: " << (attivo ? "Attivo" : "Disattivo") << endl;
    oss << "  Ultima attivazione: " << (attivo ? ultimaAttivazione.toString() : "Mai") << endl;
    oss << "  Consumo totale: " << fixed << setprecision(2) << consumoTotale << " litri" << endl;
    oss << "  Timer: " << (orarioTimer.has_value() ? orarioTimer.value().toString() : "Non impostato") << endl;
    return oss.str();
}

bool ImpiantoCarnivoro::dovrebbeEssereAttivo(const Orario& orario) const {
    if (!orarioTimer.has_value()) {
        return false;
    }
    // Calcola l'orario di spegnimento
    Orario orarioSpegnimento = calcolaOrarioSpegnimento();

    // Verifica se l'orario corrente è compreso tra l'orario di attivazione e quello di spegnimento
    return orario.isDentroIntervallo(orarioTimer.value(), orarioSpegnimento);
}

Orario ImpiantoCarnivoro::calcolaOrarioSpegnimento() const {
    if (!orarioTimer.has_value()) {
        return Orario();
    }

    // Calcola l'orario di spegnimento (orario timer + 2 ore e 30 minuti)
    int durataIrrigazione = 150; // 2 ore e 30 minuti = 150 minuti
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