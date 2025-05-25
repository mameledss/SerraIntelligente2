# 🌱 Sistema di Gestione Serra Intelligente

Sistema per la gestione di serre automatizzate, sviluppato in C++ con architettura orientata agli oggetti. Il sistema supporta diversi tipi di impianti con comportamenti personalizzati e sistema di irrigazione gestita da timer.

## 📋 Caratteristiche Principali

- **Gestione Multi-Impianto**: Supporta 5 tipologie diverse di impianti
- **Sistema Timer**: Timer con comportamenti specifici per ogni tipo di impianto
- **Simulazione Temporale**: Sistema di orario interno con aggiornamento automatico degli stati
- **Monitoraggio Consumi**: Tracciamento del consumo idrico per ogni impianto
- **Interfaccia CLI**: Interfaccia a linea di comando per la gestione di tutto il sistema
- **Logging Sistema**: Sistema di logging integrato con livelli di errore

## ➕​ Caratteristiche Aggiuntive

- **Logging su file .txt**: Tutti gli eventi vengono salvati in un file .txt
- **Aggiunta impianti con comando add**: È possibile aggiungere nuovi impianti tramite comando "add"
- **Rimozione impianti da riga di comando**: È possibile rimuovere gli impianti tramite comando "rm -i"

## 🏗️ Architettura del Sistema

### Classi Principali

#### `Serra`
Classe principale che gestisce l'intera serra:
- Gestione collezione di impianti
- Sistema di orario interno
- Aggiornamento automatico stati
- Interfacce per controllo e monitoraggio

#### `Impianto` (Classe Base Astratta)
Definisce l'interfaccia comune per tutti gli impianti:
- Gestione stato (acceso/spento)
- Tracking consumo idrico
- Sistema timer base
- Logging attività

#### `CommandParser`
Parser per l'elaborazione dei comandi:
- Tokenizzazione comando
- Validazione parametri
- Suddivisione compiti ai metodi appropriati
- Gestione degli errori

#### `Orario`
Classe per la gestione del tempo:
- Operatori di confronto
- Calcoli temporali
- Validazione formato HH:MM
- Gestione attraversamento mezzanotte

## 🌿 Tipologie di Impianti

### 1. **Impianto Tropicale**
- **Consumo**: 0.5 litri/minuto
- **Modalità**: Automatico
- **Durata Irrigazione**: 2 ore e 30 minuti
- **Comportamento**: Si attiva all'orario programmato e si spegne automaticamente

### 2. **Impianto Alpino**
- **Consumo**: 0.5 litri/minuto  
- **Modalità**: Automatico
- **Durata Irrigazione**: 2 ore e 30 minuti
- **Comportamento**: Si attiva all'orario programmato e si spegne automaticamente

### 3. **Impianto Carnivoro**
- **Consumo**: 0.5 litri/minuto
- **Modalità**: Automatico  
- **Durata Irrigazione**: 2 ore e 30 minuti
- **Comportamento**: Si attiva all'orario programmato e si spegne automaticamente

### 4. **Impianto Desertico**
- **Consumo**: 0.3 litri/minuto
- **Modalità**: Timer con intervallo personalizzabile
- **Comportamento**: 
  - Supporta timer con orario inizio/fine specifici
  - Modalità "infinita" (fino a fine giornata)

### 5. **Impianto Mediterraneo**
- **Consumo**: 0.4 litri/minuto
- **Modalità**: Adattiva
- **Comportamento**:
  - **Temperatura Target**: 25°C - 28°C
  - **Attivazione**: Automatica quando temperatura < 25°C
  - **Disattivazione**: Automatica quando temperatura ≥ 28°C

## 💻 Guida Comandi

### Comandi di Configurazione Sistema

#### Aggiunta impianto
```bash
add tipoImpianto nomeImpianto
# Esempio: add Tropicale Palma
```

#### Rimozione impianto
```bash
rm -i nomeImpianto
# Esempio: rm -i Palma
```

#### Impostazione Orario
```bash
set time HH:MM          # Imposta orario di sistema
# Esempio: set time 14:30
```

#### Reset Sistema  
```bash
reset time             # Reset orario a 00:00
reset timers           # Rimuove tutti i timer attivi
reset all              # Reset completo sistema
```

### Comandi Gestione Impianti

#### Controllo Manuale
```bash
set [nome_impianto] on     # Accensione manuale immediata
set [nome_impianto] off    # Spegnimento manuale

# Esempi:
set Tropicale on
set Desertico off
```

#### Timer Automatici

**Timer Singolo** (Tropicale, Alpino, Carnivoro):
```bash
set [nome_impianto] HH:MM  # Timer con durata fissa (2h 30min)

# Esempi:
set Tropicale 08:00        # Si attiva alle 08:00, si spegne alle 10:30
set Carnivoro 18:30        # Si attiva alle 18:30, si spegne alle 21:00
```

**Timer Intervallo** (Solo Desertico):
```bash
set [nome_impianto] HH:MM HH:MM    # Timer con inizio/fine personalizzati

# Esempi:
set Desertico 06:00 07:30          # Attivo dalle 06:00 alle 07:30
set Desertico 20:00 23:00          # Attivo dalle 20:00 alle 23:00
```

#### Rimozione Timer
```bash
rm [nome_impianto]         # Rimuove timer e spegne se attivo

# Esempi:
rm Tropicale
rm Desertico
```

### Comandi Monitoraggio

#### Visualizzazione Stato
```bash
show                      # Panoramica tutti gli impianti
show [nome_impianto]      # Dettagli impianto specifico

# Esempi:
show                      # Tabella riassuntiva
show Mediterraneo         # Dettagli completi inclusa temperatura
```

## 📊 Output di Esempio

### Panoramica Sistema
```
[14:30] Stato attuale degli impianti
--------------------------------------------------------
ID    Nome           Tipo           Stato     Consumo (litri)
--------------------------------------------------------
1     Tropicale      Tropicale      Spento    12.50
2     Desertico      Desertico      Acceso    8.30
3     Carnivoro      Carnivoro      Spento    0.00
4     Alpino         Alpino         Acceso    15.75
5     Mediterraneo   Mediterraneo   Acceso    22.40
--------------------------------------------------------
```

### Dettagli Impianto Specifico
```
Dettagli impianto:
ID: 5
Nome: Mediterraneo
Stato: Acceso
Ultima attivazione: [13:45]
Consumo totale: 22.40 litri
```

## 🔧 Funzionalità Avanzate

### Sistema di Logging
- **Livello 0**: Informazioni normali (cout)
- **Livello 1**: Errori e avvisi (cerr)
- **Timestamp**: Ogni messaggio include orario di sistema

### Gestione Errori
- Validazione formato orari (HH:MM)
- Validazione formato comandi
- Controllo esistenza impianti
- Prevenzione impostazione orari retroattivi

### Simulazione Temporale
Il sistema simula il passaggio del tempo minuto per minuto:
- Aggiornamento automatico stati impianti
- Calcolo consumi in tempo reale
- Gestione attivazioni/disattivazioni programmate
- Simulazione termica per impianto Mediterraneo

## 🤝 Autori (In ordine alfabetico)
- Brunello Andrea [@Andrea9698065](https://www.github.com/Andrea9698065)
- Dalla Santa Manuel [@mameledss](https://www.github.com/mameledss)
- Gitaric Ivan [@IvanGitaric](https://www.github.com/IvanGitaric)
