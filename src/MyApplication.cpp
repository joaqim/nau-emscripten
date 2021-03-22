#include "MyApplication.h"

#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_EMSCRIPTEN
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/EmscriptenApplication.h>
#include <emscripten.h>
#include <emscripten/html5.h>

static const char *emscripten_result_to_string(EMSCRIPTEN_RESULT result) {
  if (result == EMSCRIPTEN_RESULT_SUCCESS) return "EMSCRIPTEN_RESULT_SUCCESS";
  if (result == EMSCRIPTEN_RESULT_DEFERRED) return "EMSCRIPTEN_RESULT_DEFERRED";
  if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
  if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
  if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) return "EMSCRIPTEN_RESULT_INVALID_TARGET";
  if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
  if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) return "EMSCRIPTEN_RESULT_INVALID_PARAM";
  if (result == EMSCRIPTEN_RESULT_FAILED) return "EMSCRIPTEN_RESULT_FAILED";
  if (result == EMSCRIPTEN_RESULT_NO_DATA) return "EMSCRIPTEN_RESULT_NO_DATA";
  return "Unknown EMSCRIPTEN_RESULT!";
}

#define TEST_RESULT(x) if (ret != EMSCRIPTEN_RESULT_SUCCESS) printf("%s returned %s.\n", #x, emscripten_result_to_string(ret));
#endif

#include "Cube.cpp"



namespace Nau {

MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments, Configuration{}.setTitle("Aim Nau").setWindowFlags(Configuration::WindowFlag::Resizable)} {
  using namespace Math::Literals;
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  //GL::Renderer::setFrontFace(GL::Renderer::FrontFace::ClockWise);
    _pCameraObject = new Object3D{&_scene};
    _pCameraObject->translate(Vector3f::zAxis(7.5f));

     /* Tilt slighty downward */
    _pCameraObject->rotateX(-0.51_radf);


    _pCamera = new SceneGraph::Camera3D(*_pCameraObject);

    _viewMatrix = (Matrix4::lookAt({0.f, 0.f, 5.f}, {0.f, 0.f, 0.f}, Vector3f::zAxis(1.f)));

    _pCamera->setProjectionMatrix(
        Matrix4::perspectiveProjection(
          60.0_degf,
          Vector2{GL::defaultFramebuffer.viewport().size()}.aspectRatio(),
          0.001f,
          100.0f))
      .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
      .setViewport(GL::defaultFramebuffer.viewport().size());

       _pManipulator = new Object3D{&_scene};

    (*_pManipulator)
      .scale(Vector3f{2.0f, 2.0f,2.0f});
#if 0
    (*_pManipulator)
      .translate(Vector3f::yAxis(-0.3f))
      .rotateX(Math::Rad<float>(30.f));
    //.rotateX(30.0_degf);
#endif


    auto mesh = getCube();
    new MeshDrawable(*_pManipulator, &_drawables);

#if 0    /* Load a scene importer plugin */
    PluginManager::Manager<Trade::AbstractImporter> manager;
    Containers::Pointer<Trade::AbstractImporter> importer =
      manager.loadAndInstantiate("AnyImageImporter");
    if(!importer || !importer->openFile("../data/shape_in_lights.png"))
      Fatal{} << "Can't open image.png with AnyImageImporter";

    Containers::Optional<Trade::ImageData2D> image = importer->image2D(0);
    if(!image) Fatal{} << "Importing the image failed";
#endif
    Debug{} << "Init!";
    Debug{} << "PointerLockActive" << isPointerLockActive();
    Debug{} << "FullscreenActive" << isFullscreenActive();
}

void MyApplication::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

  using namespace Magnum::Math::Literals;

  /*
     _transformation =
     Matrix4::rotationX(30.0_degf)*Matrix4::rotationY(40.0_degf);
     _projection =
     Matrix4::perspectiveProjection(
     35.0_degf, Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f)*
     Matrix4::translation(Vector3::zAxis(-10.0f));
     _color = Color3::fromHsv({35.0_degf, 1.0f, 1.0f});
     */


  _pCamera->draw(_drawables);
  swapBuffers();
}

void MyApplication::viewportEvent(ViewportEvent &event) {
  GL::defaultFramebuffer.setViewport({{}, event.windowSize()});
  _pCamera->setViewport(event.windowSize());
}

void MyApplication::mouseScrollEvent(MouseScrollEvent& event) {
  if(!event.offset().y()) return;

  if(!isFullscreenActive()){event.setAccepted();return;}
  /* Distance to origin */
  Float distance = _pCameraObject->transformation().translation().z();

  /* Move 15% of the distance back or forward */
  distance *= 1 - (event.offset().y() > 0 ? 1/0.85f : 0.85f);
  _pCameraObject->translate(Vector3f::zAxis(distance));


  redraw();
}

