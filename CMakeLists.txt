cmake_minimum_required(VERSION 3.16.3) # The lowest version I have tested on

project(SymmetricalGarbanzo VERSION 0.0
                            DESCRIPTION "A game"
                            LANGUAGES CXX)

add_executable(garbanzo src/main.cpp)
target_include_directories(garbanzo PUBLIC include/)