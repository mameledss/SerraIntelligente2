#ifndef ORARIO_H
#define ORARIO_H

#include <string>
using namespace std;

class Orario { //classe Orario, per rappresentare un orario del giorno
private:
    int ore; //ore (0-23)
    int minuti; //minuti (0-59)
public:
    Orario(); //costruttore di default: inizializza ore e minuti a 0
    Orario(int ore, int minuti); //costruttore con ore e minuti come parametri
    Orario(const string& orarioStr); //costruttore con stringa nel formato "HH:MM"
    int getOre() const; //ritorna il valore delle ore
    int getMinuti() const; //ritorna il valore dei minuti
    void setOre(int ore); //imposta il valore delle ore
    void setMinuti(int minuti); //e dei minuti
    bool operator<(const Orario& altro) const; //operatore minore: verifica se l'orario corrente è antecedente a un altro
    bool operator<=(const Orario& altro) const; //operatore minore o uguale
    bool operator>(const Orario& altro) const; //operatore maggiore
    bool operator>=(const Orario& altro) const; //operatore maggiore o uguale
    bool operator==(const Orario& altro) const; //operatore uguale uguale: confronta se due orari sono identici
    bool operator!=(const Orario& altro) const; //operatore diverso da
    Orario operator+(const Orario& altro) const; //operatore somma tra due orari (es. 02:30 + 01:15 = 03:45)
    int diffInMinuti(const Orario& altro) const; //calcola differenza in minuti tra orario corrente e un altro orario
    string toString() const; //ritorna rappresentazione stringa dell'orario nel formato "HH:MM"
    bool isDentroIntervallo(const Orario& inizio, const Orario& fine) const; //verifica se l'orario è compreso tra due orari dati
    string format() const; //ritorna stringa in formato orario
};
#endif // ORARIO_H