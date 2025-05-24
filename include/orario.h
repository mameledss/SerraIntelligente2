#ifndef ORARIO_H
#define ORARIO_H

#include <string>
#include <iostream>
using namespace std;

class Orario {
private:
    int ore;
    int minuti;
    
public:
    // Costruttori
    Orario();
    Orario(int ore, int minuti);
    Orario(const string& orarioStr);
    
    // Metodi per ottenere ore e minuti
    int getOre() const;
    int getMinuti() const;
    
    // Metodi per impostare ore e minuti
    void setOre(int ore);
    void setMinuti(int minuti);
    
    // Confronto tra orari
    bool operator<(const Orario& altro) const;
    bool operator<=(const Orario& altro) const;
    bool operator>(const Orario& altro) const;
    bool operator>=(const Orario& altro) const;
    bool operator==(const Orario& altro) const;
    bool operator!=(const Orario& altro) const;
    
    // Operazioni tra orari
    Orario operator+(const Orario& altro) const;
    Orario operator-(const Orario& altro) const;
    
    // Calcola la differenza in minuti tra due orari
    int diffInMinuti(const Orario& altro) const;
    
    // Converti in stringa nel formato "HH:MM"
    std::string toString() const;
    
    // Verifica se l'orario è compreso nell'intervallo [inizio, fine]
    bool isDentroIntervallo(const Orario& inizio, const Orario& fine) const;
    
    // Formatta l'orario per output
    string format() const;
};

#endif // ORARIO_H