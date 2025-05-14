#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class enemigo
{
public:

    enemigo();

    // --- Setters de estadísticas ---
    void setSalud(int salud);               ///< Establece los puntos de vida del enemigo
    void setAtaqueLigero(int ataqueLigero); ///< Establece el daño de ataque ligero
    void setAtaquePesado(int ataquePesado); ///< Establece el daño de ataque pesado
    void setHabilidadEspecial(int habilidadEspecial); ///< Establece el poder de la habilidad especial

    // --- Getters de estadísticas ---
    int getSalud();            ///< Devuelve los puntos de vida actuales
    int getAtaqueLigero();     ///< Devuelve el daño de ataque ligero
    int getAtaquePesado();     ///< Devuelve el daño de ataque pesado
    int getHabilidadEspecial();///< Devuelve el poder de la habilidad especial

    // --- Lógica de juego ---
    /**
     * Actualiza el estado del enemigo:
     * - Movimiento aleatorio cada cierto tiempo
     * - Animación de frames al caminar
     * @param deltaTime Tiempo transcurrido desde el último frame (segundos)
     */
    void update(float deltaTime);

    /**
     * Dibuja el sprite del enemigo en la ventana indicada.
     * @param window Referencia a la ventana de renderizado
     */
    void draw(sf::RenderWindow& window);

private:
    // --- Estadísticas del enemigo ---
    int _salud;               ///< Puntos de vida
    int _ataqueLigero;        ///< Daño de ataques ligeros
    int _ataquePesado;        ///< Daño de ataques pesados
    int _habilidadEspecial;   ///< Poder de habilidad especial

    // --- Gráficos ---
    sf::Sprite sprite;        ///< Sprite que representa al enemigo
    sf::Texture textura;      ///< Textura del spritesheet completo

    // --- Control de movimiento y animación ---
    float tiempoDesdeUltimoMovimiento = 0.f; ///< Acumula tiempo para elegir movimiento aleatorio
    float tiempoAnimacion = 0.f;             ///< (no usado directamente) acumulador general

    const int totalFrames = 3;  ///< Número de frames en la animación de caminar
    int currentFrame = 0;       ///< Frame actual mostrado
    float frameTimer = 0.f;     ///< Acumula tiempo para cambio de frame
    float frameTime = 0.15f;    ///< Duración de cada frame (segundos)
    int frameWidth = 575;       ///< Ancho de cada frame (píxeles)
    int frameHeight = 1000;     ///< Alto de cada frame (píxeles)
    sf::IntRect frameActual;    ///< Rectángulo que define el frame en la textura
};
