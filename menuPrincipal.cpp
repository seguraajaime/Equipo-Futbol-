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
void crearContrato();
void verContratos();
void guardarContratosEnArchivo();
void verContratosMemoria();
void cargarPartidosDesdeArchivo();
void guardarPartidosEnArchivo();
void crearPartido();
void verPartidos();
void registrarResultadoPartido();
void convocarPartido();
// ------------------------------------

// === FUNCIÓN PRINCIPAL ===
int main() { 
    // Cargar los datos 
    cargarPlantillaInicial();
    cargarPartidosDesdeArchivo();

    try {
        menu();
    } catch(const exception& e) {
        cerr << "Error no esperado en el menu principal: " << e.what() << '\n';
    }

    // Guardar los datos al salir (solo si se sale por opción 4)
    // Nota: El guardar se hará al salir del menú.
    
    return 0;
}


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
    
    try {
        cout << "Directorio actual: " << filesystem::current_path().string() << '\n';
    } catch (...) {
       
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

        // --- COMPROBAR DORSAL REPETIDO ---
        for (const auto& jugPtr : g_plantilla) {
            if (jugPtr && jugPtr->getDorsal() == dorsal) {
                throw runtime_error(
                    "El dorsal " + to_string(dorsal) +
                    " ya esta ocupado por '" + jugPtr->getNombre() + "'."
                );
            }
        }

        cout << "Nombre: ";
        getline(cin, nombre);
        cout << "Posicion: ";
        getline(cin, posicion);

        auto nuevoJugador = make_unique<Jugador>(dorsal, nombre, posicion);
        nuevoJugador->firmarContrato();
        
        g_plantilla.push_back(std::move(nuevoJugador)); 
        
        guardarPlantillaEnArchivo(); 

        cout << "Jugador '" << nombre << "' fichado y guardado.\n";
        
    } catch(const exception& e) {
        cerr << "Error al fichar jugador: " << e.what() << '\n';
    }
}


void mostrarPlantilla() {
    // Muestra la plantilla desde la RAM (g_plantilla)
    // La disponibilidad se determina por si tiene contrato en g_contratos
    cout << "\n=== Lista de jugadores (Desde RAM) ===\n";

    if (g_plantilla.empty()) {
        cout << "La plantilla esta vacia.\n";
        return;
    }

    for (const auto& j : g_plantilla) {
        // Verificar si el jugador tiene contrato
        bool tieneContrato = false;
        for (const auto& c : g_contratos) {
            if (c->getDorsal() == j->getDorsal()) {
                tieneContrato = true;
                break;
            }
        }
        
        cout << "-------------------------\n";
        cout << "Dorsal:     " << j->getDorsal() << '\n';
        cout << "Nombre:     " << j->getNombre() << '\n';
        cout << "Posicion:   " << j->getPosicion() << '\n';
        cout << "Disponible: " << (tieneContrato ? "Si" : "No") << '\n';
    }
}

