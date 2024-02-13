//
// Created by Tony Adriansen on 2/5/24.
//

#include "App.hpp"
#include "../input/Keyboard.hpp"
#include "../input/Mouse.hpp"
#include "../Timer.hpp"
#include "../Sphere.hpp"

App* App::instancePtr = nullptr;

void App::run() {

  int display_w, display_h;

  while (m_running) {
    Keyboard::update();
    Mouse::update();
    m_window.pollEvents();

    m_window.beginFrame();

    // Begin ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    onUIRender();

    // end ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window.swapBuffers();

  }
}

void App::onKeyEvent(int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    Keyboard::press(key);
  } else if (action == GLFW_RELEASE) {
    Keyboard::release(key);
  }
}

void App::onMouseButtonEvent(int button, int action, int mods) {
  if (ImGui::GetIO().WantCaptureMouse) {
    return;
  }
  if (action == GLFW_PRESS) {
    Mouse::press(button);
  } else if (action == GLFW_RELEASE) {
    Mouse::release(button);
  }
}
void App::onCursorPositionEvent(double xpos, double ypos) {

}

void App::onScrollEvent(double xoffset, double yoffset) {

}

void App::onResizeEvent(int width, int height) {

}

void App::onWindowCloseEvent() {
  m_running = false;
}

App::App() :
    m_window(WIDTH + 400, HEIGHT + 400, "Software Ray Tracer"),
    m_viewportWidth(WIDTH),
    m_viewportHeight(HEIGHT) {
  assert(instancePtr == nullptr && "Only one app instance allowed");
  instancePtr = this;
  initImGui();

  m_scene.addHittable(std::make_shared<Sphere>(glm::vec3(0, 0, -1), 0.5f));
  m_scene.addHittable(std::make_shared<Sphere>(glm::vec3(0, -100.5, -1), 100));

}
App::~App() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  instancePtr = nullptr;
}

void App::initImGui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  ImGui::StyleColorsDark();
  if (!ImGui_ImplGlfw_InitForOpenGL(m_window.getContext(), true) ||
      !ImGui_ImplOpenGL3_Init(m_window.getGlslVersion())) {
    std::cout << "imgui error\n";
    exit(1);
  }
}
void App::render() {
  Timer timer;
  m_camera.onResize(m_viewportWidth, m_viewportHeight);
  m_renderer.onResize(m_viewportWidth, m_viewportHeight);
  m_renderer.render(m_camera, m_scene);
  m_lastRenderDurationMS = timer.elapsedMilliSeconds();
}

void App::onUIRender() {

  bool opened;
  static ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
//  flags |= ;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Viewport", &opened, flags);
  m_viewportWidth = (int) ImGui::GetContentRegionAvail().x;
  m_viewportHeight = (int) ImGui::GetContentRegionAvail().y;

  auto finalTexture = m_renderer.getFinalTexture();
  if (finalTexture) {
    ImGui::Image(reinterpret_cast<ImTextureID>(finalTexture->getId()),
                 ImVec2((float) finalTexture->width(), (float) finalTexture->height()),
                 ImVec2(0, 0),
                 ImVec2(1, 1));
  }
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::Begin("Settings");
  ImGui::Text("Frame Time: %.3f", m_lastRenderDurationMS);
  ImGui::Text("FPS: %.3f", 1000.0f / m_lastRenderDurationMS);
  ImGui::Text("Width: %i  Height: %i", m_viewportWidth, m_viewportHeight);

  ImGui::Checkbox("Accumulate", &m_renderer.settings().accumulate);
  ImGui::InputInt("Max Bounce Depth", &m_renderer.settings().maxBounceDepth);

  ImGui::Checkbox("Render Loop", &m_renderLoop);

  if (ImGui::Button("Render")) {
    render();
  }
  if (ImGui::Button("Reset")) {
    m_renderer.reset();
  }
  ImGui::End();

  if (m_renderLoop) {
    render();
  }
}