#include "item.h"
#include <cstdlib>
#include <cmath>
// Inicializar flag de semilla una sola vez en el juego
bool item::seeded = false;

item::item()
{
    // Carga de la textura
    if (!textura.loadFromFile("img/spritesheet_items.png"))
        std::cerr << "Error al cargar textura items" << std::endl;
    sprite.setTexture(textura);

    // Tamaño fijo de frame de 500x500
    frameWidth  = 500;
    frameHeight = 500;
    frameActual = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(frameActual);

    // Escala base
    sprite.setScale(baseScaleX, baseScaleY);

    // Seed aleatorio (solo una vez)
    if (!seeded)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    // Carga panel de recompensa
    if (!panelTexture.loadFromFile("img/panel_item.png"))
        std::cout << "Error al cargar panel_item.png\n";
    panelSprite.setTexture(panelTexture);
    panelSprite.scale(0.8f, 0.8f);
    // Centrar origen para fácil posicionamiento
    panelSprite.setOrigin(
        panelTexture.getSize().x/2.f,
        panelTexture.getSize().y/2.f
    );

    // Carga fuente y configura texto
    if (!panelFont.loadFromFile("fonts/Rochester-Regular.ttf"))
        std::cout << "Error al cargar fuente de letra carte del item\n";
    panelText.setFont(panelFont);
    panelText.setCharacterSize(20);
    panelText.setFillColor(sf::Color::Black);
    panelText.setStyle(sf::Text::Bold);
}

void item::spawn(const sf::Vector2u& windowSize)
{
// Asegura que al reaparecer el panel quede cerrado (ESTO CAUSO MUCHOS DOLORES DE CABEZA ALEATORIAMENTE!!)
   panelActive = false;
    //  Elige frame de sprite
    _tipoItem = std::rand() % totalFrames;
    frameActual.left = _tipoItem * frameWidth;
    sprite.setTextureRect(frameActual);
    //  Asigna tipo de bonus y guarda en bonusTipo
    bonusTipo = static_cast<BonusType>(_tipoItem);
    // Elige posición
    float x = static_cast<float>(std::rand() % (windowSize.x - frameWidth));
    float y = static_cast<float>(std::rand() % (windowSize.y - frameHeight));
    sprite.setPosition(x, y);
    //  Arranca reloj de vida y pulso
    lifeClock.restart();
    pulseClock.restart();
    active = true;
}

void item::update()
{
    if (!active) return;

    // Caducidad
    if (lifeClock.getElapsedTime() >= lifetime)
    {
        active = false;
        return;
    }
    // animacion del itel (latido)
    float t = pulseClock.getElapsedTime().asSeconds();
    float factor = 1.f + pulseAmplitude * std::sin(2.f * 3.14159265f * pulseSpeed * t);
    sprite.setScale(baseScaleX * factor, baseScaleY * factor);
}

bool item::tryPickup(personaje& jugador)
{
    if (!active) return false;

    if (sprite.getGlobalBounds().intersects(jugador.getBounds()))
    {
        active = false;  // desaparece del mapa
        panelActive = true; // activo el panel
            // Reinicia la animación de “pop”
    _popupScale = 0.f;

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

// Asigna al text
            panelText.setString(mensajeItem);
            break;
        case Ligero:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA COMUN, POTENCIA EL ATAQUE LIJERO!! \n";
            jugador.setAtaqueLigero(jugador.getAtaqueLigero() + valor);
            std::cout << "¡Ítem: Ataque Ligero! +" << valor
                      << " -> AL: " << jugador.getAtaqueLigero() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA COMUN, POTENCIA EL ATAQUE LIJERO!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque lijero\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaqueLigero())+ "!\n";
            panelText.setString(mensajeItem);
            break;
        case Pesado:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA RARA, POTENCIA EL ATAQUE PESADO!! \n";
            jugador.setAtaquePesado(jugador.getAtaquePesado() + valor);
            std::cout << "¡Ítem: Ataque Pesado! +" << valor
                      << " -> AP: " << jugador.getAtaquePesado() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA COMUN, POTENCIA EL ATAQUE PESADO!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de ataque pesado\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getAtaquePesado())+ "!\n";
            panelText.setString(mensajeItem);
            break;
        case Especial:
            std::cout << "\n¡!HAS ENCONTRADO UNA GEMA DE HABILIDAD!! \n";
            jugador.setHabilidadEspecial(jugador.getHabilidadEspecial() + valor);
            std::cout << "¡Ítem: Habilidad Especial! +" << valor
                      << " -> HE: " << jugador.getHabilidadEspecial() << "\n";
            mensajeItem  = "¡HAS ENCONTRADO UNA GEMA DE HABILIDAD!!\n\n";
            mensajeItem += "Te otorga " +std::to_string(valor)+" de habilidad especial\n";
            mensajeItem += "¡Ahora tienes " + std::to_string(jugador.getHabilidadEspecial())+ "!\n";
            panelText.setString(mensajeItem);
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
            panelText.setString(mensajeItem);
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
            panelText.setString(mensajeItem);
            break;
        }
        // Texto centrado
        std::string msg = mensajeItem;
        panelText.setString(msg);
        sf::FloatRect tb = panelText.getLocalBounds();
        panelText.setOrigin(tb.width/2.f, tb.height/2.f);
        return true;
    }
    return false;
}

void item::draw(sf::RenderWindow& window)
{
    if (active)
        window.draw(sprite);

    if (panelActive)
    {
        // —–– Pop-up  –––
        const float targetScale = 0.8f; //la escala máxima a la que queremos que llegue
        const float step        = 0.1f; //la escala en cada frame
        _popupScale = std::min(_popupScale + step, targetScale);    //std::min asi nunca supera el valor
        panelSprite.setScale(_popupScale, _popupScale);
        panelText  .setScale(_popupScale, _popupScale);

        panelSprite.setPosition(
            window.getSize().x/2.f,
            window.getSize().y/2.f
        );
        panelText.setPosition(
            window.getSize().x/2.f,
            window.getSize().y/2.f - 20.f
        );
        window.draw(panelSprite);
        window.draw(panelText);
    }
}

void item::handleEvent(const sf::Event& event)
{
    if (!panelActive)
        return;

    if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Enter)
    {
        panelActive = false;
    }
}

bool item::isPanelActive() const
{
    return panelActive;
}
