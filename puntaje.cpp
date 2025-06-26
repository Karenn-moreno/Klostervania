#include "Puntaje.h"
#include <cstring>

Puntaje::Puntaje() : puntos(0) {
    std::strcpy(nombre, "");
}

Puntaje::Puntaje(const std::string& n, int p) : puntos(p) {
    std::strncpy(nombre, n.c_str(), sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';
}

std::string Puntaje::getNombre() const {
    return std::string(nombre);
}

int Puntaje::getPuntos() const {
    return puntos;
}

void Puntaje::setNombre(const std::string& n) {
    std::strncpy(nombre, n.c_str(), sizeof(nombre));
    nombre[sizeof(nombre) - 1] = '\0';
}

void Puntaje::setPuntos(int p) {
    puntos = p;
}

void Puntaje::agregarPuntos(int cantidad) {
    puntos += cantidad;
}


