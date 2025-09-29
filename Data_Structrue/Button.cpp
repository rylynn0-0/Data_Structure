#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<String>
#include<functional>
#include<SFML/Audio.hpp>



class Button :public sf::Drawable{

	sf::RectangleShape normal_rect;
	sf::RectangleShape pressed_rect;
	sf::RectangleShape above_rect;
	sf::RectangleShape* show_rect;

	sf::Text normal_text;
	sf::Text pressed_text;
	sf::Text above_text;
	sf::Text* show_text;

	sf::SoundBuffer aboveBuffer;
	sf::SoundBuffer choseBuffer;
	sf::Sound aboveSound;
	sf::Sound choseSound;

	sf::Font font;
	bool is_pressed = 0;
	bool is_above = 0;
	bool is_click = 0;
	bool is_play = 0;
	

	



public:
	Button(sf::Vector2f pos, sf::Vector2f size, std::u32string str, int str_size ) :
		normal_text(font), pressed_text(font), above_text(font),show_text(new sf::Text(font))
		,aboveSound(aboveBuffer),choseSound(choseBuffer){

		

		normal_rect.setPosition(pos);
		normal_rect.setSize(size);
		pressed_rect.setPosition(pos);
		pressed_rect.setSize(size);
		above_rect.setPosition(pos);
		above_rect.setSize(size);

		normal_rect.setFillColor(sf::Color::White);
		normal_rect.setOutlineThickness(5);
		normal_rect.setOutlineColor(sf::Color::Blue);
		pressed_rect.setFillColor(sf::Color::White);
		above_rect.setFillColor(sf::Color::White);
		above_rect.setOutlineThickness(5);
		above_rect.setOutlineColor(sf::Color::Yellow);

		show_rect = &normal_rect;

		if (!font.openFromFile("Dymon-ShouXieTi.otf")) {
			std::cout << "error" << std::endl;
			return;
		}

		if (!aboveBuffer.loadFromFile("above.mp3")) {
			std::cout << "error" << std::endl;
			return;
		}

		if (!choseBuffer.loadFromFile("chose.mp3")) {
			std::cout << "error" << std::endl;
			return;
		}

		sf::Vector2f position = pos;
		std::size_t len = str.length();
		position.x += size.x / 2;
		position.y += size.y / 2;
		position.x -= str_size*len / 2;
		position.y -= str_size / 3*2;

		normal_text.setFont(font);
		above_text.setFont(font);
		pressed_text.setFont(font);

		normal_text.setCharacterSize(str_size);
		above_text.setCharacterSize(str_size);
		pressed_text.setCharacterSize(str_size);

		normal_text.setPosition(position);
		above_text.setPosition(position);
		pressed_text.setPosition(position);

		normal_text.setFillColor(sf::Color::Black);
		above_text.setFillColor(sf::Color::Black);
		pressed_text.setFillColor(sf::Color::Black);

		normal_text.setString(str);
		above_text.setString(str);
		pressed_text.setString(str);

		show_text = &normal_text;
		
		
	}

	void update(const sf::RenderWindow &window,std::optional<sf::Event> event ) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);

		if (event->getIf<sf::Event::MouseButtonPressed>()) {
			update_press(window, event);
		}

		if (pos.x >= show_rect->getPosition().x
			&& pos.x <= show_rect->getPosition().x + show_rect->getSize().x
			&& pos.y >= show_rect->getPosition().y
			&& pos.y <= show_rect->getPosition().y + show_rect->getSize().y) {
			if (!is_above)aboveSound.play();
			is_above = 1;
		}
		else is_above = 0;
		

		if (is_pressed) {
			show_rect = &pressed_rect;
			show_text = &pressed_text;
			
		}
		else if(is_above){
			show_rect = &above_rect;
			show_text = &above_text;
			
		}
		else {
			show_rect = &normal_rect;
			show_text = &normal_text;
		}
	}

	void update_press(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
			&& pos.x >= show_rect->getPosition().x
			&& pos.x <= show_rect->getPosition().x + show_rect->getSize().x
			&& pos.y >= show_rect->getPosition().y
			&& pos.y <= show_rect->getPosition().y + show_rect->getSize().y)is_pressed = 1;
		if (is_pressed && event->getIf<sf::Event::MouseButtonReleased>()) {
			is_pressed = 0;
		}
		if (is_pressed) {
			show_rect = &pressed_rect;
			show_text = &pressed_text;
			is_click = 1;
			
		}
	}

	bool click() {
		bool flag = is_click;
		is_click = 0;
		if(flag)choseSound.play();
		return flag;
	}

	bool pressed() {
		return is_pressed;
	}

	void set_release() {
		is_pressed = 0;
	}
	

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		
		target.draw(*show_rect,states);
		target.draw(*show_text, states);
		
	}
};