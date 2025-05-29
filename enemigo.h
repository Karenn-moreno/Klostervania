#pragma once

#include "personaje.h"
#include <SFML/System.hpp>
#include <vector>

class enemigo : public personaje {
public:
    // Constructor recibe posici�n inicial, ruta al spritesheet y escala opcional
    enemigo(const sf::Vector2f& posInicial = {100.f, 100.f},
            const std::string& rutaSpritesheet = "img/spritesheet_guerreroespejo.png",
            const sf::Vector2f& escala = {0.3f, 0.3f});
    ~enemigo() override = default;

    // Spawn / respawn
    void setActivo(bool activo);
    bool estaActivo() const;

    // Realiza un ataque aleatorio: ligero, pesado o especial
    // destino: posici�n donde dirigir el ataque (p.ej. posici�n del jugador)
    virtual int ataque(const sf::Vector2f& destino);

    // IA y animaci�n (usa l�gica de personaje para animar)
    void update(float deltaTime,
                bool moviendoDer = false,
                bool moviendoIzq = false,
                bool moviendoArr = false,
                bool moviendoAbj = false) override;
    void draw(sf::RenderWindow& window) override;

    // Indica si el enemigo est� en combate por turnos
    void setModoBatalla(bool activo) { _modoBatalla = activo; }

private:
    bool _modoBatalla = false;
    // Estad�sticas avanzadas
    int _maxSalud = 500;

    // Spawn / estado
    bool _activo = true;
    sf::Clock _respawnClock;
    sf::Time  _respawnDelay = sf::seconds(10.f);//segundos
    sf::Vector2f _posInicial;

    // IA
    enum class EstadoIA { patrullando, persiguiendo, atacando } _estadoIA;
    std::vector<sf::Vector2f> _puntosPatrulla;
    size_t _indicePatrulla = 0;
    float _tiempoDesdeUltimoMovimiento = 0.f;
};
