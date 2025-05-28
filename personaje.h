#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>   // Para sf::Clock
#include <string>
#include <iostream>

class personaje {
public:
    personaje();
    virtual ~personaje() = default;  // Destructor virtual para herencia segura

    // --- Setters de estadísticas ---
    void setSalud(int salud);               // Establece los puntos de vida
    void setAtaqueLigero(int ataqueLigero); // Establece el daño de ataque ligero
    void setAtaquePesado(int ataquePesado); // Establece el daño de ataque pesado
    void setHabilidadEspecial(int habilidadEspecial); // Establece el valor de la habilidad especial

    // --- Getters de estadísticas ---
    int getSalud() const;                  // Devuelve los puntos de vida
    int getAtaqueLigero() const;           // Devuelve el daño de ataque ligero
    int getAtaquePesado() const;           // Devuelve el daño de ataque pesado
    int getHabilidadEspecial() const;      // Devuelve el valor de la habilidad especial
    sf::Vector2f getPosition() const;
    const sf::Sprite& getSprite() const;   // Devuelve el sprite para consultar bounds o dibujarlo
    sf::FloatRect getBounds() const;       // Devuelve el rectángulo global (para colisiones)

    // --- Acciones de combate (pueden sobrescribirse) ---
    virtual void ataqueLigero(const sf::Vector2f& destino);
    virtual void ataquePesado(const sf::Vector2f& destino);
    virtual void habilidadEspecial(const sf::Vector2f& destino);

    // --- Lógica de juego (puede sobrescribirse) ---
    virtual void update(float deltaTime,
                        bool moviendoDer=false,
                        bool moviendoIzq=false,
                        bool moviendoArr=false,
                        bool moviendoAbj=false);
    virtual void mover(float offsetX, float offsetY); // Mueve el sprite en pantalla
    virtual void detener();                         // Detiene la animación y resetea frame

    // --- Renderizado (puede sobrescribirse) ---
    virtual void draw(sf::RenderWindow& window);    // Dibuja el personaje en la ventana

    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& pos);
    void setOrigin(float x, float y);
    void setScale(float x, float y);

    // --- Estado de animación ---
    enum class estadoPersonaje { quieto, caminando, ataqueLigero, ataquePesado, habilidadEspecial, muerto };

    // --- Acceso al estado ---
    estadoPersonaje getEstado() const { return estado; }
    void setEstado(estadoPersonaje nuevoEstado) { estado = nuevoEstado; }
    bool estaAtacando();

protected:
    estadoPersonaje estado = estadoPersonaje::quieto;
    // --- Estadísticas del personaje ---
    int _salud = 1000;           // Puntos de vida actuales
    int _ataqueLigero = 10;      // Daño de ataque ligero
    int _ataquePesado = 15;      // Daño de ataque pesado
    int _habilidadEspecial = 25; // Potencia de la habilidad especial

    // --- Gráficos ---
    sf::Sprite sprite;           // Sprite que representa al personaje
    sf::Texture textura;         // Textura completa del spritesheet
    sf::Sprite spriteProyectil;

    // --- Respiración / pulso suave ---
    sf::Clock breathClock;       // Reloj para medir ciclo de respiración
    float baseScaleX;            // Escala X original del sprite
    float baseScaleY;            // Escala Y original del sprite
    float breathAmplitude;       // Amplitud de variación de escala
    float breathSpeed;           // Velocidad de ciclo

    // --- Animación de sprites ---
    int currentFrame = 0;
    float frameTime = 0.15f;
    float frameTimer = 0.f;
    int frameWidth = 500;
    int frameHeight = 500;
    sf::IntRect frameActual;
    int totalFrames = 6;
    static constexpr int filaFrameQuieto               = 6;
    static constexpr int cantidadFrameQuieto           = 1;
    static constexpr int filaFrameCaminar              = 0;
    static constexpr int cantidadFrameCaminar          = 1;
    static constexpr int filaFrameAtaqueLigero         = 1;
    static constexpr int cantidadFrameAtaqueLigero     = 6;
    static constexpr int filaFrameAtaquePesado         = 2;
    static constexpr int cantidadFrameAtaquePesado     = 6;
    static constexpr int filaFrameHabilidadEspecial    = 3;
    static constexpr int cantidadFrameHabilidadEspecial= 6;

    // --- Control de ataque ---
    int ataqueFase = 0;
    sf::Vector2f ataqueStartPos = {100.f, 600.f};
    sf::Vector2f ataqueTargetPos;
    static constexpr float ataqueSpeed = 1200.f;
    bool ataqueLlegado = false;
    bool atacando = false;

    // --- Proyectil ---
    bool proyectilActivo = false;
    int projectileFrame = 0;

};
