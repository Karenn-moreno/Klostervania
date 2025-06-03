#include "gamePlay.h"
#include "continuarPartida.h"
#include "creditos.h"
#include "record.h"
#include "batalla.h"
#include "popUpCartel.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <memory>
#include <algorithm>
#include "personaje.h"

// ====================================================
//  Constructor y configuración inicial de gamePlay
// ====================================================
gamePlay::gamePlay()
    : window        (sf::VideoMode(1500, 900), "KLOSTERVANIA")
    , ejecutando    (true)
    , pantallaNegra ({1500.f, 900.f})
, flecha        (bufferFlecha)
, enter         (bufferEnter)
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

    // — Fuente y menú principal —
    if (!fuente.loadFromFile("fonts/Hatch.ttf"))
        std::cout << "Error al cargar fuente del menú\n";
    menuPrincipal.crearMenu(
        numOpcionesMenuPrincipal,
        fuente,
        opcionesVector,
        40,   // tamaño del texto
        600,  // posición X
        400,  // posición Y
        55,   // separación vertical
        sf::Color::Black,
        sf::Color::Red
    );

    // — Sonidos —
    if (!bufferFlecha.loadFromFile("audio/flecha.wav") ||
            !bufferEnter.loadFromFile("audio/enter.wav"))
    {
        std::cout << "Error al cargar audio\n";
    }
    flecha.setBuffer(bufferFlecha);
    enter.setBuffer(bufferEnter);

    // — Cargar prototipos de personajes jugables —
    prototipos.reserve(4);

    // ---------- Personaje A -------------
    {
        sf::Vector2f posDummy{0.f, 0.f};
        std::string rutaA = "img/spritesheet_guerrero.png";
        sf::Vector2f escalaA = {0.3f, 0.3f};
        auto pA = std::make_shared<personaje>(posDummy, rutaA, escalaA);
        if (pA->getSprite().getTexture() != nullptr)
        {
            pA->setSalud(100);
            prototipos.push_back(pA);
            std::cout << "Agregado prototipo A correctamente.\n";
        }
        else
        {
            std::cerr << "Se omitió prototipo A porque la textura no se cargó.\n";
        }
    }

    // ---------- Personaje B -------------
    {
        sf::Vector2f posDummy{0.f, 0.f};
        std::string rutaB = "img/spritesheet_personajeB.png";
        sf::Vector2f escalaB = {0.3f, 0.3f};
        auto pB = std::make_shared<personaje>(posDummy, rutaB, escalaB);
        if (pB->getSprite().getTexture() != nullptr)
        {
            pB->setSalud(120);
            prototipos.push_back(pB);
            std::cout << "Agregado prototipo B correctamente.\n";
        }
        else
        {
            std::cerr << "Se omitió prototipo B porque la textura no se cargó.\n";
        }
    }

    // ---------- Personaje C -------------
    {
        sf::Vector2f posDummy{0.f, 0.f};
        std::string rutaC = "img/spritesheet_personajeC.png";
        sf::Vector2f escalaC = {0.3f, 0.3f};
        auto pC = std::make_shared<personaje>(posDummy, rutaC, escalaC);
        if (pC->getSprite().getTexture() != nullptr)
        {
            pC->setSalud(110);
            prototipos.push_back(pC);
            std::cout << "Agregado prototipo C correctamente.\n";
        }
        else
        {
            std::cerr << "Se omitió prototipo C porque la textura no se cargó.\n";
        }
    }

    // ---------- Personaje D -------------
    {
        sf::Vector2f posDummy{0.f, 0.f};
        std::string rutaD = "img/spritesheet_personajeD.png";
        sf::Vector2f escalaD = {0.3f, 0.3f};
        auto pD = std::make_shared<personaje>(posDummy, rutaD, escalaD);
        if (pD->getSprite().getTexture() != nullptr)
        {
            pD->setSalud(130);
            prototipos.push_back(pD);
            std::cout << "Agregado prototipo D correctamente.\n";
        }
        else
        {
            std::cerr << "Se omitió prototipo D porque la textura no se cargó.\n";
        }
    }

    // Roster inicialmente vacío; aún no hay personaje activo
    roster.clear();
    jugadorActivo = nullptr;

    // — Arrancamos el reloj de deltaTime —
    reloj.restart();
    inicializarEnemigos();
    std::cout << "Enemigos creados: " << enemigos.size() << std::endl;
}

