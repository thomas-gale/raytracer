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

class FlatSurfaceShader : public GL::AbstractShaderProgram {
public:
  typedef GL::Attribute<0, Vector2> Position;
  typedef GL::Attribute<1, Vector2> TextureCoordinates;

  explicit FlatSurfaceShader() {};

  FlatSurfaceShader &setColor(const Color3 &color) {
    setUniform(_colorUniform, color);
    return *this;
  }

  FlatSurfaceShader &bindTexture(GL::Texture2D &texture) {
    texture.bind(TextureUnit);
    return *this;
  }

private:
  enum : Int { TextureUnit = 0 };

  Int _colorUniform;
};

class Raytrace : public Platform::Application {
public:
  explicit Raytrace(const Arguments &arguments);

private:
  void drawEvent() override;

  GL::Mesh mesh_;
  //FlatSurfaceShader shader_;
  Shaders::Flat2D shader_{Shaders::Flat3D::Flag::Textured};
  GL::Texture2D texture_;

  // Trade::ImageData2D image_{NoCreate};
  // Vector2i imageSize_;
  // GL::Texture2D texture_;

  // Vertex data_[4]{Vertex(Vector2(-0.9, -0.9), Vector2(0.0, 0.0)),
  //                Vertex(Vector2(0.9, -0.9), Vector2(1.0, 0.0)),
  //                Vertex(Vector2(-0.9, 0.9), Vector2(0.0, 1.0)),
  //                Vertex(Vector2(0.9, 0.9), Vector2(1.0, 1.0))};

  // GL::Buffer vertices_;
  // GL::Mesh mesh_{NoCreate};

  // Matrix4 transformationMatrix_, projectionMatrix_;

  // Corrade::Containers::Pointer<Magnum::Trade::ImageData2D> image_;
  // GL::Texture2D texture_;

  // Shaders::Flat2D shader_{Shaders::Flat3D::Flag::Textured};
  // Shaders::Flat2D shader_;

  // float dym_ = -1.0;
};

Raytrace::Raytrace(const Arguments &arguments)
    : Platform::Application{arguments, Configuration{}.setTitle("Raytrace")} {

  struct FlatVertex {
    Vector2 position;
    Vector2 textureCoordinates;
    //FlatVertex(Vector2 pos, Vector2 tex) : position(pos), textureCoordinates(tex){};
  };

  const FlatVertex data[]{
      {{-0.5f, -0.5f}, {0.0f, 0.0f}}, /* Left position and texture coordinate */
      {{0.5f, -0.5f}, {1.0f, 0.0f}}, /* Right position and texture coordinate */
      {{0.5f, 0.5f}, {0.5f, 1.0f}}   /* Top position and texture coordinate */
  };

  GL::Buffer buffer;
  buffer.setData(data);
  mesh_.setCount(3).addVertexBuffer(std::move(buffer), 0,
                                    Shaders::Flat2D::Position{},
                                    Shaders::Flat2D::TextureCoordinates{});

  /*vertices_.setData(data_, GL::BufferUsage::StaticDraw);

  mesh_ = MeshTools::compile(
      Primitives::planeSolid(Primitives::PlaneFlag::TextureCoordinates));

  mesh_.addVertexBuffer(vertices_, 0, Shaders::Flat2D::Position{},
                        Shaders::Flat2D::TextureCoordinates{});*/

  const int imSize = 128;
  Containers::Array<char> imdata;
  Containers::arrayResize(imdata, imSize * imSize * 3);
  for (int i = 0; i < imSize * imSize * 3; ++i) {
    imdata[i] = 255;
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
