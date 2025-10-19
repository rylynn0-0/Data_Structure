#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#define PI 3.1415926

class Arrow :public sf::Drawable {
	sf::RectangleShape left;
	sf::RectangleShape right;
	sf::RectangleShape middle;
	sf::Vector2f from;
	sf::Vector2f to;
	double size;
	int x = 0;

	sf::Color color = sf::Color::Black;
public:
	

	Arrow(sf::Vector2f from={0,0}, sf::Vector2f to={0,0}, double size=5) {
		this->from = from;
		this->to = to;
		this->size = size;
		build();
	}

	void build() {
		double length = sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
		sf::Vector2f from_to;
		sf::Vector2f vertical;

		from_to.x = to.x - from.x;
		from_to.y = to.y - from.y;

		vertical.x = from_to.y / length;
		vertical.y = (-from_to.x) / length;

		sf::Vector2f pos = to;
		pos.x -= vertical.x * size / 2;
		pos.y -= vertical.y * size / 2;

		middle.setPosition(pos);

		pos.x -= vertical.x * size * (sqrt(2) - 1) / 2;
		pos.y -= vertical.y * size * (sqrt(2) - 1) / 2;

		right.setPosition(pos);
		left.setPosition(pos);

		middle.setSize(sf::Vector2f(length, size));
		left.setSize(sf::Vector2f(25 + size, size));
		right.setSize(sf::Vector2f(25, size));

		middle.setFillColor(sf::Color::White);
		right.setFillColor(sf::Color::White);
		left.setFillColor(sf::Color::White);

		middle.setOutlineColor(color);
		right.setOutlineColor(color);
		left.setOutlineColor(color);

		//left.set

		middle.setOutlineThickness(x);
		right.setOutlineThickness(x);
		left.setOutlineThickness(x);

		double cos = from_to.x / length;
		double sin = from_to.y / length;

		double theta;
		if (to.y >= from.y)
			theta = std::acos(cos) * 180 / PI;
		else
			theta = asin(sin) * 180 / PI;
		if (to.y < from.y && to.x < from.x) {
			double delta = std::acos(cos) * 180 / PI * 2 + 180;
			theta -= delta;
		}


		middle.setRotation(sf::degrees(180 + theta));
		right.setRotation(sf::degrees(135 + theta));
		left.setRotation(sf::degrees(225 + theta));


	}

	void setchose(sf::Color color) {
		this->color = color;
		x = 2;
		build();
	}

	void setunchose() {
		color = sf::Color::Black;
		x = 0;
		build();
	}

	void update(sf::Vector2f from, sf::Vector2f to, double size) {
		if (this->from == from && this->to == to)return;

		this->from = from;
		this->to = to;
		this->size = size;
		build();
	}


private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(middle, states);
		target.draw(right, states);
		target.draw(left, states);


	}
};
