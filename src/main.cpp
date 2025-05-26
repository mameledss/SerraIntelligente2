//FILE AUTHOR: DALLA SANTA MANUEL
#include <iostream>
#include "Serra.h"
#include "CommandParser.h"
using namespace std;
//stampa il menu principale
void stampaIntroduzione() {
    cout << "==================================================================" << endl;
    cout << "        SISTEMA DI GESTIONE SERRA INTELLIGENTE" << endl;
    cout << "==================================================================" << endl;
    cout << "Comandi disponibili:" << endl;
    cout << "  - add [tipo_impianto] [nome]   : Aggiungi un nuovo impianto" << endl;
    cout << "  - rm -i [nome_impianto]        : Rimuovi un impianto" << endl;
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
    cout << "Tipi di impianto supportati:" << endl;
    cout << "  - Tropicale, Desertico, Carnivoro, Alpino, Mediterraneo" << endl;
    cout << "==================================================================" << endl;
    cout << "Orario attuale: 00:00" << endl;
    cout << "Nessun impianto presente. Usa 'add' per aggiungere impianti." << endl;
}

int main() {
    Serra serra; //serra
    CommandParser parser(serra); //commandparser a cui viene passata la serra come parametro
    string comando; //string per contenere il comando inserito dall'utente

    stampaIntroduzione();

    while (true) { //per sempre
        cout << "> "; //stampa il > per inicare la riga in cui inserire il comando
        getline(cin, comando); //ottiene il comando inserito dall'utente

        if (comando == "exit")  //se il comando è "exit"
            break; //interrompe l'esecuzione

        parser.elaboraComando(comando); //passa il comando al commandparser
    }
    cout << "Arrivederci!" << endl;
    return 0;
}