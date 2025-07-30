# Multi_story_path-planner

This ROS2 package for global path planning to multiple target locations in multi-story building environments.

## Key Features

Multi-story Support: Path planning across multiple building floors <br>
Multi-target Optimization: Generates optimal routes to visit multiple locations efficiently

## Environments

ROS2 Humble <br>
Ubuntu 22.04

## How to use

mkdir -p ~/ros2_ws/src <br>

cd ~/ros2_ws/src<br>

git clone https://github.com/welve/Multi_story_path-planner-Algorithm.git<br>

colcon build --packages-select multi_story_pathplanner<br>

source install/setup.bash<br>

ros2 run dijkstra box_input

## Example

<img width="820" height="688" alt="n7" src="https://github.com/user-attachments/assets/7f0e0864-c33e-4876-99f2-dd42afbe0213" />
