// box_input.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <functional>
#include <memory>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "custom_message/msg/msg.hpp"

// using namespace std;
using namespace std::chrono_literals;

bool cmp(const std::string lhs, const std::string rhs) {
  return lhs.length() <= rhs.length();
}

std::string* InputBoxInfo(int numberbox)
{    
    std::string* box_array = new std::string[numberbox];
    std::cout << std::endl << " *** Fill_box_info : ";
    int count = 0;

    for (int i = 0; i < numberbox; i++)
    {
        std::cin >> box_array[i];
    }

    for (int i = 0; i < numberbox; i++)
    {
        if(box_array[i].length() == 3)
        {
          count++;
        }
    }
    
    std::cout << std::endl << " *** Input_Box info : ";
    
    for (int i = 0; i < numberbox; i++)
    {
        std::cout << box_array[i] << " ";
    }
    
    if(count == 0)
    {
        std::sort(box_array, box_array + numberbox);
    }
    else
    {
        std::sort(box_array, box_array + numberbox, cmp);
        std::sort(box_array, box_array + (numberbox-count));
        std::sort(box_array+(numberbox-count), box_array + numberbox);
    }
    
    std::cout << std::endl << std::endl << " *** Arranged order : ";

    for (int i = 0; i < numberbox; i++)
    {
        std::cout << box_array[i] << " ";
    }

    std::cout << std::endl << std::endl ;

    return box_array;
}

class ArrayBoxPublisher : public rclcpp::Node
{
  public:
    rclcpp::Publisher<custom_message::msg::Msg>::SharedPtr array_pub;
    ArrayBoxPublisher() : Node("array_box_publisher")
    {
      auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
      array_pub = this->create_publisher<custom_message::msg::Msg>("box_info", qos_profile);
      timer_ = this->create_wall_timer(500ms, std::bind(&ArrayBoxPublisher::timer_callback, this));
    }
  

     void publish_order(std::string order[],int n)
    {
      auto message = custom_message::msg::Msg();
      
      for(int i=0; i<n; i++)
      {
        message.data[i] = order[i];
        // std::cout<< message.data[i]<<std::endl;
      }

      message.num = n;

      std::cout<< "Publishing Data : ";

      for(int i=0; i<10; i++)
      {
        std::cout<< message.data[i] <<" ";
      }

      std::cout << std::endl;
      array_pub->publish(message);
      

    }

  private:
     void timer_callback()
    {
      
      auto message = std_msgs::msg::String();
      message.data = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      // publisher_->publish(message);
    }
    
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};


int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto pub_order=std::make_shared<ArrayBoxPublisher>();
    int numberbox = 0;
    std::cout << std::endl << " *** Box_input number : ";
    std::cin >> numberbox;
    // std::string* array = new std::string[numberbox];
    std::string *array;
    array = InputBoxInfo(numberbox);
    rclcpp::WallRate loop_rate(1); 

    
    while(rclcpp::ok())
    {
      
      loop_rate.sleep();
      // rclcpp::spin(std::make_shared<ArrayBoxPublisher>());
      // pub_order->timer_;
      pub_order->publish_order(array,numberbox);
    }
    // rclcpp::spin(std::make_shared<ArrayBoxPublisher>());
    rclcpp::shutdown();
    return 0;
}



