#pragma once
#include"ListNode.h"
#include"input_box.h"
#include"Button.h"
#include<functional>
#include<math.h>
#include<mutex>
#include"show.h"


class List :public Show{
	std::vector<ListNode<int>*> mylist;
	sf::Vector2f size;
	sf::Vector2f pos;

	
	
	int n;
	int maxx;
	int used=0;
	int chosen = 0;
	int str_size = 0;

public:

	
	List(int n=5,sf::Vector2f pos= { 200,400 },sf::Vector2f size= { 200,50 },int str_size=24)
		
		{
		maxx = n;
		this->n = n;
		size.x /= 2;
		this->size = size;
		this->str_size = str_size;

		

		this->pos = pos;
		sf::Vector2f position = pos;
		position.y -= 200;

		for (int i = 0; i < maxx; i++) {
			
			mylist.push_back( new ListNode(0, position, size, str_size));
			position.x += size.x+10;
		}
		int len = 0;
		mylist[chosen]->set_chosen();
	}

	
	void reset(ListNode<int>* &node) {
		ListNode<int>* p = node;
		node = new ListNode(0, p->getPosition(), size, str_size);
		delete p;
	}

	void Insert(int num) {
		//std::cout << "insert 进程启用，id：" << std::this_thread::get_id() << std::endl;
		if (mylist[maxx - 1]->getPosition().y + 100 == pos.y && mylist[maxx - 1]->getPosition().x+mylist[maxx-1]->getSize().x + 250 > 1280) { isDone = 1; return; }
		
		for (int i = used; i > chosen; i--) {
			
			{
				if (i >= maxx)continue;
				int num_ = mylist[i - 1]->getNum();
				sf::Vector2f size = mylist[i]->getSize();
				int len = std::to_string(mylist[i - 1]->getNum()).length();
				size.x = std::max(100, str_size * len + 20);
				sf::Vector2f pos = mylist[i]->getPosition();

				std::lock_guard<std::mutex>lock(mtx);

				sf::Vector2f from = mylist[i - 1]->getPosition();
				sf::Vector2f to = mylist[i]->getPosition();
				to.x = from.x + mylist[i - 1]->getSize().x + 10;
				if (from.y != to.y) {
					to = mylist[i]->getPosition();
					rect_move_straight(from, to, size, num_,300);
				}
				else 
				rect_move_circle(from, to, size, num_);
			
				if (used >= maxx - 1)add();

				if (i >= maxx)continue;
				mylist[i]->set_num(num_);
				mylist[i]->setSize(size);
				//std::cout << "运行线程： "<<std::this_thread::get_id() << std::endl;
				
				update_position();
				
			
				isReady = 1;
			}
			
				
		}
		sf::Vector2f from = inputbox.getPosition();
		sf::Vector2f to = mylist[chosen]->getPosition();
		sf::Vector2f size = mylist[chosen]->getSize();
		int len = std::to_string(num).length();
		size.x = std::max(100, str_size * len + 20);

		{
			std::lock_guard<std::mutex>lock(mtx);
			int flag = (chosen >= maxx / 2) ? 0 : 1;
			rect_move_straight(from, to, size, num, flag);
			isReady = 1;
		}

		mylist[chosen]->set_num(num);
		mylist[chosen]->setSize(size);
		update_position();
		inputbox.got_put();
		inputbox.reset();
		insertbutton.set_release();
		if(used<maxx)used++;
		inputbox.set_normal();
		
		isDone = 1;
	}

	void Delete() {
		//std::cout << "delete 进程启用，id：" << std::this_thread::get_id() << std::endl;
		if (used < 1) {
			isReady = 1; isDone = 1; return;
		}
		for (int i = chosen; i < used-1; i++) {
			std::lock_guard<std::mutex>lock(mtx);

			sf::Vector2f from = mylist[i + 1]->getPosition();
			sf::Vector2f to = mylist[i]->getPosition();
			sf::Vector2f size = mylist[i + 1]->getSize();
			int number = mylist[i + 1]->getNum();

			if (from.y == to.y)rect_move_circle(from, to, size, number);
			else rect_move_straight(from, to, size, number,300);

			mylist[i]->set_num(number);
			update_position();
		}
		
		deletebutton.set_release();
		reset(mylist[used-1]);
		update_position();
		isDone = 1;
		isReady = 1;
		used--;
	}

