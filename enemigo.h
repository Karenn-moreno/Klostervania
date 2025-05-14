#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class enemigo
{
public:

    enemigo();

    // --- Setters de estad�sticas ---
    void setSalud(int salud);               ///< Establece los puntos de vida del enemigo
    void setAtaqueLigero(int ataqueLigero); ///< Establece el da�o de ataque ligero
    void setAtaquePesado(int ataquePesado); ///< Establece el da�o de ataque pesado
    void setHabilidadEspecial(int habilidadEspecial); ///< Establece el poder de la habilidad especial

    // --- Getters de estad�sticas ---
    int getSalud();            ///< Devuelve los puntos de vida actuales
    int getAtaqueLigero();     ///< Devuelve el da�o de ataque ligero
    int getAtaquePesado();     ///< Devuelve el da�o de ataque pesado
    int getHabilidadEspecial();///< Devuelve el poder de la habilidad especial

    // --- L�gica de juego ---
    /**
     * Actualiza el estado del enemigo:
     * - Movimiento aleatorio cada cierto tiempo
     * - Animaci�n de frames al caminar
     * @param deltaTime Tiempo transcurrido desde el �ltimo frame (segundos)
     */
    void update(float deltaTime);

    /**
     * Dibuja el sprite del enemigo en la ventana indicada.
     * @param window Referencia a la ventana de renderizado
     */
    void draw(sf::RenderWindow& window);

private:
    // --- Estad�sticas del enemigo ---
    int _salud;               ///< Puntos de vida
    int _ataqueLigero;        ///< Da�o de ataques ligeros
    int _ataquePesado;        ///< Da�o de ataques pesados
    int _habilidadEspecial;   ///< Poder de habilidad especial

    // --- Gr�ficos ---
    sf::Sprite sprite;        ///< Sprite que representa al enemigo
    sf::Texture textura;      ///< Textura del spritesheet completo

    // --- Control de movimiento y animaci�n ---
    float tiempoDesdeUltimoMovimiento = 0.f; ///< Acumula tiempo para elegir movimiento aleatorio
    float tiempoAnimacion = 0.f;             ///< (no usado directamente) acumulador general

    const int totalFrames = 3;  ///< N�mero de frames en la animaci�n de caminar
    int currentFrame = 0;       ///< Frame actual mostrado
    float frameTimer = 0.f;     ///< Acumula tiempo para cambio de frame
    float frameTime = 0.15f;    ///< Duraci�n de cada frame (segundos)
    int frameWidth = 575;       ///< Ancho de cada frame (p�xeles)
    int frameHeight = 1000;     ///< Alto de cada frame (p�xeles)
    sf::IntRect frameActual;    ///< Rect�ngulo que define el frame en la textura
};
