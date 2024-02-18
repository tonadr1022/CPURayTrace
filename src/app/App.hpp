//
// Created by Tony Adriansen on 2/5/24.
//

#ifndef RAY_TRACE_SRC_APP_HPP_
#define RAY_TRACE_SRC_APP_HPP_
#include "Window.hpp"
#include "../Camera.hpp"
#include "../SceneManager.hpp"
#include "../Renderer.hpp"

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
  bool m_reset = false;
  static App *instancePtr;
  Window m_window;
  Renderer m_renderer;
  int m_viewportWidth{}, m_viewportHeight;
  Camera m_camera;
  SceneManager m_sceneManager;
  bool m_running = true;
  bool m_renderLoop = false;
  float m_lastRenderDurationMS{};

  void initImGui();

};

#endif //RAY_TRACE_SRC_APP_HPP_
