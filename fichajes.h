#ifndef FICHAJE_H
#define FICHAJE_H

#include <string>

class Fichaje {
private:
    std::string nombreJugador;
    std::string posicion;
    int edad;
    double valorFichaje;
    std::string fechaFichaje;

public:
    // Constructor
    Fichaje(const std::string& nombre, const std::string& pos, int edad, double valor, const std::string& fecha);

    // Getters
    std::string getNombreJugador() const;
    std::string getPosicion() const;
    int getEdad() const;
    double getValorFichaje() const;
    std::string getFechaFichaje() const;

    // Setters
    void setNombreJugador(const std::string& nombre);
    void setPosicion(const std::string& pos);
    void setEdad(int edad);
    void setValorFichaje(double valor);
    void setFechaFichaje(const std::string& fecha);

    // Mostrar información del fichaje
    void mostrarInfo() const;
};

#endif