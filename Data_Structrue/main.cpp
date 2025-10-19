#pragma once
#include"State.h"

using namespace std;
void solve() {
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({ 1280, 640 }), "Data_Structrue", sf::Style::Close, sf::State::Windowed, setting);
    window.setFramerateLimit(60);

    State state;

    sf::Clock clock;

    while (window.isOpen())
    {
        auto time = clock.getElapsedTime();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            state.update(window, event);
        }

       

        state.update_shine(time);
        window.clear();
        window.draw(state);
        window.display();
    }
}

int main() {
    solve();
    return 0;
}