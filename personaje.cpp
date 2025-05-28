#include "personaje.h"
#include <cmath>  // Para std::sin y constantes matemáticas

// --- Setters de estadísticas ---
void personaje::setSalud(int salud) {
    _salud = salud;  // Actualiza puntos de vida
}

void personaje::setAtaqueLigero(int ataqueLigero) {
    _ataqueLigero = ataqueLigero;  // Actualiza daño ataque ligero
}

void personaje::setAtaquePesado(int ataquePesado) {
    _ataquePesado = ataquePesado;  // Actualiza daño ataque pesado
}

void personaje::setHabilidadEspecial(int habilidadEspecial) {
    _habilidadEspecial = habilidadEspecial;  // Actualiza poder habilidad especial
}

// --- Getters de estadísticas ---
int personaje::getSalud() const {
    return _salud;  // Devuelve puntos de vida actuales
}

int personaje::getAtaqueLigero() const {
    return _ataqueLigero;  // Devuelve daño de ataque ligero
}

int personaje::getAtaquePesado() const {
    return _ataquePesado;  // Devuelve daño de ataque pesado
}

int personaje::getHabilidadEspecial() const {
    return _habilidadEspecial;  // Devuelve valor de habilidad especial
}

const sf::Sprite& personaje::getSprite() const{
    return sprite;
};

sf::FloatRect personaje::getBounds() const
    {
        return sprite.getGlobalBounds();
    };

// Constructor: carga textura, configura sprite, frame y respiración
personaje::personaje()
    : _salud(1000)
    , _ataqueLigero(10)
    , _ataquePesado(15)
    , _habilidadEspecial(25)
    , totalFrames(6)
    , currentFrame(0)
    , frameTime(0.15f)
    , frameTimer(0.f)
    , frameWidth(500)
    , frameHeight(500)
{
    // 1) Cargar spritesheet
    if (!textura.loadFromFile("img/spritesheet_guerrero.png")) {
        std::cerr << "Error al cargar la textura del personaje\n";
    }
    sprite.setTexture(textura);

    // 2) Primer frame
    frameActual = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(frameActual);

    // 3) Escala y posición inicial
    sprite.setScale(0.25f, 0.25f);
    sprite.setPosition(50.f, 400.f);
    spriteProyectil.setTexture(textura);
    spriteProyectil.setOrigin( sprite.getOrigin() );

    // 4) Inicializar respiración
    baseScaleX      = sprite.getScale().x;
    baseScaleY      = sprite.getScale().y;
    breathAmplitude = 0.02f;
    breathSpeed     = 0.25f;
    breathClock.restart();
}

// Mueve el sprite según los desplazamientos dados
void personaje::mover(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

// Detiene la animación, resetea frame y temporizador
void personaje::detener() {
    currentFrame = 0;
    frameTimer   = 0.f;
    sprite.setTextureRect(frameActual);
}

// Dibujo: personaje y posible proyectil
void personaje::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (proyectilActivo)
        window.draw(spriteProyectil);
}