// ====================================================
//  Procesamiento de eventos
// ====================================================
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

        // 2) Pausa tras recoger ítem
        if (estado == EstadoJuego::dialogoItem)
        {
            if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Enter)
            {
                estado = EstadoJuego::Exploracion;
            }
            continue;
        }

        // 3) Navegación del menú antes de iniciar el juego
        if (!juegoIniciado && event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                opcionSeleccionada = (opcionSeleccionada - 1 + numOpcionesMenuPrincipal)
                                     % numOpcionesMenuPrincipal;
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
                        iniciarNuevaPartida();    // Mostrar menú de selección
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
            menuPrincipal.actualizarMenu(opcionSeleccionada, sf::Color::Red, sf::Color::Black);
            continue;
        }

        // 4) Si estamos en batalla, permitir cerrar popup de fin
        if (estado == EstadoJuego::Batalla && batallaGamePlay)
        {
            if (batallaGamePlay->popupFinBatalla.handleEvent(event))
                ; // el popup se cerró con Enter
        }

        // 5) Cambiar personaje activo con la tecla T
        if (juegoIniciado && jugadorActivo && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::T)
        {
            if (roster.size() > 1)
            {
                auto it = std::find(roster.begin(), roster.end(), jugadorActivo);
                if (it != roster.end())
                {
                    size_t idx = std::distance(roster.begin(), it);
                    idx = (idx + 1) % roster.size();
                    jugadorActivo = roster[idx];
                    std::cout << "Cambiado a personaje del roster, índice: " << idx << "\n";
                }
            }
        }
    }
}

// ====================================================
//  Lógica de actualización del personaje en Exploración
// ====================================================
void gamePlay::updatePersonaje(sf::Time dt)
{
    float deltaTime = dt.asSeconds();

    // 0) Actualiza todos los enemigos para gestionar respawn
    for (auto* e : enemigos)
    {
        e->update(deltaTime, false, false, false, false);
    }

    // 1) Si el popup de ítem está abierto, no hacer nada
    if (itemRecolectable.isPopupActive())
        return;

    // 2) Detectar recogida antes de moverse
    if (estado == EstadoJuego::Exploracion && jugadorActivo)
    {
        if (itemRecolectable.tryPickup(*jugadorActivo))
        {
            estado = EstadoJuego::dialogoItem;
            return;
        }
    }
    // 3) Movimiento y animación del jugador activo
    if (juegoIniciado && jugadorActivo)
    {
        // —— Detección de game-over: si salud <= 0, mostramos pop-up de derrota:
        if (jugadorActivo->getSalud() <= 0)
        {
            mostrarGameOver();
            return;
        }
        bool movDer = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool movIzq = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool movArr = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool movAbj = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

        const float speed = 2.5f;
        if (movIzq) jugadorActivo->mover(-speed, 0.f);
        if (movDer) jugadorActivo->mover(+speed, 0.f);
        if (movArr) jugadorActivo->mover(0.f, -speed);
        if (movAbj) jugadorActivo->mover(0.f, +speed);

        jugadorActivo->update(deltaTime, movDer, movIzq, movArr, movAbj);
    }

    // 4) Colisión para iniciar batalla
    if (estado == EstadoJuego::Exploracion && !batallaIniciada && jugadorActivo)
    {
        for (auto* e : enemigos)
        {
            if (e->estaActivo() &&
                    jugadorActivo->getBounds().intersects(e->getBounds()))
            {
                enemigoSeleccionado = e;
                estado = EstadoJuego::Batalla;
                break;
            }
        }
    }
}

// ====================================================
//  Renderizado del modo Exploración y menú principal
// ====================================================
void gamePlay::drawExploracion()
{
    window.clear(sf::Color::Black);

    if (!juegoIniciado)
    {
        // — Menú Principal —
        window.draw(spriteFondo);
        menuPrincipal.dibujarMenu(window);
    }
    else
    {
        // — Partida en curso —
        window.draw(spriteNuevaPartida);

        // Dibujar todos los enemigos activos
        for (auto* e : enemigos)
        {
            if (e->estaActivo())
                e->draw(window);
        }

        // Dibujar solo el personaje activo
        if (jugadorActivo)
            jugadorActivo->draw(window);

        // Dibujar ítem si corresponde
        itemRecolectable.draw(window);
    }

    window.display();
}

