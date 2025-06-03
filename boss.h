#pragma once

#include "enemigo.h"

// Clase para jefes (Bosses)
class Boss : public enemigo {
public:
    // Constructor: delega en enemigo (y en personaje) para cargar todo
    Boss(const sf::Vector2f& posInicial,
         const std::string& rutaSpritesheet,
         const sf::Vector2f& escala);

    // Sobrescribimos ataque para incluir 2 ataques adicionales
    // (el método base ataque debe ser virtual en enemigo)
    int ataque(const sf::Vector2f& destino) override;

protected:
    // Métodos para los ataques especiales del Boss
    int ataqueEspecial1(const sf::Vector2f& destino);
    int ataqueEspecial2(const sf::Vector2f& destino);

    // Filas y cantidad de frames para animaciones especiales
    static constexpr int filaFrameEspecial1      = 7;
    static constexpr int cantidadFrameEspecial1  = 4;
    static constexpr int filaFrameEspecial2      = 8;
    static constexpr int cantidadFrameEspecial2  = 4;
};

