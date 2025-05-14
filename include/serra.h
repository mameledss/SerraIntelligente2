#ifndef SERRA_H
#define SERRA_H

#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Impianto.h"
#include "Orario.h"

class Serra {
private:
    std::vector<Impianto*> impianti;
    Orario orarioCorrente;
    int prossimoIdImpianto;
    
public:
    // Costruttore
    Serra();
    ~Serra();

    // Metodi di gestione impianti
    bool aggiungiImpianto(const std::string& tipo, const std::string& nome);
    bool rimuoviImpianto(const std::string& nome);
    bool rimuoviImpianto(int id);
    
    // Metodi di controllo impianti
    bool accendiImpianto(const std::string& nome);
    bool spegniImpianto(const std::string& nome);
    bool impostaTimer(const std::string& nome, const Orario& inizio, const Orario& fine);
    bool rimuoviTimer(const std::string& nome);
    
    // Metodi di gestione del tempo
    void setOrario(const Orario& nuovoOrario);
    Orario getOrarioCorrente() const;
    void resetOrario();
    
    // Metodi di reset
    void resetTimers();
    void resetAll();
    
    // Metodi di visualizzazione
    void mostraStato() const;
    void mostraImpianto(const std::string& nome) const;
    
private:
    // Metodi ausiliari
    Impianto* trovaImpianto(const std::string& nome);
    Impianto* trovaImpianto(int id);
    void aggiornaStatiImpianti(const Orario& nuovoOrario);
};

#endif // SERRA_H