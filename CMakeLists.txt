cmake_minimum_required(VERSION 3.23.1)

project(gui_app)

# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

link_directories(lib)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(include)

target_link_libraries(${PROJECT_NAME} "-framework OpenGL" libGLEW.2.2.0.dylib libglfw.3.3.dylib)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