void MyApplication::keyPressEvent(KeyEvent &event) {
#ifndef CORRADE_TARGET_EMSCRIPTEN
  if(event.key() == KeyEvent::Key::Q || event.key() == KeyEvent::Key::Esc) {
    exit();
  }
#else
  if(event.key() == KeyEvent::Key::Q || event.key() == KeyEvent::Key::Esc) {
    exitPointerLock();
    exitFullscreen();
  } else if(event.key() == KeyEvent::Key::F) {
    //emscripten_request_fullscreen("#canvas", 1);
    //requestPointerLock();
    if(!isFullscreenActive()) {
      requestFullscreen();
    } else {
      exitFullscreen();
    }
  }
#endif
  event.setAccepted();
}

void MyApplication::pointerLockEvent(PointerLockEvent& event) {
  Debug{} << "PointerLockChange:" << event.isActive();
  event.setAccepted();
}

void MyApplication::fullscreenEvent(FullscreenEvent& event) {
  Debug{} << "FullscreenEvent:" << event.isFullscreen();
  if(event.isFullscreen()) {
    requestFullscreen();
    requestPointerLock();
  } else {
    exitFullscreen();
    exitPointerLock();
  }
  event.setAccepted();
}

void MyApplication::mousePressEvent(MouseEvent& event) {
  if(event.button() == MouseEvent::Button::Left) {
    //requestPointerLock();
    //emscripten_request_pointerlock("#canvas", 1);
  }
  event.setAccepted();
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {
  //if(event.button() == MouseEvent::Button::Left)
  //_previousPosition = Vector3f{0};
  event.setAccepted();
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {
  if(!isFullscreenActive()){event.setAccepted();return;}
  //Debug{} << "MouseMoveEvent";
  //Debug{} << event.relativePosition();
  //Debug{} << event.position();
  //if(!(event.buttons() & MouseMoveEvent::Button::Left)) return;
  //if (_previousPosition.isZero()) return;

#if 1
  const Vector2 delta = 1.0f*Vector2{event.position() - _prevMousePosition}/Vector2{framebufferSize()};
  //const auto delta = Vector2{event.relativePosition()};
  _prevMousePosition = event.position();

  auto _rotationPoint = Vector3f{0.0f,0.0f,0.0f};

  using namespace Math::Literals;
#if 0
  _pCameraObject->transformLocal(
      Matrix4::translation(_rotationPoint)*
      Matrix4::rotationX(-0.51_radf*delta.y())*
      Matrix4::rotationY(-0.51_radf*delta.x())*
      Matrix4::translation(-_rotationPoint));
#else
#endif
#endif

#if 0
  const Vector3f currentPosition = positionOnSphere(event.position());
  const Vector3f axis = Math::cross(_previousPosition, currentPosition);

  if(_previousPosition.length() < 0.001f || axis.length() < 0.001f) return;

  _pManipulator->rotate(Math::angle(_previousPosition, currentPosition), axis.normalized());
  _previousPosition = currentPosition;
#elif 0

  const Vector2 positionNormalized = Vector2{event.position()*2.f}/Vector2{_pCamera->viewport()} - Vector2{1.f};
  //const Vector2 positionNormalized = Vector2{position}/Vector2{_pCamera->viewport()} - Vector2{0.5f};
  const Float length = positionNormalized.length();
  Vector3f const result(length > 1.0f
      ? Vector3f(positionNormalized, 0.0f)
      : Vector3f(positionNormalized, 1.0f - length));
  //result.y() *= -1.0f;
  auto currentPosition=Vector3f{result*Vector3f::yScale(-1.0f)}.normalized();

  const Vector3f axis = Math::cross(_prevMousePosition, currentPosition);

  if(_prevMousePosition.length() < 0.001f || axis.length() < 0.001f) return;

  _pManipulator->rotate(Math::angle(_prevMousePosition, currentPosition), axis.normalized());
  _prevMousePosition = currentPosition;

  Debug{} << Math::angle(_prevMousePosition, currentPosition);

#endif

  event.setAccepted();
  redraw();
}
}
#ifndef CORRADE_TARGET_EMSCRIPTEN
MAGNUM_APPLICATION_MAIN(Nau::MyApplication)
#else
MAGNUM_EMSCRIPTENAPPLICATION_MAIN(Nau::MyApplication)
#endif
