#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "menu.h"
#include "nuevaPartida.h"
#include "personaje.h"
#include "enemigo.h"
#include "item.h"

int main()
{
    // Instancias de objetos de juego
    personaje miPersonaje;
    enemigo demonio;
    item miItem;

    // Opciones del menú
    const int NUM_OPCIONES = 5;
    std::string opciones[NUM_OPCIONES] = {"Nueva Partida", "Continuar Partida", "Record", "Creditos", "Salir"};
    std::vector<std::string> opcionesVector(opciones, opciones + NUM_OPCIONES);

    // Forma y color de transición
    sf::RectangleShape pantallaNegra(sf::Vector2f(1500, 900));
    pantallaNegra.setFillColor(sf::Color(0, 0, 0, 255));

    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Klostervania");
    window.setFramerateLimit(60);

    // Cargar fondos
    sf::Texture fondoPrincipal;
    if (!fondoPrincipal.loadFromFile("img/Klostervania_fondo.jpg"))
    {
        std::cout << "Error al cargar fondo principal" << std::endl;
        return -1;
    }
    sf::Sprite spriteFondo(fondoPrincipal);
    spriteFondo.setScale(1.5f, 1.1f);

    sf::Texture fondoNuevaPartida;
    if (!fondoNuevaPartida.loadFromFile("img/fondonuevaPartida.jpg"))
    {
        std::cout << "Error al cargar fondo nueva partida" << std::endl;
        return -1;
    }
    sf::Sprite spriteNuevaPartida(fondoNuevaPartida);
    spriteNuevaPartida.setScale(2.0f, 2.0f);

    // Cargar sonidos
    sf::SoundBuffer bufferFlecha, bufferEnter;
    if (!bufferFlecha.loadFromFile("audio/flecha.wav") || !bufferEnter.loadFromFile("audio/enter.wav"))
    {
        std::cout << "Error al cargar audio" << std::endl;
        return -1;
    }
    sf::Sound flecha(bufferFlecha);
    sf::Sound enter(bufferEnter);

    // Cargar fuente
    sf::Font fuente;
    if (!fuente.loadFromFile("fonts/Hatch.ttf"))
    {
        std::cout << "Error al cargar fuente" << std::endl;
        return -1;
    }

    // Configurar menú
    menu menuPrincipal;
    menuPrincipal.crearMenu(NUM_OPCIONES, fuente, opcionesVector,
                            40, 600, 400, 55,
                            sf::Color::Black, sf::Color::Red);

    int opcionSeleccionada = 0;
    bool juegoIniciado = false;

    sf::Clock clock; // Para deltaTime

    // Bucle principal
    while (window.isOpen())
    {
        // 1) Procesar eventos
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (!juegoIniciado)
            {
                // Navegación del menú
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        opcionSeleccionada = (opcionSeleccionada - 1 + NUM_OPCIONES) % NUM_OPCIONES;
                        flecha.play();
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        opcionSeleccionada = (opcionSeleccionada + 1) % NUM_OPCIONES;
                        flecha.play();
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        enter.play();
                        std::cout << "Has seleccionado: " << opciones[opcionSeleccionada] << std::endl;
                        std::cout << "opcion numero " << opcionSeleccionada << std::endl;
                        int opacidad = 255;
                        switch (opcionSeleccionada)
                        {
                        case 0: // Nueva Partida
                            std::cout << "Entrando al menú de nueva partida..." << std::endl;
                            juegoIniciado = true;       //ahora el menu no vuelve a mostrarse
                            {
                                int opacidad = 255;     //transicion de pantalla
                                while (opacidad > 0)
                                {
                                    pantallaNegra.setFillColor(sf::Color(0, 0, 0, opacidad));
                                    window.clear();
                                    window.draw(spriteNuevaPartida);
                                    window.draw(pantallaNegra);
                                    window.display();
                                    opacidad -= 8;
                                    sf::sleep(sf::milliseconds(100));
                                }
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

                        case 4: // Salir
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            window.close();
                            break;
                        default:
                            // Otras opciones (Continuar, Record, Creditos)
                            break;
                        }
                    }
                    menuPrincipal.actualizarMenu(opcionSeleccionada, sf::Color::Red, sf::Color::Black);
                }
            }
        }

        // 2) Calcular deltaTime
        float deltaTime = clock.restart().asSeconds();

        // 3) Actualizaciones de juego
        if (juegoIniciado)
        {
            // Movimiento continuo basado en teclas
            bool movDer = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            bool movIzq = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            bool movArr = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            bool movAbj = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

            if (movIzq) miPersonaje.mover(-3.f, 0.f);
            if (movDer) miPersonaje.mover( 3.f, 0.f);
            if (movArr) miPersonaje.mover(0.f, -3.f);
            if (movAbj) miPersonaje.mover(0.f,  3.f);

            miPersonaje.update(deltaTime, movDer, movIzq, movArr, movAbj);

            demonio.update(deltaTime);

            miItem.update();
            if (!miItem.isActive())
            {
                miItem.spawn(window.getSize());
            }
        }

        // 4) Dibujar
        window.clear(sf::Color::Black);
        if (!juegoIniciado)
        {
            window.draw(spriteFondo);
            menuPrincipal.dibujarMenu(window);
        }
        else
        {
            window.draw(spriteNuevaPartida);
            demonio.draw(window);
            miPersonaje.draw(window);
            miItem.draw(window);
        }
        window.display();
    }

    return 0;
}
