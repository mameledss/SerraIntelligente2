#ifndef SERRA_H
#define SERRA_H

#include <vector>
#include <string>
#include "Impianto.h"
#include "Orario.h"
using namespace std;

class Serra {
private:
    vector<Impianto*> impianti;
    Orario orarioCorrente;
    int prossimoIdImpianto;
    
public:
    // Costruttore
    Serra();
    ~Serra();

    // Metodi di gestione impianti
    bool aggiungiImpianto(const string& tipo, const string& nome);
    bool rimuoviImpianto(const string& nome);
    bool rimuoviImpianto(int id);
    
    // Metodi di controllo impianti
    bool accendiImpianto(const string& nome);
    bool accendiImpiantoOn(const string& nome);
    bool spegniImpianto(const string& nome);
    bool impostaTimer(const string& nome, const Orario& inizio, const Orario& fine);
    bool impostaTimer2(const string& nome, const Orario& inizio);
    bool rimuoviTimer(const string& nome);
    
    // Metodi di gestione del tempo
    void setOrario(const Orario& nuovoOrario);
    Orario getOrarioCorrente() const;
    void resetOrario();
    
    // Metodi di reset
    void resetTimers();
    void resetAll();
    
    // Metodi di visualizzazione
    void mostraStato() const;
    void mostraImpianto(const string& nome) const;
    
private:
    // Metodi ausiliari
    Impianto* trovaImpianto(const string& nome);
    Impianto* trovaImpianto(int id);
    void aggiornaStatiImpianti(const Orario& nuovoOrario);
};

#endif // SERRA_H