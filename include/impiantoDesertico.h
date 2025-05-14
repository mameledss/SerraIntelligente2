#ifndef IMPIANTO_DESERTICO_H
#define IMPIANTO_DESERTICO_H

#include "Impianto.h"
#include <optional>

using namespace std;

struct TimerIntervallo {
    Orario inizio;
    Orario fine;
    
    TimerIntervallo(const Orario& inizio, const Orario& fine) 
        : inizio(inizio), fine(fine) {}
};

class ImpiantoDesertico : public Impianto {
private:
    optional<TimerIntervallo> timerIntervallo;
    
public:
    // Costruttore
    ImpiantoDesertico(int id, const string& nome);
    
    // Implementazione dei metodi virtuali
    bool impostaTimer(const Orario& inizio, const Orario& fine) override;
    bool rimuoviTimer() override;
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override;
    string getInfo() const override;
    
private:
    // Metodo per verificare se l'impianto dovrebbe essere attivo a un determinato orario
    bool dovrebbeEssereAttivo(const Orario& orario) const;
};

#endif // IMPIANTO_DESERTICO_H