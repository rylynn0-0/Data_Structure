#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<thread>
#include <shared_mutex>
#include <mutex>
#include"Arrow.h"
#include"ListNode.h"

class Pointer :public sf::Drawable {
	sf::RectangleShape r1;
	sf::RectangleShape r2;

	sf::Font font;
	sf::Text text;

	Arrow arrow;
	sf::Vector2f from;
	sf::Vector2f to;
	int is_left = 0;
	double size;
public:

	Pointer(int is_left, sf::Vector2f from = { 0,0 }, sf::Vector2f to = { 0,0 }, double size = 5):text(font)
	{
		this->is_left = is_left;
		this->from = from;
		this->to = to;
		this->size = size;

		if (!font.openFromFile("Dymon-ShouXieTi.otf")) {
			std::cout << "error" << std::endl;
			return;
		}

		build();
	}

	void build() {
		r2.setPosition(from);
		if (!is_left) {
			sf::Vector2f f = from;
			f.y -= 40;
			r1.setPosition(f);
		}
		else r1.setPosition(from);
		
		sf::Vector2f s;
		s.x = size;
		s.y = 40;

		r1.setSize(s);
		
		s.x = to.x - from.x+size/2;
		s.y = size;
		r2.setSize(s);

		r1.setFillColor(sf::Color::White);
		r2.setFillColor(sf::Color::White);

		r1.setOutlineColor(sf::Color::White);
		r2.setOutlineColor(sf::Color::White);

		r1.setOutlineThickness(0);
		r2.setOutlineThickness(0);

		text.setCharacterSize(24);
		text.setFont(font);
		text.setFillColor(sf::Color::White);
		
		sf::Vector2f text_pos = (from + to) ;
		text_pos.x /= 2;
		text_pos.y /= 2;

		text_pos.x -= 24 * 2;
		if (!is_left)text_pos.x -= 12;

		if (is_left)text_pos.y -= 39;
		else text_pos.y += 10;

		text.setPosition(text_pos);

		if (is_left)text.setString("LEFT");
		else text.setString("RIGHT");

		from = to;
		if (is_left)to.y += 40;
		else to.y -= 40;

		arrow.update(from, to,size);

	}

	void update(sf::Vector2f from, sf::Vector2f to, double size = 5) {
		if (this->from == from && this->to == from && this->size == size)return;
		this->from = from;
		this->to = to;
		this->size = size;
		build();
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {

		target.draw(r1, states);
		target.draw(r2, states);
		target.draw(arrow, states);
		target.draw(text, states);


	}
};



class TreeNode :public sf::Drawable {
public:
	enum STATE {
		LINEAR,LINKED
	};

	int index = 1;
	int linkedindex = 0;
	int delta = 100;
	std::vector<TreeNode*> son;
	bool able = 0;
	TreeNode* father;
	TreeNode* next;
	TreeNode* front;
	
	STATE state;

	std::unique_ptr<std::thread> add_ptr;

	~TreeNode() {
		

		if (add_ptr != nullptr && add_ptr->joinable()) {
			add_ptr->join();
			add_ptr.reset();
		}
		delete rightnode;
		delete leftnode;
		
	}

	

private:

	
	
	int num;
	int len;
	std::string str;
	int str_size;

	std::vector<Arrow*> arrow;

	sf::Vector2f linear_pos;
	sf::Vector2f linked_pos;

	sf::Vector2f linear_size;
	double linked_size;

	ListNode<int>* leftnode;
	ListNode<int>* rightnode;

	Pointer leftpoint;
	Pointer rightpoint;

	sf::RectangleShape normal_linear;
	sf::RectangleShape above_linear;
	sf::RectangleShape pressed_linear;
	
	sf::RectangleShape* show_linear;

	sf::CircleShape normal_linked;
	sf::CircleShape above_linked;
	sf::CircleShape pressed_linked;
	sf::CircleShape *show_linked;

