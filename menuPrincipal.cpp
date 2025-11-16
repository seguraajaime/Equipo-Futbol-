#include "contrato.h"
#include "jugador.h"
#include "partido.h"
#include <sstream>
#include <iostream> 
#include <string>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <vector>
#include <memory> 
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <limits>

using namespace std;

// === DECLARACIONES GLOBALES DE GESTORES ===
vector<unique_ptr<Jugador>> g_plantilla; // Lista de jugadores en memoria
vector<unique_ptr<Contrato>> g_contratos; // Lista de contratos en memoria
vector<unique_ptr<Partido>> g_partidos; // Lista de partidos en memoria

// === DECLARACIONES DE FUNCIONES ===
// (Implementaciones al final)
time_t leerFecha(const string& mensaje);
void cargarPlantillaInicial();
void guardarPlantillaEnArchivo();
void menu();
void menuJugadores();
void menuContratos();
void menuPartidos();
void ficharJugador();
void mostrarPlantilla();
void rescindirContratoJugador();
// ------------------------------------

// === FUNCIÓN PRINCIPAL ===
int main() { 
    // Cargar los datos al inicio del programa
    cargarPlantillaInicial();

    try {
        menu();
    } catch(const exception& e) {
        cerr << "Error no esperado en el menu principal: " << e.what() << '\n';
    }

    // Guardar los datos al salir (solo si se sale por opción 4)
    // Nota: El guardar se hará al salir del menú.
    
    return 0;
}

// === IMPLEMENTACIÓN DE LA LÓGICA DE GESTIÓN ===

// [NOTA: Aquí he movido toda la clase GestorPartidos para limpiar el archivo,
// pero tú la deberías tener en 'GestorPartidos.h' y 'GestorPartidos.cpp']

// === FUNCIONES DE MENÚ PRINCIPAL ===

void menu() {
    int opcion = 0;
    do {
        cout << "\n";
        cout << "==========================================" << endl;
        cout << "    BIENVENIDO A GESTION DE EQUIPO      " << endl;
        cout << "==========================================" << endl;
        cout << "\n--- MENU PRINCIPAL ---" << endl;
        cout << "1. Gestion de Jugadores" << endl;
        cout << "2. Gestion de Contratos" << endl;
        cout << "3. Gestion de Partidos" << endl;
        cout << "4. Salir" << endl;
        cout << "\nElige una opcion: ";
        
        // **CORRECCIÓN DE INPUT**
        if (!(cin >> opcion)) {
            cout << "Opcion no valida (debe ser un numero).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (opcion) {
            case 1:
                menuJugadores();
                break;
            case 2:
                menuContratos();
                break;
            case 3:
                menuPartidos();
                break;
            case 4:
                // Guardar la plantilla antes de salir
                guardarPlantillaEnArchivo();
                cout << "\nHasta luego! Gracias por usar el sistema de gestion." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }

    } while (opcion != 4);
}

// === LÓGICA DE CARGA Y GUARDADO ===

void cargarPlantillaInicial() {
    // Carga jugadores desde archivo `jugadores.txt` evitando duplicados
    cout << "Cargando datos iniciales...\n";
    // Informar ruta de trabajo para depuracion
    try {
        cout << "Directorio actual: " << std::filesystem::current_path().string() << '\n';
    } catch (...) {
        // ignore if filesystem not available
    }

    if (!std::filesystem::exists("jugadores.txt")) {
        cout << "No existe archivo jugadores.txt en el directorio actual, se saltara la carga inicial.\n";
        return;
    }

    ifstream ifs("jugadores.txt");
    if (!ifs.is_open()) {
        cout << "No se pudo abrir jugadores.txt a pesar de existir, ver permisos.\n";
        return;
    }

    string linea;
    int cargados = 0;
    auto trim = [](string &s) {
        while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' ' || s.back() == '\t')) s.pop_back();
        size_t i = 0;
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t')) i++;
        if (i > 0) s.erase(0, i);
    };

    while (getline(ifs, linea)) {
        trim(linea);
        if (linea.empty()) continue;
        try {
            Jugador* j = Jugador::deserializar(linea);
            if (j == nullptr) continue;

            // Comprobar si dorsal ya existe en memoria
            bool existe = false;
            for (const auto& existing : g_plantilla) {
                if (existing->getDorsal() == j->getDorsal()) {
                    existe = true;
                    break;
                }
            }

            if (!existe) {
                g_plantilla.push_back(std::unique_ptr<Jugador>(j));
                ++cargados;
            } else {
                delete j; // ya existía
            }
        } catch (const exception& e) {
            cerr << "Error al deserializar jugador: " << e.what() << "\n";
        }
    }
    ifs.close();
    cout << cargados << " jugadores cargados en memoria desde jugadores.txt\n";
}

