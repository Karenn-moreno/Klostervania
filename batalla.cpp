#include "batalla.h"


batalla::batalla(personaje& jugador, enemigo& adversario, sf::Sound& soundFlecha)
    : _jugador(jugador)
    , _adversario(adversario)
    , soundFlecha   (soundFlecha)
    , turnoActual(Turno::Jugador)
    , vidaJugador(jugador.getSalud())
    , vidaAdversario(adversario.getSalud()-495) //le resto para matarlo rapido y seguir porbando
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
        case 0:  // Ataque lijero
            // 1) Aplico daño
            vidaAdversario -= _jugador.getAtaqueLigero();

            // 2) Refresco el texto de vidas
            //actualizarTexto();
            std::cout << "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene " << vidaAdversario << " de vida\n";
            turnoActual = Turno::Enemigo;
            _rondaCarga++;
            break;

        case 1: //  Ataque pesado
            if(_rondaCarga>=2)
            {
                vidaAdversario -= _jugador.getAtaquePesado();
                std::cout << "\n¡Jugador golpea con Ataque Ligero! Enemigo tiene " << vidaAdversario << " de vida\n";
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
    if (vidaAdversario <= 0 )
    {
        terminado       = true;
        _adversario.setActivo(false);
        jugadorGanoFlag = (vidaAdversario <= 0);
        // —— RECOMPENSA AL JUGADOR ——
        const int bonusSalud        = 50;
        const int bonusAtaqueLigero    =  5;
        const int bonusAtaquePesado    =  3;
        const int bonusHabilidadEspecial =  1;

        // Suma los bonus a los valores actuales:
        _jugador.setSalud( _jugador.getSalud() + bonusSalud );
        _jugador.setAtaqueLigero( _jugador.getAtaqueLigero() + bonusAtaqueLigero );
        _jugador.setAtaquePesado( _jugador.getAtaquePesado() + bonusAtaquePesado );
        _jugador.setHabilidadEspecial( _jugador.getHabilidadEspecial() + bonusHabilidadEspecial );

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

    menuBatalla.dibujarMenu(window);
// ——— Jugador ———
    sf::Sprite copiaJugador = _jugador.getSprite();
    // Resetear origen y escala limpias SIN ESTO EL DIBUJO ME PODIA QUEDAR AFUERA DE LA PANTALLA CUANDO MIRABA A LA IZQUIERDA
    copiaJugador.setOrigin(0.f, 0.f);
    copiaJugador.setScale(0.3f, 0.3f);
    copiaJugador.setPosition(70.f, 600.f);
    window.draw(copiaJugador);

    // ——— Enemigo ———
    sf::Sprite copiaAdversario = _adversario.getSprite();
    copiaAdversario.setOrigin(0.f, 0.f);
    copiaAdversario.setScale(0.2f, 0.2f);
    copiaAdversario.setPosition(1000.f, 550.f);
    window.draw(copiaAdversario);
};

bool batalla::ganador() const
{
    return jugadorGanoFlag;
}

bool batalla::finBatalla() const
{
    return terminado;
};
