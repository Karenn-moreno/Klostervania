#include "ArchivoPuntaje.h"
#include <iostream>
#include <cstdio>
#include <cstring>

ArchivoPuntaje::ArchivoPuntaje(const char* n) {
    std::strncpy(nombreArchivo, n, sizeof(nombreArchivo) - 1);
    nombreArchivo[sizeof(nombreArchivo) - 1] = '\0';
}

Puntaje ArchivoPuntaje::leerRegistro(int pos) {
    Puntaje p;
    FILE* f = fopen(nombreArchivo, "rb");
    if (f == NULL) {
        std::cout << "Error al abrir archivo en lectura\n";
        return p;
    }
    fseek(f, pos * sizeof(Puntaje), SEEK_SET);
    fread(&p, sizeof(Puntaje), 1, f);
    fclose(f);
    return p;
}

int ArchivoPuntaje::contarRegistros() {
    FILE* f = fopen(nombreArchivo, "rb");
    if (f == NULL) return 0;
    fseek(f, 0, SEEK_END);
    int tam = ftell(f);
    fclose(f);
    return tam / sizeof(Puntaje);
}

bool ArchivoPuntaje::grabarRegistro(const Puntaje& p) {
    FILE* f = fopen(nombreArchivo, "ab");
    if (f == NULL) {
        std::cout << "Error al abrir archivo en grabar\n";
        return false;
    }
    int escrito = fwrite(&p, sizeof(Puntaje), 1, f);
    fclose(f);
    return escrito == 1;
}

int ArchivoPuntaje::buscarPorNombre(const std::string& nombre) {
    int total = contarRegistros();
    for (int i = 0; i < total; i++) {
        Puntaje p = leerRegistro(i);
        if (p.getNombre() == nombre) {
            return i;
        }
    }
    return -1;
}

bool ArchivoPuntaje::actualizarRegistro(int pos, const Puntaje& p) {
    FILE* f = fopen(nombreArchivo, "rb+");
    if (f == NULL) {
        std::cout << "Error al abrir archivo en actualizar\n";
        return false;
    }
    fseek(f, pos * sizeof(Puntaje), SEEK_SET);
    int escrito = fwrite(&p, sizeof(Puntaje), 1, f);
    fclose(f);
    return escrito == 1;
}
