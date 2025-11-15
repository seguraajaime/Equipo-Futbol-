#include "Contrato.h"
#include <sstream>
#include <iostream>  
#include <string>
#include <stdexcept>
#include <ctime>
#include <vector>
#include <memory>
using namespace std;

time_t leerFecha(const string& mensaje) {
    cout << mensaje;
    int dia, mes, anio;
    cin >> dia >> mes >> anio;
    
    tm timeinfo = {};
    timeinfo.tm_mday = dia;
    timeinfo.tm_mon = mes - 1;
    timeinfo.tm_year = anio - 1900;
    timeinfo.tm_isdst = -1;
    
    time_t fecha = mktime(&timeinfo);
    if (fecha == -1) {
        cerr << "Fecha inválida. Intente de nuevo." << endl;
        return 0;
    }
    return fecha;
}

void mostrarMenu() {
    cout << "\n=== GESTOR DE CONTRATOS DE FUTBOL ===" << endl;
    cout << "1. Crear nuevo contrato" << endl;
    cout << "2. Listar contratos" << endl;
    cout << "3. Guardar contratos en archivo" << endl;
    cout << "4. Cargar contratos desde archivo" << endl;
    cout << "5. Salir" << endl;
    cout << "Seleccione una opción: ";
}

int main() {
    vector<unique_ptr<Contrato>> contratos;
    int opcion = 0;
    
    while (true) {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada
        
        switch (opcion) {
            case 1: { // Crear nuevo contrato
                cout << "\n--- CREAR NUEVO CONTRATO ---" << endl;
                
                string nombre, equipo;
                double salario;
                
                cout << "Nombre del jugador: ";
                getline(cin, nombre);
                
                cout << "Nombre del equipo: ";
                getline(cin, equipo);
                
                cout << "Salario (clausula): ";
                cin >> salario;
                cin.ignore();
                
                time_t fechaInicio = leerFecha("Fecha de inicio (DD MM AAAA): ");
                time_t fechaFin = leerFecha("Fecha de fin (DD MM AAAA): ");
                
                // 1. Crea el puntero inteligente en su propia variable
                auto nuevoContrato = unique_ptr<Contrato>(new Contrato(nombre, equipo, fechaInicio, fechaFin, salario));

                // 2. Muévelo explícitamente al vector usando std::move
                contratos.push_back(std::move(nuevoContrato));
                
                cout << "\n Contrato creado exitosamente" << endl;
                cout << "Jugador: " << contratos.back()->getnombre() << endl;
                cout << "Equipo: " << nuevoContrato->getEquipoNombre() << endl;
                cout << "Inicio: " << nuevoContrato->getFechaInicioStr() << endl;
                cout << "Fin: " << nuevoContrato->getFechaFinStr() << endl;
                cout << "Salario: $" << nuevoContrato->getClausula() << endl;
                break;
            }
            
            case 2: { // Listar contratos
                cout << "\n--- CONTRATOS REGISTRADOS ---" << endl;
                if (contratos.empty()) {
                    cout << "No hay contratos registrados." << endl;
                } else {
                    for (size_t i = 0; i < contratos.size(); i++) {
                        cout << "\n" << i + 1 << ". " << contratos[i]->getnombre() << endl;
                        cout << "   Equipo: " << contratos[i]->getEquipoNombre() << endl;
                        cout << "   Inicio: " << contratos[i]->getFechaInicioStr() << endl;
                        cout << "   Fin: " << contratos[i]->getFechaFinStr() << endl;
                        cout << "   Salario: $" << contratos[i]->getClausula() << endl;
                    }
                }
                break;
            }
            
            case 3: { // Guardar contratos en archivo
                cout << "\nNombre del archivo (ej: contratos.txt): ";
                string archivo;
                getline(cin, archivo);
                
                try {
                    Contrato::guardarEnArchivo(contratos, archivo);
                    cout << "✓ Contratos guardados exitosamente" << endl;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            
            case 4: { // Cargar contratos desde archivo
                cout << "\nNombre del archivo (ej: contratos.txt): ";
                string archivo;
                getline(cin, archivo);
                
                try {
                contratos.clear();
                contratos = Contrato::cargarDesdeArchivo(archivo);
                cout << "✓ Contratos cargados exitosamente" << endl;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            
            case 5: { // Salir
                cout << "\nLiberando memoria..." << endl;
                cout << "¡Hasta luego!" << endl;
                return 0;
            }
            
            default:
                cout << "Opción inválida. Intente de nuevo." << endl;
        }
    }
    return 0;
}

