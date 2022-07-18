cmake_minimum_required(VERSION 3.23.1)

project(gui_app)

# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(include)

# target_link_libraries(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
