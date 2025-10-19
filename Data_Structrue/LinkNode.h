#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include"Arrow.h"
class LinkNode :public sf::Drawable{
	sf::CircleShape normal_cir;
	sf::CircleShape above_cir;
	sf::CircleShape chose_cir;
	sf::CircleShape* show_cir;

	sf::SoundBuffer aboveBuffer;
	sf::SoundBuffer choseBuffer;
	sf::Sound aboveSound;
	sf::Sound choseSound;

	Arrow *arrow;

	sf::Font font;
	sf::Text text;

	int num;
	int size;
	sf::Vector2f pos;
	float radius;
	std::string str = "";

	int is_above = 0;
	int is_click = 0;
	int is_chose = 0;
	
	bool heading = 0;
	bool ending = 0;
	bool arrow_update_able = 1;
	

public:
	LinkNode* next;
	LinkNode* father;

	LinkNode(sf::Vector2f pos, int flag = 0,int num=0,float radius=35 ,int size=24):
		pos(pos), num(num), radius(radius), text(font),
		size(size),heading(heading), aboveSound(aboveBuffer), choseSound(choseBuffer) {

		if (flag == 1)heading = 1;
		if (flag == -1)ending = 1;

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

		Init();
	}

	void Init() {
		normal_cir.setPosition(pos);
		above_cir.setPosition(pos);
		chose_cir.setPosition(pos);

		normal_cir.setRadius(radius);
		above_cir.setRadius(radius);
		chose_cir.setRadius(radius);

		normal_cir.setOutlineThickness(5);
		above_cir.setOutlineThickness(5);
		chose_cir.setOutlineThickness(5);

		normal_cir.setFillColor(sf::Color::White);
		above_cir.setFillColor(sf::Color::White);
		chose_cir.setFillColor(sf::Color::White);

		normal_cir.setOutlineColor(sf::Color::Blue);
		above_cir.setOutlineColor(sf::Color::Yellow);

		show_cir = &normal_cir;
		
		if (num != 0) {
			str = std::to_string(num);
		}
		if (heading)str = "head";
		if (ending)str = "null";

		sf::Vector2f position = pos;
		int len = str.length();
		position.x += radius ;
		position.y += radius ;
		position.x -= size * len / 2;
		

		if (heading||ending)position.y += radius;
		else position.y -= size / 3 * 2;

		text.setFont(font);
		text.setCharacterSize(size);

		if (heading||ending)text.setFillColor(sf::Color::White);
		else text.setFillColor(sf::Color::Black);

		text.setString(str);
		text.setPosition(position);


	}

	double Distance(sf::Vector2f p) {
		double x = abs(pos.x + radius - p.x);
		double y = abs(pos.y + radius - p.y);
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	void update_arrow() {

		if (next) {
			sf::Vector2f next_pos = next->getPosition();

			float distance = sqrt(std::pow(pos.x - next_pos.x, 2) + std::pow(pos.y - next_pos.y, 2));
			sf::Vector2f i;
			i.x = (next_pos.x - pos.x) / distance * radius * 1.5;
			i.y = (next_pos.y - pos.y) / distance * radius * 1.5;

			sf::Vector2f pos1;
			sf::Vector2f pos2;

			pos1.x = pos.x + i.x + radius;
			pos1.y = pos.y + i.y + radius;

			pos2.x = next_pos.x - i.x + radius;
			pos2.y = next_pos.y - i.y + radius;

			if (Distance(pos1) > Distance(pos2)) {
				std::swap(pos1, pos2);
			}
			

			if (!arrow) {
				arrow = new Arrow(pos1, pos2, 5);
			}
			else {
				arrow->update(pos1, pos2, 5);
			}
		}
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event){
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

		if(arrow_update_able)update_arrow();

		float delta_x = std::abs(mouse_pos.x - pos.x - radius);
		float delta_y = std::abs(mouse_pos.y - pos.y - radius);
		float delta = std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));

		if (delta <= radius) {
			if (!is_above)aboveSound.play();
			is_above = 1;
		}
		else is_above = 0;

		if (event->getIf<sf::Event::MouseButtonPressed>()) {
			update_press(window, event);
		}


		if (is_click) {
			show_cir = &chose_cir;
		}
		else if (is_above||is_chose) {
			show_cir = &above_cir;
		}
		else {
			show_cir = &normal_cir;
		}
	}

	void update_press(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_above) {
			is_click = 1;
		}
		
	}

	bool click() {
		bool temp = is_click;
		if (temp)choseSound.play();
		is_click = 0;
		return temp;
	}

	void get_point(sf::Vector2f &from,sf::Vector2f &to) {
		sf::Vector2f next_pos = next->getPosition();

		float distance = sqrt(std::pow(pos.x - next_pos.x, 2) + std::pow(pos.y - next_pos.y, 2));
		sf::Vector2f i;
		i.x = (next_pos.x - pos.x) / distance * radius * 1.5;
		i.y = (next_pos.y - pos.y) / distance * radius * 1.5;

		sf::Vector2f pos1;
		sf::Vector2f pos2;

		pos1.x = pos.x + i.x + radius;
		pos1.y = pos.y + i.y + radius;

		pos2.x = next_pos.x - i.x + radius;
		pos2.y = next_pos.y - i.y + radius;

		if (Distance(pos1) > Distance(pos2)) {
			std::swap(pos1, pos2);
		}

		from = pos1;
		to = pos2;
	}

	void set_point(sf::Vector2f from,sf::Vector2f to) {
		
		if (!arrow) {
			arrow = new Arrow(from, to, 5);
		}
		else {
			arrow->update(from,to, 5);
		}


	}

	sf::Vector2f getPosition() {
		return pos;
	}

	void setPosition(sf::Vector2f pos) {
		this->pos = pos;
		normal_cir.setPosition(pos);
		above_cir.setPosition(pos);
		chose_cir.setPosition(pos);

		sf::Vector2f position = pos;
		int len = str.length();
		position.x += radius;
		position.y += radius;
		position.x -= size * len / 2;
		if (heading||ending)position.y += radius;
		else position.y -= size / 3 * 2;

		text.setPosition(position);
		update_arrow();
	}

	bool is_ending() {
		return ending;
	}

	void set_chose() {
		is_chose = 1;
		show_cir = &above_cir;
	}

	void set_unchose() {
		is_chose = 0;
		show_cir = &normal_cir;
	}

	void set_arrow_update(bool f) {
		arrow_update_able = f;
	}

	


	float getRadius() {
		return radius;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(*show_cir, states);
		target.draw(text, states);
		if(arrow)target.draw(*arrow, states);

	}

};