cmake_minimum_required(VERSION 3.7)

project(RayTracing)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp engine/engine.cpp engine/engine.h scene/components.cpp scene/components.h scene/scene.cpp scene/scene.h brdf/BRDF.h brdf/CookTorranceBRDF.cpp brdf/CookTorranceBRDF.h)

add_executable(RayTracing ${SOURCE_FILES})

#find_path(OpenCV_INCLUDE_DIRS ...)
#if ((OpenCV_INCLUDE_DIRS) AND (EXISTS OpenCV_INCLUDE_DIRS))
    find_package( OpenCV REQUIRED )
    include_directories( ${OpenCV_INCLUDE_DIRS} )
    target_link_libraries(RayTracing ${OpenCV_LIBS})
#else()
#    execute_process(COMMAND git submodule update --init --recursive)
#    add_subdirectory(OpenCV)
#    INCLUDE(OpenCV)
#endif()
