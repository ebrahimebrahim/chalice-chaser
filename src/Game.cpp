#include <Game.h>

int Game::run() {
    
    init();
    while (true) {
        handle_input();
        update();
        render();
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