void rescindirContratoJugador() {
    // Esta función rescinde el contrato de un jugador: elimina del archivo y de contratos asociados
    
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
            cout << "Rescindiendo contrato de: " << (*it)->getNombre() << " (dorsal " << dorsalBuscado << ")\n";
            
            // Eliminar también el contrato asociado de g_contratos
            auto itContratos = g_contratos.begin();
            while (itContratos != g_contratos.end()) {
                if ((*itContratos)->getDorsal() == dorsalBuscado) {
                    itContratos = g_contratos.erase(itContratos);
                } else {
                    ++itContratos;
                }
            }
            
            it = g_plantilla.erase(it); // Borra y avanza el iterador
            guardarPlantillaEnArchivo(); // Guarda el cambio al disco
            cout << "Jugador y su contrato eliminados correctamente.\n";
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
        cout << "2. Ver Contratos creados" << endl;
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
            crearContrato();
            break;
        }
        case 2: {
            verContratos();
            break;
        }
        case 3: {
            guardarContratosEnArchivo();
            break;
        }
        case 4: {
            verContratosMemoria();
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
void crearContrato() {
    cout << "\n=== CREAR NUEVO CONTRATO ===" << endl;

    try {
        int dorsal;
        double salario;

        cout << "Dorsal del jugador: ";
        cin >> dorsal;
        cin.ignore();

        // Buscar el jugador en g_plantilla
        Jugador* jugadorEncontrado = nullptr;
        for (auto& j : g_plantilla) {
            if (j->getDorsal() == dorsal) {
                jugadorEncontrado = j.get();
                break;
            }
        }

        if (!jugadorEncontrado) {
            throw runtime_error(
                "No existe jugador con dorsal " + to_string(dorsal) + " en la plantilla."
            );
        }

        cout << "Salario: ";
        cin >> salario;
        cin.ignore();

        time_t fechaInicio = leerFechaFormato("Fecha de inicio");
        time_t fechaFin    = leerFechaFormato("Fecha de fin");
        time_t ahora = time(nullptr); 

        // 1) La fecha de inicio no puede ser anterior a hoy
        if (fechaInicio < ahora) {
            throw invalid_argument(
                "La fecha de inicio del contrato no puede ser anterior a hoy."
            );
        }

        // 2) La fecha de fin debe ser posterior a la de inicio
        if (fechaFin <= fechaInicio) {
            throw invalid_argument(
                "La fecha de fin del contrato debe ser posterior a la fecha de inicio."
            );
        }

        // Si las fechas son válidas, creamos el contrato
        auto nuevoContrato = make_unique<Contrato>(dorsal, fechaInicio, fechaFin, salario);
        Contrato* contPtr = nuevoContrato.get();
        g_contratos.push_back(move(nuevoContrato));

        cout << "\nCONTRATO CREADO:" << endl;
        cout << "  Jugador: " << jugadorEncontrado->getNombre()
             << " (dorsal " << dorsal << ")" << endl;
        cout << "  Inicio: " << contPtr->getFechaInicioStr() << endl;
        cout << "  Fin: " << contPtr->getFechaFinStr() << endl;
        cout << "  Salario: $" << contPtr->getSalario() << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "Error en las fechas del contrato: " << e.what() << '\n';
    }
    catch (const runtime_error& e) {
        cerr << "Error al crear contrato: " << e.what() << '\n';
    }
    catch (const exception& e) {
        cerr << "Error inesperado al crear contrato: " << e.what() << '\n';
    }
}

void verContratos() {


    cout << "\n--- CONTRATOS EN MEMORIA ---" << endl;
            if (g_contratos.empty()) {
                cout << "No hay contratos." << endl;
            } else {
                for (size_t i = 0; i < g_contratos.size(); i++) {
                    // Buscar nombre del jugador por dorsal
                    string nombreJugador = "(Desconocido)";
                    for (const auto& j : g_plantilla) {
                        if (j->getDorsal() == g_contratos[i]->getDorsal()) {
                            nombreJugador = j->getNombre();
                            break;
                        }
                    }
                    
                    cout << "\n" << i+1 << ". Dorsal " << g_contratos[i]->getDorsal() << " - " << nombreJugador << endl;
                    cout << "   Inicio: " << g_contratos[i]->getFechaInicioStr() << endl;
                    cout << "   Fin: " << g_contratos[i]->getFechaFinStr() << endl;
                    cout << "   Salario: $" << g_contratos[i]->getSalario() << endl;
                }
            }
        }

void verContratosMemoria() {
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
                        // Buscar nombre del jugador por dorsal
                        string nombreJugador = "(Desconocido)";
                        for (const auto& j : g_plantilla) {
                            if (j->getDorsal() == g_contratos[i]->getDorsal()) {
                                nombreJugador = j->getNombre();
                                break;
                            }
                        }
                        
                        cout << "\n" << i+1 << ". Dorsal " << g_contratos[i]->getDorsal() << " - " << nombreJugador << endl;
                        cout << "   Inicio: " << g_contratos[i]->getFechaInicioStr() << endl;
                        cout << "   Fin: " << g_contratos[i]->getFechaFinStr() << endl;
                        cout << "   Salario: $" << g_contratos[i]->getSalario() << endl;
                    }
                }

                cout << "\n" << agregados << " contratos nuevos agregados desde el archivo" << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
}

