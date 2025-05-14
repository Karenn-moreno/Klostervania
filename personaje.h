#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>   // Para sf::Clock
#include <string>
#include <iostream>

class personaje {
public:

    personaje();

    // --- Setters de estadísticas ---
    void setSalud(int salud);               // Establece los puntos de vida
    void setAtaqueLigero(int ataqueLigero); // Establece el daño de ataque ligero
    void setAtaquePesado(int ataquePesado); // Establece el daño de ataque pesado
    void setHabilidadEspecial(int habilidadEspecial); // Establece el valor de la habilidad especial

    // --- Getters de estadísticas ---
    int getSalud() const;                  // Devuelve los puntos de vida
    int getAtaqueLigero() const;           // Devuelve el daño de ataque ligero
    int getAtaquePesado() const;           // Devuelve el daño de ataque pesado
    int getHabilidadEspecial() const;      // Devuelve el valor de la habilidad especial

    // --- Lógica de juego ---
    void update(float deltaTime,
                bool moviendoDer,
                bool moviendoIzq,
                bool moviendoArr,
                bool moviendoAbj);        // Actualiza animación, movimiento y respiración
    void mover(float offsetX, float offsetY); // Mueve el sprite en pantalla
    void detener();                         // Detiene la animación y resetea frame

    // --- Renderizado ---
    void draw(sf::RenderWindow& window);    // Dibuja el personaje en la ventana

private:
    // --- Estadísticas del personaje ---
    int _salud = 1000;           // Puntos de vida actuales
    int _ataqueLigero = 10;      // Daño de ataque ligero
    int _ataquePesado = 15;      // Daño de ataque pesado
    int _habilidadEspecial = 25; // Potencia de la habilidad especial

    // --- Gráficos ---
    sf::Sprite sprite;           // Sprite que representa al personaje
    sf::Texture textura;         // Textura completa del spritesheet

    // --- Animación de sprites ---
    const int totalFrames = 6;   // Número de frames por fila en el spritesheet
    int currentFrame = 0;        // Índice del frame actual
    float frameTime = 0.15f;     // Duración de cada frame (segundos)
    float frameTimer = 0.f;      // Acumulador de tiempo para cambiar frame
    int frameWidth = 500;        // Ancho de cada frame (píxeles)
    int frameHeight = 500;       // Alto de cada frame (píxeles)
    sf::IntRect frameActual;     // Rectángulo que selecciona el frame en la textura

    // --- Respiración / pulso suave ---
    sf::Clock breathClock;       // Reloj para medir el ciclo de respiración
    float baseScaleX;            // Escala X original del sprite
    float baseScaleY;            // Escala Y original del sprite
    float breathAmplitude;       // Amplitud de variación de escala (e.g. 0.005 = ±0.5%)
    float breathSpeed;           // Velocidad de ciclo (ciclos por segundo)
};
