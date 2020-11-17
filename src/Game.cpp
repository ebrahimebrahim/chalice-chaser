#include <Game.h>
#include <chrono>
#include <thread>

int Game::run() {
    
    init();
    std::chrono::nanoseconds update_lag{0};
    while (true) {
        auto frame_start_time = std::chrono::steady_clock::now();

        handle_input();
        
        while (update_lag >= time_per_update) {
            update();
            update_lag -= time_per_update;
        }
        
        
        render();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - frame_start_time);
        if (elapsed_time < min_time_per_frame){
            std::this_thread::sleep_for(min_time_per_frame - elapsed_time);
        }
        elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - frame_start_time);
        update_lag += elapsed_time;
        fps = double(1e9)/elapsed_time.count();
    }

    return 0;
}

void Game::init() {

}

void Game::handle_input() {

}

void Game::update() {
}

void Game::render() {

}