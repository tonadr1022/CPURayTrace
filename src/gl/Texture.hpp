//
// Created by Tony Adriansen on 2/8/24.
//

#ifndef RAY_TRACE_SRC_GL_TEXTURE_HPP_
#define RAY_TRACE_SRC_GL_TEXTURE_HPP_

#include "../raytrace_pch.hpp"


//class ImageLoader {
// public:
//  explicit ImageLoader(const char* filename);
//  ImageLoader(const char* filename, GLsizei depth);
//  [[nodiscard]] const ImageData& getData() const { return m_data; };
//  ~ImageLoader();
// private:
//  ImageData m_data;
//};
//
//ImageLoader::ImageLoader(const char* filename) {
//
//}
//
//ImageLoader::ImageLoader(const char* filename, GLsizei depth) {
//
//}
//ImageLoader::~ImageLoader() {
//
//}

template<GLenum T>
class Texture {
 public:
//  Texture() : m_texture(0), m_width(0), m_height(0) {}
  Texture() = delete;
  Texture(int width, int height) : m_texture(0), m_width(width), m_height(height) {
  }
  // delete copy constructor
  Texture(const Texture&) = delete;
  // delete copy assignment operator
  Texture& operator=(const Texture&) = delete;
  // move constructor
  Texture(Texture&& other) noexcept: m_texture(other.m_texture), m_width(other.m_width), m_height(other.m_height) {
    other.m_texture = 0;
  }
  // move operator
  Texture& operator=(Texture&& other) noexcept {
    assert(m_texture == 0);
    m_texture = other.m_texture;
    other.m_texture = 0;
    return *this;
  }
  ~Texture() {
    if (m_texture != 0) {
      glDeleteTextures(1, &m_texture);
    }
  }
  int width() {return m_width;}
  int height() {return m_height; }
  [[nodiscard]] GLuint getId() const { return m_texture; }
  [[nodiscard]] uint32_t getWidth() const { return m_width; }
  [[nodiscard]] uint32_t getHeight() const { return m_height; }

#define IS_1D (T == GL_TEXTURE_1D)
#define IS_2D (T == GL_TEXTURE_2D || T == GL_TEXTURE_1D_ARRAY)
#define IS_3D (T == GL_TEXTURE_3D || T == GL_TEXTURE_2D_ARRAY)
#define IS_1D_ARRAY (T == GL_TEXTURE_1D_ARRAY)
#define IS_2D_ARRAY (T == GL_TEXTURE_2D_ARRAY)

  void init() {
    assert(m_texture == 0);
    glGenTextures(1, &m_texture);
  }
  void bind() const {
    assert(m_texture != 0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
  }

  void bind(GLenum slot) const {
    assert(m_texture != 0);
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, m_texture);
  }

  void resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    bind();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 static_cast<int>(m_width),
                 static_cast<int>(m_height),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 nullptr);
  }


  void load2dData(const void* data, int width, int height) {
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }

  void setWrapFilter(GLint filter) {
    bind();
    if (IS_1D) {
      glTexParameteri(T, GL_TEXTURE_WRAP_S, filter);
    } else if (IS_2D) {
      glTexParameteri(T, GL_TEXTURE_WRAP_S, filter);
      glTexParameteri(T, GL_TEXTURE_WRAP_T, filter);
    } else if (IS_3D) {
      glTexParameteri(T, GL_TEXTURE_WRAP_S, filter);
      glTexParameteri(T, GL_TEXTURE_WRAP_T, filter);
      glTexParameteri(T, GL_TEXTURE_WRAP_R, filter);
    }
  }

  void setMagFilter(GLint minFilter, GLint magFilter) {
    bind();
    glTexParameteri(T, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(T, GL_TEXTURE_MAG_FILTER, magFilter);
  }
  GLuint m_texture;

 private:
  uint32_t m_width;
  uint32_t m_height;
};

#endif //RAY_TRACE_SRC_GL_TEXTURE_HPP_
