
#include<iostream>
#include<SFML/Graphics.hpp>
//#include"Button.cpp"
//#include"input_box.cpp"
//#include"ListNode.cpp"
//#include"List.cpp"
//#include"Arrow.cpp"
//#include"Scrollbox.cpp"
//#include"Linked.cpp"
//#include"ShowStack.cpp"
#include"mylib.h"


using namespace std;
void solve() {
    sf::ContextSettings setting;
    setting.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({ 1280, 640 }), "Data_Structrue",sf::Style::Close, sf::State::Windowed, setting);
    window.setFramerateLimit(60);

    //Linked link;
    /*ListNode<char> list('(', {100,100}, {100,100}, 24);
    list.set_char('(');*/
    /*TreeNode node;
    Button btn({ 50,50 });
    Button changebtn({ 50,150 });
    node.setNum(1);
    TreeNode* chose=nullptr;
    int ischose = 0;
    int x=1;*/

   /* MainStr mainStr("ababc");
    PartternStr partternStr("abc");*/

   // ShowKMP showkmp;

    //node.move_setLinkedPosition({ 100,100 },200);

    Oxy oxy({100,450});
    Oxy o2({ 800,450 });
    Button btn({ 400,500 });
    Button re({ 600,500 });
    btn.set_str(U"SVD·Ö½â");
    re.set_str(U"RESET");
    
   
    /*LinkNode node({ 200,200 }, 200);
    LinkNode node2({ 500,200 }, 100);
    LinkNode node3({ 500,300 }, 500);
    node.next = &node2;
    sf::Vector2f pos = { 200,200 };*/

    /*int x[16] = { 200,200,200,100,0,-100,-200,-200,-200,-200,-200,-100,0,100,200,200 };
    int y[16] = { 0,100,200,200,200,200,200,100,0,-100,-200,-200,-200,-200,-200,-100 };

    Arrow* a[16];
    for (int i = 0; i < 16; i++) {
        float X = 300 + x[i];
        float Y = 300 + y[i];
        a[i] = new Arrow({ 300,300 }, { X,Y }, 5);
    }*/


   // GLNode_1 gln_(GLNode_1::Tag::LIST, { 200,100 });
    //gln_.build_list("((()),((e)),(a,(b,c,d)))");




    sf::Clock clock;

    while (window.isOpen())
    {   
        auto time=clock.getElapsedTime();
       
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

           // list.update(window, event);
            //node.update(window, event);
            //btn.update(window, event);
            //changebtn.update(window, event);
            //if (btn.click() && ischose) {
            //   // std::cout << chose->getnum() << std::endl;
            //    chose->addSon(++x);
            //   
            //}

            //if (changebtn.click()) {
            //    node.changeState();
            //}


            //if (node.click()) {
            //    chose = node.getchose();             
            //    ischose = 1;
            //}

           /* node.update(window, event);
            node2.update(window, event);
            node3.update(window, event);

            
            
            pos.x += 1;
            pos.y += 1;
             node.setPosition(pos);*/
            oxy.update(window, event);
            o2.update(window, event);
            btn.update(window, event);
            re.update(window, event);
            
            if (btn.click()) {
               
              
                Eigen::Matrix2f A = oxy.getMatrix();
                Eigen::JacobiSVD<Eigen::Matrix2f> svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV);
                Eigen::Matrix2f U = svd.matrixU();
                Eigen::Matrix2f V = svd.matrixV();
                Eigen::MatrixXf S = Eigen::Matrix2f::Zero();
                std::cout << A<<std::endl;
                S(0, 0) = svd.singularValues()(0);
                S(1, 1) = svd.singularValues()(1);
               
                o2.reset();

                o2.move(U,S,V.transpose());
                
               

               


            }

            if (re.click()) {
                oxy.reset();
                o2.reset();
            }
           /* mainStr.update(window, event);
            partternStr.update(window, event);*/

           // showkmp.update(window, event);
            //gln.update(window, event);
            //gln_.update(window, event);

           
        }

       /* node.update_shine(time);*/
        window.clear(); 
       // window.draw(gln);
        window.draw(oxy);
        window.draw(o2);
        window.draw(btn);
        window.draw(re);
        //window.draw(gln_);
        //window.draw(showkmp);
       /* window.draw(mainStr);
        window.draw(partternStr);*/
       /* window.draw(node);
        window.draw(btn);
        window.draw(changebtn);*/
        //window.draw(list);
       
       /* window.draw(node);
        window.draw(node2);
        window.draw(node3);*/

        /*for (auto ar : a) {
            window.draw(*ar);
        }*/
        window.display();
    }
}

int main() {
    solve();
    return 0;
}