// ====================================================
//  Bucle principal de ejecución
// ====================================================
void gamePlay::ejecutar()
{
    while (window.isOpen() && ejecutando)
    {
        procesarEventos();
        sf::Time dt = reloj.restart();

        switch (estado)
        {
        case EstadoJuego::Exploracion:
            updatePersonaje(dt);
            drawExploracion();
            break;

        case EstadoJuego::dialogoItem:
            drawExploracion();
            break;

        case EstadoJuego::Batalla:
            // 1) Si aún no hemos creado la batalla, la inicializamos
            if (!batallaIniciada && enemigoSeleccionado && jugadorActivo)
            {
                // Crear la instancia de batalla
                std::vector<enemigo*> participantes{ enemigoSeleccionado };
                batallaGamePlay = new batalla(*jugadorActivo, participantes, flecha);

                // Guardar posición previa y teleportar al jugador al punto de batalla
                posicionPreBatalla = jugadorActivo->getSprite().getPosition();
                jugadorActivo->setPosition(100.f, 750.f);
                //jugadorActivo->setOrigin(0.f, 0.f);

                // Iniciar la batalla (carga de texturas, menús, etc.)
                batallaGamePlay->iniciarBatalla();

                // Hacemos un fade in para la batalla
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

            // 2) Ejecución de la lógica de batalla
            if (batallaGamePlay)
                batallaGamePlay->actualizar(dt.asSeconds());

            // 3) Turno del jugador: solo si la batalla NO ha terminado
            if (batallaGamePlay && !batallaGamePlay->finBatalla())
            {
                batallaGamePlay->manejarInput();
            }

            // 4) Animación del jugador (quieto durante la batalla)
            if (jugadorActivo)
                jugadorActivo->update(dt.asSeconds(), false, false, false, false);

            // 5) Dibujar la escena de batalla (fondo, menú, sprites, fade, pop-up…)
            window.clear(sf::Color::Black);
            if (batallaGamePlay)
                batallaGamePlay->drawBatalla(window);
            window.draw(pantallaNegra);
            window.display();

            // 6) Si la batalla terminó y ya cerraron el pop-up, procesar resultado
            if (batallaGamePlay && batallaGamePlay->finBatalla() && !batallaGamePlay->popupFinBatalla.isActive())
            {
                // 6.1) Restaurar al jugador a su posición previa
                jugadorActivo->setPosition(posicionPreBatalla);

                // 6.2) Cancelar cualquier estado de ataque que pudiera quedar
                jugadorActivo->setEstado(personaje::estadoPersonaje::quieto);

                // 6.3) Determinar si fue victoria o derrota
                bool jugadorGano = batallaGamePlay->ganador();

                // 6.4) Limpiar la instancia de batalla
                delete batallaGamePlay;
                batallaGamePlay   = nullptr;
                batallaIniciada   = false;

                if (jugadorGano)
                {
                    // -------- VICTORIA --------
                    // Simplemente volvemos a Exploración
                    estado = EstadoJuego::Exploracion;
                    // Nota: dejamos juegoIniciado = true para que siga la partida
                }
                else
                {
                    // -------- DERROTA --------
                    // Volvemos al menú principal
                    juegoIniciado = false;
                    estado = EstadoJuego::Exploracion;
                }
            }
            break;

        default:
            break;
        }
    }
}


bool gamePlay::batallaPopupActive() const
{
    return batallaGamePlay && batallaGamePlay->popupFinBatalla.isActive();
}

// ====================================================
//  Inicialización de enemigos
// ====================================================
void gamePlay::inicializarEnemigos()
{
    // 1) Esqueleto
    sf::Vector2f posEsqueleto(300.f, 300.f);
    std::string rutaEsqueleto = "img/spritesheet_guerreroespejo.png";
    sf::Vector2f escalaEsq   = {0.3f, 0.3f};
    enemigo* esqueleto = new enemigo(posEsqueleto, rutaEsqueleto, escalaEsq);
    esqueleto->setSalud(50);
    enemigos.push_back(esqueleto);

    // 2) Boss “laranas”
    sf::Vector2f posLaranas(800.f, 200.f);
    std::string rutaLaranas = "img/spritesheet_laranas.png";
    sf::Vector2f escalaLar  = {0.5f, 0.5f};
    Boss* laranas = new Boss(posLaranas, rutaLaranas, escalaLar);
    laranas->setSalud(300);
    enemigos.push_back(laranas);
}

// ====================================================
//  Desbloquear nuevo personaje y mostrar pop-up
// ====================================================
void gamePlay::unlockPersonaje(int prototipoIndex)
{
    if (prototipoIndex < 0 || prototipoIndex >= static_cast<int>(prototipos.size()))
        return;

    auto nuevo = prototipos[prototipoIndex];
    for (auto& p : roster)
    {
        if (p == nuevo)
            return; // ya estaba desbloqueado
    }
    nuevo->setPosition({400.f, 400.f});  // posición de aparición
    roster.push_back(nuevo);
    popupCartel.mostrar("¡Has encontrado un nuevo personaje!", window.getSize());
}

// ====================================================
//  Mostrar el pop-up de derrota y regresar al menú
// ====================================================
void gamePlay::mostrarGameOver()
{
    // 1) Texto de derrota
    std::string textoDerrota =
        "Nuestros héroes han perdido la batalla\n"
        "El fin del mundo se acerca...";

    // 2) Mostrar el pop-up (bloqueante)
    popupCartel.mostrar(textoDerrota, window.getSize());

    // 3) Al cerrar el pop-up, volvemos al menú principal:
    juegoIniciado = false;
    estado        = EstadoJuego::Exploracion;

    // 4) Restablecer vida inicial del jugador para la próxima partida
    if (jugadorActivo)
        jugadorActivo->setSalud(100);

    // 5) Seleccionar la primera opción del menú principal
    opcionSeleccionada = 0;
    menuPrincipal.actualizarMenu(
        opcionSeleccionada,
        sf::Color::Red,
        sf::Color::Black
    );
}

