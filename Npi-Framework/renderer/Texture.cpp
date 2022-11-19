#include "Texture.hpp"
#include <cstring>
#include <fstream>
#include <png.h>
#include <vector>

#include <stb_image.h>

static u32 GetNextPowerOf2(u32 v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return (v >= 64 ? v : 64);
}

static bool C3DTexToC2DImage(C2D_Image *texture, u32 width, u32 height,
                             u8 *buf) {
  if (width >= 1024 || height >= 1024)
    return false;

  C3D_Tex *tex = new C3D_Tex[sizeof(C3D_Tex)];
  Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture[sizeof(Tex3DS_SubTexture)];
  subtex->width = static_cast<u16>(width);
  subtex->height = static_cast<u16>(height);
  // RGBA -> ABGR
  for (u32 row = 0; row < subtex->width; row++) {
    for (u32 col = 0; col < subtex->height; col++) {
      u32 z = (row + col * subtex->width) * 4;

      u8 r = *(u8 *)(buf + z);
      u8 g = *(u8 *)(buf + z + 1);
      u8 b = *(u8 *)(buf + z + 2);
      u8 a = *(u8 *)(buf + z + 3);

      *(buf + z) = a;
      *(buf + z + 1) = b;
      *(buf + z + 2) = g;
      *(buf + z + 3) = r;
    }
  }

  u32 w_pow2 = GetNextPowerOf2(subtex->width);
  u32 h_pow2 = GetNextPowerOf2(subtex->height);
  subtex->left = 0.f;
  subtex->top = 1.f;
  subtex->right = (subtex->width / static_cast<float>(w_pow2));
  subtex->bottom = (1.0 - (subtex->height / static_cast<float>(h_pow2)));
  C3D_TexInit(tex, static_cast<u16>(w_pow2), static_cast<u16>(h_pow2),
              GPU_RGBA8);
  C3D_TexSetFilter(tex, GPU_NEAREST, GPU_NEAREST);

  std::memset(tex->data, 0, tex->size);

  for (u32 x = 0; x < subtex->width; x++) {
    for (u32 y = 0; y < subtex->height; y++) {
      u32 dst_pos = ((((y >> 3) * (w_pow2 >> 3) + (x >> 3)) << 6) +
                     ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) |
                      ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3))) *
                    4;
      u32 src_pos = (y * subtex->width + x) * 4;
      std::memcpy(&(static_cast<u8 *>(tex->data))[dst_pos],
                  &(static_cast<u8 *>(buf))[src_pos], 4);
    }
  }

  C3D_TexFlush(tex);
  tex->border = 0x00000000;
  C3D_TexSetWrap(tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
  if (tex && subtex) {
    texture->tex = tex;
    texture->subtex = subtex;
    return true;
  }
  return false;
}

static void OLD_C3DTexToC2DImage(C3D_Tex *tex, Tex3DS_SubTexture *subtex,
                                 u8 *buf, u32 size, u32 width, u32 height,
                                 GPU_TEXCOLOR format) {
  // RGBA -> ABGR
  for (u32 row = 0; row < width; row++) {
    for (u32 col = 0; col < height; col++) {
      u32 z = (row + col * width) * 4;

      u8 r = *(u8 *)(buf + z);
      u8 g = *(u8 *)(buf + z + 1);
      u8 b = *(u8 *)(buf + z + 2);
      u8 a = *(u8 *)(buf + z + 3);

      *(buf + z) = a;
      *(buf + z + 1) = b;
      *(buf + z + 2) = g;
      *(buf + z + 3) = r;
    }
  }

  u32 w_pow2 = GetNextPowerOf2(width);
  u32 h_pow2 = GetNextPowerOf2(height);

  subtex->width = (u16)width;
  subtex->height = (u16)height;
  subtex->left = 0.0f;
  subtex->top = 1.0f;
  subtex->right = (width / (float)w_pow2);
  subtex->bottom = 1.0 - (height / (float)h_pow2);

  C3D_TexInit(tex, (u16)w_pow2, (u16)h_pow2, format);
  C3D_TexSetFilter(tex, GPU_NEAREST, GPU_NEAREST);

  u32 pixel_size = (size / width / height);

  memset(tex->data, 0, tex->size);

  for (u32 x = 0; x < width; x++) {
    for (u32 y = 0; y < height; y++) {
      u32 dst_pos = ((((y >> 3) * (w_pow2 >> 3) + (x >> 3)) << 6) +
                     ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) |
                      ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3))) *
                    pixel_size;
      u32 src_pos = (y * width + x) * pixel_size;

      memcpy(&((u8 *)tex->data)[dst_pos], &((u8 *)buf)[src_pos], pixel_size);
    }
  }

  C3D_TexFlush(tex);

  tex->border = C2D_Color32(0, 0, 0, 0);
  C3D_TexSetWrap(tex, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);
  linearFree(buf);
}

bool Npi::Texture::IMG_LoadImageFile(const char *path) {
  stbi_uc *image = NULL;
  int width = 0, height = 0;
  int nc;

  image = stbi_load(path, &width, &height, &nc, 4);

  if (!image) {
    // printf("Image not loadet\n");
    return false;
  }

  if (width > 1024 || height > 1024) {
    stbi_image_free(image);
    return false;
  }

  this->m_texture = new C3D_Tex[sizeof(C3D_Tex)];
  
  OLD_C3DTexToC2DImage(this->m_texture, &this->m_subtexture, image, (u32)(width * height * 4),
                       (u32)width, (u32)height, GPU_RGBA8);
  //C3DTexToC2DImage(&this->m_image, width, height, image);
  this->m_image.tex = this->m_texture;
  this->m_image.subtex = &this->m_subtexture;
  //this->m_texture = this->m_image.tex;
  this->m_height = height;
  this->m_width = width;
  this->m_path = path;
  stbi_image_free(image);
  return true;
}

