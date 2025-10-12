#include<SFML/Graphics.hpp>
#include"Button.cpp"
class Scrollbox :public sf::Drawable{
	std::vector<std::u32string> str;
	int len;
	int maxx;
	int minn;
	int chosen;
	std::vector<Button*> buttons;
	sf::RectangleShape rect;
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f buttonSize = { 200,50 };
	bool  is_click=0;
	std::u32string click_state = U"";

public:
	Scrollbox(sf::Vector2f pos, sf::Vector2f size, std::vector<std::u32string>show) {
		rect.setPosition(pos);
		rect.setSize(size);
		rect.setOutlineThickness(5);
		rect.setOutlineColor(sf::Color::Yellow);
		rect.setFillColor(sf::Color(0,0,0,0));

		maxx = pos.y + size.y + 100;
		minn = pos.y - 100;
		chosen = 0;

		str = show;
		this->pos = pos;
		this->size = size;

		len = str.size();
		sf::Vector2f position = pos;
		position.x += size.x/2;
		position.x -= buttonSize.x/2;
		position.y += 25;
		
		for (int i = 0; i < len; i++) {
			
			buttons.push_back(new Button(position,buttonSize,str[i],30));

			position.y += buttonSize.y+35;

		}
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		if (auto x=event->getIf<sf::Event::MouseWheelScrolled>()) {
			int del = x->delta;
			int temp = chosen - del;
			if (temp >= 0 && temp < len) {
				
				chosen = temp;
				
				//std::cout << chosen << std::endl;
			}
		}
		

		for (Button* btn : buttons) {
			
			if (btn->click()) {
				is_click = 1;
				click_state = btn->getString();
			}

			if (btn->getPosition().y - 5 > pos.y &&
				btn->getPosition().y + btn->getSize().y + 5 < pos.y + size.y) {
				btn->update(window, event);
			}

			
		}
	}

	void update_shine(sf::Time time) {
		float del = buttons[chosen]->getPosition().y + buttons[chosen]->getSize().y / 2 - (pos.y + size.y / 2);
		//std::cout << del << std::endl;
		for (Button* btn : buttons) {
			
			btn->change_y(-del / 10);

			if (btn->getPosition().y - 5 > pos.y &&
				btn->getPosition().y + btn->getSize().y + 5 < pos.y + size.y) {
				btn->vis(1);
				
			}

			else if (btn->getPosition().y - 5 <= pos.y && btn->getPosition().y > minn) {
				float temp = 1 - std::abs(btn->getPosition().y - pos.y) / std::abs(minn - pos.y);
				btn->vis(temp);
			}

			else if (btn->getPosition().y + btn->getSize().y + 5 >= pos.y + size.y &&
				btn->getPosition().y + btn->getSize().y < maxx) {
				float temp = 1 - std::abs(btn->getPosition().y + btn->getSize().y - (pos.y + size.y)) / std::abs(maxx - (pos.y + size.y));
				btn->vis(temp);
			}

		}
	}

	bool click() {
		bool temp = is_click;
		is_click = 0;
		return temp;
	}

	std::u32string getstate() {
		return click_state;
	}

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		//target.draw(rect,states);
		for (Button* btn : buttons) {
			if(btn->getPosition().y-5>minn&&
				btn->getPosition().y+btn->getSize().y+5<maxx)
			target.draw(*btn, states);
		}

	}
};