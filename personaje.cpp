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
    bool caminando = moviendoDer || moviendoIzq;

    if (caminando) {
        frameTimer += deltaTime;
        if (frameTimer >= frameTime) {
            frameTimer = 0.02f; //modificando aca aumento el tiempo con el que cambian la velocidad de frames
            currentFrame = (currentFrame + 1) % 3;  // Se usa para decirle a SFML qué parte del spritesheet dibujar:3 frames en la fila 0
        }
        // Calcular la posición del frame en la fila 0
        int left = currentFrame * frameWidth;//cuanto me muevo a la derecha
        int top = 0;  // fila 0, para abajo no me muevo no me muevo
        sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));//seteo el nuevo cuadro del la textura

        if (moviendoIzq) {
            sprite.setScale(-0.15f, 0.15f);//mantengo el escalado que use en el constructor
            sprite.setOrigin(frameWidth, 0);//frameWidth → el origen se mueve al borde derecho del sprite (horizontalmente).
        }
        if (moviendoDer ){
            sprite.setScale(0.15f, 0.15f);
            sprite.setOrigin(0, 0);
        }
    } else {
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
}


void personaje::mover(float offsetX, float offsetY)
{
    sprite.move(offsetX, offsetY);  // Movimiento del personaje
}

void personaje::detener()
{
    currentFrame = 0;
    frameTimer = 0.0f; // Reinicia la animación
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
