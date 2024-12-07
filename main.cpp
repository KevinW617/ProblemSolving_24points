#include "game24.h"

int main(int argc, char *argv[]) {
    Game24Window *window = new Game24Window(400, 300, "24点游戏");
    window->show(argc, argv);
    return Fl::run();
}