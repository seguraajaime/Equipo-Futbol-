#include "Jugador.h"
#include <sstream>

Jugador::Jugador(int d, const std::string& n, const std::string& pos)
    : dorsal(d), nombre(n), posicion(pos), disponible(true), fechaInicioContrato(0), fechaFinContrato(0) {}

int Jugador::getDorsal() const { return dorsal; }
std::string Jugador::getNombre() const { return nombre; }
std::string Jugador::getPosicion() const { return posicion; }
bool Jugador::estaDisponible() const { return disponible; }
std::time_t Jugador::getFechaInicioContrato() const { return fechaInicioContrato; }
std::time_t Jugador::getFechaFinContrato() const { return fechaFinContrato; }

void Jugador::firmarContrato() {
    disponible = false;
    fechaInicioContrato = std::time(nullptr);
    fechaFinContrato = 0; // Reiniciar por si es un nuevo contrato
}

void Jugador::finalizarContrato() {
    disponible = true;
    fechaFinContrato = std::time(nullptr);
}

std::string Jugador::serializar() const {
    std::ostringstream oss;
    oss << dorsal << ";" << nombre << ";" << posicion << ";" << disponible << ";" << fechaInicioContrato << ";" << fechaFinContrato;
    return oss.str();
}

Jugador* Jugador::deserializar(const std::string& linea) {
    std::istringstream iss(linea);
    std::string d, nom, pos, disp, fi, ff;
    
    getline(iss, d, ';');
    getline(iss, nom, ';');
    getline(iss, pos, ';');
    getline(iss, disp, ';');
    getline(iss, fi, ';');
    getline(iss, ff, ';');
    
    Jugador* j = new Jugador(std::stoi(d), nom, pos);
    j->disponible = (disp == "1");
    j->fechaInicioContrato = std::stoll(fi);
    j->fechaFinContrato = std::stoll(ff);
    return j;
}