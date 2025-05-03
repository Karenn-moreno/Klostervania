#include "personaje.h"

void personaje::setSalud(int salud)
{
    _salud = salud;
};
void personaje::setAtaqueLigero(int ataqueLigero)
{
    _ataqueLigero = ataqueLigero;
};
void personaje::setAtaquePesado(int ataquePesado)
{
    _ataquePesado = ataquePesado;
};
void personaje::setHabilidadEspecial(int habilidadEspecial)
{
    _habilidadEspecial = habilidadEspecial;
};

int personaje::getSalud()
{
    return _salud;
};
int personaje::getAtaqueLigero()
{
    return _ataqueLigero;
};
int personaje::getAtaquePesado()
{
    return _ataquePesado;
};
int personaje::getHabilidadEspecial()
{
    return _habilidadEspecial;
};

void personaje::update(float deltaTime, bool moviendoDer, bool moviendoIzq, bool moviendoArr, bool moviendoAbj) {
    tiempoAnimacion += deltaTime;

    if (moviendoDer) {
        if (tiempoAnimacion >= 0.1f) {
            frameActual.left = 500; // Cambiar a la imagen de movimiento a la derecha
        }
        sprite.move(0, std::sin(tiempoAnimacion * 10) * 2); //Ondulación v
    }
    else if (moviendoIzq) {
        if (tiempoAnimacion >= 0.1f) {
            frameActual.left = 1000; // Cambiar a la imagen de movimiento a la izquierda
        }
        sprite.move(0, std::sin(tiempoAnimacion * 10) * 2); // Ondulación v
    }
    else {
        frameActual.left = 0; // Imagen de quieto
    }

    sprite.setTextureRect(frameActual); // Aplicar el cambio
}


void personaje::mover(float offsetX, float offsetY)
{
    sprite.move(offsetX, offsetY);  // Movimiento del personaje
}

void personaje::detener()
{
    frameActual.left = 0; // Ajusta el frame al de "quieto"
    sprite.setTextureRect(frameActual);
}

void personaje::draw(sf::RenderWindow& window)
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
