#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include "personaje.h"
#include "popUpCartel.h"

class item
{
public:
    item();   // ahora construye internamente su PopUpCartel

    void spawn(const sf::Vector2u& windowSize);
    void update();
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);
    bool isActive() const { return active; }
    bool tryPickup(personaje& jugador);
    bool isPopupActive() const;

private:
    // — Pop-up autónomo —
    PopUpCartel _popup;

    // Bonus del ítem
    enum BonusType { Salud, Ligero, Pesado, Especial, ataques, todos };
    int _tipoItem;
    BonusType bonusTipo;
    static constexpr int BONUS_VAL[] = { 50,30,20,50,30,25 };

    // Gráficos del ítem
    sf::Sprite  sprite;
    sf::Texture textura;
    static constexpr int totalFrames = 6;
    int frameWidth  = 500, frameHeight = 500;
    sf::IntRect frameActual;

    // Respawn y latido
    sf::Clock lifeClock, pulseClock;
    sf::Time  lifetime = sf::seconds(120.f);
    bool      active   = false;
    float baseScaleX     = 0.1f, baseScaleY = 0.1f;
    float pulseAmplitude = 0.05f, pulseSpeed = 0.5f;

    // Para inicializar rand() solo una vez
    static bool seeded;
    // Para que el _popup conozca el tamaño
    sf::Vector2u _windowSize;
};

