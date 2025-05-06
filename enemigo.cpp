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


void enemigo::update(float deltaTime)
{
    bool caminando = false;
    tiempoDesdeUltimoMovimiento += deltaTime;

    if (tiempoDesdeUltimoMovimiento >= 0.7f)
    {
        int direccion = rand() % 8 + 1;  // 1 a 8
        float offsetX = 0.f;
        float offsetY = 0.f;
        const float diag = 2.8f; // Aproximadamente 4 / √2, para hacer las diagonales

        switch (direccion)
        {
        case 1:
            offsetY = -4.f;
            break; // Arriba
        case 2:
            offsetY =  4.f;
            break; // Abajo
        case 3:
            offsetX =  4.f;
            break; // Derecha
        case 4:
            offsetX = -4.f;
            break; // Izquierda

        case 5:
            offsetX =  diag;
            offsetY = -diag;
            break;
        case 6:
            offsetX =  diag;
            offsetY =  diag;
            break;
        case 7:
            offsetX = -diag;
            offsetY =  diag;
            break;
        case 8:
            offsetX = -diag;
            offsetY = -diag;
            break;
        }
        sprite.move(offsetX, offsetY);
        tiempoDesdeUltimoMovimiento = 0.f;
    // Detectar si se está "moviendo hacia la derecha o izquierda" para la escala
        if (offsetX > 0) {
            sprite.setScale(0.15f, 0.15f);  // mirando a la derecha
            sprite.setOrigin(0, 0);
        } else if (offsetX < 0) {
            sprite.setScale(-0.15f, 0.15f); // mirando a la izquierda
            sprite.setOrigin(frameWidth, 0);
        }
        caminando = true;
    }
    // Animación de frames como en personaje
    if (caminando) {
        frameTimer += deltaTime;
        if (frameTimer >= frameTime) {
            frameTimer = 0.f; // igual que en personaje
            currentFrame = (currentFrame + 1) % totalFrames;
        }

        int left = currentFrame * frameWidth;
        int top = 0;  // aún en la fila 0
        sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
    } /*else {
        // si no está caminando, volver al frame quieto
        currentFrame = 0;
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }*/
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
