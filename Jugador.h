#pragma once
#include <string>
#include <ctime>
using namespace std;
//crear estruct fecha
struct fecha {
    int dia;
    int mes;
    int ano;

    fecha() : dia(0), mes(0), ano(0) {};
    fecha(int dia, int mes, int ano) : dia(dia), mes(mes), ano(ano) {}
};

inline fecha fechaHoy() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    fecha f;
    f.dia = now->tm_mday;
    f.mes = now->tm_mon + 1;
    f.ano = now->tm_year + 1900;
    return f;
}

class Jugador {
private:
    int dorsal; 
    string nombre; 
    string posicion; 
    bool disponible; 
    fecha fechaInicioContrato;
    fecha fechaFinContrato;

public:
    Jugador(int dorsal, const string & nombre, const string & posicion);
    int getDorsal() const;
    string getNombre() const;
    string getPosicion() const;
    bool estaDisponible() const;
    fecha getFechaInicioContrato() const;
    fecha getFechaFinContrato() const;
    
    void firmarContrato(); 
    void finalizarContrato(); 

    string serializar() const; 
    static Jugador* deserializar(const string & linea); 
};