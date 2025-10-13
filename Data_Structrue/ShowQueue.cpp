#pragma once
#include"Queue.cpp"
#include"show.cpp"
template <class T>
class ShowQueue :public Show {
	Queue<T> queue;

public:
	ShowQueue<T>() {
		set_delete_name(U"POP");
		set_insert_name(U"PUSH");
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		queue.update(window, event);

		Show::update(window, event);
	}

	void update_position(sf::Vector2i delta) {
		sf::Vector2f pos = queue.getPosition();
		pos.x += delta.x;
		pos.y += delta.y;
		queue.setPosition(pos);
	}

	void Insert(int num) {
		isReady = 0;
		std::lock_guard<std::mutex>lock(mtx);
		sf::Vector2f from = inputbox.getPosition();
		sf::Vector2f to = queue.getEndPosition();
		sf::Vector2f mid = queue.getRectPosition();
		mid.x += queue.getRectSize().x;
		mid.y = to.y;



		sf::Vector2f tempsize = { 200,50 };
		int len = std::to_string(num).length();
		tempsize.x = std::max(100, str_size * len + 20);
		mid.x += tempsize.x;

		rect_move_straight(from, mid, tempsize, num, 0);
		rect_move_straight(mid, to, tempsize, num, 0);
		queue.push(num);
		isDone = 1;
		isReady = 1;

		inputbox.reset();
		inputbox.got_put();
		inputbox.set_normal();
	}

	void Delete() {
		isReady = 0;

		if (queue.empty()) {
			isReady = 1;
			isDone = 1;
			return;
		}

		sf::Vector2f T = queue.getPosition();

		std::lock_guard<std::mutex>lock(mtx);
		int num = queue.Front();


		sf::Vector2f from = queue.getFrontPosition();
		
		sf::Vector2f mid = from;
		mid.x -= 200;
		//mid.x = std::min(mid.x, (float)525);
		sf::Vector2f to = mid;
		to.y = 0;

		queue.pop();

		sf::Vector2f tempsize = { 200,50 };

		int len = std::to_string(num).length();
		tempsize.x = std::max(100, str_size * len + 20);
		sf::Vector2f F = queue.getPosition();


		rect_move_straight(from, mid, tempsize, num, 0);
		rect_move_straight(mid, to, tempsize, num, 0);


		double speed = 200;
		double s = 1;
		for (int j = 0; j < speed; j += s) {
			isReady = 0;
			if (j < speed / 2)s += 0.5;
			else if (s > 1)s -= 0.5;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			queue.setPosition(FromTo_Straight(F, T, (double)j / speed),1);

			isReady = 1;
		}



		isDone = 1;
		isReady = 1;
	}

	void Clear() {
		queue.clear();
		inputbox.got_put();
		inputbox.reset();
		inputbox.set_normal();
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(queue, states);
		Show::draw(target, states);
	}
};