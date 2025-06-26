#include "CheckPoint.h"
#include "ArchivoPuntaje.h"
#include <iostream>

CheckPoint::CheckPoint()
    : activo(true), popup()
{
    if (!textura.loadFromFile("img/libro.png")) {
        std::cerr << "Error al cargar la textura del libro\n";
    } else {
        std::cout << "Libro cargado correctamente\n";
        sprite.setTexture(textura);
    }

    sprite.setScale(0.05f, 0.05f);
    popup.cargarRecursos("img/panel_item.png", "fonts/Rochester-Regular.ttf");
}

void CheckPoint::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f CheckPoint::getPosition() const
{
    return sprite.getPosition();
}

void CheckPoint::update(const personaje& jugador, const sf::Vector2u& windowSize)
{
    if (!activo) return;

    if (sprite.getGlobalBounds().intersects(jugador.getBounds()))
    {
        std::cout << "Colisión con CheckPoint detectada\n";
        ArchivoPuntaje archivo("puntos.dat");

        if (archivo.guardarPartida(jugador, getPosition()))
        {
            std::string texto = "¡Partida guardada exitosamente!\n\n"
                                "Personaje: " + jugador.getNombre() + "\n"
                                "Salud: " + std::to_string(jugador.getSalud()) + "\n"
                                "Ataques: " + std::to_string(jugador.getAtaqueLigero()) + "/" +
                                std::to_string(jugador.getAtaquePesado()) + "\n"
                                "Especial: " + std::to_string(jugador.getHabilidadEspecial());

            popup.mostrar(texto, windowSize);
        }
        else
        {
            std::cerr << "No se pudo guardar la partida\n";
        }
    }
}

void CheckPoint::draw(sf::RenderWindow& window)
{
    window.draw(sprite);

    if (popup.isActive())
        popup.draw(window);
}

void CheckPoint::handleEvent(const sf::Event& event)
{
    popup.handleEvent(event);
}

bool CheckPoint::isPopupActive() const
{
    return popup.isActive();
}
