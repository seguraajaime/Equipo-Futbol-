#pragma once
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>    
#include <string>
using namespace std;

//  Clase para representar la transacción (el contrato)
class Contrato {
private:
    int jugadorDorsal; // [cite: 41] ID del Jugador
    string equipoNombre; // [cite: 41] ID del Equipo
    time_t fechaInicio; // [cite: 41]
    time_t fechaFin; // [cite: 41]
    double clausula; // [cite: 41] Atributo extra

public:
    Contrato(int jDorsal, const string& eNombre, time_t inicio, time_t fin = 0, double clausula = 0.0);
    
    int getJugadorDorsal() const;
    string getEquipoNombre() const;
    time_t getFechaInicio() const;
    time_t getFechaFin() const;
    double getClausula() const;

    void setFechaFin(time_t fecha);
    
    string serializar() const; // [cite: 42]
    static Contrato* deserializar(const string& linea); // [cite: 42]
};
