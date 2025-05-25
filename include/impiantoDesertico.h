#ifndef IMPIANTO_DESERTICO_H
#define IMPIANTO_DESERTICO_H

#include "Impianto.h"
#include <optional> //include optional (variabile che può essere nulla o contenere un valore)
using namespace std;

struct TimerIntervallo { //struct per rappresentare intervallo di tempo
    Orario inizio; //orario di inizio
    Orario fine; //e di fine
    TimerIntervallo(const Orario& inizio, const Orario& fine) : inizio(inizio), fine(fine) {} //costruttore: inizializza inizio e fine con valori passati
};

class ImpiantoDesertico : public Impianto { //classe che rappresenta l'impianto desertico, derivato da Impianto
private:
    optional<TimerIntervallo> timerIntervallo; //intervallo di tempo tra inizio e fine
    bool dovrebbeEssereAttivo(const Orario& orario) const; //verifica se l’impianto dovrebbe essere attivo a un certo orario
public:
    ImpiantoDesertico(int id, const string& nome); //costruttore con ID e nome
    bool impostaTimer(const Orario& inizio, const Orario& fine) override; //override metodo per impostare un timer con inizio/fine
    bool impostaTimer2(const Orario& inizio) override; //override metodo alternativo di timer (solo inizio)
    bool rimuoviTimer() override; //override metodo per rimuovere timer impostato
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override; //override metodo per aggiornare stato impianto confrontando orari
    string getTipo() const override; //override metodo per restituire informazioni sotto forma di stringa
};
#endif // IMPIANTO_DESERTICO_H