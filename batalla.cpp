#include "batalla.h"

batalla::batalla(personaje& jugador, enemigo& adversario, sf::Sound& soundFlecha)
    : _jugador(jugador)
    , _adversario(adversario)
    , soundFlecha   (soundFlecha)
    , turnoActual(Turno::Jugador)
    , vidaJugador(jugador.getSalud())
    , vidaAdversario(adversario.getSalud()-400) //le resto para matarlo rapido y seguir porbando
    , terminado(false)
    , jugadorGanoFlag(false)
    , fondo({1500.f, 900.f})
{

};

void batalla::iniciarBatalla()
{
//  cargo la textura
    if (!texturaFondo.loadFromFile("img/fondoBatalla.jpg"))
    {
        std::cout << "Error al cargar fondoBatalla.jpg\n";
    }
    spriteFondo.setTexture(texturaFondo);
    spriteFondo.setScale(1.0f, 0.9f);

    // Fuente y textos
    if (!fuente.loadFromFile("fonts/Rochester-Regular.ttf"))
        std::cout << "Error al cargar fuente de batalla\n";

    menuBatalla.crearMenu(numOpcionesMenuBatalla, fuente, opcionesVectorBatalla, 20, 120, 760, 20, sf::Color::White, sf::Color::Red);

    // Configuración del recuadro de mensaje
    if (!fuenteMensaje.loadFromFile("fonts/Rochester-Regular.ttf"))
        std::cout << "Error al cargar fuenteMensaje\n";

    textoMensaje.setFont(fuenteMensaje);
    textoMensaje.setCharacterSize(20);
    textoMensaje.setFillColor(sf::Color::White);
    _adversario.setOrigin(0.f, 0.f);
    _adversario.setPosition(1100.f, 630.f);
    _adversario.setModoBatalla(true);

};

void batalla::manejarInput()
{
    // Solo aceptamos flechas en el turno del jugador
    if (turnoActual != Turno::Jugador) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        _opcionSeleccionada = (_opcionSeleccionada - 1 + numOpcionesMenuBatalla) % numOpcionesMenuBatalla;
        soundFlecha.play();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {}         //me sirve para esperar a que deje de tocar la flecha
        menuBatalla.actualizarMenu(_opcionSeleccionada, sf::Color::Red, sf::Color::White);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        _opcionSeleccionada = (_opcionSeleccionada + 1) % numOpcionesMenuBatalla;
        soundFlecha.play();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {}       //me sirve para esperar a que deje de tocar la flecha
        menuBatalla.actualizarMenu(_opcionSeleccionada, sf::Color::Red, sf::Color::White);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        std::cout << "\n=>TURNO JUGADOR:";
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
        //enter.play();

        switch (_opcionSeleccionada)
        {
        case 0:  // Ataque ligero
            vidaAdversario -= _jugador.getAtaqueLigero();
            msj += "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene "+ std::to_string(vidaAdversario)+" de salud";
            mostrarMensaje(msj);
           std::cout << "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene " << vidaAdversario << " de vida\n";
            _jugador.ataqueLigero({1100.f, 630.f});
            turnoActual = Turno::Enemigo;
            _rondaCarga++;
            break;

        case 1: //  Ataque pesado
            if(_rondaCarga>=2)
            {
                vidaAdversario -= _jugador.getAtaquePesado();
                 msj +="\n¡Jugador golpea con Ataque Pesado! Enemigo tiene " + std::to_string(vidaAdversario)+" de salud";
                 mostrarMensaje(msj);
                std::cout << "\n¡Jugador golpea con Ataque Pesado! Enemigo tiene " << vidaAdversario << " de vida\n";
                _jugador.ataquePesado({1100.f, 630.f});
                _rondaCarga=0;
                turnoActual = Turno::Enemigo;
            }
            else
            {
                msj += "\nNo se cargo el golpe pesado ";
                mostrarMensaje(msj);
                std::cout << "\nNo se cargo el golpe pesado ";
            }

            break;

        case 2: //  habilidad especial
            if (_rondaCarga>=3)
            {
                vidaAdversario -= _jugador.getHabilidadEspecial();
                 msj += "\nEl jugador ha usado la habilida especial Enemigo tiene " + std::to_string(vidaAdversario)+" de salud";
                 mostrarMensaje(msj);
                std::cout << "\nEl jugador ha usado la habilida especial";
                _jugador.habilidadEspecial({1000.f, 600.f});
                _rondaCarga=0;
                turnoActual = Turno::Enemigo;
            }
            else
            {
                msj += "\nNo se cargo la habilidad especial";
                mostrarMensaje (msj);
                std::cout << "\nNo se cargo la habilidad ";
            }

            break;

        default:

            break;
        }
    }
}

