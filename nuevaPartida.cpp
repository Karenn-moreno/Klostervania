#include "nuevaPartida.h"
#include "gamePlay.h"      // para conocer gamePlay::jugador

void gamePlay::iniciarNuevaPartida()
{
    // 1) Limpiar cualquier estado sobrante de partidas anteriores
    roster.clear();
    jugadorActivo = nullptr;

    // 2) Selección de personaje
    if (prototipos.size() == 1) {
        // Si solo hay un prototipo, lo tomamos
        auto elegido = prototipos[0];
        jugadorActivo = elegido;
    }
    else {
        // Si hay más de uno, mostramos un menú para elegir
        std::vector<std::string> nombres;
        for (int i = 0; i < (int)prototipos.size(); ++i) {
            nombres.push_back("Personaje " + std::to_string(i+1));
        }
        menu seleccionPersonaje;
        seleccionPersonaje.crearMenu(
            (int)nombres.size(),
            fuente,
            nombres,
            40,   // tamaño texto
            600,  // pos X
            400,  // pos Y
            55,   // espaciado vertical
            sf::Color::Black,
            sf::Color::Red
        );

        int opcion = 0;
        bool eligiendo = true;
        while (eligiendo) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::Up) {
                        opcion = (opcion - 1 + (int)nombres.size()) % (int)nombres.size();
                        flecha.play();
                    }
                    else if (ev.key.code == sf::Keyboard::Down) {
                        opcion = (opcion + 1) % (int)nombres.size();
                        flecha.play();
                    }
                    else if (ev.key.code == sf::Keyboard::Enter) {
                        enter.play();
                        // Esperar a que suelte Enter para no propagar la tecla
                        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {}
                        eligiendo = false;
                    }
                    seleccionPersonaje.actualizarMenu(opcion, sf::Color::Red, sf::Color::Black);
                }
            }
            window.clear(sf::Color::Black);
            seleccionPersonaje.dibujarMenu(window);
            window.display();
        }

        jugadorActivo = prototipos[opcion];
    }

    // 3) Poner al personaje activo en su posición de spawn y ajustar escala
    if (jugadorActivo) {
        jugadorActivo->setPosition({50.f, 500.f});
        jugadorActivo->setOrigin(0.f, 0.f);
        // Usamos la sobrecarga de personaje::setScale para que actualice baseScaleX/Y
        jugadorActivo->setScale(0.25f, 0.25f);
        jugadorActivo->setSalud(100);
        roster.push_back(jugadorActivo);
    }

    // 4) Volver a inicializar enemigos desde cero
    for (auto* e : enemigos) {
        delete e;
    }
    enemigos.clear();
    inicializarEnemigos();

    // 5) Asegurarnos de que no quede ningún estado de batalla pendiente
    if (batallaGamePlay) {
        delete batallaGamePlay;
        batallaGamePlay = nullptr;
    }
    batallaIniciada = false;

    // 6) Forzar modo Exploración para que dibuje el mapa en lugar de batalla
    estado = EstadoJuego::Exploracion;
}

