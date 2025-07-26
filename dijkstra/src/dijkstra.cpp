#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "custom_message/msg/msg.hpp"

using namespace std::chrono_literals;

// Dijkstra's Algorithm in C++

// #include <iostream>
#include <vector>
#include <stack>
#include <queue>

#define INT_MAX 10000000

using namespace std;

void DijkstrasTest();
void ElevatorFind(queue<string> tempqueue);
void DijkstrasTest2(string s,char *d);
void ElePush(queue<string> pubqueue,string *flr);

stack<string> stacks;
queue<string> tempqueue;
queue<string> pubqueue;
queue<string> dijqueue;
queue<string> rvizqueue;
queue<string> elequeue;

void error(const char* message)
{
    printf("Error: %s \n", message);
    exit(1);
}
const int MAX_STACK_SIZE = 20;
class Stack
{
private:
    int top;
    int data[MAX_STACK_SIZE];
public:
    Stack()
    {
        top = 1;
    }
    void push(int e)
    {
        if (isFull())
            error("Full");
        data[++top] = e;
    }
    int pop()
    {
        if (isEmpty())
        {
            error("Empty");
        }
        return data[top--];
    }
    void display() {
        printf("[Size = %d] ", top + 1);
        for (int i = 0; i <= top; ++i)
        {
            printf("%3d", data[i]);
        }
        printf("\n");
    }
    int peek() {
        if (isEmpty())
        {
            error("Empty");
        }
        return data[top];
    }
    int size() { return top; }
    bool isEmpty() { return top == 1; }
    bool isFull() { return top == MAX_STACK_SIZE - 1; }
};


class ArrayBoxSubscriber : public rclcpp::Node
{
  public:
    rclcpp::Publisher<custom_message::msg::Msg>::SharedPtr path_pub;
    ArrayBoxSubscriber() : Node("array_box_ele_publisher")
    {
      auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    //   array_ele_pub = this->create_publisher<custom_message::msg::Msg>("box_ele_info", qos_profile);
      path_pub = this->create_publisher<custom_message::msg::Msg>("path_info", qos_profile);
      array_sub = this->create_subscription<custom_message::msg::Msg>("box_info", qos_profile, std::bind(&ArrayBoxSubscriber::topic_callback, this, std::placeholders::_1));
    //   timer_ = this->create_wall_timer(500ms, std::bind(&ArrayBoxSubscriber::timer_callback, this));
      
    }

    void topic_callback(const custom_message::msg::Msg message)
    {

    //   RCLCPP_INFO(this->get_logger(),"Get data : ");
      auto submessage = custom_message::msg::Msg();
      submessage.data = message.data;
      submessage.num=message.num;

      std::cout << std::endl;
      std::cout << " ************************* hello_world *************************"<<std::endl<<std::endl;

      std::cout << " recieved data : " ;
      for(int i=0; i<10; i++)
      {
        std::cout << submessage.data[i] <<" ";
      }

       for(int i=0; i<submessage.num; i++)
      {
        tempqueue.push(submessage.data[i]);
      }
    
      ElevatorFind(tempqueue);
      
      // std::cout << std::endl<<std::endl;
      // std::cout << " temporary data : ";
      
       while (!tempqueue.empty())
      {
        // cout << tempqueue.front() << " ";
        tempqueue.pop();
      }

      dijqueue=pubqueue;
      dijqueue.push("ele");
      char *d=new char('a');
      string *crtflr = new string("1");

      std::cout << std::endl<<std::endl;
      std::cout << " pub data : ";
      
      while (!pubqueue.empty())
      {
        cout << pubqueue.front() << " ";
        ElePush(pubqueue, crtflr);
        pubqueue.pop();
      }

      elequeue.push("1");

      // std::cout << std::endl<<std::endl;
      // std::cout << " ele data : ";

      //  while (!elequeue.empty())
      // {
      //   cout << elequeue.front() << " ";
      //   elequeue.pop();
      // }

      *crtflr ="1";
      string a= "1a";

       while(!dijqueue.empty())
       {
        string s=dijqueue.front();
        DijkstrasTest2(s,d);
        if(s=="ele")
        {
        tempqueue.push(elequeue.front());
        elequeue.pop();
        }
        dijqueue.pop();
       }

      DijkstrasTest2(a,d);
      

      std::cout << std::endl<<std::endl;
      std::cout << " rviz data : ";

      int n= tempqueue.size();
      string last_floor = submessage.data[message.num-1];
      if(last_floor.length()==2)
      {
        last_floor=last_floor[0];
      }else if(last_floor.length()==3)
      {
        last_floor=last_floor.substr(0,2);
      }

      publish_path_info(tempqueue,n,last_floor);

      while (!tempqueue.empty())
      {
        cout << tempqueue.front() << " ";
        tempqueue.pop();
      }
      // std::cout<<trash.back();

      // while(!trash.empty())
      // {
      //   trash.pop();
      // }
      std::cout << std::endl << std::endl;
      std::cout << " ***************************************************************"<<std::endl;
      

      

      
    }

