#include <Corrade/Containers/GrowableArray.h>
#include <Magnum/GL/Buffer.h>
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

struct Vertex {
  Vector2 position;
  Vector2 textureCoordinates;
  Vertex(Vector2 pos, Vector2 tex) : position(pos), textureCoordinates(tex){};
};

class Raytrace : public Platform::Application {
public:
  explicit Raytrace(const Arguments &arguments);

private:
  void drawEvent() override;

  // Trade::ImageData2D image_{NoCreate};
  // Vector2i imageSize_;
  // GL::Texture2D texture_;

  Vertex data_[4]{Vertex(Vector2(-0.9, -0.9), Vector2(0.0, 0.0)),
                  Vertex(Vector2(0.9, -0.9), Vector2(1.0, 0.0)),
                  Vertex(Vector2(-0.9, 0.9), Vector2(0.0, 1.0)),
                  Vertex(Vector2(0.9, 0.9), Vector2(1.0, 1.0))};

  GL::Buffer vertices_;
  GL::Mesh mesh_{NoCreate};

  Matrix4 transformationMatrix_, projectionMatrix_;

  Corrade::Containers::Pointer<Magnum::Trade::ImageData2D> image_;
  GL::Texture2D texture_;

  Shaders::Flat2D shader_{Shaders::Flat3D::Flag::Textured};
};

Raytrace::Raytrace(const Arguments &arguments)
    : Platform::Application{arguments, Configuration{}.setTitle("Raytrace")} {

  vertices_.setData(data_, GL::BufferUsage::StaticDraw);

  mesh_ = MeshTools::compile(
      Primitives::planeSolid(Primitives::PlaneFlag::TextureCoordinates));

  mesh_.setCount(4).addVertexBuffer(vertices_, 0, Shaders::Flat2D::Position{},
                                    Shaders::Flat2D::TextureCoordinates{});

  Containers::Array<char> data;
  Containers::arrayResize(data, 32 * 32 * 3);
  for (int i = 0; i < 32 * 32 * 3; ++i) {
    data[i] = 250;
  }

  image_ = Containers::Pointer<Trade::ImageData2D>(new Trade::ImageData2D{
      PixelFormat::RGB8Unorm, {32, 32}, std::move(data)});

  texture_.setSubImage(0, {}, *image_);
}

void Raytrace::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  shader_
      //.setColor(Color4(100.0, 100.0, 100.0, 0.0))
      //.setTransformationProjectionMatrix(projectionMatrix_ *
      //                                   transformationMatrix_)
      .bindTexture(texture_)
      .draw(mesh_);

  swapBuffers();
}

MAGNUM_APPLICATION_MAIN(Raytrace)
