#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

class item {
private:
    int _salud;
    int _ataqueLigero;
    int _ataquePesado;
    int _habilidadEspecial;

    sf::Sprite   sprite;
    sf::Texture  textura;

    static constexpr int totalFrames = 6;
    int frameWidth;
    int frameHeight;
    sf::IntRect frameActual;

    // Gestión de vida del ítem
    sf::Clock lifeClock;
    sf::Time  lifetime = sf::seconds(120.f); // vive 2 min el item
    bool active = false;

    // Parámetros de pulso (latido)
    float baseScaleX    = 0.1f;
    float baseScaleY    = 0.1f;
    float pulseAmplitude = 0.05f;
    float pulseSpeed     = 1.f;
    sf::Clock pulseClock;

public:
    // Constructor por defecto: carga spritesheet, define frame base y escala
    item();

    // Spawnea el ítem en una posición y frame aleatorio
    void spawn(const sf::Vector2u& windowSize);

    // Actualiza latido y desactiva tras expirar el tiempo
    void update();

    // Dibuja el ítem si está activo
    void draw(sf::RenderWindow& window) const;

    // ¿Está aún activo?
    bool isActive() const { return active; }
};
