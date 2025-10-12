#pragma once
#include<SFML/Graphics.hpp>
#include"ListNode.cpp"
template<class T>
class Stack :public sf::Drawable {
	std::vector<ListNode<T>*> myst;

	sf::RectangleShape rect1;
	sf::RectangleShape rect2;
	sf::RectangleShape rect3;

	int len;
	T top;
	sf::Vector2f pos;
	sf::Vector2f size;
	int str_size = 24;

public:
	Stack<T>(sf::Vector2f pos={200,200}, sf::Vector2f size = {200,50}, int str_size = 24) :
	pos(pos),size(size),str_size(str_size){
		len = 0;
		rect1.setOutlineThickness(5);
		rect2.setOutlineThickness(5);
		rect3.setOutlineThickness(5);

		rect1.setOutlineColor(sf::Color::Blue);
		rect2.setOutlineColor(sf::Color::Blue);
		rect3.setOutlineColor(sf::Color::Blue);

		rect1.setSize({ 300,0 });
		rect3.setSize({ 300,0 });

		sf::Vector2f s = size;
		s.x = 0;
		s.y += 50;
		rect2.setSize(s);

		sf::Vector2f position = pos;
		position.x -= 30;
		position.y -= 25;

		rect1.setPosition(position);
		position.x += 5;
		rect2.setPosition(position);
		position.x -= 5;
		position.y += s.y;
		rect3.setPosition(position);
		
	}

	sf::Vector2f getPosition() {
		return pos;
	}

	void setPosition(sf::Vector2f pos) {
		this->pos = pos;
		sf::Vector2f position = pos;
		position.x -= 30;
		position.y -= 25;

		rect1.setPosition(position);
		position.x += 5;
		rect2.setPosition(position);
		position.x -= 5;
		position.y += size.y+50;
		rect3.setPosition(position);

		for (ListNode<T>* p : myst) {
			p->setPosition(pos);
			pos.x += p->getSize().x + 10;
		}

	}

	void push(T x) {
		sf::Vector2f pos;
		if (len != 0) {
			pos = myst[len - 1]->getPosition();
			pos.x += myst[len-1]->getSize().x + 10;
		}
		else pos = this->pos;

		top = x;
		

		myst.push_back(new ListNode<T>(0,pos,size,str_size));
		if (typeid(T) == typeid(int))myst[len]->set_num(x);
		else if (typeid(T) == typeid(char))myst[len]->set_char(x);

		
		sf::Vector2f S = { 0,0 };
		S.x =  pos.x + myst[len]->getSize().x + 25 - rect1.getPosition().x;
		S.x = std::max(S.x, (float)300.0);
		rect1.setSize(S);
		rect3.setSize(S);
		

		
		len++;
	}

	void pop() {
		if (len > 0) {
			myst[len - 1]->reset();
			ListNode<T>*p= myst[len - 1];
			myst.pop_back();
			delete p;
			len--;
			if (len > 0) {
				top = myst[len - 1]->get_num();
			}

			sf::Vector2f S = { 0,0 };
			if (len > 0)
				S.x = myst[len - 1]->getPosition().x + myst[len - 1]->getSize().x + 25 - rect1.getPosition().x;
			else S.x = 300;

			S.x = std::max(S.x, (float)300.0);
			rect1.setSize(S);
			rect3.setSize(S);
		}
	}

	void clear() {
		for (ListNode<T>* p : myst) {
			delete p;
		}
		myst.clear();
		setPosition({200,200});
		len = 0;
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		for (ListNode<T>* p : myst) {
			if (p)p->update(window, event);
		}
	}

	T Top() {
		if(len>0)return top;
		return 0;
	}

	sf::Vector2f getEndPosition() {
		sf::Vector2f position=pos;
		if (len) {
			position = myst[len - 1]->getPosition();
			position.x+=myst[len - 1]->getSize().x;
		}return position;
	}

	sf::Vector2f getEndSize() {
		sf::Vector2f s = size;
		if (len) {
			s = myst[len - 1]->getSize();
			
		}return s;
	}

	sf::Vector2f getRectPosition() {
		return rect1.getPosition();
	}

	sf::Vector2f getRectSize() {
		return rect1.getSize();
	}

	bool empty() {
		return len == 0;
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(rect1, states);
		target.draw(rect2, states);
		target.draw(rect3, states);

		for (ListNode<T>* p : myst) {
			if (p)target.draw(*p, states);
		}
	}
};