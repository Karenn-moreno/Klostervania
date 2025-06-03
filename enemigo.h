#pragma once

#include "personaje.h"
#include <SFML/Graphics.hpp>
#include <vector>

class enemigo : public personaje {
public:
    // Constructor recibe posición inicial, ruta al spritesheet y escala opcional
    enemigo(const sf::Vector2f& posInicial,
            const std::string& rutaSpritesheet,
            const sf::Vector2f& escala);
    ~enemigo() override = default;

    // --- Spawn / respawn ---
    void setActivo(bool activo);
    bool estaActivo() const;

    // --- Ataque ---
    // Realiza un ataque aleatorio: ligero, pesado o especial. Devuelve el daño.
    virtual int ataque(const sf::Vector2f&);  // Eliminado override aquí

    // --- IA y animación ---
    // Si _modoBatalla es true, delega en personaje::update para animar turnos
    void update(float deltaTime,
                bool moviendoDer  = false,
                bool moviendoIzq  = false,
                bool moviendoArr  = false,
                bool moviendoAbj  = false) override;

    // Dibuja al enemigo solo si está activo
    void draw(sf::RenderWindow& window) override;

    // --- Getters heredados de personaje ---
    // El método base es "virtual const sf::Sprite& getSprite() const"
    // Por tanto aquí debe tener la misma firma (con const al final) y marcar override:
    const sf::Sprite& getSprite() const override;
    sf::Vector2f      getPosition() const override;
    sf::FloatRect     getBounds() const override;

    // --- Modo batalla ---
    void setModoBatalla(bool activo) { _modoBatalla = activo; }
    bool estaEnBatalla() const       { return _modoBatalla; }

    bool _modoBatalla = false;

private:
    // --- Estadísticas del enemigo ---
    int _maxSalud = 500;

    // --- Spawn / estado ---
    bool _activo = true;
    sf::Clock    _respawnClock;
    sf::Time     _respawnDelay = sf::seconds(10.f);
    sf::Vector2f _posInicial;

    // --- IA de patrulla fuera de combate ---
    std::vector<sf::Vector2f> _puntosPatrulla;
    size_t                    _indicePatrulla = 0;
    float                     _tiempoDesdeUltimoMovimiento = 0.f;
};
