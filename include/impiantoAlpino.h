#ifndef IMPIANTOALPINO_H
#define IMPIANTOALPINO_H

#include "Impianto.h"
#include <optional> //include optional (variabile che può essere nulla o contenere un valore)
using namespace std;

class ImpiantoAlpino : public Impianto { //classe che rappresenta l'impianto alpino, derivato da Impianto
private:
    optional<Orario> orarioTimer; //orario di inizio del timer, opzionale
    const int durataIrrigazione = 150; //durata irrigazione fissa di 150 minuti (2 ore e mezza)
    bool dovrebbeEssereAttivo(const Orario& orario) const; //verifica se l’impianto dovrebbe essere attivo a un certo orario
    Orario calcolaOrarioSpegnimento() const; //calcola l’orario di spegnimento basato su timer di partenza + durata
public:
    ImpiantoAlpino(int id, const string& nome); //costruttore con ID e nome
    bool impostaTimer(const Orario& inizio, const Orario& fine) override; //override metodo per impostare un timer con inizio/fine
    bool impostaTimer2(const Orario& inizio) override; //override metodo alternativo di timer (solo inizio)
    bool rimuoviTimer() override; //override metodo per rimuovere timer impostato
    void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) override; //override metodo per aggiornare stato impianto confrontando orari
    string getTipo() const override; //override metodo per restituire informazioni sotto forma di stringa
};
#endif //IMPIANTOALPINO_H