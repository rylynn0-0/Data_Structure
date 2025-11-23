#pragma once
#include"TreeNode.h"
#include"Button.h"
#include"Show.h"
class BinaryTree :public Show {
	TreeNode* head;
	TreeNode* chose;

	

	Button btn;
	bool is_chose=0;
	bool is_root_set = 0;
	sf::RectangleShape r;
	
public:

	BinaryTree():Show(3),btn({940,450}) {
		head = new TreeNode();
		btn.set_str(U"STATE");
		chose = head;
		head->set_chosen();
		is_chose = 1;
		set_insert_name(U"SET");
		r.setSize({ 100,50 });
		r.setFillColor(sf::Color::White);
		r.setOutlineThickness(5);
		r.setOutlineColor(sf::Color::Blue);
		r.setPosition({200,200});
	}

	void update(const sf::RenderWindow& window, std::optional<sf::Event> event) {
		if (!isDone)return;
		std::lock_guard<std::mutex>lock(mtx);
		

		if (head->click()) {
			if (chose)chose->set_unchosen();
			chose = head->getchose();
			chose->set_chosen();
			is_chose = 1;
		}

		if (auto x= event->getIf<sf::Event::KeyPressed>()) {
			if (chose->father && (int)x->scancode == 89) {
				chose->set_unchosen();
				chose = chose->father;
				chose->set_chosen();
			}
			if (chose->son.size() && (int)x->scancode == 88) {
				chose->set_unchosen();
				chose = chose->son[0];
				chose->set_chosen();
			}
			if ((int)x->scancode == 87) {
				if (chose->front) {
					chose->set_unchosen();
					chose = chose->front;
					chose->set_chosen();
				}
				else if (chose->father) {
					TreeNode* tp = chose->father;
					while (tp->next)tp = tp->next;
					chose->set_unchosen();
					chose = tp;
					chose->set_chosen();
				}
			}
			if (  (int)x->scancode == 86) {
				if (chose->next) {
					chose->set_unchosen();
					chose = chose->next;
					chose->set_chosen();
				}
				else if (chose) {
					TreeNode* tp = chose;
					while (tp->front)tp = tp->front;
					while (tp&&!tp->son.size())tp = tp->next;
					if (tp&&tp->son.size()) {
						chose->set_unchosen();
						chose = tp->son[0];
						chose->set_chosen();
					}

				}
			}
		}

		Show::update(window, event);

		if (!is_catch) {
			btn.update(window, event);

			if (btn.click()) {
				head->changeState();
			}
		}

		if (!is_catch)
			head->update(window, event);
	}

	void Insert(int num) {
		isReady = 0;
		//std::lock_guard<std::mutex>lock(mtx);
		if (is_chose && chose == head && !is_root_set) {
			head->setNum(num);
			is_root_set = 1; 
			set_insert_name(U"ADDSON");

		}
		else if (is_chose&&chose->son.size()<2) {
			std::lock_guard<std::mutex>lock(mtx);
			chose->addSon_thread(num);
		}

		
		inputbox.reset();
		inputbox.got_put();
		inputbox.set_normal();
		sf::Vector2f s = { 100,50 };
		s.x = head->getMaxIndex() * (220) - 10;
		r.setSize(s);

		isDone = 1;
		isReady = 1;
	}

	void update_position(sf::Vector2i delta) {
		head->update_position(delta);
		sf::Vector2f pos = r.getPosition();
		pos.x += delta.x;
		pos.y += delta.y;
		if(head->getState()==TreeNode::STATE::LINEAR)r.setPosition(pos);

	}

	void update_shine(sf::Time time)const {
		head->update_shine(time);
	}

	void Delete() {
		isDone = 0;
		TreeNode* tp = chose;
		chose->set_unchosen();
		if (chose != head)chose = chose->father;
		else { is_root_set = 0; 
		set_insert_name(U"SET");
		}
		chose->set_chosen();
		tp->Delete();

		sf::Vector2f s = { 100,50 };
		s.x = head->getMaxIndex() * (220) - 10;
		r.setSize(s);

		isDone = 1;
	}

	void Clear() {
		chose->set_unchosen();
		is_root_set = 0;
		set_insert_name(U"SET");
		chose = head;
		head->Clear();
		head->setLinkedPosition({ 600,100 });
		head->setLinearPosition({ 200,200 });
		r.setPosition({ 200,200 });
		r.setSize({ 100,50 });
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		if(head->getState()==TreeNode::STATE::LINEAR)target.draw(r, states);
		target.draw(*head, states);
		target.draw(btn, states);
		Show::draw(target,states);
	}
};