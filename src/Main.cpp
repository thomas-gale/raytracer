#include <Corrade/Containers/GrowableArray.h>
#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/BufferImage.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
//#include <Magnum/Image.h>
#include <Magnum/ImageView.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Plane.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>

#include <SDL2/SDL.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

class Raytrace : public Platform::Application {
public:
  explicit Raytrace(const Arguments &arguments);

private:
  void drawEvent() override;

  GL::Mesh mesh_;
  Shaders::Flat2D shader_{Shaders::Flat3D::Flag::Textured};
  GL::Texture2D texture_;
};

Raytrace::Raytrace(const Arguments &arguments)
    : Platform::Application{arguments, Configuration{}.setTitle("Raytrace")} {

  struct FlatVertex {
    Vector2 position;
    Vector2 textureCoordinates;
  };

  // Simple Quad.
  const FlatVertex data[]{
      {{-0.9f, -0.9f}, {0.0f, 0.0f}}, /* Bottom Left position and texture coordinate */
      {{0.9f, -0.9f}, {1.0f, 0.0f}}, /* Bottom Right position and texture coordinate */
      {{0.9f, 0.9f}, {1.0f, 1.0f}},  /* Top Right position and texture coordinate */
      {{-0.9f, 0.9f}, {1.0f, 0.0f}},   /* Top Left position and texture coordinate */
      {{-0.9f, -0.9f}, {0.0f, 0.0f}}, /* Bottom Left position and texture coordinate */
      {{0.9f, 0.9f}, {1.0f, 1.0f}},  /* Top Right position and texture coordinate */
      {{-0.9f, 0.9f}, {1.0f, 0.0f}}   /* Top Left position and texture coordinate */
  };

  GL::Buffer buffer;
  buffer.setData(data);
  mesh_.setCount(6).addVertexBuffer(std::move(buffer), 0,
                                    Shaders::Flat2D::Position{},
                                    Shaders::Flat2D::TextureCoordinates{});

  const int imSize = 128;
  Containers::Array<char> imdata;
  Containers::arrayResize(imdata, imSize * imSize * 3);
  for (int i = 0; i < imSize * imSize * 3; i+=3) {
    imdata[i] = 142;
    imdata[i+1] = 0;
    imdata[i+2] = 0;
  }

  auto image = Containers::Pointer<Trade::ImageData2D>(new Trade::ImageData2D{
      PixelFormat::RGB8Unorm, {imSize, imSize}, std::move(imdata)});

  texture_.setWrapping(GL::SamplerWrapping::ClampToEdge)
      .setMagnificationFilter(GL::SamplerFilter::Linear)
      .setMinificationFilter(GL::SamplerFilter::Linear)
      .setStorage(1, GL::textureFormat(image->format()), image->size())
      .setSubImage(0, {}, *image);

  /* Loop frame as fast as possible */
  setSwapInterval(0);
}

void Raytrace::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
  shader_.bindTexture(texture_).draw(mesh_);
  swapBuffers();
}

MAGNUM_APPLICATION_MAIN(Raytrace)
