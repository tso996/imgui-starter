cmake_minimum_required(VERSION 3.23.1)


project(gui_app)

# this specifies the compiler version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_directories(lib)#this is to find the compiled static or dynamic library files

add_executable(${PROJECT_NAME} main.cpp)

include_directories(include imgui)#this includes the subdirectories in it 

add_subdirectory(imgui)#this is to find the cmakelists file in the subdirectory and build library

target_link_libraries(${PROJECT_NAME} imgui_lib "-framework OpenGL" libGLEW.2.2.0.dylib libglfw.3.3.dylib)

#this does not work sometimes
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