void guardarContratosEnArchivo() {
   cout << "\nGuardando contratos en archivo..." << endl;
            try {
                Contrato::guardarEnArchivo(g_contratos, "contratos.txt");
                cout << g_contratos.size() << " contratos guardados" << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
// === MENU PARTIDOS ===

void menuPartidos() {
    int opcion;
    
    do {
        cout << "\n--- MENU PARTIDOS ---" << endl;
        cout << "1. Crear partido" << endl;
        cout << "2. Convocar partido" << endl;
        cout << "3. Registrar resultado" << endl;
        cout << "4. Ver todos los partidos" << endl;
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
        case 1:
            crearPartido();
            break;
        case 2:
            convocarPartido();
            break;
        case 3:
            registrarResultadoPartido();
            break;
        case 4:
            verPartidos();        
            break;
        case 5:
            cout << "Volviendo al menu principal..." << endl;
            break;
        default:
            cout << "Opcion no valida." << endl;
        }
    } while (opcion != 5);
}

// === CREAR PARTIDO ===

void crearPartido() {
    cout << "\n=== CREAR NUEVO PARTIDO ===" << endl;
    string rival, resp;
    const string ourTeam = "Canadio";
            
    cout << "Nombre del equipo rival: ";
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
            
    auto nuevoPartido = make_unique<Partido>(local, visitante);
    cout << "Partido creado." << endl;
    g_partidos.push_back(std::move(nuevoPartido));

    // Guardar inmediatamente el partido nuevo en el archivo SIN borrar lo anterior
    guardarPartidosEnArchivo();   // guarda todo en modo append
}

// === CONVOCAR PARTIDO ===

void convocarPartido() {
    cout << "\n=== CONVOCAR PARTIDO ===" << endl;
    if (g_partidos.empty()) {
        cout << "No hay partidos creados." << endl;
        return;
    }

    cout << "Partidos disponibles:" << endl;
    for (size_t i = 0; i < g_partidos.size(); i++) {
        cout << i+1 << ". " 
             << g_partidos[i]->getEquipoLocal() 
             << " vs " 
             << g_partidos[i]->getEquipoVisitante() << endl;
    }

    cout << "Selecciona partido (numero): ";
    int idx;
    if (!(cin >> idx)) {
        cout << "Entrada invalida.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore();

    if (idx > 0 && idx <= (int)g_partidos.size()) {
        cout << "Fecha y hora (DD/MM/YYYY HH:MM): ";
        string fecha;
        getline(cin, fecha);
        try {
            cout << "Partido convocado para: " << fecha << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else {
        cout << "Seleccion invalida." << endl;
    }
    guardarPartidosEnArchivo();
}

// === REGISTRAR RESULTADO PARTIDO ===

void registrarResultadoPartido() {
    cout << "\n=== REGISTRAR RESULTADO PARTIDO ===" << endl;
    if (g_partidos.empty()) {
        cout << "No hay partidos creados." << endl;
        return;
    }

    cout << "Partidos disponibles:" << endl;
    for (size_t i = 0; i < g_partidos.size(); i++) {
        cout << i+1 << ". " 
             << g_partidos[i]->getEquipoLocal() 
             << " vs " 
             << g_partidos[i]->getEquipoVisitante() << endl;
    }

    cout << "Selecciona partido (numero): ";
    int idx;
    if (!(cin >> idx)) {
        cout << "Entrada invalida.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore();

    if (idx > 0 && idx <= (int)g_partidos.size()) {
        int golesLocal, golesVisitante;
        cout << "Goles " << g_partidos[idx-1]->getEquipoLocal() << ": ";
        cin >> golesLocal;
        cout << "Goles " << g_partidos[idx-1]->getEquipoVisitante() << ": ";
        cin >> golesVisitante;
        cin.ignore();
        try {
            g_partidos[idx-1]->registrarResultado(golesLocal, golesVisitante);
            cout << "Resultado registrado: " 
                 << golesLocal << " - " << golesVisitante << endl;
            // si quisieras sobrescribir el archivo con el nuevo estado:
            // guardarPartidosEnArchivo();
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } else {
        cout << "Seleccion invalida." << endl;
    }
    guardarPartidosEnArchivo();
}

// === VER PARTIDOS (CARGANDO DESDE ARCHIVO) ===

void verPartidos() {
    cout << "\nCargando partidos desde archivo..." << endl;

    cout << "\n--- PARTIDOS EN MEMORIA (DESDE ARCHIVO) ---" << endl;
    if (g_partidos.empty()) {
        cout << "No hay partidos." << endl;
    } else {
        for (size_t i = 0; i < g_partidos.size(); i++) {
            cout << "\n" << i+1 << ". " 
                 << g_partidos[i]->getEquipoLocal() 
                 << " vs " 
                 << g_partidos[i]->getEquipoVisitante() << endl;
                    
            // Mostrar fecha
            time_t fecha = g_partidos[i]->getFechaPartido();
            string fechaStr = (fecha == 0) ? "Por definir" : time_t_a_string(fecha);    
            cout << "   Fecha: " << fechaStr << endl;
                    
            // Mostrar estado y resultado
            cout << "   Estado: " << estadoToString(g_partidos[i]->getEstado()) << endl;
            if (g_partidos[i]->getEstado() == EstadoPartido::JUGADO) {
                cout << "   Resultado: " 
                     << g_partidos[i]->getGolesLocal() << " - " 
                     << g_partidos[i]->getGolesVisitante() << endl;
            }
        }
    }
}

// === GUARDAR PARTIDOS EN ARCHIVO (NO BORRA) ===
// La mantengo aunque con otro comportamiento: ahora AÑADE en vez de truncar.

void guardarPartidosEnArchivo() {
    cout << "\nGuardando partidos en archivo (append)..." << endl;
    ofstream ofs("partidos.txt", ios::app);   // 👈 APPEND: NO BORRA EL ARCHIVO
    if (!ofs) {
        cerr << "Error: No se pudo abrir partidos.txt" << endl;
    } else {
        for (const auto& p : g_partidos) {
            ofs << p->serializar() << endl;
        }
        ofs.close();
        cout << g_partidos.size() << " partidos escritos al final de partidos.txt" << endl;
    }
}

// === CARGAR PARTIDOS DESDE ARCHIVO (SIN BORRAR NADA) ===

void cargarPartidosDesdeArchivo() {
    ifstream ifs("partidos.txt");
    if (!ifs) {
        cerr << "No se pudo abrir partidos.txt" << endl;
        return;
    }

    string linea;
    int count = 0;

    // Calcular el mayor ID ya existente en memoria para mantener nextId
    int highestId = 0;
    for (const auto &exist : g_partidos)
        if (exist->getId() > highestId) highestId = exist->getId();

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
                    if (exist->getId() == p->getId()) { 
                        existe = true; 
                        break; 
                    }
                }
                if (!existe) {
                    // Actualizar highestId
                    if (p->getId() > highestId) highestId = p->getId();
                    g_partidos.push_back(std::move(p));
                    ++count;
                }
            }
        } catch (const exception& e) {
            cerr << "Error deserializando: " << e.what() << endl;
        }
    }

    // Establecer nextId a highestId+1 para evitar colisiones
    Partido::setNextId(highestId + 1);
    ifs.close();
    cout << count << " partidos cargados desde archivo." << endl;
}
