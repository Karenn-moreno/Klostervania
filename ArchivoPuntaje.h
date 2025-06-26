#pragma once
#include "Puntaje.h"

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
};