    void publish_path_info(queue<string> finalqueue,int n, string last_floor) {
      
      auto pubmessage = custom_message::msg::Msg();
      int i=0;
      while(!finalqueue.empty())
      {
        pubmessage.rviz[i]=finalqueue.front();
        finalqueue.pop();
        i++;
      }
      i=0;
      pubmessage.num=n;
      pubmessage.last=last_floor;
      RCLCPP_INFO(this->get_logger()," Path Publish!!");
      path_pub->publish(pubmessage);

    }

    rclcpp::Subscription<custom_message::msg::Msg>::SharedPtr array_sub;
    
  private:
  
    //  void timer_callback()
    // {
      
    //   auto message = std_msgs::msg::String();
    //   message.data = "Hello, world! " + std::to_string(count_++);
    //   RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    //   // publisher_->publish(message);
    // }
    
    // rclcpp::TimerBase::SharedPtr timer_;
    // rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    // size_t count_;
};


int main(int argc, char * argv[])
 {
    rclcpp::init(argc, argv);
    // auto sub_order=std::make_shared<ArrayBoxSubscriber>();
    auto message = custom_message::msg::Msg();

    // DijkstrasTest();
    // while (!stacks.empty()) {
    //     cout << stacks.top() << " ";
    //     stacks.pop();
    // }
    // cout << endl;

    // rclcpp::WallRate loop_rate(1); 
    // while(rclcpp::ok())
    // {
      
    // //   loop_rate.sleep();
    //   // rclcpp::spin(std::make_shared<ArrayBoxPublisher>());
    //   // pub_order->timer_;
    //   sub_order->topic_callback(message);
    // }

    rclcpp::spin(std::make_shared<ArrayBoxSubscriber>());
    
    
    rclcpp::shutdown();
    return 0;

}

class Node;
class Edge;

void Dijkstras();
vector<Node*>* AdjacentRemainingNodes(Node* node);
Node* ExtractSmallest(vector<Node*>& nodes);
int Distance(Node* node1, Node* node2);
bool Contains(vector<Node*>& nodes, Node* node);
void PrintShortestRouteTo(Node* destination);
void DelQueue();

vector<Node*> nodes;
vector<Edge*> edges;
Stack testStack;

class Node {
public:
    Node(string id,char ele)
        : id(id), previous(NULL), distanceFromStart(INT_MAX) {
        nodes.push_back(this);
    }

public:
    string id;
    char ele;
    Node* previous;
    int distanceFromStart;
};

class Edge {
public:
    Edge(Node* node1, Node* node2, int distance)
        : node1(node1), node2(node2), distance(distance) {
        edges.push_back(this);
    }
    bool Connects(Node* node1, Node* node2) {
        return (
            (node1 == this->node1 &&
                node2 == this->node2) ||
            (node1 == this->node2 &&
                node2 == this->node1));
    }

public:
    Node* node1;
    Node* node2;
    int distance;
};

///////////////////
// void DijkstrasTest() {
  
//     // floor : 1
//     Node* a1 = new Node("a1");
//     Node* b1 = new Node("b1");
//     Node* c1 = new Node("c1");
//     Node* d1 = new Node("d1");
//     Node* e1 = new Node("e1");
//     Node* f1 = new Node("f1");
//     Node* g1 = new Node("g1");
//     Node* h1 = new Node("h1");
//     Node* i1 = new Node("i1");
//     Node* j1 = new Node("j1");
//     Node* k1 = new Node("k1");

