#include "screen_manager.h"

#include <assert.h>

void ScreenManager::Update(double delta) {
    screens.top()->Update(delta);
}

void ScreenManager::PushScreen(IScreen* screen) {
    assert(screen != nullptr);

    if (!screens.empty())
        screens.top()->Hide();

    screens.push(screen);
    screen->Enter();
    screen->Show();
}

void ScreenManager::PopScreen() {
    assert(screens.size() > 1);

    IScreen* screen = screens.top();
    screen->Hide();
    screen->Exit();
    delete screen;

    screens.pop();
    screens.top()->Show();
}
