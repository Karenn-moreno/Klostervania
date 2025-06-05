#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>   // Para sf::Clock
#include <string>
#include <iostream>

class personaje {
public:
    // 1) Constructor por defecto (mantenerlo para quien no quiera cargar texturas aqu�)
    personaje();

    // 2) Nuevo constructor: recibe posici�n, ruta a spritesheet y escala
    personaje(const sf::Vector2f& posInicial,
              const std::string& rutaSpritesheet,
              const sf::Vector2f& escala);

    virtual ~personaje() = default;  // Destructor virtual para herencia segura

    // --- Setters de estad�sticas ---
    void setSalud(int salud);               // Establece los puntos de vida
    void setAtaqueLigero(int ataqueLigero); // Establece el da�o de ataque ligero
    void setAtaquePesado(int ataquePesado); // Establece el da�o de ataque pesado
    void setHabilidadEspecial(int habilidadEspecial); // Establece el valor de la habilidad especial

    // --- Getters de estad�sticas ---
    int getSalud() const;                  // Devuelve los puntos de vida
    int getAtaqueLigero() const;           // Devuelve el da�o de ataque ligero
    int getAtaquePesado() const;           // Devuelve el da�o de ataque pesado
    int getHabilidadEspecial() const;      // Devuelve el valor de la habilidad especial

    // --- Acciones de combate (pueden sobrescribirse) ---
    virtual void ataqueLigero(const sf::Vector2f& destino);
    virtual void ataquePesado(const sf::Vector2f& destino);
    virtual void habilidadEspecial(const sf::Vector2f& destino);
    virtual void setFrameAtaque(int frame);

    // --- L�gica de juego (puede sobrescribirse) ---
    virtual void update(float deltaTime,
                        bool moviendoDer    = false,
                        bool moviendoIzq    = false,
                        bool moviendoArriba = false,
                        bool moviendoAbajo  = false);
    virtual void mover(float offsetX, float offsetY); // Mueve el sprite en pantalla
    virtual void detener();                           // Detiene la animaci�n y resetea frame

    // --- Renderizado (puede sobrescribirse) ---
    virtual void draw(sf::RenderWindow& window);      // Dibuja el personaje

    // --- Posici�n y escala gen�ricos ---
    virtual sf::Vector2f getPosition() const;
    virtual const sf::Sprite& getSprite() const;
    virtual sf::Sprite& getSprite();
    virtual sf::FloatRect getBounds() const;
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& pos);
    void setOrigin(float x, float y);

    // Sobrecargas para escala:
    //   a) versi�n que recibe dos floats
    void setScale(float x, float y);
    //   b) versi�n que recibe un Vector2f
    void setScale(const sf::Vector2f& s);

    // --- Estado de animaci�n ---
    enum class estadoPersonaje {
        quieto,
        caminando,
        caminandoArriba,
        caminandoAbajo,
        ataqueLigero,
        ataquePesado,
        habilidadEspecial,
        muerto
    };

    // --- Acceso al estado ---
    estadoPersonaje getEstado() const { return estado; }
    void setEstado(estadoPersonaje nuevoEstado);
    virtual bool estaAtacando();

protected:
    estadoPersonaje estado = estadoPersonaje::quieto;

    // --- Estad�sticas del personaje ---
    int _salud = 1000;
    int _ataqueLigero = 10;
    int _ataquePesado = 15;
    int _habilidadEspecial = 25;

    // --- Gr�ficos / Animaci�n ---
    sf::Sprite sprite;           // Sprite que representa al personaje
    sf::Texture textura;         // Textura completa del spritesheet
    sf::Sprite spriteProyectil;

    // --- Respiraci�n / pulso suave ---
    sf::Clock breathClock;       // Reloj para medir ciclo de respiraci�n
    float baseScaleX;            // Escala X original del sprite
    float baseScaleY;            // Escala Y original del sprite
    float breathAmplitude;       // Amplitud de variaci�n de escala (p.ej. 0.02)
    float breathSpeed;           // Velocidad de ciclo (p.ej. 0.5)

    // --- Animaci�n de sprites (frame) ---
    float speed = 50.f;
    int currentFrame = 0;
    float frameTime = 0.15f;
    float frameTimer = 0.f;
    int frameWidth = 500;
    int frameHeight = 500;
    sf::IntRect frameActual;
    int totalFrames = 6;
    static constexpr int filaFrameQuieto                = 6;
    static constexpr int cantidadFrameQuieto            = 1;
    static constexpr int filaFrameCaminar               = 0;
    static constexpr int cantidadFrameCaminar           = 6;
    static constexpr int filaFrameAtaqueLigero          = 1;
    static constexpr int cantidadFrameAtaqueLigero      = 6;
    static constexpr int filaFrameAtaquePesado          = 2;
    static constexpr int cantidadFrameAtaquePesado      = 6;
    static constexpr int filaFrameHabilidadEspecial     = 3;
    static constexpr int cantidadFrameHabilidadEspecial = 6;
    static constexpr int filaFrameCaminarAbajo          = 4;
    static constexpr int cantidadFrameCaminarAbajo      = 6;
    static constexpr int filaFrameCaminarArriba         = 5;
    static constexpr int cantidadFrameCaminarArriba     = 6;

    // --- Control de ataque interno ---
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
