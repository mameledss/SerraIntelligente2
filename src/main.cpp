#include <iostream>
#include <string>
#include "../include/Serra.h"
#include "../include/CommandParser.h"
#include "../include/ImpiantoTropicale.h"
#include "../include/ImpiantoDesertico.h"

void stampaIntroduzione() {
    std::cout << "==================================================================" << std::endl;
    std::cout << "        SISTEMA DI GESTIONE SERRA INTELLIGENTE" << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "Comandi disponibili:" << std::endl;
    std::cout << "  - set [nome_impianto] on       : Accendi manualmente un impianto" << std::endl;
    std::cout << "  - set [nome_impianto] off      : Spegni manualmente un impianto" << std::endl;
    std::cout << "  - set [nome_impianto] [HH:MM] [HH:MM] : Imposta timer di accensione/spegnimento" << std::endl;
    std::cout << "  - rm [nome_impianto]           : Rimuovi un timer" << std::endl;
    std::cout << "  - show                         : Mostra tutti gli impianti" << std::endl;
    std::cout << "  - show [nome_impianto]         : Mostra dettagli di un impianto" << std::endl;
    std::cout << "  - set time [HH:MM]             : Imposta l'orario di sistema" << std::endl;
    std::cout << "  - reset time                   : Ripristina l'orario a 00:00" << std::endl;
    std::cout << "  - reset timers                 : Rimuovi tutti i timer" << std::endl;
    std::cout << "  - reset all                    : Ripristina condizioni iniziali" << std::endl;
    std::cout << "  - exit                         : Esci dal programma" << std::endl;
    std::cout << "==================================================================" << std::endl;
    std::cout << "Orario attuale: 00:00" << std::endl;
}

int main() {
    Serra serra;
    CommandParser parser(serra);
    std::string comando;

    // Aggiungi alcuni impianti predefiniti
    serra.aggiungiImpianto("Tropicale", "PalmaDaCocco");
    serra.aggiungiImpianto("Desertico", "Cactus");

    stampaIntroduzione();

    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, comando);

        if (comando == "exit") {
            break;
        }

        parser.elaboraComando(comando);
    }

    std::cout << "Arrivederci!" << std::endl;

    return 0;
}