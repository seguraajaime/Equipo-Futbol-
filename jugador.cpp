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
    try
    {
    istringstream iss(linea);
    string sd, nom, pos, sdisp;
    string sdi, smi, sai; 

    if(!getline(iss, sd,   ';')) throw invalid_argument("Falta dorsal");  
    if(!getline(iss, nom,  ';')) throw invalid_argument("Falta nombre");  
    if(!getline(iss, pos,  ';')) throw invalid_argument("Falta posicion");  
    if(!getline(iss, sdisp,';')) throw invalid_argument("Falta disponible");  
    if(!getline(iss, sdi,  ';')) throw invalid_argument("Falta dia inicio");  
    if(!getline(iss, smi,  ';')) throw invalid_argument("Falta mes inicio");  
    if(!getline(iss, sai,  ';')) throw invalid_argument("Falta ano inicio");  


    Jugador* j = new Jugador(stoi(sd), nom, pos);
    j->disponible          = (sdisp == "1");
    j->fechaInicioContrato = fecha(stoi(sdi), stoi(smi), stoi(sai));
    return j;
    }
    catch(const std::exception& e)
    {
        throw runtime_error("Error al deserializar jugador: " + linea + " (" + e.what() + ")");
    }
    
    
}