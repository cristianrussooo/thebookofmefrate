#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <limits>
#include <sstream>  // Per gestire la data di nascita

using namespace std;

// Diritti di autore
// Copyright (c) 2024 Ludos Patia FC. Tutti i diritti riservati.

// Versione del programma
const string versione = "beta 0.0.4";

// Simboli disponibili nella slot machine
const string simboli[] = {"A", "K", "Q", "J", "10", "Wild", "Book"};

// Probabilità di apparizione dei simboli (ridurre la probabilità di "Wild" e "Book")
const string simboliProbabili[] = {
    "A", "K", "Q", "J", "10",  // Aumenta la frequenza dei simboli non vincenti
    "A", "K", "Q", "J", "10",  // Aumenta ulteriormente la frequenza dei simboli non vincenti
    "Wild", "Wild", "Book"     // Riduci la frequenza di "Wild" e "Book"
};

// Struttura per rappresentare i rulli della slot
struct Slot {
    vector<string> rulli;
    Slot() : rulli(5) {}  // 5 rulli
};

// Funzione per generare un simbolo casuale con probabilità modificate
string generaSimbolo() {
    return simboliProbabili[rand() % (sizeof(simboliProbabili) / sizeof(simboliProbabili[0]))];
}

// Funzione per effettuare un giro
Slot effettuaGiro() {
    Slot slot;
    for (int i = 0; i < 5; ++i) {  // 5 rulli
        slot.rulli[i] = generaSimbolo();
    }
    return slot;
}

// Funzione per visualizzare lo stato dei rulli
void visualizzaRulli(const Slot& slot, int numeroGiocata) {
    // Pulire la schermata prima di visualizzare i risultati
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");  // Per Windows
    #else
        system("clear");  // Per Linux/macOS
    #endif

    cout << "\n\033[1;33mGiocata n." << numeroGiocata << " - Griglia Slot Machine\033[0m\n";
    
    for (int i = 0; i < 3; ++i) {  // 3 linee
        for (int j = 0; j < 5; ++j) {
            if (slot.rulli[j] == "Wild") {
                cout << "\033[1;35m" << slot.rulli[j] << "\033[0m ";  // Colorazione Wild in viola
            } else if (slot.rulli[j] == "Book") {
                cout << "\033[1;36m" << slot.rulli[j] << "\033[0m ";  // Colorazione Book in ciano
            } else {
                cout << "\033[0;32m" << slot.rulli[j] << "\033[0m ";  // Colorazione simboli normali in verde
            }
        }
        cout << endl;
    }
}

// Funzione per calcolare la vincita in base al tipo di simbolo
int calcolaVincita(const Slot& slot) {
    int vincita = 0;

    // Combinazioni di 3 simboli uguali (ad esempio, 3 Book o 3 Wild)
    for (int i = 0; i < 3; ++i) { // 3 linee
        if (slot.rulli[i] == slot.rulli[i + 1] && slot.rulli[i + 1] == slot.rulli[i + 2]) {
            if (slot.rulli[i] == "Book") {
                vincita = 50;  // Vincita per 3 Book ridotta
            } else if (slot.rulli[i] == "Wild") {
                vincita = 30;  // Vincita per 3 Wild ridotta
            }
        }
    }

    return vincita;
}

// Funzione per simulare il Jackpot
int jackpot() {
    // Ridurre la probabilità del jackpot (1% di probabilità)
    int chance = rand() % 100;  // Possibilità del jackpot
    if (chance < 1) {
        return 1000;  // Vincita del jackpot
    }
    return 0;  // Nessun jackpot
}

// Funzione per la visualizzazione dei comandi
void visualizzaComandi() {
    cout << "Se vuoi terminare il gioco, scrivi \033[1;31m'fine'\033[0m." << endl;
    cout << "Se vuoi cambiare l'importo della scommessa, scrivi \033[1;33m'cambia'\033[0m." << endl;
    cout << "Se vuoi vedere le schede vinte, scrivi \033[1;35m'schede'\033[0m." << endl;
    cout << "Se vuoi ricaricare il saldo (disabilitato quando saldo è zero), scrivi \033[1;31m'ricarica'\033[0m." << endl;
    cout << "Se vuoi puntare tutto il saldo, scrivi \033[1;32m'casa'\033[0m." << endl;
}

