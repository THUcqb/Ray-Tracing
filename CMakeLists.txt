cmake_minimum_required(VERSION 3.7)

project(RayTracing)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp raytracer.cpp raytracer.h components.cpp components.h scene.cpp scene.h brdf/BRDF.h brdf/CookTorranceBRDF.cpp brdf/CookTorranceBRDF.h)

add_executable(RayTracing ${SOURCE_FILES})

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(RayTracing ${OpenCV_LIBS})