// --- Lógica de actualización: animación, movimiento y respiración ---
void personaje::update(float deltaTime,
                       bool movDer,
                       bool movIzq,
                       bool movArr,
                       bool movAbj)
{
    // A) Secuencia de ataque ligero
if (estado == estadoPersonaje::ataqueLigero) {
        sprite.setScale(0.35f, 0.35f);
        // Si el destino (jugador) está a la izquierda, espejo horizontal:
    if (ataqueTargetPos.x < ataqueStartPos.x) {
        sprite.setScale(-baseScaleX, baseScaleY);
        sprite.setOrigin(frameWidth, 0);
    } else {
        sprite.setScale( baseScaleX, baseScaleY);
        sprite.setOrigin(0, 0);
    }
    // — Fase de movimiento —
    if (!ataqueLlegado) {
        // siempre dibuja el primer frame de la fila de ataque:
        frameActual = { 0,
                       filaFrameAtaqueLigero * frameHeight,
                       frameWidth, frameHeight };
        sprite.setTextureRect(frameActual);

        // mueve hacia el target a velocidad constante
        sf::Vector2f dir = ataqueTargetPos - sprite.getPosition();
        float dist       = std::hypot(dir.x, dir.y);
        if (dist > ataqueSpeed * deltaTime) {
            dir /= dist;  // normalizar
            sprite.move(dir * ataqueSpeed * deltaTime);
        } else {
            // llegó definitivamente
            sprite.setPosition(ataqueTargetPos);
            ataqueLlegado = true;
            currentFrame  = 0;    // arrancamos los frames
            frameTimer    = 0.f;
        }
    }
    // — Fase de animación de frames —
    else if (currentFrame < cantidadFrameAtaqueLigero) {
        frameTimer += deltaTime;
        if (frameTimer >= frameTime) {
            frameTimer -= frameTime;
            currentFrame++;
        }
        frameActual = { currentFrame * frameWidth,
                        filaFrameAtaqueLigero * frameHeight,
                        frameWidth, frameHeight };
        sprite.setTextureRect(frameActual);
    }
    // — Fase de reset —
    else {
        sprite.setPosition(ataqueStartPos);
        estado = estadoPersonaje::quieto;
    }

    return; // ya procesamos ataque completo
}
// Ataque Pesado
    if (estado == estadoPersonaje::ataquePesado) {
        sprite.setScale(0.35f, 0.35f);
        // Si el destino (jugador) está a la izquierda, espejo horizontal:
    if (ataqueTargetPos.x < ataqueStartPos.x) {
        sprite.setScale(-baseScaleX, baseScaleY);
        sprite.setOrigin(frameWidth, 0);
    } else {
        sprite.setScale( baseScaleX, baseScaleY);
        sprite.setOrigin(0, 0);
    }
        if (!ataqueLlegado) {
            frameActual = sf::IntRect(0, filaFrameAtaquePesado * frameHeight, frameWidth, frameHeight);
            sprite.setTextureRect(frameActual);
            sf::Vector2f dir = ataqueTargetPos - sprite.getPosition();
            float dist = std::hypot(dir.x, dir.y);
            if (dist > ataqueSpeed * deltaTime) {
                dir /= dist;
                sprite.move(dir * ataqueSpeed * deltaTime);
            } else {
                sprite.setPosition(ataqueTargetPos);
                ataqueLlegado = true;
                currentFrame   = 0;
                frameTimer     = 0.f;
            }
        }
        else if (currentFrame < cantidadFrameAtaquePesado) {
            frameTimer += deltaTime;
            if (frameTimer >= frameTime) {
                frameTimer -= frameTime;
                currentFrame++;
            }
            frameActual = sf::IntRect(currentFrame * frameWidth, filaFrameAtaquePesado * frameHeight, frameWidth, frameHeight);
            sprite.setTextureRect(frameActual);
        }
        else {
            sprite.setPosition(ataqueStartPos);
            estado = estadoPersonaje::quieto;
            atacando     = false;
        }
        return;
    }
    // Habilidad Especial
    if (estado == estadoPersonaje::habilidadEspecial) {
// Fase 0: MOVER EL PROYECTIL

        if (ataqueFase == 0 && !ataqueLlegado) {
        // ─── ORIENTAR PROYECTIL ───
        // Calcula la dirección al target
        sf::Vector2f dir0 = ataqueTargetPos - spriteProyectil.getPosition();
        // Decide signo según esté a la derecha o izquierda
        float signoX = (dir0.x >= 0.f) ? +1.f : -1.f;
        // Ajusta escala horizontal y origen
        spriteProyectil.setScale(signoX * baseScaleX*3.f, baseScaleY*5.f);
        spriteProyectil.setOrigin((signoX<0? frameWidth:0.f), 0.f);
         // fijamos el frame 2 del spritesheet (columna 1)
        projectileFrame = 1;
        spriteProyectil.setTextureRect(
            sf::IntRect(
                projectileFrame * frameWidth,
                filaFrameHabilidadEspecial * frameHeight,
                frameWidth,
                frameHeight
            )
        );

        sf::Vector2f dir = ataqueTargetPos - spriteProyectil.getPosition();
        float dist       = std::hypot(dir.x, dir.y);
        if (dist > ataqueSpeed * deltaTime) {
            dir /= dist;  // normalizar
            spriteProyectil.move(dir * ataqueSpeed * deltaTime);
        } else {
            spriteProyectil.setPosition(ataqueTargetPos);
            ataqueLlegado   = true;
            ataqueFase      = 1;
            projectileFrame = 1;
            frameTimer      = 0.f;
        }
    }
        // Fase 1: ANIMAR FRAMES 2..6 DEL PROYECTIL
        else if (ataqueFase == 1 && projectileFrame < cantidadFrameHabilidadEspecial) {

            frameTimer += deltaTime*1.8;
            if (frameTimer >= frameTime) {
                frameTimer -= frameTime;
                projectileFrame++;
            }
            spriteProyectil.setTextureRect(
                {projectileFrame * frameWidth,
                 filaFrameHabilidadEspecial * frameHeight,
                 frameWidth,
                 frameHeight}
            );
        }
        // Fase 2: RESET
        else {
            estado    = estadoPersonaje::quieto;
            atacando        = false;
            proyectilActivo = false;
            // devuelve al personaje a su posición original
            sprite.setPosition(ataqueStartPos);
        }

        return;
    }
    // B) Animación de caminata
    bool caminando = movDer || movIzq;
    if (caminando) {
        estado = estadoPersonaje::caminando;
        // Incrementar temporizador
        frameTimer += deltaTime;
if (frameTimer >= frameTime) {
    frameTimer = 0.f;       // descartas el exceso
    currentFrame = (currentFrame + 1) % totalFrames;
}
        // Seleccionar frame en spritesheet (fila 0)
        int left = currentFrame * frameWidth;
        sprite.setTextureRect(sf::IntRect(left, 0, frameWidth, frameHeight));
        // Ajustar orientación según dirección

        //sprite.setTextureRect(frameActual);

        // Flip horizontal
        if (movIzq) {
            sprite.setScale(-baseScaleX, baseScaleY);
            sprite.setOrigin(frameWidth, 0);
        } else {
            sprite.setScale(baseScaleX, baseScaleY);
            sprite.setOrigin(0, 0);
        }
    }
    else {
        // C) Estado quieto
        estado = estadoPersonaje::quieto;
        atacando=false;
        currentFrame    = 0;
        frameActual     = {
            0,
            filaFrameQuieto * frameHeight,
            frameWidth,
            frameHeight
        };
        sprite.setTextureRect(frameActual);
    }

    // D) Respiración / pulso
    float t = breathClock.getElapsedTime().asSeconds();
    float factor = 1.f + breathAmplitude * std::sin(2.f * 3.14159265f * breathSpeed * t);
    float signX = (sprite.getScale().x < 0) ? -1.f : 1.f;
    sprite.setScale(signX * baseScaleX * factor,
                    baseScaleY * factor);
}

