#include <iostream>
#include <exception>
#include "menuPrincipal.h"

// Declaramos las funciones del main.cpp
void cargarPlantillaInicial();
void cargarPartidosDesdeArchivo();
void menu();

int main() {
    // leemos todos los datos principales 
    cargarPlantillaInicial();
    cargarPartidosDesdeArchivo();

    // llamamos a la clase menu principal
    try {
        menu();
    } catch (const std::exception& e) {
        std::cerr << "Error no esperado en el menu principal: " << e.what() << '\n';
    }
}
