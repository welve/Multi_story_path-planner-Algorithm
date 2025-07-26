#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker_array.hpp"
#include "custom_message/msg/msg.hpp"
#include "unistd.h"
#include "string.h"
using namespace std::chrono_literals;

struct position
{
  float x;
  float y;
  float z;
};


class ArrayPathSubscriber : public rclcpp::Node
{
  public:
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr rviz_pub;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr edge_pub;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr node_pub;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr line_pub;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr line_pub2;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr pnt_pub;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr pnt_pub2;



    ArrayPathSubscriber() : Node("path_visualiazation")
    {
      auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
      path_sub = this->create_subscription<custom_message::msg::Msg>("path_info", qos_profile,std::bind(&ArrayPathSubscriber::topic_callback, this, std::placeholders::_1));
      rviz_pub = this->create_publisher<visualization_msgs::msg::Marker>("test_marker", rclcpp::QoS(1));
      edge_pub= this->create_publisher<visualization_msgs::msg::MarkerArray>("edge_marker", rclcpp::QoS(1));
      node_pub= this->create_publisher<visualization_msgs::msg::MarkerArray>("node_marker", rclcpp::QoS(1));
      line_pub= this->create_publisher<visualization_msgs::msg::MarkerArray>("path_marker", rclcpp::QoS(1));
      line_pub2= this->create_publisher<visualization_msgs::msg::MarkerArray>("path_marker2", rclcpp::QoS(1));
      pnt_pub= this->create_publisher<visualization_msgs::msg::MarkerArray>("pnt_marker", rclcpp::QoS(1));
      pnt_pub2= this->create_publisher<visualization_msgs::msg::MarkerArray>("pnt_marker2", rclcpp::QoS(1));




    //   timer_ = this->create_wall_timer(500ms, std::bind(&ArrayBoxSubscriber::timer_callback, this));

    }

    
    void topic_callback(const custom_message::msg::Msg message)
    {
        std::cout << " pub -> rviz2 " << std::endl;

        auto submessage = custom_message::msg::Msg();
        submessage.rviz = message.rviz;
        submessage.num=message.num;
        submessage.last=message.last;
        int j=0;
        for(int i=0;i<submessage.num-1;i++){
          if(submessage.rviz[i]==submessage.last)
          {
            break;
          }
          j++;  
        }

        std::cout<<submessage.rviz[j]<<" "<<submessage.last<< " " << j << std::endl;

        for(int d=j;d<submessage.num-1;d++){
          
          std::string temp;
          temp=submessage.rviz[d];

          if(temp==submessage.rviz[d+1])
          {
            break;
          }
          j++;  
        }

        std::cout<<submessage.rviz[j]<<" "<<submessage.last<< " " << j << std::endl;

        // std::cout << " recieved data : " ;
        // for(int i=0; i<submessage.num; i++)
        // {
        // std::cout << submessage.rviz[i] <<" ";
        // }
        // std::cout << std::endl<<std::endl;

        visualization_msgs::msg::Marker marker;
        visualization_msgs::msg::MarkerArray marker_arr;

        visualization_msgs::msg::Marker marker4;
        visualization_msgs::msg::MarkerArray marker4_arr; 

        float crtflr =0.0;
      
        for(int k=0;k<j;k++){
          

        marker.header.stamp.sec = 0;
        marker.header.stamp.nanosec = 0;
        marker.header.frame_id="map";
        marker.ns = "path_line";
        marker.id = 100+k;
        marker.type = visualization_msgs::msg::Marker::LINE_LIST;
        marker.action = 0;
        marker.pose = geometry_msgs::msg::Pose();
        marker.scale.set__x(0.08);
        marker.color.a = 0.8;
        marker.color.b = 0.78;
        marker.color.g = 0.70;
        marker.color.r = 0.18;
        // {
        // marker.color.a = 0.8;
        // marker.color.b = 0.57;
        // marker.color.g = 0.17;
        // marker.color.r = 0.95;
        // }

        marker4.header.stamp.sec = 0;
        marker4.header.stamp.nanosec = 0;
        marker4.header.frame_id="map";
        marker4.ns = "green_cylinder";
        marker4.id = 600+k;
        marker4.type = visualization_msgs::msg::Marker::CYLINDER;
        marker4.action = 0;
        marker4.pose = geometry_msgs::msg::Pose();
        marker4.scale.x = 0.6;
        marker4.scale.y = 0.6;
        marker4.scale.z = 0.11;
        marker4.color.a = 1.0;
        marker4.color.b = 0.49;
        marker4.color.g = 0.94;
        marker4.color.r = 0.36;

        if(submessage.rviz[k]==submessage.rviz[k+1])
        {
          std::string t = submessage.rviz[k];
          char crtloc= t[0];
          switch(crtloc) 
              {
                  case 'a':
                    marker4.pose.position.x = 0.0;
                    marker4.pose.position.y = -3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'b':
                    marker4.pose.position.x = -4.0;
                    marker4.pose.position.y = -3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'c':
                    marker4.pose.position.x = -4.0;
                    marker4.pose.position.y = -1.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'd':
                    marker4.pose.position.x = -4.0;
                    marker4.pose.position.y = 1.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'e':
                    marker4.pose.position.x = -2.0;
                    marker4.pose.position.y = 3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'f':
                    marker4.pose.position.x = 0.0;
                    marker4.pose.position.y = 3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'g':
                    marker4.pose.position.x = 2.0;
                    marker4.pose.position.y = 3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'h':
                    marker4.pose.position.x = 4.0;
                    marker4.pose.position.y = 3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'i':
                    marker4.pose.position.x = 6.0;
                    marker4.pose.position.y = 1.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'j':
                    marker4.pose.position.x = 6.0;
                    marker4.pose.position.y = -1.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'k':
                    marker4.pose.position.x = 6.0;
                    marker4.pose.position.y = -3.0;
                    marker4.pose.position.z = crtflr;
                    break;
                  case 'x':
                    
                    break;
                  case 'y':
                    
                    break;
              }
          marker4_arr.markers.push_back(marker4);
        }
        
        pnt_pub->publish(marker4_arr);


        if(submessage.rviz[k]=="1")
        {
        crtflr = 0.0;
        k++;
        }else if(submessage.rviz[k]=="2")
        {
        crtflr = 1.0;
        k++;
        }else if(submessage.rviz[k]=="3")
        {
        crtflr = 2.0;
        k++;
        }else if(submessage.rviz[k]=="4")
        {
        crtflr = 3.0;
        k++;
        }else if(submessage.rviz[k]=="5")
        {
        crtflr = 4.0;
        k++;
        }else if(submessage.rviz[k]=="6")
        {
        crtflr = 5.0;
        k++;
        }else if(submessage.rviz[k]=="7")
        {
        crtflr = 6.0;
        k++;
        }else if(submessage.rviz[k]=="8")
        {
        crtflr = 7.0;
        k++;
        }else if(submessage.rviz[k]=="9")
        {
        crtflr = 8.0;
        k++;
        }else if(submessage.rviz[k]=="10")
        {
        crtflr = 9.0;
        k++;
        }else if(submessage.rviz[k]=="11")
        {
        crtflr = 10.0;
        k++;
        }else if(submessage.rviz[k]=="12")
        {
        crtflr = 11.0;
        k++;
        }else if(submessage.rviz[k]=="13")
        {
        crtflr = 12.0;
        k++;
        }else if(submessage.rviz[k]=="14")
        {
        crtflr = 13.0;
        k++;
        }else if(submessage.rviz[k]=="15")
        {
        crtflr = 14.0;
        k++;
        }

        std::string s1=submessage.rviz[k];
        if(s1=="x1")
        {
          s1="d2";
        }else if(s1=="y1")
        {
          s1="h2";
        }
        
        std::string s2=submessage.rviz[k+1];
        if(s2=="x1")
        {
          s2="d2";
        }else if(s2=="y1")
        {
          s2="h2";
        }
        const char *str1=s1.c_str();
        const char *str2=s2.c_str();
        int line_push = strcmp(str1,str2);
        float line_side = 0.0;
        float line_side2 = 0.0;
        float line_side3 = 0.0;

        if(line_push==0)
        {
          line_side3=0.4;
        } else if (line_push<0){
          line_side =-0.2;
        } else if (line_push>0){
          line_side2 = 0.2;
        }

        auto point1 = geometry_msgs::msg::Point();
        auto point2 = geometry_msgs::msg::Point();
      
        if(submessage.rviz[k]=="a1")
        {
        point1.x = 0.0;
        point1.y = -3.0-line_side;
        point1.z = crtflr;
        }else if(submessage.rviz[k]=="b1")
        {
        point1.x = -4.0-line_side-line_side2;
        point1.y = -3.0-line_side2;
        point1.z = crtflr;
        }else if(submessage.rviz[k]=="c1")
        {
        point1.x = -4.0-line_side-line_side2;
        point1.y = -1.0;
        point1.z = crtflr;
        }else if(submessage.rviz[k]=="d1")
        {
        point1.x = -4.0-line_side-line_side2;
        point1.y = 1.0;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="e1")
        {
        point1.x = -2.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="f1")
        {
        point1.x = 0.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="g1")
        {
        point1.x = 2.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="h1")
        {
        point1.x = 4.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="i1")
        {
        point1.x = 6.0+line_side+line_side2;
        point1.y = 1.0;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="j1")
        {
        point1.x = 6.0+line_side+line_side2;
        point1.y = -1.0;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="k1")
        {
        point1.x = 6.0+line_side2;
        point1.y = -3.0-line_side-line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="x1")
        {
        point1.x = -4.0-line_side2+line_side3;
        point1.y = 3.0+line_side;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="y1")
        {
        point1.x = 6.0+line_side+line_side3;
        point1.y = 3.0+line_side2;
        point1.z = crtflr;
        }

        if(submessage.rviz[k+1]=="a1")
        {
        point2.x = 0.0;
        point2.y = -3.0-line_side-line_side2;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="b1")
        {
        point2.x = -4.0-line_side2;
        point2.y = -3.0-line_side;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="c1")
        {
        point2.x = -4.0-line_side-line_side2;
        point2.y = -1.0;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="d1")
        {
        point2.x = -4.0-line_side-line_side2;
        point2.y = 1.0;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="e1")
        {
        point2.x = -2.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="f1")
        {
        point2.x = 0.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="g1")
        {
        point2.x = 2.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="h1")
        {
        point2.x = 4.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="i1")
        {
        point2.x = 6.0+line_side+line_side2;
        point2.y = 1.0;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="j1")
        {
        point2.x = 6.0+line_side+line_side2;
        point2.y = -1.0;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="k1")
        {
        if(submessage.rviz[k]=="a1")
        {
        point2.y = -3.0+0.2;
        }
        else
        {
        point2.y = -3.0;
        }
        point2.x = 6.0+line_side;
        // point2.y = -3.0;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="x1")
        {
        point2.x = -4.0-line_side+line_side2;
        point2.y = 3.0+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="y1")
        {
        point2.x = 6.0+line_side2;
        point2.y = 3.0+line_side;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="1")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 0.0;
        }
        else if(submessage.rviz[k+1]=="2")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 1.0;
        }
        else if(submessage.rviz[k+1]=="3")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 2.0;
        }
        else if(submessage.rviz[k+1]=="4")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 3.0;
        }
        else if(submessage.rviz[k+1]=="5")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 4.0;
        }
        else if(submessage.rviz[k+1]=="6")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 5.0;
        }
        else if(submessage.rviz[k+1]=="7")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 6.0;
        }
        else if(submessage.rviz[k+1]=="8")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 7.0;
        }
        else if(submessage.rviz[k+1]=="9")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 8.0;
        }
        else if(submessage.rviz[k+1]=="10")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 9.0;
        }
        else if(submessage.rviz[k+1]=="11")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 10.0;
        }
        else if(submessage.rviz[k+1]=="12")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 11.0;
        }
        else if(submessage.rviz[k+1]=="13")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 12.0;
        }
        else if(submessage.rviz[k+1]=="14")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 13.0;
        }
        else if(submessage.rviz[k+1]=="15")
        {
          if(submessage.rviz[k]=="y1")
        {
          point1.x-=0.1;
        }  
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 14.0;
        }

      
        marker.points.push_back(point1);
        marker.points.push_back(point2);
        // rviz_pub->publish(marker);
        marker_arr.markers.push_back(marker);
        }
        line_pub->publish(marker_arr);
        

        visualization_msgs::msg::Marker marker5;
        visualization_msgs::msg::MarkerArray marker5_arr;

        visualization_msgs::msg::Marker marker6;
        visualization_msgs::msg::MarkerArray marker6_arr;
        for(int k=j;k<submessage.num-1;k++){
          

        marker5.header.stamp.sec = 0;
        marker5.header.stamp.nanosec = 0;
        marker5.header.frame_id="map";
        marker5.ns = "path_line2";
        marker5.id = 200+k;
        marker5.type = visualization_msgs::msg::Marker::LINE_LIST;
        marker5.action = 0;
        marker5.pose = geometry_msgs::msg::Pose();
        marker5.scale.set__x(0.08);
        // marker5.color.a = 0.8;
        // marker5.color.b = 0.78;
        // marker5.color.g = 0.70;
        // marker5.color.r = 0.18;
        // {
        marker5.color.a = 0.8;
        marker5.color.b = 0.57;
        marker5.color.g = 0.17;
        marker5.color.r = 0.95;
        // }

        marker6.header.stamp.sec = 0;
        marker6.header.stamp.nanosec = 0;
        marker6.header.frame_id="map";
        marker6.ns = "green_cylinder2";
        marker6.id = 700+k;
        marker6.type = visualization_msgs::msg::Marker::CYLINDER;
        marker6.action = 0;
        marker6.pose = geometry_msgs::msg::Pose();
        marker6.scale.x = 0.6;
        marker6.scale.y = 0.6;
        marker6.scale.z = 0.11;
        marker6.color.a = 1.0;
        marker6.color.b = 0.49;
        marker6.color.g = 0.94;
        marker6.color.r = 0.36;

        if(submessage.rviz[k]==submessage.rviz[k+1])
        {
          std::string t = submessage.rviz[k];
          char crtloc= t[0];
          switch(crtloc) 
              {
                  case 'a':
                    marker6.pose.position.x = 0.0;
                    marker6.pose.position.y = -3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'b':
                    marker6.pose.position.x = -4.0;
                    marker6.pose.position.y = -3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'c':
                    marker6.pose.position.x = -4.0;
                    marker6.pose.position.y = -1.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'd':
                    marker6.pose.position.x = -4.0;
                    marker6.pose.position.y = 1.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'e':
                    marker6.pose.position.x = -2.0;
                    marker6.pose.position.y = 3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'f':
                    marker6.pose.position.x = 0.0;
                    marker6.pose.position.y = 3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'g':
                    marker6.pose.position.x = 2.0;
                    marker6.pose.position.y = 3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'h':
                    marker6.pose.position.x = 4.0;
                    marker6.pose.position.y = 3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'i':
                    marker6.pose.position.x = 6.0;
                    marker6.pose.position.y = 1.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'j':
                    marker6.pose.position.x = 6.0;
                    marker6.pose.position.y = -1.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'k':
                    marker6.pose.position.x = 6.0;
                    marker6.pose.position.y = -3.0;
                    marker6.pose.position.z = crtflr;
                    break;
                  case 'x':
                    
                    break;
                  case 'y':
                    
                    break;
              }
          marker6_arr.markers.push_back(marker6);
        }
        
        pnt_pub2->publish(marker6_arr);


        if(submessage.rviz[k]=="1")
        {
        crtflr = 0.0;
        k++;
        }else if(submessage.rviz[k]=="2")
        {
        crtflr = 1.0;
        k++;
        }else if(submessage.rviz[k]=="3")
        {
        crtflr = 2.0;
        k++;
        }else if(submessage.rviz[k]=="4")
        {
        crtflr = 3.0;
        k++;
        }else if(submessage.rviz[k]=="5")
        {
        crtflr = 4.0;
        k++;
        }else if(submessage.rviz[k]=="6")
        {
        crtflr = 5.0;
        k++;
        }else if(submessage.rviz[k]=="7")
        {
        crtflr = 6.0;
        k++;
        }else if(submessage.rviz[k]=="8")
        {
        crtflr = 7.0;
        k++;
        }else if(submessage.rviz[k]=="9")
        {
        crtflr = 8.0;
        k++;
        }else if(submessage.rviz[k]=="10")
        {
        crtflr = 9.0;
        k++;
        }else if(submessage.rviz[k]=="11")
        {
        crtflr = 10.0;
        k++;
        }else if(submessage.rviz[k]=="12")
        {
        crtflr = 11.0;
        k++;
        }else if(submessage.rviz[k]=="13")
        {
        crtflr = 12.0;
        k++;
        }else if(submessage.rviz[k]=="14")
        {
        crtflr = 13.0;
        k++;
        }else if(submessage.rviz[k]=="15")
        {
        crtflr = 14.0;
        k++;
        }

        std::string s1=submessage.rviz[k];
        if(s1=="x1")
        {
          s1="d2";
        }else if(s1=="y1")
        {
          s1="h2";
        }
        
        std::string s2=submessage.rviz[k+1];
        if(s2=="x1")
        {
          s2="d2";
        }else if(s2=="y1")
        {
          s2="h2";
        }
        const char *str1=s1.c_str();
        const char *str2=s2.c_str();
        int line_push = strcmp(str1,str2);
        float line_side = 0.0;
        float line_side2 = 0.0;
        float line_side3 = 0.0;

        if(line_push==0)
        {
          line_side3=0.4;
        } else if (line_push<0){
          line_side =-0.2;
        } else if (line_push>0){
          line_side2 = 0.2;
        }

        auto point1 = geometry_msgs::msg::Point();
        auto point2 = geometry_msgs::msg::Point();
      
        if(submessage.rviz[k]=="a1")
        {
        point1.x = 0.0;
        point1.y = -3.0-line_side;
        point1.z = crtflr;
        }else if(submessage.rviz[k]=="b1")
        {
        point1.x = -4.0-line_side-line_side2;
        point1.y = -3.0-line_side2;
        point1.z = crtflr;
        }else if(submessage.rviz[k]=="c1")
        {
        point1.x = -4.0-line_side-line_side2;
        point1.y = -1.0;
        point1.z = crtflr;
        }else if(submessage.rviz[k]=="d1")
        {
        point1.x = -4.0-line_side-line_side2;
        point1.y = 1.0;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="e1")
        {
        point1.x = -2.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="f1")
        {
        point1.x = 0.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="g1")
        {
        point1.x = 2.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="h1")
        {
        point1.x = 4.0;
        point1.y = 3.0+line_side+line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="i1")
        {
        point1.x = 6.0+line_side+line_side2;
        point1.y = 1.0;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="j1")
        {
        point1.x = 6.0+line_side+line_side2;
        point1.y = -1.0;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="k1")
        {
        point1.x = 6.0+line_side2;
        point1.y = -3.0-line_side-line_side2;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="x1")
        {
        point1.x = -4.0-line_side2+line_side3;
        point1.y = 3.0+line_side;
        point1.z = crtflr;
        }
        else if(submessage.rviz[k]=="y1")
        {
        point1.x = 6.0+line_side+line_side3;
        point1.y = 3.0+line_side2;
        point1.z = crtflr;
        }

        if(submessage.rviz[k+1]=="a1")
        {
        point2.x = 0.0;
        point2.y = -3.0-line_side-line_side2;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="b1")
        {
        point2.x = -4.0-line_side2;
        point2.y = -3.0-line_side;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="c1")
        {
        point2.x = -4.0-line_side-line_side2;
        point2.y = -1.0;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="d1")
        {
        point2.x = -4.0-line_side-line_side2;
        point2.y = 1.0;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="e1")
        {
        point2.x = -2.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="f1")
        {
        point2.x = 0.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="g1")
        {
        point2.x = 2.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="h1")
        {
        point2.x = 4.0;
        point2.y = 3.0+line_side+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="i1")
        {
        point2.x = 6.0+line_side+line_side2;
        point2.y = 1.0;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="j1")
        {
        point2.x = 6.0+line_side+line_side2;
        point2.y = -1.0;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="k1")
        {
        point2.x = 6.0+line_side;
        point2.y = -3.0;
        point2.z = crtflr;
        }else if(submessage.rviz[k+1]=="x1")
        {
        point2.x = -4.0-line_side+line_side2;
        point2.y = 3.0+line_side2;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="y1")
        {
        point2.x = 6.0+line_side2;
        point2.y = 3.0+line_side;
        point2.z = crtflr;
        }
        else if(submessage.rviz[k+1]=="1")
        {
        if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }    
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 0.0;
        }
        else if(submessage.rviz[k+1]=="2")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }    
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 1.0;
        }
        else if(submessage.rviz[k+1]=="3")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 2.0;
        }
        else if(submessage.rviz[k+1]=="4")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 3.0;
        }
        else if(submessage.rviz[k+1]=="5")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 4.0;
        }
        else if(submessage.rviz[k+1]=="6")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 5.0;
        }
        else if(submessage.rviz[k+1]=="7")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 6.0;
        }
        else if(submessage.rviz[k+1]=="8")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 7.0;
        }
        else if(submessage.rviz[k+1]=="9")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 8.0;
        }
        else if(submessage.rviz[k+1]=="10")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 9.0;
        }
        else if(submessage.rviz[k+1]=="11")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 10.0;
        }
        else if(submessage.rviz[k+1]=="12")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 11.0;
        }
        else if(submessage.rviz[k+1]=="13")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 12.0;
        }
        else if(submessage.rviz[k+1]=="14")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 13.0;
        }
        else if(submessage.rviz[k+1]=="15")
        {
          if(submessage.rviz[k]=="x1")
        {
          point1.x+=0.4;
        }else if(submessage.rviz[k]=="y1")
        {
          point1.x+=0.2;
        }     
        point2.x = point1.x;
        point2.y = point1.y;
        point2.z = 14.0;
        }

      
        marker5.points.push_back(point1);
        marker5.points.push_back(point2);
        // rviz_pub->publish(marker);
        marker5_arr.markers.push_back(marker5);
        }
        line_pub2->publish(marker5_arr);



        // visualization_msgs::msg::Marker marker2;
        // marker2.header.stamp.sec = 0;
        // marker2.header.stamp.nanosec = 0;
        // marker2.header.frame_id="map";
        // marker2.ns = "while_cylinder";
        // marker2.id = 0;
        // marker2.type = visualization_msgs::msg::Marker::CYLINDER;
        // marker2.action = 0;
        // marker2.pose = geometry_msgs::msg::Pose();
        // marker2.scale.x = 0.5;
        // marker2.scale.y = 0.5;
        // marker2.scale.z = 0.1;
        // marker2.color.a = 1.0;
        // marker2.color.b = 1.0;
        // marker2.color.g = 1.0;
        // marker2.color.r = 1.0;
        // marker2.pose.position.x = 0.0;
        // marker2.pose.position.y = 0.0;
        // marker2.pose.position.z = 0.0;

        // rviz_pub2->publish(marker2);

        visualization_msgs::msg::Marker marker2;
        visualization_msgs::msg::MarkerArray marker2_arr;
        position pl;
        std::vector<position> vec_line_position;

        float xlp[] = {0,-4,-4,-4,-4,-2,0,2,4,6,6,6,6,0};
        float ylp[] = {-3,-3,-1,1,3,3,3,3,3,3,1,-1,-3,-3};

        for(int i=1;i<15;i++){
          pl.x=xlp[i-1];
          pl.y=ylp[i-1];
          pl.z=0;
          vec_line_position.push_back(pl);
        }

        for(int j=0;j<15;j++)
        {
          for(int i=1;i<15;i++)
          {
            position line_node = vec_line_position[i-1];
            position line_node2 = vec_line_position[i];
            marker2.header.stamp.sec = 0;
            marker2.header.stamp.nanosec = 0;
            marker2.header.frame_id="map";
            marker2.ns = "while_line";
            marker2.id = (17*j+i);
            marker2.type = visualization_msgs::msg::Marker::LINE_LIST;
            marker2.action = 0;
            marker2.pose = geometry_msgs::msg::Pose();
            marker2.scale.set__x(0.02);
            marker2.color.a = 1.0;
            marker2.color.b = 1.0;
            marker2.color.g = 1.0;
            marker2.color.r = 1.0;

            auto lpoint1 = geometry_msgs::msg::Point();
            auto lpoint2 = geometry_msgs::msg::Point();

            lpoint1.x = line_node.x;;
            lpoint1.y = line_node.y;
            lpoint1.z = j;

            lpoint2.x = line_node2.x;;
            lpoint2.y = line_node2.y;
            lpoint2.z = j;

            marker2.points.push_back(lpoint1);
            marker2.points.push_back(lpoint2);
            marker2_arr.markers.push_back(marker2);
          }

          edge_pub->publish(marker2_arr);
        }


        visualization_msgs::msg::MarkerArray marker3_arr;      
        visualization_msgs::msg::Marker marker3;
        position p;
        std::vector<position> vec_position;

        float xp[] = {0,-4,-4,-4,-4,-2,0,2,4,6,6,6,6};
        float yp[] = {-3,-3,-1,1,3,3,3,3,3,3,1,-1,-3}; 
        for(int i=1;i<14;i++){
          p.x=xp[i-1];
          p.y=yp[i-1];
          p.z=0;
          vec_position.push_back(p);
        }
        for(int j=0;j<15;j++)
        {
          for(int i=1;i<14;i++)
          {
            position cy_node = vec_position[i-1];
            marker3.header.stamp.sec = 0;
            marker3.header.stamp.nanosec = 0;
            marker3.header.frame_id="map";
            marker3.ns = "while_cylinder2";
            marker3.id = (13*j+i);
            marker3.type = visualization_msgs::msg::Marker::CYLINDER;
            marker3.action = 0;
            marker3.pose = geometry_msgs::msg::Pose();
            marker3.scale.x = 0.5;
            marker3.scale.y = 0.5;
            marker3.scale.z = 0.1;
            marker3.color.a = 1.0;
            marker3.color.b = 1.0;
            marker3.color.g = 1.0;
            marker3.color.r = 1.0;
            marker3.pose.position.x = cy_node.x;
            marker3.pose.position.y = cy_node.y;
            marker3.pose.position.z = (float)j;
            marker3_arr.markers.push_back(marker3);
          }
        }
        node_pub->publish(marker3_arr);
    }

  rclcpp::Subscription<custom_message::msg::Msg>::SharedPtr path_sub;

  private:

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    // rclcpp::sleep_for(std::chrono::seconds(1));
    // auto node = rclcpp::Node::make_shared("test_rviz");
    // auto pub = node->create_publisher<visualization_msgs::msg::Marker>("test_marker", rclcpp::QoS(1));
    // while (!pub->get_subscription_count()) {
    //     sleep(1);
    //     std::cout << "sleep" << std::endl;
    // }
    // std::cout << "pub ->" << std::endl;
    // visualization_msgs::msg::Marker marker;
    // marker.header.stamp.sec = 0;
    // marker.header.stamp.nanosec = 0;
    // marker.header.frame_id="map";
    // marker.ns = "while_line";
    // marker.id = 0;
    // marker.type = visualization_msgs::msg::Marker::LINE_LIST;
    // marker.action = 0;
    // marker.pose = geometry_msgs::msg::Pose();
    // marker.scale.set__x(0.08);
    // marker.color.a = 1.0;
    // marker.color.b = 1.0;
    // marker.color.g = 1.0;
    // marker.color.r = 1.0;
    // auto point1 = geometry_msgs::msg::Point();
    // point1.x = 1.0;
    // point1.y = 1.0;
    // point1.z = 0.0;
    // auto point2 = geometry_msgs::msg::Point();
    // point2.x = 2.0;
    // point2.y = 2.0;
    // point2.z = 0.0;
    // marker.points.push_back(point1);
    // marker.points.push_back(point2);
    // pub->publish(marker);
    rclcpp::spin(std::make_shared<ArrayPathSubscriber>());
    rclcpp::shutdown();
    return 0;
    
}
