cmake_minimum_required(VERSION 3.8)
project(dijkstra)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(custom_message REQUIRED)
find_package(visualization_msgs REQUIRED)


# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)


add_executable(dijkstra src/dijkstra.cpp)
ament_target_dependencies(dijkstra rclcpp std_msgs custom_message)

add_executable(box_input src/box_input.cpp)
ament_target_dependencies(box_input rclcpp std_msgs custom_message)

add_executable(visual src/visual.cpp)
ament_target_dependencies(visual rclcpp std_msgs custom_message visualization_msgs)

install(TARGETS
  dijkstra
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
  box_input
  DESTINATION lib/${PROJECT_NAME})

  install(TARGETS
  visual
  DESTINATION lib/${PROJECT_NAME})


ament_package()
