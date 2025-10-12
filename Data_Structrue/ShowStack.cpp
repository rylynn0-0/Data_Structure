#pragma once
#include"Stack.cpp"
#include"show.cpp"

template<class T>
class ShowStack :public Show {
	Stack<T> stack;
	

public:
	ShowStack<T>(){
		set_delete_name(U"POP");
		set_insert_name(U"PUSH");
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		stack.update(window, event);
		
		Show::update(window, event);
	}

	void update_position(sf::Vector2i delta) {
		sf::Vector2f pos = stack.getPosition();
		pos.x += delta.x;
		pos.y += delta.y;
		stack.setPosition(pos);
	}

	void Insert(int num) { 
		isReady = 0;
		std::lock_guard<std::mutex>lock(mtx);
		sf::Vector2f from = inputbox.getPosition();
		sf::Vector2f to = stack.getEndPosition();
		sf::Vector2f mid = stack.getRectPosition();
		mid.x += stack.getRectSize().x;
		mid.y = to.y;

		

		sf::Vector2f tempsize = { 200,50 };
		int len = std::to_string(num).length();
		tempsize.x = std::max(100, str_size * len + 20);
		mid.x += tempsize.x;

		rect_move_straight(from, mid, tempsize,num,0);
		rect_move_straight(mid, to, tempsize, num, 0);
		stack.push(num);
		isDone = 1;
		isReady = 1;
		
		inputbox.reset();
		inputbox.got_put();
		inputbox.set_normal();
	}

	void Delete() {
		isReady = 0;

		if (stack.empty()) {
			isReady = 1;
			isDone = 1;
			return;
		}

		std::lock_guard<std::mutex>lock(mtx);
		int num = stack.Top();


		sf::Vector2f from = stack.getEndPosition();
		from.x -= stack.getEndSize().x;
		sf::Vector2f mid = from;
		mid.x += 100;
		mid.x = std::max(mid.x, (float)525);
		sf::Vector2f to = mid;
		to.y = 0;

		stack.pop();

		sf::Vector2f tempsize = { 200,50 };
		
		int len = std::to_string(num).length();
		tempsize.x = std::max(100, str_size * len + 20);
		

		rect_move_straight(from, mid, tempsize, num, 0);
		rect_move_straight(mid, to, tempsize, num, 0);
		

		isDone = 1;
		isReady = 1;
	}

	void Clear() {
		stack.clear();
		inputbox.got_put();
		inputbox.reset();
		inputbox.set_normal();
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		
		target.draw(stack,states);
		Show::draw(target, states);
	}
};