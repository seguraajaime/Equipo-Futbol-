#include "partido.h"
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <fstream>
#include <algorithm>

// Gestor de partidos
class GestorPartidos {
private:
    std::vector<std::unique_ptr<Partido>> partidos;
    std::string ourTeam;

public:
    explicit GestorPartidos(const std::string& equipoNuestro) : ourTeam(equipoNuestro) {}

    void crearPartido(const std::string& local, const std::string& visitante) {
        partidos.push_back(std::make_unique<Partido>(local, visitante));
        std::cout << "✅ Partido creado con ID: " << partidos.back()->getId() << std::endl;
    }

    // Persistencia
    void guardarEnArchivo(const std::string& ruta) const {
        std::ofstream ofs(ruta, std::ios::trunc);
        if (!ofs) throw std::runtime_error("No se pudo abrir el archivo para escribir: " + ruta);
        for (const auto& p : partidos) {
            ofs << p->serializar() << std::endl;
        }
    }

    void cargarDesdeArchivo(const std::string& ruta) {
        std::ifstream ifs(ruta);
        if (!ifs) throw std::runtime_error("No se pudo abrir el archivo para leer: " + ruta);
        std::string linea;
        int maxId = 0;
        int lineaNum = 0;
        while (std::getline(ifs, linea)) {
            ++lineaNum;
            if (linea.empty()) continue;
            try {
                auto p = Partido::deserializar(linea);
                if (p) {
                    maxId = std::max(maxId, p->getId());
                    partidos.push_back(std::move(p));
                }
            } catch (const std::exception& e) {
                throw std::runtime_error(std::string("Error deserializando linea ") + std::to_string(lineaNum) + ": " + e.what());
            }
        }
        if (maxId > 0) Partido::setNextId(maxId + 1);
    }

    void convocarPartido(int id, const std::string& fecha) {
        for (auto& partido : partidos) {
            if (partido->getId() == id) {
                try {
                    partido->convocarPartido(fecha);
                    return;
                } catch (const std::exception& e) {
                    std::cerr << "❌ Error: " << e.what() << std::endl;
                }
            }
        }
        throw std::runtime_error("Partido no encontrado: " + std::to_string(id));
    }

    void registrarResultado(int id, int golesLocal, int golesVisitante) {
        for (auto& partido : partidos) {
            if (partido->getId() == id) {
                try {
                    partido->registrarResultado(golesLocal, golesVisitante);
                    return;
                } catch (const std::exception& e) {
                    std::cerr << "❌ Error: " << e.what() << std::endl;
                }
            }
        }
        throw std::runtime_error("Partido no encontrado: " + std::to_string(id));
    }

    void mostrarPartidos() {
        if (partidos.empty()) {
            std::cout << "📭 No hay partidos registrados." << std::endl;
            return;
        }

        std::cout << "\n========== PARTIDOS REGISTRADOS ==========" << std::endl;
        for (const auto& partido : partidos) {
            std::cout << partido->obtenerInfo() << std::endl;
        }
    }

    void mostrarPartido(int id) {
        for (const auto& partido : partidos) {
            if (partido->getId() == id) {
                std::cout << partido->obtenerInfo() << std::endl;
                return;
            }
        }
        throw std::runtime_error("Partido no encontrado: " + std::to_string(id));
    }

    int obtenerTotalPartidos() const {
        return partidos.size();
    }
};

// Menú principal
void mostrarMenu() {
    std::cout << "\n========== GESTOR DE PARTIDOS ==========" << std::endl;
    std::cout << "1. Crear nuevo partido" << std::endl;
    std::cout << "2. Convocar partido (programar fecha)" << std::endl;
    std::cout << "3. Registrar resultado" << std::endl;
    std::cout << "4. Ver todos los partidos" << std::endl;
    std::cout << "5. Ver detalles de un partido" << std::endl;
    std::cout << "6. Salir" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Selecciona una opción: ";
}

int main() {
    int opcion;

    std::cout << "🏆 BIENVENIDO AL GESTOR DE PARTIDOS DE FÚTBOL 🏆" << std::endl;
    std::string ourTeam;
    std::cout << "Nombre de nuestro equipo: ";
    std::getline(std::cin, ourTeam);
    GestorPartidos gestor(ourTeam);
    // Intentar cargar partidos previos (lanza excepción si falla)
    try {
        gestor.cargarDesdeArchivo("partidos.txt");
        std::cout << "🔄 Partidos cargados desde 'partidos.txt'." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "⚠️  Aviso: no se cargaron partidos: " << e.what() << std::endl;
    }

    while (true) {
        mostrarMenu();
        std::cin >> opcion;
        std::cin.ignore(); // Limpiar buffer de entrada

        try {
            switch (opcion) {
                case 1: {
                    std::string rival;
                    std::string resp;
                    std::cout << "\nNombre del equipo rival: ";
                    std::getline(std::cin, rival);
                    std::cout << "¿Somos locales? (s/n): ";
                    std::getline(std::cin, resp);
                    std::string local, visitante;
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
                    std::string fecha;
                    std::cout << "\nID del Partido: ";
                    std::cin >> id;
                    std::cin.ignore();
                    std::cout << "Fecha (YYYY-MM-DD HH:MM:SS): ";
                    std::getline(std::cin, fecha);
                    gestor.convocarPartido(id, fecha);
                    break;
                }

                case 3: {
                    int id, golesLocal, golesVisitante;
                    std::cout << "\nID del Partido: ";
                    std::cin >> id;
                    std::cout << "Goles Local: ";
                    std::cin >> golesLocal;
                    std::cout << "Goles Visitante: ";
                    std::cin >> golesVisitante;
                    gestor.registrarResultado(id, golesLocal, golesVisitante);
                    break;
                }

                case 4: {
                    gestor.mostrarPartidos();
                    break;
                }

                case 5: {
                    int id;
                    std::cout << "\nID del Partido: ";
                    std::cin >> id;
                    gestor.mostrarPartido(id);
                    break;
                }

                case 6: {
                    // Guardar partidos antes de salir (lanza excepción si falla)
                    try {
                        gestor.guardarEnArchivo("partidos.txt");
                        std::cout << "💾 Partidos guardados en 'partidos.txt'." << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "❌ No se pudo guardar 'partidos.txt': " << e.what() << std::endl;
                    }
                    std::cout << "\n👋 ¡Hasta luego!" << std::endl;
                    return 0;
                }

                default: {
                    std::cerr << "❌ Opción no válida. Intenta de nuevo." << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "❌ Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
