cmake_minimum_required(VERSION 3.13) # The lowest version I have tested on

project(ChaliceChaser VERSION 0.0
                            DESCRIPTION "a game"
                            LANGUAGES C CXX)

find_package(glfw3 3.3 REQUIRED)

set(GLAD_DIR "lib/glad")
add_library(glad ${GLAD_DIR}/include/glad/glad.h ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
                            
add_executable(chalice_chaser src/main.cpp src/Game.cpp src/GameWindow.cpp src/Shader.cpp src/Entity.cpp src/GraphicalEntity.cpp src/Prize.cpp src/Player.cpp src/Camera.cpp src/level_gen.cpp src/Wall.cpp src/Floor.cpp src/CollisionBox.cpp src/Hud.cpp src/Portal.cpp src/StbImage.cpp)
target_include_directories(chalice_chaser PUBLIC include lib ${GLAD_DIR}/include)
target_compile_features(chalice_chaser PUBLIC cxx_std_17)
target_link_libraries(chalice_chaser glfw glad ${CMAKE_DL_LIBS})