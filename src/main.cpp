#include <iostream>
#include <string>
#include "Serra.h"
#include "CommandParser.h"

using namespace std;

void stampaIntroduzione() {
    cout << "==================================================================" << endl;
    cout << "        SISTEMA DI GESTIONE SERRA INTELLIGENTE" << endl;
    cout << "==================================================================" << endl;
    cout << "Comandi disponibili:" << endl;
    cout << "  - set [nome_impianto] on       : Accendi manualmente un impianto" << endl;
    cout << "  - set [nome_impianto] off      : Spegni manualmente un impianto" << endl;
    cout << "  - set [nome_impianto] [HH:MM]  : Imposta timer di accensione/spegnimento" << endl;
    cout << "  - set [nome_impianto] [HH:MM] [HH:MM] : Imposta timer di accensione/spegnimento" << endl;
    cout << "  - rm [nome_impianto]           : Rimuovi un timer" << endl;
    cout << "  - show                         : Mostra tutti gli impianti" << endl;
    cout << "  - show [nome_impianto]         : Mostra dettagli di un impianto" << endl;
    cout << "  - set time [HH:MM]             : Imposta l'orario di sistema" << endl;
    cout << "  - reset time                   : Ripristina l'orario a 00:00" << endl;
    cout << "  - reset timers                 : Rimuovi tutti i timer" << endl;
    cout << "  - reset all                    : Ripristina condizioni iniziali" << endl;
    cout << "  - exit                         : Esci dal programma" << endl;
    cout << "==================================================================" << endl;
    cout << "Orario attuale: 00:00" << endl;
}

int main() {
    Serra serra;
    CommandParser parser(serra);
    string comando;

    // Aggiungi alcuni impianti predefiniti
    serra.aggiungiImpianto("Tropicale", "Tropicale");
    serra.aggiungiImpianto("Desertico", "Desertico");
    serra.aggiungiImpianto("Carnivoro", "Carnivoro");
    serra.aggiungiImpianto("Alpino", "Alpino");
    serra.aggiungiImpianto("Mediterraneo", "Mediterraneo");

    stampaIntroduzione();

    while (true) {
        cout << "\n> ";
        getline(cin, comando);

        if (comando == "exit") {
            break;
        }

        parser.elaboraComando(comando);
    }

    cout << "Arrivederci!" << endl;

    return 0;
}
