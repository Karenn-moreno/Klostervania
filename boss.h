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

    void update(float deltaTime,
              bool moviendoDer    = false,
              bool moviendoIzq    = false,
              bool moviendoArriba = false,
              bool moviendoAbajo  = false) override;

protected:
    // Métodos para los ataques especiales del Boss
    int ataqueEspecial1(const sf::Vector2f& destino);
    int ataqueEspecial2(const sf::Vector2f& destino);

    // Filas y cantidad de frames para animaciones especiales
    static constexpr int filaFrameEspecial1      = 4;
    static constexpr int cantidadFrameEspecial1  = 6;
    static constexpr int filaFrameEspecial2      = 5;
    static constexpr int cantidadFrameEspecial2  = 6;

    //   bandera para saber cuál especial está en curso:
    int _especialEnCurso = 0;
    //   0 = ninguna, 1 = Especial1, 2 = Especial2
};

