#include <SFML/Graphics.hpp>
#include<iostream>

int main()
{   
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({ 1280, 640 }), "Data_Structrue",sf::State::Windowed,setting);

    sf::CircleShape shape(50.f);
    shape.setPosition((sf::Vector2f)sf::Mouse::getPosition(window));
    window.setFramerateLimit(60);
    float Lerp = 0.75;
    sf::Vector2f currentPos = shape.getPosition();



    sf::Texture texture("Texturelabs_Details_141S.png");
    texture.setSmooth(1);
    sf::Sprite sprite(texture);

    double zoom = 1;
    
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltatime = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseWheel =event->getIf<sf::Event::MouseWheelScrolled>()) {
                zoom += mouseWheel->delta * 0.1;                                
            }
         
        }

        sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        mousePos.x -= texture.getSize().x/2*zoom;
        mousePos.y -= texture.getSize().y/2*zoom;

        sprite.setScale({ zoom, zoom });
        currentPos += (mousePos-currentPos) * Lerp * deltatime * (float)60;
        //shape.setPosition(currentPos);
        sprite.setPosition(currentPos);
        

        window.clear();
        //window.draw(shape);
        window.draw(sprite);
        window.display();
    }
}