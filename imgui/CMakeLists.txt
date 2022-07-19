add_library(imgui_lib imgui.cpp imgui_widgets.cpp imgui_tables.cpp imgui_impl_opengl3.cpp imgui_impl_glfw.cpp imgui_draw.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
