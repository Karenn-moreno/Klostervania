#include "gamePlay.h"
#include "batalla.h"
#include "popUpCartel.h"
#include <thread>// borrarrrrrrrrrrrrrrrrrrr
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
    , castle        (buffercastle)
    , battle        (bufferbattle)
{
    window.setFramerateLimit(60);
    pantallaNegra.setFillColor(sf::Color(0, 0, 0, 255)); // alpha=255 al inicio
    alphaFade = 255.f;
    // — Fondos —
    if (!fondoPrincipal.loadFromFile("img/Klostervania_fondo.jpg"))
        std::cout << "Error al cargar fondo principal\n";
    spriteFondo.setTexture(fondoPrincipal);
    spriteFondo.setScale(1.5f, 1.1f);

    if (!fondoNuevaPartida.loadFromFile("img/mapa.png"))
        std::cout << "Error al cargar mapa\n";
    spriteNuevaPartida.setTexture(fondoNuevaPartida);
    vista.setSize(400.f, 300.f);  // Tamaño de la vista

    mascaraColision.loadFromFile("img/mapa_colisiones_escalado.png");
    std::cout << "Intentando cargar máscara de colisión...\n";
if (!mascaraColision.loadFromFile("img/mapa_colisiones.png")) {
    std::cerr << "ERROR: No se pudo cargar la máscara de colisión\n";
} else {
    std::cout << "Máscara de colisión cargada correctamente\n";
}

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
            !bufferEnter.loadFromFile("audio/enter.wav")||
            !buffercastle.loadFromFile("audio/castle.wav")||
            !bufferbattle.loadFromFile("audio/battle.wav"));
    {
        std::cout << "Error al cargar audio\n";
    }
    flecha.setBuffer(bufferFlecha);
    enter.setBuffer(bufferEnter);
    castle.setBuffer(buffercastle);
    battle.setBuffer(bufferbattle);

    ///MODULAR VOLUMEN
    castle.setVolume(15.f);
    battle.setVolume(20.f);

    // — Arrancamos el reloj de deltaTime —
    reloj.restart();
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
                    iniciarNuevaPartida();

                    break;
                case 1:  // Continuar Partida
                    std::cout << "\nEntrando a Continuar partida";
                   // continuarPartida();
                    break;
                case 2:  // Record
                    std::cout << "\nEntrando a records";
                   // record();
                    break;
                case 3:  // Créditos
                    std::cout << "\nEntrando a los creditos";
                   // creditos();
                    break;
                case 4:  // Salir
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
    }
}

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

        bool movDer = false;
bool movIzq = false;
bool movArr = false;
bool movAbj = false;

const float speed = 2.5f;
sf::FloatRect bounds = jugadorActivo->getBounds();

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    sf::FloatRect derecha = bounds;
    derecha.left += speed;
    if (esZonaLibre(derecha))
        movDer = true;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    sf::FloatRect izquierda = bounds;
    izquierda.left -= speed;
    if (esZonaLibre(izquierda))
        movIzq = true;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    sf::FloatRect arriba = bounds;
    arriba.top -= speed;
    if (esZonaLibre(arriba))
        movArr = true;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    sf::FloatRect abajo = bounds;
    abajo.top += speed;
    if (esZonaLibre(abajo))
        movAbj = true;
}

        jugadorActivo->update(deltaTime, movDer, movIzq, movArr, movAbj);
        vista.setCenter(jugadorActivo->getPosition());

        // — Limitar la vista a los bordes del fondo escalado —
        sf::Vector2f centro = vista.getCenter();
        sf::Vector2f tam = vista.getSize();

        // Obtener tamaño real del fondo con escalado aplicado
        sf::FloatRect fondoBounds = spriteNuevaPartida.getGlobalBounds();
        float mapaAncho = fondoBounds.width;
        float mapaAlto  = fondoBounds.height;

        float halfWidth = tam.x / 2.f;
        float halfHeight = tam.y / 2.f;

        // Limitar en X
        if (centro.x < halfWidth)
            centro.x = halfWidth;
        else if (centro.x > mapaAncho - halfWidth)
            centro.x = mapaAncho - halfWidth;

        // Limitar en Y
        if (centro.y < halfHeight)
            centro.y = halfHeight;
        else if (centro.y > mapaAlto - halfHeight)
            centro.y = mapaAlto - halfHeight;

        // Aplicar la corrección final
        vista.setCenter(centro);

        itemRecolectable.update();
        if (!itemRecolectable.isActive())
            itemRecolectable.spawn(window.getSize());
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


