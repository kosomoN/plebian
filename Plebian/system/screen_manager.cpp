#include "screenmanager.h"

void ScreenManager::Update(double delta) {
    screens.top()->Update(delta);
}

void ScreenManager::PushScreen(IScreen* screen) {
    ASSERT(screen != nullptr);

    if (!screens.empty())
        screens.top()->Hide();

    screens.push(screen);
    screen->Enter();
    screen->Show();
}

void ScreenManager::PopScreen() {
    ASSERT(screens.size() > 1);

    IScreen* screen = screens.top();
    screen->Hide();
    screen->Exit();
    delete screen;

    screens.pop();
    screens.top()->Show();
}