void guardarPlantillaEnArchivo() {
    // Guarda el contenido de g_plantilla en jugadores.txt (sobrescribe)
    cout << "Guardando plantilla en jugadores.txt...\n";
    ofstream ofs("jugadores.txt", ios::trunc);
    if (!ofs.is_open()) {
        cerr << "Error: no se pudo abrir jugadores.txt para escribir.\n";
        return;
    }

    for (const auto& j : g_plantilla) {
        ofs << j->serializar() << '\n';
    }
    ofs.close();
    cout << "Plantilla guardada (" << g_plantilla.size() << " jugadores)\n";
}

// === FUNCIONES DE MENÚ JUGADORES ===

void menuJugadores() {
    int opcion = 0;
    do {
        // ... (Tu código de menú Jugadores aquí)
        cout << "\n--- MENU JUGADORES ---" << endl;
        cout << "1. Fichar jugador" << endl;
        cout << "2. Ver plantilla" << endl;
        cout << "3. Rescindir contrato" << endl;
        cout << "4. Volver al menu principal" << endl;
        cout << "Elige una opcion: ";
        
        // **CORRECCIÓN DE INPUT**
        if (!(cin >> opcion)) {
            cout << "Opcion no valida (debe ser un numero).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // ... (Tu switch con los casos 1, 2, 3, 4)
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
                 cout << "Volviendo al menu principal..." << endl;
                 break;
            default:
                 cout << "Opcion no valida." << endl;
                 break;
         }

    } while (opcion != 4);
}

void ficharJugador() {
    // **NOTA:** Esta función necesita ser refactorizada para usar g_plantilla.
    // He eliminado la lógica de archivo y la he adaptado para añadir a la RAM.
    
    string dorsalStr;
    string nombre, posicion;

    cout << "\n=== Alta de jugador ===\n";
    cout << "Dorsal: ";
    getline(cin, dorsalStr);
    
    try {
        if (dorsalStr.empty()) {
            throw invalid_argument("El dorsal no puede estar vacio");
        }

        int dorsal = stoi(dorsalStr);
        // Aquí deberías comprobar si el dorsal ya existe en g_plantilla
        
        cout << "Nombre: ";
        getline(cin, nombre);
        cout << "Posicion: ";
        getline(cin, posicion);

        // Creamos el jugador con puntero inteligente (buena práctica)
        auto nuevoJugador = make_unique<Jugador>(dorsal, nombre, posicion);
        nuevoJugador->firmarContrato(); // Firmar contrato
        
        // Añadir a la plantilla en RAM
        g_plantilla.push_back(std::move(nuevoJugador)); 
        
        // Guardamos todo el estado al archivo después del cambio (seguridad)
        guardarPlantillaEnArchivo(); 

        cout << "Jugador '" << nombre << "' fichado y guardado.\n";
        
    } catch(const exception& e) {
        cerr <<"Error al fichar jugador: " << e.what() << '\n';
    }
}

void mostrarPlantilla() {
    // Muestra la plantilla desde la RAM (g_plantilla)
    cout << "\n=== Lista de jugadores (Desde RAM) ===\n";

    if (g_plantilla.empty()) {
        cout << "La plantilla esta vacia.\n";
        return;
    }

    for (const auto& j : g_plantilla) {
        cout << "-------------------------\n";
        cout << "Dorsal:     " << j->getDorsal() << '\n';
        cout << "Nombre:     " << j->getNombre() << '\n';
        cout << "Posicion:   " << j->getPosicion() << '\n';
        // Nota: Necesitas la definición de la struct 'fecha' para que esta línea compile
        // cout << "Fecha inicio contrato: " << f.dia << "/" << f.mes << "/" << f.ano << '\n'; 
        cout << "Disponible: " << (j->estaDisponible() ? "Si" : "No") << '\n';
    }
}