void batalla::actualizar(float deltaTime)
{
    _adversario.update(deltaTime,
                       /*movDer*/ false,
                       /*movIzq*/ false,
                       /*movArr*/ false,
                       /*movAbj*/ false);
    // Si no es el turno del enemigo, no hacemos nada
    if (turnoActual != Turno::Enemigo)
        return;
    // Si ya vencimos y hemos arrancado el reloj, espera 3s
    if (victoriaIniciada)
    {
        if (victoriaClock.getElapsedTime() >= sf::seconds(3.f))
        {
            terminado = true;
        }
        return;
    }
    if (vidaAdversario <= 0 )
    {
        if(!_jugador.estaAtacando()){
        // 1) Marcar victoria y desactivar enemigo
        victoriaIniciada = true;
        _adversario.setActivo(false);
        jugadorGanoFlag = true;

        // 2)—— RECOMPENSA AL JUGADOR ——
        const int bonusSalud        = 50;
        const int bonusAtaqueLigero    =  5;
        const int bonusAtaquePesado    =  3;
        const int bonusHabilidadEspecial =  1;

        // Suma los bonus a los valores actuales:
        _jugador.setSalud( _jugador.getSalud() + bonusSalud );
        _jugador.setAtaqueLigero( _jugador.getAtaqueLigero() + bonusAtaqueLigero );
        _jugador.setAtaquePesado( _jugador.getAtaquePesado() + bonusAtaquePesado );
        _jugador.setHabilidadEspecial( _jugador.getHabilidadEspecial() + bonusHabilidadEspecial );

        // —— POP UP DE VENTANA DE VICTORIA ——
            popupFinBatalla.cargarRecursos("img/panel_item.png", "fonts/Rochester-Regular.ttf");
            mensajeFinBatalla  = "¡HAS VENCIDO AL ENEMIGO!!\n\n";
            mensajeFinBatalla += "AUMENTO DE TUS ESTADISTICAS:\n";
            mensajeFinBatalla += "Salud +"+std::to_string(bonusSalud)+"\n";
            mensajeFinBatalla += "Atauqe Ligero +"+std::to_string(bonusAtaqueLigero)+"\n";
            mensajeFinBatalla += "Ataque Pesado +"+std::to_string(bonusAtaquePesado)+"\n";
            mensajeFinBatalla += "Habilidad Especial +"+std::to_string(bonusHabilidadEspecial)+"\n";

        // consola la nueva estadística
        std::cout << "¡Recompensa! Salud +"
                  << bonusSalud << ", Ataque Ligero +"
                  << bonusAtaqueLigero << ", Ataque Pesado +"
                  << bonusAtaquePesado << ", Habilidad +"
                  << bonusHabilidadEspecial << "\n";
        // NUEVAS estadísticas tras el bonus
        std::cout << "Nuevas estadísticas -> "
                  << "Salud: " << _jugador.getSalud()
                  << ", Ataque Ligero: " << _jugador.getAtaqueLigero()
                  << ", Ataque Pesado: " << _jugador.getAtaquePesado()
                  << ", Habilidad Especial: " << _jugador.getHabilidadEspecial()
                  << "\n";
        // 3) Arranca el reloj de espera
        //victoriaClock.restart();
        }
        return;
    }
if (!_jugador.estaAtacando()) {
    // El enemigo ataca de forma aleatoria y devuelve el daño
    sf::Vector2f posJugador = _jugador.getSprite().getPosition();
    int danio = _adversario.ataque(posJugador);
    vidaJugador -= danio;
    msj += "\n¡Enemigo ataca! Jugador recibe "+ std::to_string(danio)+ " de danio";
    mostrarMensaje(msj);
    std::cout << msj << ". Vida jugador: " << vidaJugador << "\n";


    // 3) Pasar turno de nuevo al jugador y aumentar ronda
    turnoActual   = Turno::Jugador;
    _rondaTurno++;
    }
    // 4) Compruebo fin de batalla
    if (vidaJugador <= 0)
    {
        terminado       = true;
        jugadorGanoFlag = (vidaJugador <= 0);
        _adversario.setModoBatalla(false);
    }
}

void batalla::drawBatalla(sf::RenderWindow& window)
{
    window.draw(spriteFondo);
    //  Ajusto colores según disponibilidad

    sf::Color normal    = sf::Color::White;
    sf::Color disabled  = sf::Color(128,128,128);   //porque no se cargaron los otros golpes/habilidades
    sf::Color selected  = sf::Color::Red;

    // Recorre cada opción
    for (int i = 0; i < numOpcionesMenuBatalla; ++i)
    {
        // Si es la seleccionada, la pinto de rojo
        if (i == _opcionSeleccionada)
            menuBatalla.setOptionColor(i, selected);
        else
        {
            // Si es “Pesado” (índice 1) y rondaActual<2, gris  / o “ Habilidad Especial” (índice 2) y rondaActual<3, gris
            if ((i == 1 && _rondaCarga < 2) ||(i == 2 && _rondaCarga < 3))
            {
                menuBatalla.setOptionColor(i, disabled);
            }
            else
            {
                menuBatalla.setOptionColor(i, normal);
            }
        }
    }
    //ataques
    menuBatalla.dibujarMenu(window);
    //mensajes durante la batalla
   if (mensajeActivo)
    {
        window.draw(textoMensaje);
    }


 // en caso de animar el ataque, getSprite() ya va moviéndose solo
_jugador.draw(window);

    // ——— Enemigo ———
_adversario.draw(window);

//  Si la batalla ya terminó y ganaste, lanza el popup
   if (jugadorGanoFlag && !popupFinMostrado) {
        popupFinBatalla.mostrar(mensajeFinBatalla, window.getSize());
        popupFinMostrado = true;
    }

    // Dibuja el popup (si está activo)
    popupFinBatalla.draw(window);
};

bool batalla::ganador() const
{
    return jugadorGanoFlag;
}

bool batalla::finBatalla() const
{
    return terminado;
};

void batalla::mostrarMensaje(const std::string& msg)
{
    textoMensaje.setString(msg);
    sf::FloatRect b = textoMensaje.getLocalBounds();
    textoMensaje.setPosition(15.f, 110.f - b.height);
    mensajeActivo = true;
}
