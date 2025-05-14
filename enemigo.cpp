#include "enemigo.h"
#include <iostream>
#include <cstdlib>  // Para rand()

// --- Setters de estadísticas del enemigo ---
void enemigo::setSalud(int salud)
{
    _salud = salud;  // Actualiza los puntos de vida del enemigo
}

void enemigo::setAtaqueLigero(int ataqueLigero)
{
    _ataqueLigero = ataqueLigero;  // Actualiza el daño de ataque ligero
}

void enemigo::setAtaquePesado(int ataquePesado)
{
    _ataquePesado = ataquePesado;  // Actualiza el daño de ataque pesado
}

void enemigo::setHabilidadEspecial(int habilidadEspecial)
{
    _habilidadEspecial = habilidadEspecial;  // Actualiza el poder de la habilidad especial
}

// --- Getters de estadísticas del enemigo ---
int enemigo::getSalud()
{
    return _salud;  // Devuelve los puntos de vida actuales
}

int enemigo::getAtaqueLigero()
{
    return _ataqueLigero;  // Devuelve el daño de ataque ligero
}

int enemigo::getAtaquePesado()
{
    return _ataquePesado;  // Devuelve el daño de ataque pesado
}

int enemigo::getHabilidadEspecial()
{
    return _habilidadEspecial;  // Devuelve el poder de la habilidad especial
}

// Constructor: carga textura, configura sprite, frame
enemigo::enemigo()
    : _salud(500)
    , _ataqueLigero(10)
    , _ataquePesado(15)
    , _habilidadEspecial(25)
{
    if (!textura.loadFromFile("img/spritsheep_demonio_derecha.png"))    // Carga la imagen completa
    {
        std::cout << "Error al cargar la imagen del personaje" << std::endl;
    }
    sprite.setTexture(textura);  // Asocia la textura al sprite

    // Definimos el primer frame de la animación (ejemplo: cada frame mide 64x64 píxeles)
    frameActual = sf::IntRect(0, 0, 590, 1000);  // X=0, Y=0, Ancho=500, Alto=550
    sprite.setTextureRect(frameActual);  // Aplicamos la selección del primer frame
    sprite.setScale(0.15f, 0.15f); // Reduce el tamaño
    // Variables de control para el cambio de frames

    sprite.setPosition(500, 500);  // Posición inicial
}

// --- Actualización del enemigo: movimiento aleatorio y animación de frames ---
void enemigo::update(float deltaTime)
{
    bool caminando = false;
    tiempoDesdeUltimoMovimiento += deltaTime;  // Acumula tiempo desde último movimiento

    // Cada 0.7s, el enemigo elige una dirección aleatoria
    if (tiempoDesdeUltimoMovimiento >= 0.7f)
    {
        int direccion = rand() % 8 + 1;  // Valores de 1 a 8
        float offsetX = 0.f;
        float offsetY = 0.f;
        const float diag = 2.8f;  // Aproximadamente 4 / √2 para diagonales

        // Determina el desplazamiento según la dirección elegida
        switch (direccion)
        {
        case 1:
            offsetY = -4.f;
            break;          // Arriba
        case 2:
            offsetY =  4.f;
            break;          // Abajo
        case 3:
            offsetX =  4.f;
            break;          // Derecha
        case 4:
            offsetX = -4.f;
            break;          // Izquierda
        case 5:
            offsetX =  diag;
            offsetY = -diag;
            break; // Arriba-Derecha
        case 6:
            offsetX =  diag;
            offsetY =  diag;
            break; // Abajo-Derecha
        case 7:
            offsetX = -diag;
            offsetY =  diag;
            break; // Abajo-Izquierda
        case 8:
            offsetX = -diag;
            offsetY = -diag;
            break; // Arriba-Izquierda
        }

        // Mover el sprite
        sprite.move(offsetX, offsetY);
        tiempoDesdeUltimoMovimiento = 0.f;  // Reiniciar contador

        // Ajustar orientación horizontal del sprite
        if (offsetX > 0)
        {
            sprite.setScale(0.15f, 0.15f);  // Mirando a la derecha
            sprite.setOrigin(0, 0);
        }
        else if (offsetX < 0)
        {
            sprite.setScale(-0.15f, 0.15f); // Mirando a la izquierda
            sprite.setOrigin(frameWidth, 0);
        }
        caminando = true;
    }

    // --- Animación de frames cuando está caminando ---
    if (caminando)
    {
        frameTimer += deltaTime;  // Acumula tiempo para cambio de frame
        if (frameTimer >= frameTime)
        {
            frameTimer = 0.f;  // Reiniciar temporizador de frame
            currentFrame = (currentFrame + 1) % totalFrames;  // Siguiente frame
        }
        // Selecciona el rectángulo correcto en el spritesheet
        int left = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
    }
    // Si no está caminando, se mantiene el frame actual (se podría resetear a 0 aquí)
}

// --- Renderizado del enemigo ---
void enemigo::draw(sf::RenderWindow& window)
{
    if (sprite.getTexture() != nullptr)
    {
        window.draw(sprite);  // Dibuja el sprite en la ventana
    }
    else
    {
        std::cout << "Error: La textura del enemigo no está cargada" << std::endl;
    }
}
