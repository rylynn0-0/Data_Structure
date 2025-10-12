#pragma once
#include<SFML/Graphics.hpp>
#include"Arrow.cpp"
#include"Button.cpp"



class ArrowButton :public sf::Drawable{
public:
	enum TYPE {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

private:
	Button btn;
	Arrow normal_arrow;
	Arrow above_arrow;
	Arrow pressed_arrow;
	Arrow* show_arrow;
	sf::Vector2f from;
	sf::Vector2f to;
	
	bool is_click = 0;
	bool is_pressed = 0;
	
	
	TYPE type;
public:
	

	ArrowButton(TYPE type,sf::Vector2f pos,sf::Vector2f size={100,50}) :btn(pos), 
		normal_arrow({0,0},{0,0},5),
		above_arrow({ 0,0 }, { 0,0 }, 5) {
		if (type == UP || type == DOWN) {
			if (size.x < size.y)std::swap(size.x, size.y);
		}
		else {
			if (size.x > size.y)std::swap(size.x, size.y);
		}

		btn.setSize(size);
		
		 from = pos;
		from.x += size.x / 2;
		from.y += size.y / 2;

		to = from;
		if (type == UP) {
			to.y -= 1;
		}
		else if (type == DOWN) {
			to.y += 1;
		}else if (type == LEFT) {
			to.x -= 1;
		}
		else if (type == RIGHT) {
			to.x += 1;
		}

		normal_arrow.update(from, to, 10);
		
		above_arrow.update(from, to, 7);
		above_arrow.setchose(sf::Color::Yellow);
		pressed_arrow.update(from, to, 7);
		

		show_arrow = &normal_arrow;

	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		btn.update(window, event);

		if (btn.click()) {
			is_click = 1;
		}
		if (btn.pressed()) {
			is_pressed = 1;
		}
		else is_pressed = 0;

		if (is_pressed) {
			show_arrow = &pressed_arrow;
		}
		else if (btn.above()) {
			show_arrow = &above_arrow;
		}
		else {
			show_arrow = &normal_arrow;
		}

	}

	

	bool click() {
		bool flag = is_click;
		is_click = 0;
		return flag;
	}

	bool pressed() {
		return is_pressed;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		//target.draw(btn,states);
		target.draw(*show_arrow,states);

	}
};