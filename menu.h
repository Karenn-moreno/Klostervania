#pragma once

#include <vector>            // Para std::vector
#include <string>            // Para std::string
#include <SFML/Graphics.hpp> // Para sf::Font, sf::Text, sf::RenderWindow

class menu {

private:
    // --- Datos del menú ---
    int _numeroOpciones;                     // Número total de opciones
    int _opcionSeleccionada;                 // Índice de la opción actualmente activa
    sf::Font _fuente;                        // Fuente utilizada para los textos
    std::vector<std::string> _opciones;      // Textos de cada opción
    std::vector<sf::Text> textos;            // Objetos de texto para renderizar cada opción

public:

    menu ();

    void crearMenu(int numeroOpciones,
                   const sf::Font& fuente,
                   const std::vector<std::string>& opciones,
                   int sizeChar,
                   int posX,
                   int posY,
                   int interlineado,
                   sf::Color color,
                   sf::Color colorInicial);

    void actualizarMenu(int opcionSeleccionada,
                       sf::Color colorNuevo,
                       sf::Color color);

    void dibujarMenu(sf::RenderWindow& window);

    void setOptionColor(int index, sf::Color color);
};
