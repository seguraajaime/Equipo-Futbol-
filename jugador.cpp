#include "Jugador.h"
#include <sstream>
using namespace std;

Jugador::Jugador(int d, const string& n, const string& pos)
    : dorsal(d), nombre(n), posicion(pos), disponible(true), fechaInicioContrato(fecha()), fechaFinContrato(fecha()) {}

int Jugador::getDorsal() const { return dorsal; }
string Jugador::getNombre() const { return nombre; }
string Jugador::getPosicion() const { return posicion; }
bool Jugador::estaDisponible() const { return disponible; }
fecha Jugador::getFechaInicioContrato() const { return fechaInicioContrato; }
fecha Jugador::getFechaFinContrato() const { return fechaFinContrato; }

void Jugador::firmarContrato() {
    disponible = false;
    fechaInicioContrato = fechaHoy();
    fechaFinContrato = fecha(); // Reiniciar por si es un nuevo contrato
}

void Jugador::finalizarContrato() {
    disponible = true;
    fechaFinContrato = fechaHoy();
}

std::string Jugador::serializar() const {
    std::ostringstream oss;
    oss << dorsal << ";" << nombre << ";" << posicion << ";" << disponible << ";";
}

Jugador* Jugador::deserializar(const std::string& linea) {
    istringstream iss(linea);
string d, nom, pos, disp, fi, ff;
    
    getline(iss, d, ';');
    getline(iss, nom, ';');
    getline(iss, pos, ';');
    getline(iss, disp, ';');
    getline(iss, fi, ';');
    getline(iss, ff, ';');
    
    Jugador* j = new Jugador(stoi(d), nom, pos);
    j->disponible = (disp == "1");
    return j;
};