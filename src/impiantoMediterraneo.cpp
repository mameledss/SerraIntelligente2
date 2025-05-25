#include <cstdlib>
#include <ctime>
#include "ImpiantoMediterraneo.h"

ImpiantoMediterraneo::ImpiantoMediterraneo(int id, const string& nome)
    : Impianto(id, nome, 0.4f), temperatura(28.0f) { //costruttore: inizializza l’impianto con consumo di 0.4 litri/minuto e temperatura a 28°
    srand(static_cast<unsigned int>(time(nullptr))); //inizializza il generatore casuale
}

bool ImpiantoMediterraneo::impostaTimer(const Orario& inizio, const Orario& fine) {
    return false; //non supporta timer con intervallo
}

bool ImpiantoMediterraneo::impostaTimer2(const Orario& inizio) {
    return false; //non supporta timer
}

bool ImpiantoMediterraneo::rimuoviTimer() {
    return false; //non ha timer da rimuovere
}

void ImpiantoMediterraneo::aggiornaStato(const Orario& orarioPrecedente, const Orario& orarioCorrente) { //aggiorna lo stato dell’impianto in base all’orario
    int minutiTrascorsi = orarioCorrente.diffInMinuti(orarioPrecedente); //calcola minuti passati
    if (minutiTrascorsi <= 0) { //se non è passato tempo
        return; //nessun aggiornamento
    }
    bool statoIniziale = attivo; //salva lo stato iniziale
    if (!attivo) { //se è spento
        aggiornaTemperaturaSpento(minutiTrascorsi); //raffredda
        if (temperatura < 25.0f) { //se la temperatura è minore di 25°
            accendi(orarioCorrente); //accende
        }
    } else { //se è acceso
        aggiornaTemperaturaAcceso(minutiTrascorsi, orarioCorrente); //riscalda
        if (temperatura >= 28.0f) { //se la temperatura arriva a 28°
            temperatura = 28.0f; //la limita a 28°
            spegni(orarioCorrente); //spegne
        }
    }
    if (attivo && statoIniziale && orarioPrecedente != orarioCorrente) { //se è stato attivo
        aggiornaConsumo(minutiTrascorsi); //aggiorna consumo
    }
}

string ImpiantoMediterraneo::getTipo() const {
    return "Mediterraneo"; // Restituisce il tipo di impianto
}

float ImpiantoMediterraneo::getTemperatura() const { //restituisce una stringa con tipo impianto
    return temperatura;
}

void ImpiantoMediterraneo::aggiornaTemperaturaSpento(int minutiTrascorsi) { //decrementa la temperatura quando è spento
    for (int i = 0; i < minutiTrascorsi; i++) { //per ogni minuto trascorso
        float decremento = 0.01f + static_cast<float>(rand()) / RAND_MAX * (0.05f - 0.01f); //valore casuale tra 0.01 e 0.05
        temperatura -= decremento; //decrementa temperatura
        if (temperatura < 0.0f) { //se la temperatura scende sotto 0°
            temperatura = 0.0f; //la limita a 0°
            break;
        }
    }
}

void ImpiantoMediterraneo::aggiornaTemperaturaAcceso(int minutiTrascorsi, const Orario& orarioCorrente) { //aggiorna la temperatura se è acceso
    int oreComplete = minutiTrascorsi / 60; //considera solo ore intere per il riscaldamento
    for (int i = 0; i < oreComplete; i++) { //per ogni ora
        if (temperatura < 28.0f) { //se la temperatura è minore di 28°
            float incremento = 0.75f + static_cast<float>(rand()) / RAND_MAX * (1.0f - 0.75f); //valore casuale tra 0.75 e 1.0
            temperatura += incremento; //incrementa temperatura
            if (temperatura > 28.0f) { //se la temperatura supera i 28°
                temperatura = 28.0f; //la limita a 28°
                break;
            }
        }
    }
}