//     Edge* line1 = new Edge(a1, b1, 3);
//     Edge* line2 = new Edge(b1, c1, 2);
//     Edge* line3 = new Edge(c1, d1, 2);
//     Edge* line4 = new Edge(d1, e1, 2);
//     Edge* line5 = new Edge(e1, f1, 2);
//     Edge* line6 = new Edge(f1, g1, 2);
//     Edge* line7 = new Edge(g1, h1, 2);
//     Edge* line8 = new Edge(h1, i1, 2);
//     Edge* line9 = new Edge(i1, j1, 2);
//     Edge* line10 = new Edge(j1, k1, 2);
//     Edge* line11 = new Edge(k1, a1, 5);

//     // floor : 2
//     Node* a2 = new Node("a2");
//     Node* b2 = new Node("b2");
//     Node* c2 = new Node("c2");
//     Node* d2 = new Node("d2");
//     Node* e2 = new Node("e2");
//     Node* f2 = new Node("f2");
//     Node* g2 = new Node("g2");
//     Node* h2 = new Node("h2");
//     Node* i2 = new Node("i2");
//     Node* j2 = new Node("j2");

//     Edge* line12 = new Edge(a2, b2, 2);
//     Edge* line13 = new Edge(b2, c2, 2);
//     Edge* line14 = new Edge(c2, d2, 2);
//     Edge* line15 = new Edge(d2, e2, 2);
//     Edge* line16 = new Edge(e2, f2, 2);
//     Edge* line17 = new Edge(f2, g2, 2);
//     Edge* line18 = new Edge(g2, h2, 2);
//     Edge* line19 = new Edge(h2, i2, 2);
//     Edge* line20 = new Edge(i2, j2, 2);
//     Edge* line21 = new Edge(j2, a2, 8);
   
    
   

//     d1->distanceFromStart = 0;  // set start node
//     Dijkstras();
//     PrintShortestRouteTo(j1);
  
// }

