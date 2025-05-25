#include <sstream>
#include <iomanip> //per setw e setfill usati nel formato dell’orario
#include "Orario.h"

Orario::Orario() : ore(0), minuti(0) {} //costruttore di default: inizializza ore e minuti a zero

Orario::Orario(int ore, int minuti) { //costruttore con parametri: normalizza ore (0-23) e minuti (0-59)
    this->ore = ore % 24;
    this->minuti = minuti % 60;
}

Orario::Orario(const string& orarioStr) { //costruttore da stringa nel formato "HH:MM"
    size_t pos = orarioStr.find(':'); //trova la posizione del separatore :
    if (pos != string::npos) { //se : viene trovato
        ore = stoi(orarioStr.substr(0, pos)) % 24; //estrae e normalizza la parte da 0 a pos (le ore)
        minuti = stoi(orarioStr.substr(pos + 1)) % 60; //e i minuti
    } else { //altrimenti
        ore = 0; //imposta le ore a 0
        minuti = 0; //e anche i minuti
    }
}

int Orario::getOre() const { //ritorna le ore
    return ore;
}

int Orario::getMinuti() const { //ritorna i minuti
    return minuti;
}

void Orario::setOre(int ore) { //imposta le ore e le normalizza
    this->ore = ore % 24;
}

void Orario::setMinuti(int minuti) { //imposta i minuti e li normalizza
    this->minuti = minuti % 60;
}

bool Orario::operator<(const Orario& altro) const { //operatore di confronto "<"
    if (ore < altro.ore) return true; //se this.ore sono minori delle ore dell'altro ritorna true
    if (ore > altro.ore) return false; //altrimenti false
    return minuti < altro.minuti; //se this.minuti sono minori dei minuti dell'altro ritorna true
}

bool Orario::operator==(const Orario& altro) const { //operatore di confronto "=="
    return ore == altro.ore && minuti == altro.minuti; //se ore e minuti coincidono, ritorna true
}

bool Orario::operator<=(const Orario& altro) const { //operatore di confronto "<="
    return (*this < altro) || (*this == altro); //ritorna true se this è prima o uguale ad altro
}

bool Orario::operator>(const Orario& altro) const { //operatore di confronto ">"
    return !(*this <= altro); //true se this è dopo altro
}

bool Orario::operator>=(const Orario& altro) const { //operatore di confronto ">="
    return !(*this < altro); //true se this è dopo o uguale ad altro
}

bool Orario::operator!=(const Orario& altro) const { //operatore di confronto "!="
    return !(*this == altro); //true se ore o minuti sono diversi
}

Orario Orario::operator+(const Orario& altro) const {
    int totMinuti = (ore * 60 + minuti + altro.ore * 60 + altro.minuti);
    int nuoveOre = (totMinuti / 60) % 24;
    int nuoviMinuti = totMinuti % 60;
    return Orario(nuoveOre, nuoviMinuti);
}

int Orario::diffInMinuti(const Orario& altro) const { //ritorna la differenza tra due orari in minuti
    int totMinuti1 = ore * 60 + minuti; //calcola minuti totali di this
    int totMinuti2 = altro.ore * 60 + altro.minuti; //e i minuti totali dell'altro

    if (totMinuti1 < totMinuti2) { //se i minuti di this sono minori dei minuti dell'altro
        totMinuti1 += 24 * 60; //aggiunge 24 ore ai minuti di this
    }
    return totMinuti1 - totMinuti2; //ritorna la differenza
}

string Orario::toString() const { //converte l'orario in stringa formato "HH:MM"
    ostringstream oss;
    oss << setw(2) << setfill('0') << ore << ":" //due cifre, con '0' come padding a sinistra per le ore
        << setw(2) << setfill('0') << minuti; //e anche per i minuti
    return oss.str();
}

bool Orario::isDentroIntervallo(const Orario& inizio, const Orario& fine) const { //verifica se l'orario è all'interno dell'intervallo [inizio, fine]
    if (inizio <= fine) //se l'orario di inizio è minore di quello di fine
        return (*this >= inizio && *this <= fine); //ritorna true se this si trova tra inizio e fine
    else
        return (*this >= inizio || *this <= fine); //gestisce anche intervallo che passa la mezzanotte
}

string Orario::format() const { //ritorna una stringa "[HH:MM]"
    ostringstream oss;
    oss << "[" << setw(2) << setfill('0') << ore << ":" //due cifre, con '0' come padding
        << setw(2) << setfill('0') << minuti << "]";
    return oss.str();
}