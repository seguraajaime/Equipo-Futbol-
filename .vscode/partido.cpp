#include "partido.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
using namespace std;

int Partido::nextId = 1;


string estadoToString(EstadoPartido estado) {
    return estado == EstadoPartido::PROGRAMADO ? "PROGRAMADO" : "JUGADO";
}

EstadoPartido stringToEstado(const string& estadoStr) {
    if (estadoStr == "PROGRAMADO") return EstadoPartido::PROGRAMADO;
    if (estadoStr == "JUGADO") return EstadoPartido::JUGADO;
    throw invalid_argument("Estado invalido: " + estadoStr);
}

// Constructor nuevo
Partido::Partido(const string& local, const string& visitante)
    : idPartido(nextId++), equipoLocal(local), equipoVisitante(visitante),
      fechaPartido(0), golesLocal(0), golesVisitante(0), estado(EstadoPartido::PROGRAMADO) {}

// Constructor desde archivo
Partido::Partido(int id, const string& local, const string& visitante,
                 time_t fecha, int gl, int gv, const string& est)
    : idPartido(id), equipoLocal(local), equipoVisitante(visitante),
      fechaPartido(fecha), golesLocal(gl), golesVisitante(gv), estado(stringToEstado(est)) {}

int Partido::getId() const { return idPartido; }
time_t Partido::getFechaPartido() const { return fechaPartido; }
EstadoPartido Partido::getEstado() const { return estado; }

void Partido::convocarPartido(time_t fecha) {
    fechaPartido = fecha;
    estado = EstadoPartido::PROGRAMADO;
    cout << "Partido convocado para: " << put_time(localtime(&fecha), "%c") << endl;
}

void Partido::convocarPartido(const string& fechaStr) {
    tm tm = {};
    istringstream ss(fechaStr);
    //Formato: DD-MM-YYYY HH:MM:SS (día-mes-año)
    ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
    if (ss.fail()) {
        throw invalid_argument("Formato de fecha invalido. Usa 'DD-MM-YYYY HH:MM:SS'.");
    }

    time_t fecha = mktime(&tm);
    
    // Validar que la fecha no sea anterior a hoy
    time_t ahora = time(nullptr);
    struct tm* tm_ahora = localtime(&ahora);
    tm_ahora->tm_hour = 0;
    tm_ahora->tm_min = 0;
    tm_ahora->tm_sec = 0;
    time_t hoy_inicio = mktime(tm_ahora);
    
    if (fecha < hoy_inicio) {
        throw invalid_argument("La fecha del partido no puede ser anterior a hoy.");
    }
    
    convocarPartido(fecha);
}

void Partido::registrarResultado(int local, int visitante) {
    if (local < 0 || visitante < 0) {
        throw invalid_argument("Los goles no pueden ser negativos.");
    }
    golesLocal = local;
    golesVisitante = visitante;
    estado = EstadoPartido::JUGADO;
    cout << "Resultado registrado: " << equipoLocal << " " << golesLocal
              << " - " << golesVisitante << " " << equipoVisitante << endl;
}

void Partido::registrarResultado(const string& marcador) {
    size_t pos = marcador.find('-');
    if (pos == string::npos) {
        throw invalid_argument("Formato de marcador invalido. Usa 'X-Y'.");
    }

    int local = stoi(marcador.substr(0, pos));
    int visitante = stoi(marcador.substr(pos + 1));

    registrarResultado(local, visitante); 
}

string Partido::obtenerInfo() const {
    ostringstream oss;
    oss << "--- Partido ID: " << idPartido << " ---\n"
        << "Estado: " << estadoToString(estado) << "\n"
        << equipoLocal << " vs "  << equipoVisitante << "\n";

    if (fechaPartido != 0) {
        oss << "Fecha: " << put_time(localtime(&fechaPartido), "%d-%m-%Y %H:%M:%S") << "\n";
    } else {
        oss << "Fecha: Por definir\n";
    }

    if (estado == EstadoPartido::JUGADO) {
        oss << "Resultado: " << golesLocal << " - " << golesVisitante << "\n";
    }

    return oss.str();
}

string Partido::serializar() const {
    ostringstream oss;
    oss << idPartido << ";" << equipoLocal << ";" << equipoVisitante << ";";
    
    if (fechaPartido != 0) {
        oss << fechaPartido << ";" 
            << put_time(localtime(&fechaPartido), "%d-%m-%Y %H:%M:%S") << ";";
    } else {
        oss << "0;Sin definir;";
    }
    
    oss << golesLocal << ";" << golesVisitante << ";"
        << estadoToString(estado);
    return oss.str();
}

string Partido::serializarTabla() const {
    ostringstream oss;
    oss << setfill(' ')
        << setw(4) << idPartido << "| "
        << setw(20) << (equipoLocal.length() > 20 ? equipoLocal.substr(0, 17) + "..." : equipoLocal) << "| "
        << setw(20) << (equipoVisitante.length() > 20 ? equipoVisitante.substr(0, 17) + "..." : equipoVisitante) << "| ";
    
    if (fechaPartido != 0) {
        oss << setw(19) << put_time(localtime(&fechaPartido), "%d-%m-%Y %H:%M");
    } else {
        oss << setw(19) << "Sin definir";
    }
    
    oss << "| "
        << setw(6) << golesLocal << "| "
        << setw(6) << golesVisitante << "| "
        << setw(10) << estadoToString(estado);
    
    return oss.str();
}

unique_ptr<Partido> Partido::deserializar(const string& linea) {
    istringstream iss(linea);
    string id, local, visitante, fecha_ts, fecha_str, gl, gv, est;

    getline(iss, id, ';');
    getline(iss, local, ';');
    getline(iss, visitante, ';');
    getline(iss, fecha_ts, ';');
    getline(iss, fecha_str, ';');  
    getline(iss, gl, ';');
    getline(iss, gv, ';');
    getline(iss, est, ';');

    return make_unique<Partido>(
        stoi(id), local, visitante, stoll(fecha_ts),
        stoi(gl), stoi(gv), est
    );
}

void Partido::setNextId(int id) {
    nextId = id;
}