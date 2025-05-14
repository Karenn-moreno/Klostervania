#pragma once
#include <SFML/Graphics.hpp>
#include "personaje.h"
#include "enemigo.h"
#include "menu.h"
#include "item.h"
#include <SFML/Audio.hpp>

class gamePlay
{
public:
    gamePlay();             // Constructor: crea ventana, carga entidades
    void ejecutar();     // Bucle principal de juego
private:
    void procesarEventos();          // Manejo de eventos puntuales
    void manejarEntradaJugador();    // Teclas mantenidas (movimiento)
    void updatePersonaje(sf::Time dt);    // Lógica de juego
    void draw();                  // Renderizado
    sf::Clock reloj;     // Para delta time

    // ————— Ventana principal —————
    sf::RenderWindow window;

// ————— Fondos y sprites —————
    sf::Texture       fondoPrincipal;
    sf::Sprite        spriteFondo;
    sf::Texture       fondoNuevaPartida;
    sf::Sprite        spriteNuevaPartida;

// ————— Transición y menú —————
    sf::RectangleShape pantallaNegra;
    sf::Font           fuente;
    menu               menuPrincipal;
    static constexpr int numOpcionesMenuPrincipal = 5;
    std::vector<std::string> opcionesVector = {"Nueva Partida", "Continuar Partida", "Record", "Creditos", "Salir"};

// ————— Sonidos —————
    sf::SoundBuffer bufferFlecha;
    sf::SoundBuffer bufferEnter;
    sf::Sound       flecha;
    sf::Sound       enter;

// ————— Entidades del juego —————
    personaje jugador;
    enemigo    demonio;
    item       itemRecolectable;

// ————— Flags de estado —————
    bool ejecutando     = false;
    bool juegoIniciado  = false;
    int  opcionSeleccionada = 0;

};
