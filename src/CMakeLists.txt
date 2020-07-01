add_executable(hue_handler main.cpp hue_controller.cpp)
target_link_libraries(hue_handler ${CONAN_LIBS})