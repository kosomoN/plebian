#include "renderer/window.h"

int main(void) {
    Window window;
    window.Init(1280, 720);
    while (!window.ShouldClose()) {
        window.UpdateInput();
        window.SwapBuffers();
    }
}
