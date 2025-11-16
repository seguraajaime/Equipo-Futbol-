#include "Contrato.h"
#include <sstream>
#include <iostream>  
#include <string>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include <vector>
using namespace std;

//Constructor
Contrato::Contrato(int d, const string& en, time_t inicio, time_t fin, double c)
    : dorsal(d), equipoNombre(en), fechaInicio(inicio), fechaFin(fin), clausula(c) {}

//Getters y Setters
int Contrato::getDorsal() const { 
    return dorsal; 
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
    oss << dorsal << ";" << equipoNombre << ";" << fechaInicio << ";" << fechaFin << ";" << clausula;
    return oss.str();
}

//Deserializar
Contrato* Contrato::deserializar(const string& linea) {
    istringstream iss(linea);
    string sd, en, fi, ff, cl;

//Estoy verificando que existan todos los campos
   if(
    !getline(iss, sd, ';') ||
    !getline(iss, en, ';') ||
    !getline(iss, fi, ';') ||
    !getline(iss, ff, ';') ||
    !getline(iss, cl, ';'))
    {
    // Estoy informando de un error si faltan campos
        cerr << "Error de formato: Linea incompleta en Contrato::deserializar: " << linea << endl;
        return nullptr; //El puntero no apunta a nada
    }
    
    //Verificando la conversión
    try
    {
        int dorsal = stoi(sd);
        time_t inicio = stoll(fi);
        time_t fin = stoll (ff);
        double clausula = stod(cl);
    //Si no hay errores, crea y devuelve el nuevo objeto
        return new Contrato(dorsal, en, inicio, fin, clausula);
    }
    catch(const std::exception& e)
    {
        cerr << "Error de conversion de datos en Contrato::deserializar" << e.what() << "[Linea: " << linea << "]" << endl;
        return nullptr; //Indica que la deserializacion ha fallado, el puntero no apunta a nada
    }
}
//Estoy convirtiendo time_t a string
string Contrato::time_t_a_string(time_t tiempo) const {
    tm* ltm = localtime(&tiempo);

    char buffer [11]; // "dd/mm/aaaa\0"
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", ltm); //Esto formatea el tiempo en el buffer

    return string(buffer);
}
//Devuelve la fecha de inicio en formato legible al ejecutar el programa
string Contrato::getFechaInicioStr() const {
    return time_t_a_string(fechaInicio);
}
//Devuelve la fecha de fin en formato legible al ejecutar el program
string Contrato::getFechaFinStr() const {
    return time_t_a_string(fechaFin);
}

// Guarda todos los contratos en un archivo txt
void Contrato::guardarEnArchivo(
    const vector<unique_ptr<Contrato>>& contratos, 
    const string& archivo
) {
    // Abrir en modo trunc para sobrescribir el archivo con el estado actual en memoria
    ofstream fs(archivo, ios::trunc);
    if (!fs.is_open()) {
        throw runtime_error("Error: No se pudo abrir el archivo para guardar: " + archivo);
    }

    // Escribir cada contrato, reemplazando el contenido anterior
    for (const auto& contrato : contratos) {
        fs << contrato->serializar() << '\n';
    }

    fs.close();
    cout << "Contratos guardados en: " << archivo << endl;
}

// Carga todos los contratos desde un archivo txt
vector<unique_ptr<Contrato>> Contrato::cargarDesdeArchivo(const string& archivo) {
    
    // 1. Creamos el vector "seguro" que vamos a devolver
    vector<unique_ptr<Contrato>> vectorCargado;
    ifstream fs(archivo);

    if (!fs.is_open()) {
        throw runtime_error("Error: No se pudo abrir el archivo para cargar: " + archivo);
    }

    string linea;
    while (getline(fs, linea)) {
        if (linea.empty()) continue;

        // 2. 'deserializar' nos da un puntero "crudo" (peligroso)
        Contrato* ptr = Contrato::deserializar(linea);

        if (ptr != nullptr) {
            // 3. Lo "adoptamos" con un unique_ptr y lo metemos al vector
            // Esto transfiere la responsabilidad de 'delete' al vector.
            vectorCargado.push_back(unique_ptr<Contrato>(ptr));
        }
    }
    
    fs.close();
    return vectorCargado;
}

// Agrega un contrato al final del archivo (modo append)
void Contrato::appendToFile(const Contrato& contrato, const string& archivo) {
    ofstream ofs(archivo, ios::app);
    if (!ofs.is_open()) {
        throw runtime_error("Error: No se pudo abrir el archivo para append: " + archivo);
    }
    ofs << contrato.serializar() << endl;
    ofs.close();
}