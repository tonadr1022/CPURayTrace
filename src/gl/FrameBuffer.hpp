//
// Created by Tony Adriansen on 2/8/24.
//

#ifndef RAY_TRACE_SRC_GL_FRAMEBUFFER_HPP_
#define RAY_TRACE_SRC_GL_FRAMEBUFFER_HPP_

#include "../raytrace_pch.hpp"

class FrameBuffer {
 public:
  FrameBuffer() : m_fbo(0) {}
  // delete copy constructor
  FrameBuffer(const FrameBuffer&) = delete;
  // delete copy assignment operator
  FrameBuffer& operator=(const FrameBuffer&) = delete;
  // move constructor, ensure other cant access with fbo anymore
  FrameBuffer(FrameBuffer&& other) noexcept: m_fbo(other.m_fbo) {
    other.m_fbo = 0;
  }
  // move operator, ensure other cant access with fbo anymore
  FrameBuffer& operator=(FrameBuffer&& other) noexcept {
    assert(m_fbo == 0);
    m_fbo = other.m_fbo;
    other.m_fbo = 0;
    return *this;
  }
  ~FrameBuffer() {
    if (m_fbo != 0) {
      glDeleteFramebuffers(1, &m_fbo);
    }
  }

  void init() {
    assert(m_fbo == 0);
    glGenFramebuffers(1, &m_fbo);
  }

  void bind() const {
    assert(m_fbo != 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  }

  static void unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void attachTexture2D(GLuint textureColorBuffer) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
  }

 private:
  GLuint m_fbo;
};

#endif //RAY_TRACE_SRC_GL_FRAMEBUFFER_HPP_
