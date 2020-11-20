cmake_minimum_required(VERSION 3.13) # The lowest version I have tested on

project(SymmetricalGarbanzo VERSION 0.0
                            DESCRIPTION "a game"
                            LANGUAGES C CXX)

find_package(glfw3 REQUIRED)

set(GLAD_DIR "lib/glad")
add_library(glad ${GLAD_DIR}/include/glad/glad.h ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
                            
add_executable(garbanzo src/main.cpp src/Game.cpp src/GameWindow.cpp src/Shader.cpp src/Entity.cpp src/Prize.cpp src/Player.cpp src/Camera.cpp)
target_include_directories(garbanzo PUBLIC include lib ${GLAD_DIR}/include)
target_compile_features(garbanzo PUBLIC cxx_std_17)
target_link_libraries(garbanzo glfw glad ${CMAKE_DL_LIBS})

# The following target should be deleted eventually.
# It is to help experiment with rendering so I can understand the pipeline better
# before setting up my graphics classes architecture
add_executable(experiment src/experiment.cpp src/Shader.cpp)
target_include_directories(experiment PUBLIC include lib ${GLAD_DIR}/include)
target_compile_features(experiment PUBLIC cxx_std_17)
target_link_libraries(experiment glfw glad ${CMAKE_DL_LIBS})