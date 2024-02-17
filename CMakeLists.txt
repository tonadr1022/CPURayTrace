cmake_minimum_required(VERSION 3.27)
project(ray_trace)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(TARGET PROPERTY CXX_STANDARD 20)


add_executable(${PROJECT_NAME} src/main.cpp
        src/color.hpp
        src/math/Ray.hpp
        src/raytrace_pch.hpp
        src/Renderer.cpp
        src/Renderer.hpp
        src/Hittable.hpp
        src/Sphere.hpp
        src/app/App.cpp
        src/app/App.hpp
        src/app/Window.cpp
        src/input/Keyboard.cpp
        src/input/Mouse.cpp
        dep/glad/glad.c
        dep/imgui-docking/imgui.cpp
        dep/imgui-docking/imgui_demo.cpp
        dep/imgui-docking/imgui_draw.cpp
        dep/imgui-docking/imgui_tables.cpp
        dep/imgui-docking/imgui_widgets.cpp
        dep/imgui-docking/backends/imgui_impl_glfw.cpp
        dep/imgui-docking/backends/imgui_impl_opengl3.cpp
        src/gl/FrameBuffer.hpp
        src/gl/Texture.hpp
        src/Camera.cpp
        src/Camera.hpp
        src/Timer.hpp
        src/HittableList.hpp
        src/Scene.cpp
        src/Scene.hpp
        src/math/Interval.hpp
        src/math/Math.hpp
        src/SceneManager.cpp
        src/SceneManager.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        dep
        dep/stb_image
        dep/glad
        dep/imgui-docking
        dep/imgui-docking/backends
        dep/thread_pool/include
)

add_subdirectory(dep/glfw)
add_subdirectory(dep/glm)


find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glm)

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