void rescindirContratoJugador() {
    // **NOTA:** Esta función fue adaptada para leer de RAM (g_plantilla) y escribir en disco.
    
    int dorsalBuscado = -1;
    cout << "\nRescindir contrato (dorsal): ";
    
    // **CORRECCIÓN DE INPUT**
    if (!(cin >> dorsalBuscado)) {
        cout << "Opcion no valida (debe ser un numero).\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Buscar y eliminar de g_plantilla
    auto it = g_plantilla.begin();
    while (it != g_plantilla.end()) {
        if ((*it)->getDorsal() == dorsalBuscado) {
            cout << "Eliminando: " << (*it)->getNombre() << " (dorsal " << dorsalBuscado << ")\n";
            it = g_plantilla.erase(it); // Borra y avanza el iterador
            guardarPlantillaEnArchivo(); // Guarda el cambio al disco
            cout << "Jugador eliminado correctamente.\n";
            return;
        }
        ++it;
    }

    cout << "No se ha encontrado ningun jugador con ese dorsal.\n";
}

// === FUNCIÓN AUXILIAR PARA LEER FECHAS ===
time_t leerFechaFormato(const string& mensaje) {
    cout << mensaje << " (DD/MM/AAAA): ";
    string fechaStr;
    cin >> fechaStr;
    cin.ignore();
    
    int dia, mes, anio;
    if (sscanf(fechaStr.c_str(), "%d/%d/%d", &dia, &mes, &anio) != 3) {
        cerr << "Formato invalido. Use DD/MM/AAAA" << endl;
        return 0;
    }
    
    tm timeinfo = {};
    timeinfo.tm_mday = dia;
    timeinfo.tm_mon = mes - 1;
    timeinfo.tm_year = anio - 1900;
    timeinfo.tm_isdst = -1;
    
    time_t fecha = mktime(&timeinfo);
    if (fecha == -1) {
        cerr << "Fecha invalida." << endl;
        return 0;
    }
    return fecha;
}

// === FUNCIÓN PARA CONVERTIR time_t A STRING ===
string time_t_a_string(time_t tiempo) {
    if (tiempo == 0) return "Por definir";
    tm* ltm = localtime(&tiempo);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", ltm);
    return string(buffer);
}

void menuContratos() {
    int opcion;
    
    do {
        cout << "\n--- MENU CONTRATOS ---" << endl;
        cout << "1. Crear Contrato" << endl;
        cout << "2. Ver Contratos" << endl;
        cout << "3. Guardar Contratos en archivo" << endl;
        cout << "4. Cargar Contratos desde archivo" << endl;
        cout << "5. Volver al menu principal" << endl;
        cout << "Selecciona una opcion: ";
        
        if (!(cin >> opcion)) {
            cout << "Opcion no valida (debe ser un numero).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (opcion) {
        case 1: {
            cout << "\n=== CREAR NUEVO CONTRATO ===" << endl;
            string nombre, equipo;
            double salario;
            
            cout << "Nombre del jugador: ";
            getline(cin, nombre);
            cout << "Nombre del equipo: ";
            getline(cin, equipo);
            cout << "Salario/Clausula: ";
            cin >> salario;
            cin.ignore();
            
            time_t fechaInicio = leerFechaFormato("Fecha de inicio");
            time_t fechaFin = leerFechaFormato("Fecha de fin");
            
            auto nuevoContrato = make_unique<Contrato>(nombre, equipo, fechaInicio, fechaFin, salario);
            Contrato* contPtr = nuevoContrato.get();
            g_contratos.push_back(std::move(nuevoContrato));
            
            cout << "\nCONTRATO CREADO:" << endl;
            cout << "  Jugador: " << contPtr->getnombre() << endl;
            cout << "  Equipo: " << contPtr->getEquipoNombre() << endl;
            cout << "  Inicio: " << contPtr->getFechaInicioStr() << endl;
            cout << "  Fin: " << contPtr->getFechaFinStr() << endl;
            cout << "  Salario: $" << contPtr->getClausula() << endl;
            break;
        }
        case 2: {
            cout << "\n--- CONTRATOS EN MEMORIA ---" << endl;
            if (g_contratos.empty()) {
                cout << "No hay contratos." << endl;
            } else {
                for (size_t i = 0; i < g_contratos.size(); i++) {
                    cout << "\n" << i+1 << ". " << g_contratos[i]->getnombre() << endl;
                    cout << "   Equipo: " << g_contratos[i]->getEquipoNombre() << endl;
                    cout << "   Inicio: " << g_contratos[i]->getFechaInicioStr() << endl;
                    cout << "   Fin: " << g_contratos[i]->getFechaFinStr() << endl;
                    cout << "   Salario: $" << g_contratos[i]->getClausula() << endl;
                }
            }
            break;
        }
        case 3: {
            cout << "\nGuardando contratos en archivo..." << endl;
            try {
                Contrato::guardarEnArchivo(g_contratos, "contratos.txt");
                cout << g_contratos.size() << " contratos guardados" << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
            break;
        }
        case 4: {
            cout << "\nCargando contratos desde archivo..." << endl;
            try {
                auto cargados = Contrato::cargarDesdeArchivo("contratos.txt");

                // Merge sin duplicados: usamos la serialización como clave simple
                size_t agregados = 0;
                for (auto &ptr : cargados) {
                    if (!ptr) continue;

                    bool existe = false;
                    for (const auto &existente : g_contratos) {
                        if (existente->serializar() == ptr->serializar()) {
                            existe = true;
                            break;
                        }
                    }

                    if (!existe) {
                        g_contratos.push_back(std::move(ptr));
                        agregados++;
                    }
                }

                cout << "\n--- CONTRATOS EN MEMORIA (DESPUES DE CARGAR) ---" << endl;
                if (g_contratos.empty()) {
                    cout << "No hay contratos." << endl;
                } else {
                    for (size_t i = 0; i < g_contratos.size(); i++) {
                        cout << "\n" << i+1 << ". " << g_contratos[i]->getnombre() << endl;
                        cout << "   Equipo: " << g_contratos[i]->getEquipoNombre() << endl;
                        cout << "   Inicio: " << g_contratos[i]->getFechaInicioStr() << endl;
                        cout << "   Fin: " << g_contratos[i]->getFechaFinStr() << endl;
                        cout << "   Salario: $" << g_contratos[i]->getClausula() << endl;
                    }
                }

                cout << "\n" << agregados << " contratos nuevos agregados desde el archivo" << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
            break;
        }
        case 5:
            cout << "Volviendo al menu principal..." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 5);
}

void menuPartidos() {
    int opcion;
    
    do {
        cout << "\n--- MENU PARTIDOS ---" << endl;
        cout << "1. Crear Partido" << endl;
        cout << "2. Convocar Partido" << endl;
        cout << "3. Registrar Resultado" << endl;
        cout << "4. Ver Partidos" << endl;
        cout << "5. Guardar Partidos en archivo" << endl;
        cout << "6. Cargar Partidos desde archivo" << endl;
        cout << "7. Volver al menu principal" << endl;
        cout << "Selecciona una opcion: ";
        
        if (!(cin >> opcion)) {
            cout << "Opcion no valida (debe ser un numero).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (opcion) {
        case 1: {
            cout << "\n=== CREAR NUEVO PARTIDO ===" << endl;
            string local, visitante;
            cout << "Equipo local: ";
            getline(cin, local);
            cout << "Equipo visitante: ";
            getline(cin, visitante);
            
            auto nuevoPartido = make_unique<Partido>(local, visitante);
            cout << "Partido creado." << endl;
            g_partidos.push_back(std::move(nuevoPartido));
            break;
        }
        case 2: {
            cout << "\n=== CONVOCAR PARTIDO ===" << endl;
            if (g_partidos.empty()) {
                cout << "No hay partidos creados." << endl;
                break;
            }
            cout << "Partidos disponibles:" << endl;
            for (size_t i = 0; i < g_partidos.size(); i++) {
                cout << i+1 << ". " << g_partidos[i]->getEquipoLocal() << " vs " << g_partidos[i]->getEquipoVisitante() << endl;
            }
            cout << "Selecciona partido (numero): ";
            int idx;
            cin >> idx;
            cin.ignore();
            if (idx > 0 && idx <= (int)g_partidos.size()) {
                cout << "Fecha y hora (DD/MM/YYYY HH:MM): ";
                string fecha;
                getline(cin, fecha);
                try {
                    g_partidos[idx-1]->convocarPartido(fecha);
                    cout << "Partido convocado para: " << fecha << endl;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            } else {
                cout << "Seleccion invalida." << endl;
            }
            break;
        }
        case 3: {
            cout << "\n=== REGISTRAR RESULTADO ===" << endl;
            if (g_partidos.empty()) {
                cout << "No hay partidos creados." << endl;
                break;
            }
            cout << "Partidos disponibles:" << endl;
            for (size_t i = 0; i < g_partidos.size(); i++) {
                cout << i+1 << ". " << g_partidos[i]->getEquipoLocal() << " vs " << g_partidos[i]->getEquipoVisitante() << endl;
            }
            cout << "Selecciona partido (numero): ";
            int idx;
            cin >> idx;
            cin.ignore();
            if (idx > 0 && idx <= (int)g_partidos.size()) {
                int golesLocal, golesVisitante;
                cout << "Goles del equipo local: ";
                cin >> golesLocal;
                cout << "Goles del equipo visitante: ";
                cin >> golesVisitante;
                cin.ignore();
                try {
                    g_partidos[idx-1]->registrarResultado(golesLocal, golesVisitante);
                    cout << "Resultado registrado: " << golesLocal << " - " << golesVisitante << endl;
                } catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            } else {
                cout << "Seleccion invalida." << endl;
            }
            break;
        }
        case 4: {
            cout << "\n--- PARTIDOS EN MEMORIA ---" << endl;
            if (g_partidos.empty()) {
                cout << "No hay partidos." << endl;
            } else {
                for (size_t i = 0; i < g_partidos.size(); i++) {
                    cout << "\n" << i+1 << ". " << g_partidos[i]->getEquipoLocal() << " vs " << g_partidos[i]->getEquipoVisitante() << endl;
                    
                    // Mostrar fecha
                    time_t fecha = g_partidos[i]->getFechaPartido();
                    string fechaStr = (fecha == 0) ? "Por definir" : time_t_a_string(fecha);
                    cout << "   Fecha: " << fechaStr << endl;
                    
                    // Mostrar estado y resultado
                    cout << "   Estado: " << estadoToString(g_partidos[i]->getEstado()) << endl;
                    if (g_partidos[i]->getEstado() == EstadoPartido::JUGADO) {
                        cout << "   Resultado: " << g_partidos[i]->getGolesLocal() << " - " 
                             << g_partidos[i]->getGolesVisitante() << endl;
                    }
                }
            }
            break;
        }
        case 5: {
            cout << "\nGuardando partidos en archivo..." << endl;
            ofstream ofs("partidos.txt", ios::trunc);
            if (!ofs) {
                cerr << "Error: No se pudo abrir partidos.txt" << endl;
            } else {
                for (const auto& p : g_partidos) {
                    ofs << p->serializar() << endl;
                }
                ofs.close();
                cout << g_partidos.size() << " partidos guardados" << endl;
            }
            break;
        }
        case 6: {
            cout << "\nCargando partidos desde archivo..." << endl;
            ifstream ifs("partidos.txt");
            if (!ifs) {
                cerr << "No se pudo abrir partidos.txt" << endl;
            } else {
                string linea;
                int count = 0;
                // Calcular el mayor ID ya existente en memoria para mantener nextId
                int highestId = 0;
                for (const auto &exist : g_partidos) if (exist->getId() > highestId) highestId = exist->getId();

                while (getline(ifs, linea)) {
                    if (linea.empty()) continue;
                    // Trim CR
                    if (!linea.empty() && linea.back() == '\r') linea.pop_back();
                    try {
                        auto p = Partido::deserializar(linea);
                        if (p) {
                            // Evitar duplicados por ID
                            bool existe = false;
                            for (const auto &exist : g_partidos) {
                                if (exist->getId() == p->getId()) { existe = true; break; }
                            }
                            if (!existe) {
                                  cout << ++count << ". " << p->getEquipoLocal() << " vs " << p->getEquipoVisitante() << endl;
                                // Actualizar highestId
                                if (p->getId() > highestId) highestId = p->getId();
                                g_partidos.push_back(std::move(p));
                            }
                        }
                    } catch (const exception& e) {
                        cerr << "Error deserializando: " << e.what() << endl;
                    }
                }
                // Establecer nextId a highestId+1 para evitar colisiones
                Partido::setNextId(highestId + 1);
                ifs.close();
                cout << "\n" << count << " partidos cargados (se actualizaron IDs)" << endl;
            }
            break;
        }
        case 7:
            cout << "Volviendo al menu principal..." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 7);
}

