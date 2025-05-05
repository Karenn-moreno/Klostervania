#include "enemigo.h"

void enemigo::setSalud(int salud)
{
    _salud = salud;
};
void enemigo::setAtaqueLigero(int ataqueLigero)
{
    _ataqueLigero = ataqueLigero;
};
void enemigo::setAtaquePesado(int ataquePesado)
{
    _ataquePesado = ataquePesado;
};
void enemigo::setHabilidadEspecial(int habilidadEspecial)
{
    _habilidadEspecial = habilidadEspecial;
};

int enemigo::getSalud()
{
    return _salud;
};
int enemigo::getAtaqueLigero()
{
    return _ataqueLigero;
};
int enemigo::getAtaquePesado()
{
    return _ataquePesado;
};
int enemigo::getHabilidadEspecial()
{
    return _habilidadEspecial;
};


void enemigo::update(float deltaTime) {
    frameTimer += deltaTime; // Acumulamos el tiempo

    int totalFrames = 3; // El enemigo tiene 3 frames para camina

    if (frameTimer >= velocidadCambio) {
        frameTimer =0.2f; // Reducimos en lugar de reiniciar abruptamente
        currentFrame = (currentFrame + 1) % totalFrames; // Alternar entre los 3 frames
        int left = currentFrame * frameWidth; // Ajustar el cuadro en el spritesheet
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight)); // Aplicar nuevo frame
    }

    // **Movimiento aleatorio dentro del radio**
    sf::Vector2f nuevaPosicion = sprite.getPosition() + direccion * velocidad;
    sf::Vector2f diferencia = nuevaPosicion - posicionInicial;

    if (std::sqrt(diferencia.x * diferencia.x + diferencia.y * diferencia.y) > radioMovimiento) {
        direccion = generarDireccionAleatoria(); // Si se sale del radio, cambia dirección
    }

    sprite.move(direccion * velocidad);
}

sf::Vector2f enemigo::generarDireccionAleatoria() {
    float angulo = static_cast<float>(std::rand() % 360);
    float radianes = angulo * 3.14159265359f / 180.0f;
    return sf::Vector2f(std::cos(radianes), std::sin(radianes));
}

void enemigo::draw(sf::RenderWindow& window)
{
    if (sprite.getTexture() != nullptr)    // Verifica que la textura esté asignada
    {
        window.draw(sprite);// Renderiza el personaje
    }
    else
    {
        std::cout << "Error: La textura del personaje no está cargada" << std::endl;
    }
};
