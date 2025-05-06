#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "menu.h"
#include "nuevaPartida.h"
#include "personaje.h"
#include "enemigo.h"



int main()
{
    personaje personaje;
    enemigo demonio;
    const int NUM_OPCIONES = 5;
    std::string opciones[NUM_OPCIONES] = {"Nueva Partida", "Continuar Partida", "Record", "creditos", "Salir"};
    std::vector<std::string> opcionesVector(opciones, opciones + NUM_OPCIONES);
    sf::RectangleShape pantallaNegra(sf::Vector2f(1500, 900));
    pantallaNegra.setFillColor(sf::Color(0, 0, 0, 255)); // Negro con opacidad total

    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Klostervania");
    window.setFramerateLimit(60);

    // Cargar la textura menu principal
    sf::Texture fondoPrincipal;
    if (!fondoPrincipal.loadFromFile("img/Klostervania_fondo.jpg"))
    {
        std::cout << "Error al cargar la imagen" << std::endl;
        return -1;
    }
    sf::Sprite Fondo;
    Fondo.setTexture(fondoPrincipal);
    Fondo.setScale({1.5f, 1.1f}); //  factor de escala

    // Cargar la textura nueva partida
    sf::Texture fondoNuevaPartida;
    if (!fondoNuevaPartida.loadFromFile("img/fondonuevaPartida.jpg"))
    {
        std::cout << "Error al cargar la imagen" << std::endl;
        return -1;
    }
    sf::Sprite FondoNuevaPartida;
    FondoNuevaPartida.setTexture(fondoNuevaPartida);
    FondoNuevaPartida.setScale({2.0f, 2.0f});

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
    menuPrincipal.crearMenu(NUM_OPCIONES, Texto1, opcionesVector, 40, 600, 400, 55, sf::Color::Black, sf::Color::Red);

    //sf::Text textos[NUM_OPCIONES];
    int opcionSeleccionada = 0;

    // Bucle principal de la ventana
    sf::Clock clock;
    sf::Clock clockEnemigos; // Declarar el reloj una vez, antes del loop principal
    bool juegoIniciado = false;  //variable para saber si sigo en menu principal
    while (window.isOpen())
    {
        //  Revisa todos los eventos de la ventana que se activaron desde la última iteración del bucle.
        sf::Event event;
        sf::Time elapsed = clockEnemigos.restart();
        float deltaTime = elapsed.asSeconds(); // Delta Time en segundos

        demonio.update(deltaTime);
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            else if (event.type == sf::Event::KeyPressed)
                if (!juegoIniciado)
                {
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
                            std::cout << "opcion numero " << opcionSeleccionada << std::endl;
                            int opacidad = 255;
                            switch (opcionSeleccionada)
                            {
                            case 0:  // "Nueva Partida"
                                std::cout << "Entrando al menú de nueva partida..." << std::endl;
                                juegoIniciado = true;  //Ahora el menú no volverá a dibujarse
                                while (opacidad > 0)  //transicion de pantalla
                                {
                                    pantallaNegra.setFillColor(sf::Color(0, 0, 0, opacidad));
                                    window.clear();
                                    window.draw(FondoNuevaPartida);
                                    window.draw(pantallaNegra);
                                    window.display();
                                    opacidad -= 8;
                                    sf::sleep(sf::milliseconds(100));
                                }
                                nuevaPartida();

                                break;

                            case 1:  // "continuarPartida"
                                std::cout << "Entrando al menú de continuar partida..." << std::endl;
                                break;

                            case 2:  // "record"
                                std::cout << "Entrando al menú de records..." << std::endl;
                                break;

                            case 3:  // "creditos"
                                std::cout << "Entrando al menú de creditos..." << std::endl;
                                break;

                            case 4:  // "salir"
                                std::cout << "entrando al menu salir..." << std::endl;
                                std::this_thread::sleep_for(std::chrono::seconds(1));
                                window.close();  //Cerrar la aplicación
                                break;

                            default:
                                std::cout << "Opción no válida." << std::endl;
                            }
                        }
                        menuPrincipal.actualizarMenu(opcionSeleccionada, sf::Color::Red, sf::Color::Black);
                    }
                }
                else
                {
                    float deltaTime = clock.restart().asSeconds();
                    bool moviendoArr = false;
                    bool moviendoAbj = false;
                    bool moviendoDer = false;
                    bool moviendoIzq = false;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        personaje.mover(-5, 0);
                        moviendoIzq = true;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        personaje.mover(5, 0);
                        moviendoDer = true;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        personaje.mover(0, -5);
                        moviendoArr = true;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        personaje.mover(0, 5);
                        moviendoAbj = true;
                    }

                    // Llamamos a `update()` en cada ciclo del juego
                    personaje.update(deltaTime, moviendoDer, moviendoIzq, moviendoArr, moviendoAbj);
                }
        }

        // limpio la ventana con color negro
        window.clear(sf::Color::Black);

        // Dibujando todo
        if (!juegoIniciado)
        {
            window.draw(Fondo);
            menuPrincipal.dibujarMenu(window);  //Solo dibuja el menú si el juego no ha empezado
        }
        else
        {
            window.draw(FondoNuevaPartida);
            demonio.draw(window);
            personaje.draw(window);
        }
        // Mostrar el contenido de la ventana
        window.display();

    }
    return 0;
}



























