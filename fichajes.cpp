#include "Fichaje.h"
#include <iostream>

// Constructor
Fichaje::Fichaje(const std::string& nombre, const std::string& pos, int edad, double valor, const std::string& fecha)
    : nombreJugador(nombre), posicion(pos), edad(edad), valorFichaje(valor), fechaFichaje(fecha) {}

// Getters
std::string Fichaje::getNombreJugador() const {
    return nombreJugador;
}

std::string Fichaje::getPosicion() const {
    return posicion;
}

int Fichaje::getEdad() const {
    return edad;
}

double Fichaje::getValorFichaje() const {
    return valorFichaje;
}

std::string Fichaje::getFechaFichaje() const {
    return fechaFichaje;
}

// Setters
void Fichaje::setNombreJugador(const std::string& nombre) {
    nombreJugador = nombre;
}

void Fichaje::setPosicion(const std::string& pos) {
    posicion = pos;
}

void Fichaje::setEdad(int e) {
    edad = e;
}

void Fichaje::setValorFichaje(double valor) {
    valorFichaje = valor;
}

void Fichaje::setFechaFichaje(const std::string& fecha) {
    fechaFichaje = fecha;
}

// Mostrar información
void Fichaje::mostrarInfo() const {
    std::cout << "Jugador: " << nombreJugador << "\n"
              << "Posición: " << posicion << "\n"
              << "Edad: " << edad << "\n"
              << "Valor de fichaje: " << valorFichaje << " millones €\n"
              << "Fecha de fichaje: " << fechaFichaje << "\n";
}