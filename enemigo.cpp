#include "enemigo.h"
#include <iostream>
#include <cstdlib>   // Para rand()
#include <cmath>     // Para std::sqrt

enemigo::enemigo(const sf::Vector2f& posInicial,
                 const std::string& rutaSpritesheet,
                 const sf::Vector2f& escala)
: personaje()
, _posInicial(posInicial)
{
    // Inicializar estadísticas base
    setSalud(_maxSalud);

    // Carga de textura desde ruta proporcionada
    if (!textura.loadFromFile(rutaSpritesheet)) {
        std::cerr << "Error cargando textura: " << rutaSpritesheet << std::endl;
    }
    sprite.setTexture(textura);

    // Configurar frame inicial y escala
    sprite.setTextureRect({0, 0, frameWidth, frameHeight});
    sprite.setScale(escala);
    sprite.setPosition(_posInicial);

    // Definir puntos de patrulla
    _puntosPatrulla = {{100.f, 200.f}, {300.f, 200.f}};
}

void enemigo::setActivo(bool activo) {
    _activo = activo;
    if (!_activo) {
        _respawnClock.restart();
    }
}

bool enemigo::estaActivo() const {
    return _activo;
}

void enemigo::update(float deltaTime,
                     bool movDer,
                     bool movIzq,
                     bool movArr,
                     bool movAbj)
{
    // 1) Respawn (si está inactivo)
    if (!_activo) {
        if (_respawnClock.getElapsedTime() >= _respawnDelay) {
            _activo          = true;
            setSalud(_maxSalud);
            sprite.setPosition(_posInicial);

            // **Limpia aquí todos los flags de ataque**:
            estado           = estadoPersonaje::quieto;
            atacando         = false;
            proyectilActivo  = false;
            ataqueLlegado    = false;
            ataqueFase       = 0;

            // Si usas modo batalla, desactívalo para que pueda patrullar o atacar de nuevo:
            _modoBatalla     = false;
        }
        // Salimos antes de hacer cualquier otra cosa
        return;
    }

    // 2) Si estamos en combate, sólo animamos (no patrullar)
    if (_modoBatalla) {
        personaje::update(deltaTime, movDer, movIzq, movArr, movAbj);
        return;
    }

    // 3) Movimiento aleatorio fuera de combate
    _tiempoDesdeUltimoMovimiento += deltaTime;
    float offsetX = 0.f, offsetY = 0.f;
    movDer = movIzq = false;

    if (_tiempoDesdeUltimoMovimiento >= 0.7f) {
        int dir = std::rand() % 8;
        const float speed = 4.f;
        const float diag  = speed / std::sqrt(2.f);
        // … tu lógica de switch() …
        sprite.move(offsetX, offsetY);
        _tiempoDesdeUltimoMovimiento = 0.f;
    }

    // 4) Animación base usando esos flags
    personaje::update(deltaTime, movDer, movIzq, movArr, movAbj);
}
void enemigo::draw(sf::RenderWindow& window) {
    if (_activo) {
        personaje::draw(window);
    }
}

// Realiza un ataque aleatorio: ligero, pesado o especial
int enemigo::ataque(const sf::Vector2f& destino) {
    // 1) Orientación según destino
    float escX = std::abs(sprite.getScale().x);
    float escY = sprite.getScale().y;
    if (destino.x > sprite.getPosition().x) {
        // Target a la derecha -> escala positiva
        sprite.setScale( escX, escY );
        sprite.setOrigin(0.f, 0.f);
    } else {
        // Target a la izquierda → espejo horizontal
        sprite.setScale(-escX, escY);
        sprite.setOrigin(static_cast<float>(frameWidth), 0.f);
    }
    int r = std::rand() % 3;    // 0=ligero, 1=pesado, 2=especial
    int danio = 0;
    switch (r) {
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
    }
    return danio;
}
