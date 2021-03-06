find_package(Corrade REQUIRED Utility)
find_package(Magnum REQUIRED
  GL
  SceneGraph
  Trade
  Shaders
  MeshTools
  Primitives
  )

if(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED EmscriptenApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(MyApplication MyApplication.cpp)
target_link_libraries(MyApplication PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::SceneGraph
    Magnum::Trade
    Magnum::Shaders
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::Magnum
    Corrade::Utility
    )

# For Emscripten copy the boilerplate next to the executable so it can be run
# directly from the build dir; provide an install target as well
if(CORRADE_TARGET_EMSCRIPTEN)
    add_custom_command(TARGET MyApplication POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MAGNUM_EMSCRIPTENAPPLICATION_JS}
            ${MAGNUM_WEBAPPLICATION_CSS}
            ${CMAKE_CURRENT_SOURCE_DIR}/MyApplication.html
            ${CMAKE_CURRENT_SOURCE_DIR}/index.html
            $<TARGET_FILE_DIR:MyApplication>)

    target_compile_options(MyApplication PRIVATE "--emrun")

    install(TARGETS MyApplication DESTINATION ${MAGNUM_DEPLOY_PREFIX})
    install(FILES
        MyApplication.html
        index.html
        ${MAGNUM_EMSCRIPTENAPPLICATION_JS}
        ${MAGNUM_WEBAPPLICATION_CSS}
        DESTINATION ${MAGNUM_DEPLOY_PREFIX})
    install(FILES
        $<TARGET_FILE_DIR:MyApplication>/MyApplication.js.mem
        $<TARGET_FILE_DIR:MyApplication>/MyApplication.wasm
        DESTINATION ${MAGNUM_DEPLOY_PREFIX} OPTIONAL)
    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../favicon.ico
        DESTINATION ${MAGNUM_DEPLOY_PREFIX} OPTIONAL)
    install(FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/app.js
        DESTINATION ${MAGNUM_DEPLOY_PREFIX} OPTIONAL)

endif()

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyApplication)
