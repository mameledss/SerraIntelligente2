#ifndef IMPIANTOMEDITERRANEO_H
#define IMPIANTOMEDITERRANEO_H

#include "Impianto.h"
using namespace std;

class ImpiantoMediterraneo : public Impianto { //classe per rappresentare l'impianto mediterraneo derivata da Impianto
private:
    float temperatura; //temperatura corrente
    void aggiornaTemperaturaSpento(int minutiTrascorsi); //metodo per aggiornare la temperatura a impianto spento
    void aggiornaTemperaturaAcceso(int minutiTrascorsi, const Orario& orarioCorrente); //metodo per aggiornare la temperatura a impianto acceso
public:
    ImpiantoMediterraneo(int id, const string& nome); //costruttore con ID e nome
    bool impostaTimer(const Orario& inizio, const Orario& fine) override; //override metodo per impostare un timer con inizio/fine
    bool impostaTimer2(const Orario& inizio) override; //override metodo alternativo di timer (solo inizio)
    bool rimuoviTimer() override; //override metodo per rimuovere timer impostato
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override; //override metodo per aggiornare stato impianto confrontando orari
    string getTipo() const override; //override metodo per restituire informazioni sotto forma di stringa
    float getTemperatura() const; //metodo per ritornare temperatura corrente
};
#endif // IMPIANTOMEDITERRANEO_H