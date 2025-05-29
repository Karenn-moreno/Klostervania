#pragma once

#include "enemigo.h"

// Clase base para jefes (bosses)
class Boss : public enemigo {
public:
    Boss(const sf::Vector2f& posInicial,
         const std::string& rutaSpritesheet,
         const sf::Vector2f& escala);

    // Sobrescribimos ataque para incluir 2 ataques adicionales
    int ataque(const sf::Vector2f& destino) override;

protected:
    // Nuevos ataques especiales
    int ataqueEspecial1(const sf::Vector2f& destino);
    int ataqueEspecial2(const sf::Vector2f& destino);

    // Filas y cantidad de frames para los ataques especiales
    static constexpr int filaFrameEspecial1      = 7;
    static constexpr int cantidadFrameEspecial1  = 4;
    static constexpr int filaFrameEspecial2      = 8;
    static constexpr int cantidadFrameEspecial2  = 4;
};
