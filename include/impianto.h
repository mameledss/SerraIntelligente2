#ifndef IMPIANTO_H
#define IMPIANTO_H

#include <string>
#include <memory>
#include "Orario.h"

class Impianto {
protected:
    int id;
    std::string nome;
    bool attivo;
    Orario ultimaAttivazione;
    float consumoPerMinuto; // consumo d'acqua in litri per minuto
    float consumoTotale;    // consumo totale dall'inizio
    
public:
    // Costruttore
    Impianto(int id, const std::string& nome, float consumoPerMinuto);
    
    // Distruttore virtuale
    virtual ~Impianto() = default;
    
    // Metodi accessori
    int getId() const;
    std::string getNome() const;
    bool isAttivo() const;
    Orario getUltimaAttivazione() const;
    float getConsumoTotale() const;
    
    // Metodi per attivare/disattivare l'impianto
    virtual bool accendi(const Orario& orarioCorrente);
    virtual bool spegni(const Orario& orarioCorrente);
    
    // Metodo per impostare il timer (diverso per i vari tipi di impianti)
    virtual bool impostaTimer(const Orario& inizio, const Orario& fine) = 0;
    
    // Metodo per rimuovere il timer
    virtual bool rimuoviTimer() = 0;
    
    // Metodo per aggiornare lo stato in base all'orario corrente
    virtual void aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) = 0;
    
    // Metodo per ottenere informazioni sull'impianto
    virtual std::string getInfo() const = 0;
    
    // Factory method per creare nuovi impianti
    static Impianto* creaImpianto(int id, const std::string& tipo, const std::string& nome);
    
protected:
    // Metodo per aggiornare il consumo
    void aggiornaConsumo(int minutiAttivi);
};

#endif // IMPIANTO_H