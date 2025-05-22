#include "batalla.h"


batalla::batalla(personaje& jugador, enemigo& adversario, sf::Sound& soundFlecha)
    : _jugador(jugador)
    , _adversario(adversario)
    , soundFlecha   (soundFlecha)
    , turnoActual(Turno::Jugador)
    , vidaJugador(jugador.getSalud())
    , vidaAdversario(adversario.getSalud()-450) //le resto para matarlo rapido y seguir porbando
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
    //actualizarTexto();
//****************************************        NUEVO       **************************
    // Configuración del recuadro de mensaje
    if (!fuenteMensaje.loadFromFile("fonts/Rochester-Regular.ttf"))
        std::cout << "Error al cargar fuenteMensaje\n";

    textoMensaje.setFont(fuenteMensaje);
    textoMensaje.setCharacterSize(20);
    textoMensaje.setFillColor(sf::Color::White);

    // El recuadro ocupará el ancho de la ventana y 100 px de alto en la parte inferior
    cuadroMensaje.setSize({ 600.f, 100.f });
    cuadroMensaje.setFillColor(sf::Color(0, 0, 0, 180)); // semitransparente
    cuadroMensaje.setPosition(100.f, 50.f);


//**********************************************************************************************************************************
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
            mostrarMensaje("\n¡Jugador golpea con Ataque Ligero! Enemigo tiene "+ std::to_string(vidaAdversario));
            vidaAdversario -= _jugador.getAtaqueLigero();
           std::cout << "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene " << vidaAdversario << " de vida\n";
            _jugador.ataqueLigero({900.f, 600.f});
            turnoActual = Turno::Enemigo;
            _rondaCarga++;
            break;

        case 1: //  Ataque pesado
            if(_rondaCarga>=2)
            {
                vidaAdversario -= _jugador.getAtaquePesado();
                std::cout << "\n¡Jugador golpea con Ataque Pesado! Enemigo tiene " << vidaAdversario << " de vida\n";
                _rondaCarga=0;
                turnoActual = Turno::Enemigo;
            }
            else
            {
                std::cout << "\nNo se cargo el golpe pesado ";
            }

            break;

        case 2: //  habilidad especial
            if (_rondaCarga>=3)
            {
                vidaAdversario -= _jugador.getHabilidadEspecial();
                std::cout << "\nEl jugador ha usado la habilida especial";
                _rondaCarga=0;
                turnoActual = Turno::Enemigo;
            }
            else
            {
                std::cout << "\nNo se cargo la habilidad ";
            }

            break;

        default:

            break;
        }
    }
}

void batalla::actualizar(float /*deltaTime*/)
{
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
        victoriaClock.restart();
        return;
    }

    // 1) Inteligencia básica: siempre usa ataque ligero
    int dano = _adversario.getAtaqueLigero();
    vidaJugador -= dano;
    std::cout << "\n¡Enemigo ataca! Jugador recibe " << dano
              << " de daño. Vida jugador: " << vidaJugador << "\n";

    // 2) Actualizar la UI de vidas
    //actualizarTexto();

    // 3) Pasar turno de nuevo al jugador y aumentar ronda
    turnoActual   = Turno::Jugador;
    _rondaTurno++;

    // 4) Compruebo fin de batalla
    if (vidaJugador <= 0)
    {
        terminado       = true;
        jugadorGanoFlag = (vidaJugador <= 0);
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
        window.draw(cuadroMensaje);
        window.draw(textoMensaje);
    }


///  ——— Jugador ———
sf::Sprite copiaJugador = _jugador.getSprite();
copiaJugador.setOrigin(0.f, 0.f);
copiaJugador.setScale(0.3f, 0.3f);

// Si no está en animación de ataque, lo pintamos siempre en battlePlayerPos
if (_jugador.getEstado() != personaje::estadoPersonaje::ataqueLigero)
{
    copiaJugador.setPosition(100.f, 600.f);
}
 // en caso de animar el ataque, getSprite() ya va moviéndose solo
window.draw(copiaJugador);

    // ——— Enemigo ———
    sf::Sprite copiaAdversario = _adversario.getSprite();
    copiaAdversario.setOrigin(0.f, 0.f);
    copiaAdversario.setScale(0.2f, 0.2f);
    copiaAdversario.setPosition(1000.f, 550.f);
    window.draw(copiaAdversario);

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
    // centrar el texto horizontalmente dentro del recuadro
    sf::FloatRect tb = textoMensaje.getLocalBounds();
    textoMensaje.setOrigin(tb.width/2.f, tb.height/2.f);
    textoMensaje.setPosition(
        cuadroMensaje.getPosition().x + cuadroMensaje.getSize().x/2.f,
        cuadroMensaje.getPosition().y + cuadroMensaje.getSize().y/2.f - 10.f
    );
    mensajeActivo = true;
}
