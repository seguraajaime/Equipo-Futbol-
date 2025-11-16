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

vector<unique_ptr<Jugador>> g_plantilla; //jugador.txt
vector<unique_ptr<Contrato>> g_contratos; // contratos.txt
vector<unique_ptr<Partido>> g_partidos; // partidos.txt

//todas las funciones que se usan en el menu principal
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


int main() { 
    // añadir las memorias iniciales
    cargarPlantillaInicial();
    cargarPartidosDesdeArchivo();
    try {
        menu();
    } catch(const exception& e) {
        cerr << "Error no esperado en el menu principal: " << e.what() << '\n';
    }
}



void menu() {
    // imprimir por pantalla el menu 
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

void cargarPlantillaInicial() { // se carga jugadore.txt
    cout << "Cargando datos iniciales...\n";
    try {
        cout << "Directorio actual: " << filesystem::current_path().string() << '\n';
    } catch (...) {
        cout << "No se pudo abrir jugadores.txt.\n";
    }
    if (!filesystem::exists("jugadores.txt")) {
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

            // Comprobar si dorsal ya existe
            bool existe = false;
            for (const auto& existing : g_plantilla) {
                if (existing->getDorsal() == j->getDorsal()) {
                    existe = true;
                    break;
                }
            }

            if (!existe) {// Solo añadir si no existe
                g_plantilla.push_back(std::unique_ptr<Jugador>(j));
                ++cargados;
            } else {
                delete j; 
            }
        } catch (const exception& e) {
            cerr << "Error al deserializar jugador: " << e.what() << "\n";
        }
    }
    ifs.close();
    cout << cargados << " jugadores cargados en memoria desde jugadores.txt\n";
}

void guardarPlantillaEnArchivo() {
    // Guarda el contenido de g_plantilla en jugadores.txt
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

void menuJugadores() {
    int opcion = 0;
    do {
        // se imprime menu jugadores 
        cout << "\n--- MENU JUGADORES ---" << endl;
        cout << "1. Fichar jugador" << endl;
        cout << "2. Ver plantilla" << endl;
        cout << "3. Rescindir contrato" << endl;
        cout << "4. Volver al menu principal" << endl;
        cout << "Elige una opcion: ";
        if (!(cin >> opcion)) {
            cout << "Opcion no valida (debe ser un numero).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
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

        // mira si el dorsal ya existe
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
    cout << "\n=== Lista de jugadores (Desde RAM) ===\n";

    if (g_plantilla.empty()) {
        cout << "La plantilla esta vacia.\n";
        return;
    }

    for (const auto& j : g_plantilla) {
        // vemos si tiene contrato
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
    
    int dorsalBuscado = -1; // para contar el numero 0 tmb 
    cout << "\nRescindir contrato (dorsal): ";
    
    if (!(cin >> dorsalBuscado)) {
        cout << "Opcion no valida (debe ser un numero).\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // eliminamos al jugador de la plantilla
    auto it = g_plantilla.begin();
    while (it != g_plantilla.end()) {
        if ((*it)->getDorsal() == dorsalBuscado) {
            cout << "Rescindiendo contrato de: " << (*it)->getNombre() << " (dorsal " << dorsalBuscado << ")\n";
            
            // eliminamos el contrato del jugador
            auto itContratos = g_contratos.begin();
            while (itContratos != g_contratos.end()) {
                if ((*itContratos)->getDorsal() == dorsalBuscado) {
                    itContratos = g_contratos.erase(itContratos);
                } else {
                    ++itContratos;
                }
            }
            
            it = g_plantilla.erase(it); // borra al jugador
            guardarPlantillaEnArchivo(); 
            cout << "Jugador y su contrato eliminados correctamente.\n";
            return;
        }
        ++it;
    }

    cout << "No se ha encontrado ningun jugador con ese dorsal.\n";
}

time_t leerFechaFormato(const string& mensaje) { // para poder escribir la fecha en formato dd/mm/aaaa
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


string time_t_a_string(time_t tiempo) { // pasamos de time_t a string
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

    guardarPartidosEnArchivo();   
}

void convocarPartido() { // REVISAR
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
    // deberia de funcionar para que se guarde que el partido tiene fecha y hora (guardarPartidosEnArchivo();)
}

void registrarResultadoPartido() { // REVISAR
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
    // deberia de funcionar para que se guarde el resultado del partido (guardarPartidosEnArchivo();)
}

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

void guardarPartidosEnArchivo() { // Revisar 
    cout << "\nGuardando partidos en archivo (append)..." << endl;
    ofstream ofs("partidos.txt", ios::app);
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

void cargarPartidosDesdeArchivo() { // Revisar
    cout << "\nCargando partidos desde archivo..." << endl;
    ifstream ifs("partidos.txt");
    if (!ifs) {
        cerr << "No se pudo abrir partidos.txt" << endl;
        return;
    }

    string linea;
    int count = 0;

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
                bool existe = false;
                for (const auto &exist : g_partidos) {
                    if (exist->getId() == p->getId()) { 
                        existe = true; 
                        break; 
                    }
                }
                if (!existe) {
                    if (p->getId() > highestId) highestId = p->getId();
                    g_partidos.push_back(std::move(p));
                    ++count;
                }
            }
        } catch (const exception& e) {
            cerr << "Error deserializando: " << e.what() << endl;
        }
    }
    Partido::setNextId(highestId + 1);
    ifs.close();
    cout << count << " partidos cargados desde archivo." << endl;
}
