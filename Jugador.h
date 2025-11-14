#pragma once
#include <string>
#include <ctime>

//  Clase para representar jugadores
class Jugador {
private:
    int dorsal; // [cite: 35] ID único, como la matrícula
    std::string nombre; // [cite: 35]
    std::string posicion; // [cite: 35]
    bool disponible; // [cite: 35] Estado (Libre / Contratado)
    std::time_t fechaInicioContrato; // [cite: 46]
    std::time_t fechaFinContrato;

public:
    Jugador(int dorsal, const std::string& nombre, const std::string& pos);
    int getDorsal() const;
    std::string getNombre() const;
    std::string getPosicion() const;
    bool estaDisponible() const;
    std::time_t getFechaInicioContrato() const;
    std::time_t getFechaFinContrato() const;
    
    void firmarContrato(); // [cite: 36] Análogo a alquilar()
    void finalizarContrato(); // [cite: 36] Análogo a devolver()

    std::string serializar() const; // [cite: 42]
    static Jugador* deserializar(const std::string& linea); // [cite: 42]
};