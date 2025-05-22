#pragma once
#include "popUpCartel.h"
#include <stdexcept>
#include <SFML/Graphics.hpp>

class PopUpCartel {
public:

PopUpCartel();

void cargarRecursos(const std::string& rutaTextura, const std::string& rutaFuente);

void mostrar(const std::string& mensaje, const sf::Vector2u& tamVentana);

void draw(sf::RenderWindow& ventana);

bool handleEvent(const sf::Event& evt);

bool isActive() const;

private:
    sf::Sprite   _spriteFondo;
    sf::Texture  _texturaFondo;
    sf::Text     _texto;
    sf::Font     _fuente;
    bool         _activo = false;
    // animación de escala
    float _scale       = 0.f;
    static constexpr float _targetScale = 0.8f;
    static constexpr float _step        = 0.05f;

    // escala base del fondo
    float _baseScaleX = 1.f;
    float _baseScaleY = 1.f;
};
