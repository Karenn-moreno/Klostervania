#include "menu.h"
#include<iostream>

menu::menu(){};
void menu::crearMenu(int numeroOpciones, const sf::Font& fuente, const std::vector<std::string>& opciones, int sizeChar, int posX, int posY, int interlineado, sf::Color color, sf::Color colorInicial)
{
    _numeroOpciones = numeroOpciones;
    _fuente = fuente;
    _opciones = opciones;

    textos.clear();  // Limpia el vector antes de agregar nuevos textos

    for (int i = 0; i < numeroOpciones; i++)
    {
        sf::Text texto;
        texto.setFont(_fuente);
        texto.setString(_opciones[i]);
        texto.setCharacterSize(sizeChar);
        texto.setPosition(posX, posY + i * interlineado);
        if (i==0){
        texto.setFillColor(colorInicial);
        }
        else{
        texto.setFillColor(color);
        }
        textos.push_back(texto);  // Agrega el objeto `sf::Text` al vector
    }
    std::cout <<"Crear Menu de " <<numeroOpciones << " opciones\n";
    for (int i = 0; i < _numeroOpciones; i++)
    {
        std::cout << i+1 << "- " << _opciones[i] << std::endl;
    }
}

void menu::actualizarMenu(int opcionSeleccionada, sf::Color colorNuevo, sf::Color color)
{
    for (int i = 0; i < _numeroOpciones; i++)
    {
        if (i == opcionSeleccionada)
        {
            textos[i].setFillColor(colorNuevo); // Resaltar opción seleccionada
        }
        else
        {
            textos[i].setFillColor(color);
        }
    }
    std::cout<< "se presiono una tecla flecha, actualizo los colore del menu" <<std::endl;
}

void menu::dibujarMenu(sf::RenderWindow& window)
{
    for (int i = 0; i < _numeroOpciones; i++)
    {
        window.draw(textos[i]);
    }
};

