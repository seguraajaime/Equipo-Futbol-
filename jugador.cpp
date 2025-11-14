#include "Jugador.h"
#include <sstream>
#include <iostream>
#include <string>
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
    disponible = true;
    fechaInicioContrato = fechaHoy();
    fechaFinContrato = fecha(); // Reiniciar por si es un nuevo contrato
}

void Jugador::finalizarContrato() {
    disponible = true;
    fechaFinContrato = fechaHoy();
}

string Jugador::serializar() const {
    ostringstream oss;
    oss << dorsal << ";"
        << nombre << ";"
        << posicion << ";"
        << (disponible ? 1 : 0) << ";"
        << fechaInicioContrato.dia  << ";"
        << fechaInicioContrato.mes  << ";"
        << fechaInicioContrato.ano;
    return oss.str();
}


Jugador* Jugador::deserializar(const string & linea) {
    istringstream iss(linea);
    string sd, nom, pos, sdisp;
    string sdi, smi, sai; 

    getline(iss, sd,   ';');  // dorsal
    getline(iss, nom,  ';');  // nombre
    getline(iss, pos,  ';');  // posicion
    getline(iss, sdisp,';');  // disponible
    getline(iss, sdi,  ';');  // dia inicio
    getline(iss, smi,  ';');  // mes inicio
    getline(iss, sai,  ';');  // año inicio


    Jugador* j = new Jugador(std::stoi(sd), nom, pos);
    j->disponible          = (sdisp == "1");
    j->fechaInicioContrato = fecha(std::stoi(sdi), std::stoi(smi), std::stoi(sai));
    return j;
}