#include "batalla.h"


batalla::batalla(personaje& jugador, enemigo& adversario, sf::Sound& soundFlecha)
    : _jugador(jugador)
    , _adversario(adversario)
    , soundFlecha   (soundFlecha)
    , turnoActual(Turno::Jugador)
    , vidaJugador(jugador.getSalud())
    , vidaAdversario(adversario.getSalud())
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
        spriteFondo.setScale(0.9f, 0.85f);

    // Fuente y textos
    if (!fuente.loadFromFile("fonts/Rochester-Regular.ttf"))
        std::cout << "Error al cargar fuente de batalla\n";

   menuBatalla.crearMenu(numOpcionesMenuBatalla, fuente, opcionesVectorBatalla, 20, 120, 760, 20, sf::Color::White, sf::Color::Red);
    //actualizarTexto();


};

void batalla::manejarInput() {
    sf::Event event;
    int opcionSeleccionada = 0;
    // Solo aceptamos flechas en el turno del jugador
    if (turnoActual != Turno::Jugador) return;

    switch (event.key.code)
            {
            case sf::Keyboard::Up:
                opcionSeleccionada = (opcionSeleccionada - 1 + numOpcionesMenuBatalla) % numOpcionesMenuBatalla;
                soundFlecha.play();

                break;
            case sf::Keyboard::Down:
                opcionSeleccionada = (opcionSeleccionada + 1) % numOpcionesMenuBatalla;
                soundFlecha.play();
                break;
            case sf::Keyboard::Enter:
                //enter.play();
                switch (opcionSeleccionada)
                {
                case 0:  // Nueva Partida
                    std::cout<<"\nAtaque lijero";

                    break;

                case 1: //  continuar partida
                    std::cout<<"\nAtaque pesado";


                    break;

                case 2: //  record
                    std::cout<<"\nHabilidad especial";


                    break;

                default:

                    break;
                }
        //logica del enemigo =(
        turnoActual = Turno::Enemigo;
    }
    menuBatalla.actualizarMenu(opcionSeleccionada, sf::Color::Red, sf::Color::Black);
}


void batalla::drawBatalla(sf::RenderWindow& window)
{
    window.draw(spriteFondo);
    menuBatalla.dibujarMenu(window);
    sf::Sprite copiaJugador = _jugador.getSprite();
    copiaJugador.setScale(0.3f, 0.3f);

    copiaJugador.setPosition(70.f, 600.f);
    sf::Sprite copiaAdversario = _adversario.getSprite();
    copiaAdversario.setPosition(1000.f, 550.f);
    copiaAdversario.setScale(0.2f, 0.2f);
     window.draw(copiaJugador);
     window.draw(copiaAdversario);


};

bool batalla::finBatalla() const{

};
