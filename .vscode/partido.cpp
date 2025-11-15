#include "partido.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// Inicialización del ID estático
int Partido::nextId = 1;

// Funciones auxiliares
std::string estadoToString(EstadoPartido estado) {
    return estado == EstadoPartido::PROGRAMADO ? "PROGRAMADO" : "JUGADO";
}

EstadoPartido stringToEstado(const std::string& estadoStr) {
    if (estadoStr == "PROGRAMADO") return EstadoPartido::PROGRAMADO;
    if (estadoStr == "JUGADO") return EstadoPartido::JUGADO;
    throw std::invalid_argument("Estado inválido: " + estadoStr);
}

// Constructor nuevo
Partido::Partido(const std::string& local, const std::string& visitante)
    : idPartido(nextId++), equipoLocal(local), equipoVisitante(visitante),
      fechaPartido(0), golesLocal(0), golesVisitante(0), estado(EstadoPartido::PROGRAMADO) {}

// Constructor desde archivo
Partido::Partido(int id, const std::string& local, const std::string& visitante,
                 std::time_t fecha, int gl, int gv, const std::string& est)
    : idPartido(id), equipoLocal(local), equipoVisitante(visitante),
      fechaPartido(fecha), golesLocal(gl), golesVisitante(gv), estado(stringToEstado(est)) {}

int Partido::getId() const { return idPartido; }
std::time_t Partido::getFechaPartido() const { return fechaPartido; }
EstadoPartido Partido::getEstado() const { return estado; }

void Partido::convocarPartido(std::time_t fecha) {
    fechaPartido = fecha;
    estado = EstadoPartido::PROGRAMADO;
    std::cout << "📅 Partido convocado para: " << std::put_time(std::localtime(&fecha), "%c") << std::endl;
}

void Partido::convocarPartido(const std::string& fechaStr) {
    std::tm tm = {};
    std::istringstream ss(fechaStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::invalid_argument("Formato de fecha inválido. Usa 'YYYY-MM-DD HH:MM:SS'.");
    }

    std::time_t fecha = std::mktime(&tm);
    convocarPartido(fecha); // Llama a la versión original
}

void Partido::registrarResultado(int local, int visitante) {
    if (local < 0 || visitante < 0) {
        throw std::invalid_argument("❌ Los goles no pueden ser negativos.");
    }
    golesLocal = local;
    golesVisitante = visitante;
    estado = EstadoPartido::JUGADO;
    std::cout << "✅ Resultado registrado: " << equipoLocal << " " << golesLocal
              << " - " << golesVisitante << " " << equipoVisitante << std::endl;
}

void Partido::registrarResultado(const std::string& marcador) {
    size_t pos = marcador.find('-');
    if (pos == std::string::npos) {
        throw std::invalid_argument("Formato de marcador inválido. Usa 'X-Y'.");
    }

    int local = std::stoi(marcador.substr(0, pos));
    int visitante = std::stoi(marcador.substr(pos + 1));

    registrarResultado(local, visitante); // Llama a la versión original
}

std::string Partido::obtenerInfo() const {
    std::ostringstream oss;
    oss << "--- Partido ID: " << idPartido << " ---\n"
        << "Estado: " << estadoToString(estado) << "\n"
        << "Local: " << equipoLocal << " vs Visitante: " << equipoVisitante << "\n";

    if (fechaPartido != 0) {
        oss << "Fecha: " << std::put_time(std::localtime(&fechaPartido), "%c") << "\n";
    } else {
        oss << "Fecha: Por definir\n";
    }

    if (estado == EstadoPartido::JUGADO) {
        oss << "Resultado: " << golesLocal << " - " << golesVisitante << "\n";
    }

    return oss.str();
}

std::string Partido::serializar() const {
    std::ostringstream oss;
    oss << idPartido << ";" << equipoLocal << ";" << equipoVisitante << ";"
        << fechaPartido << ";" << golesLocal << ";" << golesVisitante << ";"
        << estadoToString(estado);
    return oss.str();
}

std::unique_ptr<Partido> Partido::deserializar(const std::string& linea) {
    std::istringstream iss(linea);
    std::string id, local, visitante, fecha, gl, gv, est;

    std::getline(iss, id, ';');
    std::getline(iss, local, ';');
    std::getline(iss, visitante, ';');
    std::getline(iss, fecha, ';');
    std::getline(iss, gl, ';');
    std::getline(iss, gv, ';');
    std::getline(iss, est, ';');

    return std::make_unique<Partido>(
        std::stoi(id), local, visitante, std::stoll(fecha),
        std::stoi(gl), std::stoi(gv), est
    );
}

void Partido::setNextId(int id) {
    nextId = id;
}