void DijkstrasTest2(string s,char *d) {
    
    char order;
    char crtloc;
    

    // floor : 1
    Node* a1 = new Node("a1",'x');
    Node* b1 = new Node("b1",'x');
    Node* c1 = new Node("c1",'x');
    Node* d1 = new Node("d1",'x');
    Node* e1 = new Node("e1",'x');
    Node* f1 = new Node("f1",'x');
    Node* g1 = new Node("g1",'x');
    Node* h1 = new Node("h1",'y');
    Node* i1 = new Node("i1",'y');
    Node* j1 = new Node("j1",'y');
    Node* k1 = new Node("k1",'y');
    Node* x1 = new Node("x1",'x');
    Node* y1 = new Node("y1",'y');

    Edge* line1 = new Edge(a1, b1, 3);
    Edge* line2 = new Edge(b1, c1, 2);
    Edge* line3 = new Edge(c1, d1, 2);
    Edge* line4 = new Edge(d1, x1, 2);
    Edge* line5 = new Edge(x1, e1, 2);
    Edge* line6 = new Edge(e1, f1, 2);
    Edge* line7 = new Edge(f1, g1, 2);
    Edge* line8 = new Edge(g1, h1, 2);
    Edge* line9 = new Edge(h1, y1, 2);
    Edge* line10 = new Edge(y1, i1, 2);
    Edge* line11 = new Edge(i1, j1, 2);
    Edge* line12 = new Edge(j1, k1, 2);
    Edge* line13 = new Edge(k1, a1, 5);

    // while (!tempqueue.empty()) {
      crtloc=*d;
      if(s.length()==2)
      {
      order=s[1];
      // cout << "order:"<<order<<" crtloc:"<<crtloc<<endl;
      }
      else if(s.length()==3)
      {
        if(s=="ele")
        {
          order='z';         
        }
        else
        {
          order=s[2];
        }
      }
        // crtflr=temp[0];
        // cout<<"temp : " << temp <<", order :"<<order<<", crtflr : "<<crtflr<<endl;

        if(s.length()==2) {

            switch (order) 
            {
              case 'a':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
              }

              break;

              case 'b':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
              }
            
              break;

              case 'h':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
              }
              
              break;

              case 'c':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
               }
            
              break;

              case 'd':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
              }
              
              break; 

              case 'e':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
              }
              
              break; 

              case 'f':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
              }
              
              break;

              case 'g':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
              }
              
              break;

              case 'i':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
              }
              
              break;

              case 'j':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
              }
              
              break;

              case 'k':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
              }
              
              break;

              case 'x':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
              }
              
              break;

              case 'y':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
              }
              
              break;
            
            }

        } else if( s.length()==3) {

          if(s=="ele")
          {
              
          //  cout <<"hellp_world";
           switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    // crtloc=a1->ele;
                    *d='x';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    // crtloc=b1->ele;
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    // crtloc=c1->ele;
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    // crtloc=d1->ele;
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    // crtloc=e1->ele;
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    // crtloc=f1->ele;
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    // crtloc=g1->ele;
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    // crtloc=h1->ele;
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    // crtloc=i1->ele;
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    // crtloc=j1->ele;
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    // crtloc=k1->ele;
                    break;
              }
        }
        else
        {
          switch (order) 
            {
              case 'a':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(a1);
                    *d='a';
                    break;
              }

              break;

              case 'b':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(b1);
                    *d='b';
                    break;
              }
            
              break;

              case 'h':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(h1);
                    *d='h';
                    break;
              }
              
              break;

              case 'c':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(c1);
                    *d='c';
                    break;
               }
            
              break;

              case 'd':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(d1);
                    *d='d';
                    break;
              }
              
              break; 

              case 'e':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(e1);
                    *d='e';
                    break;
              }
              
              break; 

              case 'f':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(f1);
                    *d='f';
                    break;
              }
              
              break;

              case 'g':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(g1);
                    *d='g';
                    break;
              }
              
              break;

              case 'i':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(i1);
                    *d='i';
                    break;
              }
              
              break;

              case 'j':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(j1);
                    *d='j';
                    break;
              }
              
              break;

              case 'k':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(k1);
                    *d='k';
                    break;
              }
              
              break;

              case 'x':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(x1);
                    *d='x';
                    break;
              }
              
              break;

              case 'y':
                
              switch(crtloc) 
              {
                  case 'a':
                    a1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'b':
                    b1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'c':
                    c1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'd':
                    d1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'e':
                    e1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'f':
                    f1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'g':
                    g1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'h':
                    h1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'i':
                    i1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'j':
                    j1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'k':
                    k1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'x':
                    x1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
                  case 'y':
                    y1->distanceFromStart = 0;
                    Dijkstras();
                    PrintShortestRouteTo(y1);
                    *d='y';
                    break;
              }
              
              break;
            
            }

        }
    }


    // a1->distanceFromStart = 0;  // set start node
    // Dijkstras();
    // PrintShortestRouteTo(h1);
}

///////////////////

void Dijkstras() {
    while (nodes.size() > 0) {
        Node* smallest = ExtractSmallest(nodes);
        vector<Node*>* adjacentNodes =
            AdjacentRemainingNodes(smallest);

        const int size = adjacentNodes->size();
        for (int i = 0; i < size; ++i) {
            Node* adjacent = adjacentNodes->at(i);
            int distance = Distance(smallest, adjacent) +
                smallest->distanceFromStart;

            if (distance < adjacent->distanceFromStart) {
                adjacent->distanceFromStart = distance;
                adjacent->previous = smallest;
            }
        }
        delete adjacentNodes;
    }
}

// Find the node with the smallest distance,
// remove it, and return it.
Node* ExtractSmallest(vector<Node*>& nodes) {
    int size = nodes.size();
    if (size == 0) return NULL;
    int smallestPosition = 0;
    Node* smallest = nodes.at(0);
    for (int i = 1; i < size; ++i) {
        Node* current = nodes.at(i);
        if (current->distanceFromStart <
            smallest->distanceFromStart) {
            smallest = current;
            smallestPosition = i;
        }
    }
    nodes.erase(nodes.begin() + smallestPosition);
    return smallest;
}

// Return all nodes adjacent to 'node' which are still
// in the 'nodes' collection.
vector<Node*>* AdjacentRemainingNodes(Node* node) {
    vector<Node*>* adjacentNodes = new vector<Node*>();
    const int size = edges.size();
    for (int i = 0; i < size; ++i) {
        Edge* edge = edges.at(i);
        Node* adjacent = NULL;
        if (edge->node1 == node) {
            adjacent = edge->node2;
        }
        else if (edge->node2 == node) {
            adjacent = edge->node1;
        }
        if (adjacent && Contains(nodes, adjacent)) {
            adjacentNodes->push_back(adjacent);
        }
    }
    return adjacentNodes;
}

