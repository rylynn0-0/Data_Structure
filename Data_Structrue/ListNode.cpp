#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
class ListNode :public sf::Drawable{
	int num;
	int str_size;
	sf::RectangleShape normal_rect;
	sf::RectangleShape chosen_rect;
	sf::RectangleShape above_rect;
	sf::RectangleShape* show_rect;

	sf::Text text;
	sf::Font font;

	sf::SoundBuffer aboveBuffer;
	sf::SoundBuffer choseBuffer;
	sf::Sound aboveSound;
	sf::Sound choseSound;

	std::string str;

	bool used = 0;
	bool is_chosen;
	bool is_above;
public:
	ListNode(ListNode& node):text(font),  aboveSound(aboveBuffer), choseSound(choseBuffer) {
		std::cout << "copy" << std:: endl;
		num=node.num;
		str_size=node.str_size;
		normal_rect.setFillColor(node.normal_rect.getFillColor());
		normal_rect.setPosition(node.normal_rect.getPosition());
		normal_rect.setSize(node.normal_rect.getSize());
		normal_rect.setOutlineColor(node.normal_rect.getOutlineColor());
		normal_rect.setOutlineThickness(5);

		above_rect.setFillColor(node.above_rect.getFillColor());
		above_rect.setPosition(node.above_rect.getPosition());
		above_rect.setSize(node.above_rect.getSize());
		above_rect.setOutlineColor(node.above_rect.getOutlineColor());
		above_rect.setOutlineThickness(5);

		chosen_rect.setFillColor(node.chosen_rect.getFillColor());
		chosen_rect.setPosition(node.chosen_rect.getPosition());
		chosen_rect.setSize(node.chosen_rect.getSize());
		chosen_rect.setOutlineColor(node.chosen_rect.getOutlineColor());
		chosen_rect.setOutlineThickness(5);


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
		
		str = node.str;
		text.setFont(font);
		text.setString(str);
		text.setCharacterSize(str_size);
		text.setFillColor(sf::Color::Black);
		text.setPosition(node.text.getPosition());
		

		

		used = node.used;
		is_chosen=0;
		is_above=0;
	}

	ListNode():text(font), aboveSound(aboveBuffer), choseSound(choseBuffer) {}

	ListNode(int n, sf::Vector2f pos, sf::Vector2f size,int str_size):text(font), aboveSound(aboveBuffer), choseSound(choseBuffer) {
		num = n;
		this->str_size = str_size;
		is_chosen = 0;
		is_above = 0;
		normal_rect.setPosition(pos);
		normal_rect.setSize(size);
		chosen_rect.setPosition(pos);
		chosen_rect.setSize(size);
		above_rect.setPosition(pos);
		above_rect.setSize(size);

		normal_rect.setFillColor(sf::Color::White);
		normal_rect.setOutlineThickness(5);
		normal_rect.setOutlineColor(sf::Color::Blue);

		chosen_rect.setFillColor(sf::Color::White);
		chosen_rect.setOutlineThickness(5);
		chosen_rect.setOutlineColor(sf::Color::Yellow);

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

		str = std::to_string(num);
		
		std::size_t len = str.length();
		position.x += size.x / 2;
		position.y += size.y / 2;
		position.x -= str_size * len / 2;
		position.y -= str_size / 3 * 2;

		text.setFont(font);
		text.setString("");
		text.setCharacterSize(str_size);
		text.setFillColor(sf::Color::Black);
		text.setPosition(position);
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
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

		if (is_chosen) {
			show_rect = &chosen_rect;			
		}
		else if (is_above) {
			show_rect = &above_rect;		
		}
		else {
			show_rect = &normal_rect;			
			
		}
		
	}

	sf::Vector2f getPosition() {
		return show_rect->getPosition();
	}

	void setPosition(sf::Vector2f pos) {
		normal_rect.setPosition(pos);
		above_rect.setPosition(pos);
		chosen_rect.setPosition(pos);
		sf::Vector2f position = pos;
		sf::Vector2f size = normal_rect.getSize();
		std::size_t len = str.length();
		position.x += size.x / 2;
		position.y += size.y / 2;
		position.x -= str_size * len / 2;
		position.y -= str_size / 3 * 2;
		text.setPosition(position);
	}

	sf::Vector2f getSize() {
		return normal_rect.getSize();
	}

	void setSize(sf::Vector2f size) {
		normal_rect.setSize(size);
		above_rect. setSize(size);
		chosen_rect.setSize(size);
		std::size_t len = str.length();
		sf::Vector2f position = normal_rect.getPosition();
		position.x += size.x / 2;
		position.y += size.y / 2;
		position.x -= str_size * len / 2;
		position.y -= str_size / 3 * 2;
		text.setPosition(position);
	}

	bool is_able() {
		return used;
	}

	void update_press(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
			&& pos.x >= show_rect->getPosition().x
			&& pos.x <= show_rect->getPosition().x + show_rect->getSize().x
			&& pos.y >= show_rect->getPosition().y
			&& pos.y <= show_rect->getPosition().y + show_rect->getSize().y
			)if (used) { if (!is_chosen)choseSound.play(); is_chosen = !is_chosen; }
		if (is_chosen) {
			show_rect = &chosen_rect;
			
		}
		else  {
			show_rect = &normal_rect;

		}
		

	}

	void set_num(int num) {
		sf::Vector2f size = normal_rect.getSize();
		this->num = num;
		str = std::to_string(num);

		int len = std::to_string(num).length();
		size.x = std::max(100, str_size * len + 20);

		setSize(size);

		text.setString(str);
		sf::Vector2 pos = normal_rect.getPosition();
		pos.x += normal_rect.getSize().x / 2;
		pos.y += normal_rect.getSize().y / 2;
		pos.y -= str_size / 3 * 2;
		pos.x -= str.length() * str_size /2;
		text.setPosition(pos);
		used = 1;
	}

	bool chosen() {
		return is_chosen;
	}

	void set_chosen() {
		aboveSound.play();
		is_chosen = 1;
	}

	void set_unchosen() {
		is_chosen = 0;
	}

	int getNum() {
		return num;
	}

	void reset() {
		set_num(0);
		used = 0;
	}

private :
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(*show_rect, states);
		if(used)target.draw(text, states);

	}
};