	sf::Font font;
	sf::Text LinearText;
	sf::Text LinkedText;

	sf::SoundBuffer aboveBuffer;
	sf::SoundBuffer choseBuffer;

	sf::Sound aboveSound;
	sf::Sound choseSound;

	
	std::mutex mtx;
	std::shared_mutex shared_mtx;
	std::recursive_mutex re_mtx;

	

	TreeNode* chose;

	float left;
	float right;

	bool is_above_linear;
	bool is_pressed_linear;
	bool is_chosen_linear;
	bool is_click_linear;
	bool vis = 1;

	bool moveDone = 1;
	bool Done = 1;
	bool updatePosDone = 1;
	bool updateSonPosDone = 1;
	bool addSonDone=1;
	
	TreeNode* root;
	int maxIndex;

	bool is_above_linked;
	bool is_pressed_linked;
	bool is_chosen_linked;
	bool is_click_linked;

	bool sound_half = 0;

public:
	TreeNode(sf::Vector2f linear_pos={200,200}, sf::Vector2f linked_pos={600,100}, sf::Vector2f linear_size = {100,50}, double linked_size = 35, int str_size = 24)
		:LinearText(font),LinkedText(font), aboveSound(aboveBuffer), choseSound(choseBuffer),
		linear_pos(linear_pos),linked_pos(linked_pos),linear_size(linear_size),
		linked_size(linked_size),str_size(str_size),leftpoint(1),rightpoint(0){
		Init();
		
	}

	void InitLinear() {
		normal_linear.setPosition(linear_pos);
		normal_linear.setSize(linear_size);
		normal_linear.setFillColor(sf::Color::White);
		normal_linear.setOutlineThickness(5);
		normal_linear.setOutlineColor(sf::Color::Blue);

		above_linear.setPosition(linear_pos);
		above_linear.setSize(linear_size);
		above_linear.setFillColor(sf::Color::White);
		above_linear.setOutlineThickness(5);
		above_linear.setOutlineColor(sf::Color::Yellow);

		pressed_linear.setPosition(linear_pos);
		pressed_linear.setSize(linear_size);
		pressed_linear.setFillColor(sf::Color::White);

		
		
		

		sf::Vector2f pos = linear_pos;
		pos.x += (index)*(linear_size.x + 10);
		leftnode = new ListNode<int>(0,pos,linear_size);
		pos.x += (linear_size.x + 10);
		rightnode = new ListNode<int>(0,pos,linear_size);

		update_point();



		show_linear = &normal_linear;
	}

	void update_point() {
		sf::Vector2f left_from = linear_pos;
		left_from.x += linear_size.x / 2;
		left_from.y -= 50;
		sf::Vector2f left_to = left_from;
		left_to.x += (index) * (linear_size.x + 10);

		sf::Vector2f right_from = linear_pos;
		right_from.x += linear_size.x / 2;
		right_from.y += 50;
		right_from.y += linear_size.y;
		sf::Vector2f right_to = right_from;
		right_to.x += (index + 1) * (linear_size.x+ 10);

		leftpoint.update(left_from, left_to);
		rightpoint.update(right_from, right_to);
	}

	int getMaxIndex() {
		return maxIndex;
	}

	void InitLinked() {
		normal_linked.setPosition(linked_pos);
		normal_linked.setRadius(linked_size);
		normal_linked.setFillColor(sf::Color::White);
		normal_linked.setOutlineThickness(5);
		normal_linked.setOutlineColor(sf::Color::Blue);

		above_linked.setPosition(linked_pos);
		above_linked.setRadius(linked_size);
		above_linked.setFillColor(sf::Color::White);
		above_linked.setOutlineThickness(5);
		above_linked.setOutlineColor(sf::Color::Yellow);

		pressed_linked.setPosition(linked_pos);
		pressed_linked.setRadius(linked_size);
		pressed_linked.setFillColor(sf::Color::White);


		show_linked = &normal_linked;
	}

