#include "item.h"
#include <cstdlib>
#include <cmath>

// Inicializar flag de semilla una sola vez en el juego
bool item::seeded = false;

item::item()
  : _popup()   // construye su PopUpCartel propio
{
    // 1) texturas del ítem
    if (!textura.loadFromFile("img/spritesheet_items.png"))
        std::cerr << "Error cargando spritesheet_items.png\n";
    sprite.setTexture(textura);

    frameActual = {0,0,frameWidth,frameHeight};
    sprite.setTextureRect(frameActual);
    sprite.setScale(baseScaleX, baseScaleY);

    // 2) inicializar semilla
    if (!seeded) {
        std::srand((unsigned)std::time(nullptr));
        seeded = true;
    }

    // 3) cargar recursos del popup (fondo + fuente)
    _popup.cargarRecursos("img/panel_item.png",
                          "fonts/Rochester-Regular.ttf");
}

void item::spawn(const sf::Vector2u& windowSize)
{
    // Guardamos para el popup
    _windowSize = windowSize;

    // Reseteamos
    active = true;

    // Frame aleatorio
    _tipoItem = std::rand() % totalFrames;
    frameActual.left = _tipoItem * frameWidth;
    sprite.setTextureRect(frameActual);

    bonusTipo = static_cast<BonusType>(_tipoItem);

    // Posición aleatoria en pantalla
    float x = static_cast<float>(std::rand() % (windowSize.x - frameWidth));
    float y = static_cast<float>(std::rand() % (windowSize.y - frameHeight));
    sprite.setPosition(x, y);

    lifeClock.restart();
    pulseClock.restart();
}

void item::update()
{
    if (!active) return;

    // Caducidad
    if (lifeClock.getElapsedTime() >= lifetime) {
        active = false;
        return;
    }
    // pulso de latido
    float t      = pulseClock.getElapsedTime().asSeconds();
    float factor = 1.f + pulseAmplitude * std::sin(2.f * 3.14159265f * pulseSpeed * t);
    sprite.setScale(baseScaleX * factor, baseScaleY * factor);
}

bool item::tryPickup(personaje& jugador)
{
    if (!active) return false;

    if (sprite.getGlobalBounds().intersects(jugador.getBounds()))
    {
        active = false;  // desaparece del mapa
            // Reinicia la animación de “pop”

        // Aplica bonus según bonusTipo
        int valor = BONUS_VAL[bonusTipo];
        std::string mensajeItem;
        switch (bonusTipo)
        {
        case Salud:
            std::cout << "\n¡!HAS ENCONTRADO UN CORAZON SAGRADO, AUMENTA TU SALUD!! \n";
            jugador.setSalud(jugador.getSalud() + valor);
            std::cout << "¡Ítem: Salud! +" << valor
                      << " -> Salud: " << jugador.getSalud() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UN CORAZÓN SAGRADO, AUMENTA TU SALUD!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de salud\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getSalud())+ "!\n";
            _popup.mostrar(mensajeItem, _windowSize);
            break;
        case Ligero:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA COMUN, POTENCIA EL ATAQUE LIJERO!! \n";
            jugador.setAtaqueLigero(jugador.getAtaqueLigero() + valor);
            std::cout << "¡Ítem: Ataque Ligero! +" << valor
                      << " -> AL: " << jugador.getAtaqueLigero() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA COMUN, POTENCIA EL ATAQUE LIJERO!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque lijero\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaqueLigero())+ "!\n";
            _popup.mostrar(mensajeItem, _windowSize);
            break;
        case Pesado:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA RARA, POTENCIA EL ATAQUE PESADO!! \n";
            jugador.setAtaquePesado(jugador.getAtaquePesado() + valor);
            std::cout << "¡Ítem: Ataque Pesado! +" << valor
                      << " -> AP: " << jugador.getAtaquePesado() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA COMUN, POTENCIA EL ATAQUE PESADO!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque pesado\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaquePesado())+ "!\n";
            _popup.mostrar(mensajeItem, _windowSize);
            break;
        case Especial:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA DE HABILIDAD!! \n";
            jugador.setHabilidadEspecial(jugador.getHabilidadEspecial() + valor);
            std::cout << "¡Ítem: Habilidad Especial! +" << valor
                      << " -> HE: " << jugador.getHabilidadEspecial() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA DE HABILIDAD!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de habilidad especial\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getHabilidadEspecial())+ "!\n";
            _popup.mostrar(mensajeItem, _windowSize);
            break;
        case ataques:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA MITICA, POTENCIADOR DE ATAQUES!! \n";
            jugador.setAtaqueLigero(jugador.getAtaqueLigero() + valor);
            std::cout << "¡Ítem: Ataque Ligero! +" << valor
                      << " -> AL: " << jugador.getAtaqueLigero() << "\n";
            jugador.setAtaquePesado(jugador.getAtaquePesado() + valor);
            std::cout << "¡Ítem: Ataque Pesado! +" << valor
                      << " -> AP: " << jugador.getAtaquePesado() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA MITICA, POTENCIADOR DE ATAQUES!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque lijero\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaqueLigero())+ "!\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque pesado\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaquePesado())+ "!\n";
            _popup.mostrar(mensajeItem, _windowSize);
            break;
        case todos:
            std::cout << "\n¡!HAS ENCONTRADO UNA ESFERA MITICA DE PODER, SUBE TODAS TUS ESTADISTICAS!! \n";
            jugador.setSalud(jugador.getSalud() + valor);
            std::cout << "¡Ítem: Salud! +" << valor
                      << " -> Salud: " << jugador.getSalud() << "\n";
            jugador.setAtaqueLigero(jugador.getAtaqueLigero() + valor);
            std::cout << "¡Ítem: Ataque Ligero! +" << valor
                      << " -> AL: " << jugador.getAtaqueLigero() << "\n";
            jugador.setAtaquePesado(jugador.getAtaquePesado() + valor);
            std::cout << "¡Ítem: Ataque Pesado! +" << valor
                      << " -> AP: " << jugador.getAtaquePesado() << "\n";
            jugador.setHabilidadEspecial(jugador.getHabilidadEspecial() + valor);
            std::cout << "¡Ítem: Habilidad Especial! +" << valor
                      << " -> HE: " << jugador.getHabilidadEspecial() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA ESFERA MITICA DE PODER \n SUBE TODAS TUS ESTADISTICAS!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de salud\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getSalud())+ "!\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque lijero\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaqueLigero())+ "!\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque pesado\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaquePesado())+ "!\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de habilidad especial\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getHabilidadEspecial())+ "!\n";
            _popup.mostrar(mensajeItem, _windowSize);
            break;
        }
        // Mostramos el mensaje centrado
        _popup.mostrar(mensajeItem, _windowSize);
        return true;
    }
    return false;
}

void item::draw(sf::RenderWindow& window)
{
    if (active)
        window.draw(sprite);

    if (_popup.isActive())
        _popup.draw(window);
}

bool item::isPopupActive() const
{
    return _popup.isActive();
}

void item::handleEvent(const sf::Event& event)
{
    // Si el pop-up está activo y procesa un Enter, handleEvent() devuelve true
    if (_popup.handleEvent(event)) {

    }
}
