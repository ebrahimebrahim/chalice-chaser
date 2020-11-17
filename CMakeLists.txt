cmake_minimum_required(VERSION 3.16.3) # The lowest version I have tested on

project(SymmetricalGarbanzo VERSION 0.0
                            DESCRIPTION "a game"
                            LANGUAGES CXX)

add_executable(garbanzo src/main.cpp src/Game.cpp lib/glad/src/glad.c)
target_include_directories(garbanzo PUBLIC include/ lib/glad/include/)
target_compile_features(garbanzo PUBLIC cxx_std_17)