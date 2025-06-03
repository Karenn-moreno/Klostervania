#include "boss.h"
#include <cstdlib>   // std::rand
#include <cmath>     // std::hypot

// Constructor: delega en enemigo y, automáticamente, en personaje
Boss::Boss(const sf::Vector2f& posInicial,
           const std::string& rutaSpritesheet,
           const sf::Vector2f& escala)
    : enemigo(posInicial, rutaSpritesheet, escala)
{
    // ajustar salud, stats u otras propiedades específicas del Boss

}

// ataque: elige entre 5 opciones (3 normales heredadas + 2 especiales del Boss)
int Boss::ataque(const sf::Vector2f& destino) {
    // 1) Guardar la posición base del pie (ya hace enemigo::ataque, pero aquí la repetimos para animaciones especiales)
    ataqueStartPos = sprite.getPosition();

    // 2) Obtener ancho/alto del sprite (ya escalado)
    sf::FloatRect local = sprite.getLocalBounds();
    float ancho = local.width;
    float alto  = local.height;
    float escX  = std::abs(sprite.getScale().x);
    float escY  = sprite.getScale().y;

    // 3) Flip horizontal manteniendo el origen en la base
    if (destino.x > ataqueStartPos.x) {
        // Hacia la derecha
        sprite.setScale(+escX, escY);
        sprite.setOrigin(0.f, alto);
    } else {
        // Hacia la izquierda
        sprite.setScale(-escX, escY);
        sprite.setOrigin(ancho, alto);
    }

    // 4) Elegir opción entre 0..4
    int opcion = std::rand() % 5;
    std::cout << "Boss seleccionó habilidad: " << opcion << "\n";
    int danio = 0;
    switch (opcion) {
        case 0:
            danio = getAtaqueLigero();
            ataqueLigero(destino);  // Llama a personaje::ataqueLigero
            break;
        case 1:
            danio = getAtaquePesado();
            ataquePesado(destino);  // Llama a personaje::ataquePesado
            break;
        case 2:
            danio = getHabilidadEspecial();
            habilidadEspecial(destino);  // Llama a personaje::habilidadEspecial
            break;
        case 3:
            danio = ataqueEspecial1(destino);
            break;
        case 4:
            danio = ataqueEspecial2(destino);
            break;
    }
    return danio;
}
int Boss::ataqueEspecial1(const sf::Vector2f& destino) {
    // 1) Guardar posición “pie” del Boss
    ataqueStartPos = sprite.getPosition();

    // 2) Poner estado y marcar que atacamos
    estado           = estadoPersonaje::habilidadEspecial;
    atacando         = true;
    ataqueFase       = 0;
    ataqueLlegado    = false;
    ataqueTargetPos  = { 0.f, 800.f  }; // ej. 200 px abajo
    currentFrame     = 0;
    frameTimer       = 0.f;
    proyectilActivo = true;
    _especialEnCurso = 1;  // indicar que usaremos filaFrameEspecial1

    sprite.setTextureRect({
        0,
        filaFrameEspecial1 * frameHeight,
        frameWidth,
        frameHeight
    });

    // 4) Configurar el proyectil en fila 7 (filaFrameEspecial1):
    spriteProyectil.setTexture(textura);
    spriteProyectil.setTextureRect({
        0,
        filaFrameEspecial1 * frameHeight,  // usa filaFrameEspecial1
        frameWidth,
        frameHeight
    });

    // Posicionar el proyectil en la X del Boss, Y = 0
    float xBoss = ataqueStartPos.x;
    spriteProyectil.setPosition(1000.f, 800.f);

    // Escalar el proyectil (mismo factor que la habilidad especial genérica)
    spriteProyectil.setScale(baseScaleX * 2.f, baseScaleY * 2.f);

    return getHabilidadEspecial() *1.2;
}

int Boss::ataqueEspecial2(const sf::Vector2f& destino) {
    ataqueStartPos = sprite.getPosition();

    estado           = estadoPersonaje::habilidadEspecial;
    atacando         = true;
    ataqueFase       = 0;
    ataqueLlegado    = false;
    ataqueTargetPos  = { 0.f, 800.f };
    currentFrame     = 0;
    frameTimer       = 0.f;
    proyectilActivo = true;
    _especialEnCurso = 2;  // indicar que usaremos filaFrameEspecial2

    // Animación del Boss igual que antes (fila 3)
    sprite.setTextureRect({
        0,
        filaFrameEspecial2 * frameHeight,
        frameWidth,
        frameHeight
    });

    // Configurar el proyectil en fila 8 (filaFrameEspecial2):
    spriteProyectil.setTexture(textura);
    spriteProyectil.setTextureRect({
        0,
        filaFrameEspecial2 * frameHeight,  // usa filaFrameEspecial2
        frameWidth,
        frameHeight
    });
    spriteProyectil.setPosition(0.f, 0.f);
    spriteProyectil.setScale(baseScaleX * 2.f, baseScaleY * 2.f);

    return getHabilidadEspecial() * 1.3;
}