// Return distance between two connected nodes
int Distance(Node* node1, Node* node2) {
    const int size = edges.size();
    for (int i = 0; i < size; ++i) {
        Edge* edge = edges.at(i);
        if (edge->Connects(node1, node2)) {
            return edge->distance;
        }
    }
    return -1;  // should never happen
}

// Does the 'nodes' vector contain 'node'
bool Contains(vector<Node*>& nodes, Node* node) {
    const int size = nodes.size();
    for (int i = 0; i < size; ++i) {
        if (node == nodes.at(i)) {
            return true;
        }
    }
    return false;
}

///////////////////

void PrintShortestRouteTo(Node* destination) {

    Node* previous = destination;
    // cout << "Distance from start: " << destination->distanceFromStart << endl;
    while (previous) {
       
        // cout << previous->id << " ";
        rvizqueue.push(previous->id);
        // stacks.push(previous->id);
        previous = previous->previous;
        
    } 
    DelQueue();
}

// these two not needed
vector<Edge*>* AdjacentEdges(vector<Edge*>& Edges, Node* node);
void RemoveEdge(vector<Edge*>& Edges, Edge* edge);

vector<Edge*>* AdjacentEdges(vector<Edge*>& edges, Node* node) {
    vector<Edge*>* adjacentEdges = new vector<Edge*>();

    const int size = edges.size();
    for (int i = 0; i < size; ++i) {
        Edge* edge = edges.at(i);
        if (edge->node1 == node) {
            cout << "adjacent: " << edge->node2->id << endl;
            adjacentEdges->push_back(edge);
        }
        else if (edge->node2 == node) {
            cout << "adjacent: " << edge->node1->id << endl;
            adjacentEdges->push_back(edge);
        }
    }
    return adjacentEdges;
}

void RemoveEdge(vector<Edge*>& edges, Edge* edge) {
    vector<Edge*>::iterator it;
    for (it = edges.begin(); it < edges.end(); ++it) {
        if (*it == edge) {
            edges.erase(it);
            return;
        }
    }
}

// elevator start
void ElevatorFind(queue<string> tempqueue) {

    string tempstring;
    string tempfloor;
    
    char crtflr ='1';
    
    while (!tempqueue.empty())
      {
        tempstring = tempqueue.front();
         if(tempstring.length()==2)
        {
            if(tempstring.front()==crtflr)
            {
                pubqueue.push(tempstring);
            }
            else if(tempstring.front()!=crtflr)
            {
                pubqueue.push("ele");
                pubqueue.push(tempstring);
                crtflr=tempstring.front();
            }
        }
         else if(tempstring.length()==3)
        {
            
            string newfloor;

            if(tempfloor.empty())
            {
                pubqueue.push("ele");
                tempfloor =  tempstring.substr(0,2);
                pubqueue.push(tempstring);
            }
            else if (!tempfloor.empty())
            {
                newfloor =  tempstring.substr(0,2);
                // cout<<tempfloor<<endl;
                // cout<<newfloor<<endl;
                if (tempfloor==newfloor) {
                    pubqueue.push(tempstring);
                }
                else {
                    pubqueue.push("ele");
                    pubqueue.push(tempstring);
                    tempfloor=newfloor;
                }
            }
        }
         tempqueue.pop();
      }
    // cout << endl;
    
}

void DelQueue() {

  // trash.push(rvizqueue.back());

  while(!rvizqueue.empty())
  {
    stacks.push(rvizqueue.front());
    rvizqueue.pop();
  }


  while(!stacks.empty())
  {
    tempqueue.push(stacks.top());
    stacks.pop();
  }


}

void ElePush(queue<string> pubqueue,string *flr) {
    
    string e=pubqueue.front();
    if(e.length()==2)
    {
      string newflr(1,e[0]);
      if(e!="ele" && newflr != *flr)
      { 
        elequeue.push(newflr);
        *flr = newflr;
      } 
    }else if(e.length()==3)
    {
      string newflr=e.substr(0,2);
      if(e!="ele" && newflr != *flr)
      { 
        elequeue.push(newflr);
        *flr = newflr;
      } 
    }
     
}