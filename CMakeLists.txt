cmake_minimum_required(VERSION 3.16.3) # The lowest version I have tested on

project(SymmetricalGarbanzo VERSION 0.0
                            DESCRIPTION "a game"
                            LANGUAGES C CXX)

find_package(glfw3 REQUIRED)

set(GLAD_DIR "lib/glad")
add_library(glad ${GLAD_DIR}/include/glad/glad.h ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
                            
add_executable(garbanzo src/main.cpp src/Game.cpp)
target_include_directories(garbanzo PUBLIC include ${GLAD_DIR}/include)
target_compile_features(garbanzo PUBLIC cxx_std_17)
target_link_libraries(garbanzo glfw glad ${CMAKE_DL_LIBS})