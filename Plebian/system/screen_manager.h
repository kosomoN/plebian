#ifndef SCREEN_MANAGER_H_
#define SCREEN_MANAGER_H

class ScreenManager {
public:
    void Update(double delta);
    void PushScreen(IScreen* screen);
    void PopScreen();
private:
    std::stack<IScreen*> screens;
};

#endif // SCREEN_MANAGER_H
