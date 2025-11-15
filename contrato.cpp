#include "Contrato.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>    
#include <string>
using namespace std;

//Main para que compile
int main() {
    
    std::cout << "¡El programa se esta ejecutando!" << std::endl;
}

//Constructor
Contrato::Contrato(const string& nombre, const string& en, time_t inicio, time_t fin, double c)
    : nombre(nombre), equipoNombre(en), fechaInicio(inicio), fechaFin(fin), clausula(c) {}

//Getters y Setters
string Contrato::getnombre() const { 
    return nombre; 
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
    oss << nombre << ";" << equipoNombre << ";" << fechaInicio << ";" << fechaFin << ";" << clausula;
    return oss.str();
}

//Deserializar
Contrato* Contrato::deserializar(const string& linea) {
    istringstream iss(linea);
    string nombre, en, fi, ff, cl;

//Estoy verificando que existan todos los campos
   if(
    !getline(iss, nombre, ';') ||
    !getline(iss, en, ';') ||
    !getline(iss, fi, ';') ||
    !getline(iss, ff, ';') ||
    !getline(iss, cl, ';'))
    {
    // Estoy informando de un error si faltan campos
        cerr << "Error de formato: Línea incompleta en Contrato::deserializar: " << linea << endl;
        return nullptr;
    }
    
    //Verificando la conversión
    try
    {
        time_t inicio = stoll(fi);
        time_t fin = stoll (ff);
        double clausula = stod(cl);
    //Si no hay errores, crea y devuelve el nuevo objeto
        return new Contrato(nombre, en, inicio, fin, clausula);
    }
    catch(const std::exception& e)
    {
        cerr << "Error de conversión de datos en Contrato::deserializar" << e.what() << "[Linea: " << linea << "]" << endl;
        return nullptr; //Indica que la deserialización ha fallado
    }
}
//Estoy convirtiendo time_t a string
string Contrato::time_t_a_string(time_t tiempo) const {
    tm* ltm = localtime(&tiempo);

    char buffer [11]; // "dd/mm/aaaa\0"
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", ltm); //Esto formatea el tiempo en el buffer

    return string(buffer);
}