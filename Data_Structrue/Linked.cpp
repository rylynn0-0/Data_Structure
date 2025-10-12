#pragma once
#include"show.cpp"
#include"LinkNode.cpp"

class Linked:public Show {
	LinkNode* head;
	LinkNode* p;
	Arrow *arrow;

public:
	Linked():Show(3) { Init(); }
	void Init() {
		head = new LinkNode({200,150},1);
		LinkNode* temp = new LinkNode({ 350,150 }, -1);
		head->next = temp;
		temp->father = head;
		p = head;
		p->set_chose();
		update_arrow(p);
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		Show::update(window, event);
		

		LinkNode* q = head;
		while (q) {

			q->update(window, event);
			if (!isDone);
			else if (q->click()) {
				p->set_unchose();
				p = q;
				p->set_chose();
			}
			if (!q->is_ending() && !q->next) {
				sf::Vector2f position = q->getPosition();
				position.x += 150;
				LinkNode* temp = new LinkNode(position, -1);
				temp->father = q;
				q->next = temp;
			}
			

			q = q->next;
		}
		
		if (auto x = event->getIf<sf::Event::KeyPressed>()) {
			if (!isDone);
			else if ((int)x->scancode == 87) {
				if (p->father) { 
					p->set_unchose();
					p = p->father;
					p->set_chose();
				}
			}
			else if ((int)x->scancode == 86) {
				if (p->next) {
					p->set_unchose();
					p = p->next;
					p->set_chose();
				}
			}
		}
		
		update_arrow(p);
	}

	void update_shine(sf::Time time) {
		update_arrow(p);
	}

	void update_arrow(LinkNode* p) {
		sf::Vector2f to = p->getPosition();
		to.x += p->getRadius();
		to.y -= p->getRadius() / 2;
		sf::Vector2f from = to;
		from.y -= 50;
		if (!arrow) {
			arrow = new Arrow(from, to, 5);
		}
		else {
			arrow->update(from, to, 5);
		}
	}

	void update_position(sf::Vector2i delta) {
		LinkNode* q = head;
		while (q) {
			sf::Vector2f pos = q->getPosition();
			pos.x += delta.x;
			pos.y += delta.y;
			q->setPosition(pos);
			q = q->next;
		}
		update_arrow(p);
	}

	void Clear(){
		while (head->next) {
			LinkNode* q = head->next;
			head->next = q->next;
			delete q;
		}delete head;
		inputbox.got_put();
		inputbox.reset();
		inputbox.set_normal();
		Init();
	}

