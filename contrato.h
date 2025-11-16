#pragma once
#include <string>
#include <stdexcept>
#include <ctime>
#include <vector>
#include <memory>
using namespace std;

//  Clase para representar el contrato
class Contrato {
private:
    int dorsal;
    time_t fechaInicio;
    time_t fechaFin;
    double salario;

    string time_t_a_string(time_t tiempo) const;

public:
    Contrato(int dorsal, time_t inicio, time_t fin = 0, double salario = 0.0);
    
    int getDorsal() const;
    time_t getFechaInicio() const;
    time_t getFechaFin() const;
    double getSalario() const;
    void setFechaFin(time_t fecha);
    string serializar() const;
    static Contrato* deserializar(const string& linea);

    string getFechaInicioStr() const;
    string getFechaFinStr() const;
    
    // Métodos para guardar y cargar archivos
    static void guardarEnArchivo(
        const std::vector<std::unique_ptr<Contrato>>& contratos, 
        const std::string& archivo
    );
    
    static std::vector<std::unique_ptr<Contrato>> cargarDesdeArchivo(
        const std::string& archivo
    );
    
    // Añade un contrato al archivo en modo append
    static void appendToFile(const Contrato& contrato, const std::string& archivo);
    
    // Ordena el archivo de contratos por dorsal
    static void ordenarArchivoPorDorsal(const std::string& archivo);
};
