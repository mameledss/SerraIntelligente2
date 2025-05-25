#ifndef SERRA_H
#define SERRA_H

#include <vector>
#include <string>
#include "Impianto.h"
#include "Orario.h"
using namespace std;

class Serra { //classe Serra per la gestione globale degli impianti
private:
    vector<Impianto*> impianti; //vettore di puntatori a impianti presenti nella serra
    Orario orarioCorrente; //orario attuale
    int prossimoIdImpianto; //ID progressivo da assegnare al prossimo impianto
    Impianto* trovaImpianto(const string& nome); //cerca e ritorna puntatore all'impianto con il nome dato
    Impianto* trovaImpianto(int id); //cerca e ritorna puntatore all'impianto con l'ID dato
    void aggiornaStatiImpianti(const Orario& nuovoOrario); // Aggiorna lo stato di tutti gli impianti in base al nuovo orario
public:
    Serra(); //costruttore di default della serra
    ~Serra(); //distruttore per liberare memoria degli impianti creati dinamicamente
    bool aggiungiImpianto(const string& tipo, const string& nome); //aggiunge impianto del tipo specificato con un nome
    bool rimuoviImpianto(const string& nome); //rimuove un impianto cercandolo per nome
    bool accendiImpianto(const string& nome); //accende un impianto
    bool accendiImpiantoOn(const string& nome); //accende un impianto (tramite on)
    bool spegniImpianto(const string& nome); //spegne l'impianto specificato
    bool impostaTimer(const string& nome, const Orario& inizio, const Orario& fine); //imposta un timer di accensione e spegnimento
    bool impostaTimer2(const string& nome, const Orario& inizio); //imposta un timer con solo orario di accensione
    bool rimuoviTimer(const string& nome); //rimuove il timer associato all'impianto
    void setOrario(const Orario& nuovoOrario); //imposta un nuovo orario corrente
    Orario getOrarioCorrente() const; //restituisce l'orario corrente
    void resetOrario(); //reimposta l'orario a 00:00
    void resetTimers(); //rimuove tutti i timer impostati sugli impianti
    void resetAll(); //resetta lo stato della serra: orario, impianti e timer
    void mostraStato() const; //mostra lo stato di tutti gli impianti nella serra
    void mostraImpianto(const string& nome) const; //mostra le informazioni di un impianto specifico
};
#endif // SERRA_H // Fine della protezione contro inclusione multipla