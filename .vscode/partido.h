#pragma once
#include <string>
#include <ctime>
#include <memory>
using namespace std;
enum class EstadoPartido {
    PROGRAMADO,
    JUGADO
};

// Funciones auxiliares para conversión de estado
string estadoToString(EstadoPartido estado);
EstadoPartido stringToEstado(const string& estadoStr);

class Partido {
private:
    int idPartido;
    string equipoLocal;
    string equipoVisitante;
    time_t fechaPartido;
    int golesLocal;
    int golesVisitante;
    EstadoPartido estado;

    static int nextId;

public:
    // Constructor para un partido nuevo
    Partido(const string& local, const string& visitante);

    // Constructor para deserializar
    Partido(int id, const string& local, const string& visitante,
            time_t fecha, int gl, int gv, const string& est);

    // Getters
    int getId() const;
    time_t getFechaPartido() const;
    EstadoPartido getEstado() const;

    // Métodos de gestión
    void convocarPartido(time_t fecha);
    void convocarPartido(const string& fechaStr); // Sobrecarga
    void registrarResultado(int local, int visitante);
    void registrarResultado(const std::string& marcador); // Sobrecarga
    string obtenerInfo() const;

    // Persistencia
    string serializar() const;
    string serializarTabla() const;
    static unique_ptr<Partido> deserializar(const string& linea);
    static void setNextId(int id);
};