	double Distance(sf::Vector2f p) {
		double x = abs(linked_pos.x + linked_size - p.x);
		double y = abs(linked_pos.y + linked_size - p.y);
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	void InitText() {
		sf::Vector2f position;
		
			position = linked_pos;
			len = str.length();
			position.x += linked_size;
			position.y += linked_size;
			position.x -= str_size * len / 2;
			position.y -= str_size / 3 * 2;

			LinkedText.setFont(font);
			LinkedText.setCharacterSize(str_size);
			LinkedText.setPosition(position);
			LinkedText.setFillColor(sf::Color::Black);
			LinkedText.setString(str);
		
		
			position = linear_pos;
			len = str.length();
			position.x += linear_size.x / 2;
			position.y += linear_size.y / 2;
			position.x -= str_size * len / 2;
			position.y -= str_size / 3 * 2;
		
			LinearText.setFont(font);
			LinearText.setCharacterSize(str_size);
			LinearText.setPosition(position);
			LinearText.setFillColor(sf::Color::Black);
			LinearText.setString(str);

		
	}

	void Init() {

		state = STATE::LINKED;

		if (!font.openFromFile("Dymon-ShouXieTi.otf")) {
			std::cout << "error" << std::endl;
			return;
		}

		if (!aboveBuffer.loadFromFile("above.mp3")) {
			std::cout << "error" << std::endl;
			return;
		}

		if (!choseBuffer.loadFromFile("chose.mp3")) {
			std::cout << "error" << std::endl;
			return;
		}

		InitLinear();
		InitLinked();
		InitText();
		arrow.clear();
		root = this;
		able = 1;
		left = linked_pos.x;
		right = linked_pos.x;
		chose = this;
		
	}

	void linked_update(const sf::RenderWindow& window, std::optional<sf::Event> event){
		sf::Vector2i pos = sf::Mouse::getPosition(window);

		float delta_x = std::abs(pos.x - linked_pos.x - linked_size);
		float delta_y = std::abs(pos.y - linked_pos.y - linked_size);
		float del = std::sqrt(std::pow(delta_x, 2) + std::pow(delta_y, 2));

		if (del <= linked_size) {
			if (!is_above_linked)aboveSound.play();
			is_above_linked = 1;
		}
		else is_above_linked = 0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_above_linked) {
			is_pressed_linked = 1;
			choseSound.play();
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			choseSound.pause();
			sound_half = 1;

		}

		if (is_pressed_linked && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			is_pressed_linked = 0;
			is_click_linked = 1;
		}

		if (is_pressed_linked) {
			show_linked = &pressed_linked;

		}
		else if (is_above_linked||is_chosen_linked) {
			show_linked = &above_linked;
		}
		else {
			show_linked = &normal_linked;
		}
	}

	void linear_update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		

		if (pos.x >= show_linear->getPosition().x
			&& pos.x <= show_linear->getPosition().x + show_linear->getSize().x
			&& pos.y >= show_linear->getPosition().y
			&& pos.y <= show_linear->getPosition().y + show_linear->getSize().y) {
			if (!is_above_linear)aboveSound.play();
			is_above_linear = 1;
		}
		else is_above_linear = 0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && is_above_linear) {
			is_pressed_linear = 1;
			choseSound.play();
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			choseSound.pause();
			sound_half = 1;

		}

		if (is_pressed_linear && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			is_pressed_linear = 0;
			is_click_linear = 1;
		}

