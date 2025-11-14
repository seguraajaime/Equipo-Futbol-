#include <iostream>
#include <fstream>
#include <string>
#include "Jugador.h"
using namespace std;

int main() {
    int dorsal;
    string nombre;
    string posicion;

    cout << "=== Alta de jugador ===\n";

    cout << "Dorsal: ";
    cin >> dorsal;
    cin.ignore(); // limpia el salto de línea

    cout << "Nombre: ";
    getline(cin, nombre);

    cout << "Posicion: ";
    getline(cin, posicion);
    

    // Creamos el jugador y firmamos contrato (pone fecha de hoy)
    Jugador j(dorsal, nombre, posicion);
    j.firmarContrato();

    // Abrimos el fichero en modo "append" (añadir al final)
    ofstream fout("jugadores.txt", ios::app);
    if (!fout) {
        cerr << "Error: no se pudo abrir jugadores.txt\n";
        return 1;
    }

    fout << j.serializar() << '\n';
    fout.close();

    cout << "Jugador guardado en jugadores.txt\n";
    return 0;
}
