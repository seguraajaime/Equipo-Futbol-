#pragma once
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>    
#include <string>
using namespace std;

//  Clase para representar el contrato
class Contrato {
private:
    string nombre;
    string equipoNombre;
    time_t fechaInicio;
    time_t fechaFin;
    double clausula;

    string time_t_a_string(time_t tiempo) const;

public:
    Contrato(const string& nombre, const string& eNombre, time_t inicio, time_t fin = 0, double clausula = 0.0);
    
    string getnombre() const;
    string getEquipoNombre() const;
    time_t getFechaInicio() const;
    time_t getFechaFin() const;
    double getClausula() const;
    void setFechaFin(time_t fecha);
    string serializar() const;
    static Contrato* deserializar(const string& linea);

    string getFechaInicioStr() const;
    string getFechaFinStr() const;
};
