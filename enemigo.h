#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cmath>  // Para funciones matemáticas como sqrt()


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
    sf::Vector2f direccion; // Dirección de movimiento del enemigo
    float velocidad = 1.f;// Velocidad de movimiento del enemigo
    float velocidadCambio = 30.f; // Cada cuántos segundos cambia de dirección
    sf::Vector2f posicionInicial; // Centro del área en la que puede moverse
    float radioMovimiento = 100.f; // Radio máximo de desplazamiento
    sf::Vector2f generarDireccionAleatoria(); // Genera una dirección aleatoria de movimiento

    int currentFrame = 0;
    float frameTimer = 0.f;
    float frameTime = 0.15f;
    int frameWidth = 580;   // si cada cuadro mide 128x128
    int frameHeight = 1000;
    sf::IntRect frameActual;  // Para seleccionar cada frame del spritesheet

public:
    enemigo() {
    _salud = 500;
    _ataqueLigero = 10;
    _ataquePesado = 15;
    _habilidadEspecial = 25;

    if (!textura.loadFromFile("img/spritsheep_demonio_derecha.png")) {  // Carga la imagen completa
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