		if (is_pressed_linear) {
			show_linear = &pressed_linear;

		}
		else if (is_above_linear||is_chosen_linked) {
			show_linear = &above_linear;
		}
		else {
			show_linear = &normal_linear;
		}

		
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		/*if (!Done)return;*/
		if (!addSonDone)return;
		if (state == STATE::LINKED) {
			linked_update(window, event);
		}
		else if (state == STATE::LINEAR) {
			linear_update(window, event);
		}
		for (TreeNode* s : son) {
			s->update(window, event);
		}
		maxIndex = index;
		for (TreeNode* s : son) {
			maxIndex = std::max(maxIndex, s->getMaxIndex());
		}
	}

	void update_shine(sf::Time time) {
		/*update_arrow();
		for (TreeNode* s : son) {
			s->update_shine(time);
		}*/
	}

	bool click(){
		bool flag = is_click_linked||is_click_linear;

		if (is_click_linked||is_click_linear) {
			chose = this;
		}

		is_click_linked = 0;
		is_click_linear = 0;

		

		if (flag && sound_half) {
			choseSound.play();
			sound_half = 0;
		}

		for (TreeNode* s : son) {
			if (s->click()) {
				chose = s->getchose();
				flag = 1;
			}
		}

		return flag;
	}

	TreeNode* getchose() {
		return chose;
	}

	bool pressed() {
		return is_pressed_linear||is_pressed_linked;
	}

	void set_release() {
		is_pressed_linked = 0;
		is_pressed_linear = 0;

	}

	sf::Vector2f getPosition() {
		if(state==STATE::LINKED)return show_linked->getPosition();
		else if (state == STATE::LINEAR)return show_linear->getPosition();
	}

	sf::Vector2f getSize() {
		return show_linear->getPosition();
	}

	float getRadius() {
		return show_linked->getRadius();
	}

	void update_position(sf::Vector2i delta) {
		sf::Vector2f pos = getPosition();
		pos.x += delta.x;
		pos.y += delta.y;
		setPosition(pos);
		if (state == STATE::LINKED) {
			left += delta.x;
			right += delta.x;
		}
		for (TreeNode* s : son) {
			s->update_position(delta);
		}
		update_arrow();
	}

	void setPosition(sf::Vector2f pos) {
		if (state == STATE::LINEAR)setLinearPosition(pos);
		else if (state == STATE::LINKED)setLinkedPosition(pos);
	}

	void setLinearPosition(sf::Vector2f pos) {
		linear_pos = pos;

		normal_linear.setPosition(pos);
		above_linear.setPosition(pos);
		pressed_linear.setPosition(pos);

		pos.x += normal_linear.getSize().x / 2;
		pos.y += normal_linear.getSize().y / 2;
		pos.x -= str_size * len / 2;
		pos.y -= str_size / 3 * 2;
		LinearText.setPosition(pos);

		pos = linear_pos;
		pos.x += (index) * (linear_size.x + 10);
		leftnode->setPosition(pos);
		pos.x +=  (linear_size.x + 10);
		rightnode->setPosition(pos);

		update_point();

		
		
	}

	TreeNode::STATE getState() {
		return state;
	}

	void setLinkedPosition(sf::Vector2f pos) {
		linked_pos = pos;
		normal_linked.setPosition(pos);
		above_linked.setPosition(pos);
		pressed_linked.setPosition(pos);

		pos.x += normal_linked.getRadius();
		pos.y += normal_linked.getRadius();
		pos.x -= str_size * len / 2;
		pos.y -= str_size / 3 * 2;

		LinkedText.setPosition(pos);
	}

	void move_setLinkedPosition_thread(sf::Vector2f pos, double speed=75) {
		sf::Vector2f from = linked_pos;
		sf::Vector2f to = pos;
		
		double temp = 0;
		//std::cout << 123 << std :: endl;
		for (double x = 0; x < speed; x += temp) {
			if (x < speed / 2)temp += 1;
			else if(temp>2)temp-=1;
			moveDone = 0;
			std::lock_guard<std::mutex>lock(mtx);
			sf::Vector2f dis = to - from;
			
			move(from, to, x / speed);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			moveDone = 1;
		}

		setLinkedPosition(to);
		father->update_arrow();
		update_arrow();

		
	}

	void move(sf::Vector2f from, sf::Vector2f to, double k) {
		k = std::min(k, (double)1);
		sf::Vector2f mid;
		mid.x = to.x - from.x;
		mid.y = to.y - from.y;
		from.x += mid.x * k;
		from.y += mid.y * k;
		setLinkedPosition(from);
		father->update_arrow();
		update_arrow();
		
	}

	void setLinearSize(sf::Vector2f size) {
		linear_size = size;	
		normal_linear.setSize(size);
		above_linear.setSize(size);
		pressed_linear.setSize(size);
	}

	void setLinkedRadius(float radius) {
		
		linked_size = radius;
		normal_linked.setRadius(radius);
		above_linked.setRadius(radius);
		pressed_linked.setRadius(radius);
	}

	void setNum(int num) {
		this->num = num;
		str = std::to_string(num);
		sf::Vector2f position;
		
			position = linked_pos;
			len = str.length();
			position.x += linked_size;
			position.y += linked_size;
			position.x -= str_size * len / 2;
			position.y -= str_size / 3 * 2;

			LinkedText.setPosition(position);
			LinkedText.setString(str);
		
			position = linear_pos;
			len = str.length();
			position.x += linear_size.x / 2;
			position.y += linear_size.y / 2;
			position.x -= str_size * len / 2;
			position.y -= str_size / 3 * 2;
		
			LinearText.setPosition(position);
			LinearText.setString(str);
	}

	void setFather(TreeNode* f) {
		father = f;
	}

	void setSon(TreeNode* s) {
		son.push_back(s);
	}

	TreeNode* getson(int index) {
		if (index > son.size() - 1)return NULL;
		
		if (add_ptr != nullptr && add_ptr->joinable()) {
			add_ptr->join();
			add_ptr.reset();
		}

		return son[index];

	}

	sf::Vector2f getLinkedPosition() {
		std::shared_lock<std::shared_mutex> lock(shared_mtx);
		return linked_pos;
	}

	sf::Vector2f getLinearPosition() {
		std::shared_lock<std::shared_mutex> lock(shared_mtx);
		return linear_pos;
	}

	void update_Position() {
		if (!father)return;
		
		int len = father->son.size() + 5;

		std::vector<sf::Vector2f> from ;
		std::vector<sf::Vector2f> to ;

		for (int i = 0; i < father->son.size(); i++) {
			from.push_back(father->son[i]->getLinkedPosition());
			to.push_back(from[i]);
		}	
			double timer = 0;
			
			
			for (int i = 1; i < father->son.size() ; i++) {


				int tp = i - 1;
				while (tp>=0 && !father->son[tp]->son.size())tp --;

				if (father->son[i]->son.size() && tp>=0)
					to[i].x = to[tp].x+(father->son[tp]->getright()-father->son[tp]->getleft()) / 2 + delta + (father->son[i]->getright() - father->son[i]->getleft()) / 2;
				
				else to[i].x =to[i-1].x + delta;
				
			}

			for (double x = 0; x < 150; x += timer) {
				if (x < 150 / 2)timer++;
				else if (timer > 2)timer--;
				moveDone = 0;
				for (int i = 1; i < father->son.size(); i++) {
	
					//std::lock_guard<std::mutex>lock(mtx);
						
					father->son[i]->move(from[i], to[i], x / 150);
	
				}

				
				moveDone = 1;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				
			}			
			
		

		for (int i = 0; i < father->son.size(); i++) {

			father->son[i]->update_sonPosition();
			father->son[i]->setLinkedPosition(to[i]);

			father->son[i]->left += (to[i].x - from[i].x);
			father->son[i]->right += (to[i].x - from[i].x);

		}



		father->left = father->son[0]->left;
		father->right = father->son[father->son.size()-1]->right;

		father->update_Position();
		Done = 1;
	}

	void update_sonPosition() {
		if (son.size() == 0)return;
		std::cout << index << std::endl;
		sf::Vector2f pos({ 0,0 });
		for (TreeNode* s : son) {
			std::lock_guard<std::recursive_mutex> lock(re_mtx);
			pos += s->getLinkedPosition();		
		}
		pos.y /= son.size();
		pos.x = (son[0]->getleft() + son[son.size() - 1]->getright()) / 2;
		int len = son.size() + 5;
		float del = pos.x - linked_pos.x;

		std::vector<sf::Vector2f> from(len) ;
		std::vector<sf::Vector2f> to(len);

		for (int i = 0;i<son.size();i++) {
			to[i] = son[i]-> getLinkedPosition();
			from[i] =  to[i];
			to[i].x -= del;
		}

		double timer = 0;
		for (double x = 0; x < 100; x += timer) {
			if (x < 100 / 2)timer++;
			else if (timer > 2)timer--;
			moveDone = 0;
			for (int i = 0; i < son.size(); i++) {


				//std::lock_guard<std::recursive_mutex> lock(re_mtx);

				if (del == 0)continue;
				son[i]->move(from[i], to[i], x / 100);


			}
			moveDone = 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));


		}

		for (int i = 0; i < son.size(); i++) {
			son[i]->setLinkedPosition(to[i]);
			son[i]->left -= del;
			son[i]->right -= del;
		}
		
		

		for (TreeNode* s : son) {
			//std::lock_guard<std::recursive_mutex> lock(re_mtx);
			s->update_sonPosition();
		}

		left = son[0]->left;
		right = son[son.size() - 1]->right;
		
		
	}

	float getleft() {
		return left;
	}

	float getright() {
		return right;
	}

	void update_arrow() {
		for (int i = 0; i < son.size(); i++) {
			sf::Vector2f from = linked_pos;
			sf::Vector2f to = son[i]->getLinkedPosition();
			sf::Vector2f temp = to;
			temp.x -= from.x;
			temp.y -= from.y;
			double l = temp.length();
			temp.x /= l;
			temp.y /= l;

			from.x += linked_size + temp.x * 50;
			from.y += linked_size + temp.y * 50;
			to.x += linked_size - temp.x * 50;
			to.y -= temp.y * 25;
			arrow[i]->update(from, to,5);
		}
	}

	void addSon(int num) {
		if (add_ptr != nullptr && add_ptr->joinable()&&addSonDone) {
			add_ptr->join();
			add_ptr.reset();

		}

		if (add_ptr == nullptr) {
			
			add_ptr = std::make_unique<std::thread>(&TreeNode::addSon_thread, this, num);
		}
		

	}

	int getnum() {
		return num;
	}

	void update_linear_pos() {
		sf::Vector2f pos_L = root->getLinearPosition();
		pos_L.x += (index - 1) * (linear_size.x + 10);
		setLinearPosition(pos_L);
		for (TreeNode* s : son) {
			s->update_linear_pos();
		}
	}

	void addSon_thread(int num) {
		addSonDone = 0;
		arrow.push_back(new Arrow());

		sf::Vector2f pos = linked_pos;
		pos.y += 150;
		if (son.size() != 0) {
			pos.x = son[son.size() - 1]->getLinkedPosition().x + delta;
		}

		sf::Vector2f pos_L = linear_pos;
		pos_L.x += (index) * (linear_size.x + 10);
		if (son.size() > 0)pos_L.x += (linear_size.x + 10);

		TreeNode* p = new TreeNode(pos_L,pos);
		

		p->state = this->state;

		p->setNum(num);
		p->index = 2 * index;
		if (son.size() != 0)p->index++;
		son.push_back(p);
		p->father = this;
		p->root = this->root;


		if (son.size() != 1) {
			
			p->front = son[son.size() - 2];
			p->next = p->front->next;
			p->linkedindex = p->front->linkedindex + 1;
			p->front->next = p;
			if(p->next)p->next->front = p;
		}
		else {
			TreeNode* F = front;
			TreeNode* N = next;
			while (N && !N->son.size())N = N->next;
			while (F && !F->son.size())F = F->front;
			if (F&&F->son.size()) {
				TreeNode* temp = F->son[F->son.size()-1];
				if (temp) {
					temp->next = p;
					p->front = temp;
				}
			}
			if (N && N->son.size()) {
				TreeNode* temp = N->son[0];
				if (temp) {
					temp->front = p;
					p->next = temp;
				}
			}
		}

		p->setLinkedPosition(pos);
		p->able = 1;
		update_arrow();
		
		
		update_sonPosition();
		update_Position();
		
		root->update_sonPosition();

		pos = linear_pos;
		pos.x += (p->index) * (linear_size.x + 10);
		p->leftnode = new ListNode<int>(0, pos,linear_size);
		pos.x +=(linear_size.x + 10);
		p->rightnode = new ListNode<int>(0,pos, linear_size);

		p->update_point();
		
		
		addSonDone = 1;
		Done = 1;
		
	}

	void set_chosen() {
		show_linear = &above_linear;
		show_linked = &above_linked;
		is_chosen_linked = 1;
		is_chosen_linear = 1;
	}

	void set_unchosen() {
		show_linear = &normal_linear;
		show_linked = &normal_linked;
		is_chosen_linked = 0;
		is_chosen_linear = 0;
	}

	void update_index(int index) {
		this->index = index;
		if(son.size())son[0]->update_index(index * 2);
		if(son.size()>1)son[1]->update_index(index * 2 + 1);
		update_point();
	}

	void Delete() {
		if (son.size()) {
			for (TreeNode* s : son) {
				s->Delete();
			}
		}
		bool f = 0;
		if (father) {
			for (int i = 0; i < father->son.size() - 1; i++) {
				if (father->son[i] == this) {
					f = 1;
				}
				if (f == 0)continue;
				
				father->son[i + 1]->update_index(father->son[i]->index);
				father->son[i + 1]->update_linear_pos();
				
				father->son[i] = father->son[i + 1];		
				father->arrow[i] = father->arrow[i + 1];
			}
			father->son.pop_back();
			father->arrow.pop_back();
		}
		for (Arrow* a : arrow) {
			delete a;
		}
		arrow.clear();
		if(father)father->update_sonPosition();

		TreeNode* F = front;
		TreeNode* N = next;

		if(F)F->next = N;
		if(N)N->front = F;

		if (this != root) {
			chose = father;
			delete this;
		}
		else {
			Init();
			LinkedText.setString("");
			LinearText.setString("");
		}
	}

	void Clear() {
		if (son.size()) {
			for (TreeNode* s : son) {
				s->Clear();
			}
		}
		bool f = 0;
		
		for (TreeNode* s : son) {
			delete s;
		}
		for (Arrow* a : arrow) {
			delete a;
		}
		arrow.clear();
		son.clear();

		Init();
		LinkedText.setString("");
		LinearText.setString("");
		
	}


	void changeState() {
		if (state == STATE::LINEAR) { 
			state = STATE::LINKED;
		}
		else { state = STATE::LINEAR; }

		for (TreeNode* s : son) {
			s->changeState();
		}
	}

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		
		if (state == STATE::LINEAR) {
			
			//if(!son.size())target.draw(*leftnode, states);
			//if(son.size()<2)target.draw(*rightnode, states);
			target.draw(*show_linear, states);
			target.draw(LinearText, states);
			
			if (is_chosen_linear) {
				if(son.size())target.draw(leftpoint, states);
				if(son.size()>1)target.draw(rightpoint, states);
			}
			
		}
		else if (state == STATE::LINKED) {
			target.draw(*show_linked, states);
			target.draw(LinkedText, states);

			for (Arrow* a : arrow) {
				target.draw(*a, states);
			}
		}

		for (int i = 0; i < son.size();i++) {
			target.draw(*son[i], states);
		}

	}


};





