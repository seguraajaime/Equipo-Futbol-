#include "Contrato.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>    
#include <string>
using namespace std;

//Constructor
Contrato::Contrato(int jd, const string& en, time_t inicio, time_t fin, double c)
    : jugadorDorsal(jd), equipoNombre(en), fechaInicio(inicio), fechaFin(fin), clausula(c) {}

//Getter y Setter
int Contrato::getJugadorDorsal() const { 
    return jugadorDorsal; 
}
string Contrato::getEquipoNombre() const { 
    return equipoNombre; 
}
time_t Contrato::getFechaInicio() const { 
    return fechaInicio; 
}
time_t Contrato::getFechaFin() const { 
    return fechaFin; 
}
double Contrato::getClausula() const { 
    return clausula; 
}
void Contrato::setFechaFin(time_t fecha) { 
    fechaFin = fecha; 
}

//Serializar
string Contrato::serializar() const {
    ostringstream oss;
    oss << jugadorDorsal << ";" << equipoNombre << ";" << fechaInicio << ";" << fechaFin << ";" << clausula;
    return oss.str();
}

//Deserializar
Contrato* Contrato::deserializar(const string& linea) {
    istringstream iss(linea);
    string jd, en, fi, ff, cl;
    
    getline(iss, jd, ';');
    getline(iss, en, ';');
    getline(iss, fi, ';');
    getline(iss, ff, ';');
    getline(iss, cl, ';');
    
    return new Contrato(stoi(jd), en, stoll(fi), stoll(ff), stod(cl));
}