#include "batalla.h"
#include <iostream>

batalla::batalla(personaje& jugador,
                 const std::vector<enemigo*>& adversarios,
                 sf::Sound& soundFlecha)
    : _jugador(jugador)
    , _adversarios(adversarios)
    , soundFlecha(soundFlecha)
    , vidaJugador(jugador.getSalud())
    , vidaAdversario(adversarios.empty() ? 0 : adversarios[0]->getSalud())
    , turnoActual(Turno::Jugador)
    , terminado(false)
    , jugadorGanoFlag(false)
    , popupFinMostrado(false)
    , victoriaIniciada(false)
    , _rondaCarga(0)
    , _rondaTurno(1)
    , msj("")
    , mensajeFinBatalla("")
{
    // — Transición —
    pantallaNegraFade.setSize({1500.f, 900.f});
    pantallaNegraFade.setFillColor(sf::Color(0, 0, 0, 0));
    // Guardar posición inicial del primer enemigo, si existe
    if (!_adversarios.empty())
    {
        _posEnemigoInicial = _adversarios[0]->getSprite().getPosition();
    }
}

void batalla::iniciarBatalla(sf::RenderWindow& window)
{
    // 1) Cargar textura de fondo
    if (!texturaFondo.loadFromFile("img/fondoBatalla.jpg"))
    {
        std::cout << "Error al cargar fondoBatalla.jpg\n";
    }
    spriteFondo.setTexture(texturaFondo);
    spriteFondo.setScale(1.0f, 0.9f);

    // 2) Cargar fuente para el menú de batalla
    if (!fuente.loadFromFile("fonts/Rochester-Regular.ttf"))
    {
        std::cout << "Error al cargar fuente de batalla\n";
    }

    menuBatalla.crearMenu(
        numOpcionesMenuBatalla,
        fuente,
        opcionesVectorBatalla,
        20,    // Tamaño del texto en el menú
        120,   // Posición X
        760,   // Posición Y
        20,    // Espacio vertical
        sf::Color::White,
        sf::Color::Red
    );

    // 3) Cargar fuente para la caja de texto de mensajes en batalla
    if (!fuenteMensaje.loadFromFile("fonts/Rochester-Regular.ttf"))
    {
        std::cout << "Error al cargar fuenteMensaje\n";
    }
    textoMensaje.setFont(fuenteMensaje);
    textoMensaje.setCharacterSize(20);
    textoMensaje.setFillColor(sf::Color::White);
    mensajeActivo = false;

    // 4) Preparar los enemigos
    float pisoY = 760.f;
    float posX   = 1100.f;
    for (size_t i = 0; i < _adversarios.size(); ++i) {
        enemigo* e = _adversarios[i];
        if (!e) continue;
        e->setPosition(posX, pisoY);
        e->setModoBatalla(true);
    }

    // 5) Resetear flags y datos de turno
    terminado         = false;
    jugadorGanoFlag   = false;
    popupFinMostrado  = false;
    victoriaIniciada  = false;
    _rondaCarga       = 0;
    _rondaTurno       = 1;
    msj.clear();
    mensajeFinBatalla.clear();

    fadeIn(window);
}

void batalla::manejarInput()
{
    if (turnoActual != Turno::Jugador) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        _opcionSeleccionada = (_opcionSeleccionada - 1 + numOpcionesMenuBatalla)
                              % numOpcionesMenuBatalla;
        soundFlecha.play();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}
        menuBatalla.actualizarMenu(_opcionSeleccionada, sf::Color::Red, sf::Color::White);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        _opcionSeleccionada = (_opcionSeleccionada + 1) % numOpcionesMenuBatalla;
        soundFlecha.play();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}
        menuBatalla.actualizarMenu(_opcionSeleccionada, sf::Color::Red, sf::Color::White);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
        enemigo* e = _adversarios[0];

        switch (_opcionSeleccionada)
        {
        case 0:  // Ataque Ligero
            vidaAdversario -= _jugador.getAtaqueLigero();
            msj += "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene "
                   + std::to_string(vidaAdversario) + " de salud";
            mostrarMensaje(msj);
            std::cout << "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene " << vidaAdversario << " de vida\n";
            _jugador.ataqueLigero({1100.f, 740.f});
            turnoActual = Turno::Enemigo;
            _rondaCarga++;
            break;

        case 1:  // Ataque Pesado
            if (_rondaCarga >= 2)
            {
                vidaAdversario -= _jugador.getAtaquePesado();
                msj += "\n¡Jugador golpea con Ataque Pesado! Enemigo tiene "
                    + std::to_string(vidaAdversario) + " de salud";
                mostrarMensaje(msj);
                std::cout << "\n¡Jugador golpea con Ataque Pesado! Enemigo tiene " << vidaAdversario << " de vida\n";
                _jugador.ataquePesado({1100.f, 740.f});
                _rondaCarga = 0;
                turnoActual = Turno::Enemigo;
            }
            else
            {
                msj += "\nNo se cargó el golpe pesado";
                mostrarMensaje(msj);
            }
            break;

        case 2:  // Habilidad Especial
            if (_rondaCarga >= 3)
            {
                vidaAdversario -= _jugador.getHabilidadEspecial();
                msj += "\nEl jugador ha usado Habilidad Especial. Enemigo tiene "
                    + std::to_string(vidaAdversario) + " de salud";
                mostrarMensaje(msj);
                std::cout << "\nEl jugador ha usado la habilida especial! Enemigo tiene " << vidaAdversario << " de vida\n";
                _jugador.habilidadEspecial({1000.f, 740.f});
                _rondaCarga = 0;
                turnoActual = Turno::Enemigo;
            }
            else
            {
                msj += "\nNo se cargó la Habilidad Especial";
                mostrarMensaje(msj);
            }
            break;

        default:
            break;
        }
    }
}

