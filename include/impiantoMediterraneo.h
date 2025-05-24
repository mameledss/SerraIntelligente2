#ifndef IMPIANTOMEDITERRANEO_H
#define IMPIANTOMEDITERRANEO_H

#include "Impianto.h"
using namespace std;

class ImpiantoMediterraneo : public Impianto {
private:
    float temperatura; // Temperatura corrente in gradi Celsius

    // Metodi privati per aggiornare la temperatura
    void aggiornaTemperaturaSpento(int minutiTrascorsi);
    void aggiornaTemperaturaAcceso(int minutiTrascorsi, const Orario& orarioCorrente);

public:
    ImpiantoMediterraneo(int id, const string& nome);

    // Override dei metodi virtuali della classe base
    bool impostaTimer(const Orario& inizio, const Orario& fine) override;
    bool impostaTimer2(const Orario& inizio) override;
    bool rimuoviTimer() override;
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override;
    string getInfo() const override;

    // Metodi specifici per l'impianto mediterraneo
    float getTemperatura() const;
    bool dovrebbeEssereAttivo(const Orario& orario) const;
};

#endif //IMPIANTOMEDITERRANEO_H
