#include <Corrade/Containers/GrowableArray.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
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
#include <Magnum/Primitives/Square.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/MeshData.h>

#include <SDL2/SDL.h>

using namespace Magnum;
using namespace Magnum::Math::Literals;

struct Vertex {
  Vector3 position;
  Vector2 textureCoordinates;
  Vertex(Vector3 pos, Vector2 tex) : position(pos), textureCoordinates(tex){};
};

class Raytrace : public Platform::Application {
public:
  explicit Raytrace(const Arguments &arguments);

private:
  void drawEvent() override;

  // Trade::ImageData2D image_{NoCreate};
  // Vector2i imageSize_;
  // GL::Texture2D texture_;

  Vertex data_[4]{Vertex(Vector3(0.0, 0.0, 0.0), Vector2(0.0, 0.0)),
                  Vertex(Vector3(1.0, 0.0, 0.0), Vector2(1.0, 0.0)),
                  Vertex(Vector3(1.0, 1.0, 0.0), Vector2(1.0, 1.0)),
                  Vertex(Vector3(0.0, 1.0, 0.0), Vector2(0.0, 1.0))};

  GL::Buffer vertices_;

  GL::Mesh mesh_{NoCreate};
  // Shaders::Flat2D shader_{Shaders::Flat2D::Flag::Textured |
  //                        Shaders::Flat2D::Flag::TextureTransformation};
};

Raytrace::Raytrace(const Arguments &arguments)
    : Platform::Application{arguments, Configuration{}.setTitle("Raytrace")} {

  vertices_.setData(data_, GL::BufferUsage::StaticDraw);

  mesh_ = MeshTools::compile(
      Primitives::squareSolid(Primitives::SquareFlag::TextureCoordinates));

  mesh_.addVertexBuffer(vertices_, 0, Shaders::Flat2D::Position{}, Shaders::Flat2D::TextureCoordinates{});
}

void Raytrace::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  Matrix4 transformationMatrix, projectionMatrix;

  Containers::Array<char> data;
  Containers::arrayResize(data, 32 * 32 * 3);
  for (int i = 0; i < 32 * 32 * 3; ++i) {
    data[i] = 64;
  }

  auto image = Containers::Pointer<Trade::ImageData2D>(new Trade::ImageData2D{
      PixelFormat::RGB8Unorm, {32, 32}, std::move(data)});

  GL::Texture2D texture;
  texture.setSubImage(0, {}, *image);

  Shaders::Flat3D shader{Shaders::Flat3D::Flag::Textured};
  shader
      .setTransformationProjectionMatrix(projectionMatrix *
                                         transformationMatrix)
      .bindTexture(texture)
      .draw(mesh_);

  swapBuffers();
}

MAGNUM_APPLICATION_MAIN(Raytrace)
