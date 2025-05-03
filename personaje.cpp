#include "personaje.h"

    void personaje::setSalud(int salud){
    _salud = salud;
    };
    void personaje::setAtaqueLigero(int ataqueLigero){
        _ataqueLigero = ataqueLigero;
    };
    void personaje::setAtaquePesado(int ataquePesado){
    _ataquePesado = ataquePesado;
    };
    void personaje::setHabilidadEspecial(int habilidadEspecial){
    _habilidadEspacial = habilidadEspecial;
    };


    int personaje::getSalud(){
    return _salud;
    };
    int personaje::getAtaqueLigero(){
    return _ataqueLigero;
    };
    int personaje::getAtaquePesado(){
    return _ataquePesado;
    };
    int personaje::getHabilidadEspecial(){
    return _habilidadEspacial;
    };

    void personaje::mover(float offsetX, float offsetY) {
        sprite.move(offsetX, offsetY);  // Movimiento del personaje
    }

    void personaje::draw(sf::RenderWindow& window) {
        window.draw(sprite);  // Renderiza el personaje
    }
