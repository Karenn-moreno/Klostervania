#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include "personaje.h"

class item
{
public:
    item();

    // Spawnea en posición aleatoria y asigna un bonus fijo
    void spawn(const sf::Vector2u& windowSize);

    // Llama esto cada frame para animar y gestionar respawn
    void update();
    void handleEvent(const sf::Event& event);

    // Dibuja sólo si está activo
    void draw(sf::RenderWindow& window);

    // Indica si aún está en el mapa
    bool isActive() const
    {
        return active;
    }

    // Intenta recoger el ítem: si colisiona, aplica bonus y devuelve true
    bool tryPickup(personaje& jugador);

    bool isPanelActive() const;

private:
    // Bonus que guarda este item
    enum BonusType { Salud, Ligero, Pesado, Especial, ataques, todos };
    int _tipoItem; //le asigno el dibujo y su recompensa
    BonusType bonusTipo;

    static constexpr int BONUS_VAL[] = { 50, 30, 20, 50,30, 25 };  //salud, ligero, pesado, especial, ataques, todos

    // Spritesheet y frame
    sf::Sprite   sprite;
    sf::Texture  textura;
    static constexpr int totalFrames = 6;
    int frameWidth  = 500;
    int frameHeight = 500;
    sf::IntRect frameActual;

    // Control de vida y respawn
    sf::Clock lifeClock;
    sf::Time  lifetime   = sf::seconds(120.f);
    bool      active     = false;

    // Latido
    float baseScaleX     = 0.1f;
    float baseScaleY     = 0.1f;
    float pulseAmplitude = 0.05f;
    float pulseSpeed     = 0.5f;
    sf::Clock pulseClock;

    // Semilla única
    static bool seeded;
    // --- Para el panel de recompensa ---
    sf::Texture panelTexture;
    sf::Sprite  panelSprite;
    sf::Font    panelFont;
    sf::Text    panelText;
    bool        panelActive = false;
        // Para controlar la animación de pop-up
    float _popupScale = 0.f;
};
