#include <iostream>
#include <fstream>
#include <string>
#include "Jugador.h"

using namespace std;


void menu();
void anadirJugador();
void mostrarJugadores();

int main() {
    menu();
    return 0;
}

void menu() {
    int opcion = 0;
    do {
        cout << "\n===== MENU EQUIPO =====\n";
        cout << "1. Anadir jugador\n";
        cout << "2. Ver todos los jugadores\n";
        cout << "3. Salir\n";
        cout << "Elige una opcion: ";
        cin >> opcion;
        cin.ignore(); 

        switch (opcion) {
        case 1:
            anadirJugador();
            break;
        case 2:
            mostrarJugadores();
            break;
        case 3:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opcion no valida\n";
            break;
        }

    } while (opcion != 3);
}


void anadirJugador() {
    int dorsal;
    string nombre, posicion;

    cout << "\n=== Alta de jugador ===\n";
    cout << "Dorsal: ";
    cin >> dorsal;
    cin.ignore();

    cout << "Nombre: ";
    getline(cin, nombre);

    cout << "Posicion: ";
    getline(cin, posicion);

    Jugador j(dorsal, nombre, posicion);
    j.firmarContrato();

    ofstream fout("jugadores.txt", ios::app);
    if (!fout) {
        cerr << "ERROR: No se pudo abrir jugadores.txt\n";
        return;
    }

    fout << j.serializar() << '\n';
    fout.close();

    cout << "Jugador guardado en jugadores.txt\n";
}

void mostrarJugadores() {
    ifstream fin("jugadores.txt");
    if (!fin) {
        cout << "No hay archivo jugadores.txt todavia.\n";
        return;
    }

    cout << "\n=== Lista de jugadores ===\n";

    string linea;
    bool hayAlmenosUno = false;

    while (getline(fin, linea)) {
        if (linea.empty()) continue;

        Jugador* j = Jugador::deserializar(linea);
        hayAlmenosUno = true;

        fecha f = j->getFechaInicioContrato();

        cout << "-------------------------\n";
        cout << "Dorsal:     " << j->getDorsal() << '\n';
        cout << "Nombre:     " << j->getNombre() << '\n';
        cout << "Posicion:   " << j->getPosicion() << '\n';
        cout << "Disponible: " << (j->estaDisponible() ? "Si" : "No") << '\n';
        cout << "Fecha inicio contrato: "
             << f.dia << "/" << f.mes << "/" << f.ano << '\n';

        delete j;
    }

    if (!hayAlmenosUno) {
        cout << "No hay jugadores en el archivo.\n";
    }
}
    