	void Insert(int num) {
		isDone = 0;
		if (num == 0);
		else if (!p->father) {
			isReady = 0;
			std::lock_guard<std::mutex>lock(mtx);
			LinkNode* Next = p->next;
			sf::Vector2f position = p->getPosition();
			position.y -= 125;
			position.x += 150;
			LinkNode* q= new LinkNode(position,0, num);
			
			sf::Vector2f f1,f2;
			sf::Vector2f t1,t2;
			p->get_point(f1,t1);
			p->next = q;
			p->get_point(f2,t2);

			q->father = p;
			q->next = Next;
			if (Next)Next->father = q;
			

			sf::Vector2f d1,d2;
			d1.x = (f2.x - f1.x) / 150;
			d1.y = (f2.y - f1.y) / 150;

			d2.x = (t2.x - t1.x) / 150;
			d2.y = (t2.y - t1.y) / 150;
			p->set_arrow_update(0);
			for (int i = 0; i < 150; i++) {
				
				isReady = 0;
				p->set_point(f1,t1);
				f1.x += d1.x;
				f1.y += d1.y;
				t1.x += d2.x;
				t1.y += d2.y;

				Next = q->next;
				while (Next) {
					position = Next->getPosition();
					position.x += 1;
					Next->setPosition(position);
					Next = Next->next;
				}
				q->update_arrow();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				isReady = 1;
			}
			p->set_arrow_update(1);
			
			for (int i = 0; i < 150; i++) {
				isReady = 0;
				position = q->getPosition();
				position.y += 5/6.0;
				q->setPosition(position);
				p->update_arrow();
				
				isReady = 1;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		else {
			isReady = 0;
			std::lock_guard<std::mutex>lock(mtx);
			LinkNode* Father = p->father;
			sf::Vector2f position = Father->getPosition();
			position.x += 150;
			position.y -= 125;
			LinkNode* New = new LinkNode(position, 0, num);

			sf::Vector2f f1, f2;
			sf::Vector2f t1, t2;
			Father->get_point(f1, t1);
			Father->next = New;
			Father->get_point(f2, t2);


			New->father = Father;
			New->next = p;
			LinkNode* Next = p->next;

			sf::Vector2f d1, d2;
			d1.x = (f2.x - f1.x) / 150;
			d1.y = (f2.y - f1.y) / 150;

			d2.x = (t2.x - t1.x) / 150;
			d2.y = (t2.y - t1.y) / 150;

			Father->set_arrow_update(0);
			for (int i = 0; i < 150; i++) {

				isReady = 0;
				Father->set_point(f1, t1);
				f1.x += d1.x;
				f1.y += d1.y;
				t1.x += d2.x;
				t1.y += d2.y;

				Next = New->next;
				while (Next) {
					position = Next->getPosition();
					position.x += 1;
					Next->setPosition(position);
					Next = Next->next;
				}
				New->update_arrow();
				update_arrow(p);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				isReady = 1;
			}
			Father->set_arrow_update(1);

			for (int i = 0; i < 150; i++) {
				isReady = 0;
				position = New->getPosition();
				position.y += 5 / 6.0;
				New->setPosition(position);
				Father->update_arrow();
				update_arrow(p);
				isReady = 1;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			p->father = New;
			p->set_unchose();
			p = New;
			p->set_chose();
			update_arrow(p);
			
			
		}

		inputbox.got_put();
		inputbox.reset();
		inputbox.set_normal();
		isReady = 1;
		isDone = 1;
	}

	void Delete() {
		isDone = 0;
		if (p == head||!p->next);
		else {
			LinkNode* Father = p->father;
			LinkNode* Next = p->next;

			for (int i = 0; i < 150; i++) {
				isReady = 0;
				sf::Vector2f position = p->getPosition();
				position.y -= 5/6.0;
				p->setPosition(position);
				update_arrow(p);
				Father->update_arrow();
				Next = p->next;
				while (Next) {
					position = Next->getPosition();
					position.x -= 1;
					Next->setPosition(position);
					Next = Next->next;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				isReady = 1;
			}
			Next = p->next;

			sf::Vector2f f1, f2;
			sf::Vector2f t1, t2;
			Father->get_point(f1, t1);
			Father->next = Next;
			Father->update_arrow();
			Father->get_point(f2, t2);
			
			sf::Vector2f d1, d2;
			d1.x = (f2.x - f1.x) / 150;
			d1.y = (f2.y - f1.y) / 150;

			d2.x = (t2.x - t1.x) / 150;
			d2.y = (t2.y - t1.y) / 150;
			update_arrow(Next);

			Father->set_arrow_update(0);
			for (int i = 0; i < 150; i++) {

				isReady = 0;
				Father->set_point(f1, t1);
				f1.x += d1.x;
				f1.y += d1.y;
				t1.x += d2.x;
				t1.y += d2.y;

				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				isReady = 1;
			}
			Father->set_arrow_update(1);

			Next->father = Father;
			delete p;
			
			p = Next;
			p->set_chose();
			
			
		}
		isReady = 1;
		isDone = 1;
	}
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		Show::draw(target, states);
		LinkNode* q = head;
		while (q) {
			target.draw(*q, states);
			q = q->next;
		}
		target.draw(*arrow);
	}
};