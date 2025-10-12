#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>

class InputBox :public sf::Drawable{
	std::string str;
	int num = 0;
	std::u32string normal_str;
	sf::RectangleShape normal_rect;
	sf::RectangleShape chosen_rect;
	sf::RectangleShape above_rect;
	sf::RectangleShape* show_rect;

	sf::RectangleShape line;

	sf::Font font;
	sf::Text input_text;
	sf::Text normal_text;
	sf::Text *show_text;

	sf::SoundBuffer aboveBuffer;
	sf::SoundBuffer choseBuffer;
	sf::Sound aboveSound;
	sf::Sound choseSound;

	bool is_above = 0;
	bool is_chosen = 0;
	bool is_shine = 0;
	bool is_inputting = 0;

	int str_size;
	int max_x;

	int pre_time;


public:
	InputBox(sf::Vector2f pos, sf::Vector2f size,int str_size,std::u32string nstr,int len):
		input_text(font),normal_text(font),show_text(new sf::Text(font) )
	   , aboveSound(aboveBuffer), choseSound(choseBuffer){
		normal_rect.setPosition(pos);
		chosen_rect.setPosition(pos);
		above_rect.setPosition(pos);

		pre_time = 0;
		
		len++;
		max_x = pos.x + size.x;
		max_x = std::min(max_x,(int)pos.x+len*str_size );

		normal_rect.setSize(size);
		chosen_rect.setSize(size);
		above_rect.setSize(size);

		this->str_size = str_size;


		normal_rect.setFillColor(sf::Color::White);
		chosen_rect.setFillColor(sf::Color::White);
		above_rect.setFillColor(sf::Color::White);


		normal_rect.setOutlineThickness(5);
		normal_rect.setOutlineColor(sf::Color::Blue);

		chosen_rect.setOutlineThickness(5);
		chosen_rect.setOutlineColor(sf::Color::Yellow);
		

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

		input_text.setFont(font);
		normal_text.setFont(font);

		input_text.setCharacterSize(str_size);
		normal_text.setCharacterSize(str_size);

		line.setFillColor(sf::Color::Black);

		sf::Vector2f line_size;
		line_size.x = 5;
		line_size.y = str_size;
		line.setSize(line_size);

		sf::Vector2f position = pos;
		position.x += 10;
		position.y += size.y / 2;
		position.y -= str_size / 2;
		line.setPosition(position);
		
		position.y -= str_size / 6;
		input_text.setPosition(position);
		normal_text.setPosition(position);
		

		normal_str = nstr;
		str = "";

		input_text.setString(str);
		normal_text.setString(normal_str);

		input_text.setFillColor(sf::Color::Black);
		normal_text.setFillColor(sf::Color(128, 128, 128));

		show_text = &normal_text;

	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);

		if (pos.x >= show_rect->getPosition().x
			&& pos.x <= show_rect->getPosition().x + show_rect->getSize().x
			&& pos.y >= show_rect->getPosition().y
			&& pos.y <= show_rect->getPosition().y + show_rect->getSize().y) {
			if (!is_above)aboveSound.play();
			is_above = 1;
		}
		else is_above = 0;
		
		if (event->getIf<sf::Event::MouseButtonPressed>()||event->getIf<sf::Event::KeyPressed>()) {
			update_press(window, event);
		}
		
		if (is_chosen) {
			show_rect = &chosen_rect;
			show_text = &input_text;
			
			if (auto code = event->getIf<sf::Event::KeyPressed>() ) {

				int input= (int)(code->scancode) ;
				sf::Vector2 line_pos = line.getPosition();
				if (input >= 26 && input < 35&& line_pos.x+str_size< max_x) {
					num = num * 10 + input-25;
					if (str == "0") {
						str = "";
						line_pos.x -= str_size;
					}
					str += (input + '0'-25);					
					input_text.setString(str);
					line_pos.x += str_size;
				}
				if (input == 35&&line_pos.x+str_size<max_x) {
					num *= 10;
					if (str != "0") {
						str += '0';
						input_text.setString(str);
						line_pos.x += str_size;
					}
				}
				if (input == 38&&str.size()) {
					num /= 10;
					str = str.substr(0, str.size() - 1);
					input_text.setString(str);
					line_pos.x -= str_size;
				}
				std::cout << "°´¼üÊäÈë£¬scancode£º"<<input << std::endl;
				if (input == 36) {
					is_inputting = 1;
				}

				
				
				line.setPosition(line_pos);
				
			}
			
		}
		else if (is_above) {
			if(!is_chosen)show_rect = &above_rect;
			is_shine = 0;
			
			
		}
		else {
			show_rect = &normal_rect;
			if(str=="")show_text = &normal_text;
			is_shine = 0;
			is_above = 0;
			is_chosen = 0;
		}
	}

	void update_press(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);

		if (((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) || 
			(!is_chosen && event->getIf<sf::Event::KeyPressed>()))
			&& pos.x >= show_rect->getPosition().x
			&& pos.x <= show_rect->getPosition().x + show_rect->getSize().x
			&& pos.y >= show_rect->getPosition().y
			&& pos.y <= show_rect->getPosition().y + show_rect->getSize().y) {
			if (!event->getIf<sf::Event::KeyPressed>())choseSound.play();
			is_chosen = !is_chosen;
		}

		
		
		if (is_chosen) {
			show_rect = &chosen_rect;
			show_text = &input_text;	
			is_shine = 1;
		}
		else {
			is_shine = 0;
			show_rect = &normal_rect;
			if(str=="")show_text = &normal_text;
		}
	}

	void update_shine(sf::Time time) {
		int t = time.asMilliseconds();
		if (is_chosen) {
			if (t - pre_time > 400) {
				is_shine = !is_shine;
				pre_time = t;
			}
		}
		else is_shine = 0;
	}

	int get_num() {
		return num;
	}

	void reset() {
		num = 0;
		str = "";
		input_text.setString(str);
		sf::Vector2f position = normal_rect.getPosition();
		position.x += 10;
		position.y += normal_rect.getSize().y / 2;
		position.y -= str_size / 2;
		line.setPosition(position);
	}

	void set_normal() {
		show_rect = &normal_rect;
		show_text = &normal_text;
		is_shine = 0;
		is_chosen = 0;
	}

	bool putting() {
		bool flag = is_inputting;
		is_inputting = 0;
		if(flag)choseSound.play();
		return flag;
	}

	void got_put() {
		is_inputting = 0;
	}

	bool above() {
		return is_above;
	}

	sf::Vector2f getPosition() {
		return normal_rect.getPosition();
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(*show_rect, states);
		target.draw(*show_text, states);
		if (is_shine)
			target.draw(line, states);

	}
};