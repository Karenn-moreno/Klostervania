#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class enemigo
{
public:

    enemigo();

    // --- Setters de estad�sticas ---
    void setSalud(int salud);               //  Establece los puntos de vida del enemigo
    void setAtaqueLigero(int ataqueLigero); //   Establece el da�o de ataque ligero
    void setAtaquePesado(int ataquePesado); //   Establece el da�o de ataque pesado
    void setHabilidadEspecial(int habilidadEspecial); //     Establece el poder de la habilidad especial

    // --- Getters de estad�sticas ---
    void setActivo(bool activo);//   Lo creo y lo elimino con esto
    int getSalud();            //    Devuelve los puntos de vida actuales
    int getAtaqueLigero();     //    Devuelve el da�o de ataque ligero
    int getAtaquePesado();     //    Devuelve el da�o de ataque pesado
    int getHabilidadEspecial();//    Devuelve el poder de la habilidad especial
    const sf::Sprite& getSprite() const;   // Devuelve el sprite para poder consultar sus bounds o dibujarlo
    sf::FloatRect getBounds() const;
        void setPosition(float x, float y);
    int  _maxSalud;            //   Puntos de vida maximo
    // --- L�gica de juego ---
    void update(float deltaTime);
    bool estaActivo() const;
    void draw(sf::RenderWindow& window);

private:
    // --- Estad�sticas del enemigo ---
    int _salud;               //    Puntos de vida

    int _ataqueLigero;        //    Da�o de ataques ligeros
    int _ataquePesado;        //    Da�o de ataques pesados
    int _habilidadEspecial;   //    Poder de habilidad especial
    bool _activo = true;  // empieza vivo

    int  getMaxSalud() const;
     // Para que reviva
  sf::Clock _respawnClock;
  sf::Time  _respawnDelay = sf::seconds(10.0f); // 0.5 minutos = 30 s
  sf::Vector2f _posInicial;                   // guarda d�nde naci�

    // --- Gr�ficos ---
    sf::Sprite sprite;        //    Sprite que representa al enemigo
    sf::Texture textura;      //    Textura del spritesheet completo

    // --- Control de movimiento y animaci�n ---
    float tiempoDesdeUltimoMovimiento = 0.f; // Acumula tiempo para elegir movimiento aleatorio
    float tiempoAnimacion = 0.f;             // (no usado directamente) acumulador general

    const int totalFrames = 3;  //  N�mero de frames en la animaci�n de caminar
    int currentFrame = 0;       //  Frame actual mostrado
    float frameTimer = 0.f;     //  Acumula tiempo para cambio de frame
    float frameTime = 0.15f;    //  Duraci�n de cada frame (segundos)
    int frameWidth = 575;       //  Ancho de cada frame (p�xeles)
    int frameHeight = 1000;     //  Alto de cada frame (p�xeles)
    sf::IntRect frameActual;    //  Rect�ngulo que define el frame en la textura
};
