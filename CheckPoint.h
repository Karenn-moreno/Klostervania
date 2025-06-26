#pragma once

#include <SFML/Graphics.hpp>
#include "personaje.h"
#include "popUpCartel.h"

class CheckPoint
{
private:
    sf::Sprite sprite;
    sf::Texture textura;
    bool activo;
    PopUpCartel popup;

public:
    CheckPoint();  // Constructor que carga la textura y recursos del popup

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

    void update(const personaje& jugador, const sf::Vector2u& windowSize);
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event);
    bool isPopupActive() const;
};
