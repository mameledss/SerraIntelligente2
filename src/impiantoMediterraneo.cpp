#include <sstream>
#include <iomanip>
#include <cstdlib> // Per rand() e srand()
#include <ctime>   // Per time()
#include "ImpiantoMediterraneo.h"

ImpiantoMediterraneo::ImpiantoMediterraneo(int id, const string& nome)
    : Impianto(id, nome, 0.4f), temperatura(28.0f) {
    srand(static_cast<unsigned int>(time(nullptr))); // Inizializza rand() una sola volta
}

bool ImpiantoMediterraneo::impostaTimer(const Orario& inizio, const Orario& fine) {
    return false;
}

bool ImpiantoMediterraneo::impostaTimer2(const Orario& inizio) {
    return false;
}

bool ImpiantoMediterraneo::rimuoviTimer() {
    return false;
}

void ImpiantoMediterraneo::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) {
    int minutiTrascorsi = orarioCorrente.diffInMinuti(orarioPrecedente);
    if (minutiTrascorsi <= 0) {
        return;
    }
    bool statoIniziale = attivo;

    if (!attivo) {
        aggiornaTemperaturaSpento(minutiTrascorsi);
        if (temperatura < 25.0f) {
            accendi(orarioCorrente);
        }
    } else {
        aggiornaTemperaturaAcceso(minutiTrascorsi, orarioCorrente);
        if (temperatura >= 28.0f) {
            temperatura = 28.0f;
            spegni(orarioCorrente);
        }
    }
    if (attivo && statoIniziale && orarioPrecedente != orarioCorrente) {
        aggiornaConsumo(minutiTrascorsi);
    }
}

string ImpiantoMediterraneo::getInfo() const {
    ostringstream oss;
    oss << "Impianto Mediterraneo \"" << nome << "\" (ID: " << id << ")" << endl;
    oss << "  Stato: " << (attivo ? "Attivo" : "Disattivo") << endl;
    oss << "  Temperatura: " << fixed << setprecision(2) << temperatura << "C" << endl;
    oss << "  Ultima attivazione: " << (attivo ? ultimaAttivazione.toString() : "Mai") << endl;
    oss << "  Consumo totale: " << fixed << setprecision(2) << consumoTotale << " litri" << endl;
    return oss.str();
}

float ImpiantoMediterraneo::getTemperatura() const {
    return temperatura;
}

void ImpiantoMediterraneo::aggiornaTemperaturaSpento(int minutiTrascorsi) {
    for (int i = 0; i < minutiTrascorsi; i++) {
        float decremento = 0.01f + static_cast<float>(rand()) / RAND_MAX * (0.05f - 0.01f);
        temperatura -= decremento;
        if (temperatura < 0.0f) {
            temperatura = 0.0f;
            break;
        }
    }
}

void ImpiantoMediterraneo::aggiornaTemperaturaAcceso(int minutiTrascorsi, const Orario& orarioCorrente) {
    int oreComplete = minutiTrascorsi / 60;

    for (int i = 0; i < oreComplete; i++) {
        if (temperatura < 28.0f) {
            float incremento = 0.75f + static_cast<float>(rand()) / RAND_MAX * (1.0f - 0.75f);
            temperatura += incremento;
            if (temperatura > 28.0f) {
                temperatura = 28.0f;
                break;
            }
        }
    }
}

bool ImpiantoMediterraneo::dovrebbeEssereAttivo(const Orario& orario) const {
    if (temperatura < 25.0f) {
        return true;
    } else if (temperatura >= 28.0f) {
        return false;
    }
    return attivo;
}