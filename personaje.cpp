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

sf::FloatRect personaje::getBounds() const
    {
        return sprite.getGlobalBounds();
    };

// 1) Constructor por defecto
// ———————————————————————————————
personaje::personaje()
  : estado(estadoPersonaje::quieto),
    _salud(1000),
    _ataqueLigero(10),
    _ataquePesado(15),
    _habilidadEspecial(25),
    baseScaleX(0.3f),
    baseScaleY(0.3f),
    breathAmplitude(0.02f),    // pulso muy sutil por defecto
    breathSpeed(0.5f)          // medio ciclo por segundo
{
    // Si hubiera un sprite ya asociado, ajustamos escala y reiniciamos reloj
    sprite.setScale(baseScaleX, baseScaleY);
    breathClock.restart();
}

// ———————————————————————————————
// 2) Nuevo constructor: recibe parámetros
// ———————————————————————————————
personaje::personaje(const sf::Vector2f& posInicial,
                     const std::string& rutaSpritesheet,
                     const sf::Vector2f& escala)
  : estado(estadoPersonaje::quieto),
    _salud(1000),
    _ataqueLigero(10),
    _ataquePesado(15),
    _habilidadEspecial(25),
    baseScaleX(escala.x),
    baseScaleY(escala.y),
    breathAmplitude(0.02f),
    breathSpeed(0.5f)
{
    // 1) Intentamos cargar la textura desde rutaSpritesheet
    std::cout << "Intentando cargar textura: " << rutaSpritesheet << "\n";
    if (!textura.loadFromFile(rutaSpritesheet)) {
        std::cerr << "ERROR: No pude cargar " << rutaSpritesheet << "\n";
    } else {
        std::cout << "Textura cargada OK: " << rutaSpritesheet << "\n";
    }

    // 2) Asignar textura al sprite y definir rectángulo inicial
    sprite.setTexture(textura);
    sprite.setTextureRect({0, 0, frameWidth, frameHeight});

    // 3) Aplicar escala (actualiza baseScaleX/Y )
    sprite.setScale(baseScaleX, baseScaleY);

    // 4) Fijar posición inicial del personaje
    sprite.setPosition(posInicial);

    // 5) Reiniciar reloj de respiración
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
                       bool movArriba,
                       bool movAbajo)
{
    // A) Secuencia de ataque ligero
if (estado == estadoPersonaje::ataqueLigero) {
        sprite.setScale(0.5f, 0.5f);
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
        atacando  = false;
        estado = estadoPersonaje::quieto;
    }

    return; // ya procesamos ataque completo
}
// Ataque Pesado
    if (estado == estadoPersonaje::ataquePesado) {
        sprite.setScale(0.5f, 0.5f);
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
    // 2) ¿Está caminando en alguna dirección?
bool caminando = movDer || movIzq || movArriba || movAbajo;

if (caminando) {
    // 3) Elegir fila y totalFrames según dirección
    int fila         = 0;
    int totalFrames  = 0;

    if (movArriba) {
        estado       = estadoPersonaje::caminandoArriba;
        fila         = filaFrameCaminarArriba;
        totalFrames  = cantidadFrameCaminarArriba;
    }
    else if (movAbajo) {
        estado       = estadoPersonaje::caminandoAbajo;
        fila         = filaFrameCaminarAbajo;
        totalFrames  = cantidadFrameCaminarAbajo;
    }
    else {
        estado       = estadoPersonaje::caminando;
        fila         = filaFrameCaminar;
        totalFrames  = cantidadFrameCaminar;
    }

    // 4) Avanzar el temporizador de animación
    frameTimer += deltaTime;
    if (frameTimer >= frameTime) {
        frameTimer  -= frameTime;
        currentFrame = (currentFrame + 1) % totalFrames;
    }

    // 5) Calcular y aplicar el sub-rectángulo
    int left = currentFrame * frameWidth;
    int top  = fila * frameHeight;
    sprite.setTextureRect({ left, top, frameWidth, frameHeight });

    // 6) Mover al personaje
    if (movArriba) sprite.move(0, -speed * deltaTime);
    if (movAbajo)  sprite.move(0,  speed * deltaTime);
    if (movIzq)    sprite.move(-speed * deltaTime, 0);
    if (movDer)    sprite.move( speed * deltaTime, 0);

    // 7) Flip horizontal solo para la caminata lateral,
    //    usando siempre baseScaleX/baseScaleY
    if (movIzq) {
        sprite.setScale(-baseScaleX, baseScaleY);
        sprite.setOrigin(frameWidth, 0);
    } else {
        sprite.setScale(baseScaleX, baseScaleY);
        sprite.setOrigin(0, 0);
    }
}
else {
    // --- Estado quieto ---
    estado       = estadoPersonaje::quieto;
    currentFrame = 0;

    // Frame fijo de quieto en la fila 6
    sprite.setTextureRect({
        0,
        filaFrameQuieto * frameHeight,
        frameWidth,
        frameHeight
    });

    // RESTAURAR escala y origen al valor base que le pasaste en el constructor
    sprite.setScale(baseScaleX, baseScaleY);
    sprite.setOrigin(0, 0);
}
    // D) Respiración / pulso
//float t = breathClock.getElapsedTime().asSeconds();
//float factor = 1.f + breathAmplitude * std::sin(2.f * 3.14159265f * breathSpeed * t);
//float signX = (sprite.getScale().x < 0) ? -1.f : 1.f;
//sprite.setScale(signX * baseScaleX * factor, baseScaleY * factor);
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

        // 5) **Asigna textura al proyectil** y fija su frame inicial
        spriteProyectil.setTexture(textura);  // <<== Importante
        spriteProyectil.setTextureRect({
            0,
            filaFrameHabilidadEspecial * frameHeight,
            frameWidth,
            frameHeight
        });

        // 6) Inicializa el proyectil en la X del personaje, Y=0
        float xPersonaje = ataqueStartPos.x;
        spriteProyectil.setPosition(xPersonaje, 0.f);
        // Opcional: escalado inicial del proyectil si lo quisieras
        spriteProyectil.setScale(baseScaleX, baseScaleY);
    }
}
sf::Vector2f personaje::getPosition() const {
    return sprite.getPosition();
}

const sf::Sprite& personaje::getSprite() const {
    return sprite;
}

void personaje::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void personaje::setPosition(const sf::Vector2f& pos) {
     sprite.setPosition(pos.x, pos.y);
}

void personaje::setOrigin(float x, float y) { sprite.setOrigin(x,y); }
void personaje::setScale(float x, float y)  {

    baseScaleX = x;
    baseScaleY = y;

    sprite.setScale(baseScaleX, baseScaleY);

    sprite.setOrigin(0.f, 0.f);
}

bool personaje::estaAtacando(){
     return atacando;
    };


void personaje::setScale(const sf::Vector2f& s) {
    setScale(s.x, s.y);
}
void personaje::setEstado(estadoPersonaje nuevoEstado)
{

    estado = nuevoEstado;
}