void personaje::ataqueLigero(const sf::Vector2f& destino) {
    if (estado != estadoPersonaje::ataqueLigero) {
        estado   = estadoPersonaje::ataqueLigero;
        //1) Guarda la posición actual
        ataqueStartPos = sprite.getPosition();
        atacando=true;
        currentFrame      = 0;
        frameTimer        = 0.f;
        ataqueFase        = 0;                    // empezamos fase de movimiento
        ataqueTargetPos   = destino;              // destino fijo
        ataqueLlegado     = false;   // <-- reiniciamos aquí
    }
}

// Ataque Pesado
void personaje::ataquePesado(const sf::Vector2f& destino) {
    if (estado != estadoPersonaje::ataquePesado) {
        estado  = estadoPersonaje::ataquePesado;
        //1) Guarda la posición actual
        ataqueStartPos = sprite.getPosition();
        atacando       = true;
        currentFrame   = 0;
        frameTimer     = 0.f;
        ataqueFase     = 0;
        ataqueTargetPos= destino;
        ataqueLlegado  = false;
    }
}

// Habilidad Especial
void personaje::habilidadEspecial(const sf::Vector2f& destino) {
    if (estado != estadoPersonaje::habilidadEspecial) {
        // 1) Cambia estado y marca ataque
        estado          = estadoPersonaje::habilidadEspecial;
        atacando        = true;
        ataqueFase      = 0;
        ataqueLlegado   = false;
        proyectilActivo = true;
        projectileFrame = 0;

        // 2) Guarda la posición original del personaje
        ataqueStartPos = sprite.getPosition();

        // 3) Ajusta el destino del proyectil 100px más arriba
        sf::Vector2f targetProy = { destino.x, destino.y - 300.f };
        ataqueTargetPos = targetProy;

        // 4) Fija el primer frame de la habilidad en el personaje
        sprite.setTextureRect({
            0,
            filaFrameHabilidadEspecial * frameHeight,
            frameWidth,
            frameHeight
        });

        // 5) Inicializa el proyectil en la X del personaje, Y=0
        float xPersonaje = ataqueStartPos.x;
        spriteProyectil.setPosition(xPersonaje, 0.f);
        spriteProyectil.setTextureRect({
            0,
            filaFrameHabilidadEspecial * frameHeight,
            frameWidth,
            frameHeight
        });
    }
}


void personaje::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void personaje::setPosition(const sf::Vector2f& pos) {
     sprite.setPosition(pos.x, pos.y);
}

void personaje::setOrigin(float x, float y) { sprite.setOrigin(x,y); }
void personaje::setScale(float x, float y)  { sprite.setScale(x,y); }

bool personaje::estaAtacando(){
     return atacando;
    };

sf::Vector2f personaje::getPosition() const {
    return sprite.getPosition();
}
