#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<String>
#include<functional>
#include<SFML/Audio.hpp>
#include<thread>



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

	sf::Vector2f pos;
	sf::Vector2f size;

	sf::Font font;
	bool is_pressed = 0;
	bool is_above = 0;
	bool is_click = 0;
	bool is_play = 0;
	bool able = 1;
	bool sound_half = 0;

	std::u32string str;
	
	int len;
	int str_size;
	

public:

	Button(sf::Vector2f pos, sf::Vector2f size={100,50}, std::u32string str = U"", int str_size = 24) :
		normal_text(font), pressed_text(font), above_text(font),show_text(new sf::Text(font))
		,aboveSound(aboveBuffer),choseSound(choseBuffer){

		this->str = str;
		this->str_size = str_size;
		this->pos = pos;
		this->size = size;

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
		len = str.length();
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
		if (!able)return;
		sf::Vector2i pos = sf::Mouse::getPosition(window);

		if (pos.x >= show_rect->getPosition().x
			&& pos.x <= show_rect->getPosition().x + show_rect->getSize().x
			&& pos.y >= show_rect->getPosition().y
			&& pos.y <= show_rect->getPosition().y + show_rect->getSize().y) {
			if (!is_above)aboveSound.play();
			is_above = 1;
		}
		else is_above = 0;

		
		update_press(window, event);

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
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)&& is_above) {
			is_pressed = 1;
			choseSound.play();
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			choseSound.pause();
			sound_half = 1;

		}

		if(is_pressed&&!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			is_pressed = 0;
			is_click = 1;
		}

		if (is_pressed) {
			show_rect = &pressed_rect;
			show_text = &pressed_text;
		}
		else if (is_above) {
			show_rect = &above_rect;
			show_text = &above_text;

		}
		else {
			show_rect = &normal_rect;
			show_text = &normal_text;
		}
	}

	bool click() {
		if (!able)return 0;
		bool flag = is_click;
		is_click = 0;
		
		if (flag && sound_half) {
			choseSound.play();
			sound_half = 0;
		}
		return flag;
	}

	bool pressed() {
		return is_pressed;
	}

	

	void set_release() {
		is_pressed = 0;

	}

	sf::Vector2f getPosition() {
		return show_rect->getPosition();
	}

	sf::Vector2f getSize() {
		return show_rect->getSize();
	}

	void change_y(float x) {
		
		sf::Vector2f temp=(normal_rect.getPosition());
		temp.y += x;
		normal_rect.setPosition(temp);
		above_rect.setPosition(temp);
		pressed_rect.setPosition(temp);

		temp = normal_text.getPosition();
		temp.y += x;
		normal_text.setPosition(temp);
		above_text.setPosition(temp);
		pressed_text.setPosition(temp);
	}

	void setSize(sf::Vector2f size,int csize=24) {
		this->size = size;
		normal_rect.setSize(size);
		above_rect.setSize(size);
		pressed_rect.setSize(size);
		normal_text.setCharacterSize(csize);
		above_text.setCharacterSize(csize);
		pressed_text.setCharacterSize(csize);
	}

	void setPosition(sf::Vector2f pos) {
		this->pos = pos;
		normal_rect.setPosition(pos);
		above_rect.setPosition(pos);
		pressed_rect.setPosition(pos);

		pos.x += normal_rect.getSize().x / 2;
		pos.y += normal_rect.getSize().y / 2;
		pos.x -= str_size * len / 2;
		pos.y -= str_size / 3 * 2;

		normal_text.setPosition(pos);
		above_text.setPosition(pos);
		pressed_text.setPosition(pos);
	}

	void vis(float x) {
		if (x != 1) {
			show_rect = &normal_rect;
			show_text = &normal_text;
		}

		normal_rect.setFillColor(sf::Color(255,255,255,255*x));
		normal_rect.setOutlineColor(sf::Color(0,0,255,255*x));
		normal_text.setFillColor(sf::Color(0, 0, 0, 255 * x));
	}

	void set_able() {
		able = 1;
	}

	void set_unable() {
		able = 0;
		is_above = 0;
		is_click = 0;
		is_pressed = 0;

	}

	void set_str(std::u32string str) {
		this->str = str;
		sf::Vector2f position = pos;
		len = str.length();
		position.x += size.x / 2;
		position.y += size.y / 2;
		position.x -= str_size * len / 2;
		position.y -= str_size / 3 * 2;

		normal_text.setPosition(position);
		above_text.setPosition(position);
		pressed_text.setPosition(position);

		normal_text.setString(str);
		above_text.setString(str);
		pressed_text.setString(str);

	}

	bool above() {
		return is_above;
	}

	std::u32string getString() {
		
		return str;
	}
	

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		
		target.draw(*show_rect,states);
		target.draw(*show_text, states);
		
	}
};