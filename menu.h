#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class menu {

private:
    int _numeroOpciones;
    sf::Font _fuente;
    int _opcionSeleccionada;
    std::vector<std::string> _opciones;  // Cambio de array estático a vector dinámico
    std::vector<sf::Text> textos;

public:

    Menu(int numeroOpciones, const sf::Font& fuente, const std::vector<std::string>& opciones);  // Constructor

void crearMenu(int numeroOpciones, const sf::Font& fuente, const std::vector<std::string>& opciones, int sizeChar, int posX, int posY, int interlineado, sf::Color color, sf::Color colorInicial);

void actualizarMenu(int opcionSeleccionada, sf::Color colorNuevo, sf::Color color);

void dibujarMenu(sf::RenderWindow& window);


};
