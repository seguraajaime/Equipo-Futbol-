#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Jugador.h"

using namespace std;


void menu();
void ficharJugador();
void mostrarPlantilla();
void rescindirContratoJugador();

int main() {
    menu();
    return 0;
}

void menu() {
    int opcion = 0;
    do {
        cout << "\n===== MENU EQUIPO =====\n";
        cout << "1. Fichar jugador\n";
        cout << "2. Ver toda la plantilla\n";
        cout << "3. Rescindir contrato jugador\n";
        cout << "4. Salir\n";
        cout << "Elige una opcion: ";
        cin >> opcion;
        cin.ignore(); 

        switch (opcion) {
        case 1:
            ficharJugador();
            break;
        case 2:
            mostrarPlantilla();
            break;
        case 3:
            rescindirContratoJugador();
            break;
        case 4:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opcion no valida\n";
            break;
        }

    } while (opcion != 4);
}


void ficharJugador() {
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

void mostrarPlantilla() {
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
    };

    
}
void rescindirContratoJugador() {
    ifstream fin("jugadores.txt");
    if (!fin) {
        cout << "No hay archivo jugadores.txt todavia.\n";
        return;
    }

    int modo;
    cout << "\nRescindir contrato por:\n";
    cout << "1. Dorsal\n";
    cout << "2. Volver al menu\n";
    cout << "Elige opcion: ";
    cin >> modo;
    cin.ignore();

    int dorsalBuscado = -1;
    string nombreBuscado;

    if (modo == 1) {
        cout << "Introduce el dorsal: ";
        cin >> dorsalBuscado;
        cin.ignore();
    } else if (modo == 2) {
        return;
    } else {
        cout << "Opcion no valida.\n";
        return;
    }

    vector<string> nuevasLineas;
    string linea;
    bool encontrado = false;

    while (getline(fin, linea)) {
        if (linea.empty()) continue;

        Jugador* j = Jugador::deserializar(linea);
        bool borrar = false;

        if (modo == 1 && j->getDorsal() == dorsalBuscado) {
            borrar = true;
        } 

        if (borrar) {
            encontrado = true;
            cout << "Eliminando: " << j->getNombre()
                 << " (dorsal " << j->getDorsal() << ")\n";
        } else {
            nuevasLineas.push_back(linea);  // mantenemos esta linea
        }

        delete j;
    }

    fin.close();

    if (!encontrado) {
        cout << "No se ha encontrado ningun jugador con esos datos.\n";
        return;
    }

    // Sobrescribimos el archivo SOLO con los que se quedan
    ofstream fout("jugadores.txt", ios::trunc);
    for (const string& l : nuevasLineas) {
        fout << l << '\n';
    }
    fout.close();

    cout << "Jugador eliminado correctamente.\n";
}; 