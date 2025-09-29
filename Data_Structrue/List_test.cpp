#include"Sequential_List.h"
#include<iostream>
#include<SFML/Graphics.hpp>
#include"Button.cpp"
#include"input_box.cpp"
#include"ListNode.cpp"
#include"List.cpp"

using namespace std;
void solve() {
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({ 1280, 640 }), "Data_Structrue",sf::Style::Close, sf::State::Windowed, setting);
    window.setFramerateLimit(60);
   
    List node(5,{150,300},{200,50});

    sf::Clock clock;

    while (window.isOpen())
    {   
        auto time=clock.getElapsedTime();
       
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->getIf<sf::Event::MouseButtonPressed>() 
                || event->getIf<sf::Event::MouseButtonReleased>()
                ||event->getIf<sf::Event::KeyPressed>()) {
                //botton.update_press(window, event);
                //inputbox.update_press(window, event);
            }
            //botton.update(window, event);
            //inputbox.update(window, event);
            node.update(window, event);
        }

        node.update_shine(time);
        //inputbox.update_shine(time);

        window.clear();
        //window.draw(botton);
        window.draw(node);
       // window.draw(inputbox);
        window.display();
    }
}

int main() {
    solve();
    return 0;
}