
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "personaje.h"
#include "enemigo.h"
#include "menu.h"

//      Gestiona un combate por turnos entre un jugador y un enemigo.
class batalla {
public:

    /// Constructor: recibe referencias al jugador y al enemigo que van a pelear.
    batalla(personaje& jugador, enemigo& adversario, sf::Sound& soundFlecha);

    /// Inicializa las variables de la batalla y cambia el estado interno.
    void iniciarBatalla();

    /// Procesa la entrada del turno (teclas A=ataque, D=defensa, etc.).
    void manejarInput();

    /// Actualiza la lógica de la batalla (cambio de turno, daños, fin de combate).
    void actualizar(float deltaTime);

    /// Dibuja la pantalla de batalla (fondo, vidas, menú de acciones).
    void drawBatalla(sf::RenderWindow& ventana);

    /// Devuelve true cuando la batalla ha terminado.
    bool finBatalla() const;

    /// Indica si el jugador ha ganado (true) o perdido (false).
    bool ganador() const;

private:
    personaje& _jugador;      // referencia al jugador original
    enemigo&    _adversario;  // referencia al enemigo original

    enum class Turno { Jugador, Enemigo };
    Turno turnoActual;
    int _opcionSeleccionada = 0; // se conserva entre frames

    int vidaJugador;          // copia de la vida para el combate
    int vidaAdversario;       // copia de la vida para el combate
    int _rondaTurno=1;
    int _rondaCarga = 0;      // para activar los diferentes ataques y habilidades

    bool terminado;           // indica si acabó la batalla
    bool jugadorGanoFlag;     // resultado final

// ————— Recursos gráficos —————
  sf::RectangleShape fondo;         // fondo simple
  sf::Texture         texturaFondo; // textura para cargar la jpg
  sf::Sprite          spriteFondo;  // sprite para esa textura
  sf::Font            fuente;

// ————— Transición y menú —————
    sf::RectangleShape pantallaNegra;
    menu               menuBatalla;
    static constexpr int numOpcionesMenuBatalla = 3;
    std::vector<std::string> opcionesVectorBatalla = {"Ataque ligero", "Ataque Pesado", "Habilidad Especial"};

// ————— Sonidos —————
    sf::Sound& soundFlecha;
    sf::SoundBuffer bufferGolpeLijero;
    sf::SoundBuffer bufferGolpePesado;
    sf::SoundBuffer bufferHabilidadEspecial;
    sf::Sound       golpeLijero;
    sf::Sound       golpePesado;
    sf::Sound       habilidadEspecial;

    /// Resetea el texto de estado (vidas y acciones disponibles).
    void actualizarTexto();
};
