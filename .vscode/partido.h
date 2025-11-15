#pragma once
#include <string>
#include <ctime>
#include <memory>

enum class EstadoPartido {
    PROGRAMADO,
    JUGADO
};

// Funciones auxiliares para conversión de estado
std::string estadoToString(EstadoPartido estado);
EstadoPartido stringToEstado(const std::string& estadoStr);

class Partido {
private:
    int idPartido;
    std::string equipoLocal;
    std::string equipoVisitante;
    std::time_t fechaPartido;
    int golesLocal;
    int golesVisitante;
    EstadoPartido estado;

    static int nextId;

public:
    // Constructor para un partido nuevo
    Partido(const std::string& local, const std::string& visitante);

    // Constructor para deserializar
    Partido(int id, const std::string& local, const std::string& visitante,
            std::time_t fecha, int gl, int gv, const std::string& est);

    // Getters
    int getId() const;
    std::time_t getFechaPartido() const;
    EstadoPartido getEstado() const;

    // Métodos de gestión
    void convocarPartido(std::time_t fecha);
    void convocarPartido(const std::string& fechaStr); // Sobrecarga
    void registrarResultado(int local, int visitante);
    void registrarResultado(const std::string& marcador); // Sobrecarga
    std::string obtenerInfo() const;

    // Persistencia
    std::string serializar() const;
    static std::unique_ptr<Partido> deserializar(const std::string& linea);
    static void setNextId(int id);
};