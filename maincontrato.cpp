#include "Contrato.h"
#include <sstream>
#include <iostream>  
#include <string>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <vector>
#include <memory>
using namespace std;

time_t leerFecha(const string& mensaje) {
    cout << mensaje << " (DD/MM/AAAA): ";
    string fechaStr;
    cin >> fechaStr;
    
    // Parsear formato DD/MM/AAAA
    int dia, mes, anio;
    if (sscanf(fechaStr.c_str(), "%d/%d/%d", &dia, &mes, &anio) != 3) {
        cerr << "Formato inválido. Use DD/MM/AAAA" << endl;
        return 0;
    }
    
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
    cout << "Seleccione una opcion: ";
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
                
                time_t fechaInicio = leerFecha("Fecha de inicio");
                time_t fechaFin = leerFecha("Fecha de fin");
                
                // Crear nuevo contrato y agregarlo al vector
                auto nuevoContrato = make_unique<Contrato>(nombre, equipo, fechaInicio, fechaFin, salario);
                
                // Obtener puntero antes de hacer std::move
                Contrato* contratoPtr = nuevoContrato.get();
                
                // Mover al vector
                contratos.push_back(std::move(nuevoContrato));
                
                // Mostrar toda la información del contrato creado
                cout << "\n=== CONTRATO CREADO EXITOSAMENTE ===" << endl;
                cout << "Jugador: " << contratoPtr->getnombre() << endl;
                cout << "Equipo: " << contratoPtr->getEquipoNombre() << endl;
                cout << "Fecha inicio: " << contratoPtr->getFechaInicioStr() << endl;
                cout << "Fecha fin: " << contratoPtr->getFechaFinStr() << endl;
                cout << "Salario/Clausula: $" << contratoPtr->getClausula() << endl;
                cout << "=====================================" << endl;
                break;
            }
            
            case 2: { // Listar contratos
                cout << "\n--- CONTRATOS CREADOS ---" << endl;
                if (contratos.empty()) {
                    cout << "No hay contratos registrados en esta sesion." << endl;
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
                cout << "\nGuardando contratos de esta sesion en: contratos.txt" << endl;
                try {
                    Contrato::guardarEnArchivo(contratos, "contratos.txt");
                    cout << contratos.size() << " contrato(s) guardado(s) exitosamente" << endl;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            
            case 4: { // Cargar contratos desde archivo
                cout << "\nCargando contratos desde: contratos.txt" << endl;
                try {
                    auto contratosArchivo = Contrato::cargarDesdeArchivo("contratos.txt");
                    cout << "\n--- CONTRATOS DEL ARCHIVO ---" << endl;
                    if (contratosArchivo.empty()) {
                        cout << "No hay contratos guardados en el archivo." << endl;
                    } else {
                        for (size_t i = 0; i < contratosArchivo.size(); i++) {
                            cout << "\n" << i + 1 << ". " << contratosArchivo[i]->getnombre() << endl;
                            cout << "   Equipo: " << contratosArchivo[i]->getEquipoNombre() << endl;
                            cout << "   Inicio: " << contratosArchivo[i]->getFechaInicioStr() << endl;
                            cout << "   Fin: " << contratosArchivo[i]->getFechaFinStr() << endl;
                            cout << "   Salario: $" << contratosArchivo[i]->getClausula() << endl;
                        }
                        cout << contratosArchivo.size() << " contrato(s) cargado(s) del archivo" << endl;
                    }
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
                break;
            }
            
            case 5: { // Salir
                cout << "\nLiberando memoria..." << endl;
                cout << "Hasta luego!" << endl;
                return 0;
            }
            
            default:
                cout << "Opcion invalida. Intente de nuevo." << endl;
        }
    }
    return 0;
}

