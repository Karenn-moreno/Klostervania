#include "popUpCartel.h"
#include <stdexcept>

PopUpCartel::PopUpCartel()
    : _activo(false)
{
    // El tamaño del fondo y del texto se ajustan en cargarRecursos/mostrar
}

void PopUpCartel::cargarRecursos(const std::string& rutaTextura,
                                 const std::string& rutaFuente)
{
    if (!_texturaFondo.loadFromFile(rutaTextura))
        throw std::runtime_error("PopUpCartel: no se pudo cargar textura \"" + rutaTextura + "\"");
    _spriteFondo.setTexture(_texturaFondo);

    if (!_fuente.loadFromFile(rutaFuente))
        throw std::runtime_error("PopUpCartel: no se pudo cargar fuente \"" + rutaFuente + "\"");
    _texto.setFont(_fuente);
    _texto.setCharacterSize(24);
    _texto.setFillColor(sf::Color::Black);
    _texto.setStyle(sf::Text::Bold);
}

void PopUpCartel::mostrar(const std::string& mensaje,
                          const sf::Vector2u& tamVentana)
{
    // Texto
    _texto.setString(mensaje);
    sf::FloatRect tb = _texto.getLocalBounds();
    _texto.setOrigin(tb.width*0.5f, tb.height*0.5f);
    _texto.setPosition(tamVentana.x*0.5f, tamVentana.y*0.5f - 20.f);

// — Fondo centrado —
    // calculamos escalas base para cubrir ventana
    float sx = float(tamVentana.x) / _texturaFondo.getSize().x;
    float sy = float(tamVentana.y) / _texturaFondo.getSize().y;
    _baseScaleX = sx;
    _baseScaleY = sy;
    // ¡Muy importante! fijar origen en el centro de la textura:
    _spriteFondo.setOrigin(
        _texturaFondo.getSize().x * 0.5f,
        _texturaFondo.getSize().y * 0.5f
    );
    // posiciónalo en el centro de la ventana
    _spriteFondo.setPosition(
        tamVentana.x * 0.5f,
        tamVentana.y * 0.5f
    );
    // arrancamos la animación desde escala 0
    _scale  = 0.f;
    _activo = true;
};

bool PopUpCartel::handleEvent(const sf::Event& evt)
{
    if (!_activo)
        return false;

    if (evt.type == sf::Event::KeyPressed &&
        evt.key.code == sf::Keyboard::Enter)
    {
        _activo = false;
        return true;
    }
    return false;
}

void PopUpCartel::draw(sf::RenderWindow& window)
{
    if (!_activo) return;

    // Avanzamos la escala un paso hacia _targetScale
    _scale = std::min(_scale + _step, _targetScale);

    // Aplicamos escala animada sobre la escala base
    _spriteFondo.setScale(_baseScaleX * _scale,
                          _baseScaleY * _scale);
    _texto.setScale(_scale, _scale);

    // Dibujamos
    window.setView(window.getDefaultView());
    window.draw(_spriteFondo);
    window.draw(_texto);
}

bool PopUpCartel::isActive() const {
     return _activo;
     }

