#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>

#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Compile.h>

#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>

#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Camera.h>


namespace Nau {
  using namespace Magnum;
  typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
  typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

  GL::Mesh static getCube();
  GL::Mesh static getCube() {
#if 1
    auto mesh = MeshTools::compile(Primitives::cubeSolid());
#else
    Trade::MeshData cube = Primitives::cubeSolid();
    GL::Buffer vertices;
    vertices.setData(MeshTools::interleave(cube.positions3DAsArray(),
                                           cube.normalsAsArray()));
std::pair<Containers::Array<char>, MeshIndexType> compressed =
        MeshTools::compressIndices(cube.indicesAsArray());
    GL::Buffer indices;
    indices.setData(compressed.first);

 _mesh.setPrimitive(cube.primitive())
        .setCount(cube.indexCount())
        .addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{},
                                                 Shaders::Phong::Normal{})
        .setIndexBuffer(std::move(indices), 0, compressed.second);
    return mesh;
#endif
    return mesh;
  }

  class MeshDrawable : public SceneGraph::Drawable3D {
    public:
      explicit MeshDrawable(Object3D &object, SceneGraph::DrawableGroup3D *group);
      void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D &camera) override;
      GL::Mesh _mesh;
      Shaders::Phong _phongShader;
      //Shaders::Phong _textureShader{Shaders::Phong::Flag::DiffuseTexture};
    public:
      GL::Texture2D *texture;
    private:

#if defined (CORRADE_TARGET_EMSCRIPTEN)
      GL::Buffer _vertexBuffer{GL::Buffer::TargetHint::Array};
      GL::Buffer _indexBuffer{GL::Buffer::TargetHint::ElementArray};
      GL::Buffer _normalsBuffer{GL::Buffer::TargetHint::ElementArray};
      GL::Buffer _colorBuffer{GL::Buffer::TargetHint::ElementArray};
#else
      GL::Buffer _vertexBuffer;
      GL::Buffer _indexBuffer;
      GL::Buffer _normalsBuffer;
      GL::Buffer _colorBuffer;
#endif
  };

  MeshDrawable::MeshDrawable(Object3D &object, SceneGraph::DrawableGroup3D *group) : SceneGraph::Drawable3D{object, group} {
  using namespace Magnum::Math::Literals;
  _mesh = getCube();
  //_mesh = mesh;
#if 0
  _phongShader
    .setAmbientColor(0x111111_rgbf)
    .setSpecularColor(0xffffff_rgbf)
    .setShininess(80.0f);
  _textureShader
    .setAmbientColor(0x111111_rgbf)
    .setSpecularColor(0x111111_rgbf)
    .setShininess(80.0f);
#endif
  /*

  auto indices = cube.indices();
  auto normals = cube.normals(0);
  auto vertices = cube.positions(0);

  _vertexBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
  _normalsBuffer.setData(normals, GL::BufferUsage::StaticDraw);
  _indexBuffer.setData(indices, GL::BufferUsage::StaticDraw);

  _mesh.setIndexBuffer(_indexBuffer, 0, MeshIndexType::UnsignedInt, 0, _indexBuffer.size());

  auto verticesCount = indices.size();
  {
    Containers::Array<char> indexData;
    MeshIndexType indexType;
    UnsignedInt indexStart, indexEnd;
    if(indices.size() > 0) {
      std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(indices);
      std::cout << "Index size:  \t" << indices.size() << std::endl;
      std::cout << "Index Data size: "<< indexData.size() << std::endl;
      _indexBuffer.setData(indexData, GL::BufferUsage::StaticDraw);
      _mesh.setIndexBuffer(_indexBuffer, 0, indexType, indexStart, indexEnd);
    }
#if defined(CORRADE_TARGET_EMSCRIPTEN)
    // Use gl_PointSize builtin vertex shader variable in OpenGL ES and WebGL instead.
#else
    GL::Renderer::setPointSize(5.f);
#endif

    _mesh
      .setPrimitive( MeshPrimitive::Triangles )
      //.setPrimitive( MeshPrimitive::Lines )
      .addVertexBuffer(_normalsBuffer, 0,  Shaders::Phong::Normal{})
      .addVertexBuffer(_vertexBuffer, 0, Shaders::Phong::Position{});


    _mesh.setCount(verticesCount);
    */
  }

 void MeshDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D &camera){
   using namespace Magnum::Math::Literals;
#if 1
   _phongShader
     .setDiffuseColor(0xa5c9ea_rgbf)
     //.setLightPosition(camera.cameraMatrix().transformPoint({-17.0f, 17.0f, 17.0f}))
     .setTransformationMatrix(transformationMatrix)
     .setNormalMatrix(transformationMatrix.rotationScaling())
     .setProjectionMatrix(camera.projectionMatrix());

  //_mesh.draw(_phongShader);
  _phongShader.draw(_mesh);
#else
   _textureShader
     .bindDiffuseTexture(_texture)
     .setLightPosition(camera.cameraMatrix().transformPoint({-17.0f, 17.0f, 17.0f}))
     .setTransformationMatrix(transformationMatrix)
     .setNormalMatrix(transformationMatrix.rotationScaling())
     .setProjectionMatrix(camera.projectionMatrix());
#endif
 }

}
