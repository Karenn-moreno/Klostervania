#include "boss.h"
#include <cstdlib>   // std::rand
#include <cmath>     // std::hypot

// Constructor: delega en enemigo y, automáticamente, en personaje
Boss::Boss(const sf::Vector2f& posInicial,
           const std::string& rutaSpritesheet,
           const sf::Vector2f& escala)
    : enemigo(posInicial, rutaSpritesheet, escala)
{
    // ajustar salud, stats u otras propiedades específicas del Boss

}

// ataque: elige entre 5 opciones (3 normales heredadas + 2 especiales del Boss)
int Boss::ataque(const sf::Vector2f& destino) {
    // 1) Guardar la posición base del pie (ya hace enemigo::ataque, pero aquí la repetimos para animaciones especiales)
    ataqueStartPos = sprite.getPosition();

    // 2) Obtener ancho/alto del sprite (ya escalado)
    sf::FloatRect local = sprite.getLocalBounds();
    float ancho = local.width;
    float alto  = local.height;
    float escX  = std::abs(sprite.getScale().x);
    float escY  = sprite.getScale().y;

    // 3) Flip horizontal manteniendo el origen en la base
    if (destino.x > ataqueStartPos.x) {
        // Hacia la derecha
        sprite.setScale(+escX, escY);
        sprite.setOrigin(0.f, alto);
    } else {
        // Hacia la izquierda
        sprite.setScale(-escX, escY);
        sprite.setOrigin(ancho, alto);
    }

    // 4) Elegir opción entre 0..4
    int opcion = std::rand() % 5;
    int danio = 0;
    switch (opcion) {
        case 0:
            danio = getAtaqueLigero();
            ataqueLigero(destino);  // Llama a personaje::ataqueLigero
            break;
        case 1:
            danio = getAtaquePesado();
            ataquePesado(destino);  // Llama a personaje::ataquePesado
            break;
        case 2:
            danio = getHabilidadEspecial();
            habilidadEspecial(destino);  // Llama a personaje::habilidadEspecial
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

// ataqueEspecial1: animación personalizada y daño extra
int Boss::ataqueEspecial1(const sf::Vector2f& destino) {
    // 1) Configurar estado de animación
    estado       = estadoPersonaje::habilidadEspecial;
    atacando     = true;
    ataqueFase   = 0;
    ataqueLlegado= false;
    ataqueTargetPos = destino;
    currentFrame = 0;
    frameTimer   = 0.f;

    // 2) Fijar el frame inicial de la animación especial 1
    sprite.setTextureRect({
        0,
        filaFrameEspecial1 * frameHeight,
        frameWidth,
        frameHeight
    });

    // El daño del especial: por ejemplo, base más 10
    return getHabilidadEspecial() + 10;
}

// ataqueEspecial2: otra animación y más daño
int Boss::ataqueEspecial2(const sf::Vector2f& destino) {
    // 1) Configurar estado de animación
    estado       = estadoPersonaje::habilidadEspecial;
    atacando     = true;
    ataqueFase   = 0;
    ataqueLlegado= false;
    ataqueTargetPos = destino;
    currentFrame = 0;
    frameTimer   = 0.f;

    // 2) Fijar el frame inicial de la animación especial 2
    sprite.setTextureRect({
        0,
        filaFrameEspecial2 * frameHeight,
        frameWidth,
        frameHeight
    });

    // El daño del segundo especial: base más 20
    return getHabilidadEspecial() + 20;
}
