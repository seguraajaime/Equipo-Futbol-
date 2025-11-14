#pragma once
#include <string>
#include <ctime>
using namespace std;

//  Clase para representar jugadores
struct fecha{
    int dia;
    int mes;
    int ano;
};

fecha constructorFecha(int dia, int mes, int ano);

class Jugador {
private:
    int dorsal; 
    string nombre; 
    string posicion; 
    bool disponible; 
    fecha fechaInicioContrato; 
    fecha fechaFinContrato;

public:
    Jugador(int dorsal, const string& nombre, const string& pos);
    int getDorsal() const;
    string getNombre() const;
    string getPosicion() const;
    bool estaDisponible() const;
    fecha getFechaInicioContrato() const;
    fecha getFechaFinContrato() const;
    
    void firmarContrato(); 
    void finalizarContrato(); 

    string serializar() const; 
    static Jugador* deserializar(const std::string& linea); 
};