#pragma once
#include "Puntaje.h"
#include <vector>
#include "personaje.h"
#include <fstream>

class ArchivoPuntaje {
private:
    char nombreArchivo[50];

public:
    ArchivoPuntaje(const char* n);

    Puntaje leerRegistro(int pos);
    int contarRegistros();
    bool grabarRegistro(const Puntaje& p);
    int buscarPorNombre(const std::string& nombre);
    bool actualizarRegistro(int pos, const Puntaje& p);

    std::vector<Puntaje> leerTodos();
    bool guardarPartida(const personaje& jugador, const sf::Vector2f& posicion);
    bool cargarPartida(personaje& jugador, sf::Vector2f& posicion);

};
