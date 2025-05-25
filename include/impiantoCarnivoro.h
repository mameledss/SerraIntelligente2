#ifndef IMPIANTOCARNIVORO_H
#define IMPIANTOCARNIVORO_H

#include "Impianto.h"
#include <optional> //include optional (variabile che può essere nulla o contenere un valore)
using namespace std;

class ImpiantoCarnivoro : public Impianto //classe che rappresenta l'impianto carnivoro, derivato da Impianto
{
private:
    optional<Orario> orarioTimer; //orario di inizio del timer, opzionale
    const int durataIrrigazione = 90; //durata irrigazione fissa di 90 minuti (1 ora e mezza)
    bool dovrebbeEssereAttivo(const Orario& orario) const; //verifica se l’impianto dovrebbe essere attivo a un certo orario
    Orario calcolaOrarioSpegnimento() const; //calcola l’orario di spegnimento basato su timer di partenza + durata
public:
    ImpiantoCarnivoro(int id, const string& nome); //costruttore con ID e nome
    bool impostaTimer(const Orario& inizio, const Orario& fine) override; //override metodo per impostare un timer con inizio/fine
    bool impostaTimer2(const Orario& inizio) override; //override metodo alternativo di timer (solo inizio)
    bool rimuoviTimer() override; //override metodo per rimuovere timer impostato
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override; //override metodo per aggiornare stato impianto confrontando orari
    string getTipo() const override; //override metodo per restituire informazioni sotto forma di stringa
};
#endif //IMPIANTOCARNIVORO_H