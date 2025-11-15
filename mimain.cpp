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
    try
    {
         menu();
    }catch(const exception& e)
    {
        cerr<< "Error no se pudo abrir jugadores"  << '\n';
    }
};
    
 

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
    string dorsalStr;
    string nombre, posicion;

    cout << "\n=== Alta de jugador ===\n";
    cout << "Dorsal: ";
    getline(cin, dorsalStr);
    try
    {
        if (dorsalStr.empty())
        {
            throw invalid_argument("El dorsal no puede estar vacío");
        }

        int dorsal = stoi(dorsalStr);
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
    catch(const exception& e)
    {
        cerr <<"Error el dorsal ha de ser un numero " << '\n';
    }
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
        try
        {
            Jugador* j = Jugador::deserializar(linea);
            hayAlmenosUno = true;

            fecha f = j->getFechaInicioContrato();

            cout << "-------------------------\n";
            cout << "Dorsal:     " << j->getDorsal() << '\n';
            cout << "Nombre:     " << j->getNombre() << '\n';
            cout << "Posicion:   " << j->getPosicion() << '\n';
            cout << "Disponible: " << (j->estaDisponible() ? "Si" : "No") << '\n';
            cout << "Fecha inicio contrato: " << f.dia << "/" << f.mes << "/" << f.ano << '\n';
            delete j;
        }
        catch(const exception& e)
        {
           cerr <<"Linea ignorada por error: "<< '\n';
        }   
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

    int opcion2;
    cout << "\nRescindir contrato por:\n";
    cout << "1. Dorsal\n";
    cout << "2. Volver al menu\n";
    cout << "Elige opcion: ";
    cin >> opcion2;
    cin.ignore();

    int dorsalBuscado = -1;

    if (opcion2 == 1) {
        cout << "Introduce el dorsal: ";
        cin >> dorsalBuscado;
        cin.ignore();
    } else if (opcion2 == 2) {
        cout << "Volviendo al menu...\n";
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

        if (j->getDorsal() == dorsalBuscado) {
            encontrado = true;
            cout << "Eliminando: " << j->getNombre()
                 << " (dorsal " << j->getDorsal() << ")\n";
            // NO guardamos esta línea, así se elimina
        } else {
            nuevasLineas.push_back(linea);  // mantenemos esta línea
        }

        delete j;
    }

    fin.close();

    if (!encontrado) {
        cout << "No se ha encontrado ningun jugador con esos datos.\n";
        return;
    }

    ofstream fout("jugadores.txt", ios::trunc);
    if (!fout) {
        cerr << "ERROR: No se pudo abrir jugadores.txt para escribir.\n";
        return;
    }

    for (const string& l : nuevasLineas) {
        fout << l << '\n';
    }
    fout.close();

    cout << "Jugador eliminado correctamente.\n";
}
