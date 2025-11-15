#include "partido.h"
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;
// Gestor de partidos
class GestorPartidos {
private:
    vector<unique_ptr<Partido>> partidos;
    string ourTeam;

public:
    explicit GestorPartidos(const string& equipoNuestro) : ourTeam(equipoNuestro) {}

    void crearPartido(const string& local, const string& visitante) {
        partidos.push_back(make_unique<Partido>(local, visitante));
        cout << "Partido creado con ID: " << partidos.back()->getId() << endl;
    }

    // Persistencia
    void guardarEnArchivo(const string& ruta) const {
        ofstream ofs(ruta, ios::trunc);
        if (!ofs) throw runtime_error("No se pudo abrir el archivo para escribir: " + ruta);
        
        // Guardar en formato CSV puro (legible por deserializar)
        for (const auto& p : partidos) {
            ofs << p->serializar() << endl;
        }
    }

    void mostrarTablaEnArchivo(const string& ruta) const {
        ofstream ofs(ruta, ios::trunc);
        if (!ofs) throw runtime_error("No se pudo abrir el archivo para escribir: " + ruta);
        
        // Escribir encabezado de tabla
        ofs << setfill('-') << setw(110) << "" << endl;
        ofs << setfill(' ') 
            << setw(4) << "ID" << "| "
            << setw(20) << "LOCAL" << "| "
            << setw(20) << "VISITANTE" << "| "
            << setw(19) << "FECHA" << "| "
            << setw(6) << "LOCAL" << "| "
            << setw(6) << "VISIT" << "| "
            << setw(10) << "ESTADO" << endl;
        ofs << setfill('-') << setw(110) << "" << endl;
        
        // Escribir datos de partidos
        for (const auto& p : partidos) {
            ofs << p->serializarTabla() << endl;
        }
        
        ofs << setfill('-') << setw(110) << "" << endl;
    }

    void cargarDesdeArchivo(const string& ruta) {
        ifstream ifs(ruta);
        if (!ifs) throw runtime_error("No se pudo abrir el archivo para leer: " + ruta);
        string linea;
        int maxId = 0;
        int lineaNum = 0;
        while (getline(ifs, linea)) {
            ++lineaNum;
            if (linea.empty()) continue;
            try {
                auto p = Partido::deserializar(linea);
                if (p) {
                    maxId = max(maxId, p->getId());
                    partidos.push_back(move(p));
                }
            } catch (const exception& e) {
                throw runtime_error(string("Error deserializando linea ") + to_string(lineaNum) + ": " + e.what());
            }
        }
        if (maxId > 0) Partido::setNextId(maxId + 1);
    }

