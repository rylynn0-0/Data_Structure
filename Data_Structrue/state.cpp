#pragma once
#include"Sequential_List.h"
#include<iostream>
#include<SFML/Graphics.hpp>
#include"Button.cpp"
#include"input_box.cpp"
#include"ListNode.cpp"
#include"List.cpp"
#include"Arrow.cpp"
#include"Home.cpp"
#include"Linked.cpp"
#include"ShowStack.cpp"

enum STATE {
	HOME,
	RANKEDLIST,
	LINKEDLIST,
	STACK
};

class State : public sf::Drawable {
	List* rankedlist;
	Linked* linkedlist;
	ShowStack<int>* stack;
	Home* home;
	STATE state;
	
public :
	State() { 
		home = new Home();		
		state = STATE::HOME; 
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		switch (state) {
		case STATE::RANKEDLIST:
			rankedlist->update(window, event);
			if (rankedlist->Return()) {
				state = STATE::HOME;
			}
			break;
		case STATE::LINKEDLIST:
			linkedlist->update(window, event);
			if (linkedlist->Return()) {
				state = STATE::HOME;
			}
			break;
		case STATE::STACK:
			stack->update(window, event);
			if (stack->Return()) {
				state = STATE::HOME;
			}
			break;
		case STATE::HOME:
			home->update(window, event);
			if (home->click()) {
				if (home->getstate() == U"Ë³Ðò±í") {
					state = STATE::RANKEDLIST;
					if(!rankedlist)rankedlist = new List();
				}

				if (home->getstate() == U"Á´Ê½±í") {
					state = STATE::LINKEDLIST;
					if (!linkedlist)linkedlist = new Linked();
				}

				if (home->getstate() == U"Õ»") {
					state = STATE::STACK;
					if (!stack)stack = new ShowStack<int>();
				}
				
			}
			break;
		default:
			break;
		}
	}

	void update_shine(sf::Time time) {
		switch (state) {
		case STATE::RANKEDLIST:
			rankedlist->update_shine(time);
			break;
		case STATE::LINKEDLIST:
			linkedlist->update_shine(time);
			break;
		case STATE::STACK:
			stack->update_shine(time);
			break;
		case STATE::HOME:
			home->update_shine(time);
			break;
		default:
			break;
		}
	}

	

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		switch (state) {
		case STATE::RANKEDLIST:
			target.draw(*rankedlist, states);
			break;
		case STATE::LINKEDLIST:
			target.draw(*linkedlist, states);
			break;
		case STATE::STACK:
			target.draw(*stack, states);
			break;
		case STATE::HOME:
			target.draw(*home, states);
			break;
		default:
			break;
		}
		
	}
};
											