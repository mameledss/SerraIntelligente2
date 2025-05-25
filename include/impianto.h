#ifndef IMPIANTO_H
#define IMPIANTO_H

#include <string>
#include "Orario.h"
using namespace std;

class Impianto { //classe base per rappresentare un impianto
protected:
    int id; //ID univoco dell’impianto
    string nome; //nome dell’impianto
    bool attivo; //stato (attivo/disattivo)
    Orario ultimaAttivazione; //orario ultima attivazione
    float consumoPerMinuto; //consumo in litri per ogni minuto di funzionamento
    float consumoTotale; //consumo totale accumulato
    void aggiornaConsumo(int minutiAttivi); //metodo per aggiornare il consumo totale dato un tempo di attività
public:
    Impianto(int id, const string& nome, float consumoPerMinuto); //costruttore che accetta ID, nome e consumo per minuto
    virtual ~Impianto() = default; //distruttore virtuale
    int getId() const; //metodo per restituire ID dell’impianto
    string getNome() const; //per estituire il nome
    bool isAttivo() const; //true se è attivo
    Orario getUltimaAttivazione() const; //orario dell’ultima attivazione
    float getConsumoTotale() const; //consumo totale
    virtual bool accendi(const Orario& orarioCorrente); //accende l’impianto
    virtual bool spegni(const Orario& orarioCorrente); //e lo spegne
    virtual bool impostaTimer(const Orario& inizio, const Orario& fine) = 0; //imposta un timer (puro, ogni  impianto lo implementa a modo suo)
    virtual bool impostaTimer2(const Orario& inizio) = 0; //alternativa per impostare timer (solo inizio)
    virtual bool rimuoviTimer() = 0; //rimuove il timer
    virtual void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) = 0; //aggiorna stato impianto in base all’orario
    virtual string getTipo() const = 0; //restituisce informazioni impianto in formato stringa
    static Impianto* creaImpianto(int id, const string& tipo, const string& nome); //metodo statico per creare un nuovo impianto di tipo specificato
};
#endif // IMPIANTO_H