    void convocarPartido(int id, const string& fecha) {
        for (auto& partido : partidos) {
            if (partido->getId() == id) {
                try {
                    partido->convocarPartido(fecha);
                    return;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }
        throw runtime_error("Partido no encontrado: " + to_string(id));
    }

    void registrarResultado(int id, int golesLocal, int golesVisitante) {
        for (auto& partido : partidos) {
            if (partido->getId() == id) {
                try {
                    partido->registrarResultado(golesLocal, golesVisitante);
                    return;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }
        throw runtime_error("Partido no encontrado: " + to_string(id));
    }

    void cancelarPartido(int id) {
        auto it = partidos.begin();
        while (it != partidos.end()) {
            if ((*it)->getId() == id) {
                // Verificar que el partido no sea JUGADO
                if ((*it)->getEstado() == EstadoPartido::JUGADO) {
                    throw runtime_error("No se puede cancelar un partido que ya ha sido jugado.");
                }
                partidos.erase(it);
                cout << "Partido ID " << id << " ha sido cancelado y eliminado." << endl;
                return;
            }
            ++it;
        }
        throw runtime_error("Partido no encontrado: " + to_string(id));
    }

    void mostrarPartidos() {
        if (partidos.empty()) {
            cout << "No hay partidos registrados." << endl;
            return;
        }

        cout << "\n========== PARTIDOS REGISTRADOS ==========" << endl;
        for (const auto& partido : partidos) {
            cout << partido->obtenerInfo() << endl;
        }
    }

    void mostrarPartido(int id) {
        for (const auto& partido : partidos) {
            if (partido->getId() == id) {
                cout << partido->obtenerInfo() << endl;
                return;
            }
        }
        throw runtime_error("Partido no encontrado: " + to_string(id));
    }

    int obtenerTotalPartidos() const {
        return partidos.size();
    }
};

// Menú principal
void mostrarMenu() {
    cout << "\n========== GESTOR DE PARTIDOS ==========" << endl;
    cout << "1. Crear nuevo partido" << endl;
    cout << "2. Convocar partido (programar fecha)" << endl;
    cout << "3. Registrar resultado" << endl;
    cout << "4. Ver todos los partidos" << endl;
    cout << "5. Ver detalles de un partido" << endl;
    cout << "6. Cancelar partido" << endl;
    cout << "7. Salir" << endl;
    cout << "========================================" << endl;
    cout << "Selecciona una opcion: ";
}

int main() {
    int opcion;
    const string ourTeam = "Canadio"; // Club definido

    cout << "BIENVENIDO AL GESTOR DE PARTIDOS DE " << ourTeam << endl;
    GestorPartidos gestor(ourTeam);
    // Intentar cargar partidos previos (lanza excepción si falla)
    try {
        gestor.cargarDesdeArchivo("partidos.txt");
        cout << "Partidos cargados desde 'partidos.txt'." << endl;
    } catch (const std::exception& e) {
        cout << "Aviso: no se cargaron partidos: " << e.what() << endl;
    }

    while (true) {
        mostrarMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar buffer de entrada

        try {
            switch (opcion) {
                case 1: {
                    string rival;
                    string resp;
                    cout << "\nNombre del equipo rival: ";
                    getline(cin, rival);
                    cout << "Somos locales? (s/n): ";
                    getline(cin, resp);
                    string local, visitante;
                    if (!resp.empty() && (resp[0] == 's' || resp[0] == 'S')) {
                        local = ourTeam;
                        visitante = rival;
                    } else {
                        local = rival;
                        visitante = ourTeam;
                    }
                    gestor.crearPartido(local, visitante);
                    break;
                }

                case 2: {
                    int id;
                    string fecha;
                    cout << "\nID del Partido: ";
                    cin >> id;
                    cin.ignore();
                    cout << "Fecha (DD-MM-YYYY HH:MM:SS): ";
                    getline(std::cin, fecha);
                    gestor.convocarPartido(id, fecha);
                    break;
                }

                case 3: {
                    int id, golesLocal, golesVisitante;
                    cout << "\nID del Partido: ";
                    cin >> id;
                    cout << "Goles Local: ";
                    cin >> golesLocal;
                    cout << "Goles Visitante: ";
                    cin >> golesVisitante;
                    gestor.registrarResultado(id, golesLocal, golesVisitante);
                    break;
                }

                case 4: {
                    gestor.mostrarPartidos();
                    break;
                }

                case 5: {
                    int id;
                    cout << "\nID del Partido: ";
                    cin >> id;
                    gestor.mostrarPartido(id);
                    break;
                }

                case 6: {
                    int id;
                    cout << "\nID del Partido a cancelar: ";
                    cin >> id;
                    gestor.cancelarPartido(id);
                    break;
                }

                case 7: {
                    // Guardar partidos antes de salir (lanza excepción si falla)
                    try {
                        gestor.guardarEnArchivo("partidos.txt");
                        cout << "Partidos guardados en 'partidos.txt' (formato CSV)." << endl;
                        
                        // Guardar también la tabla de visualización
                        gestor.mostrarTablaEnArchivo("partidos_tabla.txt");
                        cout << "Tabla guardada en 'partidos_tabla.txt'." << endl;
                    } catch (const exception& e) {
                        cerr << "Error al guardar: " << e.what() << endl;
                    }
                    cout << "\nHasta luego!" << endl;
                    return 0;
                }

                default: {
                    cerr << "Opcion no valida. Intenta de nuevo." << endl;
                }
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
