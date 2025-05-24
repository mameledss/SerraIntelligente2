#include "Impianto.h"
#include "ImpiantoTropicale.h"
#include "ImpiantoDesertico.h"
#include "ImpiantoAlpino.h"
#include "ImpiantoCarnivoro.h"
#include "ImpiantoMediterraneo.h"
#include <iostream>

Impianto::Impianto(int id, const string& nome, float consumoPerMinuto)
    : id(id), nome(nome), attivo(false), ultimaAttivazione(), consumoPerMinuto(consumoPerMinuto), consumoTotale(0.0f) {}

int Impianto::getId() const {
    return id;
}

string Impianto::getNome() const {
    return nome;
}

bool Impianto::isAttivo() const {
    return attivo;
}

Orario Impianto::getUltimaAttivazione() const {
    return ultimaAttivazione;
}

float Impianto::getConsumoTotale() const {
    return consumoTotale;
}

bool Impianto::accendi(const Orario& orarioCorrente) {
    if (!attivo) {
        attivo = true;
        ultimaAttivazione = orarioCorrente;
        cout << orarioCorrente.format() << " L'impianto \"" << nome << "\" si è acceso" << endl;
        return true;
    }
    return false;
}

bool Impianto::spegni(const Orario& orarioCorrente) {
    if (attivo) {
        attivo = false;
        // Aggiorna il consumo totale
        int minutiAttivi = orarioCorrente.diffInMinuti(ultimaAttivazione);
        aggiornaConsumo(minutiAttivi);
        cout << orarioCorrente.format() << " L'impianto \"" << nome << "\" si è spento" << endl;
        return true;
    }
    return false;
}

void Impianto::aggiornaConsumo(int minutiAttivi) {
    if (minutiAttivi > 0) {
        consumoTotale += minutiAttivi * consumoPerMinuto;
    }
}

Impianto* Impianto::creaImpianto(int id, const string& tipo, const string& nome) {
    if (tipo == "Tropicale") {
        return new ImpiantoTropicale(id, nome);
    } else if (tipo == "Desertico") {
        return new ImpiantoDesertico(id, nome);
    } else if (tipo == "Alpino") {
        return new ImpiantoAlpino(id, nome);
    } else if (tipo == "Carnivoro") {
        return new ImpiantoCarnivoro(id, nome);
    } else if (tipo == "Mediterraneo") {
        return new ImpiantoMediterraneo(id, nome);
    } else {
        return nullptr;
    }
}
