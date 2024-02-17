//
// Created by Tony Adriansen on 2/5/24.
//

#ifndef RAY_TRACE_SRC_APP_HPP_
#define RAY_TRACE_SRC_APP_HPP_
#include "../Renderer.hpp"
#include "Window.hpp"
class App {
 public:
  App();
  ~App();
  static App &Instance() { return *instancePtr; }
  void run();
  void onKeyEvent(int key, int scancode, int action, int mods);
  void onMouseButtonEvent(int button, int action, int mods);
  void onCursorPositionEvent(double xpos, double ypos);
  void onScrollEvent(double xoffset, double yoffset);
  void onResizeEvent(int width, int height);
  void onWindowCloseEvent();
  void onUIRender();
  void render();

 private:
  static App *instancePtr;
  Window m_window;
  Renderer m_renderer;
  int m_viewportWidth{}, m_viewportHeight;
  Camera m_camera;
  SceneManager m_sceneManager;
  bool m_running = true;
  bool m_renderLoop = true;
  float m_lastRenderDurationMS{};

  void initImGui();

};

#endif //RAY_TRACE_SRC_APP_HPP_
