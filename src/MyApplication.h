#pragma once

#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_EMSCRIPTEN
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/EmscriptenApplication.h>
#endif

#include <Corrade/Utility/Format.h>

// Resources
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/AbstractImporter.h>

#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/Array.h>

// Scene and Camera
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Camera.h>

#include "Types.h"

namespace Nau {
  using namespace Magnum;
  using Corrade::Utility::Debug;

  typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
  typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;


  class MyApplication: public Platform::Application {

    public:
      explicit MyApplication(const Arguments& arguments);

    private:
      void drawEvent() override;
    private:
      void viewportEvent(ViewportEvent &event) override;
      void keyPressEvent(KeyEvent &event) override;

#ifdef CORRADE_TARGET_EMSCRIPTEN
      void pointerLockEvent(PointerLockEvent &event) override;
      void fullscreenEvent(FullscreenEvent &event) override;
#endif

      void mouseScrollEvent(MouseScrollEvent& event) override;
      void mousePressEvent(MouseEvent& event) override;
      void mouseReleaseEvent(MouseEvent& event) override;
      void mouseMoveEvent(MouseMoveEvent& event) override;
    private:
      Containers::Array<Containers::Optional<GL::Texture2D>> _textures;
      GL::Texture2D _texture;
    private:
      Scene3D _scene;
      SceneGraph::DrawableGroup3D _drawables;
      SceneGraph::Camera3D *_pCamera;
      Object3D *_pCameraObject;
      Object3D *_pManipulator;
      Matrix4 _viewMatrix;


      Vector3f _mainCameraVelocity;
      Vector2i _prevMousePosition;
  };
}
