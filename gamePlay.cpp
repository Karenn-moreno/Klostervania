#include "gamePlay.h"
#include "nuevaPartida.h"
#include "continuarPartida.h"
#include "creditos.h"
#include "record.h"
#include "batalla.h"
#include "popUpCartel.h"

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
    inicializarEnemigos();
    std::cout << "Enemigos creados: " << enemigos.size() << std::endl;
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
        if (estado == EstadoJuego::Batalla && batallaGamePlay)
        {
            if (batallaGamePlay->popupFinBatalla.handleEvent(event))
                ; // el popup se cerró con Enter
        }
    }
}

void gamePlay::updatePersonaje(sf::Time dt)
{
    float deltaTime = dt.asSeconds();

    // 0) Actualiza todos los enemigos (activos o no) para que gestionen su respawn
    for (auto* e : enemigos) {
        // Pasamos siempre false a los flags; el update interno ignora movimiento
        e->update(deltaTime, false, false, false, false);
    }

    // 1) Si el popup del ítem está abierto, salimos
    if (itemRecolectable.isPopupActive())
        return;

    // 2) Lógica de recogida de ítem
    if (estado == EstadoJuego::Exploracion &&
        itemRecolectable.tryPickup(jugador))
    {
        estado = EstadoJuego::dialogoItem;
        return;
    }

    // 3) Movimiento y animación del jugador
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
    }

    // 4) Colisión para iniciar batalla
    if (estado == EstadoJuego::Exploracion && !batallaIniciada)
    {
        for (auto* e : enemigos)
        {
            if (e->estaActivo() &&
                jugador.getBounds().intersects(e->getBounds()))
            {
                enemigoSeleccionado = e;
                estado = EstadoJuego::Batalla;
                break;
            }
        }
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

        // Dibujar todos los enemigos activos
for (auto* e : enemigos) {
    if (e->estaActivo())
        e->draw(window);
}
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
            // Sólo al arrancar la batalla:
            if (!batallaIniciada && enemigoSeleccionado)
            {
                // 1) Creamos la batalla con un sólo participante:
                std::vector<enemigo*> participantes{ enemigoSeleccionado };

                // 2) Usamos ese vector en el constructor:
                batallaGamePlay = new batalla(jugador, participantes, flecha);

                // 3) Guardamos posición, teletransportamos y arrancamos:
                posicionPreBatalla = jugador.getSprite().getPosition();
                jugador.setPosition(100.f, 630.f);
                jugador.setOrigin(0.f, 0.f);
                jugador.setScale(0.3f, 0.3f);
                // 4) Iniciamos la batalla
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
            // Para las animaciones en la batalla
            jugador.update(dt.asSeconds(), false, false, false, false);
            // Dibujo de batalla
            window.clear(sf::Color::Black);
            batallaGamePlay->drawBatalla(window);
            window.draw(pantallaNegra);
            window.display();

            // Si la batalla terminó, procesar resultado
            if (batallaGamePlay->finBatalla()) {
        if (!batallaGamePlay->popupFinBatalla.isActive()) {
            // El usuario ya pulsó Enter para cerrar el popup
            // Restaurar posición del jugador
            jugador.setPosition(posicionPreBatalla.x, posicionPreBatalla.y);
            // Limpiar la batalla
            delete batallaGamePlay;
            batallaGamePlay   = nullptr;
            batallaIniciada   = false;
            // Volver a exploración
            estado = EstadoJuego::Exploracion;
        }
            break;
            }
        default:
            break;

    }
}
}

bool gamePlay::batallaPopupActive() const {
    return batallaGamePlay && batallaGamePlay->popupFinBatalla.isActive();
}

void gamePlay::inicializarEnemigos() {
    enemigo* esqueleto = new enemigo();
    enemigos.push_back(esqueleto);

    Boss* laranas = new Boss({800.f, 200.f}, "img/spritesheet_laranas.png",{1.f, 1.f});
    enemigos.push_back(laranas);
}
