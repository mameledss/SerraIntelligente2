#ifndef IMPIANTO_TROPICALE_H
#define IMPIANTO_TROPICALE_H

#include "Impianto.h"
#include <optional>
using namespace std;

class ImpiantoTropicale : public Impianto {
private:
    std::optional<Orario> orarioTimer;
    const int durataIrrigazione = 150; // Durata in minuti (2.5 ore)
    
public:
    // Costruttore
    ImpiantoTropicale(int id, const string& nome);
    
    // Implementazione dei metodi virtuali
    bool impostaTimer(const Orario& inizio, const Orario& fine) override;

    bool impostaTimer2(const Orario& inizio) override;

    bool rimuoviTimer() override;
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override;
    string getInfo() const override;
    
private:
    // Metodo per verificare se l'impianto dovrebbe essere attivo a un determinato orario
    bool dovrebbeEssereAttivo(const Orario& orario) const;
    
    // Metodo per calcolare l'orario di spegnimento
    Orario calcolaOrarioSpegnimento() const;
};

#endif // IMPIANTO_TROPICALE_H