void Boss::update(float deltaTime,
                  bool moviendoDer,
                  bool moviendoIzq,
                  bool moviendoArriba,
                  bool moviendoAbajo)
{
    // ————— En combate por turnos —————
    if (_modoBatalla) {
        // 1) Si el Boss está en estado “habilidadEspecial”:
        if (estado == estadoPersonaje::habilidadEspecial) {
            // Determinar qué fila usar según _especialEnCurso:
            //   0 -> filaFrameHabilidadEspecial (genérica)
            //   1 -> filaFrameEspecial1
            //   2 -> filaFrameEspecial2
            int fila;
            int totalFrames;
            if (_especialEnCurso == 1) {
                fila        = filaFrameEspecial1;
                totalFrames = cantidadFrameEspecial1;
            }
            else if (_especialEnCurso == 2) {
                fila        = filaFrameEspecial2;
                totalFrames = cantidadFrameEspecial2;
            }
            else {
                // Caso “habilidadEspecial genérica” (de personaje/enemigo)
                fila        = filaFrameHabilidadEspecial;
                totalFrames = cantidadFrameHabilidadEspecial;
            }

            // ——— Fase 0: MOVER EL PROYECTIL ———
            if (ataqueFase == 0 && !ataqueLlegado) {
                // ─── ORIENTAR PROYECTIL ───
                sf::Vector2f dir0 = ataqueTargetPos - spriteProyectil.getPosition();
                float signoX = (dir0.x >= 0.f) ? +1.f : -1.f;

                // Fijar origen en la “base” del proyectil (misma lógica que para sprite principal)
                {
                    sf::FloatRect localP = sprite.getLocalBounds();
                    float anchoP = localP.width;
                    float altoP  = localP.height;
                    spriteProyectil.setScale(signoX * baseScaleX * 2.f, baseScaleY * 2.f);
                    if (signoX < 0.f)
                        spriteProyectil.setOrigin(anchoP, altoP);
                    else
                        spriteProyectil.setOrigin(0.f, altoP);
                }

                // Fijar el sub-rectángulo inicial del proyectil (columna 1, fila variable)
                projectileFrame = 1;
                spriteProyectil.setTextureRect(
                    sf::IntRect(
                        projectileFrame * frameWidth,
                        fila * frameHeight,
                        frameWidth,
                        frameHeight
                    )
                );

                // Mover proyectil hacia ataqueTargetPos
                sf::Vector2f dir = ataqueTargetPos - spriteProyectil.getPosition();
                float dist       = std::hypot(dir.x, dir.y);
                if (dist > ataqueSpeed * deltaTime) {
                    dir /= dist;  // normalizar
                    spriteProyectil.move(dir * ataqueSpeed * deltaTime);
                } else {
                    // El proyectil llega al destino:
                    spriteProyectil.setPosition(ataqueTargetPos);
                    ataqueLlegado   = true;
                    ataqueFase      = 1;  // Pasar a fase de animación de impacto
                    projectileFrame = 1;  // Arrancar desde el frame 1
                    frameTimer      = 0.f;
                }
                return;
            }

            // ——— Fase 1: ANIMAR FRAMES 1..(totalFrames–1) DEL PROYECTIL ———
            if (ataqueFase == 1 && projectileFrame < totalFrames) {
                frameTimer += deltaTime * 1.8f;
                if (frameTimer >= frameTime) {
                    frameTimer -= frameTime;
                    projectileFrame++;
                }
                // Asegurar que no sobrepase totalFrames–1
                int frameAnim = std::min(projectileFrame, totalFrames - 1);
                spriteProyectil.setTextureRect(
                    sf::IntRect(
                        frameAnim * frameWidth,
                        fila * frameHeight,
                        frameWidth,
                        frameHeight
                    )
                );
                return;
            }

            // ——— Fase 2: RESETAR TODO ———
            {
                // 1) Apagar / ocultar proyectil poniéndolo fuera de vista
                ataqueLlegado = false;
                ataqueFase    = 0;

                // 2) Volver a la posición base del Boss (los pies)
                sprite.setPosition(ataqueStartPos);

                // 3) Restaurar escala normal y origen en la base del sprite
                sprite.setScale(baseScaleX, baseScaleY);
                {
                    sf::FloatRect local = sprite.getLocalBounds();
                    sprite.setOrigin(0.f, local.height);
                }

                // 4) Cambiar el estado a “quieto” y desactivar banderas
                estado           = estadoPersonaje::quieto;
                atacando         = false;
                proyectilActivo  = false;
                _especialEnCurso = 0;
                projectileFrame  = 0;
                frameTimer       = 0.f;
            }
            return;
        }

        // Si no está en estado “habilidadEspecial”, delegar a personaje::update
        personaje::update(deltaTime,
                          moviendoDer, moviendoIzq,
                          moviendoArriba, moviendoAbajo);
        return;
    }

    // ————— Fuera de combate —————
    // Delegar a enemigo::update para IA/patrulla normal
    enemigo::update(deltaTime,
                    moviendoDer, moviendoIzq,
                    moviendoArriba, moviendoAbajo);
}
