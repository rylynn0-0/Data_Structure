#pragma once
#include<SFML/Graphics.hpp>
#include"ListNode.cpp"

template<class T>
class Queue :public sf::Drawable{
	std::vector<ListNode<T>*>myque;

	sf::RectangleShape rect1;
	sf::RectangleShape rect2;

	int len;
	T front;
	sf::Vector2f pos;
	sf::Vector2f size;
	int str_size = 24;

public:
	Queue<T>(sf::Vector2f pos = { 200,200 }, sf::Vector2f size = { 200,50 }, int str_size = 24) :
		pos(pos), size(size), str_size(str_size) {
		len = 0;

		rect1.setOutlineThickness(5);
		rect2.setOutlineThickness(5);

		rect1.setOutlineColor(sf::Color::Blue);
		rect2.setOutlineColor(sf::Color::Blue);

		rect1.setSize({ 300,0 });
		rect2.setSize({ 300,0 });

		sf::Vector2f position = pos;
		position.x -= 30;
		position.y -= 25;

		rect1.setPosition(position);
		position.y += size.y+50;
		rect2.setPosition(position);
	}

	sf::Vector2f getPosition() {
		return pos;
	}

	void setPosition(sf::Vector2f pos,bool f=0) {
		this->pos = pos;
		sf::Vector2f position = pos;
		position.x -= 30;
		position.y -= 25;

		if(!f)rect1.setPosition(position);
		position.y += size.y + 50;
		if(!f)rect2.setPosition(position);

		for (ListNode<T>* p : myque) {
			p->setPosition(pos);
			pos.x += p->getSize().x + 10;
		}

	}

	void push(T x){
		sf::Vector2f pos;
		if (len != 0) {
			pos = myque[len - 1]->getPosition();
			pos.x += myque[len - 1]->getSize().x + 10;
		}
		else pos = this->pos;

		if (len == 0) {
			front = x;
		}


		myque.push_back(new ListNode<T>(0, pos, size, str_size));
		if (typeid(T) == typeid(int))myque[len]->set_num(x);
		else if (typeid(T) == typeid(char))myque[len]->set_char(x);

		sf::Vector2f S = { 0,0 };
		S.x = pos.x + myque[len]->getSize().x + 25 - rect1.getPosition().x;
		S.x = std::max(S.x, (float)300.0);
		rect1.setSize(S);
		rect2.setSize(S);

		len++;
	}

	void pop(){
		if (len == 0)return;

		myque[0]->reset();
		ListNode<T>* p = myque[0];
		
		for (int i = 0; i < len-1; i++) {
			myque[i] = myque[i + 1];
		}
		
		if (typeid(T) == typeid(int))front=myque[0]->get_num();
		

		myque.pop_back();
		delete p;
		
		
		len--;
		if (len == 0)pos = { 200,200 };
		else pos = myque[0]->getPosition();

	}

	void clear(){
		for (ListNode<T>* p : myque) {
			delete p;
		}
		myque.clear();
		setPosition({ 200,200 });
		len = 0;
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		for (ListNode<T>* p : myque) {
			if (p)p->update(window, event);
		}
	}

	T Front() {
		return front;
	}

	sf::Vector2f getEndPosition() {
		sf::Vector2f position = pos;
		if (len) {
			position = myque[len - 1]->getPosition();
			position.x += myque[len - 1]->getSize().x;
		}return position;
	}

	sf::Vector2f getFrontPosition() {
		sf::Vector2f position = pos;
		if (len) {
			position = myque[0]->getPosition();
		}return position;
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

		for (ListNode<T>* p : myque) {
			if (p)target.draw(*p, states);
		}
	}
};