void gamePlay::drawMenuPrincipal()
{
    window.clear(sf::Color::Black);

    if (!juegoIniciado)
    {
        // — Menú Principal —
        window.draw(spriteFondo);
        menuPrincipal.dibujarMenu(window);
    }

    window.display();
}

void gamePlay::drawExploracion()
{
    window.clear(sf::Color::Black);

    if (juegoIniciado)

    {
        if (!sonidoInicioReproducido)
    {
        castle.play(); ///  Reproducir sonido de inicio de partida
        sonidoInicioReproducido = true;
    }
         window.setView(vista);
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

void gamePlay::ejecutar()
{
    while (window.isOpen() && ejecutando)
    {
        procesarEventos();
        sf::Time dt = reloj.restart();

        switch (estado)
        {
        case EstadoJuego::MenuPrincipal:
            drawMenuPrincipal();
            break;



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
                if (!sonidoBattleReproducido)
                    {
                       castle.stop();
                       battle.play(); ///  Reproducir sonido de inicio de partida
                       sonidoBattleReproducido = true;
                    }

                // Crear la instancia de batalla
                std::vector<enemigo*> participantes{ enemigoSeleccionado };
                batallaGamePlay = new batalla(*jugadorActivo, participantes, flecha);

                // Guardar posición previa y teleportar al jugador al punto de batalla
                posicionPreBatalla = jugadorActivo->getSprite().getPosition();
                jugadorActivo->setPosition(100.f, 750.f);

                // Iniciar la batalla (carga de texturas, menús, etc.)
                batallaGamePlay->iniciarBatalla(window);
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
                battle.stop();                      /// Detener música de batallaAdd commentMore actions
                castle.play();                      /// Volver a la música de exploración
                sonidoBattleReproducido = false;    /// Habilitar para la próxima batalla

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
/*
bool ambosBossDerrotados = true;
for (auto* e : enemigos)
{
    Boss* boss = dynamic_cast<Boss*>(e);
    if (boss && boss->estaActivo())  // si aún está activo, no está derrotado
    {
        ambosBossDerrotados = false;
        break;
    }
}
if (ambosBossDerrotados)
{
    popupCartel.mostrar("¡Has derrotado a los jefes finales!\nEl mal ha sido vencido...\n\nGRACIAS POR JUGAR KLOSTERVANIA.", window.getSize());
    juegoIniciado = false;
    estado = EstadoJuego::MenuPrincipal;
}
*/

                    estado = EstadoJuego::Exploracion;
                    // Nota: dejamos juegoIniciado = true para que siga la partida
                }
                else
                {
                    // -------- DERROTA --------
                    // Volvemos al menú principal
                    juegoIniciado = false;
                    estado = EstadoJuego::MenuPrincipal;
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

void gamePlay::inicializarEnemigos()
{
/////////////////////////////////////////////// murcielagos  ////////////////////////////////////////////////////////////////////
    sf::Vector2f posMurcielagos1(200.f, 100.f);
    sf::Vector2f puntoPatrullaMurcielagos1(200.f, 270.f);
    std::string rutaMurcielagos1 = "img/murcielagos.png";
    sf::Vector2f escalaMurcielagos1   = {0.12f, 0.12f};
    enemigo* murcielagos1 = new enemigo(posMurcielagos1, rutaMurcielagos1, escalaMurcielagos1, puntoPatrullaMurcielagos1, 1);
    murcielagos1->setSalud(50);
    enemigos.push_back(murcielagos1);


////////////////////////////////////////////  esqueletos verde solo 2 atq ///////////////////////////////////////////////////////////

    sf::Vector2f posEsqueletoVerde1(300.f, 200.f);
    sf::Vector2f puntoPatrullaEsqueletoVerde1(300.f, 150.f);
    std::string rutaEsqueletoVerde1 = "img/esqueleto_verde.png";
    sf::Vector2f escalaEsqueletoVerde1   = {0.12f, 0.12f};
    enemigo* esqueletoVerde1 = new enemigo(posEsqueletoVerde1, rutaEsqueletoVerde1, escalaEsqueletoVerde1, puntoPatrullaEsqueletoVerde1, 2);
    esqueletoVerde1->setSalud(80);
    enemigos.push_back(esqueletoVerde1);


///////////////////////////////////////////// Esqueletos 2 ataques  /////////////////////////////////////////////////////////////////
    sf::Vector2f posEsqueleto1(500.f, 280.f);
    sf::Vector2f puntoPatrullaEsqueleto1(550.f, 250.f);
    std::string rutaEsqueleto1 = "img/esqueleto.png";
    sf::Vector2f escalaEsqueleto1   = {0.12f, 0.12f};
    enemigo* esqueleto1 = new enemigo(posEsqueleto1, rutaEsqueleto1, escalaEsqueleto1, puntoPatrullaEsqueleto1, 2);
    esqueleto1->setSalud(100);
    enemigos.push_back(esqueleto1);

////////////////////////////////////////////// vampiros 2 ataques  //////////////////////////////////////////////////////////////////
    sf::Vector2f posVampiro1(300.f, 280.f);
    sf::Vector2f puntoPatrullaVampiro1(450.f, 200.f);
    std::string rutaVampiro1 = "img/vampiro.png";
    sf::Vector2f escalaVampiro1   = {0.12f, 0.12f};
    enemigo* vampiro1 = new enemigo(posVampiro1, rutaVampiro1, escalaVampiro1, puntoPatrullaVampiro1, 2);
    vampiro1->setSalud(150);
    enemigos.push_back(vampiro1);

///////////////////////////////////////////// Esqueletos 3 ataques  //////////////////////////////////////////////////////////////
    sf::Vector2f posEsqueleto10(500.f, 280.f);
    sf::Vector2f puntoPatrullaEsqueleto10(550.f, 250.f);
    std::string rutaEsqueleto10 = "img/esqueleto.png";
    sf::Vector2f escalaEsqueleto10   = {0.12f, 0.12f};
    enemigo* esqueleto10 = new enemigo(posEsqueleto10, rutaEsqueleto10, escalaEsqueleto10, puntoPatrullaEsqueleto10, 3);
    esqueleto10->setSalud(100);
    enemigos.push_back(esqueleto10);

////////////////////////////////////////////// vampiros 3 ataques  //////////////////////////////////////////////////////////////
    sf::Vector2f posVampiro10(300.f, 680.f);
    sf::Vector2f puntoPatrullaVampiro10(450.f, 650.f);
    std::string rutaVampiro10 = "img/vampiro.png";
    sf::Vector2f escalaVampiro10   = {0.12f, 0.12f};
    enemigo* vampiro10 = new enemigo(posVampiro10, rutaVampiro10, escalaVampiro10, puntoPatrullaVampiro10, 3);
    vampiro10->setSalud(150);
    enemigos.push_back(vampiro10);




/////////////////////////////////////////////////////////  BOSSES  ///////////////////////////////////////////////////////////
    // 2) Boss “laranas”
    sf::Vector2f posLaranas(1300.f, 600.f);
    sf::Vector2f puntoPatrullaLaranas(1300.f, 500.f);
    std::string rutaLaranas = "img/spritesheet_laranas.png";
    sf::Vector2f escalaLar  = {0.2f, 0.2f};
    Boss* laranas = new Boss(posLaranas, rutaLaranas, escalaLar, puntoPatrullaLaranas,5);
    laranas->setSalud(30);
    enemigos.push_back(laranas);

    // 2) Boss “Klosferatu”
    sf::Vector2f posKlosferatu(1300.f, 200.f);
    sf::Vector2f puntoPatrullaKlosferatu(1300.f, 240.f);
    std::string rutaKlosferatu = "img/spritesheet_klosferatu.png";
    sf::Vector2f escalaKlosferatu  = {0.2f, 0.2f};
    Boss* klosferatu = new Boss(posKlosferatu, rutaKlosferatu, escalaKlosferatu, puntoPatrullaKlosferatu,5);
    klosferatu->setSalud(50);
    enemigos.push_back(klosferatu);
}

void gamePlay::unlockPersonaje(int prototipoIndex)//nose si llegamosss
{
    if (prototipoIndex < 0 || prototipoIndex >= static_cast<int>(prototipos.size()))
        return;

    auto nuevo = prototipos[prototipoIndex];
    for (auto& p : roster)
    {
        if (p == nuevo)
            return; // ya estaba desbloqueado
    }
    nuevo->setPosition({1000.f, 800.f});  // posición de aparición
    roster.push_back(nuevo);
    popupCartel.mostrar("¡Has encontrado un nuevo personaje!", window.getSize());
}

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
    estado        = EstadoJuego::MenuPrincipal;

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

void gamePlay::seleccionPersonaje()
{
    // — Fondo de la pantalla de selección —
    sf::Texture fondoSeleccion;
    if (!fondoSeleccion.loadFromFile("img/fondoSeleccion.png"))
    {
        std::cout << "Error al cargar fondoSeleccion.png\n";
        return;
    }
    sf::Sprite spriteFondoSeleccion(fondoSeleccion);
    spriteFondoSeleccion.setScale(1.f, 0.89f);
    fadeInTransition(spriteFondoSeleccion);

    // — Posiciones de los 6 personajes en la parte inferior —
    std::vector<sf::Vector2f> posiciones =
    {
        {270.f, 875.f}, {450.f, 875.f}, {630.f, 875.f},
        {815.f, 875.f}, {980.f, 875.f}, {1165.f, 875.f}
    };

    int seleccionActual = 0;
    bool seleccionado = false;

    // — Temporizadores para animación y espera (central) —
    sf::Clock animClock;          // controla cada frame en la animación central
    sf::Clock esperaClock;        // controla los 3s quieto en la central
    bool enEspera     = true;
    int  currentFrame = 0;
    const float frameTime = 0.15f;

    // Función interna para limpiar estado visual de un personaje seleccionado
    auto reiniciarEstadoSeleccion = [](std::shared_ptr<personaje>& p)
    {
        p->setOrigin(0.f, 0.f);
        p->setScale(0.25f, 0.25f);
        p->setEstado(personaje::estadoPersonaje::quieto);
    };

    // — Bucle de selección —
    while (window.isOpen() && !seleccionado)
    {
        // — Procesamiento de eventos —
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                ejecutando = false;
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    seleccionActual = (seleccionActual + 5) % 6;
                    flecha.play();
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    seleccionActual = (seleccionActual + 1) % 6;
                    flecha.play();
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    // Solo confirmar si el índice está en roster (desbloqueado)
                    if (seleccionActual < static_cast<int>(roster.size()))
                    {
                        jugadorActivo = roster[seleccionActual];
                        reiniciarEstadoSeleccion(jugadorActivo);
                        jugadorActivo->setPosition({50.f, 500.f});
                        jugadorActivo->setSalud(500);
                        seleccionado = true;
                        enter.play();
                    }
                }
            }
        }

        // — Animación del personaje central —
        if (!enEspera)
        {
            if (animClock.getElapsedTime().asSeconds() > frameTime)
            {
                currentFrame = (currentFrame + 1) % 6;  // ciclo de 6 frames de ataque
                animClock.restart();
                if (currentFrame == 0)
                {
                    enEspera = true;
                    esperaClock.restart();
                }
            }
        }
        else
        {
            if (esperaClock.getElapsedTime().asSeconds() > 2.f)
            {
                enEspera = false;
                currentFrame = 0;
                animClock.restart();
            }
        }

        window.clear();

        // — Dibujar fondo —
        window.draw(spriteFondoSeleccion);

        // — Dibujar personajes inferiores en estado quieto —
        for (int i = 0; i < 6; ++i)
        {
            if (i < static_cast<int>(prototipos.size()))
            {
                auto& p = prototipos[i];
                p->setPosition(posiciones[i]);
                p->setScale(0.2f, 0.2f);
                p->setEstado(personaje::estadoPersonaje::quieto);
                p->update(0.f, false, false, false, false);

                if (i >= static_cast<int>(roster.size()))
                    p->getSprite().setColor(sf::Color(255, 255, 255, 70));
                else
                    p->getSprite().setColor(sf::Color(255, 255, 255, 255));

                p->draw(window);
            }
        }

        // — Dibujar personaje central animado y superponer la cara con marco —
        if (seleccionActual < static_cast<int>(prototipos.size()))
        {
            auto& p = prototipos[seleccionActual];

            // 1) Posicionar y escalar el cuerpo animado
            p->setPosition({620.f, 700.f});
            p->setScale(0.6f, 0.6f);

            if (enEspera)
            {
                p->setEstado(personaje::estadoPersonaje::quieto);
                p->update(0.f, false, false, false, false);
            }
            else
            {
                p->setFrameAtaque(currentFrame);
            }
            p->draw(window);

            // 2) Ahora superponer la cara+marco
            // Asumimos frameWidth = frameHeight = 500 y filaFrameQuieto = 6
            const int frameWidth  = 500;
            const int frameHeight = 500;
            const int filaFrameCara = 6;
            // columna 1 para "cara+marco"
            sf::IntRect caraConMarcoRect(
                frameWidth,            // x = columna 1 * 500
                filaFrameCara * frameHeight, // y = fila 6 * 500
                frameWidth,
                frameHeight
            );

            // a) Crear sprite temporal de cara+marco usando la misma textura
            sf::Sprite spriteCara;
            spriteCara.setTexture(*p->getSprite().getTexture());
            spriteCara.setTextureRect(caraConMarcoRect);

            // b) Escalar para que encaje sobre el cuerpo (500×500 * 0.6 = 300×300)
            spriteCara.setScale(0.9f, 0.9f);

            // c) Posicionar la cara+marco centrada
            spriteCara.setPosition( 537.f , -2.f );

            // d) Opacidad reducida si está bloqueado
            if (seleccionActual >= static_cast<int>(roster.size()))
                spriteCara.setColor(sf::Color(255, 255, 255, 70));
            else
                spriteCara.setColor(sf::Color(255, 255, 255, 255));

            // e) Dibujar la cara+marco encima del cuerpo
            window.draw(spriteCara);
        }

        window.display();
    }

    // — Salimos del selector y pasamos a exploración —
    estado = EstadoJuego::Exploracion;
}

void gamePlay::agregarPersonaje(const std::string& nombre, const std::string& ruta)
{
    std::cout << "Intentando cargar textura: " << ruta << "\n";

    auto personajePtr = std::make_shared<personaje>(sf::Vector2f{0.f, 0.f}, ruta, sf::Vector2f{0.5f, 0.5f});
    if (!personajePtr->getSprite().getTexture())
    {
        std::cout << "Error al cargar personaje: " << nombre << "\n";
        return;
    }

    std::cout << "Textura cargada OK: " << ruta << "\n";
    std::cout << "Agregado personaje " << nombre << " correctamente.\n";

    prototipos.push_back(personajePtr);
}

void gamePlay::inicializarPrototipos()
{
    // ——— Personajes iniciales desbloqueados ———
    agregarPersonaje("Arcangel Simon", "img/spritesheet_Arcangel.png");
    agregarPersonaje("Wennering",      "img/spritesheet_Wennering.png");
    agregarPersonaje("Taparia",        "img/spritesheet_Taparia.png");
    agregarPersonaje("Vernary",        "img/spritesheet_Vernary.png");

    // Agregar al roster los jugables desde el inicio
    roster.push_back(prototipos[0]);
    roster.push_back(prototipos[1]);
    roster.push_back(prototipos[2]);
    roster.push_back(prototipos[3]);

    // ——— Bosses que aparecen como bloqueados ———
    agregarPersonaje("Klosferatu",     "img/spritesheet_klosferatu.png");
    agregarPersonaje("Laranas",        "img/spritesheet_laranas.png");
}

void gamePlay::iniciarNuevaPartida()
{
    std::cout << "\nIniciando una nueva partida...\n";

    // 1. Resetear datos anteriores
    roster.clear();
    prototipos.clear();
    jugadorActivo = nullptr;
    enemigoSeleccionado = nullptr;
    batallaIniciada = false;

    // 2. Cargar personajes disponibles y ocultos
    inicializarPrototipos();

    // 3. Ir al menú de selección de personaje
    seleccionPersonaje();  // ← el jugadorActivo queda configurado aquí

    // 4. Ahora sí, inicializar enemigos
    inicializarEnemigos();

    // 5. Marcar el juego como iniciado
    // Configuración inicial
    jugadorActivo->setPosition({10.f, 200.f});
    jugadorActivo->setScale({0.1f, 0.1f});
    jugadorActivo->setSalud(500);

    fadeInTransition(spriteNuevaPartida);

    juegoIniciado = true;
    estado = EstadoJuego::Exploracion;

    std::cout << "Partida iniciada correctamente.\n";
}

void gamePlay::fadeInTransition(sf::Sprite& spriteFondo)
{
    fadeInTransition(spriteFondo, nullptr);  // llama a la extendida con vista nula
}

void gamePlay::fadeInTransition(sf::Sprite& spriteFondo, sf::View* vista)
{
    // 1) Inicializar alpha y pantallaNegra
    alphaFade = 255.f;
    pantallaNegra.setSize({1500.f, 900.f});
    pantallaNegra.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alphaFade)));

    // 2) Usamos un reloj local para medir dt de esta transición
    sf::Clock relojFade;

    // 3) Bucle hasta que alphaFade llegue a 0 o la ventana se cierre
    bool terminado = false;
    while (window.isOpen() && !terminado)
    {
        // 3.a) Procesar eventos básicos para que la ventana no se congele
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                ejecutando = false;
                window.close();
                return;
            }
        }

        // 3.b) Calcular deltaTime
        float dt = relojFade.restart().asSeconds();

        // 3.c) Reducir alpha según fadeSpeed
        alphaFade -= fadeSpeed * dt;
        if (alphaFade < 0.f) alphaFade = 0.f;

        pantallaNegra.setFillColor(
            sf::Color(0, 0, 0, static_cast<sf::Uint8>(alphaFade))
        );

        // —— APLICAR la vista que nos pasen, o usar la default ——
        if (vista)
            window.setView(*vista);
        else
            window.setView(window.getDefaultView());

        // 3.d) Dibujar fondo + overlay negro
        window.clear();
        window.draw(spriteFondo);
        window.draw(pantallaNegra);
        window.display();

        if (alphaFade <= 0.f)
            terminado = true;
    }
}

bool gamePlay::esZonaLibre(const sf::FloatRect& area)                   ///recibe un rectángulo (area) que representa una zona del mapa,
{
    for (int x = area.left; x < area.left + area.width; x += 3) {       ///Recorre horizontalmente de x = area.left hasta x = area.right, con un paso de 3 píxeles para no chequear cada píxel
        for (int y = area.top; y < area.top + area.height; y += 3) {    ///Recorre verticalmente de y = area.top hasta y = area.bottom, también con paso de 3.
            if (x < 0 || y < 0 ||                                       ///x < 0 || y < 0 → Está fuera del mapa por la izquierda o arriba.
                x >= (int)mascaraColision.getSize().x ||                ///Está fuera del mapa por la derecha
                y >= (int)mascaraColision.getSize().y ||                ///o abajo.
                mascaraColision.getPixel(x, y) != sf::Color::White)     /// El píxel NO es blanco, por lo tanto hay un obstáculo.
                return false;
        }
    }
    return true;
}
