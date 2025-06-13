#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>        // <— necesario para std::vector
#include <string>        // <— necesario para std::string
#include "personaje.h"
#include "enemigo.h"
#include "menu.h"
#include "popUpCartel.h"

/// Gestiona un combate por turnos entre un jugador y un enemigo.
class batalla {
public:

    batalla(personaje& jugador,
            const std::vector<enemigo*>& adversarios,
            sf::Sound& soundFlecha);

    /// Inicializa las variables de la batalla y carga todos los recursos.
    void iniciarBatalla(sf::RenderWindow& window);

    /// Procesa la entrada del jugador en su turno (flechas + Enter).
    void manejarInput();

    /// Actualiza la lógica (turnos, daños, detección de victoria/derrota).
    void actualizar(float deltaTime);

    /// Dibuja la batalla (fondo, menú, sprites, y pop-up final).
    void drawBatalla(sf::RenderWindow& ventana);

    /// Devuelve true cuando la batalla ha terminado **y** ya se mostró el pop-up final.
    bool finBatalla() const;

    /// Indica si el jugador ganó (true) o perdió (false).
    bool ganador() const;

    /// Permite a quien instancie la batalla saber si el pop-up de fin de batalla está activo.
    bool batallaPopupActive() const;

    /// Pop-up para mostrar el mensaje de “¡Victoria!” o “Derrota”.
    PopUpCartel popupFinBatalla;

    /// Muestra un mensaje en el HUD de batalla ("¡Enemigo ataca!", etc.).
    void mostrarMensaje(const std::string& msg);

private:
    personaje&                      _jugador;          // Referencia al jugador original
    std::vector<enemigo*>           _adversarios;      // Vector de punteros a enemigos
    sf::Vector2f                    _posEnemigoInicial;// Para guardar la posición original
    sf::Sound&                      soundFlecha;       // Sonido de flecha para el menú

    enum class Turno { Jugador, Enemigo };
    Turno                        turnoActual       = Turno::Jugador;
    int                          _opcionSeleccionada = 0;

    int                          vidaJugador;       // Vida en combate
    int                          vidaAdversario;    // Vida en combate del primer enemigo

    int                          _rondaTurno    = 1;
    int                          _rondaCarga    = 0; // Para desbloquear ataques / habilidades

    bool                         terminado         = false;  // Si finalizó la batalla
    bool                         jugadorGanoFlag   = false;  // Resultado final
    bool                         popupFinMostrado  = false;  // Para no re-mostrar el pop-up
    bool                         victoriaIniciada  = false;  // Para retrasar 3s tras victoria

    // — Recursos Gráficos —
    sf::RectangleShape pantallaNegraFade;
    sf::RectangleShape           fondo;             // Rectángulo base (no siempre se usa)
    sf::Texture                  texturaFondo;      // Carga de "img/fondoBatalla.jpg"
    sf::Sprite                   spriteFondo;       // Sprite para dibujar fondo
    sf::Font                     fuente;            // Fuente para el menú
    bool desvaneciendo =false;
    float alphaFade    = 0.f;       // valor actual de opacidad (0..255)
    float fadeSpeed    =60.f;     // pixeles que sumamos por segundo
    // — Menú de batalla (3 opciones) —
    menu                         menuBatalla;
    static constexpr int         numOpcionesMenuBatalla = 3;
    std::vector<std::string>     opcionesVectorBatalla  =
        {"Ataque Ligero", "Ataque Pesado", "Habilidad Especial"};

    // — Sonidos de cada ataque (opcional) —
    sf::SoundBuffer              bufferGolpeLigero;
    sf::SoundBuffer              bufferGolpePesado;
    sf::SoundBuffer              bufferHabilidadEspecial;
    sf::Sound                     golpeLigero;
    sf::Sound                     golpePesado;
    sf::Sound                     habilidadEspecial;

    // —mensajes en la batalla —
    sf::Font                     fuenteMensaje;
    sf::Text                     textoMensaje;
    bool                         mensajeActivo    = false;
    std::string                  msj;

    // — Mensaje final (victoria o derrota) —
    std::string                  mensajeFinBatalla;

    ///  Si quieres actualizar texto en cada turno, defínelo aquí.
   // void actualizarTexto();

    // ————— Transición —————
    //sf::RectangleShape pantallaNegra;
    void fadeIn(sf::RenderWindow& window);
};
