cmake_minimum_required(VERSION 2.8)
project(myproject CXX)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
   SETTINGS compiler.cppstd=17
   SETTINGS compiler.libcxx=libstdc++
   REQUIRES
   cpp-httplib/0.6.6
   nlohmann_json/3.8.0
   libsigcpp/3.0.0
   cli11/1.6.1@bincrafters/stable
   BASIC_SETUP
   BUILD missing
)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CONAN_INCLUDE_DIRS})

add_subdirectory(src)
