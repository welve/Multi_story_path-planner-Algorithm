# Multi_story_path-planner

This ROS2 package for global path planning to multiple target locations in multi-story building environments.

## Key Features

Multi-story Support: Path planning across multiple building floors <br>
Multi-target Optimization: Generates optimal routes to visit multiple locations efficiently

## Environments

ROS2 Humble <br>
Ubuntu 22.04

# How to use

## Create workspace
mkdir -p ~/ros2_ws/src <br>
cd ~/ros2_ws/src

## Clone the package
git clone https://github.com/your-username/multi-story-pathplanner.git

## Install dependencies
cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y

## Build
colcon build --packages-select multi_story_pathplanner

## Source the workspace
source install/setup.bash
