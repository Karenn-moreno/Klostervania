#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class enemigo
{

private:
    int _salud;
    int _ataqueLigero;
    int _ataquePesado;
    int _habilidadEspecial;
    //para cargar la imagen del personaje
    sf::Sprite sprite;
    sf::Texture textura;
    //para animar el persoaje al mover
    float tiempoDesdeUltimoMovimiento = 0.f;//acumulador de tiempo para volver a moverme
    float tiempoAnimacion = 0.f;
    const int totalFrames = 3;
    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameTime = 0.15f;
    int frameWidth = 575;
    int frameHeight = 1000;
    sf::IntRect frameActual;  // Para seleccionar cada frame del spritesheet

public:
    enemigo()
    {
        _salud = 500;
        _ataqueLigero = 10;
        _ataquePesado = 15;
        _habilidadEspecial = 25;

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

    void setSalud(int salud);
    void setAtaqueLigero(int ataqueLigero);
    void setAtaquePesado(int ataquePesado);
    void setHabilidadEspecial(int habilidadEspecial);

    int getSalud();
    int getAtaqueLigero();
    int getAtaquePesado();
    int getHabilidadEspecial();

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};
