#pragma once
#include<SFML/Graphics.hpp>
#include"Button.cpp"
#include"input_box.cpp"
#include"ListNode.cpp"
#include"ArrowButton.cpp"
#include<mutex>
#define Pi 3.1415926
class Show :public sf::Drawable {
protected:
	Button returnbutton;
	InputBox inputbox;
	Button insertbutton;
	Button clearbutton;
	Button deletebutton;

	sf::Vector2i MousePos;

	bool is_catch = 0;

	ListNode<int>* temp;

	bool temp_vis = 0;

	std::mutex mtx;
	std::unique_ptr<std::thread> threadPtr;
	std::unique_ptr<std::thread>update_ptr;

	bool isReady = 1;
	bool isDone = 1;
	bool isUpdate = 1;
	bool is_return = 0;

	int str_size = 24;
public:
	Show(int len=999,std::u32string str=U"请输入") :returnbutton({25,25}, {100,50}, U"返回", 30),
		inputbox({200,400}, {250,50}, 24, str,len),
		insertbutton({460,400}, {150,50}, U"INSERT", 24),
		clearbutton({620,400}, {150,50}, U"CLEAR", 24),
		deletebutton({780,400}, {150,50}, U"DELETE", 24) 
		{}


	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		if (update_ptr == nullptr) {
			update_ptr = std::make_unique<std::thread>([this, &window, event]() {update_thread(window, event); });
		}
		if (update_ptr != nullptr && update_ptr->joinable() &&isDone && isUpdate) {
			update_ptr->join();
			update_ptr.reset();
			 
		}
	}

	virtual void update_press(const sf::RenderWindow& window, std::optional<sf::Event> event){}

	sf::Vector2f FromTo_Circle(sf::Vector2f begin, sf::Vector2f end, double x, int flag = 1) {
		sf::Vector2f center;
		center.x = (begin.x + end.x) / 2;
		center.y = (begin.y + end.y) / 2;
		sf::Vector2f pos = center;
		double delta_theta = x * Pi;
		double len = sqrt(pow(begin.x - center.x, 2) + pow(begin.y - center.y, 2));
		double cos = (begin.x - center.x) / len;
		double theta = std::acos(cos);

		/*std::cout << "theta1 " << theta << "  cos "<<cos<<std::endl;*/
		if (flag)theta += delta_theta;
		else theta -= delta_theta;
		/*std::cout << "theta2 " << theta << std::endl;*/
		pos.x = len * std::cos(theta) + center.x;
		pos.y = len * std::sin(theta) + center.y;
		return pos;
	}

	sf::Vector2f FromTo_Straight(sf::Vector2f begin, sf::Vector2f end, double times) {
		sf::Vector2f pos = begin;
		pos.x += (end.x - begin.x) * times;
		pos.y += (end.y - begin.y) * times;
		return pos;
	}

	void rect_move_circle(sf::Vector2f from, sf::Vector2f to, sf::Vector2f size, int num, int flag = 1, double speed = 150) {

		temp = new ListNode<int>(0, from, size, str_size);
		temp->set_num(num);
		temp_vis = 1;

		for (int j = 0; j < speed; j++) {
			isReady = 0;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			temp->setPosition(FromTo_Circle(from, to, (double)j / speed, flag));

			isReady = 1;
		}

		temp_vis = 0;

		isReady = 0;
		delete temp;

	}

	void rect_move_straight(sf::Vector2f from, sf::Vector2f to, sf::Vector2f size, int num, int flag = 1, double speed = 200) {

		temp = new ListNode<int>(0, from, size, str_size);
		temp->set_num(num);
		speed *= 10;
		temp_vis = 1;
		double s = 1;
		for (int j = 0; j < speed; j += s) {
			isReady = 0;
			if (j < speed / 2)s += 0.5;
			else if (s > 1)s -= 0.5;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			temp->setPosition(FromTo_Straight(from, to, (double)j / speed));

			isReady = 1;
		}

		temp_vis = 0;

		isReady = 0;
		delete temp;

	}


	virtual void Insert(int num) = 0;

	virtual void Delete(){}

	virtual void Clear() = 0;

	void button_set_unable() {
		clearbutton.set_unable();
		deletebutton.set_unable();
		insertbutton.set_unable();
		returnbutton.set_unable();
	}

	void button_set_able(){


		clearbutton.set_able();
		deletebutton.set_able();
		insertbutton.set_able();
		returnbutton.set_able();
	}

	void update_shine(sf::Time time) {
		inputbox.update_shine(time);
		
	}

	bool above() {
		return clearbutton.above() || deletebutton.above()
			|| returnbutton.above() || insertbutton.above()||inputbox.above();
	}

	virtual void update_position(sf::Vector2i delta){}

	void update_thread(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		//std::cout << "update 进程启用，id：" << std::this_thread::get_id() << std::endl;

		if (threadPtr != nullptr && threadPtr->joinable() && isDone == 1) {
			threadPtr->join();
			threadPtr.reset();
		}

		insertbutton.update(window, event);
		inputbox.update(window, event);
		clearbutton.update(window, event);
		deletebutton.update(window, event);
		returnbutton.update(window, event);
		
		
		isUpdate = 0;

		
		if (is_catch) {
			button_set_unable();
		}
		if (event->getIf<sf::Event::KeyPressed>()) {

			update_press(window, event);
		}

		if (inputbox.putting() || insertbutton.click()) {

			if (threadPtr == nullptr) {
				isDone = 0;

				threadPtr = std::make_unique<std::thread>([this]() {this->Insert(inputbox.get_num()); });
			}

		}

		if (returnbutton.click()) {
			is_return = 1;
		}
		
		if (deletebutton.click()) {

			if (threadPtr == nullptr) {
				isDone = 0;
				threadPtr = std::make_unique<std::thread>([this]() {this->Delete(); });
			}
		}
		
		if (clearbutton.click()) {
			Clear();
		}
		
		if (isDone &&!is_catch&&!above()&& sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (!is_catch) {			
				MousePos = sf::Mouse::getPosition(window);
			}
				is_catch = 1;
			}
		
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			is_catch = 0;
			button_set_able();
		}

		if (isDone&&is_catch) {
			
			sf::Vector2i NowPos = sf::Mouse::getPosition(window);
			sf::Vector2i delta=NowPos;

			delta.x -= MousePos.x;
			delta.y -= MousePos.y;

			MousePos = NowPos;

			update_position(delta);

		}
		

		while (1) {
			if (isReady)break;
		}

		
		
		isUpdate = 1;
	}

	bool Return() {
		bool temp = is_return;
		is_return = 0;
		if(temp)Clear();
		return temp;
	}

	void set_insert_name(std::u32string name) {
		insertbutton.set_str(name);
	}

	void set_delete_name(std::u32string name) {
		deletebutton.set_str(name);
	}

	
protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(inputbox, states);
		target.draw(insertbutton, states);
		target.draw(clearbutton, states);
		target.draw(deletebutton, states);
		target.draw(returnbutton, states);

		if (temp_vis) {
			target.draw(*temp, states);
		}
	}
};