// Funzione per la simulazione del gioco
void gioca() {
    string username;

    // Login
    cout << "\033[1;32mBenvenuto! Prima di iniziare, effettua il login.\033[0m" << endl;
    cout << "Inserisci il tuo username: ";
    getline(cin, username);

    // Pulizia della schermata dopo aver inserito lo username
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");  // Per Windows
    #else
        system("clear");  // Per Linux/macOS
    #endif

    cout << "\033[1;32mBenvenuto " << username << "! Il tuo saldo iniziale e' di 500 euro.\033[0m" << endl;

    int saldo = 500;  // Imposta un saldo iniziale di 500 euro
    int scommessa = 1;  // Scommessa fissa di 1 euro
    int numeroGiocata = 1; // Numero della giocata
    vector<int> vinciteHistory;  // Per tenere traccia delle vincite
    int spinGratuiti = 0;  // Contatore per gli spin gratuiti
    bool ricaricaDisabilitata = false; // Variabile per disabilitare ricarica

    while (true) {  // Ciclo infinito per non terminare mai il gioco
        // Visualizzare l'importo della scommessa con colorazione in base all'importo
        cout << "\nIl tuo saldo attuale e' : \033[1;32m" << saldo << "\033[0m euro." << endl;

        if (spinGratuiti > 0) {
            cout << "Hai ancora \033[1;33m" << spinGratuiti << "\033[0m spin gratuiti rimanenti." << endl;
        }

        // Mostrare l'importo della scommessa con colorazione dinamica
        string color = "\033[1;37m";  // Colore di default: bianco
        if (scommessa < 30) {
            color = "\033[0;32m";  // Verde se la scommessa è piccola
        } else if (scommessa >= 30 && scommessa <= 120) {
            color = "\033[1;33m";  // Arancione se tra 30 e 120
        } else {
            color = "\033[1;31m";  // Rosso se maggiore di 120
        }
        cout << "\033[1;33mLudos Patia Slot Machine - Versione: " << versione << " \033[0m" << endl;
        cout << "\033[1;32mCopyright (c) 2024 Ludos Patia FC. Tutti i diritti riservati.\033[0m" << endl;
        cout << "Username: \033[1;36m" << username << "\033[0m" << endl;
        cout << "Scommessa attuale: " << color << scommessa << " euro\033[0m" << endl;

        visualizzaComandi();

        if (saldo > 0 || spinGratuiti > 0) {
            string comando;
            getline(cin, comando);

            if (comando == "fine") {
                break;
            }

            if (comando == "cambia") {
                int nuovaScommessa;
                cout << "Inserisci il nuovo importo della scommessa: ";
                cin >> nuovaScommessa;
                if (nuovaScommessa > 0 && nuovaScommessa <= saldo) {
                    scommessa = nuovaScommessa;
                    cout << "Nuova scommessa settata a " << scommessa << " euro." << endl;
                } else {
                    cout << "\033[1;31mImporto non valido. La scommessa non e' cambiata.\033[0m" << endl;
                }
                cin.ignore();  // Per evitare problemi con la lettura dell'input successivo
                continue;
            }

            if (comando == "schede") {
                cout << "\033[1;34mSchede vinte:\033[0m" << endl;
                if (vinciteHistory.empty()) {
                    cout << "\033[1;31mNon hai ancora vinto nulla.\033[0m" << endl;
                } else {
                    for (size_t i = 0; i < vinciteHistory.size(); ++i) {
                        cout << "\033[1;36m" << i + 1 << ". " << vinciteHistory[i] << " euro\033[0m" << endl;
                    }
                }
                continue;  // Torna al ciclo
            }

            if (comando == "casa") {
                scommessa = saldo;  // Puntare tutto il saldo
                cout << "Hai deciso di puntare tutto il tuo saldo, ovvero \033[1;31m" << scommessa << " euro.\033[0m" << endl;
            }

            // Dedurre il saldo o utilizzare spin gratuiti
            if (spinGratuiti > 0) {
                spinGratuiti--;
            } else if (saldo > 0) {
                saldo -= scommessa;  // Dedurre la scommessa dal saldo
            } else {
                cout << "\033[1;31mSaldo esaurito.\033[0m" << endl;
                ricaricaDisabilitata = true; // Disabilita il comando ricarica
                cout << "Ti sono stati regalati \033[1;32m10 spin gratuiti\033[0m per darti una seconda possibilità!" << endl;
                spinGratuiti = 10;  // Regala 10 spin gratuiti
                continue;
            }

            // Effettuare il giro
            Slot slot = effettuaGiro();

            // Visualizza i risultati dei rulli
            visualizzaRulli(slot, numeroGiocata);
            numeroGiocata++;  // Incrementa il numero della giocata

            // Calcolare la vincita
            int vincita = calcolaVincita(slot);

            // Controlla il Jackpot
            int premioJackpot = jackpot();
            if (premioJackpot > 0) {
                cout << "\033[1;33m\033[1;41mJACKPOT!!! Hai vinto " << premioJackpot << " euro!\033[0m" << endl;
                saldo += premioJackpot;
                vinciteHistory.push_back(premioJackpot);  // Aggiungi il jackpot alle schede vinte
            } else {
                if (vincita > 0) {
                    cout << "\033[1;32mHai vinto " << vincita << " euro!\033[0m" << endl;
                    saldo += vincita;
                    vinciteHistory.push_back(vincita);  // Aggiungi la vincita alle schede vinte
                } else {
                    cout << "\033[1;31mNon hai vinto questa volta.\033[0m" << endl;
                }
            }
        } else {
            cout << "\033[1;31mSaldo esaurito. Non puoi piu' giocare.\033[0m" << endl;
            break;
        }
    }
}

// Funzione principale
int main() {
    srand(time(0));  // Inizializza il generatore di numeri casuali

    // Avvio del gioco
    gioca();

    return 0;
}
