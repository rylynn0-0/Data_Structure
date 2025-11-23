#pragma once
#include<SFML/Graphics.hpp>
#include"Scrollbox.h"

class Home :public sf::Drawable{
	sf::Text text;
	sf::Font font;
	Scrollbox *scrbox;
	std::vector<std::u32string> str ;
	bool is_click = 0;
	std::u32string clickstate = U"";

public:
	Home():text(font) {
		str.push_back(U"顺序表");
		str.push_back(U"链式表");
		str.push_back(U"栈");
		str.push_back(U"队列");
		str.push_back(U"KMP");
		str.push_back(U"二叉树");
		str.push_back(U"哈哈");
		Init();
	}

	void Init() {
		scrbox = new Scrollbox({ 450,350 }, { 300,125 }, str);

		if (!font.openFromFile("Dymon-ShouXieTi.otf")) {
			std::cout << "error" << std::endl;
			return;
		}

		text.setFont(font);
		text.setCharacterSize(100);
		text.setPosition({300,100});
		text.setFillColor(sf::Color::White);
		text.setOutlineThickness(10);
		text.setOutlineColor(sf::Color::Blue);
		text.setString(U"数据结构可视化");

	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		scrbox->update(window, event);
		if (scrbox->click()) {
			is_click = 1;
			clickstate = scrbox->getstate();
		}
	}

	void update_shine(sf::Time time) {
		scrbox->update_shine(time);
	}

	bool click() {
		bool temp = is_click;
		is_click = 0;
		return temp;
	}

	std::u32string getstate() {
		return clickstate;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(*scrbox,states);
		target.draw(text, states);
	}
};