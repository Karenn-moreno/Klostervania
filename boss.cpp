#include "boss.h"
#include <cstdlib>

// Constructor genérico de Boss
Boss::Boss(const sf::Vector2f& posInicial,
           const std::string& rutaSpritesheet,
           const sf::Vector2f& escala)
    : enemigo(posInicial, rutaSpritesheet, escala)
{
}

// Lógica de ataque con 5 opciones (3 normales + 2 especiales)
int Boss::ataque(const sf::Vector2f& destino) {
    // Orientación igual que en enemigo
    float escX = std::abs(sprite.getScale().x);
    float escY = sprite.getScale().y;
    if (destino.x > sprite.getPosition().x) {
        sprite.setScale(escX, escY);
        sprite.setOrigin(0.f, 0.f);
    } else {
        sprite.setScale(-escX, escY);
        sprite.setOrigin(static_cast<float>(frameWidth), 0.f);
    }

    int opcion = std::rand() % 5;
    int danio = 0;
    switch (opcion) {
        case 0:
            danio = getAtaqueLigero();
            ataqueLigero(destino);
            break;
        case 1:
            danio = getAtaquePesado();
            ataquePesado(destino);
            break;
        case 2:
            danio = getHabilidadEspecial();
            habilidadEspecial(destino);
            break;
        case 3:
            danio = ataqueEspecial1(destino);
            break;
        case 4:
            danio = ataqueEspecial2(destino);
            break;
    }
    return danio;
}

// Implementación de ataques especiales
int Boss::ataqueEspecial1(const sf::Vector2f& destino) {
    estado       = estadoPersonaje::habilidadEspecial;
    atacando     = true;
    ataqueFase   = 0;
    ataqueLlegado= false;
    ataqueTargetPos = destino;
    currentFrame = 0;
    frameTimer   = 0.f;

    // Asigna la fila de animación especial 1
    sprite.setTextureRect({0, filaFrameEspecial1 * frameHeight, frameWidth, frameHeight});
    return _habilidadEspecial + 10; // daño extra
}

int Boss::ataqueEspecial2(const sf::Vector2f& destino) {
    estado       = estadoPersonaje::habilidadEspecial;
    atacando     = true;
    ataqueFase   = 0;
    ataqueLlegado= false;
    ataqueTargetPos = destino;
    currentFrame = 0;
    frameTimer   = 0.f;

    // Asigna la fila de animación especial 2
    sprite.setTextureRect({0, filaFrameEspecial2 * frameHeight, frameWidth, frameHeight});
    return _habilidadEspecial + 20; // aún más daño
};
