#pragma once
#include"ListNode.cpp"
#include"input_box.cpp"
#include"Button.cpp"
#include<functional>
#include<math.h>
#include<mutex>
#define Pi 3.1415926

class List :public sf::Drawable{
	std::vector<ListNode*> mylist;
	sf::Vector2f size;
	sf::Vector2f pos;

	ListNode *temp;
	
	int n;
	int maxx;
	int used=0;
	int chosen = 0;
	int str_size = 0;

	InputBox inputbox;
	Button insertbutton;
	Button clearbutton;
	Button deletebutton;

	std::mutex mtx;
	std::unique_ptr<std::thread> threadPtr;
	std::unique_ptr<std::thread>update_ptr;


	

	bool isReady = 1;
	bool isDone = 1;
	bool temp_vis = 0;

	

public:

	
	List(int n,sf::Vector2f pos,sf::Vector2f size,int str_size=24):
		inputbox(pos,sf::Vector2f(size.x+50,size.y), 24, U"请输入"),
		insertbutton(sf::Vector2f(pos.x+size.x+60,pos.y),sf::Vector2f(size.x/4*3,size.y),U"INSERT",24),
		clearbutton(sf::Vector2f(pos.x+size.x+60+size.x/4*3+10,pos.y), sf::Vector2f(size.x / 4 * 3, size.y), U"CLEAR", 24),
		deletebutton(sf::Vector2f(pos.x+size.x/4*10+80,pos.y),sf::Vector2f(size.x/4*3,size.y),U"DELETE",24)
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

	sf::Vector2f FromTo_Circle(sf::Vector2f begin, sf::Vector2f end, double x,int flag=1) {
		sf::Vector2f center;
		center.x = (begin.x + end.x) / 2;
		center.y = (begin.y + end.y) / 2;
		sf::Vector2f pos=center;
		double delta_theta = x *Pi;
		double len = sqrt(pow(begin.x - center.x, 2) + pow(begin.y - center.y, 2));
		double cos = (begin.x - center.x) / len;
		double theta = std::acos(cos);
		
		/*std::cout << "theta1 " << theta << "  cos "<<cos<<std::endl;*/
		if (flag)theta += delta_theta;
		else theta -= delta_theta;
		/*std::cout << "theta2 " << theta << std::endl;*/
		pos.x = len * std::cos(theta)+center.x;
		pos.y = len * std::sin(theta)+center.y;
		return pos;
	}

	sf::Vector2f FromTo_Straight(sf::Vector2f begin, sf::Vector2f end, double times) {
		sf::Vector2f pos = begin;
		pos.x += (end.x - begin.x) * times;
		pos.y += (end.y - begin.y) * times;
		return pos;
	}

	void rect_move_circle(sf::Vector2f from, sf::Vector2f to, sf::Vector2f size, int num, int flag = 1, double speed = 150) {
		
		temp = new ListNode(0, from, size, str_size);
		temp->set_num(num);
		temp_vis = 1;

		for (int j = 0; j < speed; j++) {
			isReady = 0;

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			temp->setPosition(FromTo_Circle(from, to, (double)j / speed,flag));
			
			isReady = 1;
		}

		temp_vis = 0;

		isReady = 0;
		delete temp;
	
	}

	void rect_move_straight(sf::Vector2f from, sf::Vector2f to, sf::Vector2f size, int num, int flag = 1, double speed = 200) {

		temp = new ListNode(0, from, size, str_size);
		temp->set_num(num);
		speed *= 10;
		temp_vis = 1;
		double s = 1;
		for (int j = 0; j < speed; j+=s) {
			isReady = 0;
			if (j < speed / 2)s+=0.5;
			else if (s > 1)s-=0.5;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			temp->setPosition(FromTo_Straight(from, to, (double)j / speed));

			isReady = 1;
		}

		temp_vis = 0;

		isReady = 0;
		delete temp;

	}

	void reset(ListNode* &node) {
		ListNode* p = node;
		node = new ListNode(0, p->getPosition(), size, str_size);
		delete p;
	}

	void Insert(int num) {
		std::cout << "insert 进程启用，id：" << std::this_thread::get_id() << std::endl;
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
		std::cout << "delete 进程启用，id：" << std::this_thread::get_id() << std::endl;
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
		if (update_ptr == nullptr) {
			update_ptr = std::make_unique<std::thread>([this,&window,&event]() {update_thread(window, event); });
		}
		if (update_ptr != nullptr && update_ptr->joinable()&&isDone) {
			update_ptr->join();
			update_ptr.reset();
		}
	}

	void update_thread(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		//std::cout << "update 进程启用，id：" << std::this_thread::get_id() << std::endl;
		insertbutton.update(window, event);
		inputbox.update(window, event);
		clearbutton.update(window, event);
		deletebutton.update(window, event);
		if (event->getIf<sf::Event::KeyPressed>()) {
			update_press(window, event);
		}

		

		if (inputbox.putting()||insertbutton.click()) {
			if (threadPtr == nullptr) {
				isDone = 0;
				threadPtr = std::make_unique<std::thread>([this]() {this->Insert(inputbox.get_num()); });
			}
			
		}

		if (deletebutton.click()) {
			if (threadPtr == nullptr) {
				isDone = 0;
				threadPtr = std::make_unique<std::thread>([this]() {this->Delete(); });
			}
		}

		if (threadPtr != nullptr && threadPtr->joinable() && isDone == 1) {
			threadPtr->join();
			threadPtr.reset();			
		}

		while (1) {
			if (isReady)break;
		}

		if (clearbutton.click()) {
			Clear();
		}

	
		


		for (int i = 0; i < maxx;i++ ) {
			mylist[i]->update(window, event);
			if (mylist[i]->chosen()&&i!=chosen) {
				mylist[chosen]->set_unchosen();
				chosen = i;
				std::cout << 1 << std::endl;
			}
		}
	}

	void add() {
		sf::Vector2f position = mylist[maxx - 1]->getPosition();
		position.x += mylist[maxx - 1]->getSize().x + 10;
		
		mylist.push_back(new ListNode(0, position, size, str_size));
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

	void update_shine(sf::Time time) {
		inputbox.update_shine(time);
	}

	
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(inputbox, states);
		target.draw(insertbutton, states);
		target.draw(clearbutton, states);
		target.draw(deletebutton, states);
		for (auto i:mylist) {
			target.draw(*i,states);
		}
		if (temp_vis) {
			target.draw(*temp, states);
		}

	}
};