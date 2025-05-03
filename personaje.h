#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>

class personaje
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
    sf::IntRect frameActual;  // Para seleccionar cada frame del spritesheet
    float tiempoAnimacion;  // Controla la velocidad del cambio de frames
    int frameMaximo;  // N�mero total de frames de la animaci�n

public:
    personaje() {
    _salud = 1000;
    _ataqueLigero = 10;
    _ataquePesado = 15;
    _habilidadEspecial = 25;

    if (!textura.loadFromFile("img/spritesheetGuerrero.png")) {  // Carga la imagen completa
        std::cout << "Error al cargar la imagen del personaje" << std::endl;
    }
    sprite.setTexture(textura);  // Asocia la textura al sprite

    // Definimos el primer frame de la animaci�n (ejemplo: cada frame mide 64x64 p�xeles)
    frameActual = sf::IntRect(0, 0, 500, 550);  // X=0, Y=0, Ancho=500, Alto=550
    sprite.setTextureRect(frameActual);  // Aplicamos la selecci�n del primer frame
    sprite.setScale(0.25f, 0.25f); // Reduce el tama�o a la mitad
    // Variables de control para el cambio de frames
    tiempoAnimacion = 0;  // Contador inicial
    frameMaximo = 4;  // N�mero total de frames en el spritesheet

    sprite.setPosition(50, 400);  // Posici�n inicial
}

    void setSalud(int salud);
    void setAtaqueLigero(int ataqueLigero);
    void setAtaquePesado(int ataquePesado);
    void setHabilidadEspecial(int habilidadEspecial);

    int getSalud();
    int getAtaqueLigero();
    int getAtaquePesado();
    int getHabilidadEspecial();

    void update(float deltaTime, bool moviendoDer, bool moviendoIzq, bool moviendoArr, bool moviendoAbj);
    void mover(float offsetX, float offsetY);
    void detener();

    void draw(sf::RenderWindow& window);





};
