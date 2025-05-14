#include "item.h"
#include <cstdlib>
#include <cmath>

item::item() {
    // Carga de la textura
    if (!textura.loadFromFile("img/spritesheet_items.png"))
        std::cerr << "Error al cargar textura items" << std::endl;
    sprite.setTexture(textura);

    // Tamaño fijo de frame de 500x500
    frameWidth  = 500;
    frameHeight = 500;
    frameActual = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(frameActual);

    // Escala base
    sprite.setScale(baseScaleX, baseScaleY);

    // Seed aleatorio (solo una vez)
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
}

void item::spawn(const sf::Vector2u& windowSize) {
    int idx = std::rand() % totalFrames;
    frameActual.left = idx * frameWidth;
    sprite.setTextureRect(frameActual);

    float x = static_cast<float>(std::rand() % (windowSize.x - frameWidth));
    float y = static_cast<float>(std::rand() % (windowSize.y - frameHeight));
    sprite.setPosition(x, y);

    lifeClock.restart();
    pulseClock.restart();
    active = true;
}

void item::update() {
    if (!active) return;
    if (lifeClock.getElapsedTime() >= lifetime) {
        active = false;
        return;
    }
    // Pulso de latido
    float t = pulseClock.getElapsedTime().asSeconds();
    float factor = 1.f + pulseAmplitude * std::sin(2.f * 3.14159265f * pulseSpeed * t);
    sprite.setScale(baseScaleX * factor, baseScaleY * factor);
}

void item::draw(sf::RenderWindow& window) const {
    if (active)
        window.draw(sprite);
}
