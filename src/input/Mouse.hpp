//
// Created by Tony Adriansen on 12/17/23.
//

#ifndef VOXEL_ENGINE_MOUSE_HPP
#define VOXEL_ENGINE_MOUSE_HPP

#include "../raytrace_pch.hpp"
#include "KeyPress.hpp"

class Mouse {
 public:
  static void press(int key);
  static void release(int key);
  static bool isPressed(int key);
  static bool isPressedThisFrame(int key);
  static bool isHeld(int key);
  static void setWindow(GLFWwindow *pWindow);
  static void update();

 private:
  static std::unordered_map<int, KeyPress> keyStates;
  static GLFWwindow *window;

};

#endif //VOXEL_ENGINE_MOUSE_HPP
