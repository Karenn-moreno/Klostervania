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
        itemRecolectable.handleEvent(event);
        // 2) Pausa tras recoger ítem (RecompensaItem)
        if (estado == EstadoJuego::dialogoItem)
        {
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter)
            {
                estado = EstadoJuego::Exploracion;
            }
            // No procesar más eventos mientras esté en pausa
            continue;
        }

        // 3) Navegación del menú antes de iniciar el juego
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
                            std::cout << "\nIniciando una nueva partida";
                            juegoIniciado = true;
                            {
                                // Transición de pantalla
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
                        case 1:  // Continuar Partida
                            std::cout << "\nEntrando a Continuar partida";
                            continuarPartida();
                            break;
                        case 2:  // Record
                            std::cout << "\nEntrando a records";
                            record();
                            break;
                        case 3:  // Créditos
                            std::cout << "\nEntrando a los creditos";
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
            // Actualiza colores del menú tras Up/Down/Enter
            menuPrincipal.actualizarMenu(opcionSeleccionada, sf::Color::Red, sf::Color::Black);
            continue;
        }
    }
}


void gamePlay::updatePersonaje(sf::Time dt)
{
    float deltaTime = dt.asSeconds();

    // 1) Si el panel está abierto, no hacemos nada más
    if (itemRecolectable.isPanelActive())
        return;

    // 2) Actualizamos el ítem (respawn, pulso…)
    itemRecolectable.update();

    // 3) Detectar recogida: SIEMPRE antes de mover o iniciar batalla
    if (estado == EstadoJuego::Exploracion &&
        itemRecolectable.tryPickup(jugador))
    {
        estado = EstadoJuego::dialogoItem;
        return;  // salimos sin tocar nada más
    }

    // 4) Movimiento y animación del jugador
    if (juegoIniciado)
    {
        bool movDer = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool movIzq = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool movArr = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool movAbj = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        const float speed = 2.5f;
        if (movIzq) jugador.mover(-speed, 0.f);
        if (movDer) jugador.mover( speed, 0.f);
        if (movArr) jugador.mover(0.f, -speed);
        if (movAbj) jugador.mover(0.f,  speed);

        jugador.update(deltaTime, movDer, movIzq, movArr, movAbj);
        demonio.update(deltaTime);

        if (!itemRecolectable.isActive())
            itemRecolectable.spawn(window.getSize());
    }

    // 5) Colisión para iniciar batalla
    if (estado == EstadoJuego::Exploracion && !batallaIniciada &&
        demonio.estaActivo() &&
        jugador.getBounds().intersects(demonio.getBounds()))
    {
        estado = EstadoJuego::Batalla;
    }
}


void gamePlay::drawExploracion()
{
    window.clear(sf::Color::Black);

    // Si no ha iniciado el juego, muestra menú
    if (!juegoIniciado)
    {
        window.draw(spriteFondo);
        menuPrincipal.dibujarMenu(window);
    }
    else
{
        // Fondo de nivel
        window.draw(spriteNuevaPartida);

        // Entidades (si el demonio sigue activo)
        if (demonio.estaActivo())
            demonio.draw(window);

        // Jugador siempre
        jugador.draw(window);

        // Ítem texto

    itemRecolectable.draw(window);
}
    window.display();
}


void gamePlay::ejecutar()
{
    while (window.isOpen() && ejecutando)
    {
        // Captura y maneja eventos (incluye pausa por recompensa)
        procesarEventos();

        // Tiempo delta
        sf::Time dt = reloj.restart();

        // Dispara la lógica según el estado del juego
        switch (estado)
        {
            case EstadoJuego::Exploracion:
                // Exploración normal
                updatePersonaje(dt);
                drawExploracion();
                break;

            case EstadoJuego::dialogoItem:
                // Pausa tras recoger ítem: dibuja sólo el paisaje o futura ventana de recompensa
                drawExploracion();
                break;

            case EstadoJuego::Batalla:
                // Iniciar batalla si aún no lo hemos hecho
                if (!batallaIniciada)
                {
                    std::cout << "\nSe inició una batalla";
                    batallaGamePlay = new batalla(jugador, demonio, flecha);
                    batallaGamePlay->iniciarBatalla();
                    // Transición de pantalla
                    for (int opacidad = 255; opacidad >= 0; opacidad -= 5)
                    {
                        pantallaNegra.setFillColor(sf::Color(0, 0, 0, opacidad));
                        window.clear();
                        batallaGamePlay->drawBatalla(window);
                        window.draw(pantallaNegra);
                        window.display();
                    }
                    batallaIniciada = true;
                }

                // Turno del jugador
                batallaGamePlay->manejarInput();
                // Turno del enemigo
                batallaGamePlay->actualizar(dt.asSeconds());
                // Dibujo de batalla
                window.clear(sf::Color::Black);
                batallaGamePlay->drawBatalla(window);
                window.draw(pantallaNegra);
                window.display();

                // Si la batalla terminó, procesar resultado
                if (batallaGamePlay->finBatalla())
                {
                    if (batallaGamePlay->ganador())
                    {
                        std::cout << "\n¡Has vencido al enemigo!\n";
                        demonio.setActivo(false);
                    }
                    else
                    {
                        std::cout << "\nHas sido derrotado…\n";
                    }

                    // Limpieza de la batalla
                    delete batallaGamePlay;
                    batallaGamePlay = nullptr;
                    batallaIniciada = false;

                    // Volver a exploración
                    estado = EstadoJuego::Exploracion;
                }
                break;

            default:
                break;
        }
    }
}

