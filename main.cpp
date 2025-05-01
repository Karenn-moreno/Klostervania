#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "menu.h"

const int NUM_OPCIONES = 4;
std::string opciones[NUM_OPCIONES] = {"Nueva Partida", "Continuar Partida", "Record", "Salir"};
std::vector<std::string> opcionesVector(opciones, opciones + NUM_OPCIONES);

int main()
{
    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Klostervania");
    window.setFramerateLimit(60);

    // Cargar la textura
    sf::Texture fondoPrincipal;
    if (!fondoPrincipal.loadFromFile("img/Klostervania_fondo.jpg"))
    {
        std::cout << "Error al cargar la imagen" << std::endl;
        return -1;
    }
    sf::Sprite Fondo;
    Fondo.setTexture(fondoPrincipal);
    Fondo.setScale({1.6f, 1.2f}); //  factor de escala

    //cargo sonidos
    sf::SoundBuffer _Flecha;
        if (!_Flecha.loadFromFile("audio/flecha.wav"))
    {
        std::cout << "Error al cargar audio de flecha" << std::endl;
        return -1;
    }
    sf::Sound Flecha;
    Flecha.setBuffer(_Flecha);

    sf::SoundBuffer _Enter;
        if (!_Enter.loadFromFile("audio/enter.wav"))
    {
        std::cout << "Error al cargar audio de enter" << std::endl;
        return -1;
    }
    sf::Sound Enter;
    Enter.setBuffer(_Enter);

    //Cargo la fuente y color
    sf::Font Texto1;
    if(!Texto1.loadFromFile("fonts/Hatch.ttf"))
    {
        std::cout << "Error al cargar en texto nueva partida" << std::endl;
        return EXIT_FAILURE;
    }

    //armamos el menu dinamico
    menu menuPrincipal;
    menuPrincipal.crearMenu(NUM_OPCIONES, Texto1, opcionesVector, 40, 600, 550, 60, sf::Color::Red);

    //sf::Text textos[NUM_OPCIONES];
    int opcionSeleccionada = 0;

    // Bucle principal de la ventana
    while (window.isOpen())
    {
        //  Revisa todos los eventos de la ventana que se activaron desde la última iteración del bucle.
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)   // Aquí dentro es correcto
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    opcionSeleccionada = (opcionSeleccionada - 1 + NUM_OPCIONES) % NUM_OPCIONES;
                    Flecha.play();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    opcionSeleccionada = (opcionSeleccionada + 1) % NUM_OPCIONES;
                    Flecha.play();
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    Enter.play();
                    std::cout << "Has seleccionado: " << opciones[opcionSeleccionada] << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    window.close();
                }
                menuPrincipal.actualizarMenu(opcionSeleccionada, sf::Color::White, sf::Color::Red);
            }
        }

        // limpio la ventana con color negro
        window.clear(sf::Color::Black);

        // Dibujando todo
        window.draw(Fondo);
        menuPrincipal.dibujarMenu(window);

        // Mostrar el contenido de la ventana
        window.display();

    }
    return 0;
}



























