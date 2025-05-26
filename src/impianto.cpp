//AUTHOR: BRUNELLO
#include "CommandParser.h"
#include "Impianto.h"
#include "ImpiantoTropicale.h"
#include "ImpiantoDesertico.h"
#include "ImpiantoAlpino.h"
#include "ImpiantoCarnivoro.h"
#include "ImpiantoMediterraneo.h"

Impianto::Impianto(int id, const string& nome, float consumoPerMinuto) //costruttore classe Impianto: inizializza id, nome, consumo e imposta l’impianto come spento
    : id(id), nome(nome), attivo(false), ultimaAttivazione(), consumoPerMinuto(consumoPerMinuto), consumoTotale(0.0f) {}

int Impianto::getId() const { //restituisce ID dell’impianto
    return id;
}

string Impianto::getNome() const { // restituisce il nome dell’impianto
    return nome;
}

bool Impianto::isAttivo() const { //verifica se l’impianto è attualmente acceso
    return attivo;
}

Orario Impianto::getUltimaAttivazione() const { //restituisce l’orario dell’ultima accensione
    return ultimaAttivazione;
}

float Impianto::getConsumoTotale() const { //restituisce il consumo totale
    return consumoTotale;
}

bool Impianto::accendi(const Orario& orarioCorrente) { //accende l’impianto se è spento e registra l’orario di accensione
    if (!attivo) { //se non è attivo
        attivo = true; //lo attiva
        ultimaAttivazione = orarioCorrente; //l'ultima attivazione corrisponde all'orario corrente
        logMessage(orarioCorrente, " L'impianto \"" + nome + "\" si e' acceso", 0);
        return true;
    }
    return false; //l’impianto era già acceso
}

bool Impianto::spegni(const Orario& orarioCorrente) { //spegne l’impianto se è acceso e aggiorna il consumo
    if (attivo) { //se è attivo
        attivo = false; //lo spegne
        int minutiAttivi = orarioCorrente.diffInMinuti(ultimaAttivazione); //calcola tempo di funzionamento
        aggiornaConsumo(minutiAttivi); //aggiorna il consumo corrispondente
        logMessage(orarioCorrente, " L'impianto \"" + nome + "\" si e' spento", 0);
        return true;
    }
    return false; //l’impianto era già spento
}

void Impianto::aggiornaConsumo(int minutiAttivi) { //aggiorna il consumo totale basandosi sui minuti di attività e sul consumo al minuto
    if (minutiAttivi > 0) { //se è rimasto attivo per almeno 1 minuto
        consumoTotale += minutiAttivi * consumoPerMinuto; //agiorna il consumo totale
    }
}

Impianto* Impianto::creaImpianto(int id, const string& tipo, const string& nome) { //metodo statico per creare nuovo impianto del tipo richiesto
    if (tipo == "Tropicale")
        return new ImpiantoTropicale(id, nome);
    else if (tipo == "Desertico")
        return new ImpiantoDesertico(id, nome);
    else if (tipo == "Alpino")
        return new ImpiantoAlpino(id, nome);
    else if (tipo == "Carnivoro")
        return new ImpiantoCarnivoro(id, nome);
    else if (tipo == "Mediterraneo")
        return new ImpiantoMediterraneo(id, nome);
    else
        return nullptr; //tipo non riconosciuto
}