void batalla::actualizar(float deltaTime)
{
    if (_adversarios.empty() || terminado)
        return;

    enemigo* e = _adversarios[0];
    e->update(deltaTime, false, false, false, false);

    if (desvaneciendo)
    {
        // Solo avanzamos el fade aquí; nunca entramos a turno de Enemigo
        if (alphaFade < 255.f)
        {
            alphaFade += fadeSpeed * deltaTime;
            if (alphaFade > 255.f) alphaFade = 255.f;
            pantallaNegraFade.setFillColor(
                sf::Color(0, 0, 0, static_cast<sf::Uint8>(alphaFade))
            );
        }
        else
        {
            // Cuando el fade ya alcanzó opacidad total, marcamos la batalla como terminada
            enemigo* e = _adversarios[0];
            e->setActivo(false);
            terminado = true;
        }
        return;
    }

    if (turnoActual != Turno::Enemigo)
    {
        return;
    }
    // Verificar si el enemigo murió
    if (vidaAdversario <= 0)
    {
        // 1) Victoria del jugador: solo la primera vez que entra aquí
        if (!victoriaIniciada)
        {
            victoriaIniciada = true;
            jugadorGanoFlag = true;

            // —— Recompensa al jugador ——  (se ejecuta aún si está en medio de la animación)
            const int bonusSalud             = 50;
            const int bonusAtaqueLigero      =  5;
            const int bonusAtaquePesado      =  3;
            const int bonusHabilidadEspecial =  1;

            _jugador.setSalud(_jugador.getSalud() + bonusSalud);
            _jugador.setAtaqueLigero(_jugador.getAtaqueLigero() + bonusAtaqueLigero);
            _jugador.setAtaquePesado(_jugador.getAtaquePesado() + bonusAtaquePesado);
            _jugador.setHabilidadEspecial(_jugador.getHabilidadEspecial() + bonusHabilidadEspecial);

            // 2) Preparamos el texto que irá en el pop-up:
            mensajeFinBatalla  = "¡HAS VENCIDO AL ENEMIGO!\n\n";
            mensajeFinBatalla += "AUMENTO DE TUS ESTADÍSTICAS:\n";
            mensajeFinBatalla += "Salud +" + std::to_string(bonusSalud) + "\n";
            mensajeFinBatalla += "Ataque Ligero +" + std::to_string(bonusAtaqueLigero) + "\n";
            mensajeFinBatalla += "Ataque Pesado +" + std::to_string(bonusAtaquePesado) + "\n";
            mensajeFinBatalla += "Habilidad Especial +" + std::to_string(bonusHabilidadEspecial) + "\n";

            std::cout << "¡Recompensa! Salud +"
                      << bonusSalud << ", Ataque Ligero +"
                      << bonusAtaqueLigero << ", Ataque Pesado +"
                      << bonusAtaquePesado << ", Habilidad +"
                      << bonusHabilidadEspecial << "\n";
            std::cout << "Nuevas estadísticas -> "
                      << "Salud: " << _jugador.getSalud()
                      << ", Ataque Ligero: " << _jugador.getAtaqueLigero()
                      << ", Ataque Pesado: " << _jugador.getAtaquePesado()
                      << ", Habilidad Especial: " << _jugador.getHabilidadEspecial()
                      << "\n";

            // 3) Arrancamos el fade out YA, sin esperar a que termine la animacion, da el efecto que se muere
            desvaneciendo = true;
            alphaFade     = 0.f;
        }
        return;
    }
    // Turno del enemigo: ataca si el jugador no está en animación de ataque
    if (!_jugador.estaAtacando())
    {
        sf::Vector2f posJugador = _jugador.getSprite().getPosition();
        int danio = e->ataque(posJugador);
        vidaJugador -= danio;
        msj += "\n¡Enemigo ataca! Jugador recibe " + std::to_string(danio) + " de danio";
        mostrarMensaje(msj);
        std::cout << msj << "\n   Vida jugador: " << vidaJugador << "\n";
        // Verificar fin por derrota del jugador
        if (vidaJugador <= 0)
        {
            // Solo la primera vez que detectamos vida <= 0:
            if (!desvaneciendo)
            {
                desvaneciendo = true;
                alphaFade     = 0.f;
                mensajeFinBatalla =
                    "Nuestros héroes han perdido la batalla\n"
                    "El fin del mundo se acerca...";
            }
        }
        else
        {
            turnoActual = Turno::Jugador;
            _rondaTurno++;
        };
    }
}