bool IMG_LoadImageBuffer(C2D_Image *texture, C3D_Tex *tex,
                         Tex3DS_SubTexture *subtex, unsigned char *buff) {
  stbi_uc *image = NULL;
  int width = 0, height = 0;
  int nc;

  image = stbi_load_from_memory(buff, sizeof(buff), &width, &height, &nc, 4);

  if (width > 1024 || height > 1024) {
    stbi_image_free(image);
    return false;
  }

  tex = new C3D_Tex[sizeof(C3D_Tex)];
  subtex = new Tex3DS_SubTexture[sizeof(Tex3DS_SubTexture)];
  OLD_C3DTexToC2DImage(tex, subtex, image, (u32)(width * height * 4),
                       (u32)width, (u32)height, GPU_RGBA8);
  texture->tex = tex;
  texture->subtex = subtex;
  stbi_image_free(image);
  return true;
}

namespace Npi {
Texture::Texture()
    : m_width(0), m_height(0), m_path(""), m_texture(nullptr) { /* do nothing */
}

Texture::Texture(Npi::Texture &t_texture) { operator=(t_texture); }

Texture::~Texture() {
  if (m_texture != nullptr) {
    unloadImage(m_texture);
  }
}

bool Texture::loadFromFile(const std::string &t_filename) {
  return loadFile(t_filename.c_str());
}

bool Texture::loadFromBuffer(const void *t_buffer) {
  return loadBuffer((unsigned char *)t_buffer);
}

int Texture::getWidth() { return m_width; }

int Texture::getHeight() { return m_height; }

int Texture::getCalculatedWidth() { return getNextPow2(m_width); }

int Texture::getCalculatedHeight() { return getNextPow2(m_height); }

std::string Texture::getPath() { return m_path; }

C3D_Tex *Texture::getTexture() { return m_texture; }

Tex3DS_SubTexture &Texture::getSubtexture() { return m_subtexture; }

C2D_Image &Texture::getImage() { return m_image; }

Npi::Texture &Texture::operator=(Npi::Texture &rhs) {
  if (this == &rhs)
    return *this;

  m_width = rhs.getWidth();
  m_height = rhs.getHeight();
  m_path = rhs.getPath();

  if (m_texture)
    C3D_TexDelete(m_texture);

  if (rhs.getTexture() != nullptr) {
    m_texture = static_cast<C3D_Tex *>(malloc(sizeof(C3D_Tex)));
    C3D_TexInit(m_texture, getNextPow2(rhs.getTexture()->width),
                getNextPow2(rhs.getTexture()->height), GPU_RGBA8);
    memcpy(m_texture, rhs.getTexture(), sizeof(C3D_Tex));
  }

  return *this;
}

// private methods
bool Texture::loadFile(const char *path) {
  unloadImage(m_texture);
  bool res = this->IMG_LoadImageFile(path);
  // FILE *fp = fopen(path, "rb");
  // bool res = IMG_LoadImageBuffer(&this->m_image, this->m_texture,
  // &this->m_subtexture, (unsigned char*)fp);
  return res;
}

bool Texture::loadBuffer(unsigned char *buffer) {
  unloadImage(m_texture);
  bool res = IMG_LoadImageBuffer(&this->m_image, this->m_texture,
                                 &this->m_subtexture, buffer);
  return res;
}

inline void Texture::unloadImage(C3D_Tex *t_image) {
  if (t_image)
    C3D_TexDelete(t_image);
}

inline u32 Texture::getNextPow2(u32 v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v >= 64 ? v : 64;
}

bool Texture::loadFromPng(const std::string &t_filename) {
  unsigned char *ImageBuffer;
  int width, height;
  unloadImage(this->m_texture);
  ImageBuffer = stbi_load(t_filename.c_str(), &width, &height, NULL, 4);

  this->m_texture = new C3D_Tex;
  this->m_subtexture =
      Tex3DS_SubTexture({(u16)width, (u16)height, 0.0f, 1.0f, width / 1024.0f,
                         1.0f - (height / 1024.0f)});

  C3D_TexInit(this->m_texture, 1024, 1024, GPU_RGBA8);
  C3D_TexSetFilter(this->m_texture, GPU_LINEAR, GPU_LINEAR);
  this->m_texture->border = 0xFFFFFFFF;
  C3D_TexSetWrap(this->m_texture, GPU_CLAMP_TO_BORDER, GPU_CLAMP_TO_BORDER);

  for (u32 x = 0; x < width && x < 1024; x++) {
    for (u32 y = 0; y < height && y < 1024; y++) {
      const u32 dstPos = ((((y >> 3) * (1024 >> 3) + (x >> 3)) << 6) +
                          ((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) |
                           ((y & 2) << 2) | ((x & 4) << 2) | ((y & 4) << 3))) *
                         4;

      const u32 srcPos = (y * width + x) * 4;
      ((uint8_t *)this->m_texture->data)[dstPos + 0] = ImageBuffer[srcPos + 3];
      ((uint8_t *)this->m_texture->data)[dstPos + 1] = ImageBuffer[srcPos + 2];
      ((uint8_t *)this->m_texture->data)[dstPos + 2] = ImageBuffer[srcPos + 1];
      ((uint8_t *)this->m_texture->data)[dstPos + 3] = ImageBuffer[srcPos + 0];
    }
  }

  this->m_image.tex = this->m_texture;
  this->m_image.subtex = &this->m_subtexture;
  return true;
}
} // namespace Npi