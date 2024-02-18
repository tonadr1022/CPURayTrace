//
// Created by Tony Adriansen on 2/5/24.
//

#include "../Scene.hpp"
#include "App.hpp"
#include "../input/Keyboard.hpp"
#include "../input/Mouse.hpp"
#include "../Timer.hpp"

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
  yoffset > 0 ? m_camera.setFOV(m_camera.fov() + 1) : m_camera.setFOV(m_camera.fov() - 1);
  m_reset = true;
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

  auto scene = std::make_shared<Scene>();
  m_sceneManager.setActiveScene(scene);
  int matIndex = 0;
  // ground
  m_sceneManager.createLambertian(glm::vec3(0.5f));
  m_sceneManager.createSphere(glm::vec3(0, -1000, -1), 1000, matIndex++);

  for (int x = -11; x < 11; x++) {
    for (int z = -11; z < 11; z++) {
      float chooseMat = Math::randomFloat();
      glm::vec3 center(x + 0.9 * Math::randomFloat(), 0.2, z + 0.9 * Math::randomFloat());

      if (glm::length(center - glm::vec3(4, 0.2, 0)) > 0.9) {
        if (chooseMat < 0.8) {
          // lambertian diffuse
          auto albedo = Math::randVec3();
          m_sceneManager.createLambertian(albedo);
        } else if (chooseMat < 0.95) {
          auto albedo = Math::randVec3(0.5, 1.0);
          float fuzz = Math::randomFloat(0, 0.5f);
          m_sceneManager.createMetal(albedo, fuzz);
        } else {
          m_sceneManager.createDielectric(1.5f);
        }
        m_sceneManager.createSphere(center, 0.2, matIndex++);
      }
    }
  }
  m_sceneManager.createDielectric(1.5);
  m_sceneManager.createSphere({0, 1, 0}, 1.0f, matIndex++);

  m_sceneManager.createLambertian({0.4, 0.2, 0.1});
  m_sceneManager.createSphere({-4, 1, 0}, 1.0f, matIndex++);

  m_sceneManager.createMetal({0.7, 0.6, 0.5}, 0.0);
  m_sceneManager.createSphere({4, 1, 0}, 1.0f, matIndex++);
//
//  m_sceneManager.createLambertian(glm::vec3(0.1f, 0.2f, 0.5f));
//  m_sceneManager.createDielectric(1.5);
//  m_sceneManager.createMetal(glm::vec3(0.8, 0.6, 0.2), 0.0f);
//
//  m_sceneManager.createSphere(glm::vec3(0, 0, -1), 0.5f, 1);
//  // glass hollow
//  m_sceneManager.createSphere(glm::vec3(-1, 0, -1), 0.5f, 2);
//  m_sceneManager.createSphere(glm::vec3(-1, 0, -1), -0.45f, 2);
//  m_sceneManager.createSphere(glm::vec3(-1, 0, -1), 0.1f, 0);
//
//  m_sceneManager.createSphere(glm::vec3(1, 0, -1), 0.5f, 3);

  m_camera.setLookFrom({13, 2, 3});
  m_camera.setLookAt({0, 0, 0});
  m_camera.setVUp({0, 1, 0});
  m_camera.defocusAngle = 0.6f;
  m_camera.focusDistance = 10.0f;
  m_camera.setFOV(90);
  m_camera.setDimensions(m_viewportWidth, m_viewportHeight);

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
  m_camera.setDimensions(m_viewportWidth, m_viewportHeight);
  m_renderer.onResize(m_viewportWidth, m_viewportHeight);
  m_renderer.render(m_camera, *m_sceneManager.scene());
  m_lastRenderDurationMS = timer.elapsedMilliSeconds();
}

void App::onUIRender() {

  bool opened;
  static ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
//  flags |= ;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Viewport", &opened, flags);
  auto currActualSize = ImGui::GetContentRegionAvail();

  if (m_viewportWidth != (int) currActualSize.x || m_viewportHeight != (int) currActualSize.y) {
    ImGui::SetWindowSize(ImVec2((float) m_viewportWidth, (float) m_viewportHeight));
  }
//  m_viewportWidth = (int) ImGui::GetContentRegionAvail().x;
//  m_viewportHeight = (int) ImGui::GetContentRegionAvail().y;

  auto finalTexture = m_renderer.getFinalTexture();
  if (finalTexture) {
    ImGui::Image(reinterpret_cast<ImTextureID>(finalTexture->getId()),
                 ImVec2((float) finalTexture->width(), (float) finalTexture->height()),
                 ImVec2(0, 0),
                 ImVec2(1, 1));
  }
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::Begin("Scene");
  static int tabIndex = 0;
  {
    ImGui::SameLine();
    if (ImGui::Button("Settings")) {
      tabIndex = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Scene")) {
      tabIndex = 1;
    }
  }
  if (tabIndex == 0) {
    ImGui::Text("Frame Time: %.3f", m_lastRenderDurationMS);
    ImGui::Text("FPS: %.3f", 1000.0f / m_lastRenderDurationMS);
//    ImGui::Text("Width: %i  Height: %i", m_viewportWidth, m_viewportHeight);
    ImGui::InputInt("Width", &m_viewportWidth);
    ImGui::InputInt("Height", &m_viewportHeight);
    ImGui::Text("Frame Index %i", m_renderer.frameIndex());

    ImGui::Checkbox("Accumulate", &m_renderer.settings().accumulate);

    if (ImGui::InputInt("Max Bounce Depth", &m_renderer.settings().maxBounceDepth)) {
      m_reset = true;
    }
    float fov = m_camera.fov();
    if (ImGui::InputFloat("FOV", &fov, 20, 120)) {
      m_camera.setFOV(fov);
    }

    ImGui::Checkbox("Render Loop", &m_renderLoop);

    if (ImGui::Button("Render")) {
      render();
    }
    if (ImGui::Button("Reset")) {
      m_renderer.reset();
    }

    if (ImGui::Button("Screenshot")) {
      m_renderer.screenshot();
    }

  } else if (tabIndex == 1) {
    Scene& scene = *m_sceneManager.scene();
    for (int i = 0; i < scene.spheres.size(); i++) {
      ImGui::PushID(i);
      Sphere& sphere = scene.spheres[i];

      if (ImGui::DragFloat3("Position", glm::value_ptr(sphere.center), 0.1f) ||
          ImGui::DragFloat("Radius", &sphere.radius, 0.1f) ||
          ImGui::DragInt("Material", &sphere.materialIndex, 1, 0, (int) scene.materials.size() - 1)) {
        m_reset = true;
      }

      ImGui::Separator();
      ImGui::PopID();
    }

    ImGui::Text("Materials");

    for (int i = 0; i < scene.materials.size(); i++) {
      ImGui::PushID(i);
      Material& material = scene.materials[i];

      if (ImGui::ColorEdit3("Albedo", glm::value_ptr(material.albedo)) ||
          ImGui::Combo("Type", (int*) &material.type, "Lambertian\0Metal\0Dielectric\0")) {
        m_reset = true;
      }
      if (material.type == Metal) {
        if (ImGui::SliderFloat("Fuzz", &material.fuzz, 0.0f, 1.0f, "%.3f")) {
          m_reset = true;
        }
      }

      ImGui::Separator();
      ImGui::PopID();

    }

  }

  ImGui::End();

  if (m_reset) {
    m_renderer.reset();
    m_reset = false;
  }
  if (m_renderLoop) {
    render();
  }

}