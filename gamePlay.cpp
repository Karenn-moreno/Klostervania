#include "gamePlay.h"
#include "nuevaPartida.h"
#include "continuarPartida.h"
#include "creditos.h"
#include "record.h"
#include "batalla.h"


gamePlay::gamePlay()
    : window            (sf::VideoMode(1500, 900), "KLOSTERVANIA")
    , ejecutando        (true)
    , pantallaNegra     ({1500.f, 900.f})
, flecha            (bufferFlecha)
, enter             (bufferEnter)
{
    window.setFramerateLimit(60);

    // — Fondos —
    if (!fondoPrincipal.loadFromFile("img/Klostervania_fondo.jpg"))
        std::cout << "Error al cargar fondo principal\n";
    spriteFondo.setTexture(fondoPrincipal);
    spriteFondo.setScale(1.5f, 1.1f);

    if (!fondoNuevaPartida.loadFromFile("img/fondonuevaPartida.jpg"))
        std::cout << "Error al cargar fondo nueva partida\n";
    spriteNuevaPartida.setTexture(fondoNuevaPartida);
    spriteNuevaPartida.setScale(2.0f, 2.0f);

    // — Transición —
    pantallaNegra.setFillColor(sf::Color(0,0,0,255));

    // — Fuente y menú —
    if (!fuente.loadFromFile("fonts/Hatch.ttf"))
        std::cout << "Error al cargar fuente del menú\n";
    menuPrincipal.crearMenu( numOpcionesMenuPrincipal, fuente, opcionesVector, 40, 600, 400, 55, sf::Color::Black, sf::Color::Red);

    // — Sonidos —
    if (!bufferFlecha.loadFromFile("audio/flecha.wav") ||
            !bufferEnter .loadFromFile("audio/enter.wav"))
    {
        std::cout << "Error al cargar audio\n";
    }
    flecha.setBuffer(bufferFlecha);
    enter.setBuffer(bufferEnter);

    // — Arrancamos el reloj de deltaTime —
    reloj.restart();
}

void gamePlay::procesarEventos()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // 1) Salida de la aplicación
        if (event.type == sf::Event::Closed)
        {
            ejecutando = false;
            window.close();
        }

        // 2) Navegación del menú antes de iniciar el juego
        if (!juegoIniciado && event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                opcionSeleccionada = (opcionSeleccionada - 1 + numOpcionesMenuPrincipal) % numOpcionesMenuPrincipal;
                flecha.play();
                break;
            case sf::Keyboard::Down:
                opcionSeleccionada = (opcionSeleccionada + 1) % numOpcionesMenuPrincipal;
                flecha.play();
                break;
            case sf::Keyboard::Enter:
                enter.play();
                switch (opcionSeleccionada)
                {
                case 0:  // Nueva Partida
                    std::cout<<"\nIniciando una nueva partida";
                    juegoIniciado = true;
                    {
                        //  transicion de pantalla
                        int opacidad = 255;
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

                case 1: //  continuar partida
                    std::cout<<"\nEntrando a Continuar partida";
                    continuarPartida();

                    break;

                case 2: //  record
                    std::cout<<"\nEntrando a records";
                    record();

                    break;

                case 3: //  Creditos
                    std::cout<<"\nEntrando a los creditos";
                    creditos();

                    break;

                case 4:  // Salir
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    ejecutando = false;
                    window.close();
                    break;
                default:

                    break;
                }

                break;
            default:
                break;
            }
            // Actualiza colores del menú tras pulsar Enter
            menuPrincipal.actualizarMenu(opcionSeleccionada, sf::Color::Red, sf::Color::Black);
        }
    }
}

void gamePlay::updatePersonaje(sf::Time dt)
{
    float deltaTime = dt.asSeconds();

    if (juegoIniciado)
    {
//  Movimiento continuo —
        bool movDer = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool movIzq = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool movArr = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool movAbj = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        const float speed = 2.5f;
        if (movIzq) jugador.mover(-speed, 0.f);
        if (movDer) jugador.mover( speed, 0.f);
        if (movArr) jugador.mover(0.f, -speed);
        if (movAbj) jugador.mover(0.f,  speed );

//   Actualización de entidades —
        jugador.update(deltaTime, movDer, movIzq, movArr, movAbj);
        demonio.update(deltaTime);

        itemRecolectable.update();
        if (!itemRecolectable.isActive())
            itemRecolectable.spawn(window.getSize());
    }

//  Detectar colisión solo en exploración
    if (estado == EstadoJuego::Exploracion && !batallaIniciada)
    {
        if (jugador.getBounds().intersects(demonio.getBounds()))
        {
            std::cout<<"\nSe choco un enemigo!";
            estado = EstadoJuego::Batalla;
        }
    }
}

void gamePlay::drawExploracion()
{
    window.clear(sf::Color::Black);

    if (!juegoIniciado)
    {
// Menú principal
        window.draw(spriteFondo);
        menuPrincipal.dibujarMenu(window);
    }
    else
    {
// Mundo: fondo de partida y entidades
        window.draw(spriteNuevaPartida);
        demonio.draw(window);
        jugador.draw(window);
        itemRecolectable.draw(window);
    }

    window.display();
}

void gamePlay::ejecutar()
{
    while (window.isOpen() && ejecutando)
    {
        procesarEventos();
        sf::Time dt = reloj.restart();

        if (estado == EstadoJuego::Exploracion)
        {
            updatePersonaje(dt);
            drawExploracion();
        }
        else if (estado == EstadoJuego::Batalla)
        {
            if (!batallaIniciada)
            {
                std::cout << "\nSe inició una batalla";
                batallaIniciada = true;
                batallaGamePlay = new batalla(jugador, demonio, flecha);
                batallaGamePlay->iniciarBatalla();
            }
            // ————— transicion de pantalla  —————
        for (int opacidad = 255; opacidad >= 0; opacidad -= 5) {
        pantallaNegra.setFillColor(sf::Color(0, 0, 0, opacidad));
        window.clear();
            // ————— DIBUJO DE BATALLA —————

            batallaGamePlay->drawBatalla(window);
            window.draw(pantallaNegra);
            window.display();
        }
            if (batallaGamePlay->finBatalla())
            {
                delete batallaGamePlay;
                batallaGamePlay   = nullptr;
                batallaIniciada   = false;
                estado            = EstadoJuego::Exploracion;
            }
        }
    }
};