void batalla::drawBatalla(sf::RenderWindow& window)
{
    // 1) Dibujar fondo de batalla
    window.draw(spriteFondo);

    // 2) Colorear opciones del menú según disponibilidad
    sf::Color normal   = sf::Color::White;
    sf::Color disabled = sf::Color(128, 128, 128);
    sf::Color selected = sf::Color::Red;

    for (int i = 0; i < numOpcionesMenuBatalla; ++i)
    {
        if (i == _opcionSeleccionada)
        {
            menuBatalla.setOptionColor(i, selected);
        }
        else if ((i == 1 && _rondaCarga < 2) ||
                 (i == 2 && _rondaCarga < 3))
        {
            menuBatalla.setOptionColor(i, disabled);
        }
        else
        {
            menuBatalla.setOptionColor(i, normal);
        }
    }
    menuBatalla.dibujarMenu(window);

    // 3) Mostrar mensaje en curso (si existe)
    if (mensajeActivo)
    {
        window.draw(textoMensaje);
    }

    // 4) Dibujar al jugador
    _jugador.draw(window);

    // 5) Dibujar al enemigo (si existe)
    if (!_adversarios.empty())
    {
        enemigo* e = _adversarios[0];
        e->draw(window);
    }
    // 6) Dibujo del fade-out (si empezó el desvanecimiento o ya tiene alpha > 0)
    if (desvaneciendo || alphaFade > 0.f)
    {
        window.draw(pantallaNegraFade);
    }
    // 7) Si la batalla terminó y el pop-up no se mostró, cargar recursos y mostrarlo:
    if (terminado && !popupFinMostrado)
    {
        // Cargar recursos en el pop-up (imagen + fuente)
        popupFinBatalla.cargarRecursos(
            "img/panel_item.png",          // Ruta de la imagen del panel
            "fonts/Rochester-Regular.ttf"  // Ruta de la fuente para escribir texto
        );
        // Mostrar el pop-up (bloqueante hasta que el usuario presione Enter)

        popupFinBatalla.mostrar(mensajeFinBatalla, window.getSize());
        popupFinMostrado = true;
    }

    // 8) Dibujar el pop-up (si está activo) para que permanezca visible
    popupFinBatalla.draw(window);
}

bool batalla::ganador() const
{
    return jugadorGanoFlag;
}

bool batalla::finBatalla() const
{
    return terminado && popupFinMostrado;
}

bool batalla::batallaPopupActive() const
{
    return popupFinBatalla.isActive();
}

void batalla::mostrarMensaje(const std::string& msg)
{
    textoMensaje.setString(msg);
    sf::FloatRect b = textoMensaje.getLocalBounds();
    textoMensaje.setPosition(15.f, 110.f - b.height);
    mensajeActivo = true;
}
void batalla::fadeIn(sf::RenderWindow& window)
{
    float alphaFade = 255.f;
    const float fadeSpeed = 150.f;

    // Pantalla negra del tamaño de la ventana
    sf::RectangleShape pantallaNegra({window.getSize().x * 1.f, window.getSize().y * 1.f});
    pantallaNegra.setFillColor(sf::Color(0, 0, 0, 255));

    sf::Clock relojFade;

    while (window.isOpen() && alphaFade > 0.f)
    {
        // Procesar eventos básicos para no congelar la ventana
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
        }

        // Calcular cuánto tiempo pasó
        float dt = relojFade.restart().asSeconds();
        alphaFade -= fadeSpeed * dt;
        if (alphaFade < 0.f) alphaFade = 0.f;

        pantallaNegra.setFillColor(
            sf::Color(0, 0, 0, static_cast<sf::Uint8>(alphaFade))
        );

        // Usar vista por defecto para que la escena de batalla esté centrada
        window.setView(window.getDefaultView());

        // Dibujar fondo de batalla y superponer pantalla negra
        window.clear();
        drawBatalla(window);       // Esta es tu función ya implementada
        window.draw(pantallaNegra);
        window.display();
    }
}


