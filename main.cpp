#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



const int NUM_OPCIONES = 4;
std::string opciones[NUM_OPCIONES] = {"Nueva Partida", "Continuar Partida", "Record", "Salir"};

void actualizarTexto(sf::Text textos[], int opcionSeleccionada)
{
    for (int i = 0; i < NUM_OPCIONES; i++)
    {
        if (i == opcionSeleccionada)
        {
            textos[i].setFillColor(sf::Color::White); // Resaltar opción seleccionada
        }
        else
        {
            textos[i].setFillColor(sf::Color::Red);
        }
    }
}


int main()
{
    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(1500, 900), "Klostervania");
    window.setFramerateLimit(60);

    // Cargar la textura
    sf::Texture fondoPrincipal;
    if (!fondoPrincipal.loadFromFile("img/Klostervania_fondo.jpg"))
    {
        std::cout << "Error al cargar la imagen" << std::endl;
        return -1;
    }
    sf::Sprite Fondo;
    Fondo.setTexture(fondoPrincipal);
    Fondo.setScale({1.6f, 1.2f}); //  factor de escala

    //Cargo la fuente y color
    sf::Font Texto1;
    if(!Texto1.loadFromFile("fonts/Hatch.ttf"))
    {
        std::cout << "Error al cargar en texto nueva partida" << std::endl;
        return EXIT_FAILURE;
    }
    //cargo sonidos
    sf::SoundBuffer _Flecha;
        if (!_Flecha.loadFromFile("audio/flecha.wav"))
    {
        std::cout << "Error al cargar audio de flecha" << std::endl;
        return -1;
    }
    sf::Sound Flecha;
    Flecha.setBuffer(_Flecha);

    sf::SoundBuffer _Enter;
        if (!_Enter.loadFromFile("audio/enter.wav"))
    {
        std::cout << "Error al cargar audio de enter" << std::endl;
        return -1;
    }
    sf::Sound Enter;
    Enter.setBuffer(_Enter);

    //armamos el menu dinamico
    sf::Text textos[NUM_OPCIONES];

    int opcionSeleccionada = 0;

    for (int i = 0; i < NUM_OPCIONES; i++)
    {
        textos[i].setFont(Texto1);
        textos[i].setString(opciones[i]);
        textos[i].setCharacterSize(40);
        textos[i].setPosition(600, 550 + i * 60);
        textos[i].setFillColor(sf::Color::Red);
    }
    // Bucle principal de la ventana
    while (window.isOpen())
    {
        //  Revisa todos los eventos de la ventana que se activaron desde la última iteración del bucle.
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)   // Aquí dentro es correcto
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    opcionSeleccionada = (opcionSeleccionada - 1 + NUM_OPCIONES) % NUM_OPCIONES;
                    Flecha.play();
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    opcionSeleccionada = (opcionSeleccionada + 1) % NUM_OPCIONES;
                    Flecha.play();

                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    Enter.play();
                    std::cout << "Has seleccionado: " << opciones[opcionSeleccionada] << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    window.close();
                }
                actualizarTexto(textos, opcionSeleccionada);
            }
        }

        // limpio la ventana con color negro
        window.clear(sf::Color::Black);


        // Dibujando todo
        window.draw(Fondo);
        for (int i = 0; i < NUM_OPCIONES; i++)
            window.draw(textos[i]);

        // Mostrar el contenido de la ventana
        window.display();

    }
    return 0;
}



























