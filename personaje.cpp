#include "personaje.h"
#include <cmath>  // Para std::sin y constantes matemáticas

// --- Setters de estadísticas ---
void personaje::setSalud(int salud) {
    _salud = salud;  // Actualiza puntos de vida
}

void personaje::setAtaqueLigero(int ataqueLigero) {
    _ataqueLigero = ataqueLigero;  // Actualiza daño ataque ligero
}

void personaje::setAtaquePesado(int ataquePesado) {
    _ataquePesado = ataquePesado;  // Actualiza daño ataque pesado
}

void personaje::setHabilidadEspecial(int habilidadEspecial) {
    _habilidadEspecial = habilidadEspecial;  // Actualiza poder habilidad especial
}

// --- Getters de estadísticas ---
int personaje::getSalud() const {
    return _salud;  // Devuelve puntos de vida actuales
}

int personaje::getAtaqueLigero() const {
    return _ataqueLigero;  // Devuelve daño de ataque ligero
}

int personaje::getAtaquePesado() const {
    return _ataquePesado;  // Devuelve daño de ataque pesado
}

int personaje::getHabilidadEspecial() const {
    return _habilidadEspecial;  // Devuelve valor de habilidad especial
}

const sf::Sprite& personaje::getSprite() const{
    return sprite;
};

sf::FloatRect personaje::getBounds() const
    {
        return sprite.getGlobalBounds();
    };

// Constructor: carga textura, configura sprite, frame y respiración
personaje::personaje()
    : _salud(1000)
    , _ataqueLigero(10)
    , _ataquePesado(15)
    , _habilidadEspecial(25)
    , totalFrames(6)
    , currentFrame(0)
    , frameTime(0.15f)
    , frameTimer(0.f)
    , frameWidth(500)
    , frameHeight(500)
{
    // 1) Cargar spritesheet
    if (!textura.loadFromFile("img/spritesheet_guerrero.png")) {
        std::cerr << "Error al cargar la textura del personaje\n";
    }
    sprite.setTexture(textura);

    // 2) Primer frame
    frameActual = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(frameActual);

    // 3) Escala y posición inicial
    sprite.setScale(0.25f, 0.25f);
    sprite.setPosition(50.f, 400.f);

    // 4) Inicializar respiración
    baseScaleX      = sprite.getScale().x;
    baseScaleY      = sprite.getScale().y;
    breathAmplitude = 0.02f;
    breathSpeed     = 0.25f;
    breathClock.restart();
}

// Mueve el sprite según los desplazamientos dados
void personaje::mover(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

// Detiene la animación, resetea frame y temporizador
void personaje::detener() {
    currentFrame = 0;
    frameTimer   = 0.f;
    sprite.setTextureRect(frameActual);
}

// Dibuja el sprite en la ventana
void personaje::draw(sf::RenderWindow& window) {
    if (sprite.getTexture()) {
        window.draw(sprite);
    }
}

// --- Lógica de actualización: animación, movimiento y respiración ---
void personaje::update(float deltaTime, bool moviendoDer, bool moviendoIzq, bool moviendoArr, bool moviendoAbj) {
    // 1) Animación de caminata
    bool caminando = moviendoDer || moviendoIzq;
    if (caminando) {
        // Incrementar temporizador
        frameTimer += deltaTime;
        if (frameTimer >= frameTime) {
            frameTimer = 0.f;
            // Ciclo de frames
            currentFrame = (currentFrame + 1) % totalFrames;
        }
        // Seleccionar frame en spritesheet (fila 0)
        int left = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
        // Ajustar orientación según dirección
        if (moviendoIzq) {
            sprite.setScale(-baseScaleX, baseScaleY);
            sprite.setOrigin(frameWidth, 0);
        } else if (moviendoDer) {
            sprite.setScale(baseScaleX, baseScaleY);
            sprite.setOrigin(0, 0);
        }
    } else {
        // En reposo, resetear a primer frame
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect(0, 3000, frameWidth, frameHeight));
    }

    // 2) Efecto de respiración (pulso suave)
    float t = breathClock.getElapsedTime().asSeconds();
    float factor = 1.f + breathAmplitude * std::sin(2.f * 3.14159265f * breathSpeed * t);
    // Preservar flip horizontal
    float signX = sprite.getScale().x < 0 ? -1.f : 1.f;
    sprite.setScale(signX * baseScaleX * factor,
                    baseScaleY * factor);
}
