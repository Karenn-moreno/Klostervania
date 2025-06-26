#pragma once
#include <string>

class Puntaje {
private:
    char nombre[30];
    int puntos;

public:
    Puntaje();
    Puntaje(const std::string& n, int p);

    std::string getNombre() const;
    int getPuntos() const;

    void setNombre(const std::string& n);
    void setPuntos(int p);
    void agregarPuntos(int cantidad);
};
