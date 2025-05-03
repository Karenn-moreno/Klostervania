#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class personaje
{

private:
    int _salud;
    int _ataqueLigero;
    int _ataquePesado;
    int _habilidadEspacial;
    sf::Sprite sprite;
    sf::Texture textura;



public:
    personaje(){
        _salud=1000;
        _ataqueLigero=10;
        _ataquePesado=15;
        _habilidadEspacial=25;
        sf::Sprite sprite;
        sf::Texture textura;

        // Cargar la textura al crear el objeto
        if (!textura.loadFromFile("img/personaje.png")) {
            std::cout << "Error al cargar la imagen del personaje" << std::endl;
        }
        sprite.setTexture(textura);
        sprite.setPosition(200, 400);  // Posición inicial
        };

    void setSalud(int salud);
    void setAtaqueLigero(int ataqueLigero);
    void setAtaquePesado(int ataquePesado);
    void setHabilidadEspecial(int habilidadEspecial);

    int getSalud();
    int getAtaqueLigero();
    int getAtaquePesado();
    int getHabilidadEspecial();

    void mover(float offsetX, float offsetY);

    void draw(sf::RenderWindow& window);






};