	void Clear() {
		mylist.clear();
		clearbutton.set_release();
		sf::Vector2f position = pos;
		position.y -= 200;
		maxx = n;
		used = 0;
		for (int i = 0; i < maxx; i++) {

			mylist.push_back(new ListNode(0, position, size, str_size));
			position.x += size.x + 10;
		}
		chosen = 0;
		
		mylist[chosen]->set_chosen();
		inputbox.got_put();
		inputbox.reset();
		inputbox.set_normal();
	}

	void update_position() {
		sf::Vector2f pos = mylist[0]->getPosition();
		sf::Vector2f begin = pos;
		for (int i = 1; i < maxx; i++) {
			pos.x += mylist[i - 1]->getSize().x+10;
			if (pos.x + mylist[i]->getSize().x > 1280) {
				pos.x = begin.x;
				pos.y += 100;
			}
			mylist[i]->setPosition(pos);
		}
	}

	void set_up(int num) {
		insertbutton.set_release();
		
		inputbox.got_put();
		inputbox.reset();
		mylist[chosen]->set_num(num);
		sf::Vector2f size=mylist[chosen]->getSize();
		int len = std::to_string(num).length();
		size.x = std::max(100, str_size * len + 20);
		mylist[chosen]->setSize(size);
		update_position();
		
		
		
		mylist[chosen++]->set_unchosen();
		if (chosen >= maxx)chosen = 0;
		if(chosen<maxx)mylist[chosen]->set_chosen();
	}

	void set_down(int num) {
		insertbutton.set_release();

		inputbox.got_put();
		inputbox.reset();
		mylist[chosen]->set_num(num);
		sf::Vector2f size = mylist[chosen]->getSize();
		int len = std::to_string(num).length();
		size.x = std::max(100, str_size * len + 20);
		mylist[chosen]->setSize(size);
		update_position();


		mylist[chosen--]->set_unchosen();
		if (chosen < 0)chosen = maxx-1;
		mylist[chosen]->set_chosen();
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		
		for (int i = 0; i < maxx;i++ ) {
			mylist[i]->update(window, event);
			if (mylist[i]->chosen()&&i!=chosen) {
				mylist[chosen]->set_unchosen();
				chosen = i;
			}
		}

		Show::update(window, event);
	}

	void add() {
		sf::Vector2f position = mylist[maxx - 1]->getPosition();
		position.x += mylist[maxx - 1]->getSize().x + 10;
		
		mylist.push_back(new ListNode<int>(0, position, size, str_size));
		maxx++;
	
	}

	void update_press(const sf::RenderWindow& window, std::optional<sf::Event> event) {
	
		if (used >= maxx - 1 && chosen >= maxx - 1) add();
		
		if (auto input = event->getIf<sf::Event::KeyPressed>()) {

			int code = (int)input->scancode;
			int f = chosen - 1;
			int n = chosen + 1;
			if (f < 0)f += maxx;
			if (n >= maxx)n -= maxx;
			if (code == 87 && mylist[f]->is_able()) {
				mylist[chosen]->set_unchosen();
				chosen = f;
				mylist[chosen]->set_chosen();
			}
			else if (code == 86 && mylist[chosen]->is_able()) {
				mylist[chosen]->set_unchosen();
				chosen = n;
				mylist[chosen]->set_chosen();
			}
		}
		
		
		for (int i = 0; i < maxx; i++) {
			mylist[i]->update_press(window, event);
			if (mylist[i]->chosen()&&i!=chosen) {
				mylist[chosen]->set_unchosen();
				chosen = i;
			}
		}
	}

	void update_position(sf::Vector2i delta) {
		for (ListNode<int>* p : mylist) {
			sf::Vector2f pos = p->getPosition();
			pos.x += delta.x;
			pos.y += delta.y;
			p->setPosition(pos);
		}
	}


	
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		
		for (auto i:mylist) {
			target.draw(*i,states);
		}
		
		Show::draw(target, states);

	}
};