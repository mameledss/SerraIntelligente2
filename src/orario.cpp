#include <iomanip>
#include <sstream>
#include "Orario.h"

Orario::Orario() : ore(0), minuti(0) {}

Orario::Orario(int ore, int minuti) {
    this->ore = ore % 24;
    this->minuti = minuti % 60;
}

Orario::Orario(const string& orarioStr) {
    size_t pos = orarioStr.find(':');
    if (pos != string::npos) {
        ore = stoi(orarioStr.substr(0, pos)) % 24;
        minuti = stoi(orarioStr.substr(pos + 1)) % 60;
    } else {
        ore = 0;
        minuti = 0;
    }
}

int Orario::getOre() const {
    return ore;
}

int Orario::getMinuti() const {
    return minuti;
}

void Orario::setOre(int ore) {
    this->ore = ore % 24;
}

void Orario::setMinuti(int minuti) {
    this->minuti = minuti % 60;
}

bool Orario::operator<(const Orario& altro) const {
    if (ore < altro.ore) return true;
    if (ore > altro.ore) return false;
    return minuti < altro.minuti;
}

bool Orario::operator<=(const Orario& altro) const {
    return (*this < altro) || (*this == altro);
}

bool Orario::operator>(const Orario& altro) const {
    return !(*this <= altro);
}

bool Orario::operator>=(const Orario& altro) const {
    return !(*this < altro);
}

bool Orario::operator==(const Orario& altro) const {
    return ore == altro.ore && minuti == altro.minuti;
}

bool Orario::operator!=(const Orario& altro) const {
    return !(*this == altro);
}

Orario Orario::operator+(const Orario& altro) const {
    int totMinuti = (ore * 60 + minuti + altro.ore * 60 + altro.minuti);
    int nuoveOre = (totMinuti / 60) % 24;
    int nuoviMinuti = totMinuti % 60;
    return Orario(nuoveOre, nuoviMinuti);
}

Orario Orario::operator-(const Orario& altro) const {
    int totMinuti1 = ore * 60 + minuti;
    int totMinuti2 = altro.ore * 60 + altro.minuti;

    if (totMinuti1 < totMinuti2) {
        totMinuti1 += 24 * 60; // Aggiunge un giorno
    }

    int diffMinuti = totMinuti1 - totMinuti2;
    int nuoveOre = (diffMinuti / 60) % 24;
    int nuoviMinuti = diffMinuti % 60;

    return Orario(nuoveOre, nuoviMinuti);
}

int Orario::diffInMinuti(const Orario& altro) const {
    int totMinuti1 = ore * 60 + minuti;
    int totMinuti2 = altro.ore * 60 + altro.minuti;

    if (totMinuti1 < totMinuti2) {
        totMinuti1 += 24 * 60; // Aggiunge un giorno
    }

    return totMinuti1 - totMinuti2;
}

string Orario::toString() const {
    ostringstream oss;
    oss << setw(2) << setfill('0') << ore << ":"
        << setw(2) << setfill('0') << minuti;
    return oss.str();
}

bool Orario::isDentroIntervallo(const Orario& inizio, const Orario& fine) const {
    // Se l'intervallo è all'interno dello stesso giorno
    if (inizio <= fine) {
        return (*this >= inizio && *this <= fine);
    }
    // Se l'intervallo attraversa la mezzanotte
    else {
        return (*this >= inizio || *this <= fine);
    }
}

string Orario::format() const {
    ostringstream oss;
    oss << "[" << setw(2) << setfill('0') << ore << ":"
        << setw(2) << setfill('0') << minuti << "]";
    return oss.str();
}
