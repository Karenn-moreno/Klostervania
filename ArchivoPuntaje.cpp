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

std::vector<Puntaje> ArchivoPuntaje::leerTodos() {
    std::vector<Puntaje> lista;
    FILE* f = fopen(nombreArchivo, "rb");
    if (f == NULL) return lista;

    Puntaje p;
    while (fread(&p, sizeof(Puntaje), 1, f) == 1) {
        lista.push_back(p);
    }

    fclose(f);
    return lista;
}
bool ArchivoPuntaje::guardarPartida(const personaje& jugador, const sf::Vector2f& pos)
{
    std::ofstream out("partida.dat");
    if (!out.is_open()) return false;

   out << jugador.getNombre() << "\n";
    out << jugador.getSalud() << "\n";
    out << jugador.getAtaqueLigero() << "\n";
    out << jugador.getAtaquePesado() << "\n";
    out << jugador.getHabilidadEspecial() << "\n";
    out << pos.x << " " << pos.y << "\n";

    return true;
}

bool ArchivoPuntaje::cargarPartida(personaje& jugador, sf::Vector2f& posicion)
{
    std::ifstream in("partida.dat");
    if (!in.is_open()) {
        std::cerr << "No se pudo abrir el archivo de partida\n";
        return false;
    }

    std::string nombre;
    if (!std::getline(in, nombre)) {
        std::cerr << "Error al leer el nombre del jugador\n";
        return false;
    }

    jugador.setNombre(nombre);

    int salud, al, ap, he;
    float posX, posY;

    if (!(in >> salud >> al >> ap >> he >> posX >> posY)) {
        std::cerr << "Error al leer estadísticas o posición del jugador\n";
        return false;
    }

    jugador.setSalud(salud);
    jugador.setAtaqueLigero(al);
    jugador.setAtaquePesado(ap);
    jugador.setHabilidadEspecial(he);
    posicion = {posX, posY};

    return true;
}
