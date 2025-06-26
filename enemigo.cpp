#include "enemigo.h"
#include <cstdlib>   // std::rand
#include <cmath>     // std::hypot
#include <iostream>

// Constructor: delega en personaje(posInicial, rutaSpritesheet, escala)
// para que ese constructor fije escala y origin en la base automáticamente.
enemigo::enemigo(const sf::Vector2f& posInicial,
                 const std::string& rutaSpritesheet,
                 const sf::Vector2f& escala,
                 const sf::Vector2f& puntoPatrulla,
                 int cantAtaque)
    : personaje(posInicial, rutaSpritesheet, escala)
{
    _posInicial=posInicial;
    _cantAtaque=cantAtaque;
    _puntoPatrulla = puntoPatrulla;
    if (!fuenteTexto.loadFromFile("fonts/Rochester-Regular.ttf"))
    {
        std::cerr << "Error al cargar la fuente\n";
    }
    textoVida.setFont(fuenteTexto);
    textoVida.setCharacterSize(50);
//textoVida.setScale(0.09f, 0.09f);
    textoVida.setOutlineColor(sf::Color::Black);
    textoVida.setOutlineThickness(1);
}

// Activa o desactiva el enemigo; si se desactiva, arranca el reloj de respawn.
void enemigo::setActivo(bool activo)
{
    _activo = activo;
    if (!_activo)
    {
        _respawnClock.restart();
    }
}

bool enemigo::estaActivo() const
{
    return _activo;
}

// Dibuja el enemigo solo si está activo
void enemigo::draw(sf::RenderWindow& window)
{
    if (_activo)
    {
        personaje::draw(window);
    }
}

// getSprite, getPosition y getBounds simplemente delegan a la clase base:
const sf::Sprite& enemigo::getSprite() const
{
    return sprite;
}

sf::Vector2f enemigo::getPosition() const
{
    return sprite.getPosition();
}

sf::FloatRect enemigo::getBounds() const
{
    return sprite.getGlobalBounds();
}

// update: controla respawn, combate por turnos y patrulla fuera de combate
void enemigo::update(float deltaTime,
                     bool movDer,
                     bool movIzq,
                     bool movArr,
                     bool movAbj, int saludJugador )
{

//Asignar el texto y color después del update base
    textoVida.setString(std::to_string(getSalud()));
    if (saludJugador > 0)
    {
        float ratio = static_cast<float>(getSalud()) / saludJugador;

        if (ratio > 5.f)
            textoVida.setFillColor(sf::Color::Red);
        else if (ratio > 2.f)
            textoVida.setFillColor(sf::Color(255, 140, 0)); // Naranja;
        else if (ratio > 1.5f)
            textoVida.setFillColor(sf::Color::Yellow);
        else
            textoVida.setFillColor(sf::Color::White);
    }
    else
    {
        textoVida.setFillColor(sf::Color::White); // Por defecto
    }

// Reposicionar el texto arriba del enemigo
    sf::FloatRect bounds = textoVida.getLocalBounds();
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    textoVida.setPosition(
        spriteBounds.left + (spriteBounds.width / 2.f) - (bounds.width / 2.f),
        spriteBounds.top - bounds.height - 5.f
    );

    // 1) Respawn si está inactivo
    if (!_activo)
    {
        if (esBoss) return; // Si es un boss, nunca se reactiva

        if (_respawnClock.getElapsedTime() >= _respawnDelay)
        {
            // Reactivar enemigo y restaurar estado inicial
            _activo = true;
            setSalud(_maxSalud);
            sprite.setPosition(_posInicial);

            // Restaurar escala normal y origin en la base
            sprite.setScale(baseScaleX, baseScaleY);
            {
                sf::FloatRect local = sprite.getLocalBounds();
                sprite.setOrigin(0.f, local.height);
            }

            // Resetear flags internos de combate
            estado          = estadoPersonaje::quieto;
            atacando        = false;
            proyectilActivo = false;
            ataqueLlegado   = false;
            ataqueFase      = 0;
            _modoBatalla    = false;
        }
        // Salir sin hacer nada más mientras esté inactivo
        return;
    }

    // 2) Si está en combate por turnos, solo animar mediante personaje::update
    if (_modoBatalla)
    {
        personaje::update(deltaTime, movDer, movIzq, movArr, movAbj, saludJugador);
        return;
    }

    // 3) IA de patrulla simple: ir al punto y volver
    _tiempoDesdeUltimoMovimiento += deltaTime;

    if (_tiempoDesdeUltimoMovimiento >= 0.7f)
    {
        sf::Vector2f destino = _volviendo ? _posInicial : _puntoPatrulla;
        sf::Vector2f pos     = sprite.getPosition();
        sf::Vector2f dir     = destino - pos;
        float dist           = std::hypot(dir.x, dir.y);

        const float speedP = 4.f;
        if (dist > 1.f)
        {
            dir /= dist; // normalizar
            sprite.move(dir * speedP);
        }
        else
        {
            _volviendo = !_volviendo; // cuando llega, da la vuelta
        }

        _tiempoDesdeUltimoMovimiento = 0.f;
    }

    // 4) Delegar animaciones de caminata/respiración a personaje::update
    personaje::update(deltaTime, false, false, false, false, saludJugador);

}
// ataque: elige ataque aleatorio y lo ejecuta; devuelve el daño causado
int enemigo::ataque(const sf::Vector2f& destino)
{
    // 1) Guardar la posición base del “pie” antes de modificar el origen
    ataqueStartPos = sprite.getPosition();

    // 2) Obtener dimensiones reales del sprite (ya escalado)
    sf::FloatRect local = sprite.getLocalBounds();
    float ancho = local.width;
    float alto  = local.height;
    float escX  = std::abs(sprite.getScale().x);
    float escY  = sprite.getScale().y;

    // 3) Flip horizontal manteniendo origen en la base (Y = alto)
    if (destino.x > ataqueStartPos.x)
    {
        // Atacar a la derecha
        sprite.setScale(+escX, escY);
        sprite.setOrigin(0.f, alto);
    }
    else
    {
        // Atacar a la izquierda (flip)
        sprite.setScale(-escX, escY);
        sprite.setOrigin(ancho, alto);
    }

    // 4) Elegir tipo de ataque (ligero, pesado, especial) y lanzar animación
    int r = std::rand() % _cantAtaque; // 0 = ligero, 1 = pesado, 2 = especial
    int danio = 0;
    switch (r)
    {
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
